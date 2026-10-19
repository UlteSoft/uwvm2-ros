// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "profile_internal.hpp"
#include "native_rcl_service_registry.h"
namespace uwvm2::ros_gateway::default_control_gateway::detail {
template<class Service>class ServiceProof {
 std::shared_ptr<profile const> owner_;uwvm_rmw_codec const*root_{};
public:
 ServiceProof(std::shared_ptr<profile const>p,std::string_view name):owner_{std::move(p)},root_{owner_?uwvm_default_control_gateway_detail::codec(*owner_,name):nullptr}{}
 uwvm_rmw_codec const*root()const noexcept{return root_;}
 bool selected(ros::capability_spec const&c)const noexcept {
  uwvm_rmw_codec const*r{};return owner_&&!uwvm_default_control_gateway_detail::select(*owner_,c,r)&&r==root_;
 }
 std::uint32_t validate(bool response,std::uint8_t const*b,std::size_t n)const noexcept {
  return root_?uwvm_default_control_gateway_detail::body(*owner_,root_->native_type,response,b,n):95u;
 }
};
}
