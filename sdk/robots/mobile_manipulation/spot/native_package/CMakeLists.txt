cmake_minimum_required(VERSION 3.16)
project(uwvm_spot_bridge_native)
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(uwvm_spot_bridge REQUIRED)
# Actual source install; no downloaded SDK, credential, socket, permission or
# generated message substitute is produced by configuring this project.
ament_python_install_package(uwvm_mobile_manipulation PACKAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
ament_export_dependencies(uwvm_spot_bridge)
ament_package()
