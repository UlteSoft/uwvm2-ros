cmake_minimum_required(VERSION 3.20)
project(uwvm_native_service_registry LANGUAGES CXX)
option(UWVM_BUILD_NATIVE_SERVICE_REGISTRY_COMPONENT "Build only, never automatically execute Native witnesses" OFF)
if(NOT UWVM_BUILD_NATIVE_SERVICE_REGISTRY_COMPONENT)
  return()
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT IS_ABSOLUTE "${UWVM_SERVICE_REGISTRY_ROOT}")
  message(FATAL_ERROR "Supply actual reviewed coherent composed source root")
endif()
foreach(_file gateways/ros2/native_rcl_service_registry.h gateways/ros2/native_rcl_service.h
    gateways/ros2/compat/NativeRclServiceProbe.cmake gateways/ros2/compat/native_rcl_service_features.h.in
    src/uwvm2/imported/ros/common/rmw_service_bindings.h src/uwvm2/imported/ros/ipc/rmw_service.h
    sdk/rmw_wasm/include/uwvm_rmw/cpp_codecs.hpp sdk/rmw_wasm/include/uwvm_rmw/codec.h
    sdk/rmw_wasm/src/cpp_codecs.cpp third-parties/fast_io/include/fast_io.h
    third-parties/fast_io/include/fast_io_freestanding.h)
  if(NOT EXISTS "${UWVM_SERVICE_REGISTRY_ROOT}/${_file}")
    message(FATAL_ERROR "Missing actual reviewed dependency: ${_file}")
  endif()
endforeach()
find_package(ament_cmake REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)
foreach(_package rcl rclcpp rmw rosidl_runtime_c rosidl_runtime_cpp rosidl_typesupport_cpp
    rosidl_typesupport_introspection_cpp std_msgs std_srvs example_interfaces service_msgs builtin_interfaces
    geometry_msgs sensor_msgs)
  find_package(${_package} REQUIRED)
endforeach()
foreach(_target rcl::rcl rclcpp::rclcpp rmw::rmw
    std_msgs::std_msgs__rosidl_generator_cpp std_srvs::std_srvs__rosidl_generator_cpp
    example_interfaces::example_interfaces__rosidl_generator_cpp
    geometry_msgs::geometry_msgs__rosidl_generator_cpp sensor_msgs::sensor_msgs__rosidl_generator_cpp)
  if(NOT TARGET "${_target}")
    message(FATAL_ERROR "Actual selected SDK target missing: ${_target}")
  endif()
endforeach()
include("${UWVM_SERVICE_REGISTRY_ROOT}/gateways/ros2/compat/NativeRclServiceProbe.cmake")
uwvm_ros2_probe_native_rcl_service("${CMAKE_CURRENT_BINARY_DIR}/actual-native-features")
add_library(native-service-registry-headers INTERFACE)
target_compile_features(native-service-registry-headers INTERFACE cxx_std_20)
target_include_directories(native-service-registry-headers INTERFACE
  "${UWVM_NATIVE_RCL_SERVICE_FEATURE_INCLUDE_DIR}" "${UWVM_SERVICE_REGISTRY_ROOT}"
  "${UWVM_SERVICE_REGISTRY_ROOT}/src" "${UWVM_SERVICE_REGISTRY_ROOT}/sdk/include"
  "${UWVM_SERVICE_REGISTRY_ROOT}/sdk/rmw_wasm/include"
  "${UWVM_SERVICE_REGISTRY_ROOT}/third-parties/fast_io/include")
target_link_libraries(native-service-registry-headers INTERFACE rcl::rcl rclcpp::rclcpp rmw::rmw
  Threads::Threads OpenSSL::Crypto std_msgs::std_msgs__rosidl_generator_cpp
  std_srvs::std_srvs__rosidl_generator_cpp example_interfaces::example_interfaces__rosidl_generator_cpp
  geometry_msgs::geometry_msgs__rosidl_generator_cpp sensor_msgs::sensor_msgs__rosidl_generator_cpp)
# Link the actual immutable reusable C++ codec component, never the Guest RMW
# implementation into a Host process or a handwritten substitute middleware.
add_library(native-service-registry-codecs STATIC "${UWVM_SERVICE_REGISTRY_ROOT}/sdk/rmw_wasm/src/cpp_codecs.cpp")
target_link_libraries(native-service-registry-codecs PUBLIC native-service-registry-headers)
foreach(_package std_msgs std_srvs example_interfaces service_msgs builtin_interfaces)
  foreach(_provider rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
    set(_target "${_package}::${_package}__${_provider}")
    if(NOT TARGET "${_target}")
      message(FATAL_ERROR "Actual complete original SDK TypeSupport target missing: ${_target}")
    endif()
    target_link_libraries(native-service-registry-codecs PUBLIC "${_target}")
  endforeach()
endforeach()
add_executable(native-service-registry-values value_contract.cc)
target_link_libraries(native-service-registry-values PRIVATE native-service-registry-codecs)
add_executable(native-service-registry-contract native_contract.cc)
target_link_libraries(native-service-registry-contract PRIVATE native-service-registry-codecs)
# No execute_process/test/try_run/native network side effects. All actual
# compiler/probe commands above occur only in Root's future owned invocation.
