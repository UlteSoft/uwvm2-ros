cmake_minimum_required(VERSION 3.20)
project(uwvm_ros2_rmw_service_policy_contract LANGUAGES CXX)
# Explicitly opt in only after the complete source and resource envelope have
# been admitted. This file installs no auto-running test or substitute ROS SDK.
option(UWVM_BUILD_RMW_SERVICE_POLICY_CONTRACT "Build actual Host/Gateway policy contracts" OFF)
if(UWVM_BUILD_RMW_SERVICE_POLICY_CONTRACT)
  set(root "${CMAKE_CURRENT_LIST_DIR}/../..")
  if(NOT EXISTS "${root}/third-parties/fast_io/include/fast_io.h")
    message(FATAL_ERROR "Require the actual reviewed vendored fast_io headers")
  endif()
  add_executable(uwvm_rmw_service_policy_contract value_contract.cc)
  add_executable(uwvm_rmw_service_max_wait_contract max_wait_contract.cc)
  target_compile_features(uwvm_rmw_service_max_wait_contract PRIVATE cxx_std_20)
  target_include_directories(uwvm_rmw_service_max_wait_contract PRIVATE
    "${root}/src" "${root}/third-parties/fast_io/include")
  target_compile_features(uwvm_rmw_service_policy_contract PRIVATE cxx_std_20)
  target_include_directories(uwvm_rmw_service_policy_contract PRIVATE
    "${root}/src" "${root}/third-parties/fast_io/include")
endif()
