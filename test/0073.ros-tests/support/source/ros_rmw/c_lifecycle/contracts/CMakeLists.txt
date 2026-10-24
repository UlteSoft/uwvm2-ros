# SPDX-License-Identifier: Apache-2.0
if(NOT UWVM_C_LIFECYCLE_MODE STREQUAL "NATIVE_CONTRACT" OR NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
 message(FATAL_ERROR "These delegated real-fini linker-wrap contracts require separate original Linux native SDK")
endif()
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
 message(FATAL_ERROR "A real linker supporting --wrap is required; no replacement C functions")
endif()
find_package(rclcpp REQUIRED)
find_package(rcl REQUIRED)
find_package(rcutils REQUIRED)
foreach(_target type_description_interfaces::type_description_interfaces__rosidl_generator_c rosidl_runtime_c::rosidl_runtime_c rcutils::rcutils)
 get_target_property(_kind "${_target}" TYPE)
 if(NOT _kind STREQUAL "STATIC_LIBRARY")
  message(FATAL_ERROR "Actual static C libraries required for the real allocator-fault interposition contract")
 endif()
endforeach()
add_executable(uwvm_c_service_ownership ownership_contract.cpp header_compile.cpp)
target_compile_features(uwvm_c_service_ownership PRIVATE cxx_std_17)
target_link_libraries(uwvm_c_service_ownership PRIVATE uwvm_rmw_c_service_lifecycle rclcpp::rclcpp rcl::rcl rcutils::rcutils)
target_link_options(uwvm_c_service_ownership PRIVATE
 "-Wl,--wrap=type_description_interfaces__srv__GetTypeDescription_Request__fini"
 "-Wl,--wrap=type_description_interfaces__srv__GetTypeDescription_Response__fini"
 "-Wl,--wrap=malloc")
option(UWVM_C_LIFECYCLE_CONTRACT_SANITIZERS "Use real native Address/LeakSanitizer instrumentation" OFF)
if(UWVM_C_LIFECYCLE_CONTRACT_SANITIZERS)
 target_compile_options(uwvm_c_service_ownership PRIVATE -fsanitize=address,leak -fno-omit-frame-pointer)
 target_link_options(uwvm_c_service_ownership PRIVATE -fsanitize=address,leak)
endif()
# Authored executable only. No configure-time try_run, add_test, automatic ROS
# process, graph probe or native transport is started by this source package.
