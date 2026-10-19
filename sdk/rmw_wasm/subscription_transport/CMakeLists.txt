cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_subscription_transport LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Source recipe only. No native type, raw ABI, weak fallback or test is generated.
find_package(rmw REQUIRED)
find_package(rcutils REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
if(NOT IS_ABSOLUTE "${UWVM_RMW_INCLUDE_DIR}" OR NOT EXISTS "${UWVM_RMW_INCLUDE_DIR}/uwvm_rmw_c/provider.h")
 message(FATAL_ERROR "Use the actual complete current B/C original-SDK headers")
endif()
if(NOT IS_ABSOLUTE "${UWVM_ROS_SDK_INCLUDE_DIR}" OR NOT EXISTS "${UWVM_ROS_SDK_INCLUDE_DIR}/roswasm/rmw_subscription_v1.h")
 message(FATAL_ERROR "Use the actual Root raw7 header; independent source closure is still required")
endif()
include(CheckCSourceCompiles)
set(CMAKE_REQUIRED_LIBRARIES rmw::rmw)
check_c_source_compiles([=[
#include <rmw/types.h>
#include <stdint.h>
#include <stdbool.h>
_Static_assert(sizeof(rmw_message_info_t)==sizeof(rmw_message_info_t),"actual original type");
_Static_assert(sizeof(((rmw_message_info_t*)0)->source_timestamp)==8,"full signed source time");
_Static_assert(sizeof(((rmw_message_info_t*)0)->received_timestamp)==8,"full signed receive time");
_Static_assert((rmw_time_point_value_t)-1<0,"signed actual native nanoseconds");
_Static_assert(_Generic(((rmw_message_info_t*)0)->source_timestamp,int64_t:1,default:0),"actual signed source field");
_Static_assert(_Generic(((rmw_message_info_t*)0)->received_timestamp,int64_t:1,default:0),"actual signed receive field");
_Static_assert(_Generic(((rmw_message_info_t*)0)->publication_sequence_number,uint64_t:1,default:0),"actual unsigned publication field");
_Static_assert(_Generic(((rmw_message_info_t*)0)->reception_sequence_number,uint64_t:1,default:0),"actual unsigned reception field");
_Static_assert(sizeof(((rmw_message_info_t*)0)->publication_sequence_number)==8,"full native publication sequence");
_Static_assert(sizeof(((rmw_message_info_t*)0)->reception_sequence_number)==8,"full native reception sequence");
_Static_assert(sizeof(((rmw_message_info_t*)0)->publisher_gid.data)==RMW_GID_STORAGE_SIZE,"actual exact GID extent");
_Static_assert(RMW_GID_STORAGE_SIZE>0&&RMW_GID_STORAGE_SIZE<=32,"complete protocol width");
_Static_assert(RMW_MESSAGE_INFO_SEQUENCE_NUMBER_UNSUPPORTED==UINT64_MAX,"original unsupported sentinel");
int main(void){rmw_message_info_t v={0};v.from_intra_process=false;return v.from_intra_process?1:0;}
]=] UWVM_RMW_SUBSCRIPTION_MESSAGE_INFO_API)
unset(CMAKE_REQUIRED_LIBRARIES)
configure_file(uwvm_rmw_subscription_features.h.in "${CMAKE_CURRENT_BINARY_DIR}/uwvm_rmw_subscription_features.h")
add_library(uwvm_rmw_subscription_transport STATIC src/subscription_transport.c)
target_compile_features(uwvm_rmw_subscription_transport PUBLIC c_std_11)
target_include_directories(uwvm_rmw_subscription_transport PUBLIC
 "${CMAKE_CURRENT_SOURCE_DIR}/include" "${UWVM_RMW_INCLUDE_DIR}" "${UWVM_ROS_SDK_INCLUDE_DIR}"
 PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(uwvm_rmw_subscription_transport PUBLIC rmw::rmw rcutils::rcutils rosidl_runtime_c::rosidl_runtime_c)
# Actual B owns uwvm_rmw_qos_equal/identifier, C owns the process singleton. Link
# this static component with the complete genuine A/B/C target, not a fake ABI.
# The seven unresolved raw symbols are real optional Wasm imports. They are NOT
# implemented by the production target; missing Host support must fail linking.
option(UWVM_RMW_SUBSCRIPTION_BUILD_MODELS "Build explicit local raw7 protocol models only" OFF)
if(UWVM_RMW_SUBSCRIPTION_BUILD_MODELS)
 find_package(std_msgs REQUIRED)
 if(NOT TARGET rmw_uwvm)
  message(FATAL_ERROR "Models need the actual complete A/B/C target, not fake B functions")
 endif()
 get_property(_cpp TARGET rmw_uwvm PROPERTY UWVM_RMW_CPP_CODECS_ATTACHED)
 if(NOT _cpp)
  message(FATAL_ERROR "Attach the actual separately reviewed CPP10 component, all original generated types/support and its source-recompile closure first")
 endif()
 if(NOT UWVM_RMW_SUBSCRIPTION_MESSAGE_INFO_API)
  message(FATAL_ERROR "Actual selected SDK message-info contract unavailable; no substitute model type")
 endif()
 add_executable(uwvm_rmw_subscription_models tests/model_contract.cc)
 target_compile_features(uwvm_rmw_subscription_models PRIVATE cxx_std_20)
 if(NOT EXISTS "${UWVM_RMW_INCLUDE_DIR}/../tests/published_cpp_identity_fixture.hpp")
  message(FATAL_ERROR "Actual separately reviewed CPP10 published identity fixture source required for models")
 endif()
 target_include_directories(uwvm_rmw_subscription_models PRIVATE "${UWVM_RMW_INCLUDE_DIR}/../tests")
 target_compile_options(uwvm_rmw_subscription_models PRIVATE -UNDEBUG)
 target_link_libraries(uwvm_rmw_subscription_models PRIVATE uwvm_rmw_subscription_transport rmw_uwvm
  std_msgs::std_msgs__rosidl_generator_c std_msgs::std_msgs__rosidl_typesupport_c std_msgs::std_msgs__rosidl_typesupport_introspection_c)
endif()
# No add_test, try_run, subprocess driver, network or automatic native execution.
option(UWVM_RMW_SUBSCRIPTION_BUILD_EXAMPLE "Build actual Guest RMW checked subscription source" OFF)
if(UWVM_RMW_SUBSCRIPTION_BUILD_EXAMPLE)
 if(NOT UWVM_RMW_SUBSCRIPTION_MESSAGE_INFO_API)
  message(FATAL_ERROR "Actual selected SDK metadata layout must be available for the example")
 endif()
 if(NOT TARGET rmw_uwvm)
  message(FATAL_ERROR "Actual complete static original-WASI A/B/C target required")
 endif()
 find_package(std_msgs REQUIRED)
 add_executable(checked_rmw_subscription examples/checked_subscription.c)
 target_compile_features(checked_rmw_subscription PRIVATE c_std_11)
 target_link_libraries(checked_rmw_subscription PRIVATE uwvm_rmw_subscription_transport rmw_uwvm
  std_msgs::std_msgs__rosidl_generator_c std_msgs::std_msgs__rosidl_typesupport_c std_msgs::std_msgs__rosidl_typesupport_introspection_c)
endif()
option(UWVM_RMW_SUBSCRIPTION_BUILD_NATIVE_PEER "Build a separate real native ROS publisher; never execute it here" OFF)
if(UWVM_RMW_SUBSCRIPTION_BUILD_NATIVE_PEER)
 find_package(rclcpp REQUIRED)
 find_package(std_msgs REQUIRED)
 add_executable(raw7_native_publisher tests/native_publisher.cc)
 target_compile_features(raw7_native_publisher PRIVATE cxx_std_17)
 target_link_libraries(raw7_native_publisher PRIVATE rclcpp::rclcpp std_msgs::std_msgs__rosidl_typesupport_cpp)
endif()
