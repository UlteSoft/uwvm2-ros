/* SPDX-License-Identifier: Apache-2.0
 * Real generated C++ object codecs for the fixed modern ROS2 SDK. This file is
 * not a namespace facade or a replacement generated header. The implementation
 * includes the actual std_msgs/String, std_srvs/Empty, AddTwoInts and complete
 * Event/ServiceEventInfo/Time generated types and introspection libraries.
 *
 * Registry initialization owns all local storage before warming actual native
 * metadata. It opens no capability. Call it under an owned loader/mapping phase
 * BEFORE a stopped/native-map checkpoint: original TypeSupport dispatch may
 * lazily load native libraries. WASI must use A's actual static forwarding path,
 * never a fake dlopen. An actual native identity export and policy are separate
 * qualification inputs; providing bytes/function pointers is not qualification.
 *
 * The nonmoving Registry, actual native libraries, and immutable profile must
 * outlive EVERY borrowing option/context/entity. Uninstall the profile only
 * after all contexts/options finish; destroying borrowed storage is caller error.
 * No compiled old uwvm_rmw_codec layout may be mixed with the new trailing state
 * field. B/C/adapters/application must all recompile against this exact header.
 */
#pragma once
#include "codec.h"
#include <array>
#include <cstdint>
#include <memory>
#include <span>
namespace uwvm_rmw_cpp {
using Hash=std::array<std::uint8_t,32>;
enum class Identity:std::size_t {string,time,event_info,empty_request,empty_response,
  empty_event,add_request,add_response,add_event,empty_service,add_service,count};
struct NativeIdentities {std::array<Hash,static_cast<std::size_t>(Identity::count)> hashes{};};
enum class Codec:std::size_t {string,empty_request,empty_response,empty_service,
  add_request,add_response,add_service,count};
namespace detail {struct RegistryState;}
class Registry {
  std::unique_ptr<detail::RegistryState> state_;
  std::uint32_t error_{22};
public:
  /** Complete selected native RIHS01 identities are mandatory, never defaulted
   * from the runtime or computed here. The String canonical extent is separate
   * from its original unbounded native field, and must be4..4194304 bytes. */
  explicit Registry(NativeIdentities const&,std::size_t maximum_string_wire) noexcept;
  ~Registry();
  Registry(Registry const&)=delete;Registry& operator=(Registry const&)=delete;
  Registry(Registry&&)=delete;Registry& operator=(Registry&&)=delete;
  std::uint32_t error()const noexcept{return error_;}
  /** Empty on failure. Seven contiguous immutable descriptors are suitable for
   * the actual B profile.codecs/codec_count; no selection/grant is installed. */
  std::span<uwvm_rmw_codec const> codecs()const noexcept;
  uwvm_rmw_codec const* codec(Codec)const noexcept;
};
}
