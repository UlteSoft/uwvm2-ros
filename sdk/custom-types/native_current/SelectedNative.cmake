# This is a finite symbol-resolution input contract for THIS link oracle.
# Toolchain executables are trusted; it is not a CMake/compiler sandbox. Normal
# target/sysroot/stdlib/cross flags remain valid. A permissive undefined-symbol
# mode cannot turn absent genuine TypeSupport symbols into a successful probe.
function(uwvm_current_native_link_mode_guard)
  # Decode only the same literal wrappers admitted by the target-closure
  # walker. Check their complete resulting stream: inspecting raw generator
  # expression spelling would separate adjacent -z/undefs options. Opaque or
  # nested expressions require an explicit concrete SDK recipe for this oracle.
  set(_decoded)
  foreach(_entry IN LISTS ARGN)
    if(_entry MATCHES [=[^\$<(LINK_ONLY|BUILD_INTERFACE|INSTALL_INTERFACE):([^<>]*)>$]=])
      set(_entry "${CMAKE_MATCH_2}")
    elseif(_entry MATCHES [=[\$<]=])
      message(FATAL_ERROR "This Native link oracle requires a concrete link expression: ${_entry}; resolve it in an explicit SDK recipe")
    endif()
    list(APPEND _decoded "${_entry}")
  endforeach()
  # Drivers accept multi-element forwarding. Keep a case-preserving stream for
  # genuine short ld options: -i and -Ur are partial-link aliases, while -I,
  # -R and -rpath have different meanings. This is inspection only; the actual
  # compiler/linker flags retain their exact spelling and ordering.
  set(_mode "${_decoded}")
  set(_forwarded "${_mode}")
  string(REPLACE ";" " " _forwarded "${_forwarded}")
  string(REPLACE "-Xlinker" " " _forwarded "${_forwarded}")
  string(REPLACE "-Wl," " " _forwarded "${_forwarded}")
  string(REPLACE "LINKER:" " " _forwarded "${_forwarded}")
  string(REPLACE "SHELL:" " " _forwarded "${_forwarded}")
  string(TOLOWER "${_forwarded}" _lower)
  # Inspect these three Wasm modes conservatively across literal shell quotes
  # and escapes. This does not parse shell/environment expansion or response
  # files, and never rewrites any actual driver/linker argument.
  set(_wasm_modes "${_lower}")
  string(REPLACE "\"" "" _wasm_modes "${_wasm_modes}")
  string(REPLACE "'" "" _wasm_modes "${_wasm_modes}")
  string(REPLACE "\\" "" _wasm_modes "${_wasm_modes}")
  if(_lower MATCHES "(^|[=,: ;\t])--?[^=, ;\t]*unresolved" OR
     _lower MATCHES "allow[-_]shlib[-_]undefined" OR
     _wasm_modes MATCHES "(^|[=,: ;\t])--(allow-undefined(-file)?|import-undefined)($|[=,: ;\t])" OR
     _lower MATCHES "-undefined[=, ;\t]+(dynamic_lookup|suppress|warning)" OR
     _lower MATCHES "(^|[=,: ;\t])-z[=, ;\t]+undefs($|[, ;\t])" OR
     _lower MATCHES "noinhibit[-_]exec" OR
     _lower MATCHES "(^|[ ,;\t])/force(:|$|[ ,;\t])" OR
     _lower MATCHES "(^|[=, ;\t])--relocatable($|[, ;\t])" OR
     _forwarded MATCHES "(^|[=, ;\t])-(r|i|Ur)($|[, ;\t])")
    message(FATAL_ERROR "Native link oracle refuses permissive/incomplete symbol-resolution mode: ${_mode}")
  endif()
endfunction()

function(uwvm_current_native_link_environment)
  # Both real C and C++ driver inputs belong to this shared link oracle.
  # Wasm symbol-suppression flags do not qualify a missing library symbol;
  # explicit import_module/import_name declarations need no such global mode.
  set(_variables CMAKE_C_FLAGS CMAKE_CXX_FLAGS CMAKE_EXE_LINKER_FLAGS CMAKE_REQUIRED_FLAGS
    CMAKE_REQUIRED_DEFINITIONS CMAKE_REQUIRED_LINK_OPTIONS CMAKE_REQUIRED_LIBRARIES
    CMAKE_C_STANDARD_LIBRARIES CMAKE_C_LINK_FLAGS CMAKE_C_LINK_EXECUTABLE
    CMAKE_CXX_STANDARD_LIBRARIES CMAKE_CXX_LINK_FLAGS CMAKE_CXX_LINK_EXECUTABLE)
  set(_configs DEBUG RELEASE RELWITHDEBINFO MINSIZEREL ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})
  foreach(_config IN LISTS _configs)
    string(TOUPPER "${_config}" _upper)
    list(APPEND _variables "CMAKE_C_FLAGS_${_upper}" "CMAKE_CXX_FLAGS_${_upper}" "CMAKE_EXE_LINKER_FLAGS_${_upper}")
  endforeach()
  foreach(_variable IN LISTS _variables)
    if(DEFINED ${_variable})
      uwvm_current_native_link_mode_guard("${${_variable}}")
    endif()
  endforeach()
  # Inspect concrete target link closures, including aliases and wrappers that
  # merely nominate one literal target. Do not pretend to evaluate arbitrary
  # generator expressions before CMake's actual target/configuration selection.
  set(_queue ${ARGN})
  set(_seen)
  set(_steps 0)
  while(_queue)
    math(EXPR _steps "${_steps}+1")
    if(_steps GREATER 512)
      message(FATAL_ERROR "Native link-oracle target closure exceeds512 edges")
    endif()
    list(POP_FRONT _queue _target)
    if(NOT TARGET "${_target}")
      message(FATAL_ERROR "Actual selected target disappeared: ${_target}")
    endif()
    get_target_property(_alias "${_target}" ALIASED_TARGET)
    if(_alias)
      set(_target "${_alias}")
    endif()
    if(_target IN_LIST _seen)
      continue()
    endif()
    list(APPEND _seen "${_target}")
    set(_properties LINK_OPTIONS INTERFACE_LINK_OPTIONS LINK_FLAGS
      INTERFACE_LINK_LIBRARIES LINK_LIBRARIES INTERFACE_LINK_LIBRARIES_DIRECT
      INTERFACE_LINK_LIBRARIES_DIRECT_EXCLUDE IMPORTED_LINK_INTERFACE_LIBRARIES)
    get_target_property(_imported_configs "${_target}" IMPORTED_CONFIGURATIONS)
    set(_target_configs ${_configs} NOCONFIG)
    if(_imported_configs)
      list(APPEND _target_configs ${_imported_configs})
    endif()
    foreach(_config IN LISTS _target_configs)
      string(TOUPPER "${_config}" _upper)
      list(APPEND _properties "LINK_FLAGS_${_upper}" "IMPORTED_LINK_INTERFACE_LIBRARIES_${_upper}")
    endforeach()
    foreach(_property IN LISTS _properties)
      get_target_property(_entries "${_target}" "${_property}")
      if(_entries AND NOT _entries STREQUAL "_entries-NOTFOUND")
        uwvm_current_native_link_mode_guard("${_entries}")
        foreach(_entry IN LISTS _entries)
          if(_entry MATCHES [=[^\$<(LINK_ONLY|BUILD_INTERFACE|INSTALL_INTERFACE):([^<>]*)>$]=])
            set(_entry "${CMAKE_MATCH_2}")
          elseif(_entry MATCHES [=[\$<]=])
            message(FATAL_ERROR "This Native link oracle requires a concrete link expression: ${_entry}; resolve it in an explicit SDK recipe")
          endif()
          if(_property MATCHES "LINK_LIBRARIES|LINK_INTERFACE_LIBRARIES" AND TARGET "${_entry}")
            list(APPEND _queue "${_entry}")
          endif()
        endforeach()
      endif()
    endforeach()
  endwhile()
endfunction()

# Trusted SOURCE selection only. Configure reads/links but never runs a Native
# fixture, generator or exporter. All Native execution/evidence remains owned
# by the external controller. No target/policy permission is created here.
function(uwvm_select_current_native_contract target source catalog generated python expected_interface)
  foreach(path source catalog generated python)
    if(NOT IS_ABSOLUTE "${${path}}")
      message(FATAL_ERROR "Explicit absolute ${path} path is mandatory")
    endif()
  endforeach()
  if(NOT TARGET "${target}")
    message(FATAL_ERROR "Select an existing real executable target")
  endif()
  if(NOT expected_interface STREQUAL "example_interfaces/srv/AddTwoInts")
    message(FATAL_ERROR "This concrete native codec target supports only actual AddTwoInts; no generic SDK claim")
  endif()
  # This checker reads the complete actual parent/bounded/catalog/products;
  # it cannot materialize a missing output or promote an old source receipt.
  execute_process(COMMAND "${python}" -B
    "${source}/sdk/custom-types/native_current/select_bundle.py"
    --source-root "${source}" --catalog "${catalog}" --generated "${generated}"
    --ros-version 2 --expected-interface "${expected_interface}"
    RESULT_VARIABLE selection_status OUTPUT_VARIABLE selection_observation ERROR_VARIABLE selection_error
    TIMEOUT 30)
  if(NOT selection_status STREQUAL "0")
    message(FATAL_ERROR "Actual current paired source admission refused: ${selection_error}")
  endif()
  # Preserve the SOURCE-only observation in this fresh build directory. It is
  # not a Native getter/export, install proof or successful contract execution.
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${target}-source-selection.json" "${selection_observation}")
  find_package(rclcpp REQUIRED)
  find_package(rosidl_runtime_c REQUIRED)
  find_package(rosidl_runtime_cpp REQUIRED)
  find_package(rosidl_typesupport_cpp REQUIRED)
  find_package(rosidl_typesupport_introspection_cpp REQUIRED)
  find_package(example_interfaces REQUIRED)
  find_package(service_msgs REQUIRED)
  find_package(builtin_interfaces REQUIRED)
  set(actual_libraries rclcpp::rclcpp rosidl_runtime_c::rosidl_runtime_c
    rosidl_runtime_cpp::rosidl_runtime_cpp rosidl_typesupport_cpp::rosidl_typesupport_cpp
    rosidl_typesupport_introspection_cpp::rosidl_typesupport_introspection_cpp)
  foreach(package example_interfaces service_msgs builtin_interfaces)
    foreach(suffix rosidl_generator_cpp rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
      list(APPEND actual_libraries "${package}::${package}__${suffix}")
    endforeach()
  endforeach()
  foreach(library IN LISTS actual_libraries)
    if(NOT TARGET "${library}")
      message(FATAL_ERROR "Actual installed SDK imported target is missing: ${library}; no fake target/ABI fallback")
    endif()
  endforeach()
  set(selected_definitions
    "UWVM_ROS_CUSTOM_TOPIC_MANIFEST=\"manifest.hpp\""
    "UWVM_ROS_CUSTOM_ROS2_HEADER=\"native_ros2.hpp\""
    UWVM_ROS2_HAVE_TYPE_DESCRIPTION=1 UWVM_NATIVE_BOUND_CONTRACT_SELECTION=1)
  target_compile_features(${target} PRIVATE cxx_std_20)
  target_compile_options(${target} PRIVATE -UNDEBUG -MD)
  target_compile_definitions(${target} PRIVATE ${selected_definitions})
  # The actual Gateway common/config -> ipc/parameter closure selects the
  # C++20 fast_io path; it is not supplied by a ROS package or generated bundle.
  if(NOT EXISTS "${source}/third-parties/fast_io/include/fast_io_freestanding.h")
    message(FATAL_ERROR "Actual selected source fast_io dependency is missing")
  endif()
  set(selected_includes "${source}" "${source}/src" "${source}/sdk/include"
    "${source}/third-parties/fast_io/include" "${generated}")
  target_include_directories(${target} PRIVATE ${selected_includes})
  # This target uses only the keyword link signature. Peer/exporter targets
  # retain their existing ament signatures; they are separate executables.
  target_link_libraries(${target} PRIVATE ${actual_libraries})
  uwvm_current_native_link_environment(${target} ${actual_libraries})
  # Function scope prevents an outer STATIC_LIBRARY try-compile setting from
  # avoiding actual symbol resolution. This does not run the probe executable.
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_REQUIRED_INCLUDES ${selected_includes})
  set(CMAKE_REQUIRED_LIBRARIES ${actual_libraries})
  set(CMAKE_REQUIRED_DEFINITIONS -UNDEBUG)
  foreach(definition IN LISTS selected_definitions)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS "-D${definition}")
  endforeach()
  include(CheckCXXSourceCompiles)
  # The actual passed bundle MUST be rechecked on each fresh configure. A
  # previous probe cache is not proof for different source/catalog/SDK objects.
  unset(UWVM_CURRENT_NATIVE_PAIRED_LINK CACHE)
  unset(UWVM_CURRENT_NATIVE_PAIRED_LINK)
  check_cxx_source_compiles([=[
#include "gateways/ros2/custom_topics.h"
#include <example_interfaces/srv/add_two_ints.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
namespace ipc=uwvm2::imported::ros::ipc;
namespace native=uwvm2::ros_gateway::ros2_custom_topics;
using S=example_interfaces::srv::AddTwoInts;
static_assert(ipc::custom_native_interface_schema<S>::schema().record_count>0,"complete modern Service protocol schema required");
int main(){
 auto s=rosidl_typesupport_cpp::get_service_type_support_handle<S>();
 auto q=rosidl_typesupport_cpp::get_message_type_support_handle<S::Request>();
 auto r=rosidl_typesupport_cpp::get_message_type_support_handle<S::Response>();
 if(!s||!q||!r||!s->get_type_hash_func||!s->get_type_description_func||
    !q->get_type_hash_func||!q->get_type_description_func||!r->get_type_hash_func||!r->get_type_description_func)return 1;
 auto descriptor=ipc::find_custom_service(2,"example_interfaces/srv/AddTwoInts");
 return !(descriptor&&s->get_type_hash_func(s)&&s->get_type_description_func(s)&&
          q->get_type_hash_func(q)&&q->get_type_description_func(q)&&r->get_type_hash_func(r)&&r->get_type_description_func(r)&&
          native::native_interface_contract<S>(s,descriptor->type,descriptor->native_identity));
}
]=] UWVM_CURRENT_NATIVE_PAIRED_LINK)
  if(NOT UWVM_CURRENT_NATIVE_PAIRED_LINK)
    message(FATAL_ERROR "Actual paired headers/modern Service+Request+Response API/symbol compile-link failed; Native support unavailable")
  endif()
endfunction()
