// SPDX-License-Identifier: Apache-2.0
#pragma once
#ifndef UWVM_MODULE
#include <cstdint>
#include <memory>
#include <uwvm2/utils/container/impl.h>
#include <uwvm2/utils/cmdline/impl.h>
#include <uwvm2/runtime/lib/uwvm_runtime_threaded_profile_api.h>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params
{
#if UWVM2_HAS_THREADED_PROFILE_HOST_API
namespace details
{
    struct wasi_thread_signal_policy
    {
        ::uwvm2::runtime::lib::threaded_signal_policy channel{};
        ::std::uint32_t grace_ms{},retire_ms{};
        [[nodiscard]] bool selected() const noexcept{return channel.read_fd!=0u;}
    };
    // CLI-thread DATA only; immutable after complete actual parsing. Zero
    // installs no handler, map, Source, clock or ROS authority.
    inline wasi_thread_signal_policy wasi_thread_signal_config{};
    inline bool wasi_thread_signals_existed{};
    #if defined(UWVM_MODULE)
        extern "C++"
#else
        inline
#endif
        void wasi_thread_signals_pretreatment(char8_t const* const*&,char8_t const* const*,
        ::uwvm2::utils::container::vector<::uwvm2::utils::cmdline::parameter_parsing_results>&) noexcept;
    #if defined(UWVM_MODULE)
        extern "C++"
#else
        inline
#endif
        ::uwvm2::utils::cmdline::parameter_return_type wasi_thread_signals_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
}
inline constexpr ::uwvm2::utils::cmdline::parameter wasi_thread_signals{
    .name{u8"--wasi-thread-signals"},
    .describe{u8"Explicit Linux CLI SIGINT/SIGTERM delivery to the Source-owned WASI pipe; separately requires --wasi-threads."},
    .usage{u8"<read-fd> <cookie> <max-frames> <max-pipe-bytes> <grace-ms> <retire-ms>"},
    .handle = ::std::addressof(details::wasi_thread_signals_callback),
    .pretreatment = ::std::addressof(details::wasi_thread_signals_pretreatment),
    .is_exist = ::std::addressof(details::wasi_thread_signals_existed),
    .cate = ::uwvm2::utils::cmdline::categorization::runtime};
#endif
}
