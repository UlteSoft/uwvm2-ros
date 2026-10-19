/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for unitree_api/msg/RequestIdentity. */
using Ros2Unitree_apiMsgRequestIdentity=roswasmc_bounded_2_unitree_api_msg_RequestIdentity;
/** Complete bounded guest value for unitree_api/msg/RequestLease. */
using Ros2Unitree_apiMsgRequestLease=roswasmc_bounded_2_unitree_api_msg_RequestLease;
/** Complete bounded guest value for unitree_api/msg/RequestPolicy. */
using Ros2Unitree_apiMsgRequestPolicy=roswasmc_bounded_2_unitree_api_msg_RequestPolicy;
/** Complete bounded guest value for unitree_api/msg/RequestHeader. */
using Ros2Unitree_apiMsgRequestHeader=roswasmc_bounded_2_unitree_api_msg_RequestHeader;
/** Complete bounded guest value for unitree_api/msg/Request. */
using Ros2Unitree_apiMsgRequest=roswasmc_bounded_2_unitree_api_msg_Request;
/** Complete bounded guest value for unitree_api/msg/ResponseStatus. */
using Ros2Unitree_apiMsgResponseStatus=roswasmc_bounded_2_unitree_api_msg_ResponseStatus;
/** Complete bounded guest value for unitree_api/msg/ResponseHeader. */
using Ros2Unitree_apiMsgResponseHeader=roswasmc_bounded_2_unitree_api_msg_ResponseHeader;
/** Complete bounded guest value for unitree_api/msg/Response. */
using Ros2Unitree_apiMsgResponse=roswasmc_bounded_2_unitree_api_msg_Response;
}
namespace roswasmcpp {
/** Checked codec for unitree_api/msg/RequestIdentity; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_apiMsgRequestIdentity> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_e3daab89abcdaf6526511a189cfcc0c78aaebe8aefc53814ba77f6eb80cdc4e2";
  static bool encode(uwvm_custom::Ros2Unitree_apiMsgRequestIdentity const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_api_msg_RequestIdentity_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_api_msg_RequestIdentity_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_api_msg_RequestIdentity_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_apiMsgRequestIdentity& out) {return !roswasmc_bounded_2_unitree_api_msg_RequestIdentity_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_api/msg/RequestLease; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_apiMsgRequestLease> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_3dcb1fe2998c880e2d9196967da172ccd9060ebc7f6d2feb8682dc3d104c2e6e";
  static bool encode(uwvm_custom::Ros2Unitree_apiMsgRequestLease const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_api_msg_RequestLease_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_api_msg_RequestLease_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_api_msg_RequestLease_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_apiMsgRequestLease& out) {return !roswasmc_bounded_2_unitree_api_msg_RequestLease_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_api/msg/RequestPolicy; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_apiMsgRequestPolicy> {
  static constexpr size_t max_wire_bytes=5;
  static constexpr char const* codec_identity="UWVMROS1_61f1a1eafec2f694d24cf798963dcb5a0c8f40e9791db5a502f0736cc8587bb8";
  static bool encode(uwvm_custom::Ros2Unitree_apiMsgRequestPolicy const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_api_msg_RequestPolicy_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_api_msg_RequestPolicy_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_api_msg_RequestPolicy_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_apiMsgRequestPolicy& out) {return !roswasmc_bounded_2_unitree_api_msg_RequestPolicy_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_api/msg/RequestHeader; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_apiMsgRequestHeader> {
  static constexpr size_t max_wire_bytes=29;
  static constexpr char const* codec_identity="UWVMROS1_c8e8b83e2cdc978c5279483dbfac8d4185a3e05c87799b424643f0fcb02d0fd0";
  static bool encode(uwvm_custom::Ros2Unitree_apiMsgRequestHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_api_msg_RequestHeader_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_api_msg_RequestHeader_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_api_msg_RequestHeader_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_apiMsgRequestHeader& out) {return !roswasmc_bounded_2_unitree_api_msg_RequestHeader_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_api/msg/Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_apiMsgRequest> {
  static constexpr size_t max_wire_bytes=6181;
  static constexpr char const* codec_identity="UWVMROS1_39bb2426d68c160c07e7cff951d234c8ec9d0c70fddb5d432d9b238d08565eb2";
  static bool encode(uwvm_custom::Ros2Unitree_apiMsgRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_api_msg_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_api_msg_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_api_msg_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_apiMsgRequest& out) {return !roswasmc_bounded_2_unitree_api_msg_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_api/msg/ResponseStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_apiMsgResponseStatus> {
  static constexpr size_t max_wire_bytes=4;
  static constexpr char const* codec_identity="UWVMROS1_aaad17e09d5d6549455f69060d1bb03ec81cddfc0ede323a5f2365a6c1a7b7b1";
  static bool encode(uwvm_custom::Ros2Unitree_apiMsgResponseStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_api_msg_ResponseStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_api_msg_ResponseStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_api_msg_ResponseStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_apiMsgResponseStatus& out) {return !roswasmc_bounded_2_unitree_api_msg_ResponseStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_api/msg/ResponseHeader; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_apiMsgResponseHeader> {
  static constexpr size_t max_wire_bytes=20;
  static constexpr char const* codec_identity="UWVMROS1_20ddef706f0be7d7e2e356ab2222b6e198bd36fd6d8a81706553d5a883de85e0";
  static bool encode(uwvm_custom::Ros2Unitree_apiMsgResponseHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_api_msg_ResponseHeader_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_api_msg_ResponseHeader_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_api_msg_ResponseHeader_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_apiMsgResponseHeader& out) {return !roswasmc_bounded_2_unitree_api_msg_ResponseHeader_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for unitree_api/msg/Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Unitree_apiMsgResponse> {
  static constexpr size_t max_wire_bytes=6172;
  static constexpr char const* codec_identity="UWVMROS1_c048bd88b9c7d86cde04e6a589c50aeba396be29acf0d58bc6972f2dd8d8b749";
  static bool encode(uwvm_custom::Ros2Unitree_apiMsgResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_unitree_api_msg_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_unitree_api_msg_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_unitree_api_msg_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Unitree_apiMsgResponse& out) {return !roswasmc_bounded_2_unitree_api_msg_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
