/** @file
 * @brief Explicit ROS2 optional service-client import selection.
 * This header imports no ROS1 symbol and opens no capability. Existing guests
 * which use only ros2.h do not gain a dependency on the optional namespace.
 */
#ifndef ROSWASMC_SERVICE_CLIENT_ROS2_H
#define ROSWASMC_SERVICE_CLIENT_ROS2_H
#include "service_client.h"
/** @brief Obtain the immutable ROS2 extension table; no permission is implied. */
static inline roswasmc_service_client_backend const* roswasmc_ros2_service_client_backend(void) {
    static roswasmc_service_client_backend const api={2,uwvm_ros2_client_v1_abi_version,
        uwvm_ros2_client_v1_features,uwvm_ros2_client_v1_service_available};return &api;
}
#endif
