# @file CMakeLists.txt
# @brief Compile the actual gateway and ownership witnesses from two checkouts.
# @details Test drivers remain in the VM repository. UWVM_ROS_SDK_ROOT selects
# the sibling SDK source explicitly; generated feature probes use installed ROS.
cmake_minimum_required(VERSION 3.24)
project(uwvm_runtime_repair LANGUAGES C CXX)
if(NOT UWVM_ROS_SDK_ROOT OR NOT UWVM_ROS_SOURCE_ROOT)
  message(FATAL_ERROR "Provide UWVM_ROS_SDK_ROOT and UWVM_ROS_SOURCE_ROOT")
endif()
set(UWVM_ROS2_BUILD_PROBES OFF CACHE BOOL "Build only these focused witnesses" FORCE)
# The witness consumes the production target's imported ROS dependencies from
# the parent directory; package targets must be visible in both directories.
set(CMAKE_FIND_PACKAGE_TARGETS_GLOBAL TRUE)
add_subdirectory("${UWVM_ROS_SDK_ROOT}/src/gateway/ros2" gateway)

add_executable(service-client-pool service_client_pool.cc)
target_compile_features(service-client-pool PRIVATE cxx_std_17)
target_include_directories(service-client-pool PRIVATE
  "${UWVM_ROS_SOURCE_ROOT}/src" "${UWVM_ROS_SDK_ROOT}/src")
target_link_libraries(service-client-pool PRIVATE pthread)

# Both witnesses include the production adapter and consume its exact feature
# probes and dependency closure. Assertions are required even in Release builds.
foreach(witness IN ITEMS native-service-reuse native-service-benchmark ros-service-peer)
  string(REPLACE "-" "_" source_name "${witness}")
  add_executable(${witness} "${source_name}.cc")
  target_compile_features(${witness} PRIVATE cxx_std_23)
  target_compile_options(${witness} PRIVATE -UNDEBUG)
  target_compile_definitions(${witness} PRIVATE
    "$<TARGET_PROPERTY:uwvm-ros2-gateway,COMPILE_DEFINITIONS>")
  target_include_directories(${witness} PRIVATE "${UWVM_ROS_SDK_ROOT}/src"
    "$<TARGET_PROPERTY:uwvm-ros2-gateway,INCLUDE_DIRECTORIES>")
  target_link_libraries(${witness} PRIVATE
    "$<TARGET_PROPERTY:uwvm-ros2-gateway,LINK_LIBRARIES>" dl)
endforeach()
target_compile_options(service-client-pool PRIVATE -UNDEBUG)
