# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(uwvm_original_signal_bootstrap LANGUAGES CXX)
find_package(rclcpp REQUIRED)
find_package(uwvm_rmw_threaded_platform REQUIRED)
set(_signal uwvm_rmw_threaded_platform::uwvm_rmw_wasi_signal)
get_target_property(_feature "${_signal}" UWVM_WASI_SIGNAL_ADMITTED)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "WASI" OR NOT _feature STREQUAL "1")
  message(FATAL_ERROR "Actual original-threaded WASI source closure is required")
endif()
foreach(_role UWVM_SIGNAL_READ_FD UWVM_SIGNAL_SOURCE_COOKIE)
  if(NOT DEFINED ${_role} OR NOT "${${_role}}" MATCHES "^[0-9]+$")
    message(FATAL_ERROR "Supply actual immutable numeric role: ${_role}")
  endif()
endforeach()
# This is a concrete original program body/bootstrap library. The outer Root-
# admitted profile/initial-main supplies its real platform/B/C preparation and
# finalization; no unresolved weak/fake profile functions are emitted here.
add_library(uwvm_original_signal_bootstrap STATIC
  ../src/original_bootstrap.cpp original_context.cpp)
target_compile_features(uwvm_original_signal_bootstrap PUBLIC cxx_std_17)
target_link_libraries(uwvm_original_signal_bootstrap PUBLIC rclcpp::rclcpp "${_signal}")
target_compile_definitions(uwvm_original_signal_bootstrap PRIVATE
  UWVM_SIGNAL_READ_FD=${UWVM_SIGNAL_READ_FD}
  UWVM_SIGNAL_SOURCE_COOKIE=${UWVM_SIGNAL_SOURCE_COOKIE}ULL)
