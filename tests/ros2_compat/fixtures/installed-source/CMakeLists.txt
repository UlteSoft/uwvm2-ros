cmake_minimum_required(VERSION 3.16)
project(uwvm_source_proof_msgs)
# This owned fixture uses the genuine selected SDK generators. It is neither an
# upstream vendor package nor an official ROS test. No node is built or started.
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(action_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(unique_identifier_msgs REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME} "msg/Child.msg" "msg/Defaults.msg"
  "srv/Transform.srv" "action/Traverse.action"
  DEPENDENCIES action_msgs builtin_interfaces unique_identifier_msgs)
ament_export_dependencies(rosidl_default_runtime)
ament_package()
