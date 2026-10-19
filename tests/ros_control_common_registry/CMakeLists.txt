cmake_minimum_required(VERSION 3.20)
project(uwvm_control_common_registry_contracts LANGUAGES CXX)

# This project never invokes the generator, a ROS tool, a contract executable,
# try_run(), or CTest. Supply an already reviewed coherent source snapshot and
# already generated products. An unavailable preparation manifest is an expected
# negative input; a frozen manifest must contain independently exported real IDs.
set(UWVM_SOURCE "" CACHE PATH "Absolute coherent UWVM2 source root containing the reviewed control-registry production changes")
set(UWVM_CONTROL_PREPARATION_MANIFEST "" CACHE FILEPATH "Absolute generated preparation control_manifest.hpp")
set(UWVM_CONTROL_FROZEN_MANIFEST "" CACHE FILEPATH "Absolute generated control_manifest.hpp with actual independently exported native identities")
set(UWVM_CONTROL_LEGACY_MANIFEST "" CACHE FILEPATH "Optional independent real existing vendor/standard manifest.hpp; never generated or rewritten here")
option(UWVM_CONTROL_BUILD_PREPARATION "Build the unavailable preparation-registry contract" OFF)
option(UWVM_CONTROL_BUILD_FROZEN "Build the real frozen-registry and whole-table poison contracts" OFF)

if(NOT UWVM_CONTROL_BUILD_PREPARATION AND NOT UWVM_CONTROL_BUILD_FROZEN)
  message(STATUS "No control-common contract target selected; both build options default OFF")
  return()
endif()

function(control_require_path value description)
  if(NOT IS_ABSOLUTE "${value}" OR NOT EXISTS "${value}")
    message(FATAL_ERROR "${description} must be an existing absolute path")
  endif()
  # These paths become C/C++ include operands. Do not accept list separators,
  # quotes, newlines or escape characters as alternative compiler definitions.
  if("${value}" MATCHES "[;\"\\\\\n\r]")
    message(FATAL_ERROR "${description} contains an unsupported include-path character")
  endif()
endfunction()

control_require_path("${UWVM_SOURCE}" "UWVM_SOURCE")
foreach(header IN ITEMS
    src/uwvm2/imported/ros/capability/manager.h
    src/uwvm2/imported/ros/ipc/custom_interface.h
    src/uwvm2/imported/ros/ipc/custom_type.h
    src/uwvm2/imported/ros/ipc/custom_wire.h
    third-parties/fast_io/include/fast_io.h)
  if(NOT EXISTS "${UWVM_SOURCE}/${header}")
    message(FATAL_ERROR "Coherent source snapshot is missing ${header}")
  endif()
endforeach()
find_package(Threads REQUIRED)

function(control_contract_target target manifest frozen poison)
  control_require_path("${manifest}" "${target} manifest")
  if(IS_DIRECTORY "${manifest}")
    message(FATAL_ERROR "${target} manifest must be a file")
  endif()
  get_filename_component(product_dir "${manifest}" DIRECTORY)
  if(NOT EXISTS "${product_dir}/bounded_records.h" OR IS_DIRECTORY "${product_dir}/bounded_records.h")
    message(FATAL_ERROR "${target} needs bounded_records.h from the SAME reviewed generator product directory")
  endif()
  add_executable(${target} registry_contract.cc)
  # Actual manager/policy headers consume the repository fast_io concepts.
  # This is a host-side source contract, not a WASI or native ROS build.
  target_compile_features(${target} PRIVATE cxx_std_20)
  target_include_directories(${target} PRIVATE
    "${UWVM_SOURCE}/src" "${UWVM_SOURCE}/third-parties/fast_io/include" "${product_dir}")
  target_link_libraries(${target} PRIVATE Threads::Threads)
  target_compile_definitions(${target} PRIVATE
    "UWVM_CONTROL_CONTRACT_SELF=\"${CMAKE_CURRENT_SOURCE_DIR}/registry_contract.cc\""
    "UWVM_ROS_ORIGINAL_CONTROL_MANIFEST=\"${CMAKE_CURRENT_SOURCE_DIR}/registry_contract.cc\""
    "UWVM_CONTROL_PROVIDED_MANIFEST=\"${manifest}\""
    "UWVM_CONTROL_EXPECT_FROZEN=${frozen}" "UWVM_CONTROL_POISON_TABLE=${poison}")
  if(UWVM_CONTROL_LEGACY_MANIFEST)
    control_require_path("${UWVM_CONTROL_LEGACY_MANIFEST}" "actual legacy manifest")
    if(IS_DIRECTORY "${UWVM_CONTROL_LEGACY_MANIFEST}")
      message(FATAL_ERROR "actual legacy manifest must be a file")
    endif()
    get_filename_component(legacy_dir "${UWVM_CONTROL_LEGACY_MANIFEST}" DIRECTORY)
    target_include_directories(${target} PRIVATE "${legacy_dir}")
    target_compile_definitions(${target} PRIVATE
      "UWVM_ROS_CUSTOM_TOPIC_MANIFEST=\"${UWVM_CONTROL_LEGACY_MANIFEST}\"" UWVM_CONTROL_HAS_LEGACY=1)
  else()
    target_compile_definitions(${target} PRIVATE UWVM_CONTROL_HAS_LEGACY=0)
  endif()
endfunction()

if(UWVM_CONTROL_BUILD_PREPARATION)
  control_contract_target(control_registry_preparation "${UWVM_CONTROL_PREPARATION_MANIFEST}" 0 0)
endif()
if(UWVM_CONTROL_BUILD_FROZEN)
  control_contract_target(control_registry_frozen "${UWVM_CONTROL_FROZEN_MANIFEST}" 1 0)
  # A separate translation unit image is needed: actual whole-table admission
  # is constexpr, so a runtime local-array assertion alone cannot test lookups.
  control_contract_target(control_registry_poisoned "${UWVM_CONTROL_FROZEN_MANIFEST}" 1 1)
endif()
