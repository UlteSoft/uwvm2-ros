/** @file
 * @brief UWVM2 ROS Runtime Extension ABI v1 common scalar types and byte helpers.
 * @see ../LICENSE for the SDK license.
 */
#ifndef UWVM_ROS_COMMON_ABI_H
#define UWVM_ROS_COMMON_ABI_H
#include <stdint.h>
#include <stddef.h>
/** @brief Opaque instance-owned handle; never a native pointer or transferable authority. */
typedef uint64_t uwvm_ros_handle;
/** @brief Unsigned i64 byte offset in the explicitly selected guest memory. */
typedef uint64_t uwvm_ros_offset;
/** @brief Stable numeric errors. Import abi_version returns a version value instead. */
enum uwvm_ros_error {
    UWVM_ROS_SUCCESS=0, UWVM_ROS_UNKNOWN_MODULE=2, UWVM_ROS_BAD_HANDLE=9, UWVM_ROS_AGAIN=11,
    UWVM_ROS_DENIED=13, UWVM_ROS_CONFLICT=17, UWVM_ROS_INVALID=22, UWVM_ROS_TOO_LARGE=90,
    UWVM_ROS_UNSUPPORTED=95, UWVM_ROS_DISCONNECTED=107,
    UWVM_ROS_TIMEOUT=110, UWVM_ROS_REVOKED=125
};
/* Handles and returned lengths occupy eight little-endian bytes. Every offset
   and length is an i64 in both wasm32 and memory64; memory index is an i32.
   Copy helpers use memory 0. All data is copied during the import call. */
/** @brief Convert a guest pointer to an i64 offset; this does not validate a range or grant access. */
static inline uwvm_ros_offset uwvm_ros_address(void const* p) { return (uint64_t)(uintptr_t)p; }
/** @brief Load exactly eight little-endian bytes, including unaligned output slots. */
static inline uint64_t uwvm_ros_load_u64(unsigned char const* p) {
    uint64_t v=0; for(unsigned i=0;i<8;++i) v|=(uint64_t)p[i]<<(8*i); return v;
}
/** @brief Store one four-byte little-endian integer without alignment requirements. */
static inline void uwvm_ros_store_u32(unsigned char* p, uint32_t v) {
    for(unsigned i=0;i<4;++i) p[i]=(unsigned char)(v>>(8*i));
}
#endif
