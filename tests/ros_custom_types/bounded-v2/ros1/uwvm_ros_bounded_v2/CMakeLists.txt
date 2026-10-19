cmake_minimum_required(VERSION 3.16)
project(uwvm_ros_bounded_v2)
# Official message_generation owns the native schemas and MD5 traits. The UWVM
# generator runs only later against exported actual identities; it is not genmsg.
find_package(catkin REQUIRED COMPONENTS message_generation actionlib_msgs std_msgs)
add_message_files(FILES Point.msg Envelope.msg)
add_service_files(FILES Echo.srv Empty.srv)
add_action_files(FILES Digest.action)
generate_messages(DEPENDENCIES actionlib_msgs std_msgs)
catkin_package(CATKIN_DEPENDS message_runtime actionlib_msgs std_msgs)
