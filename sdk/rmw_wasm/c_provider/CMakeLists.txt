cmake_minimum_required(VERSION 3.20)
project(uwvm_rmw_c_contract LANGUAGES C)
# Build ONLY after exact A/B/C integration into a genuine admitted SDK prefix.
# Never put sdk/compat/rclc/include on this project's include search path.
find_package(ament_cmake REQUIRED)
find_package(rmw_uwvm REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_typesupport_introspection_c REQUIRED)
add_library(uwvm_rmw_add_two_ints_codec STATIC ../src/add_two_ints_codec.c)
target_compile_features(uwvm_rmw_add_two_ints_codec PUBLIC c_std_11)
target_link_libraries(uwvm_rmw_add_two_ints_codec PUBLIC rmw_uwvm::rmw_uwvm
 example_interfaces::example_interfaces__rosidl_generator_c
 example_interfaces::example_interfaces__rosidl_typesupport_c
 example_interfaces::example_interfaces__rosidl_typesupport_introspection_c)
add_executable(uwvm_rmw_c_models tests/contract.c)
target_link_libraries(uwvm_rmw_c_models PRIVATE uwvm_rmw_add_two_ints_codec)
# Contract operations and assertions must execute in Release as well. The
# source also rejects NDEBUG, including a conflicting later injected option.
if(MSVC)
 target_compile_options(uwvm_rmw_c_models PRIVATE /UNDEBUG)
else()
 target_compile_options(uwvm_rmw_c_models PRIVATE -UNDEBUG)
endif()
# This binary is a host-local logical transport model. It does not attest D's
# wire/native implementation and must never be counted as native ROS delivery.
