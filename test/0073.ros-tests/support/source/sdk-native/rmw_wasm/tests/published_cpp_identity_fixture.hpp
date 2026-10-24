/* SPDX-License-Identifier: Apache-2.0
 * EXPLICIT NATIVE-CONTRACT FIXTURE ONLY. These are copied published RIHS01
 * values from actual official buildfarm JSON retained in the source package:
 * std_msgs/std_srvs5.3.8 and example_interfaces0.12.1 Jazzy20260911 amd64.
 * They are NOT a fresh installed-SDK export, a default production profile, a
 * hash derivation, transport qualification, or permission to access ROS.
 * A different real SDK is a real identity mismatch; obtain and qualify its
 * complete export rather than adjusting these expectations after a failure.
 */
#pragma once
#include <uwvm_rmw/cpp_codecs.hpp>
namespace cpp_fixture {
inline uwvm_rmw_cpp::NativeIdentities published() {
  static constexpr char const* hex[]{
    "df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18", // std_msgs/msg/String
    "b106235e25a4c5ed35098aa0a61a3ee9c9b18d197f398b0e4206cea9acf9c197", // builtin_interfaces/msg/Time
    "41bcbbe07a75c9b52bc96bfd5c24d7f0fc0a08c0cb7921b3373c5732345a6f45", // service_msgs/msg/ServiceEventInfo
    "458eaf200a3f63d9cc53d19436edebc0c140ef4c93ca75d5bd4ce19fd7759e35", // std_srvs/srv/Empty_Request
    "0a2858b3f60df644a61d0fa44cfc923a3d3628e0ad9c8ad8534b1a2c92a413dc", // std_srvs/srv/Empty_Response
    "66fa7fcfb5f2bf04b9a8a55603318c5c402edfc4bd56bb0ed7b602990c29305a", // std_srvs/srv/Empty_Event
    "000c5fd92d6b2e1a05949348f584d6d652adea1e92d691792011ac2273508302", // example_interfaces/srv/AddTwoInts_Request
    "de5c030d4af33cba2749310b249737b631594703f9300495f48bffb2b44dcc2f", // example_interfaces/srv/AddTwoInts_Response
    "32c1d140259c71e5c355115942bcb31df98b4330e4d906b1b75ccb1c9b3ce6c8", // example_interfaces/srv/AddTwoInts_Event
    "5888399dedec5ccc85ea6451949fd2c9f97bfdf963f9a588821639fcd31b5d19", // std_srvs/srv/Empty
    "e118de6bf5eeb66a2491b5bda11202e7b68f198d6f67922cf30364858239c81a", // example_interfaces/srv/AddTwoInts
  };
  uwvm_rmw_cpp::NativeIdentities out;
  auto digit=[](char c)->unsigned{return c>='0'&&c<='9'?unsigned(c-'0'):unsigned(c-'a'+10);};
  for(std::size_t i{};i!=out.hashes.size();++i)for(std::size_t j{};j!=32;++j)
    out.hashes[i][j]=static_cast<std::uint8_t>((digit(hex[i][2*j])<<4)|digit(hex[i][2*j+1]));
  return out;
}
}
