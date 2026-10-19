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
/** Complete bounded guest value for geometry_msgs/msg/TransformStamped. */
using Ros2Geometry_msgsMsgTransformStamped=roswasmc_bounded_2_geometry_msgs_msg_TransformStamped;
/** Complete bounded guest value for industrial_msgs/msg/DebugLevel. */
using Ros2Industrial_msgsMsgDebugLevel=roswasmc_bounded_2_industrial_msgs_msg_DebugLevel;
/** Complete bounded guest value for industrial_msgs/msg/DeviceInfo. */
using Ros2Industrial_msgsMsgDeviceInfo=roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo;
/** Complete bounded guest value for industrial_msgs/msg/RobotMode. */
using Ros2Industrial_msgsMsgRobotMode=roswasmc_bounded_2_industrial_msgs_msg_RobotMode;
/** Complete bounded guest value for industrial_msgs/msg/TriState. */
using Ros2Industrial_msgsMsgTriState=roswasmc_bounded_2_industrial_msgs_msg_TriState;
/** Complete bounded guest value for industrial_msgs/msg/RobotStatus. */
using Ros2Industrial_msgsMsgRobotStatus=roswasmc_bounded_2_industrial_msgs_msg_RobotStatus;
/** Complete bounded guest value for industrial_msgs/msg/ServiceReturnCode. */
using Ros2Industrial_msgsMsgServiceReturnCode=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode;
/** Complete bounded guest value for industrial_msgs/srv/CmdJointTrajectory_Request. */
using Ros2Industrial_msgsSrvCmdJointTrajectory_Request=roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request;
/** Complete bounded guest value for industrial_msgs/srv/CmdJointTrajectory_Response. */
using Ros2Industrial_msgsSrvCmdJointTrajectory_Response=roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response;
/** Complete bounded guest value for industrial_msgs/srv/GetRobotInfo_Request. */
using Ros2Industrial_msgsSrvGetRobotInfo_Request=roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request;
/** Complete bounded guest value for industrial_msgs/srv/GetRobotInfo_Response. */
using Ros2Industrial_msgsSrvGetRobotInfo_Response=roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response;
/** Complete bounded guest value for industrial_msgs/srv/SetDrivePower_Request. */
using Ros2Industrial_msgsSrvSetDrivePower_Request=roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request;
/** Complete bounded guest value for industrial_msgs/srv/SetDrivePower_Response. */
using Ros2Industrial_msgsSrvSetDrivePower_Response=roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response;
/** Complete bounded guest value for industrial_msgs/srv/SetRemoteLoggerLevel_Request. */
using Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request=roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request;
/** Complete bounded guest value for industrial_msgs/srv/SetRemoteLoggerLevel_Response. */
using Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response=roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response;
/** Complete bounded guest value for industrial_msgs/srv/StartMotion_Request. */
using Ros2Industrial_msgsSrvStartMotion_Request=roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request;
/** Complete bounded guest value for industrial_msgs/srv/StartMotion_Response. */
using Ros2Industrial_msgsSrvStartMotion_Response=roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response;
/** Complete bounded guest value for industrial_msgs/srv/StopMotion_Request. */
using Ros2Industrial_msgsSrvStopMotion_Request=roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request;
/** Complete bounded guest value for industrial_msgs/srv/StopMotion_Response. */
using Ros2Industrial_msgsSrvStopMotion_Response=roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response;
/** Complete bounded guest value for motoros2_interfaces/msg/AlarmCauseRemedy. */
using Ros2Motoros2_interfacesMsgAlarmCauseRemedy=roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy;
/** Complete bounded guest value for motoros2_interfaces/msg/AlarmInfo. */
using Ros2Motoros2_interfacesMsgAlarmInfo=roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo;
/** Complete bounded guest value for motoros2_interfaces/msg/ErrorInfo. */
using Ros2Motoros2_interfacesMsgErrorInfo=roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo;
/** Complete bounded guest value for motoros2_interfaces/msg/InformJobCrudResultCodes. */
using Ros2Motoros2_interfacesMsgInformJobCrudResultCodes=roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes;
/** Complete bounded guest value for motoros2_interfaces/msg/InitTrajEnum. */
using Ros2Motoros2_interfacesMsgInitTrajEnum=roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum;
/** Complete bounded guest value for motoros2_interfaces/msg/IoResultCodes. */
using Ros2Motoros2_interfacesMsgIoResultCodes=roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes;
/** Complete bounded guest value for motoros2_interfaces/msg/MotionReadyEnum. */
using Ros2Motoros2_interfacesMsgMotionReadyEnum=roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum;
/** Complete bounded guest value for motoros2_interfaces/msg/QueueResultEnum. */
using Ros2Motoros2_interfacesMsgQueueResultEnum=roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum;
/** Complete bounded guest value for motoros2_interfaces/msg/SelectionResultCodes. */
using Ros2Motoros2_interfacesMsgSelectionResultCodes=roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes;
/** Complete bounded guest value for motoros2_interfaces/srv/GetActiveAlarmInfo_Request. */
using Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request=roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/GetActiveAlarmInfo_Response. */
using Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response=roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/ListInformJobs_Request. */
using Ros2Motoros2_interfacesSrvListInformJobs_Request=roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/ListInformJobs_Response. */
using Ros2Motoros2_interfacesSrvListInformJobs_Response=roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/QueueTrajPoint_Request. */
using Ros2Motoros2_interfacesSrvQueueTrajPoint_Request=roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/QueueTrajPoint_Response. */
using Ros2Motoros2_interfacesSrvQueueTrajPoint_Response=roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/ReadGroupIO_Request. */
using Ros2Motoros2_interfacesSrvReadGroupIO_Request=roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/ReadGroupIO_Response. */
using Ros2Motoros2_interfacesSrvReadGroupIO_Response=roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/ReadMRegister_Request. */
using Ros2Motoros2_interfacesSrvReadMRegister_Request=roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/ReadMRegister_Response. */
using Ros2Motoros2_interfacesSrvReadMRegister_Response=roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/ReadSingleIO_Request. */
using Ros2Motoros2_interfacesSrvReadSingleIO_Request=roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/ReadSingleIO_Response. */
using Ros2Motoros2_interfacesSrvReadSingleIO_Response=roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/ResetError_Request. */
using Ros2Motoros2_interfacesSrvResetError_Request=roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/ResetError_Response. */
using Ros2Motoros2_interfacesSrvResetError_Response=roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/SelectMotionTool_Request. */
using Ros2Motoros2_interfacesSrvSelectMotionTool_Request=roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/SelectMotionTool_Response. */
using Ros2Motoros2_interfacesSrvSelectMotionTool_Response=roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/StartPointQueueMode_Request. */
using Ros2Motoros2_interfacesSrvStartPointQueueMode_Request=roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/StartPointQueueMode_Response. */
using Ros2Motoros2_interfacesSrvStartPointQueueMode_Response=roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/StartTrajMode_Request. */
using Ros2Motoros2_interfacesSrvStartTrajMode_Request=roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/StartTrajMode_Response. */
using Ros2Motoros2_interfacesSrvStartTrajMode_Response=roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/WriteGroupIO_Request. */
using Ros2Motoros2_interfacesSrvWriteGroupIO_Request=roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/WriteGroupIO_Response. */
using Ros2Motoros2_interfacesSrvWriteGroupIO_Response=roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/WriteMRegister_Request. */
using Ros2Motoros2_interfacesSrvWriteMRegister_Request=roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/WriteMRegister_Response. */
using Ros2Motoros2_interfacesSrvWriteMRegister_Response=roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response;
/** Complete bounded guest value for motoros2_interfaces/srv/WriteSingleIO_Request. */
using Ros2Motoros2_interfacesSrvWriteSingleIO_Request=roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request;
/** Complete bounded guest value for motoros2_interfaces/srv/WriteSingleIO_Response. */
using Ros2Motoros2_interfacesSrvWriteSingleIO_Response=roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response;
/** Complete bounded guest value for sensor_msgs/msg/JointState. */
using Ros2Sensor_msgsMsgJointState=roswasmc_bounded_2_sensor_msgs_msg_JointState;
/** Complete bounded guest value for std_srvs/srv/Trigger_Request. */
using Ros2Std_srvsSrvTrigger_Request=roswasmc_bounded_2_std_srvs_srv_Trigger_Request;
/** Complete bounded guest value for std_srvs/srv/Trigger_Response. */
using Ros2Std_srvsSrvTrigger_Response=roswasmc_bounded_2_std_srvs_srv_Trigger_Response;
/** Complete bounded guest value for tf2_msgs/msg/TFMessage. */
using Ros2Tf2_msgsMsgTFMessage=roswasmc_bounded_2_tf2_msgs_msg_TFMessage;
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
/** Checked codec for geometry_msgs/msg/TransformStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgTransformStamped> {
  static constexpr size_t max_wire_bytes=648;
  static constexpr char const* codec_identity="UWVMROS1_be553b5a9c73b687cde56ff1c381e3c67168fd2c0366830be3c65382c5bbd792";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Geometry_msgsMsgTransformStamped default_value() noexcept {uwvm_custom::Ros2Geometry_msgsMsgTransformStamped value={0};(void)roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgTransformStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgTransformStamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/msg/DebugLevel; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsMsgDebugLevel> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_989475cc5d981d8c5ab91fb091235871f7a1c7d87d246eb2250086e6c3c37c79";
  static bool encode(uwvm_custom::Ros2Industrial_msgsMsgDebugLevel const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsMsgDebugLevel& out) {return !roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/msg/DeviceInfo; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsMsgDeviceInfo> {
  static constexpr size_t max_wire_bytes=2580;
  static constexpr char const* codec_identity="UWVMROS1_f9a5962800e17495f34dbb73f37373df22f900e528c630497417f78cdecb283b";
  static bool encode(uwvm_custom::Ros2Industrial_msgsMsgDeviceInfo const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsMsgDeviceInfo& out) {return !roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/msg/RobotMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsMsgRobotMode> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_4f9173c5e27815ca33be603b8645a22420bda3ab308962127060d2413d092c4c";
  static bool encode(uwvm_custom::Ros2Industrial_msgsMsgRobotMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_msg_RobotMode_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_msg_RobotMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_msg_RobotMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsMsgRobotMode& out) {return !roswasmc_bounded_2_industrial_msgs_msg_RobotMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/msg/TriState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsMsgTriState> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b7ae62ce19d642ad46e962ffb50cd76c43c96ac673ac36d690f99c1dd8d2a841";
  static bool encode(uwvm_custom::Ros2Industrial_msgsMsgTriState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_msg_TriState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_msg_TriState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsMsgTriState& out) {return !roswasmc_bounded_2_industrial_msgs_msg_TriState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/msg/RobotStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsMsgRobotStatus> {
  static constexpr size_t max_wire_bytes=150;
  static constexpr char const* codec_identity="UWVMROS1_1086f16ba5d614f3d87c3ab195240b5a0683ef490f2e92434e4ea3bdbe7554ba";
  static bool encode(uwvm_custom::Ros2Industrial_msgsMsgRobotStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsMsgRobotStatus& out) {return !roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/msg/ServiceReturnCode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsMsgServiceReturnCode> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_62eb4415a63d7686f16ea343193238838c78aa8e9b9786a65c92fb227e03d3e5";
  static bool encode(uwvm_custom::Ros2Industrial_msgsMsgServiceReturnCode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsMsgServiceReturnCode& out) {return !roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/CmdJointTrajectory_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvCmdJointTrajectory_Request> {
  static constexpr size_t max_wire_bytes=4164;
  static constexpr char const* codec_identity="UWVMROS1_1def3934b243546a4591b7be1ef2eb27002fbd9d3725302016e3b1846805149d";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvCmdJointTrajectory_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvCmdJointTrajectory_Request& out) {return !roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/CmdJointTrajectory_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvCmdJointTrajectory_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1e4d064ef659e5d1cc73bdd90f82657a16087c96e0f75b632e2be2be884bca8b";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvCmdJointTrajectory_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvCmdJointTrajectory_Response& out) {return !roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/GetRobotInfo_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvGetRobotInfo_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_688a272f6ee39921013c6c39a7b0b0c4e2cda5119787088234103518ba25c1d2";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvGetRobotInfo_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvGetRobotInfo_Request& out) {return !roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/GetRobotInfo_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvGetRobotInfo_Response> {
  static constexpr size_t max_wire_bytes=32768;
  static constexpr char const* codec_identity="UWVMROS1_6d37cdcd8afc918329d7856e682d7fcdae2931832474a9194143c21079ed4152";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvGetRobotInfo_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvGetRobotInfo_Response& out) {return !roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/SetDrivePower_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvSetDrivePower_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b4fc5e109e131a97820ab5856b34839c572ac0b0c256d70457c176e401844f96";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvSetDrivePower_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvSetDrivePower_Request& out) {return !roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/SetDrivePower_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvSetDrivePower_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_050fb9a727ebedd240e9023b9a2379a5d72e0d9fa335bead0c40d783abb22cff";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvSetDrivePower_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvSetDrivePower_Response& out) {return !roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/SetRemoteLoggerLevel_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_bbadd39c05cab90b14f4b1fad23ff05ac83325f6cc57810cd649e04fa29fe014";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Request& out) {return !roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/SetRemoteLoggerLevel_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b8681d031a4bb5a67b842277af2e2156e8339bdef23be88d2b64e296a7e3d63b";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvSetRemoteLoggerLevel_Response& out) {return !roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/StartMotion_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvStartMotion_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_0a96fb9489f863612a91bf82f995f2f28bcb7fb403bde74c617925c789e4dcd8";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvStartMotion_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvStartMotion_Request& out) {return !roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/StartMotion_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvStartMotion_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_3bb3650fbe638a6d300bcd9fcd5acf574bd54d00a7368edd3014bf44dcf66c16";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvStartMotion_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvStartMotion_Response& out) {return !roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/StopMotion_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvStopMotion_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_fe5a7cf0da30270eab5cfb1ab8c1fbd37d2b4c4f91ef29662a0ddcac39e6ef49";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvStopMotion_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvStopMotion_Request& out) {return !roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for industrial_msgs/srv/StopMotion_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Industrial_msgsSrvStopMotion_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_c26bda240d86becc5b8b4ea80380eba3602092a5ec4a06818dbfbd38c24015b5";
  static bool encode(uwvm_custom::Ros2Industrial_msgsSrvStopMotion_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Industrial_msgsSrvStopMotion_Response& out) {return !roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/AlarmCauseRemedy; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgAlarmCauseRemedy> {
  static constexpr size_t max_wire_bytes=1032;
  static constexpr char const* codec_identity="UWVMROS1_f977609022f4ca593b9089d0038d1c28d1bc4b533e272f38c3379495f326ddf5";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgAlarmCauseRemedy const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgAlarmCauseRemedy& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/AlarmInfo; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgAlarmInfo> {
  static constexpr size_t max_wire_bytes=25248;
  static constexpr char const* codec_identity="UWVMROS1_7cf3fa3037f28f295dd62009e708ea3e46a80f83471d0a0f9fb55af125c0c27f";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgAlarmInfo const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgAlarmInfo& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/ErrorInfo; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgErrorInfo> {
  static constexpr size_t max_wire_bytes=532;
  static constexpr char const* codec_identity="UWVMROS1_1cfe60c0af6bc157db84c4d9e0b20a8f2e5c78fd4afd6908d32b3217f3734b67";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgErrorInfo const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgErrorInfo& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/InformJobCrudResultCodes; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgInformJobCrudResultCodes> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_f5ed037b83926b48a88fa486ed8db5f5d3e39e4a486d4a90190ffd6a70452613";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgInformJobCrudResultCodes const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgInformJobCrudResultCodes& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/InitTrajEnum; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgInitTrajEnum> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_9279979f9e3570f48d6e23bf812a4680e3deed9af3309019efdff5464d7b7763";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgInitTrajEnum const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgInitTrajEnum& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/IoResultCodes; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgIoResultCodes> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_ef9ab829ae79d4e58336a56259b6beb2ad26a8b1b4070013d9004ac4ab269915";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgIoResultCodes const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgIoResultCodes& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/MotionReadyEnum; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgMotionReadyEnum> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_c8d4c2b0eba665e01cddc730421a3a42c5b344f4de35ae0a32204891603326d6";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgMotionReadyEnum const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgMotionReadyEnum& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/QueueResultEnum; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgQueueResultEnum> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_146c6ce3f275b9e4a81622d73cca3d2298cf9f7bb95a783c0dea6279cb5b4f23";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgQueueResultEnum const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgQueueResultEnum& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/msg/SelectionResultCodes; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesMsgSelectionResultCodes> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_46168b1f851163985e75842d9d795c5715160864a7736e7f92f95818c9435602";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesMsgSelectionResultCodes const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesMsgSelectionResultCodes& out) {return !roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/GetActiveAlarmInfo_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_aadcd63f3bc47424c38767afdc1589561e3c90e8aeae9bdceef88b1033d57a7e";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/GetActiveAlarmInfo_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response> {
  static constexpr size_t max_wire_bytes=32768;
  static constexpr char const* codec_identity="UWVMROS1_e11bdbb95b6ded88d035696260c58dad32bcf3604aa4e89dd41c0dab26541567";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvGetActiveAlarmInfo_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ListInformJobs_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvListInformJobs_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_e77eedadef98d98321b1757b88284022afcdb1ffef52b98820b0c4ba596d643c";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvListInformJobs_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvListInformJobs_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ListInformJobs_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvListInformJobs_Response> {
  static constexpr size_t max_wire_bytes=8780;
  static constexpr char const* codec_identity="UWVMROS1_49021f0abc00f8df1ef49605a244cb6bba8d8b45ece5365ae35d747d2506eadc";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvListInformJobs_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvListInformJobs_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/QueueTrajPoint_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvQueueTrajPoint_Request> {
  static constexpr size_t max_wire_bytes=1228;
  static constexpr char const* codec_identity="UWVMROS1_3d37f9981d6abaca33ec0b5378fa3a1ac301791cc9be94d5bdcf4ea6aa81b8d0";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvQueueTrajPoint_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvQueueTrajPoint_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/QueueTrajPoint_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvQueueTrajPoint_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_d4887ed24d39332a5ccfd4a3d072004dd8f314fe5c166bf0bd57520a826cbccd";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvQueueTrajPoint_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvQueueTrajPoint_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ReadGroupIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvReadGroupIO_Request> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_6ee09ac379e169fc23b81a551e9a9ed21c3f82b5010859984e911b17152beb7f";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvReadGroupIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvReadGroupIO_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ReadGroupIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvReadGroupIO_Response> {
  static constexpr size_t max_wire_bytes=522;
  static constexpr char const* codec_identity="UWVMROS1_292fcf6e8c46a2509ee4cc5238d443157f37fed0de898367248b967e9001ea0d";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvReadGroupIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvReadGroupIO_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ReadMRegister_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvReadMRegister_Request> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_f31f0983466054f5ac5dcb2ab80ab7721d606a681b833ebb376cc87f96054f6f";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvReadMRegister_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvReadMRegister_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ReadMRegister_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvReadMRegister_Response> {
  static constexpr size_t max_wire_bytes=523;
  static constexpr char const* codec_identity="UWVMROS1_8d6ded2876adfa2079711205cb0110a82ead6f51db05f266bf285308cd815a04";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvReadMRegister_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvReadMRegister_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ReadSingleIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvReadSingleIO_Request> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_30e28db303b772f26b2b3cf6165a0ff79037d037d60e8115b4ee8e177f17e8bb";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvReadSingleIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvReadSingleIO_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ReadSingleIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvReadSingleIO_Response> {
  static constexpr size_t max_wire_bytes=525;
  static constexpr char const* codec_identity="UWVMROS1_b42013571274e85769ffac4d47129de8b76e4a675bdef3efefebc9ee19e92082";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvReadSingleIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvReadSingleIO_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ResetError_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvResetError_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_2737855d0608478946e2d9ab7f06c92585199c1082ec8406a012f50ad56beae3";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvResetError_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvResetError_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/ResetError_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvResetError_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_0af14ee02c157ecc39b143225a580e475473306389ccc3ba198cf22ca47d82f3";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvResetError_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvResetError_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/SelectMotionTool_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvSelectMotionTool_Request> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_e6c884ae0a97337c1e441efe099b7869dc95205fcbca0a8082d4d9b06f5b438c";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvSelectMotionTool_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvSelectMotionTool_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/SelectMotionTool_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvSelectMotionTool_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_088349a49c14c52282c25b428628462c48618c3b5202df560dcc5c9bed742640";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvSelectMotionTool_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvSelectMotionTool_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/StartPointQueueMode_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvStartPointQueueMode_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_58a4827fc6535ef0ffc64ee25c5c4af75aa0fe199fa0350a88a95d2e991164a1";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvStartPointQueueMode_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvStartPointQueueMode_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/StartPointQueueMode_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvStartPointQueueMode_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_bc553d5640e1c4644a5d5bdc2dfcefc9dbe789ef1e2a23dd620f08963467b6ff";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvStartPointQueueMode_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvStartPointQueueMode_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/StartTrajMode_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvStartTrajMode_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_0b8b2ad5db5901047035e1070b675618afc516a1ae8c3308cfdcad1174d34789";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvStartTrajMode_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvStartTrajMode_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/StartTrajMode_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvStartTrajMode_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_170e25737e6a2b06f5d5d9f331d748c97cf0306284271757953a4a801c722939";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvStartTrajMode_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvStartTrajMode_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/WriteGroupIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvWriteGroupIO_Request> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_2d348ed71408a0dd1dd28ee2ed2ca1db56bb07a14546d0fe13ccf64e8553d52c";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvWriteGroupIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvWriteGroupIO_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/WriteGroupIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvWriteGroupIO_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_06769934711b27fd32cdcbf849504e160189ee82ea32e0e71a2903af47c5d766";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvWriteGroupIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvWriteGroupIO_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/WriteMRegister_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvWriteMRegister_Request> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_8df4536be27f0632052725d5616fd5b047f49fc306fc7735854d9de4fb15d4ef";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvWriteMRegister_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvWriteMRegister_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/WriteMRegister_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvWriteMRegister_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_a61cc0596a7545ef4836bdc64dc9d6b4ed99b0abd18a726eb37e6215f51c729a";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvWriteMRegister_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvWriteMRegister_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/WriteSingleIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvWriteSingleIO_Request> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_6361affcd8b7ebaaa1fe6b5806e64a7c18f7a96a1178614cfce487de813a6392";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvWriteSingleIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvWriteSingleIO_Request& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for motoros2_interfaces/srv/WriteSingleIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Motoros2_interfacesSrvWriteSingleIO_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_12328db372a572fe9b0c7207331a7971d5d7c9033376e3be410db579b85fb794";
  static bool encode(uwvm_custom::Ros2Motoros2_interfacesSrvWriteSingleIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Motoros2_interfacesSrvWriteSingleIO_Response& out) {return !roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for tf2_msgs/msg/TFMessage; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Tf2_msgsMsgTFMessage> {
  static constexpr size_t max_wire_bytes=10372;
  static constexpr char const* codec_identity="UWVMROS1_4428597a9c2783fd8f0f083a16e8fd1b3d286487f4800c55fcc5571e7f79e65a";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Tf2_msgsMsgTFMessage default_value() noexcept {uwvm_custom::Ros2Tf2_msgsMsgTFMessage value={0};(void)roswasmc_bounded_2_tf2_msgs_msg_TFMessage_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Tf2_msgsMsgTFMessage const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_tf2_msgs_msg_TFMessage_valid(&value))return false;size_t n=roswasmc_bounded_2_tf2_msgs_msg_TFMessage_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_tf2_msgs_msg_TFMessage_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Tf2_msgsMsgTFMessage& out) {return !roswasmc_bounded_2_tf2_msgs_msg_TFMessage_decode(bytes.data(),bytes.size(),&out);}
};
}
