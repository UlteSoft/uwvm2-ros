/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for unitree_legged_msgs/BmsCmd. */
using Ros1Unitree_legged_msgsBmsCmd=roswasmc_bounded_1_unitree_legged_msgs_BmsCmd;
/** Complete bounded guest value for unitree_legged_msgs/BmsState. */
using Ros1Unitree_legged_msgsBmsState=roswasmc_bounded_1_unitree_legged_msgs_BmsState;
/** Complete bounded guest value for unitree_legged_msgs/Cartesian. */
using Ros1Unitree_legged_msgsCartesian=roswasmc_bounded_1_unitree_legged_msgs_Cartesian;
/** Complete bounded guest value for unitree_legged_msgs/LED. */
using Ros1Unitree_legged_msgsLED=roswasmc_bounded_1_unitree_legged_msgs_LED;
/** Complete bounded guest value for unitree_legged_msgs/HighCmd. */
using Ros1Unitree_legged_msgsHighCmd=roswasmc_bounded_1_unitree_legged_msgs_HighCmd;
/** Complete bounded guest value for unitree_legged_msgs/IMU. */
using Ros1Unitree_legged_msgsIMU=roswasmc_bounded_1_unitree_legged_msgs_IMU;
/** Complete bounded guest value for unitree_legged_msgs/MotorState. */
using Ros1Unitree_legged_msgsMotorState=roswasmc_bounded_1_unitree_legged_msgs_MotorState;
/** Complete bounded guest value for unitree_legged_msgs/HighState. */
using Ros1Unitree_legged_msgsHighState=roswasmc_bounded_1_unitree_legged_msgs_HighState;
/** Complete bounded guest value for unitree_legged_msgs/MotorCmd. */
using Ros1Unitree_legged_msgsMotorCmd=roswasmc_bounded_1_unitree_legged_msgs_MotorCmd;
/** Complete bounded guest value for unitree_legged_msgs/LowCmd. */
using Ros1Unitree_legged_msgsLowCmd=roswasmc_bounded_1_unitree_legged_msgs_LowCmd;
/** Complete bounded guest value for unitree_legged_msgs/LowState. */
using Ros1Unitree_legged_msgsLowState=roswasmc_bounded_1_unitree_legged_msgs_LowState;
}
namespace roswasmcpp {
/** Checked codec for unitree_legged_msgs/BmsCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsBmsCmd> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_75732753b156fff8db3ba7e39541444bed2b71e139389a9b5bfbc49a4fa14928";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsBmsCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsBmsCmd& out) {return !roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/BmsState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsBmsState> {
  static constexpr size_t max_wire_bytes=34;
  static constexpr char const* codec_identity="UWVMROS1_ef0f93501f54d0ff760039d86bd1ba1739cb0c888b3a873611679484ccec094a";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsBmsState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_BmsState_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_BmsState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_BmsState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsBmsState& out) {return !roswasmc_bounded_1_unitree_legged_msgs_BmsState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/Cartesian; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsCartesian> {
  static constexpr size_t max_wire_bytes=12;
  static constexpr char const* codec_identity="UWVMROS1_43c7c62ba2a689751a0a15e326c5c9690a9b1f166b6726a0f803b98d2b3fa722";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsCartesian const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_Cartesian_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsCartesian& out) {return !roswasmc_bounded_1_unitree_legged_msgs_Cartesian_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/LED; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsLED> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_b4efac59eaf4d3f09c069e4833dd82b5422980f6a9f27db84950243a47164ea5";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsLED const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_LED_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_LED_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_LED_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsLED& out) {return !roswasmc_bounded_1_unitree_legged_msgs_LED_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/HighCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsHighCmd> {
  static constexpr size_t max_wire_bytes=129;
  static constexpr char const* codec_identity="UWVMROS1_ec6ec8937b2629a4441adc4a21eed83fca940b18d46b970d96e05c957ac90b38";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsHighCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_HighCmd_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_HighCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_HighCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsHighCmd& out) {return !roswasmc_bounded_1_unitree_legged_msgs_HighCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/IMU; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsIMU> {
  static constexpr size_t max_wire_bytes=53;
  static constexpr char const* codec_identity="UWVMROS1_29d9ec99e676fe8747665da588b78c8ab77a3197e281ba892ab389d2d81fa276";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsIMU const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_IMU_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_IMU_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_IMU_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsIMU& out) {return !roswasmc_bounded_1_unitree_legged_msgs_IMU_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/MotorState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsMotorState> {
  static constexpr size_t max_wire_bytes=38;
  static constexpr char const* codec_identity="UWVMROS1_be540cb22db5f08b6b9a8082777c76244e80f3148e9cfd63f510e4b2acdc0abf";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsMotorState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_MotorState_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_MotorState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_MotorState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsMotorState& out) {return !roswasmc_bounded_1_unitree_legged_msgs_MotorState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/HighState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsHighState> {
  static constexpr size_t max_wire_bytes=1087;
  static constexpr char const* codec_identity="UWVMROS1_82ee0efaab21b5cb89e99f5c0f5daec583b48dd5feb2dd6a7e09ba9d9110b53d";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsHighState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_HighState_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_HighState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_HighState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsHighState& out) {return !roswasmc_bounded_1_unitree_legged_msgs_HighState_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/MotorCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsMotorCmd> {
  static constexpr size_t max_wire_bytes=33;
  static constexpr char const* codec_identity="UWVMROS1_632b25f209e72857182d92752aa72e3ab47a45e895cc352ee6dc05f1c1168083";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsMotorCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsMotorCmd& out) {return !roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/LowCmd; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsLowCmd> {
  static constexpr size_t max_wire_bytes=734;
  static constexpr char const* codec_identity="UWVMROS1_11d3ac7c497f01fbf6624c39ec71fa613c462cd62acb20c1f263cdde672e9181";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsLowCmd const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_LowCmd_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_LowCmd_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_LowCmd_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsLowCmd& out) {return !roswasmc_bounded_1_unitree_legged_msgs_LowCmd_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_legged_msgs/LowState; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Unitree_legged_msgsLowState> {
  static constexpr size_t max_wire_bytes=1069;
  static constexpr char const* codec_identity="UWVMROS1_37425ed49c0eebd762b6084ee413c7706fc43defc524414d3f50cdd2d25552c0";
  static bool encode(uwvm_custom::Ros1Unitree_legged_msgsLowState const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_unitree_legged_msgs_LowState_valid(&value))return false;size_t n=roswasmc_bounded_1_unitree_legged_msgs_LowState_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_unitree_legged_msgs_LowState_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Unitree_legged_msgsLowState& out) {return !roswasmc_bounded_1_unitree_legged_msgs_LowState_decode(bytes.data(),bytes.size(),&out);}
};
}
