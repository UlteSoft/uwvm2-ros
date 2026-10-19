cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_service_host_values LANGUAGES CXX)
set(UWVM_SOURCE_ROOT "" CACHE PATH "Actual complete reviewed source root")
if(NOT IS_ABSOLUTE "${UWVM_SOURCE_ROOT}" OR NOT EXISTS "${UWVM_SOURCE_ROOT}/src/uwvm2/uwvm/imported/ros/rmw_service_state.h")
 message(FATAL_ERROR "Actual complete current source root required")
endif()
add_executable(rmw-service-host-values value_contract.cc)
target_compile_features(rmw-service-host-values PRIVATE cxx_std_20)
target_include_directories(rmw-service-host-values PRIVATE "${UWVM_SOURCE_ROOT}/src")
# Pure actual helper values. No native substitutes, add_test/try_run/process,
# automatic execution, generated headers or configure-time network activity.
