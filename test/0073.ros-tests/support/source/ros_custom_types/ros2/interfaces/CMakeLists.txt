cmake_minimum_required(VERSION 3.16)
project(uwvm_ros_custom_test_msgs)
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# This is the official native interface-generation step. It must precede the
# uwvm adapter generator, which consumes its authoritative description/hash
# artifacts rather than inventing or accepting a Guest-reported native hash.
rosidl_generate_interfaces(${PROJECT_NAME} "msg/FixedSample.msg")
ament_export_dependencies(rosidl_default_runtime)
ament_package()
