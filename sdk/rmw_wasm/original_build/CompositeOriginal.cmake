# SPDX-License-Identifier: Apache-2.0
# Called from the actual original package's project() hook, before its real
# ament_package() exports dependencies. No source is generated/replaced here.
# A fresh original tree must already contain the four exact reviewed ports.
include_guard(GLOBAL)

function(_uwvm_attach_original_rclcpp_composite)
  if(NOT TARGET rclcpp)
    message(FATAL_ERROR "The real original rclcpp target was not created")
  endif()
  # Both helpers verify whole exact source bodies and actual executable-link
  # admission. The second is PUBLIC because original public templates are also
  # instantiated by application translation units.
  uwvm_attach_original_rclcpp_wasi_signal(rclcpp)
  uwvm_attach_original_rclcpp_c_service_lifecycle(rclcpp)
  target_link_libraries(rclcpp PUBLIC
    rmw_uwvm::rmw_uwvm uwvm_rmw_platform::uwvm_rmw_platform
    uwvm_rmw_dispatch::uwvm_rmw_dispatch)
  set_property(TARGET rclcpp PROPERTY UWVM_ORIGINAL_RCLCPP_COMPOSITE 1)
  set_property(TARGET rclcpp APPEND PROPERTY EXPORT_PROPERTIES UWVM_ORIGINAL_RCLCPP_COMPOSITE)
  target_compile_definitions(rclcpp PUBLIC UWVM_ORIGINAL_RCLCPP_COMPOSITE=1)
endfunction()

# A macro deliberately preserves ament_export_dependencies' directory scope;
# hiding its macro state inside a CMake function would lose the actual exports.
macro(uwvm_prepare_original_rclcpp_composite)
  option(UWVM_BUILD_ORIGINAL_RCLCPP_WASM
    "Compile the fixed original rclcpp with genuine threaded WASI source ports" OFF)
  if(NOT UWVM_BUILD_ORIGINAL_RCLCPP_WASM)
    message(FATAL_ERROR "Explicit UWVM_BUILD_ORIGINAL_RCLCPP_WASM=ON is required for this source port")
  endif()
  if(NOT PROJECT_NAME STREQUAL "rclcpp" OR
     NOT CMAKE_SYSTEM_NAME STREQUAL "WASI" OR BUILD_SHARED_LIBS OR
     NOT CMAKE_CXX_COMPILER_TARGET MATCHES "^wasm32-(wasi|wasip1)-threads$" OR
     NOT CMAKE_TRY_COMPILE_TARGET_TYPE STREQUAL "EXECUTABLE" OR
     NOT UWVM_RMW_ENABLE_THREADED_PROFILE)
    message(FATAL_ERROR "The complete actual threaded static WASI source build is required")
  endif()
  # Includes added by the imported helper packages must be exported before the
  # original ament_package() call, not introduced only by a late deferred call.
  find_package(ament_cmake REQUIRED)
  find_package(rmw_uwvm REQUIRED)
  find_package(uwvm_rmw_platform REQUIRED)
  find_package(uwvm_rmw_dispatch REQUIRED)
  find_package(uwvm_rmw_threaded_platform REQUIRED)
  find_package(uwvm_rmw_c_service_lifecycle REQUIRED)
  foreach(_target rmw_uwvm::rmw_uwvm uwvm_rmw_platform::uwvm_rmw_platform
                  uwvm_rmw_dispatch::uwvm_rmw_dispatch
                  uwvm_rmw_threaded_platform::uwvm_rmw_wasi_signal
                  uwvm_rmw_c_service_lifecycle::uwvm_rmw_c_service_lifecycle)
    if(NOT TARGET "${_target}")
      message(FATAL_ERROR "Missing real static composition target: ${_target}")
    endif()
    get_target_property(_type "${_target}" TYPE)
    if(_target STREQUAL "uwvm_rmw_c_service_lifecycle::uwvm_rmw_c_service_lifecycle")
      # The real shared-ownership helper is header-only and exports the actual
      # static generated-C/runtime libraries it successfully executable-linked.
      if(NOT _type STREQUAL "INTERFACE_LIBRARY")
        message(FATAL_ERROR "The genuine C lifecycle helper must retain its header-only interface")
      endif()
    elseif(NOT _type STREQUAL "STATIC_LIBRARY")
      message(FATAL_ERROR "Actual static composition library required: ${_target}")
    endif()
  endforeach()
  get_target_property(_platform uwvm_rmw_platform::uwvm_rmw_platform INTERFACE_COMPILE_DEFINITIONS)
  get_target_property(_dispatch uwvm_rmw_dispatch::uwvm_rmw_dispatch INTERFACE_COMPILE_DEFINITIONS)
  get_target_property(_core rmw_uwvm::rmw_uwvm INTERFACE_LINK_LIBRARIES)
  if(NOT "UWVM_RMW_PLATFORM_THREADED=1" IN_LIST _platform OR
     "UWVM_RMW_PLATFORM_THREADED=0" IN_LIST _platform OR
     NOT "UWVM_RMW_ENABLE_THREADED_PROFILE=1" IN_LIST _dispatch OR
     NOT "uwvm_rmw_dispatch::uwvm_rmw_dispatch" IN_LIST _core)
    message(FATAL_ERROR "Actual core/platform/dispatch must all select the same real threaded gate")
  endif()
  get_target_property(_signal uwvm_rmw_threaded_platform::uwvm_rmw_wasi_signal UWVM_WASI_SIGNAL_ADMITTED)
  get_target_property(_c_lifetime uwvm_rmw_c_service_lifecycle::uwvm_rmw_c_service_lifecycle UWVM_C_LIFECYCLE_LINK_ADMITTED)
  if(NOT _signal STREQUAL "1" OR NOT _c_lifetime STREQUAL "1")
    message(FATAL_ERROR "Genuine executable-link probes for both source ports are required")
  endif()
  include("${uwvm_rmw_threaded_platform_DIR}/FixedOriginalPort.cmake")
  include("${uwvm_rmw_c_service_lifecycle_DIR}/FixedOriginalPort.cmake")
  if(NOT COMMAND uwvm_attach_original_rclcpp_wasi_signal OR
     NOT COMMAND uwvm_attach_original_rclcpp_c_service_lifecycle)
    message(FATAL_ERROR "Both actual fixed-original source attachment helpers are required")
  endif()
  ament_export_dependencies(rmw_uwvm uwvm_rmw_platform uwvm_rmw_dispatch
    uwvm_rmw_threaded_platform uwvm_rmw_c_service_lifecycle)
  cmake_language(DEFER CALL _uwvm_attach_original_rclcpp_composite)
endmacro()
