// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "profile_internal.hpp"
#include "native_rmw_subscription.h"
namespace uwvm2::ros_gateway::default_control_gateway::detail {
template<class Native>class TopicProof {
 std::shared_ptr<profile const>owner_;uwvm_rmw_codec const*root_{};
public:
 TopicProof(std::shared_ptr<profile const>p,std::string_view name):owner_{std::move(p)},root_{owner_?uwvm_default_control_gateway_detail::codec(*owner_,name):nullptr}{}
 uwvm_rmw_codec const*root()const noexcept{return root_;}
 bool selected(ros::capability_spec const&cap)const noexcept {
  uwvm_rmw_codec const*r{};return owner_&&!uwvm_default_control_gateway_detail::select(*owner_,cap,r)&&r==root_&&
   root_->generated_support==rosidl_typesupport_cpp::get_message_type_support_handle<Native>();
 }
 std::uint32_t validate(std::uint8_t const*b,std::size_t n)const noexcept {
  return root_?uwvm_default_control_gateway_detail::body(*owner_,root_->native_type,false,b,n):95u;
 }
};
}
