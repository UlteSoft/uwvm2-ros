cmake_minimum_required(VERSION 3.16)
project(uwvm_rclc_native_service_oracle C)
find_package(ament_cmake REQUIRED)
find_package(rcl REQUIRED)
find_package(rclc REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(std_srvs REQUIRED)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(rclc_service_native_peer peer.c)
set_property(TARGET rclc_service_native_peer PROPERTY C_STANDARD 11)
# Use the installed SDK exclusively. No Guest adapter include directory or
# compat.c/services.inc is permitted in this native target's actual full-MD.
ament_target_dependencies(rclc_service_native_peer rcl rclc example_interfaces std_srvs)
