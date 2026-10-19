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
/** UTF-8 byte storage for uwvm_limx_bridge/msg/NativeHeader.frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_frame_id_text;
/** Complete ROS2 uwvm_limx_bridge/msg/NativeHeader value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader {
  /** Declared field seq; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t seq;
  /** Declared field stamp_sec; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t stamp_sec;
  /** Declared field stamp_nsec; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t stamp_nsec;
  /** Declared field frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_frame_id_text frame_id;
} roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader;
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_NATIVEHEADER_MAX_WIRE_BYTES 144u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_NATIVEHEADER_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_NATIVEHEADER_CODEC_IDENTITY "UWVMROS1_d86b96b26db6c2a28dfa86e2255cb3cccd6cd24648e877746d8d75394d1635b2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_valid(roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader const* value) {
  if(!value)return 0;
  if(value->frame_id.size>128u||!uwvm_bounded_utf8(value->frame_id.data,value->frame_id.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_wire_size(roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4+value->frame_id.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_write_fields(roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->seq,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->stamp_sec,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->stamp_nsec,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->frame_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->frame_id.data,value->frame_id.size);*offset+=value->frame_id.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->seq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->stamp_sec,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->stamp_nsec,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->frame_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->frame_id.size>128u||value->frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->frame_id.data,p+*offset,value->frame_id.size);*offset+=value->frame_id.size;
  return roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_encode(roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader* value) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>144u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_limx_bridge/msg/ImuData value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader header;
  /** Declared field imustamp; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t imustamp;
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t status;
  /** Declared field euler; canonical capacity is part of CODEC_IDENTITY. */
  double euler[3];
  /** Declared field quat; canonical capacity is part of CODEC_IDENTITY. */
  double quat[4];
  /** Declared field acc; canonical capacity is part of CODEC_IDENTITY. */
  double acc[3];
  /** Declared field gyro; canonical capacity is part of CODEC_IDENTITY. */
  double gyro[3];
} roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData;
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_IMUDATA_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_IMUDATA_MIN_WIRE_BYTES 132u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_IMUDATA_CODEC_IDENTITY "UWVMROS1_53ba9eb23b36de77e1793bdd19a31f708aca557b81f4ae72927b2a44f5ce097b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_valid(roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_valid(&value->header))return 0;
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->euler[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<4u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->quat[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acc[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->gyro[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_wire_size(roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData const* value) {size_t n=0;
  n+=roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_wire_size(&value->header);
  n+=8;
  n+=4;
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_write_fields(roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->imustamp,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,4);*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->euler[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<4u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->quat[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acc[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->gyro[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData* value) {
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->imustamp,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->status,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->euler[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->quat[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->acc[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->gyro[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_encode(roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData* value) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData next={0};size_t offset=0;if(!value||(length&&!input)||length<132u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_limx_bridge/msg/JointState.names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_names_text;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_names_text data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_names_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_q_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_v_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_vd_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_tau_sequence;
/** Complete ROS2 uwvm_limx_bridge/msg/JointState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader header;
  /** Declared field names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_names_sequence names;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_q_sequence q;
  /** Declared field v; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_v_sequence v;
  /** Declared field vd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_vd_sequence vd;
  /** Declared field tau; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_tau_sequence tau;
  /** Declared field na; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t na;
} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState;
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTSTATE_MAX_WIRE_BYTES 5544u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTSTATE_MIN_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTSTATE_CODEC_IDENTITY "UWVMROS1_d99e5855c0dd55d4d1b5a724ad5719734a668a512ea09e553c8f4a560b97a3a4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_valid(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_valid(&value->header))return 0;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  if(value->names.data[i].size>64u||!uwvm_bounded_utf8(value->names.data[i].data,value->names.data[i].size))return 0;
  }}
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->v.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->vd.size>64u)return 0;
  {size_t i;for(i=0;i<value->vd.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vd.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_wire_size(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_wire_size(&value->header);
  n+=4;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  n+=4+value->names.data[i].size;
  }}
  n+=4;
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->vd.size>64u)return 0;
  {size_t i;for(i=0;i<value->vd.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  n+=4;
  }}
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_write_fields(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->names.size;++i){
  uwvm_bounded_put(p+*offset,value->names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->names.data[i].data,value->names.data[i].size);*offset+=value->names.data[i].size;
  }}
  uwvm_bounded_put(p+*offset,value->q.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->q.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->v.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->v.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->v.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->vd.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->vd.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vd.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->tau.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->tau.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->na,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState* value) {
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->names.data[i].size>64u||value->names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->names.data[i].data,p+*offset,value->names.data[i].size);*offset+=value->names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->q.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->v.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->v.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->vd.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->vd.size>64u)return 0;
  {size_t i;for(i=0;i<value->vd.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->vd.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->tau.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->na,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_encode(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState* value) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState next={0};size_t offset=0;if(!value||(length&&!input)||length<40u||length>5544u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_limx_bridge/msg/JointCmd.names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_names_text;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_names_text data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_names_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_q_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_v_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_tau_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_kp_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_kd_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_mode_sequence;
/** Complete ROS2 uwvm_limx_bridge/msg/JointCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader header;
  /** Declared field names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_names_sequence names;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_q_sequence q;
  /** Declared field v; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_v_sequence v;
  /** Declared field tau; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_tau_sequence tau;
  /** Declared field kp; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_kp_sequence kp;
  /** Declared field kd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_kd_sequence kd;
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_mode_sequence mode;
  /** Declared field na; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t na;
} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd;
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTCMD_MAX_WIRE_BYTES 5872u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTCMD_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTCMD_CODEC_IDENTITY "UWVMROS1_8c477a93cc192cd3039b285f111a56eb1305707d798587ef7ab9504f714ce6f0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_valid(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_valid(&value->header))return 0;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  if(value->names.data[i].size>64u||!uwvm_bounded_utf8(value->names.data[i].data,value->names.data[i].size))return 0;
  }}
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->v.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->kp.size>64u)return 0;
  {size_t i;for(i=0;i<value->kp.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->kd.size>64u)return 0;
  {size_t i;for(i=0;i<value->kd.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->mode.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_wire_size(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd const* value) {size_t n=0;
  n+=roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_wire_size(&value->header);
  n+=4;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  n+=4+value->names.data[i].size;
  }}
  n+=4;
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->kp.size>64u)return 0;
  {size_t i;for(i=0;i<value->kp.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->kd.size>64u)return 0;
  {size_t i;for(i=0;i<value->kd.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->mode.size;++i){
  n+=1;
  }}
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_write_fields(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->names.size;++i){
  uwvm_bounded_put(p+*offset,value->names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->names.data[i].data,value->names.data[i].size);*offset+=value->names.data[i].size;
  }}
  uwvm_bounded_put(p+*offset,value->q.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->q.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->v.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->v.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->v.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->tau.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->tau.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->kp.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->kp.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->kd.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->kd.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->mode.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->mode.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->na,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd* value) {
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->names.data[i].size>64u||value->names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->names.data[i].data,p+*offset,value->names.data[i].size);*offset+=value->names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->q.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->v.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->v.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->tau.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->kp.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->kp.size>64u)return 0;
  {size_t i;for(i=0;i<value->kp.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kp.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->kd.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->kd.size>64u)return 0;
  {size_t i;for(i=0;i<value->kd.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kd.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->mode.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->mode.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->na,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_encode(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd* value) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>5872u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_limx_bridge/msg/JointCmdLimx.names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_names_text;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_names_text data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_names_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_q_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_v_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_tau_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_kp_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_kd_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_mode_sequence;
/** At most64 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[64];} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_parallel_solver_mode_sequence;
/** Complete ROS2 uwvm_limx_bridge/msg/JointCmdLimx value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader header;
  /** Declared field names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_names_sequence names;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_q_sequence q;
  /** Declared field v; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_v_sequence v;
  /** Declared field tau; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_tau_sequence tau;
  /** Declared field kp; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_kp_sequence kp;
  /** Declared field kd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_kd_sequence kd;
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_mode_sequence mode;
  /** Declared field na; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t na;
  /** Declared field parallel_solver_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_parallel_solver_mode_sequence parallel_solver_mode;
} roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx;
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTCMDLIMX_MAX_WIRE_BYTES 5940u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTCMDLIMX_MIN_WIRE_BYTES 52u
#define ROSWASMC_BOUNDED_2_UWVM_LIMX_BRIDGE_MSG_JOINTCMDLIMX_CODEC_IDENTITY "UWVMROS1_d469dd32f446fb4013144b7e176cdb831c146b6a9279ea776ec4a0c053060be9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_valid(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_valid(&value->header))return 0;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  if(value->names.data[i].size>64u||!uwvm_bounded_utf8(value->names.data[i].data,value->names.data[i].size))return 0;
  }}
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->v.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->kp.size>64u)return 0;
  {size_t i;for(i=0;i<value->kp.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->kd.size>64u)return 0;
  {size_t i;for(i=0;i<value->kd.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->mode.size;++i){
  }}
  if(value->parallel_solver_mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->parallel_solver_mode.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_wire_size(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx const* value) {size_t n=0;
  n+=roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_wire_size(&value->header);
  n+=4;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  n+=4+value->names.data[i].size;
  }}
  n+=4;
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->kp.size>64u)return 0;
  {size_t i;for(i=0;i<value->kp.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->kd.size>64u)return 0;
  {size_t i;for(i=0;i<value->kd.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->mode.size;++i){
  n+=1;
  }}
  n+=4;
  n+=4;
  if(value->parallel_solver_mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->parallel_solver_mode.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_write_fields(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->names.size;++i){
  uwvm_bounded_put(p+*offset,value->names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->names.data[i].data,value->names.data[i].size);*offset+=value->names.data[i].size;
  }}
  uwvm_bounded_put(p+*offset,value->q.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->q.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->v.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->v.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->v.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->tau.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->tau.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->kp.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->kp.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->kd.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->kd.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->mode.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->mode.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->na,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->parallel_solver_mode.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->parallel_solver_mode.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->parallel_solver_mode.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx* value) {
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->names.size>64u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->names.data[i].size>64u||value->names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->names.data[i].data,p+*offset,value->names.data[i].size);*offset+=value->names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->q.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->q.size>64u)return 0;
  {size_t i;for(i=0;i<value->q.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->v.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->v.size>64u)return 0;
  {size_t i;for(i=0;i<value->v.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->v.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->tau.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->tau.size>64u)return 0;
  {size_t i;for(i=0;i<value->tau.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->kp.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->kp.size>64u)return 0;
  {size_t i;for(i=0;i<value->kp.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kp.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->kd.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->kd.size>64u)return 0;
  {size_t i;for(i=0;i<value->kd.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kd.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->mode.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->mode.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->na,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->parallel_solver_mode.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->parallel_solver_mode.size>64u)return 0;
  {size_t i;for(i=0;i<value->parallel_solver_mode.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->parallel_solver_mode.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_encode(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx* value) {
  roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx next={0};size_t offset=0;if(!value||(length&&!input)||length<52u||length>5940u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
