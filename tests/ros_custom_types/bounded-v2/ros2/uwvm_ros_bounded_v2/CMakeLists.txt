cmake_minimum_required(VERSION 3.16)
project(uwvm_ros_bounded_v2)
# Official rosidl emits the complete native interfaces, component type hashes
# and description closure. No UWVM adapter registry is produced by this step.
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Point.msg" "msg/Envelope.msg" "srv/Echo.srv" "srv/Empty.srv"
  "action/Digest.action")
ament_export_dependencies(rosidl_default_runtime)
ament_package()
