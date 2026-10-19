cmake_minimum_required(VERSION 3.16)
project(uwvm_unitree_official_native_identity LANGUAGES CXX)

# This target exports metadata only. It has no ROS context, endpoint, test,
# try_run, network acquisition or automatic package installation step.
set(BUILD_TESTING OFF CACHE BOOL "No SDK tests in the metadata build" FORCE)
set(CATKIN_ENABLE_TESTING OFF CACHE BOOL "No catkin tests" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(UWVM_SOURCE "" CACHE PATH "Complete frozen uwvm source used by both peers")
set(UWVM_UNITREE_SELECTION_DIR "" CACHE PATH "Fresh prepare.py selection directory")
set(UWVM_UNITREE_CXX_STANDARD "17" CACHE STRING "Actual selected SDK standard:17/20/23")
set(UWVM_UNITREE_CXX11_ABI "SDK_DEFAULT" CACHE STRING "SDK_DEFAULT or previously qualified libstdc++0/1")
if(NOT UWVM_UNITREE_CXX_STANDARD MATCHES "^(17|20|23)$")
  message(FATAL_ERROR "Explicit C++17/20/23 required")
endif()
if(UWVM_UNITREE_CXX_STANDARD STREQUAL "23" AND CMAKE_VERSION VERSION_LESS "3.20")
  message(FATAL_ERROR "C++23 needs an explicitly selected modern CMake")
endif()
if(NOT IS_ABSOLUTE "${UWVM_SOURCE}" OR NOT EXISTS "${UWVM_SOURCE}/gateways/ros2/compat/identity.h")
  message(FATAL_ERROR "Explicit frozen UWVM_SOURCE required")
endif()
if(NOT IS_ABSOLUTE "${UWVM_UNITREE_SELECTION_DIR}" OR
   NOT EXISTS "${UWVM_UNITREE_SELECTION_DIR}/selection.hpp" OR
   NOT EXISTS "${UWVM_UNITREE_SELECTION_DIR}/selection.cmake")
  message(FATAL_ERROR "Explicit prepared selection directory required")
endif()
include("${UWVM_UNITREE_SELECTION_DIR}/selection.cmake")
if(NOT ((UWVM_UNITREE_ROS_VERSION STREQUAL "1" AND UWVM_UNITREE_FAMILY STREQUAL "unitree_legged_msgs") OR
        (UWVM_UNITREE_ROS_VERSION STREQUAL "2" AND UWVM_UNITREE_FAMILY MATCHES "^(unitree_go|unitree_hg|unitree_api)$")))
  message(FATAL_ERROR "Exactly one of the four closed Unitree native profiles is required")
endif()

add_executable(unitree-native-identities native_identity.cpp)
set_target_properties(unitree-native-identities PROPERTIES CXX_STANDARD ${UWVM_UNITREE_CXX_STANDARD}
  CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_include_directories(unitree-native-identities PRIVATE
  "${UWVM_UNITREE_SELECTION_DIR}" "${UWVM_SOURCE}" "${UWVM_SOURCE}/src")
target_compile_definitions(unitree-native-identities PRIVATE UWVM_UNITREE_ROS_VERSION=${UWVM_UNITREE_ROS_VERSION})
if(UWVM_UNITREE_CXX11_ABI MATCHES "^[01]$")
  target_compile_definitions(unitree-native-identities PRIVATE _GLIBCXX_USE_CXX11_ABI=${UWVM_UNITREE_CXX11_ABI})
elseif(NOT UWVM_UNITREE_CXX11_ABI STREQUAL "SDK_DEFAULT")
  message(FATAL_ERROR "Use SDK_DEFAULT or an actually qualified0/1 ABI value")
endif()
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
  message(FATAL_ERROR "The full-MD/link-trace recipe requires an explicitly qualified GNU/Clang compiler")
endif()
# Older CMake can emit -MD without -MF. State the actual object-specific file;
# admission still inspects the final effective compiler argv and retained file.
target_compile_options(unitree-native-identities PRIVATE -O1 -g0 -UNDEBUG -MD
  -MF "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/unitree-native-identities.dir/native_identity.cpp.o.d")
target_link_options(unitree-native-identities PRIVATE -Wl,--trace)

# Recent ament releases can omit ament_target_dependencies. Match the narrow
# maintained Gateway dependency selection without importing its rclcpp/probe
# project. All branches keep the plain target_link_libraries signature.
function(uwvm_unitree_dependencies target)
  if(COMMAND ament_target_dependencies)
    ament_target_dependencies(${target} ${ARGN})
  else()
    foreach(package IN LISTS ARGN)
      if(TARGET ${package}::${package})
        target_link_libraries(${target} ${package}::${package})
      elseif(DEFINED ${package}_TARGETS AND NOT "${${package}_TARGETS}" STREQUAL "")
        target_link_libraries(${target} ${${package}_TARGETS})
      elseif(DEFINED ${package}_LIBRARIES AND NOT "${${package}_LIBRARIES}" STREQUAL "")
        target_include_directories(${target} SYSTEM PRIVATE ${${package}_INCLUDE_DIRS})
        target_link_libraries(${target} ${${package}_LIBRARIES})
      else()
        message(FATAL_ERROR "Selected native package ${package} has no supported explicit link interface")
      endif()
    endforeach()
  endif()
endfunction()

if(UWVM_UNITREE_ROS_VERSION STREQUAL "1")
  find_package(catkin REQUIRED COMPONENTS roscpp unitree_legged_msgs)
  target_include_directories(unitree-native-identities SYSTEM PRIVATE ${catkin_INCLUDE_DIRS})
  target_link_libraries(unitree-native-identities ${catkin_LIBRARIES})
else()
  find_package(ament_cmake REQUIRED)
  find_package(rosidl_typesupport_cpp REQUIRED)
  find_package(rosidl_typesupport_introspection_cpp REQUIRED)
  find_package(${UWVM_UNITREE_FAMILY} REQUIRED)
  find_package(OpenSSL REQUIRED)
  target_compile_definitions(unitree-native-identities PRIVATE UWVM_ROS2_HAVE_OPENSSL=1)
  # Old ament uses the plain target_link_libraries signature; do not mix it with
  # keyword PRIVATE/PUBLIC calls. The fixed selected package supplies the real
  # generated C++ types and normal type-support dispatcher.
  uwvm_unitree_dependencies(unitree-native-identities ${UWVM_UNITREE_FAMILY}
    rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
  # Require the actual package's introspection implementation, not just its
  # headers. Older exports may expose an unnamespaced library instead.
  if(TARGET ${UWVM_UNITREE_FAMILY}::${UWVM_UNITREE_FAMILY}__rosidl_typesupport_introspection_cpp)
    target_link_libraries(unitree-native-identities
      ${UWVM_UNITREE_FAMILY}::${UWVM_UNITREE_FAMILY}__rosidl_typesupport_introspection_cpp)
  elseif(TARGET ${UWVM_UNITREE_FAMILY}__rosidl_typesupport_introspection_cpp)
    target_link_libraries(unitree-native-identities ${UWVM_UNITREE_FAMILY}__rosidl_typesupport_introspection_cpp)
  else()
    get_filename_component(_unitree_package_prefix "${${UWVM_UNITREE_FAMILY}_DIR}/../../.." REALPATH)
    find_library(UWVM_UNITREE_INTROSPECTION_LIBRARY
      NAMES ${UWVM_UNITREE_FAMILY}__rosidl_typesupport_introspection_cpp
      PATHS "${_unitree_package_prefix}/lib" ${${UWVM_UNITREE_FAMILY}_LIBRARY_DIRS}
      NO_DEFAULT_PATH)
    if(NOT UWVM_UNITREE_INTROSPECTION_LIBRARY)
      message(FATAL_ERROR "Selected official package has no linkable C++ introspection library")
    endif()
    target_link_libraries(unitree-native-identities "${UWVM_UNITREE_INTROSPECTION_LIBRARY}")
  endif()
  target_link_libraries(unitree-native-identities OpenSSL::Crypto)
endif()

# These are configured recipe facts, not executed-build or native acceptance.
# Preserve CMakeCache, compile_commands, verbose logs, full-MD dependencies,
# link.txt and --trace inputs with the actual artifact/SDK before/after pins.
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/native-build-contract.txt" CONTENT
"status=CONFIGURED_NOT_EXECUTED\nfamily=${UWVM_UNITREE_FAMILY}\nros_version=${UWVM_UNITREE_ROS_VERSION}\ncxx_standard=${UWVM_UNITREE_CXX_STANDARD}\ncxx11_abi=${UWVM_UNITREE_CXX11_ABI}\ncompiler=${CMAKE_CXX_COMPILER}\ncompiler_version=${CMAKE_CXX_COMPILER_VERSION}\nbinary=$<TARGET_FILE:unitree-native-identities>\nfull_dependencies=-MD -MF\nlink_inputs=actual verbose --trace output\n")
