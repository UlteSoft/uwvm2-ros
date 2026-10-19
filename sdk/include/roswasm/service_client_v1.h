/*************************************************************
 * UWVM2 optional service-client extension1. APL-2.0.        *
 *************************************************************/
#ifndef UWVM_ROSWASM_SERVICE_CLIENT_V1_H
#define UWVM_ROSWASM_SERVICE_CLIENT_V1_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__wasm__)
#define UWVM_ROS_CLIENT_IMPORT(module,name) __attribute__((__import_module__(module),__import_name__(name)))
#else
#define UWVM_ROS_CLIENT_IMPORT(module,name)
#endif
/* This OPTIONAL independent namespace does not replace or extend the original
 * business-v1 tuple lists. Each backend must actually be compiled/admitted.
 * Missing Host extension imports fail linking. Missing native query/QoS APIs
 * return95; old Gateways cannot attest the new manifest/operation and retain
 * their truthful authentication/protocol/transport refusal. No refusal or old
 * response is converted into an availability answer.
 * Both handles are real full64-bit capabilities from the same backend/instance:
 * independently granted service.available and its configured service.call.
 * There is no Guest retirement/management/command-selector import.
 *
 * features success writes exactly8 little-endian bytes. service_available writes
 * exactly1 byte0/1. Any error leaves output untouched. wait_ns0 still performs
 * one genuine native bounded query; positive waits require explicit finite Host
 * limits. Negative/infinite waits are invalid22, not silently saturated.
 * A true result is a snapshot, not a promise of subsequent remote call success.
 */
enum uwvm_ros_client_feature {
    UWVM_ROS_CLIENT_TYPED_QUERY=1,
    UWVM_ROS_CLIENT_FINITE_WAIT=2,
    UWVM_ROS_CLIENT_ACTUAL_QOS_CHECK=4,
    UWVM_ROS_CLIENT_TYPED_MD5_HEADER=8
};
UWVM_ROS_CLIENT_IMPORT("uwvm_ros1_client_v1","abi_version")
uint32_t uwvm_ros1_client_v1_abi_version(void);
UWVM_ROS_CLIENT_IMPORT("uwvm_ros1_client_v1","features")
uint32_t uwvm_ros1_client_v1_features(uint64_t availability,uint64_t source,uint32_t memory,uint64_t output);
UWVM_ROS_CLIENT_IMPORT("uwvm_ros1_client_v1","service_available")
uint32_t uwvm_ros1_client_v1_service_available(uint64_t availability,uint64_t source,uint64_t wait_ns,uint32_t memory,uint64_t output);
UWVM_ROS_CLIENT_IMPORT("uwvm_ros2_client_v1","abi_version")
uint32_t uwvm_ros2_client_v1_abi_version(void);
UWVM_ROS_CLIENT_IMPORT("uwvm_ros2_client_v1","features")
uint32_t uwvm_ros2_client_v1_features(uint64_t availability,uint64_t source,uint32_t memory,uint64_t output);
UWVM_ROS_CLIENT_IMPORT("uwvm_ros2_client_v1","service_available")
uint32_t uwvm_ros2_client_v1_service_available(uint64_t availability,uint64_t source,uint64_t wait_ns,uint32_t memory,uint64_t output);
#undef UWVM_ROS_CLIENT_IMPORT
#ifdef __cplusplus
}
#endif
#endif
