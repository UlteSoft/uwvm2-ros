/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for uwvm_booster_sdk_bridge/msg/ImuState. */
using Ros2Uwvm_booster_sdk_bridgeMsgImuState=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState;
/** Complete bounded guest value for uwvm_booster_sdk_bridge/msg/MotorCmd. */
using Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd;
/** Complete bounded guest value for uwvm_booster_sdk_bridge/msg/MotorState. */
using Ros2Uwvm_booster_sdk_bridgeMsgMotorState=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState;
/** Complete bounded guest value for uwvm_booster_sdk_bridge/msg/LowCmd. */
using Ros2Uwvm_booster_sdk_bridgeMsgLowCmd=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd;
/** Complete bounded guest value for uwvm_booster_sdk_bridge/msg/LowState. */
using Ros2Uwvm_booster_sdk_bridgeMsgLowState=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState;
}
namespace roswasmcpp {
/** Checked codec for uwvm_booster_sdk_bridge/msg/ImuState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgImuState> {
  static constexpr size_t max_wire_bytes=36;
  static constexpr char const* codec_identity="UWVMROS1_858ad4a47d2409da6b4e2513178d7d3b0d4568d6ce488729de0b9a220a100f04";
  static bool encode(uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgImuState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgImuState& out) {return !roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_booster_sdk_bridge/msg/MotorCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_66fec912031dfbd29b8e4e747dcdc2dc465506bd003323bc929929bf6a9d31a4";
  static bool encode(uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd& out) {return !roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_booster_sdk_bridge/msg/MotorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgMotorState> {
  static constexpr size_t max_wire_bytes=30;
  static constexpr char const* codec_identity="UWVMROS1_958168dc1e32d50827f255080117a82421d7fb6ae4ad6a05763ce2d3173a30af";
  static bool encode(uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgMotorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgMotorState& out) {return !roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_booster_sdk_bridge/msg/LowCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgLowCmd> {
  static constexpr size_t max_wire_bytes=733;
  static constexpr char const* codec_identity="UWVMROS1_3324a161b056f9282ffa8dce3254a1456b75f6eae5f4a067e844e125e94cba3a";
  static bool encode(uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgLowCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgLowCmd& out) {return !roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_booster_sdk_bridge/msg/LowState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgLowState> {
  static constexpr size_t max_wire_bytes=1784;
  static constexpr char const* codec_identity="UWVMROS1_ca4bcb79077b0e026cf45a389981622ff9c80570ee813ff5633c605fe5021a58";
  static bool encode(uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgLowState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_booster_sdk_bridgeMsgLowState& out) {return !roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_decode(bytes.data(),bytes.size(),&out);}
};
}
