// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "default_control_gateway.h"
#include "registry.hpp"
#include <uwvm_rmw/codec.h>
namespace uwvm_default_control_gateway_detail {
using ::uwvm2::ros_gateway::default_control_gateway::profile;
uwvm_rmw_codec const* codec(profile const&,std::string_view) noexcept;
std::uint32_t body(profile const&,std::string_view,bool,void const*,std::size_t) noexcept;
std::uint32_t select(profile const&,::uwvm2::imported::ros::capability_spec const&,uwvm_rmw_codec const*&) noexcept;
}
