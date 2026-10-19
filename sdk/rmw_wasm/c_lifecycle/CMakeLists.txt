# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_c_service_lifecycle LANGUAGES CXX)
find_package(ament_cmake REQUIRED)
option(UWVM_ENABLE_ORIGINAL_RCLCPP_C_LIFECYCLE "Explicit fixed-original source-recompiled C ownership port" OFF)
option(UWVM_C_LIFECYCLE_BUILD_CONTRACTS "Build authored actual-SDK native contracts; never auto-run" OFF)
option(UWVM_C_LIFECYCLE_BUILD_EXAMPLE "Build actual generated-C owned-value example" OFF)
set(UWVM_C_LIFECYCLE_MODE "" CACHE STRING "Explicit STATIC_WASI or NATIVE_CONTRACT")
set(UWVM_C_LIFECYCLE_LINK_ADMITTED 0)
if(UWVM_ENABLE_ORIGINAL_RCLCPP_C_LIFECYCLE)
 if(NOT UWVM_C_LIFECYCLE_MODE STREQUAL "STATIC_WASI" AND NOT UWVM_C_LIFECYCLE_MODE STREQUAL "NATIVE_CONTRACT")
  message(FATAL_ERROR "Select the real Guest toolchain or separate native-contract mode")
 endif()
 if(UWVM_C_LIFECYCLE_MODE STREQUAL "STATIC_WASI" AND NOT CMAKE_SYSTEM_NAME STREQUAL "WASI")
  message(FATAL_ERROR "STATIC_WASI requires a real selected WASI toolchain")
 endif()
 find_package(type_description_interfaces REQUIRED)
 find_package(rosidl_runtime_c REQUIRED)
 set(_gtd type_description_interfaces::type_description_interfaces__rosidl_generator_c)
 set(_runtime rosidl_runtime_c::rosidl_runtime_c)
 foreach(_target "${_gtd}" "${_runtime}")
  if(NOT TARGET "${_target}")
   message(FATAL_ERROR "Actual generated-C/runtime target required: ${_target}")
  endif()
  if(UWVM_C_LIFECYCLE_MODE STREQUAL "STATIC_WASI")
   get_target_property(_kind "${_target}" TYPE)
   if(NOT _kind STREQUAL "STATIC_LIBRARY")
    message(FATAL_ERROR "All Guest C ownership libraries must be actual static targets")
   endif()
  endif()
 endforeach()
 # Function scope restores all caller settings, including a cross toolchain's
 # STATIC_LIBRARY try-compile shortcut. This is a genuine compile AND LINK, no run.
 function(_uwvm_c_lifecycle_probe result)
  include(CheckCXXSourceCompiles)
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  set(CMAKE_REQUIRED_LIBRARIES "${_gtd}" "${_runtime}")
  unset(UWVM_C_LIFECYCLE_REAL_C_API)
  unset(UWVM_C_LIFECYCLE_REAL_C_API CACHE)
  check_cxx_source_compiles([=[
#include <type_traits>
#include <type_description_interfaces/srv/detail/get_type_description__struct.h>
#include <type_description_interfaces/srv/detail/get_type_description__functions.h>
using Q=type_description_interfaces__srv__GetTypeDescription_Request;
using S=type_description_interfaces__srv__GetTypeDescription_Response;
static_assert(std::is_standard_layout<Q>::value&&std::is_trivially_copyable<Q>::value,"actual C request");
static_assert(std::is_standard_layout<S>::value&&std::is_trivially_copyable<S>::value,"actual C response");
static bool(*volatile qi)(Q*)=&type_description_interfaces__srv__GetTypeDescription_Request__init;
static bool(*volatile si)(S*)=&type_description_interfaces__srv__GetTypeDescription_Response__init;
static void(*volatile qf)(Q*)=&type_description_interfaces__srv__GetTypeDescription_Request__fini;
static void(*volatile sf)(S*)=&type_description_interfaces__srv__GetTypeDescription_Response__fini;
int main(){Q q{};S s{};bool a=qi(&q),b=si(&s);qf(&q);sf(&s);return a&&b?0:1;}
]=] UWVM_C_LIFECYCLE_REAL_C_API)
  set(${result} "${UWVM_C_LIFECYCLE_REAL_C_API}" PARENT_SCOPE)
 endfunction()
 _uwvm_c_lifecycle_probe(_actual_link)
 if(NOT _actual_link)
  message(FATAL_ERROR "Complete actual generated-C init/fini headers and linker symbols required")
 endif()
 set(UWVM_C_LIFECYCLE_LINK_ADMITTED 1)
 add_library(uwvm_rmw_c_service_lifecycle INTERFACE)
 target_compile_features(uwvm_rmw_c_service_lifecycle INTERFACE cxx_std_17)
 target_compile_definitions(uwvm_rmw_c_service_lifecycle INTERFACE UWVM_RCLCPP_C_LIFECYCLE_PORT=1)
 target_include_directories(uwvm_rmw_c_service_lifecycle INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/uwvm_rmw_c_service_lifecycle>")
 target_link_libraries(uwvm_rmw_c_service_lifecycle INTERFACE "${_gtd}" "${_runtime}")
 set_property(TARGET uwvm_rmw_c_service_lifecycle PROPERTY UWVM_C_LIFECYCLE_LINK_ADMITTED 1)
 set_property(TARGET uwvm_rmw_c_service_lifecycle PROPERTY EXPORT_PROPERTIES UWVM_C_LIFECYCLE_LINK_ADMITTED)
 install(TARGETS uwvm_rmw_c_service_lifecycle EXPORT uwvm_rmw_c_service_lifecycle)
 ament_export_targets(uwvm_rmw_c_service_lifecycle HAS_LIBRARY_TARGET)
endif()
if(UWVM_C_LIFECYCLE_BUILD_CONTRACTS OR UWVM_C_LIFECYCLE_BUILD_EXAMPLE)
 if(NOT UWVM_ENABLE_ORIGINAL_RCLCPP_C_LIFECYCLE)
  message(FATAL_ERROR "Explicit source port must be enabled; no OFF-profile stubs")
 endif()
endif()
if(UWVM_C_LIFECYCLE_BUILD_CONTRACTS)
 add_subdirectory(contracts)
endif()
if(UWVM_C_LIFECYCLE_BUILD_EXAMPLE)
 add_executable(uwvm_c_owned_values examples/owned_values.cpp)
 target_link_libraries(uwvm_c_owned_values PRIVATE uwvm_rmw_c_service_lifecycle)
endif()
install(DIRECTORY include/ DESTINATION include/uwvm_rmw_c_service_lifecycle)
install(DIRECTORY upstream/ DESTINATION share/${PROJECT_NAME}/upstream)
install(FILES cmake/FixedOriginalPort.cmake DESTINATION share/${PROJECT_NAME}/cmake)
install(FILES README.md NOTICE LICENSE recipe.json DESTINATION share/${PROJECT_NAME})
configure_file(cmake/dependencies.cmake.in dependencies.cmake @ONLY)
ament_package(CONFIG_EXTRAS "${CMAKE_CURRENT_BINARY_DIR}/dependencies.cmake" cmake/FixedOriginalPort.cmake)
