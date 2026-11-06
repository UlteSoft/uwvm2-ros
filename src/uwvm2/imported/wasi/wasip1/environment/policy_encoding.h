/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
#include "environment.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::environment
{
    // Emit canonical little-endian, length-prefixed bytes to sink(unsigned char).
    // Call only under the trusted initialization/update quiescence proof. This
    // encoding excludes dynamic counters, offsets, closed FDs and guest memory.
    // Process-local resource incarnations must be combined with the VM nonce.
    template<wasip1_memory Memory, typename Configuration, typename Sink>
    [[nodiscard]] bool serialize_policy_with_configuration(wasip1_environment<Memory> const& env, Configuration const& configuration, Sink&& sink)
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        auto number{[&](::std::uint64_t value) {for(unsigned i{};i!=8u;++i)sink(static_cast<unsigned char>(value>>(i*8u)));}};
        auto text{[&](auto const& value) {number(value.size());for(auto c:value)sink(static_cast<unsigned char>(c));}};
        text(::uwvm2::utils::container::u8string_view{u8"uwvm.wasip1.policy.v6"});
        number(configuration.interface_enabled);
        for(auto value:configuration.interfaces.entries)sink(static_cast<unsigned char>(value));number(static_cast<::std::uint32_t>(configuration.non_fd_permissions));
        if constexpr(requires { configuration.fd_limit; }) { number(configuration.fd_limit); }
        else { number(configuration.fd_storage.fd_limit); }
        number(env.disable_utf8_check);
        number(static_cast<::std::uint64_t>(reinterpret_cast<::std::uintptr_t>(env.wasip1_proc_raise_func_ptr)));
        number(static_cast<::std::uint64_t>(reinterpret_cast<::std::uintptr_t>(env.wasip1_sched_yield_func_ptr)));
        for(auto const& limit:configuration.limits.operations)
        {number(limit.request_bytes);number(limit.window_bytes);number(limit.window_calls);number(limit.total_bytes);number(limit.total_calls);number(limit.concurrent);for(auto action:limit.actions)sink(static_cast<unsigned char>(action));for(auto action:limit.exit_actions)sink(static_cast<unsigned char>(action));sink(static_cast<unsigned char>(limit.native_wait_exit_action));}
        for(auto value:configuration.limits.clock_granularity_ns)number(value);
        number(configuration.limits.max_poll_subscriptions);number(configuration.limits.max_poll_wait_ns);number(configuration.limits.max_args_env_bytes);
        number(configuration.limits.filesystem_base_rights);number(configuration.limits.filesystem_inheriting_rights);
        ::std::uint64_t approved_string_bytes{};
        auto strings{[&](auto const& entries) {
            auto& bytes{approved_string_bytes};number(entries.size());
            for(auto const& entry:entries)
            {
                if(entry.size()>=configuration.limits.max_args_env_bytes || bytes>configuration.limits.max_args_env_bytes-entry.size()-1u)return false;
                bytes+=entry.size()+1u;text(entry);
            }
            return true;
        }};
        if(!strings(configuration.argv)||!strings(configuration.envs))return false;
        number(env.policy_resources.size());
        for(auto const& resource:env.policy_resources)
        {
            if(resource.incarnation==0u)return false;
            number(resource.kind);number(resource.logical_slot);number(resource.incarnation);text(resource.label);
            number(resource.rights_base);number(resource.rights_inheriting);
            // Rebinding a live reserved slot is a new host policy, never a
            // guest fd lifecycle operation. Closed tombstones remain legitimate.
            if(resource.kind<3u)
            {
                if(resource.logical_slot>=env.fd_storage.opens.size())return false;
                auto const* fd{env.fd_storage.opens.index_unchecked(resource.logical_slot).fd_p};
                if(fd==nullptr)return false;
                if(fd->close_pos==SIZE_MAX && static_cast<::std::uint64_t>(reinterpret_cast<::std::uintptr_t>(fd->wasi_fd.ptr))!=resource.incarnation)return false;
            }
        }
        return true;
#else
        static_cast<void>(env);static_cast<void>(configuration);static_cast<void>(sink);return false;
#endif
    }
    template<wasip1_memory Memory, typename Sink>
    [[nodiscard]] bool serialize_policy(wasip1_environment<Memory> const& env, Sink&& sink)
    { return serialize_policy_with_configuration(env, env, static_cast<Sink&&>(sink)); }

}
