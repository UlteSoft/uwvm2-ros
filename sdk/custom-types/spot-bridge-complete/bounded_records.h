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
/** Complete ROS2 uwvm_spot_bridge/srv/Command_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field budget_ns; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t budget_ns;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_COMMAND_REQUEST_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_COMMAND_REQUEST_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_COMMAND_REQUEST_CODEC_IDENTITY "UWVMROS1_29996d4dacb4ea847c5d629cb634d4aa5e2451627d2469d6cdf301cc007968f8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->budget_ns,8);*offset+=8;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->budget_ns,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request* value) {
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_spot_bridge/srv/Command_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t error;
  /** Declared field unknown; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t unknown;
  /** Declared field native_accepted; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t native_accepted;
  /** Declared field native_command_id; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t native_command_id;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_COMMAND_RESPONSE_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_COMMAND_RESPONSE_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_COMMAND_RESPONSE_CODEC_IDENTITY "UWVMROS1_e35556e7439c8d5ca79cb0f4ad23cf73c85a8a79d1382cde398dd85082dd1733"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response const* value) {
  if(!value)return 0;
  if(value->unknown>1)return 0;
  if(value->native_accepted>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=4;
  n+=1;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->error,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->unknown,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->native_accepted,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->native_command_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->unknown,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->native_accepted,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->native_command_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response* value) {
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_spot_bridge/srv/Feedback_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field budget_ns; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t budget_ns;
  /** Declared field command_token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t command_token;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_FEEDBACK_REQUEST_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_FEEDBACK_REQUEST_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_FEEDBACK_REQUEST_CODEC_IDENTITY "UWVMROS1_de64ce283f49f5c324912ec262f7d15b6fd828879c01039d455c765743c45ba3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->budget_ns,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->command_token,8);*offset+=8;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->budget_ns,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->command_token,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request* value) {
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16384 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[16384];} roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_native_proto_sequence;
/** Complete ROS2 uwvm_spot_bridge/srv/Feedback_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t error;
  /** Declared field unknown; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t unknown;
  /** Declared field native_command_id; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t native_command_id;
  /** Declared field has_full_body_feedback; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t has_full_body_feedback;
  /** Declared field full_body_status; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t full_body_status;
  /** Declared field has_mobility_feedback; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t has_mobility_feedback;
  /** Declared field mobility_status; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t mobility_status;
  /** Declared field feedback_kind; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t feedback_kind;
  /** Declared field has_subcommand_status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t has_subcommand_status;
  /** Declared field subcommand_status; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t subcommand_status;
  /** Declared field native_proto; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_native_proto_sequence native_proto;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_FEEDBACK_RESPONSE_MAX_WIRE_BYTES 16429u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_FEEDBACK_RESPONSE_MIN_WIRE_BYTES 45u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_FEEDBACK_RESPONSE_CODEC_IDENTITY "UWVMROS1_dc801501c5f5c993eea419f4a94f5c7eb57a5cfe113d0351a3b1e3fbfd514901"
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_FEEDBACK_RESPONSE_CARTESIAN_MAX_WIRE_BYTES UINT64_C(16429)
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_FEEDBACK_RESPONSE_AGGREGATE_WIRE_BYTES 32768u
/** Shape validation retains every field; aggregate quota is checked separately. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_shape(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response const* value) {if(!value)return 0;
  if(value->unknown>1)return 0;
  if(value->has_full_body_feedback>1)return 0;
  if(value->has_mobility_feedback>1)return 0;
  if(value->has_subcommand_status>1)return 0;
  if(value->native_proto.size>16384u)return 0;
  {size_t i;for(i=0;i<value->native_proto.size;++i){
  }}
  return 1;}
/** Checked addition under this record's full admitted wire extent. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(size_t* n,size_t amount) {if(*n>16429u||amount>16429u-*n)return 0;*n+=amount;return 1;}
/** Full validation and measure; quota90 preserves the caller's size sentinel. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_measure(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response const* value,size_t* length) {size_t n=0;
  if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_shape(value)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,8))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,8))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,1))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,1))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,1))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,1))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,1))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->native_proto.size;++i){
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_add(&n,1))return UWVM_BOUNDED_TOO_LARGE;
  }}
  *length=n;return 0;}
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response const* value) {size_t n=0;return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_measure(value,&n)==0;}
/** Compatibility size view; use measure for an unambiguous error/status. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response const* value) {size_t n=0;return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_measure(value,&n)?0:n;}
/** Internal no-fail writer, entered only after complete measure/overlap proof. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->error,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->unknown,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->native_command_id,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->has_full_body_feedback,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->full_body_status,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->has_mobility_feedback,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->mobility_status,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->feedback_kind,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->has_subcommand_status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->subcommand_status,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->native_proto.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->native_proto.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->native_proto.data[i],1);*offset+=1;
  }}
}
/** Internal reader/status keeps all intermediate mutations private. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_read_status(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response* value) {size_t measured=0;
  if(!value||!offset||*offset>n||(n&&!p))return UWVM_BOUNDED_INVALID;
  if(*offset>n||n-*offset<8)return UWVM_BOUNDED_INVALID;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return UWVM_BOUNDED_INVALID;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return UWVM_BOUNDED_INVALID;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->unknown,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->native_command_id,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return UWVM_BOUNDED_INVALID;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->has_full_body_feedback,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->full_body_status,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return UWVM_BOUNDED_INVALID;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->has_mobility_feedback,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mobility_status,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return UWVM_BOUNDED_INVALID;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->feedback_kind,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return UWVM_BOUNDED_INVALID;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->has_subcommand_status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->subcommand_status,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->native_proto.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->native_proto.size>16384u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->native_proto.size;++i){
  if(*offset>n||n-*offset<1)return UWVM_BOUNDED_INVALID;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->native_proto.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_measure(value,&measured);
}
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response* value) {return roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_read_status(p,n,offset,value)==0;}
/** No writes before full shape, nested quotas, exact size and disjoint destinations. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response const* value,void* output,size_t capacity,size_t* length) {size_t n=0,offset=0;uint32_t error;
  if(!length)return UWVM_BOUNDED_INVALID;error=roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_measure(value,&n);if(error)return error;
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;}
/** The compiler may reserve the full temporary stack frame even on preflight failure. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response* value) {size_t offset=0;uint32_t error;
  if(!value||(length&&!input)||length<45u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(length>16429u)return UWVM_BOUNDED_TOO_LARGE;
  {roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response next={0};error=roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_read_status((unsigned char const*)input,length,&offset,&next);if(error)return error;if(offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;}}
/** Complete ROS2 uwvm_spot_bridge/srv/Lease_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field budget_ns; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t budget_ns;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_LEASE_REQUEST_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_LEASE_REQUEST_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_LEASE_REQUEST_CODEC_IDENTITY "UWVMROS1_a9aacd78dc6ae9d18fb6bb0d185797c6463c882edadf251eb5c21e3aa3c538a1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->budget_ns,8);*offset+=8;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->budget_ns,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request* value) {
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_spot_bridge/srv/Lease_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t error;
  /** Declared field unknown; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t unknown;
  /** Declared field provider_holds_body_lease; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t provider_holds_body_lease;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_LEASE_RESPONSE_MAX_WIRE_BYTES 22u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_LEASE_RESPONSE_MIN_WIRE_BYTES 22u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_LEASE_RESPONSE_CODEC_IDENTITY "UWVMROS1_ce875eb701b607c01f8a19dc8fcff607829824ad99276f906c7fc8fc5567f6d3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response const* value) {
  if(!value)return 0;
  if(value->unknown>1)return 0;
  if(value->provider_holds_body_lease>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->error,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->unknown,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->provider_holds_body_lease,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->unknown,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->provider_holds_body_lease,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response* value) {
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<22u||length>22u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_spot_bridge/srv/State_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field budget_ns; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t budget_ns;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_STATE_REQUEST_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_STATE_REQUEST_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_STATE_REQUEST_CODEC_IDENTITY "UWVMROS1_9becf278bc168ed6d5b15fe7f1ec35b83e274de8a5173cb2335948306bc7a47e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->budget_ns,8);*offset+=8;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->budget_ns,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request* value) {
  roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_spot_bridge/srv/State_Response.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_names_text;
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_names_text data[32];} roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_names_sequence;
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[32];} roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_position_sequence;
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[32];} roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_velocity_sequence;
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[32];} roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_load_sequence;
/** At most16384 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[16384];} roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_native_proto_sequence;
/** Complete ROS2 uwvm_spot_bridge/srv/State_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t error;
  /** Declared field unknown; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t unknown;
  /** Declared field acquisition_sec; canonical capacity is part of CODEC_IDENTITY. */
  int64_t acquisition_sec;
  /** Declared field acquisition_nsec; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t acquisition_nsec;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_names_sequence joint_names;
  /** Declared field joint_position; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_position_sequence joint_position;
  /** Declared field joint_velocity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_velocity_sequence joint_velocity;
  /** Declared field joint_load; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_joint_load_sequence joint_load;
  /** Declared field motor_power_state; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t motor_power_state;
  /** Declared field native_proto; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_native_proto_sequence native_proto;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_STATE_RESPONSE_MAX_WIRE_BYTES 21433u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_STATE_RESPONSE_MIN_WIRE_BYTES 57u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_STATE_RESPONSE_CODEC_IDENTITY "UWVMROS1_8ffde1e9e2470975f03e038dbd51cf288c506671b4f3fe64b59223cb9711fb6c"
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_STATE_RESPONSE_CARTESIAN_MAX_WIRE_BYTES UINT64_C(21433)
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_STATE_RESPONSE_AGGREGATE_WIRE_BYTES 32768u
/** Shape validation retains every field; aggregate quota is checked separately. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_shape(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response const* value) {if(!value)return 0;
  if(value->unknown>1)return 0;
  if(value->joint_names.size>32u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>128u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(value->joint_position.size>32u)return 0;
  {size_t i;for(i=0;i<value->joint_position.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_position.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->joint_velocity.size>32u)return 0;
  {size_t i;for(i=0;i<value->joint_velocity.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_velocity.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->joint_load.size>32u)return 0;
  {size_t i;for(i=0;i<value->joint_load.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_load.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->native_proto.size>16384u)return 0;
  {size_t i;for(i=0;i<value->native_proto.size;++i){
  }}
  return 1;}
/** Checked addition under this record's full admitted wire extent. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(size_t* n,size_t amount) {if(*n>21433u||amount>21433u-*n)return 0;*n+=amount;return 1;}
/** Full validation and measure; quota90 preserves the caller's size sentinel. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_measure(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response const* value,size_t* length) {size_t n=0;
  if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_shape(value)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,8))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,8))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,1))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,8))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4)||!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,value->joint_names.data[i].size))return UWVM_BOUNDED_TOO_LARGE;
  }}
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->joint_position.size;++i){
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,8))return UWVM_BOUNDED_TOO_LARGE;
  }}
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->joint_velocity.size;++i){
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,8))return UWVM_BOUNDED_TOO_LARGE;
  }}
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->joint_load.size;++i){
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,8))return UWVM_BOUNDED_TOO_LARGE;
  }}
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->native_proto.size;++i){
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_add(&n,1))return UWVM_BOUNDED_TOO_LARGE;
  }}
  *length=n;return 0;}
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response const* value) {size_t n=0;return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_measure(value,&n)==0;}
/** Compatibility size view; use measure for an unambiguous error/status. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response const* value) {size_t n=0;return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_measure(value,&n)?0:n;}
/** Internal no-fail writer, entered only after complete measure/overlap proof. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->error,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->unknown,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->acquisition_sec,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->acquisition_nsec,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_names.data[i].data,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  uwvm_bounded_put(p+*offset,value->joint_position.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_position.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_position.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->joint_velocity.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_velocity.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_velocity.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->joint_load.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_load.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_load.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->motor_power_state,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->native_proto.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->native_proto.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->native_proto.data[i],1);*offset+=1;
  }}
}
/** Internal reader/status keeps all intermediate mutations private. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_read_status(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response* value) {size_t measured=0;
  if(!value||!offset||*offset>n||(n&&!p))return UWVM_BOUNDED_INVALID;
  if(*offset>n||n-*offset<8)return UWVM_BOUNDED_INVALID;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return UWVM_BOUNDED_INVALID;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return UWVM_BOUNDED_INVALID;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->unknown,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<8)return UWVM_BOUNDED_INVALID;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->acquisition_sec,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->acquisition_nsec,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.size>32u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>128u||value->joint_names.data[i].size>n-*offset)return UWVM_BOUNDED_INVALID;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->joint_position.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_position.size>32u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->joint_position.size;++i){
  if(*offset>n||n-*offset<8)return UWVM_BOUNDED_INVALID;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->joint_position.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->joint_velocity.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_velocity.size>32u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->joint_velocity.size;++i){
  if(*offset>n||n-*offset<8)return UWVM_BOUNDED_INVALID;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->joint_velocity.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->joint_load.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_load.size>32u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->joint_load.size;++i){
  if(*offset>n||n-*offset<8)return UWVM_BOUNDED_INVALID;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->joint_load.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->motor_power_state,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->native_proto.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->native_proto.size>16384u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->native_proto.size;++i){
  if(*offset>n||n-*offset<1)return UWVM_BOUNDED_INVALID;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->native_proto.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_measure(value,&measured);
}
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response* value) {return roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_read_status(p,n,offset,value)==0;}
/** No writes before full shape, nested quotas, exact size and disjoint destinations. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response const* value,void* output,size_t capacity,size_t* length) {size_t n=0,offset=0;uint32_t error;
  if(!length)return UWVM_BOUNDED_INVALID;error=roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_measure(value,&n);if(error)return error;
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;}
/** The compiler may reserve the full temporary stack frame even on preflight failure. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response* value) {size_t offset=0;uint32_t error;
  if(!value||(length&&!input)||length<57u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(length>21433u)return UWVM_BOUNDED_TOO_LARGE;
  {roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response next={0};error=roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_read_status((unsigned char const*)input,length,&offset,&next);if(error)return error;if(offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;}}
/** Complete ROS2 uwvm_spot_bridge/srv/Velocity_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field budget_ns; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t budget_ns;
  /** Declared field x_mps; canonical capacity is part of CODEC_IDENTITY. */
  double x_mps;
  /** Declared field y_mps; canonical capacity is part of CODEC_IDENTITY. */
  double y_mps;
  /** Declared field yaw_rps; canonical capacity is part of CODEC_IDENTITY. */
  double yaw_rps;
  /** Declared field duration_ns; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t duration_ns;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_VELOCITY_REQUEST_MAX_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_VELOCITY_REQUEST_MIN_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_VELOCITY_REQUEST_CODEC_IDENTITY "UWVMROS1_ffe3b1dec91a25cd84d221ba4ebeae576bd7d6b96f258579c0b3d280cad79f8d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x_mps,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y_mps,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->yaw_rps,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->budget_ns,8);*offset+=8;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x_mps,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y_mps,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->yaw_rps,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->duration_ns,8);*offset+=8;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->budget_ns,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->x_mps,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->y_mps,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->yaw_rps,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->duration_ns,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request* value) {
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<56u||length>56u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_spot_bridge/srv/Velocity_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response {
  /** Declared field token; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t token;
  /** Declared field nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t nonce;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t error;
  /** Declared field unknown; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t unknown;
  /** Declared field native_accepted; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t native_accepted;
  /** Declared field native_command_id; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t native_command_id;
} roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response;
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_VELOCITY_RESPONSE_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_VELOCITY_RESPONSE_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_UWVM_SPOT_BRIDGE_SRV_VELOCITY_RESPONSE_CODEC_IDENTITY "UWVMROS1_031f37beeb5a03c4e65d4be8fd70bc680408d110a020e066bea593d86498401a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_valid(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response const* value) {
  if(!value)return 0;
  if(value->unknown>1)return 0;
  if(value->native_accepted>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_wire_size(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=4;
  n+=1;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_write_fields(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->token,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->error,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->unknown,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->native_accepted,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->native_command_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->token,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->unknown,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->native_accepted,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->native_command_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_encode(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response* value) {
  roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
