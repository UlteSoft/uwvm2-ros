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
/** Complete bounded guest value for kuka_driver_interfaces/msg/FRIState. */
using Ros2Kuka_driver_interfacesMsgFRIState=roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIState;
/** Complete bounded guest value for kuka_driver_interfaces/msg/FRIStateArray. */
using Ros2Kuka_driver_interfacesMsgFRIStateArray=roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIStateArray;
/** Complete bounded guest value for kuka_driver_interfaces/msg/FriConfiguration. */
using Ros2Kuka_driver_interfacesMsgFriConfiguration=roswasmc_bounded_2_kuka_driver_interfaces_msg_FriConfiguration;
/** Complete bounded guest value for kuka_driver_interfaces/msg/HardwareEvent. */
using Ros2Kuka_driver_interfacesMsgHardwareEvent=roswasmc_bounded_2_kuka_driver_interfaces_msg_HardwareEvent;
/** Complete bounded guest value for kuka_driver_interfaces/msg/KssStatus. */
using Ros2Kuka_driver_interfacesMsgKssStatus=roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus;
/** Complete bounded guest value for kuka_driver_interfaces/msg/KssStatusArray. */
using Ros2Kuka_driver_interfacesMsgKssStatusArray=roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatusArray;
/** Complete bounded guest value for lifecycle_msgs/msg/State. */
using Ros2Lifecycle_msgsMsgState=roswasmc_bounded_2_lifecycle_msgs_msg_State;
/** Complete bounded guest value for lifecycle_msgs/msg/Transition. */
using Ros2Lifecycle_msgsMsgTransition=roswasmc_bounded_2_lifecycle_msgs_msg_Transition;
/** Complete bounded guest value for lifecycle_msgs/srv/ChangeState_Request. */
using Ros2Lifecycle_msgsSrvChangeState_Request=roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request;
/** Complete bounded guest value for lifecycle_msgs/srv/ChangeState_Response. */
using Ros2Lifecycle_msgsSrvChangeState_Response=roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response;
/** Complete bounded guest value for lifecycle_msgs/srv/GetState_Request. */
using Ros2Lifecycle_msgsSrvGetState_Request=roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request;
/** Complete bounded guest value for lifecycle_msgs/srv/GetState_Response. */
using Ros2Lifecycle_msgsSrvGetState_Response=roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response;
/** Complete bounded guest value for sensor_msgs/msg/JointState. */
using Ros2Sensor_msgsMsgJointState=roswasmc_bounded_2_sensor_msgs_msg_JointState;
/** Complete bounded guest value for std_srvs/srv/Trigger_Request. */
using Ros2Std_srvsSrvTrigger_Request=roswasmc_bounded_2_std_srvs_srv_Trigger_Request;
/** Complete bounded guest value for std_srvs/srv/Trigger_Response. */
using Ros2Std_srvsSrvTrigger_Response=roswasmc_bounded_2_std_srvs_srv_Trigger_Response;
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
  static constexpr size_t max_wire_bytes=76;
  static constexpr char const* codec_identity="UWVMROS1_afba9e74be3c5e3321750c1c4c30ba2925269f75b9da43a6d5c1e93d293d7396";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/msg/JointTrajectoryPoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint> {
  static constexpr size_t max_wire_bytes=408;
  static constexpr char const* codec_identity="UWVMROS1_4d55f494f060a9a3ff23bedd54da49f9e70e6b77acbdf911878afbf672cbadc5";
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/msg/JointTrajectory; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory> {
  static constexpr size_t max_wire_bytes=4164;
  static constexpr char const* codec_identity="UWVMROS1_d74956440763bd09c76ec623efeb322aa89be8ef6410b9125d4e2cb1aeeb69c0";
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
  static constexpr size_t max_wire_bytes=324;
  static constexpr char const* codec_identity="UWVMROS1_6590b7ca1cdaa89da04b7a661266e2d4c6e3e2668dc90505a7c24d7be8a43242";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint default_value() noexcept {uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint value={0};(void)roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/msg/MultiDOFJointTrajectory; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory> {
  static constexpr size_t max_wire_bytes=1516;
  static constexpr char const* codec_identity="UWVMROS1_6ea66f146cdce31369361e521be484d2c39bca818cc4b2069e1209d250ec2401";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory default_value() noexcept {uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory value={0};(void)roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/msg/JointTolerance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsMsgJointTolerance> {
  static constexpr size_t max_wire_bytes=92;
  static constexpr char const* codec_identity="UWVMROS1_ad7701ac1cb0dd68a947577272964b0d6051d1846019de97e9c9d0f7be7b2fa2";
  static bool encode(uwvm_custom::Ros2Control_msgsMsgJointTolerance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_msg_JointTolerance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsMsgJointTolerance& out) {return !roswasmc_bounded_2_control_msgs_msg_JointTolerance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/msg/JointComponentTolerance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsMsgJointComponentTolerance> {
  static constexpr size_t max_wire_bytes=542;
  static constexpr char const* codec_identity="UWVMROS1_658f3d3f180b301e5c612188390187ec4034064976f133018048bb1fdec55342";
  static bool encode(uwvm_custom::Ros2Control_msgsMsgJointComponentTolerance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsMsgJointComponentTolerance& out) {return !roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/action/FollowJointTrajectory_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal> {
  static constexpr size_t max_wire_bytes=20920;
  static constexpr char const* codec_identity="UWVMROS1_8945d5b29832f1190b0a4f8c4af4e82686c4b913413affe9c0945570b4fb1f16";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal default_value() noexcept {uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal value={0};(void)roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal& out) {return !roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/action/FollowJointTrajectory_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result> {
  static constexpr size_t max_wire_bytes=4104;
  static constexpr char const* codec_identity="UWVMROS1_c698e3a3a772735ff6027b04af28e9bc88d6b7138863ce5737aa090f543dac70";
  static bool encode(uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result& out) {return !roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/action/FollowJointTrajectory_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback> {
  static constexpr size_t max_wire_bytes=4128;
  static constexpr char const* codec_identity="UWVMROS1_68f3809217bd914cd971e1365ca825111c1394b2587c6989736adcaa8a40a7cb";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback default_value() noexcept {uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback value={0};(void)roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback& out) {return !roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for kuka_driver_interfaces/msg/FRIState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Kuka_driver_interfacesMsgFRIState> {
  static constexpr size_t max_wire_bytes=40;
  static constexpr char const* codec_identity="UWVMROS1_27be8431e4a8cda3bb4d6d5a8a69fd86a0d07a1d920799c232d63a4941ee6716";
  static bool encode(uwvm_custom::Ros2Kuka_driver_interfacesMsgFRIState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIState_valid(&value))return false;size_t n=roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Kuka_driver_interfacesMsgFRIState& out) {return !roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for kuka_driver_interfaces/msg/FRIStateArray; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Kuka_driver_interfacesMsgFRIStateArray> {
  static constexpr size_t max_wire_bytes=1736;
  static constexpr char const* codec_identity="UWVMROS1_4312aa4103fd0c92fe7cd9d1201c40e327b05812cdc60130d11b8c2e0eb47449";
  static bool encode(uwvm_custom::Ros2Kuka_driver_interfacesMsgFRIStateArray const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIStateArray_valid(&value))return false;size_t n=roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIStateArray_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIStateArray_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Kuka_driver_interfacesMsgFRIStateArray& out) {return !roswasmc_bounded_2_kuka_driver_interfaces_msg_FRIStateArray_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for kuka_driver_interfaces/msg/FriConfiguration; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Kuka_driver_interfacesMsgFriConfiguration> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_a60335766ad606adc5699c833cc6bc43adc3067691bcdbf587bbfb56e8a7127e";
  static bool encode(uwvm_custom::Ros2Kuka_driver_interfacesMsgFriConfiguration const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_kuka_driver_interfaces_msg_FriConfiguration_valid(&value))return false;size_t n=roswasmc_bounded_2_kuka_driver_interfaces_msg_FriConfiguration_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_kuka_driver_interfaces_msg_FriConfiguration_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Kuka_driver_interfacesMsgFriConfiguration& out) {return !roswasmc_bounded_2_kuka_driver_interfaces_msg_FriConfiguration_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for kuka_driver_interfaces/msg/HardwareEvent; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Kuka_driver_interfacesMsgHardwareEvent> {
  static constexpr size_t max_wire_bytes=517;
  static constexpr char const* codec_identity="UWVMROS1_cb542e9311957712257c6d8ae273b3d2d2339760978e11e3798982c84fbc5e68";
  static bool encode(uwvm_custom::Ros2Kuka_driver_interfacesMsgHardwareEvent const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_kuka_driver_interfaces_msg_HardwareEvent_valid(&value))return false;size_t n=roswasmc_bounded_2_kuka_driver_interfaces_msg_HardwareEvent_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_kuka_driver_interfaces_msg_HardwareEvent_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Kuka_driver_interfacesMsgHardwareEvent& out) {return !roswasmc_bounded_2_kuka_driver_interfaces_msg_HardwareEvent_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for kuka_driver_interfaces/msg/KssStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Kuka_driver_interfacesMsgKssStatus> {
  static constexpr size_t max_wire_bytes=9;
  static constexpr char const* codec_identity="UWVMROS1_b28bc2f19a0f410f0cd1d5b1d59daf1d09acb3e8312b4267f622a4d119f16b82";
  static bool encode(uwvm_custom::Ros2Kuka_driver_interfacesMsgKssStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Kuka_driver_interfacesMsgKssStatus& out) {return !roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for kuka_driver_interfaces/msg/KssStatusArray; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Kuka_driver_interfacesMsgKssStatusArray> {
  static constexpr size_t max_wire_bytes=1240;
  static constexpr char const* codec_identity="UWVMROS1_3c8e93b369eefea89c0aa56ab657721edd041ac66e18cfbf4fc4201df60dbebd";
  static bool encode(uwvm_custom::Ros2Kuka_driver_interfacesMsgKssStatusArray const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatusArray_valid(&value))return false;size_t n=roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatusArray_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatusArray_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Kuka_driver_interfacesMsgKssStatusArray& out) {return !roswasmc_bounded_2_kuka_driver_interfaces_msg_KssStatusArray_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for lifecycle_msgs/msg/State; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Lifecycle_msgsMsgState> {
  static constexpr size_t max_wire_bytes=517;
  static constexpr char const* codec_identity="UWVMROS1_ebd49e5f87ddaa204cc4072ceef9840676c6f65f67448a027e26570dae0ded94";
  static bool encode(uwvm_custom::Ros2Lifecycle_msgsMsgState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_lifecycle_msgs_msg_State_valid(&value))return false;size_t n=roswasmc_bounded_2_lifecycle_msgs_msg_State_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_lifecycle_msgs_msg_State_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Lifecycle_msgsMsgState& out) {return !roswasmc_bounded_2_lifecycle_msgs_msg_State_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for lifecycle_msgs/msg/Transition; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Lifecycle_msgsMsgTransition> {
  static constexpr size_t max_wire_bytes=517;
  static constexpr char const* codec_identity="UWVMROS1_1f9bb864622854b446a37cf3c6e29e9cf41f8efa37375c5a56205b4c02ed10cb";
  static bool encode(uwvm_custom::Ros2Lifecycle_msgsMsgTransition const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_lifecycle_msgs_msg_Transition_valid(&value))return false;size_t n=roswasmc_bounded_2_lifecycle_msgs_msg_Transition_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_lifecycle_msgs_msg_Transition_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Lifecycle_msgsMsgTransition& out) {return !roswasmc_bounded_2_lifecycle_msgs_msg_Transition_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for lifecycle_msgs/srv/ChangeState_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Lifecycle_msgsSrvChangeState_Request> {
  static constexpr size_t max_wire_bytes=517;
  static constexpr char const* codec_identity="UWVMROS1_b04270f03616b8e581036d327d18bde62d38627ae8acf88fb77d0ed7b3b3d3d1";
  static bool encode(uwvm_custom::Ros2Lifecycle_msgsSrvChangeState_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Lifecycle_msgsSrvChangeState_Request& out) {return !roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for lifecycle_msgs/srv/ChangeState_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Lifecycle_msgsSrvChangeState_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_d2dc541d6995976a2a1a2c03d499aa9dccaf487736b11704d778c86fddaf5032";
  static bool encode(uwvm_custom::Ros2Lifecycle_msgsSrvChangeState_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Lifecycle_msgsSrvChangeState_Response& out) {return !roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for lifecycle_msgs/srv/GetState_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Lifecycle_msgsSrvGetState_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_86a3d813de32971a271e062acfaf201eb718aff8d68a4e343d6fceaae2b5098d";
  static bool encode(uwvm_custom::Ros2Lifecycle_msgsSrvGetState_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Lifecycle_msgsSrvGetState_Request& out) {return !roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for lifecycle_msgs/srv/GetState_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Lifecycle_msgsSrvGetState_Response> {
  static constexpr size_t max_wire_bytes=517;
  static constexpr char const* codec_identity="UWVMROS1_697418ec4095e98c855b1f9d238a3bd74a59ccbbc3eb31981c4df3c3b829ea39";
  static bool encode(uwvm_custom::Ros2Lifecycle_msgsSrvGetState_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Lifecycle_msgsSrvGetState_Response& out) {return !roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/msg/JointState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgJointState> {
  static constexpr size_t max_wire_bytes=1196;
  static constexpr char const* codec_identity="UWVMROS1_88d44f7bdb40f8c45539a041a25903609561fa74db90b9972e1369ebbff6c884";
  static bool encode(uwvm_custom::Ros2Sensor_msgsMsgJointState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&value))return false;size_t n=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_sensor_msgs_msg_JointState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Sensor_msgsMsgJointState& out) {return !roswasmc_bounded_2_sensor_msgs_msg_JointState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_srvs/srv/Trigger_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_srvsSrvTrigger_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_f1d8ad4ab7cf476cada6301f71ae24e046c244ba62bb6735183efe8e8c530a77";
  static bool encode(uwvm_custom::Ros2Std_srvsSrvTrigger_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_srvs_srv_Trigger_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_std_srvs_srv_Trigger_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_srvs_srv_Trigger_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_srvsSrvTrigger_Request& out) {return !roswasmc_bounded_2_std_srvs_srv_Trigger_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_srvs/srv/Trigger_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_srvsSrvTrigger_Response> {
  static constexpr size_t max_wire_bytes=517;
  static constexpr char const* codec_identity="UWVMROS1_95fe65bd59112bb00093f66c6853844b01ccea8e6ded47e73038718aa886f39d";
  static bool encode(uwvm_custom::Ros2Std_srvsSrvTrigger_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_srvs_srv_Trigger_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_std_srvs_srv_Trigger_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_srvs_srv_Trigger_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_srvsSrvTrigger_Response& out) {return !roswasmc_bounded_2_std_srvs_srv_Trigger_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
