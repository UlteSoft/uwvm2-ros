/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
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
# include <cstddef>
# include <cstdint>
# include <climits>
# include <cstring>
# include <limits>
# include <concepts>
# include <bit>
# include <memory>
# include <type_traits>
# include <utility>
// macro
# include <uwvm2/uwvm_predefine/utils/ansies/uwvm_color_push_macro.h>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
// import
# include <fast_io.h>
# include <fast_io_device.h>
# if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(__linux__)
#  include <sys/random.h>
# endif
# include <uwvm2/uwvm_predefine/utils/ansies/impl.h>
# include <uwvm2/uwvm_predefine/io/impl.h>
# include <uwvm2/utils/mutex/impl.h>
# include <uwvm2/utils/debug/impl.h>
# include <uwvm2/object/memory/linear/impl.h>
# include <uwvm2/imported/wasi/wasip1/abi/impl.h>
# include <uwvm2/imported/wasi/wasip1/fd_manager/impl.h>
# include <uwvm2/imported/wasi/wasip1/memory/impl.h>
# include <uwvm2/imported/wasi/wasip1/environment/impl.h>
# include "random_get.h"
# include "base.h"
# include "posix.h"
#endif

#ifndef UWVM_CPP_EXCEPTIONS
# if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wcpp"
# elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wcpp"
# endif
# warning "Without enabling C++ exceptions, using this WASI function may cause termination."
# if defined(__clang__)
#  pragma clang diagnostic pop
# elif defined(__GNUC__)
#  pragma GCC diagnostic pop
# endif
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

#if defined(UWVM_IMPORT_WASI_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1_WASM64)

UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::func
{
    /// @brief     WasiPreview1.random_get_wasm64
    /// @details   __wasi_errno_t random_get_wasm64(void *buf, size_t buf_len);

    inline constexpr ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t random_get_wasm64(
        ::uwvm2::imported::wasi::wasip1::environment::wasip1_environment<::uwvm2::object::memory::linear::native_memory_t> & env,
        ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t buf,
        ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t buf_len) noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(!env.allows_interface(u8"random_get_wasm64")) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }
#endif

        auto const trace_wasip1_call{env.trace_wasip1_call};

        if(trace_wasip1_call) [[unlikely]]
        {
            ::uwvm2::imported::wasi::wasip1::func::print_wasip1_trace_message(env, u8"random_get_wasm64(", ::fast_io::mnp::addrvw(buf), u8", ", buf_len, u8")");
        }

        // Even an empty request requires explicit randomness authority. Denial must
        // precede constructing native_white_hole and must leave guest bytes untouched.
        if(!env.permits(::uwvm2::imported::wasi::wasip1::environment::permission_t::random)) [[unlikely]]
        {
            return ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t::enotcapable;
        }

#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation reservation{
            ::uwvm2::imported::wasi::wasip1::environment::budget_kind::random, env.limits, static_cast<::std::uint64_t>(buf_len)};
        if(!reservation) { return reservation.template error<::uwvm2::imported::wasi::wasip1::abi::errno_t>(); }
#endif
        auto const memory_pointer{env.get_memory()};
        if(memory_pointer == nullptr) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }
        auto& memory{*memory_pointer};
        if(memory.memtag_shadow.enabled() && memory.memtag_shadow.shared())
        { return random_get_shared_tagged(memory,buf,buf_len); }


        {
            // Full locking is required during reading.
            [[maybe_unused]] auto const memory_locker_guard{::uwvm2::imported::wasi::wasip1::memory::lock_memory(memory)};

            if(!::uwvm2::imported::wasi::wasip1::func::check_wasip1_memory_bounds_wasm64_unlocked(memory, buf, buf_len))
            { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }

            auto const output_span{::uwvm2::imported::wasi::wasip1::memory::checked_span_unlocked(memory,buf,buf_len)};
            if(!output_span) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }
            auto const write_memory_begin{output_span.data};
            auto const write_memory_end{write_memory_begin + buf_len};

# ifdef UWVM_CPP_EXCEPTIONS
            try
# endif
            {
                // The construction of the native_white_hole component may throw exceptions on certain platforms. Additionally, even if interrupted mid-read, it
                // the native source may return a partial result before reporting an error.

                // If an error occurs at the underlying layer, the buffer is not guaranteed to retain its state prior to the call.
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(__linux__)
                auto* cursor{write_memory_begin};
                unsigned interrupted{};
                while(cursor != write_memory_end)
                {
                    auto& ledger{::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger};
                    if(::uwvm2::imported::wasi::wasip1::environment::host_wait_cancelled()) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::ecanceled; }
                    ledger.native_random_calls.fetch_add(1u, ::std::memory_order_relaxed);
                    auto const n{::getrandom(cursor, static_cast<::std::size_t>(write_memory_end-cursor), GRND_NONBLOCK)};
                    if(n > 0) { cursor += n; continue; }
                    if(n < 0 && errno == EINTR && ++interrupted < 8u) { continue; }
                    return n < 0 && (errno == EAGAIN || errno == EINTR) ? ::uwvm2::imported::wasi::wasip1::abi::errno_t::eagain :
                        ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio;
                }
#else
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                ::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.native_random_calls.fetch_add(1u, ::std::memory_order_relaxed);
#endif
                ::fast_io::operations::read_all_bytes(::fast_io::native_white_hole{},
                                                      reinterpret_cast<::std::byte*>(write_memory_begin),
                                                      reinterpret_cast<::std::byte*>(write_memory_end));
#endif
            }
# ifdef UWVM_CPP_EXCEPTIONS
            catch(::fast_io::error e)
            {
                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
            }
# endif
        }

        return ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t::esuccess;
    }
}  // namespace uwvm2::imported::wasi::wasip1::func

#endif

#ifndef UWVM_MODULE
// macro
# include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
# include <uwvm2/uwvm_predefine/utils/ansies/uwvm_color_pop_macro.h>
#endif

