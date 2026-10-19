# Load with -C for EVERY cross package. Do not pass the facade include path.
foreach(_name BUILD_SHARED_LIBS BUILD_TESTING)
  if(DEFINED ${_name} AND ${_name})
    message(FATAL_ERROR "${_name}=ON conflicts with the closed static build recipe")
  endif()
  set(${_name} OFF CACHE BOOL "Explicit static cross-build" FORCE)
endforeach()
foreach(_pair
    "RMW_IMPLEMENTATION=rmw_uwvm"
    "RCL_LOGGING_IMPLEMENTATION=rcl_logging_uwvm"
    "STATIC_ROSIDL_TYPESUPPORT_C=rosidl_typesupport_introspection_c"
    "STATIC_ROSIDL_TYPESUPPORT_CPP=rosidl_typesupport_introspection_cpp")
  string(REPLACE "=" ";" _parts "${_pair}")
  list(GET _parts 0 _name)
  list(GET _parts 1 _value)
  if(DEFINED ${_name} AND NOT "${${_name}}" STREQUAL "${_value}")
    message(FATAL_ERROR "Conflicting ${_name}")
  endif()
  if(NOT "$ENV{${_name}}" STREQUAL "" AND NOT "$ENV{${_name}}" STREQUAL "${_value}")
    message(FATAL_ERROR "Conflicting environment ${_name}; do not inherit another native typesupport")
  endif()
  set(${_name} "${_value}" CACHE STRING "One actual original implementation" FORCE)
endforeach()
set(RMW_IMPLEMENTATION_DISABLE_RUNTIME_SELECTION ON CACHE BOOL "Official static RMW forwarding" FORCE)
set(CMAKE_PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/project-hook.cmake" CACHE FILEPATH "Exact scoped original platform source hook" FORCE)
