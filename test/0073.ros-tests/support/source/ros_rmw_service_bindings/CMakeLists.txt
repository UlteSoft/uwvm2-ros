cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_service_binding_values LANGUAGES CXX)
set(UWVM_SOURCE_ROOT "" CACHE PATH "Actual complete reviewed source root")
if(NOT IS_ABSOLUTE "${UWVM_SOURCE_ROOT}" OR NOT EXISTS "${UWVM_SOURCE_ROOT}/src/uwvm2/imported/ros/common/rmw_service_bindings.h")
 message(FATAL_ERROR "Actual complete common header and its original dependency source required")
endif()
add_executable(rmw-service-bindings-values value_contract.cc)
target_compile_features(rmw-service-bindings-values PRIVATE cxx_std_17)
target_include_directories(rmw-service-bindings-values PRIVATE "${UWVM_SOURCE_ROOT}/src")
# Actual standard library only. No fake native types/provider, process, network,
# configure-time try_run, add_test or automatic model execution is introduced.
