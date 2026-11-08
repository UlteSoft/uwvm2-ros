
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
# if defined(__linux__) || defined(__FreeBSD__) || (defined(__APPLE__) && defined(__MACH__))
    // The resolver constructs only this explicit WASI disposition mode set.
    // Keep the sealed-input path independent of FastIO's unexported converter;
    // never let write rights add implicit create/truncate disposition.
    [[nodiscard]] inline constexpr int path_open_explicit_posix_flags(::fast_io::open_mode mode) noexcept
    {
        int flags{};
#  ifdef O_NOFOLLOW
        flags |= O_NOFOLLOW;
#  endif
#  ifdef O_CLOEXEC
        flags |= O_CLOEXEC;
#  elif defined(_O_NOINHERIT)
        flags |= _O_NOINHERIT;
#  endif
#  ifdef O_BINARY
        flags |= O_BINARY;
#  endif
#  ifdef O_LARGEFILE
        flags |= O_LARGEFILE;
#  endif
        if((mode & ::fast_io::open_mode::creat) != ::fast_io::open_mode::none) { flags |= O_CREAT; }
        if((mode & ::fast_io::open_mode::excl) != ::fast_io::open_mode::none) { flags |= O_EXCL; }
        if((mode & ::fast_io::open_mode::trunc) != ::fast_io::open_mode::none) { flags |= O_TRUNC; }
        if((mode & ::fast_io::open_mode::directory) != ::fast_io::open_mode::none)
        {
#  ifdef O_DIRECTORY
            flags |= O_DIRECTORY;
#  else
            return {};
#  endif
        }
        if((mode & ::fast_io::open_mode::no_block) != ::fast_io::open_mode::none)
        {
#  ifdef O_NONBLOCK
            flags |= O_NONBLOCK;
#  else
            return {};
#  endif
        }
#  ifdef O_SYNC
        if((mode & ::fast_io::open_mode::sync) != ::fast_io::open_mode::none) { flags |= O_SYNC; }
#  endif
#  ifdef O_DSYNC
        if((mode & ::fast_io::open_mode::dsync) != ::fast_io::open_mode::none) { flags |= O_DSYNC; }
#  endif
#  ifdef O_RSYNC
        if((mode & ::fast_io::open_mode::rsync) != ::fast_io::open_mode::none) { flags |= O_RSYNC; }
#  endif
        auto const read{(mode & ::fast_io::open_mode::in) != ::fast_io::open_mode::none};
        auto const write{(mode & (::fast_io::open_mode::out | ::fast_io::open_mode::app)) != ::fast_io::open_mode::none};
        if((mode & ::fast_io::open_mode::app) != ::fast_io::open_mode::none) { flags |= O_APPEND; }
        return flags | (write ? (read ? O_RDWR : O_WRONLY) : O_RDONLY);
    }
# endif

    // Snapshot only under the real descriptor lock; its retained resource owns
    // the immutable directory chain after close/renumber releases that binding.
    struct path_open_parent
    {
        ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_ref_t wasi_fd{
            ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_no_construct};
        ::uwvm2::imported::wasi::wasip1::abi::rights_t rights_base{}, rights_inherit{};
    };

    // Common host-only resolver for ordinary32/64 and actual shared calls. Guest
    // memory transport and descriptor publication remain in the admitted caller.
    template<typename Environment, typename Cancel>
    inline ::uwvm2::imported::wasi::wasip1::abi::errno_t path_open_host(
        Environment& env, path_open_parent const& parent,
        ::uwvm2::imported::wasi::wasip1::abi::lookupflags_t dirflags,
        ::uwvm2::utils::container::u8string const& path,
        ::uwvm2::imported::wasi::wasip1::abi::oflags_t oflags,
        ::uwvm2::imported::wasi::wasip1::abi::rights_t fs_rights_base,
        ::uwvm2::imported::wasi::wasip1::abi::rights_t fs_rights_inheriting,
        ::uwvm2::imported::wasi::wasip1::abi::fdflags_t fdflags,
        ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_unique_ptr_t& new_wasi_fd,
        ::uwvm2::imported::wasi::wasip1::fd_manager::fd_slot_reservation& reserved_slot,
        Cancel const& cancelled) noexcept
    {
        auto const& curr_fd{parent};
        auto& wasm_fd_storage{env.fd_storage};
        // Final-file creation only: ordinary paths retain their existing native
        // open and make no additional OS calls. Debug startup publishes a sealed
        // host input identity before any guest can enter this function.
        auto const open_guest_native_file{[](auto const& directory, auto const& name, ::fast_io::open_mode mode)
        {
# if defined(__linux__) || defined(__FreeBSD__) || (defined(__APPLE__) && defined(__MACH__))
            if(::uwvm2::utils::control::console_input_sealed()) [[unlikely]]
            {
                auto const flags{path_open_explicit_posix_flags(mode)};
                // name is a validated owning host path component, including its
                // terminal zero; this view cannot outlive the synchronous call.
                // Darwin must use the same pre-truncation descriptor identity check:
                // guest path_open cannot reopen or overwrite the host's debug input.
                auto const opened{::uwvm2::utils::control::open_guest_path_sealed_host_api(
                    directory.native_handle(), reinterpret_cast<char const*>(name.c_str()), flags, 436u)};
                if(opened.descriptor == -1) [[unlikely]] { ::fast_io::throw_posix_error(opened.error); }
                // Exclusive fd ownership transfers to native_file exactly once.
                return ::fast_io::native_file{opened.descriptor};
            }
# endif
# if defined(_WIN32) && !defined(__CYGWIN__)
            if(::uwvm2::utils::control::console_input_sealed()) [[unlikely]]
            {
                // A retained debugger descriptor must not lock the filename.
                // Windows needs delete sharing for rename/unlink while open;
                // live path observations still query this same kernel handle.
                auto opened{::fast_io::native_file{at(directory), name, mode | ::fast_io::open_mode::shared_delete}};
                // The protected Windows command input is a pipe or console
                // buffer, never a disk file. Inspect the actual opened kernel
                // object so reparse points and device aliases cannot bypass
                // a lexical path check; no guest handle has been published.
                if(::uwvm2::utils::control::inspect_guest_native_handle_host_api(opened.native_handle()) !=
                   ::uwvm2::utils::control::sealed_input_decision::allow)
                {
#  ifdef UWVM_CPP_EXCEPTIONS
                    throw ::fast_io::error{::fast_io::win32_domain_value, 5uz /*ERROR_ACCESS_DENIED*/};
#  else
                    ::fast_io::fast_terminate();
#  endif
                }
                return opened;
            }
# endif
            return ::fast_io::native_file{at(directory), name, mode};
        }};

        if((curr_fd.rights_base & ::uwvm2::imported::wasi::wasip1::abi::rights_t::right_path_open) !=
           ::uwvm2::imported::wasi::wasip1::abi::rights_t::right_path_open) [[unlikely]]
        {
            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable;
        }

        // If ptr is null, it indicates an attempt to open a closed file. However, the preceding check for close pos already prevents such closed files from
        // being processed, making this a virtual machine implementation error.
        if(curr_fd.wasi_fd.ptr == nullptr) [[unlikely]]
        {
// This will be checked at runtime.
# if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
            ::uwvm2::utils::debug::trap_and_inform_bug_pos();
# endif
            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio;
        }

        switch(curr_fd.wasi_fd.ptr->wasi_fd_storage.type)
        {
            [[unlikely]] case ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::null:
            {
                return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio;
            }
            case ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::file: [[fallthrough]];
            case ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::file_observer:
            {
                return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir;
            }
            [[likely]] case ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::dir:
            {
                break;
            }
# if defined(_WIN32) && !defined(__CYGWIN__)
            case ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::socket: [[fallthrough]];
            case ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::socket_observer:
            {
                return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir;
            }
# endif
            [[unlikely]] default:
            {
# if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
                ::uwvm2::utils::debug::trap_and_inform_bug_pos();
# endif
                return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio;
            }
        }

        if((static_cast<::std::uint32_t>(dirflags) & ~1u) != 0u ||
           (static_cast<::std::uint32_t>(oflags) & ~15u) != 0u ||
           (static_cast<::std::uint32_t>(fdflags) & ~31u) != 0u)
        { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::einval; }

        // Retrieve the current directory, which is the top element of the directory stack.
        auto const& curr_dir_stack{curr_fd.wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
        if(curr_dir_stack.empty()) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio; }

        auto const& curr_dir_stack_entry{curr_dir_stack.dir_stack.back_unchecked()};
        if(curr_dir_stack_entry.ptr == nullptr) [[unlikely]]
        {
// This will be checked at runtime.
# if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
            ::uwvm2::utils::debug::trap_and_inform_bug_pos();
# endif
            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio;
        }

        ::fast_io::dir_io_observer curr_dir_io_observer{};

        bool const is_observer{curr_dir_stack_entry.ptr->dir_stack.is_observer};

        if(is_observer)
        {
            auto& curr_dir_io_observer_ref{curr_dir_stack_entry.ptr->dir_stack.storage.observer};
            curr_dir_io_observer = curr_dir_io_observer_ref;
        }
        else
        {
            auto& curr_dir_file_ref{curr_dir_stack_entry.ptr->dir_stack.storage.file};
            curr_dir_io_observer = curr_dir_file_ref;
        }

        auto const& curr_fd_native_file{curr_dir_io_observer};

        bool const symlink_follow{(dirflags & ::uwvm2::imported::wasi::wasip1::abi::lookupflags_t::lookup_symlink_follow) ==
                                  ::uwvm2::imported::wasi::wasip1::abi::lookupflags_t::lookup_symlink_follow};

        // Bound metadata before allocating a second copy of guest paths.
        ::uwvm2::imported::wasi::wasip1::fd_manager::record_checkpoint_path(*new_wasi_fd.fd_p->wasi_fd.ptr,
            curr_dir_stack.dir_stack,::uwvm2::utils::container::u8string_view{path.data(),path.size()},symlink_follow);
        if(path.empty()) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::einval; }

        // WASI does not guarantee that strings are null-terminated, so you must check for zero characters in the middle and construct one yourself.
        if(!env.disable_utf8_check) [[likely]]
        {
            auto const u8res{
                ::uwvm2::utils::utf::check_legal_utf8<::uwvm2::utils::utf::utf8_specification::utf8_rfc3629_and_zero_illegal>(path.cbegin(), path.cend())};
            if(u8res.err != ::uwvm2::utils::utf::utf_error_code::success) [[unlikely]]
            {
                // If the path string is not valid UTF-8, the function shall fail with ERRNO_ILSEQ.
                return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eilseq;
            }
        }
        else
        {
            auto const u8res{::uwvm2::utils::utf::check_has_zero_illegal_unchecked(path.cbegin(), path.cend())};
            if(u8res.err != ::uwvm2::utils::utf::utf_error_code::success) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eilseq; }
        }

        auto const split_path_res{
            ::uwvm2::imported::wasi::wasip1::func::split_posix_path(::uwvm2::utils::container::u8string_view{path.data(), path.size()})};

        // The path series functions in wasip1 reject absolute paths.
        if(split_path_res.is_absolute) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }
        if(split_path_res.res.empty()) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::einval; }

# if (defined(_WIN32) || defined(__CYGWIN__)) || (defined(__MSDOS__) || defined(__DJGPP__))
        // For the Windows API, the parsing strategy differs from POSIX. Windows supports the backslash as a delimiter while rejecting many characters.
        // This only eliminates the possibility of multi-level paths; the remaining issue of invalid filenames is handled by the API.
        for(auto const& split_curr: split_path_res.res)
        {
            if(split_curr.dir_type == ::uwvm2::imported::wasi::wasip1::func::dir_type_e::next)
            {
                auto const& next_name{split_curr.next_name};
                for(auto const curr_char: next_name)
                {
                    // Simultaneously eliminate errors at both the path syntax layer and the file system syntax layer.
                    if(::fast_io::char_category::is_dos_file_invalid_character(curr_char)) [[unlikely]]
                    {
                        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::einval;
                    }
                }
            }
        }
# endif

        // Parent base rights authorize path operations. Both rights sets
        // installed on the child are bounded by the parent's inheriting rights.
        if((fs_rights_base & ~curr_fd.rights_inherit) != ::uwvm2::imported::wasi::wasip1::abi::rights_t{})
        {
            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable;
        }

        // The new inheriting permissions must be a subset of the old inheriting permissions.
        if((fs_rights_inheriting & ~curr_fd.rights_inherit) != ::uwvm2::imported::wasi::wasip1::abi::rights_t{})
        {
            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable;
        }

        // storage, This is new file descriptor; no need to lock it.
        new_wasi_fd.fd_p->rights_base = fs_rights_base;
        new_wasi_fd.fd_p->rights_inherit = fs_rights_inheriting;

        // oflags
        auto const is_creat{(oflags & ::uwvm2::imported::wasi::wasip1::abi::oflags_t::o_creat) == ::uwvm2::imported::wasi::wasip1::abi::oflags_t::o_creat};
        auto const is_dir{(oflags & ::uwvm2::imported::wasi::wasip1::abi::oflags_t::o_directory) ==
                          ::uwvm2::imported::wasi::wasip1::abi::oflags_t::o_directory};
        auto const is_excl{(oflags & ::uwvm2::imported::wasi::wasip1::abi::oflags_t::o_excl) == ::uwvm2::imported::wasi::wasip1::abi::oflags_t::o_excl};
        auto const is_trunc{(oflags & ::uwvm2::imported::wasi::wasip1::abi::oflags_t::o_trunc) == ::uwvm2::imported::wasi::wasip1::abi::oflags_t::o_trunc};

        auto const is_append{(fdflags & ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_append) ==
                             ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_append};
        auto const is_dsync{(fdflags & ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_dsync) ==
                            ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_dsync};
        auto const is_nonblock{(fdflags & ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_nonblock) ==
                               ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_nonblock};
        auto const is_rsync{(fdflags & ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_rsync) ==
                            ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_rsync};
        auto const is_sync{(fdflags & ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_sync) ==
                           ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_sync};

        auto const is_read{(fs_rights_base & ::uwvm2::imported::wasi::wasip1::abi::rights_t::right_fd_read) ==
                           ::uwvm2::imported::wasi::wasip1::abi::rights_t::right_fd_read};
        auto const is_write{(fs_rights_base & ::uwvm2::imported::wasi::wasip1::abi::rights_t::right_fd_write) ==
                            ::uwvm2::imported::wasi::wasip1::abi::rights_t::right_fd_write};

        // Truncation is a path operation authorized by the parent's
        // PATH_FILESTAT_SET_SIZE below. It does not require or add FD_WRITE
        // authority on the returned descriptor.
        if(is_dir)
        {
            if(is_trunc || is_append || is_excl || is_creat) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::einval; }
        }
        if(is_excl && !is_creat) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::einval; }

        // Check every requested operation before opening a native object.
        // A child FD's write rights do not grant its parent create/truncate rights.
        using operation_rights = ::uwvm2::imported::wasi::wasip1::abi::rights_t;
        auto required = operation_rights::right_path_open;
        if(is_creat) { required |= operation_rights::right_path_create_file; }
        if(is_trunc) { required |= operation_rights::right_path_filestat_set_size; }
        if(is_sync || is_rsync) { required |= operation_rights::right_fd_sync; }
        if((required & ~curr_fd.rights_base) != operation_rights{} ||
           (is_dsync && (curr_fd.rights_base & (operation_rights::right_fd_datasync | operation_rights::right_fd_sync)) == operation_rights{}))
        { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

# if defined(_WIN32) && !defined(__CYGWIN__)
        // Win32 directory handles cannot preserve NONBLOCK flags.
        if(is_dir && is_nonblock) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotsup; }
        // The Win32 provider cannot preserve WASI SYNC semantics/flags.
        if(is_sync) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotsup; }
# endif

#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        auto const& retained_parent{curr_fd.wasi_fd};
        bool const filesystem_operation{retained_parent.ptr->checkpoint_stdio_index < 0};
        ::uwvm2::imported::wasi::wasip1::environment::budget_reservation filesystem_reservation{
            ::uwvm2::imported::wasi::wasip1::environment::budget_kind::filesystem_io, env.limits, 0u, filesystem_operation};
        if(filesystem_operation && !filesystem_reservation) { return filesystem_reservation.template error<::uwvm2::imported::wasi::wasip1::abi::errno_t>(); }
#endif

        auto const reservation_error{reserved_slot.reserve(wasm_fd_storage, cancelled)};
        if(reservation_error != ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess) { return reservation_error; }
        if(cancelled()) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::ecanceled; }

        // WASI access rights never imply O_CREAT/O_TRUNC (or their NT equivalents).
        ::fast_io::open_mode fast_io_oflags{::fast_io::open_mode::explicit_disposition};
        if(is_creat) { fast_io_oflags |= ::fast_io::open_mode::creat; }
        if(is_dir) { fast_io_oflags |= ::fast_io::open_mode::directory; }
        if(is_excl) { fast_io_oflags |= ::fast_io::open_mode::excl; }
        if(is_trunc) { fast_io_oflags |= ::fast_io::open_mode::trunc; }

        if(is_append) { fast_io_oflags |= ::fast_io::open_mode::app; }
        if(is_nonblock) { fast_io_oflags |= ::fast_io::open_mode::no_block; }
        if(is_sync) { fast_io_oflags |= ::fast_io::open_mode::sync; }
        if(is_dsync)
        {
# ifdef O_DSYNC
            fast_io_oflags |= ::fast_io::open_mode::dsync;
# else
            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotsup;
# endif
        }
        if(is_rsync)
        {
# ifdef O_RSYNC
            fast_io_oflags |= ::fast_io::open_mode::rsync;
# else
            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotsup;
# endif
        }

        if(is_read) { fast_io_oflags |= ::fast_io::open_mode::in; }
        // Native write access performs only the explicitly authorized
        // open-time truncation. Child rights remain exactly as requested.
        if(is_write || is_trunc) { fast_io_oflags |= ::fast_io::open_mode::out; }

        if(!is_read && !is_write && !is_dir && !is_trunc) { fast_io_oflags |= ::fast_io::open_mode::in; }

        // fast_io no fsync and dsync

        if(is_dir) { new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.reset_type(::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::dir); }
        else
        {
            new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.reset_type(::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::file);
# if defined(_WIN32) && !defined(__CYGWIN__)
            // Since Win32 cannot retrieve flags after creation, they must be recorded simultaneously.
            // Exclude representations not supported by Windows
            constexpr auto and_flags{~(::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_rsync |
                                       ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_dsync |
                                       ::uwvm2::imported::wasi::wasip1::abi::fdflags_t::fdflag_sync)};
            new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.file_fd.fdflags = fdflags & and_flags;
# endif
        }

        // path stack
        ::uwvm2::utils::container::vector<::uwvm2::imported::wasi::wasip1::func::dir_with_name_t> path_stack{};
        path_stack.reserve(split_path_res.res.size());

        // cend cannot be nullptr
        auto const split_last{split_path_res.res.cend() - 1u};

        for(auto split_curr{split_path_res.res.begin()}; split_curr != split_path_res.res.end(); ++split_curr)
        {
            if(split_curr == split_last)
            {
                // Create the final path

                switch(split_curr->dir_type)
                {
                    case ::uwvm2::imported::wasi::wasip1::func::dir_type_e::curr:
                    {
                        if(is_dir)
                        {
                            auto& storage_dir_stack{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                            storage_dir_stack = curr_dir_stack;

                            for(auto& dir_with_name_curr: path_stack)
                            {
                                // Create a new `dir_file` by moving an existing one to ensure no touch-and-touch-out issues.
                                auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                newdir.ptr->dir_stack.name = ::std::move(dir_with_name_curr.name);
                                newdir.ptr->dir_stack.storage.file = ::std::move(dir_with_name_curr.file);
                            }
                        }
                        else [[unlikely]]
                        {
                            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eisdir;
                        }

                        auto& selected_directory{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                        auto const open_error{::uwvm2::imported::wasi::wasip1::func::path_open_independent_directory(selected_directory, fast_io_oflags)};
                        if(open_error != ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess) [[unlikely]] { return open_error; }

                        break;
                    }
                    case ::uwvm2::imported::wasi::wasip1::func::dir_type_e::prev:
                    {
                        if(is_dir)
                        {
                            if(path_stack.empty()) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }
                            else
                            {
                                auto& storage_dir_stack{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                                storage_dir_stack = curr_dir_stack;

                                auto const path_stack_last{path_stack.cend() - 1u};

                                for(auto dir_with_name_curr{path_stack.cbegin()}; dir_with_name_curr != path_stack_last; ++dir_with_name_curr)
                                {
                                    // Create a new `dir_file` by moving an existing one to ensure no touch-and-touch-out issues.
                                    auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                    newdir.ptr->dir_stack.name = ::std::move(dir_with_name_curr->name);
                                    newdir.ptr->dir_stack.storage.file = ::std::move(dir_with_name_curr->file);
                                }
                            }
                        }
                        else [[unlikely]]
                        {
                            return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eisdir;
                        }

                        auto& selected_directory{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                        auto const open_error{::uwvm2::imported::wasi::wasip1::func::path_open_independent_directory(selected_directory, fast_io_oflags)};
                        if(open_error != ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess) [[unlikely]] { return open_error; }

                        break;
                    }
                    case ::uwvm2::imported::wasi::wasip1::func::dir_type_e::next:
                    {
                        // detect symlink
                        auto& next_name{split_curr->next_name};

                        ::uwvm2::utils::container::u8string symlink_symbol{};
                        bool is_symlink{};

                        if(path_stack.empty())
                        {
# ifdef UWVM_CPP_EXCEPTIONS
                            try
# endif
                            {
                                // readlinkat is symlink_nofollow
                                symlink_symbol = ::fast_io::native_readlinkat<char8_t>(at(curr_fd_native_file), next_name);
                                is_symlink = true;
                            }
# ifdef UWVM_CPP_EXCEPTIONS
                            catch(::fast_io::error e)
                            {
                            }
# endif

                            if(is_symlink)
                            {
                                if(!symlink_follow)
                                {
                                    if(is_dir) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                    else
                                    {
                                        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eloop;
                                    }
                                }
                                else
                                {
                                    auto [file_symlink_iterative_err,
                                          file_symlink_iterative_file_name]{::uwvm2::imported::wasi::wasip1::func::file_symlink_iterative_with_name(
                                        env.disable_utf8_check,
                                        curr_fd_native_file,
                                        path_stack,
                                        ::uwvm2::utils::container::u8string_view{symlink_symbol.data(), symlink_symbol.size()})};
                                    if(file_symlink_iterative_err != ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess) [[unlikely]]
                                    {
                                        return file_symlink_iterative_err;
                                    }

                                    // file_symlink_iterative_file_name is not symlink
                                    // path_stack maybe empty
                                    auto& open_file_name{file_symlink_iterative_file_name};

                                    if(path_stack.empty())
                                    {
                                        if(is_dir)
                                        {
                                            ::fast_io::dir_file new_dir_file{};

# ifdef UWVM_CPP_EXCEPTIONS
                                            try
# endif
                                            {
                                                // dir_file default nofollow
                                                new_dir_file = ::uwvm2::imported::wasi::wasip1::func::open_authorized_directory(at(curr_fd_native_file), open_file_name, fast_io_oflags);

# if defined(__CYGWIN__)
                                                // Cygwin uses the Win32 CreateFile function internally to open directories, allowing both directories and
                                                // files to be opened simultaneously. A check must be added here.
                                                struct ::stat st;
                                                ::uwvm2::imported::wasi::wasip1::func::posix::fstat(new_dir_file.native_handle(), ::std::addressof(st));
                                                if(!S_ISDIR(st.st_mode)) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
# elif defined(__MSDOS__) || defined(__DJGPP__)
                                                // djgpp's `open` function does not distinguish between directories and files; manual differentiation is
                                                // required.
                                                ::fast_io::details::check_dos_fd_is_dir(new_dir_file.native_handle());
# endif
                                            }
# ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                                // Windows 9x can only distinguish between a directory and other items (files or nothing at all).

#  if defined(_WIN32) && defined(_WIN32_WINDOWS)
                                                if(e.code == 2uz /*ERROR_FILE_NOT_FOUND*/) [[unlikely]]
                                                {
                                                    bool is_file{};
#   ifdef UWVM_CPP_EXCEPTIONS
                                                    try
#   endif
                                                    {
                                                        // native_file default nofollow
                                                        ::fast_io::native_file{at(curr_fd_native_file), open_file_name, ::fast_io::open_mode::in};
                                                        is_file = true;
                                                    }
#   ifdef UWVM_CPP_EXCEPTIONS
                                                    catch(::fast_io::error e)
                                                    {
                                                    }
#   endif
                                                    if(is_file) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                                }
#  endif

                                                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                            }
# endif

                                            auto& storage_dir_stack{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                                            storage_dir_stack = curr_dir_stack;

                                            for(auto& dir_with_name_curr: path_stack)
                                            {
                                                // Create a new `dir_file` by moving an existing one to ensure no touch-and-touch-out issues.
                                                auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                                newdir.ptr->dir_stack.name = ::std::move(dir_with_name_curr.name);
                                                newdir.ptr->dir_stack.storage.file = ::std::move(dir_with_name_curr.file);
                                            }

                                            auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                            newdir.ptr->dir_stack.name = ::std::move(open_file_name);
                                            newdir.ptr->dir_stack.storage.file = ::std::move(new_dir_file);
                                        }
                                        else
                                        {
# ifdef UWVM_CPP_EXCEPTIONS
                                            try
# endif
                                            {
                                                // native_file default nofollow
                                                new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage
                                                    .file_fd
# if defined(_WIN32) && !defined(__CYGWIN__)
                                                    .file
# endif
                                                    = open_guest_native_file(curr_fd_native_file, open_file_name, fast_io_oflags);
                                            }
# ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                            }
# endif
                                        }
                                    }
                                    else
                                    {
                                        if(is_dir)
                                        {
                                            ::fast_io::dir_file new_dir_file{};

# ifdef UWVM_CPP_EXCEPTIONS
                                            try
# endif
                                            {
                                                // dir_file default nofollow
                                                new_dir_file = ::uwvm2::imported::wasi::wasip1::func::open_authorized_directory(at(path_stack.back_unchecked().file), open_file_name, fast_io_oflags);

# if defined(__CYGWIN__)
                                                // Cygwin uses the Win32 CreateFile function internally to open directories, allowing both directories and
                                                // files to be opened simultaneously. A check must be added here.
                                                struct ::stat st;
                                                ::uwvm2::imported::wasi::wasip1::func::posix::fstat(new_dir_file.native_handle(), ::std::addressof(st));
                                                if(!S_ISDIR(st.st_mode)) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
# elif defined(__MSDOS__) || defined(__DJGPP__)
                                                // djgpp's `open` function does not distinguish between directories and files; manual differentiation is
                                                // required.
                                                ::fast_io::details::check_dos_fd_is_dir(new_dir_file.native_handle());
# endif
                                            }
# ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                                // Windows 9x can only distinguish between a directory and other items (files or nothing at all).

#  if defined(_WIN32) && defined(_WIN32_WINDOWS)
                                                if(e.code == 2uz /*ERROR_FILE_NOT_FOUND*/) [[unlikely]]
                                                {
                                                    bool is_file{};
#   ifdef UWVM_CPP_EXCEPTIONS
                                                    try
#   endif
                                                    {
                                                        // native_file default nofollow
                                                        ::fast_io::native_file{at(path_stack.back_unchecked().file),
                                                                               open_file_name,
                                                                               ::fast_io::open_mode::in};
                                                        is_file = true;
                                                    }
#   ifdef UWVM_CPP_EXCEPTIONS
                                                    catch(::fast_io::error e)
                                                    {
                                                    }
#   endif
                                                    if(is_file) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                                }
#  endif

                                                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                            }
# endif

                                            auto& storage_dir_stack{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                                            storage_dir_stack = curr_dir_stack;

                                            for(auto& dir_with_name_curr: path_stack)
                                            {
                                                // Create a new `dir_file` by moving an existing one to ensure no touch-and-touch-out issues.
                                                auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                                newdir.ptr->dir_stack.name = ::std::move(dir_with_name_curr.name);
                                                newdir.ptr->dir_stack.storage.file = ::std::move(dir_with_name_curr.file);
                                            }

                                            auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                            newdir.ptr->dir_stack.name = ::std::move(open_file_name);
                                            newdir.ptr->dir_stack.storage.file = ::std::move(new_dir_file);
                                        }
                                        else
                                        {
# ifdef UWVM_CPP_EXCEPTIONS
                                            try
# endif
                                            {
                                                // native_file default nofollow
                                                new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage
                                                    .file_fd
# if defined(_WIN32) && !defined(__CYGWIN__)
                                                    .file
# endif
                                                    = open_guest_native_file(path_stack.back_unchecked().file, open_file_name, fast_io_oflags);
                                            }
# ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                            }
# endif
                                        }
                                    }
                                }
                            }
                            else
                            {
                                auto& open_file_name{next_name};

                                if(is_dir)
                                {
                                    ::fast_io::dir_file new_dir_file{};

# ifdef UWVM_CPP_EXCEPTIONS
                                    try
# endif
                                    {
                                        // dir_file default nofollow
                                        new_dir_file = ::uwvm2::imported::wasi::wasip1::func::open_authorized_directory(at(curr_fd_native_file), open_file_name, fast_io_oflags);

# if defined(__CYGWIN__)
                                        // Cygwin uses the Win32 CreateFile function internally to open directories, allowing both directories and files
                                        // to be opened simultaneously. A check must be added here.
                                        struct ::stat st;
                                        ::uwvm2::imported::wasi::wasip1::func::posix::fstat(new_dir_file.native_handle(), ::std::addressof(st));
                                        if(!S_ISDIR(st.st_mode)) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
# elif defined(__MSDOS__) || defined(__DJGPP__)
                                        // djgpp's `open` function does not distinguish between directories and files; manual differentiation is
                                        // required.
                                        ::fast_io::details::check_dos_fd_is_dir(new_dir_file.native_handle());
# endif
                                    }
# ifdef UWVM_CPP_EXCEPTIONS
                                    catch(::fast_io::error e)
                                    {
                                        // Windows 9x can only distinguish between a directory and other items (files or nothing at all).

#  if defined(_WIN32) && defined(_WIN32_WINDOWS)
                                        if(e.code == 2uz /*ERROR_FILE_NOT_FOUND*/) [[unlikely]]
                                        {
                                            bool is_file{};
#   ifdef UWVM_CPP_EXCEPTIONS
                                            try
#   endif
                                            {
                                                // native_file default nofollow
                                                ::fast_io::native_file{at(curr_fd_native_file), open_file_name, ::fast_io::open_mode::in};
                                                is_file = true;
                                            }
#   ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                            }
#   endif
                                            if(is_file) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                        }
#  endif

                                        return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                    }
# endif

                                    auto& storage_dir_stack{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                                    storage_dir_stack = curr_dir_stack;

                                    for(auto& dir_with_name_curr: path_stack)
                                    {
                                        // Create a new `dir_file` by moving an existing one to ensure no touch-and-touch-out issues.
                                        auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                        newdir.ptr->dir_stack.name = ::std::move(dir_with_name_curr.name);
                                        newdir.ptr->dir_stack.storage.file = ::std::move(dir_with_name_curr.file);
                                    }

                                    auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                    newdir.ptr->dir_stack.name = ::std::move(open_file_name);
                                    newdir.ptr->dir_stack.storage.file = ::std::move(new_dir_file);
                                }
                                else
                                {
# ifdef UWVM_CPP_EXCEPTIONS
                                    try
# endif
                                    {
                                        // native_file default nofollow
                                        new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage
                                            .file_fd
# if defined(_WIN32) && !defined(__CYGWIN__)
                                            .file
# endif
                                            = open_guest_native_file(curr_fd_native_file, open_file_name, fast_io_oflags);
                                    }
# ifdef UWVM_CPP_EXCEPTIONS
                                    catch(::fast_io::error e)
                                    {
                                        return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                    }
# endif
                                }
                            }
                        }
                        else
                        {
# ifdef UWVM_CPP_EXCEPTIONS
                            try
# endif
                            {
                                // readlinkat is symlink_nofollow
                                symlink_symbol = ::fast_io::native_readlinkat<char8_t>(at(path_stack.back_unchecked().file), next_name);
                                is_symlink = true;
                            }
# ifdef UWVM_CPP_EXCEPTIONS
                            catch(::fast_io::error e)
                            {
                            }
# endif

                            if(is_symlink)
                            {
                                if(!symlink_follow)
                                {
                                    if(is_dir) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                    else
                                    {
                                        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eloop;
                                    }
                                }
                                else
                                {
                                    auto [file_symlink_iterative_err,
                                          file_symlink_iterative_file_name]{::uwvm2::imported::wasi::wasip1::func::file_symlink_iterative_with_name(
                                        env.disable_utf8_check,
                                        curr_fd_native_file,
                                        path_stack,
                                        ::uwvm2::utils::container::u8string_view{symlink_symbol.data(), symlink_symbol.size()})};
                                    if(file_symlink_iterative_err != ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess) [[unlikely]]
                                    {
                                        return file_symlink_iterative_err;
                                    }

                                    // file_symlink_iterative_file_name is not symlink
                                    // path_stack maybe empty
                                    auto& open_file_name{file_symlink_iterative_file_name};

                                    if(path_stack.empty())
                                    {
                                        if(is_dir)
                                        {
                                            ::fast_io::dir_file new_dir_file{};

# ifdef UWVM_CPP_EXCEPTIONS
                                            try
# endif
                                            {
                                                // dir_file default nofollow
                                                new_dir_file = ::uwvm2::imported::wasi::wasip1::func::open_authorized_directory(at(curr_fd_native_file), open_file_name, fast_io_oflags);

# if defined(__CYGWIN__)
                                                // Cygwin uses the Win32 CreateFile function internally to open directories, allowing both directories and
                                                // files to be opened simultaneously. A check must be added here.
                                                struct ::stat st;
                                                ::uwvm2::imported::wasi::wasip1::func::posix::fstat(new_dir_file.native_handle(), ::std::addressof(st));
                                                if(!S_ISDIR(st.st_mode)) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
# elif defined(__MSDOS__) || defined(__DJGPP__)
                                                // djgpp's `open` function does not distinguish between directories and files; manual differentiation is
                                                // required.
                                                ::fast_io::details::check_dos_fd_is_dir(new_dir_file.native_handle());
# endif
                                            }
# ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                                // Windows 9x can only distinguish between a directory and other items (files or nothing at all).

#  if defined(_WIN32) && defined(_WIN32_WINDOWS)
                                                if(e.code == 2uz /*ERROR_FILE_NOT_FOUND*/) [[unlikely]]
                                                {
                                                    bool is_file{};
#   ifdef UWVM_CPP_EXCEPTIONS
                                                    try
#   endif
                                                    {
                                                        // native_file default nofollow
                                                        ::fast_io::native_file{at(curr_fd_native_file), open_file_name, ::fast_io::open_mode::in};
                                                        is_file = true;
                                                    }
#   ifdef UWVM_CPP_EXCEPTIONS
                                                    catch(::fast_io::error e)
                                                    {
                                                    }
#   endif
                                                    if(is_file) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                                }
#  endif

                                                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                            }
# endif

                                            auto& storage_dir_stack{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                                            storage_dir_stack = curr_dir_stack;

                                            for(auto& dir_with_name_curr: path_stack)
                                            {
                                                // Create a new `dir_file` by moving an existing one to ensure no touch-and-touch-out issues.
                                                auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                                newdir.ptr->dir_stack.name = ::std::move(dir_with_name_curr.name);
                                                newdir.ptr->dir_stack.storage.file = ::std::move(dir_with_name_curr.file);
                                            }

                                            auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                            newdir.ptr->dir_stack.name = ::std::move(open_file_name);
                                            newdir.ptr->dir_stack.storage.file = ::std::move(new_dir_file);
                                        }
                                        else
                                        {
# ifdef UWVM_CPP_EXCEPTIONS
                                            try
# endif
                                            {
                                                // native_file default nofollow
                                                new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage
                                                    .file_fd
# if defined(_WIN32) && !defined(__CYGWIN__)
                                                    .file
# endif
                                                    = open_guest_native_file(curr_fd_native_file, open_file_name, fast_io_oflags);
                                            }
# ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                            }
# endif
                                        }
                                    }
                                    else
                                    {
                                        if(is_dir)
                                        {
                                            ::fast_io::dir_file new_dir_file{};

# ifdef UWVM_CPP_EXCEPTIONS
                                            try
# endif
                                            {
                                                // dir_file default nofollow
                                                new_dir_file = ::uwvm2::imported::wasi::wasip1::func::open_authorized_directory(at(path_stack.back_unchecked().file), open_file_name, fast_io_oflags);

# if defined(__CYGWIN__)
                                                // Cygwin uses the Win32 CreateFile function internally to open directories, allowing both directories and
                                                // files to be opened simultaneously. A check must be added here.
                                                struct ::stat st;
                                                ::uwvm2::imported::wasi::wasip1::func::posix::fstat(new_dir_file.native_handle(), ::std::addressof(st));
                                                if(!S_ISDIR(st.st_mode)) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
# elif defined(__MSDOS__) || defined(__DJGPP__)
                                                // djgpp's `open` function does not distinguish between directories and files; manual differentiation is
                                                // required.
                                                ::fast_io::details::check_dos_fd_is_dir(new_dir_file.native_handle());
# endif
                                            }
# ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                                // Windows 9x can only distinguish between a directory and other items (files or nothing at all).

#  if defined(_WIN32) && defined(_WIN32_WINDOWS)
                                                if(e.code == 2uz /*ERROR_FILE_NOT_FOUND*/) [[unlikely]]
                                                {
                                                    bool is_file{};
#   ifdef UWVM_CPP_EXCEPTIONS
                                                    try
#   endif
                                                    {
                                                        // native_file default nofollow
                                                        ::fast_io::native_file{at(path_stack.back_unchecked().file),
                                                                               open_file_name,
                                                                               ::fast_io::open_mode::in};
                                                        is_file = true;
                                                    }
#   ifdef UWVM_CPP_EXCEPTIONS
                                                    catch(::fast_io::error e)
                                                    {
                                                    }
#   endif
                                                    if(is_file) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                                }
#  endif

                                                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                            }
# endif

                                            auto& storage_dir_stack{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                                            storage_dir_stack = curr_dir_stack;

                                            for(auto& dir_with_name_curr: path_stack)
                                            {
                                                // Create a new `dir_file` by moving an existing one to ensure no touch-and-touch-out issues.
                                                auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                                newdir.ptr->dir_stack.name = ::std::move(dir_with_name_curr.name);
                                                newdir.ptr->dir_stack.storage.file = ::std::move(dir_with_name_curr.file);
                                            }

                                            auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                            newdir.ptr->dir_stack.name = ::std::move(open_file_name);
                                            newdir.ptr->dir_stack.storage.file = ::std::move(new_dir_file);
                                        }
                                        else
                                        {
# ifdef UWVM_CPP_EXCEPTIONS
                                            try
# endif
                                            {
                                                // native_file default nofollow
                                                new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage
                                                    .file_fd
# if defined(_WIN32) && !defined(__CYGWIN__)
                                                    .file
# endif
                                                    = open_guest_native_file(path_stack.back_unchecked().file, open_file_name, fast_io_oflags);
                                            }
# ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                                return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                            }
# endif
                                        }
                                    }
                                }
                            }
                            else
                            {
                                auto& open_file_name{next_name};

                                if(is_dir)
                                {
                                    ::fast_io::dir_file new_dir_file{};

# ifdef UWVM_CPP_EXCEPTIONS
                                    try
# endif
                                    {
                                        // dir_file default nofollow
                                        new_dir_file = ::uwvm2::imported::wasi::wasip1::func::open_authorized_directory(at(path_stack.back_unchecked().file), open_file_name, fast_io_oflags);

# if defined(__CYGWIN__)
                                        // Cygwin uses the Win32 CreateFile function internally to open directories, allowing both directories and files
                                        // to be opened simultaneously. A check must be added here.
                                        struct ::stat st;
                                        ::uwvm2::imported::wasi::wasip1::func::posix::fstat(new_dir_file.native_handle(), ::std::addressof(st));
                                        if(!S_ISDIR(st.st_mode)) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
# elif defined(__MSDOS__) || defined(__DJGPP__)
                                        // djgpp's `open` function does not distinguish between directories and files; manual differentiation is
                                        // required.
                                        ::fast_io::details::check_dos_fd_is_dir(new_dir_file.native_handle());
# endif
                                    }
# ifdef UWVM_CPP_EXCEPTIONS
                                    catch(::fast_io::error e)
                                    {
                                        // Windows 9x can only distinguish between a directory and other items (files or nothing at all).

#  if defined(_WIN32) && defined(_WIN32_WINDOWS)
                                        if(e.code == 2uz /*ERROR_FILE_NOT_FOUND*/) [[unlikely]]
                                        {
                                            bool is_file{};
#   ifdef UWVM_CPP_EXCEPTIONS
                                            try
#   endif
                                            {
                                                // native_file default nofollow
                                                ::fast_io::native_file{at(path_stack.back_unchecked().file), open_file_name, ::fast_io::open_mode::in};
                                                is_file = true;
                                            }
#   ifdef UWVM_CPP_EXCEPTIONS
                                            catch(::fast_io::error e)
                                            {
                                            }
#   endif
                                            if(is_file) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                        }
#  endif

                                        return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                    }
# endif

                                    auto& storage_dir_stack{new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage.dir_stack};
                                    storage_dir_stack = curr_dir_stack;

                                    for(auto& dir_with_name_curr: path_stack)
                                    {
                                        // Create a new `dir_file` by moving an existing one to ensure no touch-and-touch-out issues.
                                        auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                        newdir.ptr->dir_stack.name = ::std::move(dir_with_name_curr.name);
                                        newdir.ptr->dir_stack.storage.file = ::std::move(dir_with_name_curr.file);
                                    }

                                    auto& newdir{storage_dir_stack.dir_stack.emplace_back()};
                                    newdir.ptr->dir_stack.name = ::std::move(open_file_name);
                                    newdir.ptr->dir_stack.storage.file = ::std::move(new_dir_file);
                                }
                                else
                                {
# ifdef UWVM_CPP_EXCEPTIONS
                                    try
# endif
                                    {
                                        // native_file default nofollow
                                        new_wasi_fd.fd_p->wasi_fd.ptr->wasi_fd_storage.storage
                                            .file_fd
# if defined(_WIN32) && !defined(__CYGWIN__)
                                            .file
# endif
                                            = open_guest_native_file(path_stack.back_unchecked().file, open_file_name, fast_io_oflags);
                                    }
# ifdef UWVM_CPP_EXCEPTIONS
                                    catch(::fast_io::error e)
                                    {
                                        return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                    }
# endif
                                }
                            }
                        }

                        break;
                    }
                    [[unlikely]] default:
                    {
# if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
                        ::uwvm2::utils::debug::trap_and_inform_bug_pos();
# endif
                        ::std::unreachable();
                    }
                }
            }
            else
            {
                // The path traversed in the middle

                switch(split_curr->dir_type)
                {
                    case ::uwvm2::imported::wasi::wasip1::func::dir_type_e::curr:
                    {
                        break;
                    }
                    case ::uwvm2::imported::wasi::wasip1::func::dir_type_e::prev:
                    {
                        if(path_stack.empty()) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }
                        else
                        {
                            path_stack.pop_back_unchecked();
                        }

                        break;
                    }
                    case ::uwvm2::imported::wasi::wasip1::func::dir_type_e::next:
                    {
                        auto& next_name{split_curr->next_name};

                        ::uwvm2::utils::container::u8string symlink_symbol{};
                        bool is_symlink{};

                        if(path_stack.empty())
                        {
# ifdef UWVM_CPP_EXCEPTIONS
                            try
# endif
                            {
                                // readlinkat is symlink_nofollow
                                symlink_symbol = ::fast_io::native_readlinkat<char8_t>(at(curr_fd_native_file), next_name);
                                is_symlink = true;
                            }
# ifdef UWVM_CPP_EXCEPTIONS
                            catch(::fast_io::error e)
                            {
                            }
# endif

                            if(is_symlink)
                            {
                                if(symlink_symbol.empty()) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio; }

                                auto const errno_t{::uwvm2::imported::wasi::wasip1::func::path_symlink_iterative_with_name(
                                    env.disable_utf8_check,
                                    curr_fd_native_file,
                                    path_stack,
                                    ::uwvm2::utils::container::u8string_view{symlink_symbol.data(), symlink_symbol.size()})};
                                if(errno_t != ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess) [[unlikely]] { return errno_t; }
                            }
                            else
                            {
                                ::fast_io::dir_file next{};
# ifdef UWVM_CPP_EXCEPTIONS
                                try
# endif
                                {
                                    // default is symlink_nofollow
                                    next = ::uwvm2::imported::wasi::wasip1::func::open_authorized_directory(at(curr_fd_native_file), next_name);

                                    // The `Win32 CreateFileW` function can simultaneously open both a directory and a file when specifying a
                                    // directory. However, under the current fast_io strategy, `dir_file` does not utilize `CreateFileW`.
                                    // Instead, it employs `NTCreateFile` on NT systems and `FindNextFile` on Win9x systems.

# if defined(__CYGWIN__)
                                    // Cygwin uses the Win32 CreateFile function internally to open directories, allowing both directories and files to be
                                    // opened simultaneously. A check must be added here.
                                    struct ::stat st;
                                    if(::uwvm2::imported::wasi::wasip1::func::posix::fstat(next.native_handle(), ::std::addressof(st)) < 0) [[unlikely]]
                                    {
                                        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio;
                                    }
                                    if(!S_ISDIR(st.st_mode)) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
# elif defined(__MSDOS__) || defined(__DJGPP__)
                                    // djgpp's `open` function does not distinguish between directories and files; manual differentiation is
                                    // required.
                                    ::fast_io::details::check_dos_fd_is_dir(next.native_handle());
# endif
                                }
# ifdef UWVM_CPP_EXCEPTIONS
                                catch(::fast_io::error e)
                                {
                                    // Windows 9x can only distinguish between a directory and other items (files or nothing at all).

#  if defined(_WIN32) && defined(_WIN32_WINDOWS)
                                    if(e.code == 2uz /*ERROR_FILE_NOT_FOUND*/) [[unlikely]]
                                    {
                                        bool is_file{};
#   ifdef UWVM_CPP_EXCEPTIONS
                                        try
#   endif
                                        {
                                            // native_file default nofollow
                                            ::fast_io::native_file{at(curr_fd_native_file), next_name, ::fast_io::open_mode::in};
                                            is_file = true;
                                        }
#   ifdef UWVM_CPP_EXCEPTIONS
                                        catch(::fast_io::error e)
                                        {
                                        }
#   endif
                                        if(is_file) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                    }
#  endif

                                    return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                }
# endif

                                path_stack.emplace_back(::std::move(next_name), ::std::move(next));
                            }
                        }
                        else
                        {
# ifdef UWVM_CPP_EXCEPTIONS
                            try
# endif
                            {
                                // readlinkat is symlink_nofollow
                                symlink_symbol = ::fast_io::native_readlinkat<char8_t>(at(path_stack.back_unchecked().file), next_name);
                                is_symlink = true;
                            }
# ifdef UWVM_CPP_EXCEPTIONS
                            catch(::fast_io::error e)
                            {
                            }
# endif

                            if(is_symlink)
                            {
                                if(symlink_symbol.empty()) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio; }

                                auto const errno_t{::uwvm2::imported::wasi::wasip1::func::path_symlink_iterative_with_name(
                                    env.disable_utf8_check,
                                    curr_fd_native_file,
                                    path_stack,
                                    ::uwvm2::utils::container::u8string_view{symlink_symbol.data(), symlink_symbol.size()})};
                                if(errno_t != ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess) [[unlikely]] { return errno_t; }
                            }
                            else
                            {
                                ::fast_io::dir_file next{};
# ifdef UWVM_CPP_EXCEPTIONS
                                try
# endif
                                {
                                    // default is symlink_nofollow
                                    next = ::uwvm2::imported::wasi::wasip1::func::open_authorized_directory(at(path_stack.back_unchecked().file), next_name);

                                    // The `Win32 CreateFileW` function can simultaneously open both a directory and a file when specifying a
                                    // directory. However, under the current fast_io strategy, `dir_file` does not utilize `CreateFileW`.
                                    // Instead, it employs `NTCreateFile` on NT systems and `FindNextFile` on Win9x systems.

# if defined(__CYGWIN__)
                                    // Cygwin uses the Win32 CreateFile function internally to open directories, allowing both directories and files to be
                                    // opened simultaneously. A check must be added here.
                                    struct ::stat st;
                                    if(::uwvm2::imported::wasi::wasip1::func::posix::fstat(next.native_handle(), ::std::addressof(st)) < 0) [[unlikely]]
                                    {
                                        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::eio;
                                    }
                                    if(!S_ISDIR(st.st_mode)) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
# elif defined(__MSDOS__) || defined(__DJGPP__)
                                    // djgpp's `open` function does not distinguish between directories and files; manual differentiation is
                                    // required.
                                    ::fast_io::details::check_dos_fd_is_dir(next.native_handle());
# endif
                                }
# ifdef UWVM_CPP_EXCEPTIONS
                                catch(::fast_io::error e)
                                {
                                    // Windows 9x can only distinguish between a directory and other items (files or nothing at all).

#  if defined(_WIN32) && defined(_WIN32_WINDOWS)
                                    if(e.code == 2uz /*ERROR_FILE_NOT_FOUND*/) [[unlikely]]
                                    {
                                        bool is_file{};
#   ifdef UWVM_CPP_EXCEPTIONS
                                        try
#   endif
                                        {
                                            // native_file default nofollow
                                            ::fast_io::native_file{at(path_stack.back_unchecked().file), next_name, ::fast_io::open_mode::in};
                                            is_file = true;
                                        }
#   ifdef UWVM_CPP_EXCEPTIONS
                                        catch(::fast_io::error e)
                                        {
                                        }
#   endif
                                        if(is_file) { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotdir; }
                                    }
#  endif

                                    return ::uwvm2::imported::wasi::wasip1::func::path_errno_from_fast_io_error(e);
                                }
# endif

                                path_stack.emplace_back(::std::move(next_name), ::std::move(next));
                            }
                        }

                        break;
                    }
                    [[unlikely]] default:
                    {
# if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
                        ::uwvm2::utils::debug::trap_and_inform_bug_pos();
# endif
                        ::std::unreachable();
                    }
                }
            }
        }

        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess;
    }
}  // namespace uwvm2::imported::wasi::wasip1::func

#endif

#ifndef UWVM_MODULE
# include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
# include <uwvm2/uwvm_predefine/utils/ansies/uwvm_color_pop_macro.h>
#endif
