
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
#include <uwvm2/utils/control/host_interfaces.h>

#ifndef UWVM_MODULE
// std
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <climits>
# include <version>
# include <limits>
# include <concepts>
# include <bit>
// macro
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
// import
# include <fast_io.h>
# include <fast_io_device.h>
# include <uwvm2/utils/debug/impl.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/type/impl.h>
# include <uwvm2/object/memory/impl.h>
# include <uwvm2/imported/wasi/wasip1/abi/impl.h>
# include <uwvm2/imported/wasi/wasip1/fd_manager/impl.h>
# include <uwvm2/imported/wasi/wasip1/memory/impl.h>
# if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
#  include "resource_limits.h"
#  include "bounded_stdio.h"
# endif
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::environment
{
    /// @brief Explicit non-descriptor authority for one concrete WASI environment.
    /// @details Import visibility does not grant these operations. Empty authority
    ///          is the default, including for environments constructed by embedders.
    ///          Each clock domain is independent. stdio_full_rights affects only
    ///          initialization and deliberately opts into the former broad masks.
    enum class permission_t : ::std::uint32_t
    {
        clock_realtime = 1u,
        clock_monotonic = 2u,
        clock_process_cputime = 4u,
        clock_thread_cputime = 8u,
        random = 16u,
        proc_raise = 32u,
        sched_yield = 64u,
        stdio_full_rights = 128u,
        stdin_read = 256u, stdout_write = 512u, stderr_write = 1024u,
        stdio_metadata = 2048u, stdio_readiness = 4096u,
        args = 8192u, environ = 16384u, proc_exit = 32768u, thread_spawn = 65536u,
        realtime_resolution = 131072u, monotonic_resolution = 262144u, process_resolution = 524288u, thread_resolution = 1048576u,
        realtime_timer = 2097152u, monotonic_timer = 4194304u, process_timer = 8388608u, thread_timer = 16777216u
    };

    [[nodiscard]] inline constexpr permission_t operator|(permission_t left, permission_t right) noexcept
    { return static_cast<permission_t>(static_cast<::std::uint32_t>(left) | static_cast<::std::uint32_t>(right)); }
    [[nodiscard]] inline constexpr permission_t operator&(permission_t left, permission_t right) noexcept
    { return static_cast<permission_t>(static_cast<::std::uint32_t>(left) & static_cast<::std::uint32_t>(right)); }
    [[nodiscard]] inline constexpr permission_t operator~(permission_t value) noexcept
    { return static_cast<permission_t>(~static_cast<::std::uint32_t>(value)); }

    /// @brief Map a recognized WASI clock ID to its independently granted bit.
    /// @return The corresponding bit, or zero for an invalid clock ID. Callers
    ///         retain their invalid-ID diagnostic before checking authority.
    [[nodiscard]] inline constexpr permission_t clock_permission(::uwvm2::imported::wasi::wasip1::abi::clockid_t id) noexcept
    {
        using clockid = ::uwvm2::imported::wasi::wasip1::abi::clockid_t;
        switch(id)
        {
            case clockid::clock_realtime: return permission_t::clock_realtime;
            case clockid::clock_monotonic: return permission_t::clock_monotonic;
            case clockid::clock_process_cputime_id: return permission_t::clock_process_cputime;
            case clockid::clock_thread_cputime_id: return permission_t::clock_thread_cputime;
            default: return permission_t{};
        }
    }

    [[nodiscard]] inline constexpr permission_t clock_resolution_permission(::uwvm2::imported::wasi::wasip1::abi::clockid_t id) noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        return static_cast<permission_t>(static_cast<::std::uint32_t>(clock_permission(id)) << 17u);
#else
        return clock_permission(id);
#endif
    }
    [[nodiscard]] inline constexpr permission_t clock_timer_permission(::uwvm2::imported::wasi::wasip1::abi::clockid_t id) noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        return static_cast<permission_t>(static_cast<::std::uint32_t>(clock_permission(id)) << 21u);
#else
        return clock_permission(id);
#endif
    }

    // Exact interface identities. Unknown/future imports have no implicit bit.
    [[nodiscard]] inline constexpr ::std::uint64_t host_function_bit(::uwvm2::utils::container::u8string_view name) noexcept
    {
#if defined(UWVM_IMPORT_WASI_WASIP1_WASM64)
        // The registered memory64 extension belongs to the same namespace.
        // Signature/registry validation still rejects unknown function names.
        if(name.size()>7uz && ::uwvm2::utils::container::u8string_view{name.data()+name.size()-7uz,7uz}==u8"_wasm64")
        { name=::uwvm2::utils::container::u8string_view{name.data(),name.size()-7uz}; }
#endif
        if(name == u8"args_get") { return ::std::uint64_t{1} << 0u; }
        if(name == u8"args_sizes_get") { return ::std::uint64_t{1} << 1u; }
        if(name == u8"clock_res_get") { return ::std::uint64_t{1} << 2u; }
        if(name == u8"clock_time_get") { return ::std::uint64_t{1} << 3u; }
        if(name == u8"environ_get") { return ::std::uint64_t{1} << 4u; }
        if(name == u8"environ_sizes_get") { return ::std::uint64_t{1} << 5u; }
        if(name == u8"fd_advise") { return ::std::uint64_t{1} << 6u; }
        if(name == u8"fd_allocate") { return ::std::uint64_t{1} << 7u; }
        if(name == u8"fd_close") { return ::std::uint64_t{1} << 8u; }
        if(name == u8"fd_datasync") { return ::std::uint64_t{1} << 9u; }
        if(name == u8"fd_fdstat_get") { return ::std::uint64_t{1} << 10u; }
        if(name == u8"fd_fdstat_set_flags") { return ::std::uint64_t{1} << 11u; }
        if(name == u8"fd_fdstat_set_rights") { return ::std::uint64_t{1} << 12u; }
        if(name == u8"fd_filestat_get") { return ::std::uint64_t{1} << 13u; }
        if(name == u8"fd_filestat_set_size") { return ::std::uint64_t{1} << 14u; }
        if(name == u8"fd_filestat_set_times") { return ::std::uint64_t{1} << 15u; }
        if(name == u8"fd_pread") { return ::std::uint64_t{1} << 16u; }
        if(name == u8"fd_prestat_dir_name") { return ::std::uint64_t{1} << 17u; }
        if(name == u8"fd_prestat_get") { return ::std::uint64_t{1} << 18u; }
        if(name == u8"fd_pwrite") { return ::std::uint64_t{1} << 19u; }
        if(name == u8"fd_read") { return ::std::uint64_t{1} << 20u; }
        if(name == u8"fd_readdir") { return ::std::uint64_t{1} << 21u; }
        if(name == u8"fd_renumber") { return ::std::uint64_t{1} << 22u; }
        if(name == u8"fd_seek") { return ::std::uint64_t{1} << 23u; }
        if(name == u8"fd_sync") { return ::std::uint64_t{1} << 24u; }
        if(name == u8"fd_tell") { return ::std::uint64_t{1} << 25u; }
        if(name == u8"fd_write") { return ::std::uint64_t{1} << 26u; }
        if(name == u8"path_create_directory") { return ::std::uint64_t{1} << 27u; }
        if(name == u8"path_filestat_get") { return ::std::uint64_t{1} << 28u; }
        if(name == u8"path_filestat_set_times") { return ::std::uint64_t{1} << 29u; }
        if(name == u8"path_link") { return ::std::uint64_t{1} << 30u; }
        if(name == u8"path_open") { return ::std::uint64_t{1} << 31u; }
        if(name == u8"path_readlink") { return ::std::uint64_t{1} << 32u; }
        if(name == u8"path_remove_directory") { return ::std::uint64_t{1} << 33u; }
        if(name == u8"path_rename") { return ::std::uint64_t{1} << 34u; }
        if(name == u8"path_symlink") { return ::std::uint64_t{1} << 35u; }
        if(name == u8"path_unlink_file") { return ::std::uint64_t{1} << 36u; }
        if(name == u8"poll_oneoff") { return ::std::uint64_t{1} << 37u; }
#if !defined(UWVM_ROS_ZERO_HOST_DEFAULTS) || defined(UWVM_CPP_EXCEPTIONS)
        if(name == u8"proc_exit") { return ::std::uint64_t{1} << 38u; }
#endif
        if(name == u8"proc_raise") { return ::std::uint64_t{1} << 39u; }
        if(name == u8"random_get") { return ::std::uint64_t{1} << 40u; }
        if(name == u8"sched_yield") { return ::std::uint64_t{1} << 41u; }
        if(name == u8"sock_accept") { return ::std::uint64_t{1} << 42u; }
        if(name == u8"sock_recv") { return ::std::uint64_t{1} << 43u; }
        if(name == u8"sock_send") { return ::std::uint64_t{1} << 44u; }
        if(name == u8"sock_shutdown") { return ::std::uint64_t{1} << 45u; }
        if(name == u8"thread-spawn") { return ::std::uint64_t{1} << 46u; }
        return 0u;
    }

    template <typename memory_type>
    concept wasip1_memory = requires(memory_type& mem, ::std::size_t offset, ::std::byte* begin, ::std::byte* end) {
        { ::uwvm2::imported::wasi::wasip1::memory::check_memory_bounds(mem, offset, 0uz) };

        { ::uwvm2::imported::wasi::wasip1::memory::get_basic_wasm_type_from_memory<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32>(mem, offset) };
        { ::uwvm2::imported::wasi::wasip1::memory::get_basic_wasm_type_from_memory<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u64>(mem, offset) };
        {
            ::uwvm2::imported::wasi::wasip1::memory::store_basic_wasm_type_to_memory<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32>(
                mem,
                offset,
                ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32{})
        };
        {
            ::uwvm2::imported::wasi::wasip1::memory::store_basic_wasm_type_to_memory<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u64>(
                mem,
                offset,
                ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u64{})
        };
        { ::uwvm2::imported::wasi::wasip1::memory::read_all_from_memory(mem, offset, begin, end) };
        { ::uwvm2::imported::wasi::wasip1::memory::write_all_to_memory(mem, offset, begin, end) };

        {
            ::uwvm2::imported::wasi::wasip1::memory::get_basic_wasm_type_from_memory_unchecked<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32>(mem,
                                                                                                                                                       offset)
        };
        {
            ::uwvm2::imported::wasi::wasip1::memory::get_basic_wasm_type_from_memory_unchecked<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u64>(mem,
                                                                                                                                                       offset)
        };
        {
            ::uwvm2::imported::wasi::wasip1::memory::store_basic_wasm_type_to_memory_unchecked<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32>(
                mem,
                offset,
                ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32{})
        };
        {
            ::uwvm2::imported::wasi::wasip1::memory::store_basic_wasm_type_to_memory_unchecked<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u64>(
                mem,
                offset,
                ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u64{})
        };
        { ::uwvm2::imported::wasi::wasip1::memory::read_all_from_memory_unchecked(mem, offset, begin, end) };
        { ::uwvm2::imported::wasi::wasip1::memory::write_all_to_memory_unchecked(mem, offset, begin, end) };
    };

#if __cpp_lib_atomic_wait >= 201907L
    static_assert(wasip1_memory<::uwvm2::object::memory::linear::allocator_memory_t>);
#endif

#if defined(UWVM_SUPPORT_MMAP)
    static_assert(wasip1_memory<::uwvm2::object::memory::linear::mmap_memory_t>);
#endif

    struct mount_dir_root_t
    {
        ::uwvm2::utils::container::u8string preload_dir{};
        ::fast_io::dir_file entry{};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        // A host-selected pair applies only to this root. Otherwise the
        // module's explicit filesystem masks apply (zero by default).
        bool rights_configured{};
        ::std::uint64_t rights_base{}, rights_inheriting{};
#endif
    };

    using wasip1_proc_exit_ptr_t = void (*)(::uwvm2::parser::wasm::standard::wasm1::type::wasm_i32) noexcept;
    using wasip1_proc_raise_ptr_t = ::uwvm2::imported::wasi::wasip1::abi::errno_t (*)(::uwvm2::parser::wasm::standard::wasm1::type::wasm_i32) noexcept;
    using wasip1_sched_yield_ptr_t = ::uwvm2::imported::wasi::wasip1::abi::errno_t (*)() noexcept;

    enum class trace_wasip1_output_target_t : unsigned
    {
        none = 0u,
        out,
        err,
        file
    };

    enum class trace_wasip1_group_kind_t : unsigned
    {
        global = 0u,
        single,
        custom_group
    };

#if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
    enum class handle_type_e : unsigned
    {
        listen,
        connect,
        bind
    };

    using sock_family_t = ::fast_io::sock_family;
    using sock_protocol_t = ::fast_io::sock_protocol;
    using sock_type_t = ::fast_io::sock_type;
    using ip_t = ::fast_io::ip;

    struct preopen_socket_t
    {
        // sock_family != local
        ip_t ip{};
        sock_family_t sock_family{};
        sock_protocol_t sock_protocol{};
        sock_type_t sock_type{};

        handle_type_e handle_type{};

        // sock_family == local
        ::uwvm2::utils::container::u8string local_unix_path{};

        // wasi fd
        ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd{};
    };
#endif

    struct policy_resource_binding
    {
        ::std::uint64_t kind{}, logical_slot{}, incarnation{};
        ::uwvm2::utils::container::u8string label{};
        // Immutable initial authority, separate from a guest's later fd-rights
        // reduction or close. Bind both masks into the effective policy digest.
        ::std::uint64_t rights_base{}, rights_inheriting{};
    };

    /// @brief [singleton]
    template <wasip1_memory memory_type>
    struct wasip1_environment
    {
        // wasip1 only support only one memory (memory[0])
        memory_type* wasip1_memory{};
        // allow user custom argv, generate argv from system api is not supported
        ::uwvm2::utils::container::vector<::uwvm2::utils::container::u8string_view> argv{};
        // allow user custom envs, use current vm envs by default
        ::uwvm2::utils::container::vector<::uwvm2::utils::container::u8string_view> envs{};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        // Authorized policy replacement owns its approved bytes in this stable
        // environment. Views are published only while execution is drained.
        ::uwvm2::utils::container::vector<::uwvm2::utils::container::u8string> policy_owned_argv{};
        ::uwvm2::utils::container::vector<::uwvm2::utils::container::u8string> policy_owned_envs{};
#endif

        ::uwvm2::imported::wasi::wasip1::fd_manager::wasm_fd_storage_t fd_storage{};  // [singleton]

        /// @brief Provide predefined wasi content, which is already opened during command-line processing (to prevent TOCTOU). Subsequent operations utilize
        ///        this content via dup.
        /// @note  For platforms that support dup, use dup; for platforms that do not support dup, use observer.
        ::uwvm2::utils::container::vector<mount_dir_root_t> mount_dir_roots{};

#if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
        /// @brief Automatically binds during environment initialization
        /// @note  Begin initializing the socket during environment initialization.
        ::uwvm2::utils::container::vector<preopen_socket_t> preopen_sockets{};
#endif

        /// @brief Custom process exit function pointer for WASI exit handling (No no-return operation is required, as plugin systems typically cannot operate
        ///        without returning.)
        /// @note  If not set, the default exit behavior will be used. (exit(3))
        wasip1_proc_exit_ptr_t wasip1_proc_exit_func_ptr{};
        wasip1_proc_raise_ptr_t wasip1_proc_raise_func_ptr{};
        wasip1_sched_yield_ptr_t wasip1_sched_yield_func_ptr{};

        bool trace_wasip1_call{};
        // Optional host console observer; installed only by the debug launcher.
        // true means the validated write was consumed by its bounded IO pane.
        bool (*debug_console_write)(int, ::fast_io::io_scatter_t const*, ::std::size_t) noexcept{};
        ::std::uint64_t debug_trace_identity{};
        void (*debug_fd_created)(::std::uint64_t) noexcept{};
        // Numeric IO metadata only, after the original iovec validation and
        // after successful result publication. No payload or memory borrow.
        void (*debug_io_observed)(::std::uint64_t,::std::uint64_t,bool) noexcept{};
        bool (*debug_console_read)(int, ::fast_io::io_scatter_t const*, ::std::size_t, ::std::size_t&) noexcept{};
        trace_wasip1_output_target_t trace_wasip1_output_target{};
        ::uwvm2::utils::container::u8string trace_wasip1_output_file_path_storage{};
        ::fast_io::u8native_file trace_wasip1_output_file{};
        trace_wasip1_group_kind_t trace_wasip1_group_kind{trace_wasip1_group_kind_t::global};
        ::uwvm2::utils::container::u8string trace_wasip1_group_name_storage{};
        bool disable_utf8_check{};

        /// @brief Authority installed by trusted configuration, never by Guest input.
        /// @note Checkpoint data must not elevate this live policy. A zero request
        ///       is rejected so an unknown clock ID cannot accidentally authorize.
        permission_t non_fd_permissions{};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        resource_limits limits{};
        // Immutable approved resource identities, installed by the initializer.
        // Guest close/seek do not rewrite the approval contract. No owners are
        // retained here, so close still releases its native stream normally.
        ::uwvm2::utils::container::vector<policy_resource_binding> policy_resources{};
#endif
        // Namespace admission is separate from operation/resource authority.
        // Cached imports recheck admission against the immediate caller.
        bool interface_enabled{};
        ::uwvm2::utils::control::host_interfaces::policy interfaces{};
        bool proc_exit_requested{};
        bool proc_exit_denied{};
        ::std::uint32_t proc_exit_code{};

        [[nodiscard]] inline constexpr bool allows_interface(::uwvm2::utils::container::u8string_view name) const noexcept
        {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            return interface_enabled && interfaces.allowed(::uwvm2::utils::control::host_interfaces::find(
                ::uwvm2::utils::control::host_interfaces::family::wasip1, name));
#else
            static_cast<void>(name); return true;
#endif
        }

        [[nodiscard]] inline constexpr bool grants_stdio(unsigned index) const noexcept
        {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            return permits(index == 0u ? permission_t::stdin_read : index == 1u ? permission_t::stdout_write : permission_t::stderr_write);
#else
            static_cast<void>(index); return true;
#endif
        }


        [[nodiscard]] inline constexpr bool permits(permission_t requested) const noexcept
        { return requested != permission_t{} && (non_fd_permissions & requested) == requested; }

        /// @brief Project trusted policy to the rights installed on a standard stream.
        /// @param input True for stdin; false for stdout/stderr. Descriptor labels
        ///        do not override native access modes or create another resource.
        [[nodiscard]] inline constexpr ::uwvm2::imported::wasi::wasip1::abi::rights_t stdio_base_rights(bool input) const noexcept
        {
            using rights = ::uwvm2::imported::wasi::wasip1::abi::rights_t;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            return (input ? rights::right_fd_read : rights::right_fd_write) |
                (permits(permission_t::stdio_metadata) ? rights::right_fd_filestat_get : rights{}) |
                (permits(permission_t::stdio_readiness) ? rights::right_poll_fd_readwrite : rights{});
#else
            return permits(permission_t::stdio_full_rights) ? static_cast<rights>(-1) :
                rights::right_fd_filestat_get | rights::right_poll_fd_readwrite |
                (input ? rights::right_fd_read : rights::right_fd_write);
#endif
        }
        [[nodiscard]] inline constexpr ::uwvm2::imported::wasi::wasip1::abi::rights_t stdio_inheriting_rights() const noexcept
        {
            using rights = ::uwvm2::imported::wasi::wasip1::abi::rights_t;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            return rights{};
#else
            return permits(permission_t::stdio_full_rights) ? static_cast<rights>(-1) : rights{};
#endif
        }

        // Native callers retain the ordinary field. Runtime-managed environments
        // resolve immediate-caller memory without mutating shared resource policy.
        memory_type* (*wasip1_memory_resolver)(wasip1_environment const*) noexcept{};

        [[nodiscard]] inline constexpr memory_type* get_memory() const noexcept
        {
            return wasip1_memory_resolver == nullptr ? wasip1_memory : wasip1_memory_resolver(this);
        }

    };

}  // namespace uwvm2::imported::wasi::wasip1::environment

#ifndef UWVM_MODULE
// macro
# include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif

#include "diagnostics.h"
