/** @file
 * @brief Pure projections of the immediate caller's other host policies.
 * @details These readers accept const policy objects. They contain no loader,
 * admission, revocation, filesystem, network or native invocation operation.
 * They do not export argv/environ values, host paths, payloads or addresses.
 * ROSv1 rows describe configured grants; ROSv2 separately reports its live
 * revocations. Approved WASI descriptor rights are labelled as initial rights.
 */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/scene/permission_report.h>
#include <uwvm2/utils/control/host_interfaces.h>
#include <uwvm2/uwvm/wasm/storage/all_module.h>
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
#include <uwvm2/uwvm/imported/wasi/wasip1/storage/env.h>
#endif
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
#include <uwvm2/uwvm/imported/ros/storage.h>
#endif
#endif
namespace uwvm2::uwvm::imported::scene::permission_observer
{
    namespace report=::uwvm2::imported::ros::scene::permission_report;
    namespace api=::uwvm2::utils::control::host_interfaces;
    using error=::uwvm2::imported::ros::scene::error;
    inline error unsupported(report::page& out)noexcept
    {return out.finish(false,false)?error::unsupported:error::too_large;}
    /** @brief Read exact import exposure, separately from operation authority. */
    inline error interfaces(report::query const& query,report::page& out,api::family group,
                            api::policy const& policy,bool enabled)noexcept
    {
        if(!query.function.empty())
        {
            auto index=api::find(group,query.function);if(index==api::invalid)return error::unsupported;
            if(!out.add("interface",query.function,static_cast<unsigned>(enabled?policy.entries[index]:api::access::hidden)))return error::too_large;
        }
        else for(std::size_t i{};i<api::catalogue.size();++i)
        {
            auto const& item=api::catalogue[i];if(item.group!=group)continue;
            if(!out.add("interface",item.key,static_cast<unsigned>(enabled?policy.entries[i]:api::access::hidden)))return error::too_large;
        }
        return error::success;
    }
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
    template<typename Environment>
    error wasip1(report::query const& query,report::page& out,Environment const& env)noexcept
    {
        auto status=interfaces(query,out,api::family::wasip1,env.interfaces,env.interface_enabled);
        if(status!=error::success)return status;
        if(!query.function.empty())return out.finish(true,env.interface_enabled)?error::success:error::too_large;
        if(!out.add("permission","non-fd-mask",static_cast<std::uint64_t>(env.non_fd_permissions)))return error::too_large;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        auto const& limits=env.limits;
        constexpr std::array scopes{std::string_view{"random"},std::string_view{"stdin"},std::string_view{"stdout"},std::string_view{"stderr"},
            std::string_view{"realtime"},std::string_view{"monotonic"},std::string_view{"process"},std::string_view{"thread"},std::string_view{"poll"},
            std::string_view{"signal"},std::string_view{"yield"},std::string_view{"filesystem"},std::string_view{"socket-read"},std::string_view{"socket-write"},
            std::string_view{"socket-accept"},std::string_view{"socket-shutdown"}};
        static_assert(scopes.size()==std::tuple_size_v<decltype(limits.operations)>);
        constexpr std::array keys{std::string_view{"request-bytes"},std::string_view{"window-bytes"},std::string_view{"window-calls"},
            std::string_view{"total-bytes"},std::string_view{"total-calls"},std::string_view{"concurrent"}};
        for(std::size_t i{};i<scopes.size();++i)
        {
            auto const& value=limits.operations[i];
            std::array values{value.request_bytes,value.window_bytes,value.window_calls,value.total_bytes,value.total_calls,value.concurrent};
            for(std::size_t j{};j<values.size();++j)
                if(!out.add(scopes[i],keys[j],values[j])||!out.add(scopes[i],keys[j],static_cast<unsigned>(value.actions[j]),"limit-action")||
                   !out.add(scopes[i],keys[j],static_cast<unsigned>(value.exit_actions[j]),"exit-action"))return error::too_large;
            if(!out.add(scopes[i],"native-wait-exit-action",static_cast<unsigned>(value.native_wait_exit_action)))return error::too_large;
        }
        for(std::size_t i{};i<4;++i)if(!out.add(scopes[i+4],"clock-granularity-ns",limits.clock_granularity_ns[i]))return error::too_large;
        if(!out.add("limit","max-poll-subscriptions",limits.max_poll_subscriptions)||!out.add("limit","max-poll-wait-ns",limits.max_poll_wait_ns)||
           !out.add("limit","max-args-env-bytes",limits.max_args_env_bytes)||!out.add("permission","filesystem-base-rights",limits.filesystem_base_rights)||
           !out.add("permission","filesystem-inheriting-rights",limits.filesystem_inheriting_rights))return error::too_large;
        for(auto const& resource:env.policy_resources)
            if(!out.add("approved-resource","logical-slot",resource.logical_slot)||!out.add("approved-resource","kind",resource.kind)||
               !out.add("approved-resource","initial-base-rights",resource.rights_base)||
               !out.add("approved-resource","initial-inheriting-rights",resource.rights_inheriting))return error::too_large;
#endif
        return out.finish(true,env.interface_enabled)?error::success:error::too_large;
    }
#endif
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
    inline error legacy(report::query const& query,report::page& out,::uwvm2::imported::ros::target_policy const* target)
    {
        api::policy empty;auto status=interfaces(query,out,query.target==report::family::ros1v1?api::family::ros1:api::family::ros2,
            target?target->interfaces:empty,target&&target->enabled);
        if(status!=error::success)return status;
        if(target&&query.function.empty())
        {
            if(!out.add("self","scope",static_cast<unsigned>(target->target_scope),::uwvm2::imported::ros::to_string(target->target_scope))||
               !out.add("self","permission-group",0,target->name))return error::too_large;
            for(auto const& [alias,cap]:target->capabilities)
            {
                if(!out.add(alias,"configured-operation",target->enabled,cap.operation)||!out.add(alias,"resource",0,cap.resource)||
                   !out.add(alias,"type",0,cap.type)||!out.add(alias,"type-identity",0,cap.type_identity))return error::too_large;
                for(auto const& [key,value]:cap.limits)if(!out.add(alias,key,0,value))return error::too_large;
            }
        }
        return out.finish(true,target&&target->enabled)?error::success:error::too_large;
    }
#endif
    /** @brief Observe one compiled family, without selecting or starting it. */
    inline error read(std::string_view module,report::query const& query,report::page& out,bool other=false)
    {
        (void)module;(void)other;
        switch(query.target)
        {
        case report::family::preloadwasm:
            if(!query.function.empty())return error::unsupported;
            return out.add("feature","query-only",1)&&out.finish(true,true)?error::success:error::too_large;
        case report::family::preloaddl:
#if defined(UWVM_SUPPORT_PRELOAD_DL)
            if(!query.function.empty())return error::unsupported;
            return out.add("feature","query-only",1)&&out.add("feature","isolated",0)&&out.finish(true,true)?error::success:error::too_large;
#else
            return unsupported(out);
#endif
        case report::family::wasip1:
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
            {
                namespace storage=::uwvm2::uwvm::imported::wasi::wasip1::storage;
                using kind=::uwvm2::uwvm::wasm::type::module_type_t;
                ::uwvm2::utils::container::u8string_view name{reinterpret_cast<char8_t const*>(module.data()),module.size()};
                auto found=::uwvm2::uwvm::wasm::storage::all_module.find(name);
                if(found==::uwvm2::uwvm::wasm::storage::all_module.end())return error::bad_handle;
                storage::wasip1_module_target_kind_t target;
                if(found->second.type==kind::exec_wasm)target=storage::wasip1_module_target_kind_t::main_wasm;
                else if(found->second.type==kind::preloaded_wasm)target=storage::wasip1_module_target_kind_t::preload_wasm;
#if defined(UWVM_SUPPORT_PRELOAD_DL)
                else if(found->second.type==kind::preloaded_dl)target=storage::wasip1_module_target_kind_t::preloaded_dl;
#endif
                else return error::unsupported;
                auto const* policy=storage::find_wasip1_module_override_const(target,name);
                if(query.function.empty())
                {
                    if(!out.add("self","module",0,module))return error::too_large;
                    if(policy)
                    {
                        auto const& group=policy->trace_wasip1_group_name_storage;
                        if(!out.add("self","permission-scope",static_cast<unsigned>(policy->trace_wasip1_group_kind))||
                           !out.add("self","permission-group",0,{reinterpret_cast<char const*>(group.data()),group.size()}))return error::too_large;
                    }
                    else if(!out.add("self","permission-scope",0,"unconfigured"))return error::too_large;
                }
                return wasip1(query,out,policy?policy->env:storage::default_wasip1_env);
            }
#else
            return unsupported(out);
#endif
        case report::family::ros1v1:
#if defined(UWVM_IMPORT_ROS1)
            return legacy(query,out,::uwvm2::uwvm::imported::ros::runtime_policy.effective(::uwvm2::imported::ros::version::ros1,module));
#else
            return unsupported(out);
#endif
        case report::family::ros2v1:
#if defined(UWVM_IMPORT_ROS2)
            return legacy(query,out,::uwvm2::uwvm::imported::ros::runtime_policy.effective(::uwvm2::imported::ros::version::ros2,module));
#else
            return unsupported(out);
#endif
        case report::family::rosv2:return error::invalid; // The deployment owns its locked projection.
        }
        return error::unsupported;
    }
}
