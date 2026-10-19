cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_c_codecs LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(UWVM_RMW_C_CODECS_MODE "" CACHE STRING "Explicit STATIC_WASI or NATIVE_CONTRACT")
if(NOT UWVM_RMW_C_CODECS_MODE STREQUAL "STATIC_WASI" AND NOT UWVM_RMW_C_CODECS_MODE STREQUAL "NATIVE_CONTRACT")
 message(FATAL_ERROR "Select actual static-WASI or separate original-native contract mode")
endif()
find_package(rmw REQUIRED)
find_package(rcutils REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
find_package(rosidl_typesupport_introspection_c REQUIRED)
find_package(rosidl_cmake REQUIRED)
if(NOT IS_ABSOLUTE "${UWVM_RMW_INCLUDE_DIR}" OR NOT EXISTS "${UWVM_RMW_INCLUDE_DIR}/uwvm_rmw/codec.h")
 message(FATAL_ERROR "Actual current source-recompiled codec.h with validation_state required")
endif()
if(NOT IS_ABSOLUTE "${UWVM_ROS_SDK_INCLUDE_DIR}" OR NOT EXISTS "${UWVM_ROS_SDK_INCLUDE_DIR}/roswasm/codec.h")
 message(FATAL_ERROR "Actual reviewed canonical SDK headers required")
endif()
set(_actual_targets)
foreach(_package std_msgs std_srvs example_interfaces service_msgs builtin_interfaces)
 find_package(${_package} REQUIRED)
 foreach(_provider rosidl_generator_c rosidl_typesupport_c rosidl_typesupport_introspection_c)
  if(TARGET "${_package}::${_package}__${_provider}")
   set(_actual "${_package}::${_package}__${_provider}")
  elseif(TARGET "${_package}" AND NOT _provider STREQUAL "rosidl_generator_c")
   rosidl_get_typesupport_target(_actual "${_package}" "${_provider}")
  else()
   message(FATAL_ERROR "Actual generated ${_package}/${_provider} target is required; no imported shim")
  endif()
  if(NOT _actual OR NOT TARGET "${_actual}")
   message(FATAL_ERROR "Actual generated target missing")
  endif()
  if(UWVM_RMW_C_CODECS_MODE STREQUAL "STATIC_WASI")
   get_target_property(_kind "${_actual}" TYPE)
   if(NOT _kind STREQUAL "STATIC_LIBRARY")
    message(FATAL_ERROR "Every actual generated Guest provider must be static")
   endif()
  endif()
  list(APPEND _actual_targets "${_actual}")
 endforeach()
endforeach()
# Real original headers AND linker symbols are required. Never execute the
# probe. Reprobe after SDK changes and refuse caller -D/cache feature claims.
include(CheckCSourceCompiles)
unset(UWVM_RMW_C_CODECS_COMPLETE_API)
unset(UWVM_RMW_C_CODECS_COMPLETE_API CACHE)
set(CMAKE_REQUIRED_LIBRARIES ${_actual_targets} rosidl_typesupport_introspection_c::rosidl_typesupport_introspection_c rosidl_runtime_c::rosidl_runtime_c)
if(DEFINED CMAKE_TRY_COMPILE_TARGET_TYPE)
 set(_had_try_compile_type TRUE)
 set(_saved_try_compile_type "${CMAKE_TRY_COMPILE_TARGET_TYPE}")
endif()
# A cross toolchain's STATIC_LIBRARY shortcut is not a link proof.
set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
check_c_source_compiles([=[
#include <std_msgs/msg/string.h>
#include <std_srvs/srv/empty.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <service_msgs/msg/service_event_info.h>
#include <builtin_interfaces/msg/time.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <rosidl_runtime_c/type_description/field_type__struct.h>
#include <rosidl_typesupport_introspection_c/identifier.h>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/service_introspection.h>
#include <stdint.h>
#include <stddef.h>
static rosidl_message_type_support_t const*volatile messages[9];
static rosidl_service_type_support_t const*volatile services[2];
_Static_assert(sizeof(((std_srvs__srv__Empty_Request*)0)->structure_needs_at_least_one_member)==1,"actual Empty placeholder");
_Static_assert(_Generic(((example_interfaces__srv__AddTwoInts_Request*)0)->a,int64_t:1,default:0),"actual signed int64");
_Static_assert(sizeof(((service_msgs__msg__ServiceEventInfo*)0)->client_gid)==16,"actual full fixed Event GID");
int main(void){
 messages[0]=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String);
 messages[1]=ROSIDL_GET_MSG_TYPE_SUPPORT(builtin_interfaces,msg,Time);
 messages[2]=ROSIDL_GET_MSG_TYPE_SUPPORT(service_msgs,msg,ServiceEventInfo);
 messages[3]=ROSIDL_GET_MSG_TYPE_SUPPORT(std_srvs,srv,Empty_Request);
 messages[4]=ROSIDL_GET_MSG_TYPE_SUPPORT(std_srvs,srv,Empty_Response);
 messages[5]=ROSIDL_GET_MSG_TYPE_SUPPORT(std_srvs,srv,Empty_Event);
 messages[6]=ROSIDL_GET_MSG_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts_Request);
 messages[7]=ROSIDL_GET_MSG_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts_Response);
 messages[8]=ROSIDL_GET_MSG_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts_Event);
 services[0]=ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs,srv,Empty);services[1]=ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts);
 rosidl_message_type_support_t const*m=messages[0];rosidl_service_type_support_t const*s=services[0];
 rosidl_typesupport_introspection_c__MessageMember f={0};rosidl_typesupport_introspection_c__MessageMembers mm={0};
 rosidl_typesupport_introspection_c__ServiceMembers sm={0};
 rosidl_runtime_c__type_description__TypeDescription const*d=m->get_type_description_func(m);
 rosidl_runtime_c__type_description__TypeSource__Sequence const*src=m->get_type_description_sources_func(m);
 (void)m->get_type_hash_func(m)->version;(void)s->get_type_hash_func(s);(void)s->get_type_description_func(s);(void)s->get_type_description_sources_func(s);
 (void)s->request_typesupport;(void)s->response_typesupport;(void)s->event_typesupport;
 (void)s->event_message_create_handle_function;(void)s->event_message_destroy_handle_function;
 rosidl_event_message_create_handle_function_function volatile create_event=ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(rosidl_typesupport_c,std_srvs,srv,Empty);
 rosidl_event_message_destroy_handle_function_function volatile destroy_event=ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(rosidl_typesupport_c,example_interfaces,srv,AddTwoInts);
 (void)create_event;(void)destroy_event;
 (void)sm.event_members_;(void)f.is_key_;(void)mm.has_any_key_member_;(void)f.fetch_function;(void)f.assign_function;(void)f.resize_function;
 (void)d->type_description.fields.data[0].default_value;(void)src->data[0].raw_file_contents;
 (void)rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE;
 rosidl_runtime_c__String str={0};if(rosidl_runtime_c__String__init(&str)){
  (void)rosidl_runtime_c__String__assignn(&str,"x",1);rosidl_runtime_c__String__fini(&str);}
 return m->func(m,rosidl_typesupport_introspection_c__identifier)==NULL;
}
]=] UWVM_RMW_C_CODECS_COMPLETE_API)
unset(CMAKE_REQUIRED_LIBRARIES)
if(_had_try_compile_type)
 set(CMAKE_TRY_COMPILE_TARGET_TYPE "${_saved_try_compile_type}")
else()
 unset(CMAKE_TRY_COMPILE_TARGET_TYPE)
endif()
configure_file(uwvm_rmw_c_codecs_features.h.in "${CMAKE_CURRENT_BINARY_DIR}/uwvm_rmw_c_codecs_features.h")
add_library(uwvm_rmw_c_codecs STATIC src/c_codecs.c)
target_compile_features(uwvm_rmw_c_codecs PUBLIC c_std_11)
target_include_directories(uwvm_rmw_c_codecs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${UWVM_RMW_INCLUDE_DIR}" "${UWVM_ROS_SDK_INCLUDE_DIR}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(uwvm_rmw_c_codecs PUBLIC ${_actual_targets} rcutils::rcutils rosidl_runtime_c::rosidl_runtime_c rmw::rmw rosidl_typesupport_introspection_c::rosidl_typesupport_introspection_c)
option(UWVM_RMW_C_CODECS_BUILD_CONTRACT "Build unexecuted actual-SDK contracts" OFF)
option(UWVM_RMW_C_CODECS_BUILD_EXAMPLES "Build explicit actual original-RMW pure-C bootstraps" OFF)
if(UWVM_RMW_C_CODECS_BUILD_CONTRACT)
 if(NOT UWVM_RMW_C_CODECS_MODE STREQUAL "NATIVE_CONTRACT" OR NOT UWVM_RMW_C_CODECS_COMPLETE_API)
  message(FATAL_ERROR "Actual complete original native SDK is mandatory for contracts")
 endif()
 add_executable(uwvm_rmw_c_codec_contract tests/codec_contract.c)
 target_link_libraries(uwvm_rmw_c_codec_contract PRIVATE uwvm_rmw_c_codecs)
 target_compile_options(uwvm_rmw_c_codec_contract PRIVATE -UNDEBUG)
endif()
if(UWVM_RMW_C_CODECS_BUILD_EXAMPLES)
 foreach(_needed rmw_uwvm uwvm_rmw_service_transport uwvm_rmw_subscription_transport uwvm_rmw_publisher_transport)
  if(NOT TARGET "${_needed}")
   message(FATAL_ERROR "Actual reviewed ${_needed} target required, no source stub/companion")
  endif()
 endforeach()
 if(NOT UWVM_RMW_C_CODECS_COMPLETE_API)
  message(FATAL_ERROR "Actual complete generated C API required for examples")
 endif()
 foreach(_name checked_service_c checked_string_c checked_composite_c)
  add_executable(${_name} examples/${_name}.c)
  target_compile_features(${_name} PRIVATE c_std_11)
  target_link_libraries(${_name} PRIVATE uwvm_rmw_c_codecs rmw_uwvm uwvm_rmw_service_transport uwvm_rmw_subscription_transport uwvm_rmw_publisher_transport)
 endforeach()
endif()
# No add_test/try_run/process/Native execution at configure or build admission.

option(UWVM_RMW_C_CODECS_BUILD_NATIVE_PEERS "Build separate original native counterparts only" OFF)
if(UWVM_RMW_C_CODECS_BUILD_NATIVE_PEERS)
 if(NOT UWVM_RMW_C_CODECS_MODE STREQUAL "NATIVE_CONTRACT")
  message(FATAL_ERROR "Independent native peers require a separate original-native toolchain")
 endif()
 enable_language(CXX)
 find_package(rclcpp REQUIRED)
 find_package(Threads REQUIRED)
 add_executable(pure_c_native_service_peer tests/native_service_peer.cc)
 target_compile_features(pure_c_native_service_peer PRIVATE cxx_std_17)
 target_link_libraries(pure_c_native_service_peer PRIVATE rclcpp::rclcpp Threads::Threads std_srvs::std_srvs__rosidl_typesupport_cpp example_interfaces::example_interfaces__rosidl_typesupport_cpp)
 add_executable(pure_c_native_string_peer tests/native_string_peer.cc)
 target_compile_features(pure_c_native_string_peer PRIVATE cxx_std_17)
 target_link_libraries(pure_c_native_string_peer PRIVATE rclcpp::rclcpp Threads::Threads std_msgs::std_msgs__rosidl_typesupport_cpp)
 # No rmw_uwvm, Registry or Guest provider target is linked to either peer.
endif()
