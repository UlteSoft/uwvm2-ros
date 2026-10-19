/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for uwvm_groot_bridge/msg/RGBFrame. */
using Ros2Uwvm_groot_bridgeMsgRGBFrame=roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame;
/** Complete bounded guest value for uwvm_groot_bridge/msg/FloatTensor. */
using Ros2Uwvm_groot_bridgeMsgFloatTensor=roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor;
/** Complete bounded guest value for uwvm_groot_bridge/msg/TextSequence. */
using Ros2Uwvm_groot_bridgeMsgTextSequence=roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence;
/** Complete bounded guest value for uwvm_groot_bridge/srv/InferAction_Request. */
using Ros2Uwvm_groot_bridgeSrvInferAction_Request=roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request;
/** Complete bounded guest value for uwvm_groot_bridge/srv/InferAction_Response. */
using Ros2Uwvm_groot_bridgeSrvInferAction_Response=roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response;
}
namespace roswasmcpp {
/** Checked codec for uwvm_groot_bridge/msg/RGBFrame; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_groot_bridgeMsgRGBFrame> {
  static constexpr size_t max_wire_bytes=860;
  static constexpr char const* codec_identity="UWVMROS1_db7172fe7a7f0672c907f31ca0b1cb6286b804246d62e0414ac4313ae1096364";
  static bool encode(uwvm_custom::Ros2Uwvm_groot_bridgeMsgRGBFrame const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_groot_bridgeMsgRGBFrame& out) {return !roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_groot_bridge/msg/FloatTensor; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_groot_bridgeMsgFloatTensor> {
  static constexpr size_t max_wire_bytes=1364;
  static constexpr char const* codec_identity="UWVMROS1_a091d322ab737d2a9415f889fb78a18df7aec6ce74cfab0dcd5bb2a61ae3abae";
  static bool encode(uwvm_custom::Ros2Uwvm_groot_bridgeMsgFloatTensor const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_groot_bridgeMsgFloatTensor& out) {return !roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_groot_bridge/msg/TextSequence; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_groot_bridgeMsgTextSequence> {
  static constexpr size_t max_wire_bytes=1120;
  static constexpr char const* codec_identity="UWVMROS1_895be974f2560f0afff7614f5400ff7205e013cda269825a8443628621c8e80d";
  static bool encode(uwvm_custom::Ros2Uwvm_groot_bridgeMsgTextSequence const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_groot_bridgeMsgTextSequence& out) {return !roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_groot_bridge/srv/InferAction_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_groot_bridgeSrvInferAction_Request> {
  static constexpr size_t max_wire_bytes=10296;
  static constexpr char const* codec_identity="UWVMROS1_78f85929fb8646e9a92f17cafa3e8296fde42aaf49a56b271531ebad869a0fa6";
  static bool encode(uwvm_custom::Ros2Uwvm_groot_bridgeSrvInferAction_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_groot_bridgeSrvInferAction_Request& out) {return !roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_groot_bridge/srv/InferAction_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_groot_bridgeSrvInferAction_Response> {
  static constexpr size_t max_wire_bytes=10929;
  static constexpr char const* codec_identity="UWVMROS1_787281a2adea187ae43ff951a414dd7563b3e08318414e5199e1950b8143a3ce";
  static bool encode(uwvm_custom::Ros2Uwvm_groot_bridgeSrvInferAction_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_groot_bridgeSrvInferAction_Response& out) {return !roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
