/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for geometry_msgs/Point. */
using Ros1Geometry_msgsPoint=roswasmc_bounded_1_geometry_msgs_Point;
/** Complete bounded guest value for geometry_msgs/Quaternion. */
using Ros1Geometry_msgsQuaternion=roswasmc_bounded_1_geometry_msgs_Quaternion;
/** Complete bounded guest value for geometry_msgs/Pose. */
using Ros1Geometry_msgsPose=roswasmc_bounded_1_geometry_msgs_Pose;
/** Complete bounded guest value for geometry_msgs/PoseWithCovariance. */
using Ros1Geometry_msgsPoseWithCovariance=roswasmc_bounded_1_geometry_msgs_PoseWithCovariance;
/** Complete bounded guest value for geometry_msgs/Vector3. */
using Ros1Geometry_msgsVector3=roswasmc_bounded_1_geometry_msgs_Vector3;
/** Complete bounded guest value for geometry_msgs/Twist. */
using Ros1Geometry_msgsTwist=roswasmc_bounded_1_geometry_msgs_Twist;
/** Complete bounded guest value for geometry_msgs/TwistWithCovariance. */
using Ros1Geometry_msgsTwistWithCovariance=roswasmc_bounded_1_geometry_msgs_TwistWithCovariance;
/** Complete bounded guest value for time. */
using Ros1Time=roswasmc_bounded_1_time;
/** Complete bounded guest value for std_msgs/Header. */
using Ros1Std_msgsHeader=roswasmc_bounded_1_std_msgs_Header;
/** Complete bounded guest value for nav_msgs/Odometry. */
using Ros1Nav_msgsOdometry=roswasmc_bounded_1_nav_msgs_Odometry;
/** Complete bounded guest value for scout_msgs/ScoutBmsStatus. */
using Ros1Scout_msgsScoutBmsStatus=roswasmc_bounded_1_scout_msgs_ScoutBmsStatus;
/** Complete bounded guest value for scout_msgs/ScoutDriverState. */
using Ros1Scout_msgsScoutDriverState=roswasmc_bounded_1_scout_msgs_ScoutDriverState;
/** Complete bounded guest value for scout_msgs/ScoutLightCmd. */
using Ros1Scout_msgsScoutLightCmd=roswasmc_bounded_1_scout_msgs_ScoutLightCmd;
/** Complete bounded guest value for scout_msgs/ScoutLightState. */
using Ros1Scout_msgsScoutLightState=roswasmc_bounded_1_scout_msgs_ScoutLightState;
/** Complete bounded guest value for scout_msgs/ScoutMotorState. */
using Ros1Scout_msgsScoutMotorState=roswasmc_bounded_1_scout_msgs_ScoutMotorState;
/** Complete bounded guest value for scout_msgs/ScoutRsStatus. */
using Ros1Scout_msgsScoutRsStatus=roswasmc_bounded_1_scout_msgs_ScoutRsStatus;
/** Complete bounded guest value for scout_msgs/ScoutStatus. */
using Ros1Scout_msgsScoutStatus=roswasmc_bounded_1_scout_msgs_ScoutStatus;
}
namespace roswasmcpp {
/** Checked codec for geometry_msgs/Point; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsPoint> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_b55e2bde60f6cedb426cada54bc7baefc536c7bc731bd49fb152d89a892de62b";
  static bool encode(uwvm_custom::Ros1Geometry_msgsPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Point_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Point_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Point_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsPoint& out) {return !roswasmc_bounded_1_geometry_msgs_Point_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Quaternion; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsQuaternion> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_48fe20d5d29a8ca1a630605a58f80baacc52d9a65b1ca87efd50ffdd52882934";
  static bool encode(uwvm_custom::Ros1Geometry_msgsQuaternion const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Quaternion_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Quaternion_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Quaternion_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsQuaternion& out) {return !roswasmc_bounded_1_geometry_msgs_Quaternion_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Pose; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsPose> {
  static constexpr size_t max_wire_bytes=56;
  static constexpr char const* codec_identity="UWVMROS1_e49ed287cd0de31a16c8cd49ad8f41f81d44e23306b8dcb61da6e0e789595471";
  static bool encode(uwvm_custom::Ros1Geometry_msgsPose const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Pose_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Pose_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Pose_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsPose& out) {return !roswasmc_bounded_1_geometry_msgs_Pose_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/PoseWithCovariance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsPoseWithCovariance> {
  static constexpr size_t max_wire_bytes=344;
  static constexpr char const* codec_identity="UWVMROS1_17b25963aafb3adfd0432febe040c9b38a531a7a667949b89e0f3937585ef8cf";
  static bool encode(uwvm_custom::Ros1Geometry_msgsPoseWithCovariance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsPoseWithCovariance& out) {return !roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Vector3; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsVector3> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_e90efcf4803d82763e0b83f8ef5502b5d042158873fcc91be32543d25d7632bc";
  static bool encode(uwvm_custom::Ros1Geometry_msgsVector3 const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Vector3_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsVector3& out) {return !roswasmc_bounded_1_geometry_msgs_Vector3_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Twist; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsTwist> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_d5a69cfff4380fcf3e9c131ddb3dfb6def041cd7308a7ebdbc150b6483dfd921";
  static bool encode(uwvm_custom::Ros1Geometry_msgsTwist const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Twist_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Twist_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Twist_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsTwist& out) {return !roswasmc_bounded_1_geometry_msgs_Twist_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/TwistWithCovariance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsTwistWithCovariance> {
  static constexpr size_t max_wire_bytes=336;
  static constexpr char const* codec_identity="UWVMROS1_ff6790d181503b424d84726c8f4a7852cef14c6dd93e6d32cf3c37023f74bcdf";
  static bool encode(uwvm_custom::Ros1Geometry_msgsTwistWithCovariance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsTwistWithCovariance& out) {return !roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for time; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Time> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_90b75571d2bcc21b824eb993b485a001bf97e281adda7907e1b2e3357e4804b0";
  static bool encode(uwvm_custom::Ros1Time const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_time_valid(&value))return false;size_t n=roswasmc_bounded_1_time_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_time_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Time& out) {return !roswasmc_bounded_1_time_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_msgs/Header; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Std_msgsHeader> {
  static constexpr size_t max_wire_bytes=144;
  static constexpr char const* codec_identity="UWVMROS1_ee2b4e736d36ebcdb562d3ef7832777233d39d366837e9148a76aa2c36756435";
  static bool encode(uwvm_custom::Ros1Std_msgsHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_std_msgs_Header_valid(&value))return false;size_t n=roswasmc_bounded_1_std_msgs_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_std_msgs_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Std_msgsHeader& out) {return !roswasmc_bounded_1_std_msgs_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for nav_msgs/Odometry; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Nav_msgsOdometry> {
  static constexpr size_t max_wire_bytes=956;
  static constexpr char const* codec_identity="UWVMROS1_c10d3ce1f2824c33d728d0006fb83134330d0332af122f179cf2dac32181a2ca";
  static bool encode(uwvm_custom::Ros1Nav_msgsOdometry const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_nav_msgs_Odometry_valid(&value))return false;size_t n=roswasmc_bounded_1_nav_msgs_Odometry_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_nav_msgs_Odometry_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Nav_msgsOdometry& out) {return !roswasmc_bounded_1_nav_msgs_Odometry_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/ScoutBmsStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Scout_msgsScoutBmsStatus> {
  static constexpr size_t max_wire_bytes=44;
  static constexpr char const* codec_identity="UWVMROS1_6921538c08e86ab5b9ecdbf974c6cb5b580488f4ed96ccbdb8b7824d51e9cf95";
  static bool encode(uwvm_custom::Ros1Scout_msgsScoutBmsStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Scout_msgsScoutBmsStatus& out) {return !roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/ScoutDriverState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Scout_msgsScoutDriverState> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_4c56c71ae621b2b8822d742894d19946492002453c93b72faccb4f2abc9d4275";
  static bool encode(uwvm_custom::Ros1Scout_msgsScoutDriverState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_scout_msgs_ScoutDriverState_valid(&value))return false;size_t n=roswasmc_bounded_1_scout_msgs_ScoutDriverState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_scout_msgs_ScoutDriverState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Scout_msgsScoutDriverState& out) {return !roswasmc_bounded_1_scout_msgs_ScoutDriverState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/ScoutLightCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Scout_msgsScoutLightCmd> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_cf08cccf454f88111d5cc4a4b1ace08de72cf1ae9a9400c23a148126da95942a";
  static bool encode(uwvm_custom::Ros1Scout_msgsScoutLightCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_scout_msgs_ScoutLightCmd_valid(&value))return false;size_t n=roswasmc_bounded_1_scout_msgs_ScoutLightCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_scout_msgs_ScoutLightCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Scout_msgsScoutLightCmd& out) {return !roswasmc_bounded_1_scout_msgs_ScoutLightCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/ScoutLightState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Scout_msgsScoutLightState> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_af55048f9200cfa80fc419664204a515f628cc610a352ab3ed8443dd7ac0ae51";
  static bool encode(uwvm_custom::Ros1Scout_msgsScoutLightState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_scout_msgs_ScoutLightState_valid(&value))return false;size_t n=roswasmc_bounded_1_scout_msgs_ScoutLightState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_scout_msgs_ScoutLightState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Scout_msgsScoutLightState& out) {return !roswasmc_bounded_1_scout_msgs_ScoutLightState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/ScoutMotorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Scout_msgsScoutMotorState> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_ca67a704d7bb4ba6b2839db1fdb8a75bc753868ec871d101c24c85d8202a79b7";
  static bool encode(uwvm_custom::Ros1Scout_msgsScoutMotorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_scout_msgs_ScoutMotorState_valid(&value))return false;size_t n=roswasmc_bounded_1_scout_msgs_ScoutMotorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_scout_msgs_ScoutMotorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Scout_msgsScoutMotorState& out) {return !roswasmc_bounded_1_scout_msgs_ScoutMotorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/ScoutRsStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Scout_msgsScoutRsStatus> {
  static constexpr size_t max_wire_bytes=153;
  static constexpr char const* codec_identity="UWVMROS1_deb068b75fb629df3724c141ed3be9f6ee8a4aac7727cdaa28769cb61211f7de";
  static bool encode(uwvm_custom::Ros1Scout_msgsScoutRsStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_scout_msgs_ScoutRsStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_scout_msgs_ScoutRsStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_scout_msgs_ScoutRsStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Scout_msgsScoutRsStatus& out) {return !roswasmc_bounded_1_scout_msgs_ScoutRsStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for scout_msgs/ScoutStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Scout_msgsScoutStatus> {
  static constexpr size_t max_wire_bytes=381;
  static constexpr char const* codec_identity="UWVMROS1_ff62854d4de0aae3dfbbc312822c6f912294f0feb04a7fff943df602d40bcbe2";
  static bool encode(uwvm_custom::Ros1Scout_msgsScoutStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_scout_msgs_ScoutStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_scout_msgs_ScoutStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_scout_msgs_ScoutStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Scout_msgsScoutStatus& out) {return !roswasmc_bounded_1_scout_msgs_ScoutStatus_decode(bytes.data(),bytes.size(),&out);}
};
}
