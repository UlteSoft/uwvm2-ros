cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_service_transport LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
find_package(rmw REQUIRED)
find_package(rcutils REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
if(NOT IS_ABSOLUTE "${UWVM_RMW_INCLUDE_DIR}" OR NOT EXISTS "${UWVM_RMW_INCLUDE_DIR}/uwvm_rmw_c/provider.h")
 message(FATAL_ERROR "Actual complete current B/C/CPP10 headers are required")
endif()
if(NOT IS_ABSOLUTE "${UWVM_ROS_SDK_INCLUDE_DIR}" OR NOT EXISTS "${UWVM_ROS_SDK_INCLUDE_DIR}/roswasm/rmw_service_v1.h")
 message(FATAL_ERROR "Actual independently closed Root raw12 source is required; declarations alone are not Runtime support")
endif()
include(CheckCSourceCompiles)
set(CMAKE_REQUIRED_LIBRARIES rmw::rmw)
# Recheck the actual selected original SDK on every configure. A stale cache or
# caller -D value must not stand in for the complete signed layout compile/link.
unset(UWVM_RMW_SERVICE_INFO_API)
unset(UWVM_RMW_SERVICE_INFO_API CACHE)
check_c_source_compiles([=[
#include <rmw/types.h>
#include <stdint.h>
#include <stddef.h>
_Static_assert(RMW_GID_STORAGE_SIZE==16,"actual complete selected native request writer width");
_Static_assert(sizeof(((rmw_service_info_t*)0)->request_id.writer_guid)==16,"actual complete writer");
_Static_assert(sizeof(rmw_request_id_t)==24&&offsetof(rmw_request_id_t,writer_guid)==0&&offsetof(rmw_request_id_t,sequence_number)==16,"complete selected original request ID fields");
_Static_assert(sizeof(rmw_service_info_t)==40&&offsetof(rmw_service_info_t,source_timestamp)==0&&offsetof(rmw_service_info_t,received_timestamp)==8&&offsetof(rmw_service_info_t,request_id)==16,"complete selected original service info fields; unknown extensions unavailable");
_Static_assert(_Generic(((rmw_service_info_t*)0)->request_id.sequence_number,int64_t:1,default:0),"actual signed sequence");
_Static_assert(_Generic(((rmw_service_info_t*)0)->source_timestamp,int64_t:1,default:0),"actual signed source time");
_Static_assert(_Generic(((rmw_service_info_t*)0)->received_timestamp,int64_t:1,default:0),"actual signed receive time");
_Static_assert(sizeof(((rmw_service_info_t*)0)->request_id.sequence_number)==8,"full native sequence");
_Static_assert(sizeof(((rmw_service_info_t*)0)->source_timestamp)==8,"full source time");
_Static_assert(sizeof(((rmw_service_info_t*)0)->received_timestamp)==8,"full receive time");
int main(void){rmw_service_info_t actual={0};return actual.request_id.writer_guid[0];}
]=] UWVM_RMW_SERVICE_INFO_API)
unset(CMAKE_REQUIRED_LIBRARIES)
configure_file(uwvm_rmw_service_features.h.in "${CMAKE_CURRENT_BINARY_DIR}/uwvm_rmw_service_features.h")
add_library(uwvm_rmw_service_transport STATIC src/service_transport.c)
target_compile_features(uwvm_rmw_service_transport PUBLIC c_std_11)
target_include_directories(uwvm_rmw_service_transport PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${UWVM_RMW_INCLUDE_DIR}" "${UWVM_ROS_SDK_INCLUDE_DIR}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(uwvm_rmw_service_transport PUBLIC rmw::rmw rcutils::rcutils rosidl_runtime_c::rosidl_runtime_c)
# Actual B/C resolves identifier/qos and owns the immutable process singleton.
# Production raw12 symbols have NO definitions here, no weak/fake fallback.
option(UWVM_RMW_SERVICE_BUILD_MODELS "Build actual-SDK/local raw12+raw7 protocol models only" OFF)
option(UWVM_RMW_SERVICE_BUILD_INTEROP_EXAMPLES "Build C API main with actual generated C++ companion" OFF)
option(UWVM_RMW_SERVICE_BUILD_PURE_C_ADD_EXAMPLE "Build genuine C generated AddTwoInts example" OFF)
option(UWVM_RMW_SERVICE_BUILD_NATIVE_PEER "Build separate original native SDK counterparts only" OFF)
if(UWVM_RMW_SERVICE_BUILD_MODELS OR UWVM_RMW_SERVICE_BUILD_INTEROP_EXAMPLES OR UWVM_RMW_SERVICE_BUILD_PURE_C_ADD_EXAMPLE)
 if(NOT TARGET rmw_uwvm OR NOT UWVM_RMW_SERVICE_INFO_API)
  message(FATAL_ERROR "Actual complete current A/B/C target and actual signed full service-info API required")
 endif()
 find_package(example_interfaces REQUIRED)
 find_package(std_srvs REQUIRED)
endif()
if(UWVM_RMW_SERVICE_BUILD_MODELS OR UWVM_RMW_SERVICE_BUILD_INTEROP_EXAMPLES)
 get_property(_cpp TARGET rmw_uwvm PROPERTY UWVM_RMW_CPP_CODECS_ATTACHED)
 if(NOT _cpp)
  message(FATAL_ERROR "Explicitly attach actual reviewed complete CPP10 against same current header before this component")
 endif()
endif()
if(UWVM_RMW_SERVICE_BUILD_MODELS)
 if(NOT TARGET uwvm_rmw_subscription_transport OR NOT EXISTS "${UWVM_RMW_INCLUDE_DIR}/../tests/published_cpp_identity_fixture.hpp")
  message(FATAL_ERROR "Actual reviewed C subscription component and actual CPP10 identity fixture required; no fake substitute provider")
 endif()
 add_executable(uwvm_rmw_service_models tests/model_contract.cc)
 target_compile_features(uwvm_rmw_service_models PRIVATE cxx_std_20)
 target_include_directories(uwvm_rmw_service_models PRIVATE "${UWVM_RMW_INCLUDE_DIR}/../tests")
 target_compile_options(uwvm_rmw_service_models PRIVATE -UNDEBUG)
 target_link_libraries(uwvm_rmw_service_models PRIVATE uwvm_rmw_service_transport uwvm_rmw_subscription_transport rmw_uwvm)
endif()
if(UWVM_RMW_SERVICE_BUILD_INTEROP_EXAMPLES)
 add_library(uwvm_rmw_service_example_objects STATIC examples/native_objects.cpp)
 target_compile_features(uwvm_rmw_service_example_objects PUBLIC cxx_std_20)
 target_link_libraries(uwvm_rmw_service_example_objects PUBLIC rmw_uwvm)
 foreach(_name checked_service checked_availability)
  add_executable(${_name} examples/${_name}.c)
  target_compile_features(${_name} PRIVATE c_std_11)
  target_link_libraries(${_name} PRIVATE uwvm_rmw_service_example_objects uwvm_rmw_service_transport rmw_uwvm)
 endforeach()
endif()
if(UWVM_RMW_SERVICE_BUILD_PURE_C_ADD_EXAMPLE)
 if(NOT EXISTS "${UWVM_RMW_INCLUDE_DIR}/../src/add_two_ints_codec.c")
  message(FATAL_ERROR "Actual existing complete C AddTwoInts codec source required")
 endif()
 add_executable(checked_add_two_ints_c examples/checked_add_two_ints_c.c "${UWVM_RMW_INCLUDE_DIR}/../src/add_two_ints_codec.c")
 target_compile_features(checked_add_two_ints_c PRIVATE c_std_11)
 target_link_libraries(checked_add_two_ints_c PRIVATE uwvm_rmw_service_transport rmw_uwvm
  example_interfaces::example_interfaces__rosidl_generator_c example_interfaces::example_interfaces__rosidl_typesupport_c example_interfaces::example_interfaces__rosidl_typesupport_introspection_c)
endif()
if(UWVM_RMW_SERVICE_BUILD_NATIVE_PEER)
 find_package(rclcpp REQUIRED)
 find_package(std_srvs REQUIRED)
 find_package(example_interfaces REQUIRED)
 add_executable(raw12_native_service_peer tests/native_services.cc)
 target_compile_features(raw12_native_service_peer PRIVATE cxx_std_17)
 target_link_libraries(raw12_native_service_peer PRIVATE rclcpp::rclcpp std_srvs::std_srvs__rosidl_typesupport_cpp example_interfaces::example_interfaces__rosidl_typesupport_cpp)
endif()
# Explicit source-only build recipe. No add_test/try_run/process/network launch.
# Actual native peers and actual static-WASI guests use SEPARATE toolchains.
