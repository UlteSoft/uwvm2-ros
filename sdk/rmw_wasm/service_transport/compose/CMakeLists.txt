cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_service_explicit_composition LANGUAGES C CXX)
set(UWVM_COMPOSED_SOURCE_ROOT "" CACHE PATH "Actual exact coherent source tree")
set(UWVM_RMW_CPP_MODE "" CACHE STRING "Explicit STATIC_WASI or NATIVE_CONTRACT")
option(UWVM_RMW_SERVICE_BUILD_MODELS "Actual SDK/local synthetic protocol models" OFF)
option(UWVM_RMW_SERVICE_BUILD_INTEROP_EXAMPLES "Actual C API plus real C++ object companion" OFF)
option(UWVM_RMW_SERVICE_BUILD_PURE_C_ADD_EXAMPLE "Actual C objects and actual C AddTwoInts codec" OFF)
if(NOT IS_ABSOLUTE "${UWVM_COMPOSED_SOURCE_ROOT}" OR NOT EXISTS "${UWVM_COMPOSED_SOURCE_ROOT}/sdk/rmw_wasm/cmake/CMakeLists.txt" OR
 NOT EXISTS "${UWVM_COMPOSED_SOURCE_ROOT}/sdk/rmw_wasm/service_transport/src/service_transport.c")
 message(FATAL_ERROR "Actual independently integrated A/B/C/service source tree required")
endif()
set(UWVM_RMW_INCLUDE_DIR "${UWVM_COMPOSED_SOURCE_ROOT}/sdk/rmw_wasm/include")
set(UWVM_ROS_SDK_INCLUDE_DIR "${UWVM_COMPOSED_SOURCE_ROOT}/sdk/include")
# The actual A -S package entry owns the genuine complete B/C source list and
# original upstream utility/platform dependencies. This is not a replacement.
add_subdirectory("${UWVM_COMPOSED_SOURCE_ROOT}/sdk/rmw_wasm/cmake" "${CMAKE_CURRENT_BINARY_DIR}/actual-rmw")
if(UWVM_RMW_SERVICE_BUILD_MODELS OR UWVM_RMW_SERVICE_BUILD_INTEROP_EXAMPLES)
 include("${UWVM_COMPOSED_SOURCE_ROOT}/sdk/rmw_wasm/cpp/attach.cmake")
 uwvm_rmw_attach_cpp_codecs(rmw_uwvm "${UWVM_RMW_CPP_MODE}")
endif()
if(UWVM_RMW_SERVICE_BUILD_MODELS)
 # This actual second provider is needed for the real composite forwarding
 # model. Never synthesize or duplicate its source with a stub in this project.
 add_subdirectory("${UWVM_COMPOSED_SOURCE_ROOT}/sdk/rmw_wasm/subscription_transport" "${CMAKE_CURRENT_BINARY_DIR}/actual-subscription")
endif()
add_subdirectory("${UWVM_COMPOSED_SOURCE_ROOT}/sdk/rmw_wasm/service_transport" "${CMAKE_CURRENT_BINARY_DIR}/actual-service")
