// APL-2.0. Diagnostics do not grant authority or alter ABI errno.
#pragma once
#include <uwvm2/utils/control/host_diagnostics.h>
namespace uwvm2::imported::wasi::wasip1::environment
{
    namespace host_diagnostic=::uwvm2::utils::control::host_diagnostics;
    template<typename Env> inline std::string_view diagnostic_scope(Env const& env) noexcept
    {return env.trace_wasip1_group_kind==trace_wasip1_group_kind_t::custom_group?"group":"single";}
    template<typename Env> inline std::string_view diagnostic_target(Env const& env) noexcept
    {return host_diagnostic::view(env.trace_wasip1_group_name_storage);}
    template<typename Env> inline void diagnostic_denied(Env const& env,std::size_t id,bool interface,std::uint64_t first=UINT64_MAX) noexcept
    {
        namespace api=::uwvm2::utils::control::host_interfaces;
        if(host_diagnostic::selected(id,host_diagnostic::event::denied)==host_diagnostic::severity::off)return;
#if defined(__cpp_exceptions)
        try
#endif
        {
            std::string remedy;
            if(!interface)
            {
                auto function=api::catalogue[id].function;
                if(function.ends_with("_wasm64"))function.remove_suffix(7);
                std::string permission;
                if(function.starts_with("args_"))permission="args";
                else if(function.starts_with("environ_"))permission="environ";
                else if(function=="random_get")permission="random";
                else if(function=="proc_exit")permission="proc-exit";
                else if(function=="proc_raise")permission="proc-raise";
                else if(function=="sched_yield")permission="sched-yield";
                else if(function=="thread-spawn")permission="thread-spawn";
                else if(function=="fd_write" && first==1)permission="stdout.write";
                else if(function=="fd_write" && first==2)permission="stderr.write";
                else if(function=="fd_read" && first==0)permission="stdin.read";
                else if(function.starts_with("fd_") && first<3 && !env.grants_stdio(static_cast<unsigned>(first)))
                {
                    constexpr std::string_view streams[]{"stdin.read","stdout.write","stderr.write"};
                    permission=streams[first];
                }
                else if((function=="fd_fdstat_get" || function=="fd_filestat_get") && first<3 &&
                        !env.permits(permission_t::stdio_metadata))permission="stdio.metadata";
                else if(function.starts_with("clock_") && first<4)
                {
                    constexpr std::string_view clocks[]{"realtime","monotonic","process","thread"};
                    permission=clocks[first];permission+=function=="clock_res_get"?"-resolution":"-clock";
                }
                if(!permission.empty())remedy=host_diagnostic::command(api::family::wasip1,diagnostic_scope(env),diagnostic_target(env),"grant")+" "+permission;
                else if(function.starts_with("path_") || function.starts_with("fd_"))
                {
                    remedy=host_diagnostic::command(api::family::wasip1,diagnostic_scope(env),diagnostic_target(env),"mount-dir-rights")+
                        " <existing-guest-preopen> <approved-base-rights:u64> <approved-inheriting-rights:u64>; "
                        "use its authorized descriptor; guest calls cannot increase descriptor rights";
                }
                else if(function=="poll_oneoff")
                {
                    remedy=host_diagnostic::command(api::family::wasip1,diagnostic_scope(env),diagnostic_target(env),"grant")+
                        " <stdio.readiness|realtime-timer|monotonic-timer|process-timer|thread-timer> for the actual subscription";
                }
                else remedy="--help wasi; configure the exact socket/preopen rights required by this call";
            }
            host_diagnostic::report(id,host_diagnostic::event::denied,diagnostic_scope(env),diagnostic_target(env),
                interface?"interface calls are not allowed":"operation or resource authority was not granted",remedy);
        }
#if defined(__cpp_exceptions)
        catch(...) {
            if(host_diagnostic::selected(id,host_diagnostic::event::denied)==host_diagnostic::severity::error && host_diagnostic::fatal)
                host_diagnostic::fatal();
        }
#endif
    }
}
