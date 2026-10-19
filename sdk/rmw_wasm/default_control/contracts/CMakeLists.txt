# Build only against the real output of this source profile. No replacement
# ROS types, generated test headers or configure-time executions are provided.
cmake_minimum_required(VERSION 3.20)
project(uwvm_default_control_canonical_contracts LANGUAGES C)
set(UWVM_DEFAULT_CONTROL_CANONICAL_DIR "" CACHE PATH
    "Fresh actual default-control C/C++ canonical generator output")
if(NOT IS_ABSOLUTE "${UWVM_DEFAULT_CONTROL_CANONICAL_DIR}")
  message(FATAL_ERROR "Supply the absolute fresh canonical generator directory")
endif()
foreach(_input bounded_records.h control-schema.json generation.json)
  if(NOT EXISTS "${UWVM_DEFAULT_CONTROL_CANONICAL_DIR}/${_input}")
    message(FATAL_ERROR "Missing actual generated input: ${_input}")
  endif()
endforeach()
add_executable(uwvm_default_control_hash_capacity type_hash_capacity.c)
target_compile_features(uwvm_default_control_hash_capacity PRIVATE c_std_11)
target_include_directories(uwvm_default_control_hash_capacity PRIVATE
  "${UWVM_DEFAULT_CONTROL_CANONICAL_DIR}")
# Contract assertions must remain enabled even in a release build.
if(MSVC)
  target_compile_options(uwvm_default_control_hash_capacity PRIVATE /UNDEBUG)
else()
  target_compile_options(uwvm_default_control_hash_capacity PRIVATE -UNDEBUG)
endif()
# Execution is explicit and belongs to the external resource/timeout harness.
