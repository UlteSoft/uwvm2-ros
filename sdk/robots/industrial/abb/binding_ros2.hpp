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
namespace uwvm_industrial::abb_ros2 {
namespace abb_egm_msgs {
using EGMChannelState=::uwvm_custom::Ros2Abb_egm_msgsMsgEGMChannelState;
using EGMState=::uwvm_custom::Ros2Abb_egm_msgsMsgEGMState;
}
namespace abb_rapid_msgs {
using ConfData=::uwvm_custom::Ros2Abb_rapid_msgsMsgConfData;
using ExtJoint=::uwvm_custom::Ros2Abb_rapid_msgsMsgExtJoint;
using JointTarget=::uwvm_custom::Ros2Abb_rapid_msgsMsgJointTarget;
using LoadData=::uwvm_custom::Ros2Abb_rapid_msgsMsgLoadData;
using Orient=::uwvm_custom::Ros2Abb_rapid_msgsMsgOrient;
using Pos=::uwvm_custom::Ros2Abb_rapid_msgsMsgPos;
using Pose=::uwvm_custom::Ros2Abb_rapid_msgsMsgPose;
using RobJoint=::uwvm_custom::Ros2Abb_rapid_msgsMsgRobJoint;
using RobTarget=::uwvm_custom::Ros2Abb_rapid_msgsMsgRobTarget;
using ToolData=::uwvm_custom::Ros2Abb_rapid_msgsMsgToolData;
using WObjData=::uwvm_custom::Ros2Abb_rapid_msgsMsgWObjData;
}
namespace abb_rapid_sm_addin_msgs {
using EGMActivateSettings=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings;
using EGMRunSettings=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings;
using EGMSettings=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings;
using EGMSetupSettings=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings;
using EGMStopSettings=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings;
using RuntimeState=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState;
using StateMachineState=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState;
using GetEGMSettingsClient=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsClient;
using GetEGMSettingsServer=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettingsServer;
using GetEGMSettingsRequest=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request;
using GetEGMSettingsResponse=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response;
using SetEGMSettingsClient=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsClient;
using SetEGMSettingsServer=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettingsServer;
using SetEGMSettingsRequest=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request;
using SetEGMSettingsResponse=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response;
using SetRAPIDRoutineClient=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineClient;
using SetRAPIDRoutineServer=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutineServer;
using SetRAPIDRoutineRequest=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request;
using SetRAPIDRoutineResponse=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response;
using SetSGCommandClient=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandClient;
using SetSGCommandServer=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommandServer;
using SetSGCommandRequest=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request;
using SetSGCommandResponse=::uwvm_custom::Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response;
}
namespace abb_robot_msgs {
using MechanicalUnitState=::uwvm_custom::Ros2Abb_robot_msgsMsgMechanicalUnitState;
using RAPIDSymbolPath=::uwvm_custom::Ros2Abb_robot_msgsMsgRAPIDSymbolPath;
using RAPIDTaskState=::uwvm_custom::Ros2Abb_robot_msgsMsgRAPIDTaskState;
using ServiceResponses=::uwvm_custom::Ros2Abb_robot_msgsMsgServiceResponses;
using SystemState=::uwvm_custom::Ros2Abb_robot_msgsMsgSystemState;
using GetFileContentsClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContentsClient;
using GetFileContentsServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContentsServer;
using GetFileContentsRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContents_Request;
using GetFileContentsResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetFileContents_Response;
using GetIOSignalClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignalClient;
using GetIOSignalServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignalServer;
using GetIOSignalRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignal_Request;
using GetIOSignalResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetIOSignal_Response;
using GetRAPIDBoolClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBoolClient;
using GetRAPIDBoolServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBoolServer;
using GetRAPIDBoolRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBool_Request;
using GetRAPIDBoolResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDBool_Response;
using GetRAPIDDnumClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnumClient;
using GetRAPIDDnumServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnumServer;
using GetRAPIDDnumRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request;
using GetRAPIDDnumResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response;
using GetRAPIDNumClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNumClient;
using GetRAPIDNumServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNumServer;
using GetRAPIDNumRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNum_Request;
using GetRAPIDNumResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDNum_Response;
using GetRAPIDStringClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDStringClient;
using GetRAPIDStringServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDStringServer;
using GetRAPIDStringRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDString_Request;
using GetRAPIDStringResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDString_Response;
using GetRAPIDSymbolClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbolClient;
using GetRAPIDSymbolServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbolServer;
using GetRAPIDSymbolRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request;
using GetRAPIDSymbolResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response;
using GetRobotControllerDescriptionClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionClient;
using GetRobotControllerDescriptionServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescriptionServer;
using GetRobotControllerDescriptionRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request;
using GetRobotControllerDescriptionResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response;
using GetSpeedRatioClient=::uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatioClient;
using GetSpeedRatioServer=::uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatioServer;
using GetSpeedRatioRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatio_Request;
using GetSpeedRatioResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvGetSpeedRatio_Response;
using SetFileContentsClient=::uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContentsClient;
using SetFileContentsServer=::uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContentsServer;
using SetFileContentsRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContents_Request;
using SetFileContentsResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvSetFileContents_Response;
using SetIOSignalClient=::uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignalClient;
using SetIOSignalServer=::uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignalServer;
using SetIOSignalRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignal_Request;
using SetIOSignalResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvSetIOSignal_Response;
using SetRAPIDBoolClient=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBoolClient;
using SetRAPIDBoolServer=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBoolServer;
using SetRAPIDBoolRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBool_Request;
using SetRAPIDBoolResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDBool_Response;
using SetRAPIDDnumClient=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnumClient;
using SetRAPIDDnumServer=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnumServer;
using SetRAPIDDnumRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request;
using SetRAPIDDnumResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response;
using SetRAPIDNumClient=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNumClient;
using SetRAPIDNumServer=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNumServer;
using SetRAPIDNumRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNum_Request;
using SetRAPIDNumResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDNum_Response;
using SetRAPIDStringClient=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDStringClient;
using SetRAPIDStringServer=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDStringServer;
using SetRAPIDStringRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDString_Request;
using SetRAPIDStringResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDString_Response;
using SetRAPIDSymbolClient=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbolClient;
using SetRAPIDSymbolServer=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbolServer;
using SetRAPIDSymbolRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request;
using SetRAPIDSymbolResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response;
using SetSpeedRatioClient=::uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatioClient;
using SetSpeedRatioServer=::uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatioServer;
using SetSpeedRatioRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatio_Request;
using SetSpeedRatioResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvSetSpeedRatio_Response;
using TriggerWithResultCodeClient=::uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCodeClient;
using TriggerWithResultCodeServer=::uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCodeServer;
using TriggerWithResultCodeRequest=::uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request;
using TriggerWithResultCodeResponse=::uwvm_custom::Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response;
}
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
namespace geometry_msgs {
using Quaternion=::uwvm_custom::Ros2Geometry_msgsMsgQuaternion;
using Transform=::uwvm_custom::Ros2Geometry_msgsMsgTransform;
using Twist=::uwvm_custom::Ros2Geometry_msgsMsgTwist;
using Vector3=::uwvm_custom::Ros2Geometry_msgsMsgVector3;
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
