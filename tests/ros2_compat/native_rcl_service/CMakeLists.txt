cmake_minimum_required(VERSION 3.20)
project(uwvm_native_direct_rcl_service LANGUAGES CXX)
# Source-only recipe. No CTest registration, try_run, network, generated fallback
# source or executed native probe. Root owns all future compiler/process limits.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT IS_ABSOLUTE "${UWVM_DIRECT_RCL_ROOT}" OR NOT EXISTS "${UWVM_DIRECT_RCL_ROOT}/gateways/ros2/native_rcl_service.h")
  message(FATAL_ERROR "Supply the actual reviewed composed source root")
endif()
set(_cpp "${UWVM_DIRECT_RCL_ROOT}/sdk/rmw_wasm")
foreach(_file gateways/ros2/native_rmw_metadata.h gateways/ros2/qos.h src/uwvm2/imported/ros/ipc/rmw_metadata.h third-parties/fast_io/include/fast_io_freestanding.h)
  if(NOT EXISTS "${UWVM_DIRECT_RCL_ROOT}/${_file}")
    message(FATAL_ERROR "Actual reviewed Gateway/header dependency missing: ${_file}")
  endif()
endforeach()
foreach(_file include/uwvm_rmw/cpp_codecs.hpp include/uwvm_rmw/codec.h src/cpp_codecs.cpp tests/published_cpp_identity_fixture.hpp)
  if(NOT EXISTS "${_cpp}/${_file}")
    message(FATAL_ERROR "Actual separately reviewed C++ codec dependency missing: ${_file}")
  endif()
endforeach()
find_package(ament_cmake REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)
foreach(_package rcl rclcpp rmw rosidl_runtime_c rosidl_runtime_cpp rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp std_msgs std_srvs example_interfaces service_msgs builtin_interfaces geometry_msgs sensor_msgs)
  find_package(${_package} REQUIRED)
endforeach()
# Original installed targets, not aliases manufactured by this recipe.
foreach(_target rcl::rcl rclcpp::rclcpp rmw::rmw std_msgs::std_msgs__rosidl_generator_cpp std_srvs::std_srvs__rosidl_generator_cpp example_interfaces::example_interfaces__rosidl_generator_cpp geometry_msgs::geometry_msgs__rosidl_generator_cpp sensor_msgs::sensor_msgs__rosidl_generator_cpp)
  if(NOT TARGET "${_target}")
    message(FATAL_ERROR "Actual selected SDK target missing: ${_target}")
  endif()
endforeach()
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_LIBRARIES rcl::rcl rclcpp::rclcpp rmw::rmw std_srvs::std_srvs__rosidl_generator_cpp)
# Volatile global function pointers require real linked symbol references. No
# synthetic struct/function is supplied. The program is NEVER run by configure.
check_cxx_source_compiles([=[
#include <rcl/client.h>
#include <rcl/service.h>
#include <rmw/rmw.h>
#include <rmw/time.h>
#include <rclcpp/node_interfaces/node_base_interface.hpp>
#include <std_srvs/srv/empty.hpp>
#include <type_traits>
decltype(&rcl_send_request) volatile a=&rcl_send_request;
decltype(&rcl_take_response_with_info) volatile b=&rcl_take_response_with_info;
decltype(&rcl_take_request_with_info) volatile c=&rcl_take_request_with_info;
decltype(&rcl_send_response) volatile d=&rcl_send_response;
decltype(&rcl_client_request_publisher_get_actual_qos) volatile e=&rcl_client_request_publisher_get_actual_qos;
decltype(&rcl_client_response_subscription_get_actual_qos) volatile f=&rcl_client_response_subscription_get_actual_qos;
decltype(&rcl_service_request_subscription_get_actual_qos) volatile g=&rcl_service_request_subscription_get_actual_qos;
decltype(&rcl_service_response_publisher_get_actual_qos) volatile h=&rcl_service_response_publisher_get_actual_qos;
decltype(&rmw_get_gid_for_client) volatile i=&rmw_get_gid_for_client;
decltype(&rmw_time_normalize) volatile j=&rmw_time_normalize;
decltype(&rcl_client_init) volatile k=&rcl_client_init;
decltype(&rcl_service_init) volatile l=&rcl_service_init;
decltype(&rcl_client_fini) volatile m=&rcl_client_fini;
decltype(&rcl_service_fini) volatile n=&rcl_service_fini;
decltype(&rcl_client_get_rmw_handle) volatile o=&rcl_client_get_rmw_handle;
decltype(&rcl_client_get_service_name) volatile p=&rcl_client_get_service_name;
decltype(&rcl_service_get_service_name) volatile q=&rcl_service_get_service_name;
static_assert(sizeof(rmw_request_id_t::writer_guid)==16);
static_assert(sizeof(rmw_gid_t::data)==16);
static_assert(sizeof(rmw_service_info_t::source_timestamp)==8&&sizeof(rmw_service_info_t::received_timestamp)==8);
static_assert(std::is_signed_v<decltype(rmw_service_info_t::source_timestamp)>);
static_assert(std::is_signed_v<decltype(rmw_service_info_t::received_timestamp)>);
static_assert(sizeof(std_srvs::srv::Empty::Event)>0);
int main(){return a&&b&&c&&d&&e&&f&&g&&h&&i&&j&&k&&l&&m&&n&&o&&p&&q?0:1;}
]=] UWVM_NATIVE_RCL_SERVICE_API)
# Independent real-symbol check: old SDKs may support all direct business APIs
# but lack this readiness API. Such builds keep business support and return95
# from available(); no version label or alternate client enables the query.
check_cxx_source_compiles([=[
#include <rcl/graph.h>
decltype(&rcl_service_server_is_available) volatile query=&rcl_service_server_is_available;
int main(){return query?0:1;}
]=] UWVM_NATIVE_RCL_AVAILABILITY_API)
unset(CMAKE_REQUIRED_LIBRARIES)
configure_file(native_rcl_service_features.h.in "${CMAKE_CURRENT_BINARY_DIR}/native_rcl_service_features.h")
add_library(native-direct-rcl-provider INTERFACE)
target_include_directories(native-direct-rcl-provider INTERFACE "${CMAKE_CURRENT_BINARY_DIR}" "${UWVM_DIRECT_RCL_ROOT}" "${UWVM_DIRECT_RCL_ROOT}/src" "${_cpp}/include" "${UWVM_DIRECT_RCL_ROOT}/sdk/include" "${UWVM_DIRECT_RCL_ROOT}/third-parties/fast_io/include")
target_link_libraries(native-direct-rcl-provider INTERFACE rcl::rcl rclcpp::rclcpp rmw::rmw Threads::Threads OpenSSL::Crypto
  std_msgs::std_msgs__rosidl_generator_cpp std_srvs::std_srvs__rosidl_generator_cpp
  example_interfaces::example_interfaces__rosidl_generator_cpp
  geometry_msgs::geometry_msgs__rosidl_generator_cpp sensor_msgs::sensor_msgs__rosidl_generator_cpp)
add_executable(native-direct-rcl-records records_contract.cc)
target_link_libraries(native-direct-rcl-records PRIVATE native-direct-rcl-provider)
if(UWVM_NATIVE_RCL_SERVICE_API)
  add_library(native-direct-rcl-cpp-codecs STATIC "${_cpp}/src/cpp_codecs.cpp")
  target_include_directories(native-direct-rcl-cpp-codecs PUBLIC "${_cpp}/include" "${UWVM_DIRECT_RCL_ROOT}/sdk/include")
  foreach(_package std_msgs std_srvs example_interfaces service_msgs builtin_interfaces)
    foreach(_provider rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
      set(_actual "${_package}::${_package}__${_provider}")
      if(NOT TARGET "${_actual}")
        message(FATAL_ERROR "Missing actual generated SDK TypeSupport: ${_actual}")
      endif()
      target_link_libraries(native-direct-rcl-cpp-codecs PUBLIC "${_actual}")
    endforeach()
  endforeach()
  # This links only the actual reusable codec component, NOT the Guest rmw_uwvm
  # implementation into a Host native RMW. No static/fake host middleware swap.
  add_executable(native-direct-rcl-contract native_contract.cc)
  target_include_directories(native-direct-rcl-contract PRIVATE "${_cpp}/tests")
  target_link_libraries(native-direct-rcl-contract PRIVATE native-direct-rcl-provider native-direct-rcl-cpp-codecs)
else()
  message(STATUS "Direct RCL native service API unavailable; provider create remains95, native contract not invented")
endif()
