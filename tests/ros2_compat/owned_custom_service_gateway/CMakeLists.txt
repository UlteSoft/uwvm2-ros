# SPDX-License-Identifier: Apache-2.0
# Explicit source-only contract build. No test process starts during configure.
cmake_minimum_required(VERSION 3.20)
project(uwvm_owned_custom_gateway_contracts LANGUAGES CXX)
option(UWVM_BUILD_OWNED_CUSTOM_GATEWAY_CONTRACTS "Build authored actual production contracts" OFF)
if(UWVM_BUILD_OWNED_CUSTOM_GATEWAY_CONTRACTS)
  if(NOT IS_ABSOLUTE "${UWVM_SOURCE}" OR NOT EXISTS "${UWVM_SOURCE}/gateways/common/owned_service_storage.h")
    message(FATAL_ERROR "Actual reviewed source checkout required")
  endif()
  # Config admission uses the same real common generated registry as Gateway.
  # Without it the canonical custom-service keys/catalog are intentionally absent.
  if(NOT IS_ABSOLUTE "${UWVM_ROS_CUSTOM_REGISTRY_HEADER}" OR
      NOT EXISTS "${UWVM_ROS_CUSTOM_REGISTRY_HEADER}" OR IS_DIRECTORY "${UWVM_ROS_CUSTOM_REGISTRY_HEADER}")
    message(FATAL_ERROR "Actual complete generated common registry header required for wire witness")
  endif()
    if("${UWVM_ROS_CUSTOM_REGISTRY_HEADER}" MATCHES "[;\"\\\\\r\n]")
      message(FATAL_ERROR "UWVM_ROS_CUSTOM_REGISTRY_HEADER contains an unsupported include-path character")
    endif()
  foreach(kind storage native_wire)
    add_executable(owned-custom-gateway-${kind} "${kind}_contract.cc")
    target_compile_features(owned-custom-gateway-${kind} PRIVATE cxx_std_17)
    target_include_directories(owned-custom-gateway-${kind} PRIVATE "${UWVM_SOURCE}" "${UWVM_SOURCE}/src"
      "${UWVM_SOURCE}/third-parties/fast_io/include")
    target_link_libraries(owned-custom-gateway-${kind} PRIVATE pthread)
  endforeach()
  target_compile_definitions(owned-custom-gateway-native_wire PRIVATE
    UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_ROS_CUSTOM_REGISTRY_HEADER}")
  # The genuine full fast_io header requires concepts; keep this explicit.
  target_compile_features(owned-custom-gateway-native_wire PRIVATE cxx_std_20)
endif()
