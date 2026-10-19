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
namespace uwvm_industrial::abb_ros1 {
namespace abb_egm_msgs {
using EGMChannelState=::uwvm_custom::Ros1Abb_egm_msgsEGMChannelState;
using EGMState=::uwvm_custom::Ros1Abb_egm_msgsEGMState;
}
namespace abb_rapid_msgs {
using ConfData=::uwvm_custom::Ros1Abb_rapid_msgsConfData;
using ExtJoint=::uwvm_custom::Ros1Abb_rapid_msgsExtJoint;
using JointTarget=::uwvm_custom::Ros1Abb_rapid_msgsJointTarget;
using LoadData=::uwvm_custom::Ros1Abb_rapid_msgsLoadData;
using Orient=::uwvm_custom::Ros1Abb_rapid_msgsOrient;
using Pos=::uwvm_custom::Ros1Abb_rapid_msgsPos;
using Pose=::uwvm_custom::Ros1Abb_rapid_msgsPose;
using RobJoint=::uwvm_custom::Ros1Abb_rapid_msgsRobJoint;
using RobTarget=::uwvm_custom::Ros1Abb_rapid_msgsRobTarget;
using ToolData=::uwvm_custom::Ros1Abb_rapid_msgsToolData;
using WObjData=::uwvm_custom::Ros1Abb_rapid_msgsWObjData;
}
namespace abb_rapid_sm_addin_msgs {
using EGMActivateSettings=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings;
using EGMRunSettings=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMRunSettings;
using EGMSettings=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMSettings;
using EGMSetupSettings=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings;
using EGMStopSettings=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsEGMStopSettings;
using GetEGMSettingsClient=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsClient;
using GetEGMSettingsServer=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsServer;
using GetEGMSettingsRequest=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest;
using GetEGMSettingsResponse=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse;
using RuntimeState=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsRuntimeState;
using SetEGMSettingsClient=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsClient;
using SetEGMSettingsServer=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsServer;
using SetEGMSettingsRequest=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest;
using SetEGMSettingsResponse=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse;
using SetRAPIDRoutineClient=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineClient;
using SetRAPIDRoutineServer=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineServer;
using SetRAPIDRoutineRequest=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest;
using SetRAPIDRoutineResponse=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse;
using SetSGCommandClient=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandClient;
using SetSGCommandServer=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandServer;
using SetSGCommandRequest=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest;
using SetSGCommandResponse=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse;
using StateMachineState=::uwvm_custom::Ros1Abb_rapid_sm_addin_msgsStateMachineState;
}
namespace abb_robot_msgs {
using GetFileContentsClient=::uwvm_custom::Ros1Abb_robot_msgsGetFileContentsClient;
using GetFileContentsServer=::uwvm_custom::Ros1Abb_robot_msgsGetFileContentsServer;
using GetFileContentsRequest=::uwvm_custom::Ros1Abb_robot_msgsGetFileContentsRequest;
using GetFileContentsResponse=::uwvm_custom::Ros1Abb_robot_msgsGetFileContentsResponse;
using GetIOSignalClient=::uwvm_custom::Ros1Abb_robot_msgsGetIOSignalClient;
using GetIOSignalServer=::uwvm_custom::Ros1Abb_robot_msgsGetIOSignalServer;
using GetIOSignalRequest=::uwvm_custom::Ros1Abb_robot_msgsGetIOSignalRequest;
using GetIOSignalResponse=::uwvm_custom::Ros1Abb_robot_msgsGetIOSignalResponse;
using GetRAPIDBoolClient=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolClient;
using GetRAPIDBoolServer=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolServer;
using GetRAPIDBoolRequest=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolRequest;
using GetRAPIDBoolResponse=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDBoolResponse;
using GetRAPIDDnumClient=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumClient;
using GetRAPIDDnumServer=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumServer;
using GetRAPIDDnumRequest=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumRequest;
using GetRAPIDDnumResponse=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDDnumResponse;
using GetRAPIDNumClient=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumClient;
using GetRAPIDNumServer=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumServer;
using GetRAPIDNumRequest=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumRequest;
using GetRAPIDNumResponse=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDNumResponse;
using GetRAPIDStringClient=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringClient;
using GetRAPIDStringServer=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringServer;
using GetRAPIDStringRequest=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringRequest;
using GetRAPIDStringResponse=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDStringResponse;
using GetRAPIDSymbolClient=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolClient;
using GetRAPIDSymbolServer=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolServer;
using GetRAPIDSymbolRequest=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolRequest;
using GetRAPIDSymbolResponse=::uwvm_custom::Ros1Abb_robot_msgsGetRAPIDSymbolResponse;
using GetRobotControllerDescriptionClient=::uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionClient;
using GetRobotControllerDescriptionServer=::uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionServer;
using GetRobotControllerDescriptionRequest=::uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest;
using GetRobotControllerDescriptionResponse=::uwvm_custom::Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse;
using GetSpeedRatioClient=::uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioClient;
using GetSpeedRatioServer=::uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioServer;
using GetSpeedRatioRequest=::uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioRequest;
using GetSpeedRatioResponse=::uwvm_custom::Ros1Abb_robot_msgsGetSpeedRatioResponse;
using MechanicalUnitState=::uwvm_custom::Ros1Abb_robot_msgsMechanicalUnitState;
using RAPIDSymbolPath=::uwvm_custom::Ros1Abb_robot_msgsRAPIDSymbolPath;
using RAPIDTaskState=::uwvm_custom::Ros1Abb_robot_msgsRAPIDTaskState;
using ServiceResponses=::uwvm_custom::Ros1Abb_robot_msgsServiceResponses;
using SetFileContentsClient=::uwvm_custom::Ros1Abb_robot_msgsSetFileContentsClient;
using SetFileContentsServer=::uwvm_custom::Ros1Abb_robot_msgsSetFileContentsServer;
using SetFileContentsRequest=::uwvm_custom::Ros1Abb_robot_msgsSetFileContentsRequest;
using SetFileContentsResponse=::uwvm_custom::Ros1Abb_robot_msgsSetFileContentsResponse;
using SetIOSignalClient=::uwvm_custom::Ros1Abb_robot_msgsSetIOSignalClient;
using SetIOSignalServer=::uwvm_custom::Ros1Abb_robot_msgsSetIOSignalServer;
using SetIOSignalRequest=::uwvm_custom::Ros1Abb_robot_msgsSetIOSignalRequest;
using SetIOSignalResponse=::uwvm_custom::Ros1Abb_robot_msgsSetIOSignalResponse;
using SetRAPIDBoolClient=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolClient;
using SetRAPIDBoolServer=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolServer;
using SetRAPIDBoolRequest=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolRequest;
using SetRAPIDBoolResponse=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDBoolResponse;
using SetRAPIDDnumClient=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumClient;
using SetRAPIDDnumServer=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumServer;
using SetRAPIDDnumRequest=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumRequest;
using SetRAPIDDnumResponse=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDDnumResponse;
using SetRAPIDNumClient=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumClient;
using SetRAPIDNumServer=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumServer;
using SetRAPIDNumRequest=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumRequest;
using SetRAPIDNumResponse=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDNumResponse;
using SetRAPIDStringClient=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringClient;
using SetRAPIDStringServer=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringServer;
using SetRAPIDStringRequest=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringRequest;
using SetRAPIDStringResponse=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDStringResponse;
using SetRAPIDSymbolClient=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolClient;
using SetRAPIDSymbolServer=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolServer;
using SetRAPIDSymbolRequest=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolRequest;
using SetRAPIDSymbolResponse=::uwvm_custom::Ros1Abb_robot_msgsSetRAPIDSymbolResponse;
using SetSpeedRatioClient=::uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioClient;
using SetSpeedRatioServer=::uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioServer;
using SetSpeedRatioRequest=::uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioRequest;
using SetSpeedRatioResponse=::uwvm_custom::Ros1Abb_robot_msgsSetSpeedRatioResponse;
using SystemState=::uwvm_custom::Ros1Abb_robot_msgsSystemState;
using TriggerWithResultCodeClient=::uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeClient;
using TriggerWithResultCodeServer=::uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeServer;
using TriggerWithResultCodeRequest=::uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeRequest;
using TriggerWithResultCodeResponse=::uwvm_custom::Ros1Abb_robot_msgsTriggerWithResultCodeResponse;
}
namespace control_msgs {
using FollowJointTrajectoryClient=::uwvm_custom::Ros1Control_msgsFollowJointTrajectoryClient;
using FollowJointTrajectoryServer=::uwvm_custom::Ros1Control_msgsFollowJointTrajectoryServer;
using FollowJointTrajectoryGoal=::uwvm_custom::Ros1Control_msgsFollowJointTrajectoryGoal;
using FollowJointTrajectoryResult=::uwvm_custom::Ros1Control_msgsFollowJointTrajectoryResult;
using FollowJointTrajectoryFeedback=::uwvm_custom::Ros1Control_msgsFollowJointTrajectoryFeedback;
using JointTolerance=::uwvm_custom::Ros1Control_msgsJointTolerance;
}
namespace sensor_msgs {
using JointState=::uwvm_custom::Ros1Sensor_msgsJointState;
}
namespace std_msgs {
using Header=::uwvm_custom::Ros1Std_msgsHeader;
}
namespace trajectory_msgs {
using JointTrajectory=::uwvm_custom::Ros1Trajectory_msgsJointTrajectory;
using JointTrajectoryPoint=::uwvm_custom::Ros1Trajectory_msgsJointTrajectoryPoint;
}
}
