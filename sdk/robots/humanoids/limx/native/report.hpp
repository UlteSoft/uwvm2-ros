#pragma once
#include "gateways/common/native_io.h"
/** Native diagnostics only: checked fixed storage, no printf/locale/allocation. */
namespace humanoid_native_report {
template<class... V> std::uint32_t emit(V const&... values)noexcept {
 ::uwvm2::ros_gateway::native_io::fixed_text<1024> text;
 auto append=[&](auto const& value)noexcept{using T=std::decay_t<decltype(value)>;if constexpr(std::is_integral_v<T>)return text.number(value);else return text.append(value);};
 if(!(append(values)&&...)||!text.append("\n"))return 90;
 return ::uwvm2::ros_gateway::native_io::write_all(STDOUT_FILENO,text.view())?0u:5u;
}
}
