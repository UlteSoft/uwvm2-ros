// Real configuration helpers and public callbacks; prepared for Linux cgroup execution.
// SPDX-License-Identifier: Apache-2.0
// This fixture does not execute a Wasm Guest or call host clock/random/signal APIs.
// It checks the actual policy implementation rather than a substitute permission table.
#include <array>
#include <cstdint>
#include <fast_io.h>
#include <uwvm2/uwvm/cmdline/callback/wasip1_permissions.h>
#include <uwvm2/uwvm/cmdline/callback/wasip1_single_inherit_system_environment.h>
#include <uwvm2/uwvm/cmdline/callback/wasip1_single_noinherit_system_environment.h>
#include <uwvm2/uwvm/cmdline/callback/wasip1_group_inherit_system_environment.h>
#include <uwvm2/uwvm/cmdline/callback/wasip1_group_noinherit_system_environment.h>

namespace storage = ::uwvm2::uwvm::imported::wasi::wasip1::storage;
namespace environment = ::uwvm2::imported::wasi::wasip1::environment;
namespace abi = ::uwvm2::imported::wasi::wasip1::abi;
namespace callbacks = ::uwvm2::uwvm::cmdline::params::details;
namespace parameters = ::uwvm2::uwvm::cmdline::params;
using permission = environment::permission_t;
using result = ::uwvm2::utils::cmdline::parameter_return_type;
using callback = ::uwvm2::utils::cmdline::handle_func_type;
using token = ::uwvm2::utils::cmdline::parameter_parsing_results;
using token_type = ::uwvm2::utils::cmdline::parameter_parsing_results_type;

namespace
{
    unsigned checks{};
    void require(bool condition)
    {
        ++checks;
        if(!condition) { ::fast_io::io::perrln("FAIL WASIp1 configuration check=", checks); ::fast_io::fast_terminate(); }
    }
    // Invoke a real callback with bounded synthetic parser tokens. This does not
    // replace the full CLI parser tests: duplicate presence flags on parameter
    // objects are inspected separately below and full CLI cases are documented.
    result invoke(callback fn, char8_t const* operand, char8_t const* selector = nullptr)
    {
        token args[3]{};
        args[0].str = u8"--fixture"; args[0].type = token_type::parameter;
        if(operand == nullptr) { return fn(args, args, args + 1u); }
        // All synthetic operands are terminated fixture literals. The explicit
        // fast_io view records their valid length instead of assigning a raw
        // pointer to the parser's bounded string representation.
        args[1].str = ::fast_io::u8cstring_view{::fast_io::mnp::os_c_str(selector == nullptr ? operand : selector)};
        args[1].type = token_type::arg;
        auto* end{args + 2u};
        if(selector != nullptr)
        {
            args[2].str = ::fast_io::u8cstring_view{::fast_io::mnp::os_c_str(operand)};
            args[2].type = token_type::arg; end = args + 3u;
        }
        auto const outcome{fn(args, args, end)};
        if(outcome == result::def)
        {
            require(args[1].type == token_type::occupied_arg);
            if(selector != nullptr) { require(args[2].type == token_type::occupied_arg); }
        }
        return outcome;
    }
    void reset_global()
    {
        storage::default_wasip1_env.non_fd_permissions = {};
        storage::wasip1_global_permission_presence = {};
    }
    void reset_target(storage::wasip1_group_state_t& state)
    { state.non_fd_permissions = {}; state.permission_presence = {}; }
    struct named_bit { char8_t const* name; permission bit; };
    constexpr named_bit names[]{
        {u8"realtime-clock", permission::clock_realtime},
        {u8"monotonic-clock", permission::clock_monotonic},
        {u8"process-clock", permission::clock_process_cputime},
        {u8"thread-clock", permission::clock_thread_cputime},
        {u8"random", permission::random}, {u8"proc-raise", permission::proc_raise},
        {u8"sched-yield", permission::sched_yield}, {u8"stdio-full-rights", permission::stdio_full_rights}
    };
}

int main()
{
    require(storage::wasip1_noinherit_system_environment);
    require(!storage::wasip1_noinherit_system_environment_is_set);
    require(storage::default_wasip1_env.non_fd_permissions == permission{});
    require(parameters::wasip1_global_noinherit_system_environment.is_exist == &storage::wasip1_noinherit_system_environment_is_set);
    require(parameters::wasip1_global_inherit_system_environment.is_exist == parameters::wasip1_global_noinherit_system_environment.is_exist);
    require(parameters::wasip1_global_noinherit_system_environment.is_exist != &storage::wasip1_noinherit_system_environment);
    storage::wasip1_env_type fresh{};
    require(!fresh.permits(permission{}));
    require(fresh.non_fd_permissions == permission{});
    require(static_cast<::std::uint64_t>(fresh.stdio_base_rights(true)) == 0x08200002u);
    require(static_cast<::std::uint64_t>(fresh.stdio_base_rights(false)) == 0x08200040u);
    require(fresh.stdio_inheriting_rights() == abi::rights_t{});
    fresh.non_fd_permissions = permission::stdio_full_rights;
    require(fresh.stdio_base_rights(true) == static_cast<abi::rights_t>(-1));
    require(fresh.stdio_base_rights(false) == static_cast<abi::rights_t>(-1));
    require(fresh.stdio_inheriting_rights() == static_cast<abi::rights_t>(-1));

    auto* single{storage::try_create_targetless_wasip1_module_override(u8"restricted")};
    auto const group_index{storage::try_create_named_wasip1_group_index(u8"workers")};
    auto* group{storage::find_wasip1_group_state(group_index)};
    require(single != nullptr && group != nullptr);
    require(single->non_fd_permissions == permission{} && group->non_fd_permissions == permission{});
    for(auto const& item : names)
    {
        reset_global(); reset_target(*single); reset_target(*group);
        require(storage::parse_wasip1_permission(::fast_io::u8string_view{::fast_io::mnp::os_c_str(item.name)}) == item.bit);
        require(invoke(callbacks::wasip1_global_grant_callback, item.name) == result::def);
        require(storage::default_wasip1_env.non_fd_permissions == item.bit);
        require(invoke(callbacks::wasip1_global_grant_callback, item.name) == result::return_m1_imme);
        require(invoke(callbacks::wasip1_global_deny_callback, item.name) == result::return_m1_imme);
        require(invoke(callbacks::wasip1_single_deny_callback, item.name, u8"restricted") == result::def);
        require(single->permission_presence == item.bit && single->non_fd_permissions == permission{});
        require(single->has_override());
        require(storage::effective_wasip1_permissions(storage::default_wasip1_env.non_fd_permissions,
                single->non_fd_permissions, single->permission_presence) == permission{});
        require(invoke(callbacks::wasip1_single_grant_callback, item.name, u8"restricted") == result::return_m1_imme);
        require(invoke(callbacks::wasip1_group_grant_callback, item.name, u8"workers") == result::def);
        require(group->permission_presence == item.bit && group->non_fd_permissions == item.bit);
        require(invoke(callbacks::wasip1_group_deny_callback, item.name, u8"workers") == result::return_m1_imme);
        reset_global();
        require(invoke(callbacks::wasip1_global_deny_callback, item.name) == result::def);
        require(storage::wasip1_global_permission_presence == item.bit && storage::default_wasip1_env.non_fd_permissions == permission{});
    }
    reset_global(); reset_target(*single); reset_target(*group);
    require(invoke(callbacks::wasip1_global_grant_callback, u8"*") == result::return_m1_imme);
    require(invoke(callbacks::wasip1_global_grant_callback, nullptr) == result::return_m1_imme);
    require(storage::wasip1_global_permission_presence == permission{});
    require(invoke(callbacks::wasip1_single_grant_callback, u8"random", u8"absent") == result::return_m1_imme);
    require(invoke(callbacks::wasip1_group_grant_callback, u8"random", u8"absent") == result::return_m1_imme);
    require(invoke(callbacks::wasip1_single_grant_callback, u8"RANDOM", u8"restricted") == result::return_m1_imme);
    require(invoke(callbacks::wasip1_group_deny_callback, u8"*", u8"workers") == result::return_m1_imme);
    require(single->permission_presence == permission{} && group->permission_presence == permission{});

    // Independent bit selection truth table over the real shared merge helper.
    // This runs only after compilation in the required Linux cgroup.
    for(unsigned global{}; global != 256u; ++global)
        for(unsigned target{}; target != 256u; ++target)
            for(unsigned present{}; present != 256u; ++present)
            {
                unsigned expected{};
                for(auto const& item : names)
                {
                    auto const bit{static_cast<unsigned>(item.bit)};
                    if(((present & bit) != 0u ? target : global) & bit) { expected |= bit; }
                }
                require(static_cast<unsigned>(storage::effective_wasip1_permissions(
                    static_cast<permission>(global), static_cast<permission>(target), static_cast<permission>(present))) == expected);
            }
    ::fast_io::io::println("PASS WASIp1 configuration checks=", checks);
}
