/** @file Scalar observed-LowState perception ABI for the simulator example.
 *
 * This is a joint-state validity/ordering module, not an image or camera
 * algorithm. The main supplies the 36 actual q/dq/tau_est binary32 words for
 * twelve ordered joints after complete generated LowState decoding. Each
 * word crosses a real Wasm import into a separately owned linear memory.
 * No pointer, ROS capability or native handle crosses this boundary.
 *
 * begin returns a nonzero, nonwrapping application generation. word consumes
 * exactly the next index and rejects nonfinite binary32 data. end compares an
 * independently accumulated little-endian FNV value checksum and retires the
 * generation. The checksum and generation are correlation, not authorization.
 * Always abort a begun generation after a failed word/end; preserve the first
 * error. No ROS operation is permitted until the entire import stack unwinds.
 */
#ifndef UWVM_UNITREE_PERCEPTION_ABI_H
#define UWVM_UNITREE_PERCEPTION_ABI_H
#include "unitree_motion_abi.h"
#if defined(UWVM_UNITREE_PERCEPTION_IMPLEMENTATION)
#define UWVM_UP_FN(name) UWVM_UM_EXPORT(name)
#else
#define UWVM_UP_FN(name) UWVM_UM_IMPORT("perception",name)
#endif
UWVM_UM_EXTERN UWVM_UP_FN("perception_begin") uint32_t uwvm_unitree_perception_begin(uint32_t);
UWVM_UM_EXTERN UWVM_UP_FN("perception_status") uint32_t uwvm_unitree_perception_status(void);
UWVM_UM_EXTERN UWVM_UP_FN("perception_word") uint32_t uwvm_unitree_perception_word(uint32_t,uint32_t,uint32_t);
UWVM_UM_EXTERN UWVM_UP_FN("perception_end") uint32_t uwvm_unitree_perception_end(uint32_t,uint32_t);
UWVM_UM_EXTERN UWVM_UP_FN("perception_abort") uint32_t uwvm_unitree_perception_abort(uint32_t);
#endif
