cmake_minimum_required(VERSION 3.16)
project(uwvm_spot_bridge)
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/State.srv" "srv/Lease.srv" "srv/Command.srv" "srv/Velocity.srv" "srv/Feedback.srv")
# Python installation is intentionally in the separate native_package project;
# do not combine rosidl's Python install with ament_python_install_package here.
ament_export_dependencies(rosidl_default_runtime)
ament_package()
