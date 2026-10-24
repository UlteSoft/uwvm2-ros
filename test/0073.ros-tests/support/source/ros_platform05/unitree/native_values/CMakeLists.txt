cmake_minimum_required(VERSION 3.16)
project(uwvm_unitree_native_value_witness LANGUAGES CXX)

# This fixture is a native-only value witness. It creates no ROS Node, endpoint,
# Gateway, Runtime registry or Guest capability. Build it separately from Wasm.
# Every path and installed package must belong to the caller's admitted source
# and genuine native SDK. A configured target is not a passed execution.
set(UWVM_UNITREE_FAMILY "" CACHE STRING "One genuine unitree_go, unitree_hg or unitree_api package")
set(UWVM_UNITREE_ORACLE "" CACHE PATH "Fresh --native-ros2 oracle output")
set(UWVM_UNITREE_GENERATED "" CACHE PATH "Matching complete portable records")
set(UWVM_SDK_INCLUDE "" CACHE PATH "Current sdk/origin/include directory")
if(NOT UWVM_UNITREE_FAMILY MATCHES "^unitree_(go|hg|api)$")
  message(FATAL_ERROR "Select exactly one original ROS2 Unitree family")
endif()
foreach(path IN ITEMS UWVM_UNITREE_ORACLE UWVM_UNITREE_GENERATED UWVM_SDK_INCLUDE)
  if(NOT IS_ABSOLUTE "${${path}}" OR NOT IS_DIRECTORY "${${path}}")
    message(FATAL_ERROR "${path} must name an explicit admitted absolute directory")
  endif()
endforeach()
if(NOT EXISTS "${UWVM_UNITREE_ORACLE}/contract.c" OR
   NOT EXISTS "${UWVM_UNITREE_GENERATED}/bounded_records.hpp" OR
   NOT EXISTS "${UWVM_SDK_INCLUDE}/roswasm/roswasmcpp.hpp" OR
   NOT EXISTS "${UWVM_SDK_INCLUDE}/../common/abi.h" OR
   NOT EXISTS "${UWVM_SDK_INCLUDE}/../common/codec.hpp")
  message(FATAL_ERROR "The complete original oracle/generated/SDK input set is required")
endif()
set(BUILD_TESTING OFF CACHE BOOL "Only this explicitly selected witness" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(${UWVM_UNITREE_FAMILY} REQUIRED)
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
  message(FATAL_ERROR "A genuine full-dependency and native link trace is required")
endif()
add_executable(unitree-native-values "${UWVM_UNITREE_ORACLE}/contract.c")
set_source_files_properties("${UWVM_UNITREE_ORACLE}/contract.c" PROPERTIES LANGUAGE CXX)
set_target_properties(unitree-native-values PROPERTIES
  CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_include_directories(unitree-native-values PRIVATE
  "${UWVM_UNITREE_GENERATED}" "${UWVM_SDK_INCLUDE}")
ament_target_dependencies(unitree-native-values rclcpp ${UWVM_UNITREE_FAMILY})
target_compile_options(unitree-native-values PRIVATE -O1 -g0 -UNDEBUG
  -fno-fast-math -ffp-contract=off -MD)
target_link_options(unitree-native-values PRIVATE -Wl,--trace)
