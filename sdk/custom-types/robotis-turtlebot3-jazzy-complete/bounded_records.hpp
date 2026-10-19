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
/** Complete bounded guest value for geometry_msgs/msg/Transform. */
using Ros2Geometry_msgsMsgTransform=roswasmc_bounded_2_geometry_msgs_msg_Transform;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for geometry_msgs/msg/TransformStamped. */
using Ros2Geometry_msgsMsgTransformStamped=roswasmc_bounded_2_geometry_msgs_msg_TransformStamped;
/** Complete bounded guest value for geometry_msgs/msg/Twist. */
using Ros2Geometry_msgsMsgTwist=roswasmc_bounded_2_geometry_msgs_msg_Twist;
/** Complete bounded guest value for geometry_msgs/msg/TwistStamped. */
using Ros2Geometry_msgsMsgTwistStamped=roswasmc_bounded_2_geometry_msgs_msg_TwistStamped;
/** Complete bounded guest value for geometry_msgs/msg/TwistWithCovariance. */
using Ros2Geometry_msgsMsgTwistWithCovariance=roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance;
/** Complete bounded guest value for nav_msgs/msg/Odometry. */
using Ros2Nav_msgsMsgOdometry=roswasmc_bounded_2_nav_msgs_msg_Odometry;
/** Complete bounded guest value for sensor_msgs/msg/BatteryState. */
using Ros2Sensor_msgsMsgBatteryState=roswasmc_bounded_2_sensor_msgs_msg_BatteryState;
/** Complete bounded guest value for sensor_msgs/msg/Imu. */
using Ros2Sensor_msgsMsgImu=roswasmc_bounded_2_sensor_msgs_msg_Imu;
/** Complete bounded guest value for sensor_msgs/msg/JointState. */
using Ros2Sensor_msgsMsgJointState=roswasmc_bounded_2_sensor_msgs_msg_JointState;
/** Complete bounded guest value for sensor_msgs/msg/LaserScan. */
using Ros2Sensor_msgsMsgLaserScan=roswasmc_bounded_2_sensor_msgs_msg_LaserScan;
/** Complete bounded guest value for sensor_msgs/msg/MagneticField. */
using Ros2Sensor_msgsMsgMagneticField=roswasmc_bounded_2_sensor_msgs_msg_MagneticField;
/** Complete bounded guest value for std_srvs/srv/SetBool_Request. */
using Ros2Std_srvsSrvSetBool_Request=roswasmc_bounded_2_std_srvs_srv_SetBool_Request;
/** Complete bounded guest value for std_srvs/srv/SetBool_Response. */
using Ros2Std_srvsSrvSetBool_Response=roswasmc_bounded_2_std_srvs_srv_SetBool_Response;
/** Complete bounded guest value for std_srvs/srv/Trigger_Request. */
using Ros2Std_srvsSrvTrigger_Request=roswasmc_bounded_2_std_srvs_srv_Trigger_Request;
/** Complete bounded guest value for std_srvs/srv/Trigger_Response. */
using Ros2Std_srvsSrvTrigger_Response=roswasmc_bounded_2_std_srvs_srv_Trigger_Response;
/** Complete bounded guest value for turtlebot3_msgs/action/Patrol_Goal. */
using Ros2Turtlebot3_msgsActionPatrol_Goal=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal;
/** Complete bounded guest value for turtlebot3_msgs/action/Patrol_Result. */
using Ros2Turtlebot3_msgsActionPatrol_Result=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result;
/** Complete bounded guest value for turtlebot3_msgs/action/Patrol_Feedback. */
using Ros2Turtlebot3_msgsActionPatrol_Feedback=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback;
/** Complete bounded guest value for turtlebot3_msgs/msg/SensorState. */
using Ros2Turtlebot3_msgsMsgSensorState=roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState;
/** Complete bounded guest value for turtlebot3_msgs/msg/Sound. */
using Ros2Turtlebot3_msgsMsgSound=roswasmc_bounded_2_turtlebot3_msgs_msg_Sound;
/** Complete bounded guest value for turtlebot3_msgs/msg/VersionInfo. */
using Ros2Turtlebot3_msgsMsgVersionInfo=roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo;
/** Complete bounded guest value for turtlebot3_msgs/srv/Dqn_Request. */
using Ros2Turtlebot3_msgsSrvDqn_Request=roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request;
/** Complete bounded guest value for turtlebot3_msgs/srv/Dqn_Response. */
using Ros2Turtlebot3_msgsSrvDqn_Response=roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response;
/** Complete bounded guest value for turtlebot3_msgs/srv/Goal_Request. */
using Ros2Turtlebot3_msgsSrvGoal_Request=roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request;
/** Complete bounded guest value for turtlebot3_msgs/srv/Goal_Response. */
using Ros2Turtlebot3_msgsSrvGoal_Response=roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response;
/** Complete bounded guest value for turtlebot3_msgs/srv/Sound_Request. */
using Ros2Turtlebot3_msgsSrvSound_Request=roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request;
/** Complete bounded guest value for turtlebot3_msgs/srv/Sound_Response. */
using Ros2Turtlebot3_msgsSrvSound_Response=roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response;
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
/** Checked codec for std_msgs/msg/Header; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_msgsMsgHeader> {
  static constexpr size_t max_wire_bytes=140;
  static constexpr char const* codec_identity="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/TransformStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgTransformStamped> {
  static constexpr size_t max_wire_bytes=456;
  static constexpr char const* codec_identity="UWVMROS1_1cb6033612e0632fe87e6cf44bf5a899220370c372ba25b989a066171925ee7a";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Geometry_msgsMsgTransformStamped default_value() noexcept {uwvm_custom::Ros2Geometry_msgsMsgTransformStamped value={0};(void)roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgTransformStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgTransformStamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for geometry_msgs/msg/TwistStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgTwistStamped> {
  static constexpr size_t max_wire_bytes=188;
  static constexpr char const* codec_identity="UWVMROS1_7448b885388ff3454de1a35b8c5c895ee58b791267b435df1e4cf7c1eee0f573";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgTwistStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgTwistStamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for nav_msgs/msg/Odometry; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Nav_msgsMsgOdometry> {
  static constexpr size_t max_wire_bytes=1080;
  static constexpr char const* codec_identity="UWVMROS1_4eb9aa4edb3d288080deaae0326c26a75923e69d40c9b10219baf6d441ea8111";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Nav_msgsMsgOdometry default_value() noexcept {uwvm_custom::Ros2Nav_msgsMsgOdometry value={0};(void)roswasmc_bounded_2_nav_msgs_msg_Odometry_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Nav_msgsMsgOdometry const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_nav_msgs_msg_Odometry_valid(&value))return false;size_t n=roswasmc_bounded_2_nav_msgs_msg_Odometry_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_nav_msgs_msg_Odometry_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Nav_msgsMsgOdometry& out) {return !roswasmc_bounded_2_nav_msgs_msg_Odometry_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/msg/BatteryState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgBatteryState> {
  static constexpr size_t max_wire_bytes=828;
  static constexpr char const* codec_identity="UWVMROS1_9083c57dde00bba912db4dd736027cb92bbed2b1a79f965570dee86e1ff5a96e";
  static bool encode(uwvm_custom::Ros2Sensor_msgsMsgBatteryState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_sensor_msgs_msg_BatteryState_valid(&value))return false;size_t n=roswasmc_bounded_2_sensor_msgs_msg_BatteryState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_sensor_msgs_msg_BatteryState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Sensor_msgsMsgBatteryState& out) {return !roswasmc_bounded_2_sensor_msgs_msg_BatteryState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/msg/Imu; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgImu> {
  static constexpr size_t max_wire_bytes=436;
  static constexpr char const* codec_identity="UWVMROS1_e38d1c936a2a4a870b2f069af8d1796f69e6ce7b8cae0a792925ed2f6c0cae1b";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Sensor_msgsMsgImu default_value() noexcept {uwvm_custom::Ros2Sensor_msgsMsgImu value={0};(void)roswasmc_bounded_2_sensor_msgs_msg_Imu_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Sensor_msgsMsgImu const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_sensor_msgs_msg_Imu_valid(&value))return false;size_t n=roswasmc_bounded_2_sensor_msgs_msg_Imu_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_sensor_msgs_msg_Imu_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Sensor_msgsMsgImu& out) {return !roswasmc_bounded_2_sensor_msgs_msg_Imu_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/msg/JointState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgJointState> {
  static constexpr size_t max_wire_bytes=468;
  static constexpr char const* codec_identity="UWVMROS1_5c4d1ef0128c748e20f6b30c9c3bb5ef6e3c529a0b05b81764ac4774ce765bcf";
  static bool encode(uwvm_custom::Ros2Sensor_msgsMsgJointState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&value))return false;size_t n=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_sensor_msgs_msg_JointState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Sensor_msgsMsgJointState& out) {return !roswasmc_bounded_2_sensor_msgs_msg_JointState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/msg/LaserScan; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgLaserScan> {
  static constexpr size_t max_wire_bytes=8368;
  static constexpr char const* codec_identity="UWVMROS1_89faa8e14fc7f79a62a576d35c0bb377caf0cbaef6a6f181a5af2da0ead6cce4";
  static bool encode(uwvm_custom::Ros2Sensor_msgsMsgLaserScan const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_sensor_msgs_msg_LaserScan_valid(&value))return false;size_t n=roswasmc_bounded_2_sensor_msgs_msg_LaserScan_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_sensor_msgs_msg_LaserScan_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Sensor_msgsMsgLaserScan& out) {return !roswasmc_bounded_2_sensor_msgs_msg_LaserScan_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/msg/MagneticField; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgMagneticField> {
  static constexpr size_t max_wire_bytes=236;
  static constexpr char const* codec_identity="UWVMROS1_56ae1437e0fbe7a6722ecf4795483d15f4c616e2e891372c30fa78f18be2cf8b";
  static bool encode(uwvm_custom::Ros2Sensor_msgsMsgMagneticField const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_sensor_msgs_msg_MagneticField_valid(&value))return false;size_t n=roswasmc_bounded_2_sensor_msgs_msg_MagneticField_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_sensor_msgs_msg_MagneticField_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Sensor_msgsMsgMagneticField& out) {return !roswasmc_bounded_2_sensor_msgs_msg_MagneticField_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_srvs/srv/SetBool_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_srvsSrvSetBool_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_2523e7915bb78fe01b68f2a856611b32a88f624a6e197a6ff312b613e8c65910";
  static bool encode(uwvm_custom::Ros2Std_srvsSrvSetBool_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_srvs_srv_SetBool_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_std_srvs_srv_SetBool_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_srvs_srv_SetBool_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_srvsSrvSetBool_Request& out) {return !roswasmc_bounded_2_std_srvs_srv_SetBool_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_srvs/srv/SetBool_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_srvsSrvSetBool_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_f2d46a8440538094221a151716d8ec44db02455f59845c50aaf4e7964c27ebe4";
  static bool encode(uwvm_custom::Ros2Std_srvsSrvSetBool_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_srvs_srv_SetBool_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_std_srvs_srv_SetBool_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_srvs_srv_SetBool_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_srvsSrvSetBool_Response& out) {return !roswasmc_bounded_2_std_srvs_srv_SetBool_Response_decode(bytes.data(),bytes.size(),&out);}
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
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_58806c592727ad1011b32edb6bca3ca32d8e5dbe7f2fa20c86220daf64a10392";
  static bool encode(uwvm_custom::Ros2Std_srvsSrvTrigger_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_srvs_srv_Trigger_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_std_srvs_srv_Trigger_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_srvs_srv_Trigger_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_srvsSrvTrigger_Response& out) {return !roswasmc_bounded_2_std_srvs_srv_Trigger_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/action/Patrol_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Goal> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_929abdbc422698464043e7e8d12c2a5edd418dcd52bbde4a542b33e6862ba96f";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Goal& out) {return !roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/action/Patrol_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Result> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_1d1fa277be54e1df3bd314a632ff80d79f22a306fc232219a7c23cce7a7703a3";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Result& out) {return !roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/action/Patrol_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Feedback> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_7c2f3824653a3048e1b93fa95584543a01efae8bb67253d3371a2988407a601e";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsActionPatrol_Feedback& out) {return !roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/msg/SensorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsMsgSensorState> {
  static constexpr size_t max_wire_bytes=168;
  static constexpr char const* codec_identity="UWVMROS1_7974ef7ca70ababf0e3f7a5e4099335a1398933833260798482f155291a62358";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsMsgSensorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsMsgSensorState& out) {return !roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/msg/Sound; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsMsgSound> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_e8bd9a5fdf84ad1df0f264bd13a0e8966918de19ab62e91c7524dd8fc17a4e4b";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsMsgSound const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsMsgSound& out) {return !roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/msg/VersionInfo; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsMsgVersionInfo> {
  static constexpr size_t max_wire_bytes=780;
  static constexpr char const* codec_identity="UWVMROS1_7668c77b01a05eeb8ba3c5f259f3f3a9dfad3d4fdf8aa566d65f6e8f1c0f2d6a";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsMsgVersionInfo const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsMsgVersionInfo& out) {return !roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/srv/Dqn_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsSrvDqn_Request> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_14042e65db9f4feef7a43bfb44255795470bb0ed91a3cf301ebec98c2d4e2d0e";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsSrvDqn_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsSrvDqn_Request& out) {return !roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/srv/Dqn_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsSrvDqn_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_5d256e9c87d7959917f07e7cfcc0049fc18f79f547d83b72bd388a63c1417c6e";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsSrvDqn_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsSrvDqn_Response& out) {return !roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/srv/Goal_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsSrvGoal_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_f76094f3cf00772bb80b59e846f1160ae6255fc0d931bbcba362104081698ded";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsSrvGoal_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsSrvGoal_Request& out) {return !roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/srv/Goal_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsSrvGoal_Response> {
  static constexpr size_t max_wire_bytes=9;
  static constexpr char const* codec_identity="UWVMROS1_8b9022a7fb8c68a0b063b080cd05c230b653169badbe205bcd8051b8020f7990";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsSrvGoal_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsSrvGoal_Response& out) {return !roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/srv/Sound_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsSrvSound_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_0dcba95f5ba646d9a24b0054ba603daf1d1e5e4e9a6ffe7387cd61bff2e4151a";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsSrvSound_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsSrvSound_Request& out) {return !roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/srv/Sound_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Turtlebot3_msgsSrvSound_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_bc5a5fd3e2bf733a278b8a1315c8905b27fff49b4844df238d5574149068998e";
  static bool encode(uwvm_custom::Ros2Turtlebot3_msgsSrvSound_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Turtlebot3_msgsSrvSound_Response& out) {return !roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
