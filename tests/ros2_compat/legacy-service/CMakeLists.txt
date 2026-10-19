cmake_minimum_required(VERSION 3.16)
project(uwvm_legacy_service_native_contract LANGUAGES CXX)
# Root alone configures/builds/runs. This entry neither downloads dependencies
# nor fabricates type support, traits, package metadata or a compiler provider.
set(UWVM_SOURCE "" CACHE PATH "Actual complete current uwvm source")
if(NOT IS_ABSOLUTE "${UWVM_SOURCE}" OR NOT EXISTS "${UWVM_SOURCE}/gateways/ros2/compat/metadata_export.h")
  message(FATAL_ERROR "Select actual frozen current source")
endif()
find_package(ament_cmake REQUIRED)
find_package(std_srvs REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rosidl_typesupport_cpp REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)
add_executable(legacy-service-native-contract native_contract.cc)
target_compile_features(legacy-service-native-contract PRIVATE cxx_std_17)
target_compile_definitions(legacy-service-native-contract PRIVATE UWVM_ROS2_HAVE_OPENSSL=1)
target_include_directories(legacy-service-native-contract PRIVATE "${UWVM_SOURCE}" "${UWVM_SOURCE}/src")
ament_target_dependencies(legacy-service-native-contract std_srvs builtin_interfaces rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
target_link_libraries(legacy-service-native-contract PRIVATE OpenSSL::Crypto)
# Actual final argv/MD target/system headers and real link trace must be retained;
# these options alone are not evidence that the owner consumed their outputs.
target_compile_options(legacy-service-native-contract PRIVATE -O1 -g0 -UNDEBUG -std=gnu++17 -MD
  -MF "${CMAKE_BINARY_DIR}/CMakeFiles/legacy-service-native-contract.dir/native_contract.cc.o.d"
  -MT "${CMAKE_BINARY_DIR}/CMakeFiles/legacy-service-native-contract.dir/native_contract.cc.o")
target_link_options(legacy-service-native-contract PRIVATE -Wl,-t)
