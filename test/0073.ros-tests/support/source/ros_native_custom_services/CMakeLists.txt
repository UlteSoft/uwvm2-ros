cmake_minimum_required(VERSION 3.20)
project(uwvm_owned_native_custom_services LANGUAGES C CXX)
# UNEXECUTED recipe: no try_run, CTest, fetch, native startup or generated stub.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
foreach(key UWVM_CUSTOM_ROOT UWVM_CUSTOM_MANIFEST UWVM_CUSTOM_NATIVE_HEADER)
  if(NOT IS_ABSOLUTE "${${key}}" OR NOT EXISTS "${${key}}" OR "${${key}}" MATCHES "[;\"\\\r\n]")
    message(FATAL_ERROR "Select actual reviewed absolute ${key}")
  endif()
endforeach()
if(NOT IS_DIRECTORY "${UWVM_CUSTOM_ROOT}")
  message(FATAL_ERROR "Actual composed source tree required")
endif()
if(UWVM_CUSTOM_EXPORT_HEADER AND (NOT IS_ABSOLUTE "${UWVM_CUSTOM_EXPORT_HEADER}" OR NOT EXISTS "${UWVM_CUSTOM_EXPORT_HEADER}" OR "${UWVM_CUSTOM_EXPORT_HEADER}" MATCHES "[;\"\\\r\n]"))
  message(FATAL_ERROR "Actual independent original-SDK export header must be an absolute reviewed file")
endif()
find_package(ament_cmake REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)
foreach(p rcl rclcpp rmw std_msgs std_srvs example_interfaces builtin_interfaces service_msgs geometry_msgs sensor_msgs rosidl_runtime_c rosidl_runtime_cpp rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp xarm_msgs)
  find_package(${p} REQUIRED)
endforeach()
set(actual rcl::rcl rclcpp::rclcpp rmw::rmw)
foreach(p std_msgs std_srvs example_interfaces builtin_interfaces service_msgs geometry_msgs sensor_msgs xarm_msgs)
  foreach(kind rosidl_generator_cpp rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
    set(t "${p}::${p}__${kind}")
    if(NOT TARGET ${t})
      message(FATAL_ERROR "Actual installed SDK target absent: ${t}")
    endif()
    list(APPEND actual ${t})
  endforeach()
endforeach()
include("${UWVM_CUSTOM_ROOT}/src/ros/gateway/ros2/compat/Probe.cmake")
include("${UWVM_CUSTOM_ROOT}/src/ros/gateway/ros2/compat/NativeRclServiceProbe.cmake")
uwvm_ros2_probe_native_rcl_service("${CMAKE_CURRENT_BINARY_DIR}")
add_library(custom-contract-base INTERFACE)
target_compile_features(custom-contract-base INTERFACE cxx_std_20)
target_include_directories(custom-contract-base INTERFACE "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/owned-custom-services/owned-custom-services" "${UWVM_CUSTOM_ROOT}" "${UWVM_CUSTOM_ROOT}/src" "${UWVM_CUSTOM_ROOT}/sdk/origin/include" "${UWVM_CUSTOM_ROOT}/sdk/origin/rmw_wasm/include" "${UWVM_CUSTOM_ROOT}/src/ros/gateway/ros2" "${UWVM_CUSTOM_ROOT}/third-parties/fast_io/include")
target_compile_definitions(custom-contract-base INTERFACE UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_CUSTOM_MANIFEST}" UWVM_ROS_CUSTOM_ROS2_HEADER="${UWVM_CUSTOM_NATIVE_HEADER}")
if(UWVM_CUSTOM_EXPORT_HEADER)
  target_compile_definitions(custom-contract-base INTERFACE UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER="${UWVM_CUSTOM_EXPORT_HEADER}")
endif()
target_link_libraries(custom-contract-base INTERFACE ${actual} OpenSSL::Crypto Threads::Threads)
add_library(original-cpp-codecs STATIC "${UWVM_CUSTOM_ROOT}/sdk/origin/rmw_wasm/src/cpp_codecs.cpp")
target_link_libraries(original-cpp-codecs PUBLIC custom-contract-base)
add_library(owned-custom-services STATIC)
target_link_libraries(owned-custom-services PRIVATE custom-contract-base)
include("${UWVM_CUSTOM_ROOT}/src/ros/gateway/ros2/custom_owned_services.cmake")
uwvm_ros2_owned_custom_service_sources(owned-custom-services SDK_TARGETS ${actual})
add_executable(custom-service-records records_contract.cc)
target_link_libraries(custom-service-records PRIVATE custom-contract-base)
target_compile_options(custom-service-records PRIVATE -UNDEBUG)
add_executable(custom-service-link link_contract.cc)
target_link_libraries(custom-service-link PRIVATE owned-custom-services original-cpp-codecs custom-contract-base)
add_executable(custom-service-export export_native_contract.cc)
target_link_libraries(custom-service-export PRIVATE custom-contract-base)
if(UWVM_CUSTOM_EXPORT_HEADER)
  add_executable(custom-service-native native_contract.cc)
  target_compile_options(custom-service-native PRIVATE -UNDEBUG)
  target_link_libraries(custom-service-native PRIVATE owned-custom-services original-cpp-codecs custom-contract-base)
else()
  message(STATUS "No independent export: typed factories stay95; no positive Native contract target is fabricated")
endif()
add_executable(custom-service-xarm-peer native_xarm_peer.cc)
target_link_libraries(custom-service-xarm-peer PRIVATE rclcpp::rclcpp xarm_msgs::xarm_msgs__rosidl_typesupport_cpp xarm_msgs::xarm_msgs__rosidl_generator_cpp)

# Real C++17-compatible Native diagnostic adapter, no stdio/iostream shim.
target_include_directories(custom-service-xarm-peer PRIVATE "${UWVM_CUSTOM_ROOT}")
target_link_libraries(custom-service-xarm-peer PRIVATE Threads::Threads)

# Complete scalar-default C/CPP wire/native companions. Existing targets and
# original Native/RMW/Gateway interfaces remain unchanged. No configure/run was
# performed by the author. These are executable COMPILE/LINK probes, not tests.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
get_filename_component(scalar_bundle "${UWVM_CUSTOM_MANIFEST}" DIRECTORY)
if(NOT EXISTS "${scalar_bundle}/bounded_records.h" OR NOT EXISTS "${scalar_bundle}/bounded_records.hpp")
  message(FATAL_ERROR "Select the actual whole generated scalar bundle with C/C++ records; no stub header")
endif()
find_package(rosidl_typesupport_c REQUIRED)
# The public C observation header always names the real rosidl support types,
# including the feature0/95 branch. find_package alone supplies no include
# usage requirements. A missing xArm C target is a supported95 witness; a
# missing mandatory runtime target cannot compile this public API and must fail
# configuration explicitly, before any probe or original C object operation.
if(NOT TARGET rosidl_runtime_c::rosidl_runtime_c)
  message(FATAL_ERROR "Actual rosidl_runtime_c target required for scalar witness public headers")
endif()
include(CheckCSourceCompiles)
set(scalar_c_targets xarm_msgs::xarm_msgs__rosidl_generator_c xarm_msgs::xarm_msgs__rosidl_typesupport_c rosidl_runtime_c::rosidl_runtime_c)
set(scalar_c_present TRUE)
foreach(t IN LISTS scalar_c_targets)
  if(NOT TARGET ${t})
    set(scalar_c_present FALSE)
  endif()
endforeach()
# A reused builddir or caller -D=1 is NOT actual SDK field/layout/link evidence.
unset(UWVM_CUSTOM_SCALAR_C_METADATA_API)
unset(UWVM_CUSTOM_SCALAR_C_METADATA_API CACHE)
if(scalar_c_present)
  set(CMAKE_REQUIRED_LIBRARIES ${scalar_c_targets})
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  check_c_source_compiles([=[
#include <xarm_msgs/srv/get_set_modbus_data.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/primitives_sequence_functions.h>
#include <stdbool.h>
int main(void){
 xarm_msgs__srv__GetSetModbusData_Request q;
 xarm_msgs__srv__GetSetModbusData_Response s;
 xarm_msgs__srv__GetSetModbusData_Event e;
 _Static_assert(sizeof q.modbus_length==2&&sizeof q.ret_length==2&&sizeof q.host_id==1,"actual complete original fields");
 q.modbus_length=0;q.ret_length=0;q.host_id=9;q.is_transparent_transmission=false;q.use_503_port=false;
 (void)sizeof q.modbus_data.capacity;(void)sizeof s.ret;(void)sizeof s.message.capacity;(void)sizeof s.ret_data.capacity;
 (void)sizeof e.info;(void)sizeof e.request.capacity;(void)sizeof e.response.capacity;
 bool (*volatile init)(xarm_msgs__srv__GetSetModbusData_Request*)=&xarm_msgs__srv__GetSetModbusData_Request__init;
 void (*volatile fini)(xarm_msgs__srv__GetSetModbusData_Request*)=&xarm_msgs__srv__GetSetModbusData_Request__fini;
 if(!init(&q))return 1;fini(&q);
 if(!xarm_msgs__srv__GetSetModbusData_Response__init(&s))return 1;
 xarm_msgs__srv__GetSetModbusData_Response__fini(&s);
 const rosidl_message_type_support_t *a=ROSIDL_GET_MSG_TYPE_SUPPORT(xarm_msgs,srv,GetSetModbusData_Request);
 const rosidl_message_type_support_t *b=ROSIDL_GET_MSG_TYPE_SUPPORT(xarm_msgs,srv,GetSetModbusData_Response);
 const rosidl_message_type_support_t *c=ROSIDL_GET_MSG_TYPE_SUPPORT(xarm_msgs,srv,GetSetModbusData_Event);
 const rosidl_service_type_support_t *r=ROSIDL_GET_SRV_TYPE_SUPPORT(xarm_msgs,srv,GetSetModbusData);
 return !(a&&b&&c&&r&&a->get_type_hash_func&&a->get_type_description_func&&a->get_type_description_sources_func&&
 b->get_type_description_func&&c->get_type_description_func&&r->get_type_hash_func&&r->get_type_description_func&&
 r->event_message_create_handle_function&&r->event_message_destroy_handle_function&&r->event_typesupport);
}
]=] UWVM_CUSTOM_SCALAR_C_METADATA_API)
else()
  # Known absent actual targets: source compiles an explicit unsupported95
  # witness, never a fake successful native C implementation.
  set(UWVM_CUSTOM_SCALAR_C_METADATA_API 0)
endif()
configure_file(scalar_contract_features.h.in "${CMAKE_CURRENT_BINARY_DIR}/scalar_contract_features.h")
add_library(custom-service-scalar-c OBJECT scalar_c_contract.c)
target_include_directories(custom-service-scalar-c PRIVATE "${CMAKE_CURRENT_BINARY_DIR}" "${scalar_bundle}")
target_link_libraries(custom-service-scalar-c PRIVATE rosidl_runtime_c::rosidl_runtime_c)
if(scalar_c_present)
  target_link_libraries(custom-service-scalar-c PRIVATE ${scalar_c_targets})
endif()
if(UWVM_CUSTOM_EXPORT_HEADER)
  add_executable(custom-service-scalar-native scalar_native_contract.cc $<TARGET_OBJECTS:custom-service-scalar-c>)
  target_compile_options(custom-service-scalar-native PRIVATE -UNDEBUG)
  target_include_directories(custom-service-scalar-native PRIVATE "${scalar_bundle}")
  target_link_libraries(custom-service-scalar-native PRIVATE owned-custom-services original-cpp-codecs custom-contract-base rosidl_runtime_c::rosidl_runtime_c)
  if(scalar_c_present)
    target_link_libraries(custom-service-scalar-native PRIVATE ${scalar_c_targets})
  endif()
endif()
add_executable(custom-service-modbus-peer native_modbus_peer.cc)
target_include_directories(custom-service-modbus-peer PRIVATE "${UWVM_CUSTOM_ROOT}")
target_link_libraries(custom-service-modbus-peer PRIVATE rclcpp::rclcpp xarm_msgs::xarm_msgs__rosidl_typesupport_cpp xarm_msgs::xarm_msgs__rosidl_generator_cpp Threads::Threads)
