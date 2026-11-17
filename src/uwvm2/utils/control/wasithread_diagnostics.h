/** @file @brief I1T rejection diagnostics, emitted after releasing admission locks. */
#pragma once
#include "wasithread.h"
#include "host_diagnostics.h"
namespace uwvm2::utils::control::wasithread
{
    inline void diagnose(policy::admission const& result,::std::u8string_view module) noexcept
    {
        namespace diagnostic=host_diagnostics;
        constexpr auto id=host_interfaces::find(host_interfaces::family::wasithread,::std::string_view{"thread-spawn"});
        auto kind=result.source.exists?result.source.kind:scope::single;
        auto target=result.source.exists?result.source.target:module;
        ::std::string_view reason,field;
        switch(result.reason)
        {
            case failure::none:return;
            case failure::unbound:reason="the executing module has no valid authenticated identity";break;
            case failure::disabled:reason="the I1T scope or execution admission is disabled";break;
            case failure::hidden:reason="thread-spawn is hidden for the executing module";break;
            case failure::denied:reason="thread-spawn is explicitly denied";break;
            case failure::execution_profile:
                diagnostic::report(id,diagnostic::event::denied,name(kind),diagnostic::view(target),
                    "no private threaded execution profile was selected; no native thread was created",
                    "--wasi-threads <children> <spawns> <stack-bytes> <storage-bytes> <code-bytes> <shared-memory-bytes>");
                return;
            case failure::extra_threads:reason="additional-thread capacity is exhausted";field="max-extra-threads";break;
            case failure::threads:reason="main plus additional-thread capacity is exhausted";field="max-threads";break;
            case failure::spawns:reason="cumulative spawn admission capacity is exhausted";field="max-spawns";break;
            case failure::exhausted:reason="thread counter representation is exhausted";field="max-threads";break;
        }
        if(field.empty()){diagnostic::report(id,diagnostic::event::denied,name(kind),diagnostic::view(target),reason);return;}
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
#endif
        {
            auto remedy=diagnostic::command(host_interfaces::family::wasithread,name(kind),diagnostic::view(target),"limit");
            remedy+=' ';remedy+=field;remedy+=" <approved-limit>";
            diagnostic::report(id,diagnostic::event::limited,name(kind),diagnostic::view(target),reason,remedy);return;
        }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        catch(...) {}
#endif
        if(diagnostic::selected(id,diagnostic::event::limited)==diagnostic::severity::error&&diagnostic::fatal)diagnostic::fatal();
    }
}
