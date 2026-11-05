/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-03-31
 * @copyright   APL-2.0 License
 */

/****************************************
 *  _   _ __        ____     __ __  __  *
 * | | | |\ \      / /\ \   / /|  \/  | *
 * | | | | \ \ /\ / /  \ \ / / | |\/| | *
 * | |_| |  \ V  V /    \ V /  | |  | | *
 *  \___/    \_/\_/      \_/   |_|  |_| *
 *                                      *
 ****************************************/

#pragma once

#ifndef UWVM_MODULE
// std
# include <array>
# include <memory>
# include <cstdint>
// macro
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/uwvm/utils/ansies/uwvm_color_push_macro.h>
// import
# include <fast_io.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/utils/cmdline/impl.h>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params
{
    namespace details
    {
        inline bool management_profile_is_exist{};
        inline bool management_development{};
        inline bool mode_is_exist{};  // [global]
        // An inherited, connected host socket is an explicit launch capability.
        // It is never a guest descriptor or a pathname to be opened later.
        inline int debug_jit_control_fd{-1};
        inline bool debug_jit_control_fd_is_exist{};
        // Windows launcher passes an inherited client HANDLE value. Zero means
        // no late-control grant; guest Wasm never supplies or sees this number.
        inline ::std::uintptr_t debug_jit_control_handle{};
        inline bool debug_jit_control_handle_is_exist{};
        inline constexpr ::uwvm2::utils::container::u8string_view mode_alias{u8"-m"};
        inline constexpr ::uwvm2::utils::container::u8string_view runtime_debug_jit_alias{u8"-Rdbg"};
#if defined(UWVM_MODULE)
        extern "C++"
#else
        inline constexpr
#endif
            ::uwvm2::utils::cmdline::parameter_return_type runtime_debug_jit_callback(
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#if defined(UWVM_MODULE)
        extern "C++"
#else
        inline constexpr
#endif
            ::uwvm2::utils::cmdline::parameter_return_type mode_callback(::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
                                                                         ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#if defined(UWVM_MODULE)
        extern "C++"
#endif
        ::uwvm2::utils::cmdline::parameter_return_type debug_jit_control_fd_callback(
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#if defined(UWVM_MODULE)
        extern "C++"
#endif
        ::uwvm2::utils::cmdline::parameter_return_type debug_jit_control_handle_callback(
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;

#if defined(UWVM_MODULE)
        extern "C++"
#endif
        ::uwvm2::utils::cmdline::parameter_return_type management_profile_callback(
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;

    }  // namespace details

#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wbraced-scalar-init"
#endif
    namespace details
    {
        inline int update_control_fd{-1}, diagnostic_control_fd{-1};
        inline bool diagnostic_control_fd_exists{}, release_diagnostic_requested{};
        inline ::uwvm2::utils::container::u8string_view update_call_export{};
        inline bool update_call_export_exists{};
        inline ::std::array<unsigned char, 32> update_public_key{}, update_contract{}, update_scope{};
        inline bool update_control_fd_exists{}, update_public_key_exists{}, update_contract_exists{}, update_scope_exists{};
#if defined(UWVM_MODULE)
        extern "C++"
#else
        inline
#endif
        ::uwvm2::utils::cmdline::parameter_return_type update_option_callback(
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter diagnostic_control_fd{
        .name{u8"--diagnostic-control-fd"}, .describe{u8"RELEASE-only bounded read-only diagnostics on a separate authenticated Unix seqpacket socket."},
        .usage{u8"<fd>"}, .handle{::std::addressof(details::update_option_callback)},
        .is_exist{::std::addressof(details::diagnostic_control_fd_exists)}, .cate{::uwvm2::utils::cmdline::categorization::debug}};
    inline constexpr ::uwvm2::utils::cmdline::parameter update_control_fd{
        .name{u8"--update-control-fd"}, .describe{u8"Production patch manager on an authenticated inherited Unix stream; separate from debugging."},
        .usage{u8"<fd>"}, .handle{::std::addressof(details::update_option_callback)},
        .is_exist{::std::addressof(details::update_control_fd_exists)}, .cate{::uwvm2::utils::cmdline::categorization::update}};
    inline constexpr ::uwvm2::utils::cmdline::parameter update_public_key{
        .name{u8"--update-public-key"}, .describe{u8"Trusted local deployment Ed25519 public key (32 bytes, hexadecimal)."},
        .usage{u8"<hex>"}, .handle{::std::addressof(details::update_option_callback)},
        .is_exist{::std::addressof(details::update_public_key_exists)}, .cate{::uwvm2::utils::cmdline::categorization::update}};
    inline constexpr ::uwvm2::utils::cmdline::parameter update_contract{
        .name{u8"--update-contract"}, .describe{u8"Initial approved permission contract reference; changes require a separately signed policy transition."},
        .usage{u8"<hex>"}, .handle{::std::addressof(details::update_option_callback)},
        .is_exist{::std::addressof(details::update_contract_exists)}, .cate{::uwvm2::utils::cmdline::categorization::update}};
    inline constexpr ::uwvm2::utils::cmdline::parameter update_call_export{
        .name{u8"--update-call-export"}, .describe{u8"Permit one named business export on the update channel; scalar ABI only, disabled by default."},
        .usage{u8"<name>"}, .handle{::std::addressof(details::update_option_callback)},
        .is_exist{::std::addressof(details::update_call_export_exists)}, .cate{::uwvm2::utils::cmdline::categorization::update}};
    inline constexpr ::uwvm2::utils::cmdline::parameter update_scope{
        .name{u8"--update-scope"}, .describe{u8"Stable deployment/component/device scope SHA-256 assigned by the trusted launcher."},
        .usage{u8"<hex>"}, .handle{::std::addressof(details::update_option_callback)},
        .is_exist{::std::addressof(details::update_scope_exists)}, .cate{::uwvm2::utils::cmdline::categorization::update}};
    inline constexpr ::uwvm2::utils::cmdline::parameter management_profile{
        .name{u8"--management-profile"},
        .describe{u8"Trusted launch profile: release (default, diagnostics only) or explicit dev Guest debugging. Does not grant Guest imports."},
        .usage{u8"<release|dev>"},
        .handle{::std::addressof(details::management_profile_callback)},
        .is_exist{::std::addressof(details::management_profile_is_exist)},
        .cate{::uwvm2::utils::cmdline::categorization::debug}};
    inline constexpr ::uwvm2::utils::cmdline::parameter mode{.name{u8"--mode"},
                                                             .describe{u8"Select the operation mode. Default: run."},
                                                             .usage{u8"[section-details|validation|run|debug]"},
                                                             .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(details::mode_alias), 1uz}},
                                                             .handle{::std::addressof(details::mode_callback)},
                                                             .is_exist{::std::addressof(details::mode_is_exist)}};
    // Share --mode's duplicate-selection flag: argument order must not silently
    // turn an explicit debugger launch back into a normal run. Startup applies
    // the same LLVM/full defaults and incompatible-backend fatal as -m debug.
    inline constexpr ::uwvm2::utils::cmdline::parameter runtime_debug_jit{
        .name{u8"--runtime-debug"},
        .describe{u8"Shortcut for -m debug; enter the built-in LLVM JIT full debugger."},
        .alias{::uwvm2::utils::cmdline::kns_u8_str_scatter_t{::std::addressof(details::runtime_debug_jit_alias), 1uz}},
        .handle{::std::addressof(details::runtime_debug_jit_callback)},
        .is_exist{::std::addressof(details::mode_is_exist)},
        .cate{::uwvm2::utils::cmdline::categorization::runtime}};
    inline constexpr ::uwvm2::utils::cmdline::parameter debug_jit_control_fd{
        .name{u8"--debug-jit-control-fd"},
        .describe{u8"Opt in to late LLVM-full debugger commands from a trusted inherited Linux packet or macOS framed Unix endpoint."},
        .usage{u8"<connected-fd>"},
        .handle{::std::addressof(details::debug_jit_control_fd_callback)},
        .is_exist{::std::addressof(details::debug_jit_control_fd_is_exist)},
        .cate{::uwvm2::utils::cmdline::categorization::debug}};
    inline constexpr ::uwvm2::utils::cmdline::parameter debug_jit_control_handle{
        .name{u8"--debug-jit-control-handle"},
        .describe{u8"Windows only: opt in to LLVM-full late debugger commands from a direct-parent inherited private pipe HANDLE."},
        .usage{u8"<connected-decimal-handle>"},
        .handle{::std::addressof(details::debug_jit_control_handle_callback)},
        .is_exist{::std::addressof(details::debug_jit_control_handle_is_exist)},
        .cate{::uwvm2::utils::cmdline::categorization::debug}};
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
}

#ifndef UWVM_MODULE
// macro
# include <uwvm2/uwvm/utils/ansies/uwvm_color_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
