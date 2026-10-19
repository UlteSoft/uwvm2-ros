# SPDX-License-Identifier: Apache-2.0
include(CheckCXXSourceCompiles)
function(uwvm_probe_wasi_signal output)
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "WASI" OR
    NOT CMAKE_CXX_COMPILER_TARGET MATCHES "^wasm32-(wasi|wasip1)-threads$")
    set(${output} 0 PARENT_SCOPE)
    return()
  endif()
  # Function scope prevents STATIC_LIBRARY from bypassing the real link and
  # prevents any caller's try_compile/required flag state from being changed.
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  # A permissive unresolved-symbol switch cannot qualify a genuine library link.
  # This covers the actual supplied flag/library strings, not arbitrary
  # transitive imported-target/response-file contents. Full final-command,
  # archive and module import/MD audit remains independently mandatory.
  set(_selected_link_flags "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_REQUIRED_FLAGS} ${CMAKE_REQUIRED_LINK_OPTIONS} ${CMAKE_REQUIRED_LIBRARIES}")
  set(_configs DEBUG RELEASE RELWITHDEBINFO MINSIZEREL ${CMAKE_BUILD_TYPE} ${CMAKE_TRY_COMPILE_CONFIGURATION})
  foreach(_selected_config IN LISTS _configs)
    string(TOUPPER "${_selected_config}" _config)
    string(APPEND _selected_link_flags " ${CMAKE_CXX_FLAGS_${_config}} ${CMAKE_EXE_LINKER_FLAGS_${_config}}")
  endforeach()
  if(_selected_link_flags MATCHES "--(allow-undefined|import-undefined|unresolved-symbols)")
    message(FATAL_ERROR "A permissive unresolved-symbol link cannot admit this platform")
  endif()
  set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -pthread -matomics -mbulk-memory -fwasm-exceptions -mllvm -wasm-use-legacy-eh=false -D_WASI_EMULATED_SIGNAL")
  list(APPEND CMAKE_REQUIRED_LIBRARIES unwind)
  list(APPEND CMAKE_REQUIRED_LINK_OPTIONS -pthread -fwasm-exceptions
    -Wl,--import-memory -Wl,--export-memory)
  file(READ "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../probes/threaded_signal.cpp" actual_source)
  # No inherited/future cache value can supply qualification for this source.
  unset(_UWVM_REAL_WASI_SIGNAL_LINK CACHE)
  unset(_UWVM_REAL_WASI_SIGNAL_LINK)
  check_cxx_source_compiles("${actual_source}" _UWVM_REAL_WASI_SIGNAL_LINK)
  if(_UWVM_REAL_WASI_SIGNAL_LINK)
    set(${output} 1 PARENT_SCOPE)
  else()
    set(${output} 0 PARENT_SCOPE)
  endif()
  unset(_UWVM_REAL_WASI_SIGNAL_LINK CACHE)
endfunction()
