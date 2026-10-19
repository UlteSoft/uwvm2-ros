/** @file C++20 typed bounded canonical records, independent of native ROS libraries.
 * Includes use the existing roswasmcpp Publisher/Subscription codecs and do not grant
 * aliases. Custom Service/Action component codecs also accept the raw typed RPC facade. */
#pragma once
#include "bounded_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** Complete bounded guest value for builtin_interfaces/msg/Duration. */
using Ros2Builtin_interfacesMsgDuration=roswasmc_bounded_2_builtin_interfaces_msg_Duration;
/** Complete bounded guest value for builtin_interfaces/msg/Time. */
using Ros2Builtin_interfacesMsgTime=roswasmc_bounded_2_builtin_interfaces_msg_Time;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Drive. */
using Ros2Clearpath_platform_msgsMsgDrive=roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive;
/** Complete bounded guest value for clearpath_platform_msgs/msg/DriveFeedback. */
using Ros2Clearpath_platform_msgsMsgDriveFeedback=roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Fans. */
using Ros2Clearpath_platform_msgsMsgFans=roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans;
/** Complete bounded guest value for std_msgs/msg/Header. */
using Ros2Std_msgsMsgHeader=roswasmc_bounded_2_std_msgs_msg_Header;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Feedback. */
using Ros2Clearpath_platform_msgsMsgFeedback=roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback;
/** Complete bounded guest value for clearpath_platform_msgs/msg/RGB. */
using Ros2Clearpath_platform_msgsMsgRGB=roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Lights. */
using Ros2Clearpath_platform_msgsMsgLights=roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Power. */
using Ros2Clearpath_platform_msgsMsgPower=roswasmc_bounded_2_clearpath_platform_msgs_msg_Power;
/** Complete bounded guest value for clearpath_platform_msgs/msg/Status. */
using Ros2Clearpath_platform_msgsMsgStatus=roswasmc_bounded_2_clearpath_platform_msgs_msg_Status;
/** Complete bounded guest value for clearpath_platform_msgs/msg/StopStatus. */
using Ros2Clearpath_platform_msgsMsgStopStatus=roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus;
/** Complete bounded guest value for clearpath_platform_msgs/srv/ConfigureMcu_Request. */
using Ros2Clearpath_platform_msgsSrvConfigureMcu_Request=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request;
/** Complete bounded guest value for clearpath_platform_msgs/srv/ConfigureMcu_Response. */
using Ros2Clearpath_platform_msgsSrvConfigureMcu_Response=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response;
}
namespace roswasmcpp {
/** Checked codec for builtin_interfaces/msg/Duration; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Builtin_interfacesMsgDuration> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4";
  static bool encode(uwvm_custom::Ros2Builtin_interfacesMsgDuration const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value))return false;size_t n=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_builtin_interfaces_msg_Duration_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Builtin_interfacesMsgDuration& out) {return !roswasmc_bounded_2_builtin_interfaces_msg_Duration_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for builtin_interfaces/msg/Time; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Builtin_interfacesMsgTime> {
  static constexpr size_t max_wire_bytes=8;
  static constexpr char const* codec_identity="UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7";
  static bool encode(uwvm_custom::Ros2Builtin_interfacesMsgTime const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(&value))return false;size_t n=roswasmc_bounded_2_builtin_interfaces_msg_Time_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_builtin_interfaces_msg_Time_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Builtin_interfacesMsgTime& out) {return !roswasmc_bounded_2_builtin_interfaces_msg_Time_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Drive; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgDrive> {
  static constexpr size_t max_wire_bytes=9;
  static constexpr char const* codec_identity="UWVMROS1_be8664128ff89a066550f1a2496c30396f39bafd1d96483c545048a328138785";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgDrive const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgDrive& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/DriveFeedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgDriveFeedback> {
  static constexpr size_t max_wire_bytes=25;
  static constexpr char const* codec_identity="UWVMROS1_008f51d4c8341723ffec72651f93c00db0f9feceb037ee7b6b460ffcafd3f287";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgDriveFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgDriveFeedback& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Fans; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgFans> {
  static constexpr size_t max_wire_bytes=6;
  static constexpr char const* codec_identity="UWVMROS1_addc15a9b6db4f7a0eba3545f02ece0800e48796f71e387b4d249af861f95317";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgFans const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgFans& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for std_msgs/msg/Header; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Std_msgsMsgHeader> {
  static constexpr size_t max_wire_bytes=140;
  static constexpr char const* codec_identity="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
  static bool encode(uwvm_custom::Ros2Std_msgsMsgHeader const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value))return false;size_t n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_std_msgs_msg_Header_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Std_msgsMsgHeader& out) {return !roswasmc_bounded_2_std_msgs_msg_Header_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Feedback; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgFeedback> {
  static constexpr size_t max_wire_bytes=192;
  static constexpr char const* codec_identity="UWVMROS1_15fdfd2e12df38bab09bf083beb61ce23d3c1082e794c34d51eec4c473e29442";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgFeedback const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgFeedback& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/RGB; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgRGB> {
  static constexpr size_t max_wire_bytes=3;
  static constexpr char const* codec_identity="UWVMROS1_42db8731293054ef44002afc54c16dac45544354932d5353cf749fb376cef1a1";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgRGB const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgRGB& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Lights; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgLights> {
  static constexpr size_t max_wire_bytes=10;
  static constexpr char const* codec_identity="UWVMROS1_14da254234c9c897e39bf4ee0db8faad0798d23fe7017195f35f88fb6e0b3332";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgLights const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgLights& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Power; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgPower> {
  static constexpr size_t max_wire_bytes=169;
  static constexpr char const* codec_identity="UWVMROS1_e97ba89dea149ff43d0134038985ea45d2f072af592ea128aa5502a053478d22";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgPower const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgPower& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/Status; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgStatus> {
  static constexpr size_t max_wire_bytes=684;
  static constexpr char const* codec_identity="UWVMROS1_331afb70728e70e0391da1b2a0f2b97fdc8742a363a2b007606121ad778dd5c1";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgStatus& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/msg/StopStatus; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsMsgStopStatus> {
  static constexpr size_t max_wire_bytes=143;
  static constexpr char const* codec_identity="UWVMROS1_101f96c8d48e7a945a2f94108e7b48393fc9d4bd75507dd709b95180ff4794d2";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsMsgStopStatus const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsMsgStopStatus& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/srv/ConfigureMcu_Request; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Request> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_9c48451b208c4de7a51c5075587dfbe710e7ec6b249c7e1eef519022154023fa";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Request const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Request& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_decode(bytes.data(),bytes.size(),&out);}
};
/** Checked codec for clearpath_platform_msgs/srv/ConfigureMcu_Response; canonical maximum is not an actual message length. */
template<> struct Codec<uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Response> {
  static constexpr size_t max_wire_bytes=261;
  static constexpr char const* codec_identity="UWVMROS1_552e715f290b5b9aa5dc926519cf2ab5120d6b2ae5eca9902efcc3f8c9e459ad";
  static bool encode(uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Response const& value,wire::bytes& out) {
    if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_valid(&value))return false;size_t n=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_wire_size(&value),used=0;wire::bytes next(n);
    if(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_encode(&value,next.data(),next.size(),&used)||used!=n)return false;out=std::move(next);return true;}
  static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Clearpath_platform_msgsSrvConfigureMcu_Response& out) {return !roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_decode(bytes.data(),bytes.size(),&out);}
};
}
