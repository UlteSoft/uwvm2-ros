# SPDX-License-Identifier: Apache-2.0
# Copyright 2026 UWVM contributors.
# This is a narrowly scoped CMake project hook, not a source-tree rewrite.
# Run only in separately admitted copies of the fixed originals. A different
# upstream revision needs a separately reviewed hook; hashes are not retagged.
if(NOT CMAKE_SYSTEM_NAME STREQUAL "WASI" OR BUILD_SHARED_LIBS)
  message(FATAL_ERROR "UWVM static platform hook requires the actual WASI cross toolchain and static libraries")
endif()
set(_UWVM_PLATFORM_ROOT "${CMAKE_CURRENT_LIST_DIR}/../platform")
if(NOT COMMAND _uwvm_require_hash)
  function(_uwvm_require_hash path expected)
    if(NOT EXISTS "${path}" OR IS_DIRECTORY "${path}")
      message(FATAL_ERROR "Missing original source ${path}")
    endif()
    file(SHA256 "${path}" actual)
    if(NOT actual STREQUAL expected)
      message(FATAL_ERROR "Original source differs from this hook's reviewed preimage: ${path}")
    endif()
  endfunction()
  function(_uwvm_replace_source target relative expected replacement)
    get_target_property(_root ${target} SOURCE_DIR)
    _uwvm_require_hash("${_root}/${relative}" "${expected}")
    get_target_property(_sources ${target} SOURCES)
    set(_new)
    set(_count 0)
    foreach(_source IN LISTS _sources)
      if(_source STREQUAL relative OR _source STREQUAL "${_root}/${relative}")
        list(APPEND _new "${replacement}")
        math(EXPR _count "${_count}+1")
      else()
        list(APPEND _new "${_source}")
      endif()
    endforeach()
    if(NOT _count EQUAL 1)
      message(FATAL_ERROR "Expected exactly one original ${target}/${relative}")
    endif()
    set_property(TARGET ${target} PROPERTY SOURCES "${_new}")
  endfunction()
  function(_uwvm_rcutils_end)
    if(NOT TARGET rcutils)
      message(FATAL_ERROR "Original rcutils target was not created")
    endif()
    _uwvm_replace_source(rcutils src/time_unix.c
      b484b51e52f697157911cb2b757f5ce16d865bddfcf51dc969404a6b653b7476
      "${_UWVM_PLATFORM_ROOT}/src/rcutils_time.c")
    _uwvm_replace_source(rcutils src/shared_library.c
      d9042d0381d875c6c6305c0e63066543fc5b74ee1d8753e9da31c6b9e5fc5746
      "${_UWVM_PLATFORM_ROOT}/src/rcutils_loader.c")
    target_link_libraries(rcutils PUBLIC uwvm_rmw_platform::uwvm_rmw_platform)
  endfunction()
  function(_uwvm_static_end)
    if(BUILD_SHARED_LIBS OR NOT RMW_IMPLEMENTATION_DISABLE_RUNTIME_SELECTION OR
       NOT RMW_IMPLEMENTATION STREQUAL "rmw_uwvm" OR NOT RCL_LOGGING_IMPLEMENTATION STREQUAL "rcl_logging_uwvm")
      message(FATAL_ERROR "Original package changed the closed static implementation selection")
    endif()
    if(PROJECT_NAME STREQUAL "rmw_implementation" AND TARGET rmw_implementation)
      message(FATAL_ERROR "Dynamic RMW selector target unexpectedly exists")
    endif()
  endfunction()
endif()
if(PROJECT_NAME STREQUAL "rcutils")
  _uwvm_require_hash("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
    c7705fb6ea2f5b7f61d83c244a83116e6e2e13a7f9e5a5b81d9f59d51dac8416)
  # Must be registered BEFORE original ament_package() materializes its config;
  # a deferred dependency export would be too late. Platform itself has no
  # rcutils dependency, so this prebuilt actual package introduces no cycle.
  find_package(ament_cmake REQUIRED)
  find_package(uwvm_rmw_platform REQUIRED)
  ament_export_dependencies(uwvm_rmw_platform)
  cmake_language(DEFER CALL _uwvm_rcutils_end)
elseif(PROJECT_NAME STREQUAL "rmw_implementation")
  _uwvm_require_hash("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
    db4034ed8d3b0098b87a9061c27746cf4061039da9f300ef9a6ca615aac4d8aa)
elseif(PROJECT_NAME STREQUAL "rosidl_typesupport_c")
  _uwvm_require_hash("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
    d0a1438f4156b037ea6d970f842d9fb504c5927ffc86a57d82ab6e076e6b0270)
elseif(PROJECT_NAME STREQUAL "rosidl_typesupport_cpp")
  _uwvm_require_hash("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
    048b8ab7e2ce225c0cd8515bc36c6004c868db15542089eba85533efe1575c10)
elseif(PROJECT_NAME STREQUAL "rcl")
  _uwvm_require_hash("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
    1ac7db626d33df0b29dd0e74133c1b1c5d9f4cca14056a1b4a411689499de0d2)
elseif(PROJECT_NAME STREQUAL "rclcpp")
  include("${CMAKE_CURRENT_LIST_DIR}/../original_build/CompositeOriginal.cmake")
  uwvm_prepare_original_rclcpp_composite()
endif()
cmake_language(DEFER CALL _uwvm_static_end)
