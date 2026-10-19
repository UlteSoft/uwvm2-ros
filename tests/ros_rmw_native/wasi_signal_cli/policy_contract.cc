// SPDX-License-Identifier: Apache-2.0
#include <array>
#include <cstdint>
#include <uwvm2/uwvm/cmdline/callback/wasi_thread_signals.h>
#include <uwvm2/uwvm/run/threaded_signal_cli.h>
#if !UWVM2_HAS_THREADED_PROFILE_HOST_API
#error This source contract requires the actual Linux threaded Runtime build.
#endif
// Pure admission/arithmetic contracts only. Root links into its actual Native
// driver; none is an OS signal, syscall, Guest, timing or physical-join proof.
extern "C++" int uwvm_signal_cli_policy_contract() noexcept
{
    namespace p=::uwvm2::uwvm::cmdline::params::details;
    namespace c=::uwvm2::uwvm::run::threaded_signal_cli;
    p::wasi_thread_signal_policy output{};
    if(output.selected())return 1;
    ::std::uint64_t number=55u;
    for(auto bad:{u8"",u8"0",u8"-1",u8"+1",u8"1e3",u8"1x",u8" 1",u8"1 ",u8"\t1",u8"0x10",u8"18446744073709551616"})
        if(p::wasi_thread_signals_number(::uwvm2::utils::container::u8cstring_view{::fast_io::mnp::os_c_str(bad)},number)||number!=55u)return 2;
    if(!p::wasi_thread_signals_number(u8"18446744073709551615",number)||number!=UINT64_MAX)return 3;
    if(!p::wasi_thread_signals_number(u8"0007",number)||number!=7u)return 3;
    ::std::array<::std::uint64_t,6> values{3u,UINT64_MAX,1048576u,1048576u,60000u,60000u};
    if(!p::wasi_thread_signals_derive(values,output)||!output.selected()||output.channel.cookie!=UINT64_MAX)return 4;
    auto const valid=values;auto const saved=output;
    for(auto position:{0u,1u,2u,3u,4u,5u})
    {
        values=valid;values[position]=0u;
        if(p::wasi_thread_signals_derive(values,output)||output.channel.cookie!=saved.channel.cookie||
            output.channel.read_fd!=saved.channel.read_fd||output.channel.maximum_frames!=saved.channel.maximum_frames||
            output.channel.maximum_pipe_bytes!=saved.channel.maximum_pipe_bytes||output.grace_ms!=saved.grace_ms||output.retire_ms!=saved.retire_ms)return 5;
    }
    for(auto position:{2u,3u,4u,5u})
    {values=valid;++values[position];if(p::wasi_thread_signals_derive(values,output))return 6;}
    values=valid;values[0]=2u;if(p::wasi_thread_signals_derive(values,output))return 7;
    values=valid;values[0]=static_cast<::std::uint64_t>(INT32_MAX)+1u;if(p::wasi_thread_signals_derive(values,output))return 8;
    values=valid;values[3]=31u;if(p::wasi_thread_signals_derive(values,output))return 9;
    c::clock::time_point actual_math_input{};auto end=c::clock::time_point::min();
    if(!c::deadline_after(actual_math_input,60000u,end)||end-actual_math_input!=::std::chrono::seconds{60})return 10;
    auto const sentinel=end;
    if(c::deadline_after(c::clock::time_point::max(),1u,end)||end!=sentinel)return 11;
    if(c::deadline_after(actual_math_input,0u,end)||end!=sentinel)return 12;
    ::uwvm2::runtime::lib::threaded_retirement row{true,false,0u,0u,0,0u,0u};
    if(!c::complete(row))return 13;
    row.physically_joined=false;if(c::complete(row))return 14;row.physically_joined=true;
    row.preparing=1u;if(c::complete(row))return 15;row.preparing=0u;
    row.native_pending=1u;if(c::complete(row))return 16;row.native_pending=0u;
    row.native_error=EBUSY;if(c::complete(row))return 17;row.native_error=0;
    row.compiler_tickets_pending=1u;if(c::complete(row))return 18;row.compiler_tickets_pending=0u;
    row.native_cleanup_pending=1u;if(c::complete(row))return 19;
    return 0;
}
