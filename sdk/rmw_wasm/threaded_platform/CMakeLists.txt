# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_threaded_platform LANGUAGES CXX)
find_package(ament_cmake REQUIRED)
option(UWVM_BUILD_ORIGINAL_RCLCPP_WASI_SIGNAL "Build the actual explicit WASI signal transport" OFF)
if(BUILD_SHARED_LIBS)
  message(FATAL_ERROR "This fixed threaded Wasm-EH profile uses actual static libraries")
endif()
set(UWVM_RMW_WASI_SIGNAL_P1_API 0)
if(UWVM_BUILD_ORIGINAL_RCLCPP_WASI_SIGNAL)
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "WASI")
    message(FATAL_ERROR "An admitted genuine WASI toolchain is required; no native stubs")
  endif()
  include(cmake/Probe.cmake)
  uwvm_probe_wasi_signal(UWVM_RMW_WASI_SIGNAL_P1_API)
  if(NOT UWVM_RMW_WASI_SIGNAL_P1_API)
    message(FATAL_ERROR "Actual pthread/CV/semaphore/TLS/Wasm-EH/Preview1 compile+link closure is missing")
  endif()
endif()
configure_file(cmake/uwvm_rmw_wasi_signal_features.h.in
  generated/uwvm_rmw_wasi_signal_features.h @ONLY)
add_library(uwvm_rmw_wasi_signal STATIC src/wasi_signal.cpp)
target_compile_features(uwvm_rmw_wasi_signal PUBLIC cxx_std_17)
target_include_directories(uwvm_rmw_wasi_signal PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/uwvm_rmw_threaded_platform>")
target_include_directories(uwvm_rmw_wasi_signal PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
if(UWVM_RMW_WASI_SIGNAL_P1_API)
  find_package(Threads REQUIRED)
  target_compile_options(uwvm_rmw_wasi_signal PUBLIC
    -pthread -matomics -mbulk-memory -fwasm-exceptions -mllvm -wasm-use-legacy-eh=false)
  target_compile_definitions(uwvm_rmw_wasi_signal PUBLIC _WASI_EMULATED_SIGNAL)
  target_link_options(uwvm_rmw_wasi_signal PUBLIC -pthread -fwasm-exceptions
    -Wl,--import-memory -Wl,--export-memory)
  target_link_libraries(uwvm_rmw_wasi_signal PUBLIC Threads::Threads unwind)
endif()
# This library alone NEVER removes the existing original-rclcpp project-hook
# refusal. Root must separately close its real B/C threaded/default-codec cohort
# and use the fixed-original two-file source port on a fully admitted copy.
set_property(TARGET uwvm_rmw_wasi_signal PROPERTY UWVM_WASI_SIGNAL_ADMITTED ${UWVM_RMW_WASI_SIGNAL_P1_API})
set_property(TARGET uwvm_rmw_wasi_signal PROPERTY EXPORT_PROPERTIES UWVM_WASI_SIGNAL_ADMITTED)
install(TARGETS uwvm_rmw_wasi_signal EXPORT uwvm_rmw_wasi_signal ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/uwvm_rmw_threaded_platform)
install(FILES cmake/FixedOriginalPort.cmake DESTINATION share/${PROJECT_NAME}/cmake)
install(DIRECTORY upstream/ DESTINATION share/${PROJECT_NAME}/upstream)
ament_export_targets(uwvm_rmw_wasi_signal HAS_LIBRARY_TARGET)
configure_file(cmake/dependencies.cmake.in dependencies.cmake @ONLY)
ament_package(CONFIG_EXTRAS "${CMAKE_CURRENT_BINARY_DIR}/dependencies.cmake")
