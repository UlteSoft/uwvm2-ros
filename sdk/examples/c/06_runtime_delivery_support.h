/** @file
 * @brief Direct checked basic-WASI gates shared by real C/C++ delivery examples.
 * Only fd0 reads, fd1 writes and proc_exit are used. A custom _start recipe
 * avoids CRT argument/environment queries; this example never grants a clock,
 * random, socket, process, thread or filesystem right. Every read/write checks
 * both errno and exact byte count; EOF/short transfer is a failed protocol gate.
 * The supervisor owns deadlines/poll pacing and must prove actual native match
 * before supplying P. Markers are observations, not self-issued ROS acceptance.
 */
#ifndef ROSWASM_RUNTIME_DELIVERY_SUPPORT_H
#define ROSWASM_RUNTIME_DELIVERY_SUPPORT_H
#include "select_backend.h"
#include <roswasm/node.h>
#if !defined(__wasm__)
#error "Runtime delivery examples require an actual WebAssembly target"
#endif
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sdk_delivery_iovec {void* data;size_t size;} sdk_delivery_iovec;
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_read")))
uint32_t sdk_delivery_fd_read(uint32_t,sdk_delivery_iovec const*,uint32_t,uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
uint32_t sdk_delivery_fd_write(uint32_t,sdk_delivery_iovec const*,uint32_t,uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("proc_exit"),noreturn))
void sdk_delivery_exit(uint32_t);
#ifdef __cplusplus
}
#endif
/** @brief Checked exact marker write, no hidden retry after partial output. */
static inline uint32_t sdk_delivery_emit(char const* marker,size_t length) {
    sdk_delivery_iovec vector={(void*)marker,length};uint32_t written=UINT32_MAX;
    uint32_t error=sdk_delivery_fd_write(1,&vector,1,&written);return error?error:written==length?0:UWVM_ROS_INVALID;
}
/** @brief Read exactly one supervisor token; newline/EOF/extra token is invalid. */
static inline uint32_t sdk_delivery_gate(char expected) {
    char token=0;sdk_delivery_iovec vector={&token,1};uint32_t read=UINT32_MAX;
    uint32_t error=sdk_delivery_fd_read(0,&vector,1,&read);return error?error:read==1&&token==expected?0:UWVM_ROS_INVALID;
}
/** @brief Protected byte sentinel uses explicit iteration, not native struct I/O. */
static inline void sdk_delivery_fill(unsigned char* bytes,size_t count,unsigned char value) {size_t n;for(n=0;n<count;++n) bytes[n]=value;}
static inline int sdk_delivery_equal(unsigned char const* bytes,size_t count,unsigned char value) {size_t n;for(n=0;n<count;++n) if(bytes[n]!=value) return 0;return 1;}
/** @brief Raw denial must preserve the actual eight-byte handle output slot. */
static inline uint32_t sdk_delivery_denied(char const* alias,size_t length) {
    unsigned char handle[8];uint32_t code;sdk_delivery_fill(handle,8,0xa5);
    code=ROSWASM_IMPORT(capability_open)(0,uwvm_ros_address(alias),length,uwvm_ros_address(handle));
    return code==UWVM_ROS_DENIED&&sdk_delivery_equal(handle,8,0xa5)?0:UWVM_ROS_INVALID;
}
/** @brief Verify operation denial/staleness and untouched receive+8LE length. */
static inline uint32_t sdk_delivery_poll_refusal(uint64_t handle,uint32_t expected) {
    unsigned char receive[64],length[8];uint32_t code;sdk_delivery_fill(receive,64,0xa5);sdk_delivery_fill(length,8,0xff);
    code=ROSWASM_IMPORT(event_poll)(handle,0,uwvm_ros_address(receive),sizeof receive,uwvm_ros_address(length));
    return code==expected&&sdk_delivery_equal(receive,64,0xa5)&&sdk_delivery_equal(length,8,0xff)?0:UWVM_ROS_INVALID;
}
/** @brief Observer wraps one actual SDK poll without replacing its result.
 * It verifies complete canonical reply bytes, untouched unused receive bytes,
 * and AGAIN sentinels. The wrapper neither dequeues twice nor simulates events.
 */
static uint32_t sdk_delivery_poll_error;
static uint32_t sdk_delivery_observed_poll(uint64_t handle,uint32_t memory,uint64_t output,uint64_t capacity,uint64_t slot) {
    static unsigned char const reply[]={19,0,0,0,'r','o','s','w','a','s','m','-','j','s','-','r','e','l','a','y','-','i','n'};
    unsigned char* bytes=(unsigned char*)(uintptr_t)output;unsigned char* count=(unsigned char*)(uintptr_t)slot;
    uint32_t code;size_t n;if(memory||capacity!=64) {sdk_delivery_poll_error=UWVM_ROS_INVALID;return UWVM_ROS_INVALID;}
    sdk_delivery_fill(bytes,64,0xa5);sdk_delivery_fill(count,8,0xff);
    code=ROSWASM_IMPORT(event_poll)(handle,memory,output,capacity,slot);
    if(code==UWVM_ROS_AGAIN) {if(!sdk_delivery_equal(bytes,64,0xa5)||!sdk_delivery_equal(count,8,0xff)) sdk_delivery_poll_error=UWVM_ROS_INVALID;}
    else if(!code) {
        if(uwvm_ros_load_u64(count)!=sizeof reply) sdk_delivery_poll_error=UWVM_ROS_INVALID;
        for(n=0;n<sizeof reply;++n) if(bytes[n]!=reply[n]) sdk_delivery_poll_error=UWVM_ROS_INVALID;
        if(!sdk_delivery_equal(bytes+sizeof reply,64-sizeof reply,0xa5)) sdk_delivery_poll_error=UWVM_ROS_INVALID;
    } else sdk_delivery_poll_error=UWVM_ROS_INVALID;
    return code;
}
#endif
