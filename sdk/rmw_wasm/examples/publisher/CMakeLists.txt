cmake_minimum_required(VERSION 3.20)
project(uwvm_original_rmw_publisher_example LANGUAGES C)
# Use the genuine installed A/B/C prefix. No compat/rclc facade include path.
find_package(rmw_uwvm REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
if(NOT DEFINED UWVM_ROS_SOURCE_ROOT OR NOT EXISTS "${UWVM_ROS_SOURCE_ROOT}/sdk/rmw_wasm/publisher_transport/CMakeLists.txt")
  message(FATAL_ERROR "UWVM_ROS_SOURCE_ROOT must identify the real composed source tree")
endif()
# Select real source headers from this same recompiled cohort, never stale B
# headers from a different layout. Every consumer must be rebuilt together.
set(UWVM_RMW_INCLUDE_DIR "${UWVM_ROS_SOURCE_ROOT}/sdk/rmw_wasm/include")
set(UWVM_ROS_SDK_INCLUDE_DIR "${UWVM_ROS_SOURCE_ROOT}/sdk/include")
if(UWVM_RMW_PUBLISHER_TRANSPORT_BUILD_MODELS)
  message(FATAL_ERROR "Local strong raw models must not be linked with this real application")
endif()
set(UWVM_RMW_PUBLISHER_TRANSPORT_BUILD_MODELS OFF CACHE BOOL "Real import application" FORCE)
add_subdirectory("${UWVM_ROS_SOURCE_ROOT}/sdk/rmw_wasm/publisher_transport" "${CMAKE_CURRENT_BINARY_DIR}/publisher_transport")
add_executable(checked_publisher checked_publisher.c)
target_compile_features(checked_publisher PRIVATE c_std_11)
target_link_libraries(checked_publisher PRIVATE rmw_uwvm::rmw_uwvm uwvm_rmw_publisher_transport
  std_msgs::std_msgs__rosidl_generator_c std_msgs::std_msgs__rosidl_typesupport_introspection_c
  std_msgs::std_msgs__rosidl_typesupport_c rosidl_runtime_c::rosidl_runtime_c)
# Raw symbols remain real optional Wasm imports. Do not supply native weak
# definitions or a blanket allow-undefined-imports option to mask a missing ABI.
