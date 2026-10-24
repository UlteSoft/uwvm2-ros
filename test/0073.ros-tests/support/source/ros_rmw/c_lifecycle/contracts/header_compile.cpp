// SPDX-License-Identifier: Apache-2.0
// Compile/link seam only. Root must supply an ACTUAL initialized Service for any
// future call; no fake rcl node/service, object layout or TypeSupport is created.
#include <rclcpp/service.hpp>
#include <platform/ros2/cpp/uwvm_rmw/c_service_lifecycle.hpp>
#if RCLCPP_UWVM_SERVICE_C_LIFECYCLE_HEADER_VERSION != 1 || RCLCPP_UWVM_CALLBACK_C_LIFECYCLE_HEADER_VERSION != 1
#error "Both actual original headers must be source-ported"
#endif
struct CompileCService{
 using Request=type_description_interfaces__srv__GetTypeDescription_Request;
 using Response=type_description_interfaces__srv__GetTypeDescription_Response;
};
std::shared_ptr<void> actual_original_service_request_creation(rclcpp::Service<CompileCService>&s){return s.create_request();}
