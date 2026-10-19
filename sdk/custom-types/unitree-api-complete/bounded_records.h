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
/** Complete ROS2 unitree_api/msg/RequestIdentity value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_api_msg_RequestIdentity {
  /** Declared field id; canonical capacity is part of CODEC_IDENTITY. */
  int64_t id;
  /** Declared field api_id; canonical capacity is part of CODEC_IDENTITY. */
  int64_t api_id;
} roswasmc_bounded_2_unitree_api_msg_RequestIdentity;
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTIDENTITY_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTIDENTITY_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTIDENTITY_CODEC_IDENTITY "UWVMROS1_e3daab89abcdaf6526511a189cfcc0c78aaebe8aefc53814ba77f6eb80cdc4e2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_api_msg_RequestIdentity_valid(roswasmc_bounded_2_unitree_api_msg_RequestIdentity const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_api_msg_RequestIdentity_wire_size(roswasmc_bounded_2_unitree_api_msg_RequestIdentity const* value) {size_t n=0;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_api_msg_RequestIdentity_write_fields(roswasmc_bounded_2_unitree_api_msg_RequestIdentity const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->id,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->api_id,8);*offset+=8;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_api_msg_RequestIdentity_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_api_msg_RequestIdentity* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->id,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->api_id,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_unitree_api_msg_RequestIdentity_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_RequestIdentity_encode(roswasmc_bounded_2_unitree_api_msg_RequestIdentity const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_api_msg_RequestIdentity_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_api_msg_RequestIdentity_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_api_msg_RequestIdentity_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_RequestIdentity_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_api_msg_RequestIdentity* value) {
  roswasmc_bounded_2_unitree_api_msg_RequestIdentity next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestIdentity_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_api/msg/RequestLease value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_api_msg_RequestLease {
  /** Declared field id; canonical capacity is part of CODEC_IDENTITY. */
  int64_t id;
} roswasmc_bounded_2_unitree_api_msg_RequestLease;
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTLEASE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTLEASE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTLEASE_CODEC_IDENTITY "UWVMROS1_3dcb1fe2998c880e2d9196967da172ccd9060ebc7f6d2feb8682dc3d104c2e6e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_api_msg_RequestLease_valid(roswasmc_bounded_2_unitree_api_msg_RequestLease const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_api_msg_RequestLease_wire_size(roswasmc_bounded_2_unitree_api_msg_RequestLease const* value) {size_t n=0;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_api_msg_RequestLease_write_fields(roswasmc_bounded_2_unitree_api_msg_RequestLease const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->id,8);*offset+=8;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_api_msg_RequestLease_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_api_msg_RequestLease* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->id,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_unitree_api_msg_RequestLease_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_RequestLease_encode(roswasmc_bounded_2_unitree_api_msg_RequestLease const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_api_msg_RequestLease_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_api_msg_RequestLease_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_api_msg_RequestLease_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_RequestLease_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_api_msg_RequestLease* value) {
  roswasmc_bounded_2_unitree_api_msg_RequestLease next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestLease_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_api/msg/RequestPolicy value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_api_msg_RequestPolicy {
  /** Declared field priority; canonical capacity is part of CODEC_IDENTITY. */
  int32_t priority;
  /** Declared field noreply; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t noreply;
} roswasmc_bounded_2_unitree_api_msg_RequestPolicy;
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTPOLICY_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTPOLICY_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTPOLICY_CODEC_IDENTITY "UWVMROS1_61f1a1eafec2f694d24cf798963dcb5a0c8f40e9791db5a502f0736cc8587bb8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_api_msg_RequestPolicy_valid(roswasmc_bounded_2_unitree_api_msg_RequestPolicy const* value) {
  if(!value)return 0;
  if(value->noreply>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_api_msg_RequestPolicy_wire_size(roswasmc_bounded_2_unitree_api_msg_RequestPolicy const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_api_msg_RequestPolicy_write_fields(roswasmc_bounded_2_unitree_api_msg_RequestPolicy const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->priority,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->noreply,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_api_msg_RequestPolicy_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_api_msg_RequestPolicy* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->priority,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->noreply,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_unitree_api_msg_RequestPolicy_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_RequestPolicy_encode(roswasmc_bounded_2_unitree_api_msg_RequestPolicy const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_api_msg_RequestPolicy_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_api_msg_RequestPolicy_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_api_msg_RequestPolicy_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_RequestPolicy_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_api_msg_RequestPolicy* value) {
  roswasmc_bounded_2_unitree_api_msg_RequestPolicy next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestPolicy_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_api/msg/RequestHeader value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_api_msg_RequestHeader {
  /** Declared field identity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_RequestIdentity identity;
  /** Declared field lease; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_RequestLease lease;
  /** Declared field policy; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_RequestPolicy policy;
} roswasmc_bounded_2_unitree_api_msg_RequestHeader;
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTHEADER_MAX_WIRE_BYTES 29u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTHEADER_MIN_WIRE_BYTES 29u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUESTHEADER_CODEC_IDENTITY "UWVMROS1_c8e8b83e2cdc978c5279483dbfac8d4185a3e05c87799b424643f0fcb02d0fd0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_api_msg_RequestHeader_valid(roswasmc_bounded_2_unitree_api_msg_RequestHeader const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestIdentity_valid(&value->identity))return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestLease_valid(&value->lease))return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestPolicy_valid(&value->policy))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_api_msg_RequestHeader_wire_size(roswasmc_bounded_2_unitree_api_msg_RequestHeader const* value) {size_t n=0;
  n+=roswasmc_bounded_2_unitree_api_msg_RequestIdentity_wire_size(&value->identity);
  n+=roswasmc_bounded_2_unitree_api_msg_RequestLease_wire_size(&value->lease);
  n+=roswasmc_bounded_2_unitree_api_msg_RequestPolicy_wire_size(&value->policy);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_api_msg_RequestHeader_write_fields(roswasmc_bounded_2_unitree_api_msg_RequestHeader const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_unitree_api_msg_RequestIdentity_write_fields(&value->identity,p,offset);
  roswasmc_bounded_2_unitree_api_msg_RequestLease_write_fields(&value->lease,p,offset);
  roswasmc_bounded_2_unitree_api_msg_RequestPolicy_write_fields(&value->policy,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_api_msg_RequestHeader_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_api_msg_RequestHeader* value) {
  if(!roswasmc_bounded_2_unitree_api_msg_RequestIdentity_read_fields(p,n,offset,&value->identity))return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestLease_read_fields(p,n,offset,&value->lease))return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestPolicy_read_fields(p,n,offset,&value->policy))return 0;
  return roswasmc_bounded_2_unitree_api_msg_RequestHeader_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_RequestHeader_encode(roswasmc_bounded_2_unitree_api_msg_RequestHeader const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_api_msg_RequestHeader_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_api_msg_RequestHeader_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_api_msg_RequestHeader_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_RequestHeader_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_api_msg_RequestHeader* value) {
  roswasmc_bounded_2_unitree_api_msg_RequestHeader next={0};size_t offset=0;if(!value||(length&&!input)||length<29u||length>29u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestHeader_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for unitree_api/msg/Request.parameter; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_unitree_api_msg_Request_parameter_text;
/** At most2048 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2048];} roswasmc_bounded_2_unitree_api_msg_Request_binary_sequence;
/** Complete ROS2 unitree_api/msg/Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_api_msg_Request {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_RequestHeader header;
  /** Declared field parameter; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_Request_parameter_text parameter;
  /** Declared field binary; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_Request_binary_sequence binary;
} roswasmc_bounded_2_unitree_api_msg_Request;
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUEST_MAX_WIRE_BYTES 6181u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUEST_MIN_WIRE_BYTES 37u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUEST_CODEC_IDENTITY "UWVMROS1_39bb2426d68c160c07e7cff951d234c8ec9d0c70fddb5d432d9b238d08565eb2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_api_msg_Request_valid(roswasmc_bounded_2_unitree_api_msg_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestHeader_valid(&value->header))return 0;
  if(value->parameter.size>4096u||!uwvm_bounded_utf8(value->parameter.data,value->parameter.size))return 0;
  if(value->binary.size>2048u)return 0;
  {size_t i;for(i=0;i<value->binary.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_api_msg_Request_wire_size(roswasmc_bounded_2_unitree_api_msg_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_unitree_api_msg_RequestHeader_wire_size(&value->header);
  n+=4+value->parameter.size;
  n+=4;
  if(value->binary.size>2048u)return 0;
  {size_t i;for(i=0;i<value->binary.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_api_msg_Request_write_fields(roswasmc_bounded_2_unitree_api_msg_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_unitree_api_msg_RequestHeader_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->parameter.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->parameter.data,value->parameter.size);*offset+=value->parameter.size;
  uwvm_bounded_put(p+*offset,value->binary.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->binary.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->binary.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_api_msg_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_api_msg_Request* value) {
  if(!roswasmc_bounded_2_unitree_api_msg_RequestHeader_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->parameter.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->parameter.size>4096u||value->parameter.size>n-*offset)return 0;uwvm_bounded_copy(value->parameter.data,p+*offset,value->parameter.size);*offset+=value->parameter.size;
  if(*offset>n||n-*offset<4)return 0;value->binary.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->binary.size>2048u)return 0;
  {size_t i;for(i=0;i<value->binary.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->binary.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_unitree_api_msg_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_Request_encode(roswasmc_bounded_2_unitree_api_msg_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_api_msg_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_api_msg_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_api_msg_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_Request_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_api_msg_Request* value) {
  roswasmc_bounded_2_unitree_api_msg_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<37u||length>6181u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_api_msg_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_api/msg/ResponseStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_api_msg_ResponseStatus {
  /** Declared field code; canonical capacity is part of CODEC_IDENTITY. */
  int32_t code;
} roswasmc_bounded_2_unitree_api_msg_ResponseStatus;
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSESTATUS_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSESTATUS_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSESTATUS_CODEC_IDENTITY "UWVMROS1_aaad17e09d5d6549455f69060d1bb03ec81cddfc0ede323a5f2365a6c1a7b7b1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_api_msg_ResponseStatus_valid(roswasmc_bounded_2_unitree_api_msg_ResponseStatus const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_api_msg_ResponseStatus_wire_size(roswasmc_bounded_2_unitree_api_msg_ResponseStatus const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_api_msg_ResponseStatus_write_fields(roswasmc_bounded_2_unitree_api_msg_ResponseStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->code,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_api_msg_ResponseStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_api_msg_ResponseStatus* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->code,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_api_msg_ResponseStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_ResponseStatus_encode(roswasmc_bounded_2_unitree_api_msg_ResponseStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_api_msg_ResponseStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_api_msg_ResponseStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_api_msg_ResponseStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_ResponseStatus_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_api_msg_ResponseStatus* value) {
  roswasmc_bounded_2_unitree_api_msg_ResponseStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_api_msg_ResponseStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_api/msg/ResponseHeader value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_api_msg_ResponseHeader {
  /** Declared field identity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_RequestIdentity identity;
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_ResponseStatus status;
} roswasmc_bounded_2_unitree_api_msg_ResponseHeader;
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSEHEADER_MAX_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSEHEADER_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSEHEADER_CODEC_IDENTITY "UWVMROS1_20ddef706f0be7d7e2e356ab2222b6e198bd36fd6d8a81706553d5a883de85e0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_api_msg_ResponseHeader_valid(roswasmc_bounded_2_unitree_api_msg_ResponseHeader const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_RequestIdentity_valid(&value->identity))return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_ResponseStatus_valid(&value->status))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_api_msg_ResponseHeader_wire_size(roswasmc_bounded_2_unitree_api_msg_ResponseHeader const* value) {size_t n=0;
  n+=roswasmc_bounded_2_unitree_api_msg_RequestIdentity_wire_size(&value->identity);
  n+=roswasmc_bounded_2_unitree_api_msg_ResponseStatus_wire_size(&value->status);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_api_msg_ResponseHeader_write_fields(roswasmc_bounded_2_unitree_api_msg_ResponseHeader const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_unitree_api_msg_RequestIdentity_write_fields(&value->identity,p,offset);
  roswasmc_bounded_2_unitree_api_msg_ResponseStatus_write_fields(&value->status,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_api_msg_ResponseHeader_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_api_msg_ResponseHeader* value) {
  if(!roswasmc_bounded_2_unitree_api_msg_RequestIdentity_read_fields(p,n,offset,&value->identity))return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_ResponseStatus_read_fields(p,n,offset,&value->status))return 0;
  return roswasmc_bounded_2_unitree_api_msg_ResponseHeader_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_ResponseHeader_encode(roswasmc_bounded_2_unitree_api_msg_ResponseHeader const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_api_msg_ResponseHeader_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_api_msg_ResponseHeader_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_api_msg_ResponseHeader_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_ResponseHeader_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_api_msg_ResponseHeader* value) {
  roswasmc_bounded_2_unitree_api_msg_ResponseHeader next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>20u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_api_msg_ResponseHeader_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for unitree_api/msg/Response.data; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_unitree_api_msg_Response_data_text;
/** At most2048 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;int8_t data[2048];} roswasmc_bounded_2_unitree_api_msg_Response_binary_sequence;
/** Complete ROS2 unitree_api/msg/Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_api_msg_Response {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_ResponseHeader header;
  /** Declared field data; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_Response_data_text data;
  /** Declared field binary; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_api_msg_Response_binary_sequence binary;
} roswasmc_bounded_2_unitree_api_msg_Response;
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSE_MAX_WIRE_BYTES 6172u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSE_CODEC_IDENTITY "UWVMROS1_c048bd88b9c7d86cde04e6a589c50aeba396be29acf0d58bc6972f2dd8d8b749"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_api_msg_Response_valid(roswasmc_bounded_2_unitree_api_msg_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_unitree_api_msg_ResponseHeader_valid(&value->header))return 0;
  if(value->data.size>4096u||!uwvm_bounded_utf8(value->data.data,value->data.size))return 0;
  if(value->binary.size>2048u)return 0;
  {size_t i;for(i=0;i<value->binary.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_api_msg_Response_wire_size(roswasmc_bounded_2_unitree_api_msg_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_unitree_api_msg_ResponseHeader_wire_size(&value->header);
  n+=4+value->data.size;
  n+=4;
  if(value->binary.size>2048u)return 0;
  {size_t i;for(i=0;i<value->binary.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_api_msg_Response_write_fields(roswasmc_bounded_2_unitree_api_msg_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_unitree_api_msg_ResponseHeader_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->data.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->data.data,value->data.size);*offset+=value->data.size;
  uwvm_bounded_put(p+*offset,value->binary.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->binary.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->binary.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_api_msg_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_api_msg_Response* value) {
  if(!roswasmc_bounded_2_unitree_api_msg_ResponseHeader_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->data.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->data.size>4096u||value->data.size>n-*offset)return 0;uwvm_bounded_copy(value->data.data,p+*offset,value->data.size);*offset+=value->data.size;
  if(*offset>n||n-*offset<4)return 0;value->binary.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->binary.size>2048u)return 0;
  {size_t i;for(i=0;i<value->binary.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->binary.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_unitree_api_msg_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_Response_encode(roswasmc_bounded_2_unitree_api_msg_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_api_msg_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_api_msg_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_api_msg_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_api_msg_Response_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_api_msg_Response* value) {
  roswasmc_bounded_2_unitree_api_msg_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>6172u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_api_msg_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
