# Original source cross-build only. The actual tools/sysroot must be admitted by
# Root's source/tool owner BEFORE invoking CMake; these are paths, not forecasts.
set(CMAKE_SYSTEM_NAME WASI)
set(CMAKE_SYSTEM_PROCESSOR wasm32)
foreach(_role UWVM_WASI_CC UWVM_WASI_CXX UWVM_WASI_AR UWVM_WASI_RANLIB UWVM_WASI_SYSROOT UWVM_CROSS_PREFIX)
  if(NOT DEFINED ${_role} OR NOT IS_ABSOLUTE "${${_role}}" OR NOT EXISTS "${${_role}}")
    message(FATAL_ERROR "Missing admitted actual ${_role}")
  endif()
endforeach()
set(CMAKE_C_COMPILER "${UWVM_WASI_CC}")
set(CMAKE_CXX_COMPILER "${UWVM_WASI_CXX}")
set(CMAKE_AR "${UWVM_WASI_AR}")
set(CMAKE_RANLIB "${UWVM_WASI_RANLIB}")
set(CMAKE_SYSROOT "${UWVM_WASI_SYSROOT}")
set(CMAKE_C_COMPILER_TARGET wasm32-wasi)
set(CMAKE_CXX_COMPILER_TARGET wasm32-wasi)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_FIND_ROOT_PATH "${UWVM_WASI_SYSROOT};${UWVM_CROSS_PREFIX}")
# Host Python/CMake/generators execute on the host; headers/libs/packages do not.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
# CMake re-reads a toolchain in try_compile projects; preserve explicit inputs.
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
  UWVM_WASI_CC UWVM_WASI_CXX UWVM_WASI_AR UWVM_WASI_RANLIB UWVM_WASI_SYSROOT UWVM_CROSS_PREFIX)
