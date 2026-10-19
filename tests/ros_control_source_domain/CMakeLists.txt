# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(uwvm_control_source_domain_contracts LANGUAGES C CXX)
set(UWVM_CONTROL_GENERATED "" CACHE PATH "Explicit fresh admitted C/C++ output directory")
set(UWVM_CONTROL_REPOSITORY "" CACHE PATH "Complete reviewed source tree (no downloaded SDK substitute)")
option(UWVM_CONTROL_BUILD_NATIVE_TRUE "Build actual installed-SDK full sources witness; no automatic run" OFF)
set(UWVM_CONTROL_C_LIFECYCLE_TARGET "uwvm_rmw_c_service_lifecycle::uwvm_rmw_c_service_lifecycle" CACHE STRING "Actual admitted ownership port target")
if(NOT IS_ABSOLUTE "${UWVM_CONTROL_GENERATED}" OR NOT IS_ABSOLUTE "${UWVM_CONTROL_REPOSITORY}")
 message(FATAL_ERROR "Explicit absolute generated/source roots required")
endif()
foreach(_source bounded_records.h bounded_records.hpp control_wire.hpp control-schema.json generation.json)
 if(NOT EXISTS "${UWVM_CONTROL_GENERATED}/${_source}")
  message(FATAL_ERROR "Actual freshly generated control source required: ${_source}")
 endif()
endforeach()
add_executable(uwvm_control_source_c source_domain.c)
target_compile_features(uwvm_control_source_c PRIVATE c_std_11)
target_include_directories(uwvm_control_source_c PRIVATE "${UWVM_CONTROL_GENERATED}")
add_executable(uwvm_control_source_cpp source_domain_cpp.cpp)
target_compile_features(uwvm_control_source_cpp PRIVATE cxx_std_20)
target_include_directories(uwvm_control_source_cpp PRIVATE "${UWVM_CONTROL_GENERATED}" "${UWVM_CONTROL_REPOSITORY}/src" "${UWVM_CONTROL_REPOSITORY}/sdk/include")
if(UWVM_CONTROL_BUILD_NATIVE_TRUE)
 find_package(rclcpp REQUIRED)
 find_package(type_description_interfaces REQUIRED)
 find_package(rosidl_runtime_c REQUIRED)
 if(NOT TARGET "${UWVM_CONTROL_C_LIFECYCLE_TARGET}")
  find_package(uwvm_rmw_c_service_lifecycle REQUIRED)
 endif()
 set(_native_targets rclcpp::rclcpp
  type_description_interfaces::type_description_interfaces__rosidl_generator_c
  type_description_interfaces::type_description_interfaces__rosidl_typesupport_c
  type_description_interfaces::type_description_interfaces__rosidl_typesupport_cpp
  rosidl_runtime_c::rosidl_runtime_c "${UWVM_CONTROL_C_LIFECYCLE_TARGET}")
 foreach(_target IN LISTS _native_targets)
  if(NOT TARGET "${_target}")
   message(FATAL_ERROR "Real installed SDK/ownership target missing: ${_target}")
  endif()
 endforeach()
 # The native Node implementation itself must be rebuilt with the real PUBLIC
 # ownership helper, not only compiled application headers carrying a marker.
 get_target_property(_rclcpp_links rclcpp::rclcpp INTERFACE_LINK_LIBRARIES)
 list(FIND _rclcpp_links "${UWVM_CONTROL_C_LIFECYCLE_TARGET}" _ownership_link)
 if(_ownership_link LESS 0)
  message(FATAL_ERROR "Actual original rclcpp PUBLIC C ownership link closure is required")
 endif()
 get_target_property(_admitted "${UWVM_CONTROL_C_LIFECYCLE_TARGET}" UWVM_C_LIFECYCLE_LINK_ADMITTED)
 if(NOT _admitted)
  message(FATAL_ERROR "Actual C init/fini linker admission has not succeeded")
 endif()
 # A cross toolchain's STATIC_LIBRARY shortcut must not fake modern API link
 # admission. This function restores all caller variables on return. No try_run.
 function(_uwvm_control_actual_sources_link result)
  include(CheckCXXSourceCompiles)
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  set(CMAKE_REQUIRED_LIBRARIES ${_native_targets})
  unset(UWVM_CONTROL_REAL_SOURCES_API)
  unset(UWVM_CONTROL_REAL_SOURCES_API CACHE)
  check_cxx_source_compiles([=[
#include <type_description_interfaces/srv/get_type_description.hpp>
#include <type_description_interfaces/srv/detail/get_type_description__type_support.h>
#include <rosidl_runtime_c/service_type_support_struct.h>
#include <rosidl_typesupport_interface/macros.h>
int main(){auto s=ROSIDL_GET_SRV_TYPE_SUPPORT(type_description_interfaces,srv,GetTypeDescription);
 if(!s||!s->get_type_hash_func||!s->get_type_description_func||!s->get_type_description_sources_func)return 95;
 auto h=s->get_type_hash_func(s);auto d=s->get_type_description_func(s);auto v=s->get_type_description_sources_func(s);
 type_description_interfaces::srv::GetTypeDescription::Request q;
 return h&&d&&v&&q.include_type_sources?0:1;}
]=] UWVM_CONTROL_REAL_SOURCES_API)
  set(${result} "${UWVM_CONTROL_REAL_SOURCES_API}" PARENT_SCOPE)
 endfunction()
 _uwvm_control_actual_sources_link(_linked)
 if(NOT _linked)
  message(FATAL_ERROR "Complete genuine modern source/hash/description API must compile AND LINK")
 endif()
 add_executable(uwvm_control_native_sources_true native_sources_true.cc)
 # Real fast_io parse_by_scan/decimal scanner uses its actual concepts API.
 target_compile_features(uwvm_control_native_sources_true PRIVATE cxx_std_20)
 if(NOT EXISTS "${UWVM_CONTROL_REPOSITORY}/third-parties/fast_io/include/fast_io.h")
  message(FATAL_ERROR "Actual source-pinned fast_io headers required for strict Native CLI scanning")
 endif()
 target_include_directories(uwvm_control_native_sources_true PRIVATE "${UWVM_CONTROL_GENERATED}" "${UWVM_CONTROL_REPOSITORY}/src" "${UWVM_CONTROL_REPOSITORY}/third-parties/fast_io/include")
 target_link_libraries(uwvm_control_native_sources_true PRIVATE ${_native_targets})
endif()
# No add_test, try_run, generated source command, native subprocess, transport
# startup, automatic acquisition or configuration mutation is performed here.
