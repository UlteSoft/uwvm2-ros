/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for geometry_msgs/Point. */
using Ros1Geometry_msgsPoint=roswasmc_bounded_1_geometry_msgs_Point;
/** Complete bounded guest value for geometry_msgs/Quaternion. */
using Ros1Geometry_msgsQuaternion=roswasmc_bounded_1_geometry_msgs_Quaternion;
/** Complete bounded guest value for geometry_msgs/Pose. */
using Ros1Geometry_msgsPose=roswasmc_bounded_1_geometry_msgs_Pose;
/** Complete bounded guest value for time. */
using Ros1Time=roswasmc_bounded_1_time;
/** Complete bounded guest value for std_msgs/Header. */
using Ros1Std_msgsHeader=roswasmc_bounded_1_std_msgs_Header;
/** Complete bounded guest value for geometry_msgs/PoseStamped. */
using Ros1Geometry_msgsPoseStamped=roswasmc_bounded_1_geometry_msgs_PoseStamped;
/** Complete bounded guest value for geometry_msgs/Vector3. */
using Ros1Geometry_msgsVector3=roswasmc_bounded_1_geometry_msgs_Vector3;
/** Complete bounded guest value for geometry_msgs/Twist. */
using Ros1Geometry_msgsTwist=roswasmc_bounded_1_geometry_msgs_Twist;
/** Complete bounded guest value for geometry_msgs/Wrench. */
using Ros1Geometry_msgsWrench=roswasmc_bounded_1_geometry_msgs_Wrench;
/** Complete bounded guest value for ur_dashboard_msgs/AddToLogRequest. */
using Ros1Ur_dashboard_msgsAddToLogRequest=roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest;
/** Complete bounded guest value for ur_dashboard_msgs/AddToLogResponse. */
using Ros1Ur_dashboard_msgsAddToLogResponse=roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse;
/** Complete bounded guest value for ur_dashboard_msgs/GetLoadedProgramRequest. */
using Ros1Ur_dashboard_msgsGetLoadedProgramRequest=roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest;
/** Complete bounded guest value for ur_dashboard_msgs/GetLoadedProgramResponse. */
using Ros1Ur_dashboard_msgsGetLoadedProgramResponse=roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse;
/** Complete bounded guest value for ur_dashboard_msgs/GetProgramStateRequest. */
using Ros1Ur_dashboard_msgsGetProgramStateRequest=roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest;
/** Complete bounded guest value for ur_dashboard_msgs/ProgramState. */
using Ros1Ur_dashboard_msgsProgramState=roswasmc_bounded_1_ur_dashboard_msgs_ProgramState;
/** Complete bounded guest value for ur_dashboard_msgs/GetProgramStateResponse. */
using Ros1Ur_dashboard_msgsGetProgramStateResponse=roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse;
/** Complete bounded guest value for ur_dashboard_msgs/GetRobotModeRequest. */
using Ros1Ur_dashboard_msgsGetRobotModeRequest=roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest;
/** Complete bounded guest value for ur_dashboard_msgs/RobotMode. */
using Ros1Ur_dashboard_msgsRobotMode=roswasmc_bounded_1_ur_dashboard_msgs_RobotMode;
/** Complete bounded guest value for ur_dashboard_msgs/GetRobotModeResponse. */
using Ros1Ur_dashboard_msgsGetRobotModeResponse=roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse;
/** Complete bounded guest value for ur_dashboard_msgs/GetSafetyModeRequest. */
using Ros1Ur_dashboard_msgsGetSafetyModeRequest=roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest;
/** Complete bounded guest value for ur_dashboard_msgs/SafetyMode. */
using Ros1Ur_dashboard_msgsSafetyMode=roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode;
/** Complete bounded guest value for ur_dashboard_msgs/GetSafetyModeResponse. */
using Ros1Ur_dashboard_msgsGetSafetyModeResponse=roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse;
/** Complete bounded guest value for ur_dashboard_msgs/IsInRemoteControlRequest. */
using Ros1Ur_dashboard_msgsIsInRemoteControlRequest=roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest;
/** Complete bounded guest value for ur_dashboard_msgs/IsInRemoteControlResponse. */
using Ros1Ur_dashboard_msgsIsInRemoteControlResponse=roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse;
/** Complete bounded guest value for ur_dashboard_msgs/IsProgramRunningRequest. */
using Ros1Ur_dashboard_msgsIsProgramRunningRequest=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest;
/** Complete bounded guest value for ur_dashboard_msgs/IsProgramRunningResponse. */
using Ros1Ur_dashboard_msgsIsProgramRunningResponse=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse;
/** Complete bounded guest value for ur_dashboard_msgs/IsProgramSavedRequest. */
using Ros1Ur_dashboard_msgsIsProgramSavedRequest=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest;
/** Complete bounded guest value for ur_dashboard_msgs/IsProgramSavedResponse. */
using Ros1Ur_dashboard_msgsIsProgramSavedResponse=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse;
/** Complete bounded guest value for ur_dashboard_msgs/LoadRequest. */
using Ros1Ur_dashboard_msgsLoadRequest=roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest;
/** Complete bounded guest value for ur_dashboard_msgs/LoadResponse. */
using Ros1Ur_dashboard_msgsLoadResponse=roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse;
/** Complete bounded guest value for ur_dashboard_msgs/PopupRequest. */
using Ros1Ur_dashboard_msgsPopupRequest=roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest;
/** Complete bounded guest value for ur_dashboard_msgs/PopupResponse. */
using Ros1Ur_dashboard_msgsPopupResponse=roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse;
/** Complete bounded guest value for ur_dashboard_msgs/RawRequestRequest. */
using Ros1Ur_dashboard_msgsRawRequestRequest=roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest;
/** Complete bounded guest value for ur_dashboard_msgs/RawRequestResponse. */
using Ros1Ur_dashboard_msgsRawRequestResponse=roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse;
/** Complete bounded guest value for ur_dashboard_msgs/SetModeGoal. */
using Ros1Ur_dashboard_msgsSetModeGoal=roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal;
/** Complete bounded guest value for ur_dashboard_msgs/SetModeResult. */
using Ros1Ur_dashboard_msgsSetModeResult=roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult;
/** Complete bounded guest value for ur_dashboard_msgs/SetModeFeedback. */
using Ros1Ur_dashboard_msgsSetModeFeedback=roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback;
/** Complete bounded guest value for ur_msgs/Analog. */
using Ros1Ur_msgsAnalog=roswasmc_bounded_1_ur_msgs_Analog;
/** Complete bounded guest value for ur_msgs/Digital. */
using Ros1Ur_msgsDigital=roswasmc_bounded_1_ur_msgs_Digital;
/** Complete bounded guest value for ur_msgs/GetRobotSoftwareVersionRequest. */
using Ros1Ur_msgsGetRobotSoftwareVersionRequest=roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest;
/** Complete bounded guest value for ur_msgs/GetRobotSoftwareVersionResponse. */
using Ros1Ur_msgsGetRobotSoftwareVersionResponse=roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse;
/** Complete bounded guest value for ur_msgs/IOStates. */
using Ros1Ur_msgsIOStates=roswasmc_bounded_1_ur_msgs_IOStates;
/** Complete bounded guest value for ur_msgs/MasterboardDataMsg. */
using Ros1Ur_msgsMasterboardDataMsg=roswasmc_bounded_1_ur_msgs_MasterboardDataMsg;
/** Complete bounded guest value for ur_msgs/RobotModeDataMsg. */
using Ros1Ur_msgsRobotModeDataMsg=roswasmc_bounded_1_ur_msgs_RobotModeDataMsg;
/** Complete bounded guest value for ur_msgs/RobotStateRTMsg. */
using Ros1Ur_msgsRobotStateRTMsg=roswasmc_bounded_1_ur_msgs_RobotStateRTMsg;
/** Complete bounded guest value for ur_msgs/SetAnalogOutputRequest. */
using Ros1Ur_msgsSetAnalogOutputRequest=roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest;
/** Complete bounded guest value for ur_msgs/SetAnalogOutputResponse. */
using Ros1Ur_msgsSetAnalogOutputResponse=roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse;
/** Complete bounded guest value for ur_msgs/SetForceModeRequest. */
using Ros1Ur_msgsSetForceModeRequest=roswasmc_bounded_1_ur_msgs_SetForceModeRequest;
/** Complete bounded guest value for ur_msgs/SetForceModeResponse. */
using Ros1Ur_msgsSetForceModeResponse=roswasmc_bounded_1_ur_msgs_SetForceModeResponse;
/** Complete bounded guest value for ur_msgs/SetIORequest. */
using Ros1Ur_msgsSetIORequest=roswasmc_bounded_1_ur_msgs_SetIORequest;
/** Complete bounded guest value for ur_msgs/SetIOResponse. */
using Ros1Ur_msgsSetIOResponse=roswasmc_bounded_1_ur_msgs_SetIOResponse;
/** Complete bounded guest value for ur_msgs/SetPayloadRequest. */
using Ros1Ur_msgsSetPayloadRequest=roswasmc_bounded_1_ur_msgs_SetPayloadRequest;
/** Complete bounded guest value for ur_msgs/SetPayloadResponse. */
using Ros1Ur_msgsSetPayloadResponse=roswasmc_bounded_1_ur_msgs_SetPayloadResponse;
/** Complete bounded guest value for ur_msgs/SetSpeedSliderFractionRequest. */
using Ros1Ur_msgsSetSpeedSliderFractionRequest=roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest;
/** Complete bounded guest value for ur_msgs/SetSpeedSliderFractionResponse. */
using Ros1Ur_msgsSetSpeedSliderFractionResponse=roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse;
/** Complete bounded guest value for ur_msgs/ToolDataMsg. */
using Ros1Ur_msgsToolDataMsg=roswasmc_bounded_1_ur_msgs_ToolDataMsg;
}
namespace roswasmcpp {
/** Checked codec for geometry_msgs/Point; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsPoint> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_b55e2bde60f6cedb426cada54bc7baefc536c7bc731bd49fb152d89a892de62b";
  static bool encode(uwvm_custom::Ros1Geometry_msgsPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Point_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Point_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Point_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsPoint& out) {return !roswasmc_bounded_1_geometry_msgs_Point_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Quaternion; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsQuaternion> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_48fe20d5d29a8ca1a630605a58f80baacc52d9a65b1ca87efd50ffdd52882934";
  static bool encode(uwvm_custom::Ros1Geometry_msgsQuaternion const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Quaternion_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Quaternion_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Quaternion_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsQuaternion& out) {return !roswasmc_bounded_1_geometry_msgs_Quaternion_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Pose; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsPose> {
  static constexpr size_t max_wire_bytes=56;
  static constexpr char const* codec_identity="UWVMROS1_e49ed287cd0de31a16c8cd49ad8f41f81d44e23306b8dcb61da6e0e789595471";
  static bool encode(uwvm_custom::Ros1Geometry_msgsPose const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Pose_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Pose_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Pose_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsPose& out) {return !roswasmc_bounded_1_geometry_msgs_Pose_decode(bytes.data(),bytes.size(),&out);}
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
  static constexpr size_t max_wire_bytes=144;
  static constexpr char const* codec_identity="UWVMROS1_ee2b4e736d36ebcdb562d3ef7832777233d39d366837e9148a76aa2c36756435";
  static bool encode(uwvm_custom::Ros1Std_msgsHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_std_msgs_Header_valid(&value))return false;size_t n=roswasmc_bounded_1_std_msgs_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_std_msgs_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Std_msgsHeader& out) {return !roswasmc_bounded_1_std_msgs_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/PoseStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsPoseStamped> {
  static constexpr size_t max_wire_bytes=200;
  static constexpr char const* codec_identity="UWVMROS1_bb59c4e82f561266c0f10500bfbbd1a809e9c1122ecbf559a812326af3eb2e95";
  static bool encode(uwvm_custom::Ros1Geometry_msgsPoseStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_PoseStamped_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_PoseStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_PoseStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsPoseStamped& out) {return !roswasmc_bounded_1_geometry_msgs_PoseStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Vector3; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsVector3> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_e90efcf4803d82763e0b83f8ef5502b5d042158873fcc91be32543d25d7632bc";
  static bool encode(uwvm_custom::Ros1Geometry_msgsVector3 const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Vector3_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsVector3& out) {return !roswasmc_bounded_1_geometry_msgs_Vector3_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Twist; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsTwist> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_d5a69cfff4380fcf3e9c131ddb3dfb6def041cd7308a7ebdbc150b6483dfd921";
  static bool encode(uwvm_custom::Ros1Geometry_msgsTwist const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Twist_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Twist_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Twist_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsTwist& out) {return !roswasmc_bounded_1_geometry_msgs_Twist_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for geometry_msgs/Wrench; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Geometry_msgsWrench> {
  static constexpr size_t max_wire_bytes=48;
  static constexpr char const* codec_identity="UWVMROS1_b0938c4e084df68cf7c46445070ac3e9305d06f678275845aec6055db77617c3";
  static bool encode(uwvm_custom::Ros1Geometry_msgsWrench const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_geometry_msgs_Wrench_valid(&value))return false;size_t n=roswasmc_bounded_1_geometry_msgs_Wrench_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_geometry_msgs_Wrench_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Geometry_msgsWrench& out) {return !roswasmc_bounded_1_geometry_msgs_Wrench_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/AddToLogRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsAddToLogRequest> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_faf6c1ab9c749a05ae3f4e6196cc1c3574560daae1463ebe8684bf13ea938ac4";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsAddToLogRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsAddToLogRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/AddToLogResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsAddToLogResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_d495f7609d04abde44cb35ec5114618a2875e0a30986d2f7cc62c8ce99354e7c";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsAddToLogResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsAddToLogResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/GetLoadedProgramRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsGetLoadedProgramRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_54eecd0b652707f9c16dcbacbef05902112db993500404e1adaf26618e1d3a1a";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsGetLoadedProgramRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsGetLoadedProgramRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/GetLoadedProgramResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsGetLoadedProgramResponse> {
  static constexpr size_t max_wire_bytes=521;
  static constexpr char const* codec_identity="UWVMROS1_1680747345d40412a4731c48f08c402ac97fb12dd871fb320c2a9d002453f87a";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsGetLoadedProgramResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsGetLoadedProgramResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/GetProgramStateRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsGetProgramStateRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_cf14c6f86ef1904f80e88117dc2f2d29a003b52e21bc6569795771f04cf072a2";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsGetProgramStateRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsGetProgramStateRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/ProgramState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsProgramState> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_97a57db64f20abcb9929b5296c75d66d994f2e9e26449810197c657694d3850e";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsProgramState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsProgramState& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/GetProgramStateResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsGetProgramStateResponse> {
  static constexpr size_t max_wire_bytes=781;
  static constexpr char const* codec_identity="UWVMROS1_366caf89303bfff48d3c0f89aa2a640c3d29af5a0c19d76ceccf26d50c3ab2ed";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsGetProgramStateResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsGetProgramStateResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/GetRobotModeRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsGetRobotModeRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_b9ffe283d7dbcb85879dbf6964d393f5d44661227bb26e1acc34308edb288609";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsGetRobotModeRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsGetRobotModeRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/RobotMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsRobotMode> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b2772853eff2ec9b849b6de4a90ce98487a8eff49d1371283dce503908702066";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsRobotMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsRobotMode& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/GetRobotModeResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsGetRobotModeResponse> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_f6c0d0d03a956a34b00fd63540226b6abb8b106e2a35cb912b4886a5c4cecc3f";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsGetRobotModeResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsGetRobotModeResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/GetSafetyModeRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsGetSafetyModeRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_f697e2ff2bcd56523563060a46bca2e1b7af487128f05562fcb18bfa73cbca8b";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsGetSafetyModeRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsGetSafetyModeRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/SafetyMode; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsSafetyMode> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_5a0c2737166cf2e2bd4c17b46c597f674b8d806b802d4e86ee0d76fc55f29cf7";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsSafetyMode const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsSafetyMode& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/GetSafetyModeResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsGetSafetyModeResponse> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_70121af922588eaab9a34855e3da0cde837b1449d2a2d6c28addc70916df1a15";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsGetSafetyModeResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsGetSafetyModeResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/IsInRemoteControlRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsIsInRemoteControlRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_4b3c39f36f953904eb582d8c5f91161f2864782d4a4ab209a7dab32a117175da";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsIsInRemoteControlRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsIsInRemoteControlRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/IsInRemoteControlResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsIsInRemoteControlResponse> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_6c4af27571b48a5b8e8f1f857b2c02a333e7b4b360f8f3645a39630594c1663a";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsIsInRemoteControlResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsIsInRemoteControlResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/IsProgramRunningRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsIsProgramRunningRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_0be481297028886e15f07ace4178ca0aec595724cd83ab01d0155e7c6aa6666f";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsIsProgramRunningRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsIsProgramRunningRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/IsProgramRunningResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsIsProgramRunningResponse> {
  static constexpr size_t max_wire_bytes=262;
  static constexpr char const* codec_identity="UWVMROS1_4d9de4f7e7071d142c09e3184d4a6f29d4044d003ad11ba3b72e1129ecf144ac";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsIsProgramRunningResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsIsProgramRunningResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/IsProgramSavedRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsIsProgramSavedRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_45ce1466c35adc812096bfb8b2cd84dfa87940109831c9f797b5786afa14f5a8";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsIsProgramSavedRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsIsProgramSavedRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/IsProgramSavedResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsIsProgramSavedResponse> {
  static constexpr size_t max_wire_bytes=522;
  static constexpr char const* codec_identity="UWVMROS1_eb53e077fe7b216e27fc57f838666a54a84e479e02e508d41df5cff9f0637a52";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsIsProgramSavedResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsIsProgramSavedResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/LoadRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsLoadRequest> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_dfbde7b2376c7c9ba3926e4803f16b22f7ade3b2f768a25886a1490656edd2bd";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsLoadRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsLoadRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/LoadResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsLoadResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_0ed3b3f2a0066248734e64c0706e52ebf07113569a98baf249a99adfff59be02";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsLoadResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsLoadResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/PopupRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsPopupRequest> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_11556bfb811a28141b6c5e9c11720920a389d735b8c09ed1a8f47d05f293e90d";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsPopupRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsPopupRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/PopupResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsPopupResponse> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_33ac5b2128d6e7c2e562af7e2b8a8e907c16c07034b377920cba4fbd04ca9e14";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsPopupResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsPopupResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/RawRequestRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsRawRequestRequest> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_ca8e3fdb45888bf4feb3c0f4ed05b7b9499a74493aaa0a8727f9f513922adfb4";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsRawRequestRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsRawRequestRequest& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/RawRequestResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsRawRequestResponse> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_76dbb0c8d0143bd4d39bbf66c6866b0771013fe67fbad76887c6490c91f39e80";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsRawRequestResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsRawRequestResponse& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/SetModeGoal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsSetModeGoal> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_27ba58b57af47e8efff15a70fb0562d5904d019c17e39a499bf003730434a528";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsSetModeGoal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsSetModeGoal& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/SetModeResult; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsSetModeResult> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_e558d01061183cabca7022bd66aeb1958e14dfc62226a701c86020e73ffa1114";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsSetModeResult const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsSetModeResult& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_dashboard_msgs/SetModeFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_dashboard_msgsSetModeFeedback> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_d29b507dcc5d72ee086c8a5a9a7a762d677ed408ec02dbe661abc17e2de5c7e2";
  static bool encode(uwvm_custom::Ros1Ur_dashboard_msgsSetModeFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_dashboard_msgsSetModeFeedback& out) {return !roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/Analog; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsAnalog> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_638dc3b30def8193a501b4c1c778792a60c8c683a96880e9fa6db6c01774f6a7";
  static bool encode(uwvm_custom::Ros1Ur_msgsAnalog const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_Analog_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_Analog_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_Analog_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsAnalog& out) {return !roswasmc_bounded_1_ur_msgs_Analog_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/Digital; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsDigital> {
  static constexpr size_t max_wire_bytes=2;
  static constexpr char const* codec_identity="UWVMROS1_c5840cfc5e26fe2f18817e46a0322a92e02ca21983920c71ebeb585769f5622c";
  static bool encode(uwvm_custom::Ros1Ur_msgsDigital const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_Digital_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_Digital_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_Digital_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsDigital& out) {return !roswasmc_bounded_1_ur_msgs_Digital_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/GetRobotSoftwareVersionRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsGetRobotSoftwareVersionRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_bbcdd98ee8d951a8a4fa873ad1c755f16c5cce14ceb7a614343dd44ccf84b9c8";
  static bool encode(uwvm_custom::Ros1Ur_msgsGetRobotSoftwareVersionRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsGetRobotSoftwareVersionRequest& out) {return !roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/GetRobotSoftwareVersionResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsGetRobotSoftwareVersionResponse> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_36ab51ce55e3630f8ca71f364f4c5cc1f4b010f678777e4fd7b03dd480f92e64";
  static bool encode(uwvm_custom::Ros1Ur_msgsGetRobotSoftwareVersionResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsGetRobotSoftwareVersionResponse& out) {return !roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/IOStates; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsIOStates> {
  static constexpr size_t max_wire_bytes=404;
  static constexpr char const* codec_identity="UWVMROS1_4b10f6efdd2f129267c407bc1f8caed03776fef6d49f0a1b9dfbf2451ad8f033";
  static bool encode(uwvm_custom::Ros1Ur_msgsIOStates const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_IOStates_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_IOStates_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_IOStates_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsIOStates& out) {return !roswasmc_bounded_1_ur_msgs_IOStates_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/MasterboardDataMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsMasterboardDataMsg> {
  static constexpr size_t max_wire_bytes=62;
  static constexpr char const* codec_identity="UWVMROS1_f80f92b90cb33dd5b118df81447dfcc33c1ab230a91bb915a1be684401da265f";
  static bool encode(uwvm_custom::Ros1Ur_msgsMasterboardDataMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsMasterboardDataMsg& out) {return !roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/RobotModeDataMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsRobotModeDataMsg> {
  static constexpr size_t max_wire_bytes=15;
  static constexpr char const* codec_identity="UWVMROS1_64c4247d865e0731b13ca13ce2e355df664cabf7f3d646051d9d3ef48b05a231";
  static bool encode(uwvm_custom::Ros1Ur_msgsRobotModeDataMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsRobotModeDataMsg& out) {return !roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/RobotStateRTMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsRobotStateRTMsg> {
  static constexpr size_t max_wire_bytes=768;
  static constexpr char const* codec_identity="UWVMROS1_e3d93b6141f749b93c229d8eabebbeb2782a16621fea4d7d39096794e7a67ce7";
  static bool encode(uwvm_custom::Ros1Ur_msgsRobotStateRTMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsRobotStateRTMsg& out) {return !roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetAnalogOutputRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetAnalogOutputRequest> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_1830a63c8853ebe8842f0e8e6870fbdee136dfe78726ba581fd1c54f910c4789";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetAnalogOutputRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetAnalogOutputRequest& out) {return !roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetAnalogOutputResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetAnalogOutputResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_bb84c969f904082699f67d385b093b0c6de97bfacc33eab86932f4564a823580";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetAnalogOutputResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetAnalogOutputResponse& out) {return !roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetForceModeRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetForceModeRequest> {
  static constexpr size_t max_wire_bytes=335;
  static constexpr char const* codec_identity="UWVMROS1_c148d6eb37258e10f5d16e76e41c334fc57a3d751c0c35c47be4cabf4c015a6b";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetForceModeRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetForceModeRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetForceModeRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetForceModeRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetForceModeRequest& out) {return !roswasmc_bounded_1_ur_msgs_SetForceModeRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetForceModeResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetForceModeResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_bb1eeb0b38d81d642066bed1510245423f62d25983a2de89e985d1fd1932308c";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetForceModeResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetForceModeResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetForceModeResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetForceModeResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetForceModeResponse& out) {return !roswasmc_bounded_1_ur_msgs_SetForceModeResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetIORequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetIORequest> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_3dbd2f05686ddaac16b37dc8ec98303acb672d176be34fc906cf32f2d4cd1872";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetIORequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetIORequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetIORequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetIORequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetIORequest& out) {return !roswasmc_bounded_1_ur_msgs_SetIORequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetIOResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetIOResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_7fc137aa3db3178a450b8b2396b7821c911c79656e52bf48814745c261aecf61";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetIOResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetIOResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetIOResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetIOResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetIOResponse& out) {return !roswasmc_bounded_1_ur_msgs_SetIOResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetPayloadRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetPayloadRequest> {
  static constexpr size_t max_wire_bytes=28;
  static constexpr char const* codec_identity="UWVMROS1_63ae68b44ce6aaad37cbda779fb51dee78923df27c582b4aab4104b16a307ee3";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetPayloadRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetPayloadRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetPayloadRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetPayloadRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetPayloadRequest& out) {return !roswasmc_bounded_1_ur_msgs_SetPayloadRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetPayloadResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetPayloadResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_a3e991f1357f2d3017ccbb7636a6248e8774434d723f5f6c77ff251dd61fce73";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetPayloadResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetPayloadResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetPayloadResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetPayloadResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetPayloadResponse& out) {return !roswasmc_bounded_1_ur_msgs_SetPayloadResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetSpeedSliderFractionRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetSpeedSliderFractionRequest> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_f192d6d655b2ed7725bf5afe6977a352870abfd4d07e0c3380a09d280a0a0a32";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetSpeedSliderFractionRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetSpeedSliderFractionRequest& out) {return !roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/SetSpeedSliderFractionResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsSetSpeedSliderFractionResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_cb08c6758ce2eeeac93bc579d7895b19b84e74b7e2d19d87831023df579288b0";
  static bool encode(uwvm_custom::Ros1Ur_msgsSetSpeedSliderFractionResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsSetSpeedSliderFractionResponse& out) {return !roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for ur_msgs/ToolDataMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Ur_msgsToolDataMsg> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_1fad1aba069bc474306dc421a62950ffeb3a0837668f08a9adf41ac1bc2bb390";
  static bool encode(uwvm_custom::Ros1Ur_msgsToolDataMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_ur_msgs_ToolDataMsg_valid(&value))return false;size_t n=roswasmc_bounded_1_ur_msgs_ToolDataMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_ur_msgs_ToolDataMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Ur_msgsToolDataMsg& out) {return !roswasmc_bounded_1_ur_msgs_ToolDataMsg_decode(bytes.data(),bytes.size(),&out);}
};
}
