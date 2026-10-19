/** @file Explicit Unitree command checksum layouts, independent of ROS and DDS.
 *
 * These are field views, NOT wire structs and NOT native Unitree ABI structs.
 * Never serialize sizeof(view), cast a ROS message to words, or checksum the
 * compact bounded-v2 canonical record.  All stores below are explicit LE and
 * padding is zero.  The official source/commit lock and discrepancy are in
 * ../../PROTOCOL.md; retain ../../NOTICE.md. CRC detects transmission errors;
 * it authenticates nothing.
 *
 * Supported command space requires every named reserved field to be zero.
 * Nonzero values are refused, never silently cleared.  In particular the pinned
 * ROS2 HG helper copies only one of four top-level reserve words, unlike SDK2
 * Python.  Agreement outside this zero-reserved space is NOT claimed.
 *
 * Functions allocate no heap, perform no I/O/import, and obtain no ROS rights.
 * On error22/90 they preserve all caller outputs, including length/checksum.
 */
#ifndef UWVM_ROBOTS_UNITREE_CRC_H
#define UWVM_ROBOTS_UNITREE_CRC_H
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#if defined(__cplusplus)
static_assert(CHAR_BIT==8 && sizeof(float)==4 && FLT_RADIX==2 && FLT_MANT_DIG==24 && FLT_MAX_EXP==128,"IEEE binary32 required");
#else
_Static_assert(CHAR_BIT==8 && sizeof(float)==4 && FLT_RADIX==2 && FLT_MANT_DIG==24 && FLT_MAX_EXP==128,"IEEE binary32 required");
#endif
#define UWVM_UNITREE_INVALID 22u
#define UWVM_UNITREE_TOO_SMALL 90u
#define UWVM_UNITREE_GO_MOTORS 20u
#define UWVM_UNITREE_HG_MOTORS 35u
#define UWVM_UNITREE_GO_LOWCMD_BYTES 812u
#define UWVM_UNITREE_GO_CRC_OFFSET 808u
#define UWVM_UNITREE_HG_LOWCMD_BYTES 1004u
#define UWVM_UNITREE_HG_CRC_OFFSET 1000u
#define UWVM_UNITREE_CRC_MAX_BYTES 32768u

typedef struct uwvm_unitree_go_motor_cmd {
    uint8_t mode;
    float q, dq, tau, kp, kd;
    uint32_t reserve[3];
} uwvm_unitree_go_motor_cmd;
typedef struct uwvm_unitree_go_lowcmd {
    uint8_t head[2], level_flag, frame_reserve;
    uint32_t sn[2], version[2];
    uint16_t bandwidth;
    uwvm_unitree_go_motor_cmd motor_cmd[20];
    uint8_t bms_off, bms_reserve[3], wireless_remote[40], led[12], fan[2], gpio;
    uint32_t reserve;
} uwvm_unitree_go_lowcmd;
typedef struct uwvm_unitree_hg_motor_cmd {
    uint8_t mode;
    float q, dq, tau, kp, kd;
    uint32_t reserve;
} uwvm_unitree_hg_motor_cmd;
typedef struct uwvm_unitree_hg_lowcmd {
    uint8_t mode_pr, mode_machine;
    uwvm_unitree_hg_motor_cmd motor_cmd[35];
    uint32_t reserve[4];
} uwvm_unitree_hg_lowcmd;

/** Overflow-safe object range check; no pointer arithmetic across objects. */
static inline int uwvm_unitree_overlap(void const* a,size_t an,void const* b,size_t bn) {
    uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;
    if((an&&!a)||(bn&&!b)||an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y)return 1;
    return an&&bn&&x<y+bn&&y<x+an;
}
static inline void uwvm_unitree_put16(uint8_t* p,uint16_t value) {
    p[0]=(uint8_t)value;p[1]=(uint8_t)(value>>8);
}
static inline void uwvm_unitree_put32(uint8_t* p,uint32_t value) {
    p[0]=(uint8_t)value;p[1]=(uint8_t)(value>>8);p[2]=(uint8_t)(value>>16);p[3]=(uint8_t)(value>>24);
}
static inline uint32_t uwvm_unitree_get32(uint8_t const* p) {
    return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static inline int uwvm_unitree_finite(float value) {
    uint32_t bits;memcpy(&bits,&value,4);return (bits&UINT32_C(0x7f800000))!=UINT32_C(0x7f800000);
}
static inline void uwvm_unitree_put_float(uint8_t* p,float value) {
    uint32_t bits;memcpy(&bits,&value,4);uwvm_unitree_put32(p,bits);
}
/** Core polynomial algorithm: each LE32 input word is processed MSB first.
 * Multiple-of-four length is mandatory; 0 bytes yields 0xffffffff.  No final
 * XOR/reflection is applied.  This is NOT zlib CRC32 or CRC32C.  The algorithm
 * accepts all word bits; the family packers impose their separate value rules.
 */
static inline uint32_t uwvm_unitree_crc32_words_le(void const* data,size_t bytes,uint32_t* output) {
    uint32_t crc=UINT32_C(0xffffffff);size_t i;unsigned bit;
    uint8_t const* p=(uint8_t const*)data;
    if(!output||(bytes&&!data)||(bytes&3u)||bytes>UWVM_UNITREE_CRC_MAX_BYTES||uwvm_unitree_overlap(data,bytes,output,sizeof *output))return UWVM_UNITREE_INVALID;
    for(i=0;i<bytes;i+=4){uint32_t word=uwvm_unitree_get32(p+i);for(bit=0;bit<32;++bit){
        uint32_t top=crc>>31;crc<<=1;if(top)crc^=UINT32_C(0x04c11db7);
        if(word&(UINT32_C(0x80000000)>>bit))crc^=UINT32_C(0x04c11db7);
    }}
    *output=crc;return 0;
}
/** Shared transactional destination validation, covering every pair of outputs. */
static inline int uwvm_unitree_destinations(void const* value,size_t size,void* output,size_t bytes,size_t* written,uint32_t* crc) {
    return value&&output&&written&&crc &&
      !uwvm_unitree_overlap(value,size,output,bytes)&&!uwvm_unitree_overlap(value,size,written,sizeof *written)&&
      !uwvm_unitree_overlap(value,size,crc,sizeof *crc)&&!uwvm_unitree_overlap(output,bytes,written,sizeof *written)&&
      !uwvm_unitree_overlap(output,bytes,crc,sizeof *crc)&&!uwvm_unitree_overlap(written,sizeof *written,crc,sizeof *crc);
}
/** GO/SDK2 family: compact canonical749 != padded checksum812.
 * Header24, motor20*36, BMS4, wireless40, LED12, fan2, GPIO1, pad1,
 * reserve4, CRC4.  A field view's existing CRC is deliberately not an input.
 */
static inline uint32_t uwvm_unitree_go_lowcmd_pack_crc(uwvm_unitree_go_lowcmd const* value,void* output,size_t capacity,size_t* written,uint32_t* crc) {
    uint8_t bytes[UWVM_UNITREE_GO_LOWCMD_BYTES]={0};uint32_t next;size_t i,j;
    if(!value||value->frame_reserve||value->reserve)return UWVM_UNITREE_INVALID;
    for(i=0;i<3;++i)if(value->bms_reserve[i])return UWVM_UNITREE_INVALID;
    for(i=0;i<20;++i){uwvm_unitree_go_motor_cmd const* m=&value->motor_cmd[i];
        if(!uwvm_unitree_finite(m->q)||!uwvm_unitree_finite(m->dq)||!uwvm_unitree_finite(m->tau)||!uwvm_unitree_finite(m->kp)||!uwvm_unitree_finite(m->kd))return UWVM_UNITREE_INVALID;
        for(j=0;j<3;++j)if(m->reserve[j])return UWVM_UNITREE_INVALID;
    }
    if(capacity<sizeof bytes)return UWVM_UNITREE_TOO_SMALL;
    if(!uwvm_unitree_destinations(value,sizeof *value,output,sizeof bytes,written,crc))return UWVM_UNITREE_INVALID;
    bytes[0]=value->head[0];bytes[1]=value->head[1];bytes[2]=value->level_flag;
    for(i=0;i<2;++i){uwvm_unitree_put32(bytes+4+4*i,value->sn[i]);uwvm_unitree_put32(bytes+12+4*i,value->version[i]);}
    uwvm_unitree_put16(bytes+20,value->bandwidth);
    for(i=0;i<20;++i){uint8_t* p=bytes+24+36*i;uwvm_unitree_go_motor_cmd const* m=&value->motor_cmd[i];
        p[0]=m->mode;uwvm_unitree_put_float(p+4,m->q);uwvm_unitree_put_float(p+8,m->dq);
        uwvm_unitree_put_float(p+12,m->tau);uwvm_unitree_put_float(p+16,m->kp);uwvm_unitree_put_float(p+20,m->kd);
    }
    bytes[744]=value->bms_off;memcpy(bytes+748,value->wireless_remote,40);memcpy(bytes+788,value->led,12);
    memcpy(bytes+800,value->fan,2);bytes[802]=value->gpio;
    if(uwvm_unitree_crc32_words_le(bytes,UWVM_UNITREE_GO_CRC_OFFSET,&next))return UWVM_UNITREE_INVALID;
    uwvm_unitree_put32(bytes+UWVM_UNITREE_GO_CRC_OFFSET,next);memcpy(output,bytes,sizeof bytes);*written=sizeof bytes;*crc=next;return 0;
}
/** HG family: compact canonical897 != padded checksum1004.
 * Header4, motor35*28, reserve4*4, CRC4.  No physical robot joint order or
 * actuator count is inferred from the 35-slot protocol carrier.
 */
static inline uint32_t uwvm_unitree_hg_lowcmd_pack_crc(uwvm_unitree_hg_lowcmd const* value,void* output,size_t capacity,size_t* written,uint32_t* crc) {
    uint8_t bytes[UWVM_UNITREE_HG_LOWCMD_BYTES]={0};uint32_t next;size_t i;
    if(!value)return UWVM_UNITREE_INVALID;
    for(i=0;i<4;++i)if(value->reserve[i])return UWVM_UNITREE_INVALID;
    for(i=0;i<35;++i){uwvm_unitree_hg_motor_cmd const* m=&value->motor_cmd[i];
        if(m->reserve||!uwvm_unitree_finite(m->q)||!uwvm_unitree_finite(m->dq)||!uwvm_unitree_finite(m->tau)||!uwvm_unitree_finite(m->kp)||!uwvm_unitree_finite(m->kd))return UWVM_UNITREE_INVALID;
    }
    if(capacity<sizeof bytes)return UWVM_UNITREE_TOO_SMALL;
    if(!uwvm_unitree_destinations(value,sizeof *value,output,sizeof bytes,written,crc))return UWVM_UNITREE_INVALID;
    bytes[0]=value->mode_pr;bytes[1]=value->mode_machine;
    for(i=0;i<35;++i){uint8_t* p=bytes+4+28*i;uwvm_unitree_hg_motor_cmd const* m=&value->motor_cmd[i];
        p[0]=m->mode;uwvm_unitree_put_float(p+4,m->q);uwvm_unitree_put_float(p+8,m->dq);
        uwvm_unitree_put_float(p+12,m->tau);uwvm_unitree_put_float(p+16,m->kp);uwvm_unitree_put_float(p+20,m->kd);
    }
    if(uwvm_unitree_crc32_words_le(bytes,UWVM_UNITREE_HG_CRC_OFFSET,&next))return UWVM_UNITREE_INVALID;
    uwvm_unitree_put32(bytes+UWVM_UNITREE_HG_CRC_OFFSET,next);memcpy(output,bytes,sizeof bytes);*written=sizeof bytes;*crc=next;return 0;
}
#endif
