/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for geometry_msgs/msg/Point. */
using Ros2Geometry_msgsMsgPoint=roswasmc_bounded_2_geometry_msgs_msg_Point;
/** Complete bounded guest value for geometry_msgs/msg/Quaternion. */
using Ros2Geometry_msgsMsgQuaternion=roswasmc_bounded_2_geometry_msgs_msg_Quaternion;
/** Complete bounded guest value for geometry_msgs/msg/Pose. */
using Ros2Geometry_msgsMsgPose=roswasmc_bounded_2_geometry_msgs_msg_Pose;
/** Complete bounded guest value for geometry_msgs/msg/PoseWithCovariance. */
using Ros2Geometry_msgsMsgPoseWithCovariance=roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance;
/** Complete bounded guest value for geometry_msgs/msg/PoseWithCovarianceStamped. */
using Ros2Geometry_msgsMsgPoseWithCovarianceStamped=roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovarianceStamped;
/** Complete bounded guest value for isaac_ros_apriltag_interfaces/msg/AprilTagDetection. */
using Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection=roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetection;
/** Complete bounded guest value for isaac_ros_apriltag_interfaces/msg/AprilTagDetectionArray. */
using Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray=roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetectionArray;
}
namespace roswasmcpp {
/** Checked codec for builtin_interfaces/msg/Time; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Builtin_interfacesMsgTime> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7";
  static bool encode(uwvm_custom::Ros2Builtin_interfacesMsgTime const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(&value))return false;size_t n=roswasmc_bounded_2_builtin_interfaces_msg_Time_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_builtin_interfaces_msg_Time_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Builtin_interfacesMsgTime& out) {return !roswasmc_bounded_2_builtin_interfaces_msg_Time_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_msgs/msg/Header; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_msgsMsgHeader> {
  static constexpr size_t max_wire_bytes=76;
  static constexpr char const* codec_identity="UWVMROS1_afba9e74be3c5e3321750c1c4c30ba2925269f75b9da43a6d5c1e93d293d7396";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/Point; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgPoint> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_80f43d054abe333673c940c6d0fd88e8e26cea4a1cff53ff789cccaa38240616";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Point_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Point_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Point_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgPoint& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Point_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/Quaternion; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgQuaternion> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_34a32c3470cbfe2ff12340781a5bf22f6c370a8f5c06669f25a34321ade81672";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Geometry_msgsMsgQuaternion default_value() noexcept {uwvm_custom::Ros2Geometry_msgsMsgQuaternion value={0};(void)roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgQuaternion const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Quaternion_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgQuaternion& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Quaternion_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/Pose; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgPose> {
  static constexpr size_t max_wire_bytes=56;
  static constexpr char const* codec_identity="UWVMROS1_9468a10b8b2728fe71a370b4348ffe5e81b884ff957533f8700aa16e70b12777";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Geometry_msgsMsgPose default_value() noexcept {uwvm_custom::Ros2Geometry_msgsMsgPose value={0};(void)roswasmc_bounded_2_geometry_msgs_msg_Pose_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgPose const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Pose_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Pose_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Pose_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgPose& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Pose_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/PoseWithCovariance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovariance> {
  static constexpr size_t max_wire_bytes=344;
  static constexpr char const* codec_identity="UWVMROS1_8eb3e25c45769eb29691011c05315ed50f2b97eff488f9e8ee2dc70bddab4dc2";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovariance default_value() noexcept {uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovariance value={0};(void)roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovariance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovariance& out) {return !roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/PoseWithCovarianceStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovarianceStamped> {
  static constexpr size_t max_wire_bytes=420;
  static constexpr char const* codec_identity="UWVMROS1_ea380172cde0ee5fef319aff50d92a80c314510e3d12efec45e9ae1f74974af3";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovarianceStamped default_value() noexcept {uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovarianceStamped value={0};(void)roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovarianceStamped_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovarianceStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovarianceStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovarianceStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovarianceStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgPoseWithCovarianceStamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovarianceStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for isaac_ros_apriltag_interfaces/msg/AprilTagDetection; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection> {
  static constexpr size_t max_wire_bytes=612;
  static constexpr char const* codec_identity="UWVMROS1_5d840f0d9239a948ce5d17bbee6e248db8151ff31070667e506e9c1647ac6fe7";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection default_value() noexcept {uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection value={0};(void)roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetection_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetection_valid(&value))return false;size_t n=roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetection_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetection_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection& out) {return !roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetection_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for isaac_ros_apriltag_interfaces/msg/AprilTagDetectionArray; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray> {
  static constexpr size_t max_wire_bytes=9872;
  static constexpr char const* codec_identity="UWVMROS1_60359e405aedd2a627e60bc35e849989fe5ed1d426afb2a884036aef93bfe0a5";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray default_value() noexcept {uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray value={0};(void)roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetectionArray_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetectionArray_valid(&value))return false;size_t n=roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetectionArray_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetectionArray_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray& out) {return !roswasmc_bounded_2_isaac_ros_apriltag_interfaces_msg_AprilTagDetectionArray_decode(bytes.data(),bytes.size(),&out);}
};
}
