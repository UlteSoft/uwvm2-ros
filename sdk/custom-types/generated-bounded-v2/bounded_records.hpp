/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for uwvm_ros_bounded_v2/Point. */
using Ros1Uwvm_ros_bounded_v2Point=roswasmc_bounded_1_uwvm_ros_bounded_v2_Point;
/** Complete bounded guest value for uwvm_ros_bounded_v2/Envelope. */
using Ros1Uwvm_ros_bounded_v2Envelope=roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope;
/** Complete bounded guest value for uwvm_ros_bounded_v2/EchoRequest. */
using Ros1Uwvm_ros_bounded_v2EchoRequest=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest;
/** Complete bounded guest value for uwvm_ros_bounded_v2/EchoResponse. */
using Ros1Uwvm_ros_bounded_v2EchoResponse=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse;
/** Complete bounded guest value for uwvm_ros_bounded_v2/EmptyRequest. */
using Ros1Uwvm_ros_bounded_v2EmptyRequest=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest;
/** Complete bounded guest value for uwvm_ros_bounded_v2/EmptyResponse. */
using Ros1Uwvm_ros_bounded_v2EmptyResponse=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse;
/** Complete bounded guest value for uwvm_ros_bounded_v2/DigestGoal. */
using Ros1Uwvm_ros_bounded_v2DigestGoal=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal;
/** Complete bounded guest value for uwvm_ros_bounded_v2/DigestResult. */
using Ros1Uwvm_ros_bounded_v2DigestResult=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult;
/** Complete bounded guest value for uwvm_ros_bounded_v2/DigestFeedback. */
using Ros1Uwvm_ros_bounded_v2DigestFeedback=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback;
/** Complete bounded guest value for uwvm_ros_bounded_v2/msg/Point. */
using Ros2Uwvm_ros_bounded_v2MsgPoint=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point;
/** Complete bounded guest value for uwvm_ros_bounded_v2/msg/Envelope. */
using Ros2Uwvm_ros_bounded_v2MsgEnvelope=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope;
/** Complete bounded guest value for uwvm_ros_bounded_v2/srv/Echo_Request. */
using Ros2Uwvm_ros_bounded_v2SrvEcho_Request=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request;
/** Complete bounded guest value for uwvm_ros_bounded_v2/srv/Echo_Response. */
using Ros2Uwvm_ros_bounded_v2SrvEcho_Response=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response;
/** Complete bounded guest value for uwvm_ros_bounded_v2/srv/Empty_Request. */
using Ros2Uwvm_ros_bounded_v2SrvEmpty_Request=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request;
/** Complete bounded guest value for uwvm_ros_bounded_v2/srv/Empty_Response. */
using Ros2Uwvm_ros_bounded_v2SrvEmpty_Response=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response;
/** Complete bounded guest value for uwvm_ros_bounded_v2/action/Digest_Goal. */
using Ros2Uwvm_ros_bounded_v2ActionDigest_Goal=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal;
/** Complete bounded guest value for uwvm_ros_bounded_v2/action/Digest_Result. */
using Ros2Uwvm_ros_bounded_v2ActionDigest_Result=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result;
/** Complete bounded guest value for uwvm_ros_bounded_v2/action/Digest_Feedback. */
using Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback;
}
namespace roswasmcpp {
/** Checked codec for uwvm_ros_bounded_v2/Point; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2Point> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2Point const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2Point& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/Envelope; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2Envelope> {
  static constexpr size_t max_wire_bytes=120;
  static constexpr char const* codec_identity="UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2Envelope const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2Envelope& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/EchoRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2EchoRequest> {
  static constexpr size_t max_wire_bytes=124;
  static constexpr char const* codec_identity="UWVMROS1_fc0c655e5a6a780160b2953a6fba67e28745d60b46c80f55be5ffb8b82da4420";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2EchoRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2EchoRequest& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/EchoResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2EchoResponse> {
  static constexpr size_t max_wire_bytes=121;
  static constexpr char const* codec_identity="UWVMROS1_b06920091695407488787551c23510de5a76cf0368bbed5ea82d833d36c8385b";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2EchoResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2EchoResponse& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/EmptyRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2EmptyRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_3f00a65933d0d7b1bc1848910b3d2a503b9f2b8a9f09ef58a83fc27e534dcf40";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2EmptyRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2EmptyRequest& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/EmptyResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2EmptyResponse> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_9ddb085437fd7750f10f08121d4cfd64d83bd293553a6ed9090044ec0625a817";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2EmptyResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2EmptyResponse& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/DigestGoal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestGoal> {
  static constexpr size_t max_wire_bytes=120;
  static constexpr char const* codec_identity="UWVMROS1_08876b8b3f345f7baa27b2e371458f77eddb4f636cb00f03b6725d91d523accc";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestGoal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestGoal& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/DigestResult; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestResult> {
  static constexpr size_t max_wire_bytes=41;
  static constexpr char const* codec_identity="UWVMROS1_4753d6deda29277eabe2e0b2b53ee10cbbf1a546eaf41ddf6a56174850339bcd";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestResult const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestResult& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/DigestFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestFeedback> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_1ff8a8acc544c733fe0719dc89ecbb3c99da08b156d1af7d29c5931006bee619";
  static bool encode(uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_valid(&value))return false;size_t n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestFeedback& out) {return !roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/msg/Point; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2MsgPoint> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2MsgPoint const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2MsgPoint& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/msg/Envelope; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2MsgEnvelope> {
  static constexpr size_t max_wire_bytes=120;
  static constexpr char const* codec_identity="UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2MsgEnvelope const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2MsgEnvelope& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/srv/Echo_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEcho_Request> {
  static constexpr size_t max_wire_bytes=124;
  static constexpr char const* codec_identity="UWVMROS1_dd30d845fcb865e94b268c77fc18bc7906d226a4ca3274224cc6e202e69b93f3";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEcho_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEcho_Request& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/srv/Echo_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEcho_Response> {
  static constexpr size_t max_wire_bytes=121;
  static constexpr char const* codec_identity="UWVMROS1_995cb48a672a99327ed71ffb5af6d7518e11aae2f4068e17801b38799a52e5e5";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEcho_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEcho_Response& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/srv/Empty_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEmpty_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_60e31c2bafbb5c26e57de70bd2f26cea80c8f1d2299ed064aa6d74f51b25a8f4";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEmpty_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEmpty_Request& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/srv/Empty_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEmpty_Response> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_159ef50f81d76904657508db066fc6ffdb7229581aaeb5e2d425cd9b1e31cf2b";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEmpty_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEmpty_Response& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/action/Digest_Goal; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Goal> {
  static constexpr size_t max_wire_bytes=120;
  static constexpr char const* codec_identity="UWVMROS1_b853f639f7516b2fd475fdd1e238f182f34423a38f7e3fa0430dc952f76bb58c";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Goal const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Goal& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/action/Digest_Result; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Result> {
  static constexpr size_t max_wire_bytes=41;
  static constexpr char const* codec_identity="UWVMROS1_9a80357b1963e18b15441487bc1ca7dc876faa47c4a97df377dbc4f98cacd3da";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Result const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Result& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for uwvm_ros_bounded_v2/action/Digest_Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_940051bd38fe0ab7f0dc08355ff3da10782ee3d9705adb60ac218ea8f82180c1";
  static bool encode(uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback& out) {return !roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
}
