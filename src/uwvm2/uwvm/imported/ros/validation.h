/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
#ifndef UWVM_MODULE
# include <fast_io.h>
# include <fast_io_unit/string.h>
# include <vector>
# if defined(__linux__)
#  include <sys/stat.h>
#  include <sys/socket.h>
#  include <cerrno>
# endif
# include "storage.h"
# include <uwvm2/uwvm/wasm/storage/impl.h>
# include <uwvm2/uwvm/runtime/runtime_mode/impl.h>
# include <uwvm2/runtime/lib/uwvm_runtime.h>
# include <uwvm2/uwvm/imported/wasi/wasip1/storage/impl.h>
#endif
// Imported storage headers restore their temporary capability macros. Establish
// a balanced scope for the guards below regardless of the including header.
# include <uwvm2/utils/macro/push_macros.h>
# ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
#  include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
# endif
namespace uwvm2::uwvm::imported::ros
{
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
# if !defined(__linux__) || \
     !(defined(__x86_64__) || (defined(__aarch64__) && !defined(__AARCH64EB__)) || \
       (defined(__arm__) && defined(__ARM_ARCH) && __ARM_ARCH >= 7 && defined(__ARM_PCS_VFP) && !defined(__ARMEB__)))
#  error "The selected ROS distributions require Linux amd64, little-endian arm64 or ARMv7 hard-float."
# endif
#endif
    inline ::std::vector<::std::string> loaded_module_names()
    {
        ::std::vector<::std::string> names{};
        auto append{[&](auto name)
        {
            if(!name.empty()) { names.emplace_back(reinterpret_cast<char const*>(name.data()), name.size()); }
        }};
        append(::uwvm2::uwvm::wasm::storage::active_execute_wasm().module_name);
        for(auto const& module : ::uwvm2::uwvm::wasm::storage::active_preloaded_wasm()) { append(module.module_name); }
        return names;
    }
    inline bool diagnostic_only() noexcept
    { return check_policy[0] || check_policy[1] || dump_policy[0] || dump_policy[1]; }
    inline ::uwvm2::imported::ros::result validate_execution_backend()
    {
        namespace model = ::uwvm2::imported::ros;
        if(!policy_any_enabled) { return {}; }
        if(!::uwvm2::runtime::lib::ros_execution_backend_supported_host_api())
        { return {model::errc::unsupported, "ROS requires the interpreter or LLVM full compilation with verified native unwind and authenticated native-ABI provenance"}; }
        return {};
    }
    inline ::uwvm2::imported::ros::result validate_loaded_policy(bool executing)
    {
        namespace model = ::uwvm2::imported::ros;
        auto const names{loaded_module_names()};
        if(auto result{runtime_policy.validate(names)}; !result) { return result; }
        if(!executing) { return {}; }
        if(policy_any_enabled)
        {
#if defined(__linux__)
            // WASI standard descriptors are capabilities too. A connected ROS
            // socket inherited as stdin/out/err must not bypass this host layer.
            for(int descriptor{}; descriptor != 3; ++descriptor)
            {
                struct stat state{};
                if(::fstat(descriptor, &state) != 0 && errno != EBADF)
                { return {model::errc::permission_denied, "cannot verify inherited standard descriptor identity"}; }
                if(S_ISSOCK(state.st_mode))
                { return {model::errc::permission_denied, "strict ROS worker rejects socket standard descriptors"}; }
            }
#endif

            if(auto result{validate_execution_backend()}; !result) { return result; }
#if defined(UWVM_SUPPORT_PRELOAD_DL)
            if(!::uwvm2::uwvm::wasm::storage::preloaded_dl.empty())
            { return {model::errc::unsupported, "strict ROS worker cannot admit additional native DL plugins"}; }
#endif
#if defined(UWVM_SUPPORT_WEAK_SYMBOL)
            if(!::uwvm2::uwvm::wasm::storage::weak_symbol.empty())
            { return {model::errc::unsupported, "strict ROS worker cannot admit additional weak-symbol plugins"}; }
#endif
        }
#ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
# if defined(UWVM_IMPORT_WASI_WASIP1)
        namespace wasi = ::uwvm2::uwvm::imported::wasi::wasip1::storage;
        for(auto const& name : names)
        {
            if(!policy_any_enabled) { continue; }
            auto const u8name{::uwvm2::utils::container::u8string_view{reinterpret_cast<char8_t const*>(name.data()), name.size()}};
            auto const kind{name == names.front() ? wasi::wasip1_module_target_kind_t::main_wasm : wasi::wasip1_module_target_kind_t::preload_wasm};
            auto const state{wasi::find_wasip1_module_override_const(kind, u8name)};
            bool const enabled{state && state->enabled_is_set ? state->enabled : ::uwvm2::uwvm::wasm::storage::local_preload_wasip1};
            if(!enabled) { continue; }
            bool const noinherit{state && state->noinherit_system_environment_is_set ? state->noinherit_system_environment : wasi::wasip1_noinherit_system_environment};
            if(!noinherit) { return {model::errc::permission_denied, "ROS worker requires WASI host environment isolation for " + name}; }
            // Existing WASI mount and socket tables compose with global tables.
            // Deny these alternate Guest channels before WASI initialization.
            if(!wasi::default_wasip1_env.mount_dir_roots.empty() || (state && !state->mount_dir_roots.empty()))
            { return {model::errc::permission_denied, "ROS worker rejects WASI directory preopens for " + name}; }
#  if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
            if(!wasi::default_wasip1_env.preopen_sockets.empty() || (state && !state->preopen_sockets.empty()))
            { return {model::errc::permission_denied, "ROS worker rejects WASI socket preopens for " + name}; }
#  endif
        }
# endif
#endif
        return {};
    }
    inline void print_policy_error(::uwvm2::imported::ros::result const& error) noexcept
    {
        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] ROS policy: ",
            ::uwvm2::utils::container::u8string_view{reinterpret_cast<char8_t const*>(error.detail.data()), error.detail.size()});
    }
    inline bool dump_effective_policy()
    {
#if defined(__cpp_exceptions)
        try
#endif
        {
            auto const names{loaded_module_names()};
            for(unsigned i{}; i != 2; ++i)
            {
                if(!dump_policy[i]) { continue; }
                auto const json{runtime_policy.canonical_dump(i == 0 ? ::uwvm2::imported::ros::version::ros1 : ::uwvm2::imported::ros::version::ros2, names)};
                ::fast_io::io::println(::fast_io::out(),json);
            }
            return true; // Native output writes immediately; no stdio flush is pending.
        }
#if defined(__cpp_exceptions)
        catch(...) { return false; }
#endif
    }
}
# ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
#  include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# endif
# include <uwvm2/utils/macro/pop_macros.h>
#endif
