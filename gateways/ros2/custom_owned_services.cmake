# SPDX-License-Identifier: Apache-2.0
# Additive SOURCE setup only. This file has not been configured or executed.
include_guard(GLOBAL)
include(CheckCXXSourceCompiles)
include(CMakeParseArguments)
set_property(GLOBAL PROPERTY UWVM_OWNED_SERVICE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(UWVM_ROS2_CUSTOM_SERVICE_SHARDS 32 CACHE STRING "Actual complete custom-Service partitions1..256")

# The real consumer first selects its original SDK/provider/manifest and Native
# feature headers. SDK_TARGETS are actual imported link targets, not booleans.
# COMPLETE metadata remains0 if the REAL executable compile/link fails. The
# independent expected export/header is a separate mandatory runtime proof.
function(uwvm_ros2_owned_custom_service_sources target)
  cmake_parse_arguments(S "" "" "SDK_TARGETS;SDK_INCLUDE_DIRS" ${ARGN})
  if(NOT TARGET ${target} OR NOT S_SDK_TARGETS)
    message(FATAL_ERROR "Need an existing real consumer and selected SDK targets")
  endif()
  foreach(t IN LISTS S_SDK_TARGETS)
    if(NOT TARGET ${t})
      message(FATAL_ERROR "Missing actual original SDK target: ${t}")
    endif()
  endforeach()
  get_property(done TARGET ${target} PROPERTY UWVM_OWNED_SERVICES_SHARDED)
  if(done)
    message(FATAL_ERROR "Owned custom Service sources already installed")
  endif()
  if(NOT UWVM_ROS2_CUSTOM_SERVICE_SHARDS MATCHES "^[1-9][0-9]?[0-9]?$" OR UWVM_ROS2_CUSTOM_SERVICE_SHARDS GREATER 256)
    message(FATAL_ERROR "Actual Service partitions must be1..256")
  endif()
  get_property(dir GLOBAL PROPERTY UWVM_OWNED_SERVICE_SOURCE_DIR)
  get_filename_component(root "${dir}/../.." ABSOLUTE)
  if(NOT EXISTS "${root}/third-parties/fast_io/include/fast_io_freestanding.h")
    message(FATAL_ERROR "Exact selected vendored fast_io lexical closure missing")
  endif()
  if(NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 20)
    set(CMAKE_CXX_STANDARD 20)
  endif()
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  target_compile_features(${target} PRIVATE cxx_std_20)
  find_package(OpenSSL REQUIRED COMPONENTS Crypto)
  set(CMAKE_REQUIRED_LIBRARIES ${S_SDK_TARGETS} OpenSSL::Crypto)
  set(CMAKE_REQUIRED_INCLUDES ${S_SDK_INCLUDE_DIRS})
  # Deliberately clear BOTH namespaces. A stale SDK result or -D=1 is not proof.
  unset(UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API)
  unset(UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API CACHE)
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  check_cxx_source_compiles([=[
#include <std_srvs/srv/empty.hpp>
#include <builtin_interfaces/msg/time.hpp>
#include <service_msgs/msg/service_event_info.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_introspection_cpp/service_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/identifier.hpp>
#include <rosidl_runtime_c/service_type_support_struct.h>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <openssl/sha.h>
int main(){
 using S=std_srvs::srv::Empty;
 auto volatile get=&rosidl_typesupport_cpp::get_service_type_support_handle<S>;
 auto h=get(); if(!h)return 1;
 auto n=h->func(h,rosidl_typesupport_introspection_cpp::typesupport_identifier);
 auto q=rosidl_typesupport_cpp::get_message_type_support_handle<S::Request>();
 auto r=rosidl_typesupport_cpp::get_message_type_support_handle<S::Response>();
 auto e=rosidl_typesupport_cpp::get_message_type_support_handle<S::Event>();
 auto t=rosidl_typesupport_cpp::get_message_type_support_handle<builtin_interfaces::msg::Time>();
 auto i=rosidl_typesupport_cpp::get_message_type_support_handle<service_msgs::msg::ServiceEventInfo>();
 auto m=static_cast<rosidl_typesupport_introspection_cpp::ServiceMembers const*>(n->data);
 auto hash=h->get_type_hash_func(h);auto d=h->get_type_description_func(h);auto sources=h->get_type_description_sources_func(h);
 auto create=h->event_message_create_handle_function;auto destroy=h->event_message_destroy_handle_function;
 unsigned char out[32];auto volatile sha=&::SHA256;sha(reinterpret_cast<unsigned char const*>(""),0,out);
 return !(hash&&d&&sources&&create&&destroy&&m->event_members_&&h->event_typesupport&&q&&r&&e&&t&&i&&q->get_type_description_sources_func(q)&&S::Request{}.structure_needs_at_least_one_member==0);
}
]=] UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API)
  set(gen "${CMAKE_CURRENT_BINARY_DIR}/owned-custom-services/${target}")
  file(MAKE_DIRECTORY "${gen}")
  configure_file("${dir}/custom_owned_service_features.h.in" "${gen}/custom_owned_service_features.h")
  target_include_directories(${target} PRIVATE "${gen}" "${dir}" "${root}/src" "${root}/sdk/include" "${root}/sdk/rmw_wasm/include" "${root}/third-parties/fast_io/include" ${S_SDK_INCLUDE_DIRS})
  target_link_libraries(${target} PRIVATE ${S_SDK_TARGETS} OpenSSL::Crypto)
  target_compile_definitions(${target} PRIVATE UWVM_ROS2_CUSTOM_SERVICE_SHARDS=${UWVM_ROS2_CUSTOM_SERVICE_SHARDS})
  target_sources(${target} PRIVATE "${dir}/custom_owned_services_dispatch.cc")
  math(EXPR last "${UWVM_ROS2_CUSTOM_SERVICE_SHARDS}-1")
  foreach(UWVM_ROS2_CUSTOM_SERVICE_SHARD_INDEX RANGE 0 ${last})
    set(shard "${gen}/custom_owned_service_shard_${UWVM_ROS2_CUSTOM_SERVICE_SHARD_INDEX}.cc")
    configure_file("${dir}/custom_owned_services_shard.cc.in" "${shard}" @ONLY)
    target_sources(${target} PRIVATE "${shard}")
  endforeach()
  set_property(TARGET ${target} PROPERTY UWVM_OWNED_SERVICES_SHARDED TRUE)
endfunction()
