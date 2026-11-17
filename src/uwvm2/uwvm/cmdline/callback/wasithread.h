/** @file @brief Cold host configuration of independent I1T authority and counts. */
#pragma once
#ifndef UWVM_MODULE
#include <array>
#include <string_view>
#include <uwvm2/utils/control/wasithread.h>
#include <uwvm2/utils/control/wasithread_system.h>
#include <uwvm2/utils/control/host_diagnostics.h>
#include <uwvm2/utils/cmdline/impl.h>
#include <uwvm2/uwvm/io/impl.h>
#endif
namespace uwvm2::uwvm::cmdline::params::details::wasithread_details
{
    namespace model=::uwvm2::utils::control::wasithread;
    enum class action {create,bind,enable,disable,interface,limit,ratio,inspect};
    using status=::uwvm2::utils::cmdline::parameter_return_type;
    /** @brief Parse a complete command before policy mutation; never grants WASIp1 rights. */
    template<model::scope Scope,action Action,::std::size_t Count>
    inline status callback(::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        using kind=::uwvm2::utils::cmdline::parameter_parsing_results_type;
        auto fail=[]() noexcept {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                u8"uwvm: [error] Invalid wasithread (I1T) policy: check scope creation, unique module binding, arguments and cold configuration. See --help wasithread.");
            return status::return_m1_imme;
        };
        ::std::array<::std::u8string_view,Count> arguments{};auto* next=current+1u;
        for(auto& value:arguments)
        {
            if(next==end||(next->type!=kind::arg&&next->type!=kind::occupied_arg))return fail();
            value={next->str.data(),next->str.size()};++next;
        }
        ::std::u8string_view target;::std::size_t index{};
        if constexpr(Scope!=model::scope::global){target=arguments[0];index=1;}
        auto& policy=model::runtime_policy();bool done{};
        if constexpr(Action==action::create)done=policy.create(Scope,target);
        else if constexpr(Action==action::bind)done=policy.bind(Scope,target,arguments[index]);
        else if constexpr(Action==action::enable||Action==action::disable)done=policy.enable(Scope,target,Action==action::enable);
        else if constexpr(Action==action::interface)
        {
            if(arguments[index]!=u8"thread-spawn")return fail();
            auto value=arguments[index+1];model::exposure mode;
            if(value==u8"allow")mode=model::exposure::allow;
            else if(value==u8"deny")mode=model::exposure::deny;
            else if(value==u8"hidden")mode=model::exposure::hidden;
            else return fail();
            done=policy.expose(Scope,target,mode);
        }
        else if constexpr(Action==action::limit||Action==action::ratio)
        {
            model::limit field;auto key=arguments[index];::std::uint64_t value{};
            if(key==u8"max-extra-threads")field=model::limit::extra_threads;
            else if(key==u8"max-threads")field=model::limit::threads;
            else if(key==u8"max-spawns")field=model::limit::spawns;
            else return fail();
            if constexpr(Action==action::ratio)
            {
                if(field==model::limit::spawns)return fail();
                float ratio{};if(!model::parse_ratio(arguments[index+1],ratio))return fail();
                auto system=model::inspect_system_ceiling();
                if(!system.available||!model::scale_limit(system.effective,ratio,value))return fail();
                ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,u8"wasithread I1T ratio: system-ceiling=",system.effective,
                    u8" kernel=",system.kernel,u8" process=",system.process,u8" cgroup=",system.cgroup,u8" admitted-limit=",value);
            }
            else if(!model::parse_limit(arguments[index+1],value))return fail();
            done=policy.constrain(Scope,target,field,value);
        }
        else if constexpr(Action==action::inspect)
        {
            auto value=policy.inspect(Scope,target);if(!value.exists)return fail();
            namespace diagnostic=::uwvm2::utils::control::host_diagnostics;
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            try
#endif
            {
                auto quoted=diagnostic::quote_argument(diagnostic::view(target));
                ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,u8"wasithread I1T scope=",
                    ::fast_io::mnp::code_cvt(model::name(Scope)),u8" target=",::fast_io::mnp::code_cvt(::std::string_view{quoted.data(),quoted.size()}),u8" enabled=",value.enabled,
                    u8" interface=",::fast_io::mnp::code_cvt(model::name(value.interface)),u8" sealed=",value.sealed,
                    u8" mains=",value.occupied.mains,u8" extra=",value.occupied.children,u8" spawn-admissions=",value.occupied.spawn_admissions);
                // UINT64_MAX is the exact printed unlimited sentinel. The
                // command accepts only the word unlimited for that sentinel.
                ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,u8"max-extra-threads=",value.bound.extra_threads,
                    u8" max-threads=",value.bound.threads,u8" max-spawns=",value.bound.spawns,u8" (18446744073709551615=unlimited)");
                done=true;
            }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            catch(...){return fail();}
#endif
        }
        if(!done)return fail();
        for(auto* item=current+1u;item!=next;++item)item->type=kind::occupied_arg;
        return status::def;
    }
}
