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
/** Complete bounded guest value for geometry_msgs/Transform. */
using Ros1Geometry_msgsTransform=roswasmc_bounded_1_geometry_msgs_Transform;
/** Complete bounded guest value for time. */
using Ros1Time=roswasmc_bounded_1_time;
/** Complete bounded guest value for std_msgs/Header. */
using Ros1Std_msgsHeader=roswasmc_bounded_1_std_msgs_Header;
/** Complete bounded guest value for geometry_msgs/TransformStamped. */
using Ros1Geometry_msgsTransformStamped=roswasmc_bounded_1_geometry_msgs_TransformStamped;
/** Complete bounded guest value for geometry_msgs/Twist. */
using Ros1Geometry_msgsTwist=roswasmc_bounded_1_geometry_msgs_Twist;
/** Complete bounded guest value for geometry_msgs/TwistStamped. */
using Ros1Geometry_msgsTwistStamped=roswasmc_bounded_1_geometry_msgs_TwistStamped;
/** Complete bounded guest value for geometry_msgs/TwistWithCovariance. */
using Ros1Geometry_msgsTwistWithCovariance=roswasmc_bounded_1_geometry_msgs_TwistWithCovariance;
/** Complete bounded guest value for nav_msgs/Odometry. */
using Ros1Nav_msgsOdometry=roswasmc_bounded_1_nav_msgs_Odometry;
/** Complete bounded guest value for sensor_msgs/BatteryState. */
using Ros1Sensor_msgsBatteryState=roswasmc_bounded_1_sensor_msgs_BatteryState;
/** Complete bounded guest value for sensor_msgs/Imu. */
using Ros1Sensor_msgsImu=roswasmc_bounded_1_sensor_msgs_Imu;
/** Complete bounded guest value for sensor_msgs/JointState. */
using Ros1Sensor_msgsJointState=roswasmc_bounded_1_sensor_msgs_JointState;
/** Complete bounded guest value for sensor_msgs/LaserScan. */
using Ros1Sensor_msgsLaserScan=roswasmc_bounded_1_sensor_msgs_LaserScan;
/** Complete bounded guest value for sensor_msgs/MagneticField. */
using Ros1Sensor_msgsMagneticField=roswasmc_bounded_1_sensor_msgs_MagneticField;
/** Complete bounded guest value for std_srvs/SetBoolRequest. */
using Ros1Std_srvsSetBoolRequest=roswasmc_bounded_1_std_srvs_SetBoolRequest;
/** Complete bounded guest value for std_srvs/SetBoolResponse. */
using Ros1Std_srvsSetBoolResponse=roswasmc_bounded_1_std_srvs_SetBoolResponse;
/** Complete bounded guest value for std_srvs/TriggerRequest. */
using Ros1Std_srvsTriggerRequest=roswasmc_bounded_1_std_srvs_TriggerRequest;
/** Complete bounded guest value for std_srvs/TriggerResponse. */
using Ros1Std_srvsTriggerResponse=roswasmc_bounded_1_std_srvs_TriggerResponse;
/** Complete bounded guest value for turtlebot3_example/Turtlebot3Goal. */
using Ros1Turtlebot3_exampleTurtlebot3Goal=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal;
/** Complete bounded guest value for turtlebot3_example/Turtlebot3Result. */
using Ros1Turtlebot3_exampleTurtlebot3Result=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result;
/** Complete bounded guest value for turtlebot3_example/Turtlebot3Feedback. */
using Ros1Turtlebot3_exampleTurtlebot3Feedback=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback;
/** Complete bounded guest value for turtlebot3_msgs/SensorState. */
using Ros1Turtlebot3_msgsSensorState=roswasmc_bounded_1_turtlebot3_msgs_SensorState;
/** Complete bounded guest value for turtlebot3_msgs/Sound. */
using Ros1Turtlebot3_msgsSound=roswasmc_bounded_1_turtlebot3_msgs_Sound;
/** Complete bounded guest value for turtlebot3_msgs/VersionInfo. */
using Ros1Turtlebot3_msgsVersionInfo=roswasmc_bounded_1_turtlebot3_msgs_VersionInfo;
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
/** Checked codec for geometry_msgs/Transform; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsTransform> {
  static constexpr size_t max_wire_bytes=56;
  static constexpr char const* codec_identity="UWVMROS1_61aabab03e8f766ea05949c9e5f4989186d72a26f793b8d11752e53f1f300085";
  static bool encode(uwvm_custom::Ros1Geometry_msgsTransform const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Transform_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Transform_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Transform_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsTransform& out) {return !roswasmc_bounded_1_geometry_msgs_Transform_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for geometry_msgs/TransformStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsTransformStamped> {
  static constexpr size_t max_wire_bytes=460;
  static constexpr char const* codec_identity="UWVMROS1_9237b5dc5d3beacf4310244ce746a4210b9864855b611d1bf54cf23c28d7b79a";
  static bool encode(uwvm_custom::Ros1Geometry_msgsTransformStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_TransformStamped_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_TransformStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_TransformStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsTransformStamped& out) {return !roswasmc_bounded_1_geometry_msgs_TransformStamped_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for geometry_msgs/TwistStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsTwistStamped> {
  static constexpr size_t max_wire_bytes=192;
  static constexpr char const* codec_identity="UWVMROS1_27a31bcca6a50fbc6c6259b53d229872336a75628d6afd9de174eee2481946c7";
  static bool encode(uwvm_custom::Ros1Geometry_msgsTwistStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_TwistStamped_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_TwistStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_TwistStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsTwistStamped& out) {return !roswasmc_bounded_1_geometry_msgs_TwistStamped_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for nav_msgs/Odometry; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Nav_msgsOdometry> {
  static constexpr size_t max_wire_bytes=1084;
  static constexpr char const* codec_identity="UWVMROS1_e0ff5eb7aef868891846ac7618249b12c18fcf0969653f24fdeaa7644e636ca0";
  static bool encode(uwvm_custom::Ros1Nav_msgsOdometry const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_nav_msgs_Odometry_valid(&value))return false;size_t n=roswasmc_bounded_1_nav_msgs_Odometry_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_nav_msgs_Odometry_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Nav_msgsOdometry& out) {return !roswasmc_bounded_1_nav_msgs_Odometry_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/BatteryState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Sensor_msgsBatteryState> {
  static constexpr size_t max_wire_bytes=832;
  static constexpr char const* codec_identity="UWVMROS1_4b176902362348cd3b7eec6ae21e2035d33df6990b4682ddf5123ef6e0339441";
  static bool encode(uwvm_custom::Ros1Sensor_msgsBatteryState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_sensor_msgs_BatteryState_valid(&value))return false;size_t n=roswasmc_bounded_1_sensor_msgs_BatteryState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_sensor_msgs_BatteryState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Sensor_msgsBatteryState& out) {return !roswasmc_bounded_1_sensor_msgs_BatteryState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/Imu; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Sensor_msgsImu> {
  static constexpr size_t max_wire_bytes=440;
  static constexpr char const* codec_identity="UWVMROS1_de9c3f5bb37e1dc20be73cb44f614a9ff8645c4cfbc305dc18425d8d9f130bc3";
  static bool encode(uwvm_custom::Ros1Sensor_msgsImu const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_sensor_msgs_Imu_valid(&value))return false;size_t n=roswasmc_bounded_1_sensor_msgs_Imu_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_sensor_msgs_Imu_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Sensor_msgsImu& out) {return !roswasmc_bounded_1_sensor_msgs_Imu_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/JointState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Sensor_msgsJointState> {
  static constexpr size_t max_wire_bytes=472;
  static constexpr char const* codec_identity="UWVMROS1_f27093d33cca1405ec35a9e525a95aa4002421a579cb78c6da3be3719f3cb66e";
  static bool encode(uwvm_custom::Ros1Sensor_msgsJointState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_sensor_msgs_JointState_valid(&value))return false;size_t n=roswasmc_bounded_1_sensor_msgs_JointState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_sensor_msgs_JointState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Sensor_msgsJointState& out) {return !roswasmc_bounded_1_sensor_msgs_JointState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/LaserScan; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Sensor_msgsLaserScan> {
  static constexpr size_t max_wire_bytes=8372;
  static constexpr char const* codec_identity="UWVMROS1_f7aee00335bf7d165004957149192f58abe5a27b5c4884bdf8d1ae9be06b366b";
  static bool encode(uwvm_custom::Ros1Sensor_msgsLaserScan const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_sensor_msgs_LaserScan_valid(&value))return false;size_t n=roswasmc_bounded_1_sensor_msgs_LaserScan_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_sensor_msgs_LaserScan_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Sensor_msgsLaserScan& out) {return !roswasmc_bounded_1_sensor_msgs_LaserScan_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/MagneticField; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Sensor_msgsMagneticField> {
  static constexpr size_t max_wire_bytes=240;
  static constexpr char const* codec_identity="UWVMROS1_1817b7ce8903797288ccf293373f81890c75af59d03d948339943a531ab754fe";
  static bool encode(uwvm_custom::Ros1Sensor_msgsMagneticField const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_sensor_msgs_MagneticField_valid(&value))return false;size_t n=roswasmc_bounded_1_sensor_msgs_MagneticField_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_sensor_msgs_MagneticField_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Sensor_msgsMagneticField& out) {return !roswasmc_bounded_1_sensor_msgs_MagneticField_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_srvs/SetBoolRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Std_srvsSetBoolRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1621dbb16e2455a199cc871a0e784642a73bc57e76a4d524778163fab97f6aac";
  static bool encode(uwvm_custom::Ros1Std_srvsSetBoolRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_std_srvs_SetBoolRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_std_srvs_SetBoolRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_std_srvs_SetBoolRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Std_srvsSetBoolRequest& out) {return !roswasmc_bounded_1_std_srvs_SetBoolRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_srvs/SetBoolResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Std_srvsSetBoolResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_1e12b91618f540070e34e170e12db7659f8686cdf5fb8e1bf098b8d483522cd6";
  static bool encode(uwvm_custom::Ros1Std_srvsSetBoolResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_std_srvs_SetBoolResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_std_srvs_SetBoolResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_std_srvs_SetBoolResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Std_srvsSetBoolResponse& out) {return !roswasmc_bounded_1_std_srvs_SetBoolResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_srvs/TriggerRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Std_srvsTriggerRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_d4e98dca9dc9a031d6b0f51937613f4f29c36156f0f05475339149794d47db1e";
  static bool encode(uwvm_custom::Ros1Std_srvsTriggerRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_std_srvs_TriggerRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_std_srvs_TriggerRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_std_srvs_TriggerRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Std_srvsTriggerRequest& out) {return !roswasmc_bounded_1_std_srvs_TriggerRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_srvs/TriggerResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Std_srvsTriggerResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_e625281ddea64aff38d8647e2673b418b77474cae0501500b280cabb3d247d1d";
  static bool encode(uwvm_custom::Ros1Std_srvsTriggerResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_std_srvs_TriggerResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_std_srvs_TriggerResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_std_srvs_TriggerResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Std_srvsTriggerResponse& out) {return !roswasmc_bounded_1_std_srvs_TriggerResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_example/Turtlebot3Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Goal> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_f6eb642301e239005c1790cfd5b9ff00388d98d27a17cbb0edb103b6ac7bfad5";
  static bool encode(uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_valid(&value))return false;size_t n=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Goal& out) {return !roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_example/Turtlebot3Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Result> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_c749a56ed9a0c74d1b167f69cc9b4435f9549d49144ef2951a4c3db501bad528";
  static bool encode(uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_valid(&value))return false;size_t n=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Result& out) {return !roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_example/Turtlebot3Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Feedback> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_077b5593a1e4c23c89c4d6a87dc1d67d8fa1bf04fa3aa440f54c8eda60b688f8";
  static bool encode(uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_valid(&value))return false;size_t n=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Turtlebot3_exampleTurtlebot3Feedback& out) {return !roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/SensorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Turtlebot3_msgsSensorState> {
  static constexpr size_t max_wire_bytes=172;
  static constexpr char const* codec_identity="UWVMROS1_0ef7dd1a4b965ae027b081a6b34f111731a3adfbabcc6fd547d4da7378fcb81f";
  static bool encode(uwvm_custom::Ros1Turtlebot3_msgsSensorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_turtlebot3_msgs_SensorState_valid(&value))return false;size_t n=roswasmc_bounded_1_turtlebot3_msgs_SensorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_turtlebot3_msgs_SensorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Turtlebot3_msgsSensorState& out) {return !roswasmc_bounded_1_turtlebot3_msgs_SensorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/Sound; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Turtlebot3_msgsSound> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_0e5af3d5a2acbfff5bf5ba94c1520490fe281e70aff02cfb6ad31acd89e7536a";
  static bool encode(uwvm_custom::Ros1Turtlebot3_msgsSound const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_turtlebot3_msgs_Sound_valid(&value))return false;size_t n=roswasmc_bounded_1_turtlebot3_msgs_Sound_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_turtlebot3_msgs_Sound_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Turtlebot3_msgsSound& out) {return !roswasmc_bounded_1_turtlebot3_msgs_Sound_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for turtlebot3_msgs/VersionInfo; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Turtlebot3_msgsVersionInfo> {
  static constexpr size_t max_wire_bytes=780;
  static constexpr char const* codec_identity="UWVMROS1_8f41d37cf43fda2e3649d85fcca83ad735dc5a8122f7829a98b18659bcad8b3a";
  static bool encode(uwvm_custom::Ros1Turtlebot3_msgsVersionInfo const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_valid(&value))return false;size_t n=roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Turtlebot3_msgsVersionInfo& out) {return !roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_decode(bytes.data(),bytes.size(),&out);}
};
}
