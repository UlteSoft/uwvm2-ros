/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for unitree_hg/msg/BmsCmd. */
using Ros2Unitree_hgMsgBmsCmd=roswasmc_bounded_2_unitree_hg_msg_BmsCmd;
/** Complete bounded guest value for unitree_hg/msg/BmsState. */
using Ros2Unitree_hgMsgBmsState=roswasmc_bounded_2_unitree_hg_msg_BmsState;
/** Complete bounded guest value for unitree_hg/msg/MotorCmd. */
using Ros2Unitree_hgMsgMotorCmd=roswasmc_bounded_2_unitree_hg_msg_MotorCmd;
/** Complete bounded guest value for unitree_hg/msg/HandCmd. */
using Ros2Unitree_hgMsgHandCmd=roswasmc_bounded_2_unitree_hg_msg_HandCmd;
/** Complete bounded guest value for unitree_hg/msg/MotorState. */
using Ros2Unitree_hgMsgMotorState=roswasmc_bounded_2_unitree_hg_msg_MotorState;
/** Complete bounded guest value for unitree_hg/msg/PressSensorState. */
using Ros2Unitree_hgMsgPressSensorState=roswasmc_bounded_2_unitree_hg_msg_PressSensorState;
/** Complete bounded guest value for unitree_hg/msg/IMUState. */
using Ros2Unitree_hgMsgIMUState=roswasmc_bounded_2_unitree_hg_msg_IMUState;
/** Complete bounded guest value for unitree_hg/msg/HandState. */
using Ros2Unitree_hgMsgHandState=roswasmc_bounded_2_unitree_hg_msg_HandState;
/** Complete bounded guest value for unitree_hg/msg/LowCmd. */
using Ros2Unitree_hgMsgLowCmd=roswasmc_bounded_2_unitree_hg_msg_LowCmd;
/** Complete bounded guest value for unitree_hg/msg/LowState. */
using Ros2Unitree_hgMsgLowState=roswasmc_bounded_2_unitree_hg_msg_LowState;
/** Complete bounded guest value for unitree_hg/msg/MainBoardState. */
using Ros2Unitree_hgMsgMainBoardState=roswasmc_bounded_2_unitree_hg_msg_MainBoardState;
}
namespace roswasmcpp {
/** Checked codec for unitree_hg/msg/BmsCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgBmsCmd> {
  static constexpr size_t max_wire_bytes=41;
  static constexpr char const* codec_identity="UWVMROS1_a267c92d22aa04a15f7d68c87b7638b15a368ac973f49a42021dc91f991b13de";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgBmsCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_BmsCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_BmsCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_BmsCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgBmsCmd& out) {return !roswasmc_bounded_2_unitree_hg_msg_BmsCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/BmsState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgBmsState> {
  static constexpr size_t max_wire_bytes=161;
  static constexpr char const* codec_identity="UWVMROS1_b7ac163c4ab4786d37a9ec027767258adcfc06637ed8b4ef67e30c517397761d";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgBmsState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_BmsState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_BmsState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_BmsState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgBmsState& out) {return !roswasmc_bounded_2_unitree_hg_msg_BmsState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/MotorCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgMotorCmd> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_f3e472df5d061a1515ac185f87b01f702ef2f2bbeff39e42c6c7088d1da26cbb";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgMotorCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_MotorCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_MotorCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_MotorCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgMotorCmd& out) {return !roswasmc_bounded_2_unitree_hg_msg_MotorCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/HandCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgHandCmd> {
  static constexpr size_t max_wire_bytes=895;
  static constexpr char const* codec_identity="UWVMROS1_2ebc0e32e831a48e21f0b15f39cb477d999bf3a415e7863713ce37416020a835";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgHandCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_HandCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_HandCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_HandCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgHandCmd& out) {return !roswasmc_bounded_2_unitree_hg_msg_HandCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/MotorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgMotorState> {
  static constexpr size_t max_wire_bytes=53;
  static constexpr char const* codec_identity="UWVMROS1_65497908fe0136a5c90f11b4a2cda2110507bcc8805f9347ac82be3b32fe5934";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgMotorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_MotorState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_MotorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_MotorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgMotorState& out) {return !roswasmc_bounded_2_unitree_hg_msg_MotorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/PressSensorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgPressSensorState> {
  static constexpr size_t max_wire_bytes=104;
  static constexpr char const* codec_identity="UWVMROS1_4abc296d4acd21132c0c1d185e3fafd9f0db8836a5c16ed040ebcc6127380fe5";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgPressSensorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_PressSensorState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_PressSensorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_PressSensorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgPressSensorState& out) {return !roswasmc_bounded_2_unitree_hg_msg_PressSensorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/IMUState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgIMUState> {
  static constexpr size_t max_wire_bytes=54;
  static constexpr char const* codec_identity="UWVMROS1_d83832e8c27273785113c8388f7f22ea1251d6198817255f13a7e542afb26420";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgIMUState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_IMUState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_IMUState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_IMUState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgIMUState& out) {return !roswasmc_bounded_2_unitree_hg_msg_IMUState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/HandState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgHandState> {
  static constexpr size_t max_wire_bytes=2365;
  static constexpr char const* codec_identity="UWVMROS1_5721fdfc155f15ca331dbd398f07cb909196b07e93cabdb6dfbb67b10d4abc4a";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgHandState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_HandState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_HandState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_HandState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgHandState& out) {return !roswasmc_bounded_2_unitree_hg_msg_HandState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/LowCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgLowCmd> {
  static constexpr size_t max_wire_bytes=897;
  static constexpr char const* codec_identity="UWVMROS1_8303ac6e2e3273251662d31daf966a11ced5b5ac10b93aa6253eebc56f4ec461";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgLowCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_LowCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_LowCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_LowCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgLowCmd& out) {return !roswasmc_bounded_2_unitree_hg_msg_LowCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/LowState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgLowState> {
  static constexpr size_t max_wire_bytes=1983;
  static constexpr char const* codec_identity="UWVMROS1_7eed2692672f536a476643f1d3a07affc8e5b3272663e3d282c199fff1c2fa1d";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgLowState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_LowState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_LowState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_LowState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgLowState& out) {return !roswasmc_bounded_2_unitree_hg_msg_LowState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_hg/msg/MainBoardState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_hgMsgMainBoardState> {
  static constexpr size_t max_wire_bytes=72;
  static constexpr char const* codec_identity="UWVMROS1_2a27a979aadc7bef33a757e169680faa632936c2634ee787f0c9f25cb08535a6";
  static bool encode(uwvm_custom::Ros2Unitree_hgMsgMainBoardState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_hg_msg_MainBoardState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_hg_msg_MainBoardState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_hg_msg_MainBoardState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_hgMsgMainBoardState& out) {return !roswasmc_bounded_2_unitree_hg_msg_MainBoardState_decode(bytes.data(),bytes.size(),&out);}
};
}
