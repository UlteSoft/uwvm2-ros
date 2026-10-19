// SPDX-License-Identifier: Apache-2.0
#pragma once
#ifndef UWVM_MODULE
#include <array>
#include <cstdint>
#include <fast_io.h>
#include <uwvm2/uwvm/io/impl.h>
#include <uwvm2/uwvm/cmdline/params/impl.h>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params::details
{
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
[[nodiscard]] inline bool wasi_thread_signals_number(::uwvm2::utils::container::u8string_view text,
    ::std::uint64_t&output) noexcept
{
    auto const first=text.cbegin(),last=text.cend();
    if(first==last||!::fast_io::char_category::is_c_digit(*first))return false;
    ::std::uint64_t value{};
    // noskipws=true, skipzero=true preserves r1's positive decimal leading-zero
    // spelling while refusing signs, whitespace, radix prefixes and suffixes.
    // Actual fast_io checks overflow; only the fully consumed local temporary
    // may commit. UINT64_MAX remains a legal association cookie.
    auto const result=::fast_io::parse_by_scan(first,last,::fast_io::mnp::dec_get<true,true>(value));
    if(result.code!=::fast_io::parse_code::ok||result.iter!=last||!value)return false;
    output=value;return true;
}
[[nodiscard]] inline bool wasi_thread_signals_derive(::std::array<::std::uint64_t,6> const&v,
    wasi_thread_signal_policy&output) noexcept
{
    if(v[0]<3u||v[0]>INT32_MAX||!v[1]||!v[2]||v[2]>1048576u||v[3]<32u||v[3]>1048576u||
       !v[4]||v[4]>60000u||!v[5]||v[5]>60000u)return false;
    wasi_thread_signal_policy candidate{{static_cast<::std::uint32_t>(v[0]),v[1],v[2],v[3]},
        static_cast<::std::uint32_t>(v[4]),static_cast<::std::uint32_t>(v[5])};
    output=candidate;return true;
}
#if defined(UWVM_MODULE)
    extern "C++"
#else
    inline
#endif
    void wasi_thread_signals_pretreatment(char8_t const* const*&current,char8_t const* const*end,
    ::uwvm2::utils::container::vector<::uwvm2::utils::cmdline::parameter_parsing_results>&parsed) noexcept
{
    auto next=current+1u;
    for(unsigned count{};count<6u&&next!=end&&*next!=nullptr;++count,++next)
    {
        auto const text=::uwvm2::utils::container::u8cstring_view{::fast_io::mnp::os_c_str(*next)};
        if(!text.empty()&&text.front_unchecked()==u8'-'&&
           (text.size()==1u||text.index_unchecked(1u)<u8'0'||text.index_unchecked(1u)>u8'9'))break;
        parsed.emplace_back_unchecked(text,nullptr,::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg);
    }
    current=next;
}
#if defined(UWVM_MODULE)
    extern "C++"
#else
    inline
#endif
    ::uwvm2::utils::cmdline::parameter_return_type wasi_thread_signals_callback(
    ::uwvm2::utils::cmdline::parameter_parsing_results*,::uwvm2::utils::cmdline::parameter_parsing_results*current,
    ::uwvm2::utils::cmdline::parameter_parsing_results*end) noexcept
{
    using kind=::uwvm2::utils::cmdline::parameter_parsing_results_type;
    auto reject=[]()noexcept
    {::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,u8"uwvm: [error] --wasi-thread-signals requires all six positive finite values; fd>=3, frames/pipe<=1048576, grace/retire<=60000ms. No signal handler was installed.\n");
     return ::uwvm2::utils::cmdline::parameter_return_type::return_m1_imme;};
    ::std::array<::std::uint64_t,6> values{};auto next=current+1u;
    for(auto&value:values)
    {if(next==end||(next->type!=kind::arg&&next->type!=kind::occupied_arg)||!wasi_thread_signals_number(next->str,value))return reject();++next;}
    wasi_thread_signal_policy admitted{};
    if(!wasi_thread_signals_derive(values,admitted)||
       !::uwvm2::runtime::lib::runtime_threaded_profile_signal_configure_host_api(admitted.channel))return reject();
    // Complete admitted DATA only. sigaction is owned by the later actual run
    // scope, after ALL options parsed successfully and before Guest launch.
    wasi_thread_signal_config=admitted;
    for(auto p=current+1u;p!=next;++p)p->type=kind::occupied_arg;
    return ::uwvm2::utils::cmdline::parameter_return_type::def;
}
#endif
}
