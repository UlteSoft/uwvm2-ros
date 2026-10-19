/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for uwvm_spot_bridge/srv/Command_Request. */
using Ros2Uwvm_spot_bridgeSrvCommand_Request=roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request;
/** Complete bounded guest value for uwvm_spot_bridge/srv/Command_Response. */
using Ros2Uwvm_spot_bridgeSrvCommand_Response=roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response;
/** Complete bounded guest value for uwvm_spot_bridge/srv/Feedback_Request. */
using Ros2Uwvm_spot_bridgeSrvFeedback_Request=roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request;
/** Complete bounded guest value for uwvm_spot_bridge/srv/Feedback_Response. */
using Ros2Uwvm_spot_bridgeSrvFeedback_Response=roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response;
/** Complete bounded guest value for uwvm_spot_bridge/srv/Lease_Request. */
using Ros2Uwvm_spot_bridgeSrvLease_Request=roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request;
/** Complete bounded guest value for uwvm_spot_bridge/srv/Lease_Response. */
using Ros2Uwvm_spot_bridgeSrvLease_Response=roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response;
/** Complete bounded guest value for uwvm_spot_bridge/srv/State_Request. */
using Ros2Uwvm_spot_bridgeSrvState_Request=roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request;
/** Complete bounded guest value for uwvm_spot_bridge/srv/State_Response. */
using Ros2Uwvm_spot_bridgeSrvState_Response=roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response;
/** Complete bounded guest value for uwvm_spot_bridge/srv/Velocity_Request. */
using Ros2Uwvm_spot_bridgeSrvVelocity_Request=roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request;
/** Complete bounded guest value for uwvm_spot_bridge/srv/Velocity_Response. */
using Ros2Uwvm_spot_bridgeSrvVelocity_Response=roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response;
}
namespace roswasmcpp {
/** Checked codec for uwvm_spot_bridge/srv/Command_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvCommand_Request> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_29996d4dacb4ea847c5d629cb634d4aa5e2451627d2469d6cdf301cc007968f8";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvCommand_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvCommand_Request& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/Command_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvCommand_Response> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_e35556e7439c8d5ca79cb0f4ad23cf73c85a8a79d1382cde398dd85082dd1733";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvCommand_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvCommand_Response& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/Feedback_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvFeedback_Request> {
  static constexpr size_t max_wire_bytes=32;
  static constexpr char const* codec_identity="UWVMROS1_de64ce283f49f5c324912ec262f7d15b6fd828879c01039d455c765743c45ba3";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvFeedback_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvFeedback_Request& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/Feedback_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvFeedback_Response> {
  static constexpr size_t max_wire_bytes=16429;
  static constexpr char const* codec_identity="UWVMROS1_dc801501c5f5c993eea419f4a94f5c7eb57a5cfe113d0351a3b1e3fbfd514901";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvFeedback_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvFeedback_Response& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/Lease_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvLease_Request> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_a9aacd78dc6ae9d18fb6bb0d185797c6463c882edadf251eb5c21e3aa3c538a1";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvLease_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvLease_Request& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/Lease_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvLease_Response> {
  static constexpr size_t max_wire_bytes=22;
  static constexpr char const* codec_identity="UWVMROS1_ce875eb701b607c01f8a19dc8fcff607829824ad99276f906c7fc8fc5567f6d3";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvLease_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvLease_Response& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/State_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvState_Request> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_9becf278bc168ed6d5b15fe7f1ec35b83e274de8a5173cb2335948306bc7a47e";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvState_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvState_Request& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/State_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvState_Response> {
  static constexpr size_t max_wire_bytes=21433;
  static constexpr char const* codec_identity="UWVMROS1_8ffde1e9e2470975f03e038dbd51cf288c506671b4f3fe64b59223cb9711fb6c";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvState_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvState_Response& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/Velocity_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvVelocity_Request> {
  static constexpr size_t max_wire_bytes=56;
  static constexpr char const* codec_identity="UWVMROS1_ffe3b1dec91a25cd84d221ba4ebeae576bd7d6b96f258579c0b3d280cad79f8d";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvVelocity_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvVelocity_Request& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_spot_bridge/srv/Velocity_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_spot_bridgeSrvVelocity_Response> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_031f37beeb5a03c4e65d4be8fd70bc680408d110a020e066bea593d86498401a";
  static bool encode(uwvm_custom::Ros2Uwvm_spot_bridgeSrvVelocity_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_spot_bridgeSrvVelocity_Response& out) {return !roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
