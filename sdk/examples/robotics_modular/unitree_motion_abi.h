/** @file Scalar-only ABI for the simulator Go2 motion preload.
 *
 * These imports address the separately instantiated Wasm module named motion.
 * They carry no address, native handle, capability, C++ object or function
 * pointer. Every registration belongs to this motion instance and has a
 * monotonically increasing, non-wrapping application generation. The token is
 * application bookkeeping, not Runtime authentication or delegated ROS rights.
 *
 * begin(step, dt_ns) computes exactly the SDK simulation_control.h trajectory
 * for twenty motors, then calls the independent callbacks module for the first
 * twelve active joints. Failure returns generation zero; inspect status for
 * its error. A live registration must be ended before starting another one.
 *
 * word(generation, slot, field) returns an unrestricted uint32 value. Field0 is
 * the integer mode; fields1..5 are the binary32 representations of q, dq, kp,
 * kd and tau. Slots are 0..19. There is deliberately no float error sentinel:
 * the caller must read status immediately after every word and require zero.
 * Invalid/stale reads return zero AND set status22/9. No caller output pointer
 * is accepted. end verifies the actual callback count/checksum and retires the
 * registration before returning. End any nonzero generation even if copying
 * a field failed; preserve the primary failure while checking cleanup.
 *
 * All imports must unwind before root main performs CRC or any ROS operation.
 * Libraries receive zero ROS and WASI authority. In particular, this interface
 * never enables a ROS host call under a cross-module ancestry frame.
 */
#ifndef UWVM_UNITREE_MODULAR_MOTION_ABI_H
#define UWVM_UNITREE_MODULAR_MOTION_ABI_H
#include <stdint.h>
#if !defined(__wasm__) || !defined(__clang__) || UINTPTR_MAX != UINT32_MAX
#error "Use a reviewed Clang wasm32 toolchain for this scalar module ABI"
#endif
#ifdef __cplusplus
#define UWVM_UM_EXTERN extern "C"
#else
#define UWVM_UM_EXTERN extern
#endif
#define UWVM_UM_EXPORT(name) __attribute__((export_name(name)))
#define UWVM_UM_IMPORT(module,name) __attribute__((import_module(module),import_name(name)))
#if defined(UWVM_UNITREE_MOTION_IMPLEMENTATION)
#define UWVM_UM_MOTION(name) UWVM_UM_EXPORT(name)
#else
#define UWVM_UM_MOTION(name) UWVM_UM_IMPORT("motion",name)
#endif
#define UWVM_UM_OWNER UINT32_C(0x554d4f54)
#define UWVM_UM_MOTORS 20u
#define UWVM_UM_ACTIVE_JOINTS 12u
#define UWVM_UM_FIELDS 6u
UWVM_UM_EXTERN UWVM_UM_MOTION("motion_begin") uint32_t uwvm_unitree_motion_begin(uint32_t step,uint32_t dt_ns);
UWVM_UM_EXTERN UWVM_UM_MOTION("motion_status") uint32_t uwvm_unitree_motion_status(void);
UWVM_UM_EXTERN UWVM_UM_MOTION("motion_word") uint32_t uwvm_unitree_motion_word(uint32_t generation,uint32_t slot,uint32_t field);
UWVM_UM_EXTERN UWVM_UM_MOTION("motion_end") uint32_t uwvm_unitree_motion_end(uint32_t generation);

/** A value checksum, not an authentication primitive. FNV-1a consumes each
 * scalar in explicit little-endian byte order, independent of native layout.
 * The callback module implements its own recurrence over the same fields.
 */
static inline uint32_t uwvm_um_checksum_word(uint32_t checksum,uint32_t word) {
    unsigned i;for(i=0;i<4;++i){checksum^=(word>>(i*8))&255u;checksum*=UINT32_C(16777619);}return checksum;
}
#endif
