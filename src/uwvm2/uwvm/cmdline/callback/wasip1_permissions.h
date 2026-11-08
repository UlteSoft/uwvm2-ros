// Explicit authority callbacks use the same target dispatcher as other WASI options.
// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/push_macros.h>
# ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
#  include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
# endif
# include <uwvm2/uwvm/cmdline/params/impl.h>
# include <uwvm2/uwvm/imported/wasi/wasip1/storage/impl.h>
# include "wasip1_single_common.h"
# include "wasip1_group_common.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params::details
{
#ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
# if defined(UWVM_IMPORT_WASI_WASIP1)
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_socket_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_single_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_single_socket_rights,current,end,wasip1_module_details::target_action_t::socket_rights); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_socket_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_group_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_group_socket_rights,current,end,wasip1_module_details::target_action_t::socket_rights); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_socket_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        return wasip1_module_details::apply_socket_rights(::uwvm2::uwvm::cmdline::params::wasip1_global_socket_rights,
            ::uwvm2::uwvm::imported::wasi::wasip1::storage::default_wasip1_env,current+1u,end);
#else
        return wasip1_module_details::print_usage_error(::uwvm2::uwvm::cmdline::params::wasip1_global_socket_rights,u8"Requires ROS socket policy.");
#endif
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_socket_abi_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        using token=::uwvm2::utils::cmdline::parameter_parsing_results_type;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET) && defined(__linux__)
        if(current+1u==end || current[1].type!=token::arg || (current[1].str!=u8"standard"&&current[1].str!=u8"wasix"))
            return wasip1_module_details::print_usage_error(::uwvm2::uwvm::cmdline::params::wasip1_socket_abi,u8"Choose exactly standard or wasix; no automatic signature fallback.");
        ::uwvm2::uwvm::imported::wasi::wasip1::storage::wasip1_wasix_socket_abi=current[1].str==u8"wasix";
        current[1].type=token::occupied_arg;
        return ::uwvm2::utils::cmdline::parameter_return_type::def;
#else
        return wasip1_module_details::print_usage_error(::uwvm2::uwvm::cmdline::params::wasip1_socket_abi,u8"Runtime socket ABI selection requires the Linux ROS socket provider.");
#endif
    }

#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_mount_dir_rights_callback(
        [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current, ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_single_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_single_mount_dir_rights, current, end,
        wasip1_module_details::target_action_t::mount_dir_rights); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_mount_dir_rights_callback(
        [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current, ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_group_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_group_mount_dir_rights, current, end,
        wasip1_module_details::target_action_t::mount_dir_rights); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_limit_callback(
        [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current, ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_single_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_single_limit, current, end,
        wasip1_module_details::target_action_t::set_resource_limit); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_limit_callback(
        [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current, ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_group_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_group_limit, current, end,
        wasip1_module_details::target_action_t::set_resource_limit); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_limit_action_callback(
        [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current, ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_single_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_single_limit_action, current, end,
        wasip1_module_details::target_action_t::set_resource_limit_action); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_limit_action_callback(
        [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current, ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_group_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_group_limit_action, current, end,
        wasip1_module_details::target_action_t::set_resource_limit_action); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_limit_exit_action_callback(
        [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current, ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_single_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_single_limit_exit_action, current, end,
        wasip1_module_details::target_action_t::set_resource_limit_exit_action); }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_limit_exit_action_callback(
        [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current, ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_group_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_group_limit_exit_action, current, end,
        wasip1_module_details::target_action_t::set_resource_limit_exit_action); }
    namespace wasip1_permission_details
    {
        // Parse and consume one permission operand. The configured-bit mask is
        // separate from the permission value, so explicit deny and duplicate
        // grant are distinguishable even when the safe default value is zero.
        [[nodiscard]] inline constexpr ::uwvm2::utils::cmdline::parameter_return_type
            apply_global(::uwvm2::utils::cmdline::parameter const& parameter,
                         ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
                         ::uwvm2::utils::cmdline::parameter_parsing_results* para_end,
                         bool grant) noexcept
        {
            namespace storage = ::uwvm2::uwvm::imported::wasi::wasip1::storage;
            using permission = ::uwvm2::imported::wasi::wasip1::environment::permission_t;
            using result = ::uwvm2::utils::cmdline::parameter_return_type;
            using token_type = ::uwvm2::utils::cmdline::parameter_parsing_results_type;
            auto const operand{para_curr + 1u};
            if(operand == para_end || operand->type != token_type::arg) [[unlikely]]
            { return wasip1_module_details::print_usage_error(parameter, u8"Missing WASI permission name."); }
            auto const bit{storage::parse_wasip1_permission(::uwvm2::utils::container::u8string_view{operand->str})};
            if(bit == permission{}) [[unlikely]]
            { return wasip1_module_details::print_usage_error(parameter, u8"Unknown WASI permission name; wildcards are not supported."); }
            if((storage::wasip1_global_permission_presence & bit) != permission{}) [[unlikely]]
            { return wasip1_module_details::print_usage_error(parameter, u8"Duplicate or conflicting global grant/deny for this WASI permission."); }
            storage::wasip1_global_permission_presence = storage::wasip1_global_permission_presence | bit;
            auto& value{storage::default_wasip1_env.non_fd_permissions};
            value = grant ? value | bit : value & ~bit;
            operand->type = token_type::occupied_arg;
            return result::def;
        }
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
        ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_grant_callback(
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_begin,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_end) noexcept
    {
        return wasip1_permission_details::apply_global(::uwvm2::uwvm::cmdline::params::wasip1_global_grant, para_curr, para_end, true);
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
        ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_deny_callback(
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_begin,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_end) noexcept
    {
        return wasip1_permission_details::apply_global(::uwvm2::uwvm::cmdline::params::wasip1_global_deny, para_curr, para_end, false);
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
        ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_grant_callback(
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_begin,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_end) noexcept
    {
        return wasip1_single_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_single_grant, para_curr, para_end,
            wasip1_module_details::target_action_t::grant_permission);
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
        ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_deny_callback(
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_begin,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_end) noexcept
    {
        return wasip1_single_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_single_deny, para_curr, para_end,
            wasip1_module_details::target_action_t::deny_permission);
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
        ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_grant_callback(
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_begin,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_end) noexcept
    {
        return wasip1_group_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_group_grant, para_curr, para_end,
            wasip1_module_details::target_action_t::grant_permission);
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
        ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_deny_callback(
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_begin,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_end) noexcept
    {
        return wasip1_group_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_group_deny, para_curr, para_end,
            wasip1_module_details::target_action_t::deny_permission);
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
        ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_inherit_system_environment_callback(
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_begin,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_end) noexcept
    {
        // The parser has already claimed the shared presence marker. A safe
        // default alone never occupies that marker, and opposite/repeated
        // options fail during parsing before any Guest initialization.
        ::uwvm2::uwvm::imported::wasi::wasip1::storage::wasip1_noinherit_system_environment = false;
        return ::uwvm2::utils::cmdline::parameter_return_type::def;
    }
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
        ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_noinherit_system_environment_callback(
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_begin,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_curr,
            [[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* para_end) noexcept
    {
        // The parser has already claimed the shared presence marker. A safe
        // default alone never occupies that marker, and opposite/repeated
        // options fail during parsing before any Guest initialization.
        ::uwvm2::uwvm::imported::wasi::wasip1::storage::wasip1_noinherit_system_environment = true;
        return ::uwvm2::utils::cmdline::parameter_return_type::def;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_interface_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_single_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_single_interface,current,end,
        wasip1_module_details::target_action_t::interface); }
    inline constexpr ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_interface_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    { return wasip1_group_details::apply_action(::uwvm2::uwvm::cmdline::params::wasip1_group_interface,current,end,
        wasip1_module_details::target_action_t::interface); }
# endif
#endif
}
#ifndef UWVM_MODULE
# ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
#  include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# endif
# include <uwvm2/utils/macro/pop_macros.h>
#endif
