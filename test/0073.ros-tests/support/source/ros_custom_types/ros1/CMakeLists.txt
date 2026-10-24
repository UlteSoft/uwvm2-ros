cmake_minimum_required(VERSION 3.10)
project(uwvm_ros1_custom_topic_native_fixture LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This fixture requires the same reviewed common/native generated headers as the
# custom-enabled Gateway. Build the official fixture catkin package first. The
# headers must register uwvm_custom_ros1_fixture/Packet with its real native MD5.
set(UWVM_ROS_CUSTOM_REGISTRY_HEADER "" CACHE FILEPATH "Generated common topic registry")
set(UWVM_ROS_CUSTOM_ROS1_HEADER "" CACHE FILEPATH "Generated native ROS1 provider")
if(NOT UWVM_ROS_CUSTOM_REGISTRY_HEADER OR NOT UWVM_ROS_CUSTOM_ROS1_HEADER)
    message(FATAL_ERROR "The native custom topic fixture requires both reviewed generated headers")
endif()
foreach(_uwvm_custom_header UWVM_ROS_CUSTOM_REGISTRY_HEADER UWVM_ROS_CUSTOM_ROS1_HEADER)
    if(NOT EXISTS "${${_uwvm_custom_header}}" OR IS_DIRECTORY "${${_uwvm_custom_header}}")
        message(FATAL_ERROR "Missing generated custom topic header")
    endif()
    get_filename_component(${_uwvm_custom_header} "${${_uwvm_custom_header}}" REALPATH)
    if("${${_uwvm_custom_header}}" MATCHES "[\";\\\\\r\n]")
        message(FATAL_ERROR "Unsupported generated header path character")
    endif()
endforeach()
find_package(catkin REQUIRED COMPONENTS roscpp uwvm_custom_ros1_fixture)
add_executable(uwvm-ros1-custom-topic-native-fixture native_topics.cc)
target_include_directories(uwvm-ros1-custom-topic-native-fixture PRIVATE ../../../src ${catkin_INCLUDE_DIRS})
target_compile_definitions(uwvm-ros1-custom-topic-native-fixture PRIVATE
    UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_ROS_CUSTOM_REGISTRY_HEADER}"
    UWVM_ROS_CUSTOM_ROS1_HEADER="${UWVM_ROS_CUSTOM_ROS1_HEADER}")
target_compile_options(uwvm-ros1-custom-topic-native-fixture PRIVATE -Wall -Wextra -Wpedantic -UNDEBUG)
target_link_libraries(uwvm-ros1-custom-topic-native-fixture PRIVATE ${catkin_LIBRARIES} pthread)
if(catkin_EXPORTED_TARGETS)
    add_dependencies(uwvm-ros1-custom-topic-native-fixture ${catkin_EXPORTED_TARGETS})
endif()
