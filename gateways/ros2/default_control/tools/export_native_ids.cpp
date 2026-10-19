// SPDX-License-Identifier: Apache-2.0
// Separate owned NATIVE exporter. No Registry expected ID is read and no entity
// is created. Actual getters may load libraries; run only in a real owned warm
// loader phase, then retain complete native MD/source/build/provenance evidence.
#include "registry.hpp"
#include "default_control_gateway_features.h"
#include <fast_io.h>
#include <array>
#include <cstring>
#include <string_view>
#if UWVM_DEFAULT_CONTROL_GATEWAY_NATIVE_API
#include <rosidl_runtime_c/type_hash.h>
#endif
using namespace uwvm_default_control_gateway_detail;
int main(){
#if !UWVM_DEFAULT_CONTROL_GATEWAY_NATIVE_API
 fast_io::perr("Actual complete original SDK metadata is unavailable (95)\n");return 95;
#else
 try {
 auto const&g=generated_graph();if(g.identity_names.empty()||g.identity_names.size()>maximum_identities)return 22;
 std::array<std::array<std::uint8_t,32>,maximum_identities>hashes{};std::array<bool,maximum_identities>present{};
 for(auto const&n:g.nodes){if(n.identity>=g.identity_names.size()||g.identity_names[n.identity]!=n.name)return 22;
 auto inspect=[&](auto const*p){if(!p||!p->get_type_hash_func)return false;auto h=p->get_type_hash_func(p);if(!h||h->version!=1)return false;
 bool nonzero{};for(auto b:h->value)nonzero|=b!=0;if(!nonzero)return false;
 if(present[n.identity])return std::memcmp(hashes[n.identity].data(),h->value,32)==0;
 std::memcpy(hashes[n.identity].data(),h->value,32);present[n.identity]=true;return true;};
 if(n.kind==Kind::message){if(!n.message_generic||!n.message_introspection||!inspect(n.message_generic())||!inspect(n.message_introspection()))return 22;}
 else if(!n.service_generic||!n.service_introspection||!inspect(n.service_generic())||!inspect(n.service_introspection()))return 22;
 }
 // No partial JSON is written until every actual original C/CPP hash agrees.
 for(std::size_t i{};i<g.identity_names.size();++i)if(!present[i])return 22;
 constexpr char digits[]="0123456789abcdef";
 fast_io::print("{\"format_version\":1,\"status\":\"ACTUAL_NATIVE_EXPORT_REQUIRES_EXTERNAL_QUALIFICATION\",\"native_types\":[");
 for(std::size_t i{};i<g.identity_names.size();++i){std::array<char,64>hex{};for(std::size_t j{};j<32;++j){hex[2*j]=digits[hashes[i][j]>>4];hex[2*j+1]=digits[hashes[i][j]&15];}
 auto name=g.identity_names[i];if(i)fast_io::print(",");fast_io::print("{\"native_type\":\"",fast_io::mnp::strvw(name.data(),name.data()+name.size()),"\",\"rihs01\":\"",fast_io::mnp::strvw(hex.data(),hex.data()+hex.size()),"\"}");}
 fast_io::print("]}\n");return 0;
 }catch(...){fast_io::perr("Original native export failed; no identity is qualified\n");return 22;}
#endif
}
