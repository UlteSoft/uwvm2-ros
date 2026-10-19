/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for franka_msgs/action/ErrorRecovery_Goal. */
using Ros2Franka_msgsActionErrorRecovery_Goal=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal;
/** Complete bounded guest value for franka_msgs/action/ErrorRecovery_Result. */
using Ros2Franka_msgsActionErrorRecovery_Result=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result;
/** Complete bounded guest value for franka_msgs/action/ErrorRecovery_Feedback. */
using Ros2Franka_msgsActionErrorRecovery_Feedback=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback;
/** Complete bounded guest value for franka_msgs/msg/GraspEpsilon. */
using Ros2Franka_msgsMsgGraspEpsilon=roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon;
/** Complete bounded guest value for franka_msgs/action/Grasp_Goal. */
using Ros2Franka_msgsActionGrasp_Goal=roswasmc_bounded_2_franka_msgs_action_Grasp_Goal;
/** Complete bounded guest value for franka_msgs/action/Grasp_Result. */
using Ros2Franka_msgsActionGrasp_Result=roswasmc_bounded_2_franka_msgs_action_Grasp_Result;
/** Complete bounded guest value for franka_msgs/action/Grasp_Feedback. */
using Ros2Franka_msgsActionGrasp_Feedback=roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback;
/** Complete bounded guest value for franka_msgs/action/Homing_Goal. */
using Ros2Franka_msgsActionHoming_Goal=roswasmc_bounded_2_franka_msgs_action_Homing_Goal;
/** Complete bounded guest value for franka_msgs/action/Homing_Result. */
using Ros2Franka_msgsActionHoming_Result=roswasmc_bounded_2_franka_msgs_action_Homing_Result;
/** Complete bounded guest value for franka_msgs/action/Homing_Feedback. */
using Ros2Franka_msgsActionHoming_Feedback=roswasmc_bounded_2_franka_msgs_action_Homing_Feedback;
/** Complete bounded guest value for franka_msgs/action/Move_Goal. */
using Ros2Franka_msgsActionMove_Goal=roswasmc_bounded_2_franka_msgs_action_Move_Goal;
/** Complete bounded guest value for franka_msgs/action/Move_Result. */
using Ros2Franka_msgsActionMove_Result=roswasmc_bounded_2_franka_msgs_action_Move_Result;
/** Complete bounded guest value for franka_msgs/action/Move_Feedback. */
using Ros2Franka_msgsActionMove_Feedback=roswasmc_bounded_2_franka_msgs_action_Move_Feedback;
/** Complete bounded guest value for franka_msgs/action/PTPMotion_Goal. */
using Ros2Franka_msgsActionPTPMotion_Goal=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal;
/** Complete bounded guest value for franka_msgs/msg/TargetStatus. */
using Ros2Franka_msgsMsgTargetStatus=roswasmc_bounded_2_franka_msgs_msg_TargetStatus;
/** Complete bounded guest value for franka_msgs/action/PTPMotion_Result. */
using Ros2Franka_msgsActionPTPMotion_Result=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result;
/** Complete bounded guest value for franka_msgs/action/PTPMotion_Feedback. */
using Ros2Franka_msgsActionPTPMotion_Feedback=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback;
/** Complete bounded guest value for geometry_msgs/msg/Vector3. */
using Ros2Geometry_msgsMsgVector3=roswasmc_bounded_2_geometry_msgs_msg_Vector3;
/** Complete bounded guest value for franka_msgs/msg/CollisionIndicators. */
using Ros2Franka_msgsMsgCollisionIndicators=roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators;
/** Complete bounded guest value for franka_msgs/msg/Elbow. */
using Ros2Franka_msgsMsgElbow=roswasmc_bounded_2_franka_msgs_msg_Elbow;
/** Complete bounded guest value for franka_msgs/msg/Errors. */
using Ros2Franka_msgsMsgErrors=roswasmc_bounded_2_franka_msgs_msg_Errors;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for sensor_msgs/msg/JointState. */
using Ros2Sensor_msgsMsgJointState=roswasmc_bounded_2_sensor_msgs_msg_JointState;
/** Complete bounded guest value for geometry_msgs/msg/Wrench. */
using Ros2Geometry_msgsMsgWrench=roswasmc_bounded_2_geometry_msgs_msg_Wrench;
/** Complete bounded guest value for geometry_msgs/msg/WrenchStamped. */
using Ros2Geometry_msgsMsgWrenchStamped=roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped;
/** Complete bounded guest value for geometry_msgs/msg/Inertia. */
using Ros2Geometry_msgsMsgInertia=roswasmc_bounded_2_geometry_msgs_msg_Inertia;
/** Complete bounded guest value for geometry_msgs/msg/InertiaStamped. */
using Ros2Geometry_msgsMsgInertiaStamped=roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped;
/** Complete bounded guest value for geometry_msgs/msg/Point. */
using Ros2Geometry_msgsMsgPoint=roswasmc_bounded_2_geometry_msgs_msg_Point;
/** Complete bounded guest value for geometry_msgs/msg/Quaternion. */
using Ros2Geometry_msgsMsgQuaternion=roswasmc_bounded_2_geometry_msgs_msg_Quaternion;
/** Complete bounded guest value for geometry_msgs/msg/Pose. */
using Ros2Geometry_msgsMsgPose=roswasmc_bounded_2_geometry_msgs_msg_Pose;
/** Complete bounded guest value for geometry_msgs/msg/PoseStamped. */
using Ros2Geometry_msgsMsgPoseStamped=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped;
/** Complete bounded guest value for geometry_msgs/msg/Twist. */
using Ros2Geometry_msgsMsgTwist=roswasmc_bounded_2_geometry_msgs_msg_Twist;
/** Complete bounded guest value for geometry_msgs/msg/TwistStamped. */
using Ros2Geometry_msgsMsgTwistStamped=roswasmc_bounded_2_geometry_msgs_msg_TwistStamped;
/** Complete bounded guest value for geometry_msgs/msg/Accel. */
using Ros2Geometry_msgsMsgAccel=roswasmc_bounded_2_geometry_msgs_msg_Accel;
/** Complete bounded guest value for geometry_msgs/msg/AccelStamped. */
using Ros2Geometry_msgsMsgAccelStamped=roswasmc_bounded_2_geometry_msgs_msg_AccelStamped;
/** Complete bounded guest value for geometry_msgs/msg/Vector3Stamped. */
using Ros2Geometry_msgsMsgVector3Stamped=roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped;
/** Complete bounded guest value for franka_msgs/msg/FrankaRobotState. */
using Ros2Franka_msgsMsgFrankaRobotState=roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState;
/** Complete bounded guest value for franka_msgs/srv/SetCartesianStiffness_Request. */
using Ros2Franka_msgsSrvSetCartesianStiffness_Request=roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request;
/** Complete bounded guest value for franka_msgs/srv/SetCartesianStiffness_Response. */
using Ros2Franka_msgsSrvSetCartesianStiffness_Response=roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response;
/** Complete bounded guest value for franka_msgs/srv/SetForceTorqueCollisionBehavior_Request. */
using Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request=roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request;
/** Complete bounded guest value for franka_msgs/srv/SetForceTorqueCollisionBehavior_Response. */
using Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response=roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response;
/** Complete bounded guest value for franka_msgs/srv/SetFullCollisionBehavior_Request. */
using Ros2Franka_msgsSrvSetFullCollisionBehavior_Request=roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request;
/** Complete bounded guest value for franka_msgs/srv/SetFullCollisionBehavior_Response. */
using Ros2Franka_msgsSrvSetFullCollisionBehavior_Response=roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response;
/** Complete bounded guest value for franka_msgs/srv/SetJointStiffness_Request. */
using Ros2Franka_msgsSrvSetJointStiffness_Request=roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request;
/** Complete bounded guest value for franka_msgs/srv/SetJointStiffness_Response. */
using Ros2Franka_msgsSrvSetJointStiffness_Response=roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response;
/** Complete bounded guest value for franka_msgs/srv/SetLoad_Request. */
using Ros2Franka_msgsSrvSetLoad_Request=roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request;
/** Complete bounded guest value for franka_msgs/srv/SetLoad_Response. */
using Ros2Franka_msgsSrvSetLoad_Response=roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response;
/** Complete bounded guest value for franka_msgs/srv/SetStiffnessFrame_Request. */
using Ros2Franka_msgsSrvSetStiffnessFrame_Request=roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request;
/** Complete bounded guest value for franka_msgs/srv/SetStiffnessFrame_Response. */
using Ros2Franka_msgsSrvSetStiffnessFrame_Response=roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response;
/** Complete bounded guest value for franka_msgs/srv/SetTCPFrame_Request. */
using Ros2Franka_msgsSrvSetTCPFrame_Request=roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request;
/** Complete bounded guest value for franka_msgs/srv/SetTCPFrame_Response. */
using Ros2Franka_msgsSrvSetTCPFrame_Response=roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response;
/** Complete bounded guest value for franka_spine_msgs/action/MoveAbsolute_Goal. */
using Ros2Franka_spine_msgsActionMoveAbsolute_Goal=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal;
/** Complete bounded guest value for franka_spine_msgs/action/MoveAbsolute_Result. */
using Ros2Franka_spine_msgsActionMoveAbsolute_Result=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result;
/** Complete bounded guest value for franka_spine_msgs/action/MoveAbsolute_Feedback. */
using Ros2Franka_spine_msgsActionMoveAbsolute_Feedback=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback;
/** Complete bounded guest value for franka_spine_msgs/msg/UserLimits. */
using Ros2Franka_spine_msgsMsgUserLimits=roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits;
/** Complete bounded guest value for franka_spine_msgs/msg/SpineParameters. */
using Ros2Franka_spine_msgsMsgSpineParameters=roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters;
/** Complete bounded guest value for franka_spine_msgs/msg/SpinePosition. */
using Ros2Franka_spine_msgsMsgSpinePosition=roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition;
/** Complete bounded guest value for franka_spine_msgs/msg/SpineState. */
using Ros2Franka_spine_msgsMsgSpineState=roswasmc_bounded_2_franka_spine_msgs_msg_SpineState;
/** Complete bounded guest value for franka_spine_msgs/srv/FaultReset_Request. */
using Ros2Franka_spine_msgsSrvFaultReset_Request=roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request;
/** Complete bounded guest value for franka_spine_msgs/srv/FaultReset_Response. */
using Ros2Franka_spine_msgsSrvFaultReset_Response=roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response;
/** Complete bounded guest value for franka_spine_msgs/srv/GetParameters_Request. */
using Ros2Franka_spine_msgsSrvGetParameters_Request=roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request;
/** Complete bounded guest value for franka_spine_msgs/srv/GetParameters_Response. */
using Ros2Franka_spine_msgsSrvGetParameters_Response=roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response;
/** Complete bounded guest value for franka_spine_msgs/srv/GetPosition_Request. */
using Ros2Franka_spine_msgsSrvGetPosition_Request=roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request;
/** Complete bounded guest value for franka_spine_msgs/srv/GetPosition_Response. */
using Ros2Franka_spine_msgsSrvGetPosition_Response=roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response;
/** Complete bounded guest value for franka_spine_msgs/srv/GetSpineState_Request. */
using Ros2Franka_spine_msgsSrvGetSpineState_Request=roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request;
/** Complete bounded guest value for franka_spine_msgs/srv/GetSpineState_Response. */
using Ros2Franka_spine_msgsSrvGetSpineState_Response=roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response;
/** Complete bounded guest value for franka_spine_msgs/srv/Halt_Request. */
using Ros2Franka_spine_msgsSrvHalt_Request=roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request;
/** Complete bounded guest value for franka_spine_msgs/srv/Halt_Response. */
using Ros2Franka_spine_msgsSrvHalt_Response=roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response;
/** Complete bounded guest value for franka_spine_msgs/srv/SwitchOff_Request. */
using Ros2Franka_spine_msgsSrvSwitchOff_Request=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request;
/** Complete bounded guest value for franka_spine_msgs/srv/SwitchOff_Response. */
using Ros2Franka_spine_msgsSrvSwitchOff_Response=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response;
/** Complete bounded guest value for franka_spine_msgs/srv/SwitchOn_Request. */
using Ros2Franka_spine_msgsSrvSwitchOn_Request=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request;
/** Complete bounded guest value for franka_spine_msgs/srv/SwitchOn_Response. */
using Ros2Franka_spine_msgsSrvSwitchOn_Response=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response;
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
/** Checked codec for franka_msgs/action/ErrorRecovery_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Goal> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_2caee3e8d5c04f12239a71f6e9a9259b818b9f1ad75103442bfc9d2343a3a070";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Goal& out) {return !roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/ErrorRecovery_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Result> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_9112b16c42034737b96214f23249d499ab6a9f0aee529cfec34c67cdc76809e9";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Result& out) {return !roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/ErrorRecovery_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Feedback> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_cc817bcbe5790860e83c71e50a8c3610cf3965e34036b3099712cdcdb426127f";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionErrorRecovery_Feedback& out) {return !roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/msg/GraspEpsilon; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsMsgGraspEpsilon> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_21787ae5929dc83b9f644f4914e22da9ddd3e4f7ad3d8c6cda6bbcdfeba20c38";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Franka_msgsMsgGraspEpsilon default_value() noexcept {uwvm_custom::Ros2Franka_msgsMsgGraspEpsilon value={0};(void)roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Franka_msgsMsgGraspEpsilon const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsMsgGraspEpsilon& out) {return !roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Grasp_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionGrasp_Goal> {
  static constexpr size_t max_wire_bytes=40;
  static constexpr char const* codec_identity="UWVMROS1_b7ef701e9fd93b5da9894499234d3647ecc4cea1936f637105c172f4579b6e1f";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Franka_msgsActionGrasp_Goal default_value() noexcept {uwvm_custom::Ros2Franka_msgsActionGrasp_Goal value={0};(void)roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Franka_msgsActionGrasp_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionGrasp_Goal& out) {return !roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Grasp_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionGrasp_Result> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_9a4c3150c0c2d5eb8ff3dad403473a9546ee37cc3086a55a628a1a85b905b733";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionGrasp_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Grasp_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Grasp_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Grasp_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionGrasp_Result& out) {return !roswasmc_bounded_2_franka_msgs_action_Grasp_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Grasp_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionGrasp_Feedback> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_2e7697286c002e2d891e9c3167aa0dd1664acfc66c649fff6e8f9101c42703fe";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionGrasp_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionGrasp_Feedback& out) {return !roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Homing_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionHoming_Goal> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_8008426ce3fa8c8d62133704f8738201b8154b324ba07e7b6f605cd3132a885d";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionHoming_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Homing_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Homing_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Homing_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionHoming_Goal& out) {return !roswasmc_bounded_2_franka_msgs_action_Homing_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Homing_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionHoming_Result> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_74a9677213ffa9ba3a26b3053956350b8f75d5d1769152e5e3d4d26370bcd77c";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionHoming_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Homing_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Homing_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Homing_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionHoming_Result& out) {return !roswasmc_bounded_2_franka_msgs_action_Homing_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Homing_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionHoming_Feedback> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_2260e092d369a0d2e4bf97ca2dfa78c02b6836b8167e71d63e76b5c0c40a6b46";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionHoming_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionHoming_Feedback& out) {return !roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Move_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionMove_Goal> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_9f9e308ebd034344ee73b22a07d005944975c037943279504b2414bdd9ad206f";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionMove_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Move_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Move_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Move_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionMove_Goal& out) {return !roswasmc_bounded_2_franka_msgs_action_Move_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Move_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionMove_Result> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_66aa505c035357274c68be773e1b502c7d77f195f3dca848e818811ce95dd266";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionMove_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Move_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Move_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Move_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionMove_Result& out) {return !roswasmc_bounded_2_franka_msgs_action_Move_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/Move_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionMove_Feedback> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_18d0e658f109492380c135e787adbdcc026b051e9a547fcea70453bbd9750596";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionMove_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_Move_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_Move_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_Move_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionMove_Feedback& out) {return !roswasmc_bounded_2_franka_msgs_action_Move_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/PTPMotion_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionPTPMotion_Goal> {
  static constexpr size_t max_wire_bytes=128;
  static constexpr char const* codec_identity="UWVMROS1_7e1b02257a7be37e7bccd0870ebced1e1560e7250eaeaa4547836e721f2ea2cc";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionPTPMotion_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionPTPMotion_Goal& out) {return !roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/msg/TargetStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsMsgTargetStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_df49eedc13cd8be2055361d8640c519bfa018ed2a421aa2b59b35c8828eac585";
  static bool encode(uwvm_custom::Ros2Franka_msgsMsgTargetStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_msg_TargetStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_msg_TargetStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_msg_TargetStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsMsgTargetStatus& out) {return !roswasmc_bounded_2_franka_msgs_msg_TargetStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/PTPMotion_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionPTPMotion_Result> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_b61fabf13a3d8d910a35a5145e87b4dd80180b0678ef5580784f461e850fa4c1";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionPTPMotion_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionPTPMotion_Result& out) {return !roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/action/PTPMotion_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsActionPTPMotion_Feedback> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_c4dcfeb668595ebc341d76b5cbc606c1ae4c38b28a1b329f8436e61f506a0300";
  static bool encode(uwvm_custom::Ros2Franka_msgsActionPTPMotion_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsActionPTPMotion_Feedback& out) {return !roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for franka_msgs/msg/CollisionIndicators; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsMsgCollisionIndicators> {
  static constexpr size_t max_wire_bytes=208;
  static constexpr char const* codec_identity="UWVMROS1_2bdfb3ece01b47b07fae84a998b23c56fc9482a0518969e43304b318d4ee0ce1";
  static bool encode(uwvm_custom::Ros2Franka_msgsMsgCollisionIndicators const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsMsgCollisionIndicators& out) {return !roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/msg/Elbow; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsMsgElbow> {
  static constexpr size_t max_wire_bytes=80;
  static constexpr char const* codec_identity="UWVMROS1_686cf2da29007046f9855ffeecac304c159245ec3da153990ab83be4ffce53b6";
  static bool encode(uwvm_custom::Ros2Franka_msgsMsgElbow const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_msg_Elbow_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_msg_Elbow_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_msg_Elbow_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsMsgElbow& out) {return !roswasmc_bounded_2_franka_msgs_msg_Elbow_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/msg/Errors; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsMsgErrors> {
  static constexpr size_t max_wire_bytes=36;
  static constexpr char const* codec_identity="UWVMROS1_c7927af1abe7710bb686574ad6f2459f8425af1d6cdb3189160bb732b5a6bc8c";
  static bool encode(uwvm_custom::Ros2Franka_msgsMsgErrors const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_msg_Errors_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_msg_Errors_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_msg_Errors_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsMsgErrors& out) {return !roswasmc_bounded_2_franka_msgs_msg_Errors_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for sensor_msgs/msg/JointState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgJointState> {
  static constexpr size_t max_wire_bytes=1248;
  static constexpr char const* codec_identity="UWVMROS1_912ba4261e2dfbec0e4a543ea8e5d308ef1d26877b4a7d71162a981ac64c40a3";
  static bool encode(uwvm_custom::Ros2Sensor_msgsMsgJointState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&value))return false;size_t n=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_sensor_msgs_msg_JointState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Sensor_msgsMsgJointState& out) {return !roswasmc_bounded_2_sensor_msgs_msg_JointState_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for geometry_msgs/msg/WrenchStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgWrenchStamped> {
  static constexpr size_t max_wire_bytes=188;
  static constexpr char const* codec_identity="UWVMROS1_e23600874a626ba9d52f043e14ed04beb3e316d47c3bca8bd87cfeed3f5d9d66";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgWrenchStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgWrenchStamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/Inertia; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgInertia> {
  static constexpr size_t max_wire_bytes=80;
  static constexpr char const* codec_identity="UWVMROS1_4a1b7a6a2516724a9a12e5eb166a85cd73d33fc6075c8d517ff2a279fd29e12a";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgInertia const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Inertia_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Inertia_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Inertia_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgInertia& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Inertia_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/InertiaStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgInertiaStamped> {
  static constexpr size_t max_wire_bytes=220;
  static constexpr char const* codec_identity="UWVMROS1_19b0b442c5858ff1eee20e4658ecc3a2b76fdd64c4a21a1642fae5ab38198576";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgInertiaStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgInertiaStamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for geometry_msgs/msg/Accel; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgAccel> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_87264cb87970315248345c43cc629bbc0c9f9b2a8c6c4bc089da81c7af552fde";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgAccel const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_Accel_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_Accel_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_Accel_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgAccel& out) {return !roswasmc_bounded_2_geometry_msgs_msg_Accel_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/msg/AccelStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Geometry_msgsMsgAccelStamped> {
  static constexpr size_t max_wire_bytes=188;
  static constexpr char const* codec_identity="UWVMROS1_c04d3ae310bd82f7c4c9f30c2ff185ab54989aa3adcc759c7d3eb19853819899";
  static bool encode(uwvm_custom::Ros2Geometry_msgsMsgAccelStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Geometry_msgsMsgAccelStamped& out) {return !roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for franka_msgs/msg/FrankaRobotState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsMsgFrankaRobotState> {
  static constexpr size_t max_wire_bytes=10169;
  static constexpr char const* codec_identity="UWVMROS1_0fdc5c99a098f4c891956bb6cd81505995d5dda41a740f88ee456f48f6e07282";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2Franka_msgsMsgFrankaRobotState default_value() noexcept {uwvm_custom::Ros2Franka_msgsMsgFrankaRobotState value={0};(void)roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2Franka_msgsMsgFrankaRobotState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsMsgFrankaRobotState& out) {return !roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetCartesianStiffness_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetCartesianStiffness_Request> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_ae78b9887a5419225cfe0727bac83d8c53f789acd53bd47f024485b02aa98260";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetCartesianStiffness_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetCartesianStiffness_Request& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetCartesianStiffness_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetCartesianStiffness_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_3194f7469bbf228e653f5a93336fd5de4a1b2d7c1fe9542f3fadc8ad4fd51afa";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetCartesianStiffness_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetCartesianStiffness_Response& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetForceTorqueCollisionBehavior_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request> {
  static constexpr size_t max_wire_bytes=208;
  static constexpr char const* codec_identity="UWVMROS1_829642a9ea418a53f7252f7489bd7f5ed976bf3689443012daf216db895cf99c";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetForceTorqueCollisionBehavior_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_7d5640c34446e4262a8c00f0b242b68c0095688f0208fd692f21d855003eb47a";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetFullCollisionBehavior_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetFullCollisionBehavior_Request> {
  static constexpr size_t max_wire_bytes=416;
  static constexpr char const* codec_identity="UWVMROS1_d6898555d931aee3a5b4f5f97830cb9982f2e378c59c40bcaa0201840d2f2824";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetFullCollisionBehavior_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetFullCollisionBehavior_Request& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetFullCollisionBehavior_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetFullCollisionBehavior_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_76430e384e9ad8b3f3c937e3004e8dc15a51d28abecd108c698e60d39c54f300";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetFullCollisionBehavior_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetFullCollisionBehavior_Response& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetJointStiffness_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetJointStiffness_Request> {
  static constexpr size_t max_wire_bytes=56;
  static constexpr char const* codec_identity="UWVMROS1_65b5d6599d1b97a0356f599102c22e0ae8c33b85f14c5a49b6c097eb227d334c";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetJointStiffness_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetJointStiffness_Request& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetJointStiffness_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetJointStiffness_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_1a857ab809079e0eec664250a9521ddc1cdf66e61aa2e0f22eae8442f5231c4d";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetJointStiffness_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetJointStiffness_Response& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetLoad_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetLoad_Request> {
  static constexpr size_t max_wire_bytes=104;
  static constexpr char const* codec_identity="UWVMROS1_a616a844864b7bbf4ee18ac481ddd8406bec47354b4519a0e17f8a305635b7f8";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetLoad_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetLoad_Request& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetLoad_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetLoad_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_991ff575719ef9442063a13c45a9660c672f5607e6e40cf6ab5c7a3173fa00bf";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetLoad_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetLoad_Response& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetStiffnessFrame_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetStiffnessFrame_Request> {
  static constexpr size_t max_wire_bytes=128;
  static constexpr char const* codec_identity="UWVMROS1_f8ee91daba1642286b63178c18bf2a005597e94951f11a185241193132c9f402";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetStiffnessFrame_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetStiffnessFrame_Request& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetStiffnessFrame_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetStiffnessFrame_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_e32cb83b1f0f3cda363566525c0b1fe367190f2e66f68282b330f4517f6ea744";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetStiffnessFrame_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetStiffnessFrame_Response& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetTCPFrame_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetTCPFrame_Request> {
  static constexpr size_t max_wire_bytes=128;
  static constexpr char const* codec_identity="UWVMROS1_2ef445bfa494a6bc50ee3337695562255c06890216c30ba17df95e691871441f";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetTCPFrame_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetTCPFrame_Request& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_msgs/srv/SetTCPFrame_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_msgsSrvSetTCPFrame_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_87e4f5556c292289fe17898a40f1f5dd6161e7cc87ca570c86e9f69080d1df38";
  static bool encode(uwvm_custom::Ros2Franka_msgsSrvSetTCPFrame_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_msgsSrvSetTCPFrame_Response& out) {return !roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/action/MoveAbsolute_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Goal> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_a0c2f9037fd6730641b19b6da6ce1c6ca03bbd517fdd791c159effc93290cf13";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Goal& out) {return !roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/action/MoveAbsolute_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Result> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_86b5c823e6e6604595462a76cd259daef420b2dff169f6dbe00f742f079155d5";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Result& out) {return !roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/action/MoveAbsolute_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Feedback> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_cf2f760313ab8c6cc01dd79271619926320021746451f376e56d18f63ffad929";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsActionMoveAbsolute_Feedback& out) {return !roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/msg/UserLimits; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsMsgUserLimits> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_20256e37f0ae710b5cb9ab9438b5ff8144ce2ab6fc606c5335d0f5de5384e368";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsMsgUserLimits const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsMsgUserLimits& out) {return !roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/msg/SpineParameters; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsMsgSpineParameters> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_4d4c45cfccf278c99a189e92d438d0b11b2b693012cb0f76b65b6822eb816223";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsMsgSpineParameters const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsMsgSpineParameters& out) {return !roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/msg/SpinePosition; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsMsgSpinePosition> {
  static constexpr size_t max_wire_bytes=149;
  static constexpr char const* codec_identity="UWVMROS1_71be15c40f39074dfdbe79a3587847ae512c5a3c8df23cc9415d5bfe59d05775";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsMsgSpinePosition const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsMsgSpinePosition& out) {return !roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/msg/SpineState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsMsgSpineState> {
  static constexpr size_t max_wire_bytes=921;
  static constexpr char const* codec_identity="UWVMROS1_ad0b5dc5d79956da854582765c147d3d33edab569c4829e69cc7990edfb89441";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsMsgSpineState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsMsgSpineState& out) {return !roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/FaultReset_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvFaultReset_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_0f836229e8e8a1713b9e9a8d7f0f43677ad93380e1f7350ff1597d25067b7f33";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvFaultReset_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvFaultReset_Request& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/FaultReset_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvFaultReset_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_1ebeded0dbe4a4197a52de3fa43dc57fa372bd4e6ef5e2ded2558ccbf691410c";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvFaultReset_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvFaultReset_Response& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/GetParameters_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvGetParameters_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_80ef5d2ef6ff2df9d14f29417d1928429315c0b7be9ec59513b9b3943a970b14";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvGetParameters_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvGetParameters_Request& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/GetParameters_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvGetParameters_Response> {
  static constexpr size_t max_wire_bytes=277;
  static constexpr char const* codec_identity="UWVMROS1_8e648792534d7f4996179f31d353d2594cf63031edef4451fd20a3838dc896ed";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvGetParameters_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvGetParameters_Response& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/GetPosition_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvGetPosition_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_66b05d1f08331161cafed301b3ab95a60dbb16aeb1a930ee780db163cb37c6c2";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvGetPosition_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvGetPosition_Request& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/GetPosition_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvGetPosition_Response> {
  static constexpr size_t max_wire_bytes=9;
  static constexpr char const* codec_identity="UWVMROS1_18d9af57cc9f82c3c03d01e850da254e9431820b8291f03dafd2e8f74e849ab8";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvGetPosition_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvGetPosition_Response& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/GetSpineState_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvGetSpineState_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_c4c456174d7b6647d09d986d37c041b942418941a5e01c447486727e919b8a67";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvGetSpineState_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvGetSpineState_Request& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/GetSpineState_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvGetSpineState_Response> {
  static constexpr size_t max_wire_bytes=781;
  static constexpr char const* codec_identity="UWVMROS1_297d70e2c5c24dcea4bdf8c5f9e5c8c32256e8f35f1900d8afd914ab2acfdcd1";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvGetSpineState_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvGetSpineState_Response& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/Halt_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvHalt_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_50af7625abf7ec20a30ec49b01e8d32de2ee0d4710820c8de659df211be8c111";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvHalt_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvHalt_Request& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/Halt_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvHalt_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_150e1d90b485a6ddf79f771379a0e1d3f99a77f88ab1cf1f569d614a63ac3d77";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvHalt_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvHalt_Response& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/SwitchOff_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOff_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_097f00adfdc8d1a49615792158a36052917c41385683b39de6686f591964fc16";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOff_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOff_Request& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/SwitchOff_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOff_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_d3ab8516e62428b2caf71673f398ccc7199b7b25aac4c37b42c15a79878e54ad";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOff_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOff_Response& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/SwitchOn_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOn_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_840d12dbe4d4a8558e72c2facbccfc547de35536f749573b6de60ce9a9238a56";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOn_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOn_Request& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for franka_spine_msgs/srv/SwitchOn_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOn_Response> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_8c58d63385a5fc6f160585504b3c9c5ca5375c6187f7a494c3db696c5cc7a37d";
  static bool encode(uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOn_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Franka_spine_msgsSrvSwitchOn_Response& out) {return !roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
