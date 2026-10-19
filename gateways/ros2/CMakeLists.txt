cmake_minimum_required(VERSION 3.16)
project(uwvm_ros2_gateway LANGUAGES CXX)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action QUIET)
find_package(action_tutorials_interfaces QUIET)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(rcl_action QUIET)
find_package(lifecycle_msgs QUIET)
find_package(rclcpp_lifecycle QUIET)
find_package(tf2 QUIET)
find_package(tf2_ros QUIET)
find_package(tf2_msgs QUIET)
find_package(rcl REQUIRED)
find_package(rmw REQUIRED)
find_package(rosidl_typesupport_introspection_cpp QUIET)
find_package(OpenSSL QUIET COMPONENTS Crypto)
include(compat/Probe.cmake)
add_executable(uwvm-ros2-gateway main.cc)
target_compile_definitions(uwvm-ros2-gateway PRIVATE UWVM_ROS2_USE_COMPOSED_NODE=1)
target_include_directories(uwvm-ros2-gateway PRIVATE ../.. ../../src "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_options(uwvm-ros2-gateway PRIVATE -Wall -Wextra -Wpedantic)
uwvm_ros2_dependencies(uwvm-ros2-gateway rclcpp std_msgs geometry_msgs sensor_msgs std_srvs rcl_interfaces)
foreach(optional IN ITEMS tf2 tf2_ros tf2_msgs rclcpp_action action_tutorials_interfaces rcl_action lifecycle_msgs rosidl_typesupport_introspection_cpp)
  if(${optional}_FOUND)
    uwvm_ros2_dependencies(uwvm-ros2-gateway ${optional})
  endif()
endforeach()
if(OpenSSL_FOUND)
  target_link_libraries(uwvm-ros2-gateway OpenSSL::Crypto)
endif()
target_link_libraries(uwvm-ros2-gateway pthread)

# Opt-in actual original RCL Client/Service transport. OFF retains the original
# C++17 SDK route and does not compile CPP10, create a Registry or add rights.
option(UWVM_ROS2_BUILD_OWNED_RCL_SERVICE "Build explicitly selected complete original RCL service transport" OFF)
option(UWVM_ROS2_BUILD_OWNED_RCL_SERVICE_CONTRACTS "Build source-only authored owned service contracts" OFF)
option(UWVM_ROS2_BUILD_OWNED_CUSTOM_RCL_SERVICE "Build complete selected owned custom service adapters" OFF)
set(UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER "" CACHE FILEPATH "Independent actual Native service export plus typed layout proof")
if(UWVM_ROS2_BUILD_OWNED_CUSTOM_RCL_SERVICE AND NOT UWVM_ROS2_BUILD_OWNED_RCL_SERVICE)
  message(FATAL_ERROR "Owned custom service route requires explicitly enabled owned RCL service")
endif()
if(UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER AND NOT UWVM_ROS2_BUILD_OWNED_CUSTOM_RCL_SERVICE)
  message(FATAL_ERROR "Native owned-service export selected without its explicit production route")
endif()
if(UWVM_ROS2_BUILD_OWNED_RCL_SERVICE)
  if(CMAKE_VERSION VERSION_LESS 3.20)
    message(FATAL_ERROR "Owned RCL service requires actual CMake3.20 or newer")
  endif()
  if(CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "Owned RCL service requires explicitly selected UWVM_ROS2_CXX_STANDARD=20 or23; legacy C++17 is unchanged")
  endif()
  set(_uwvm_owned_root "${CMAKE_CURRENT_LIST_DIR}/../..")
  foreach(_header third-parties/fast_io/include/fast_io.h sdk/rmw_wasm/include/uwvm_rmw/cpp_codecs.hpp
      sdk/rmw_wasm/include/uwvm_rmw/codec.h sdk/include/roswasm/codec.h)
    if(NOT EXISTS "${_uwvm_owned_root}/${_header}")
      message(FATAL_ERROR "Actual complete owned service source dependency missing: ${_header}")
    endif()
  endforeach()
  find_package(OpenSSL REQUIRED COMPONENTS Crypto)
  include("${_uwvm_owned_root}/sdk/rmw_wasm/cpp/attach.cmake")
  # Keep a new library's keyword signature separate from the actual Gateway's
  # historical plain ament link signature; do not mix CMake link forms.
  add_library(uwvm-owned-service-codecs STATIC)
  target_include_directories(uwvm-owned-service-codecs PUBLIC
    "${_uwvm_owned_root}/sdk/rmw_wasm/include" "${_uwvm_owned_root}/sdk/include")
  uwvm_rmw_attach_cpp_codecs(uwvm-owned-service-codecs NATIVE_CONTRACT)
  target_include_directories(uwvm-ros2-gateway PRIVATE
    "${_uwvm_owned_root}/sdk/rmw_wasm/include" "${_uwvm_owned_root}/sdk/include"
    "${_uwvm_owned_root}/third-parties/fast_io/include")
  target_compile_definitions(uwvm-ros2-gateway PRIVATE UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE=1)
  target_link_libraries(uwvm-ros2-gateway uwvm-owned-service-codecs rcl::rcl rmw::rmw OpenSSL::Crypto)
  include(compat/NativeRclServiceProbe.cmake)
  uwvm_ros2_probe_native_rcl_service("${CMAKE_CURRENT_BINARY_DIR}")
  if(UWVM_ROS2_BUILD_OWNED_RCL_SERVICE_CONTRACTS)
    add_executable(uwvm-owned-service-gateway-values ../../tests/ros2_compat/owned_service_gateway/value_contract.cc)
    target_compile_features(uwvm-owned-service-gateway-values PRIVATE cxx_std_20)
    target_include_directories(uwvm-owned-service-gateway-values PRIVATE "${_uwvm_owned_root}" "${_uwvm_owned_root}/src"
      "${_uwvm_owned_root}/sdk/rmw_wasm/include" "${_uwvm_owned_root}/sdk/include"
      "${_uwvm_owned_root}/third-parties/fast_io/include" "${CMAKE_CURRENT_BINARY_DIR}")
    uwvm_ros2_dependencies(uwvm-owned-service-gateway-values rclcpp std_srvs example_interfaces)
    target_link_libraries(uwvm-owned-service-gateway-values uwvm-owned-service-codecs rcl::rcl rmw::rmw OpenSSL::Crypto pthread)
    add_executable(uwvm-owned-service-gateway-wire ../../tests/ros2_compat/owned_service_gateway/native_wire_contract.cc)
    target_compile_features(uwvm-owned-service-gateway-wire PRIVATE cxx_std_20)
    target_include_directories(uwvm-owned-service-gateway-wire PRIVATE "${_uwvm_owned_root}" "${_uwvm_owned_root}/src"
      "${_uwvm_owned_root}/third-parties/fast_io/include")
    target_link_libraries(uwvm-owned-service-gateway-wire PRIVATE pthread)
  endif()
elseif(UWVM_ROS2_BUILD_OWNED_RCL_SERVICE_CONTRACTS)
  message(FATAL_ERROR "Owned service contracts require the explicitly selected production route")
endif()

# Custom Topic, Service and Action support is selected by the trusted build.
# Empty defaults retain
# the closed built-in catalog and grant no additional ROS operation. Generate
# the common manifest, typed native provider and SDK codec headers from the same
# official bounded interfaces, then explicitly supply all three inputs. The
# historical TOPIC_MANIFEST macro carries all three interface categories and
# retains its name for compatibility with existing trusted build selections.
# The provider must name every required installed ROS2 interface package in the
# semicolon-separated list; graph discovery never loads an arbitrary package.
set(UWVM_ROS_CUSTOM_REGISTRY_HEADER "" CACHE FILEPATH "Generated common bounded ROS interface registry header")
set(UWVM_ROS_CUSTOM_ROS2_HEADER "" CACHE FILEPATH "Generated typed ROS2 Topic, Service and Action provider header")
set(UWVM_ROS_CUSTOM_ROS2_PACKAGES "" CACHE STRING "Explicit ROS2 packages required by the generated provider")
if(UWVM_ROS_CUSTOM_REGISTRY_HEADER OR UWVM_ROS_CUSTOM_ROS2_HEADER OR UWVM_ROS_CUSTOM_ROS2_PACKAGES)
  if(NOT UWVM_ROS_CUSTOM_REGISTRY_HEADER OR NOT UWVM_ROS_CUSTOM_ROS2_HEADER OR NOT UWVM_ROS_CUSTOM_ROS2_PACKAGES)
    message(FATAL_ERROR "Custom ROS2 interfaces require a registry header, native provider header and explicit package list")
  endif()
  foreach(custom_header IN ITEMS UWVM_ROS_CUSTOM_REGISTRY_HEADER UWVM_ROS_CUSTOM_ROS2_HEADER)
    if(NOT IS_ABSOLUTE "${${custom_header}}" OR NOT EXISTS "${${custom_header}}" OR IS_DIRECTORY "${${custom_header}}")
      message(FATAL_ERROR "${custom_header} must select an existing absolute header path")
    endif()
    # Semicolons and quotes would alter a CMake list or a quoted include macro.
    if("${${custom_header}}" MATCHES "[;\"\\\\\r\n]")
      message(FATAL_ERROR "${custom_header} contains an unsupported include-path character")
    endif()
  endforeach()
  set(custom_packages_seen "")
  foreach(custom_package IN LISTS UWVM_ROS_CUSTOM_ROS2_PACKAGES)
    if(NOT custom_package MATCHES "^[a-z][a-z0-9_]*$")
      message(FATAL_ERROR "Custom ROS2 interface package names must be explicit package identifiers")
    endif()
    list(FIND custom_packages_seen "${custom_package}" custom_package_duplicate)
    if(NOT custom_package_duplicate EQUAL -1)
      message(FATAL_ERROR "Duplicate explicit custom ROS2 interface package name")
    endif()
    list(APPEND custom_packages_seen "${custom_package}")
    find_package(${custom_package} REQUIRED)
  endforeach()
  target_compile_definitions(uwvm-ros2-gateway PRIVATE
    UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_ROS_CUSTOM_REGISTRY_HEADER}"
    UWVM_ROS_CUSTOM_ROS2_HEADER="${UWVM_ROS_CUSTOM_ROS2_HEADER}")
  target_include_directories(uwvm-ros2-gateway PRIVATE ../../sdk/include)
  uwvm_ros2_dependencies(uwvm-ros2-gateway ${UWVM_ROS_CUSTOM_ROS2_PACKAGES})
  # Same target definitions, dependencies and Release optimization reach every
  # partition; no separately configured library can drift from main's provider.
  include(custom_topics.cmake)
  uwvm_ros2_custom_topic_sources(uwvm-ros2-gateway)
  include(custom_services.cmake)
  uwvm_ros2_custom_service_sources(uwvm-ros2-gateway)
endif()

# Full custom owned service adapters are a second explicit build selection.
# A profile file and ordinary service capabilities remain independently required.
# No source row is installed by graph discovery or by merely linking this target.
if(UWVM_ROS2_BUILD_OWNED_CUSTOM_RCL_SERVICE)
  if(NOT UWVM_ROS_CUSTOM_REGISTRY_HEADER OR NOT UWVM_ROS_CUSTOM_ROS2_HEADER OR NOT UWVM_ROS_CUSTOM_ROS2_PACKAGES)
    message(FATAL_ERROR "Owned custom services require the complete original generated provider/manifest/package selection")
  endif()
  if(NOT UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER OR NOT IS_ABSOLUTE "${UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER}" OR
      NOT EXISTS "${UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER}" OR IS_DIRECTORY "${UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER}" OR
      "${UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER}" MATCHES "[;\"\\\\\r\n]")
    message(FATAL_ERROR "Actual independent original Native export/typed-layout header must be an existing absolute safe path")
  endif()
  set(_owned_custom_sdk_targets rclcpp::rclcpp rcl::rcl rmw::rmw)
  set(_owned_custom_packages std_msgs std_srvs example_interfaces service_msgs builtin_interfaces ${UWVM_ROS_CUSTOM_ROS2_PACKAGES})
  list(REMOVE_DUPLICATES _owned_custom_packages)
  foreach(_package IN LISTS _owned_custom_packages)
    find_package(${_package} REQUIRED)
    foreach(_provider rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
      if(TARGET "${_package}::${_package}__${_provider}")
        set(_actual_target "${_package}::${_package}__${_provider}")
      elseif(TARGET "${_package}")
        rosidl_get_typesupport_target(_actual_target "${_package}" "${_provider}")
      else()
        message(FATAL_ERROR "Actual complete ${_package}/${_provider} native target missing")
      endif()
      if(NOT _actual_target OR NOT TARGET "${_actual_target}")
        message(FATAL_ERROR "Actual complete Native type support target missing")
      endif()
      list(APPEND _owned_custom_sdk_targets "${_actual_target}")
    endforeach()
  endforeach()
  # CheckCXXSourceCompiles consumes actual imported SDK targets, never this new
  # unbuilt local static library as though it were a preexisting link product.
  foreach(_actual_target IN LISTS _owned_custom_sdk_targets)
    if(NOT TARGET "${_actual_target}")
      message(FATAL_ERROR "Actual selected modern SDK target absent: ${_actual_target}")
    endif()
    get_target_property(_is_imported "${_actual_target}" IMPORTED)
    if(NOT _is_imported)
      message(FATAL_ERROR "Owned custom metadata probe needs an installed imported SDK target")
    endif()
  endforeach()
  add_library(uwvm-owned-custom-service-adapters STATIC)
  target_compile_definitions(uwvm-owned-custom-service-adapters PRIVATE UWVM_ROS2_USE_COMPOSED_NODE=1)
  target_compile_definitions(uwvm-owned-custom-service-adapters PRIVATE
    UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_ROS_CUSTOM_REGISTRY_HEADER}"
    UWVM_ROS_CUSTOM_ROS2_HEADER="${UWVM_ROS_CUSTOM_ROS2_HEADER}"
    UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER="${UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER}")
  include(custom_owned_services.cmake)
  uwvm_ros2_owned_custom_service_sources(uwvm-owned-custom-service-adapters SDK_TARGETS ${_owned_custom_sdk_targets})
  target_include_directories(uwvm-owned-custom-service-adapters PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
  target_compile_definitions(uwvm-ros2-gateway PRIVATE UWVM_ROS2_OWNED_CUSTOM_RCL_SERVICE_PROFILE=1
    UWVM_ROS2_CUSTOM_SERVICE_SHARDS=${UWVM_ROS2_CUSTOM_SERVICE_SHARDS}
    UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER="${UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER}")
  target_link_libraries(uwvm-ros2-gateway uwvm-owned-custom-service-adapters)
endif()

# Every concrete selected Action gets its own options/lifecycle instantiation.
# The tutorial Action flag does not select the custom adapter.
uwvm_ros2_probe_selected_custom_actions()
uwvm_ros2_write_features()

# Existing Jazzy independent witnesses require its RIHS metadata protocol. A
# legacy SDK can build the actual Gateway without pretending these stricter
# witnesses compiled or ran. The capability manifest records actual SDK APIs.
if(UWVM_ROS2_BUILD_PROBES AND UWVM_ROS2_HAVE_TYPE_DESCRIPTION)
add_executable(uwvm-ros2-native-probe ../../tests/ros2/native_probe.cc)
target_include_directories(uwvm-ros2-native-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-native-probe rclcpp std_msgs geometry_msgs sensor_msgs std_srvs)
target_link_libraries(uwvm-ros2-native-probe pthread)
if(UWVM_ROS2_HAVE_ACTION_GRAPH)
add_executable(uwvm-ros2-resources-probe ../../tests/ros_resources/native_ros2.cc)
target_include_directories(uwvm-ros2-resources-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-resources-probe rclcpp std_msgs geometry_msgs sensor_msgs std_srvs rcl_interfaces rcl_action)
target_link_libraries(uwvm-ros2-resources-probe pthread)
endif()
if(UWVM_ROS2_HAVE_TF)
add_executable(uwvm-ros2-tf-probe ../../tests/ros_tf/native_ros2.cc)
target_include_directories(uwvm-ros2-tf-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-tf-probe rclcpp geometry_msgs tf2 tf2_ros tf2_msgs)
target_link_libraries(uwvm-ros2-tf-probe pthread)
endif()

if(UWVM_ROS2_HAVE_LIFECYCLE AND rclcpp_lifecycle_FOUND)
add_executable(uwvm-ros2-lifecycle-probe ../../tests/ros_lifecycle/native_ros2.cc)
target_include_directories(uwvm-ros2-lifecycle-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-lifecycle-probe rclcpp rclcpp_lifecycle lifecycle_msgs)
target_link_libraries(uwvm-ros2-lifecycle-probe pthread)
endif()

add_executable(uwvm-ros2-secure-probe ../../tests/ros_security/native_secure_ros2.cc)
target_include_directories(uwvm-ros2-secure-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-secure-probe rclcpp std_msgs)
target_link_libraries(uwvm-ros2-secure-probe pthread)

if(UWVM_ROS2_HAVE_ACTION)
add_executable(uwvm-ros2-action-probe ../../tests/ros_action/native_ros2.cc)
target_include_directories(uwvm-ros2-action-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-action-probe rclcpp rclcpp_action action_tutorials_interfaces)
target_link_libraries(uwvm-ros2-action-probe pthread)
endif()

add_executable(uwvm-ros2-perf-probe ../../tests/ros_perf/native_ros2.cc)
target_include_directories(uwvm-ros2-perf-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-perf-probe rclcpp std_msgs)
target_link_libraries(uwvm-ros2-perf-probe pthread)

add_executable(uwvm-ros2-ipc-service-probe ../../tests/ros_perf/ipc_service_ros2.cc)
target_include_directories(uwvm-ros2-ipc-service-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-ipc-service-probe rclcpp std_srvs)
target_link_libraries(uwvm-ros2-ipc-service-probe pthread)

add_executable(uwvm-ros2-five-module-probe ../../tests/ros_multimodule/native_ros2.cc)
target_include_directories(uwvm-ros2-five-module-probe PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
uwvm_ros2_dependencies(uwvm-ros2-five-module-probe rclcpp std_msgs)
target_link_libraries(uwvm-ros2-five-module-probe pthread)

endif()

# Optional model executable; OFF preserves the default historical target set.
# It inspects installed metadata/fake API shapes and never initializes ROS.
option(UWVM_ROS2_BUILD_COMPAT_TESTS "Build the SDK compatibility contract model" OFF)
if(UWVM_ROS2_BUILD_COMPAT_TESTS)
  add_executable(uwvm-ros2-compat-contract compat/tests_contract.cc)
  target_include_directories(uwvm-ros2-compat-contract PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
  target_compile_options(uwvm-ros2-compat-contract PRIVATE -UNDEBUG)
  uwvm_ros2_dependencies(uwvm-ros2-compat-contract rclcpp std_msgs std_srvs)
  if(rosidl_typesupport_introspection_cpp_FOUND)
    uwvm_ros2_dependencies(uwvm-ros2-compat-contract rosidl_typesupport_introspection_cpp)
  endif()
  if(OpenSSL_FOUND)
    target_link_libraries(uwvm-ros2-compat-contract OpenSSL::Crypto)
  endif()
  target_link_libraries(uwvm-ros2-compat-contract pthread)
  # This ownership model uses no ROS context, client, executor or SDK types.
  # It checks the exact generic lease used by the legacy public Action route.
  add_executable(uwvm-ros2-action-client-lease-contract ../../tests/ros2_compat/action_client_lease_contract.cc)
  target_compile_options(uwvm-ros2-action-client-lease-contract PRIVATE -UNDEBUG)
  target_link_libraries(uwvm-ros2-action-client-lease-contract pthread)
  # SDK-free data pairing checks use the same constexpr policy as the actual
  # selected custom Action compile/link probe; no native endpoint is created.
  add_executable(uwvm-ros2-action-catalog-contract ../../tests/ros2_compat/action_catalog_contract.cc)
  target_include_directories(uwvm-ros2-action-catalog-contract PRIVATE ../../src)
  target_compile_options(uwvm-ros2-action-catalog-contract PRIVATE -UNDEBUG)
  # This separate golden-identity regression requires the actual target Crypto
  # compile/link probe. A missing dependency must not become a stub success.
  if(UWVM_ROS2_HAVE_OPENSSL)
    add_executable(uwvm-ros2-rihs-contract ../../tests/ros2_compat/rihs_contract.cc)
    target_include_directories(uwvm-ros2-rihs-contract PRIVATE ../../src "${CMAKE_CURRENT_BINARY_DIR}")
    target_compile_options(uwvm-ros2-rihs-contract PRIVATE -UNDEBUG)
    target_link_libraries(uwvm-ros2-rihs-contract OpenSSL::Crypto)
  else()
    message(STATUS "uwvm-ros2-rihs-contract unavailable: target OpenSSL Crypto compile/link probe failed")
  endif()
endif()


# Complete original default-Node control adapters are an independent opt-in
# selection. OFF adds no SDK dependency, schema, endpoint or Guest authority.
# Canonical source, the all-C++ generated proof, and independent actual ID DATA
# are three separate caller-frozen inputs; this configure never generates them.
option(UWVM_ROS2_BUILD_DEFAULT_RMW_CONTROL_PROFILE "Build complete original ROS2 control transport" OFF)
set(UWVM_DEFAULT_CONTROL_CANONICAL_DIR "" CACHE PATH "Actual reviewed complete canonical control source")
set(UWVM_DEFAULT_CONTROL_GATEWAY_GENERATED_DIR "" CACHE PATH "Actual reviewed all-C++ control generated source")
set(UWVM_DEFAULT_CONTROL_GATEWAY_IDENTITIES_DIR "" CACHE PATH "Actual independent frozen Native ID input source")
if(UWVM_ROS2_BUILD_DEFAULT_RMW_CONTROL_PROFILE)
  if(NOT UWVM_ROS2_BUILD_OWNED_RCL_SERVICE)
    message(FATAL_ERROR "Complete original control route requires explicit owned RCL service transport")
  endif()
  foreach(_input UWVM_DEFAULT_CONTROL_CANONICAL_DIR UWVM_DEFAULT_CONTROL_GATEWAY_GENERATED_DIR UWVM_DEFAULT_CONTROL_GATEWAY_IDENTITIES_DIR)
    if(NOT IS_ABSOLUTE "${${_input}}" OR NOT IS_DIRECTORY "${${_input}}" OR "${${_input}}" MATCHES "[;\"\r\n]")
      message(FATAL_ERROR "${_input} requires an existing explicit safe absolute source directory")
    endif()
  endforeach()
  foreach(_file bounded_records.h control_manifest.hpp control-schema.json generation.json)
    if(NOT EXISTS "${UWVM_DEFAULT_CONTROL_CANONICAL_DIR}/${_file}" OR IS_DIRECTORY "${UWVM_DEFAULT_CONTROL_CANONICAL_DIR}/${_file}")
      message(FATAL_ERROR "Actual complete control source missing: ${_file}")
    endif()
  endforeach()
  foreach(_file default_control_gateway_identity_input.hpp identity-input.json)
    if(NOT EXISTS "${UWVM_DEFAULT_CONTROL_GATEWAY_IDENTITIES_DIR}/${_file}" OR IS_DIRECTORY "${UWVM_DEFAULT_CONTROL_GATEWAY_IDENTITIES_DIR}/${_file}")
      message(FATAL_ERROR "Actual independent frozen control ID DATA source missing: ${_file}")
    endif()
  endforeach()
  set(UWVM_DEFAULT_CONTROL_GATEWAY_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")
  cmake_path(NORMAL_PATH UWVM_DEFAULT_CONTROL_GATEWAY_PROJECT_ROOT)
  set(UWVM_RMW_INCLUDE_DIR "${UWVM_DEFAULT_CONTROL_GATEWAY_PROJECT_ROOT}/sdk/rmw_wasm/include")
  set(UWVM_DEFAULT_CONTROL_GATEWAY_FEATURES_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  add_subdirectory(default_control "${CMAKE_CURRENT_BINARY_DIR}/default-control-native")
  target_compile_definitions(uwvm_default_control_gateway PUBLIC UWVM_ROS2_USE_COMPOSED_NODE=1)
  if(NOT TARGET uwvm_default_control_gateway::uwvm_default_control_gateway)
    message(FATAL_ERROR "Actual complete control static target required")
  endif()
  set(_original_control_manifest "${UWVM_DEFAULT_CONTROL_CANONICAL_DIR}/control_manifest.hpp")
  target_compile_definitions(uwvm-ros2-gateway PRIVATE UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE=1
    UWVM_ROS_ORIGINAL_CONTROL_MANIFEST="${_original_control_manifest}")
  target_include_directories(uwvm-ros2-gateway PRIVATE "${UWVM_DEFAULT_CONTROL_GATEWAY_IDENTITIES_DIR}")
  target_link_libraries(uwvm-ros2-gateway uwvm_default_control_gateway::uwvm_default_control_gateway)
  # One usage interface carries the complete selected catalog environment.
  # It is PUBLIC on the control static library so its real linked contracts
  # inherit exactly the same inline tables. Main and the existing owned custom
  # dispatch/shard library consume that same interface; no TU sees a prefix.
  add_library(uwvm-ros2-selected-control-catalog INTERFACE)
  target_compile_definitions(uwvm-ros2-selected-control-catalog INTERFACE
    UWVM_ROS_ORIGINAL_CONTROL_MANIFEST="${_original_control_manifest}")
  target_include_directories(uwvm-ros2-selected-control-catalog INTERFACE
    "${UWVM_DEFAULT_CONTROL_GATEWAY_PROJECT_ROOT}/sdk/include")
  if(UWVM_ROS_CUSTOM_REGISTRY_HEADER)
    # custom_topics.h requires this original paired common/native selection.
    # Sharing only the common macro would fail its actual paired-header guard.
    target_compile_definitions(uwvm-ros2-selected-control-catalog INTERFACE
      UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_ROS_CUSTOM_REGISTRY_HEADER}"
      UWVM_ROS_CUSTOM_ROS2_HEADER="${UWVM_ROS_CUSTOM_ROS2_HEADER}"
      UWVM_ROS2_CUSTOM_TOPIC_SHARDS=${UWVM_ROS2_CUSTOM_TOPIC_SHARDS}
      UWVM_ROS2_CUSTOM_SERVICE_SHARDS=${UWVM_ROS2_CUSTOM_SERVICE_SHARDS})
    # These modern installed interface targets provide the actual native header
    # and symbol closure of the explicitly selected provider. Never invent a
    # target or substitute a success boolean for a missing package export.
    foreach(_package IN LISTS UWVM_ROS_CUSTOM_ROS2_PACKAGES)
      foreach(_provider rosidl_generator_cpp rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
        set(_native_provider_target "${_package}::${_package}__${_provider}")
        if(NOT TARGET "${_native_provider_target}")
          message(FATAL_ERROR "Selected complete control/vendor composition requires actual native target: ${_native_provider_target}")
        endif()
        target_link_libraries(uwvm-ros2-selected-control-catalog INTERFACE "${_native_provider_target}")
      endforeach()
    endforeach()
  endif()
  target_link_libraries(uwvm_default_control_gateway PUBLIC uwvm-ros2-selected-control-catalog)
  # Preserve the existing plain link signature on executable ament consumers.
  target_link_libraries(uwvm-ros2-gateway uwvm-ros2-selected-control-catalog)
  if(TARGET uwvm-owned-custom-service-adapters)
    target_link_libraries(uwvm-owned-custom-service-adapters PRIVATE uwvm-ros2-selected-control-catalog)
  endif()
  if(TARGET uwvm-owned-service-gateway-values)
    target_link_libraries(uwvm-owned-service-gateway-values uwvm-ros2-selected-control-catalog)
  endif()
endif()
