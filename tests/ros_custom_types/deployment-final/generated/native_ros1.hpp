/** Materialized exact native provider row sets; never macro-overwrite substitution. */
#pragma once
#include "fixed/native_ros1.hpp"
#undef UWVM_ROS_CUSTOM_ROS1_TOPIC_TYPES
#undef UWVM_ROS_CUSTOM_ROS1_TYPES
#include "bounded/native_ros1.hpp"
#undef UWVM_ROS_CUSTOM_ROS1_TOPIC_TYPES
#undef UWVM_ROS_CUSTOM_ROS1_TYPES
#undef UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES
#undef UWVM_ROS_CUSTOM_ROS1_ACTION_TYPES
#define UWVM_ROS_CUSTOM_ROS1_FIXED_TYPES(M) \
  M(::uwvm_custom_ros1_fixture::Packet,"uwvm_custom_ros1_fixture/Packet","f9f763c3e1630ff9c5652c744e59a68a","UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544",23,::uwvm_custom_native::Ros1Uwvm_custom_ros1_fixturePacketCodec)
#define UWVM_ROS_CUSTOM_ROS1_BOUNDED_TYPES(M) \
  M(::uwvm_ros_bounded_v2::Point,"uwvm_ros_bounded_v2/Point","d01b93b38572826dbde8043bc3a87849","UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28",5,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2PointCodec) \
  M(::uwvm_ros_bounded_v2::Envelope,"uwvm_ros_bounded_v2/Envelope","76b87d274fb061da1933bd67ff4757f6","UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4",120,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EnvelopeCodec)
#define UWVM_ROS_CUSTOM_ROS1_TYPES(M) \
  M(::uwvm_custom_ros1_fixture::Packet,"uwvm_custom_ros1_fixture/Packet","f9f763c3e1630ff9c5652c744e59a68a","UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544",23,::uwvm_custom_native::Ros1Uwvm_custom_ros1_fixturePacketCodec) \
  M(::uwvm_ros_bounded_v2::Point,"uwvm_ros_bounded_v2/Point","d01b93b38572826dbde8043bc3a87849","UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28",5,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2PointCodec) \
  M(::uwvm_ros_bounded_v2::Envelope,"uwvm_ros_bounded_v2/Envelope","76b87d274fb061da1933bd67ff4757f6","UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4",120,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EnvelopeCodec)
#define UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES(M) \
  M(::uwvm_ros_bounded_v2::Echo,"uwvm_ros_bounded_v2/Echo","b03988b8955d560e020d9dde463a8cfb","UWVMROS1_82964f2f9649d7066eca130ee0eed9500fd90ca4744518a4c4bb12cf79aae64e",::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EchoRequestCodec,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EchoResponseCodec) \
  M(::uwvm_ros_bounded_v2::Empty,"uwvm_ros_bounded_v2/Empty","d41d8cd98f00b204e9800998ecf8427e","UWVMROS1_297cd9938f8c2dc7d89d2a439d4e5a82aa5e591c379f8ff42692bf53b3b3cd33",::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EmptyRequestCodec,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2EmptyResponseCodec)
#define UWVM_ROS_CUSTOM_ROS1_ACTION_TYPES(M) \
  M(::uwvm_ros_bounded_v2::DigestAction,"uwvm_ros_bounded_v2/Digest","0091d83f95485b037b22ed41ee04754f","UWVMROS1_36823c6a605aaac10b1b0f8d289209ca802e1c2227c7cb0ee2bebc0431b3508e",::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2DigestGoalCodec,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2DigestResultCodec,::uwvm_custom_bounded_native::Ros1Uwvm_ros_bounded_v2DigestFeedbackCodec)
#define UWVM_ROS_CUSTOM_ROS1_TOPIC_TYPES(M) UWVM_ROS_CUSTOM_ROS1_TYPES(M)
