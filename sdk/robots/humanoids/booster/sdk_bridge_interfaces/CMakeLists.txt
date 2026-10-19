cmake_minimum_required(VERSION 3.16)
project(uwvm_booster_sdk_bridge LANGUAGES C CXX)
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME} "msg/ImuState.msg" "msg/MotorCmd.msg"
  "msg/MotorState.msg" "msg/LowCmd.msg" "msg/LowState.msg")
ament_export_dependencies(rosidl_default_runtime)
ament_package()
