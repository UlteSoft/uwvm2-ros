cmake_minimum_required(VERSION 3.16)
project(uwvm_ros2_custom_topic_fixture LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_typesupport_cpp REQUIRED)
find_package(uwvm_ros_custom_test_msgs REQUIRED)

# Configuration is intentionally explicit: this executable cannot silently
# select a sample hash or turn the empty default catalog into custom support.
set(UWVM_ROS_CUSTOM_REGISTRY_HEADER "" CACHE FILEPATH "Generated shared fixed-topic registry")
set(UWVM_ROS_CUSTOM_ROS2_HEADER "" CACHE FILEPATH "Generated typed ROS2 fixed-topic provider")
if(NOT UWVM_ROS_CUSTOM_REGISTRY_HEADER OR NOT UWVM_ROS_CUSTOM_ROS2_HEADER)
  message(FATAL_ERROR "Generate and explicitly select both custom-topic provider headers first")
endif()
foreach(custom_header IN ITEMS UWVM_ROS_CUSTOM_REGISTRY_HEADER UWVM_ROS_CUSTOM_ROS2_HEADER)
  if(NOT IS_ABSOLUTE "${${custom_header}}" OR NOT EXISTS "${${custom_header}}" OR IS_DIRECTORY "${${custom_header}}")
    message(FATAL_ERROR "${custom_header} must select an existing absolute header path")
  endif()
  if("${${custom_header}}" MATCHES "[;\"\\\\\r\n]")
    message(FATAL_ERROR "${custom_header} contains an unsupported include-path character")
  endif()
endforeach()

get_filename_component(UWVM_CUSTOM_FIXTURE_SOURCE "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
add_executable(uwvm-ros2-custom-topic-probe native_topics.cc)
target_include_directories(uwvm-ros2-custom-topic-probe PRIVATE
  "${UWVM_CUSTOM_FIXTURE_SOURCE}/src"
  "${UWVM_CUSTOM_FIXTURE_SOURCE}/sdk/origin/include"
  "${UWVM_CUSTOM_FIXTURE_SOURCE}/third-parties/fast_io/include")
target_compile_definitions(uwvm-ros2-custom-topic-probe PRIVATE
  UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_ROS_CUSTOM_REGISTRY_HEADER}"
  UWVM_ROS_CUSTOM_ROS2_HEADER="${UWVM_ROS_CUSTOM_ROS2_HEADER}")
target_compile_options(uwvm-ros2-custom-topic-probe PRIVATE -Wall -Wextra -Wpedantic -UNDEBUG)
ament_target_dependencies(uwvm-ros2-custom-topic-probe rclcpp rosidl_typesupport_cpp uwvm_ros_custom_test_msgs)
target_link_libraries(uwvm-ros2-custom-topic-probe pthread)

# This fixture directly uses the real adapter. It needs the same complete
# out-of-line provider implementation as Gateway, with its existing explicit
# manifest/native definitions, SDK dependencies and assertion-enabled flags.
include("${UWVM_CUSTOM_FIXTURE_SOURCE}/src/ros/gateway/ros2/custom_topics.cmake")
uwvm_ros2_custom_topic_sources(uwvm-ros2-custom-topic-probe)
