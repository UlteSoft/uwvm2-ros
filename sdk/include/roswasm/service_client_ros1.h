/** @file
 * @brief Explicit ROS1 optional service-client import selection.
 * This header imports no ROS2 symbol and opens no capability. Existing guests
 * which use only ros1.h do not gain a dependency on the optional namespace.
 */
#ifndef ROSWASMC_SERVICE_CLIENT_ROS1_H
#define ROSWASMC_SERVICE_CLIENT_ROS1_H
#include "service_client.h"
/** @brief Obtain the immutable ROS1 extension table; no permission is implied. */
static inline roswasmc_service_client_backend const* roswasmc_ros1_service_client_backend(void) {
    static roswasmc_service_client_backend const api={1,uwvm_ros1_client_v1_abi_version,
        uwvm_ros1_client_v1_features,uwvm_ros1_client_v1_service_available};return &api;
}
#endif
