/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for abb_egm_msgs/msg/EGMChannelState. */
using Ros2Abb_egm_msgsMsgEGMChannelState=roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState;
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for abb_egm_msgs/msg/EGMState. */
using Ros2Abb_egm_msgsMsgEGMState=roswasmc_bounded_2_abb_egm_msgs_msg_EGMState;
/** Complete bounded guest value for abb_rapid_msgs/msg/ConfData. */
using Ros2Abb_rapid_msgsMsgConfData=roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData;
/** Complete bounded guest value for abb_rapid_msgs/msg/ExtJoint. */
using Ros2Abb_rapid_msgsMsgExtJoint=roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint;
/** Complete bounded guest value for abb_rapid_msgs/msg/RobJoint. */
using Ros2Abb_rapid_msgsMsgRobJoint=roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint;
/** Complete bounded guest value for abb_rapid_msgs/msg/JointTarget. */
using Ros2Abb_rapid_msgsMsgJointTarget=roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget;
/** Complete bounded guest value for abb_rapid_msgs/msg/Pos. */
using Ros2Abb_rapid_msgsMsgPos=roswasmc_bounded_2_abb_rapid_msgs_msg_Pos;
/** Complete bounded guest value for abb_rapid_msgs/msg/Orient. */
using Ros2Abb_rapid_msgsMsgOrient=roswasmc_bounded_2_abb_rapid_msgs_msg_Orient;
/** Complete bounded guest value for abb_rapid_msgs/msg/LoadData. */
using Ros2Abb_rapid_msgsMsgLoadData=roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData;
/** Complete bounded guest value for abb_rapid_msgs/msg/Pose. */
using Ros2Abb_rapid_msgsMsgPose=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose;
/** Complete bounded guest value for abb_rapid_msgs/msg/RobTarget. */
using Ros2Abb_rapid_msgsMsgRobTarget=roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget;
/** Complete bounded guest value for abb_rapid_msgs/msg/ToolData. */
using Ros2Abb_rapid_msgsMsgToolData=roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData;
/** Complete bounded guest value for abb_rapid_msgs/msg/WObjData. */
using Ros2Abb_rapid_msgsMsgWObjData=roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/msg/EGMActivateSettings. */
using Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/msg/EGMRunSettings. */
using Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/msg/EGMSetupSettings. */
using Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/msg/EGMStopSettings. */
using Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/msg/EGMSettings. */
using Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/msg/StateMachineState. */
using Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/msg/RuntimeState. */
using Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Request. */
using Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Response. */
using Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Request. */
using Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Response. */
using Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request. */
using Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Response. */
using Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/srv/SetSGCommand_Request. */
using Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/srv/SetSGCommand_Response. */
using Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response;
/** Complete bounded guest value for abb_robot_msgs/msg/MechanicalUnitState. */
using Ros2Abb_robot_msgsMsgMechanicalUnitState=roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState;
/** Complete bounded guest value for abb_robot_msgs/msg/RAPIDSymbolPath. */
using Ros2Abb_robot_msgsMsgRAPIDSymbolPath=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath;
/** Complete bounded guest value for abb_robot_msgs/msg/RAPIDTaskState. */
using Ros2Abb_robot_msgsMsgRAPIDTaskState=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState;
/** Complete bounded guest value for abb_robot_msgs/msg/ServiceResponses. */
using Ros2Abb_robot_msgsMsgServiceResponses=roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses;
/** Complete bounded guest value for abb_robot_msgs/msg/SystemState. */
using Ros2Abb_robot_msgsMsgSystemState=roswasmc_bounded_2_abb_robot_msgs_msg_SystemState;
/** Complete bounded guest value for abb_robot_msgs/srv/GetFileContents_Request. */
using Ros2Abb_robot_msgsSrvGetFileContents_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetFileContents_Response. */
using Ros2Abb_robot_msgsSrvGetFileContents_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/GetIOSignal_Request. */
using Ros2Abb_robot_msgsSrvGetIOSignal_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetIOSignal_Response. */
using Ros2Abb_robot_msgsSrvGetIOSignal_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDBool_Request. */
using Ros2Abb_robot_msgsSrvGetRAPIDBool_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDBool_Response. */
using Ros2Abb_robot_msgsSrvGetRAPIDBool_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDDnum_Request. */
using Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDDnum_Response. */
using Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDNum_Request. */
using Ros2Abb_robot_msgsSrvGetRAPIDNum_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDNum_Response. */
using Ros2Abb_robot_msgsSrvGetRAPIDNum_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDString_Request. */
using Ros2Abb_robot_msgsSrvGetRAPIDString_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDString_Response. */
using Ros2Abb_robot_msgsSrvGetRAPIDString_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDSymbol_Request. */
using Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRAPIDSymbol_Response. */
using Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRobotControllerDescription_Request. */
using Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetRobotControllerDescription_Response. */
using Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/GetSpeedRatio_Request. */
using Ros2Abb_robot_msgsSrvGetSpeedRatio_Request=roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/GetSpeedRatio_Response. */
using Ros2Abb_robot_msgsSrvGetSpeedRatio_Response=roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/SetFileContents_Request. */
using Ros2Abb_robot_msgsSrvSetFileContents_Request=roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/SetFileContents_Response. */
using Ros2Abb_robot_msgsSrvSetFileContents_Response=roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/SetIOSignal_Request. */
using Ros2Abb_robot_msgsSrvSetIOSignal_Request=roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/SetIOSignal_Response. */
using Ros2Abb_robot_msgsSrvSetIOSignal_Response=roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDBool_Request. */
using Ros2Abb_robot_msgsSrvSetRAPIDBool_Request=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDBool_Response. */
using Ros2Abb_robot_msgsSrvSetRAPIDBool_Response=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDDnum_Request. */
using Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDDnum_Response. */
using Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDNum_Request. */
using Ros2Abb_robot_msgsSrvSetRAPIDNum_Request=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDNum_Response. */
using Ros2Abb_robot_msgsSrvSetRAPIDNum_Response=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDString_Request. */
using Ros2Abb_robot_msgsSrvSetRAPIDString_Request=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDString_Response. */
using Ros2Abb_robot_msgsSrvSetRAPIDString_Response=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDSymbol_Request. */
using Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/SetRAPIDSymbol_Response. */
using Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/SetSpeedRatio_Request. */
using Ros2Abb_robot_msgsSrvSetSpeedRatio_Request=roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/SetSpeedRatio_Response. */
using Ros2Abb_robot_msgsSrvSetSpeedRatio_Response=roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response;
/** Complete bounded guest value for abb_robot_msgs/srv/TriggerWithResultCode_Request. */
using Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request=roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request;
/** Complete bounded guest value for abb_robot_msgs/srv/TriggerWithResultCode_Response. */
using Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response=roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response;
/** Complete bounded guest value for builtin_interfaces/msg/Duration. */
using Ros2Builtin_interfacesMsgDuration=roswasmc_bounded_2_builtin_interfaces_msg_Duration;
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
/** Complete bounded guest value for std_srvs/srv/Trigger_Request. */
using Ros2Std_srvsSrvTrigger_Request=roswasmc_bounded_2_std_srvs_srv_Trigger_Request;
/** Complete bounded guest value for std_srvs/srv/Trigger_Response. */
using Ros2Std_srvsSrvTrigger_Response=roswasmc_bounded_2_std_srvs_srv_Trigger_Response;
}
namespace roswasmcpp {
/** Checked codec for abb_egm_msgs/msg/EGMChannelState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_egm_msgsMsgEGMChannelState> {
  static constexpr size_t max_wire_bytes=81;
  static constexpr char const* codec_identity="UWVMROS1_fbd288b93963fccc92aa114570c0a4f6a1fdc5b473cb8e252922720f5d88b519";
  static bool encode(uwvm_custom::Ros2Abb_egm_msgsMsgEGMChannelState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_egm_msgsMsgEGMChannelState& out) {return !roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for abb_egm_msgs/msg/EGMState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_egm_msgsMsgEGMState> {
  static constexpr size_t max_wire_bytes=1376;
  static constexpr char const* codec_identity="UWVMROS1_5b10efb13c3be7290c7e391d55679a174969887edd1b8baca682edac311a8e5d";
  static bool encode(uwvm_custom::Ros2Abb_egm_msgsMsgEGMState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_egm_msgsMsgEGMState& out) {return !roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/ConfData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgConfData> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_0c22cc29db62f5001ef20ea7b131de63099e31ac5f150d3e9f0bb62120d60571";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgConfData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgConfData& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/ExtJoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgExtJoint> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_8b799c6bec9e4eab0900a633a93375a7792e49fde0bf302f6752115a001a4ae2";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgExtJoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgExtJoint& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/RobJoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgRobJoint> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_6d842aebc0a3b222a727063a41ef7e96f2b8c14ffbba5c8d6e140ba2890e69cd";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgRobJoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgRobJoint& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/JointTarget; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgJointTarget> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_5949cf0121391646c1f04b54d8f7deaae0497f4920f7817739fc25bfddc4e540";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgJointTarget const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgJointTarget& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/Pos; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgPos> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_31d490d6ead5a1a78295dc4e934f0b89e74e4732da2b57d28d297477bf4cbdd2";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgPos const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgPos& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/Orient; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgOrient> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_821d887b88298422f9466aaf3926db580f878e964055fbd72226b7f76f2f536e";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgOrient const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgOrient& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/LoadData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgLoadData> {
  static constexpr size_t max_wire_bytes=44;
  static constexpr char const* codec_identity="UWVMROS1_f73e480a0e6746a4df8f4d62bc123d68d01d3dc48f0bc5f0eac51b859a0750f9";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgLoadData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgLoadData& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/Pose; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgPose> {
  static constexpr size_t max_wire_bytes=28;
  static constexpr char const* codec_identity="UWVMROS1_8f1535fe21326706fd0b16314857162d36bca91ce8e8f6a8bec3864dafb9e2f2";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgPose const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgPose& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/RobTarget; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgRobTarget> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_8decb56e8faf0a6a954af974adfea5c500aea8392e6a778ae7394174323489d8";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgRobTarget const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgRobTarget& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/ToolData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgToolData> {
  static constexpr size_t max_wire_bytes=73;
  static constexpr char const* codec_identity="UWVMROS1_948fa0bbaaf95f0d88ca03cad687f16102261f54b0da889ae8bca82042fc5237";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgToolData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgToolData& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/msg/WObjData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_msgsMsgWObjData> {
  static constexpr size_t max_wire_bytes=574;
  static constexpr char const* codec_identity="UWVMROS1_b36f0448512fab6028431d19b39a8ae31b7b81b2615ffc8311139f63b5d8e7cd";
  static bool encode(uwvm_custom::Ros2Abb_rapid_msgsMsgWObjData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_msgsMsgWObjData& out) {return !roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/msg/EGMActivateSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings> {
  static constexpr size_t max_wire_bytes=716;
  static constexpr char const* codec_identity="UWVMROS1_fce73aea80d9b8a299b8e2ab742602871146b3ebbd624a2e807e25c1ac107cfa";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/msg/EGMRunSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings> {
  static constexpr size_t max_wire_bytes=40;
  static constexpr char const* codec_identity="UWVMROS1_92a2a654eb2eda89077933f955f04ed87884827b8875cf563570eed5d211b0e2";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/msg/EGMSetupSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_2565bed5b83b132c69f37d7294c69181800ad2974ddc54f60868da57e28e782c";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/msg/EGMStopSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_0f3f2f6034567a7d49dd140ff9ff620e8567548139597b9b823e63a947673cee";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/msg/EGMSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings> {
  static constexpr size_t max_wire_bytes=767;
  static constexpr char const* codec_identity="UWVMROS1_622d3a975eecb11f7729aecfadc161cada271208d6fe7599f455750b90144ef3";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/msg/StateMachineState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_ff2a09e4b6e8ef26cbdc3ecdffdceb33d6b2c82f0d51ed2c38ce9648c6f47711";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/msg/RuntimeState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState> {
  static constexpr size_t max_wire_bytes=8368;
  static constexpr char const* codec_identity="UWVMROS1_3c0f157544fa82c4e206a4fb42f8ba7f67213d19533f79b77c66dec6fd0460d9";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_363be2459fcc9c475c6305cbcfebf7689e891eb27645b7667960aace23d56e8e";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response> {
  static constexpr size_t max_wire_bytes=1285;
  static constexpr char const* codec_identity="UWVMROS1_d8f340e1c6d78fc0a782c3279efb34ae5e11df175a82f9dd5ee82e603d6b2ffb";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request> {
  static constexpr size_t max_wire_bytes=835;
  static constexpr char const* codec_identity="UWVMROS1_c2f3dd535e9475526bf18bdffa0e9a839049d315c9aebb150f8561ee3361a2bd";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_6a1adeebe31dfe101a6d57019e26ee2f1144b9a6ce30aee08d4c6fe56ea6838d";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request> {
  static constexpr size_t max_wire_bytes=584;
  static constexpr char const* codec_identity="UWVMROS1_4561b7e5e296a1a329d5f5684bcbde1ef41324554a34e9a79a85824286dd477c";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_980d2a16536289f446161acdf2fc57d459baa2290d74f1dd5aaf8129126cbba1";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/srv/SetSGCommand_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request> {
  static constexpr size_t max_wire_bytes=73;
  static constexpr char const* codec_identity="UWVMROS1_5d6983aa85c2c52f8ec06cbd723d5929cfc381db2e0bfffb4c27078497d86c90";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/srv/SetSGCommand_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_87cdc41c81d49930dce7193d4b7af98add266a6e9a7cf5e69457d01ded388cf8";
  static bool encode(uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response& out) {return !roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/msg/MechanicalUnitState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsMsgMechanicalUnitState> {
  static constexpr size_t max_wire_bytes=69;
  static constexpr char const* codec_identity="UWVMROS1_1dc7a7dcbaa19e78cd6419f8844287a7fbd9d7a839ab1a8ad97ef0be34f3a3a8";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsMsgMechanicalUnitState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsMsgMechanicalUnitState& out) {return !roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/msg/RAPIDSymbolPath; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsMsgRAPIDSymbolPath> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_249523197567fbf638f30487a7f5b8d8ff5ea746fcd4a65a35f2dbb18a3adf0e";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsMsgRAPIDSymbolPath const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsMsgRAPIDSymbolPath& out) {return !roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/msg/RAPIDTaskState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsMsgRAPIDTaskState> {
  static constexpr size_t max_wire_bytes=71;
  static constexpr char const* codec_identity="UWVMROS1_ff698e375b0ff33026cfd248c1ac734f939763aba25a6c4f7cac628d445422fd";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsMsgRAPIDTaskState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsMsgRAPIDTaskState& out) {return !roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/msg/ServiceResponses; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsMsgServiceResponses> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_4e01b972b67454e851801f28aee09c0774b67907d2241cc67cae7d045c18f646";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsMsgServiceResponses const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsMsgServiceResponses& out) {return !roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/msg/SystemState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsMsgSystemState> {
  static constexpr size_t max_wire_bytes=2327;
  static constexpr char const* codec_identity="UWVMROS1_c1cdbf0e578fddc5bf70cc2cc181d786f3e8189afc6a67962de6eb273973a38f";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsMsgSystemState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsMsgSystemState& out) {return !roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetFileContents_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContents_Request> {
  static constexpr size_t max_wire_bytes=516;
  static constexpr char const* codec_identity="UWVMROS1_bdcf73a965bd05356e48019b74d633176fdeda62452bc7185d6b6ff30576016b";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContents_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContents_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetFileContents_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContents_Response> {
  static constexpr size_t max_wire_bytes=4618;
  static constexpr char const* codec_identity="UWVMROS1_f05617145a40043e87cbc251f833274d2a962bd7cdaabc5f9a8e4591bd56e107";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContents_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContents_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetIOSignal_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignal_Request> {
  static constexpr size_t max_wire_bytes=516;
  static constexpr char const* codec_identity="UWVMROS1_2406d53c2fce44e72ad7e6f1c9e75ea19c4df694ff9f00917229ec758209b1f5";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignal_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignal_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetIOSignal_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignal_Response> {
  static constexpr size_t max_wire_bytes=1034;
  static constexpr char const* codec_identity="UWVMROS1_1f668ef78c353f2ec8defdac619d349cda731576f2429cf07107b9b56ba9030e";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignal_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignal_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDBool_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBool_Request> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_c381080d235c78216a8e64275819ff3fb97e1b1de96e7da758c4889630b8e0ff";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBool_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBool_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDBool_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBool_Response> {
  static constexpr size_t max_wire_bytes=519;
  static constexpr char const* codec_identity="UWVMROS1_b853e212489fb815866fa29b165eea474a977483158b6203699c4a1293aa6acd";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBool_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBool_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDDnum_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_71057c2fe44bc96505ec6e05d998fcb6323b281fff40808ce990207c48626979";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDDnum_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response> {
  static constexpr size_t max_wire_bytes=526;
  static constexpr char const* codec_identity="UWVMROS1_af0420c126368a7e45b728c7fef76062b5ef2f0adea7535ad38e4771244edd56";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDNum_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNum_Request> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_03d5da34af8d90ed8a20f8ca38fa75d9e777496828978f0f348e3c1c8f67d0a5";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNum_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNum_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDNum_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNum_Response> {
  static constexpr size_t max_wire_bytes=522;
  static constexpr char const* codec_identity="UWVMROS1_a0da03014dd001e58efc4a5b274cd5c62016e7fa257af1120a8142a79635220d";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNum_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNum_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDString_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDString_Request> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_c7bc9c788301e619a165c18a1cdb0aca80df8def5be31da5dbf34c0038e73596";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDString_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDString_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDString_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDString_Response> {
  static constexpr size_t max_wire_bytes=1034;
  static constexpr char const* codec_identity="UWVMROS1_cf7bfdb2af5b91eb58b19b909f7fa3357dc1dad02716f8fa4465b53f3f4a1458";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDString_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDString_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDSymbol_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_e216a54cdae70f8147489928ff05a4b172ba4fc47e9ed5df4828b070354ac469";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRAPIDSymbol_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response> {
  static constexpr size_t max_wire_bytes=1034;
  static constexpr char const* codec_identity="UWVMROS1_b1f6de7ad0640f5645678d327804dc7e732bdad38805e642b13ed7294c21d602";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRobotControllerDescription_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_caf70e11b386a933f62d06a433ca1cedaa3c8ecc75b7e8de8c1d5ae00a29f03e";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetRobotControllerDescription_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response> {
  static constexpr size_t max_wire_bytes=4618;
  static constexpr char const* codec_identity="UWVMROS1_1af2591f035379597f376ed1e2343f73a89f9fef5c29269aa420243a82e6c368";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetSpeedRatio_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatio_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_54ee2c897e2bf2f39c96cc42246ffe5997c8df280280433e8748b3bcf24c299c";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatio_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatio_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/GetSpeedRatio_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatio_Response> {
  static constexpr size_t max_wire_bytes=519;
  static constexpr char const* codec_identity="UWVMROS1_baa271a258c7ca274e0145fea1abb7cbc43e98c82b8e40d67c4743237a19910b";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatio_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatio_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetFileContents_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContents_Request> {
  static constexpr size_t max_wire_bytes=4616;
  static constexpr char const* codec_identity="UWVMROS1_3bfa8ad1846fc31acae47a875d3555a78a64b777923eb9b62e87a836f041b26b";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContents_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContents_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetFileContents_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContents_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_1586344401d43055460b49f2487567638f66838cfe5cac99a47e0a39c0b06ede";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContents_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContents_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetIOSignal_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignal_Request> {
  static constexpr size_t max_wire_bytes=1032;
  static constexpr char const* codec_identity="UWVMROS1_2d206d0020179263e83e745a428616ed53d809b0c65a79ad495cb882ae9e49b7";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignal_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignal_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetIOSignal_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignal_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_1fa28cc3160d51f71d0740cbe145663ec360d6fa488c2a6bf14ad6ab92004a75";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignal_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignal_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDBool_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBool_Request> {
  static constexpr size_t max_wire_bytes=205;
  static constexpr char const* codec_identity="UWVMROS1_4e707e429acc860d032d216bcd2c1eb1a0f731fad7e7c0bef7beed810c6e4a52";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBool_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBool_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDBool_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBool_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_80d2b31c93771cfa8a19ff82a3d9d63951bf9d28c039d2725017327fa3b0114a";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBool_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBool_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDDnum_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request> {
  static constexpr size_t max_wire_bytes=212;
  static constexpr char const* codec_identity="UWVMROS1_cece086311a908f12c0b0ab3988ca829e891749c74687c717d2a52bfb8ceaf76";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDDnum_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_e5f77fe9850f109d873a2784b9690c8aa0632b9e91d158ed4d14daf1aaf90ea6";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDNum_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNum_Request> {
  static constexpr size_t max_wire_bytes=208;
  static constexpr char const* codec_identity="UWVMROS1_a5bee9a64be69dc4246f68837a44362f52795ae3c6420a2df47685606283ea02";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNum_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNum_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDNum_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNum_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_744f3c5fac58ff098c96cedcea94fe6fa87c88ceb89674449f50fbdcd1ca30bb";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNum_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNum_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDString_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDString_Request> {
  static constexpr size_t max_wire_bytes=720;
  static constexpr char const* codec_identity="UWVMROS1_f21c45f3f8cd4c0b50781bdd487fdc6ec0c0b35adbc7e36e9ca5b7cc47c3d461";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDString_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDString_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDString_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDString_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_1496ef4f4ffe48fe74b9e25b925c19f10f0e171a9d33fa2e56c15aa5cfbe4774";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDString_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDString_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDSymbol_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request> {
  static constexpr size_t max_wire_bytes=720;
  static constexpr char const* codec_identity="UWVMROS1_928d427801b79775595fed6e3779f9ba9e7e8e27f2de66cf79610cb51bb72bcf";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetRAPIDSymbol_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_e156616a51bf4691a1c3853d62a8b0c43bb17d1a330cd4e7a58eceb5fe442382";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetSpeedRatio_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatio_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_da30970377ba1bc151b544560773f02e17f4eaed56eb4c8d791de72bdef04355";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatio_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatio_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/SetSpeedRatio_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatio_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_25e6d7edaa0503a4d45d893e886db4215bfd0af8711307c141bc63552609e3a5";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatio_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatio_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/TriggerWithResultCode_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_fd1d49c3b4e489464c6662f306210137f71d15192f64cd8a8abf93acd5fbff8d";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/srv/TriggerWithResultCode_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_e38422b2a008f07e108505ac3ce65c24189a1d6657e88a6289e17d7d4ae38d51";
  static bool encode(uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response& out) {return !roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for builtin_interfaces/msg/Duration; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Builtin_interfacesMsgDuration> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4";
  static bool encode(uwvm_custom::Ros2Builtin_interfacesMsgDuration const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value))return false;size_t n=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_builtin_interfaces_msg_Duration_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Builtin_interfacesMsgDuration& out) {return !roswasmc_bounded_2_builtin_interfaces_msg_Duration_decode(bytes.data(),bytes.size(),&out);}
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
