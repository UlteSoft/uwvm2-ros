/** @file Checked basic WASIp1 byte I/O, restricted to this Wasm32 example.
 * There is no environment, clock, random, file-open, socket or native pointer
 * import. Root supplies finite pipes and explicit stdio rights. Short transfers
 * progress within a bounded budget; EOF/zero progress and oversized output fail.
 * Text is formatted by fast_io in the C++ caller, never by a custom formatter.
 */
#ifndef UWVM_UNITREE_EXAMPLE_BASIC_IO_H
#define UWVM_UNITREE_EXAMPLE_BASIC_IO_H
#include <stdint.h>
#include <stddef.h>
#if !defined(__wasm__) || !defined(__clang__) || UINTPTR_MAX != UINT32_MAX
#error "Select the real Clang wasm32 basic-WASI example recipe"
#endif
#ifdef __cplusplus
#define UWVM_UNITREE_EXTERN extern "C"
#else
#define UWVM_UNITREE_EXTERN extern
#endif
typedef struct uwvm_unitree_iovec {void* pointer;size_t bytes;} uwvm_unitree_iovec;
UWVM_UNITREE_EXTERN __attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_read")))
uint32_t uwvm_unitree_example_read(uint32_t,uwvm_unitree_iovec const*,uint32_t,uint32_t*);
UWVM_UNITREE_EXTERN __attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
uint32_t uwvm_unitree_example_write(uint32_t,uwvm_unitree_iovec const*,uint32_t,uint32_t*);
static uint32_t uwvm_unitree_example_input(void* output,size_t bytes) {
    uint8_t* p=(uint8_t*)output;unsigned tries=0;size_t at=0;
    if(bytes>12)return 22;
    while(at<bytes&&tries++<32){uint32_t done=UINT32_MAX;uwvm_unitree_iovec v={p+at,bytes-at};
        uint32_t error=uwvm_unitree_example_read(0,&v,1,&done);
        if(error==27)continue; /* WASIp1 INTR, never a ROS operation retry. */
        if(error)return error;if(!done||done>bytes-at)return 22;at+=done;}
    return at==bytes?0:110;
}
static uint32_t uwvm_unitree_example_output(char const* input,size_t bytes) {
    unsigned tries=0;size_t at=0;if(bytes>4096)return 90;
    while(at<bytes&&tries++<4096){uint32_t done=UINT32_MAX;uwvm_unitree_iovec v={(void*)(input+at),bytes-at};
        uint32_t error=uwvm_unitree_example_write(1,&v,1,&done);
        if(error==27)continue;if(error)return error;if(!done||done>bytes-at)return 107;at+=done;}
    return at==bytes?0:110;
}
#endif
