/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for geometry_msgs/msg/Point. */
using Ros2Geometry_msgsMsgPoint=roswasmc_bounded_2_geometry_msgs_msg_Point;
/** Complete bounded guest value for geometry_msgs/msg/Quaternion. */
using Ros2Geometry_msgsMsgQuaternion=roswasmc_bounded_2_geometry_msgs_msg_Quaternion;
/** Complete bounded guest value for geometry_msgs/msg/Pose. */
using Ros2Geometry_msgsMsgPose=roswasmc_bounded_2_geometry_msgs_msg_Pose;
/** Complete bounded guest value for geometry_msgs/msg/Vector3. */
using Ros2Geometry_msgsMsgVector3=roswasmc_bounded_2_geometry_msgs_msg_Vector3;
/** Complete bounded guest value for geometry_msgs/msg/Twist. */
using Ros2Geometry_msgsMsgTwist=roswasmc_bounded_2_geometry_msgs_msg_Twist;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for std_msgs/msg/String. */
using Ros2Std_msgsMsgString=roswasmc_bounded_2_std_msgs_msg_String;
/** Complete bounded guest value for urinterfaces/msg/RobotMode. */
using Ros2UrinterfacesMsgRobotMode=roswasmc_bounded_2_urinterfaces_msg_RobotMode;
/** Complete bounded guest value for urinterfaces/action/SetMode_Goal. */
using Ros2UrinterfacesActionSetMode_Goal=roswasmc_bounded_2_urinterfaces_action_SetMode_Goal;
/** Complete bounded guest value for urinterfaces/action/SetMode_Result. */
using Ros2UrinterfacesActionSetMode_Result=roswasmc_bounded_2_urinterfaces_action_SetMode_Result;
/** Complete bounded guest value for urinterfaces/action/SetMode_Feedback. */
using Ros2UrinterfacesActionSetMode_Feedback=roswasmc_bounded_2_urinterfaces_action_SetMode_Feedback;
/** Complete bounded guest value for urinterfaces/msg/ActiveTCP. */
using Ros2UrinterfacesMsgActiveTCP=roswasmc_bounded_2_urinterfaces_msg_ActiveTCP;
/** Complete bounded guest value for urinterfaces/msg/Analog. */
using Ros2UrinterfacesMsgAnalog=roswasmc_bounded_2_urinterfaces_msg_Analog;
/** Complete bounded guest value for urinterfaces/msg/AnalogIO. */
using Ros2UrinterfacesMsgAnalogIO=roswasmc_bounded_2_urinterfaces_msg_AnalogIO;
/** Complete bounded guest value for urinterfaces/msg/AnalogIOStateStamped. */
using Ros2UrinterfacesMsgAnalogIOStateStamped=roswasmc_bounded_2_urinterfaces_msg_AnalogIOStateStamped;
/** Complete bounded guest value for urinterfaces/msg/AnalogStamped. */
using Ros2UrinterfacesMsgAnalogStamped=roswasmc_bounded_2_urinterfaces_msg_AnalogStamped;
/** Complete bounded guest value for urinterfaces/msg/BoolStamped. */
using Ros2UrinterfacesMsgBoolStamped=roswasmc_bounded_2_urinterfaces_msg_BoolStamped;
/** Complete bounded guest value for urinterfaces/msg/ControlCycle. */
using Ros2UrinterfacesMsgControlCycle=roswasmc_bounded_2_urinterfaces_msg_ControlCycle;
/** Complete bounded guest value for urinterfaces/msg/Current. */
using Ros2UrinterfacesMsgCurrent=roswasmc_bounded_2_urinterfaces_msg_Current;
/** Complete bounded guest value for urinterfaces/msg/Digital. */
using Ros2UrinterfacesMsgDigital=roswasmc_bounded_2_urinterfaces_msg_Digital;
/** Complete bounded guest value for urinterfaces/msg/DigitalIO. */
using Ros2UrinterfacesMsgDigitalIO=roswasmc_bounded_2_urinterfaces_msg_DigitalIO;
/** Complete bounded guest value for urinterfaces/msg/DigitalIOStateStamped. */
using Ros2UrinterfacesMsgDigitalIOStateStamped=roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped;
/** Complete bounded guest value for urinterfaces/msg/DigitalStamped. */
using Ros2UrinterfacesMsgDigitalStamped=roswasmc_bounded_2_urinterfaces_msg_DigitalStamped;
/** Complete bounded guest value for urinterfaces/msg/DoubleMultiArrayStamped. */
using Ros2UrinterfacesMsgDoubleMultiArrayStamped=roswasmc_bounded_2_urinterfaces_msg_DoubleMultiArrayStamped;
/** Complete bounded guest value for urinterfaces/msg/Float32Stamped. */
using Ros2UrinterfacesMsgFloat32Stamped=roswasmc_bounded_2_urinterfaces_msg_Float32Stamped;
/** Complete bounded guest value for urinterfaces/msg/Float64Stamped. */
using Ros2UrinterfacesMsgFloat64Stamped=roswasmc_bounded_2_urinterfaces_msg_Float64Stamped;
/** Complete bounded guest value for urinterfaces/msg/JointMode. */
using Ros2UrinterfacesMsgJointMode=roswasmc_bounded_2_urinterfaces_msg_JointMode;
/** Complete bounded guest value for urinterfaces/msg/JointTemperature. */
using Ros2UrinterfacesMsgJointTemperature=roswasmc_bounded_2_urinterfaces_msg_JointTemperature;
/** Complete bounded guest value for urinterfaces/msg/JointVoltage. */
using Ros2UrinterfacesMsgJointVoltage=roswasmc_bounded_2_urinterfaces_msg_JointVoltage;
/** Complete bounded guest value for urinterfaces/msg/JsonStringType. */
using Ros2UrinterfacesMsgJsonStringType=roswasmc_bounded_2_urinterfaces_msg_JsonStringType;
/** Complete bounded guest value for urinterfaces/msg/RuntimeState. */
using Ros2UrinterfacesMsgRuntimeState=roswasmc_bounded_2_urinterfaces_msg_RuntimeState;
/** Complete bounded guest value for urinterfaces/msg/SafetyStatus. */
using Ros2UrinterfacesMsgSafetyStatus=roswasmc_bounded_2_urinterfaces_msg_SafetyStatus;
/** Complete bounded guest value for urinterfaces/msg/SharedVariablesEditResponse. */
using Ros2UrinterfacesMsgSharedVariablesEditResponse=roswasmc_bounded_2_urinterfaces_msg_SharedVariablesEditResponse;
/** Complete bounded guest value for urinterfaces/msg/SpeedSliderFraction. */
using Ros2UrinterfacesMsgSpeedSliderFraction=roswasmc_bounded_2_urinterfaces_msg_SpeedSliderFraction;
/** Complete bounded guest value for urinterfaces/msg/TCPState. */
using Ros2UrinterfacesMsgTCPState=roswasmc_bounded_2_urinterfaces_msg_TCPState;
/** Complete bounded guest value for urinterfaces/msg/ToolOutputMode. */
using Ros2UrinterfacesMsgToolOutputMode=roswasmc_bounded_2_urinterfaces_msg_ToolOutputMode;
/** Complete bounded guest value for urinterfaces/msg/VariableUpdate. */
using Ros2UrinterfacesMsgVariableUpdate=roswasmc_bounded_2_urinterfaces_msg_VariableUpdate;
/** Complete bounded guest value for urinterfaces/msg/VoltageCurrentStamped. */
using Ros2UrinterfacesMsgVoltageCurrentStamped=roswasmc_bounded_2_urinterfaces_msg_VoltageCurrentStamped;
/** Complete bounded guest value for urinterfaces/msg/WorldModelNames. */
using Ros2UrinterfacesMsgWorldModelNames=roswasmc_bounded_2_urinterfaces_msg_WorldModelNames;
/** Complete bounded guest value for urinterfaces/srv/SetAnalogOutput_Request. */
using Ros2UrinterfacesSrvSetAnalogOutput_Request=roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Request;
/** Complete bounded guest value for urinterfaces/srv/SetAnalogOutput_Response. */
using Ros2UrinterfacesSrvSetAnalogOutput_Response=roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Response;
/** Complete bounded guest value for urinterfaces/srv/SetDigitalOutput_Request. */
using Ros2UrinterfacesSrvSetDigitalOutput_Request=roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Request;
/** Complete bounded guest value for urinterfaces/srv/SetDigitalOutput_Response. */
using Ros2UrinterfacesSrvSetDigitalOutput_Response=roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Response;
/** Complete bounded guest value for urinterfaces/srv/SetSpeedFraction_Request. */
using Ros2UrinterfacesSrvSetSpeedFraction_Request=roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Request;
/** Complete bounded guest value for urinterfaces/srv/SetSpeedFraction_Response. */
using Ros2UrinterfacesSrvSetSpeedFraction_Response=roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Response;
/** Complete bounded guest value for urinterfaces/srv/SharedVariablesEditor_Request. */
using Ros2UrinterfacesSrvSharedVariablesEditor_Request=roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Request;
/** Complete bounded guest value for urinterfaces/srv/SharedVariablesEditor_Response. */
using Ros2UrinterfacesSrvSharedVariablesEditor_Response=roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Response;
/** Complete bounded guest value for urinterfaces/srv/VariablesViewer_Request. */
using Ros2UrinterfacesSrvVariablesViewer_Request=roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Request;
/** Complete bounded guest value for urinterfaces/srv/VariablesViewer_Response. */
using Ros2UrinterfacesSrvVariablesViewer_Response=roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Response;
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
/** Checked codec for std_msgs/msg/Header; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_msgsMsgHeader> {
  static constexpr size_t max_wire_bytes=140;
  static constexpr char const* codec_identity="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_msgs/msg/String; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_msgsMsgString> {
  static constexpr size_t max_wire_bytes=1028;
  static constexpr char const* codec_identity="UWVMROS1_c48b5497cbf487f56eb073a982b93acd501c6bb72f0e9ef9dce2175ce7ac15c5";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgString const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_String_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_String_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_String_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgString& out) {return !roswasmc_bounded_2_std_msgs_msg_String_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/RobotMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgRobotMode> {
  static constexpr size_t max_wire_bytes=141;
  static constexpr char const* codec_identity="UWVMROS1_a16ec0ed8caa1a2b48cd4116d88877911921848469cd139f4783e4f2c549e7ea";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgRobotMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_RobotMode_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_RobotMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_RobotMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgRobotMode& out) {return !roswasmc_bounded_2_urinterfaces_msg_RobotMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/action/SetMode_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesActionSetMode_Goal> {
  static constexpr size_t max_wire_bytes=143;
  static constexpr char const* codec_identity="UWVMROS1_d1d998e217d6f3d3eefd8ffccecff8463580346150a59a1c7a2dac8d609e0e09";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2UrinterfacesActionSetMode_Goal default_value() noexcept {uwvm_custom::Ros2UrinterfacesActionSetMode_Goal value={0};(void)roswasmc_bounded_2_urinterfaces_action_SetMode_Goal_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2UrinterfacesActionSetMode_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_action_SetMode_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_action_SetMode_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_action_SetMode_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesActionSetMode_Goal& out) {return !roswasmc_bounded_2_urinterfaces_action_SetMode_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/action/SetMode_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesActionSetMode_Result> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_500243a508824c42fe2da424040aa60bf2426ac607f2f6016fab014f81518af7";
  static bool encode(uwvm_custom::Ros2UrinterfacesActionSetMode_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_action_SetMode_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_action_SetMode_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_action_SetMode_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesActionSetMode_Result& out) {return !roswasmc_bounded_2_urinterfaces_action_SetMode_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/action/SetMode_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesActionSetMode_Feedback> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_22fb71c17066829bd66c98bc631c82b2bfef3c501e08960a6bfad3d902f76537";
  static bool encode(uwvm_custom::Ros2UrinterfacesActionSetMode_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_action_SetMode_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_action_SetMode_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_action_SetMode_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesActionSetMode_Feedback& out) {return !roswasmc_bounded_2_urinterfaces_action_SetMode_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/ActiveTCP; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgActiveTCP> {
  static constexpr size_t max_wire_bytes=400;
  static constexpr char const* codec_identity="UWVMROS1_e62d31728eca201dee256d88d1d268bf2ecb9607ed5c48783432f966edf459ae";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgActiveTCP const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_ActiveTCP_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_ActiveTCP_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_ActiveTCP_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgActiveTCP& out) {return !roswasmc_bounded_2_urinterfaces_msg_ActiveTCP_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/Analog; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgAnalog> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_e81cffd909f3bc387ee00c37537434341ef93fd160e465be033aa9bc412b979b";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgAnalog const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_Analog_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_Analog_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_Analog_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgAnalog& out) {return !roswasmc_bounded_2_urinterfaces_msg_Analog_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/AnalogIO; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgAnalogIO> {
  static constexpr size_t max_wire_bytes=200;
  static constexpr char const* codec_identity="UWVMROS1_3130f3e3857f21bab5092d9c6abea8c45965c07ebb9c1ab14b28fa51b7d71e0c";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgAnalogIO const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_AnalogIO_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_AnalogIO_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_AnalogIO_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgAnalogIO& out) {return !roswasmc_bounded_2_urinterfaces_msg_AnalogIO_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/AnalogIOStateStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgAnalogIOStateStamped> {
  static constexpr size_t max_wire_bytes=340;
  static constexpr char const* codec_identity="UWVMROS1_a40e839bb4acfcb26503c79f01216b7aea9ab8ff25c50dca42578b206ab40a89";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgAnalogIOStateStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_AnalogIOStateStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_AnalogIOStateStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_AnalogIOStateStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgAnalogIOStateStamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_AnalogIOStateStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/AnalogStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgAnalogStamped> {
  static constexpr size_t max_wire_bytes=146;
  static constexpr char const* codec_identity="UWVMROS1_749557c6e79b882385927c62d3c45963797ce116d7be6235bd9f186a7b4da76c";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgAnalogStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_AnalogStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_AnalogStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_AnalogStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgAnalogStamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_AnalogStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/BoolStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgBoolStamped> {
  static constexpr size_t max_wire_bytes=141;
  static constexpr char const* codec_identity="UWVMROS1_1a4128638df10bd9a8d33af0ed3c2ee9b4deb771d45dfe83f2a9a75c7f77de66";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgBoolStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_BoolStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_BoolStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_BoolStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgBoolStamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_BoolStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/ControlCycle; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgControlCycle> {
  static constexpr size_t max_wire_bytes=148;
  static constexpr char const* codec_identity="UWVMROS1_99a4d96e5673f5ac49eab13cd8b28af55ac83e1626c39ed20ff46655df4d6278";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgControlCycle const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_ControlCycle_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_ControlCycle_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_ControlCycle_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgControlCycle& out) {return !roswasmc_bounded_2_urinterfaces_msg_ControlCycle_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/Current; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgCurrent> {
  static constexpr size_t max_wire_bytes=144;
  static constexpr char const* codec_identity="UWVMROS1_759f2139335bcb720f7f4a38cd8b1413f203f2dd2b72149fa0de0fb7a0a778df";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgCurrent const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_Current_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_Current_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_Current_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgCurrent& out) {return !roswasmc_bounded_2_urinterfaces_msg_Current_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/Digital; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgDigital> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_c60c73df0383bb9a3e23ae3148cff9d6eb49b88c2d10add0af6a86ec9af54512";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgDigital const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_Digital_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_Digital_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_Digital_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgDigital& out) {return !roswasmc_bounded_2_urinterfaces_msg_Digital_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/DigitalIO; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgDigitalIO> {
  static constexpr size_t max_wire_bytes=136;
  static constexpr char const* codec_identity="UWVMROS1_11235b6010feb7e3e5e6c2be8acd3fbba1c25ecbf97ae5e0c899a0e3de3c4e4a";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgDigitalIO const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_DigitalIO_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_DigitalIO_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_DigitalIO_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgDigitalIO& out) {return !roswasmc_bounded_2_urinterfaces_msg_DigitalIO_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/DigitalIOStateStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgDigitalIOStateStamped> {
  static constexpr size_t max_wire_bytes=412;
  static constexpr char const* codec_identity="UWVMROS1_542db76916f234e79216804426dc4e8d386a0ef547907391c921f51ed45e64c9";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgDigitalIOStateStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgDigitalIOStateStamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/DigitalStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgDigitalStamped> {
  static constexpr size_t max_wire_bytes=142;
  static constexpr char const* codec_identity="UWVMROS1_dc86d5a04c0801089ddf6b61b5fd33d63df8b5599550c6a0e84e19bbff375865";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgDigitalStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_DigitalStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_DigitalStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_DigitalStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgDigitalStamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_DigitalStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/DoubleMultiArrayStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgDoubleMultiArrayStamped> {
  static constexpr size_t max_wire_bytes=272;
  static constexpr char const* codec_identity="UWVMROS1_82b52f88248ea8d6cd7b809d1fd5aa55b4bbe1618044e739998a7ec74a23a8ae";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgDoubleMultiArrayStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_DoubleMultiArrayStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_DoubleMultiArrayStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_DoubleMultiArrayStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgDoubleMultiArrayStamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_DoubleMultiArrayStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/Float32Stamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgFloat32Stamped> {
  static constexpr size_t max_wire_bytes=144;
  static constexpr char const* codec_identity="UWVMROS1_5989940d19890b6d3b78172e9920708745b78e35c1c4fb238412c37a813d30d3";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgFloat32Stamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_Float32Stamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_Float32Stamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_Float32Stamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgFloat32Stamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_Float32Stamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/Float64Stamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgFloat64Stamped> {
  static constexpr size_t max_wire_bytes=148;
  static constexpr char const* codec_identity="UWVMROS1_4f8950feff6f20dbdbf9cac1f01168824be3698a4bd90355a470b580930edb21";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgFloat64Stamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_Float64Stamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_Float64Stamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_Float64Stamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgFloat64Stamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_Float64Stamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/JointMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgJointMode> {
  static constexpr size_t max_wire_bytes=147;
  static constexpr char const* codec_identity="UWVMROS1_6d766192096ced3c8568a98fb286c309e00cfb97e34b2611fff70e5bb1eaa95c";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgJointMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_JointMode_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_JointMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_JointMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgJointMode& out) {return !roswasmc_bounded_2_urinterfaces_msg_JointMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/JointTemperature; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgJointTemperature> {
  static constexpr size_t max_wire_bytes=164;
  static constexpr char const* codec_identity="UWVMROS1_af77f1ec4c6b4d71e6387315d2388e7493c229dbf216848e61d5638cb308d017";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgJointTemperature const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_JointTemperature_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_JointTemperature_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_JointTemperature_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgJointTemperature& out) {return !roswasmc_bounded_2_urinterfaces_msg_JointTemperature_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/JointVoltage; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgJointVoltage> {
  static constexpr size_t max_wire_bytes=188;
  static constexpr char const* codec_identity="UWVMROS1_9fa2179402b5dc29b898ac1321c1f6f86676300939776a6e32de4529f3776b61";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgJointVoltage const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_JointVoltage_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_JointVoltage_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_JointVoltage_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgJointVoltage& out) {return !roswasmc_bounded_2_urinterfaces_msg_JointVoltage_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/JsonStringType; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgJsonStringType> {
  static constexpr size_t max_wire_bytes=1028;
  static constexpr char const* codec_identity="UWVMROS1_05da0dcf2728cd73f875421c457804a146ceffe76e9d4619be71c2c92efdd728";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgJsonStringType const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_JsonStringType_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_JsonStringType_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_JsonStringType_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgJsonStringType& out) {return !roswasmc_bounded_2_urinterfaces_msg_JsonStringType_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/RuntimeState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgRuntimeState> {
  static constexpr size_t max_wire_bytes=141;
  static constexpr char const* codec_identity="UWVMROS1_711ca0e324cbeb36fcedea4881b8658a4b453402c410c8d3efaa57205aaf74b6";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgRuntimeState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_RuntimeState_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_RuntimeState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_RuntimeState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgRuntimeState& out) {return !roswasmc_bounded_2_urinterfaces_msg_RuntimeState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/SafetyStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgSafetyStatus> {
  static constexpr size_t max_wire_bytes=141;
  static constexpr char const* codec_identity="UWVMROS1_8f266e1ee8ab2ee9791ff4b52f9dae8ef982844c8d24a0bdfb4bb93cc55b78d4";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgSafetyStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_SafetyStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_SafetyStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_SafetyStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgSafetyStatus& out) {return !roswasmc_bounded_2_urinterfaces_msg_SafetyStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/SharedVariablesEditResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgSharedVariablesEditResponse> {
  static constexpr size_t max_wire_bytes=1029;
  static constexpr char const* codec_identity="UWVMROS1_870ce8f649642b6edcce8ba5c7cb27818ec138d7afb2cde926638d21ea748212";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgSharedVariablesEditResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_SharedVariablesEditResponse_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_SharedVariablesEditResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_SharedVariablesEditResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgSharedVariablesEditResponse& out) {return !roswasmc_bounded_2_urinterfaces_msg_SharedVariablesEditResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/SpeedSliderFraction; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgSpeedSliderFraction> {
  static constexpr size_t max_wire_bytes=148;
  static constexpr char const* codec_identity="UWVMROS1_55158782c2bc4d4741e8f2850efddf09d27666f9c3aeeded762d8d8e4198fccd";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgSpeedSliderFraction const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_SpeedSliderFraction_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_SpeedSliderFraction_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_SpeedSliderFraction_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgSpeedSliderFraction& out) {return !roswasmc_bounded_2_urinterfaces_msg_SpeedSliderFraction_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/TCPState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgTCPState> {
  static constexpr size_t max_wire_bytes=244;
  static constexpr char const* codec_identity="UWVMROS1_49d8f678cc32669dd81528bb4c5d97f28019745159ef5e7267c8a076cd315b92";
  /** Real C-value declared initialization; T{} intentionally remains zero aggregate storage. */
  static uwvm_custom::Ros2UrinterfacesMsgTCPState default_value() noexcept {uwvm_custom::Ros2UrinterfacesMsgTCPState value={0};(void)roswasmc_bounded_2_urinterfaces_msg_TCPState_default(&value);return value;}
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgTCPState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_TCPState_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_TCPState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_TCPState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgTCPState& out) {return !roswasmc_bounded_2_urinterfaces_msg_TCPState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/ToolOutputMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgToolOutputMode> {
  static constexpr size_t max_wire_bytes=143;
  static constexpr char const* codec_identity="UWVMROS1_21df545459050dc86de1fb111c9a4b2c8bb50c3729137b8d5c5134dd33b84660";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgToolOutputMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_ToolOutputMode_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_ToolOutputMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_ToolOutputMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgToolOutputMode& out) {return !roswasmc_bounded_2_urinterfaces_msg_ToolOutputMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/VariableUpdate; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgVariableUpdate> {
  static constexpr size_t max_wire_bytes=1168;
  static constexpr char const* codec_identity="UWVMROS1_7028ea5c674be886f4fac1d2ac0452167489f2c23d7ea759cdbec90feccbdb20";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgVariableUpdate const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_VariableUpdate_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_VariableUpdate_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_VariableUpdate_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgVariableUpdate& out) {return !roswasmc_bounded_2_urinterfaces_msg_VariableUpdate_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/VoltageCurrentStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgVoltageCurrentStamped> {
  static constexpr size_t max_wire_bytes=156;
  static constexpr char const* codec_identity="UWVMROS1_bcc3f6871a7bf0ff691d0c9c1b6596a633597de795b486c2b82ac922ede76805";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgVoltageCurrentStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_VoltageCurrentStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_VoltageCurrentStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_VoltageCurrentStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgVoltageCurrentStamped& out) {return !roswasmc_bounded_2_urinterfaces_msg_VoltageCurrentStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/msg/WorldModelNames; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesMsgWorldModelNames> {
  static constexpr size_t max_wire_bytes=16592;
  static constexpr char const* codec_identity="UWVMROS1_499f722fc97565bd0124888d7c20f4e9f8947893c49b1608dcf201fe3538d7a9";
  static bool encode(uwvm_custom::Ros2UrinterfacesMsgWorldModelNames const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_msg_WorldModelNames_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_msg_WorldModelNames_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_msg_WorldModelNames_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesMsgWorldModelNames& out) {return !roswasmc_bounded_2_urinterfaces_msg_WorldModelNames_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/SetAnalogOutput_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvSetAnalogOutput_Request> {
  static constexpr size_t max_wire_bytes=100;
  static constexpr char const* codec_identity="UWVMROS1_8be07a94dd334a03871180a9c73bab11377664e60500ac68d312398d51acd32d";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvSetAnalogOutput_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvSetAnalogOutput_Request& out) {return !roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/SetAnalogOutput_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvSetAnalogOutput_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_14016138286129e4f155ac2f82f5573e8a4af7d7c5ede55ce57f3ad76cb82e4e";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvSetAnalogOutput_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvSetAnalogOutput_Response& out) {return !roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/SetDigitalOutput_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvSetDigitalOutput_Request> {
  static constexpr size_t max_wire_bytes=36;
  static constexpr char const* codec_identity="UWVMROS1_ec6c587810af7f3d04b10058e69c8095775e8b6feede5fc0f60ca1dfda0b5e47";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvSetDigitalOutput_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvSetDigitalOutput_Request& out) {return !roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/SetDigitalOutput_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvSetDigitalOutput_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_0c5f0b8a7f1d14c6223838c3a28cb2236848b81ecfda29d2b704557579e9162d";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvSetDigitalOutput_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvSetDigitalOutput_Response& out) {return !roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/SetSpeedFraction_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvSetSpeedFraction_Request> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_21fca7e952e32f24c905094c09776968c8cd80b7712a03de7c2a693ea79a7d63";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvSetSpeedFraction_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvSetSpeedFraction_Request& out) {return !roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/SetSpeedFraction_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvSetSpeedFraction_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_386b067db70c17a88297d005650f6a05c0fb72eb5ee142a25c3e51c70b80c6e6";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvSetSpeedFraction_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvSetSpeedFraction_Response& out) {return !roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/SharedVariablesEditor_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvSharedVariablesEditor_Request> {
  static constexpr size_t max_wire_bytes=1028;
  static constexpr char const* codec_identity="UWVMROS1_c32d00237370b2eeb596c76bd9cda1ccfcab7618c71dec77d273f712462f5fa3";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvSharedVariablesEditor_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvSharedVariablesEditor_Request& out) {return !roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/SharedVariablesEditor_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvSharedVariablesEditor_Response> {
  static constexpr size_t max_wire_bytes=1029;
  static constexpr char const* codec_identity="UWVMROS1_4134f7ed9715b3f6e3fd7ecca44fcae67cdd5dc2fa432b07ab75485901d1d5ea";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvSharedVariablesEditor_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvSharedVariablesEditor_Response& out) {return !roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/VariablesViewer_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvVariablesViewer_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_6ecb0db6328d813bfb987ae987d3b7f2c6913dba2cf2a62f644a8eb2f760152a";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvVariablesViewer_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvVariablesViewer_Request& out) {return !roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for urinterfaces/srv/VariablesViewer_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2UrinterfacesSrvVariablesViewer_Response> {
  static constexpr size_t max_wire_bytes=1168;
  static constexpr char const* codec_identity="UWVMROS1_e8cb16ca847189ac38c0d972afbdd2208d8641c29a0ee0cce8284e9e0dd249a2";
  static bool encode(uwvm_custom::Ros2UrinterfacesSrvVariablesViewer_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2UrinterfacesSrvVariablesViewer_Response& out) {return !roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
