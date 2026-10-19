/** @file Generated C++20 guest codecs. Include with the SDK include directory.
 * The C record aliases retain fixed arrays. Publisher/Subscription use Codec
 * specializations; codec selection creates no permission. No I/O occurs. */
#pragma once
#include "fixed_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** @brief ROS1 uwvm_custom_ros1_fixture/Packet fixed guest value. */
using Ros1Uwvm_custom_ros1_fixturePacket = roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet;
/** @brief ROS2 uwvm_ros_custom_test_msgs/msg/FixedSample fixed guest value. */
using Ros2Uwvm_ros_custom_test_msgsFixedSample = roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample;
}
namespace roswasmcpp {
/** @brief Exact checked codec for uwvm_custom_ros1_fixture/Packet; native identity is verified by the host. */
template<> struct Codec<uwvm_custom::Ros1Uwvm_custom_ros1_fixturePacket> {
    static constexpr size_t wire_bytes=23;
    static constexpr char const* codec_identity="UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544";
    static bool encode(uwvm_custom::Ros1Uwvm_custom_ros1_fixturePacket const& value,wire::bytes& out) {
        unsigned char bytes[23];size_t n;if(roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_encode(&value,bytes,sizeof bytes,&n))return false;wire::bytes next(bytes,bytes+n);out=std::move(next);return true;}
    static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Uwvm_custom_ros1_fixturePacket& out) {return !roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_decode(bytes.data(),bytes.size(),&out);}
};
/** @brief Exact checked codec for uwvm_ros_custom_test_msgs/msg/FixedSample; native identity is verified by the host. */
template<> struct Codec<uwvm_custom::Ros2Uwvm_ros_custom_test_msgsFixedSample> {
    static constexpr size_t wire_bytes=45;
    static constexpr char const* codec_identity="UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516";
    static bool encode(uwvm_custom::Ros2Uwvm_ros_custom_test_msgsFixedSample const& value,wire::bytes& out) {
        unsigned char bytes[45];size_t n;if(roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_encode(&value,bytes,sizeof bytes,&n))return false;wire::bytes next(bytes,bytes+n);out=std::move(next);return true;}
    static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Uwvm_ros_custom_test_msgsFixedSample& out) {return !roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_decode(bytes.data(),bytes.size(),&out);}
};
}
