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
# include <new>
# include <type_traits>
# include <utility>
// macro
# include <uwvm2/uwvm_predefine/utils/ansies/uwvm_color_push_macro.h>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
// import
# include <fast_io.h>
# include <uwvm2/uwvm_predefine/utils/ansies/impl.h>
# include <uwvm2/uwvm_predefine/io/impl.h>
# include <uwvm2/utils/mutex/impl.h>
# include <uwvm2/utils/debug/impl.h>
# include <uwvm2/object/memory/linear/impl.h>
# include <uwvm2/imported/wasi/wasip1/abi/impl.h>
# include <uwvm2/imported/wasi/wasip1/fd_manager/impl.h>
# include <uwvm2/imported/wasi/wasip1/memory/impl.h>
# include <uwvm2/imported/wasi/wasip1/environment/impl.h>
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

#ifdef UWVM_IMPORT_WASI_WASIP1

UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::func
{
    /// @brief     WasiPreview1.fd_close
    /// @details   __wasi_errno_t fd_close(__wasi_fd_t fd);
    /// @note      Close a file descriptor.

    // Host-only descriptor transaction shared by ordinary and admitted threaded
    // calls. The caller selects cancellable acquisition of the actual FD mutex.
    template<typename Environment, typename Cancel, typename Acquire>
    inline ::uwvm2::imported::wasi::wasip1::abi::errno_t fd_close_host(
        Environment& env, ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
        Cancel const& cancelled, Acquire const& acquire_fd) noexcept
    {
        using error = ::uwvm2::imported::wasi::wasip1::abi::errno_t;
        namespace descriptors = ::uwvm2::imported::wasi::wasip1::fd_manager;
        if(fd < 0) { return error::ebadf; }
        if constexpr((::std::numeric_limits<::std::size_t>::max)() < (::std::numeric_limits<::std::make_unsigned_t<decltype(fd)>>::max)())
        { if(static_cast<::std::make_unsigned_t<decltype(fd)>>(fd) > (::std::numeric_limits<::std::size_t>::max)()) { return error::ebadf; } }
        auto& storage{env.fd_storage};
        descriptors::wasi_fd_unique_ptr_t retired_cell{descriptors::wasi_no_construct};
        descriptors::wasi_fd_ref_t retired_resource{descriptors::wasi_no_construct};
        {
            descriptors::fd_table_write_guard writer{};
            if(!writer.acquire(storage.fds_rwlock, cancelled)) { return error::ecanceled; }
            auto const index{static_cast<::std::size_t>(fd)};
            descriptors::wasi_fd_t* selected{};
            auto sparse{storage.renumber_map.end()};
            bool const dense{index < storage.opens.size()};
            if(dense) { selected = storage.opens.index_unchecked(index).fd_p; }
            else
            {
                sparse = storage.renumber_map.find(fd);
                if(sparse == storage.renumber_map.end()) { return error::ebadf; }
                selected = sparse->second.fd_p;
            }
            if(selected == nullptr) { return error::eio; }
            auto const acquired{acquire_fd(*selected)};
            if(acquired != error::esuccess) { return acquired; }
            ::uwvm2::utils::mutex::mutex_merely_release_guard_t held{::std::addressof(selected->fd_mutex)};
            if(cancelled()) { return error::ecanceled; }
            if(selected->close_pos != SIZE_MAX) { return error::ebadf; }
            if(selected->wasi_fd.ptr == nullptr) { return error::eio; }
            if(dense)
            {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                // Standard streams stay reserved and are never allocator holes.
                if(index < 3u) { selected->close_pos = SIZE_MAX - 1u; }
                else
#endif
                {
                    selected->close_pos = storage.closes.size();
                    storage.closes.emplace_back(index);
                }
            }
            selected->rights_base = {}; selected->rights_inherit = {};
            retired_resource = ::std::move(selected->wasi_fd);
            if(!dense)
            {
                retired_cell.fd_p = sparse->second.release();
                storage.renumber_map.erase(sparse);
            }
        }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        ::uwvm2::imported::wasi::wasip1::environment::process_budget_ledger.notify_waiters();
#endif
        // Both locks are gone before a native close or node/mutex destruction.
        retired_resource.reset();
        return error::esuccess;
    }

    inline constexpr ::uwvm2::imported::wasi::wasip1::abi::errno_t fd_close_base(
        ::uwvm2::imported::wasi::wasip1::environment::wasip1_environment<::uwvm2::object::memory::linear::native_memory_t>& env,
        ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd) noexcept
    {
        return fd_close_host(env, fd, []() noexcept
        {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            return ::uwvm2::imported::wasi::wasip1::environment::host_wait_cancelled();
#else
            return false;
#endif
        }, [](auto& selected) noexcept
        {
            selected.fd_mutex.lock();
            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess;
        });
    }

    inline constexpr ::uwvm2::imported::wasi::wasip1::abi::errno_t fd_close(
        ::uwvm2::imported::wasi::wasip1::environment::wasip1_environment<::uwvm2::object::memory::linear::native_memory_t> & env,
        ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd) noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(!env.allows_interface(u8"fd_close")) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }
#endif

        auto const trace_wasip1_call{env.trace_wasip1_call};

        if(trace_wasip1_call) [[unlikely]] { ::uwvm2::imported::wasi::wasip1::func::print_wasip1_trace_message(env, u8"fd_close(", fd, u8")"); }

        return fd_close_base(env, fd);
    }
}  // namespace uwvm2::imported::wasi::wasip1::func

#endif

#ifndef UWVM_MODULE
// macro
# include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
# include <uwvm2/uwvm_predefine/utils/ansies/uwvm_color_pop_macro.h>
#endif

