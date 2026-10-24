cmake_minimum_required(VERSION 3.16)
project(uwvm_bounded_native_metadata LANGUAGES CXX)
# Both fixtures retain C++17 SDK compatibility and checked native_io text I/O.
# This second-stage target requires the already installed official native package.
# UWVM_SOURCE is the exact frozen project root selected by the trusted controller.
set(CATKIN_ENABLE_TESTING OFF CACHE BOOL "Only the two owned native fixtures are built" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "No SDK test targets are added" FORCE)
set(UWVM_SOURCE "" CACHE PATH "Frozen uwvm2 project source root")
set(UWVM_CUSTOM_NATIVE_ROS_VERSION "" CACHE STRING "Exactly 1 or 2")
if(NOT IS_DIRECTORY "${UWVM_SOURCE}/third-parties/fast_io/include")
  message(FATAL_ERROR "Select the frozen UWVM_SOURCE with vendored fast_io")
endif()
add_executable(bounded-native-identities native_identity.cc)
add_executable(bounded-native-peer native_peer.cc)
find_package(Threads REQUIRED)
target_link_libraries(bounded-native-peer PRIVATE Threads::Threads)
foreach(target bounded-native-identities bounded-native-peer)
  target_compile_features(${target} PRIVATE cxx_std_17)
  target_compile_definitions(${target} PRIVATE UWVM_CUSTOM_NATIVE_ROS_VERSION=${UWVM_CUSTOM_NATIVE_ROS_VERSION})
  target_include_directories(${target} PRIVATE "${UWVM_SOURCE}/src/ros/gateway/common")
  if(target STREQUAL "bounded-native-identities")
    set(native_source_file native_identity.cc)
  else()
    set(native_source_file native_peer.cc)
  endif()
  # CMake 3.16 may add -MD without -MF. Select the same complete dependency
  # filename explicitly; a newer CMake's later effective -MF must still match
  # this target-specific object. Actual verbose argv, not this option alone,
  # remains the preparation qualification evidence.
  target_compile_options(${target} PRIVATE -O1 -g0 -UNDEBUG -std=gnu++17 -MD
    -MF "/tmp/native-build/CMakeFiles/${target}.dir/${native_source_file}.o.d")
endforeach()
# Qualify the final effective MD/MF argv and linked SDK inputs from the actual
# verbose native products; source options alone are not compiler evidence.
if(UWVM_CUSTOM_NATIVE_ROS_VERSION STREQUAL "1")
  find_package(catkin REQUIRED COMPONENTS roscpp actionlib uwvm_ros_bounded_v2)
  foreach(target bounded-native-identities bounded-native-peer)
    target_include_directories(${target} PRIVATE ${catkin_INCLUDE_DIRS})
    target_link_libraries(${target} PRIVATE ${catkin_LIBRARIES})
  endforeach()
elseif(UWVM_CUSTOM_NATIVE_ROS_VERSION STREQUAL "2")
  find_package(ament_cmake REQUIRED)
  find_package(uwvm_ros_bounded_v2 REQUIRED)
  find_package(rosidl_typesupport_cpp REQUIRED)
  find_package(rclcpp REQUIRED)
  find_package(rclcpp_action REQUIRED)
  ament_target_dependencies(bounded-native-identities PUBLIC uwvm_ros_bounded_v2 rosidl_typesupport_cpp)
  ament_target_dependencies(bounded-native-peer PUBLIC uwvm_ros_bounded_v2 rclcpp rclcpp_action)
else()
  message(FATAL_ERROR "UWVM_CUSTOM_NATIVE_ROS_VERSION must be 1 or 2")
endif()
