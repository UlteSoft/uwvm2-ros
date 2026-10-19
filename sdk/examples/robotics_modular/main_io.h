/** @file Checked basic WASIp1 in the root main only.
 * Explicit imports avoid CRT argument/environment queries. No library imports
 * this file. Do not add clock/random/file/socket grants to fix a build error.
 * Each operation checks errno and exact count, with no short-transfer replay.
 */
#ifndef ROSWASM_ROBOTICS_MAIN_IO_H
#define ROSWASM_ROBOTICS_MAIN_IO_H
#include "scalar_abi.h"
#include <stddef.h>
typedef struct robot_iovec {void* data;size_t size;} robot_iovec;
#ifdef __cplusplus
static_assert(sizeof(robot_iovec)==8,"WASIp1 wasm32 iovec must be exactly8 bytes");
#else
_Static_assert(sizeof(robot_iovec)==8,"WASIp1 wasm32 iovec must be exactly8 bytes");
#endif
ROBOT_EXTERN ROBOT_IMPORT("wasi_snapshot_preview1","fd_write") uint32_t robot_write(uint32_t,robot_iovec const*,uint32_t,uint32_t*);
ROBOT_EXTERN ROBOT_IMPORT("wasi_snapshot_preview1","fd_read") uint32_t robot_read(uint32_t,robot_iovec const*,uint32_t,uint32_t*);
ROBOT_EXTERN ROBOT_IMPORT("wasi_snapshot_preview1","proc_exit") __attribute__((noreturn)) void robot_exit(uint32_t);
static uint32_t robot_marker(char const* text,size_t length) {
    robot_iovec vector = {(void*)text,length}; uint32_t count = UINT32_MAX;
    uint32_t error = robot_write(1,&vector,1,&count);
    return error ? error : count == length ? 0 : 22;
}
#define ROBOT_MARKER(literal) robot_marker(literal,sizeof(literal)-1)
static uint32_t robot_gate(void) {
    char gate = 0; robot_iovec vector = {&gate,1}; uint32_t count = UINT32_MAX;
    uint32_t error = robot_read(0,&vector,1,&count);
    return error ? error : count == 1 && gate == 'P' ? 0 : 22;
}
#endif
