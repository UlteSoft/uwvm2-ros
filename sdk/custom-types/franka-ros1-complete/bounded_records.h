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
/** Complete ROS1 franka_example_controllers/JointTorqueComparison value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison {
  /** Declared field tau_error; canonical capacity is part of CODEC_IDENTITY. */
  double tau_error[7];
  /** Declared field tau_commanded; canonical capacity is part of CODEC_IDENTITY. */
  double tau_commanded[7];
  /** Declared field tau_measured; canonical capacity is part of CODEC_IDENTITY. */
  double tau_measured[7];
  /** Declared field root_mean_square_error; canonical capacity is part of CODEC_IDENTITY. */
  double root_mean_square_error;
} roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison;
#define ROSWASMC_BOUNDED_1_FRANKA_EXAMPLE_CONTROLLERS_JOINTTORQUECOMPARISON_MAX_WIRE_BYTES 176u
#define ROSWASMC_BOUNDED_1_FRANKA_EXAMPLE_CONTROLLERS_JOINTTORQUECOMPARISON_MIN_WIRE_BYTES 176u
#define ROSWASMC_BOUNDED_1_FRANKA_EXAMPLE_CONTROLLERS_JOINTTORQUECOMPARISON_CODEC_IDENTITY "UWVMROS1_dde6eaf404002642e64af59eb5367948b00bc07c0b2fd8a57e359e9df3084254"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_valid(roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_error[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_commanded[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_measured[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->root_mean_square_error,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_wire_size(roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison const* value) {size_t n=0;
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_write_fields(roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_error[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_commanded[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_measured[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->root_mean_square_error,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison* value) {
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tau_error[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tau_commanded[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tau_measured[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->root_mean_square_error,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_encode(roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_decode(void const* input,size_t length,roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison* value) {
  roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison next={0};size_t offset=0;if(!value||(length&&!input)||length<176u||length>176u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/GraspEpsilon value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_GraspEpsilon {
  /** Declared field inner; canonical capacity is part of CODEC_IDENTITY. */
  double inner;
  /** Declared field outer; canonical capacity is part of CODEC_IDENTITY. */
  double outer;
} roswasmc_bounded_1_franka_gripper_GraspEpsilon;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPEPSILON_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPEPSILON_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPEPSILON_CODEC_IDENTITY "UWVMROS1_538ac801e9874841f232909d2d9a16893a8d29c8380cd623c00c408690f978f9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_GraspEpsilon_valid(roswasmc_bounded_1_franka_gripper_GraspEpsilon const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->inner,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->outer,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_GraspEpsilon_wire_size(roswasmc_bounded_1_franka_gripper_GraspEpsilon const* value) {size_t n=0;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_GraspEpsilon_write_fields(roswasmc_bounded_1_franka_gripper_GraspEpsilon const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->inner,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->outer,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_GraspEpsilon_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_GraspEpsilon* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->inner,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->outer,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_franka_gripper_GraspEpsilon_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_GraspEpsilon_encode(roswasmc_bounded_1_franka_gripper_GraspEpsilon const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_GraspEpsilon_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_GraspEpsilon_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_GraspEpsilon_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_GraspEpsilon_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_GraspEpsilon* value) {
  roswasmc_bounded_1_franka_gripper_GraspEpsilon next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_GraspEpsilon_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/GraspGoal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_GraspGoal {
  /** Declared field width; canonical capacity is part of CODEC_IDENTITY. */
  double width;
  /** Declared field epsilon; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_gripper_GraspEpsilon epsilon;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  double speed;
  /** Declared field force; canonical capacity is part of CODEC_IDENTITY. */
  double force;
} roswasmc_bounded_1_franka_gripper_GraspGoal;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPGOAL_MAX_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPGOAL_MIN_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPGOAL_CODEC_IDENTITY "UWVMROS1_d3dd0d3f47e49e06a95d907a6219335b35234749518ab645e102257595ed7bcf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_GraspGoal_valid(roswasmc_bounded_1_franka_gripper_GraspGoal const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->width,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(!roswasmc_bounded_1_franka_gripper_GraspEpsilon_valid(&value->epsilon))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->force,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_GraspGoal_wire_size(roswasmc_bounded_1_franka_gripper_GraspGoal const* value) {size_t n=0;
  n+=8;
  n+=roswasmc_bounded_1_franka_gripper_GraspEpsilon_wire_size(&value->epsilon);
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_GraspGoal_write_fields(roswasmc_bounded_1_franka_gripper_GraspGoal const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->width,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  roswasmc_bounded_1_franka_gripper_GraspEpsilon_write_fields(&value->epsilon,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->force,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_GraspGoal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_GraspGoal* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->width,&bits,8);}*offset+=8;
  if(!roswasmc_bounded_1_franka_gripper_GraspEpsilon_read_fields(p,n,offset,&value->epsilon))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->speed,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->force,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_franka_gripper_GraspGoal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_GraspGoal_encode(roswasmc_bounded_1_franka_gripper_GraspGoal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_GraspGoal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_GraspGoal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_GraspGoal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_GraspGoal_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_GraspGoal* value) {
  roswasmc_bounded_1_franka_gripper_GraspGoal next={0};size_t offset=0;if(!value||(length&&!input)||length<40u||length>40u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_GraspGoal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_gripper/GraspResult.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_gripper_GraspResult_error_text;
/** Complete ROS1 franka_gripper/GraspResult value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_GraspResult {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_gripper_GraspResult_error_text error;
} roswasmc_bounded_1_franka_gripper_GraspResult;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPRESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPRESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPRESULT_CODEC_IDENTITY "UWVMROS1_9ce1e4f95f16fd6c1603f0b30a9fcc33e4f8436cdb96710ea91eb20b38a1e02e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_GraspResult_valid(roswasmc_bounded_1_franka_gripper_GraspResult const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_GraspResult_wire_size(roswasmc_bounded_1_franka_gripper_GraspResult const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_GraspResult_write_fields(roswasmc_bounded_1_franka_gripper_GraspResult const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_GraspResult_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_GraspResult* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_gripper_GraspResult_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_GraspResult_encode(roswasmc_bounded_1_franka_gripper_GraspResult const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_GraspResult_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_GraspResult_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_GraspResult_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_GraspResult_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_GraspResult* value) {
  roswasmc_bounded_1_franka_gripper_GraspResult next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_GraspResult_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/GraspFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_GraspFeedback {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_gripper_GraspFeedback;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPFEEDBACK_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPFEEDBACK_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_GRASPFEEDBACK_CODEC_IDENTITY "UWVMROS1_24ca23625df0bd9fb984e65c8ecd7a3af162a3db00c15951225df5e18d3a4d0d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_GraspFeedback_valid(roswasmc_bounded_1_franka_gripper_GraspFeedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_GraspFeedback_wire_size(roswasmc_bounded_1_franka_gripper_GraspFeedback const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_GraspFeedback_write_fields(roswasmc_bounded_1_franka_gripper_GraspFeedback const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_GraspFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_GraspFeedback* value) {
  return roswasmc_bounded_1_franka_gripper_GraspFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_GraspFeedback_encode(roswasmc_bounded_1_franka_gripper_GraspFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_GraspFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_GraspFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_GraspFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_GraspFeedback_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_GraspFeedback* value) {
  roswasmc_bounded_1_franka_gripper_GraspFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_GraspFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/HomingGoal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_HomingGoal {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_gripper_HomingGoal;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGGOAL_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGGOAL_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGGOAL_CODEC_IDENTITY "UWVMROS1_4b4f861fa9ff44afda4217cf613720e90353b0d43c1fc3425d5937157353910f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_HomingGoal_valid(roswasmc_bounded_1_franka_gripper_HomingGoal const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_HomingGoal_wire_size(roswasmc_bounded_1_franka_gripper_HomingGoal const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_HomingGoal_write_fields(roswasmc_bounded_1_franka_gripper_HomingGoal const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_HomingGoal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_HomingGoal* value) {
  return roswasmc_bounded_1_franka_gripper_HomingGoal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_HomingGoal_encode(roswasmc_bounded_1_franka_gripper_HomingGoal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_HomingGoal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_HomingGoal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_HomingGoal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_HomingGoal_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_HomingGoal* value) {
  roswasmc_bounded_1_franka_gripper_HomingGoal next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_HomingGoal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_gripper/HomingResult.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_gripper_HomingResult_error_text;
/** Complete ROS1 franka_gripper/HomingResult value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_HomingResult {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_gripper_HomingResult_error_text error;
} roswasmc_bounded_1_franka_gripper_HomingResult;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGRESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGRESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGRESULT_CODEC_IDENTITY "UWVMROS1_1ddf202b4e7dbf100d304b4eb32aa0ce8dca6ffadcf720820f2d7932af056477"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_HomingResult_valid(roswasmc_bounded_1_franka_gripper_HomingResult const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_HomingResult_wire_size(roswasmc_bounded_1_franka_gripper_HomingResult const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_HomingResult_write_fields(roswasmc_bounded_1_franka_gripper_HomingResult const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_HomingResult_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_HomingResult* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_gripper_HomingResult_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_HomingResult_encode(roswasmc_bounded_1_franka_gripper_HomingResult const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_HomingResult_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_HomingResult_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_HomingResult_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_HomingResult_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_HomingResult* value) {
  roswasmc_bounded_1_franka_gripper_HomingResult next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_HomingResult_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/HomingFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_HomingFeedback {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_gripper_HomingFeedback;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGFEEDBACK_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGFEEDBACK_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_HOMINGFEEDBACK_CODEC_IDENTITY "UWVMROS1_4a1118d8ab544b2bc99ff878356aae49b4d3ecb4cb5ba7e2f93f7c0c7a29512e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_HomingFeedback_valid(roswasmc_bounded_1_franka_gripper_HomingFeedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_HomingFeedback_wire_size(roswasmc_bounded_1_franka_gripper_HomingFeedback const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_HomingFeedback_write_fields(roswasmc_bounded_1_franka_gripper_HomingFeedback const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_HomingFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_HomingFeedback* value) {
  return roswasmc_bounded_1_franka_gripper_HomingFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_HomingFeedback_encode(roswasmc_bounded_1_franka_gripper_HomingFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_HomingFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_HomingFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_HomingFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_HomingFeedback_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_HomingFeedback* value) {
  roswasmc_bounded_1_franka_gripper_HomingFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_HomingFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/MoveGoal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_MoveGoal {
  /** Declared field width; canonical capacity is part of CODEC_IDENTITY. */
  double width;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  double speed;
} roswasmc_bounded_1_franka_gripper_MoveGoal;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVEGOAL_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVEGOAL_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVEGOAL_CODEC_IDENTITY "UWVMROS1_c83296f2540854e78cc159691c229f5d50267b3fdb31789e325a55fcd7d6830a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_MoveGoal_valid(roswasmc_bounded_1_franka_gripper_MoveGoal const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->width,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_MoveGoal_wire_size(roswasmc_bounded_1_franka_gripper_MoveGoal const* value) {size_t n=0;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_MoveGoal_write_fields(roswasmc_bounded_1_franka_gripper_MoveGoal const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->width,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_MoveGoal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_MoveGoal* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->width,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->speed,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_franka_gripper_MoveGoal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_MoveGoal_encode(roswasmc_bounded_1_franka_gripper_MoveGoal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_MoveGoal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_MoveGoal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_MoveGoal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_MoveGoal_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_MoveGoal* value) {
  roswasmc_bounded_1_franka_gripper_MoveGoal next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_MoveGoal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_gripper/MoveResult.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_gripper_MoveResult_error_text;
/** Complete ROS1 franka_gripper/MoveResult value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_MoveResult {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_gripper_MoveResult_error_text error;
} roswasmc_bounded_1_franka_gripper_MoveResult;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVERESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVERESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVERESULT_CODEC_IDENTITY "UWVMROS1_527adf0720dabcc91866a0858cddfbfae1ed4203d4eb34a3965ebe085d0ac3b3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_MoveResult_valid(roswasmc_bounded_1_franka_gripper_MoveResult const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_MoveResult_wire_size(roswasmc_bounded_1_franka_gripper_MoveResult const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_MoveResult_write_fields(roswasmc_bounded_1_franka_gripper_MoveResult const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_MoveResult_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_MoveResult* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_gripper_MoveResult_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_MoveResult_encode(roswasmc_bounded_1_franka_gripper_MoveResult const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_MoveResult_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_MoveResult_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_MoveResult_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_MoveResult_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_MoveResult* value) {
  roswasmc_bounded_1_franka_gripper_MoveResult next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_MoveResult_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/MoveFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_MoveFeedback {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_gripper_MoveFeedback;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVEFEEDBACK_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVEFEEDBACK_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_MOVEFEEDBACK_CODEC_IDENTITY "UWVMROS1_8e02ca1d66ae38a5d71ca5f708aa2c57bb3df573d37b692e24986dba384c5f80"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_MoveFeedback_valid(roswasmc_bounded_1_franka_gripper_MoveFeedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_MoveFeedback_wire_size(roswasmc_bounded_1_franka_gripper_MoveFeedback const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_MoveFeedback_write_fields(roswasmc_bounded_1_franka_gripper_MoveFeedback const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_MoveFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_MoveFeedback* value) {
  return roswasmc_bounded_1_franka_gripper_MoveFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_MoveFeedback_encode(roswasmc_bounded_1_franka_gripper_MoveFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_MoveFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_MoveFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_MoveFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_MoveFeedback_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_MoveFeedback* value) {
  roswasmc_bounded_1_franka_gripper_MoveFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_MoveFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/StopGoal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_StopGoal {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_gripper_StopGoal;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPGOAL_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPGOAL_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPGOAL_CODEC_IDENTITY "UWVMROS1_ecb09ccc03cb2140a457a11c174a614392ce1f9263e46c5b82427a99f7ff0245"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_StopGoal_valid(roswasmc_bounded_1_franka_gripper_StopGoal const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_StopGoal_wire_size(roswasmc_bounded_1_franka_gripper_StopGoal const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_StopGoal_write_fields(roswasmc_bounded_1_franka_gripper_StopGoal const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_StopGoal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_StopGoal* value) {
  return roswasmc_bounded_1_franka_gripper_StopGoal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_StopGoal_encode(roswasmc_bounded_1_franka_gripper_StopGoal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_StopGoal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_StopGoal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_StopGoal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_StopGoal_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_StopGoal* value) {
  roswasmc_bounded_1_franka_gripper_StopGoal next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_StopGoal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_gripper/StopResult.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_gripper_StopResult_error_text;
/** Complete ROS1 franka_gripper/StopResult value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_StopResult {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_gripper_StopResult_error_text error;
} roswasmc_bounded_1_franka_gripper_StopResult;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPRESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPRESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPRESULT_CODEC_IDENTITY "UWVMROS1_343e16d9b839b61ed8f8c1ce93005cb3a21511a220095200eb075dfb62a40c61"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_StopResult_valid(roswasmc_bounded_1_franka_gripper_StopResult const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_StopResult_wire_size(roswasmc_bounded_1_franka_gripper_StopResult const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_StopResult_write_fields(roswasmc_bounded_1_franka_gripper_StopResult const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_StopResult_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_StopResult* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_gripper_StopResult_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_StopResult_encode(roswasmc_bounded_1_franka_gripper_StopResult const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_StopResult_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_StopResult_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_StopResult_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_StopResult_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_StopResult* value) {
  roswasmc_bounded_1_franka_gripper_StopResult next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_StopResult_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_gripper/StopFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_gripper_StopFeedback {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_gripper_StopFeedback;
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPFEEDBACK_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPFEEDBACK_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_GRIPPER_STOPFEEDBACK_CODEC_IDENTITY "UWVMROS1_62b637081e95f59df491f71b0ca84394a7e1f36137ca389a8f6d0d3a0f19038d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_gripper_StopFeedback_valid(roswasmc_bounded_1_franka_gripper_StopFeedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_gripper_StopFeedback_wire_size(roswasmc_bounded_1_franka_gripper_StopFeedback const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_gripper_StopFeedback_write_fields(roswasmc_bounded_1_franka_gripper_StopFeedback const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_gripper_StopFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_gripper_StopFeedback* value) {
  return roswasmc_bounded_1_franka_gripper_StopFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_StopFeedback_encode(roswasmc_bounded_1_franka_gripper_StopFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_gripper_StopFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_gripper_StopFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_gripper_StopFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_gripper_StopFeedback_decode(void const* input,size_t length,roswasmc_bounded_1_franka_gripper_StopFeedback* value) {
  roswasmc_bounded_1_franka_gripper_StopFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_gripper_StopFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/ErrorRecoveryGoal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYGOAL_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYGOAL_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYGOAL_CODEC_IDENTITY "UWVMROS1_b4f11555bc4919bf59aa548bd21046bdc7b1e2ed5cce6dc46298eb7486ef7b64"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_valid(roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_wire_size(roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_write_fields(roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal* value) {
  return roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_encode(roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal* value) {
  roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/ErrorRecoveryResult value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYRESULT_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYRESULT_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYRESULT_CODEC_IDENTITY "UWVMROS1_6f06297f59f06dc46e3f983b72101ae3c25cd054bca44cea1f6dd11a9aaa7794"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_valid(roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_wire_size(roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_write_fields(roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult* value) {
  return roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_encode(roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult* value) {
  roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/ErrorRecoveryFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYFEEDBACK_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYFEEDBACK_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORRECOVERYFEEDBACK_CODEC_IDENTITY "UWVMROS1_3faee6d8afdc0524598cec0fe30fd51bd723e8e44849f035368723e59be87a21"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_valid(roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_wire_size(roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_write_fields(roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback* value) {
  return roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_encode(roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback* value) {
  roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/Errors value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_Errors {
  /** Declared field joint_position_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_position_limits_violation;
  /** Declared field cartesian_position_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_position_limits_violation;
  /** Declared field self_collision_avoidance_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t self_collision_avoidance_violation;
  /** Declared field joint_velocity_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_velocity_violation;
  /** Declared field cartesian_velocity_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_velocity_violation;
  /** Declared field force_control_safety_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t force_control_safety_violation;
  /** Declared field joint_reflex; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_reflex;
  /** Declared field cartesian_reflex; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_reflex;
  /** Declared field max_goal_pose_deviation_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t max_goal_pose_deviation_violation;
  /** Declared field max_path_pose_deviation_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t max_path_pose_deviation_violation;
  /** Declared field cartesian_velocity_profile_safety_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_velocity_profile_safety_violation;
  /** Declared field joint_position_motion_generator_start_pose_invalid; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_position_motion_generator_start_pose_invalid;
  /** Declared field joint_motion_generator_position_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_motion_generator_position_limits_violation;
  /** Declared field joint_motion_generator_velocity_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_motion_generator_velocity_limits_violation;
  /** Declared field joint_motion_generator_velocity_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_motion_generator_velocity_discontinuity;
  /** Declared field joint_motion_generator_acceleration_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_motion_generator_acceleration_discontinuity;
  /** Declared field cartesian_position_motion_generator_start_pose_invalid; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_position_motion_generator_start_pose_invalid;
  /** Declared field cartesian_motion_generator_elbow_limit_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_elbow_limit_violation;
  /** Declared field cartesian_motion_generator_velocity_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_velocity_limits_violation;
  /** Declared field cartesian_motion_generator_velocity_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_velocity_discontinuity;
  /** Declared field cartesian_motion_generator_acceleration_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_acceleration_discontinuity;
  /** Declared field cartesian_motion_generator_elbow_sign_inconsistent; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_elbow_sign_inconsistent;
  /** Declared field cartesian_motion_generator_start_elbow_invalid; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_start_elbow_invalid;
  /** Declared field cartesian_motion_generator_joint_position_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_joint_position_limits_violation;
  /** Declared field cartesian_motion_generator_joint_velocity_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_joint_velocity_limits_violation;
  /** Declared field cartesian_motion_generator_joint_velocity_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_joint_velocity_discontinuity;
  /** Declared field cartesian_motion_generator_joint_acceleration_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_joint_acceleration_discontinuity;
  /** Declared field cartesian_position_motion_generator_invalid_frame; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_position_motion_generator_invalid_frame;
  /** Declared field force_controller_desired_force_tolerance_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t force_controller_desired_force_tolerance_violation;
  /** Declared field controller_torque_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t controller_torque_discontinuity;
  /** Declared field start_elbow_sign_inconsistent; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t start_elbow_sign_inconsistent;
  /** Declared field communication_constraints_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t communication_constraints_violation;
  /** Declared field power_limit_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t power_limit_violation;
  /** Declared field joint_p2p_insufficient_torque_for_planning; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_p2p_insufficient_torque_for_planning;
  /** Declared field tau_j_range_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t tau_j_range_violation;
  /** Declared field instability_detected; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t instability_detected;
  /** Declared field joint_move_in_wrong_direction; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_move_in_wrong_direction;
  /** Declared field cartesian_spline_motion_generator_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_spline_motion_generator_violation;
  /** Declared field joint_via_motion_generator_planning_joint_limit_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_via_motion_generator_planning_joint_limit_violation;
  /** Declared field base_acceleration_initialization_timeout; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t base_acceleration_initialization_timeout;
  /** Declared field base_acceleration_invalid_reading; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t base_acceleration_invalid_reading;
} roswasmc_bounded_1_franka_msgs_Errors;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORS_MAX_WIRE_BYTES 41u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORS_MIN_WIRE_BYTES 41u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_ERRORS_CODEC_IDENTITY "UWVMROS1_80e3d1157d53bf80d4db3621f9f22b35a1ef665382974599ff016709c6a629ca"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_Errors_valid(roswasmc_bounded_1_franka_msgs_Errors const* value) {
  if(!value)return 0;
  if(value->joint_position_limits_violation>1)return 0;
  if(value->cartesian_position_limits_violation>1)return 0;
  if(value->self_collision_avoidance_violation>1)return 0;
  if(value->joint_velocity_violation>1)return 0;
  if(value->cartesian_velocity_violation>1)return 0;
  if(value->force_control_safety_violation>1)return 0;
  if(value->joint_reflex>1)return 0;
  if(value->cartesian_reflex>1)return 0;
  if(value->max_goal_pose_deviation_violation>1)return 0;
  if(value->max_path_pose_deviation_violation>1)return 0;
  if(value->cartesian_velocity_profile_safety_violation>1)return 0;
  if(value->joint_position_motion_generator_start_pose_invalid>1)return 0;
  if(value->joint_motion_generator_position_limits_violation>1)return 0;
  if(value->joint_motion_generator_velocity_limits_violation>1)return 0;
  if(value->joint_motion_generator_velocity_discontinuity>1)return 0;
  if(value->joint_motion_generator_acceleration_discontinuity>1)return 0;
  if(value->cartesian_position_motion_generator_start_pose_invalid>1)return 0;
  if(value->cartesian_motion_generator_elbow_limit_violation>1)return 0;
  if(value->cartesian_motion_generator_velocity_limits_violation>1)return 0;
  if(value->cartesian_motion_generator_velocity_discontinuity>1)return 0;
  if(value->cartesian_motion_generator_acceleration_discontinuity>1)return 0;
  if(value->cartesian_motion_generator_elbow_sign_inconsistent>1)return 0;
  if(value->cartesian_motion_generator_start_elbow_invalid>1)return 0;
  if(value->cartesian_motion_generator_joint_position_limits_violation>1)return 0;
  if(value->cartesian_motion_generator_joint_velocity_limits_violation>1)return 0;
  if(value->cartesian_motion_generator_joint_velocity_discontinuity>1)return 0;
  if(value->cartesian_motion_generator_joint_acceleration_discontinuity>1)return 0;
  if(value->cartesian_position_motion_generator_invalid_frame>1)return 0;
  if(value->force_controller_desired_force_tolerance_violation>1)return 0;
  if(value->controller_torque_discontinuity>1)return 0;
  if(value->start_elbow_sign_inconsistent>1)return 0;
  if(value->communication_constraints_violation>1)return 0;
  if(value->power_limit_violation>1)return 0;
  if(value->joint_p2p_insufficient_torque_for_planning>1)return 0;
  if(value->tau_j_range_violation>1)return 0;
  if(value->instability_detected>1)return 0;
  if(value->joint_move_in_wrong_direction>1)return 0;
  if(value->cartesian_spline_motion_generator_violation>1)return 0;
  if(value->joint_via_motion_generator_planning_joint_limit_violation>1)return 0;
  if(value->base_acceleration_initialization_timeout>1)return 0;
  if(value->base_acceleration_invalid_reading>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_Errors_wire_size(roswasmc_bounded_1_franka_msgs_Errors const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
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
static inline void roswasmc_bounded_1_franka_msgs_Errors_write_fields(roswasmc_bounded_1_franka_msgs_Errors const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_position_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_position_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->self_collision_avoidance_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_velocity_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_velocity_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->force_control_safety_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_reflex,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_reflex,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->max_goal_pose_deviation_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->max_path_pose_deviation_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_velocity_profile_safety_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_position_motion_generator_start_pose_invalid,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_motion_generator_position_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_motion_generator_velocity_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_motion_generator_velocity_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_motion_generator_acceleration_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_position_motion_generator_start_pose_invalid,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_elbow_limit_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_velocity_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_velocity_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_acceleration_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_elbow_sign_inconsistent,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_start_elbow_invalid,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_joint_position_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_joint_velocity_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_joint_velocity_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_joint_acceleration_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_position_motion_generator_invalid_frame,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->force_controller_desired_force_tolerance_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->controller_torque_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->start_elbow_sign_inconsistent,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->communication_constraints_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->power_limit_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_p2p_insufficient_torque_for_planning,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->tau_j_range_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->instability_detected,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_move_in_wrong_direction,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_spline_motion_generator_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_via_motion_generator_planning_joint_limit_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->base_acceleration_initialization_timeout,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->base_acceleration_invalid_reading,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_Errors_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_Errors* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_position_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_position_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->self_collision_avoidance_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_velocity_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_velocity_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->force_control_safety_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_reflex,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_reflex,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->max_goal_pose_deviation_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->max_path_pose_deviation_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_velocity_profile_safety_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_position_motion_generator_start_pose_invalid,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_motion_generator_position_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_motion_generator_velocity_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_motion_generator_velocity_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_motion_generator_acceleration_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_position_motion_generator_start_pose_invalid,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_elbow_limit_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_velocity_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_velocity_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_acceleration_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_elbow_sign_inconsistent,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_start_elbow_invalid,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_joint_position_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_joint_velocity_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_joint_velocity_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_joint_acceleration_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_position_motion_generator_invalid_frame,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->force_controller_desired_force_tolerance_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->controller_torque_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->start_elbow_sign_inconsistent,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->communication_constraints_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->power_limit_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_p2p_insufficient_torque_for_planning,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->tau_j_range_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->instability_detected,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_move_in_wrong_direction,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_spline_motion_generator_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_via_motion_generator_planning_joint_limit_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->base_acceleration_initialization_timeout,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->base_acceleration_invalid_reading,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_franka_msgs_Errors_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_Errors_encode(roswasmc_bounded_1_franka_msgs_Errors const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_Errors_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_Errors_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_Errors_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_Errors_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_Errors* value) {
  roswasmc_bounded_1_franka_msgs_Errors next={0};size_t offset=0;if(!value||(length&&!input)||length<41u||length>41u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_Errors_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS1 franka_msgs/FrankaState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_FrankaState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field cartesian_collision; canonical capacity is part of CODEC_IDENTITY. */
  double cartesian_collision[6];
  /** Declared field cartesian_contact; canonical capacity is part of CODEC_IDENTITY. */
  double cartesian_contact[6];
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  double q[7];
  /** Declared field q_d; canonical capacity is part of CODEC_IDENTITY. */
  double q_d[7];
  /** Declared field dq; canonical capacity is part of CODEC_IDENTITY. */
  double dq[7];
  /** Declared field dq_d; canonical capacity is part of CODEC_IDENTITY. */
  double dq_d[7];
  /** Declared field ddq_d; canonical capacity is part of CODEC_IDENTITY. */
  double ddq_d[7];
  /** Declared field theta; canonical capacity is part of CODEC_IDENTITY. */
  double theta[7];
  /** Declared field dtheta; canonical capacity is part of CODEC_IDENTITY. */
  double dtheta[7];
  /** Declared field tau_J; canonical capacity is part of CODEC_IDENTITY. */
  double tau_J[7];
  /** Declared field dtau_J; canonical capacity is part of CODEC_IDENTITY. */
  double dtau_J[7];
  /** Declared field tau_J_d; canonical capacity is part of CODEC_IDENTITY. */
  double tau_J_d[7];
  /** Declared field K_F_ext_hat_K; canonical capacity is part of CODEC_IDENTITY. */
  double K_F_ext_hat_K[6];
  /** Declared field elbow; canonical capacity is part of CODEC_IDENTITY. */
  double elbow[2];
  /** Declared field elbow_d; canonical capacity is part of CODEC_IDENTITY. */
  double elbow_d[2];
  /** Declared field elbow_c; canonical capacity is part of CODEC_IDENTITY. */
  double elbow_c[2];
  /** Declared field delbow_c; canonical capacity is part of CODEC_IDENTITY. */
  double delbow_c[2];
  /** Declared field ddelbow_c; canonical capacity is part of CODEC_IDENTITY. */
  double ddelbow_c[2];
  /** Declared field joint_collision; canonical capacity is part of CODEC_IDENTITY. */
  double joint_collision[7];
  /** Declared field joint_contact; canonical capacity is part of CODEC_IDENTITY. */
  double joint_contact[7];
  /** Declared field O_F_ext_hat_K; canonical capacity is part of CODEC_IDENTITY. */
  double O_F_ext_hat_K[6];
  /** Declared field O_dP_EE_d; canonical capacity is part of CODEC_IDENTITY. */
  double O_dP_EE_d[6];
  /** Declared field O_ddP_O; canonical capacity is part of CODEC_IDENTITY. */
  double O_ddP_O[3];
  /** Declared field O_dP_EE_c; canonical capacity is part of CODEC_IDENTITY. */
  double O_dP_EE_c[6];
  /** Declared field O_ddP_EE_c; canonical capacity is part of CODEC_IDENTITY. */
  double O_ddP_EE_c[6];
  /** Declared field tau_ext_hat_filtered; canonical capacity is part of CODEC_IDENTITY. */
  double tau_ext_hat_filtered[7];
  /** Declared field m_ee; canonical capacity is part of CODEC_IDENTITY. */
  double m_ee;
  /** Declared field F_x_Cee; canonical capacity is part of CODEC_IDENTITY. */
  double F_x_Cee[3];
  /** Declared field I_ee; canonical capacity is part of CODEC_IDENTITY. */
  double I_ee[9];
  /** Declared field m_load; canonical capacity is part of CODEC_IDENTITY. */
  double m_load;
  /** Declared field F_x_Cload; canonical capacity is part of CODEC_IDENTITY. */
  double F_x_Cload[3];
  /** Declared field I_load; canonical capacity is part of CODEC_IDENTITY. */
  double I_load[9];
  /** Declared field m_total; canonical capacity is part of CODEC_IDENTITY. */
  double m_total;
  /** Declared field F_x_Ctotal; canonical capacity is part of CODEC_IDENTITY. */
  double F_x_Ctotal[3];
  /** Declared field I_total; canonical capacity is part of CODEC_IDENTITY. */
  double I_total[9];
  /** Declared field O_T_EE; canonical capacity is part of CODEC_IDENTITY. */
  double O_T_EE[16];
  /** Declared field O_T_EE_d; canonical capacity is part of CODEC_IDENTITY. */
  double O_T_EE_d[16];
  /** Declared field O_T_EE_c; canonical capacity is part of CODEC_IDENTITY. */
  double O_T_EE_c[16];
  /** Declared field F_T_EE; canonical capacity is part of CODEC_IDENTITY. */
  double F_T_EE[16];
  /** Declared field F_T_NE; canonical capacity is part of CODEC_IDENTITY. */
  double F_T_NE[16];
  /** Declared field NE_T_EE; canonical capacity is part of CODEC_IDENTITY. */
  double NE_T_EE[16];
  /** Declared field EE_T_K; canonical capacity is part of CODEC_IDENTITY. */
  double EE_T_K[16];
  /** Declared field time; canonical capacity is part of CODEC_IDENTITY. */
  double time;
  /** Declared field control_command_success_rate; canonical capacity is part of CODEC_IDENTITY. */
  double control_command_success_rate;
  /** Declared field robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t robot_mode;
  /** Declared field current_errors; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_Errors current_errors;
  /** Declared field last_motion_errors; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_Errors last_motion_errors;
} roswasmc_bounded_1_franka_msgs_FrankaState;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_MAX_WIRE_BYTES 2619u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_MIN_WIRE_BYTES 2491u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_CODEC_IDENTITY "UWVMROS1_79f22f36ce2c11f519ee29e65a248ec1b67240e27712b2667e9b803a2562ee71"
/** Original scalar constant ROBOT_MODE_OTHER=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_ROBOT_MODE_OTHER UINT32_C(0)
/** Original scalar constant ROBOT_MODE_IDLE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_ROBOT_MODE_IDLE UINT32_C(1)
/** Original scalar constant ROBOT_MODE_MOVE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_ROBOT_MODE_MOVE UINT32_C(2)
/** Original scalar constant ROBOT_MODE_GUIDING=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_ROBOT_MODE_GUIDING UINT32_C(3)
/** Original scalar constant ROBOT_MODE_REFLEX=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_ROBOT_MODE_REFLEX UINT32_C(4)
/** Original scalar constant ROBOT_MODE_USER_STOPPED=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_ROBOT_MODE_USER_STOPPED UINT32_C(5)
/** Original scalar constant ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY UINT32_C(6)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_FrankaState_valid(roswasmc_bounded_1_franka_msgs_FrankaState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->cartesian_collision[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->cartesian_contact[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->q[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->q_d[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dq[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dq_d[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ddq_d[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->theta[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dtheta[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_J[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dtau_J[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_J_d[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->K_F_ext_hat_K[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->elbow[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->elbow_d[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->elbow_c[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->delbow_c[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ddelbow_c[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_collision[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_contact[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_F_ext_hat_K[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_dP_EE_d[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_ddP_O[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_dP_EE_c[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_ddP_EE_c[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_ext_hat_filtered[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m_ee,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_x_Cee[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->I_ee[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m_load,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_x_Cload[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->I_load[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m_total,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_x_Ctotal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->I_total[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_T_EE[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_T_EE_d[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_T_EE_c[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_T_EE[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_T_NE[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->NE_T_EE[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->EE_T_K[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->time,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->control_command_success_rate,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(!roswasmc_bounded_1_franka_msgs_Errors_valid(&value->current_errors))return 0;
  if(!roswasmc_bounded_1_franka_msgs_Errors_valid(&value->last_motion_errors))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_FrankaState_wire_size(roswasmc_bounded_1_franka_msgs_FrankaState const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  n+=8;
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  n+=8;
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  n+=8;
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  n+=8;
  n+=8;
  n+=1;
  n+=roswasmc_bounded_1_franka_msgs_Errors_wire_size(&value->current_errors);
  n+=roswasmc_bounded_1_franka_msgs_Errors_wire_size(&value->last_motion_errors);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_FrankaState_write_fields(roswasmc_bounded_1_franka_msgs_FrankaState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->cartesian_collision[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->cartesian_contact[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->q[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->q_d[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dq[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dq_d[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ddq_d[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->theta[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dtheta[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_J[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dtau_J[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_J_d[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->K_F_ext_hat_K[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->elbow[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->elbow_d[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->elbow_c[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->delbow_c[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ddelbow_c[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_collision[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_contact[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_F_ext_hat_K[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_dP_EE_d[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_ddP_O[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_dP_EE_c[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_ddP_EE_c[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tau_ext_hat_filtered[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m_ee,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_x_Cee[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->I_ee[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m_load,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_x_Cload[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->I_load[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m_total,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_x_Ctotal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->I_total[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_T_EE[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_T_EE_d[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->O_T_EE_c[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_T_EE[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_T_NE[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->NE_T_EE[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->EE_T_K[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->time,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->control_command_success_rate,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->robot_mode,1);*offset+=1;
  roswasmc_bounded_1_franka_msgs_Errors_write_fields(&value->current_errors,p,offset);
  roswasmc_bounded_1_franka_msgs_Errors_write_fields(&value->last_motion_errors,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_FrankaState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_FrankaState* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->cartesian_collision[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->cartesian_contact[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->q[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->q_d[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->dq[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->dq_d[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ddq_d[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->theta[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->dtheta[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tau_J[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->dtau_J[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tau_J_d[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->K_F_ext_hat_K[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->elbow[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->elbow_d[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->elbow_c[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->delbow_c[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ddelbow_c[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->joint_collision[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->joint_contact[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->O_F_ext_hat_K[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->O_dP_EE_d[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->O_ddP_O[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->O_dP_EE_c[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->O_ddP_EE_c[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tau_ext_hat_filtered[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->m_ee,&bits,8);}*offset+=8;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->F_x_Cee[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->I_ee[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->m_load,&bits,8);}*offset+=8;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->F_x_Cload[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->I_load[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->m_total,&bits,8);}*offset+=8;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->F_x_Ctotal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->I_total[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->O_T_EE[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->O_T_EE_d[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->O_T_EE_c[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->F_T_EE[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->F_T_NE[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->NE_T_EE[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->EE_T_K[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->time,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->control_command_success_rate,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->robot_mode,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_1_franka_msgs_Errors_read_fields(p,n,offset,&value->current_errors))return 0;
  if(!roswasmc_bounded_1_franka_msgs_Errors_read_fields(p,n,offset,&value->last_motion_errors))return 0;
  return roswasmc_bounded_1_franka_msgs_FrankaState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_FrankaState_encode(roswasmc_bounded_1_franka_msgs_FrankaState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_FrankaState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_FrankaState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_FrankaState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_FrankaState_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_FrankaState* value) {
  roswasmc_bounded_1_franka_msgs_FrankaState next={0};size_t offset=0;if(!value||(length&&!input)||length<2491u||length>2619u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_FrankaState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/SetCartesianImpedanceRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest {
  /** Declared field cartesian_stiffness; canonical capacity is part of CODEC_IDENTITY. */
  double cartesian_stiffness[6];
} roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETCARTESIANIMPEDANCEREQUEST_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETCARTESIANIMPEDANCEREQUEST_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETCARTESIANIMPEDANCEREQUEST_CODEC_IDENTITY "UWVMROS1_ff604aae9ab1ef36b5af107966e64ee2115ee17618a4ece6f15997c889cacbdf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_valid(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->cartesian_stiffness[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_wire_size(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest const* value) {size_t n=0;
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_write_fields(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->cartesian_stiffness[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest* value) {
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->cartesian_stiffness[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_encode(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest* value) {
  roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/SetCartesianImpedanceResponse.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_error_text;
/** Complete ROS1 franka_msgs/SetCartesianImpedanceResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_error_text error;
} roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETCARTESIANIMPEDANCERESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETCARTESIANIMPEDANCERESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETCARTESIANIMPEDANCERESPONSE_CODEC_IDENTITY "UWVMROS1_b091d2fe98c84528460ba3d84524c38355e725c161456edf0034e62c6f63ab5b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_valid(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_wire_size(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_write_fields(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_encode(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse* value) {
  roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/SetEEFrameRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetEEFrameRequest {
  /** Declared field NE_T_EE; canonical capacity is part of CODEC_IDENTITY. */
  double NE_T_EE[16];
} roswasmc_bounded_1_franka_msgs_SetEEFrameRequest;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETEEFRAMEREQUEST_MAX_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETEEFRAMEREQUEST_MIN_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETEEFRAMEREQUEST_CODEC_IDENTITY "UWVMROS1_1bc740d3998a14dd6c0604d5024ebda2b4916a8497950841b14a8073c6c1ecfc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_valid(roswasmc_bounded_1_franka_msgs_SetEEFrameRequest const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->NE_T_EE[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_wire_size(roswasmc_bounded_1_franka_msgs_SetEEFrameRequest const* value) {size_t n=0;
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_write_fields(roswasmc_bounded_1_franka_msgs_SetEEFrameRequest const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->NE_T_EE[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetEEFrameRequest* value) {
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->NE_T_EE[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_encode(roswasmc_bounded_1_franka_msgs_SetEEFrameRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetEEFrameRequest* value) {
  roswasmc_bounded_1_franka_msgs_SetEEFrameRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<128u||length>128u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetEEFrameRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/SetEEFrameResponse.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_error_text;
/** Complete ROS1 franka_msgs/SetEEFrameResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetEEFrameResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_error_text error;
} roswasmc_bounded_1_franka_msgs_SetEEFrameResponse;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETEEFRAMERESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETEEFRAMERESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETEEFRAMERESPONSE_CODEC_IDENTITY "UWVMROS1_b40c9595d57d6ed52ea4eca78e21f2c3d81f5a0afaad2420511211931f7d1c67"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_valid(roswasmc_bounded_1_franka_msgs_SetEEFrameResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_wire_size(roswasmc_bounded_1_franka_msgs_SetEEFrameResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_write_fields(roswasmc_bounded_1_franka_msgs_SetEEFrameResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetEEFrameResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_encode(roswasmc_bounded_1_franka_msgs_SetEEFrameResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetEEFrameResponse* value) {
  roswasmc_bounded_1_franka_msgs_SetEEFrameResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetEEFrameResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/SetForceTorqueCollisionBehaviorRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest {
  /** Declared field lower_torque_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double lower_torque_thresholds_nominal[7];
  /** Declared field upper_torque_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double upper_torque_thresholds_nominal[7];
  /** Declared field lower_force_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double lower_force_thresholds_nominal[6];
  /** Declared field upper_force_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double upper_force_thresholds_nominal[6];
} roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFORCETORQUECOLLISIONBEHAVIORREQUEST_MAX_WIRE_BYTES 208u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFORCETORQUECOLLISIONBEHAVIORREQUEST_MIN_WIRE_BYTES 208u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFORCETORQUECOLLISIONBEHAVIORREQUEST_CODEC_IDENTITY "UWVMROS1_2513f0ea74bf6a871a85ce00c0f822891f2c0bed890b87e35d25a637aa62c4b5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_valid(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_wire_size(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest const* value) {size_t n=0;
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_write_fields(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest* value) {
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_torque_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_torque_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_force_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_force_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_encode(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest* value) {
  roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<208u||length>208u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/SetForceTorqueCollisionBehaviorResponse.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_error_text;
/** Complete ROS1 franka_msgs/SetForceTorqueCollisionBehaviorResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_error_text error;
} roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFORCETORQUECOLLISIONBEHAVIORRESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFORCETORQUECOLLISIONBEHAVIORRESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFORCETORQUECOLLISIONBEHAVIORRESPONSE_CODEC_IDENTITY "UWVMROS1_72418d73d504ae385d7ddce7e3cef859a104346dbfea7fc9da82e0f7e33ca2f5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_valid(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_wire_size(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_write_fields(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_encode(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse* value) {
  roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/SetFullCollisionBehaviorRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest {
  /** Declared field lower_torque_thresholds_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double lower_torque_thresholds_acceleration[7];
  /** Declared field upper_torque_thresholds_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double upper_torque_thresholds_acceleration[7];
  /** Declared field lower_torque_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double lower_torque_thresholds_nominal[7];
  /** Declared field upper_torque_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double upper_torque_thresholds_nominal[7];
  /** Declared field lower_force_thresholds_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double lower_force_thresholds_acceleration[6];
  /** Declared field upper_force_thresholds_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double upper_force_thresholds_acceleration[6];
  /** Declared field lower_force_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double lower_force_thresholds_nominal[6];
  /** Declared field upper_force_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double upper_force_thresholds_nominal[6];
} roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFULLCOLLISIONBEHAVIORREQUEST_MAX_WIRE_BYTES 416u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFULLCOLLISIONBEHAVIORREQUEST_MIN_WIRE_BYTES 416u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFULLCOLLISIONBEHAVIORREQUEST_CODEC_IDENTITY "UWVMROS1_47a86620ec5abdd6cc820b095d8f5f4214c1d9af00ac1ff0740137bbdda7217c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_valid(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_wire_size(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest const* value) {size_t n=0;
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_write_fields(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest* value) {
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_torque_thresholds_acceleration[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_torque_thresholds_acceleration[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_torque_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_torque_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_force_thresholds_acceleration[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_force_thresholds_acceleration[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_force_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_force_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_encode(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest* value) {
  roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<416u||length>416u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/SetFullCollisionBehaviorResponse.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_error_text;
/** Complete ROS1 franka_msgs/SetFullCollisionBehaviorResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_error_text error;
} roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFULLCOLLISIONBEHAVIORRESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFULLCOLLISIONBEHAVIORRESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETFULLCOLLISIONBEHAVIORRESPONSE_CODEC_IDENTITY "UWVMROS1_26f46e286c42d51a8ef45db7c8b3bf315fb02faf7b2be5d3741563d43169a0e2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_valid(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_wire_size(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_write_fields(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_encode(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse* value) {
  roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/SetJointImpedanceRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest {
  /** Declared field joint_stiffness; canonical capacity is part of CODEC_IDENTITY. */
  double joint_stiffness[7];
} roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETJOINTIMPEDANCEREQUEST_MAX_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETJOINTIMPEDANCEREQUEST_MIN_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETJOINTIMPEDANCEREQUEST_CODEC_IDENTITY "UWVMROS1_29b5419f698b39452d38fbc58a0e3c572b8d285b70c8acf7d326c440eb417c53"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_valid(roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_stiffness[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_wire_size(roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest const* value) {size_t n=0;
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_write_fields(roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_stiffness[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest* value) {
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->joint_stiffness[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_encode(roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest* value) {
  roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<56u||length>56u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/SetJointImpedanceResponse.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_error_text;
/** Complete ROS1 franka_msgs/SetJointImpedanceResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_error_text error;
} roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETJOINTIMPEDANCERESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETJOINTIMPEDANCERESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETJOINTIMPEDANCERESPONSE_CODEC_IDENTITY "UWVMROS1_1ae0cd7c4ad89de09ccffd6c4999435fa6965509e99a9adb98b7f3ebb3ea4c55"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_valid(roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_wire_size(roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_write_fields(roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_encode(roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse* value) {
  roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/SetKFrameRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetKFrameRequest {
  /** Declared field EE_T_K; canonical capacity is part of CODEC_IDENTITY. */
  double EE_T_K[16];
} roswasmc_bounded_1_franka_msgs_SetKFrameRequest;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETKFRAMEREQUEST_MAX_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETKFRAMEREQUEST_MIN_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETKFRAMEREQUEST_CODEC_IDENTITY "UWVMROS1_36afcdc6402259fb5a71ae1b27be0bb8623a64dd93caffa376adea7862c86446"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetKFrameRequest_valid(roswasmc_bounded_1_franka_msgs_SetKFrameRequest const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->EE_T_K[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetKFrameRequest_wire_size(roswasmc_bounded_1_franka_msgs_SetKFrameRequest const* value) {size_t n=0;
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetKFrameRequest_write_fields(roswasmc_bounded_1_franka_msgs_SetKFrameRequest const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->EE_T_K[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetKFrameRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetKFrameRequest* value) {
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->EE_T_K[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_franka_msgs_SetKFrameRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetKFrameRequest_encode(roswasmc_bounded_1_franka_msgs_SetKFrameRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetKFrameRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetKFrameRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetKFrameRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetKFrameRequest_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetKFrameRequest* value) {
  roswasmc_bounded_1_franka_msgs_SetKFrameRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<128u||length>128u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetKFrameRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/SetKFrameResponse.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_msgs_SetKFrameResponse_error_text;
/** Complete ROS1 franka_msgs/SetKFrameResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetKFrameResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_SetKFrameResponse_error_text error;
} roswasmc_bounded_1_franka_msgs_SetKFrameResponse;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETKFRAMERESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETKFRAMERESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETKFRAMERESPONSE_CODEC_IDENTITY "UWVMROS1_4ea77d3e827a9b9eaf3c3f9805f78c38bfe91789555d0469cffd4ff4266cd443"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetKFrameResponse_valid(roswasmc_bounded_1_franka_msgs_SetKFrameResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetKFrameResponse_wire_size(roswasmc_bounded_1_franka_msgs_SetKFrameResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetKFrameResponse_write_fields(roswasmc_bounded_1_franka_msgs_SetKFrameResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetKFrameResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetKFrameResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_msgs_SetKFrameResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetKFrameResponse_encode(roswasmc_bounded_1_franka_msgs_SetKFrameResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetKFrameResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetKFrameResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetKFrameResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetKFrameResponse_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetKFrameResponse* value) {
  roswasmc_bounded_1_franka_msgs_SetKFrameResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetKFrameResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 franka_msgs/SetLoadRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetLoadRequest {
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  double mass;
  /** Declared field F_x_center_load; canonical capacity is part of CODEC_IDENTITY. */
  double F_x_center_load[3];
  /** Declared field load_inertia; canonical capacity is part of CODEC_IDENTITY. */
  double load_inertia[9];
} roswasmc_bounded_1_franka_msgs_SetLoadRequest;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETLOADREQUEST_MAX_WIRE_BYTES 104u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETLOADREQUEST_MIN_WIRE_BYTES 104u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETLOADREQUEST_CODEC_IDENTITY "UWVMROS1_d7c08dd9105cb7b83cb251e80c7d2dd1e9f7bb2a43f614bff6d00d808cccbcb5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetLoadRequest_valid(roswasmc_bounded_1_franka_msgs_SetLoadRequest const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->mass,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_x_center_load[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->load_inertia[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetLoadRequest_wire_size(roswasmc_bounded_1_franka_msgs_SetLoadRequest const* value) {size_t n=0;
  n+=8;
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetLoadRequest_write_fields(roswasmc_bounded_1_franka_msgs_SetLoadRequest const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->mass,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->F_x_center_load[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->load_inertia[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetLoadRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetLoadRequest* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->mass,&bits,8);}*offset+=8;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->F_x_center_load[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->load_inertia[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_franka_msgs_SetLoadRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetLoadRequest_encode(roswasmc_bounded_1_franka_msgs_SetLoadRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetLoadRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetLoadRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetLoadRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetLoadRequest_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetLoadRequest* value) {
  roswasmc_bounded_1_franka_msgs_SetLoadRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<104u||length>104u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetLoadRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/SetLoadResponse.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_franka_msgs_SetLoadResponse_error_text;
/** Complete ROS1 franka_msgs/SetLoadResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_franka_msgs_SetLoadResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_franka_msgs_SetLoadResponse_error_text error;
} roswasmc_bounded_1_franka_msgs_SetLoadResponse;
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETLOADRESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETLOADRESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_FRANKA_MSGS_SETLOADRESPONSE_CODEC_IDENTITY "UWVMROS1_f18952407bf9e2813deeaa3734a781e90b74cb79892919c1218812ad71255cf3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_franka_msgs_SetLoadResponse_valid(roswasmc_bounded_1_franka_msgs_SetLoadResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_franka_msgs_SetLoadResponse_wire_size(roswasmc_bounded_1_franka_msgs_SetLoadResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_franka_msgs_SetLoadResponse_write_fields(roswasmc_bounded_1_franka_msgs_SetLoadResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_franka_msgs_SetLoadResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_franka_msgs_SetLoadResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_1_franka_msgs_SetLoadResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetLoadResponse_encode(roswasmc_bounded_1_franka_msgs_SetLoadResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_franka_msgs_SetLoadResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_franka_msgs_SetLoadResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_franka_msgs_SetLoadResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_franka_msgs_SetLoadResponse_decode(void const* input,size_t length,roswasmc_bounded_1_franka_msgs_SetLoadResponse* value) {
  roswasmc_bounded_1_franka_msgs_SetLoadResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_franka_msgs_SetLoadResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
