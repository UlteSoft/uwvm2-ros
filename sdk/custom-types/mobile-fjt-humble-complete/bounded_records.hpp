/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for builtin_interfaces/msg/Duration. */
using Ros2Builtin_interfacesMsgDuration=roswasmc_bounded_2_builtin_interfaces_msg_Duration;
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for trajectory_msgs/msg/JointTrajectoryPoint. */
using Ros2Trajectory_msgsMsgJointTrajectoryPoint=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint;
/** Complete bounded guest value for trajectory_msgs/msg/JointTrajectory. */
using Ros2Trajectory_msgsMsgJointTrajectory=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory;
/** Complete bounded guest value for geometry_msgs/msg/Vector3. */
using Ros2Geometry_msgsMsgVector3=roswasmc_bounded_2_geometry_msgs_msg_Vector3;
/** Complete bounded guest value for geometry_msgs/msg/Quaternion. */
using Ros2Geometry_msgsMsgQuaternion=roswasmc_bounded_2_geometry_msgs_msg_Quaternion;
/** Complete bounded guest value for geometry_msgs/msg/Transform. */
using Ros2Geometry_msgsMsgTransform=roswasmc_bounded_2_geometry_msgs_msg_Transform;
/** Complete bounded guest value for geometry_msgs/msg/Twist. */
using Ros2Geometry_msgsMsgTwist=roswasmc_bounded_2_geometry_msgs_msg_Twist;
/** Complete bounded guest value for trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
using Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint;
/** Complete bounded guest value for trajectory_msgs/msg/MultiDOFJointTrajectory. */
using Ros2Trajectory_msgsMsgMultiDOFJointTrajectory=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory;
/** Complete bounded guest value for control_msgs/msg/JointTolerance. */
using Ros2Control_msgsMsgJointTolerance=roswasmc_bounded_2_control_msgs_msg_JointTolerance;
/** Complete bounded guest value for control_msgs/msg/JointComponentTolerance. */
using Ros2Control_msgsMsgJointComponentTolerance=roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance;
/** Complete bounded guest value for control_msgs/action/FollowJointTrajectory_Goal. */
using Ros2Control_msgsActionFollowJointTrajectory_Goal=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal;
/** Complete bounded guest value for control_msgs/action/FollowJointTrajectory_Result. */
using Ros2Control_msgsActionFollowJointTrajectory_Result=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result;
/** Complete bounded guest value for control_msgs/action/FollowJointTrajectory_Feedback. */
using Ros2Control_msgsActionFollowJointTrajectory_Feedback=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback;
/** Complete bounded guest value for sensor_msgs/msg/JointState. */
using Ros2Sensor_msgsMsgJointState=roswasmc_bounded_2_sensor_msgs_msg_JointState;
}
namespace roswasmcpp {
/** Checked codec for builtin_interfaces/msg/Duration; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Builtin_interfacesMsgDuration> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4";
  static bool encode(uwvm_custom::Ros2Builtin_interfacesMsgDuration const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value))return false;size_t n=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_builtin_interfaces_msg_Duration_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Builtin_interfacesMsgDuration& out) {return !roswasmc_bounded_2_builtin_interfaces_msg_Duration_decode(bytes.data(),bytes.size(),&out);}
};
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
  static constexpr size_t max_wire_bytes=140;
  static constexpr char const* codec_identity="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/msg/JointTrajectoryPoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint> {
  static constexpr size_t max_wire_bytes=280;
  static constexpr char const* codec_identity="UWVMROS1_a512b971211baa8604440e02a046b586e7970293242607002ceb89196c9d99b1";
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/msg/JointTrajectory; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory> {
  static constexpr size_t max_wire_bytes=1484;
  static constexpr char const* codec_identity="UWVMROS1_e32bd7016b5fefcab6adeb42072e2d56c45d4b9f7cb372bdb5bfac97abf8cb77";
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for geometry_msgs/msg/Transform; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgTransform> {
  static constexpr size_t max_wire_bytes=56;
  static constexpr char const* codec_identity="UWVMROS1_7ddcb1c3fca43f4a0299d43fb6fac1b77a8f26001af809dc13307f4f9c61d65b";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Geometry_msgsMsgTransform default_value() noexcept {uwvm_custom::Ros2Geometry_msgsMsgTransform value={0};(void)roswasmc_bounded_2_geometry_msgs_msg_Transform_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgTransform const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Transform_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Transform_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Transform_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgTransform& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Transform_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for trajectory_msgs/msg/MultiDOFJointTrajectoryPoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint> {
  static constexpr size_t max_wire_bytes=844;
  static constexpr char const* codec_identity="UWVMROS1_4b4b6ba78f11918fcdc138544f7ef1a140d1fc51b385349ab1ad5716472c3daf";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint default_value() noexcept {uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint value={0};(void)roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/msg/MultiDOFJointTrajectory; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory> {
  static constexpr size_t max_wire_bytes=2048;
  static constexpr char const* codec_identity="UWVMROS1_b822a2d5db9b83e916dcc9db4932952a314773ef53c9fbaf825902e0eb7c8cb5";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory default_value() noexcept {uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory value={0};(void)roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/msg/JointTolerance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsMsgJointTolerance> {
  static constexpr size_t max_wire_bytes=156;
  static constexpr char const* codec_identity="UWVMROS1_3864af88029b3b1f51907b3da63d2d821104c29b9469a146a5a34f73f3598b8a";
  static bool encode(uwvm_custom::Ros2Control_msgsMsgJointTolerance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_msg_JointTolerance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsMsgJointTolerance& out) {return !roswasmc_bounded_2_control_msgs_msg_JointTolerance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/msg/JointComponentTolerance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsMsgJointComponentTolerance> {
  static constexpr size_t max_wire_bytes=158;
  static constexpr char const* codec_identity="UWVMROS1_8a10f659b38feae6dfcf89818d35a164743b02c0e5cfe2d86aae7eda56e914fb";
  static bool encode(uwvm_custom::Ros2Control_msgsMsgJointComponentTolerance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsMsgJointComponentTolerance& out) {return !roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/action/FollowJointTrajectory_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal> {
  static constexpr size_t max_wire_bytes=8580;
  static constexpr char const* codec_identity="UWVMROS1_fa8334eb129cc91c50febaccfebb371f6fbdd82188d6301823344d6664300018";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal default_value() noexcept {uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal value={0};(void)roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal& out) {return !roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/action/FollowJointTrajectory_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result> {
  static constexpr size_t max_wire_bytes=264;
  static constexpr char const* codec_identity="UWVMROS1_df1e08076a5c725b865cc8807462d9ccf9577df11cbb71fcc0d280d77d0e5649";
  static bool encode(uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result& out) {return !roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/action/FollowJointTrajectory_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback> {
  static constexpr size_t max_wire_bytes=4708;
  static constexpr char const* codec_identity="UWVMROS1_ae2175027c3e45997d272b9610d15ef52cc665d23c0a371892d10b3d100e3529";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback default_value() noexcept {uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback value={0};(void)roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback& out) {return !roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/msg/JointState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgJointState> {
  static constexpr size_t max_wire_bytes=1404;
  static constexpr char const* codec_identity="UWVMROS1_bba20c36db70a98b594ba603d05b8a979038fb95b701221ba3b8689f55b06b11";
  static bool encode(uwvm_custom::Ros2Sensor_msgsMsgJointState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&value))return false;size_t n=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_sensor_msgs_msg_JointState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Sensor_msgsMsgJointState& out) {return !roswasmc_bounded_2_sensor_msgs_msg_JointState_decode(bytes.data(),bytes.size(),&out);}
};
}
