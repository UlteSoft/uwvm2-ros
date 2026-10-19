cmake_minimum_required(VERSION 3.16)
project(uwvm_limx_bridge LANGUAGES C CXX)
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/NativeHeader.msg" "msg/ImuData.msg" "msg/JointState.msg"
  "msg/JointCmd.msg" "msg/JointCmdLimx.msg")
ament_export_dependencies(rosidl_default_runtime)
ament_package()
