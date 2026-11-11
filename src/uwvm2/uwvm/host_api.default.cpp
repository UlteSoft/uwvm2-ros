/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2026-03-12
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

#ifndef UWVM_MODULE
// std
# include <cstddef>
# include <cstdint>
# include <memory>
// macro
# include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
# include <uwvm2/uwvm/runtime/macro/push_macros.h>
// import
# include <uwvm2/imported/wasi/wasip1/impl.h>
# include <uwvm2/uwvm/imported/wasi/wasip1/storage/impl.h>
# include <uwvm2/uwvm/wasm/storage/impl.h>
# include <uwvm2/uwvm/wasm/type/impl.h>
# include <uwvm2/runtime/lib/uwvm_runtime.h>
# include <uwvm2/uwvm/io/host_diagnostics.h>
#endif

#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
namespace uwvm2::uwvm::wasm::type::details
{
    [[nodiscard]] inline bool wasip1_host_api_is_exposed() noexcept
    {
        bool expose_host_api{::uwvm2::uwvm::wasm::storage::preload_expose_wasip1_host_api};
# if defined(UWVM_USE_THREAD_LOCAL)
        // Keep the TLS build on the direct thread_local variables. The accessor
        // path below is only for non-TLS compatibility builds.
        if(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_target_is_set)
        {
            if(auto const override_state{::uwvm2::uwvm::imported::wasi::wasip1::storage::find_wasip1_module_override_const(
                   ::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_target_kind,
                   ::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_target_module_name)};
               override_state != nullptr && override_state->expose_host_api_is_set) [[unlikely]]
            {
                expose_host_api = override_state->expose_host_api;
            }
        }
# else
        if(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_target_is_set_ref())
        {
            if(auto const override_state{::uwvm2::uwvm::imported::wasi::wasip1::storage::find_wasip1_module_override_const(
                   ::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_target_kind_ref(),
                   ::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_target_module_name_ref())};
               override_state != nullptr && override_state->expose_host_api_is_set) [[unlikely]]
            {
                expose_host_api = override_state->expose_host_api;
            }
        }
# endif

        return expose_host_api;

    }
    [[nodiscard]] inline bool wasip1_host_api_call_is_allowed(::uwvm2::utils::container::u8string_view function) noexcept
    {
        // A driver may cache a table shared by several registrations. Recheck
        // its current logical target and the exact API on EVERY call. Exposing
        // the table alone grants no Guest-memory or native-resource operation.
        if(!wasip1_host_api_is_exposed()) { return false; }
# if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        auto& env{::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env()};
        if(!env.allows_interface(function))
        {
            namespace api = ::uwvm2::utils::control::host_interfaces;
            auto const id{api::find(api::family::wasip1, function)};
            ::uwvm2::imported::wasi::wasip1::environment::diagnostic_denied(env, id, true);
            return false;
        }
# else
        (void)function;
# endif
        return true;
    }

    // Keep auditing and quota coordination in one native implementation. Each
    // typed table entry contributes only its small synchronous provider thunk.
    [[nodiscard]]
# if defined(__GNUC__) || defined(__clang__)
    __attribute__((__noinline__))
# elif defined(_MSC_VER)
    __declspec(noinline)
# endif
    ::uwvm2::imported::wasi::wasip1::abi::errno_t invoke_wasip1_host_api_erased(
        ::uwvm2::utils::container::u8string_view function,
        ::uwvm2::imported::wasi::wasip1::abi::errno_t (*invoke)(void*) noexcept, void* context,
        ::std::uint64_t first, ::std::uint64_t second) noexcept
    {
        using error = ::uwvm2::imported::wasi::wasip1::abi::errno_t;
# if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        namespace api = ::uwvm2::utils::control::host_interfaces;
        namespace diagnostic = ::uwvm2::utils::control::host_diagnostics;
        namespace environment = ::uwvm2::imported::wasi::wasip1::environment;
        auto& env{::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env()};
        auto const id{api::find(api::family::wasip1, function)};
        diagnostic::call_scope quota{id, environment::diagnostic_scope(env), environment::diagnostic_target(env)};
# else
        (void)function; (void)first; (void)second;
# endif
        struct provider_state { error (*invoke)(void*) noexcept; void* context; error result; };
        provider_state provider{invoke, context, error::eio};
        auto const completed{::uwvm2::runtime::lib::runtime_invoke_wasip1_host_api_with_quota_wait(
            [](void* state) noexcept
            {
                auto& provider{*static_cast<provider_state*>(state)};
                provider.result = provider.invoke(provider.context);
            }, ::std::addressof(provider))};
        using status = ::uwvm2::runtime::lib::runtime_host_quota_wait_result;
        if(completed == status::cancelled) { return error::ecanceled; }
        if(completed == status::unavailable) { return error::eio; }
# if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(provider.result == error::enotcapable)
        {
            if((function == u8"fd_renumber" || function == u8"fd_renumber_wasm64") &&
               !(first < 3u && !env.grants_stdio(static_cast<unsigned>(first))) &&
               second < 3u && !env.grants_stdio(static_cast<unsigned>(second))) { first = second; }
            environment::diagnostic_denied(env, id, false, first);
        }
# endif
        return provider.result;
    }

    template<typename Invoke>
    [[nodiscard]] inline ::uwvm2::imported::wasi::wasip1::abi::errno_t invoke_wasip1_host_api(
        ::uwvm2::utils::container::u8string_view function, Invoke invoke,
        ::std::uint64_t first = UINT64_MAX, ::std::uint64_t second = UINT64_MAX) noexcept
    {
        return invoke_wasip1_host_api_erased(function,
            [](void* context) noexcept { return (*static_cast<Invoke*>(context))(); }, ::std::addressof(invoke), first, second);
    }
}
#endif

// wasip1 host api wrappers
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_args_get(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t argv_ptrsz,
                                                                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t argv_buf_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"args_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"args_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::args_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), argv_ptrsz, argv_buf_ptrsz);
    }, static_cast<::std::uint64_t>(argv_ptrsz), static_cast<::std::uint64_t>(argv_buf_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t
    uwvm_wasip1_args_sizes_get(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t argc_ptrsz,
                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t argv_buf_size_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"args_sizes_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"args_sizes_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::args_sizes_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                 argc_ptrsz,
                                                                 argv_buf_size_ptrsz);
    }, static_cast<::std::uint64_t>(argc_ptrsz), static_cast<::std::uint64_t>(argv_buf_size_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t
    uwvm_wasip1_clock_res_get(::uwvm2::imported::wasi::wasip1::abi::clockid_t clock_id,
                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t resolution_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"clock_res_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"clock_res_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::clock_res_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                clock_id,
                                                                resolution_ptrsz);
    }, static_cast<::std::uint64_t>(clock_id), static_cast<::std::uint64_t>(resolution_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_clock_time_get(::uwvm2::imported::wasi::wasip1::abi::clockid_t clock_id,
                                                                                    ::uwvm2::imported::wasi::wasip1::abi::timestamp_t precision,
                                                                                    ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t time_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"clock_time_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"clock_time_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::clock_time_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                 clock_id,
                                                                 precision,
                                                                 time_ptrsz);
    }, static_cast<::std::uint64_t>(clock_id), static_cast<::std::uint64_t>(precision));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t
    uwvm_wasip1_environ_get(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t environ_ptrsz,
                            ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t environ_buf_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"environ_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"environ_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::environ_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                              environ_ptrsz,
                                                              environ_buf_ptrsz);
    }, static_cast<::std::uint64_t>(environ_ptrsz), static_cast<::std::uint64_t>(environ_buf_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t
    uwvm_wasip1_environ_sizes_get(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t environ_count_ptrsz,
                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t environ_buf_size_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"environ_sizes_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"environ_sizes_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::environ_sizes_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                    environ_count_ptrsz,
                                                                    environ_buf_size_ptrsz);
    }, static_cast<::std::uint64_t>(environ_count_ptrsz), static_cast<::std::uint64_t>(environ_buf_size_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_advise(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::filesize_t offset,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::filesize_t len,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::advice_t advice) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_advise")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_advise", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_advise(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, offset, len, advice);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(offset));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_allocate(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::filesize_t offset,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::filesize_t len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_allocate")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_allocate", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_allocate(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, offset, len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(offset));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_close(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_close")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_close", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_close(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd);
    }, static_cast<::std::uint64_t>(fd));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_datasync(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_datasync")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_datasync", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_datasync(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd);
    }, static_cast<::std::uint64_t>(fd));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_fdstat_get(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t stat_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_fdstat_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_fdstat_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_fdstat_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, stat_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(stat_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_fdstat_set_flags(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                         ::uwvm2::imported::wasi::wasip1::abi::fdflags_t flags) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_fdstat_set_flags")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_fdstat_set_flags", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_fdstat_set_flags(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, flags);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t
    uwvm_wasip1_fd_fdstat_set_rights(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                     ::uwvm2::imported::wasi::wasip1::abi::rights_t fs_rights_base,
                                     ::uwvm2::imported::wasi::wasip1::abi::rights_t fs_rights_inheriting) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_fdstat_set_rights")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_fdstat_set_rights", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_fdstat_set_rights(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                       fd,
                                                                       fs_rights_base,
                                                                       fs_rights_inheriting);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(fs_rights_base));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_filestat_get(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                     ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t stat_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_filestat_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_filestat_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_filestat_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, stat_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(stat_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_filestat_set_size(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                          ::uwvm2::imported::wasi::wasip1::abi::filesize_t size) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_filestat_set_size")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_filestat_set_size", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_filestat_set_size(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, size);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(size));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_filestat_set_times(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                           ::uwvm2::imported::wasi::wasip1::abi::timestamp_t atim,
                                                                                           ::uwvm2::imported::wasi::wasip1::abi::timestamp_t mtim,
                                                                                           ::uwvm2::imported::wasi::wasip1::abi::fstflags_t fstflags) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_filestat_set_times")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_filestat_set_times", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_filestat_set_times(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                        fd,
                                                                        atim,
                                                                        mtim,
                                                                        fstflags);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(atim));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_pread(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t iovs,
                                                                              ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t iovs_len,
                                                                              ::uwvm2::imported::wasi::wasip1::abi::filesize_t offset,
                                                                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t nread) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_pread")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_pread", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_pread(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                           fd,
                                                           iovs,
                                                           iovs_len,
                                                           offset,
                                                           nread);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(iovs));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_prestat_dir_name(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                         ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path,
                                                                                         ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_prestat_dir_name")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_prestat_dir_name", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_prestat_dir_name(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, path, path_len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_prestat_get(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                    ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t buf_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_prestat_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_prestat_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_prestat_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, buf_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(buf_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_pwrite(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t iovs,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t iovs_len,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::filesize_t offset,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t nwritten) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_pwrite")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_pwrite", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_pwrite(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                            fd,
                                                            iovs,
                                                            iovs_len,
                                                            offset,
                                                            nwritten);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(iovs));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_read(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t iovs,
                                                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t iovs_len,
                                                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t nread) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_read")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_read", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_read(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, iovs, iovs_len, nread);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(iovs));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_readdir(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t buf_ptrsz,
                                                                                ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t buf_len,
                                                                                ::uwvm2::imported::wasi::wasip1::abi::dircookie_t cookie,
                                                                                ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t buf_used_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_readdir")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_readdir", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_readdir(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                             fd,
                                                             buf_ptrsz,
                                                             buf_len,
                                                             cookie,
                                                             buf_used_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(buf_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_renumber(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd_from,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd_to) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_renumber")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_renumber", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_renumber(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd_from, fd_to);
    }, static_cast<::std::uint64_t>(fd_from), static_cast<::std::uint64_t>(fd_to));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_seek(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                             ::uwvm2::imported::wasi::wasip1::abi::filedelta_t offset,
                                                                             ::uwvm2::imported::wasi::wasip1::abi::whence_t whence,
                                                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t new_offset_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_seek")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_seek", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_seek(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                          fd,
                                                          offset,
                                                          whence,
                                                          new_offset_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(offset));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_sync(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_sync")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_sync", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_sync(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd);
    }, static_cast<::std::uint64_t>(fd));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_tell(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t tell_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_tell")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_tell", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_tell(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, tell_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(tell_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_fd_write(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t iovs,
                                                                              ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t iovs_len,
                                                                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t nwritten) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_write")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_write", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_write(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, iovs, iovs_len, nwritten);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(iovs));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_create_directory(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                           ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path_ptrsz,
                                                                                           ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_create_directory")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_create_directory", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_create_directory(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                        fd,
                                                                        path_ptrsz,
                                                                        path_len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_filestat_get(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                       ::uwvm2::imported::wasi::wasip1::abi::lookupflags_t flags,
                                                                                       ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path_ptrsz,
                                                                                       ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len,
                                                                                       ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t buf_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_filestat_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_filestat_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_filestat_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                    fd,
                                                                    flags,
                                                                    path_ptrsz,
                                                                    path_len,
                                                                    buf_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_filestat_set_times(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                             ::uwvm2::imported::wasi::wasip1::abi::lookupflags_t flags,
                                                                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path_ptrsz,
                                                                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len,
                                                                                             ::uwvm2::imported::wasi::wasip1::abi::timestamp_t atim,
                                                                                             ::uwvm2::imported::wasi::wasip1::abi::timestamp_t mtim,
                                                                                             ::uwvm2::imported::wasi::wasip1::abi::fstflags_t fstflags) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_filestat_set_times")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_filestat_set_times", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_filestat_set_times(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                          fd,
                                                                          flags,
                                                                          path_ptrsz,
                                                                          path_len,
                                                                          atim,
                                                                          mtim,
                                                                          fstflags);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_link(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t old_fd,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::lookupflags_t old_flags,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t old_path_ptrsz,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t old_path_len,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t new_fd,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t new_path_ptrsz,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t new_path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_link")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_link", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_link(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                            old_fd,
                                                            old_flags,
                                                            old_path_ptrsz,
                                                            old_path_len,
                                                            new_fd,
                                                            new_path_ptrsz,
                                                            new_path_len);
    }, static_cast<::std::uint64_t>(old_fd), static_cast<::std::uint64_t>(old_flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_open(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t dirfd,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::lookupflags_t dirflags,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path_ptrsz,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::oflags_t oflags,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::rights_t fs_rights_base,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::rights_t fs_rights_inheriting,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::fdflags_t fdflags,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t fd_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_open")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_open", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_open(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                            dirfd,
                                                            dirflags,
                                                            path_ptrsz,
                                                            path_len,
                                                            oflags,
                                                            fs_rights_base,
                                                            fs_rights_inheriting,
                                                            fdflags,
                                                            fd_ptrsz);
    }, static_cast<::std::uint64_t>(dirfd), static_cast<::std::uint64_t>(dirflags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t
    uwvm_wasip1_path_readlink(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path_ptrsz,
                              ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len,
                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t buf_ptrsz,
                              ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t buf_len,
                              ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t buf_used_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_readlink")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_readlink", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_readlink(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                fd,
                                                                path_ptrsz,
                                                                path_len,
                                                                buf_ptrsz,
                                                                buf_len,
                                                                buf_used_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_remove_directory(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                           ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path_ptrsz,
                                                                                           ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_remove_directory")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_remove_directory", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_remove_directory(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                        fd,
                                                                        path_ptrsz,
                                                                        path_len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_rename(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t old_fd,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t old_path_ptrsz,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t old_path_len,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t new_fd,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t new_path_ptrsz,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t new_path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_rename")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_rename", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_rename(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                              old_fd,
                                                              old_path_ptrsz,
                                                              old_path_len,
                                                              new_fd,
                                                              new_path_ptrsz,
                                                              new_path_len);
    }, static_cast<::std::uint64_t>(old_fd), static_cast<::std::uint64_t>(old_path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_symlink(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t old_path_ptrsz,
                                                                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t old_path_len,
                                                                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t new_fd,
                                                                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t new_path_ptrsz,
                                                                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t new_path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_symlink")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_symlink", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_symlink(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                               old_path_ptrsz,
                                                               old_path_len,
                                                               new_fd,
                                                               new_path_ptrsz,
                                                               new_path_len);
    }, static_cast<::std::uint64_t>(old_path_ptrsz), static_cast<::std::uint64_t>(old_path_len));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_path_unlink_file(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t fd,
                                                                                      ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t path_ptrsz,
                                                                                      ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_unlink_file")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_unlink_file", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_unlink_file(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                   fd,
                                                                   path_ptrsz,
                                                                   path_len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_poll_oneoff(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t in,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t out,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t nsubscriptions,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t nevents) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"poll_oneoff")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"poll_oneoff", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::poll_oneoff(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                              in,
                                                              out,
                                                              nsubscriptions,
                                                              nevents);
    }, static_cast<::std::uint64_t>(in), static_cast<::std::uint64_t>(out));
}

extern "C" void uwvm_wasip1_proc_exit(::uwvm2::imported::wasi::wasip1::abi::exitcode_t code) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"proc_exit")) [[unlikely]] { return; }

# if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    auto& env{::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env()};
    if(!env.permits(::uwvm2::imported::wasi::wasip1::environment::permission_t::proc_exit))
    {
        namespace api = ::uwvm2::utils::control::host_interfaces;
        ::uwvm2::imported::wasi::wasip1::environment::diagnostic_denied(
            env, api::find(api::family::wasip1, ::uwvm2::utils::container::u8string_view{u8"proc_exit"}), false);
        return;
    }
# endif
    (void)::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"proc_exit", [&]() noexcept
    {
        // A denied native exit must not close the cache writer or audit journal.
        ::uwvm2::runtime::lib::runtime_stop_before_proc_exit_host_api();
        ::uwvm2::imported::wasi::wasip1::func::proc_exit(
            ::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), code);
        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess;
    }, static_cast<::std::uint64_t>(code));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_proc_raise(::uwvm2::imported::wasi::wasip1::abi::signal_t code) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"proc_raise")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"proc_raise", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::proc_raise(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), code);
    }, static_cast<::std::uint64_t>(code));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_random_get(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t buf,
                                                                                ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t buf_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"random_get")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"random_get", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::random_get(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), buf, buf_len);
    }, static_cast<::std::uint64_t>(buf), static_cast<::std::uint64_t>(buf_len));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_sched_yield() noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sched_yield")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sched_yield", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sched_yield(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env());
    });
}

# if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_sock_accept(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t sock_fd,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::fdflags_t fd_flags,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t ro_fd_ptrsz
#  ifdef UWVM_IMPORT_WASI_WASIP1_SUPPORT_WASIX_SOCKET
                                                                                 ,
                                                                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t ro_addr_ptrsz
#  endif
                                                                                 ) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sock_accept")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sock_accept", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sock_accept(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                              sock_fd,
                                                              fd_flags,
                                                              ro_fd_ptrsz
#  ifdef UWVM_IMPORT_WASI_WASIP1_SUPPORT_WASIX_SOCKET
                                                              ,
                                                              ro_addr_ptrsz
#  endif
    );
    }, static_cast<::std::uint64_t>(sock_fd), static_cast<::std::uint64_t>(fd_flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_sock_recv(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t sock_fd,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t ri_data_ptrsz,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t ri_data_len,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::riflags_t ri_flags,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t ro_data_len_ptrsz,
                                                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t ro_flags_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sock_recv")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sock_recv", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sock_recv(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                            sock_fd,
                                                            ri_data_ptrsz,
                                                            ri_data_len,
                                                            ri_flags,
                                                            ro_data_len_ptrsz,
                                                            ro_flags_ptrsz);
    }, static_cast<::std::uint64_t>(sock_fd), static_cast<::std::uint64_t>(ri_data_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t
    uwvm_wasip1_sock_send(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t sock_fd,
                          ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t si_data_ptrsz,
                          ::uwvm2::imported::wasi::wasip1::abi::wasi_size_t si_data_len,
                          ::uwvm2::imported::wasi::wasip1::abi::siflags_t si_flags,
                          ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_t ret_data_len_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sock_send")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sock_send", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sock_send(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                            sock_fd,
                                                            si_data_ptrsz,
                                                            si_data_len,
                                                            si_flags,
                                                            ret_data_len_ptrsz);
    }, static_cast<::std::uint64_t>(sock_fd), static_cast<::std::uint64_t>(si_data_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_sock_shutdown(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t sock_fd,
                                                                                   ::uwvm2::imported::wasi::wasip1::abi::sdflags_t how) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sock_shutdown")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sock_shutdown", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sock_shutdown(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), sock_fd, how);
    }, static_cast<::std::uint64_t>(sock_fd), static_cast<::std::uint64_t>(how));
}

# endif
# if defined(UWVM_IMPORT_WASI_WASIP1_WASM64)
extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_args_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t argv_ptrsz,
                                ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t argv_buf_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"args_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"args_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::args_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                  argv_ptrsz,
                                                                  argv_buf_ptrsz);
    }, static_cast<::std::uint64_t>(argv_ptrsz), static_cast<::std::uint64_t>(argv_buf_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_args_sizes_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t argc_ptrsz,
                                      ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t argv_buf_size_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"args_sizes_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"args_sizes_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::args_sizes_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                        argc_ptrsz,
                                                                        argv_buf_size_ptrsz);
    }, static_cast<::std::uint64_t>(argc_ptrsz), static_cast<::std::uint64_t>(argv_buf_size_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_clock_res_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::clockid_wasm64_t clock_id,
                                     ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t resolution_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"clock_res_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"clock_res_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::clock_res_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                       clock_id,
                                                                       resolution_ptrsz);
    }, static_cast<::std::uint64_t>(clock_id), static_cast<::std::uint64_t>(resolution_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_clock_time_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::clockid_wasm64_t clock_id,
                                      ::uwvm2::imported::wasi::wasip1::abi::timestamp_wasm64_t precision,
                                      ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t time_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"clock_time_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"clock_time_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::clock_time_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                        clock_id,
                                                                        precision,
                                                                        time_ptrsz);
    }, static_cast<::std::uint64_t>(clock_id), static_cast<::std::uint64_t>(precision));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_environ_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t environ_ptrsz,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t environ_buf_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"environ_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"environ_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::environ_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                     environ_ptrsz,
                                                                     environ_buf_ptrsz);
    }, static_cast<::std::uint64_t>(environ_ptrsz), static_cast<::std::uint64_t>(environ_buf_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_environ_sizes_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t environ_count_ptrsz,
                                         ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t environ_buf_size_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"environ_sizes_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"environ_sizes_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::environ_sizes_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                           environ_count_ptrsz,
                                                                           environ_buf_size_ptrsz);
    }, static_cast<::std::uint64_t>(environ_count_ptrsz), static_cast<::std::uint64_t>(environ_buf_size_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_advise_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                 [[maybe_unused]] ::uwvm2::imported::wasi::wasip1::abi::filesize_wasm64_t offset,
                                 [[maybe_unused]] ::uwvm2::imported::wasi::wasip1::abi::filesize_wasm64_t len,
                                 ::uwvm2::imported::wasi::wasip1::abi::advice_wasm64_t advice) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_advise_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_advise_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_advise_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                   fd,
                                                                   offset,
                                                                   len,
                                                                   advice);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(offset));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_allocate_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                   ::uwvm2::imported::wasi::wasip1::abi::filesize_wasm64_t offset,
                                   ::uwvm2::imported::wasi::wasip1::abi::filesize_wasm64_t len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_allocate_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_allocate_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_allocate_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, offset, len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(offset));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_close_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_close_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_close_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_close_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd);
    }, static_cast<::std::uint64_t>(fd));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_datasync_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_datasync_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_datasync_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_datasync_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd);
    }, static_cast<::std::uint64_t>(fd));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_fdstat_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                     ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t stat_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_fdstat_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_fdstat_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_fdstat_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, stat_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(stat_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_fdstat_set_flags_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                           ::uwvm2::imported::wasi::wasip1::abi::fdflags_wasm64_t flags) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_fdstat_set_flags_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_fdstat_set_flags_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_fdstat_set_flags_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, flags);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_fdstat_set_rights_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                            ::uwvm2::imported::wasi::wasip1::abi::rights_wasm64_t fs_rights_base,
                                            ::uwvm2::imported::wasi::wasip1::abi::rights_wasm64_t fs_rights_inheriting) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_fdstat_set_rights_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_fdstat_set_rights_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_fdstat_set_rights_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                              fd,
                                                                              fs_rights_base,
                                                                              fs_rights_inheriting);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(fs_rights_base));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_filestat_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                       ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t stat_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_filestat_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_filestat_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_filestat_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, stat_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(stat_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_filestat_set_size_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                            ::uwvm2::imported::wasi::wasip1::abi::filesize_wasm64_t size) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_filestat_set_size_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_filestat_set_size_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_filestat_set_size_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, size);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(size));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_filestat_set_times_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                             ::uwvm2::imported::wasi::wasip1::abi::timestamp_wasm64_t atim,
                                             ::uwvm2::imported::wasi::wasip1::abi::timestamp_wasm64_t mtim,
                                             ::uwvm2::imported::wasi::wasip1::abi::fstflags_wasm64_t fstflags) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_filestat_set_times_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_filestat_set_times_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_filestat_set_times_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                               fd,
                                                                               atim,
                                                                               mtim,
                                                                               fstflags);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(atim));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_pread_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t iovs,
                                ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t iovs_len,
                                ::uwvm2::imported::wasi::wasip1::abi::filesize_wasm64_t offset,
                                ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t nread) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_pread_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_pread_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_pread_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                  fd,
                                                                  iovs,
                                                                  iovs_len,
                                                                  offset,
                                                                  nread);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(iovs));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_prestat_dir_name_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                           ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t path,
                                           ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_prestat_dir_name_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_prestat_dir_name_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_prestat_dir_name_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                             fd,
                                                                             path,
                                                                             path_len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_prestat_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                      ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t buf_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_prestat_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_prestat_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_prestat_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, buf_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(buf_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_pwrite_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t iovs,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t iovs_len,
                                 ::uwvm2::imported::wasi::wasip1::abi::filesize_wasm64_t offset,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t nwritten) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_pwrite_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_pwrite_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_pwrite_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                   fd,
                                                                   iovs,
                                                                   iovs_len,
                                                                   offset,
                                                                   nwritten);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(iovs));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_read_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t iovs,
                               ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t iovs_len,
                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t nread) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_read_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_read_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_read_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                 fd,
                                                                 iovs,
                                                                 iovs_len,
                                                                 nread);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(iovs));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_readdir_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t buf_ptrsz,
                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t buf_len,
                                  ::uwvm2::imported::wasi::wasip1::abi::dircookie_wasm64_t cookie,
                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t buf_used_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_readdir_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_readdir_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_readdir_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                    fd,
                                                                    buf_ptrsz,
                                                                    buf_len,
                                                                    cookie,
                                                                    buf_used_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(buf_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_renumber_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd_from,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd_to) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_renumber_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_renumber_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_renumber_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd_from, fd_to);
    }, static_cast<::std::uint64_t>(fd_from), static_cast<::std::uint64_t>(fd_to));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_seek_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                               ::uwvm2::imported::wasi::wasip1::abi::filedelta_wasm64_t offset,
                               ::uwvm2::imported::wasi::wasip1::abi::whence_wasm64_t whence,
                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t new_offset_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_seek_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_seek_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_seek_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                 fd,
                                                                 offset,
                                                                 whence,
                                                                 new_offset_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(offset));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_sync_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_sync_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_sync_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_sync_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd);
    }, static_cast<::std::uint64_t>(fd));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_tell_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t tell_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_tell_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_tell_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_tell_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), fd, tell_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(tell_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_fd_write_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t iovs,
                                ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t iovs_len,
                                ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t nwritten) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"fd_write_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"fd_write_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::fd_write_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                  fd,
                                                                  iovs,
                                                                  iovs_len,
                                                                  nwritten);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(iovs));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_create_directory_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t path_ptrsz,
                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_create_directory_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_create_directory_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_create_directory_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                               fd,
                                                                               path_ptrsz,
                                                                               path_len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_filestat_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                         ::uwvm2::imported::wasi::wasip1::abi::lookupflags_wasm64_t flags,
                                         ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t path_ptrsz,
                                         ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t path_len,
                                         ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t buf_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_filestat_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_filestat_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_filestat_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                           fd,
                                                                           flags,
                                                                           path_ptrsz,
                                                                           path_len,
                                                                           buf_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_filestat_set_times_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                               ::uwvm2::imported::wasi::wasip1::abi::lookupflags_wasm64_t flags,
                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t path_ptrsz,
                                               ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t path_len,
                                               ::uwvm2::imported::wasi::wasip1::abi::timestamp_wasm64_t atim,
                                               ::uwvm2::imported::wasi::wasip1::abi::timestamp_wasm64_t mtim,
                                               ::uwvm2::imported::wasi::wasip1::abi::fstflags_wasm64_t fstflags) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_filestat_set_times_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_filestat_set_times_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_filestat_set_times_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                                 fd,
                                                                                 flags,
                                                                                 path_ptrsz,
                                                                                 path_len,
                                                                                 atim,
                                                                                 mtim,
                                                                                 fstflags);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_link_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t old_fd,
                                 ::uwvm2::imported::wasi::wasip1::abi::lookupflags_wasm64_t old_flags,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t old_path_ptrsz,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t old_path_len,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t new_fd,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t new_path_ptrsz,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t new_path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_link_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_link_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_link_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                   old_fd,
                                                                   old_flags,
                                                                   old_path_ptrsz,
                                                                   old_path_len,
                                                                   new_fd,
                                                                   new_path_ptrsz,
                                                                   new_path_len);
    }, static_cast<::std::uint64_t>(old_fd), static_cast<::std::uint64_t>(old_flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_open_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t dirfd,
                                 ::uwvm2::imported::wasi::wasip1::abi::lookupflags_wasm64_t dirflags,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t path_ptrsz,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t path_len,
                                 ::uwvm2::imported::wasi::wasip1::abi::oflags_wasm64_t oflags,
                                 ::uwvm2::imported::wasi::wasip1::abi::rights_wasm64_t fs_rights_base,
                                 ::uwvm2::imported::wasi::wasip1::abi::rights_wasm64_t fs_rights_inheriting,
                                 ::uwvm2::imported::wasi::wasip1::abi::fdflags_wasm64_t fdflags,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t fd_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_open_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_open_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_open_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                   dirfd,
                                                                   dirflags,
                                                                   path_ptrsz,
                                                                   path_len,
                                                                   oflags,
                                                                   fs_rights_base,
                                                                   fs_rights_inheriting,
                                                                   fdflags,
                                                                   fd_ptrsz);
    }, static_cast<::std::uint64_t>(dirfd), static_cast<::std::uint64_t>(dirflags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_readlink_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                     ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t path_ptrsz,
                                     ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t path_len,
                                     ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t buf_ptrsz,
                                     ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t buf_len,
                                     ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t buf_used_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_readlink_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_readlink_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_readlink_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                       fd,
                                                                       path_ptrsz,
                                                                       path_len,
                                                                       buf_ptrsz,
                                                                       buf_len,
                                                                       buf_used_ptrsz);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_remove_directory_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t path_ptrsz,
                                             ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_remove_directory_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_remove_directory_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_remove_directory_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                               fd,
                                                                               path_ptrsz,
                                                                               path_len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_rename_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t old_fd,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t old_path_ptrsz,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t old_path_len,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t new_fd,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t new_path_ptrsz,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t new_path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_rename_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_rename_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_rename_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                     old_fd,
                                                                     old_path_ptrsz,
                                                                     old_path_len,
                                                                     new_fd,
                                                                     new_path_ptrsz,
                                                                     new_path_len);
    }, static_cast<::std::uint64_t>(old_fd), static_cast<::std::uint64_t>(old_path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_symlink_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t old_path_ptrsz,
                                    ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t old_path_len,
                                    ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t new_fd,
                                    ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t new_path_ptrsz,
                                    ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t new_path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_symlink_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_symlink_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_symlink_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                      old_path_ptrsz,
                                                                      old_path_len,
                                                                      new_fd,
                                                                      new_path_ptrsz,
                                                                      new_path_len);
    }, static_cast<::std::uint64_t>(old_path_ptrsz), static_cast<::std::uint64_t>(old_path_len));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_path_unlink_file_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t fd,
                                        ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t path_ptrsz,
                                        ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t path_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"path_unlink_file_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"path_unlink_file_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::path_unlink_file_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                          fd,
                                                                          path_ptrsz,
                                                                          path_len);
    }, static_cast<::std::uint64_t>(fd), static_cast<::std::uint64_t>(path_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_poll_oneoff_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t in,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t out,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t nsubscriptions,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t nevents) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"poll_oneoff_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"poll_oneoff_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::poll_oneoff_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                     in,
                                                                     out,
                                                                     nsubscriptions,
                                                                     nevents);
    }, static_cast<::std::uint64_t>(in), static_cast<::std::uint64_t>(out));
}

extern "C" void uwvm_wasip1_proc_exit_wasm64(::uwvm2::imported::wasi::wasip1::abi::exitcode_wasm64_t code) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"proc_exit_wasm64")) [[unlikely]] { return; }

# if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    auto& env{::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env()};
    if(!env.permits(::uwvm2::imported::wasi::wasip1::environment::permission_t::proc_exit))
    {
        namespace api = ::uwvm2::utils::control::host_interfaces;
        ::uwvm2::imported::wasi::wasip1::environment::diagnostic_denied(
            env, api::find(api::family::wasip1, ::uwvm2::utils::container::u8string_view{u8"proc_exit_wasm64"}), false);
        return;
    }
# endif
    (void)::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"proc_exit_wasm64", [&]() noexcept
    {
        // A denied native exit must not close the cache writer or audit journal.
        ::uwvm2::runtime::lib::runtime_stop_before_proc_exit_host_api();
        ::uwvm2::imported::wasi::wasip1::func::proc_exit_wasm64(
            ::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), code);
        return ::uwvm2::imported::wasi::wasip1::abi::errno_t::esuccess;
    }, static_cast<::std::uint64_t>(code));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_proc_raise_wasm64(::uwvm2::imported::wasi::wasip1::abi::signal_t code) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"proc_raise_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"proc_raise_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::proc_raise_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), code);
    }, static_cast<::std::uint64_t>(code));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_random_get_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t buf,
                                  ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t buf_len) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"random_get_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"random_get_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::random_get_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), buf, buf_len);
    }, static_cast<::std::uint64_t>(buf), static_cast<::std::uint64_t>(buf_len));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_t uwvm_wasip1_sched_yield_wasm64() noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sched_yield_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sched_yield_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sched_yield_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env());
    });
}

#  if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_sock_accept_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t sock_fd,
                                   ::uwvm2::imported::wasi::wasip1::abi::fdflags_wasm64_t fd_flags,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t ro_fd_ptrsz
#   ifdef UWVM_IMPORT_WASI_WASIP1_SUPPORT_WASIX_SOCKET
                                   ,
                                   ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t ro_addr_ptrsz
#   endif
                                   ) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sock_accept_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sock_accept_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sock_accept_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                     sock_fd,
                                                                     fd_flags,
                                                                     ro_fd_ptrsz
#   ifdef UWVM_IMPORT_WASI_WASIP1_SUPPORT_WASIX_SOCKET
                                                                     ,
                                                                     ro_addr_ptrsz
#   endif
    );
    }, static_cast<::std::uint64_t>(sock_fd), static_cast<::std::uint64_t>(fd_flags));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_sock_recv_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t sock_fd,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t ri_data_ptrsz,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t ri_data_len,
                                 ::uwvm2::imported::wasi::wasip1::abi::riflags_wasm64_t ri_flags,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t ro_data_len_ptrsz,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t ro_flags_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sock_recv_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sock_recv_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sock_recv_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                   sock_fd,
                                                                   ri_data_ptrsz,
                                                                   ri_data_len,
                                                                   ri_flags,
                                                                   ro_data_len_ptrsz,
                                                                   ro_flags_ptrsz);
    }, static_cast<::std::uint64_t>(sock_fd), static_cast<::std::uint64_t>(ri_data_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_sock_send_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t sock_fd,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t si_data_ptrsz,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_size_wasm64_t si_data_len,
                                 ::uwvm2::imported::wasi::wasip1::abi::siflags_wasm64_t si_flags,
                                 ::uwvm2::imported::wasi::wasip1::abi::wasi_void_ptr_wasm64_t ret_data_len_ptrsz) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sock_send_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sock_send_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sock_send_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(),
                                                                   sock_fd,
                                                                   si_data_ptrsz,
                                                                   si_data_len,
                                                                   si_flags,
                                                                   ret_data_len_ptrsz);
    }, static_cast<::std::uint64_t>(sock_fd), static_cast<::std::uint64_t>(si_data_ptrsz));
}

extern "C" ::uwvm2::imported::wasi::wasip1::abi::errno_wasm64_t
    uwvm_wasip1_sock_shutdown_wasm64(::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_wasm64_t sock_fd,
                                     ::uwvm2::imported::wasi::wasip1::abi::sdflags_wasm64_t how) noexcept
{
    if(!::uwvm2::uwvm::wasm::type::details::wasip1_host_api_call_is_allowed(u8"sock_shutdown_wasm64")) [[unlikely]] { return ::uwvm2::imported::wasi::wasip1::abi::errno_t::enotcapable; }

    return ::uwvm2::uwvm::wasm::type::details::invoke_wasip1_host_api(u8"sock_shutdown_wasm64", [&]() noexcept
    {
        return ::uwvm2::imported::wasi::wasip1::func::sock_shutdown_wasm64(::uwvm2::uwvm::imported::wasi::wasip1::storage::current_wasip1_env(), sock_fd, how);
    }, static_cast<::std::uint64_t>(sock_fd), static_cast<::std::uint64_t>(how));
}

#  endif
# endif
#endif

namespace uwvm2::uwvm::wasm::type
{
    extern "C" uwvm_wasip1_host_api_v1 const* uwvm_get_wasip1_host_api_v1() noexcept
    {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
        if(!details::wasip1_host_api_is_exposed()) [[unlikely]] { return nullptr; }

        static uwvm_wasip1_host_api_v1 const wasip1_host_api_v1{
            .struct_size = sizeof(uwvm_wasip1_host_api_v1),
            .abi_version = wasip1_host_api_v1_abi_version,
            .args_get = uwvm_wasip1_args_get,
            .args_sizes_get = uwvm_wasip1_args_sizes_get,
            .clock_res_get = uwvm_wasip1_clock_res_get,
            .clock_time_get = uwvm_wasip1_clock_time_get,
            .environ_get = uwvm_wasip1_environ_get,
            .environ_sizes_get = uwvm_wasip1_environ_sizes_get,
            .fd_advise = uwvm_wasip1_fd_advise,
            .fd_allocate = uwvm_wasip1_fd_allocate,
            .fd_close = uwvm_wasip1_fd_close,
            .fd_datasync = uwvm_wasip1_fd_datasync,
            .fd_fdstat_get = uwvm_wasip1_fd_fdstat_get,
            .fd_fdstat_set_flags = uwvm_wasip1_fd_fdstat_set_flags,
            .fd_fdstat_set_rights = uwvm_wasip1_fd_fdstat_set_rights,
            .fd_filestat_get = uwvm_wasip1_fd_filestat_get,
            .fd_filestat_set_size = uwvm_wasip1_fd_filestat_set_size,
            .fd_filestat_set_times = uwvm_wasip1_fd_filestat_set_times,
            .fd_pread = uwvm_wasip1_fd_pread,
            .fd_prestat_dir_name = uwvm_wasip1_fd_prestat_dir_name,
            .fd_prestat_get = uwvm_wasip1_fd_prestat_get,
            .fd_pwrite = uwvm_wasip1_fd_pwrite,
            .fd_read = uwvm_wasip1_fd_read,
            .fd_readdir = uwvm_wasip1_fd_readdir,
            .fd_renumber = uwvm_wasip1_fd_renumber,
            .fd_seek = uwvm_wasip1_fd_seek,
            .fd_sync = uwvm_wasip1_fd_sync,
            .fd_tell = uwvm_wasip1_fd_tell,
            .fd_write = uwvm_wasip1_fd_write,
            .path_create_directory = uwvm_wasip1_path_create_directory,
            .path_filestat_get = uwvm_wasip1_path_filestat_get,
            .path_filestat_set_times = uwvm_wasip1_path_filestat_set_times,
            .path_link = uwvm_wasip1_path_link,
            .path_open = uwvm_wasip1_path_open,
            .path_readlink = uwvm_wasip1_path_readlink,
            .path_remove_directory = uwvm_wasip1_path_remove_directory,
            .path_rename = uwvm_wasip1_path_rename,
            .path_symlink = uwvm_wasip1_path_symlink,
            .path_unlink_file = uwvm_wasip1_path_unlink_file,
            .poll_oneoff = uwvm_wasip1_poll_oneoff,
            .proc_exit = uwvm_wasip1_proc_exit,
            .proc_raise = uwvm_wasip1_proc_raise,
            .random_get = uwvm_wasip1_random_get,
            .sched_yield = uwvm_wasip1_sched_yield,
# if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
            .sock_accept = uwvm_wasip1_sock_accept,
            .sock_recv = uwvm_wasip1_sock_recv,
            .sock_send = uwvm_wasip1_sock_send,
            .sock_shutdown = uwvm_wasip1_sock_shutdown,
# endif
# if defined(UWVM_IMPORT_WASI_WASIP1_WASM64)
            .args_get_wasm64 = uwvm_wasip1_args_get_wasm64,
            .args_sizes_get_wasm64 = uwvm_wasip1_args_sizes_get_wasm64,
            .clock_res_get_wasm64 = uwvm_wasip1_clock_res_get_wasm64,
            .clock_time_get_wasm64 = uwvm_wasip1_clock_time_get_wasm64,
            .environ_get_wasm64 = uwvm_wasip1_environ_get_wasm64,
            .environ_sizes_get_wasm64 = uwvm_wasip1_environ_sizes_get_wasm64,
            .fd_advise_wasm64 = uwvm_wasip1_fd_advise_wasm64,
            .fd_allocate_wasm64 = uwvm_wasip1_fd_allocate_wasm64,
            .fd_close_wasm64 = uwvm_wasip1_fd_close_wasm64,
            .fd_datasync_wasm64 = uwvm_wasip1_fd_datasync_wasm64,
            .fd_fdstat_get_wasm64 = uwvm_wasip1_fd_fdstat_get_wasm64,
            .fd_fdstat_set_flags_wasm64 = uwvm_wasip1_fd_fdstat_set_flags_wasm64,
            .fd_fdstat_set_rights_wasm64 = uwvm_wasip1_fd_fdstat_set_rights_wasm64,
            .fd_filestat_get_wasm64 = uwvm_wasip1_fd_filestat_get_wasm64,
            .fd_filestat_set_size_wasm64 = uwvm_wasip1_fd_filestat_set_size_wasm64,
            .fd_filestat_set_times_wasm64 = uwvm_wasip1_fd_filestat_set_times_wasm64,
            .fd_pread_wasm64 = uwvm_wasip1_fd_pread_wasm64,
            .fd_prestat_dir_name_wasm64 = uwvm_wasip1_fd_prestat_dir_name_wasm64,
            .fd_prestat_get_wasm64 = uwvm_wasip1_fd_prestat_get_wasm64,
            .fd_pwrite_wasm64 = uwvm_wasip1_fd_pwrite_wasm64,
            .fd_read_wasm64 = uwvm_wasip1_fd_read_wasm64,
            .fd_readdir_wasm64 = uwvm_wasip1_fd_readdir_wasm64,
            .fd_renumber_wasm64 = uwvm_wasip1_fd_renumber_wasm64,
            .fd_seek_wasm64 = uwvm_wasip1_fd_seek_wasm64,
            .fd_sync_wasm64 = uwvm_wasip1_fd_sync_wasm64,
            .fd_tell_wasm64 = uwvm_wasip1_fd_tell_wasm64,
            .fd_write_wasm64 = uwvm_wasip1_fd_write_wasm64,
            .path_create_directory_wasm64 = uwvm_wasip1_path_create_directory_wasm64,
            .path_filestat_get_wasm64 = uwvm_wasip1_path_filestat_get_wasm64,
            .path_filestat_set_times_wasm64 = uwvm_wasip1_path_filestat_set_times_wasm64,
            .path_link_wasm64 = uwvm_wasip1_path_link_wasm64,
            .path_open_wasm64 = uwvm_wasip1_path_open_wasm64,
            .path_readlink_wasm64 = uwvm_wasip1_path_readlink_wasm64,
            .path_remove_directory_wasm64 = uwvm_wasip1_path_remove_directory_wasm64,
            .path_rename_wasm64 = uwvm_wasip1_path_rename_wasm64,
            .path_symlink_wasm64 = uwvm_wasip1_path_symlink_wasm64,
            .path_unlink_file_wasm64 = uwvm_wasip1_path_unlink_file_wasm64,
            .poll_oneoff_wasm64 = uwvm_wasip1_poll_oneoff_wasm64,
            .proc_exit_wasm64 = uwvm_wasip1_proc_exit_wasm64,
            .proc_raise_wasm64 = uwvm_wasip1_proc_raise_wasm64,
            .random_get_wasm64 = uwvm_wasip1_random_get_wasm64,
            .sched_yield_wasm64 = uwvm_wasip1_sched_yield_wasm64,
#  if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
            .sock_accept_wasm64 = uwvm_wasip1_sock_accept_wasm64,
            .sock_recv_wasm64 = uwvm_wasip1_sock_recv_wasm64,
            .sock_send_wasm64 = uwvm_wasip1_sock_send_wasm64,
            .sock_shutdown_wasm64 = uwvm_wasip1_sock_shutdown_wasm64,
#  endif
# endif
        };

        return ::std::addressof(wasip1_host_api_v1);
#else
        return nullptr;
#endif
    }
}  // namespace uwvm2::uwvm::wasm::type

namespace uwvm2::uwvm::wasm::type
{
#if defined(UWVM_RUNTIME_HAS_BACKEND)
    extern "C" ::std::size_t uwvm_preload_memory_descriptor_count() noexcept { return ::uwvm2::runtime::lib::preload_memory_descriptor_count_host_api(); }

    extern "C" bool uwvm_preload_memory_descriptor_at(::std::size_t descriptor_index, uwvm_preload_memory_descriptor_t* out) noexcept
    { return ::uwvm2::runtime::lib::preload_memory_descriptor_at_host_api(descriptor_index, out); }

    extern "C" bool uwvm_preload_memory_read(::std::size_t memory_index, ::std::uint_least64_t offset, void* destination, ::std::size_t size) noexcept
    { return ::uwvm2::runtime::lib::preload_memory_read_host_api(memory_index, offset, destination, size); }

    extern "C" bool uwvm_preload_memory_write(::std::size_t memory_index, ::std::uint_least64_t offset, void const* source, ::std::size_t size) noexcept
    { return ::uwvm2::runtime::lib::preload_memory_write_host_api(memory_index, offset, source, size); }
#else
    extern "C" ::std::size_t uwvm_preload_memory_descriptor_count() noexcept { return 0uz; }

    extern "C" bool uwvm_preload_memory_descriptor_at([[maybe_unused]] ::std::size_t descriptor_index,
                                                      [[maybe_unused]] uwvm_preload_memory_descriptor_t* out) noexcept
    { return false; }

    extern "C" bool uwvm_preload_memory_read([[maybe_unused]] ::std::size_t memory_index,
                                             [[maybe_unused]] ::std::uint_least64_t offset,
                                             [[maybe_unused]] void* destination,
                                             [[maybe_unused]] ::std::size_t size) noexcept
    { return false; }

    extern "C" bool uwvm_preload_memory_write([[maybe_unused]] ::std::size_t memory_index,
                                              [[maybe_unused]] ::std::uint_least64_t offset,
                                              [[maybe_unused]] void const* source,
                                              [[maybe_unused]] ::std::size_t size) noexcept
    { return false; }
#endif

    extern "C" uwvm_preload_host_api_v1 const* uwvm_get_preload_host_api_v1() noexcept
    {
        static uwvm_preload_host_api_v1 const preload_host_api_v1{
            .struct_size = sizeof(uwvm_preload_host_api_v1),
            .abi_version = preload_host_api_v1_abi_version,
            .memory_descriptor_count = uwvm_preload_memory_descriptor_count,
            .memory_descriptor_at = uwvm_preload_memory_descriptor_at,
            .memory_read = uwvm_preload_memory_read,
            .memory_write = uwvm_preload_memory_write,
        };

        return ::std::addressof(preload_host_api_v1);
    }
}  // namespace uwvm2::uwvm::wasm::type

#ifndef UWVM_MODULE
# include <uwvm2/uwvm/runtime/macro/pop_macros.h>
# include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
#endif
