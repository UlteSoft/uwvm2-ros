/* SPDX-License-Identifier: Apache-2.0
 * Real upstream bootstrap helper, not an rclcpp namespace facade. Caller owns
 * the Registry/profile for the entire actual upstream app/context lifetime.
 * This function only selects seven exact real-object codecs; it does not grant
 * endpoints, clock/log/env/graph access or install a policy. A's original static
 * forwarding and C/D's real providers must be composed separately.
 */
#pragma once
#include <uwvm_rmw/bridge.h>
#include <uwvm_rmw/cpp_codecs.hpp>
namespace uwvm_rmw_cpp_example {
inline std::uint32_t bind_registry(uwvm_rmw_cpp::Registry const& registry,uwvm_rmw_profile& profile) noexcept {
  if(registry.error())return registry.error();auto values=registry.codecs();if(values.size()!=7)return 22;
  if(profile.version!=1||!profile.backend||profile.backend->version!=2)return 22;
  profile.codecs=values.data();profile.codec_count=values.size();return 0;
}
/* Endpoint codec_index uses uwvm_rmw_cpp::Codec, never a guessed name:
 * string=0, empty_request=1, empty_response=2, empty_service=3,
 * add_request=4, add_response=5, add_service=6.
 * Only string/empty_service/add_service are endpoint roots. Component/event
 * identities are validated as complete original dependencies, not extra grants.
 */
}
