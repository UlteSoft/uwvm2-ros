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
/** Complete ROS2 unitree_hg/msg/BmsCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_BmsCmd {
  /** Declared field cmd; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cmd;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t reserve[40];
} roswasmc_bounded_2_unitree_hg_msg_BmsCmd;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_BMSCMD_MAX_WIRE_BYTES 41u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_BMSCMD_MIN_WIRE_BYTES 41u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_BMSCMD_CODEC_IDENTITY "UWVMROS1_a267c92d22aa04a15f7d68c87b7638b15a368ac973f49a42021dc91f991b13de"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_BmsCmd_valid(roswasmc_bounded_2_unitree_hg_msg_BmsCmd const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<40u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_BmsCmd_wire_size(roswasmc_bounded_2_unitree_hg_msg_BmsCmd const* value) {size_t n=0;
  n+=1;
  {size_t i;for(i=0;i<40u;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_BmsCmd_write_fields(roswasmc_bounded_2_unitree_hg_msg_BmsCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->cmd,1);*offset+=1;
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_BmsCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_BmsCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cmd,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->reserve[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_unitree_hg_msg_BmsCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_BmsCmd_encode(roswasmc_bounded_2_unitree_hg_msg_BmsCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_BmsCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_BmsCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_BmsCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_BmsCmd_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_BmsCmd* value) {
  roswasmc_bounded_2_unitree_hg_msg_BmsCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<41u||length>41u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_BmsCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_hg/msg/BmsState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_BmsState {
  /** Declared field version_high; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t version_high;
  /** Declared field version_low; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t version_low;
  /** Declared field fn; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t fn;
  /** Declared field cell_vol; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t cell_vol[40];
  /** Declared field bmsvoltage; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t bmsvoltage[3];
  /** Declared field current; canonical capacity is part of CODEC_IDENTITY. */
  int32_t current;
  /** Declared field soc; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t soc;
  /** Declared field soh; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t soh;
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  int16_t temperature[12];
  /** Declared field cycle; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t cycle;
  /** Declared field manufacturer_date; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t manufacturer_date;
  /** Declared field bmsstate; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t bmsstate[5];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[3];
} roswasmc_bounded_2_unitree_hg_msg_BmsState;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_BMSSTATE_MAX_WIRE_BYTES 161u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_BMSSTATE_MIN_WIRE_BYTES 161u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_BMSSTATE_CODEC_IDENTITY "UWVMROS1_b7ac163c4ab4786d37a9ec027767258adcfc06637ed8b4ef67e30c517397761d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_BmsState_valid(roswasmc_bounded_2_unitree_hg_msg_BmsState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<40u;++i){
  }}
  {size_t i;for(i=0;i<3u;++i){
  }}
  {size_t i;for(i=0;i<12u;++i){
  }}
  {size_t i;for(i=0;i<5u;++i){
  }}
  {size_t i;for(i=0;i<3u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_BmsState_wire_size(roswasmc_bounded_2_unitree_hg_msg_BmsState const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  {size_t i;for(i=0;i<40u;++i){
  n+=2;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  n+=4;
  n+=1;
  n+=1;
  {size_t i;for(i=0;i<12u;++i){
  n+=2;
  }}
  n+=2;
  n+=2;
  {size_t i;for(i=0;i<5u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_BmsState_write_fields(roswasmc_bounded_2_unitree_hg_msg_BmsState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->version_high,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->version_low,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->fn,1);*offset+=1;
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->cell_vol[i],2);*offset+=2;
  }}
  {size_t i;for(i=0;i<3u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->bmsvoltage[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->current,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->soc,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->soh,1);*offset+=1;
  {size_t i;for(i=0;i<12u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature[i],2);*offset+=2;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->cycle,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->manufacturer_date,2);*offset+=2;
  {size_t i;for(i=0;i<5u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->bmsstate[i],4);*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_BmsState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_BmsState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->version_high,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->version_low,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->fn,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->cell_vol[i],&bits,2);}*offset+=2;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bmsvoltage[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->soc,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->soh,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<12u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->temperature[i],&bits,2);}*offset+=2;
  }}
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->cycle,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->manufacturer_date,&bits,2);}*offset+=2;
  {size_t i;for(i=0;i<5u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bmsstate[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_hg_msg_BmsState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_BmsState_encode(roswasmc_bounded_2_unitree_hg_msg_BmsState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_BmsState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_BmsState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_BmsState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_BmsState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_BmsState* value) {
  roswasmc_bounded_2_unitree_hg_msg_BmsState next={0};size_t offset=0;if(!value||(length&&!input)||length<161u||length>161u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_BmsState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_hg/msg/MotorCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_MotorCmd {
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
  uint32_t reserve;
} roswasmc_bounded_2_unitree_hg_msg_MotorCmd;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MOTORCMD_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MOTORCMD_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MOTORCMD_CODEC_IDENTITY "UWVMROS1_f3e472df5d061a1515ac185f87b01f702ef2f2bbeff39e42c6c7088d1da26cbb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_MotorCmd_valid(roswasmc_bounded_2_unitree_hg_msg_MotorCmd const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_MotorCmd_wire_size(roswasmc_bounded_2_unitree_hg_msg_MotorCmd const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_MotorCmd_write_fields(roswasmc_bounded_2_unitree_hg_msg_MotorCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_MotorCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_MotorCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kp,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kd,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_hg_msg_MotorCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_MotorCmd_encode(roswasmc_bounded_2_unitree_hg_msg_MotorCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_MotorCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_MotorCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_MotorCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_MotorCmd_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_MotorCmd* value) {
  roswasmc_bounded_2_unitree_hg_msg_MotorCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most35 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_unitree_hg_msg_MotorCmd data[35];} roswasmc_bounded_2_unitree_hg_msg_HandCmd_motor_cmd_sequence;
/** Complete ROS2 unitree_hg/msg/HandCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_HandCmd {
  /** Declared field motor_cmd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_hg_msg_HandCmd_motor_cmd_sequence motor_cmd;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[4];
} roswasmc_bounded_2_unitree_hg_msg_HandCmd;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_HANDCMD_MAX_WIRE_BYTES 895u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_HANDCMD_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_HANDCMD_CODEC_IDENTITY "UWVMROS1_2ebc0e32e831a48e21f0b15f39cb477d999bf3a415e7863713ce37416020a835"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_HandCmd_valid(roswasmc_bounded_2_unitree_hg_msg_HandCmd const* value) {
  if(!value)return 0;
  if(value->motor_cmd.size>35u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorCmd_valid(&value->motor_cmd.data[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_HandCmd_wire_size(roswasmc_bounded_2_unitree_hg_msg_HandCmd const* value) {size_t n=0;
  n+=4;
  if(value->motor_cmd.size>35u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  n+=roswasmc_bounded_2_unitree_hg_msg_MotorCmd_wire_size(&value->motor_cmd.data[i]);
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_HandCmd_write_fields(roswasmc_bounded_2_unitree_hg_msg_HandCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->motor_cmd.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  roswasmc_bounded_2_unitree_hg_msg_MotorCmd_write_fields(&value->motor_cmd.data[i],p,offset);
  }}
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_HandCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_HandCmd* value) {
  if(*offset>n||n-*offset<4)return 0;value->motor_cmd.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_cmd.size>35u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorCmd_read_fields(p,n,offset,&value->motor_cmd.data[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_hg_msg_HandCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_HandCmd_encode(roswasmc_bounded_2_unitree_hg_msg_HandCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_HandCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_HandCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_HandCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_HandCmd_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_HandCmd* value) {
  roswasmc_bounded_2_unitree_hg_msg_HandCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>895u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_HandCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_hg/msg/MotorState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_MotorState {
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
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  int16_t temperature[2];
  /** Declared field vol; canonical capacity is part of CODEC_IDENTITY. */
  float vol;
  /** Declared field sensor; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t sensor[2];
  /** Declared field motorstate; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t motorstate;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[4];
} roswasmc_bounded_2_unitree_hg_msg_MotorState;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MOTORSTATE_MAX_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MOTORSTATE_MIN_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MOTORSTATE_CODEC_IDENTITY "UWVMROS1_65497908fe0136a5c90f11b4a2cda2110507bcc8805f9347ac82be3b32fe5934"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_MotorState_valid(roswasmc_bounded_2_unitree_hg_msg_MotorState const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau_est,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vol,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<4u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_MotorState_wire_size(roswasmc_bounded_2_unitree_hg_msg_MotorState const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<2u;++i){
  n+=2;
  }}
  n+=4;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  n+=4;
  {size_t i;for(i=0;i<4u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_MotorState_write_fields(roswasmc_bounded_2_unitree_hg_msg_MotorState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau_est,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature[i],2);*offset+=2;
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vol,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->sensor[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->motorstate,4);*offset+=4;
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_MotorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_MotorState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ddq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau_est,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->temperature[i],&bits,2);}*offset+=2;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->vol,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sensor[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->motorstate,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_hg_msg_MotorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_MotorState_encode(roswasmc_bounded_2_unitree_hg_msg_MotorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_MotorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_MotorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_MotorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_MotorState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_MotorState* value) {
  roswasmc_bounded_2_unitree_hg_msg_MotorState next={0};size_t offset=0;if(!value||(length&&!input)||length<53u||length>53u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_hg/msg/PressSensorState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_PressSensorState {
  /** Declared field pressure; canonical capacity is part of CODEC_IDENTITY. */
  float pressure[12];
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  float temperature[12];
  /** Declared field lost; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t lost;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve;
} roswasmc_bounded_2_unitree_hg_msg_PressSensorState;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_PRESSSENSORSTATE_MAX_WIRE_BYTES 104u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_PRESSSENSORSTATE_MIN_WIRE_BYTES 104u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_PRESSSENSORSTATE_CODEC_IDENTITY "UWVMROS1_4abc296d4acd21132c0c1d185e3fafd9f0db8836a5c16ed040ebcc6127380fe5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_PressSensorState_valid(roswasmc_bounded_2_unitree_hg_msg_PressSensorState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<12u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pressure[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<12u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->temperature[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_PressSensorState_wire_size(roswasmc_bounded_2_unitree_hg_msg_PressSensorState const* value) {size_t n=0;
  {size_t i;for(i=0;i<12u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<12u;++i){
  n+=4;
  }}
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_PressSensorState_write_fields(roswasmc_bounded_2_unitree_hg_msg_PressSensorState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<12u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pressure[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<12u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->temperature[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->lost,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_PressSensorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_PressSensorState* value) {
  {size_t i;for(i=0;i<12u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->pressure[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<12u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->temperature[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->lost,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_hg_msg_PressSensorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_PressSensorState_encode(roswasmc_bounded_2_unitree_hg_msg_PressSensorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_PressSensorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_PressSensorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_PressSensorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_PressSensorState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_PressSensorState* value) {
  roswasmc_bounded_2_unitree_hg_msg_PressSensorState next={0};size_t offset=0;if(!value||(length&&!input)||length<104u||length>104u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_PressSensorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_hg/msg/IMUState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_IMUState {
  /** Declared field quaternion; canonical capacity is part of CODEC_IDENTITY. */
  float quaternion[4];
  /** Declared field gyroscope; canonical capacity is part of CODEC_IDENTITY. */
  float gyroscope[3];
  /** Declared field accelerometer; canonical capacity is part of CODEC_IDENTITY. */
  float accelerometer[3];
  /** Declared field rpy; canonical capacity is part of CODEC_IDENTITY. */
  float rpy[3];
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  int16_t temperature;
} roswasmc_bounded_2_unitree_hg_msg_IMUState;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_IMUSTATE_MAX_WIRE_BYTES 54u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_IMUSTATE_MIN_WIRE_BYTES 54u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_IMUSTATE_CODEC_IDENTITY "UWVMROS1_d83832e8c27273785113c8388f7f22ea1251d6198817255f13a7e542afb26420"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_IMUState_valid(roswasmc_bounded_2_unitree_hg_msg_IMUState const* value) {
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
static inline size_t roswasmc_bounded_2_unitree_hg_msg_IMUState_wire_size(roswasmc_bounded_2_unitree_hg_msg_IMUState const* value) {size_t n=0;
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
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_IMUState_write_fields(roswasmc_bounded_2_unitree_hg_msg_IMUState const* value,unsigned char* p,size_t* offset) {
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
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_IMUState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_IMUState* value) {
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
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->temperature,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_unitree_hg_msg_IMUState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_IMUState_encode(roswasmc_bounded_2_unitree_hg_msg_IMUState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_IMUState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_IMUState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_IMUState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_IMUState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_IMUState* value) {
  roswasmc_bounded_2_unitree_hg_msg_IMUState next={0};size_t offset=0;if(!value||(length&&!input)||length<54u||length>54u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_IMUState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most35 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_unitree_hg_msg_MotorState data[35];} roswasmc_bounded_2_unitree_hg_msg_HandState_motor_state_sequence;
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_unitree_hg_msg_PressSensorState data[4];} roswasmc_bounded_2_unitree_hg_msg_HandState_press_sensor_state_sequence;
/** Complete ROS2 unitree_hg/msg/HandState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_HandState {
  /** Declared field motor_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_hg_msg_HandState_motor_state_sequence motor_state;
  /** Declared field press_sensor_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_hg_msg_HandState_press_sensor_state_sequence press_sensor_state;
  /** Declared field imu_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_hg_msg_IMUState imu_state;
  /** Declared field power_v; canonical capacity is part of CODEC_IDENTITY. */
  float power_v;
  /** Declared field power_a; canonical capacity is part of CODEC_IDENTITY. */
  float power_a;
  /** Declared field system_v; canonical capacity is part of CODEC_IDENTITY. */
  float system_v;
  /** Declared field device_v; canonical capacity is part of CODEC_IDENTITY. */
  float device_v;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t error[2];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[2];
} roswasmc_bounded_2_unitree_hg_msg_HandState;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_HANDSTATE_MAX_WIRE_BYTES 2365u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_HANDSTATE_MIN_WIRE_BYTES 94u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_HANDSTATE_CODEC_IDENTITY "UWVMROS1_5721fdfc155f15ca331dbd398f07cb909196b07e93cabdb6dfbb67b10d4abc4a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_HandState_valid(roswasmc_bounded_2_unitree_hg_msg_HandState const* value) {
  if(!value)return 0;
  if(value->motor_state.size>35u)return 0;
  {size_t i;for(i=0;i<value->motor_state.size;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorState_valid(&value->motor_state.data[i]))return 0;
  }}
  if(value->press_sensor_state.size>4u)return 0;
  {size_t i;for(i=0;i<value->press_sensor_state.size;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_PressSensorState_valid(&value->press_sensor_state.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_unitree_hg_msg_IMUState_valid(&value->imu_state))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->power_v,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->power_a,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->system_v,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->device_v,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_HandState_wire_size(roswasmc_bounded_2_unitree_hg_msg_HandState const* value) {size_t n=0;
  n+=4;
  if(value->motor_state.size>35u)return 0;
  {size_t i;for(i=0;i<value->motor_state.size;++i){
  n+=roswasmc_bounded_2_unitree_hg_msg_MotorState_wire_size(&value->motor_state.data[i]);
  }}
  n+=4;
  if(value->press_sensor_state.size>4u)return 0;
  {size_t i;for(i=0;i<value->press_sensor_state.size;++i){
  n+=roswasmc_bounded_2_unitree_hg_msg_PressSensorState_wire_size(&value->press_sensor_state.data[i]);
  }}
  n+=roswasmc_bounded_2_unitree_hg_msg_IMUState_wire_size(&value->imu_state);
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_HandState_write_fields(roswasmc_bounded_2_unitree_hg_msg_HandState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->motor_state.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_state.size;++i){
  roswasmc_bounded_2_unitree_hg_msg_MotorState_write_fields(&value->motor_state.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->press_sensor_state.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->press_sensor_state.size;++i){
  roswasmc_bounded_2_unitree_hg_msg_PressSensorState_write_fields(&value->press_sensor_state.data[i],p,offset);
  }}
  roswasmc_bounded_2_unitree_hg_msg_IMUState_write_fields(&value->imu_state,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->power_v,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->power_a,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->system_v,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->device_v,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->error[i],4);*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_HandState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_HandState* value) {
  if(*offset>n||n-*offset<4)return 0;value->motor_state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_state.size>35u)return 0;
  {size_t i;for(i=0;i<value->motor_state.size;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorState_read_fields(p,n,offset,&value->motor_state.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->press_sensor_state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->press_sensor_state.size>4u)return 0;
  {size_t i;for(i=0;i<value->press_sensor_state.size;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_PressSensorState_read_fields(p,n,offset,&value->press_sensor_state.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_unitree_hg_msg_IMUState_read_fields(p,n,offset,&value->imu_state))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->power_v,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->power_a,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->system_v,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->device_v,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_hg_msg_HandState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_HandState_encode(roswasmc_bounded_2_unitree_hg_msg_HandState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_HandState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_HandState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_HandState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_HandState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_HandState* value) {
  roswasmc_bounded_2_unitree_hg_msg_HandState next={0};size_t offset=0;if(!value||(length&&!input)||length<94u||length>2365u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_HandState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_hg/msg/LowCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_LowCmd {
  /** Declared field mode_pr; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode_pr;
  /** Declared field mode_machine; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode_machine;
  /** Declared field motor_cmd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_hg_msg_MotorCmd motor_cmd[35];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[4];
  /** Declared field crc; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t crc;
} roswasmc_bounded_2_unitree_hg_msg_LowCmd;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_LOWCMD_MAX_WIRE_BYTES 897u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_LOWCMD_MIN_WIRE_BYTES 897u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_LOWCMD_CODEC_IDENTITY "UWVMROS1_8303ac6e2e3273251662d31daf966a11ced5b5ac10b93aa6253eebc56f4ec461"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_LowCmd_valid(roswasmc_bounded_2_unitree_hg_msg_LowCmd const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<35u;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorCmd_valid(&value->motor_cmd[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_LowCmd_wire_size(roswasmc_bounded_2_unitree_hg_msg_LowCmd const* value) {size_t n=0;
  n+=1;
  n+=1;
  {size_t i;for(i=0;i<35u;++i){
  n+=roswasmc_bounded_2_unitree_hg_msg_MotorCmd_wire_size(&value->motor_cmd[i]);
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=4;
  }}
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_LowCmd_write_fields(roswasmc_bounded_2_unitree_hg_msg_LowCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode_pr,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode_machine,1);*offset+=1;
  {size_t i;for(i=0;i<35u;++i){
  roswasmc_bounded_2_unitree_hg_msg_MotorCmd_write_fields(&value->motor_cmd[i],p,offset);
  }}
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->crc,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_LowCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_LowCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode_pr,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode_machine,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<35u;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorCmd_read_fields(p,n,offset,&value->motor_cmd[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->crc,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_hg_msg_LowCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_LowCmd_encode(roswasmc_bounded_2_unitree_hg_msg_LowCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_LowCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_LowCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_LowCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_LowCmd_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_LowCmd* value) {
  roswasmc_bounded_2_unitree_hg_msg_LowCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<897u||length>897u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_LowCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_hg/msg/LowState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_LowState {
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t version[2];
  /** Declared field mode_pr; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode_pr;
  /** Declared field mode_machine; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode_machine;
  /** Declared field tick; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t tick;
  /** Declared field imu_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_hg_msg_IMUState imu_state;
  /** Declared field motor_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_hg_msg_MotorState motor_state[35];
  /** Declared field wireless_remote; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t wireless_remote[40];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[4];
  /** Declared field crc; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t crc;
} roswasmc_bounded_2_unitree_hg_msg_LowState;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_LOWSTATE_MAX_WIRE_BYTES 1983u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_LOWSTATE_MIN_WIRE_BYTES 1983u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_LOWSTATE_CODEC_IDENTITY "UWVMROS1_7eed2692672f536a476643f1d3a07affc8e5b3272663e3d282c199fff1c2fa1d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_LowState_valid(roswasmc_bounded_2_unitree_hg_msg_LowState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  if(!roswasmc_bounded_2_unitree_hg_msg_IMUState_valid(&value->imu_state))return 0;
  {size_t i;for(i=0;i<35u;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorState_valid(&value->motor_state[i]))return 0;
  }}
  {size_t i;for(i=0;i<40u;++i){
  }}
  {size_t i;for(i=0;i<4u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_LowState_wire_size(roswasmc_bounded_2_unitree_hg_msg_LowState const* value) {size_t n=0;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  n+=1;
  n+=1;
  n+=4;
  n+=roswasmc_bounded_2_unitree_hg_msg_IMUState_wire_size(&value->imu_state);
  {size_t i;for(i=0;i<35u;++i){
  n+=roswasmc_bounded_2_unitree_hg_msg_MotorState_wire_size(&value->motor_state[i]);
  }}
  {size_t i;for(i=0;i<40u;++i){
  n+=1;
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=4;
  }}
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_LowState_write_fields(roswasmc_bounded_2_unitree_hg_msg_LowState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->version[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode_pr,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode_machine,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->tick,4);*offset+=4;
  roswasmc_bounded_2_unitree_hg_msg_IMUState_write_fields(&value->imu_state,p,offset);
  {size_t i;for(i=0;i<35u;++i){
  roswasmc_bounded_2_unitree_hg_msg_MotorState_write_fields(&value->motor_state[i],p,offset);
  }}
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->wireless_remote[i],1);*offset+=1;
  }}
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->crc,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_LowState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_LowState* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->version[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode_pr,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode_machine,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tick,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_2_unitree_hg_msg_IMUState_read_fields(p,n,offset,&value->imu_state))return 0;
  {size_t i;for(i=0;i<35u;++i){
  if(!roswasmc_bounded_2_unitree_hg_msg_MotorState_read_fields(p,n,offset,&value->motor_state[i]))return 0;
  }}
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->wireless_remote[i],&bits,1);}*offset+=1;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->crc,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_hg_msg_LowState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_LowState_encode(roswasmc_bounded_2_unitree_hg_msg_LowState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_LowState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_LowState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_LowState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_LowState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_LowState* value) {
  roswasmc_bounded_2_unitree_hg_msg_LowState next={0};size_t offset=0;if(!value||(length&&!input)||length<1983u||length>1983u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_LowState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_hg/msg/MainBoardState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_hg_msg_MainBoardState {
  /** Declared field fan_state; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t fan_state[6];
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  int16_t temperature[6];
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value[6];
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t state[6];
} roswasmc_bounded_2_unitree_hg_msg_MainBoardState;
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MAINBOARDSTATE_MAX_WIRE_BYTES 72u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MAINBOARDSTATE_MIN_WIRE_BYTES 72u
#define ROSWASMC_BOUNDED_2_UNITREE_HG_MSG_MAINBOARDSTATE_CODEC_IDENTITY "UWVMROS1_2a27a979aadc7bef33a757e169680faa632936c2634ee787f0c9f25cb08535a6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_hg_msg_MainBoardState_valid(roswasmc_bounded_2_unitree_hg_msg_MainBoardState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<6u;++i){
  }}
  {size_t i;for(i=0;i<6u;++i){
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_hg_msg_MainBoardState_wire_size(roswasmc_bounded_2_unitree_hg_msg_MainBoardState const* value) {size_t n=0;
  {size_t i;for(i=0;i<6u;++i){
  n+=2;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=2;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_hg_msg_MainBoardState_write_fields(roswasmc_bounded_2_unitree_hg_msg_MainBoardState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<6u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->fan_state[i],2);*offset+=2;
  }}
  {size_t i;for(i=0;i<6u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature[i],2);*offset+=2;
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->state[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_hg_msg_MainBoardState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_hg_msg_MainBoardState* value) {
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->fan_state[i],&bits,2);}*offset+=2;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->temperature[i],&bits,2);}*offset+=2;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->state[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_hg_msg_MainBoardState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_MainBoardState_encode(roswasmc_bounded_2_unitree_hg_msg_MainBoardState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_hg_msg_MainBoardState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_hg_msg_MainBoardState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_hg_msg_MainBoardState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_hg_msg_MainBoardState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_hg_msg_MainBoardState* value) {
  roswasmc_bounded_2_unitree_hg_msg_MainBoardState next={0};size_t offset=0;if(!value||(length&&!input)||length<72u||length>72u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_hg_msg_MainBoardState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
