/** @file
 * @brief Optional, allocation-free native service observation for C11 guests.
 *
 * This separate dispatch table preserves roswasmc_backend and the original
 * business ABI. Include service_client_ros1.h or service_client_ros2.h explicitly
 * to select real imports. Merely including a header grants no ROS rights.
 *
 * Open an independent service.available owner through the ordinary C capability
 * API. Borrow the existing service.call owner of the same stable Context. The
 * Host must separately grant and precisely pair both aliases, native identity,
 * resource, codec and QoS. The SDK neither discovers an alias nor fills policy.
 */
#ifndef ROSWASMC_SERVICE_CLIENT_H
#define ROSWASMC_SERVICE_CLIENT_H
#include "roswasmc.h"
#include "service_client_v1.h"
#ifdef __cplusplus
extern "C" {
#endif
/** @brief Explicit optional imports; backend and all pointed-to code must remain
 * alive until every borrower has retired. No function pointer grants authority.
 */
typedef struct roswasmc_service_client_backend {
    uint32_t version;
    uint32_t (*abi_version)(void);
    uint32_t (*features)(uint64_t,uint64_t,uint32_t,uint64_t);
    uint32_t (*query)(uint64_t,uint64_t,uint64_t,uint32_t,uint64_t);
} roswasmc_service_client_backend;
/** @brief Check local owner association without invoking any import.
 * Handles remain opaque full-width values. This check is not a replacement for
 * the Host's actual Instance, generation, operation and native-session checks.
 */
static inline uint32_t roswasmc_service_client_owners(roswasmc_service_client_backend const* api,
    roswasmc_capability const* available,roswasmc_capability const* source) {
    if(!roswasmc_is_open(available)||!roswasmc_is_open(source))return UWVM_ROS_BAD_HANDLE;
    if(!api||api->version<1||api->version>2||available->context!=source->context||
       api->version!=available->context->backend->version||available->handle==source->handle)return UWVM_ROS_INVALID;
    return UWVM_ROS_SUCCESS;
}
/** @brief Read an actual feature observation, committing output only on success.
 * All-ones local storage detects an erroneous success without an output write.
 * Accepted masks are the complete catalog masks (ROS1=11, ROS2=7) or explicit0;
 * a missing/failed import never becomes a default feature answer. This function
 * spends the independently configured observation quota, never call quota.
 */
static inline uint32_t roswasmc_service_client_features(roswasmc_service_client_backend const* api,
    roswasmc_capability const* available,roswasmc_capability const* source,uint64_t* output) {
    unsigned char slot[8]={255,255,255,255,255,255,255,255};uint64_t value;uint32_t error;
    error=roswasmc_service_client_owners(api,available,source);if(error)return error;
    if(!output)return UWVM_ROS_INVALID;
    if(!api->abi_version||!api->features)return UWVM_ROS_UNSUPPORTED;
    if(api->abi_version()!=1)return UWVM_ROS_UNSUPPORTED;
    error=api->features(available->handle,source->handle,0,uwvm_ros_address(slot));if(error)return error;
    value=uwvm_ros_load_u64(slot);
    if(value!=0&&value!=(api->version==1?11u:7u))return UWVM_ROS_INVALID;
    *output=value;return UWVM_ROS_SUCCESS;
}
/** @brief Perform one genuine finite native readiness observation.
 * wait_ns0 still performs the actual native query. Positive waits are physical
 * native waits, bounded by explicit Host limits; no scheduler credit, graph
 * listing, request submission or empty business request substitutes for time.
 * Values above60e9ns are invalid. The Host may impose a smaller explicit bound.
 *
 * Only successful canonical0/1 changes *output. Error11 is busy/quota refusal,
 * 95 is unsupported, and110 is an operation timeout; none means false readiness.
 * A true result is a snapshot and does not promise a later call will succeed.
 * The borrowed source must outlive this call. Never retry a business operation
 * merely because a later readiness observation succeeded.
 */
static inline uint32_t roswasmc_service_client_query(roswasmc_service_client_backend const* api,
    roswasmc_capability const* available,roswasmc_capability const* source,uint64_t wait_ns,int* output) {
    unsigned char slot=255;uint32_t error=roswasmc_service_client_owners(api,available,source);if(error)return error;
    if(!output||wait_ns>UINT64_C(60000000000))return UWVM_ROS_INVALID;
    if(!api->abi_version||!api->query)return UWVM_ROS_UNSUPPORTED;
    if(api->abi_version()!=1)return UWVM_ROS_UNSUPPORTED;
    error=api->query(available->handle,source->handle,wait_ns,0,uwvm_ros_address(&slot));if(error)return error;
    if(slot>1)return UWVM_ROS_INVALID;
    *output=(int)slot;return UWVM_ROS_SUCCESS;
}
#ifdef __cplusplus
}
#endif
#endif
