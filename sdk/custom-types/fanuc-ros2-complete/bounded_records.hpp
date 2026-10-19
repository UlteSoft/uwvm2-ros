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
/** Complete bounded guest value for fanuc_msgs/msg/IOType. */
using Ros2Fanuc_msgsMsgIOType=roswasmc_bounded_2_fanuc_msgs_msg_IOType;
/** Complete bounded guest value for fanuc_msgs/msg/AnalogIO. */
using Ros2Fanuc_msgsMsgAnalogIO=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO;
/** Complete bounded guest value for fanuc_msgs/msg/AnalogIOCmd. */
using Ros2Fanuc_msgsMsgAnalogIOCmd=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd;
/** Complete bounded guest value for fanuc_msgs/msg/AnalogIOState. */
using Ros2Fanuc_msgsMsgAnalogIOState=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState;
/** Complete bounded guest value for fanuc_msgs/msg/BoolIO. */
using Ros2Fanuc_msgsMsgBoolIO=roswasmc_bounded_2_fanuc_msgs_msg_BoolIO;
/** Complete bounded guest value for fanuc_msgs/msg/CollaborativeSpeedScaling. */
using Ros2Fanuc_msgsMsgCollaborativeSpeedScaling=roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling;
/** Complete bounded guest value for fanuc_msgs/msg/ConnectionStatus. */
using Ros2Fanuc_msgsMsgConnectionStatus=roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus;
/** Complete bounded guest value for fanuc_msgs/msg/ForceSensor. */
using Ros2Fanuc_msgsMsgForceSensor=roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor;
/** Complete bounded guest value for fanuc_msgs/msg/Frame. */
using Ros2Fanuc_msgsMsgFrame=roswasmc_bounded_2_fanuc_msgs_msg_Frame;
/** Complete bounded guest value for fanuc_msgs/msg/IOCmd. */
using Ros2Fanuc_msgsMsgIOCmd=roswasmc_bounded_2_fanuc_msgs_msg_IOCmd;
/** Complete bounded guest value for fanuc_msgs/msg/IOState. */
using Ros2Fanuc_msgsMsgIOState=roswasmc_bounded_2_fanuc_msgs_msg_IOState;
/** Complete bounded guest value for fanuc_msgs/msg/NumReg. */
using Ros2Fanuc_msgsMsgNumReg=roswasmc_bounded_2_fanuc_msgs_msg_NumReg;
/** Complete bounded guest value for fanuc_msgs/msg/NumRegCmd. */
using Ros2Fanuc_msgsMsgNumRegCmd=roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd;
/** Complete bounded guest value for fanuc_msgs/msg/NumRegState. */
using Ros2Fanuc_msgsMsgNumRegState=roswasmc_bounded_2_fanuc_msgs_msg_NumRegState;
/** Complete bounded guest value for fanuc_msgs/msg/RMIStatus. */
using Ros2Fanuc_msgsMsgRMIStatus=roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus;
/** Complete bounded guest value for fanuc_msgs/msg/RobotStatus. */
using Ros2Fanuc_msgsMsgRobotStatus=roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus;
/** Complete bounded guest value for fanuc_msgs/msg/RobotStatusExt. */
using Ros2Fanuc_msgsMsgRobotStatusExt=roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt;
/** Complete bounded guest value for fanuc_msgs/srv/CfgForceSensor_Request. */
using Ros2Fanuc_msgsSrvCfgForceSensor_Request=roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request;
/** Complete bounded guest value for fanuc_msgs/srv/CfgForceSensor_Response. */
using Ros2Fanuc_msgsSrvCfgForceSensor_Response=roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetAnalogIO_Request. */
using Ros2Fanuc_msgsSrvGetAnalogIO_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetAnalogIO_Response. */
using Ros2Fanuc_msgsSrvGetAnalogIO_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetBoolIO_Request. */
using Ros2Fanuc_msgsSrvGetBoolIO_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetBoolIO_Response. */
using Ros2Fanuc_msgsSrvGetBoolIO_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetCartesianPosition_Request. */
using Ros2Fanuc_msgsSrvGetCartesianPosition_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetCartesianPosition_Response. */
using Ros2Fanuc_msgsSrvGetCartesianPosition_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetGroupIO_Request. */
using Ros2Fanuc_msgsSrvGetGroupIO_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetGroupIO_Response. */
using Ros2Fanuc_msgsSrvGetGroupIO_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetNumReg_Request. */
using Ros2Fanuc_msgsSrvGetNumReg_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetNumReg_Response. */
using Ros2Fanuc_msgsSrvGetNumReg_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetPayloadComp_Request. */
using Ros2Fanuc_msgsSrvGetPayloadComp_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetPayloadComp_Response. */
using Ros2Fanuc_msgsSrvGetPayloadComp_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetPayloadID_Request. */
using Ros2Fanuc_msgsSrvGetPayloadID_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetPayloadID_Response. */
using Ros2Fanuc_msgsSrvGetPayloadID_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetPayloadValue_Request. */
using Ros2Fanuc_msgsSrvGetPayloadValue_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetPayloadValue_Response. */
using Ros2Fanuc_msgsSrvGetPayloadValue_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetPosReg_Request. */
using Ros2Fanuc_msgsSrvGetPosReg_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetPosReg_Response. */
using Ros2Fanuc_msgsSrvGetPosReg_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetTCPSpeed_Request. */
using Ros2Fanuc_msgsSrvGetTCPSpeed_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetTCPSpeed_Response. */
using Ros2Fanuc_msgsSrvGetTCPSpeed_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetUFrameData_Request. */
using Ros2Fanuc_msgsSrvGetUFrameData_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetUFrameData_Response. */
using Ros2Fanuc_msgsSrvGetUFrameData_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetUFrameUTool_Request. */
using Ros2Fanuc_msgsSrvGetUFrameUTool_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetUFrameUTool_Response. */
using Ros2Fanuc_msgsSrvGetUFrameUTool_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response;
/** Complete bounded guest value for fanuc_msgs/srv/GetUToolData_Request. */
using Ros2Fanuc_msgsSrvGetUToolData_Request=roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request;
/** Complete bounded guest value for fanuc_msgs/srv/GetUToolData_Response. */
using Ros2Fanuc_msgsSrvGetUToolData_Response=roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response;
/** Complete bounded guest value for fanuc_msgs/srv/ReadError_Request. */
using Ros2Fanuc_msgsSrvReadError_Request=roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request;
/** Complete bounded guest value for fanuc_msgs/srv/ReadError_Response. */
using Ros2Fanuc_msgsSrvReadError_Response=roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response;
/** Complete bounded guest value for fanuc_msgs/srv/Reset_Request. */
using Ros2Fanuc_msgsSrvReset_Request=roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request;
/** Complete bounded guest value for fanuc_msgs/srv/Reset_Response. */
using Ros2Fanuc_msgsSrvReset_Response=roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetAnalogIO_Request. */
using Ros2Fanuc_msgsSrvSetAnalogIO_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetAnalogIO_Response. */
using Ros2Fanuc_msgsSrvSetAnalogIO_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetBoolIO_Request. */
using Ros2Fanuc_msgsSrvSetBoolIO_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetBoolIO_Response. */
using Ros2Fanuc_msgsSrvSetBoolIO_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetGenOverride_Request. */
using Ros2Fanuc_msgsSrvSetGenOverride_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetGenOverride_Response. */
using Ros2Fanuc_msgsSrvSetGenOverride_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetGroupIO_Request. */
using Ros2Fanuc_msgsSrvSetGroupIO_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetGroupIO_Response. */
using Ros2Fanuc_msgsSrvSetGroupIO_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetNumReg_Request. */
using Ros2Fanuc_msgsSrvSetNumReg_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetNumReg_Response. */
using Ros2Fanuc_msgsSrvSetNumReg_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetPayloadComp_Request. */
using Ros2Fanuc_msgsSrvSetPayloadComp_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetPayloadComp_Response. */
using Ros2Fanuc_msgsSrvSetPayloadComp_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetPayloadID_Request. */
using Ros2Fanuc_msgsSrvSetPayloadID_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetPayloadID_Response. */
using Ros2Fanuc_msgsSrvSetPayloadID_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetPayloadValue_Request. */
using Ros2Fanuc_msgsSrvSetPayloadValue_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetPayloadValue_Response. */
using Ros2Fanuc_msgsSrvSetPayloadValue_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetPosReg_Request. */
using Ros2Fanuc_msgsSrvSetPosReg_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetPosReg_Response. */
using Ros2Fanuc_msgsSrvSetPosReg_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetUFrameData_Request. */
using Ros2Fanuc_msgsSrvSetUFrameData_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetUFrameData_Response. */
using Ros2Fanuc_msgsSrvSetUFrameData_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetUFrameUTool_Request. */
using Ros2Fanuc_msgsSrvSetUFrameUTool_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetUFrameUTool_Response. */
using Ros2Fanuc_msgsSrvSetUFrameUTool_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SetUToolData_Request. */
using Ros2Fanuc_msgsSrvSetUToolData_Request=roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SetUToolData_Response. */
using Ros2Fanuc_msgsSrvSetUToolData_Response=roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response;
/** Complete bounded guest value for fanuc_msgs/srv/SwitchControlState_Request. */
using Ros2Fanuc_msgsSrvSwitchControlState_Request=roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request;
/** Complete bounded guest value for fanuc_msgs/srv/SwitchControlState_Response. */
using Ros2Fanuc_msgsSrvSwitchControlState_Response=roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response;
/** Complete bounded guest value for rmi_msgs/msg/CallParam. */
using Ros2Rmi_msgsMsgCallParam=roswasmc_bounded_2_rmi_msgs_msg_CallParam;
/** Complete bounded guest value for rmi_msgs/msg/Cartesian. */
using Ros2Rmi_msgsMsgCartesian=roswasmc_bounded_2_rmi_msgs_msg_Cartesian;
/** Complete bounded guest value for rmi_msgs/msg/Command. */
using Ros2Rmi_msgsMsgCommand=roswasmc_bounded_2_rmi_msgs_msg_Command;
/** Complete bounded guest value for rmi_msgs/msg/CommandOptions. */
using Ros2Rmi_msgsMsgCommandOptions=roswasmc_bounded_2_rmi_msgs_msg_CommandOptions;
/** Complete bounded guest value for rmi_msgs/msg/Joint. */
using Ros2Rmi_msgsMsgJoint=roswasmc_bounded_2_rmi_msgs_msg_Joint;
/** Complete bounded guest value for rmi_msgs/msg/MotionInstructionOptions. */
using Ros2Rmi_msgsMsgMotionInstructionOptions=roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions;
/** Complete bounded guest value for rmi_msgs/msg/MotionType. */
using Ros2Rmi_msgsMsgMotionType=roswasmc_bounded_2_rmi_msgs_msg_MotionType;
/** Complete bounded guest value for rmi_msgs/msg/Representation. */
using Ros2Rmi_msgsMsgRepresentation=roswasmc_bounded_2_rmi_msgs_msg_Representation;
/** Complete bounded guest value for rmi_msgs/msg/SpeedType. */
using Ros2Rmi_msgsMsgSpeedType=roswasmc_bounded_2_rmi_msgs_msg_SpeedType;
/** Complete bounded guest value for rmi_msgs/msg/TermType. */
using Ros2Rmi_msgsMsgTermType=roswasmc_bounded_2_rmi_msgs_msg_TermType;
/** Complete bounded guest value for rmi_msgs/srv/AddCall_Request. */
using Ros2Rmi_msgsSrvAddCall_Request=roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request;
/** Complete bounded guest value for rmi_msgs/srv/AddCall_Response. */
using Ros2Rmi_msgsSrvAddCall_Response=roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response;
/** Complete bounded guest value for rmi_msgs/srv/AddMotionInstruction_Request. */
using Ros2Rmi_msgsSrvAddMotionInstruction_Request=roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request;
/** Complete bounded guest value for rmi_msgs/srv/AddMotionInstruction_Response. */
using Ros2Rmi_msgsSrvAddMotionInstruction_Response=roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response;
/** Complete bounded guest value for rmi_msgs/srv/AddSetPayload_Request. */
using Ros2Rmi_msgsSrvAddSetPayload_Request=roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request;
/** Complete bounded guest value for rmi_msgs/srv/AddSetPayload_Response. */
using Ros2Rmi_msgsSrvAddSetPayload_Response=roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response;
/** Complete bounded guest value for rmi_msgs/srv/AddSetUFrame_Request. */
using Ros2Rmi_msgsSrvAddSetUFrame_Request=roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request;
/** Complete bounded guest value for rmi_msgs/srv/AddSetUFrame_Response. */
using Ros2Rmi_msgsSrvAddSetUFrame_Response=roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response;
/** Complete bounded guest value for rmi_msgs/srv/AddSetUTool_Request. */
using Ros2Rmi_msgsSrvAddSetUTool_Request=roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request;
/** Complete bounded guest value for rmi_msgs/srv/AddSetUTool_Response. */
using Ros2Rmi_msgsSrvAddSetUTool_Response=roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response;
/** Complete bounded guest value for rmi_msgs/srv/AddWaitDIN_Request. */
using Ros2Rmi_msgsSrvAddWaitDIN_Request=roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request;
/** Complete bounded guest value for rmi_msgs/srv/AddWaitDIN_Response. */
using Ros2Rmi_msgsSrvAddWaitDIN_Response=roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response;
/** Complete bounded guest value for rmi_msgs/srv/AddWaitTime_Request. */
using Ros2Rmi_msgsSrvAddWaitTime_Request=roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request;
/** Complete bounded guest value for rmi_msgs/srv/AddWaitTime_Response. */
using Ros2Rmi_msgsSrvAddWaitTime_Response=roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response;
/** Complete bounded guest value for rmi_msgs/srv/CallCommand_Request. */
using Ros2Rmi_msgsSrvCallCommand_Request=roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request;
/** Complete bounded guest value for rmi_msgs/srv/CallCommand_Response. */
using Ros2Rmi_msgsSrvCallCommand_Response=roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response;
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
/** Checked codec for fanuc_msgs/msg/IOType; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgIOType> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_1815aa6b1bcab1efe21de0e31570ca650553b5c70f0793301f82c0ca7934c586";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgIOType const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_IOType_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgIOType& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_IOType_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/AnalogIO; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgAnalogIO> {
  static constexpr size_t max_wire_bytes=74;
  static constexpr char const* codec_identity="UWVMROS1_d62097527ba99bf6e803283350161cd1415173a22eb3cb20331a232366076d00";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgAnalogIO const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgAnalogIO& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/AnalogIOCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgAnalogIOCmd> {
  static constexpr size_t max_wire_bytes=2372;
  static constexpr char const* codec_identity="UWVMROS1_62f38a088ed23f8daf7163f26514ae1aabd7b1b64845ccdbc281cb7839d857ba";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgAnalogIOCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgAnalogIOCmd& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/AnalogIOState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgAnalogIOState> {
  static constexpr size_t max_wire_bytes=2372;
  static constexpr char const* codec_identity="UWVMROS1_820b41fecbf6db8e2b2e67d60cd1167cf474b22ca6c489f4d2d64dd33eaac9a4";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgAnalogIOState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgAnalogIOState& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/BoolIO; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgBoolIO> {
  static constexpr size_t max_wire_bytes=71;
  static constexpr char const* codec_identity="UWVMROS1_9209ae1b8f5407a31beface0a3286f58bd823aabdc560c2a4bf4a9b1ca125aa7";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgBoolIO const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgBoolIO& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/CollaborativeSpeedScaling; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgCollaborativeSpeedScaling> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_693a3b581527d2eaf0f697f31e87ca2da428d1f51b821d42d6b06af387a2fb45";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgCollaborativeSpeedScaling const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgCollaborativeSpeedScaling& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/ConnectionStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgConnectionStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_8ce24fb87538922b1f3c0ae856eb223d716143064eb46a6cfce5fec01b45b078";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgConnectionStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgConnectionStatus& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/ForceSensor; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgForceSensor> {
  static constexpr size_t max_wire_bytes=28;
  static constexpr char const* codec_identity="UWVMROS1_06c99b43d7347646179850f3ba79192da7ce71296667d93d88659efbc1004032";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgForceSensor const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgForceSensor& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/Frame; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgFrame> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_609d1b7e49315f7f0a0fe86fa997673acd0d009e57b563690f36624b59b9effb";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgFrame const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_Frame_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_Frame_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgFrame& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_Frame_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/IOCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgIOCmd> {
  static constexpr size_t max_wire_bytes=2276;
  static constexpr char const* codec_identity="UWVMROS1_462552bbaa3c9ed4caaf3108960d514e4ab61fcf2d6a84f0465b485bec1de45d";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgIOCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgIOCmd& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/IOState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgIOState> {
  static constexpr size_t max_wire_bytes=2276;
  static constexpr char const* codec_identity="UWVMROS1_332533445014c03d626a2c4dbdffd9421748aa6b73f49d407441fd6b9f2b24d8";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgIOState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_IOState_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_IOState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_IOState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgIOState& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_IOState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/NumReg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgNumReg> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_0fb9344c5768ea52ab072939f368f1e73dcc0ff83e74b4cb6d35fca47c1d29cc";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgNumReg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_NumReg_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_NumReg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_NumReg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgNumReg& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_NumReg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/NumRegCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgNumRegCmd> {
  static constexpr size_t max_wire_bytes=196;
  static constexpr char const* codec_identity="UWVMROS1_9c76500073889377525a0802c46bd96589a2639dd9534cf7ad2b9956cd95b5b6";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgNumRegCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgNumRegCmd& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/NumRegState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgNumRegState> {
  static constexpr size_t max_wire_bytes=196;
  static constexpr char const* codec_identity="UWVMROS1_9c8ff4917a6dd033e6a6776ae8525e93c2924d776f05fd54e7e8d2251fc48b3a";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgNumRegState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgNumRegState& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/RMIStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgRMIStatus> {
  static constexpr size_t max_wire_bytes=1048;
  static constexpr char const* codec_identity="UWVMROS1_dc2e7eaabf44807142581905838f8951653682a6c6d61ebcf56494b16a10d5e8";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgRMIStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgRMIStatus& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/RobotStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgRobotStatus> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_3f31ee3631dc6f4c90eb5ded6209ccebf0f4193ddfe5f5032bdd2bcd71f8cc31";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgRobotStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgRobotStatus& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/msg/RobotStatusExt; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsMsgRobotStatusExt> {
  static constexpr size_t max_wire_bytes=1042;
  static constexpr char const* codec_identity="UWVMROS1_8ba47a11bfbe02026fe79b32e1ed02e5f609499ea55bb57c9f09af9fba389648";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsMsgRobotStatusExt const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsMsgRobotStatusExt& out) {return !roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/CfgForceSensor_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensor_Request> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_01973ed21546cb747ad2ce2a990a8ec4e70efb66e1c8fc8856cf1082870984ff";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensor_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensor_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/CfgForceSensor_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensor_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_41799ce44c937b1524889391db61e2b4d35795055cb4bbacfe5cc8bbd5b9a37b";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensor_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensor_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetAnalogIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIO_Request> {
  static constexpr size_t max_wire_bytes=70;
  static constexpr char const* codec_identity="UWVMROS1_cd567f97de2de581975c3d4f280eec545dea37c941d316fc3708bbc4d3af94d9";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIO_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetAnalogIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIO_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_3b53d3ce95d95454a782778c160fcc102ead869c5681c9c67224c373fef1b217";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIO_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetBoolIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIO_Request> {
  static constexpr size_t max_wire_bytes=70;
  static constexpr char const* codec_identity="UWVMROS1_ce8c4b1efd2828e92656e897baf041e8965367a236079231101807c898d4b50c";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIO_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetBoolIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIO_Response> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_7dbdcdde8d76c4ef422ea158551dd912273220c25a08f28fc8d19fdf8bc8a01c";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIO_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetCartesianPosition_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPosition_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_a694a17bde88131efcadac4f134464a7777beab4b09e17624ae7b173ab62c3e4";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPosition_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPosition_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetCartesianPosition_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPosition_Response> {
  static constexpr size_t max_wire_bytes=54;
  static constexpr char const* codec_identity="UWVMROS1_0516f0145717b06550674e02f0b9c47f9b312f4f361c791f56257fbb79e67329";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPosition_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPosition_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetGroupIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIO_Request> {
  static constexpr size_t max_wire_bytes=70;
  static constexpr char const* codec_identity="UWVMROS1_dbd2e8e791e8debc745843827ed296b51bf84159f4c4fe5d912c231d621de216";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIO_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetGroupIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIO_Response> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_9e131b91f36badc90ddae54a9855095241ab86c51b419b684d62879fae0bda7f";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIO_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetNumReg_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetNumReg_Request> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_727b5ff4ab9c8976280df12e80d3aee03b31288087adb94ee82161574fc8dcb5";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetNumReg_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetNumReg_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetNumReg_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetNumReg_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_78ae7ff82b3b2cd7d512fee8335f14a0759a71425fcf3d1b150dbe5e2a0179b3";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetNumReg_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetNumReg_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetPayloadComp_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadComp_Request> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_d48b88619f4a68ceeed02336e9ef078c27905707a21d977448fbbfbd560f398d";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadComp_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadComp_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetPayloadComp_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadComp_Response> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_012b216c949f99755a36678b6be28d032f698b472ce4fa9ba975bc6d453ae4e4";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadComp_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadComp_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetPayloadID_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadID_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_6c943659905c8adc7aa4b73832584994c83bfe009d18dc30a11d99b9fc0d6eef";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadID_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadID_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetPayloadID_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadID_Response> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_5551f4eab590c87fd7679d593c9c446903bdaa02aa1ed2512b146768122a950c";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadID_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadID_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetPayloadValue_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValue_Request> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_d82667bd644873ae4fa5de0289ed94791eb2a7af49c1b8f86ed7f91cf875407a";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValue_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValue_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetPayloadValue_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValue_Response> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_c3361ceec2ccd5b4d3ef566cbd885f8cddba75b4f1dc1712144147827689f06d";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValue_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValue_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetPosReg_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetPosReg_Request> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_f7f1391c5ea385bff615d95c5e38f01a809828b142112829dc436916b063505a";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetPosReg_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetPosReg_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetPosReg_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetPosReg_Response> {
  static constexpr size_t max_wire_bytes=601;
  static constexpr char const* codec_identity="UWVMROS1_e540225b0d532c460905582ad99ee175dd771e971c7e75f5dfb9325d39ed6b27";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetPosReg_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetPosReg_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetTCPSpeed_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeed_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_6bdfd44465b75bbd4e232f802dbd3c0bfa28e90a93f9a28b3c15fb150bce7ac7";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeed_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeed_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetTCPSpeed_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeed_Response> {
  static constexpr size_t max_wire_bytes=13;
  static constexpr char const* codec_identity="UWVMROS1_3ec07d99342f275506452f32076f9e1ea01bf32f43662311708a9b44ca951147";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeed_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeed_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetUFrameData_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameData_Request> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_c1ef7077fd7f61ed8ff46931c8e39c5950658cfe1d36cffac10083ab304307be";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameData_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameData_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetUFrameData_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameData_Response> {
  static constexpr size_t max_wire_bytes=30;
  static constexpr char const* codec_identity="UWVMROS1_f956e1447bd4a15f547fdf5d6775836018d50153270fe8863c4b30d83422fd2e";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameData_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameData_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetUFrameUTool_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUTool_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_3d57da6f8003b33b2a079d4f284eb0f182d7e088074112b7f4ee25d3113bd443";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUTool_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUTool_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetUFrameUTool_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUTool_Response> {
  static constexpr size_t max_wire_bytes=7;
  static constexpr char const* codec_identity="UWVMROS1_f9cb64b89f17b6811e4073b9f94d0fd6844d21ec4b846f78bfd13cb14a35a6b3";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUTool_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUTool_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetUToolData_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetUToolData_Request> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_10c3799dcb1b09dbcb6d1b794a00ecfaf69a8f457d95eeb92855f79dba7be1bc";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetUToolData_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetUToolData_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/GetUToolData_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvGetUToolData_Response> {
  static constexpr size_t max_wire_bytes=30;
  static constexpr char const* codec_identity="UWVMROS1_f3a2b94f337cec9c58156b803675f38885350cc3fc3171234c0d361afc800d81";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvGetUToolData_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvGetUToolData_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/ReadError_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvReadError_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_4b67ed8e2ecccefb6bdead80249f5916e6ea192d822183292db43351839fb6f9";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Fanuc_msgsSrvReadError_Request default_value() noexcept {uwvm_custom::Ros2Fanuc_msgsSrvReadError_Request value={0};(void)roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvReadError_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvReadError_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/ReadError_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvReadError_Response> {
  static constexpr size_t max_wire_bytes=2585;
  static constexpr char const* codec_identity="UWVMROS1_6e28db10c7afb06cb1256f6d74a43e26c5efa6c13fec8826ae0e623a5eb954b6";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvReadError_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvReadError_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/Reset_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvReset_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_b73f661d569f6af8dad1cd6625f4c0c55ef2c14c79b3e9af0f17fdc02736abc4";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvReset_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvReset_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/Reset_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvReset_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_5e9b92e5a112b4934ce0893afaa09156fdb47c26b0aa6ae9a27d61920d308ab8";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvReset_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvReset_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetAnalogIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIO_Request> {
  static constexpr size_t max_wire_bytes=74;
  static constexpr char const* codec_identity="UWVMROS1_a6bb4acfae23db79464faff849f359657537026d7c79f8a6c60259caec5e87f4";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIO_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetAnalogIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIO_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_aeac383e2c4a292f1237494bff5b261cd0197542e00456b4ee0edf9b7fe8a39b";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIO_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetBoolIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIO_Request> {
  static constexpr size_t max_wire_bytes=71;
  static constexpr char const* codec_identity="UWVMROS1_0e8ace58c769670a2519e733d020a3ca902bff99982514d5cd9ecda391c1a4cf";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIO_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetBoolIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIO_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_c9c68ca577dd6e4d1743385d9c69c8f1c6e0780ae247877d823ab97a68e675a0";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIO_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetGenOverride_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverride_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_7d321e33aaf6f58013c33ffc012c1ddbd27aefff3f21d7456030a1d590362039";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverride_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverride_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetGenOverride_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverride_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_1ffcb25cad7dbf51c216753dac2c117a191fb081a5a562e63d34d447e61f0a59";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverride_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverride_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetGroupIO_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIO_Request> {
  static constexpr size_t max_wire_bytes=72;
  static constexpr char const* codec_identity="UWVMROS1_42a76aec283e26e3a2c99bb27a23778e34effb6c3328754793f783038a2b84bd";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIO_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIO_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetGroupIO_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIO_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_aa6d2c99e8817273f8f2ab179df61d60efd8ee2cee0e1ec76c647956e22bff17";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIO_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIO_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetNumReg_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetNumReg_Request> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_7e362b671b8770db33f3876308c41a930863a0805771fa05fcbb170782207e36";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetNumReg_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetNumReg_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetNumReg_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetNumReg_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_7c7850ad8bb5c861d9b436311a8cf985e2e029d362877be80c5a6cb19021e482";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetNumReg_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetNumReg_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetPayloadComp_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadComp_Request> {
  static constexpr size_t max_wire_bytes=30;
  static constexpr char const* codec_identity="UWVMROS1_92c495d71657249e15c172e850aa91869929a25817437567778df1b01d9037a8";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadComp_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadComp_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetPayloadComp_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadComp_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_37a9876e2a9c514fa8c4337ff1351f778d350318bdc6d8aa5d569b4039a2f76b";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadComp_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadComp_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetPayloadID_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadID_Request> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_aa15b59db17d2afb7ef9e74676a9b744763954ae063efafb91a3ab3c9852a1c0";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadID_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadID_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetPayloadID_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadID_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_7b704b36da10dc71b4cfaad92ea02ed22e4966077dc0940b03b8b8e49c5769c7";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadID_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadID_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetPayloadValue_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValue_Request> {
  static constexpr size_t max_wire_bytes=31;
  static constexpr char const* codec_identity="UWVMROS1_5d56ce8f2f8df9c3450d6a5dc77e9e77861471805aa6b9c6f8815c2e7c324974";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValue_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValue_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetPayloadValue_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValue_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_09f30071b49ac57353429e8a2862ee80d3dcab9237e8192d008c12a3628cc6ab";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValue_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValue_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetPosReg_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetPosReg_Request> {
  static constexpr size_t max_wire_bytes=599;
  static constexpr char const* codec_identity="UWVMROS1_f9af742e55a044a1a363a45cbe4a0a8ce4ca62fcaf43759041cef89149d96d30";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetPosReg_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetPosReg_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetPosReg_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetPosReg_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_fb6f889d30c921df31f55cdc760346eb0d655816e71b52d4aac7a1e56d4e9cb2";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetPosReg_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetPosReg_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetUFrameData_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameData_Request> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_a067808d8fe5ca1418e18ac492313ad9211cc8f3bf402ad0ac51a2850a2bc169";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameData_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameData_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetUFrameData_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameData_Response> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_f436e4fe5bd06b048f4f53d2df4933ac8eac5a784f31b42ccf13f5eaf1696eee";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameData_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameData_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetUFrameUTool_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUTool_Request> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_0b86b0a37e4dc23e0ff651df867f5958e7440bf4712b47779d69ca699af4c09f";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUTool_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUTool_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetUFrameUTool_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUTool_Response> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_b77837445c9663f29f23465cc7178a183d245486f74bcff732fc2f4025b82a67";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUTool_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUTool_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetUToolData_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetUToolData_Request> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_7d5b911133645744c8def79bc16c8b7c4ee4e87a057ea33741031d6a94e620ab";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetUToolData_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetUToolData_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SetUToolData_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSetUToolData_Response> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_b33d265f275dfcb5fe20432d5e38b55574c85260135bded477c2d5fa2c85c7cb";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSetUToolData_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSetUToolData_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SwitchControlState_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlState_Request> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_b485b7071fe2737c7dc8f0360a15b7be30f997430142d8260454fd423172cc21";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlState_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlState_Request& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for fanuc_msgs/srv/SwitchControlState_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlState_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_8a3d7a3c1037174f4118843e34222ef8811eff3cc885a0c688d98071e1a9496c";
  static bool encode(uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlState_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlState_Response& out) {return !roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/CallParam; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgCallParam> {
  static constexpr size_t max_wire_bytes=592;
  static constexpr char const* codec_identity="UWVMROS1_ed625d80db6c4314b8373f2d205344d6ab8d71be46182dab5a497d36564a2255";
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgCallParam const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_CallParam_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_CallParam_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_CallParam_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgCallParam& out) {return !roswasmc_bounded_2_rmi_msgs_msg_CallParam_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/Cartesian; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgCartesian> {
  static constexpr size_t max_wire_bytes=45;
  static constexpr char const* codec_identity="UWVMROS1_2a0e1fff119007499a63d55c9722f666be1436a9212d9e43b556af347e8e4b40";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsMsgCartesian default_value() noexcept {uwvm_custom::Ros2Rmi_msgsMsgCartesian value={0};(void)roswasmc_bounded_2_rmi_msgs_msg_Cartesian_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgCartesian const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_Cartesian_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_Cartesian_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_Cartesian_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgCartesian& out) {return !roswasmc_bounded_2_rmi_msgs_msg_Cartesian_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/Command; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgCommand> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_d2741856b8cdf57aa716566edfc705957611557b7da5b6099b173903bdd9960c";
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgCommand const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_Command_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_Command_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_Command_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgCommand& out) {return !roswasmc_bounded_2_rmi_msgs_msg_Command_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/CommandOptions; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgCommandOptions> {
  static constexpr size_t max_wire_bytes=1036;
  static constexpr char const* codec_identity="UWVMROS1_0be0c508035725bf9686807cb8b5267e23f709b7aa34b0910a44ff3604670147";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsMsgCommandOptions default_value() noexcept {uwvm_custom::Ros2Rmi_msgsMsgCommandOptions value={0};(void)roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgCommandOptions const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgCommandOptions& out) {return !roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/Joint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgJoint> {
  static constexpr size_t max_wire_bytes=36;
  static constexpr char const* codec_identity="UWVMROS1_0ba90adf6cc287bc306fd2a77c52c2e4d62731cfac37bb2cc116ee846249a42d";
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgJoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_Joint_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_Joint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_Joint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgJoint& out) {return !roswasmc_bounded_2_rmi_msgs_msg_Joint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/MotionInstructionOptions; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgMotionInstructionOptions> {
  static constexpr size_t max_wire_bytes=2612;
  static constexpr char const* codec_identity="UWVMROS1_f8d9ed2e083c577b0cae8965df71aa99643d12ce5a5a6ce558894edfb756bcc6";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsMsgMotionInstructionOptions default_value() noexcept {uwvm_custom::Ros2Rmi_msgsMsgMotionInstructionOptions value={0};(void)roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgMotionInstructionOptions const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgMotionInstructionOptions& out) {return !roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/MotionType; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgMotionType> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_c51cc7a32f6853d13bedaf3306dd79e445de6c0d16ae0ec39034ce0ae6d19c8a";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsMsgMotionType default_value() noexcept {uwvm_custom::Ros2Rmi_msgsMsgMotionType value={0};(void)roswasmc_bounded_2_rmi_msgs_msg_MotionType_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgMotionType const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_MotionType_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_MotionType_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_MotionType_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgMotionType& out) {return !roswasmc_bounded_2_rmi_msgs_msg_MotionType_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/Representation; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgRepresentation> {
  static constexpr size_t max_wire_bytes=516;
  static constexpr char const* codec_identity="UWVMROS1_b439506f2dc2135bb484b3abb308dfdf5c07367af5a4d6d0f5435ad9bba8ea6f";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsMsgRepresentation default_value() noexcept {uwvm_custom::Ros2Rmi_msgsMsgRepresentation value={0};(void)roswasmc_bounded_2_rmi_msgs_msg_Representation_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgRepresentation const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_Representation_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_Representation_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_Representation_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgRepresentation& out) {return !roswasmc_bounded_2_rmi_msgs_msg_Representation_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/SpeedType; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgSpeedType> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_e4f4dbda84d8d8cd4d026b0beb52005663c7c4ee602c8d1e4d5853589777c282";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsMsgSpeedType default_value() noexcept {uwvm_custom::Ros2Rmi_msgsMsgSpeedType value={0};(void)roswasmc_bounded_2_rmi_msgs_msg_SpeedType_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgSpeedType const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_SpeedType_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_SpeedType_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_SpeedType_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgSpeedType& out) {return !roswasmc_bounded_2_rmi_msgs_msg_SpeedType_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/msg/TermType; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsMsgTermType> {
  static constexpr size_t max_wire_bytes=68;
  static constexpr char const* codec_identity="UWVMROS1_5ddbb483126e0ab5799ed865480da457179d83a271581c1471aec940bedc4535";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsMsgTermType default_value() noexcept {uwvm_custom::Ros2Rmi_msgsMsgTermType value={0};(void)roswasmc_bounded_2_rmi_msgs_msg_TermType_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsMsgTermType const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_msg_TermType_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_msg_TermType_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_msg_TermType_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsMsgTermType& out) {return !roswasmc_bounded_2_rmi_msgs_msg_TermType_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddCall_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddCall_Request> {
  static constexpr size_t max_wire_bytes=9992;
  static constexpr char const* codec_identity="UWVMROS1_4d652cfbaef29487a0a688932ee926d7a4026c5e7ac8a0eace583b503c37bcd8";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddCall_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddCall_Request& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddCall_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddCall_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_4f8495dfc3c69ec640444fd24843a7bb3e30008d90945bdd64817d8acb7964e1";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddCall_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddCall_Response& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddMotionInstruction_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Request> {
  static constexpr size_t max_wire_bytes=4014;
  static constexpr char const* codec_identity="UWVMROS1_3f22c49cb2d32e6a8f3954f3d3cbf0021c87e0363b1f2ded4ac20a97a5b59391";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Request default_value() noexcept {uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Request value={0};(void)roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Request& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddMotionInstruction_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_79fee3e965fda63c3a53ad2178ff48614878acc9f6af74cc27a8c2812b68ac6b";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Response& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddSetPayload_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddSetPayload_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_45da9f81e8c26f49490a4e6eba5c8be0ee67285124540eb1c3e1f2c3e0fd821b";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddSetPayload_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddSetPayload_Request& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddSetPayload_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddSetPayload_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_bfc5bc7b92f292c2980496cb18fbe953ea1b49d45f7950f0125bbbf880bdf72a";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddSetPayload_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddSetPayload_Response& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddSetUFrame_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrame_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_64c83dd2be89b4b6dcf3b3c2ea70184971e6a359914ff1a5cd26d4b114d35475";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrame_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrame_Request& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddSetUFrame_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrame_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_e344ef6efacc8d569111d1c6ef4028e1a5bb54af49d3e8f486ae89b83c674647";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrame_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrame_Response& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddSetUTool_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddSetUTool_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_d1327022326cd41caed6001449517a41f589e148c38d6db30923922175f05c46";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddSetUTool_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddSetUTool_Request& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddSetUTool_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddSetUTool_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_dd4c36458027b87559200cf5f6aa72a60af2d103f663f2fa9daca47ce8020ff6";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddSetUTool_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddSetUTool_Response& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddWaitDIN_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddWaitDIN_Request> {
  static constexpr size_t max_wire_bytes=518;
  static constexpr char const* codec_identity="UWVMROS1_c32214f1ff1b0b6980583fc3088f412e856efb5eaa9c336cf798ef8ad85c61ab";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddWaitDIN_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddWaitDIN_Request& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddWaitDIN_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddWaitDIN_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_7d3933f573619ba49f19df7c11f1d73ef211c17581f660f8c0fbcc152cd519d4";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddWaitDIN_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddWaitDIN_Response& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddWaitTime_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddWaitTime_Request> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_8487f4b7dcfb044c72f7fb62a31ffe9cdc4321a9ebdaa0550a363e32acbadc46";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddWaitTime_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddWaitTime_Request& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/AddWaitTime_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvAddWaitTime_Response> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_f9578c08b971f32f91d1c94b8713cafe648e25147394e20c85bb335254fa20c8";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvAddWaitTime_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvAddWaitTime_Response& out) {return !roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/CallCommand_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Request> {
  static constexpr size_t max_wire_bytes=1104;
  static constexpr char const* codec_identity="UWVMROS1_d1f890a2beec0ca9294ff4f641d1738855d576a917ed05eb34517886c73f381c";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Request default_value() noexcept {uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Request value={0};(void)roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Request& out) {return !roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for rmi_msgs/srv/CallCommand_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Response> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_64d92e6fadc651073469318ad15d2e98ec3d059c629838cefc5820b17ab6f1bc";
  static bool encode(uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Response& out) {return !roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_decode(bytes.data(),bytes.size(),&out);}
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
