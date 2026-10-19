/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for uwvm_limx_bridge/msg/NativeHeader. */
using Ros2Uwvm_limx_bridgeMsgNativeHeader=roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader;
/** Complete bounded guest value for uwvm_limx_bridge/msg/ImuData. */
using Ros2Uwvm_limx_bridgeMsgImuData=roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData;
/** Complete bounded guest value for uwvm_limx_bridge/msg/JointState. */
using Ros2Uwvm_limx_bridgeMsgJointState=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState;
/** Complete bounded guest value for uwvm_limx_bridge/msg/JointCmd. */
using Ros2Uwvm_limx_bridgeMsgJointCmd=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd;
/** Complete bounded guest value for uwvm_limx_bridge/msg/JointCmdLimx. */
using Ros2Uwvm_limx_bridgeMsgJointCmdLimx=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx;
}
namespace roswasmcpp {
/** Checked codec for uwvm_limx_bridge/msg/NativeHeader; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_limx_bridgeMsgNativeHeader> {
  static constexpr size_t max_wire_bytes=144;
  static constexpr char const* codec_identity="UWVMROS1_d86b96b26db6c2a28dfa86e2255cb3cccd6cd24648e877746d8d75394d1635b2";
  static bool encode(uwvm_custom::Ros2Uwvm_limx_bridgeMsgNativeHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_limx_bridgeMsgNativeHeader& out) {return !roswasmc_bounded_2_uwvm_limx_bridge_msg_NativeHeader_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_limx_bridge/msg/ImuData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_limx_bridgeMsgImuData> {
  static constexpr size_t max_wire_bytes=260;
  static constexpr char const* codec_identity="UWVMROS1_53ba9eb23b36de77e1793bdd19a31f708aca557b81f4ae72927b2a44f5ce097b";
  static bool encode(uwvm_custom::Ros2Uwvm_limx_bridgeMsgImuData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_limx_bridgeMsgImuData& out) {return !roswasmc_bounded_2_uwvm_limx_bridge_msg_ImuData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_limx_bridge/msg/JointState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointState> {
  static constexpr size_t max_wire_bytes=5544;
  static constexpr char const* codec_identity="UWVMROS1_d99e5855c0dd55d4d1b5a724ad5719734a668a512ea09e553c8f4a560b97a3a4";
  static bool encode(uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointState& out) {return !roswasmc_bounded_2_uwvm_limx_bridge_msg_JointState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_limx_bridge/msg/JointCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointCmd> {
  static constexpr size_t max_wire_bytes=5872;
  static constexpr char const* codec_identity="UWVMROS1_8c477a93cc192cd3039b285f111a56eb1305707d798587ef7ab9504f714ce6f0";
  static bool encode(uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointCmd& out) {return !roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_limx_bridge/msg/JointCmdLimx; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointCmdLimx> {
  static constexpr size_t max_wire_bytes=5940;
  static constexpr char const* codec_identity="UWVMROS1_d469dd32f446fb4013144b7e176cdb831c146b6a9279ea776ec4a0c053060be9";
  static bool encode(uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointCmdLimx const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_limx_bridgeMsgJointCmdLimx& out) {return !roswasmc_bounded_2_uwvm_limx_bridge_msg_JointCmdLimx_decode(bytes.data(),bytes.size(),&out);}
};
}
