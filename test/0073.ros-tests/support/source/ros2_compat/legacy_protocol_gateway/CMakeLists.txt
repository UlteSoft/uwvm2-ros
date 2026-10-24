cmake_minimum_required(VERSION 3.16)
project(uwvm_legacy_protocol_gateway_witness LANGUAGES CXX)

# This is an explicitly selected trusted-process witness. Configuration never
# starts a ROS actor, enables a Runtime import or modifies a Guest policy.
option(UWVM_BUILD_LEGACY_PROTOCOL_GATEWAY_WITNESS "Build the real isolated legacy protocol witness" OFF)
if(NOT UWVM_BUILD_LEGACY_PROTOCOL_GATEWAY_WITNESS)
  message(FATAL_ERROR "Explicit UWVM_BUILD_LEGACY_PROTOCOL_GATEWAY_WITNESS=ON is required")
endif()
foreach(header UWVM_ROS_CUSTOM_REGISTRY_HEADER UWVM_ROS_CUSTOM_ROS2_HEADER)
  if(NOT IS_ABSOLUTE "${${header}}" OR NOT EXISTS "${${header}}" OR IS_DIRECTORY "${${header}}" OR
      "${${header}}" MATCHES "[;\"\\\r\n]")
    message(FATAL_ERROR "${header} must be an explicit existing absolute safe header path")
  endif()
endforeach()
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(uwvm_source_proof_msgs REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)
add_executable(uwvm-legacy-protocol-gateway native_session.cc)
target_compile_features(uwvm-legacy-protocol-gateway PRIVATE cxx_std_17)
target_compile_options(uwvm-legacy-protocol-gateway PRIVATE -Wall -Wextra -Wpedantic -UNDEBUG)
target_include_directories(uwvm-legacy-protocol-gateway PRIVATE ../../.. ../../../src ../../../sdk/origin/include)
target_compile_definitions(uwvm-legacy-protocol-gateway PRIVATE
  UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_ROS_CUSTOM_REGISTRY_HEADER}"
  UWVM_ROS_CUSTOM_ROS2_HEADER="${UWVM_ROS_CUSTOM_ROS2_HEADER}")
ament_target_dependencies(uwvm-legacy-protocol-gateway rclcpp rclcpp_action rcl_interfaces uwvm_source_proof_msgs
  rosidl_typesupport_introspection_cpp)
target_link_libraries(uwvm-legacy-protocol-gateway OpenSSL::Crypto pthread)
