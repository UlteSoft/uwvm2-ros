/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for unitree_go/msg/AudioData. */
using Ros2Unitree_goMsgAudioData=roswasmc_bounded_2_unitree_go_msg_AudioData;
/** Complete bounded guest value for unitree_go/msg/BmsCmd. */
using Ros2Unitree_goMsgBmsCmd=roswasmc_bounded_2_unitree_go_msg_BmsCmd;
/** Complete bounded guest value for unitree_go/msg/BmsState. */
using Ros2Unitree_goMsgBmsState=roswasmc_bounded_2_unitree_go_msg_BmsState;
/** Complete bounded guest value for unitree_go/msg/Error. */
using Ros2Unitree_goMsgError=roswasmc_bounded_2_unitree_go_msg_Error;
/** Complete bounded guest value for unitree_go/msg/Go2FrontVideoData. */
using Ros2Unitree_goMsgGo2FrontVideoData=roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData;
/** Complete bounded guest value for unitree_go/msg/HeightMap. */
using Ros2Unitree_goMsgHeightMap=roswasmc_bounded_2_unitree_go_msg_HeightMap;
/** Complete bounded guest value for unitree_go/msg/IMUState. */
using Ros2Unitree_goMsgIMUState=roswasmc_bounded_2_unitree_go_msg_IMUState;
/** Complete bounded guest value for unitree_go/msg/InterfaceConfig. */
using Ros2Unitree_goMsgInterfaceConfig=roswasmc_bounded_2_unitree_go_msg_InterfaceConfig;
/** Complete bounded guest value for unitree_go/msg/LidarState. */
using Ros2Unitree_goMsgLidarState=roswasmc_bounded_2_unitree_go_msg_LidarState;
/** Complete bounded guest value for unitree_go/msg/MotorCmd. */
using Ros2Unitree_goMsgMotorCmd=roswasmc_bounded_2_unitree_go_msg_MotorCmd;
/** Complete bounded guest value for unitree_go/msg/LowCmd. */
using Ros2Unitree_goMsgLowCmd=roswasmc_bounded_2_unitree_go_msg_LowCmd;
/** Complete bounded guest value for unitree_go/msg/MotorState. */
using Ros2Unitree_goMsgMotorState=roswasmc_bounded_2_unitree_go_msg_MotorState;
/** Complete bounded guest value for unitree_go/msg/LowState. */
using Ros2Unitree_goMsgLowState=roswasmc_bounded_2_unitree_go_msg_LowState;
/** Complete bounded guest value for unitree_go/msg/MotorCmds. */
using Ros2Unitree_goMsgMotorCmds=roswasmc_bounded_2_unitree_go_msg_MotorCmds;
/** Complete bounded guest value for unitree_go/msg/MotorStates. */
using Ros2Unitree_goMsgMotorStates=roswasmc_bounded_2_unitree_go_msg_MotorStates;
/** Complete bounded guest value for unitree_go/msg/PathPoint. */
using Ros2Unitree_goMsgPathPoint=roswasmc_bounded_2_unitree_go_msg_PathPoint;
/** Complete bounded guest value for unitree_go/msg/Req. */
using Ros2Unitree_goMsgReq=roswasmc_bounded_2_unitree_go_msg_Req;
/** Complete bounded guest value for unitree_go/msg/Res. */
using Ros2Unitree_goMsgRes=roswasmc_bounded_2_unitree_go_msg_Res;
/** Complete bounded guest value for unitree_go/msg/SportModeCmd. */
using Ros2Unitree_goMsgSportModeCmd=roswasmc_bounded_2_unitree_go_msg_SportModeCmd;
/** Complete bounded guest value for unitree_go/msg/TimeSpec. */
using Ros2Unitree_goMsgTimeSpec=roswasmc_bounded_2_unitree_go_msg_TimeSpec;
/** Complete bounded guest value for unitree_go/msg/SportModeState. */
using Ros2Unitree_goMsgSportModeState=roswasmc_bounded_2_unitree_go_msg_SportModeState;
/** Complete bounded guest value for unitree_go/msg/UwbState. */
using Ros2Unitree_goMsgUwbState=roswasmc_bounded_2_unitree_go_msg_UwbState;
/** Complete bounded guest value for unitree_go/msg/UwbSwitch. */
using Ros2Unitree_goMsgUwbSwitch=roswasmc_bounded_2_unitree_go_msg_UwbSwitch;
/** Complete bounded guest value for unitree_go/msg/WirelessController. */
using Ros2Unitree_goMsgWirelessController=roswasmc_bounded_2_unitree_go_msg_WirelessController;
}
namespace roswasmcpp {
/** Checked codec for unitree_go/msg/AudioData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgAudioData> {
  static constexpr size_t max_wire_bytes=2060;
  static constexpr char const* codec_identity="UWVMROS1_aa14877a076c7fc6b662efa965cc8a75f42b3f84990a078c7c3c5def01d2c9fe";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgAudioData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_AudioData_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_AudioData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_AudioData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgAudioData& out) {return !roswasmc_bounded_2_unitree_go_msg_AudioData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/BmsCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgBmsCmd> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_d212c1ab0c92d64801a3b10c3a4d79517ea24a830db3d85ca2bb18e1e0ea5339";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgBmsCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_BmsCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_BmsCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_BmsCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgBmsCmd& out) {return !roswasmc_bounded_2_unitree_go_msg_BmsCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/BmsState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgBmsState> {
  static constexpr size_t max_wire_bytes=44;
  static constexpr char const* codec_identity="UWVMROS1_7e4d5e3c0a2eddddf39c68b73bff5f7721e1623b0a9322dce23d3e551a07f5b0";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgBmsState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_BmsState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_BmsState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_BmsState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgBmsState& out) {return !roswasmc_bounded_2_unitree_go_msg_BmsState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/Error; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgError> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_c3e572e08288b6ed0b32575e645fe546f92b5091d62e57db01ec00d27f5e4afd";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgError const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_Error_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_Error_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_Error_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgError& out) {return !roswasmc_bounded_2_unitree_go_msg_Error_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/Go2FrontVideoData; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgGo2FrontVideoData> {
  static constexpr size_t max_wire_bytes=3092;
  static constexpr char const* codec_identity="UWVMROS1_322b6bd4e563436838fe6a05174d173c0d95eb48108b889dca4f61ce6cc71c60";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgGo2FrontVideoData const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgGo2FrontVideoData& out) {return !roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/HeightMap; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgHeightMap> {
  static constexpr size_t max_wire_bytes=8356;
  static constexpr char const* codec_identity="UWVMROS1_f4aed8c4e1ba382c418ca83a10ae617355f1169757128be8b7cca46611bb0012";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgHeightMap const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_HeightMap_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_HeightMap_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_HeightMap_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgHeightMap& out) {return !roswasmc_bounded_2_unitree_go_msg_HeightMap_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/IMUState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgIMUState> {
  static constexpr size_t max_wire_bytes=53;
  static constexpr char const* codec_identity="UWVMROS1_e5326e97c3bfaa06962ad1989d1aa13fa0e60af15d39d9ef6da6ac0a37261bbc";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgIMUState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_IMUState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_IMUState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_IMUState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgIMUState& out) {return !roswasmc_bounded_2_unitree_go_msg_IMUState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/InterfaceConfig; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgInterfaceConfig> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_9b96ed6672f6a3e039edb28dec8384cd195116dda929d5ba7be85ef7e7668110";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgInterfaceConfig const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgInterfaceConfig& out) {return !roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/LidarState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgLidarState> {
  static constexpr size_t max_wire_bytes=465;
  static constexpr char const* codec_identity="UWVMROS1_edc9253eb66e466ae5a893b830b0bc5c9102ef18d2bbed7f7d9ead67bcb1a8e6";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgLidarState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_LidarState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_LidarState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_LidarState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgLidarState& out) {return !roswasmc_bounded_2_unitree_go_msg_LidarState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/MotorCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgMotorCmd> {
  static constexpr size_t max_wire_bytes=33;
  static constexpr char const* codec_identity="UWVMROS1_e5b88c02a6d339ae4ae2706678a3d82f89904f024b6068f4fd174e1e045b0989";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgMotorCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_MotorCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_MotorCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_MotorCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgMotorCmd& out) {return !roswasmc_bounded_2_unitree_go_msg_MotorCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/LowCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgLowCmd> {
  static constexpr size_t max_wire_bytes=749;
  static constexpr char const* codec_identity="UWVMROS1_efd061d06d38ecc686edb95d1a891c26860b16da1e35747fc97eeaf4a5d93d63";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgLowCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_LowCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_LowCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_LowCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgLowCmd& out) {return !roswasmc_bounded_2_unitree_go_msg_LowCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/MotorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgMotorState> {
  static constexpr size_t max_wire_bytes=42;
  static constexpr char const* codec_identity="UWVMROS1_b0b5e638b9fd9eec6d0019422d417f80c60e61d47f4750fe9adac8c5084a2a62";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgMotorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_MotorState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_MotorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_MotorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgMotorState& out) {return !roswasmc_bounded_2_unitree_go_msg_MotorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/LowState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgLowState> {
  static constexpr size_t max_wire_bytes=1050;
  static constexpr char const* codec_identity="UWVMROS1_56c01d52b26d7b2ef1fa7475fc8a353777ed6952a6f1ceee01da81f1edad3cab";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgLowState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_LowState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_LowState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_LowState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgLowState& out) {return !roswasmc_bounded_2_unitree_go_msg_LowState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/MotorCmds; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgMotorCmds> {
  static constexpr size_t max_wire_bytes=664;
  static constexpr char const* codec_identity="UWVMROS1_59f6b09147eb868cad93205c27cdd339e55f5928c8eef32da11ec5675c61a671";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgMotorCmds const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_MotorCmds_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_MotorCmds_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_MotorCmds_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgMotorCmds& out) {return !roswasmc_bounded_2_unitree_go_msg_MotorCmds_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/MotorStates; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgMotorStates> {
  static constexpr size_t max_wire_bytes=844;
  static constexpr char const* codec_identity="UWVMROS1_0843528158199493384c36a65b0fb1e15244d21160c33aac82f9a87718c08178";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgMotorStates const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_MotorStates_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_MotorStates_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_MotorStates_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgMotorStates& out) {return !roswasmc_bounded_2_unitree_go_msg_MotorStates_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/PathPoint; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgPathPoint> {
  static constexpr size_t max_wire_bytes=28;
  static constexpr char const* codec_identity="UWVMROS1_35eb1cc629dcecbace17b342029c287310c30d4e6d988740bf2c212a1251cee4";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgPathPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_PathPoint_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_PathPoint_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_PathPoint_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgPathPoint& out) {return !roswasmc_bounded_2_unitree_go_msg_PathPoint_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/Req; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgReq> {
  static constexpr size_t max_wire_bytes=4232;
  static constexpr char const* codec_identity="UWVMROS1_43e0126993336d81a3f6a6706979c19f3a193f6a1ef504cadcd864be8bfaf84e";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgReq const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_Req_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_Req_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_Req_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgReq& out) {return !roswasmc_bounded_2_unitree_go_msg_Req_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/Res; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgRes> {
  static constexpr size_t max_wire_bytes=5260;
  static constexpr char const* codec_identity="UWVMROS1_53c1d85c0cb17cfdd7d3799d6e92602a70700e8c55da4045f501a6a12ff55bfb";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgRes const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_Res_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_Res_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_Res_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgRes& out) {return !roswasmc_bounded_2_unitree_go_msg_Res_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/SportModeCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgSportModeCmd> {
  static constexpr size_t max_wire_bytes=887;
  static constexpr char const* codec_identity="UWVMROS1_cf90a065007b9b8cedde3f0b58aa79da3b39a1456dc1b5297f4fe799d92ff22c";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgSportModeCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_SportModeCmd_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_SportModeCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_SportModeCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgSportModeCmd& out) {return !roswasmc_bounded_2_unitree_go_msg_SportModeCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/TimeSpec; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgTimeSpec> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_9ad3c6ed5a9ff0bba22593b50e1d777ae2f63480a01a744d777ee6b7dad0ecc4";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgTimeSpec const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_TimeSpec_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_TimeSpec_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_TimeSpec_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgTimeSpec& out) {return !roswasmc_bounded_2_unitree_go_msg_TimeSpec_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/SportModeState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgSportModeState> {
  static constexpr size_t max_wire_bytes=227;
  static constexpr char const* codec_identity="UWVMROS1_2935b9d838087267840b379582862a15c2e1a72fe8ad2bcbf8f19af2f8054760";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgSportModeState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_SportModeState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_SportModeState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_SportModeState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgSportModeState& out) {return !roswasmc_bounded_2_unitree_go_msg_SportModeState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/UwbState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgUwbState> {
  static constexpr size_t max_wire_bytes=55;
  static constexpr char const* codec_identity="UWVMROS1_017fbafd61a006aeaf6cb80456ac89f429d30a562da2e0d394c6cfb406e61a34";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgUwbState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_UwbState_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_UwbState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_UwbState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgUwbState& out) {return !roswasmc_bounded_2_unitree_go_msg_UwbState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/UwbSwitch; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgUwbSwitch> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_4546618797829b3911d1533d8d6bacc9da969d40d7752424de34bbd3de4bede5";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgUwbSwitch const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_UwbSwitch_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_UwbSwitch_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_UwbSwitch_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgUwbSwitch& out) {return !roswasmc_bounded_2_unitree_go_msg_UwbSwitch_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_go/msg/WirelessController; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_goMsgWirelessController> {
  static constexpr size_t max_wire_bytes=18;
  static constexpr char const* codec_identity="UWVMROS1_536e69ce5459227cb68b3c6d484a96bfa40e1400aab820f8ad0d0d62ff6ba438";
  static bool encode(uwvm_custom::Ros2Unitree_goMsgWirelessController const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_go_msg_WirelessController_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_go_msg_WirelessController_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_go_msg_WirelessController_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_goMsgWirelessController& out) {return !roswasmc_bounded_2_unitree_go_msg_WirelessController_decode(bytes.data(),bytes.size(),&out);}
};
}
