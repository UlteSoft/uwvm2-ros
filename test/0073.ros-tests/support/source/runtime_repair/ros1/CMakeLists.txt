# @file CMakeLists.txt
# @brief Build the same native comparison peer against the selected ROS1 SDK.
# @details The production gateway provides the selected native ABI, public
# dependency closure and feature probes. No middleware headers are substituted.
cmake_minimum_required(VERSION 3.16)
project(uwvm_ros1_runtime_repair LANGUAGES CXX)
if(NOT UWVM_ROS_SOURCE_ROOT OR NOT UWVM_ROS_SDK_ROOT)
    message(FATAL_ERROR "Select both the VM and SDK source checkouts")
endif()
set(UWVM_ROS1_BUILD_PROBES OFF CACHE BOOL "Build the focused comparison" FORCE)
set(UWVM_ROS1_CXX_STANDARD 17 CACHE STRING "Native selected ROS1 SDK ABI" FORCE)
add_subdirectory("${UWVM_ROS_SDK_ROOT}/src/platform/ros1/native_gateway" gateway)
add_executable(ros-service-peer ../ros_service_peer.cc)
target_compile_features(ros-service-peer PRIVATE cxx_std_17)
target_compile_definitions(ros-service-peer PRIVATE UWVM_ROS_BENCH_VERSION=1)
target_link_libraries(ros-service-peer PRIVATE uwvm-ros1-sdk pthread)
add_executable(ros1-connection-reuse ../ros1_connection_reuse.cc)
target_compile_features(ros1-connection-reuse PRIVATE cxx_std_17)
target_link_libraries(ros1-connection-reuse PRIVATE uwvm-ros1-sdk pthread)
