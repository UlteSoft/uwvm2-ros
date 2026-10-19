cmake_minimum_required(VERSION 3.10)
project(uwvm_custom_ros1_fixture)

# Generate the real native C++ message through the official ROS1 toolchain.
# The trusted UWVM adapter generator consumes the matching definition and native
# identity after this package is built; this package contains no adapter registry.
find_package(catkin REQUIRED COMPONENTS message_generation)
add_message_files(FILES Packet.msg)
generate_messages()
catkin_package(CATKIN_DEPENDS message_runtime)
