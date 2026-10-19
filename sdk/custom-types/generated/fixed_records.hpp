/** @file Generated C++20 guest codecs. Include with the SDK include directory.
 * The C record aliases retain fixed arrays. Publisher/Subscription use Codec
 * specializations; codec selection creates no permission. No I/O occurs. */
#pragma once
#include "fixed_records.h"
#include <roswasm/roswasmcpp.hpp>
namespace uwvm_custom {
/** @brief ROS1 example_interfaces_custom/Packet fixed guest value. */
using Ros1Example_interfaces_customPacket = roswasmc_custom_1_example_interfaces_custom_Packet;
/** @brief ROS1 example_interfaces_custom/ScalarRecord fixed guest value. */
using Ros1Example_interfaces_customScalarRecord = roswasmc_custom_1_example_interfaces_custom_ScalarRecord;
/** @brief ROS1 example_interfaces_custom/NestedRecord fixed guest value. */
using Ros1Example_interfaces_customNestedRecord = roswasmc_custom_1_example_interfaces_custom_NestedRecord;
/** @brief ROS2 example_interfaces_custom/msg/FixedSample fixed guest value. */
using Ros2Example_interfaces_customFixedSample = roswasmc_custom_2_example_interfaces_custom_msg_FixedSample;
}
namespace roswasmcpp {
/** @brief Exact checked codec for example_interfaces_custom/Packet; native identity is verified by the host. */
template<> struct Codec<uwvm_custom::Ros1Example_interfaces_customPacket> {
    static constexpr size_t wire_bytes=23;
    static constexpr char const* codec_identity="UWVMROS1_e68d0934f545cd81105ac13682379dc84f023c860c070c046e69c7f70e94d407";
    static bool encode(uwvm_custom::Ros1Example_interfaces_customPacket const& value,wire::bytes& out) {
        unsigned char bytes[23];size_t n;if(roswasmc_custom_1_example_interfaces_custom_Packet_encode(&value,bytes,sizeof bytes,&n))return false;wire::bytes next(bytes,bytes+n);out=std::move(next);return true;}
    static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Example_interfaces_customPacket& out) {return !roswasmc_custom_1_example_interfaces_custom_Packet_decode(bytes.data(),bytes.size(),&out);}
};
/** @brief Exact checked codec for example_interfaces_custom/ScalarRecord; native identity is verified by the host. */
template<> struct Codec<uwvm_custom::Ros1Example_interfaces_customScalarRecord> {
    static constexpr size_t wire_bytes=43;
    static constexpr char const* codec_identity="UWVMROS1_dfab9988d24ce356a6a914d4ae6f6a7c8c44dc8118cfc768e41fa8bf47f386c6";
    static bool encode(uwvm_custom::Ros1Example_interfaces_customScalarRecord const& value,wire::bytes& out) {
        unsigned char bytes[43];size_t n;if(roswasmc_custom_1_example_interfaces_custom_ScalarRecord_encode(&value,bytes,sizeof bytes,&n))return false;wire::bytes next(bytes,bytes+n);out=std::move(next);return true;}
    static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Example_interfaces_customScalarRecord& out) {return !roswasmc_custom_1_example_interfaces_custom_ScalarRecord_decode(bytes.data(),bytes.size(),&out);}
};
/** @brief Exact checked codec for example_interfaces_custom/NestedRecord; native identity is verified by the host. */
template<> struct Codec<uwvm_custom::Ros1Example_interfaces_customNestedRecord> {
    static constexpr size_t wire_bytes=98;
    static constexpr char const* codec_identity="UWVMROS1_b0856889a47eb923055e44730a21c04d498472c373a1b9539a8f213a7c09ae5c";
    static bool encode(uwvm_custom::Ros1Example_interfaces_customNestedRecord const& value,wire::bytes& out) {
        unsigned char bytes[98];size_t n;if(roswasmc_custom_1_example_interfaces_custom_NestedRecord_encode(&value,bytes,sizeof bytes,&n))return false;wire::bytes next(bytes,bytes+n);out=std::move(next);return true;}
    static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros1Example_interfaces_customNestedRecord& out) {return !roswasmc_custom_1_example_interfaces_custom_NestedRecord_decode(bytes.data(),bytes.size(),&out);}
};
/** @brief Exact checked codec for example_interfaces_custom/msg/FixedSample; native identity is verified by the host. */
template<> struct Codec<uwvm_custom::Ros2Example_interfaces_customFixedSample> {
    static constexpr size_t wire_bytes=45;
    static constexpr char const* codec_identity="UWVMROS1_b794a6eb1863626cf279af1fbad1f071fce945d6585f3fdb22979c8f24e32663";
    static bool encode(uwvm_custom::Ros2Example_interfaces_customFixedSample const& value,wire::bytes& out) {
        unsigned char bytes[45];size_t n;if(roswasmc_custom_2_example_interfaces_custom_msg_FixedSample_encode(&value,bytes,sizeof bytes,&n))return false;wire::bytes next(bytes,bytes+n);out=std::move(next);return true;}
    static bool decode(std::span<unsigned char const> bytes,uwvm_custom::Ros2Example_interfaces_customFixedSample& out) {return !roswasmc_custom_2_example_interfaces_custom_msg_FixedSample_decode(bytes.data(),bytes.size(),&out);}
};
}
