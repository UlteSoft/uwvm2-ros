
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
# include <cerrno>
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
// platform
# if (!defined(__NEWLIB__) || defined(__CYGWIN__)) && !defined(_WIN32) && __has_include(<dirent.h>) && !defined(_PICOLIBC__)
#  include <unistd.h>
#  include <errno.h>
#  include <fcntl.h>
#  include <sys/stat.h>
# endif
// import
# include <fast_io.h>
# include <fast_io_device.h>
# include <uwvm2/uwvm_predefine/utils/ansies/impl.h>
# include <uwvm2/uwvm_predefine/io/impl.h>
# include <uwvm2/utils/mutex/impl.h>
# include <uwvm2/utils/debug/impl.h>
# include <uwvm2/utils/control/impl.h>
# include <uwvm2/object/memory/linear/impl.h>
# include <uwvm2/imported/wasi/wasip1/abi/impl.h>
# include <uwvm2/imported/wasi/wasip1/fd_manager/impl.h>
# include <uwvm2/imported/wasi/wasip1/memory/impl.h>
# include <uwvm2/imported/wasi/wasip1/environment/impl.h>
# include "base.h"
# include "posix.h"
# include "path_open_host.h"
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
    /// @brief     WasiPreview1.path_open
    /// @details   __wasi_errno_t path_open(__wasi_fd_t dirfd, __wasi_lookupflags_t dirflags, const char *path, size_t path_len, __wasi_oflags_t oflags,
    /// __wasi_rights_t fs_rights_base, __wasi_rights_t fs_rights_inheriting, __wasi_fdflags_t fdflags, __wasi_fd_t *fd);

    inline constexpr ::uwvm2::imported::wasi::wasip1::abi::errno_t path_open(
        ::uwvm2::imported::wasi::wasip1::environment::wasip1_environment<::uwvm2::object::memory::linear::native_memory_t> & env,
        ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t dirfd,
        ::uwvm2::imported::wasi::wasip1::abi::lookupflags_t dirflags,
        ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path_ptrsz,
        ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len,
        ::uwvm2::imported::wasi::wasip1::abi::oflags_t oflags,
        ::uwvm2::imported::wasi::wasip1::abi::rights_t fs_rights_base,
        ::uwvm2::imported::wasi::wasip1::abi::rights_t fs_rights_inheriting,
        ::uwvm2::imported::wasi::wasip1::abi::fdflags_t fdflags,
        ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t fd_ptrsz) noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(!env.allows_interface(u8"path_open")) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }
#endif

        auto const memory_pointer{env.get_memory()};
        if(memory_pointer == nullptr) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }
        auto& memory{*memory_pointer};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        // Check all guest path ranges before copying either path or invoking native I/O.
        if(!check_wasip1_memory_bounds_wasm32(memory, path_ptrsz, path_len))
        { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }
#endif

#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        // Validate every result buffer before any native side effect or partial
        // output. Memory may grow, but cannot shrink during an active host call.
        if(!check_wasip1_memory_bounds_wasm32(memory, fd_ptrsz, 4uz))
        { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }
#endif


        // path_ptrsz is ConstPointer<u8> (alignment 1).
        if(!check_wasip1_guest_pointer_alignment<4uz>(fd_ptrsz, u8"path_open.fd (fd)"))
        { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::einval; }

        auto const trace_wasip1_call{env.trace_wasip1_call};

        if(trace_wasip1_call) [[unlikely]]
        {
            ::uwvm2::imported::wasi::wasip1::func::print_wasip1_trace_message(
                env,
                u8"path_open(",
                dirfd,
                u8", ",
                static_cast<::std::underlying_type_t<::std::remove_cvref_t<decltype(dirflags)>>>(dirflags),
                u8", ",
                ::fast_io::mnp::addrvw(path_ptrsz),
                u8", ",
                path_len,
                u8", ",
                static_cast<::std::underlying_type_t<::std::remove_cvref_t<decltype(oflags)>>>(oflags),
                u8", ",
                static_cast<::std::underlying_type_t<::std::remove_cvref_t<decltype(fs_rights_base)>>>(fs_rights_base),
                u8", ",
                static_cast<::std::underlying_type_t<::std::remove_cvref_t<decltype(fs_rights_inheriting)>>>(fs_rights_inheriting),
                u8", ",
                static_cast<::std::underlying_type_t<::std::remove_cvref_t<decltype(fdflags)>>>(fdflags),
                u8", ",
                ::fast_io::mnp::addrvw(fd_ptrsz),
                u8")");
        }

        // The negative value fd is invalid, and this check prevents subsequent undefined behavior.
        if(dirfd < 0) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::ebadf; }

        auto& wasm_fd_storage{env.fd_storage};

        // The pointer to `wasm_fd` is fixed and remains unchanged even when the vector within `fd_manager` is resized.
        ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_t* curr_wasi_fd_t_p;  // no initialize

        // storage, This is new file descriptor; no need to lock it.
        ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_unique_ptr_t new_wasi_fd{};
        ::uwvm2::imported::wasi::wasip1::fd_manager::fd_slot_reservation reserved_slot{};
        path_open_parent parent{};


        {
            // Subsequent operations involving the file descriptor require locking. curr_fd_release_guard release when return.
            ::uwvm2::utils::mutex::mutex_merely_release_guard_t curr_fd_release_guard{};

            {
                // Prevent operations to obtain the size or perform resizing at this time.
                // Only a lock is required when acquiring the unique pointer for the file descriptor. The lock can be released once the acquisition is complete.
                // Since the file descriptor's location is fixed and accessed via the unique pointer,

                // Simply acquiring data using a shared_lock
                ::uwvm2::utils::mutex::rw_fair_shared_guard_t fds_lock{wasm_fd_storage.fds_rwlock};

                // Negative states have been excluded, so the conversion result will only be positive numbers.
                using unsigned_fd_t = ::std::make_unsigned_t<::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t>;
                auto const unsigned_dirfd{static_cast<unsigned_fd_t>(dirfd)};

                // On platforms where `size_t` is smaller than the `fd` type, this check must be added.
                constexpr auto size_t_max{::std::numeric_limits<::std::size_t>::max()};
                if constexpr(::std::numeric_limits<unsigned_fd_t>::max() > size_t_max)
                {
                    if(unsigned_dirfd > size_t_max) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::ebadf; }
                }

                auto const dirfd_opens_pos{static_cast<::std::size_t>(unsigned_dirfd)};

                // The minimum value in rename_map is greater than opensize.
                if(wasm_fd_storage.opens.size() <= dirfd_opens_pos)
                {
                    // Possibly within the tree being renumbered
                    if(auto const renumber_map_iter{wasm_fd_storage.renumber_map.find(dirfd)}; renumber_map_iter != wasm_fd_storage.renumber_map.end())
                    {
                        curr_wasi_fd_t_p = renumber_map_iter->second.fd_p;
                    }
                    else [[unlikely]]
                    {
                        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::ebadf;
                    }
                }
                else
                {
                    // The addition here is safe.
                    curr_wasi_fd_t_p = wasm_fd_storage.opens.index_unchecked(dirfd_opens_pos).fd_p;
                }

                // curr_wasi_fd_t_p never nullptr
# if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
                if(curr_wasi_fd_t_p == nullptr) [[unlikely]]
                {
                    // Security issues inherent to virtual machines
                    ::uwvm2::utils::debug::trap_and_inform_bug_pos();
                }
# endif

                // Other threads will definitely lock fds_rwlock when performing close operations (since they need to access the fd vector). If the current
                // thread is performing fdatasync, no other thread can be executing any close operations simultaneously, eliminating any destruction issues.
                // Therefore, acquiring the lock at this point is safe. However, the problem arises when, immediately after acquiring the lock and before
                // releasing the manager lock and beginning fd operations, another thread executes a deletion that removes this fd. Subsequent operations by the
                // current thread would then encounter issues. Thus, locking must occur before releasing fds_rwlock.
                curr_fd_release_guard.device_p = ::std::addressof(curr_wasi_fd_t_p->fd_mutex);
                curr_fd_release_guard.lock();

                // After unlocking fds_lock, members within `wasm_fd_storage_t` can no longer be accessed or modified.
            }

            // curr_fd_uniptr is not null.
            auto& curr_fd{*curr_wasi_fd_t_p};

            // If obtained from the renumber map, it will always be the correct value. If obtained from the open vec, it requires checking whether it is closed.
            // Therefore, a unified check is implemented.
            if(curr_fd.close_pos != SIZE_MAX) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::ebadf; }

            parent.wasi_fd = curr_fd.wasi_fd;
            parent.rights_base = curr_fd.rights_base;
            parent.rights_inherit = curr_fd.rights_inherit;
        }

        // check path (Copy from WASM memory for use)
        ::uwvm2::utils::container::u8string path{};

        if constexpr(::std::numeric_limits<::uwvm2::imported::wasi::wasip1::abi::wasi_size_t>::max() > ::std::numeric_limits<::std::size_t>::max())
        {
            if(path_len > ::std::numeric_limits<::std::size_t>::max()) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eoverflow; }
        }

        path.resize(static_cast<::std::size_t>(path_len));
        auto const declared_shared{[]<typename Memory>(Memory const& value) noexcept
        {
            if constexpr(requires { value.sequentially_consistent_size; }) { return value.sequentially_consistent_size; }
            else { return false; }
        }(memory)};
        {
            [[maybe_unused]] auto const memory_locker_guard{::uwvm2::imported::wasi::wasip1::memory::lock_memory(memory)};
            if(!::uwvm2::imported::wasi::wasip1::memory::copy_guest_bytes_unlocked<false>(
                memory, path_ptrsz, path.data(), path.size(), declared_shared))
            { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }
        }

        // check posixfd buffer
        if(!::uwvm2::imported::wasi::wasip1::func::check_wasip1_memory_bounds_wasm32(memory,
                                                                            fd_ptrsz,
                                                                            sizeof(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t)))
        { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }

        auto const opened{path_open_host(env, parent, dirflags, path, oflags, fs_rights_base,
            fs_rights_inheriting, fdflags, new_wasi_fd, reserved_slot, []() noexcept
            {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                return ::uwvm2::imported::wasi::wasip1::environment::host_wait_cancelled();
#else
                return false;
#endif
            })};
        if(opened != ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess) { return opened; }

        auto const new_fd{reserved_slot.descriptor()};
        ::std::byte encoded_fd[4]{};
        for(unsigned index{}; index != 4u; ++index)
        { encoded_fd[index] = static_cast<::std::byte>(static_cast<::std::uint32_t>(new_fd) >> (index * 8u)); }
        {
            [[maybe_unused]] auto const memory_locker_guard{::uwvm2::imported::wasi::wasip1::memory::lock_memory(memory)};
            if(!::uwvm2::imported::wasi::wasip1::memory::copy_guest_bytes_unlocked<true>(
                memory, fd_ptrsz, encoded_fd, 4u, declared_shared))
            { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::efault; }
        }
        // No result-copy failure can leave an unpublished resource occupying a
        // live descriptor. Publication preserves the fixed node and mutex.
        reserved_slot.commit(*new_wasi_fd.fd_p);
        if(env.debug_fd_created!=nullptr) { env.debug_fd_created(new_fd); }

        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess;
    }
}  // namespace uwvm2::imported::wasi::wasip1::func

#endif

#ifndef UWVM_MODULE
// macro
# include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
# include <uwvm2/uwvm_predefine/utils/ansies/uwvm_color_pop_macro.h>
#endif
