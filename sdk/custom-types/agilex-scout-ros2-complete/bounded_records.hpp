/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for geometry_msgs/msg/Point. */
using Ros2Geometry_msgsMsgPoint=roswasmc_bounded_2_geometry_msgs_msg_Point;
/** Complete bounded guest value for geometry_msgs/msg/Quaternion. */
using Ros2Geometry_msgsMsgQuaternion=roswasmc_bounded_2_geometry_msgs_msg_Quaternion;
/** Complete bounded guest value for geometry_msgs/msg/Pose. */
using Ros2Geometry_msgsMsgPose=roswasmc_bounded_2_geometry_msgs_msg_Pose;
/** Complete bounded guest value for geometry_msgs/msg/PoseWithCovariance. */
using Ros2Geometry_msgsMsgPoseWithCovariance=roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance;
/** Complete bounded guest value for geometry_msgs/msg/Vector3. */
using Ros2Geometry_msgsMsgVector3=roswasmc_bounded_2_geometry_msgs_msg_Vector3;
/** Complete bounded guest value for geometry_msgs/msg/Twist. */
using Ros2Geometry_msgsMsgTwist=roswasmc_bounded_2_geometry_msgs_msg_Twist;
/** Complete bounded guest value for geometry_msgs/msg/TwistWithCovariance. */
using Ros2Geometry_msgsMsgTwistWithCovariance=roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for nav_msgs/msg/Odometry. */
using Ros2Nav_msgsMsgOdometry=roswasmc_bounded_2_nav_msgs_msg_Odometry;
/** Complete bounded guest value for scout_msgs/msg/ScoutActuatorState. */
using Ros2Scout_msgsMsgScoutActuatorState=roswasmc_bounded_2_scout_msgs_msg_ScoutActuatorState;
/** Complete bounded guest value for scout_msgs/msg/ScoutLightCmd. */
using Ros2Scout_msgsMsgScoutLightCmd=roswasmc_bounded_2_scout_msgs_msg_ScoutLightCmd;
/** Complete bounded guest value for scout_msgs/msg/ScoutLightState. */
using Ros2Scout_msgsMsgScoutLightState=roswasmc_bounded_2_scout_msgs_msg_ScoutLightState;
/** Complete bounded guest value for scout_msgs/msg/ScoutRCState. */
using Ros2Scout_msgsMsgScoutRCState=roswasmc_bounded_2_scout_msgs_msg_ScoutRCState;
/** Complete bounded guest value for scout_msgs/msg/ScoutStatus. */
using Ros2Scout_msgsMsgScoutStatus=roswasmc_bounded_2_scout_msgs_msg_ScoutStatus;
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
/** Checked codec for geometry_msgs/msg/Vector3; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgVector3> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_5adebf211fe375fba92c0628f9b8ebf05bff9ab22ddc4bbf00629167af295d5e";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgVector3 const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Vector3_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgVector3& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Vector3_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/Twist; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgTwist> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_1a6c18d19bb6853a2cff3b775974ba31f8e3674c1bbe566444ccabf0ef9d117e";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgTwist const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Twist_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgTwist& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Twist_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/TwistWithCovariance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgTwistWithCovariance> {
  static constexpr size_t max_wire_bytes=336;
  static constexpr char const* codec_identity="UWVMROS1_96c9fddad7b7c240e850d8c27ddb5f8343764cac791ef9e210196f7941aaf404";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgTwistWithCovariance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgTwistWithCovariance& out) {return !roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_msgs/msg/Header; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_msgsMsgHeader> {
  static constexpr size_t max_wire_bytes=140;
  static constexpr char const* codec_identity="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for nav_msgs/msg/Odometry; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Nav_msgsMsgOdometry> {
  static constexpr size_t max_wire_bytes=952;
  static constexpr char const* codec_identity="UWVMROS1_fdfdd74c211c22bfd167c3e33a098fe21198cb095bbd0313fc765f38b1f334ec";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Nav_msgsMsgOdometry default_value() noexcept {uwvm_custom::Ros2Nav_msgsMsgOdometry value={0};(void)roswasmc_bounded_2_nav_msgs_msg_Odometry_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Nav_msgsMsgOdometry const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_nav_msgs_msg_Odometry_valid(&value))return false;size_t n=roswasmc_bounded_2_nav_msgs_msg_Odometry_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_nav_msgs_msg_Odometry_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Nav_msgsMsgOdometry& out) {return !roswasmc_bounded_2_nav_msgs_msg_Odometry_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/msg/ScoutActuatorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Scout_msgsMsgScoutActuatorState> {
  static constexpr size_t max_wire_bytes=19;
  static constexpr char const* codec_identity="UWVMROS1_e7955c07aedaf42f8d97f873d60ae5c0dfc562faa1d64848f11b333f797350c6";
  static bool encode(uwvm_custom::Ros2Scout_msgsMsgScoutActuatorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_scout_msgs_msg_ScoutActuatorState_valid(&value))return false;size_t n=roswasmc_bounded_2_scout_msgs_msg_ScoutActuatorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_scout_msgs_msg_ScoutActuatorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Scout_msgsMsgScoutActuatorState& out) {return !roswasmc_bounded_2_scout_msgs_msg_ScoutActuatorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/msg/ScoutLightCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Scout_msgsMsgScoutLightCmd> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_a82206813682fbf1a6b41009c12191ad9ce47c98444d8ea9792e7b212ee602d7";
  static bool encode(uwvm_custom::Ros2Scout_msgsMsgScoutLightCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_scout_msgs_msg_ScoutLightCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_scout_msgs_msg_ScoutLightCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_scout_msgs_msg_ScoutLightCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Scout_msgsMsgScoutLightCmd& out) {return !roswasmc_bounded_2_scout_msgs_msg_ScoutLightCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/msg/ScoutLightState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Scout_msgsMsgScoutLightState> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_4f42b9365cc76673b156e725aac9448e0f0d806105d3717cdb2628c384a6d76a";
  static bool encode(uwvm_custom::Ros2Scout_msgsMsgScoutLightState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_scout_msgs_msg_ScoutLightState_valid(&value))return false;size_t n=roswasmc_bounded_2_scout_msgs_msg_ScoutLightState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_scout_msgs_msg_ScoutLightState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Scout_msgsMsgScoutLightState& out) {return !roswasmc_bounded_2_scout_msgs_msg_ScoutLightState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/msg/ScoutRCState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Scout_msgsMsgScoutRCState> {
  static constexpr size_t max_wire_bytes=9;
  static constexpr char const* codec_identity="UWVMROS1_ba13e4c728041d14352c021fe5c1c5eee5d531c84ac64c61d944d6192dcd4298";
  static bool encode(uwvm_custom::Ros2Scout_msgsMsgScoutRCState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_scout_msgs_msg_ScoutRCState_valid(&value))return false;size_t n=roswasmc_bounded_2_scout_msgs_msg_ScoutRCState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_scout_msgs_msg_ScoutRCState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Scout_msgsMsgScoutRCState& out) {return !roswasmc_bounded_2_scout_msgs_msg_ScoutRCState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/msg/ScoutStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Scout_msgsMsgScoutStatus> {
  static constexpr size_t max_wire_bytes=249;
  static constexpr char const* codec_identity="UWVMROS1_75db4db1dcc07210d543b8e6bd6b6a7aadbf23db8043ff372a3b224c5c235b86";
  static bool encode(uwvm_custom::Ros2Scout_msgsMsgScoutStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_scout_msgs_msg_ScoutStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_scout_msgs_msg_ScoutStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_scout_msgs_msg_ScoutStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Scout_msgsMsgScoutStatus& out) {return !roswasmc_bounded_2_scout_msgs_msg_ScoutStatus_decode(bytes.data(),bytes.size(),&out);}
};
}
