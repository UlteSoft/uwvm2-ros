// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <fast_io.h>
#include <cstdint>
#include <string_view>
namespace uwvm_default_control_detail {
inline bool decimal(std::string_view text,std::uint64_t&output)noexcept {
 if(text.empty())return false;for(char c:text)if(c<'0'||c>'9')return false;
 try {std::uint64_t next{};auto result=fast_io::parse_by_scan(text.data(),text.data()+text.size(),fast_io::mnp::dec_get<true,true>(next));
  if(result.code!=fast_io::parse_code::ok||result.iter!=text.data()+text.size())return false;
  output=next;return true;
 }catch(...){return false;}
}
}
