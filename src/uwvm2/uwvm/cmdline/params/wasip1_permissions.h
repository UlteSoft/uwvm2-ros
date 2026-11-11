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
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_socket_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_single_socket_rights{
        .name{u8"--wasip1-single-socket-rights"},
        .describe{u8"Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces."},
        .usage{u8"<single:str> <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>"},.handle=::std::addressof(details::wasip1_single_socket_rights_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_socket_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_group_socket_rights{
        .name{u8"--wasip1-group-socket-rights"},
        .describe{u8"Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces."},
        .usage{u8"<group:str> <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>"},.handle=::std::addressof(details::wasip1_group_socket_rights_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_global_socket_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_global_socket_rights{
        .name{u8"--wasip1-global-socket-rights"},
        .describe{u8"Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces."},
        .usage{u8"<initial-fd:i32> <base:csv|none> <accepted-child:csv|none>"},.handle=::std::addressof(details::wasip1_global_socket_rights_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_socket_abi_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_socket_abi{
        .name{u8"--wasip1-socket-abi"},
        .describe{u8"Choose the immutable wasm32 sock_accept ABI for this process: standard (default, 3 arguments) or wasix (4 arguments, peer address). Grants no interfaces or socket rights."},
        .usage{u8"<standard|wasix>"},.handle=::std::addressof(details::wasip1_socket_abi_callback),
        .is_exist=::std::addressof(::uwvm2::uwvm::imported::wasi::wasip1::storage::wasip1_socket_abi_is_set),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};

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
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_single_interface_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_single_interface{
        .name{u8"--wasip1-single-interface"},
        .describe{u8"Configure one exact WASIp1 API: hidden fails linking; deny exposes but rejects; allow retains resource checks. Default hidden."},
        .usage{u8"<single:str> <api:str> <hidden|deny|allow>"},
        .handle = ::std::addressof(details::wasip1_single_interface_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
#  if defined(UWVM_MODULE)
    extern "C++"
#  else
    inline constexpr
#  endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_group_interface_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept; }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_group_interface{
        .name{u8"--wasip1-group-interface"},
        .describe{u8"Configure one exact WASIp1 API: hidden fails linking; deny exposes but rejects; allow retains resource checks. Default hidden."},
        .usage{u8"<group:str> <api:str> <hidden|deny|allow>"},
        .handle = ::std::addressof(details::wasip1_group_interface_callback),
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

#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    // Global is one explicitly bound environment, never an ambient default.
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_add_module_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_add_module{
        .name{u8"--wasip1-global-add-module"},
        .describe{u8"Bind a module to the shared Global WASIp1 environment. Conflicts with Single/Group membership; grants no authority."},
        .usage{u8"<module:str>"}, .handle=::std::addressof(details::wasip1_ros_global_add_module_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_enable_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_enable{
        .name{u8"--wasip1-global-enable"},
        .describe{u8"Enable WASI Preview 1 for the Global group."},
        .usage{u8""}, .handle=::std::addressof(details::wasip1_ros_global_enable_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_disable_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_disable{
        .name{u8"--wasip1-global-disable"},
        .describe{u8"Disable WASI Preview 1 for the Global group."},
        .usage{u8""}, .handle=::std::addressof(details::wasip1_ros_global_disable_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_interface_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_interface{
        .name{u8"--wasip1-global-interface"},
        .describe{u8"Configure one exact WASIp1 API: hidden fails linking; deny exposes but rejects; allow retains resource checks. Default hidden."},
        .usage{u8"<api:str> <hidden|deny|allow>"}, .handle=::std::addressof(details::wasip1_ros_global_interface_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_noinherit_system_environment_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_noinherit_system_environment{
        .name{u8"--wasip1-global-noinherit-system-environment"},
        .describe{u8"Do not inherit host environment variables into the Global group."},
        .usage{u8""}, .handle=::std::addressof(details::wasip1_ros_global_noinherit_system_environment_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_inherit_system_environment_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_inherit_system_environment{
        .name{u8"--wasip1-global-inherit-system-environment"},
        .describe{u8"Inherit host environment variables into the Global group."},
        .usage{u8""}, .handle=::std::addressof(details::wasip1_ros_global_inherit_system_environment_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_disable_utf8_check_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_disable_utf8_check{
        .name{u8"--wasip1-global-disable-utf8-check"},
        .describe{u8"Disable WASI Preview 1 UTF-8 checks for the Global group."},
        .usage{u8""}, .handle=::std::addressof(details::wasip1_ros_global_disable_utf8_check_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_enable_utf8_check_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_enable_utf8_check{
        .name{u8"--wasip1-global-enable-utf8-check"},
        .describe{u8"Enable WASI Preview 1 UTF-8 checks for the Global group."},
        .usage{u8""}, .handle=::std::addressof(details::wasip1_ros_global_enable_utf8_check_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_set_argv0_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_set_argv0{
        .name{u8"--wasip1-global-set-argv0"},
        .describe{u8"Override WASI Preview 1 argv[0] for the Global group."},
        .usage{u8"<argv0:str>"}, .handle=::std::addressof(details::wasip1_ros_global_set_argv0_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_force_args_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    void wasip1_ros_global_force_args_pretreatment(char8_t const* const*&, char8_t const* const*,
        ::uwvm2::utils::container::vector<::uwvm2::utils::cmdline::parameter_parsing_results>&) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_force_args{
        .name{u8"--wasip1-global-force-args"},
        .describe{u8"Replace the Global group's WASI Preview 1 argv vector with an explicit argument list."},
        .usage{u8"<num:size_t> [arg:str]..."}, .handle=::std::addressof(details::wasip1_ros_global_force_args_callback),
        .pretreatment=::std::addressof(details::wasip1_ros_global_force_args_pretreatment),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_set_fd_limit_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_set_fd_limit{
        .name{u8"--wasip1-global-set-fd-limit"},
        .describe{u8"Set the WASI Preview 1 fd limit for the Global group."},
        .usage{u8"<limit:size_t>"}, .handle=::std::addressof(details::wasip1_ros_global_set_fd_limit_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_add_or_replace_environment_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_add_or_replace_environment{
        .name{u8"--wasip1-global-add-or-replace-environment"},
        .describe{u8"Add or replace one environment variable for the Global group."},
        .usage{u8"<env:str> <value:str>"}, .handle=::std::addressof(details::wasip1_ros_global_add_or_replace_environment_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_delete_system_environment_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_delete_system_environment{
        .name{u8"--wasip1-global-delete-system-environment"},
        .describe{u8"Delete one inherited host environment variable for the Global group."},
        .usage{u8"<env:str>"}, .handle=::std::addressof(details::wasip1_ros_global_delete_system_environment_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_mount_dir_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_mount_dir{
        .name{u8"--wasip1-global-mount-dir"},
        .describe{u8"Mount a host directory into the Global group's WASI Preview 1 sandbox."},
        .usage{u8"<wasi dir:str> <system dir:path>"}, .handle=::std::addressof(details::wasip1_ros_global_mount_dir_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_mount_dir_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_mount_dir_rights{
        .name{u8"--wasip1-global-mount-dir-rights"},
        .describe{u8"Set explicit base/inheriting rights for one previously declared preopen; other roots are unchanged."},
        .usage{u8"<guest-dir:str> <base:u64> <inheriting:u64>"}, .handle=::std::addressof(details::wasip1_ros_global_mount_dir_rights_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_socket_rights_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_socket_rights{
        .name{u8"--wasip1-global-socket-rights"},
        .describe{u8"Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces."},
        .usage{u8"<initial-fd:i32> <base:csv|none> <accepted-child:csv|none>"}, .handle=::std::addressof(details::wasip1_ros_global_socket_rights_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
# if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_socket_tcp_listen_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_socket_tcp_listen{
        .name{u8"--wasip1-global-socket-tcp-listen"},
        .describe{u8"Listen on a WASI Preview 1 TCP server socket for the Global group."},
        .usage{u8"<fd:i32> [<ipv4|ipv6>:<port>|unix <path>]"}, .handle=::std::addressof(details::wasip1_ros_global_socket_tcp_listen_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
# endif
# if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_socket_tcp_connect_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_socket_tcp_connect{
        .name{u8"--wasip1-global-socket-tcp-connect"},
        .describe{u8"Connect a WASI Preview 1 TCP client socket for the Global group."},
        .usage{u8"<fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]"}, .handle=::std::addressof(details::wasip1_ros_global_socket_tcp_connect_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
# endif
# if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_socket_udp_bind_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_socket_udp_bind{
        .name{u8"--wasip1-global-socket-udp-bind"},
        .describe{u8"Bind a WASI Preview 1 UDP socket for the Global group."},
        .usage{u8"<fd:i32> [<ipv4|ipv6>:<port>|unix <path>]"}, .handle=::std::addressof(details::wasip1_ros_global_socket_udp_bind_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
# endif
# if defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_socket_udp_connect_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_socket_udp_connect{
        .name{u8"--wasip1-global-socket-udp-connect"},
        .describe{u8"Connect a WASI Preview 1 UDP socket for the Global group."},
        .usage{u8"<fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]"}, .handle=::std::addressof(details::wasip1_ros_global_socket_udp_connect_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
# endif
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_grant_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_grant{
        .name{u8"--wasip1-global-grant"},
        .describe{u8"Explicitly grant one finite WASI permission for the Global group."},
        .usage{u8"<permission:str>"}, .handle=::std::addressof(details::wasip1_ros_global_grant_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_deny_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_deny{
        .name{u8"--wasip1-global-deny"},
        .describe{u8"Explicitly deny one finite WASI permission for the Global group."},
        .usage{u8"<permission:str>"}, .handle=::std::addressof(details::wasip1_ros_global_deny_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_limit_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_limit{
        .name{u8"--wasip1-global-limit"},
        .describe{u8"Set one explicit WASI resource limit (decimal or unlimited; no implicit grant)."},
        .usage{u8"<limit:str> <value:u64|unlimited>"}, .handle=::std::addressof(details::wasip1_ros_global_limit_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_limit_action_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_limit_action{
        .name{u8"--wasip1-global-limit-action"},
        .describe{u8"Choose reject or synchronous wait for an explicit operation limit; wait supports windows and concurrency."},
        .usage{u8"<limit:str> <reject|wait>"}, .handle=::std::addressof(details::wasip1_ros_global_limit_action_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
    namespace details {
# if defined(UWVM_MODULE)
    extern "C++"
# else
    inline constexpr
# endif
    ::uwvm2::utils::cmdline::parameter_return_type wasip1_ros_global_limit_exit_action_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasip1_ros_global_limit_exit_action{
        .name{u8"--wasip1-global-limit-exit-action"},
        .describe{u8"Choose cancel or wait on guest exit for one window/concurrency quota or native-wait; default cancel."},
        .usage{u8"<limit:str> <cancel|wait>"}, .handle=::std::addressof(details::wasip1_ros_global_limit_exit_action_callback),
        .cate=::uwvm2::utils::cmdline::categorization::wasi};
#endif
# endif
#endif
}
#ifndef UWVM_MODULE
# ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
#  include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
# endif
# include <uwvm2/utils/macro/pop_macros.h>
#endif
