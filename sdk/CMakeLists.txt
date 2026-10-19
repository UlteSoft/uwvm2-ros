# Source-only SDK integration. This project performs no native ROS discovery,
# downloads or compiler checks. Enable C/C++ in the consuming application and
# choose a Wasm toolchain there; linking an interface target grants no authority.
cmake_minimum_required(VERSION 3.16)
project(roswasm_guest_sdk VERSION 1.0.0 LANGUAGES NONE)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# The complete installed tree preserves the compatibility forwarding headers'
# relative paths. Do not flatten include/ separately from ros1/ros2/common/.
set(ROSWASM_SDK_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/roswasm/sdk")
set(ROSWASM_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/roswasm")

add_library(roswasm_c INTERFACE)
add_library(roswasm::c ALIAS roswasm_c)
set_target_properties(roswasm_c PROPERTIES EXPORT_NAME c)
target_include_directories(roswasm_c INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${ROSWASM_SDK_INSTALL_DIR}/include>")

add_library(roswasm_cpp INTERFACE)
add_library(roswasm::cpp ALIAS roswasm_cpp)
set_target_properties(roswasm_cpp PROPERTIES EXPORT_NAME cpp)
target_link_libraries(roswasm_cpp INTERFACE roswasm_c)
# Set the interface property without enabling a host compiler in this SDK.
set_property(TARGET roswasm_cpp PROPERTY INTERFACE_COMPILE_FEATURES cxx_std_20)

# Optional portable Unitree value helpers. This interface adds only headers:
# it does not discover a native ROS SDK, load a registry, select a robot/network,
# generate a native identity or grant any Guest operation. C++ applications
# additionally link roswasm::cpp; C applications can retain the C-only target.
add_library(roswasm_unitree INTERFACE)
add_library(roswasm::unitree ALIAS roswasm_unitree)
set_target_properties(roswasm_unitree PROPERTIES EXPORT_NAME unitree)
target_link_libraries(roswasm_unitree INTERFACE roswasm_c)
target_include_directories(roswasm_unitree INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/robots/unitree/include>"
    "$<INSTALL_INTERFACE:${ROSWASM_SDK_INSTALL_DIR}/robots/unitree/include>")

install(TARGETS roswasm_c roswasm_cpp roswasm_unitree EXPORT roswasmTargets)
install(EXPORT roswasmTargets FILE roswasmTargets.cmake NAMESPACE roswasm::
    DESTINATION "${ROSWASM_CONFIG_INSTALL_DIR}")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION "${ROSWASM_SDK_INSTALL_DIR}"
    PATTERN "target" EXCLUDE PATTERN "node_modules" EXCLUDE
    PATTERN "__pycache__" EXCLUDE PATTERN ".git" EXCLUDE
    PATTERN "build" EXCLUDE PATTERN "verification-*.json" EXCLUDE)

configure_package_config_file("cmake/roswasmConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/roswasmConfig.cmake"
    INSTALL_DESTINATION "${ROSWASM_CONFIG_INSTALL_DIR}"
    PATH_VARS ROSWASM_SDK_INSTALL_DIR)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/roswasmConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}" COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/roswasmConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/roswasmConfigVersion.cmake"
    DESTINATION "${ROSWASM_CONFIG_INSTALL_DIR}")
