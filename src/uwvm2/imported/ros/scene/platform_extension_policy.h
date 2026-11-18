/** @file
 * @brief ROSv2 child-module declarations narrowing common isolated-driver routes.
 * @details A module identity is an update/ABI boundary, never an authority
 * owner. The parent environment owns all grants, handles, controller leases,
 * budgets, receipts and revocation. Enabling a child only removes an additional
 * restriction; it cannot create or reopen a common driver capability.
 *
 * Registration is trusted build-time input. No file, plugin, SDK directory,
 * guest string or CLI option can register an implementation. This policy stores
 * closed routes to existing typed device/control operations, not arbitrary
 * opcodes or native function pointers. These declarations do not register a
 * vendor implementation. All native vendor binaries, including open-source
 * builds, execute outside the VM. Portable algorithms execute in Wasm; the
 * common dispatcher authenticates every handle and source witness. Platform
 * selection cannot relax confinement, mint authority or reset fault history.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#include "robot_provider_contract.h"
#include "authority.h"
#include "platform_module.h"

namespace uwvm2::imported::ros::scene::platform_extensions
{
    struct route
    {
        std::string_view name;
        robot_contract::operation common;
    };
    /** @brief Compiled operation routes; registering these grants no authority.
     * @details The definition and its strings are copied into fast_io-backed
     * owned storage. Lifetimes do not depend on the caller's spans. Only exact
     * operation names and the existing closed device/control carriers are legal.
     */
    struct definition {std::string_view module;std::span<route const> routes;};
    /** @brief Shared typed permission vocabulary; no vendor implementation is admitted.
     * @details The external driver still needs an exact parent resource grant,
     * pinned endpoint, semantic schema and independent receiver safety policy.
     * Every constituent operation keeps its ordinary grant and revocation.
     */
    inline constexpr std::array foundation_routes{
        route{"device.open",robot_contract::operation::device_open},
        route{"device.submit",robot_contract::operation::device_submit},
        route{"device.result",robot_contract::operation::device_result},
        route{"device.stop",robot_contract::operation::device_stop},
        route{"device.describe",robot_contract::operation::device_describe},
        route{"control.open",robot_contract::operation::control_open},
        route{"control.sample",robot_contract::operation::control_sample},
        route{"control.submit",robot_contract::operation::control_submit},
        route{"control.result",robot_contract::operation::control_result},
        route{"control.stop",robot_contract::operation::control_stop}};
    class policy
    {
        struct owned_route {::uwvm2::utils::container::std_string name;robot_contract::operation common;};
        struct child
        {
            ::uwvm2::utils::container::std_string module;
            ::uwvm2::utils::container::std_vector<owned_route> routes;
            std::size_t bindings{};
            bool selected{},enabled{};
        };
        ::uwvm2::utils::container::std_vector<child> children_;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,std::size_t,std::less<>> owners_;
        bool sealed_{};
        child* find(std::string_view name)noexcept
        {for(auto& item:children_)if(item.module==name)return &item;return nullptr;}
        child const* find(std::string_view name)const noexcept
        {for(auto const& item:children_)if(item.module==name)return &item;return nullptr;}
        /** @brief Materialize only a selected foundation profile; unused platforms allocate nothing. */
        error ensure_foundation(std::string_view module)noexcept
        {
            if(find(module))return error::success;
            if(!foundation_module(module))return error::unsupported;
            try{return register_module({module,foundation_routes});}
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
    public:
        /** @brief Copy a trusted declaration atomically before configuration is sealed.
         * @details No fixed 64-entry registration array is used. Allocation
         * failures propagate to the recoverable CLI/provider boundary; a failed
         * copy leaves the previous catalogue intact. Duplicate names and invalid
         * enum values are rejected before indexing any common contract.
         */
        error register_module(definition const& source)
        {
            if(sealed_)return error::denied;
            if(!module_name(source.module)||source.routes.empty())return error::invalid;
            if(find(source.module))return error::conflict;
            child value;value.module.assign(source.module);
            for(auto const& item:source.routes)
            {
                if(!identifier(item.name)||item.common>robot_contract::operation::control_stop)return error::invalid;
                for(auto const& existing:value.routes)if(existing.name==item.name)return error::conflict;
                value.routes.push_back({::uwvm2::utils::container::std_string(item.name),item.common});
            }
            children_.push_back(std::move(value));return error::success;
        }
        /** @brief Select a child once; disabled and absent are both zero authority. */
        error select(std::string_view module,bool enabled)noexcept
        {
            if(sealed_)return error::denied;
            if(auto status=ensure_foundation(module);status!=error::success)return status;
            auto* item=find(module);
            if(item->selected)return error::conflict;
            item->selected=true;item->enabled=enabled;return error::success;
        }
        /** @brief Narrow a child to an already granted common device alias.
         * @details This never configures, opens, broadens or clones the grant.
         * The dispatcher rechecks the live parent grant on every call. One
         * device has one platform owner. Control routes resolve their existing
         * controller's device; no second control/device resource is created.
         */
        error bind(std::string_view module,grant const& permission)
        {
            if(sealed_)return error::denied;
            if(!identifier(permission.alias)||!((permission.kind==purpose::device&&permission.binding=="framed-scalar")||
               (permission.kind==purpose::driver&&permission.binding=="registered-driver")))return error::denied;
            if(auto status=ensure_foundation(module);status!=error::success)return status;
            auto* item=find(module);
            auto index=static_cast<std::size_t>(item-children_.data());
            if(!owners_.emplace(permission.alias,index).second)return error::conflict;
            ++item->bindings;return error::success;
        }
        /** @brief Freeze child restrictions before exposing any provider.
         * @details An enabled module needs an explicit resource binding. An
         * unused registered module remains disabled. No native work is started.
         */
        error seal()noexcept
        {
            if(sealed_)return error::success;
            for(auto const& item:children_)if(item.enabled&&!item.bindings)return error::invalid;
            sealed_=true;return error::success;
        }
        /** @brief Resolve a route only within a sealed, enabled child and exact parent alias.
         * @details This result is not an admission ticket. The common broker
         * must still check the caller, exposure, grant generation, scope and
         * limits. No operation, output parameter or counter changes on failure.
         */
        error resolve(std::string_view module,std::string_view name,std::string_view alias,robot_contract::operation& output)const noexcept
        {
            if(!sealed_)return error::denied;
            auto const* item=find(module);auto owner=owners_.find(alias);
            if(!item||!item->enabled||owner==owners_.end()||&children_[owner->second]!=item)return error::denied;
            for(auto const& entry:item->routes)if(entry.name==name){output=entry.common;return error::success;}
            return error::denied;
        }
        /** @brief Look up a closed carrier before the live parent capability is checked.
         * @details The result grants nothing. The dispatcher must subsequently
         * resolve the alias obtained from the live parent admission, never an
         * alias supplied by guest memory.
         */
        error operation(std::string_view module,std::string_view name,robot_contract::operation& output)const noexcept
        {
            if(!sealed_)return error::denied;
            auto const* item=find(module);if(!item||!item->enabled)return error::denied;
            for(auto const& entry:item->routes)if(entry.name==name){output=entry.common;return error::success;}
            return error::denied;
        }
        /** @brief Apply child ownership to common entry points as well as extension carriers.
         * @details An unbound common resource is unaffected. A bound resource
         * has exactly one platform owner and is unavailable while that child
         * is disabled. Calling a common device/control import cannot bypass the
         * switch. Startup preparation uses this check before native activation.
         */
        bool allows(std::string_view alias)const noexcept
        {auto found=owners_.find(alias);return found==owners_.end()||children_[found->second].enabled;}
        /** @brief Restrict an attached driver to one of the child's compiled semantic routes.
         * @details A platform switch never authorizes an opaque native command.
         * The common channel must name the same canonical device/control operation.
         */
        bool contract_matches(std::string_view alias,std::string_view semantic)const noexcept
        {
            auto found=owners_.find(alias);if(found==owners_.end())return true;
            for(auto const& route:children_[found->second].routes)
                if(robot_contract::operations[static_cast<std::size_t>(route.common)].name==semantic)return true;
            return false;
        }
        std::string_view owner(std::string_view alias)const noexcept
        {auto found=owners_.find(alias);return found==owners_.end()?std::string_view{}:std::string_view(children_[found->second].module);}
        /** @brief Read cached configuration only, without granting or activating a provider. */
        template<class Visitor> bool visit_selections(Visitor visit)const
        {
            for(auto const& child:children_)if(child.selected&&!visit(child.module,child.enabled))return false;
            return true;
        }
        bool enabled(std::string_view module)const noexcept
        {auto const* item=find(module);return item&&item->selected&&item->enabled;}
    };
}
