/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for booster_interface/msg/BoosterApiReqMsg. */
using Ros2Booster_interfaceMsgBoosterApiReqMsg=roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg;
/** Complete bounded guest value for booster_interface/msg/BoosterApiRespMsg. */
using Ros2Booster_interfaceMsgBoosterApiRespMsg=roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg;
/** Complete bounded guest value for booster_interface/msg/ButtonEventMsg. */
using Ros2Booster_interfaceMsgButtonEventMsg=roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg;
/** Complete bounded guest value for booster_interface/msg/FallDownState. */
using Ros2Booster_interfaceMsgFallDownState=roswasmc_bounded_2_booster_interface_msg_FallDownState;
/** Complete bounded guest value for booster_interface/msg/HandParam. */
using Ros2Booster_interfaceMsgHandParam=roswasmc_bounded_2_booster_interface_msg_HandParam;
/** Complete bounded guest value for booster_interface/msg/HandCommand. */
using Ros2Booster_interfaceMsgHandCommand=roswasmc_bounded_2_booster_interface_msg_HandCommand;
/** Complete bounded guest value for booster_interface/msg/HandDdsMsg. */
using Ros2Booster_interfaceMsgHandDdsMsg=roswasmc_bounded_2_booster_interface_msg_HandDdsMsg;
/** Complete bounded guest value for booster_interface/msg/ImuState. */
using Ros2Booster_interfaceMsgImuState=roswasmc_bounded_2_booster_interface_msg_ImuState;
/** Complete bounded guest value for booster_interface/msg/MotorCmd. */
using Ros2Booster_interfaceMsgMotorCmd=roswasmc_bounded_2_booster_interface_msg_MotorCmd;
/** Complete bounded guest value for booster_interface/msg/LowCmd. */
using Ros2Booster_interfaceMsgLowCmd=roswasmc_bounded_2_booster_interface_msg_LowCmd;
/** Complete bounded guest value for booster_interface/msg/MotorState. */
using Ros2Booster_interfaceMsgMotorState=roswasmc_bounded_2_booster_interface_msg_MotorState;
/** Complete bounded guest value for booster_interface/msg/LowState. */
using Ros2Booster_interfaceMsgLowState=roswasmc_bounded_2_booster_interface_msg_LowState;
/** Complete bounded guest value for booster_interface/msg/Odometer. */
using Ros2Booster_interfaceMsgOdometer=roswasmc_bounded_2_booster_interface_msg_Odometer;
/** Complete bounded guest value for booster_interface/msg/RawBytesMsg. */
using Ros2Booster_interfaceMsgRawBytesMsg=roswasmc_bounded_2_booster_interface_msg_RawBytesMsg;
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for booster_interface/msg/RawBytesStamped. */
using Ros2Booster_interfaceMsgRawBytesStamped=roswasmc_bounded_2_booster_interface_msg_RawBytesStamped;
/** Complete bounded guest value for booster_interface/msg/RemoteControllerState. */
using Ros2Booster_interfaceMsgRemoteControllerState=roswasmc_bounded_2_booster_interface_msg_RemoteControllerState;
/** Complete bounded guest value for booster_interface/srv/AgentService_Request. */
using Ros2Booster_interfaceSrvAgentService_Request=roswasmc_bounded_2_booster_interface_srv_AgentService_Request;
/** Complete bounded guest value for booster_interface/srv/AgentService_Response. */
using Ros2Booster_interfaceSrvAgentService_Response=roswasmc_bounded_2_booster_interface_srv_AgentService_Response;
/** Complete bounded guest value for booster_interface/srv/RpcService_Request. */
using Ros2Booster_interfaceSrvRpcService_Request=roswasmc_bounded_2_booster_interface_srv_RpcService_Request;
/** Complete bounded guest value for booster_interface/srv/RpcService_Response. */
using Ros2Booster_interfaceSrvRpcService_Response=roswasmc_bounded_2_booster_interface_srv_RpcService_Response;
}
namespace roswasmcpp {
/** Checked codec for booster_interface/msg/BoosterApiReqMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgBoosterApiReqMsg> {
  static constexpr size_t max_wire_bytes=4108;
  static constexpr char const* codec_identity="UWVMROS1_e2f9b66467bcb40a8194c744694f314e7fca56061f1cf3121eed565f724edd40";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgBoosterApiReqMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgBoosterApiReqMsg& out) {return !roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/BoosterApiRespMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgBoosterApiRespMsg> {
  static constexpr size_t max_wire_bytes=4108;
  static constexpr char const* codec_identity="UWVMROS1_21ce208a51f60843eecb72edbc408e926353b1a6f178424c0b691ac237fed11f";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgBoosterApiRespMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgBoosterApiRespMsg& out) {return !roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/ButtonEventMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgButtonEventMsg> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_b61682fd9c35bd54c52000224411f4373b8620e16587b31d8a683076e0d14fe4";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgButtonEventMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgButtonEventMsg& out) {return !roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/FallDownState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgFallDownState> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_8bf168ccefac72eda6070704c70e12e62776d29ad3db908095cc01220f4b533f";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgFallDownState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_FallDownState_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_FallDownState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_FallDownState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgFallDownState& out) {return !roswasmc_bounded_2_booster_interface_msg_FallDownState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/HandParam; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgHandParam> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_3b91722e751e7c00c773335bb42af34850190ecc26a029e8792c49d912def158";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgHandParam const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_HandParam_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_HandParam_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_HandParam_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgHandParam& out) {return !roswasmc_bounded_2_booster_interface_msg_HandParam_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/HandCommand; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgHandCommand> {
  static constexpr size_t max_wire_bytes=528;
  static constexpr char const* codec_identity="UWVMROS1_06f3d6c0e9bef1da409330038aadbfacfe60b02e099f2ae0c4dcb92a632b20a0";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgHandCommand const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_HandCommand_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_HandCommand_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_HandCommand_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgHandCommand& out) {return !roswasmc_bounded_2_booster_interface_msg_HandCommand_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/HandDdsMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgHandDdsMsg> {
  static constexpr size_t max_wire_bytes=1060;
  static constexpr char const* codec_identity="UWVMROS1_55af9d6f272a72e75e86e1a6164d235aca5f2ce99c2529749c0d5e33d6091397";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgHandDdsMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgHandDdsMsg& out) {return !roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/ImuState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgImuState> {
  static constexpr size_t max_wire_bytes=36;
  static constexpr char const* codec_identity="UWVMROS1_838ee8d4b93b0c3673e0b535fd3905913e94be24d6d21a01504dedaaaaa81092";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgImuState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_ImuState_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_ImuState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_ImuState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgImuState& out) {return !roswasmc_bounded_2_booster_interface_msg_ImuState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/MotorCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgMotorCmd> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_69d814e13595d53dcf44f2a5ccb19df0f8f2be0908c310c3b60cec33bcf27392";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgMotorCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_MotorCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_MotorCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_MotorCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgMotorCmd& out) {return !roswasmc_bounded_2_booster_interface_msg_MotorCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/LowCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgLowCmd> {
  static constexpr size_t max_wire_bytes=730;
  static constexpr char const* codec_identity="UWVMROS1_845726a14e61d211b493041f4dda56e1556b76b9d30337a2ee5f6319a3d1e870";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgLowCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_LowCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_LowCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_LowCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgLowCmd& out) {return !roswasmc_bounded_2_booster_interface_msg_LowCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/MotorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgMotorState> {
  static constexpr size_t max_wire_bytes=30;
  static constexpr char const* codec_identity="UWVMROS1_6b19c0e8865e796fff5a498cff731ba268fa92ea2944e6c2c5869009b3aa066b";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgMotorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_MotorState_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_MotorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_MotorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgMotorState& out) {return !roswasmc_bounded_2_booster_interface_msg_MotorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/LowState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgLowState> {
  static constexpr size_t max_wire_bytes=1784;
  static constexpr char const* codec_identity="UWVMROS1_811db19cab7b790ed48506bca3e3ef512c1a661dc3ac99cbce6234efb6de631f";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgLowState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_LowState_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_LowState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_LowState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgLowState& out) {return !roswasmc_bounded_2_booster_interface_msg_LowState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/Odometer; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgOdometer> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_4fea33e3e2ae186dbeaf49c6068019b702e742266541087a5d280b9ce42c671a";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgOdometer const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_Odometer_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_Odometer_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_Odometer_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgOdometer& out) {return !roswasmc_bounded_2_booster_interface_msg_Odometer_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/RawBytesMsg; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgRawBytesMsg> {
  static constexpr size_t max_wire_bytes=2052;
  static constexpr char const* codec_identity="UWVMROS1_557e8e79f206c9cc8b820ed7c2ad8cc01e9b01327197b745709993d958b4578c";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgRawBytesMsg const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgRawBytesMsg& out) {return !roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_decode(bytes.data(),bytes.size(),&out);}
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
  static constexpr size_t max_wire_bytes=140;
  static constexpr char const* codec_identity="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/RawBytesStamped; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgRawBytesStamped> {
  static constexpr size_t max_wire_bytes=2192;
  static constexpr char const* codec_identity="UWVMROS1_27cddd472b2d8c41c39effb87a8a53261aff309500dbb68df6ebd69a19c7720e";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgRawBytesStamped const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgRawBytesStamped& out) {return !roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/msg/RemoteControllerState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceMsgRemoteControllerState> {
  static constexpr size_t max_wire_bytes=42;
  static constexpr char const* codec_identity="UWVMROS1_f8a21705728862632c27b71d98c044a12bf1f9f9e0c1729fec27afe2c60c48a7";
  static bool encode(uwvm_custom::Ros2Booster_interfaceMsgRemoteControllerState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceMsgRemoteControllerState& out) {return !roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/srv/AgentService_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceSrvAgentService_Request> {
  static constexpr size_t max_wire_bytes=4100;
  static constexpr char const* codec_identity="UWVMROS1_d8b972ee6c7f82ee948c03b22354d1073f120ad2616a9e833b578348775404e8";
  static bool encode(uwvm_custom::Ros2Booster_interfaceSrvAgentService_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_srv_AgentService_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_srv_AgentService_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_srv_AgentService_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceSrvAgentService_Request& out) {return !roswasmc_bounded_2_booster_interface_srv_AgentService_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/srv/AgentService_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceSrvAgentService_Response> {
  static constexpr size_t max_wire_bytes=4100;
  static constexpr char const* codec_identity="UWVMROS1_86aecf9ee8652016b5c74e30d2dd4100e8b547542a81a98e7795942b47b87520";
  static bool encode(uwvm_custom::Ros2Booster_interfaceSrvAgentService_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_srv_AgentService_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_srv_AgentService_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_srv_AgentService_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceSrvAgentService_Response& out) {return !roswasmc_bounded_2_booster_interface_srv_AgentService_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/srv/RpcService_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceSrvRpcService_Request> {
  static constexpr size_t max_wire_bytes=4108;
  static constexpr char const* codec_identity="UWVMROS1_cc8eb67d80bdf131f1878a1be998d68560e235ffef6a539876410dfcd6c00150";
  static bool encode(uwvm_custom::Ros2Booster_interfaceSrvRpcService_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_srv_RpcService_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_srv_RpcService_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_srv_RpcService_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceSrvRpcService_Request& out) {return !roswasmc_bounded_2_booster_interface_srv_RpcService_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for booster_interface/srv/RpcService_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Booster_interfaceSrvRpcService_Response> {
  static constexpr size_t max_wire_bytes=4108;
  static constexpr char const* codec_identity="UWVMROS1_7aeb7b45542f9f594e1f61be2f715abcde54b1e55efdde15100b0c553ae96322";
  static bool encode(uwvm_custom::Ros2Booster_interfaceSrvRpcService_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_booster_interface_srv_RpcService_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_booster_interface_srv_RpcService_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_booster_interface_srv_RpcService_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Booster_interfaceSrvRpcService_Response& out) {return !roswasmc_bounded_2_booster_interface_srv_RpcService_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
