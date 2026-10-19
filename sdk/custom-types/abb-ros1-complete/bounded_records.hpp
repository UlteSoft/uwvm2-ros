/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for abb_egm_msgs/EGMChannelState. */
using Ros1Abb_egm_msgsEGMChannelState=roswasmc_bounded_1_abb_egm_msgs_EGMChannelState;
/** Complete bounded guest value for time. */
using Ros1Time=roswasmc_bounded_1_time;
/** Complete bounded guest value for std_msgs/Header. */
using Ros1Std_msgsHeader=roswasmc_bounded_1_std_msgs_Header;
/** Complete bounded guest value for abb_egm_msgs/EGMState. */
using Ros1Abb_egm_msgsEGMState=roswasmc_bounded_1_abb_egm_msgs_EGMState;
/** Complete bounded guest value for abb_rapid_msgs/ConfData. */
using Ros1Abb_rapid_msgsConfData=roswasmc_bounded_1_abb_rapid_msgs_ConfData;
/** Complete bounded guest value for abb_rapid_msgs/ExtJoint. */
using Ros1Abb_rapid_msgsExtJoint=roswasmc_bounded_1_abb_rapid_msgs_ExtJoint;
/** Complete bounded guest value for abb_rapid_msgs/RobJoint. */
using Ros1Abb_rapid_msgsRobJoint=roswasmc_bounded_1_abb_rapid_msgs_RobJoint;
/** Complete bounded guest value for abb_rapid_msgs/JointTarget. */
using Ros1Abb_rapid_msgsJointTarget=roswasmc_bounded_1_abb_rapid_msgs_JointTarget;
/** Complete bounded guest value for abb_rapid_msgs/Pos. */
using Ros1Abb_rapid_msgsPos=roswasmc_bounded_1_abb_rapid_msgs_Pos;
/** Complete bounded guest value for abb_rapid_msgs/Orient. */
using Ros1Abb_rapid_msgsOrient=roswasmc_bounded_1_abb_rapid_msgs_Orient;
/** Complete bounded guest value for abb_rapid_msgs/LoadData. */
using Ros1Abb_rapid_msgsLoadData=roswasmc_bounded_1_abb_rapid_msgs_LoadData;
/** Complete bounded guest value for abb_rapid_msgs/Pose. */
using Ros1Abb_rapid_msgsPose=roswasmc_bounded_1_abb_rapid_msgs_Pose;
/** Complete bounded guest value for abb_rapid_msgs/RobTarget. */
using Ros1Abb_rapid_msgsRobTarget=roswasmc_bounded_1_abb_rapid_msgs_RobTarget;
/** Complete bounded guest value for abb_rapid_msgs/ToolData. */
using Ros1Abb_rapid_msgsToolData=roswasmc_bounded_1_abb_rapid_msgs_ToolData;
/** Complete bounded guest value for abb_rapid_msgs/WObjData. */
using Ros1Abb_rapid_msgsWObjData=roswasmc_bounded_1_abb_rapid_msgs_WObjData;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/EGMActivateSettings. */
using Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/EGMRunSettings. */
using Ros1Abb_rapid_sm_addin_msgsEGMRunSettings=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/EGMSetupSettings. */
using Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/EGMStopSettings. */
using Ros1Abb_rapid_sm_addin_msgsEGMStopSettings=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/EGMSettings. */
using Ros1Abb_rapid_sm_addin_msgsEGMSettings=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/GetEGMSettingsRequest. */
using Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/GetEGMSettingsResponse. */
using Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/StateMachineState. */
using Ros1Abb_rapid_sm_addin_msgsStateMachineState=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/RuntimeState. */
using Ros1Abb_rapid_sm_addin_msgsRuntimeState=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/SetEGMSettingsRequest. */
using Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/SetEGMSettingsResponse. */
using Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/SetRAPIDRoutineRequest. */
using Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/SetRAPIDRoutineResponse. */
using Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/SetSGCommandRequest. */
using Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest;
/** Complete bounded guest value for abb_rapid_sm_addin_msgs/SetSGCommandResponse. */
using Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse;
/** Complete bounded guest value for abb_robot_msgs/GetFileContentsRequest. */
using Ros1Abb_robot_msgsGetFileContentsRequest=roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest;
/** Complete bounded guest value for abb_robot_msgs/GetFileContentsResponse. */
using Ros1Abb_robot_msgsGetFileContentsResponse=roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse;
/** Complete bounded guest value for abb_robot_msgs/GetIOSignalRequest. */
using Ros1Abb_robot_msgsGetIOSignalRequest=roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest;
/** Complete bounded guest value for abb_robot_msgs/GetIOSignalResponse. */
using Ros1Abb_robot_msgsGetIOSignalResponse=roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse;
/** Complete bounded guest value for abb_robot_msgs/RAPIDSymbolPath. */
using Ros1Abb_robot_msgsRAPIDSymbolPath=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDBoolRequest. */
using Ros1Abb_robot_msgsGetRAPIDBoolRequest=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDBoolResponse. */
using Ros1Abb_robot_msgsGetRAPIDBoolResponse=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDDnumRequest. */
using Ros1Abb_robot_msgsGetRAPIDDnumRequest=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDDnumResponse. */
using Ros1Abb_robot_msgsGetRAPIDDnumResponse=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDNumRequest. */
using Ros1Abb_robot_msgsGetRAPIDNumRequest=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDNumResponse. */
using Ros1Abb_robot_msgsGetRAPIDNumResponse=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDStringRequest. */
using Ros1Abb_robot_msgsGetRAPIDStringRequest=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDStringResponse. */
using Ros1Abb_robot_msgsGetRAPIDStringResponse=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDSymbolRequest. */
using Ros1Abb_robot_msgsGetRAPIDSymbolRequest=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest;
/** Complete bounded guest value for abb_robot_msgs/GetRAPIDSymbolResponse. */
using Ros1Abb_robot_msgsGetRAPIDSymbolResponse=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse;
/** Complete bounded guest value for abb_robot_msgs/GetRobotControllerDescriptionRequest. */
using Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest=roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest;
/** Complete bounded guest value for abb_robot_msgs/GetRobotControllerDescriptionResponse. */
using Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse=roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse;
/** Complete bounded guest value for abb_robot_msgs/GetSpeedRatioRequest. */
using Ros1Abb_robot_msgsGetSpeedRatioRequest=roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest;
/** Complete bounded guest value for abb_robot_msgs/GetSpeedRatioResponse. */
using Ros1Abb_robot_msgsGetSpeedRatioResponse=roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse;
/** Complete bounded guest value for abb_robot_msgs/MechanicalUnitState. */
using Ros1Abb_robot_msgsMechanicalUnitState=roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState;
/** Complete bounded guest value for abb_robot_msgs/RAPIDTaskState. */
using Ros1Abb_robot_msgsRAPIDTaskState=roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState;
/** Complete bounded guest value for abb_robot_msgs/ServiceResponses. */
using Ros1Abb_robot_msgsServiceResponses=roswasmc_bounded_1_abb_robot_msgs_ServiceResponses;
/** Complete bounded guest value for abb_robot_msgs/SetFileContentsRequest. */
using Ros1Abb_robot_msgsSetFileContentsRequest=roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest;
/** Complete bounded guest value for abb_robot_msgs/SetFileContentsResponse. */
using Ros1Abb_robot_msgsSetFileContentsResponse=roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse;
/** Complete bounded guest value for abb_robot_msgs/SetIOSignalRequest. */
using Ros1Abb_robot_msgsSetIOSignalRequest=roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest;
/** Complete bounded guest value for abb_robot_msgs/SetIOSignalResponse. */
using Ros1Abb_robot_msgsSetIOSignalResponse=roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDBoolRequest. */
using Ros1Abb_robot_msgsSetRAPIDBoolRequest=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDBoolResponse. */
using Ros1Abb_robot_msgsSetRAPIDBoolResponse=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDDnumRequest. */
using Ros1Abb_robot_msgsSetRAPIDDnumRequest=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDDnumResponse. */
using Ros1Abb_robot_msgsSetRAPIDDnumResponse=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDNumRequest. */
using Ros1Abb_robot_msgsSetRAPIDNumRequest=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDNumResponse. */
using Ros1Abb_robot_msgsSetRAPIDNumResponse=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDStringRequest. */
using Ros1Abb_robot_msgsSetRAPIDStringRequest=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDStringResponse. */
using Ros1Abb_robot_msgsSetRAPIDStringResponse=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDSymbolRequest. */
using Ros1Abb_robot_msgsSetRAPIDSymbolRequest=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest;
/** Complete bounded guest value for abb_robot_msgs/SetRAPIDSymbolResponse. */
using Ros1Abb_robot_msgsSetRAPIDSymbolResponse=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse;
/** Complete bounded guest value for abb_robot_msgs/SetSpeedRatioRequest. */
using Ros1Abb_robot_msgsSetSpeedRatioRequest=roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest;
/** Complete bounded guest value for abb_robot_msgs/SetSpeedRatioResponse. */
using Ros1Abb_robot_msgsSetSpeedRatioResponse=roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse;
/** Complete bounded guest value for abb_robot_msgs/SystemState. */
using Ros1Abb_robot_msgsSystemState=roswasmc_bounded_1_abb_robot_msgs_SystemState;
/** Complete bounded guest value for abb_robot_msgs/TriggerWithResultCodeRequest. */
using Ros1Abb_robot_msgsTriggerWithResultCodeRequest=roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest;
/** Complete bounded guest value for abb_robot_msgs/TriggerWithResultCodeResponse. */
using Ros1Abb_robot_msgsTriggerWithResultCodeResponse=roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse;
/** Complete bounded guest value for duration. */
using Ros1Duration=roswasmc_bounded_1_duration;
/** Complete bounded guest value for trajectory_msgs/JointTrajectoryPoint. */
using Ros1Trajectory_msgsJointTrajectoryPoint=roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint;
/** Complete bounded guest value for trajectory_msgs/JointTrajectory. */
using Ros1Trajectory_msgsJointTrajectory=roswasmc_bounded_1_trajectory_msgs_JointTrajectory;
/** Complete bounded guest value for control_msgs/JointTolerance. */
using Ros1Control_msgsJointTolerance=roswasmc_bounded_1_control_msgs_JointTolerance;
/** Complete bounded guest value for control_msgs/FollowJointTrajectoryGoal. */
using Ros1Control_msgsFollowJointTrajectoryGoal=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal;
/** Complete bounded guest value for control_msgs/FollowJointTrajectoryResult. */
using Ros1Control_msgsFollowJointTrajectoryResult=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult;
/** Complete bounded guest value for control_msgs/FollowJointTrajectoryFeedback. */
using Ros1Control_msgsFollowJointTrajectoryFeedback=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback;
/** Complete bounded guest value for sensor_msgs/JointState. */
using Ros1Sensor_msgsJointState=roswasmc_bounded_1_sensor_msgs_JointState;
}
namespace roswasmcpp {
/** Checked codec for abb_egm_msgs/EGMChannelState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_egm_msgsEGMChannelState> {
  static constexpr size_t max_wire_bytes=81;
  static constexpr char const* codec_identity="UWVMROS1_452d2adde3b1475fe3ff09ea11ff2ec7105a58c32e20d764e0c2ff152cb432f2";
  static bool encode(uwvm_custom::Ros1Abb_egm_msgsEGMChannelState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_egm_msgsEGMChannelState& out) {return !roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_decode(bytes.data(),bytes.size(),&out);}
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
  static constexpr size_t max_wire_bytes=80;
  static constexpr char const* codec_identity="UWVMROS1_ca1b8ffce276bdc4f86dccae58ede9f860ffe8778c6d4ffe0fb70675777801db";
  static bool encode(uwvm_custom::Ros1Std_msgsHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_std_msgs_Header_valid(&value))return false;size_t n=roswasmc_bounded_1_std_msgs_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_std_msgs_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Std_msgsHeader& out) {return !roswasmc_bounded_1_std_msgs_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_egm_msgs/EGMState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_egm_msgsEGMState> {
  static constexpr size_t max_wire_bytes=1380;
  static constexpr char const* codec_identity="UWVMROS1_f7fe1d3924dc4e497c58aab27da69cfa6ab1a11f64b970b42e2d040a1312c4d8";
  static bool encode(uwvm_custom::Ros1Abb_egm_msgsEGMState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_egm_msgs_EGMState_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_egm_msgs_EGMState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_egm_msgs_EGMState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_egm_msgsEGMState& out) {return !roswasmc_bounded_1_abb_egm_msgs_EGMState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/ConfData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsConfData> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_293eb01e8fa150a8a3c741626b76804255d0eb321a0484aac402201447392af2";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsConfData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_ConfData_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_ConfData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_ConfData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsConfData& out) {return !roswasmc_bounded_1_abb_rapid_msgs_ConfData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/ExtJoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsExtJoint> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_653a47c44ab85151fd9e6215901fb154962c32fe1082386733ec6205eaae613a";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsExtJoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsExtJoint& out) {return !roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/RobJoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsRobJoint> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_5d45bc0efae8c4f5b26e4043fe537e163c4cc4c6458d1eac9be429c37aaf82f0";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsRobJoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_RobJoint_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_RobJoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_RobJoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsRobJoint& out) {return !roswasmc_bounded_1_abb_rapid_msgs_RobJoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/JointTarget; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsJointTarget> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_4a1cc175468da9204c5f2f399b76712617daec21a0d1675a6e73c84ff170dc24";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsJointTarget const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_JointTarget_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_JointTarget_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_JointTarget_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsJointTarget& out) {return !roswasmc_bounded_1_abb_rapid_msgs_JointTarget_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/Pos; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsPos> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_79edd4553fad8a8f833a3399e354c93538d4971bc3820cc14cf540e960356c01";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsPos const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_Pos_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_Pos_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_Pos_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsPos& out) {return !roswasmc_bounded_1_abb_rapid_msgs_Pos_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/Orient; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsOrient> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_5a9c29323a5e3bc0c4c0afa7d305738cd52b49ae339e72f6b34b35b165f3d270";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsOrient const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_Orient_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_Orient_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_Orient_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsOrient& out) {return !roswasmc_bounded_1_abb_rapid_msgs_Orient_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/LoadData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsLoadData> {
  static constexpr size_t max_wire_bytes=44;
  static constexpr char const* codec_identity="UWVMROS1_2cc985cc188a03e7c6de3d22310802cac1e0a748bae7f828d957436c1e03303d";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsLoadData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_LoadData_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_LoadData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_LoadData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsLoadData& out) {return !roswasmc_bounded_1_abb_rapid_msgs_LoadData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/Pose; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsPose> {
  static constexpr size_t max_wire_bytes=28;
  static constexpr char const* codec_identity="UWVMROS1_3e0cfbb7e25376639d1ff6e555a86e69f8c9ed177c1ef14b301d90b206a19c99";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsPose const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_Pose_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsPose& out) {return !roswasmc_bounded_1_abb_rapid_msgs_Pose_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/RobTarget; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsRobTarget> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_0abb3aee8b1303ccf9419f2774829d1231de3ac033e4b644d585f195972f09ce";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsRobTarget const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_RobTarget_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_RobTarget_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_RobTarget_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsRobTarget& out) {return !roswasmc_bounded_1_abb_rapid_msgs_RobTarget_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/ToolData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsToolData> {
  static constexpr size_t max_wire_bytes=73;
  static constexpr char const* codec_identity="UWVMROS1_e2feaee4bda891f594542384d70883184262777e359fb037a648b7526ef76cba";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsToolData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_ToolData_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_ToolData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_ToolData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsToolData& out) {return !roswasmc_bounded_1_abb_rapid_msgs_ToolData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_msgs/WObjData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_msgsWObjData> {
  static constexpr size_t max_wire_bytes=574;
  static constexpr char const* codec_identity="UWVMROS1_6de4cf654a15ef3b63d9b0f3d0984438f21963c10ffd1b00238113b6fae8454f";
  static bool encode(uwvm_custom::Ros1Abb_rapid_msgsWObjData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_msgs_WObjData_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_msgs_WObjData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_msgs_WObjData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_msgsWObjData& out) {return !roswasmc_bounded_1_abb_rapid_msgs_WObjData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/EGMActivateSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings> {
  static constexpr size_t max_wire_bytes=716;
  static constexpr char const* codec_identity="UWVMROS1_c50be5682021e5d1a7a938e6fe3595cec7c0b0ae727070c0924da731c2c73d4d";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/EGMRunSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMRunSettings> {
  static constexpr size_t max_wire_bytes=40;
  static constexpr char const* codec_identity="UWVMROS1_09a843da7aa89c60e6404f8d36b9a1a57f75e330329ac043449fd438b7029506";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMRunSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMRunSettings& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/EGMSetupSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_ece31a7631bb3bb248aece52d4d5f6e190a94dcc3052b2761b759bed24513779";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/EGMStopSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMStopSettings> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_64009cc452b9b2b12b3501d150e8c88c9cc07c796df06775195fa17c2a103a08";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMStopSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMStopSettings& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/EGMSettings; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMSettings> {
  static constexpr size_t max_wire_bytes=767;
  static constexpr char const* codec_identity="UWVMROS1_ebc0ca24a54451f863964c74bf0257b5b25546d0d4b8a75ac75c265e1d62317b";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMSettings const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMSettings& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/GetEGMSettingsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_42cca857367e08720f989416d18d6a1f7d677f7e5bf49435b9735a9eb10a8f9c";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/GetEGMSettingsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse> {
  static constexpr size_t max_wire_bytes=1285;
  static constexpr char const* codec_identity="UWVMROS1_094fd8b0b992c26d708bbd09952d36a3921ad5e5e66f45edb9d19af5ec3472bc";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/StateMachineState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsStateMachineState> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_9ef8cc3a0438ad892acc1a2f485039ca49abce6fedf40cc5d9b2fe03dd9490bb";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsStateMachineState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsStateMachineState& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/RuntimeState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsRuntimeState> {
  static constexpr size_t max_wire_bytes=8372;
  static constexpr char const* codec_identity="UWVMROS1_50d7eab2eed3ce0b55160e85b44b4108d966290f6b8f126980ee452846f3660c";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsRuntimeState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsRuntimeState& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/SetEGMSettingsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest> {
  static constexpr size_t max_wire_bytes=835;
  static constexpr char const* codec_identity="UWVMROS1_fe85c34f8dff2c0a547f89385d75771af13a03d997b880ab7be2eb84ea800155";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/SetEGMSettingsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_c0d4e594a51a6cdd245ad03b6e27bbdc298c5c9fbe26cc25fa3594deef9b2591";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/SetRAPIDRoutineRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest> {
  static constexpr size_t max_wire_bytes=584;
  static constexpr char const* codec_identity="UWVMROS1_b16ba3e98ee1d45864532b01f914746f041e49463b9330ef6056e427d199a5a3";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/SetRAPIDRoutineResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_44de2385f12a8612fa75ca7c3f97b07f22d04e920bdeca0f0bd7fed5322c680e";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/SetSGCommandRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest> {
  static constexpr size_t max_wire_bytes=73;
  static constexpr char const* codec_identity="UWVMROS1_e39d7fee66c1efb9ea963216823f6e10bd632d78634d8c47f3283a1a1779f0c2";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_rapid_sm_addin_msgs/SetSGCommandResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_2f22b2098d86dd0764277247eb7f64dccecebf0cddce85654b01c1928eb9c192";
  static bool encode(uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse& out) {return !roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetFileContentsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetFileContentsRequest> {
  static constexpr size_t max_wire_bytes=516;
  static constexpr char const* codec_identity="UWVMROS1_75f5802b385cc1efb524fadea657fda1d5cc4298c5720222d33cd3dd39656f21";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetFileContentsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetFileContentsRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetFileContentsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetFileContentsResponse> {
  static constexpr size_t max_wire_bytes=4618;
  static constexpr char const* codec_identity="UWVMROS1_693b53ed168f81a7c93ac8083977c4f8bd6f4a3999d079f5b47029699ba68fdb";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetFileContentsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetFileContentsResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetIOSignalRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetIOSignalRequest> {
  static constexpr size_t max_wire_bytes=516;
  static constexpr char const* codec_identity="UWVMROS1_d98f7577756b476cac0dd325d45f333fcd2b735df7dce58fb660790b5a91f83b";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetIOSignalRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetIOSignalRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetIOSignalResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetIOSignalResponse> {
  static constexpr size_t max_wire_bytes=1034;
  static constexpr char const* codec_identity="UWVMROS1_10a66146c4815b0d758ec81dde1c552695c97a8727ee995299018b23f129d79e";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetIOSignalResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetIOSignalResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/RAPIDSymbolPath; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsRAPIDSymbolPath> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_07308d9ce1e71a02f5cc3546cdbaaa0b24a06534a55aa3ee3354b498038faad2";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsRAPIDSymbolPath const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsRAPIDSymbolPath& out) {return !roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDBoolRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolRequest> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_46e49c0d6796175d0c56460dd4c7d2557deaa8db4f1a3bec40b53636d3b44b96";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDBoolResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolResponse> {
  static constexpr size_t max_wire_bytes=519;
  static constexpr char const* codec_identity="UWVMROS1_bfe21e95cb79f1d9bc1bc1dc2f61c744a2a7dc49e5a5ed07b6c8619344b97b3f";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDDnumRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumRequest> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_9058d8774de0da657344a00d75a7f75878da2b0453bf25708bc8ed7f45c90c9d";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDDnumResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumResponse> {
  static constexpr size_t max_wire_bytes=526;
  static constexpr char const* codec_identity="UWVMROS1_9c209b4b12d733e33b76cc33ae93b6d561ce1eb7f8864429a56bd073317a01b3";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDNumRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumRequest> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_a316395c8a19b7c0a9c3a555e625fffc7d647c7b567f38c53b1c3735b50ee910";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDNumResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumResponse> {
  static constexpr size_t max_wire_bytes=522;
  static constexpr char const* codec_identity="UWVMROS1_1573cba9a9046a6b304a49e3a77266fa0d6f615d2ea9a24692c25616970fd9b0";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDStringRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringRequest> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_56514142e9e064aa43d09078b49ed8ee0366189727c35a9bc130b35007da4c9c";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDStringResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringResponse> {
  static constexpr size_t max_wire_bytes=1034;
  static constexpr char const* codec_identity="UWVMROS1_d1188827af3c307a721f8128b6f14fa6020f3187afce8a4162ce6562cdd1c854";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDSymbolRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolRequest> {
  static constexpr size_t max_wire_bytes=204;
  static constexpr char const* codec_identity="UWVMROS1_31302ad912a8ff39b56a429bc9dae22cbfc2c2101b8cd05162d00d9206aecf5b";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRAPIDSymbolResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolResponse> {
  static constexpr size_t max_wire_bytes=1034;
  static constexpr char const* codec_identity="UWVMROS1_182dc465ea5fb9543791b6fb76520313cee1ed472ab0296c8ef7a9ed3d4d110f";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRobotControllerDescriptionRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_bc5f87f4d0f33cbc640c8188fbb15c9b0f43b3067f2406700d6f5bd2475089b6";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetRobotControllerDescriptionResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse> {
  static constexpr size_t max_wire_bytes=4618;
  static constexpr char const* codec_identity="UWVMROS1_7db41283fbe12a010e526b87d77a5bb7faecdf85595ef2dc13f14bfed56b6e39";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetSpeedRatioRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_82230aebf6fa923e09bcd413ecb3b4803a6f1cfd3e8334e0ce0c496b73008373";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/GetSpeedRatioResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioResponse> {
  static constexpr size_t max_wire_bytes=519;
  static constexpr char const* codec_identity="UWVMROS1_7784360ddafa1b8ad9626fde0afa2b2335f6a294507036c3aee953df99e91940";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/MechanicalUnitState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsMechanicalUnitState> {
  static constexpr size_t max_wire_bytes=69;
  static constexpr char const* codec_identity="UWVMROS1_872cc6e8bdf32426bb9e24322c951452aab6ebcbc1f2998f4fb19c34abece8fa";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsMechanicalUnitState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsMechanicalUnitState& out) {return !roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/RAPIDTaskState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsRAPIDTaskState> {
  static constexpr size_t max_wire_bytes=71;
  static constexpr char const* codec_identity="UWVMROS1_ab1245cc352efa91cdcc8de801392857e25a4101a141439554b761525b31b947";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsRAPIDTaskState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsRAPIDTaskState& out) {return !roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/ServiceResponses; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsServiceResponses> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_f0a3c4b160bf0ca52dbbb4e26d2c8a7d3c6a40ac70787ba59cf181945e4276d4";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsServiceResponses const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsServiceResponses& out) {return !roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetFileContentsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetFileContentsRequest> {
  static constexpr size_t max_wire_bytes=4616;
  static constexpr char const* codec_identity="UWVMROS1_f764448d9754e9ebc5e5fab64f0c8ebb01c534ec7b95751ed7d58e8ce0bd3012";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetFileContentsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetFileContentsRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetFileContentsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetFileContentsResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_3de2a08b8e158e0612898880d6d9c98f1995828eb53bd5639b47f730c6cbefd0";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetFileContentsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetFileContentsResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetIOSignalRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetIOSignalRequest> {
  static constexpr size_t max_wire_bytes=1032;
  static constexpr char const* codec_identity="UWVMROS1_57a0197e2a5de7efd2fc91d945d699be24559baec6651179a1256f1d9d35bbc9";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetIOSignalRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetIOSignalRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetIOSignalResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetIOSignalResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_becc0f844ce6a7fabd8a683175edd8033ba5c943c2de152991755ee112b460e5";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetIOSignalResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetIOSignalResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDBoolRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolRequest> {
  static constexpr size_t max_wire_bytes=205;
  static constexpr char const* codec_identity="UWVMROS1_d8e5420678fe5feeffa20ad9ab8431678ec346f764c94871c4496c62e9dd8e8d";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDBoolResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_da2ae1231cb0c1ec452fb2733d5c99dcae78ad0bfd10771241708931b271bf13";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDDnumRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumRequest> {
  static constexpr size_t max_wire_bytes=212;
  static constexpr char const* codec_identity="UWVMROS1_0f17e3068f86bca6effb99f3bc850234b0163028f372340c450fce31657b9cf9";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDDnumResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_1905aed9683e03494baa54627ddbaf4aed0b92357823c0c042f9b7c6a970c8ed";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDNumRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumRequest> {
  static constexpr size_t max_wire_bytes=208;
  static constexpr char const* codec_identity="UWVMROS1_9982d02cb0b6ee5813f5fb38072d665226b7885aeb90517af881f4fc990f392f";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDNumResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_af1181242b4afdb6d387d39c24d0bc343e872fb9ab21c0ec1e451785786a4e64";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDStringRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringRequest> {
  static constexpr size_t max_wire_bytes=720;
  static constexpr char const* codec_identity="UWVMROS1_e06839324f8abfa8780b683386f87bab1ec04f1e6463e63c0599f5c2f8a75f0c";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDStringResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_e175bdff5a150722b8c50cf2e1c0f28a99289a0b148339efdee4b034ea3b96b4";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDSymbolRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolRequest> {
  static constexpr size_t max_wire_bytes=720;
  static constexpr char const* codec_identity="UWVMROS1_0654a5a8e5ec42f4feab54b086a008e02a74d44df65d2fcf60f007736e3fec7d";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetRAPIDSymbolResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_a4ca0a8821337d1f16ba1f2ad2b232aceaba419308ca03b27f43dac427a32846";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetSpeedRatioRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_a6ac368d243d488901b03d5e4303600d5f0eaf62150801103e5f5f9c0863fab5";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SetSpeedRatioResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_9775dc1029993d1dfdf5f3080e6ae4262177ba96cdc0fbe9851dc4fea16c70d8";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/SystemState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsSystemState> {
  static constexpr size_t max_wire_bytes=2331;
  static constexpr char const* codec_identity="UWVMROS1_b57aa1fdf098aa78c51a96bd2978e02f97cbae67148c0028557d2a3e1e8630f2";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsSystemState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_SystemState_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_SystemState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_SystemState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsSystemState& out) {return !roswasmc_bounded_1_abb_robot_msgs_SystemState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/TriggerWithResultCodeRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_999f63504788e522f7d0108a895cf35cd101e9a50856e480ae9edabf791bcf94";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeRequest& out) {return !roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for abb_robot_msgs/TriggerWithResultCodeResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeResponse> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_4ef5c987592e849766b82b7fd99a65b7a1e17605f3e3341c99803181887967ff";
  static bool encode(uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeResponse& out) {return !roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for duration; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Duration> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_8dbec6cdfef79495208abb358c5e8283ec8d9d471f984424afcda997d142cca8";
  static bool encode(uwvm_custom::Ros1Duration const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_duration_valid(&value))return false;size_t n=roswasmc_bounded_1_duration_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_duration_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Duration& out) {return !roswasmc_bounded_1_duration_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/JointTrajectoryPoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Trajectory_msgsJointTrajectoryPoint> {
  static constexpr size_t max_wire_bytes=408;
  static constexpr char const* codec_identity="UWVMROS1_9773572fff8616235c4d7e23fccbe01f28dcfa62c7c80323a65c8c305feba3c5";
  static bool encode(uwvm_custom::Ros1Trajectory_msgsJointTrajectoryPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_valid(&value))return false;size_t n=roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Trajectory_msgsJointTrajectoryPoint& out) {return !roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/JointTrajectory; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Trajectory_msgsJointTrajectory> {
  static constexpr size_t max_wire_bytes=4168;
  static constexpr char const* codec_identity="UWVMROS1_079e6c432f0a858d6bd47c87aaf353a8b0676706809e0f984705b1a688de282b";
  static bool encode(uwvm_custom::Ros1Trajectory_msgsJointTrajectory const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectory_valid(&value))return false;size_t n=roswasmc_bounded_1_trajectory_msgs_JointTrajectory_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_trajectory_msgs_JointTrajectory_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Trajectory_msgsJointTrajectory& out) {return !roswasmc_bounded_1_trajectory_msgs_JointTrajectory_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/JointTolerance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Control_msgsJointTolerance> {
  static constexpr size_t max_wire_bytes=92;
  static constexpr char const* codec_identity="UWVMROS1_c9f71f98726d4a6621dcc8db96a394a24941a79d9041c8ad7a07724ee64d3c88";
  static bool encode(uwvm_custom::Ros1Control_msgsJointTolerance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_control_msgs_JointTolerance_valid(&value))return false;size_t n=roswasmc_bounded_1_control_msgs_JointTolerance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_control_msgs_JointTolerance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Control_msgsJointTolerance& out) {return !roswasmc_bounded_1_control_msgs_JointTolerance_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/FollowJointTrajectoryGoal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Control_msgsFollowJointTrajectoryGoal> {
  static constexpr size_t max_wire_bytes=6392;
  static constexpr char const* codec_identity="UWVMROS1_e9fda6f9e732450f2cf1af046ceb67815e6e1a22105eda2fee38f8f470252d40";
  static bool encode(uwvm_custom::Ros1Control_msgsFollowJointTrajectoryGoal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_valid(&value))return false;size_t n=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Control_msgsFollowJointTrajectoryGoal& out) {return !roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/FollowJointTrajectoryResult; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Control_msgsFollowJointTrajectoryResult> {
  static constexpr size_t max_wire_bytes=4104;
  static constexpr char const* codec_identity="UWVMROS1_71da832be83c08804668aa63576768bbe831f21f5f270cdc8d851b30d2fc4281";
  static bool encode(uwvm_custom::Ros1Control_msgsFollowJointTrajectoryResult const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_valid(&value))return false;size_t n=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Control_msgsFollowJointTrajectoryResult& out) {return !roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for control_msgs/FollowJointTrajectoryFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Control_msgsFollowJointTrajectoryFeedback> {
  static constexpr size_t max_wire_bytes=2124;
  static constexpr char const* codec_identity="UWVMROS1_a1ad5a1f8a00b60f97d06209737993bd395d83d1a008705d65193b63933b62a8";
  static bool encode(uwvm_custom::Ros1Control_msgsFollowJointTrajectoryFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_valid(&value))return false;size_t n=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Control_msgsFollowJointTrajectoryFeedback& out) {return !roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for sensor_msgs/JointState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Sensor_msgsJointState> {
  static constexpr size_t max_wire_bytes=1200;
  static constexpr char const* codec_identity="UWVMROS1_8d1bb9c4903dd4b070a5ab157b3590815722d6b574d610c9b9fb5ead8be7a057";
  static bool encode(uwvm_custom::Ros1Sensor_msgsJointState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_sensor_msgs_JointState_valid(&value))return false;size_t n=roswasmc_bounded_1_sensor_msgs_JointState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_sensor_msgs_JointState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Sensor_msgsJointState& out) {return !roswasmc_bounded_1_sensor_msgs_JointState_decode(bytes.data(),bytes.size(),&out);}
};
}
