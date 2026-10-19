cmake_minimum_required(VERSION 3.20)
project(uwvm_unitree_text_source_contract LANGUAGES CXX)
option(UWVM_BUILD_UNITREE_TEXT_SOURCE_CONTRACT "Build local formatter source/value contract only" OFF)
set(UWVM_SOURCE_ROOT "" CACHE PATH "Actual reviewed complete source root")
if(UWVM_BUILD_UNITREE_TEXT_SOURCE_CONTRACT)
  if(NOT IS_ABSOLUTE "${UWVM_SOURCE_ROOT}" OR
      NOT EXISTS "${UWVM_SOURCE_ROOT}/third-parties/fast_io/include/fast_io.h" OR
      NOT EXISTS "${UWVM_SOURCE_ROOT}/sdk/robots/unitree/examples/direct_text.h")
    message(FATAL_ERROR "Actual complete source/fast_io/header inputs required")
  endif()
  add_executable(unitree_text_contract formatter_contract.cpp)
  target_compile_features(unitree_text_contract PRIVATE cxx_std_20)
  target_compile_options(unitree_text_contract PRIVATE -UNDEBUG)
  target_include_directories(unitree_text_contract PRIVATE
    "${UWVM_SOURCE_ROOT}/sdk/robots/unitree/examples"
    "${UWVM_SOURCE_ROOT}/third-parties/fast_io/include")
endif()
# No source generation, toolchain download, try_run, add_test or automatic run.
