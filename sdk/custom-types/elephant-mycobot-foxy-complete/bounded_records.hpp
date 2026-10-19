/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for mecharm_interfaces/msg/MecharmAngles. */
using Ros2Mecharm_interfacesMsgMecharmAngles=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles;
/** Complete bounded guest value for mecharm_interfaces/msg/MecharmCoords. */
using Ros2Mecharm_interfacesMsgMecharmCoords=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords;
/** Complete bounded guest value for mecharm_interfaces/msg/MecharmGripperStatus. */
using Ros2Mecharm_interfacesMsgMecharmGripperStatus=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus;
/** Complete bounded guest value for mecharm_interfaces/msg/MecharmPumpStatus. */
using Ros2Mecharm_interfacesMsgMecharmPumpStatus=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus;
/** Complete bounded guest value for mecharm_interfaces/msg/MecharmSetAngles. */
using Ros2Mecharm_interfacesMsgMecharmSetAngles=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles;
/** Complete bounded guest value for mecharm_interfaces/msg/MecharmSetCoords. */
using Ros2Mecharm_interfacesMsgMecharmSetCoords=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords;
/** Complete bounded guest value for mecharm_interfaces/srv/GetAngles_Request. */
using Ros2Mecharm_interfacesSrvGetAngles_Request=roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request;
/** Complete bounded guest value for mecharm_interfaces/srv/GetAngles_Response. */
using Ros2Mecharm_interfacesSrvGetAngles_Response=roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response;
/** Complete bounded guest value for mecharm_interfaces/srv/GetCoords_Request. */
using Ros2Mecharm_interfacesSrvGetCoords_Request=roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request;
/** Complete bounded guest value for mecharm_interfaces/srv/GetCoords_Response. */
using Ros2Mecharm_interfacesSrvGetCoords_Response=roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response;
/** Complete bounded guest value for mecharm_interfaces/srv/GripperStatus_Request. */
using Ros2Mecharm_interfacesSrvGripperStatus_Request=roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request;
/** Complete bounded guest value for mecharm_interfaces/srv/GripperStatus_Response. */
using Ros2Mecharm_interfacesSrvGripperStatus_Response=roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response;
/** Complete bounded guest value for mecharm_interfaces/srv/PumpStatus_Request. */
using Ros2Mecharm_interfacesSrvPumpStatus_Request=roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request;
/** Complete bounded guest value for mecharm_interfaces/srv/PumpStatus_Response. */
using Ros2Mecharm_interfacesSrvPumpStatus_Response=roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response;
/** Complete bounded guest value for mecharm_interfaces/srv/SetAngles_Request. */
using Ros2Mecharm_interfacesSrvSetAngles_Request=roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request;
/** Complete bounded guest value for mecharm_interfaces/srv/SetAngles_Response. */
using Ros2Mecharm_interfacesSrvSetAngles_Response=roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response;
/** Complete bounded guest value for mecharm_interfaces/srv/SetCoords_Request. */
using Ros2Mecharm_interfacesSrvSetCoords_Request=roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request;
/** Complete bounded guest value for mecharm_interfaces/srv/SetCoords_Response. */
using Ros2Mecharm_interfacesSrvSetCoords_Response=roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response;
/** Complete bounded guest value for mycobot_interfaces/msg/MycobotAngles. */
using Ros2Mycobot_interfacesMsgMycobotAngles=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles;
/** Complete bounded guest value for mycobot_interfaces/msg/MycobotCoords. */
using Ros2Mycobot_interfacesMsgMycobotCoords=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords;
/** Complete bounded guest value for mycobot_interfaces/msg/MycobotGripperStatus. */
using Ros2Mycobot_interfacesMsgMycobotGripperStatus=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus;
/** Complete bounded guest value for mycobot_interfaces/msg/MycobotPumpStatus. */
using Ros2Mycobot_interfacesMsgMycobotPumpStatus=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus;
/** Complete bounded guest value for mycobot_interfaces/msg/MycobotSetAngles. */
using Ros2Mycobot_interfacesMsgMycobotSetAngles=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles;
/** Complete bounded guest value for mycobot_interfaces/msg/MycobotSetCoords. */
using Ros2Mycobot_interfacesMsgMycobotSetCoords=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords;
/** Complete bounded guest value for mycobot_interfaces/srv/GetAngles_Request. */
using Ros2Mycobot_interfacesSrvGetAngles_Request=roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request;
/** Complete bounded guest value for mycobot_interfaces/srv/GetAngles_Response. */
using Ros2Mycobot_interfacesSrvGetAngles_Response=roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response;
/** Complete bounded guest value for mycobot_interfaces/srv/GetCoords_Request. */
using Ros2Mycobot_interfacesSrvGetCoords_Request=roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request;
/** Complete bounded guest value for mycobot_interfaces/srv/GetCoords_Response. */
using Ros2Mycobot_interfacesSrvGetCoords_Response=roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response;
/** Complete bounded guest value for mycobot_interfaces/srv/GripperStatus_Request. */
using Ros2Mycobot_interfacesSrvGripperStatus_Request=roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request;
/** Complete bounded guest value for mycobot_interfaces/srv/GripperStatus_Response. */
using Ros2Mycobot_interfacesSrvGripperStatus_Response=roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response;
/** Complete bounded guest value for mycobot_interfaces/srv/PumpStatus_Request. */
using Ros2Mycobot_interfacesSrvPumpStatus_Request=roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request;
/** Complete bounded guest value for mycobot_interfaces/srv/PumpStatus_Response. */
using Ros2Mycobot_interfacesSrvPumpStatus_Response=roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response;
/** Complete bounded guest value for mycobot_interfaces/srv/SetAngles_Request. */
using Ros2Mycobot_interfacesSrvSetAngles_Request=roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request;
/** Complete bounded guest value for mycobot_interfaces/srv/SetAngles_Response. */
using Ros2Mycobot_interfacesSrvSetAngles_Response=roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response;
/** Complete bounded guest value for mycobot_interfaces/srv/SetCoords_Request. */
using Ros2Mycobot_interfacesSrvSetCoords_Request=roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request;
/** Complete bounded guest value for mycobot_interfaces/srv/SetCoords_Response. */
using Ros2Mycobot_interfacesSrvSetCoords_Response=roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response;
/** Complete bounded guest value for mycobot_pro450_interfaces/msg/MycobotAngles. */
using Ros2Mycobot_pro450_interfacesMsgMycobotAngles=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles;
/** Complete bounded guest value for mycobot_pro450_interfaces/msg/MycobotCoords. */
using Ros2Mycobot_pro450_interfacesMsgMycobotCoords=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords;
/** Complete bounded guest value for mycobot_pro450_interfaces/msg/MycobotGripperStatus. */
using Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus;
/** Complete bounded guest value for mycobot_pro450_interfaces/msg/MycobotSetAngles. */
using Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles;
/** Complete bounded guest value for mycobot_pro450_interfaces/msg/MycobotSetCoords. */
using Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/GetAngles_Request. */
using Ros2Mycobot_pro450_interfacesSrvGetAngles_Request=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/GetAngles_Response. */
using Ros2Mycobot_pro450_interfacesSrvGetAngles_Response=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/GetCoords_Request. */
using Ros2Mycobot_pro450_interfacesSrvGetCoords_Request=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/GetCoords_Response. */
using Ros2Mycobot_pro450_interfacesSrvGetCoords_Response=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/GetGripperValue_Request. */
using Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/GetGripperValue_Response. */
using Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/GripperStatus_Request. */
using Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/GripperStatus_Response. */
using Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/SetAngles_Request. */
using Ros2Mycobot_pro450_interfacesSrvSetAngles_Request=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/SetAngles_Response. */
using Ros2Mycobot_pro450_interfacesSrvSetAngles_Response=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/SetCoords_Request. */
using Ros2Mycobot_pro450_interfacesSrvSetCoords_Request=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request;
/** Complete bounded guest value for mycobot_pro450_interfaces/srv/SetCoords_Response. */
using Ros2Mycobot_pro450_interfacesSrvSetCoords_Response=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response;
/** Complete bounded guest value for mypalletizer_interfaces/msg/MypalAngles. */
using Ros2Mypalletizer_interfacesMsgMypalAngles=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles;
/** Complete bounded guest value for mypalletizer_interfaces/msg/MypalCoords. */
using Ros2Mypalletizer_interfacesMsgMypalCoords=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords;
/** Complete bounded guest value for mypalletizer_interfaces/msg/MypalGripperStatus. */
using Ros2Mypalletizer_interfacesMsgMypalGripperStatus=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus;
/** Complete bounded guest value for mypalletizer_interfaces/msg/MypalPumpStatus. */
using Ros2Mypalletizer_interfacesMsgMypalPumpStatus=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus;
/** Complete bounded guest value for mypalletizer_interfaces/msg/MypalSetAngles. */
using Ros2Mypalletizer_interfacesMsgMypalSetAngles=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles;
/** Complete bounded guest value for mypalletizer_interfaces/msg/MypalSetCoords. */
using Ros2Mypalletizer_interfacesMsgMypalSetCoords=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords;
/** Complete bounded guest value for mypalletizer_interfaces/srv/GetAngles_Request. */
using Ros2Mypalletizer_interfacesSrvGetAngles_Request=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request;
/** Complete bounded guest value for mypalletizer_interfaces/srv/GetAngles_Response. */
using Ros2Mypalletizer_interfacesSrvGetAngles_Response=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response;
/** Complete bounded guest value for mypalletizer_interfaces/srv/GetCoords_Request. */
using Ros2Mypalletizer_interfacesSrvGetCoords_Request=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request;
/** Complete bounded guest value for mypalletizer_interfaces/srv/GetCoords_Response. */
using Ros2Mypalletizer_interfacesSrvGetCoords_Response=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response;
/** Complete bounded guest value for mypalletizer_interfaces/srv/GripperStatus_Request. */
using Ros2Mypalletizer_interfacesSrvGripperStatus_Request=roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request;
/** Complete bounded guest value for mypalletizer_interfaces/srv/GripperStatus_Response. */
using Ros2Mypalletizer_interfacesSrvGripperStatus_Response=roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response;
/** Complete bounded guest value for mypalletizer_interfaces/srv/PumpStatus_Request. */
using Ros2Mypalletizer_interfacesSrvPumpStatus_Request=roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request;
/** Complete bounded guest value for mypalletizer_interfaces/srv/PumpStatus_Response. */
using Ros2Mypalletizer_interfacesSrvPumpStatus_Response=roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response;
/** Complete bounded guest value for mypalletizer_interfaces/srv/SetAngles_Request. */
using Ros2Mypalletizer_interfacesSrvSetAngles_Request=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request;
/** Complete bounded guest value for mypalletizer_interfaces/srv/SetAngles_Response. */
using Ros2Mypalletizer_interfacesSrvSetAngles_Response=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response;
/** Complete bounded guest value for mypalletizer_interfaces/srv/SetCoords_Request. */
using Ros2Mypalletizer_interfacesSrvSetCoords_Request=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request;
/** Complete bounded guest value for mypalletizer_interfaces/srv/SetCoords_Response. */
using Ros2Mypalletizer_interfacesSrvSetCoords_Response=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response;
}
namespace roswasmcpp {
/** Checked codec for mecharm_interfaces/msg/MecharmAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesMsgMecharmAngles> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_37d12e0fc3b7bbb50bdcee0ca624a5c637f9554f3c64d640f538aeda57417331";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesMsgMecharmAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesMsgMecharmAngles& out) {return !roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/msg/MecharmCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesMsgMecharmCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_e1bb96b1908c796e2a89eb4dd2402a6cc02bcc5077c52268bb378a52dd3029ed";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesMsgMecharmCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesMsgMecharmCoords& out) {return !roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/msg/MecharmGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesMsgMecharmGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_2123b61d7b9ec218ee074473c26e5ec82fde818f40796047fc369cb84516e046";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesMsgMecharmGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesMsgMecharmGripperStatus& out) {return !roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/msg/MecharmPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesMsgMecharmPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_4d44c84c1fdd3f4db92d9975545348bfd727320e63c9fbfde5adf0b81ca04add";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesMsgMecharmPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesMsgMecharmPumpStatus& out) {return !roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/msg/MecharmSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesMsgMecharmSetAngles> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_54b634e4c5d0bc9fdf04a98d31816e24d3a9c5eb497bb38fb16e2de593cf81f5";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesMsgMecharmSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesMsgMecharmSetAngles& out) {return !roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/msg/MecharmSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesMsgMecharmSetCoords> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_44944ee22cd4254167083cfdcd7377ee2f59dbe27414932c980670a471a6e7c2";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesMsgMecharmSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesMsgMecharmSetCoords& out) {return !roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/GetAngles_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvGetAngles_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_95b1d7786c03c6fec98fec0706088481fe9c7aa48c9f33f4325031d0437d97df";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvGetAngles_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvGetAngles_Request& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/GetAngles_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvGetAngles_Response> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_b1a035847ac7a888053ba8de7494abb6d2cdc01f61e4d0a51cb3ef6f479adf61";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvGetAngles_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvGetAngles_Response& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/GetCoords_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvGetCoords_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_353827e7125cfa6c829f1138cc33901099a26cf73a8d045cf806b427994ccef1";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvGetCoords_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvGetCoords_Request& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/GetCoords_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvGetCoords_Response> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_f7fcb8c4fdb47c06e93a8f67613490f7ab0ef2af7efb02092874722670b36c75";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvGetCoords_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvGetCoords_Response& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/GripperStatus_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvGripperStatus_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_044bf47303f5da0d0e17e795b8567b714d55680e8144c8e2e71ff7032859dfbb";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvGripperStatus_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvGripperStatus_Request& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/GripperStatus_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvGripperStatus_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_5aeee3c79bb6ebe4a4cb77f54552e97dcc155c65151a5965761a360d41acbea3";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvGripperStatus_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvGripperStatus_Response& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/PumpStatus_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvPumpStatus_Request> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_f80774682b7b1d447ff2f8dd7f6ac2f917dd4f71ccd3b1685a57b579fa41d6b9";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvPumpStatus_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvPumpStatus_Request& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/PumpStatus_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvPumpStatus_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_935c0e2aaca3f1e8aad8e4487484292b3d30ccdff180a9aaa50f454f75ecff1b";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvPumpStatus_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvPumpStatus_Response& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/SetAngles_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvSetAngles_Request> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_86897875ef9a609098bc1eabba63aec7d960de380db98370e7a67626b2253eeb";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvSetAngles_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvSetAngles_Request& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/SetAngles_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvSetAngles_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_4782fe6a137dee35bf39d863be0aea118f61dcef092c11b013c44c6f4c129620";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvSetAngles_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvSetAngles_Response& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/SetCoords_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvSetCoords_Request> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_a0daefe14076b73f1a64924ffc15a6b9647abe1b1887377e642f41b1a7920916";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvSetCoords_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvSetCoords_Request& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mecharm_interfaces/srv/SetCoords_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mecharm_interfacesSrvSetCoords_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_43d02cfe61b6fb08f035e1f8ea78496cdcb488ed4c916f528415dce4ab810b7f";
  static bool encode(uwvm_custom::Ros2Mecharm_interfacesSrvSetCoords_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mecharm_interfacesSrvSetCoords_Response& out) {return !roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/msg/MycobotAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesMsgMycobotAngles> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_38f099339ed5ee9fe56a5b44c3f0849e2ff7ff721b2a95b5cf49e3b92f6a9d82";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesMsgMycobotAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesMsgMycobotAngles& out) {return !roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/msg/MycobotCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesMsgMycobotCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_8d9606c561db7680e816f5a19fd148c374f271acee8b261f7d060f370e0399d7";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesMsgMycobotCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesMsgMycobotCoords& out) {return !roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/msg/MycobotGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesMsgMycobotGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_4dc3ab413af3da01e26a3289ef9e5ef39488d00483631d701b4d617d9c9ba33d";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesMsgMycobotGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesMsgMycobotGripperStatus& out) {return !roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/msg/MycobotPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesMsgMycobotPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_860004dc3975ab44b5a1b0a77a9ff01ab58bfc71d7142d842d3bf9c636365e58";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesMsgMycobotPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesMsgMycobotPumpStatus& out) {return !roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/msg/MycobotSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesMsgMycobotSetAngles> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_798576c5f0b03e46e5cc5d2edf81bc7042042482ef574643ca136621346d731e";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesMsgMycobotSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesMsgMycobotSetAngles& out) {return !roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/msg/MycobotSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesMsgMycobotSetCoords> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_462b7629172037a6959d08984896980c8c07ae5b6364c6d848b172dea23cab49";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesMsgMycobotSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesMsgMycobotSetCoords& out) {return !roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/GetAngles_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvGetAngles_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_ea433bdb6e3cb77923720a56bec24aaec62c88480248a4ed425370092474d38e";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvGetAngles_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvGetAngles_Request& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/GetAngles_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvGetAngles_Response> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_14d6e2d8a6f7234719fae06d8807c651965387e252160df555ede0a070123b8a";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvGetAngles_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvGetAngles_Response& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/GetCoords_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvGetCoords_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_31ef03eefc143e9d040572e378a5803f44f68bac6d03fbb44ca27f87fb5fb076";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvGetCoords_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvGetCoords_Request& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/GetCoords_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvGetCoords_Response> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_c9886b5cc5f3c48116a1c884f75ab4af49be46e2c0476ffc3aa8167ff96f7787";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvGetCoords_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvGetCoords_Response& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/GripperStatus_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvGripperStatus_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_6c5ae66e66f12834b7d623e07a5b3ef2b8127cd9cd83cb36d6793a68d921fcd5";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvGripperStatus_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvGripperStatus_Request& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/GripperStatus_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvGripperStatus_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_530f1e1acaa8cda6b43f8a8c0a38f5376eaebce3e99f7fb194a28143bed2d1ab";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvGripperStatus_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvGripperStatus_Response& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/PumpStatus_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvPumpStatus_Request> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_50b7fa285b93dd9e6b3b0eafda0afeb5911ab6e3b79d58f03f1cf1f76e732cd0";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvPumpStatus_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvPumpStatus_Request& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/PumpStatus_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvPumpStatus_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b347313e1d4430f4e78a72fb8c35d53a39d1d391290373f84f1f8b2b44375eb0";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvPumpStatus_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvPumpStatus_Response& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/SetAngles_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvSetAngles_Request> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_a06efd8b6af878d55c26fda8a7177a0b6c8b687f7e1b9f2e17e2ecd19c440310";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvSetAngles_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvSetAngles_Request& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/SetAngles_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvSetAngles_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_af4daae72e9ec28c8848b76b556c9387e65ba9698d8f0f6e5ba17522472ab665";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvSetAngles_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvSetAngles_Response& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/SetCoords_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvSetCoords_Request> {
  static constexpr size_t max_wire_bytes=26;
  static constexpr char const* codec_identity="UWVMROS1_76a98d5db6f98255046f13855dc7468049894805d83e3d3a7fbdaace40292213";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvSetCoords_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvSetCoords_Request& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_interfaces/srv/SetCoords_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_interfacesSrvSetCoords_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_75e3ae0e6db60a275b08e450c420969aa1ee05d0f34864e8fb6535d2d13613b9";
  static bool encode(uwvm_custom::Ros2Mycobot_interfacesSrvSetCoords_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_interfacesSrvSetCoords_Response& out) {return !roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/msg/MycobotAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotAngles> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_2351398be27155ac63b8a3d15ffd0d3088dd7cf284182a9b494791b7780aa6da";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotAngles& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/msg/MycobotCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotCoords> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_b8513545b0975757c6b7f756a5cedfe8aa0c5d28d0e573adfde001b452e923de";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotCoords& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/msg/MycobotGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_1d097932f2e6b9b4e73ff838b5223b585b230e4edc5c229b310eb00404f6e784";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/msg/MycobotSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_23d208d7fa8f7b7488be7c7db487698db4b227ef65e99c55d996901d27c3e591";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/msg/MycobotSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_60f40a2f6a577e5785fd69e0cbe9128b301aeaec6b70fd678c71e7f9a5426cea";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/GetAngles_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetAngles_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_d2d8197675b9dadd50ae1a22b165a84e2e76b293b28cc70bec211225c60626c4";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetAngles_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetAngles_Request& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/GetAngles_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetAngles_Response> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_13056bbe3bdef4feb8488783e1c5cf725d1fb94006f36f2221f61081084227f8";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetAngles_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetAngles_Response& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/GetCoords_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetCoords_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_bda408ece90e671ce58ae428f79f01c1429c3b3aa3c1ef2efe9dc045faa9e3c2";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetCoords_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetCoords_Request& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/GetCoords_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetCoords_Response> {
  static constexpr size_t max_wire_bytes=24;
  static constexpr char const* codec_identity="UWVMROS1_01e8a3ef4c048100af32d92cd027769e32eacd82dba36dc3bde30c70cb44afdf";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetCoords_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetCoords_Response& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/GetGripperValue_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_17cb5e3826409b661c2ad4bad8fde2b4c826dde95d8600e1a17bcf4908eca125";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/GetGripperValue_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_6b210e18d0696735196fb2e166b7168165dba6f03b97759a437fc8dc08ab2676";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/GripperStatus_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_edc3352d37effe3923c55a024d6acac85a3cb49350974ea35934efa41555ccac";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/GripperStatus_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_65166c2943fd319555cc1a2dcdaf4475b1e7779960907fe5ab6d1b5dc5536fc2";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/SetAngles_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetAngles_Request> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_ec2659577304938ca4f78c1b4add50190766d8d1b551397668ef7ad03e2ccb09";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetAngles_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetAngles_Request& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/SetAngles_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetAngles_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_28dc59c06324913c1ccbe286ca05b9f47585653a8f086fc5d99a01f6e9ee08b4";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetAngles_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetAngles_Response& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/SetCoords_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetCoords_Request> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_e0c550200472f77aa582a46a5149c05c95648615a306f254599f820992890c81";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetCoords_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetCoords_Request& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mycobot_pro450_interfaces/srv/SetCoords_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetCoords_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_9a5ee934233519095e1b28568490f8d958c319e2544976fae0b7de53a54a3f51";
  static bool encode(uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetCoords_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mycobot_pro450_interfacesSrvSetCoords_Response& out) {return !roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/msg/MypalAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalAngles> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_0a2dc324515448310b0f026fb26c6c5c6da452e5e7067096e315abe3ab86a35d";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalAngles& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/msg/MypalCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalCoords> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_839491548a07528380403fa31c89889ce82f28b2f82e6fd9474835659714b4f7";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalCoords& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/msg/MypalGripperStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalGripperStatus> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_f6e4ad6193272d6401bbbfcc20c0f38b6a147a81d9de1ad956d7c154f29daeb0";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalGripperStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalGripperStatus& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/msg/MypalPumpStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalPumpStatus> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_008dfd13cad42029330909fb7dc68392ba7f6678ad1baa14a41aad1e759f523c";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalPumpStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalPumpStatus& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/msg/MypalSetAngles; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalSetAngles> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_b9fdd6ac956fdeb4934ff41982e616f7c0c45b11169b6d60ba74fbcc61458bb5";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalSetAngles const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalSetAngles& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/msg/MypalSetCoords; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalSetCoords> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_0c1b65f543cdd239a5a56a7839ac607c5310cd4145ef6a6da5639a9af8ba4c79";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalSetCoords const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesMsgMypalSetCoords& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/GetAngles_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvGetAngles_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_c8124a1063d47b24dae2e279f51d4e48ebf9936a5e305b81b53387a4be344f78";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvGetAngles_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvGetAngles_Request& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/GetAngles_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvGetAngles_Response> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_33812a320c14891a5bc76a86ccbb0a5507cfbcc3706e01bc304263edabc6f75d";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvGetAngles_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvGetAngles_Response& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/GetCoords_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvGetCoords_Request> {
  static constexpr size_t max_wire_bytes=0;
  static constexpr char const* codec_identity="UWVMROS1_b3db3a0e7d2aed097027798a897caf833f9cc1b8bb027f98c0db016968fa4440";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvGetCoords_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvGetCoords_Request& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/GetCoords_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvGetCoords_Response> {
  static constexpr size_t max_wire_bytes=16;
  static constexpr char const* codec_identity="UWVMROS1_9d237cfa03c4b3c17d99fe0a9c08d1cafa3e8d0e404191e2169eea8683c54c84";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvGetCoords_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvGetCoords_Response& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/GripperStatus_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvGripperStatus_Request> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_b6d4c5991599ec4ac56e5f65dacd5a1ab89f073e9f07dc2b03692f4289fdcc9d";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvGripperStatus_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvGripperStatus_Request& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/GripperStatus_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvGripperStatus_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_bd0c427dc564ff7201019c6d33c7d0c743a65d263c67eb420b5e653d48e4daf4";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvGripperStatus_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvGripperStatus_Response& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/PumpStatus_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvPumpStatus_Request> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_6b14af554cb5c65c9411b2c6a76d49cb7cbe06dedcd26d47804d00d12ccf2e2f";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvPumpStatus_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvPumpStatus_Request& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/PumpStatus_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvPumpStatus_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_187a703ce648bc82a276961d82ed8abd5365d8db7af3bb211e590620e1bb2b45";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvPumpStatus_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvPumpStatus_Response& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/SetAngles_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvSetAngles_Request> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_3997bb19c183b59fbca1675695ea5ee548a247a34f9682dae3fb68fdf1a155ec";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvSetAngles_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvSetAngles_Request& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/SetAngles_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvSetAngles_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_a7952f549b35aa140adac4f18bbe8538c0f6230aa884459091cd74720ab757c9";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvSetAngles_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvSetAngles_Response& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/SetCoords_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvSetCoords_Request> {
  static constexpr size_t max_wire_bytes=17;
  static constexpr char const* codec_identity="UWVMROS1_c72f4f1cb71f1ad1076bcc89499abbcfc5eab173fadd19c9a266baf1c32d1541";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvSetCoords_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvSetCoords_Request& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for mypalletizer_interfaces/srv/SetCoords_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Mypalletizer_interfacesSrvSetCoords_Response> {
  static constexpr size_t max_wire_bytes=1;
  static constexpr char const* codec_identity="UWVMROS1_0cedaf9087ba0ae3726b9dd0d304665a226fc6df2ee068f6ac2b1c12ce88cb5b";
  static bool encode(uwvm_custom::Ros2Mypalletizer_interfacesSrvSetCoords_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Mypalletizer_interfacesSrvSetCoords_Response& out) {return !roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
