/** Complete original family typed aliases; never Native layout aliases or authority. */
#pragma once
#ifndef UWVM_INDUSTRIAL_GENERATED_RECORDS
#error "Select the actual same-source generated bounded_records.hpp"
#endif
#include UWVM_INDUSTRIAL_GENERATED_RECORDS
#ifndef UWVM_INDUSTRIAL_GENERATED_INTERFACES
#error "Select the matching actual generated bounded_interfaces.hpp"
#endif
#include UWVM_INDUSTRIAL_GENERATED_INTERFACES
namespace uwvm_industrial::fanuc_ros2 {
namespace builtin_interfaces {
using Duration=::uwvm_custom::Ros2Builtin_interfacesMsgDuration;
using Time=::uwvm_custom::Ros2Builtin_interfacesMsgTime;
}
namespace control_msgs {
using FollowJointTrajectoryClient=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectoryClient;
using FollowJointTrajectoryServer=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectoryServer;
using FollowJointTrajectoryGoal=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal;
using FollowJointTrajectoryResult=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result;
using FollowJointTrajectoryFeedback=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback;
using JointComponentTolerance=::uwvm_custom::Ros2Control_msgsMsgJointComponentTolerance;
using JointTolerance=::uwvm_custom::Ros2Control_msgsMsgJointTolerance;
}
namespace fanuc_msgs {
using AnalogIO=::uwvm_custom::Ros2Fanuc_msgsMsgAnalogIO;
using AnalogIOCmd=::uwvm_custom::Ros2Fanuc_msgsMsgAnalogIOCmd;
using AnalogIOState=::uwvm_custom::Ros2Fanuc_msgsMsgAnalogIOState;
using BoolIO=::uwvm_custom::Ros2Fanuc_msgsMsgBoolIO;
using CollaborativeSpeedScaling=::uwvm_custom::Ros2Fanuc_msgsMsgCollaborativeSpeedScaling;
using ConnectionStatus=::uwvm_custom::Ros2Fanuc_msgsMsgConnectionStatus;
using ForceSensor=::uwvm_custom::Ros2Fanuc_msgsMsgForceSensor;
using Frame=::uwvm_custom::Ros2Fanuc_msgsMsgFrame;
using IOCmd=::uwvm_custom::Ros2Fanuc_msgsMsgIOCmd;
using IOState=::uwvm_custom::Ros2Fanuc_msgsMsgIOState;
using IOType=::uwvm_custom::Ros2Fanuc_msgsMsgIOType;
using NumReg=::uwvm_custom::Ros2Fanuc_msgsMsgNumReg;
using NumRegCmd=::uwvm_custom::Ros2Fanuc_msgsMsgNumRegCmd;
using NumRegState=::uwvm_custom::Ros2Fanuc_msgsMsgNumRegState;
using RMIStatus=::uwvm_custom::Ros2Fanuc_msgsMsgRMIStatus;
using RobotStatus=::uwvm_custom::Ros2Fanuc_msgsMsgRobotStatus;
using RobotStatusExt=::uwvm_custom::Ros2Fanuc_msgsMsgRobotStatusExt;
using CfgForceSensorClient=::uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensorClient;
using CfgForceSensorServer=::uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensorServer;
using CfgForceSensorRequest=::uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensor_Request;
using CfgForceSensorResponse=::uwvm_custom::Ros2Fanuc_msgsSrvCfgForceSensor_Response;
using GetAnalogIOClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIOClient;
using GetAnalogIOServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIOServer;
using GetAnalogIORequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIO_Request;
using GetAnalogIOResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetAnalogIO_Response;
using GetBoolIOClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIOClient;
using GetBoolIOServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIOServer;
using GetBoolIORequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIO_Request;
using GetBoolIOResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetBoolIO_Response;
using GetCartesianPositionClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPositionClient;
using GetCartesianPositionServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPositionServer;
using GetCartesianPositionRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPosition_Request;
using GetCartesianPositionResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetCartesianPosition_Response;
using GetGroupIOClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIOClient;
using GetGroupIOServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIOServer;
using GetGroupIORequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIO_Request;
using GetGroupIOResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetGroupIO_Response;
using GetNumRegClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetNumRegClient;
using GetNumRegServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetNumRegServer;
using GetNumRegRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetNumReg_Request;
using GetNumRegResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetNumReg_Response;
using GetPayloadCompClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadCompClient;
using GetPayloadCompServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadCompServer;
using GetPayloadCompRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadComp_Request;
using GetPayloadCompResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadComp_Response;
using GetPayloadIDClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadIDClient;
using GetPayloadIDServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadIDServer;
using GetPayloadIDRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadID_Request;
using GetPayloadIDResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadID_Response;
using GetPayloadValueClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValueClient;
using GetPayloadValueServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValueServer;
using GetPayloadValueRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValue_Request;
using GetPayloadValueResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetPayloadValue_Response;
using GetPosRegClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetPosRegClient;
using GetPosRegServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetPosRegServer;
using GetPosRegRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetPosReg_Request;
using GetPosRegResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetPosReg_Response;
using GetTCPSpeedClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeedClient;
using GetTCPSpeedServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeedServer;
using GetTCPSpeedRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeed_Request;
using GetTCPSpeedResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetTCPSpeed_Response;
using GetUFrameDataClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameDataClient;
using GetUFrameDataServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameDataServer;
using GetUFrameDataRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameData_Request;
using GetUFrameDataResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameData_Response;
using GetUFrameUToolClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUToolClient;
using GetUFrameUToolServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUToolServer;
using GetUFrameUToolRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUTool_Request;
using GetUFrameUToolResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetUFrameUTool_Response;
using GetUToolDataClient=::uwvm_custom::Ros2Fanuc_msgsSrvGetUToolDataClient;
using GetUToolDataServer=::uwvm_custom::Ros2Fanuc_msgsSrvGetUToolDataServer;
using GetUToolDataRequest=::uwvm_custom::Ros2Fanuc_msgsSrvGetUToolData_Request;
using GetUToolDataResponse=::uwvm_custom::Ros2Fanuc_msgsSrvGetUToolData_Response;
using ReadErrorClient=::uwvm_custom::Ros2Fanuc_msgsSrvReadErrorClient;
using ReadErrorServer=::uwvm_custom::Ros2Fanuc_msgsSrvReadErrorServer;
using ReadErrorRequest=::uwvm_custom::Ros2Fanuc_msgsSrvReadError_Request;
using ReadErrorResponse=::uwvm_custom::Ros2Fanuc_msgsSrvReadError_Response;
using ResetClient=::uwvm_custom::Ros2Fanuc_msgsSrvResetClient;
using ResetServer=::uwvm_custom::Ros2Fanuc_msgsSrvResetServer;
using ResetRequest=::uwvm_custom::Ros2Fanuc_msgsSrvReset_Request;
using ResetResponse=::uwvm_custom::Ros2Fanuc_msgsSrvReset_Response;
using SetAnalogIOClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIOClient;
using SetAnalogIOServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIOServer;
using SetAnalogIORequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIO_Request;
using SetAnalogIOResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetAnalogIO_Response;
using SetBoolIOClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIOClient;
using SetBoolIOServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIOServer;
using SetBoolIORequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIO_Request;
using SetBoolIOResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetBoolIO_Response;
using SetGenOverrideClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverrideClient;
using SetGenOverrideServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverrideServer;
using SetGenOverrideRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverride_Request;
using SetGenOverrideResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetGenOverride_Response;
using SetGroupIOClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIOClient;
using SetGroupIOServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIOServer;
using SetGroupIORequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIO_Request;
using SetGroupIOResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetGroupIO_Response;
using SetNumRegClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetNumRegClient;
using SetNumRegServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetNumRegServer;
using SetNumRegRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetNumReg_Request;
using SetNumRegResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetNumReg_Response;
using SetPayloadCompClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadCompClient;
using SetPayloadCompServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadCompServer;
using SetPayloadCompRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadComp_Request;
using SetPayloadCompResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadComp_Response;
using SetPayloadIDClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadIDClient;
using SetPayloadIDServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadIDServer;
using SetPayloadIDRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadID_Request;
using SetPayloadIDResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadID_Response;
using SetPayloadValueClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValueClient;
using SetPayloadValueServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValueServer;
using SetPayloadValueRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValue_Request;
using SetPayloadValueResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetPayloadValue_Response;
using SetPosRegClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetPosRegClient;
using SetPosRegServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetPosRegServer;
using SetPosRegRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetPosReg_Request;
using SetPosRegResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetPosReg_Response;
using SetUFrameDataClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameDataClient;
using SetUFrameDataServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameDataServer;
using SetUFrameDataRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameData_Request;
using SetUFrameDataResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameData_Response;
using SetUFrameUToolClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUToolClient;
using SetUFrameUToolServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUToolServer;
using SetUFrameUToolRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUTool_Request;
using SetUFrameUToolResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetUFrameUTool_Response;
using SetUToolDataClient=::uwvm_custom::Ros2Fanuc_msgsSrvSetUToolDataClient;
using SetUToolDataServer=::uwvm_custom::Ros2Fanuc_msgsSrvSetUToolDataServer;
using SetUToolDataRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSetUToolData_Request;
using SetUToolDataResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSetUToolData_Response;
using SwitchControlStateClient=::uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlStateClient;
using SwitchControlStateServer=::uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlStateServer;
using SwitchControlStateRequest=::uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlState_Request;
using SwitchControlStateResponse=::uwvm_custom::Ros2Fanuc_msgsSrvSwitchControlState_Response;
}
namespace geometry_msgs {
using Quaternion=::uwvm_custom::Ros2Geometry_msgsMsgQuaternion;
using Transform=::uwvm_custom::Ros2Geometry_msgsMsgTransform;
using Twist=::uwvm_custom::Ros2Geometry_msgsMsgTwist;
using Vector3=::uwvm_custom::Ros2Geometry_msgsMsgVector3;
}
namespace rmi_msgs {
using CallParam=::uwvm_custom::Ros2Rmi_msgsMsgCallParam;
using Cartesian=::uwvm_custom::Ros2Rmi_msgsMsgCartesian;
using Command=::uwvm_custom::Ros2Rmi_msgsMsgCommand;
using CommandOptions=::uwvm_custom::Ros2Rmi_msgsMsgCommandOptions;
using Joint=::uwvm_custom::Ros2Rmi_msgsMsgJoint;
using MotionInstructionOptions=::uwvm_custom::Ros2Rmi_msgsMsgMotionInstructionOptions;
using MotionType=::uwvm_custom::Ros2Rmi_msgsMsgMotionType;
using Representation=::uwvm_custom::Ros2Rmi_msgsMsgRepresentation;
using SpeedType=::uwvm_custom::Ros2Rmi_msgsMsgSpeedType;
using TermType=::uwvm_custom::Ros2Rmi_msgsMsgTermType;
using AddCallClient=::uwvm_custom::Ros2Rmi_msgsSrvAddCallClient;
using AddCallServer=::uwvm_custom::Ros2Rmi_msgsSrvAddCallServer;
using AddCallRequest=::uwvm_custom::Ros2Rmi_msgsSrvAddCall_Request;
using AddCallResponse=::uwvm_custom::Ros2Rmi_msgsSrvAddCall_Response;
using AddMotionInstructionClient=::uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstructionClient;
using AddMotionInstructionServer=::uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstructionServer;
using AddMotionInstructionRequest=::uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Request;
using AddMotionInstructionResponse=::uwvm_custom::Ros2Rmi_msgsSrvAddMotionInstruction_Response;
using AddSetPayloadClient=::uwvm_custom::Ros2Rmi_msgsSrvAddSetPayloadClient;
using AddSetPayloadServer=::uwvm_custom::Ros2Rmi_msgsSrvAddSetPayloadServer;
using AddSetPayloadRequest=::uwvm_custom::Ros2Rmi_msgsSrvAddSetPayload_Request;
using AddSetPayloadResponse=::uwvm_custom::Ros2Rmi_msgsSrvAddSetPayload_Response;
using AddSetUFrameClient=::uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrameClient;
using AddSetUFrameServer=::uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrameServer;
using AddSetUFrameRequest=::uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrame_Request;
using AddSetUFrameResponse=::uwvm_custom::Ros2Rmi_msgsSrvAddSetUFrame_Response;
using AddSetUToolClient=::uwvm_custom::Ros2Rmi_msgsSrvAddSetUToolClient;
using AddSetUToolServer=::uwvm_custom::Ros2Rmi_msgsSrvAddSetUToolServer;
using AddSetUToolRequest=::uwvm_custom::Ros2Rmi_msgsSrvAddSetUTool_Request;
using AddSetUToolResponse=::uwvm_custom::Ros2Rmi_msgsSrvAddSetUTool_Response;
using AddWaitDINClient=::uwvm_custom::Ros2Rmi_msgsSrvAddWaitDINClient;
using AddWaitDINServer=::uwvm_custom::Ros2Rmi_msgsSrvAddWaitDINServer;
using AddWaitDINRequest=::uwvm_custom::Ros2Rmi_msgsSrvAddWaitDIN_Request;
using AddWaitDINResponse=::uwvm_custom::Ros2Rmi_msgsSrvAddWaitDIN_Response;
using AddWaitTimeClient=::uwvm_custom::Ros2Rmi_msgsSrvAddWaitTimeClient;
using AddWaitTimeServer=::uwvm_custom::Ros2Rmi_msgsSrvAddWaitTimeServer;
using AddWaitTimeRequest=::uwvm_custom::Ros2Rmi_msgsSrvAddWaitTime_Request;
using AddWaitTimeResponse=::uwvm_custom::Ros2Rmi_msgsSrvAddWaitTime_Response;
using CallCommandClient=::uwvm_custom::Ros2Rmi_msgsSrvCallCommandClient;
using CallCommandServer=::uwvm_custom::Ros2Rmi_msgsSrvCallCommandServer;
using CallCommandRequest=::uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Request;
using CallCommandResponse=::uwvm_custom::Ros2Rmi_msgsSrvCallCommand_Response;
}
namespace sensor_msgs {
using JointState=::uwvm_custom::Ros2Sensor_msgsMsgJointState;
}
namespace std_msgs {
using Header=::uwvm_custom::Ros2Std_msgsMsgHeader;
}
namespace std_srvs {
using TriggerClient=::uwvm_custom::Ros2Std_srvsSrvTriggerClient;
using TriggerServer=::uwvm_custom::Ros2Std_srvsSrvTriggerServer;
using TriggerRequest=::uwvm_custom::Ros2Std_srvsSrvTrigger_Request;
using TriggerResponse=::uwvm_custom::Ros2Std_srvsSrvTrigger_Response;
}
namespace trajectory_msgs {
using JointTrajectory=::uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory;
using JointTrajectoryPoint=::uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint;
using MultiDOFJointTrajectory=::uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory;
using MultiDOFJointTrajectoryPoint=::uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint;
}
}
