/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
// Compile and execute this contract only in the provided Linux cgroup. It
// exercises the shared fixed-record scanner without creating any ROS entity.
// Synthetic native identities below model metadata admission; they do not
// attest an official generated ROS package or an actual native exchange.
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <fast_io.h>
#include <array>
#include <iterator>
#include <cstdlib>

namespace ipc = ::uwvm2::imported::ros::ipc;
namespace
{
    unsigned checks{};
    void require(bool condition)
    {
        ++checks;
        if(!condition) { fast_io::io::perrln("FAIL custom fixed Topic contract check=", checks);::std::abort(); }
    }
    constexpr ipc::custom_scalar_layout layout[]{
        {ipc::custom_scalar_kind::u32, 1}, {ipc::custom_scalar_kind::boolean, 1},
        {ipc::custom_scalar_kind::f64, 1}, {ipc::custom_scalar_kind::i16, 1},
        {ipc::custom_scalar_kind::u8, 3}, {ipc::custom_scalar_kind::f32, 2}};
    constexpr ipc::custom_topic_descriptor descriptor{
        1, "uwvm_custom_test/Reading", "0123456789abcdef0123456789abcdef",
        "UWVMROS1_0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef",
        26, layout, ::std::size(layout)};
}
int main()
{
    require(ipc::custom_topic_registry_well_formed({&descriptor, 1}));
    require(ipc::custom_topic_entries().size == 0); // Default builds register no custom type.
    require(!ipc::find_custom_topic(1, descriptor.type));
    require(!ipc::find_custom_topic(2, descriptor.type));
    ::std::array<::std::uint8_t, 26> bytes{};
    ipc::custom_write_bits(bytes.data(), 0x04030201, 4);bytes[4] = 1;
    ipc::custom_write_bits(bytes.data() + 5, 0xc029000000000000ULL, 8); // -12.5, finite binary64.
    ipc::custom_write_bits(bytes.data() + 13, 0x8000, 2); // Signed minimum has a fixed wire representation.
    bytes[15] = 1;bytes[16] = 2;bytes[17] = 255;
    ipc::custom_write_bits(bytes.data() + 18, 0x80000000, 4); // Negative zero remains finite.
    ipc::custom_write_bits(bytes.data() + 22, 0x7f7fffff, 4); // Largest finite binary32.
    require(bytes[0] == 1 && bytes[3] == 4 && ipc::custom_read_bits(bytes.data(), 4) == 0x04030201);
    require(ipc::validate_custom_topic(descriptor, bytes.data(), bytes.size()));
    require(!ipc::validate_custom_topic(descriptor, nullptr, bytes.size()));
    for(::std::size_t size{}; size != bytes.size(); ++size)
    { require(!ipc::validate_custom_topic(descriptor, bytes.data(), size)); }
    require(!ipc::validate_custom_topic(descriptor, bytes.data(), bytes.size() + 1));
    bytes[4] = 2;require(!ipc::validate_custom_topic(descriptor, bytes.data(), bytes.size()));bytes[4] = 1;
    for(auto invalid : {0x7ff0000000000000ULL, 0xfff0000000000000ULL, 0x7ff8000000000001ULL, 0x7ff0000000000001ULL})
    {
        ipc::custom_write_bits(bytes.data() + 5, invalid, 8);
        require(!ipc::validate_custom_topic(descriptor, bytes.data(), bytes.size()));
    }
    ipc::custom_write_bits(bytes.data() + 5, 0, 8);
    for(auto invalid : {0x7f800000U, 0xff800000U, 0x7fc00001U, 0x7f800001U})
    {
        ipc::custom_write_bits(bytes.data() + 22, invalid, 4);
        require(!ipc::validate_custom_topic(descriptor, bytes.data(), bytes.size()));
    }
    ipc::custom_write_bits(bytes.data() + 22, 0, 4);
    require(ipc::validate_custom_topic(descriptor, bytes.data(), bytes.size()));
    auto bad{descriptor};bad.wire_bytes = 25;require(!ipc::custom_topic_layout_valid(bad));
    bad = descriptor;bad.layout = nullptr;require(!ipc::custom_topic_layout_valid(bad));
    bad = descriptor;bad.layout_count = 4097;require(!ipc::custom_topic_layout_valid(bad));
    bad = descriptor;bad.type = "std_msgs/String";require(!ipc::custom_topic_registry_well_formed({&bad, 1}));
    bad = descriptor;bad.type = "uwvm_custom_test/msg/Reading";require(!ipc::custom_topic_registry_well_formed({&bad, 1}));
    bad = descriptor;bad.native_identity = "guest-supplied-identity";require(!ipc::custom_topic_registry_well_formed({&bad, 1}));
    bad = descriptor;bad.codec_identity = "UWVMROS1_INVALID";require(!ipc::custom_topic_registry_well_formed({&bad, 1}));
    ::std::array duplicates{descriptor, descriptor};require(!ipc::custom_topic_registry_well_formed({duplicates.data(), duplicates.size()}));
    constexpr ipc::custom_scalar_layout zero[]{{ipc::custom_scalar_kind::u8, 0}};
    bad = descriptor;bad.layout = zero;bad.layout_count = 1;bad.wire_bytes = 0;require(!ipc::custom_topic_layout_valid(bad));
    constexpr ipc::custom_scalar_layout amplified[]{{ipc::custom_scalar_kind::u64, 4097}};
    bad = descriptor;bad.layout = amplified;bad.layout_count = 1;require(!ipc::custom_topic_layout_valid(bad));
    bad = descriptor;bad.layout = nullptr;bad.layout_count = 0;bad.wire_bytes = 0;
    require(!ipc::custom_topic_registry_well_formed({&bad, 1}));
    require(!ipc::validate_custom_topic(bad, nullptr, 0));
    fast_io::io::println("PASS custom fixed Topic scanner contracts checks=", checks);
}
