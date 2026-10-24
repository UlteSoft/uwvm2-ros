cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_subscription_transport_values LANGUAGES C CXX)
option(UWVM_BUILD_RMW_SUBSCRIPTION_TRANSPORT_VALUES "Build production value contracts, never native ROS evidence" OFF)
if(UWVM_BUILD_RMW_SUBSCRIPTION_TRANSPORT_VALUES)
  add_executable(uwvm_rmw_subscription_transport_values value_contract.cc)
  target_compile_features(uwvm_rmw_subscription_transport_values PRIVATE cxx_std_17)
  target_include_directories(uwvm_rmw_subscription_transport_values PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../../src")
  add_library(uwvm_rmw_subscription_raw_signatures OBJECT raw_signature_contract.c)
  target_compile_features(uwvm_rmw_subscription_raw_signatures PRIVATE c_std_11)
  target_include_directories(uwvm_rmw_subscription_raw_signatures PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../../sdk/origin/include")
endif()
# raw_import_inventory.c is a real Wasm link fixture, not a native executable.
# Link it separately with the selected WASI SDK, explicit export and --no-entry.
