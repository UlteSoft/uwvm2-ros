/** @file Generated immutable C++17 fixed-topic registry.
 * Include ONLY through UWVM_ROS_CUSTOM_TOPIC_MANIFEST after descriptor types.
 * Official native identities are supplied, never calculated by this tool. */
#pragma once
namespace uwvm2::imported::ros::ipc {
inline constexpr custom_scalar_layout custom_layout_Ros1Uwvm_custom_ros1_fixturePacket[]={{custom_scalar_kind::i32,1},{custom_scalar_kind::boolean,1},{custom_scalar_kind::f64,1},{custom_scalar_kind::u8,4},{custom_scalar_kind::i16,3}};
inline constexpr custom_scalar_layout custom_layout_Ros2Uwvm_ros_custom_test_msgsFixedSample[]={{custom_scalar_kind::u32,1},{custom_scalar_kind::boolean,1},{custom_scalar_kind::f32,1},{custom_scalar_kind::f64,3},{custom_scalar_kind::u8,8},{custom_scalar_kind::i16,2}};
inline constexpr custom_topic_descriptor custom_generated_topics[]={
{1,"uwvm_custom_ros1_fixture/Packet","f9f763c3e1630ff9c5652c744e59a68a","UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544",23,custom_layout_Ros1Uwvm_custom_ros1_fixturePacket,sizeof(custom_layout_Ros1Uwvm_custom_ros1_fixturePacket)/sizeof(custom_layout_Ros1Uwvm_custom_ros1_fixturePacket[0])},
{2,"uwvm_ros_custom_test_msgs/msg/FixedSample","RIHS01_5026d3427fd5649d1cb11a7cf9e79429d2775096e9bec98669dde689b4d20fc2","UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516",45,custom_layout_Ros2Uwvm_ros_custom_test_msgsFixedSample,sizeof(custom_layout_Ros2Uwvm_ros_custom_test_msgsFixedSample)/sizeof(custom_layout_Ros2Uwvm_ros_custom_test_msgsFixedSample[0])},
};
inline constexpr custom_topic_registry custom_topic_entries() noexcept {return {custom_generated_topics,sizeof(custom_generated_topics)/sizeof(custom_generated_topics[0])};}
}
