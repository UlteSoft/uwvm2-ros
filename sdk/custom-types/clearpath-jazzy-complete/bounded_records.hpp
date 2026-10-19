/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for clearpath_motor_msgs/action/LynxCalibrate_Goal. */
using Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal;
/** Complete bounded guest value for clearpath_motor_msgs/action/LynxCalibrate_Result. */
using Ros2Clearpath_motor_msgsActionLynxCalibrate_Result=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result;
/** Complete bounded guest value for clearpath_motor_msgs/action/LynxCalibrate_Feedback. */
using Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback;
/** Complete bounded guest value for clearpath_motor_msgs/action/LynxUpdate_Goal. */
using Ros2Clearpath_motor_msgsActionLynxUpdate_Goal=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal;
/** Complete bounded guest value for clearpath_motor_msgs/action/LynxUpdate_Result. */
using Ros2Clearpath_motor_msgsActionLynxUpdate_Result=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result;
/** Complete bounded guest value for clearpath_motor_msgs/action/LynxUpdate_Feedback. */
using Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback;
/** Complete bounded guest value for builtin_interfaces/msg/Duration. */
using Ros2Builtin_interfacesMsgDuration=roswasmc_bounded_2_builtin_interfaces_msg_Duration;
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for clearpath_control_msgs/msg/DeviceInfo. */
using Ros2Clearpath_control_msgsMsgDeviceInfo=roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo;
/** Complete bounded guest value for clearpath_control_msgs/msg/StreamControl. */
using Ros2Clearpath_control_msgsMsgStreamControl=roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl;
/** Complete bounded guest value for clearpath_motor_msgs/msg/LynxDebug. */
using Ros2Clearpath_motor_msgsMsgLynxDebug=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug;
/** Complete bounded guest value for clearpath_motor_msgs/msg/LynxFeedback. */
using Ros2Clearpath_motor_msgsMsgLynxFeedback=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback;
/** Complete bounded guest value for clearpath_motor_msgs/msg/LynxMotorProtection. */
using Ros2Clearpath_motor_msgsMsgLynxMotorProtection=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for clearpath_motor_msgs/msg/LynxMultiDebug. */
using Ros2Clearpath_motor_msgsMsgLynxMultiDebug=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug;
/** Complete bounded guest value for clearpath_motor_msgs/msg/LynxMultiFeedback. */
using Ros2Clearpath_motor_msgsMsgLynxMultiFeedback=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback;
/** Complete bounded guest value for clearpath_motor_msgs/msg/LynxStatus. */
using Ros2Clearpath_motor_msgsMsgLynxStatus=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus;
/** Complete bounded guest value for clearpath_motor_msgs/msg/LynxMultiStatus. */
using Ros2Clearpath_motor_msgsMsgLynxMultiStatus=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus;
/** Complete bounded guest value for clearpath_motor_msgs/msg/LynxSystemProtection. */
using Ros2Clearpath_motor_msgsMsgLynxSystemProtection=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection;
/** Complete bounded guest value for clearpath_motor_msgs/msg/PumaFeedback. */
using Ros2Clearpath_motor_msgsMsgPumaFeedback=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback;
/** Complete bounded guest value for clearpath_motor_msgs/msg/PumaMultiFeedback. */
using Ros2Clearpath_motor_msgsMsgPumaMultiFeedback=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback;
/** Complete bounded guest value for clearpath_motor_msgs/msg/PumaStatus. */
using Ros2Clearpath_motor_msgsMsgPumaStatus=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus;
/** Complete bounded guest value for clearpath_motor_msgs/msg/PumaMultiStatus. */
using Ros2Clearpath_motor_msgsMsgPumaMultiStatus=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus;
/** Complete bounded guest value for clearpath_platform_msgs/msg/DisplayStatus. */
using Ros2Clearpath_platform_msgsMsgDisplayStatus=roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Drive. */
using Ros2Clearpath_platform_msgsMsgDrive=roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive;
/** Complete bounded guest value for clearpath_platform_msgs/msg/DriveFeedback. */
using Ros2Clearpath_platform_msgsMsgDriveFeedback=roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Fans. */
using Ros2Clearpath_platform_msgsMsgFans=roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Feedback. */
using Ros2Clearpath_platform_msgsMsgFeedback=roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback;
/** Complete bounded guest value for clearpath_platform_msgs/msg/RGB. */
using Ros2Clearpath_platform_msgsMsgRGB=roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Lights. */
using Ros2Clearpath_platform_msgsMsgLights=roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights;
/** Complete bounded guest value for clearpath_platform_msgs/msg/PinoutCommand. */
using Ros2Clearpath_platform_msgsMsgPinoutCommand=roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand;
/** Complete bounded guest value for clearpath_platform_msgs/msg/PinoutState. */
using Ros2Clearpath_platform_msgsMsgPinoutState=roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Power. */
using Ros2Clearpath_platform_msgsMsgPower=roswasmc_bounded_2_clearpath_platform_msgs_msg_Power;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Status. */
using Ros2Clearpath_platform_msgsMsgStatus=roswasmc_bounded_2_clearpath_platform_msgs_msg_Status;
/** Complete bounded guest value for clearpath_platform_msgs/msg/StopStatus. */
using Ros2Clearpath_platform_msgsMsgStopStatus=roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Temperature. */
using Ros2Clearpath_platform_msgsMsgTemperature=roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature;
/** Complete bounded guest value for clearpath_control_msgs/srv/PowerControl_Request. */
using Ros2Clearpath_control_msgsSrvPowerControl_Request=roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request;
/** Complete bounded guest value for clearpath_control_msgs/srv/PowerControl_Response. */
using Ros2Clearpath_control_msgsSrvPowerControl_Response=roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response;
/** Complete bounded guest value for clearpath_platform_msgs/srv/ConfigureMcu_Request. */
using Ros2Clearpath_platform_msgsSrvConfigureMcu_Request=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request;
/** Complete bounded guest value for clearpath_platform_msgs/srv/ConfigureMcu_Response. */
using Ros2Clearpath_platform_msgsSrvConfigureMcu_Response=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response;
/** Complete bounded guest value for clearpath_platform_msgs/srv/SetPinout_Request. */
using Ros2Clearpath_platform_msgsSrvSetPinout_Request=roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request;
/** Complete bounded guest value for clearpath_platform_msgs/srv/SetPinout_Response. */
using Ros2Clearpath_platform_msgsSrvSetPinout_Response=roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response;
}
namespace roswasmcpp {
/** Checked codec for clearpath_motor_msgs/action/LynxCalibrate_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_61b01a6234b41f96464de9449b3e8d1b955decf6afa12aa3ba94062c98cec681";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/action/LynxCalibrate_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Result> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_686450363b8b15eba1e9e9e9ff05c9aacfb95bb0d113eaa7c443e4c70d55d41f";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Result& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/action/LynxCalibrate_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_ebe7badd3f72982079911d8aa72c4ddd27ff0a12d2f948e5970649317a592b53";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/action/LynxUpdate_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Goal> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_f0362f8a90391e8ffda2e8f3b798769b644091058361882c4e9457196f007565";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Goal& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/action/LynxUpdate_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Result> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_126fe871dbcb036e485d20e8ccdc8e9fb7be01757f539bfba895aabf44d341d8";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Result& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/action/LynxUpdate_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_2179ec9dee627234a1a31e9681ff214cc45ca837349cd8e407e019a354121b19";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for builtin_interfaces/msg/Time; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Builtin_interfacesMsgTime> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7";
  static bool encode(uwvm_custom::Ros2Builtin_interfacesMsgTime const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(&value))return false;size_t n=roswasmc_bounded_2_builtin_interfaces_msg_Time_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_builtin_interfaces_msg_Time_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Builtin_interfacesMsgTime& out) {return !roswasmc_bounded_2_builtin_interfaces_msg_Time_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_control_msgs/msg/DeviceInfo; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_control_msgsMsgDeviceInfo> {
  static constexpr size_t max_wire_bytes=1820;
  static constexpr char const* codec_identity="UWVMROS1_9111f5d4b5324c36eb85f01e6fdbc3c589479ff5b64a8fa7b9d467aa14675c3e";
  static bool encode(uwvm_custom::Ros2Clearpath_control_msgsMsgDeviceInfo const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_control_msgsMsgDeviceInfo& out) {return !roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_control_msgs/msg/StreamControl; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_control_msgsMsgStreamControl> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_26208e266c9185bfb0e715407ab0fe313f2d47e6bc9f54594a102c3a09d34734";
  static bool encode(uwvm_custom::Ros2Clearpath_control_msgsMsgStreamControl const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_control_msgsMsgStreamControl& out) {return !roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/LynxDebug; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxDebug> {
  static constexpr size_t max_wire_bytes=291;
  static constexpr char const* codec_identity="UWVMROS1_0341ae040f1e8a1f980bde84498c4a4f8b9c55c3806839a73727c1579ab4416e";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxDebug const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxDebug& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/LynxFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxFeedback> {
  static constexpr size_t max_wire_bytes=277;
  static constexpr char const* codec_identity="UWVMROS1_5d92c7085019ebf00ba93788b5b2f4055ad40b2c6714575a66fb75cf314429eb";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxFeedback& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/LynxMotorProtection; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMotorProtection> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_f824abfa97f6f24934e04094d66170167f19a14fb933f325cbef4ed6fca16630";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMotorProtection const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMotorProtection& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_decode(bytes.data(),bytes.size(),&out);}
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
/** Checked codec for clearpath_motor_msgs/msg/LynxMultiDebug; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiDebug> {
  static constexpr size_t max_wire_bytes=1308;
  static constexpr char const* codec_identity="UWVMROS1_056abd2153e7f1a6df223bf5daba581e6c218bb7ef365444fc5c3171dfc62e95";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiDebug const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiDebug& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/LynxMultiFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiFeedback> {
  static constexpr size_t max_wire_bytes=1252;
  static constexpr char const* codec_identity="UWVMROS1_2c83cade6bec4d766fd2cff97338642b3d2065d41e9968ccf5054c6f231242c7";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiFeedback& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/LynxStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxStatus> {
  static constexpr size_t max_wire_bytes=546;
  static constexpr char const* codec_identity="UWVMROS1_b68a7ab9bc5312c59d2ef86e49313468b631e8cacdf52468374b67f4eced5c6d";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxStatus& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/LynxMultiStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiStatus> {
  static constexpr size_t max_wire_bytes=2328;
  static constexpr char const* codec_identity="UWVMROS1_595cd0847b4332e14cc067b916166e9a6e1d1e12a7a68cdb7e1cd414a9ea9ce1";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxMultiStatus& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/LynxSystemProtection; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxSystemProtection> {
  static constexpr size_t max_wire_bytes=669;
  static constexpr char const* codec_identity="UWVMROS1_4d662de03c34bb633a122399be1a1aaea6187550c83231bf6d8a06b95fa82d9f";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxSystemProtection const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgLynxSystemProtection& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/PumaFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaFeedback> {
  static constexpr size_t max_wire_bytes=293;
  static constexpr char const* codec_identity="UWVMROS1_c020162b07a55ac5f41df6021c0d3053d7302863f4e544a738b6cc5bc4dc9659";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaFeedback& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/PumaMultiFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaMultiFeedback> {
  static constexpr size_t max_wire_bytes=1316;
  static constexpr char const* codec_identity="UWVMROS1_638309564580ad23d585799dcd46156e1e4261ebf9febfbb91754c250907c947";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaMultiFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaMultiFeedback& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/PumaStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaStatus> {
  static constexpr size_t max_wire_bytes=279;
  static constexpr char const* codec_identity="UWVMROS1_a027229db2c8500f707d97064548b27e77e24ae4840ad9722d2dc74726099da8";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaStatus& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_motor_msgs/msg/PumaMultiStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaMultiStatus> {
  static constexpr size_t max_wire_bytes=1260;
  static constexpr char const* codec_identity="UWVMROS1_30a0210fea4fffe07371c898c365ef9696501a7541935dd80770671a7163c8f2";
  static bool encode(uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaMultiStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_motor_msgsMsgPumaMultiStatus& out) {return !roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/DisplayStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgDisplayStatus> {
  static constexpr size_t max_wire_bytes=106;
  static constexpr char const* codec_identity="UWVMROS1_e4585117414581491304a26d528b44c770799f622c3f1cde037078f5cb2462c2";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgDisplayStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgDisplayStatus& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Drive; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgDrive> {
  static constexpr size_t max_wire_bytes=9;
  static constexpr char const* codec_identity="UWVMROS1_be8664128ff89a066550f1a2496c30396f39bafd1d96483c545048a328138785";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgDrive const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgDrive& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/DriveFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgDriveFeedback> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_008f51d4c8341723ffec72651f93c00db0f9feceb037ee7b6b460ffcafd3f287";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgDriveFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgDriveFeedback& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Fans; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgFans> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_a7d96f2242d992baf96f31bd8f93088547f92c72912c241c2d237f7714800cc8";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgFans const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgFans& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgFeedback> {
  static constexpr size_t max_wire_bytes=192;
  static constexpr char const* codec_identity="UWVMROS1_15fdfd2e12df38bab09bf083beb61ce23d3c1082e794c34d51eec4c473e29442";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgFeedback& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/RGB; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgRGB> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_42db8731293054ef44002afc54c16dac45544354932d5353cf749fb376cef1a1";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgRGB const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgRGB& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Lights; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgLights> {
  static constexpr size_t max_wire_bytes=10;
  static constexpr char const* codec_identity="UWVMROS1_3f975a3ec1fca4f8add9523d4f9f99963be520c0e2982e644551b71cbd4b8a27";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgLights const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgLights& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/PinoutCommand; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgPinoutCommand> {
  static constexpr size_t max_wire_bytes=18;
  static constexpr char const* codec_identity="UWVMROS1_b26c2baf1e64cefa889c4ac6bd6b7f4647d9247eca0626b83cd726547a4e80ff";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgPinoutCommand const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgPinoutCommand& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/PinoutState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgPinoutState> {
  static constexpr size_t max_wire_bytes=170;
  static constexpr char const* codec_identity="UWVMROS1_a936c6d91440709d50fa9b0be8fb567a9ce793e84f57e5379f6a160c8cbf73e7";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgPinoutState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgPinoutState& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Power; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgPower> {
  static constexpr size_t max_wire_bytes=169;
  static constexpr char const* codec_identity="UWVMROS1_3fc241d6b81da2f80bfce22933b81786b5e582715dc66cd238fa40b239e7aecc";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgPower const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgPower& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Status; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgStatus> {
  static constexpr size_t max_wire_bytes=676;
  static constexpr char const* codec_identity="UWVMROS1_1bd99cc11ea9efcb51eeba8fc1d599960c16e958a8adf33a56d47fe27be87782";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgStatus& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/StopStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgStopStatus> {
  static constexpr size_t max_wire_bytes=143;
  static constexpr char const* codec_identity="UWVMROS1_101f96c8d48e7a945a2f94108e7b48393fc9d4bd75507dd709b95180ff4794d2";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgStopStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgStopStatus& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Temperature; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgTemperature> {
  static constexpr size_t max_wire_bytes=152;
  static constexpr char const* codec_identity="UWVMROS1_b4ae4d6ace9045ce96b0931f08d3b2c7ec358ab9c6da7022afd9ea489f9b0466";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgTemperature const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgTemperature& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_control_msgs/srv/PowerControl_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_control_msgsSrvPowerControl_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1ad49d42d8895b3bd219799ab9cc080cf09d23aa4bca4c1172d19bc18fa416c4";
  static bool encode(uwvm_custom::Ros2Clearpath_control_msgsSrvPowerControl_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_control_msgsSrvPowerControl_Request& out) {return !roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_control_msgs/srv/PowerControl_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_control_msgsSrvPowerControl_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_6d74d07d6a8ff1c41ed4ca13939d54e9e642f423dc2ff6598665238ebbf10a8b";
  static bool encode(uwvm_custom::Ros2Clearpath_control_msgsSrvPowerControl_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_control_msgsSrvPowerControl_Response& out) {return !roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/srv/ConfigureMcu_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Request> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_9c48451b208c4de7a51c5075587dfbe710e7ec6b249c7e1eef519022154023fa";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Request& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/srv/ConfigureMcu_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_552e715f290b5b9aa5dc926519cf2ab5120d6b2ae5eca9902efcc3f8c9e459ad";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Response& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/srv/SetPinout_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsSrvSetPinout_Request> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_3a3a2f8f16279c2e7a057353cd1f7f11e1a65e887dd3ce0a4b7f584708afb238";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsSrvSetPinout_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsSrvSetPinout_Request& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/srv/SetPinout_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsSrvSetPinout_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_b643ef5de2dfb725906ef8f11d79601400953211b47220be7a6fcc05d1be8ee3";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsSrvSetPinout_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsSrvSetPinout_Response& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
