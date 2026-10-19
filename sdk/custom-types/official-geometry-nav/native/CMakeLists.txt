cmake_minimum_required(VERSION 3.16)
project(uwvm_geometry_native_contract LANGUAGES CXX)
# Configure/build/run only within the actual resource/process owner. CMake
# success does not qualify fullMD, tool/SDK identities, loaded libraries or ROS.
set(UWVM_SOURCE "" CACHE PATH "Actual complete frozen source")
set(UWVM_GEOMETRY_GENERATED "" CACHE PATH "Actual native-export-bound family")
set(UWVM_CUSTOM_NATIVE_ROS_VERSION "" CACHE STRING "Exactly 1 or 2")
set(BUILD_TESTING OFF CACHE BOOL "Explicitly owned native source contract" FORCE)
foreach(path UWVM_SOURCE UWVM_GEOMETRY_GENERATED)
  if(NOT IS_ABSOLUTE "${${path}}")
    message(FATAL_ERROR "Actual absolute source/generated roots required")
  endif()
endforeach()
if(NOT EXISTS "${UWVM_GEOMETRY_GENERATED}/generation.json" OR
   NOT EXISTS "${UWVM_GEOMETRY_GENERATED}/manifest.hpp" OR
   NOT EXISTS "${UWVM_GEOMETRY_GENERATED}/native_ros${UWVM_CUSTOM_NATIVE_ROS_VERSION}.hpp")
  message(FATAL_ERROR "Complete actual native-bound generated bundle required")
endif()
add_executable(geometry-native-contract native_contract.cc)
target_compile_features(geometry-native-contract PRIVATE cxx_std_17)
target_compile_definitions(geometry-native-contract PRIVATE UWVM_CUSTOM_NATIVE_ROS_VERSION=${UWVM_CUSTOM_NATIVE_ROS_VERSION})
target_include_directories(geometry-native-contract PRIVATE "${UWVM_SOURCE}" "${UWVM_SOURCE}/src" "${UWVM_GEOMETRY_GENERATED}")
target_compile_options(geometry-native-contract PRIVATE -O1 -g0 -UNDEBUG -std=gnu++17 -MD
  -MF "${CMAKE_BINARY_DIR}/CMakeFiles/geometry-native-contract.dir/native_contract.cc.o.d"
  -MT "${CMAKE_BINARY_DIR}/CMakeFiles/geometry-native-contract.dir/native_contract.cc.o")
target_link_options(geometry-native-contract PRIVATE -Wl,-t)
if(UWVM_CUSTOM_NATIVE_ROS_VERSION STREQUAL "1")
  find_package(catkin REQUIRED COMPONENTS roscpp std_msgs geometry_msgs nav_msgs)
  target_include_directories(geometry-native-contract PRIVATE ${catkin_INCLUDE_DIRS})
  target_link_libraries(geometry-native-contract PRIVATE ${catkin_LIBRARIES})
elseif(UWVM_CUSTOM_NATIVE_ROS_VERSION STREQUAL "2")
  find_package(ament_cmake REQUIRED)
  find_package(std_msgs REQUIRED)
  find_package(builtin_interfaces REQUIRED)
  find_package(geometry_msgs REQUIRED)
  find_package(nav_msgs REQUIRED)
  find_package(rosidl_typesupport_cpp REQUIRED)
  find_package(rosidl_typesupport_introspection_cpp REQUIRED)
  find_package(rclcpp REQUIRED)
  find_package(OpenSSL REQUIRED COMPONENTS Crypto)
  target_compile_definitions(geometry-native-contract PRIVATE UWVM_ROS2_HAVE_OPENSSL=1)
  target_link_libraries(geometry-native-contract PRIVATE OpenSSL::Crypto)
  ament_target_dependencies(geometry-native-contract std_msgs builtin_interfaces geometry_msgs nav_msgs
    rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
  # A separate real modern contract checks the newly supported default family.
  # It deliberately refuses a selected pre-TypeDescription SDK at compile time.
  add_executable(geometry-native-defaults-contract native_defaults_contract.cc)
  target_compile_features(geometry-native-defaults-contract PRIVATE cxx_std_20)
  target_compile_definitions(geometry-native-defaults-contract PRIVATE
    UWVM_ROS2_HAVE_OPENSSL=1
    UWVM_ROS_CUSTOM_TOPIC_MANIFEST="manifest.hpp"
    UWVM_ROS_CUSTOM_ROS2_HEADER="native_ros2.hpp")
  target_include_directories(geometry-native-defaults-contract PRIVATE "${UWVM_SOURCE}" "${UWVM_SOURCE}/src"
    "${UWVM_SOURCE}/third-parties/fast_io/include" "${UWVM_GEOMETRY_GENERATED}")
  target_compile_options(geometry-native-defaults-contract PRIVATE -O1 -g0 -UNDEBUG -std=gnu++20 -MD
    -MF "${CMAKE_BINARY_DIR}/CMakeFiles/geometry-native-defaults-contract.dir/native_defaults_contract.cc.o.d"
    -MT "${CMAKE_BINARY_DIR}/CMakeFiles/geometry-native-defaults-contract.dir/native_defaults_contract.cc.o")
  target_link_options(geometry-native-defaults-contract PRIVATE -Wl,-t)
  target_link_libraries(geometry-native-defaults-contract PRIVATE OpenSSL::Crypto pthread)
  ament_target_dependencies(geometry-native-defaults-contract rclcpp std_msgs builtin_interfaces geometry_msgs nav_msgs
    rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
else()
  message(FATAL_ERROR "Select exactly one real native ROS major")
endif()
