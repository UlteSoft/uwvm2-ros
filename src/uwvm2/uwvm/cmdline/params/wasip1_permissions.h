// Explicit WASI Preview 1 authority configuration. Licensed under APL-2.0 (see LICENSE file)
#pragma once
#ifndef UWVM_MODULE
# include <memory>
# include <uwvm2/utils/macro/push_macros.h>
# ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
#  include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
# endif
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/utils/cmdline/impl.h>
# include <uwvm2/uwvm/imported/wasi/wasip1/storage/impl.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params
{
#ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
# if defined(UWVM_IMPORT_WASI_WASIP1)
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_mount_dir_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_single_mount_dir_rights{
        .name{u8"--wasip1-single-mount-dir-rights"},
        .describe{u8"Set explicit base/inheriting rights for one previously declared preopen; other roots are unchanged."},
        .usage{u8"<single:str> <guest-dir:str> <base:u64> <inheriting:u64>"},
        .handle = ::std::addressof(details::wasip1_single_mount_dir_rights_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_mount_dir_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_group_mount_dir_rights{
        .name{u8"--wasip1-group-mount-dir-rights"},
        .describe{u8"Set explicit base/inheriting rights for one previously declared preopen; other roots are unchanged."},
        .usage{u8"<group:str> <guest-dir:str> <base:u64> <inheriting:u64>"},
        .handle = ::std::addressof(details::wasip1_group_mount_dir_rights_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    // These options configure authority, independently of import visibility and
    // native API exposure. Permissions have finite names; there is no wildcard.
    // Repeat grant/deny only for distinct bits. One bit cannot be configured
    // twice in a target, even when the two requested values are identical.
    namespace details
    {
#  if defined(UWVM_MODULE)
        extern "C++"
#  else
        inline constexpr
#  endif
            ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_grant_callback(
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#  if defined(UWVM_MODULE)
        extern "C++"
#  else
        inline constexpr
#  endif
            ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_deny_callback(
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#  if defined(UWVM_MODULE)
        extern "C++"
#  else
        inline constexpr
#  endif
            ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_grant_callback(
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#  if defined(UWVM_MODULE)
        extern "C++"
#  else
        inline constexpr
#  endif
            ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_deny_callback(
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#  if defined(UWVM_MODULE)
        extern "C++"
#  else
        inline constexpr
#  endif
            ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_grant_callback(
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#  if defined(UWVM_MODULE)
        extern "C++"
#  else
        inline constexpr
#  endif
            ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_deny_callback(
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
#  if defined(UWVM_MODULE)
        extern "C++"
#  else
        inline constexpr
#  endif
            ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_inherit_system_environment_callback(
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*,
                ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_limit_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_single_limit{
        .name{u8"--wasip1-single-limit"}, .describe{u8"Set one explicit WASI resource limit (decimal or unlimited; no implicit grant)."},
        .usage{u8"<single:str> <limit:str> <value:u64|unlimited>"}, .handle = ::std::addressof(details::wasip1_single_limit_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_limit_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_group_limit{
        .name{u8"--wasip1-group-limit"}, .describe{u8"Set one explicit WASI resource limit (decimal or unlimited; no implicit grant)."},
        .usage{u8"<group:str> <limit:str> <value:u64|unlimited>"}, .handle = ::std::addressof(details::wasip1_group_limit_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_limit_action_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_single_limit_action{
        .name{u8"--wasip1-single-limit-action"}, .describe{u8"Choose reject or synchronous wait for an explicit operation limit; wait supports windows and concurrency."},
        .usage{u8"<single:str> <limit:str> <reject|wait>"}, .handle = ::std::addressof(details::wasip1_single_limit_action_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_limit_action_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_group_limit_action{
        .name{u8"--wasip1-group-limit-action"}, .describe{u8"Choose reject or synchronous wait for an explicit operation limit; wait supports windows and concurrency."},
        .usage{u8"<group:str> <limit:str> <reject|wait>"}, .handle = ::std::addressof(details::wasip1_group_limit_action_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_limit_exit_action_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_single_limit_exit_action{
        .name{u8"--wasip1-single-limit-exit-action"}, .describe{u8"Choose cancel or wait on guest exit for one window/concurrency quota or native-wait; default cancel."},
        .usage{u8"<single:str> <limit:str> <cancel|wait>"}, .handle = ::std::addressof(details::wasip1_single_limit_exit_action_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_limit_exit_action_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_group_limit_exit_action{
        .name{u8"--wasip1-group-limit-exit-action"}, .describe{u8"Choose cancel or wait on guest exit for one window/concurrency quota or native-wait; default cancel."},
        .usage{u8"<group:str> <limit:str> <cancel|wait>"}, .handle = ::std::addressof(details::wasip1_group_limit_exit_action_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_global_grant{
        .name{u8"--wasip1-global-grant"},
        .describe{u8"Explicitly grant one finite WASI permission for the global default."},
        .usage{u8"<permission:str>"},
        .handle = ::std::addressof(details::wasip1_global_grant_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_global_deny{
        .name{u8"--wasip1-global-deny"},
        .describe{u8"Explicitly deny one finite WASI permission for the global default."},
        .usage{u8"<permission:str>"},
        .handle = ::std::addressof(details::wasip1_global_deny_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_single_grant{
        .name{u8"--wasip1-single-grant"},
        .describe{u8"Explicitly grant one finite WASI permission for the single module."},
        .usage{u8"<module:str> <permission:str>"},
        .handle = ::std::addressof(details::wasip1_single_grant_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_single_deny{
        .name{u8"--wasip1-single-deny"},
        .describe{u8"Explicitly deny one finite WASI permission for the single module."},
        .usage{u8"<module:str> <permission:str>"},
        .handle = ::std::addressof(details::wasip1_single_deny_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_group_grant{
        .name{u8"--wasip1-group-grant"},
        .describe{u8"Explicitly grant one finite WASI permission for the named group."},
        .usage{u8"<group:str> <permission:str>"},
        .handle = ::std::addressof(details::wasip1_group_grant_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_group_deny{
        .name{u8"--wasip1-group-deny"},
        .describe{u8"Explicitly deny one finite WASI permission for the named group."},
        .usage{u8"<group:str> <permission:str>"},
        .handle = ::std::addressof(details::wasip1_group_deny_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_global_inherit_system_environment{
        .name{u8"--wasip1-global-inherit-system-environment"},
        .describe{u8"Explicitly inherit host environment variables for the global WASI default."},
        .handle = ::std::addressof(details::wasip1_global_inherit_system_environment_callback),
        .is_exist = ::std::addressof(::uwvm2::uwvm::imported::wasi::wasip1::storage::wasip1_noinherit_system_environment_is_set),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
# endif
#endif
}
#ifndef UWVM_MODULE
# ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
#  include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# endif
# include <uwvm2/utils/macro/pop_macros.h>
#endif
