cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_publisher_transport LANGUAGES C)
# Add this real optional component to one complete reviewed B/C/A build. The
# independent sdk/compat/rclc source facade is NOT an upstream RMW SDK.
find_package(rmw REQUIRED)
find_package(rcutils REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
if(NOT DEFINED UWVM_RMW_INCLUDE_DIR OR NOT EXISTS "${UWVM_RMW_INCLUDE_DIR}/uwvm_rmw/bridge.h")
  message(FATAL_ERROR "UWVM_RMW_INCLUDE_DIR must contain the actual reviewed B bridge headers")
endif()
if(NOT DEFINED UWVM_ROS_SDK_INCLUDE_DIR OR NOT EXISTS "${UWVM_ROS_SDK_INCLUDE_DIR}/roswasm/rmw_publisher_v1.h")
  message(FATAL_ERROR "Actual reviewed optional raw header required; no substitute is generated")
endif()
add_library(uwvm_rmw_publisher_transport STATIC src/publisher_transport.c)
target_compile_features(uwvm_rmw_publisher_transport PUBLIC c_std_11)
target_include_directories(uwvm_rmw_publisher_transport PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include" "${UWVM_RMW_INCLUDE_DIR}" "${UWVM_ROS_SDK_INCLUDE_DIR}")
target_link_libraries(uwvm_rmw_publisher_transport PUBLIC rmw::rmw rcutils::rcutils rosidl_runtime_c::rosidl_runtime_c)
# The production object refers directly to exactly five raw symbols. No weak
# definitions, host callback shim, imported-function table or original81 path
# makes it link or run when those actual selected imports are unavailable.
option(UWVM_RMW_PUBLISHER_TRANSPORT_BUILD_MODELS "Build explicit local raw protocol models, not native witnesses" OFF)
if(UWVM_RMW_PUBLISHER_TRANSPORT_BUILD_MODELS)
  add_executable(uwvm_rmw_publisher_transport_models
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../tests/ros_rmw_native/publisher_transport/model_contract.c")
  target_link_libraries(uwvm_rmw_publisher_transport_models PRIVATE uwvm_rmw_publisher_transport)
  target_compile_features(uwvm_rmw_publisher_transport_models PRIVATE c_std_11)
  target_compile_options(uwvm_rmw_publisher_transport_models PRIVATE -UNDEBUG)
endif()
