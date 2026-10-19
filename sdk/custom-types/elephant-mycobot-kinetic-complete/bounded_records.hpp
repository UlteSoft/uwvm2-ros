/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for mycobot_communication/MycobotAngles. */
using Ros1Mycobot_communicationMycobotAngles=roswasmc_bounded_1_mycobot_communication_MycobotAngles;
/** Complete bounded guest value for mycobot_communication/MycobotCoords. */
using Ros1Mycobot_communicationMycobotCoords=roswasmc_bounded_1_mycobot_communication_MycobotCoords;
/** Complete bounded guest value for mycobot_communication/MycobotGripperStatus. */
using Ros1Mycobot_communicationMycobotGripperStatus=roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus;
/** Complete bounded guest value for mycobot_communication/MycobotPumpStatus. */
using Ros1Mycobot_communicationMycobotPumpStatus=roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus;
/** Complete bounded guest value for mycobot_communication/MycobotSetAngles. */
using Ros1Mycobot_communicationMycobotSetAngles=roswasmc_bounded_1_mycobot_communication_MycobotSetAngles;
/** Complete bounded guest value for mycobot_communication/MycobotSetCoords. */
using Ros1Mycobot_communicationMycobotSetCoords=roswasmc_bounded_1_mycobot_communication_MycobotSetCoords;
/** Complete bounded guest value for mycobot_communication/GetAnglesRequest. */
using Ros1Mycobot_communicationGetAnglesRequest=roswasmc_bounded_1_mycobot_communication_GetAnglesRequest;
/** Complete bounded guest value for mycobot_communication/GetAnglesResponse. */
using Ros1Mycobot_communicationGetAnglesResponse=roswasmc_bounded_1_mycobot_communication_GetAnglesResponse;
/** Complete bounded guest value for mycobot_communication/GetCoordsRequest. */
using Ros1Mycobot_communicationGetCoordsRequest=roswasmc_bounded_1_mycobot_communication_GetCoordsRequest;
/** Complete bounded guest value for mycobot_communication/GetCoordsResponse. */
using Ros1Mycobot_communicationGetCoordsResponse=roswasmc_bounded_1_mycobot_communication_GetCoordsResponse;
/** Complete bounded guest value for mycobot_communication/GripperStatusRequest. */
using Ros1Mycobot_communicationGripperStatusRequest=roswasmc_bounded_1_mycobot_communication_GripperStatusRequest;
/** Complete bounded guest value for mycobot_communication/GripperStatusResponse. */
using Ros1Mycobot_communicationGripperStatusResponse=roswasmc_bounded_1_mycobot_communication_GripperStatusResponse;
/** Complete bounded guest value for mycobot_communication/PumpStatusRequest. */
using Ros1Mycobot_communicationPumpStatusRequest=roswasmc_bounded_1_mycobot_communication_PumpStatusRequest;
/** Complete bounded guest value for mycobot_communication/PumpStatusResponse. */
using Ros1Mycobot_communicationPumpStatusResponse=roswasmc_bounded_1_mycobot_communication_PumpStatusResponse;
/** Complete bounded guest value for mycobot_communication/SetAnglesRequest. */
using Ros1Mycobot_communicationSetAnglesRequest=roswasmc_bounded_1_mycobot_communication_SetAnglesRequest;
/** Complete bounded guest value for mycobot_communication/SetAnglesResponse. */
using Ros1Mycobot_communicationSetAnglesResponse=roswasmc_bounded_1_mycobot_communication_SetAnglesResponse;
/** Complete bounded guest value for mycobot_communication/SetCoordsRequest. */
using Ros1Mycobot_communicationSetCoordsRequest=roswasmc_bounded_1_mycobot_communication_SetCoordsRequest;
/** Complete bounded guest value for mycobot_communication/SetCoordsResponse. */
using Ros1Mycobot_communicationSetCoordsResponse=roswasmc_bounded_1_mycobot_communication_SetCoordsResponse;
/** Complete bounded guest value for mypalletizer_communication/MypalAngles. */
using Ros1Mypalletizer_communicationMypalAngles=roswasmc_bounded_1_mypalletizer_communication_MypalAngles;
/** Complete bounded guest value for mypalletizer_communication/MypalCoords. */
using Ros1Mypalletizer_communicationMypalCoords=roswasmc_bounded_1_mypalletizer_communication_MypalCoords;
/** Complete bounded guest value for mypalletizer_communication/MypalGripperStatus. */
using Ros1Mypalletizer_communicationMypalGripperStatus=roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus;
/** Complete bounded guest value for mypalletizer_communication/MypalPumpStatus. */
using Ros1Mypalletizer_communicationMypalPumpStatus=roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus;
/** Complete bounded guest value for mypalletizer_communication/MypalSetAngles. */
using Ros1Mypalletizer_communicationMypalSetAngles=roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles;
/** Complete bounded guest value for mypalletizer_communication/MypalSetCoords. */
using Ros1Mypalletizer_communicationMypalSetCoords=roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords;
/** Complete bounded guest value for mypalletizer_communication/GetAnglesRequest. */
using Ros1Mypalletizer_communicationGetAnglesRequest=roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest;
/** Complete bounded guest value for mypalletizer_communication/GetAnglesResponse. */
using Ros1Mypalletizer_communicationGetAnglesResponse=roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse;
/** Complete bounded guest value for mypalletizer_communication/GetCoordsRequest. */
using Ros1Mypalletizer_communicationGetCoordsRequest=roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest;
/** Complete bounded guest value for mypalletizer_communication/GetCoordsResponse. */
using Ros1Mypalletizer_communicationGetCoordsResponse=roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse;
/** Complete bounded guest value for mypalletizer_communication/GripperStatusRequest. */
using Ros1Mypalletizer_communicationGripperStatusRequest=roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest;
/** Complete bounded guest value for mypalletizer_communication/GripperStatusResponse. */
using Ros1Mypalletizer_communicationGripperStatusResponse=roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse;
/** Complete bounded guest value for mypalletizer_communication/PumpStatusRequest. */
using Ros1Mypalletizer_communicationPumpStatusRequest=roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest;
/** Complete bounded guest value for mypalletizer_communication/PumpStatusResponse. */
using Ros1Mypalletizer_communicationPumpStatusResponse=roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse;
/** Complete bounded guest value for mypalletizer_communication/SetAnglesRequest. */
using Ros1Mypalletizer_communicationSetAnglesRequest=roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest;
/** Complete bounded guest value for mypalletizer_communication/SetAnglesResponse. */
using Ros1Mypalletizer_communicationSetAnglesResponse=roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse;
/** Complete bounded guest value for mypalletizer_communication/SetCoordsRequest. */
using Ros1Mypalletizer_communicationSetCoordsRequest=roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest;
/** Complete bounded guest value for mypalletizer_communication/SetCoordsResponse. */
using Ros1Mypalletizer_communicationSetCoordsResponse=roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse;
}
namespace roswasmcpp {
/** Checked codec for mycobot_communication/MycobotAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotAngles> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_4cbda93a13d78e9c592327caa737dfecf33adf6db898015afd56ecf7732f9aa6";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotAngles& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_1da0d6342a1f4079244a65ad413d4559047d33b049ce9da04f8ab0bd226410a2";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotCoords& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_65edcc278c4e21355c62408ff310598b3dbdf03ed426e3da0f2ee0bc6910e649";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotGripperStatus& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_0c0cb3c0fe97b9f9c65769a09f505ff6c27a60723598d656cd5f16255f36bfbd";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotPumpStatus& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotSetAngles> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_0844a6cf5033b27ad5062741c939a7cfff63edeba34f7e56c8178e1fab85b334";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotSetAngles& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/MycobotSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationMycobotSetCoords> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_39376ad94c0c9b213703a373b6a3b12fd0dad0ec20ff312143d30099842747a9";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationMycobotSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationMycobotSetCoords& out) {return !roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_8915949f81b788e5797f3bc4935e33e9e9eae82c5db3a1211129e7ef03cd2c54";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_482e6b845d6913ab7ea12580a395caf0a9815b56f985eb3a0553263fb7fd4ff3";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_54acc83a24f2f62d8ba9c14b1cea1703cb6097360720550cc438671282d45876";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_275b9bec563bb8578112b3680400523000a08a8e194044164e5334336b73d107";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_d2c94d1906d311981c72642d82bbea634cfed4eac8c0aef41219bf4f382d4585";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_3a75c0d75882836971ed4c51e50c7b88778fcfeadf9b30df211d5055eb767664";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_561f4bb1f74832111672bbfed1ef1266a91914fb1034437199f730f55324c65c";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b5a1672fe12d2e2131034bb6da8751130b64ef6d48ea48ac0a046c506da452dd";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_10bcf04e47b743a985ffefc61dff4522f3718d2ac7170960c8ae6857aaa11cbd";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_d407051eb285975d89ad71f39aede021fb564aecca0e0744126c9a599fe396b2";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_7a59a4e31eca60caea9f82142225c890a136c60a9e4c821e9463e89139945f7a";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mycobot_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1a936a95cb1bfd0a9158342618c40c587af58388abf3b679eb4bad2518aec6af";
  static bool encode(uwvm_custom::Ros1Mycobot_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mycobot_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalAngles> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_1d45a33e05e948c1b58988c84afe359138c896119f82da0d737377f0272892d9";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalAngles& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalCoords> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_c542d1a1a71224901044bc38ca2934021d811417c7c6937ca194995f67e59b80";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalCoords& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_e37f608d4499ab2ed49618fc244831d274fadb23bd408d42619e3f8409ad2ee6";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalGripperStatus& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_cdf79e610b5d49b7f73857e02c64688a71517f2fe8e3a96739b16a53ad4d0ebc";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalPumpStatus& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalSetAngles> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_8d11ba2c705bd577a329a01d039b831162742efe790c93b6c2c2631699d04f98";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalSetAngles& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/MypalSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationMypalSetCoords> {
  static constexpr size_t max_wire_bytes=18;
  static constexpr char const* codec_identity="UWVMROS1_51a2d485c8d2252f184eb27724e88c1ae06002d54454bd8ebf31962a79f7c448";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationMypalSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationMypalSetCoords& out) {return !roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGetAnglesRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_d0412d8ccc8f724f2a0660d20e24a39f6445f0cb433e775058b6b092fde65de8";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGetAnglesRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGetAnglesResponse> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_cb5a18aec2420c030d8193eb40d25691a2491b98606a7722156984e82f77f745";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGetAnglesResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGetCoordsRequest> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_47d45419f14ce576d9e8c2bb288c7b67bb161115e7b99a2c4752d09cbb8087b8";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGetCoordsRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGetCoordsResponse> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_7fdad367484b5f89c812c2513e99373844dcd160a115e326fb1c597f221b53fb";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGetCoordsResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GripperStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGripperStatusRequest> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_48d661f8fd6adfcf1703123464fcfb9351f8f831c0779ffad103a63bcf7ea709";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGripperStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGripperStatusRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/GripperStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationGripperStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_00ab8ceb190b588a9420c2e044da64857cfdb76e709e40e57835ab5732074e2b";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationGripperStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationGripperStatusResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/PumpStatusRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationPumpStatusRequest> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_6dd70636864c4450a066c1d75e67643331fac4c3e1625bce1b94c85ce7a35636";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationPumpStatusRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationPumpStatusRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/PumpStatusResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationPumpStatusResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_c2503f89c84c49db8dab2ac9f847ba9d09434d366fd8916a7d5f30ea161bb4c5";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationPumpStatusResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationPumpStatusResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/SetAnglesRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationSetAnglesRequest> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_32072510c6deddb3f372b5f300068ea457916a4e0397b30951e421a1da641d5d";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationSetAnglesRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationSetAnglesRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/SetAnglesResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationSetAnglesResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1472b47adc3171a9f1938e52bc937af6dd6bb720d33e163b70ae066a89a3211c";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationSetAnglesResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationSetAnglesResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/SetCoordsRequest; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationSetCoordsRequest> {
  static constexpr size_t max_wire_bytes=18;
  static constexpr char const* codec_identity="UWVMROS1_2d608cac1a7129ca5cd68ff52170ed40258cb6d199f4842a268a1d7570f9bda1";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationSetCoordsRequest const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationSetCoordsRequest& out) {return !roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_communication/SetCoordsResponse; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros1Mypalletizer_communicationSetCoordsResponse> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_86900f8772e9609e1d2b882b6ab613ce9609be6c589e5b9574a57b8749231308";
  static bool encode(uwvm_custom::Ros1Mypalletizer_communicationSetCoordsResponse const& value,wire::bytes& out) {
    if(!roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_valid(&value))return false;size_t n=roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Mypalletizer_communicationSetCoordsResponse& out) {return !roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_decode(bytes.data(),bytes.size(),&out);}
};
}
