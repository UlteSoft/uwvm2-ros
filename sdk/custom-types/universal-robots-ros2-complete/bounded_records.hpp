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
/** Complete bounded guest value for control_msgs/msg/JointTolerance. */
using Ros2Control_msgsMsgJointTolerance=roswasmc_bounded_2_control_msgs_msg_JointTolerance;
/** Complete bounded guest value for geometry_msgs/msg/Point. */
using Ros2Geometry_msgsMsgPoint=roswasmc_bounded_2_geometry_msgs_msg_Point;
/** Complete bounded guest value for geometry_msgs/msg/Quaternion. */
using Ros2Geometry_msgsMsgQuaternion=roswasmc_bounded_2_geometry_msgs_msg_Quaternion;
/** Complete bounded guest value for geometry_msgs/msg/Pose. */
using Ros2Geometry_msgsMsgPose=roswasmc_bounded_2_geometry_msgs_msg_Pose;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for geometry_msgs/msg/PoseStamped. */
using Ros2Geometry_msgsMsgPoseStamped=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped;
/** Complete bounded guest value for geometry_msgs/msg/Vector3. */
using Ros2Geometry_msgsMsgVector3=roswasmc_bounded_2_geometry_msgs_msg_Vector3;
/** Complete bounded guest value for geometry_msgs/msg/Twist. */
using Ros2Geometry_msgsMsgTwist=roswasmc_bounded_2_geometry_msgs_msg_Twist;
/** Complete bounded guest value for geometry_msgs/msg/Vector3Stamped. */
using Ros2Geometry_msgsMsgVector3Stamped=roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped;
/** Complete bounded guest value for geometry_msgs/msg/Wrench. */
using Ros2Geometry_msgsMsgWrench=roswasmc_bounded_2_geometry_msgs_msg_Wrench;
/** Complete bounded guest value for trajectory_msgs/msg/JointTrajectoryPoint. */
using Ros2Trajectory_msgsMsgJointTrajectoryPoint=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint;
/** Complete bounded guest value for trajectory_msgs/msg/JointTrajectory. */
using Ros2Trajectory_msgsMsgJointTrajectory=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory;
/** Complete bounded guest value for ur_dashboard_msgs/action/SetMode_Goal. */
using Ros2Ur_dashboard_msgsActionSetMode_Goal=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal;
/** Complete bounded guest value for ur_dashboard_msgs/action/SetMode_Result. */
using Ros2Ur_dashboard_msgsActionSetMode_Result=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result;
/** Complete bounded guest value for ur_dashboard_msgs/action/SetMode_Feedback. */
using Ros2Ur_dashboard_msgsActionSetMode_Feedback=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback;
/** Complete bounded guest value for ur_dashboard_msgs/msg/OperationalMode. */
using Ros2Ur_dashboard_msgsMsgOperationalMode=roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode;
/** Complete bounded guest value for ur_dashboard_msgs/msg/ProgramInformation. */
using Ros2Ur_dashboard_msgsMsgProgramInformation=roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation;
/** Complete bounded guest value for ur_dashboard_msgs/msg/ProgramState. */
using Ros2Ur_dashboard_msgsMsgProgramState=roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState;
/** Complete bounded guest value for ur_dashboard_msgs/msg/RobotMode. */
using Ros2Ur_dashboard_msgsMsgRobotMode=roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode;
/** Complete bounded guest value for ur_dashboard_msgs/msg/SafetyMode. */
using Ros2Ur_dashboard_msgsMsgSafetyMode=roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode;
/** Complete bounded guest value for ur_dashboard_msgs/msg/SafetyStatus. */
using Ros2Ur_dashboard_msgsMsgSafetyStatus=roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus;
/** Complete bounded guest value for ur_dashboard_msgs/msg/UserRole. */
using Ros2Ur_dashboard_msgsMsgUserRole=roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole;
/** Complete bounded guest value for ur_dashboard_msgs/msg/VersionInformation. */
using Ros2Ur_dashboard_msgsMsgVersionInformation=roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation;
/** Complete bounded guest value for ur_dashboard_msgs/srv/AddToLog_Request. */
using Ros2Ur_dashboard_msgsSrvAddToLog_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/AddToLog_Response. */
using Ros2Ur_dashboard_msgsSrvAddToLog_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/DownloadProgram_Request. */
using Ros2Ur_dashboard_msgsSrvDownloadProgram_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/DownloadProgram_Response. */
using Ros2Ur_dashboard_msgsSrvDownloadProgram_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/DownloadSupportFile_Request. */
using Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/DownloadSupportFile_Response. */
using Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GenerateFlightReport_Request. */
using Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GenerateFlightReport_Response. */
using Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GenerateSupportFile_Request. */
using Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GenerateSupportFile_Response. */
using Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetLoadedProgram_Request. */
using Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetLoadedProgram_Response. */
using Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetOperationalMode_Request. */
using Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetOperationalMode_Response. */
using Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetPolyScopeVersion_Request. */
using Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetPolyScopeVersion_Response. */
using Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetProgramState_Request. */
using Ros2Ur_dashboard_msgsSrvGetProgramState_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetProgramState_Response. */
using Ros2Ur_dashboard_msgsSrvGetProgramState_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetPrograms_Request. */
using Ros2Ur_dashboard_msgsSrvGetPrograms_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetPrograms_Response. */
using Ros2Ur_dashboard_msgsSrvGetPrograms_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetRobotMode_Request. */
using Ros2Ur_dashboard_msgsSrvGetRobotMode_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetRobotMode_Response. */
using Ros2Ur_dashboard_msgsSrvGetRobotMode_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetRobotModel_Request. */
using Ros2Ur_dashboard_msgsSrvGetRobotModel_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetRobotModel_Response. */
using Ros2Ur_dashboard_msgsSrvGetRobotModel_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetSafetyMode_Request. */
using Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetSafetyMode_Response. */
using Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetSafetyStatus_Request. */
using Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetSafetyStatus_Response. */
using Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetSerialNumber_Request. */
using Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetSerialNumber_Response. */
using Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetUserRole_Request. */
using Ros2Ur_dashboard_msgsSrvGetUserRole_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/GetUserRole_Response. */
using Ros2Ur_dashboard_msgsSrvGetUserRole_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/IsInRemoteControl_Request. */
using Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/IsInRemoteControl_Response. */
using Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/IsProgramRunning_Request. */
using Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/IsProgramRunning_Response. */
using Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/IsProgramSaved_Request. */
using Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/IsProgramSaved_Response. */
using Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/Load_Request. */
using Ros2Ur_dashboard_msgsSrvLoad_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/Load_Response. */
using Ros2Ur_dashboard_msgsSrvLoad_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/Popup_Request. */
using Ros2Ur_dashboard_msgsSrvPopup_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/Popup_Response. */
using Ros2Ur_dashboard_msgsSrvPopup_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/RawRequest_Request. */
using Ros2Ur_dashboard_msgsSrvRawRequest_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/RawRequest_Response. */
using Ros2Ur_dashboard_msgsSrvRawRequest_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/SetOperationalMode_Request. */
using Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/SetOperationalMode_Response. */
using Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/SetUserRole_Request. */
using Ros2Ur_dashboard_msgsSrvSetUserRole_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/SetUserRole_Response. */
using Ros2Ur_dashboard_msgsSrvSetUserRole_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response;
/** Complete bounded guest value for ur_dashboard_msgs/srv/UploadProgram_Request. */
using Ros2Ur_dashboard_msgsSrvUploadProgram_Request=roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request;
/** Complete bounded guest value for ur_dashboard_msgs/srv/UploadProgram_Response. */
using Ros2Ur_dashboard_msgsSrvUploadProgram_Response=roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response;
/** Complete bounded guest value for ur_msgs/action/FollowJointTrajectoryUntil_Goal. */
using Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal;
/** Complete bounded guest value for ur_msgs/action/FollowJointTrajectoryUntil_Result. */
using Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result;
/** Complete bounded guest value for ur_msgs/action/FollowJointTrajectoryUntil_Feedback. */
using Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback;
/** Complete bounded guest value for ur_msgs/action/SendScript_Goal. */
using Ros2Ur_msgsActionSendScript_Goal=roswasmc_bounded_2_ur_msgs_action_SendScript_Goal;
/** Complete bounded guest value for ur_msgs/action/SendScript_Result. */
using Ros2Ur_msgsActionSendScript_Result=roswasmc_bounded_2_ur_msgs_action_SendScript_Result;
/** Complete bounded guest value for ur_msgs/action/SendScript_Feedback. */
using Ros2Ur_msgsActionSendScript_Feedback=roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback;
/** Complete bounded guest value for ur_msgs/action/ToolContact_Goal. */
using Ros2Ur_msgsActionToolContact_Goal=roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal;
/** Complete bounded guest value for ur_msgs/action/ToolContact_Result. */
using Ros2Ur_msgsActionToolContact_Result=roswasmc_bounded_2_ur_msgs_action_ToolContact_Result;
/** Complete bounded guest value for ur_msgs/action/ToolContact_Feedback. */
using Ros2Ur_msgsActionToolContact_Feedback=roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback;
/** Complete bounded guest value for ur_msgs/msg/Analog. */
using Ros2Ur_msgsMsgAnalog=roswasmc_bounded_2_ur_msgs_msg_Analog;
/** Complete bounded guest value for ur_msgs/msg/Digital. */
using Ros2Ur_msgsMsgDigital=roswasmc_bounded_2_ur_msgs_msg_Digital;
/** Complete bounded guest value for ur_msgs/msg/FrictionModelParameters. */
using Ros2Ur_msgsMsgFrictionModelParameters=roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters;
/** Complete bounded guest value for ur_msgs/msg/IOStates. */
using Ros2Ur_msgsMsgIOStates=roswasmc_bounded_2_ur_msgs_msg_IOStates;
/** Complete bounded guest value for ur_msgs/msg/MasterboardDataMsg. */
using Ros2Ur_msgsMsgMasterboardDataMsg=roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg;
/** Complete bounded guest value for ur_msgs/msg/RobotModeDataMsg. */
using Ros2Ur_msgsMsgRobotModeDataMsg=roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg;
/** Complete bounded guest value for ur_msgs/msg/RobotStateRTMsg. */
using Ros2Ur_msgsMsgRobotStateRTMsg=roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg;
/** Complete bounded guest value for ur_msgs/msg/ToolDataMsg. */
using Ros2Ur_msgsMsgToolDataMsg=roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg;
/** Complete bounded guest value for ur_msgs/srv/GetRobotSoftwareVersion_Request. */
using Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request=roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request;
/** Complete bounded guest value for ur_msgs/srv/GetRobotSoftwareVersion_Response. */
using Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response=roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response;
/** Complete bounded guest value for ur_msgs/srv/SetAnalogOutput_Request. */
using Ros2Ur_msgsSrvSetAnalogOutput_Request=roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request;
/** Complete bounded guest value for ur_msgs/srv/SetAnalogOutput_Response. */
using Ros2Ur_msgsSrvSetAnalogOutput_Response=roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response;
/** Complete bounded guest value for ur_msgs/srv/SetForceMode_Request. */
using Ros2Ur_msgsSrvSetForceMode_Request=roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request;
/** Complete bounded guest value for ur_msgs/srv/SetForceMode_Response. */
using Ros2Ur_msgsSrvSetForceMode_Response=roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response;
/** Complete bounded guest value for ur_msgs/srv/SetFrictionModelParameters_Request. */
using Ros2Ur_msgsSrvSetFrictionModelParameters_Request=roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request;
/** Complete bounded guest value for ur_msgs/srv/SetFrictionModelParameters_Response. */
using Ros2Ur_msgsSrvSetFrictionModelParameters_Response=roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response;
/** Complete bounded guest value for ur_msgs/srv/SetGravity_Request. */
using Ros2Ur_msgsSrvSetGravity_Request=roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request;
/** Complete bounded guest value for ur_msgs/srv/SetGravity_Response. */
using Ros2Ur_msgsSrvSetGravity_Response=roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response;
/** Complete bounded guest value for ur_msgs/srv/SetIO_Request. */
using Ros2Ur_msgsSrvSetIO_Request=roswasmc_bounded_2_ur_msgs_srv_SetIO_Request;
/** Complete bounded guest value for ur_msgs/srv/SetIO_Response. */
using Ros2Ur_msgsSrvSetIO_Response=roswasmc_bounded_2_ur_msgs_srv_SetIO_Response;
/** Complete bounded guest value for ur_msgs/srv/SetPayload_Request. */
using Ros2Ur_msgsSrvSetPayload_Request=roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request;
/** Complete bounded guest value for ur_msgs/srv/SetPayload_Response. */
using Ros2Ur_msgsSrvSetPayload_Response=roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response;
/** Complete bounded guest value for ur_msgs/srv/SetSpeedSliderFraction_Request. */
using Ros2Ur_msgsSrvSetSpeedSliderFraction_Request=roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request;
/** Complete bounded guest value for ur_msgs/srv/SetSpeedSliderFraction_Response. */
using Ros2Ur_msgsSrvSetSpeedSliderFraction_Response=roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response;
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
/** Checked codec for control_msgs/msg/JointTolerance; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Control_msgsMsgJointTolerance> {
  static constexpr size_t max_wire_bytes=156;
  static constexpr char const* codec_identity="UWVMROS1_3864af88029b3b1f51907b3da63d2d821104c29b9469a146a5a34f73f3598b8a";
  static bool encode(uwvm_custom::Ros2Control_msgsMsgJointTolerance const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value))return false;size_t n=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_control_msgs_msg_JointTolerance_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Control_msgsMsgJointTolerance& out) {return !roswasmc_bounded_2_control_msgs_msg_JointTolerance_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for std_msgs/msg/Header; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_msgsMsgHeader> {
  static constexpr size_t max_wire_bytes=140;
  static constexpr char const* codec_identity="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/PoseStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgPoseStamped> {
  static constexpr size_t max_wire_bytes=196;
  static constexpr char const* codec_identity="UWVMROS1_71abdc844f18f02f45a354967a5f1c644fb20dc14cbc61c64aa41b927e0712a2";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Geometry_msgsMsgPoseStamped default_value() noexcept {uwvm_custom::Ros2Geometry_msgsMsgPoseStamped value={0};(void)roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgPoseStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgPoseStamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for geometry_msgs/msg/Vector3Stamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgVector3Stamped> {
  static constexpr size_t max_wire_bytes=164;
  static constexpr char const* codec_identity="UWVMROS1_c6b67646d3fc6d29c195637ea353dafa7085b2bdb55a15a8b65009200b5ed31b";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgVector3Stamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgVector3Stamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/Wrench; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgWrench> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_fdfdf2474c0129b0ce1f85972cc87fd80478ac7d2429d9de1ec58951a82a11b6";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgWrench const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Wrench_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Wrench_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Wrench_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgWrench& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Wrench_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/msg/JointTrajectoryPoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint> {
  static constexpr size_t max_wire_bytes=216;
  static constexpr char const* codec_identity="UWVMROS1_c4eafb6efd22d299b5c6fc21844d8bf69789cf27ee5414a1d7d2371f0b1acbbf";
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for trajectory_msgs/msg/JointTrajectory; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory> {
  static constexpr size_t max_wire_bytes=5164;
  static constexpr char const* codec_identity="UWVMROS1_e77c37c75123e442edb6a7f6f2a21dcbe33d8bb8fd2577f9906653a6ad5f8225";
  static bool encode(uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(&value))return false;size_t n=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory& out) {return !roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/action/SetMode_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Goal> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_8a7484a60bad21933df3f847d3fd554e41098ed596dee270335ce9f092dc313d";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Goal& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/action/SetMode_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Result> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_ef2a557ff27171319d46170ab16a80f5cba5fbd59e44c21cc1b704d01764b913";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Result& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/action/SetMode_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Feedback> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_afd74cd0a79174b53499a40b23b040ba96ec3d3e332129999525dcf6ca287ac6";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsActionSetMode_Feedback& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/msg/OperationalMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsMsgOperationalMode> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_7a0be8522cdc20e01d382218499030176a0f6565376fadc5bbdd871a01b9316e";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsMsgOperationalMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsMsgOperationalMode& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/msg/ProgramInformation; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsMsgProgramInformation> {
  static constexpr size_t max_wire_bytes=676;
  static constexpr char const* codec_identity="UWVMROS1_9299ce545acd0b6ff064998334820fbe503b71dc47a475ef93545d2cdb41f965";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsMsgProgramInformation const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsMsgProgramInformation& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/msg/ProgramState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsMsgProgramState> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_1462a3c2323792afbbfc6d48a600a6ab66a50a6f6f583e57a62da2458ea81f26";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsMsgProgramState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsMsgProgramState& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/msg/RobotMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsMsgRobotMode> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_81a79c84083520d911de108a728ad1877de78a57ec9e27b5cc432590be4616bb";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsMsgRobotMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsMsgRobotMode& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/msg/SafetyMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsMsgSafetyMode> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_2eb3d5075a2d7d1765a604c1d1d198379010867d83a98d0327488ca51f5e1aac";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsMsgSafetyMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsMsgSafetyMode& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/msg/SafetyStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsMsgSafetyStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1b1a7257c40d9c62e0b6291f241825d029fe38f3f3695265a182a270899a79e9";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsMsgSafetyStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsMsgSafetyStatus& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/msg/UserRole; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsMsgUserRole> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_7568bc9417685a9bc56cfdd2b41ff81ab99bd8e20fbe5f9e537b5b5d0689440a";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsMsgUserRole const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsMsgUserRole& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/msg/VersionInformation; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsMsgVersionInformation> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_450c41c4361f1bca51f864dd9ef596851b17ebc79d4f784e7abb8b721752443d";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsMsgVersionInformation const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsMsgVersionInformation& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/AddToLog_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvAddToLog_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_ecfe5bb0d0db92bc5dfdec9bf82fbe730e3b3b4d65680812d0540b9055590142";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvAddToLog_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvAddToLog_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/AddToLog_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvAddToLog_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_57b0c150a353d3fca6e96c4dc86e72f71b452663ede1eeb69e51069992d07e4c";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvAddToLog_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvAddToLog_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/DownloadProgram_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadProgram_Request> {
  static constexpr size_t max_wire_bytes=520;
  static constexpr char const* codec_identity="UWVMROS1_5c84420393aae13ea448d171a7b2956a55a39f3321f504b9f3bd5b4a38960c2f";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadProgram_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadProgram_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/DownloadProgram_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadProgram_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_46b0a73805282bad5f819f712275b7dd2ca4a6bb8c73c445ea8ae0a3d0f9e8e0";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadProgram_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadProgram_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/DownloadSupportFile_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_d919563629f81d5a8c10f1d2921ea16f3c912a49b8d3bcf65d17b8daa0a38140";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/DownloadSupportFile_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_76e190e6e9f1ddfff3082ea2adb0463d21ba9fb6795153fe6bbacee8a3334667";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GenerateFlightReport_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_7246d97690c0c827e9cfab2f8297cf27a10e60e41ebb3ba345f361a109057099";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request default_value() noexcept {uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request value={0};(void)roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GenerateFlightReport_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_adebf67876b5ae3377cec0aa04138c2ab73ef7c0d7942c77c6c06b248a836445";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GenerateSupportFile_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_10b0371b77db32d75a77dcb0605b325caa1915ccfb7d0917896fd75234e6ada7";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request default_value() noexcept {uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request value={0};(void)roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GenerateSupportFile_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_f370623fe690f218073f8538aa9fcf41c94d4abd3e0507e09607ef11e10edf38";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetLoadedProgram_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_030e5746e6a4da8f266e37d11cc3826715099ff1aa1c845a97042aec987707e3";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetLoadedProgram_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_3e8c7f3c8711faea01de37b9a011f34a9c6dd660cc222e46177b2c1898ec2f7e";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetOperationalMode_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_3f348b5d9e04a8ba01f5277ad259c34a6816ae0693f47c49e348a5497a90a325";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetOperationalMode_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_6c2da335fd551162020eb394b91e99aaeca17e34e7c2e98de935c81912efb5f6";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetPolyScopeVersion_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_88193ae202f0cb02746bc5556e8e8ee277fb4fae72ee4b96a6539c980dc86f13";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetPolyScopeVersion_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response> {
  static constexpr size_t max_wire_bytes=277;
  static constexpr char const* codec_identity="UWVMROS1_c8d53d0a920806e86ceb6d2ac5a692a92e62b057432554bed9dcf57bed0ff9bd";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetProgramState_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetProgramState_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_fe42d1d1aa3d7bdbaa410bce045120d6dd841213ef10c5dd158c2b76fa157f65";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetProgramState_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetProgramState_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetProgramState_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetProgramState_Response> {
  static constexpr size_t max_wire_bytes=781;
  static constexpr char const* codec_identity="UWVMROS1_b20731b8552f323837c5939f390060a6c7c92e9447df5e620891708cfce51ff4";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetProgramState_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetProgramState_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetPrograms_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPrograms_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_8d7b4f3fb9a15854b59c85c5205bf20d3e0f6a1a972ccbdf77ee8377c9e40d6d";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPrograms_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPrograms_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetPrograms_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPrograms_Response> {
  static constexpr size_t max_wire_bytes=11081;
  static constexpr char const* codec_identity="UWVMROS1_111b67c562ba0c77c2924b97058e4126055ad6deb7f99049b8caf8990b3f979c";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPrograms_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetPrograms_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetRobotMode_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotMode_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_d1d9cd94a443b20ec0d41a8db1a6d87291657597abc014e7da0f46a58c6c5614";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotMode_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotMode_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetRobotMode_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotMode_Response> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_67db750ad95b3b207d61b18513a117c868040c22c93638f07fa74645d87bcfba";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotMode_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotMode_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetRobotModel_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotModel_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_9db08f07993bd5786aea5b6bd567f9e9f49329883326ea16e6c49eef1586dc43";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotModel_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotModel_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetRobotModel_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotModel_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_faf0124c542ef7420915527c27e99a7d26c5d6274f55d869bc9416e6b7d9b3c9";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotModel_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetRobotModel_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetSafetyMode_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_5866cc4b0e10508eeb45ceecf9f9b6036ba55d04c9613622e189ccde2781dcb0";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetSafetyMode_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_f65d7ff83dc97f891032d8ab16bf71a9c2326c6ae6a033fe1d6e06f011c5a40b";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetSafetyStatus_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_0e2958bcd7b121682361c94b2926bcc4552d7ef07dc2af4609d04b1bb7054602";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetSafetyStatus_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_bb96aaa96f5c78b97ad80a14eb96865dc10b5a1d12524fcbcc75fc6d481c53e4";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetSerialNumber_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_bbb1ca066c77ed459352de69d566f3ea87b94fe593c31c911193416f92a73c9b";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetSerialNumber_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response> {
  static constexpr size_t max_wire_bytes=269;
  static constexpr char const* codec_identity="UWVMROS1_f389641d089fa6fd421348f047732de2122cd714bdb2bd4d1e611592aca349e7";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetUserRole_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetUserRole_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_a331be09dd2093e4584e6dc80f83caaadd3a8e43cca6a1c8fcfa1e2ea632c9f0";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetUserRole_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetUserRole_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/GetUserRole_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvGetUserRole_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_ed4f53e59fbe4cd33cfbf5d190b60fbe8371dda7da9d7af6e270a882eb142df8";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvGetUserRole_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvGetUserRole_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/IsInRemoteControl_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_4b6b86fc4337c36589e8781b46ddbe6992f750b565ff413ea0b069982d87a463";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/IsInRemoteControl_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_f2705c1be319c7c667f4f2224851b06c24b15b4211c2828fab89f09455e4c816";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/IsProgramRunning_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_f28274c0dc01d8aa2f981f969c05a614e6b3922601d578f99c6321a4b20fc80f";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/IsProgramRunning_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_c57cc7ecd7e81f7359824b43884a58f65748a4c77cacbcc45be17aa9147cf499";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/IsProgramSaved_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_c53d57cffc0480d66257150a8450ba90563fda5153742784831d38abcbc0b099";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/IsProgramSaved_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response> {
  static constexpr size_t max_wire_bytes=522;
  static constexpr char const* codec_identity="UWVMROS1_aa171247f31fd00a1b8820ee21cf20d159bd10d0d2887dc96b3a4dd4d372d115";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/Load_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvLoad_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_1d0cd6cdeaaafce999a7e9924721246b9f4954f22da40fd0a27fb7b657324876";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvLoad_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvLoad_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/Load_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvLoad_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_4e694de8079133a8e789add3640dfe19805cad92991a650167c2aae7067da415";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvLoad_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvLoad_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/Popup_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvPopup_Request> {
  static constexpr size_t max_wire_bytes=520;
  static constexpr char const* codec_identity="UWVMROS1_369e6eab4f689a846c52fcc5cb539da63fab3939fab254dd1e04c7b274d5f770";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvPopup_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvPopup_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/Popup_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvPopup_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_0b54f7c477c1ddf8ccb18d12d50eaf308225d4927ee377377ed354426e58f984";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvPopup_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvPopup_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/RawRequest_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvRawRequest_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_2bf66d87b20ca2de9ed94c04c79042a50239728320b43b27c9942405795b4555";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvRawRequest_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvRawRequest_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/RawRequest_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvRawRequest_Response> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_7c37f54149bab54f6e7bbfed41992eb54416b31346bc889a9632d5497111267f";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvRawRequest_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvRawRequest_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/SetOperationalMode_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_8cc61f7c64211c46e01e52eee2c05f6e709f4f0900cf24fdfd4079b0f7581a00";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/SetOperationalMode_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_f5009de077224371e9d2f6c0aecbabd24c80148f2c7e1b1383a64f6f214b202a";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/SetUserRole_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvSetUserRole_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_7bd499947803efca81401a2de496d24e7e54ba97f4f86ec2b3089146a0655f5e";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvSetUserRole_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvSetUserRole_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/SetUserRole_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvSetUserRole_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_4cab2bba5030f4e4b9e90252d64ec2e02d6eb87070b75cf43190fecd21da59fd";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvSetUserRole_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvSetUserRole_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/UploadProgram_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvUploadProgram_Request> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_f7501d0115f8d36b1942ce610b1bbc197d0276de508ac8175d539f40494f437b";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvUploadProgram_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvUploadProgram_Request& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/srv/UploadProgram_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_dashboard_msgsSrvUploadProgram_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_ae9a28c1cd4729f54968d18a16e2f8f417bdc5e9bc15809de7481f1f20e6b398";
  static bool encode(uwvm_custom::Ros2Ur_dashboard_msgsSrvUploadProgram_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_dashboard_msgsSrvUploadProgram_Response& out) {return !roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/FollowJointTrajectoryUntil_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal> {
  static constexpr size_t max_wire_bytes=7053;
  static constexpr char const* codec_identity="UWVMROS1_78fda8f2d20c502e24b1ad4fe55f34bba41de20ff88e71b56d7a0631d7a3905a";
  static bool encode(uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal& out) {return !roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/FollowJointTrajectoryUntil_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result> {
  static constexpr size_t max_wire_bytes=268;
  static constexpr char const* codec_identity="UWVMROS1_de92b9ccfb9c93f3afae9da5e8e31df9a6d6a606479fe27b9bcf14ff329bc869";
  static bool encode(uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result& out) {return !roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/FollowJointTrajectoryUntil_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback> {
  static constexpr size_t max_wire_bytes=2352;
  static constexpr char const* codec_identity="UWVMROS1_5a719f991f5c73784f9dc9e962ecfa0541e00daef7f747c312d4b633d63040ac";
  static bool encode(uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback& out) {return !roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/SendScript_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionSendScript_Goal> {
  static constexpr size_t max_wire_bytes=8465;
  static constexpr char const* codec_identity="UWVMROS1_c277cd00663786f95201e61baa658b92def3becc1f37e54aa6093bb11135819c";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Ur_msgsActionSendScript_Goal default_value() noexcept {uwvm_custom::Ros2Ur_msgsActionSendScript_Goal value={0};(void)roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Ur_msgsActionSendScript_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionSendScript_Goal& out) {return !roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/SendScript_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionSendScript_Result> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_d4c4d9721c3a576f590ba4967b2444a82969d6ccba94ddbf8c4a974535ee17d2";
  static bool encode(uwvm_custom::Ros2Ur_msgsActionSendScript_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_SendScript_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_SendScript_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_SendScript_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionSendScript_Result& out) {return !roswasmc_bounded_2_ur_msgs_action_SendScript_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/SendScript_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionSendScript_Feedback> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_094876d6467bd2b34f74cc6b574393117753add5254285e4e95d85b07328ef2c";
  static bool encode(uwvm_custom::Ros2Ur_msgsActionSendScript_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionSendScript_Feedback& out) {return !roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/ToolContact_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionToolContact_Goal> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_150298ad16b1bc37d47bd3324cd4ec301ad8402c07779e8a435ce61daa7578dd";
  static bool encode(uwvm_custom::Ros2Ur_msgsActionToolContact_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionToolContact_Goal& out) {return !roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/ToolContact_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionToolContact_Result> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_b79f6080ac18b67c25131458a2af0a3c235d624c00fc5c13da534cc546c94578";
  static bool encode(uwvm_custom::Ros2Ur_msgsActionToolContact_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionToolContact_Result& out) {return !roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/action/ToolContact_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsActionToolContact_Feedback> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_c6a11fd615f72a70a98514703de4c007a2bb161fbd8a953c5e8b899f471f4c5b";
  static bool encode(uwvm_custom::Ros2Ur_msgsActionToolContact_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsActionToolContact_Feedback& out) {return !roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/msg/Analog; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsMsgAnalog> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_68873e85836e1a65d7b67a52f5db8b3c3be082f889d2df4342386b282988f2a4";
  static bool encode(uwvm_custom::Ros2Ur_msgsMsgAnalog const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_msg_Analog_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_msg_Analog_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_msg_Analog_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsMsgAnalog& out) {return !roswasmc_bounded_2_ur_msgs_msg_Analog_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/msg/Digital; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsMsgDigital> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_e5ebd0a17ee528cff1e0a636eb0be9c7e4128d3e77be722c5f1be73df4f4c11e";
  static bool encode(uwvm_custom::Ros2Ur_msgsMsgDigital const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_msg_Digital_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_msg_Digital_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_msg_Digital_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsMsgDigital& out) {return !roswasmc_bounded_2_ur_msgs_msg_Digital_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/msg/FrictionModelParameters; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsMsgFrictionModelParameters> {
  static constexpr size_t max_wire_bytes=104;
  static constexpr char const* codec_identity="UWVMROS1_7cc9c7af77340f56b0559b4e2ad32972ff65f2a6cf89ebfbe2c48c645f8e9c89";
  static bool encode(uwvm_custom::Ros2Ur_msgsMsgFrictionModelParameters const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsMsgFrictionModelParameters& out) {return !roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/msg/IOStates; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsMsgIOStates> {
  static constexpr size_t max_wire_bytes=404;
  static constexpr char const* codec_identity="UWVMROS1_c22f9f784c89b6854fa84df416ed3704e3a312b15c8fc41162e7f5f53223fef5";
  static bool encode(uwvm_custom::Ros2Ur_msgsMsgIOStates const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_msg_IOStates_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_msg_IOStates_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_msg_IOStates_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsMsgIOStates& out) {return !roswasmc_bounded_2_ur_msgs_msg_IOStates_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/msg/MasterboardDataMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsMsgMasterboardDataMsg> {
  static constexpr size_t max_wire_bytes=62;
  static constexpr char const* codec_identity="UWVMROS1_eb3b0aab27bb5683859356ca7cf9648e53d311c1fd32cac97f68ce1d4622a9e5";
  static bool encode(uwvm_custom::Ros2Ur_msgsMsgMasterboardDataMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsMsgMasterboardDataMsg& out) {return !roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/msg/RobotModeDataMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsMsgRobotModeDataMsg> {
  static constexpr size_t max_wire_bytes=15;
  static constexpr char const* codec_identity="UWVMROS1_0181d85868dc6251289c6f7bc723757fa573c1345b6502497e335655d328bb31";
  static bool encode(uwvm_custom::Ros2Ur_msgsMsgRobotModeDataMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsMsgRobotModeDataMsg& out) {return !roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/msg/RobotStateRTMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsMsgRobotStateRTMsg> {
  static constexpr size_t max_wire_bytes=768;
  static constexpr char const* codec_identity="UWVMROS1_a30600c42e98b89da49d63073a5d30b0989aa0cb21f2bafec229dee10591d9cd";
  static bool encode(uwvm_custom::Ros2Ur_msgsMsgRobotStateRTMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsMsgRobotStateRTMsg& out) {return !roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/msg/ToolDataMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsMsgToolDataMsg> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_1eec69666515ffbdca162a10967fce03ee4c25f0c27d56c5a812b87c9a229aa0";
  static bool encode(uwvm_custom::Ros2Ur_msgsMsgToolDataMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsMsgToolDataMsg& out) {return !roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/GetRobotSoftwareVersion_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_c2d78972cb5b63138d1e780e8ff0c082dd77d5b76b09642d2c655e9f16780101";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request& out) {return !roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/GetRobotSoftwareVersion_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_bbe9984c34d69f93ad8dbfe6059c6a2e1bf43dbd27eabd97cf0a9507308d814a";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response& out) {return !roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetAnalogOutput_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetAnalogOutput_Request> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_71c3eca5c6c90758f084e275fc495feb5d1c08a467c97d941fb5864c54309bea";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetAnalogOutput_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetAnalogOutput_Request& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetAnalogOutput_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetAnalogOutput_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_71a5f3580657cb3ca702dbb63b7af3330c922624c9e9774e47e6533e2fc0a05a";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetAnalogOutput_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetAnalogOutput_Response& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetForceMode_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetForceMode_Request> {
  static constexpr size_t max_wire_bytes=331;
  static constexpr char const* codec_identity="UWVMROS1_e43a6c22510a2078de3cb2dc17ba1b7184080a6aa29a1653b7986723c0cfef6e";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Ur_msgsSrvSetForceMode_Request default_value() noexcept {uwvm_custom::Ros2Ur_msgsSrvSetForceMode_Request value={0};(void)roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetForceMode_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetForceMode_Request& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetForceMode_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetForceMode_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_3f5438d426b7b68557ac8f43dc8efc2e2106782ec0cd4fd0dee7cdd69ddc7de8";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetForceMode_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetForceMode_Response& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetFrictionModelParameters_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetFrictionModelParameters_Request> {
  static constexpr size_t max_wire_bytes=104;
  static constexpr char const* codec_identity="UWVMROS1_e44ad15f9c45d5a65eb13e6bc6e973bb20eebdafa04a5f2e8bcf4b0b54ce91c2";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetFrictionModelParameters_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetFrictionModelParameters_Request& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetFrictionModelParameters_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetFrictionModelParameters_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1caca29dda3dc32360fd010dfb352b6feb54bba66ee16edd2f4353e3382207b3";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetFrictionModelParameters_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetFrictionModelParameters_Response& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetGravity_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetGravity_Request> {
  static constexpr size_t max_wire_bytes=164;
  static constexpr char const* codec_identity="UWVMROS1_3d7fed6ae8accbddd5b09f93c3e49f2b9f99cae4cf80a951eb31ace30ec5a834";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetGravity_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetGravity_Request& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetGravity_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetGravity_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_84694df7fbb4fd15bfecaf1aae683ef1795b0c4941d1987d951467448da5f793";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetGravity_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetGravity_Response& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetIO_Request> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_c8ac4dddeb474521b4c701a0229e99af954416b7b8d7f32d2a19629d38b378c7";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetIO_Request& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetIO_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_bd32373034093ccdab140a5ba8ce8a420ef2ce441430e8306b2484f0ca535090";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetIO_Response& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetPayload_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetPayload_Request> {
  static constexpr size_t max_wire_bytes=84;
  static constexpr char const* codec_identity="UWVMROS1_e76e256e1accf2d742c4cf529e5af1e830db75f3db703a1f368d28077a87e735";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetPayload_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetPayload_Request& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetPayload_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetPayload_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_036dbaa7d42b6cba6ad29a082e078401d507292483d260ba532f7d6947c1624c";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetPayload_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetPayload_Response& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetSpeedSliderFraction_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetSpeedSliderFraction_Request> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_d8f19fb240e4b70cad74c5111cdaebad14b480f21da953c95d474064b091abd8";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetSpeedSliderFraction_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetSpeedSliderFraction_Request& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/srv/SetSpeedSliderFraction_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Ur_msgsSrvSetSpeedSliderFraction_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1ab1ad5ab4b1ed302710ec055dff7644cc6e966af919ff311eaea6cddcf7dbb9";
  static bool encode(uwvm_custom::Ros2Ur_msgsSrvSetSpeedSliderFraction_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Ur_msgsSrvSetSpeedSliderFraction_Response& out) {return !roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
