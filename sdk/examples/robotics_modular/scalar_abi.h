/** @file
 * Scalar-only example ABI. Never pass a linear-memory address, native pointer,
 * ROS capability handle, C++ object or functor between these Wasm instances.
 * All operations are synchronous. A caller must finish a call before retiring
 * its application registration. The Runtime owns its separate identity model.
 */
#ifndef ROSWASM_ROBOTICS_SCALAR_ABI_H
#define ROSWASM_ROBOTICS_SCALAR_ABI_H
#include <stdint.h>
#if !defined(__wasm__) || !defined(__clang__)
#error "Build this example with a reviewed Clang WebAssembly toolchain"
#endif
#if UINTPTR_MAX != UINT32_MAX
#error "The C/C++ example adapters require wasm32 memory-zero pointers"
#endif
#ifdef __cplusplus
#define ROBOT_EXTERN extern "C"
#else
#define ROBOT_EXTERN extern
#endif
#define ROBOT_EXPORT(name) __attribute__((export_name(name)))
#define ROBOT_IMPORT(module,name) __attribute__((import_module(module),import_name(name)))
#define ROBOT_OWNER UINT32_C(0x4d4f544e)
#define ROBOT_STALE (INT32_MIN + 9)
#define ROBOT_BUSY (INT32_MIN + 17)
#define ROBOT_INVALID (INT32_MIN + 22)
/* Valid commands are signed, dimensionless integers in [-100,100]. Distances
 * are explicit synthetic millimetres in [0,5000], never inferred camera depth.
 * Negative error sentinels are outside the entire valid command interval. */
#endif
