/** @file
 * @brief Explicit environment configuration for uwvm_ros_v2.
 * @details Environments have exact module membership; there is no global
 * fallback or inheritance from WASI or ROS-v1. Configuration is trusted CLI
 * input, frozen before the first guest admission. Unknown fields, conflicting
 * bindings, duplicate settings and unavailable providers are rejected.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include "broker.h"
#include "bound_queries.h"
#include "debug.h"
#include "call_audit.h"
#include "model_policy.h"
#include "transform_policy.h"
#include "log_diagnostic_configuration.h"
#include "recording_codecs.h"
#include "sensor_deployment_configuration.h"
#include "media_configuration.h"
#include "bridge_configuration.h"
#include "device_configuration.h"
#include "configuration_file.h"
#include "model_attachment_configuration.h"
#include "communication_deployment_provider.h"
#include "communication_creation_control_contract.h"
#include "parameter_deployment_configuration.h"
#include <algorithm>
#include <charconv>

namespace uwvm2::imported::ros::scene
{
    /** @brief Enumerate the same contract names admitted by scene interface configuration. */
    template<typename Visitor> constexpr void visit_callable_names(Visitor visit)
    {
        visit("abi");visit("capability.open");visit("capability.close");visit(permission_report::import_name);
        for(auto action:permission_report::module_operations)visit(action);
        for(auto action:module_exchange::names)visit(action);
        for(auto action:drivers::names)visit(action);
        visit("module.permission.report");visit(permission_report::vm_import_name);
        for(auto const& entry:revocation_contract::operations)visit(entry.name);
        for(auto const& entry:runtime_contract::operations)visit(entry.name);
        for(auto const& entry:artifact_contract::operations)visit(entry.name);
        for(auto const& entry:output_contract::operations)visit(entry.name);
        for(auto const& entry:managed_lifecycle_contract::operations)visit(entry.name);
        for(auto const& entry:buffer_contract::operations)visit(entry.name);
        for(auto const& entry:model_contract::operations)visit(entry.name);
        for(auto const& entry:transform_contract::operations)visit(entry.name);
        for(auto const& entry:log_diagnostic_contract::operations)visit(entry.name);
        for(auto const& entry:recording_contract::operations)visit(entry.name);
        for(auto const& entry:sensor_contract::operations)visit(entry.name);
        for(auto const& entry:robot_contract::operations)
            if(entry.id<=robot_contract::operation::control_stop)visit(entry.name);
        for(auto const& entry:robot_contract::operations)
            if(entry.id>=robot_contract::operation::media_open&&entry.id<=robot_contract::operation::media_close)visit(entry.name);
        for(auto const& entry:robot_contract::operations)
            if(entry.id>=robot_contract::operation::bridge_create&&entry.id<=robot_contract::operation::bridge_describe)visit(entry.name);
        // Removed: VM-owned native component process lifecycle imports.
        for(auto const& entry:proposal_contract::operations)visit(entry.name);
        for(auto const& entry:communication_contract::operations)visit(entry.name);
        for(auto const& entry:communication_receive_contract::operations)visit(entry.name);
        for(auto const& entry:communication_creation_control_contract::operations)visit(entry.name);
        for(auto const& entry:parameter_contract::operations)visit(entry.name);
        for(auto const& entry:action_contract::operations)visit(entry.name);
        for(auto const& entry:graph_contract::operations)visit(entry.name);
        for(auto const& entry:event_contract::operations)visit(entry.name);
    }
    inline constexpr auto callable_names=[]() constexpr
    {
        constexpr auto count=[]() constexpr {std::size_t n{};visit_callable_names([&](std::string_view){++n;});return n;}();
        std::array<std::string_view,count> result{};std::size_t i{};
        visit_callable_names([&](std::string_view name){result[i++]=name;});
        std::sort(result.begin(),result.end());
        return result;
    }();
    static_assert(std::adjacent_find(callable_names.begin(),callable_names.end())==callable_names.end());
    static_assert(std::all_of(callable_names.begin(),callable_names.end(),[](auto name){return !name.empty()&&name.size()<=64;}));
    inline bool callable(std::string_view name) noexcept
    {return std::binary_search(callable_names.begin(),callable_names.end(),name);}
    inline std::optional<purpose> parse_purpose(std::string_view text) noexcept
    {
        for(std::size_t i{};i<purpose_names.size();++i)if(text==purpose_names[i])return static_cast<purpose>(i);
        return {};
    }
    inline bool parse_unsigned(std::string_view text,std::uint64_t& value) noexcept
    {
        if(text=="unlimited"){value=UINT64_MAX;return true;}
        auto parsed=std::from_chars(text.data(),text.data()+text.size(),value);
        return !text.empty()&&parsed.ec==std::errc{}&&parsed.ptr==text.data()+text.size();
    }
    struct environment
    {
        ::uwvm2::utils::container::std_string name;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,exposure,std::less<>> interfaces;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,grant,std::less<>> grants;
        permission_report::policy observation;
        std::uint64_t module_call_bytes{},module_call_routes{};bool module_call_budget_set{};
        /// @brief Configuration-only summary; mixed routes resolve their exact admitted resource.
        enum class input_policy:std::uint8_t{clear,retain,mixed};
        input_policy module_inputs{input_policy::clear};
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,module_exchange::plan,std::less<>> module_call_plans;
        ::uwvm2::utils::container::std_set<std::pair<::uwvm2::utils::container::std_string,::uwvm2::utils::container::std_string>> assigned_limits;
        std::shared_ptr<broker> live;
        std::shared_ptr<driver_dispatch> drivers;
        ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> driver_aliases;
        std::uint64_t generation{};
        std::uint64_t identity{},input_bytes{},input_objects{};
        communication_deployment_configuration communication;
        parameter_deployment_configuration parameters;
        bool input_budget_set{},output_budget_set{},publication_budget_set{};
        std::uint64_t publication_bytes{},publication_calls{};
        ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> publication_plans;
        bool buffer_budget_set{};
        std::uint64_t buffer_bytes{},buffer_objects{},buffer_workspace{},buffer_leases{};
        std::uint64_t output_bytes{},output_objects{},output_receipt_bytes{};
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,outputs::plan,std::less<>> output_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,managed_lifecycle::plan,std::less<>> managed_lifecycle_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,buffers::plan,std::less<>> buffer_plans;
        bool model_budget_set{};
        bool sensor_budget_set{},proposal_budget_set{};
        sensor_dispatch::limits sensor_capacity;
        bool media_budget_set{};
        media_dispatch::limits media_capacity;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,media_configuration,std::less<>> media_plans;
        bool bridge_budget_set{};
        bridge_dispatch::limits bridge_capacity;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,bridge_configuration,std::less<>> bridge_plans;
        bool device_budget_set{};
        platform_extensions::policy platform_children;
        device_dispatch::limits device_capacity;
        device_supervisor::plan device_supervision;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,device_configuration,std::less<>> device_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,control_configuration,std::less<>> control_plans;
        proposal_dispatch::limits proposal_capacity;
        proposals::guest_limits proposal_guest_capacity;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,sensor_deployment_configuration,std::less<>> sensor_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,proposals::configuration,std::less<>> proposal_plans;
        ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> proposal_definitions;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,proposals::arbiter_plan,std::less<>> proposal_arbiter_plans;
        bool transform_budget_set{};
        bool log_diagnostic_budget_set{};
        log_diagnostic_budget_configuration log_diagnostic_budget;
        bool recording_budget_set{};
        recording_budget_configuration recording_budget;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,recording::configuration,std::less<>> recording_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,recording::replay_definition,std::less<>> replay_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,logs::configuration,std::less<>> log_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,diagnostics::configuration,std::less<>> diagnostic_plans;
        transforms::budget transform_capacity;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,transforms::configuration,std::less<>> transform_plans;
        models::budget model_capacity;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,models::plan,std::less<>> model_plans;
        ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> model_capacity_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,model_attachment::configuration,std::less<>> model_worker_plans;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,artifacts::input_plan,std::less<>> input_plans;
    };
    /** @brief Resolve import exposure for a member of this environment.
     * @details Public ABI discovery and the caller's own report default to
     * allow. All resource and foreign metadata imports require explicit exposure.
     */
    inline std::pair<exposure,bool> interface_exposure(environment const& entry,std::string_view action)
    {
        auto rule=entry.interfaces.find(action);
        if(rule!=entry.interfaces.end())return {rule->second,true};
        return {is_bound_query(action)?exposure::allow:exposure::hidden,false};
    }
    /** @brief Frozen scene deployment with explicit, nonoverlapping environments.
     * @details Creating an environment, binding a module, exposing a function
     * and granting its resource are separate steps. No configuration operation
     * opens a socket, reads guest-controlled files or loads a native provider.
     */
    class deployment
    {
        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,environment,std::less<>> environments;
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,::uwvm2::utils::container::std_string,std::less<>> memberships;
        call_audit::journal result_audit;
        bool frozen{},retired{},providers_ready{};
        bool const declarations_only{};
        std::optional<module_buffer_audit::limits> payload_journal;
        inline static std::atomic<std::uint64_t> next_environment{1};
        static std::uint64_t environment_identity() noexcept
        {
            auto value=next_environment.load(std::memory_order_relaxed);
            while(value&&value!=UINT64_MAX)
                if(next_environment.compare_exchange_weak(value,value+1,std::memory_order_relaxed))return value;
            return 0;
        }
        static bool implemented(grant const& value) noexcept
        {
            if(value.kind==purpose::parameter&&value.binding=="local-parameters")
            {
                for(auto const& action:value.actions)
                {
                    if(action=="capability.revoke"||action=="resource.close"||action=="request.describe"||action=="request.release")continue;
                    bool found{};for(auto const& operation:parameter_contract::operations)found|=operation.name==action;
                    if(!found)return false;
                }
                return true;
            }
            if(broker::communication_policy(value))
            {
                for(auto const& action:value.actions)
                {
                    if(action=="capability.revoke"||action=="resource.close"||action=="request.describe"||action=="request.release")continue;
                    bool found{};
                    for(auto const& operation:communication_contract::operations)found|=operation.name==action;
                    for(auto const& operation:communication_receive_contract::operations)found|=operation.name==action;
                    for(auto const& operation:action_contract::operations)found|=operation.name==action;
                    for(auto const& operation:graph_contract::operations)found|=operation.name==action;
                    for(auto const& operation:event_contract::operations)found|=operation.name==action;
                    found|=action=="resource.observe"||(value.kind==purpose::topic&&action=="data.export");
                    if(value.kind==purpose::action)for(auto const name:action_contract::transition_actions)found|=name==action;
                    for(auto const& operation:communication_creation_control_contract::operations)found|=operation.name==action;
                    if(!found||!permits(value.kind,action))return false;
                }
                return true;
            }
            if(value.kind==purpose::sensor&&value.binding=="bounded-sensor")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action!="sensor.open"&&action!="sensor.take"&&action!="sensor.describe"&&action!="resource.close"&&action!="resource.observe"&&action!="data.export"))return false;return true;}
            if(value.kind==purpose::media&&value.binding=="immutable-buffer-media")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&action!="resource.close"&&action!="media.open"&&action!="media.submit"&&action!="media.take"&&action!="media.close")return false;return true;}
            if(value.kind==purpose::bridge&&value.binding=="topic-forwarder")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&action!="resource.close"&&action!="bridge.create"&&action!="bridge.start"&&action!="bridge.stop"&&action!="bridge.describe")return false;return true;}
            if((value.kind==purpose::device&&value.binding=="framed-scalar")||(value.kind==purpose::control&&value.binding=="framed-control"))
            {
                for(auto const& action:value.actions)
                {
                    if(action=="capability.revoke"||action=="resource.close")continue;
                    bool found{};for(auto const& op:robot_contract::operations)
                        found|=op.id<=robot_contract::operation::control_stop&&op.name==action&&permits(value.kind,action);
                    if(!found)return false;
                }
                return true;
            }
            if(value.kind==purpose::component)return false;
            if(value.kind==purpose::driver&&value.binding=="registered-driver")
            {for(auto const& action:value.actions)if(!permits(purpose::driver,action))return false;return true;}
            if(value.kind==purpose::proposal&&value.binding=="model-proposals")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action!="proposal.capture"&&action!="proposal.publish"&&action!="proposal.read"&&action!="resource.close"))return false;return true;}
            if(value.kind==purpose::arbiter&&value.binding=="proposal-arbiter")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action!="proposal.accept"&&action!="request.describe"&&action!="request.release"))return false;return true;}
            if(value.kind==purpose::recording&&value.binding=="private-recording")
            {
                for(auto const& action:value.actions)
                    if(action!="capability.revoke"&&(action!="resource.close"&&action!="resource.observe"&&action!="data.export"&&
                       action!="recording.open"&&action!="recording.append"&&action!="recording.checkpoint"&&action!="recording.stop"))return false;
                return true;
            }
            if(value.kind==purpose::replay&&value.binding=="private-replay")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action!="resource.close"&&action!="replay.open"&&action!="replay.read"&&action!="replay.seek"))return false;return true;}
            if(value.kind==purpose::log&&value.binding=="private-log")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action!="log.emit"&&action!="resource.observe"))return false;return true;}
            if(value.kind==purpose::diagnostic&&value.binding=="local-diagnostic")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action!="diagnostic.read"))return false;return true;}
            if(value.kind==purpose::transform&&value.binding=="local-transform")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action=="resource.observe"||action=="data.export"||!permits(value.kind,action)))return false;return true;}
            if(value.kind==purpose::module_inspection)return value.binding=="metadata"&&!value.limits.calls&&!value.limits.bytes;
            if(value.kind==purpose::vm_inspection)return value.binding=="metadata"&&value.resource=="runtime"&&!value.limits.calls&&!value.limits.bytes;
            if(value.kind==purpose::module_call)return value.binding=="registered-message";
            if(value.kind<=purpose::wait)return value.binding=="local";
            if(value.kind==purpose::lifecycle&&value.binding=="local-lifecycle")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action!="resource.close"&&!action.starts_with("lifecycle.")))return false;return true;}
            if(value.kind==purpose::buffer&&value.binding=="sealed-buffer")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(!permits(purpose::buffer,action)))return false;return true;}
            if((value.kind==purpose::language_model||value.kind==purpose::vision_model||value.kind==purpose::world_model)&&value.binding=="local-model")
            {
                for(auto const& action:value.actions)
                    if(action!="capability.revoke"&&(action!="resource.close"&&action!="request.describe"&&action!="request.release"&&
                       action!="model.open"&&action!="model.submit"&&action!="model.take"&&action!="model.cancel"&&action!="model.describe"))return false;
                return true;
            }
#if defined(__linux__)
            if(value.kind==purpose::cache&&value.binding=="volatile-output")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(action!="artifact.read"&&action!="artifact.begin"&&action!="artifact.append"&&action!="artifact.commit"&&action!="artifact.abort"&&action!="artifact.remove"&&action!="artifact.status"&&action!="artifact.receipt"&&action!="artifact.publish"))return false;return true;}
            if(artifacts::purpose_code(value.kind)&&value.binding=="sealed-input")
            {for(auto const& action:value.actions)if(action!="capability.revoke"&&(!artifacts::input_action(action)))return false;return true;}
#endif
            return false;
        }
        static error configure(environment const& entry,std::shared_ptr<broker>& next)
        {
            auto children=entry.platform_children;
            if(auto status=children.seal();status!=error::success)return status;
            if(entry.drivers)if(auto status=entry.drivers->validate_platforms(children);status!=error::success)return status;
            next=::uwvm2::utils::container::make_shared<broker>();
            for(auto const& [action,state]:entry.interfaces)
                if(auto result=next->permissions.expose(action,state);result!=error::success)return result;
            for(auto const& [alias,cap]:entry.grants)
                if(auto result=next->permissions.configure(cap);result!=error::success)return result;
            for(auto const& [name,plan]:entry.module_call_plans)
            { (void)name;if(auto status=next->module_calls.bind(plan,entry.module_call_bytes);status!=error::success)return status; }
            for(auto const& [alias,cap]:entry.grants)
            { (void)alias;if(cap.kind==purpose::module_call)next->module_calls.bind_grant(cap); }
            if(!entry.communication.empty())
            {
                auto settings=entry.communication;std::uint64_t bytes{};
                if(!settings.frozen())if(auto status=settings.freeze(bytes);status!=error::success)return status;
                if(auto status=next->configure_communication_dispatch(entry.identity,settings.host_limits());status!=error::success)return status;
                if(auto status=settings.bind_hosts(*next->communication_store);status!=error::success)return status;
            }
            if(!entry.parameters.entries().empty())
            {
                if(auto status=next->configure_parameter_dispatch(entry.parameters.bindings());status!=error::success)return status;
                if(auto status=entry.parameters.bind(*next->parameter_store);status!=error::success)return status;
            }
            for(auto const& [alias,plan]:entry.managed_lifecycle_plans)
                if(auto result=next->managed_nodes.bind(alias,plan);result!=error::success)return result;
            for(auto const& [alias,plan]:entry.buffer_plans)
                if(auto result=next->buffer_store.bind(alias,plan);result!=error::success)return result;
            if(!entry.media_plans.empty())
            {
                if(auto status=next->configure_media_dispatch(entry.identity,entry.media_capacity);status!=error::success)return status;
                for(auto const& [alias,setting]:entry.media_plans)
                {
                    (void)alias;media_dispatch::plan plan;
                    if(auto status=setting.freeze(plan);status!=error::success)return status;
                    if(auto status=next->media_store->bind(plan);status!=error::success)return status;
                }
                if(auto status=next->media_store->prepare();status!=error::success)return status;
            }
            if(!entry.bridge_plans.empty())
            {
                if(auto status=next->configure_bridge_dispatch(entry.identity,entry.bridge_capacity);status!=error::success)return status;
                for(auto const& [alias,setting]:entry.bridge_plans)
                {
                    (void)alias;bridge_dispatch::plan plan;
                    if(auto status=setting.freeze(plan);status!=error::success)return status;
                    if(auto status=next->bridge_store->bind(plan);status!=error::success)return status;
                }
                if(auto status=next->bridge_store->prepare();status!=error::success)return status;
            }
            if(!entry.device_plans.empty())
            {
                if(auto status=next->configure_device_dispatch(entry.identity,entry.device_capacity,entry.device_supervision);status!=error::success)return status;
                if(auto status=next->device_store->configure_extensions(std::move(children));status!=error::success)return status;
            }
            for(auto const& [alias,plan]:entry.model_plans)
                if(auto result=next->model_store.bind(alias,plan);result!=error::success)return result;
            if(!entry.sensor_plans.empty())
                if(auto result=next->configure_sensor_dispatch(entry.identity,entry.sensor_capacity);result!=error::success)return result;
            if(!entry.proposal_plans.empty()||!entry.proposal_arbiter_plans.empty())
            {
                if(auto result=next->configure_proposal_dispatch(entry.identity,entry.proposal_capacity,entry.proposal_guest_capacity);result!=error::success)return result;
                ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> targets;
                for(auto const& [alias,setting]:entry.proposal_plans)
                {
                    auto prepared=setting;proposals::plan plan;
                    if(auto result=prepared.freeze(plan);result!=error::success)return result;
                    if(auto result=next->proposal_store->bind(alias,plan);result!=error::success)return result;
                    for(std::size_t index{};index<plan.tool_count;++index)
                    {
                        auto const& tool=plan.tools[index];
                        if(!targets.emplace(tool.target_alias.view()).second)continue;
                        proposal_lifecycle_target::configuration target{tool.target_alias,tool.target_profile,entry.managed_lifecycle_plans.find(tool.target_alias.view())->second};
                        if(auto result=next->proposal_target_store->bind(target);result!=error::success)return result;
                    }
                }
                for(auto const& [alias,plan]:entry.proposal_arbiter_plans)
                    if(auto result=next->proposal_store->bind_arbiter(alias,plan);result!=error::success)return result;
            }
            for(auto const& [alias,setting]:entry.transform_plans)
            {
                auto prepared=setting;
                if(!prepared.frozen())if(auto result=prepared.freeze();result!=error::success)return result;
                if(auto result=next->transform_store.bind(alias,*prepared.limits());result!=error::success)return result;
            }
            if(!entry.log_plans.empty()||!entry.diagnostic_plans.empty())
            {
                auto budget=entry.log_diagnostic_budget;log_diagnostic_dispatch::limits capacity;
                if(auto result=budget.freeze(capacity);result!=error::success)return result;
                if(auto result=next->configure_log_diagnostic(entry.identity,capacity);result!=error::success)return result;
                for(auto const& [alias,setting]:entry.log_plans)
                {
                    auto prepared=setting;logs::plan plan;
                    if(auto result=prepared.freeze(plan);result!=error::success)return result;
                    if(auto result=next->log_diagnostic_store->bind_log(alias,plan);result!=error::success)return result;
                }
                for(auto const& [alias,setting]:entry.diagnostic_plans)
                {
                    auto prepared=setting;diagnostics::plan plan;logs::name source;
                    if(auto result=prepared.freeze(plan,source);result!=error::success)return result;
                    if(auto result=next->log_diagnostic_store->bind_diagnostic(alias,source.view(),plan);result!=error::success)return result;
                }
                if(auto result=next->log_diagnostic_store->prepare();result!=error::success)return result;
            }
            if(!entry.recording_plans.empty()||!entry.replay_plans.empty())
            {
                auto budget=entry.recording_budget;recording_dispatch::limits capacity;
                if(auto result=budget.freeze(capacity);result!=error::success)return result;
                if(auto result=next->configure_recording_dispatch(entry.identity,capacity);result!=error::success)return result;
                for(auto const& [alias,setting]:entry.recording_plans)
                {
                    auto prepared=setting;recording::definition definition;
                    if(auto result=prepared.freeze(definition);result!=error::success)return result;
                    if(auto result=definition.bind(*next->recording_store,alias);result!=error::success)return result;
                }
                for(auto const& [alias,setting]:entry.replay_plans)
                    if(auto result=next->recording_store->bind_replay(alias,setting.source.view(),setting.policy);result!=error::success)return result;
                if(auto result=next->recording_store->prepare();result!=error::success)return result;
            }
            return error::success;
        }
        /** @brief Validate real source/target dependencies without probing trusted descriptors.
         * @details No definition creates a grant. Sources must use an existing
         * sealed-buffer plan, proposals the exact configured successful-output
         * codec, and targets an actual local lifecycle client graph. Physical
         * targets, arbitrary endpoint strings and dynamic validators are refused.
         */
        /** @brief Resolve bridge directions against frozen native endpoint profiles without I/O.
         * @details Fresh dedicated endpoints are required. A configured source
         * cannot also be a target, and equal topic names on equal ROS domains
         * are refused. External native-network loops require deployment review.
         */
        static error validate_bridge(environment const& entry,std::uint64_t& total_bytes)
        {
            if(entry.bridge_plans.empty())return error::success;
            auto const& capacity=entry.bridge_capacity;
            if(!entry.bridge_budget_set||entry.bridge_plans.size()>capacity.bindings||!capacity.poll_ns||capacity.poll_ns>100000000||
                capacity.supervisor_bytes<262144||capacity.supervisor_bytes>64u*1024u*1024u||capacity.supervisor_bytes%4096)return error::invalid;
            std::uint64_t retained{};::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> endpoints;
            for(auto const& [alias,setting]:entry.bridge_plans)
            {
                (void)alias;bridge_dispatch::plan plan;auto status=setting.freeze(plan);if(status!=error::success)return status;
                if(retained>capacity.storage_bytes||plan.storage_bytes>capacity.storage_bytes-retained)return error::too_large;
                retained+=plan.storage_bytes;
                communication::topic_plan const* source{};communication::topic_plan const* target{};
                std::array<std::uint64_t,2> domains{};
                for(unsigned i=0;i<2;++i)
                {
                    auto const& name=i?plan.destination_alias:plan.source_alias;auto const& profile=i?plan.destination_profile:plan.source_profile;
                    if(!endpoints.emplace(name.view()).second)return error::conflict;
                    auto definition=entry.communication.definition(name.view());auto grant=entry.grants.find(name.view());
                    if(!definition||definition->kind!=communication_configuration::family::topic||grant==entry.grants.end()||
                        grant->second.kind!=purpose::topic||grant->second.binding!="native-communication")return error::denied;
                    auto const& native=std::get<communication::topic_plan>(definition->policy);
                    if(native.selected.identity!=profile.identity.view()||native.selected.digest!=profile.sha256||
                        native.direction!=(i?communication::role::publisher:communication::role::subscription)||
                        native.max_message_bytes!=plan.message_bytes||native.max_deadline_ns<plan.deadline_ns)return error::invalid;
                    (i?target:source)=&native;
                    auto binding=entry.communication.bindings().find(name.view());if(binding==entry.communication.bindings().end())return error::denied;
                    auto worker=entry.communication.workers().find(binding->second.worker_name);if(worker==entry.communication.workers().end())return error::denied;
                    domains[i]=worker->second.definition.network.domain;bool schema_found{};auto codec=communication::codecs::find(plan.codec.view());
                    for(auto const& item:worker->second.definition.definitions)if(item.kind==communication_configuration::family::type)
                    {auto const& type=std::get<communication::type_plan>(item.policy);if(type.selected==native.type)
                        schema_found=type.type_identity==codec->identity&&type.encoding==codec->encoding&&type.validate==codec->validate;}
                    if(!schema_found)return error::denied;
                }
                if(domains[0]==domains[1]&&source->resolved_name==target->resolved_name)return error::conflict;
            }
            for(auto bytes:{retained,capacity.supervisor_bytes}){if(bytes>INT64_MAX-total_bytes)return error::too_large;total_bytes+=bytes;}
            return error::success;
        }
        /** @brief Validate scalar/control profiles, exact source cohorts and reserved safety work without OS I/O. */
        static error validate_device(environment const& entry,std::uint64_t& total_bytes)
        {
            if(entry.device_plans.empty()&&entry.control_plans.empty())return error::success;
            if(!entry.device_budget_set||entry.device_plans.size()>entry.device_capacity.devices||
                entry.control_plans.size()>entry.device_capacity.controllers||entry.device_plans.size()!=entry.control_plans.size())return error::invalid;
            std::uint64_t retained{},stop_bound{};
            ::uwvm2::utils::container::std_set<int> descriptors;
            ::uwvm2::utils::container::std_set<::uwvm2::utils::container::std_string,std::less<>> selected;
            ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,device_dispatch::device_plan,std::less<>> devices;
            auto reserve=[&](std::uint64_t bytes){if(retained>entry.device_capacity.retained_bytes||bytes>entry.device_capacity.retained_bytes-retained)return false;retained+=bytes;return true;};
            for(auto const& [alias,setting]:entry.sensor_plans)
            {
                (void)alias;auto copy=setting;sensor_dispatch::configuration plan;int fd{};
                if(auto status=copy.freeze(plan,fd);status!=error::success)return status;
                descriptors.emplace(fd);
            }
            for(auto const& [alias,setting]:entry.device_plans)
            {
                device_dispatch::device_plan plan;std::array<int,4> endpoints;
                if(auto status=setting.freeze(plan,endpoints);status!=error::success)return status;
                for(auto fd:endpoints)if(!descriptors.emplace(fd).second)return error::conflict;
                if(!reserve(plan.retained_bytes))return error::too_large;
                stop_bound=std::max(stop_bound,plan.transport.stop_ack_ns);devices.emplace(alias,plan);
            }
            for(auto const& [alias,setting]:entry.control_plans)
            {
                (void)alias;device_dispatch::control_plan plan;
                if(auto status=setting.freeze(plan);status!=error::success)return status;
                if(!reserve(plan.retained_bytes))return error::too_large;
                auto device=devices.find(plan.device_alias.view());if(device==devices.end()||!selected.emplace(plan.device_alias.view()).second)return error::invalid;
                auto const& allowed=device->second;
                if(plan.stop!=allowed.ledger.stop_profile||plan.lease_ns>allowed.arbitration.maximum_lease_ns||
                    plan.priority>allowed.arbitration.maximum_priority||plan.write_window_ns>allowed.ledger.maximum_deadline_ns||
                    plan.source_count<allowed.ledger.minimum_observations||plan.source_count>allowed.ledger.observations)return error::invalid;
                for(std::size_t i{};i<plan.source_count;++i)
                {
                    auto const& source=plan.sources[i];auto sensor=entry.sensor_plans.find(source.alias.view());auto buffer=entry.buffer_plans.find(source.buffer_alias.view());
                    if(sensor==entry.sensor_plans.end()||buffer==entry.buffer_plans.end()||buffer->second.schema!=source.buffer_profile||source.frame!=allowed.ledger.schema.frame)return error::denied;
                    auto copy=sensor->second;sensor_dispatch::configuration native;int fd{};
                    if(auto status=copy.freeze(native,fd);status!=error::success)return status;
                    if(native.source.ingress.profile!=source.profile||native.source.buffer_alias!=source.buffer_alias||native.source.buffer_profile!=source.buffer_profile)return error::denied;
                }
            }
            if(!device_supervisor::valid(entry.device_supervision,stop_bound))return error::invalid;
            for(auto bytes:{retained,entry.device_supervision.stack_bytes}){if(bytes>INT64_MAX-total_bytes)return error::too_large;total_bytes+=bytes;}
            return error::success;
        }

        /** @brief Validate recording schemas, exact source grants and aggregate reservations without allocating stores. */
        static error validate_recording(environment const& entry,std::uint64_t& total_bytes)
        {
            if(!entry.recording_budget_set)return entry.recording_plans.empty()&&entry.replay_plans.empty()?error::success:error::invalid;
            auto budget=entry.recording_budget;recording_dispatch::limits capacity;
            if(auto result=budget.freeze(capacity);result!=error::success)return result;
            if(entry.recording_plans.size()>capacity.recordings||entry.replay_plans.size()>capacity.replays)return error::too_large;
            std::uint64_t storage{},definitions{};
            for(auto const& [alias,setting]:entry.recording_plans)
            {
                (void)alias;auto prepared=setting;recording::definition definition;std::uint64_t needed{},retained{};
                if(auto result=prepared.freeze(definition);result!=error::success)return result;
                if(!definition.storage(needed)||!definition.retained(retained))return error::invalid;
                if(needed>capacity.storage_bytes-storage||retained>INT64_MAX-definitions)return error::too_large;
                storage+=needed;definitions+=retained;
                for(auto const& channel:definition.channels)
                {
                    auto source=entry.grants.find(channel.source.alias.view());auto buffer=entry.buffer_plans.find(channel.source.alias.view());
                    if(source==entry.grants.end()||source->second.kind!=purpose::buffer||source->second.binding!="sealed-buffer"||
                       !source->second.actions.contains("buffer.read")||!source->second.actions.contains("data.export")||
                       buffer==entry.buffer_plans.end()||buffer->second.schema!=channel.source.schema)return error::denied;
                }
            }
            for(auto const& [alias,setting]:entry.replay_plans)
            {
                (void)alias;auto source=entry.grants.find(setting.source.view());auto origin=entry.recording_plans.find(setting.source.view());
                if(source==entry.grants.end()||source->second.kind!=purpose::recording||source->second.binding!="private-recording"||
                   !source->second.actions.contains("resource.observe")||!source->second.actions.contains("data.export")||origin==entry.recording_plans.end())return error::denied;
                auto prepared=origin->second;recording::definition definition;std::uint64_t needed{};
                if(auto result=prepared.freeze(definition);result!=error::success)return result;
                if(!recording_dispatch::replay_storage(setting.policy,needed)||setting.policy.handles>definition.capacity.replays)return error::invalid;
                if(needed>capacity.storage_bytes-storage)return error::too_large;
                storage+=needed;
            }
            for(auto bytes:{storage,definitions,capacity.workspace_bytes})
            {if(bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;total_bytes+=bytes;}
            return error::success;
        }
        /** @brief Validate complete log/diagnostic reservations before allocating any store.
         * @details One diagnostic has one exact configured source with explicit
         * resource.observe authority. Capacity sums use checked subtraction;
         * a failed validation neither freezes user settings nor creates a provider.
         */
        static error validate_log_diagnostic(environment const& entry,std::uint64_t& total_bytes)
        {
            if(!entry.log_diagnostic_budget_set)
                return entry.log_plans.empty()&&entry.diagnostic_plans.empty()?error::success:error::invalid;
            auto budget=entry.log_diagnostic_budget;log_diagnostic_dispatch::limits capacity;
            if(auto result=budget.freeze(capacity);result!=error::success)return result;
            if(entry.log_plans.size()>capacity.logs||entry.diagnostic_plans.size()>capacity.diagnostics)return error::too_large;
            std::uint64_t storage{};
            for(auto const& [alias,setting]:entry.log_plans)
            {
                (void)alias;auto prepared=setting;logs::plan plan;std::uint64_t needed{};
                if(auto result=prepared.freeze(plan);result!=error::success)return result;
                if(!logs::store::required_storage(plan,needed)||plan.record_bytes>capacity.request_bytes)return error::invalid;
                if(needed>capacity.storage_bytes-storage)return error::too_large;
                storage+=needed;
            }
            for(auto const& [alias,setting]:entry.diagnostic_plans)
            {
                (void)alias;auto prepared=setting;diagnostics::plan plan;logs::name source;std::uint64_t needed{};
                if(auto result=prepared.freeze(plan,source);result!=error::success)return result;
                auto origin=entry.grants.find(source.view());
                if(origin==entry.grants.end()||origin->second.kind!=purpose::log||origin->second.binding!="private-log"||
                   !origin->second.actions.contains("resource.observe")||!entry.log_plans.contains(source.view()))return error::denied;
                if(!diagnostics::snapshot::required_storage(plan,needed))return error::invalid;
                if(needed>capacity.storage_bytes-storage)return error::too_large;
                storage+=needed;
            }
            for(auto bytes:{storage,capacity.workspace_bytes})
            {if(bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;total_bytes+=bytes;}
            return error::success;
        }
        // Called with the deployment mutex held and after policy is frozen.
        static error validate_media(environment const& entry,std::uint64_t& total_bytes)
        {
            if(entry.media_plans.empty())return error::success;
            if(!entry.media_budget_set||entry.media_plans.size()>entry.media_capacity.bindings)return error::invalid;
            std::uint64_t retained{};
            for(auto const& [alias,setting]:entry.media_plans)
            {
                (void)alias;media_dispatch::plan plan;auto status=setting.freeze(plan);if(status!=error::success)return status;
                if(retained>entry.media_capacity.storage_bytes||plan.storage_bytes>entry.media_capacity.storage_bytes-retained)return error::too_large;
                retained+=plan.storage_bytes;
                if(plan.storage_bytes>INT64_MAX-total_bytes)return error::too_large;
                total_bytes+=plan.storage_bytes;if(plan.workspace_bytes>INT64_MAX-total_bytes)return error::too_large;total_bytes+=plan.workspace_bytes;
                for(bool output:{false,true})
                {
                    auto const& name=output?plan.output_alias:plan.input_alias;auto found=entry.buffer_plans.find(name.view());auto grant=entry.grants.find(name.view());
                    if(found==entry.buffer_plans.end()||grant==entry.grants.end()||grant->second.kind!=purpose::buffer||grant->second.binding!="sealed-buffer")return error::denied;
                    auto const& policy=found->second;buffers::layout format;format.schema=output?plan.output_profile:plan.input_profile;
                    format.rank=3;format.shape={plan.height,plan.width,plan.channels};format.strides={plan.width*plan.channels,plan.channels,1};
                    format.color=output?plan.output_color:plan.input_color;format.bytes=plan.width*plan.height*plan.channels;
                    if(auto check=buffers::validate(format,policy);check!=error::success)return check;
                    if(format.bytes>policy.transfer_bytes)return error::too_large;
                    // Configured capacity is independent of authority: missing
                    // secondary actions remain a runtime denial, never added here.
                }
            }
            return error::success;
        }
        static error validate_sensor_proposal(environment const& entry,std::uint64_t& total_bytes)
        {
            auto add=[&](std::uint64_t bytes){if(bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return false;total_bytes+=bytes;return true;};
            if(!entry.sensor_plans.empty()&&!entry.sensor_budget_set)return error::invalid;
            if(entry.sensor_plans.size()>entry.sensor_capacity.bindings)return error::too_large;
            ::uwvm2::utils::container::std_set<int> descriptors;
            ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,sensor_dispatch::configuration,std::less<>> sensors;
            std::uint64_t retained{};
            for(auto const& [alias,setting]:entry.sensor_plans)
            {
                auto prepared=setting;sensor_dispatch::configuration plan;int descriptor{};std::uint64_t needed{};
                if(auto result=prepared.freeze(plan,descriptor);result!=error::success)return result;
                if(!sensor_dispatch::required_storage(plan,needed)||needed>entry.sensor_capacity.storage_bytes-retained)return error::too_large;
                if(!descriptors.emplace(descriptor).second)return error::conflict;
                auto buffer=entry.buffer_plans.find(plan.source.buffer_alias.view());
                if(buffer==entry.buffer_plans.end()||buffer->second.schema!=plan.source.buffer_profile||
                   buffer->second.object_bytes<plan.source.ingress.body_bytes||buffer->second.objects<plan.source.buffer_objects||
                   buffer->second.elements<plan.source.ingress.body_bytes||!buffer->second.rank||
                   !(buffer->second.element_mask&1)||!(buffer->second.order_mask&1)||!(buffer->second.color_mask&1))return error::invalid;
                retained+=needed;if(!add(plan.workspace_bytes))return error::too_large;sensors.emplace(alias,plan);
            }
            if(!add(retained))return error::too_large;
            if((!entry.proposal_plans.empty()||!entry.proposal_arbiter_plans.empty())&&!entry.proposal_budget_set)return error::invalid;
            if(entry.proposal_plans.size()>entry.proposal_capacity.proposals||entry.proposal_arbiter_plans.size()>entry.proposal_capacity.arbiters)return error::too_large;
            if(entry.proposal_budget_set)
            {
                std::uint64_t required{};
                if(!proposal_guest_dispatch::required_storage(entry.proposal_guest_capacity,required)||required>entry.proposal_guest_capacity.retained_bytes)return error::invalid;
                if(!add(entry.proposal_guest_capacity.retained_bytes))return error::too_large;
            }
            ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,proposals::plan,std::less<>> proposals;
            ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string,buffers::profile,std::less<>> targets;
            for(auto const& [alias,setting]:entry.proposal_plans)
            {
                auto prepared=setting;proposals::plan plan;
                if(auto result=prepared.freeze(plan);result!=error::success)return result;
                auto model=entry.model_plans.find(plan.model_alias.view());
                if(model==entry.model_plans.end()||model->second.model!=plan.model||model->second.task!=plan.task||model->second.streaming||
                   model->second.output_type.view()!=proposals::model_output_type||model->second.encoding.view()!=proposals::model_output_encoding)return error::invalid;
                if(!add(plan.workspace_bytes))return error::too_large;
                for(std::size_t index{};index<plan.source_count;++index)
                {
                    auto const& rule=plan.sources[index];auto source=sensors.find(rule.alias.view());
                    if(source==sensors.end()||source->second.source.ingress.profile!=rule.profile||source->second.source.buffer_alias!=rule.buffer_alias||
                       source->second.source.buffer_profile!=rule.buffer_profile||source->second.source.ingress.frame!=rule.frame)return error::invalid;
                }
                for(std::size_t index{};index<plan.tool_count;++index)
                {
                    auto const& tool=plan.tools[index];auto target=entry.managed_lifecycle_plans.find(tool.target_alias.view());
                    if(target==entry.managed_lifecycle_plans.end()||!(target->second.roles_mask&managed_lifecycle::client)||
                       tool.action!=proposal_contract::target_action::lifecycle_submit||tool.effect!=proposal_admission::effect_class::non_physical||
                       tool.argument_type.view()!=proposal_lifecycle_target::argument_type||tool.encoding.view()!=proposal_lifecycle_target::encoding||
                       tool.frame.view()!=proposal_lifecycle_target::frame||tool.schema!=proposal_lifecycle_target::argument_schema()||
                       tool.target_profile.sha256!=proposal_lifecycle_target::profile_digest(tool.target_profile.identity.view(),tool.target_alias.view(),target->second))return error::invalid;
                    auto [existing,inserted]=targets.emplace(tool.target_alias.view(),tool.target_profile);
                    if(!inserted&&existing->second!=tool.target_profile)return error::conflict;
                }
                proposals.emplace(alias,plan);
            }
            if(targets.size()>proposal_lifecycle_target::maximum_bindings)return error::too_large;
            for(auto const& [alias,plan]:entry.proposal_arbiter_plans)
            {
                (void)alias;auto source=proposals.find(plan.proposal_alias.view());
                if(source==proposals.end()||!proposals::valid(plan,source->second))return error::invalid;
            }
            return error::success;
        }
        error activate(environment& entry,caller who,std::shared_ptr<broker>& output)
        {
            if(entry.generation&&entry.generation!=who.generation)return error::denied;
            auto selected=entry.live;
            if(auto result=selected->permissions.enroll(who);result!=error::success)return result;
            entry.generation=who.generation;
            output=std::move(selected);return error::success;
        }
        error freeze()
        {
            if(retired)return error::denied;
            if(frozen)return error::success;
            ::uwvm2::utils::container::std_vector<std::pair<environment*,std::shared_ptr<broker>>> pending;
            ::uwvm2::utils::container::std_vector<std::pair<model_attachment::configuration*,model_attachment::configuration>> workers;
            ::uwvm2::utils::container::std_vector<std::pair<transforms::configuration*,transforms::configuration>> transform_settings;
            ::uwvm2::utils::container::std_vector<std::pair<communication_deployment_configuration*,communication_deployment_configuration>> communication_settings;
            bool have_providers{};std::uint64_t total_bytes{};
            attachment_registry attachments{declarations_only};
            for(auto& [name,entry]:environments)
            {
                if(entry.drivers)if(auto status=entry.drivers->claim_endpoints(attachments);status!=error::success)return status;
                std::uint64_t module_bytes{};
                if(entry.module_call_plans.size()>entry.module_call_routes)return error::invalid;
                for(auto const& [route,plan]:entry.module_call_plans)
                {
                    (void)route;auto bytes=module_exchange::reservation(plan);
                    auto client=memberships.find(plan.client),server=memberships.find(plan.server);
                    if(!entry.module_call_budget_set||!module_exchange::valid(plan)||bytes>entry.module_call_bytes-module_bytes||
                       client==memberships.end()||server==memberships.end()||client->second!=name||server->second!=name)return error::invalid;
                    module_bytes+=bytes;
                }
                if(module_bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                total_bytes+=module_bytes;
                if(!entry.communication.empty())
                {
                    auto settings=entry.communication;std::uint64_t reserved{};
                    if(auto status=settings.freeze(reserved);status!=error::success)return status;
                    for(auto const& [worker_name,worker]:settings.workers())
                    {
                        (void)worker_name;auto const& next=worker.attachment.endpoints();
                        if(auto status=attachments.session(next.identity.session);status!=error::success)return status;
                        if(auto status=attachments.descriptor(next.control);status!=error::success)return status;
                        for(auto fd:next.pipes)if(auto status=attachments.descriptor(fd);status!=error::success)return status;
                    }
                    if(reserved>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                    total_bytes+=reserved;have_providers=true;communication_settings.emplace_back(&entry.communication,std::move(settings));
                }
                if(auto status=validate_log_diagnostic(entry,total_bytes);status!=error::success)return status;
                if(auto status=validate_recording(entry,total_bytes);status!=error::success)return status;
                if(auto status=validate_sensor_proposal(entry,total_bytes);status!=error::success)return status;
                if(auto status=validate_media(entry,total_bytes);status!=error::success)return status;
                if(auto status=validate_bridge(entry,total_bytes);status!=error::success)return status;
                if(auto status=validate_device(entry,total_bytes);status!=error::success)return status;
                {
                    std::uint64_t bytes{};if(auto status=entry.parameters.validate(entry.communication,bytes);status!=error::success)return status;
                    if(bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                    total_bytes+=bytes;
                }
                if(!entry.sensor_plans.empty()||!entry.proposal_plans.empty()||!entry.device_plans.empty())have_providers=true;
                std::uint64_t retained{};
                if(entry.input_plans.size()>entry.input_objects)return error::invalid;
                for(auto const& [alias,plan]:entry.input_plans)
                {
                    if(!entry.input_budget_set||plan.bytes>entry.input_bytes-retained)return error::invalid;
                    retained+=plan.bytes;have_providers=true;
                }
                std::uint64_t output_bytes{},output_objects{},receipt_bytes{},publication_bytes{},publication_calls{};
                for(auto const& [alias,plan]:entry.output_plans)
                {
                    auto records=outputs::receipt_bytes(plan);
                    if(!entry.output_budget_set||plan.bytes>entry.output_bytes-output_bytes||plan.objects>entry.output_objects-output_objects||
                       records>entry.output_receipt_bytes-receipt_bytes)return error::invalid;
                    output_bytes+=plan.bytes;output_objects+=plan.objects;receipt_bytes+=records;have_providers=true;
                    if(plan.publication_scratch_bytes||plan.publication_scratch_calls)
                    {
                        if(!entry.publication_budget_set||plan.publication_scratch_bytes>entry.publication_bytes-publication_bytes||
                           plan.publication_scratch_calls>entry.publication_calls-publication_calls)return error::invalid;
                        publication_bytes+=plan.publication_scratch_bytes;publication_calls+=plan.publication_scratch_calls;
                    }
                }
                std::uint64_t buffer_bytes{},buffer_objects{},buffer_workspace{},buffer_leases{};
                for(auto const& [alias,plan]:entry.buffer_plans)
                {
                    if(!entry.buffer_budget_set||plan.bytes>entry.buffer_bytes-buffer_bytes||plan.objects>entry.buffer_objects-buffer_objects||
                       plan.workspace_bytes>entry.buffer_workspace-buffer_workspace||plan.leases>entry.buffer_leases-buffer_leases)return error::invalid;
                    buffer_bytes+=plan.bytes;buffer_objects+=plan.objects;buffer_workspace+=plan.workspace_bytes;buffer_leases+=plan.leases;
                }
                models::budget model_total;
                transforms::budget transform_total;
                for(auto& [alias,setting]:entry.transform_plans)
                {
                    (void)alias;auto prepared=setting;
                    if(auto status=prepared.freeze();status!=error::success)return status;
                    if(!entry.transform_budget_set||!transforms::accumulate(entry.transform_capacity,*prepared.limits(),transform_total))return error::invalid;
                    transform_settings.emplace_back(&setting,std::move(prepared));
                }
                for(auto bytes:{transform_total.data_bytes,transform_total.workspace_bytes,transform_total.request_bytes,transform_total.carrier_bytes})
                {if(bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;total_bytes+=bytes;}
                for(auto const& [alias,plan]:entry.model_plans)
                {
                    if(!entry.model_budget_set||!entry.model_capacity_plans.contains(alias)||
                       !models::accumulate(entry.model_capacity,plan,model_total))return error::invalid;
                    auto configured=entry.model_worker_plans.find(alias);if(configured==entry.model_worker_plans.end())return error::invalid;
                    auto prepared=configured->second;
                    if(auto result=prepared.freeze(plan);result!=error::success)return result;
                    if(auto status=attachments.session(prepared.identity().session);status!=error::success)return status;
                    if(auto status=attachments.descriptor(prepared.descriptor());status!=error::success)return status;
                    auto bounds=prepared.limits();
                    std::uint64_t tls_bytes=bounds.tls_identity_fd<0?0:attachment_tls::reservation(model_attachment::transport_frame(bounds));
                    for(auto bytes:{tls_bytes,bounds.supervisor_bytes,bounds.request_bytes,bounds.response_bytes,std::uint64_t{model_attachment::protocol::header_bytes+std::min<std::uint64_t>(model_attachment::protocol::fragment_bytes,std::max<std::uint64_t>(bounds.response_bytes,1024))}})
                    {
                        if(bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                        total_bytes+=bytes;
                    }
                    workers.emplace_back(&configured->second,std::move(prepared));
                    have_providers=true;
                }
                for(auto bytes:{model_total.input_bytes,model_total.output_bytes,model_total.workspace_bytes})
                {
                    if(bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                    total_bytes+=bytes;
                }
                if(buffer_bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                total_bytes+=buffer_bytes;
                if(buffer_workspace>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                total_bytes+=buffer_workspace;
                if(publication_bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                total_bytes+=publication_bytes;
                if(output_bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                total_bytes+=output_bytes;
                if(receipt_bytes>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                total_bytes+=receipt_bytes;
                if(retained>static_cast<std::uint64_t>(INT64_MAX)-total_bytes)return error::too_large;
                total_bytes+=retained;
                for(auto const& [alias,cap]:entry.grants)
                {
                    if(!implemented(cap))return error::unsupported;
                    if(cap.kind==purpose::module_call&&!entry.module_call_plans.contains(cap.resource))return error::invalid;
                    if(service_observer(cap))
                    {
                        auto source=entry.grants.find(cap.resource);
                        auto plan=entry.communication.definition(cap.resource);
                        if(source==entry.grants.end()||source->second.kind!=purpose::service||source->second.binding!="native-communication"||
                           !source->second.actions.contains("service.submit")||!plan||plan->kind!=communication_configuration::family::service||
                           std::get<communication::service_plan>(plan->policy).direction!=communication::service_role::client)return error::invalid;
                    }
                    else if(broker::communication_policy(cap)&&!entry.communication.contains(alias))return error::invalid;
                    if(cap.binding=="local-parameters"&&!entry.parameters.contains(alias))return error::invalid;
                    if(cap.binding=="sealed-input"&&!entry.input_plans.contains(alias))return error::invalid;
                    if(cap.binding=="volatile-output"&&!entry.output_plans.contains(alias))return error::invalid;
                    if(cap.binding=="local-lifecycle"&&!entry.managed_lifecycle_plans.contains(alias))return error::invalid;
                    if(cap.binding=="registered-driver"&&(!entry.drivers||!entry.drivers->contains(alias)))return error::invalid;
                    if(cap.binding=="sealed-buffer"&&!entry.buffer_plans.contains(alias))return error::invalid;
                    if(cap.binding=="local-model"&&!entry.model_plans.contains(alias))return error::invalid;
                    if(cap.binding=="local-transform"&&!entry.transform_plans.contains(alias))return error::invalid;
                    if(cap.binding=="private-log"&&!entry.log_plans.contains(alias))return error::invalid;
                    if(cap.binding=="local-diagnostic"&&!entry.diagnostic_plans.contains(alias))return error::invalid;
                    if(cap.binding=="private-recording"&&!entry.recording_plans.contains(alias))return error::invalid;
                    if(cap.binding=="private-replay"&&!entry.replay_plans.contains(alias))return error::invalid;
                    if(cap.binding=="bounded-sensor"&&!entry.sensor_plans.contains(alias))return error::invalid;
                    if(cap.binding=="immutable-buffer-media"&&!entry.media_plans.contains(alias))return error::invalid;
                    if(cap.binding=="topic-forwarder"&&!entry.bridge_plans.contains(alias))return error::invalid;
                    if(cap.binding=="framed-scalar"&&!entry.device_plans.contains(alias))return error::invalid;
                    if(cap.binding=="framed-control"&&!entry.control_plans.contains(alias))return error::invalid;
                    if(cap.binding=="model-proposals"&&!entry.proposal_plans.contains(alias))return error::invalid;
                    if(cap.binding=="proposal-arbiter"&&!entry.proposal_arbiter_plans.contains(alias))return error::invalid;
                }
                std::shared_ptr<broker> next;
                if(auto result=configure(entry,next);result!=error::success)return result;
                if(!declarations_only)pending.emplace_back(&entry,std::move(next));
            }
            for(auto& [setting,prepared]:workers)*setting=std::move(prepared);
            for(auto& [setting,prepared]:transform_settings)*setting=std::move(prepared);
            for(auto& [setting,prepared]:communication_settings)*setting=std::move(prepared);
            for(auto& [entry,state]:pending){if(entry->drivers){entry->drivers->freeze(entry->platform_children);state->driver_store=entry->drivers;}entry->live=std::move(state);}
            frozen=true;providers_ready=!declarations_only&&!have_providers;return error::success;
        }
    public:
        deployment()=default;
        enum class mode { offline_review };
        explicit deployment(mode):declarations_only{true}{}
        /** @brief Configure only the declaration during review, leaving the supplied descriptor untouched. */
        error configure_payload_journal(int fd,module_buffer_audit::limits limits)
        {
            std::lock_guard lock{mutex};
            if(frozen||retired||payload_journal||!module_buffer_audit::journal::valid_configuration(fd,limits))return error::invalid;
            if(!declarations_only&&!module_buffer_audit::output.configure(fd,limits))return error::invalid;
            payload_journal=limits;return error::success;
        }
        /** @brief Make one trusted process, environment or caller audit ceiling immutable at freeze.
         * @details The process selector has label "*". Group labels are configured scene environment
         * names. A single rule matches only an exact runtime caller name. These rules grant no imports.
         */
        error configure_call_audit(call_audit::scope scope,std::string_view label,bool denied,call_audit::ceiling limits)
        {
            std::lock_guard lock{mutex};
            if(frozen || retired || (scope==call_audit::scope::global ? label!="*" : !identifier(label)))
                return error::invalid;
            if(scope==call_audit::scope::group && !environments.contains(label)) return error::invalid;
            return result_audit.add(scope,label,denied,limits) ? error::success : error::invalid;
        }
        bool auditing_calls() const noexcept { return result_audit.active(); }

        /** @brief Read the exact caller's group label, release the authority lock, then emit metadata.
         * @details Missing membership permits process/single accounting only. Contention drops the
         * observation without waiting on a provider. Inspection cannot create membership or authority.
         */
        template<typename Sink>
        void observe_call(std::string_view module,std::uint64_t generation,std::string_view function,
                          std::uint32_t result,std::uint64_t now,Sink&& sink,
                          call_audit::phase point=call_audit::phase::returned) noexcept
        {
            std::array<char,255> group{};
            std::size_t length{};
            {
                std::unique_lock lock{mutex,std::try_to_lock};
                if(!lock.owns_lock()) {call_audit::increment(result_audit.contention);return;}
                auto member=memberships.find(module);
                if(member!=memberships.end())
                {
                    length=member->second.size();
                    if(length>group.size())return;
                    std::copy_n(member->second.data(),length,group.data());
                }
            }
            result_audit.observe({group.data(),length},module,generation,function,result,now,sink,point);
        }
        /** @brief Reserve a finite external-driver registry; no process is started.
         * @details Zero is a valid disabled capacity. This accounts owned route
         * records and buffers, not allocator metadata, kernel queues or peer RAM.
         * The external deployment remains responsible for whole-process cgroups.
         */
        error driver_budget(std::string_view name,std::uint64_t count,std::uint64_t bytes)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end()||found->second.drivers||count>65536||bytes>INT64_MAX||bytes>SIZE_MAX)return error::invalid;
            auto value=::uwvm2::utils::container::make_shared<driver_dispatch>(declarations_only);
            auto status=value->budget(static_cast<std::size_t>(count),static_cast<std::size_t>(bytes));
            if(status==error::success)found->second.drivers=std::move(value);
            return status;
        }
        /** @brief Register a trusted inherited connection for one module and operation contract.
         * @details The exact module membership, driver grant and bounded route
         * must exist first. Registration duplicates a connected descriptor only;
         * it performs no handshake, process creation, cgroup or namespace action.
         * The supplied incarnation tuple and contract digest must match receiver
         * admission. A group grant never transfers this module's channel.
         */
        error driver_bind(std::string_view name,std::string_view alias,std::string_view module,int fd,drivers::credentials peer,
            drivers::protocol::binding const& identity,drivers::limits capacity)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);auto member=memberships.find(module);
            if(frozen||retired||found==environments.end()||!found->second.drivers||member==memberships.end()||member->second!=name)return error::denied;
            auto grant=found->second.grants.find(alias);
            if(grant==found->second.grants.end()||grant->second.kind!=purpose::driver||grant->second.binding!="registered-driver")return error::denied;
            auto [item,inserted]=found->second.driver_aliases.emplace(alias);if(!inserted)return error::conflict;
            try
            {
                auto status=found->second.drivers->bind(alias,module,grant->second.resource,fd,peer,identity,capacity);
                if(status!=error::success)found->second.driver_aliases.erase(item);
                return status;
            }
            catch(...){found->second.driver_aliases.erase(item);throw;}
        }
        /** @brief Bind the audited request/result shape for an already registered route. */
        error driver_schema(std::string_view name,std::string_view alias,std::string_view operation,schema_contract::limits bounds)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end()||!found->second.drivers)return error::denied;
            return found->second.drivers->schema(alias,operation,bounds);
        }
        /** @brief Reserve finite cross-Wasm storage without authorizing a route. */
        error module_call_budget(std::string_view name,std::uint64_t bytes,std::uint64_t routes)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end()||found->second.module_call_budget_set||bytes>INT64_MAX||routes>SIZE_MAX)return error::invalid;
            found->second.module_call_bytes=bytes;found->second.module_call_routes=routes;found->second.module_call_budget_set=true;return error::success;
        }
        /** @brief Register both endpoint names and the complete bounded message contract.
         * @details Registration grants no exposure or action. Endpoint membership,
         * route reservations and corresponding grants are validated at freeze.
         * Existing routes cannot be changed while any guest can use them.
         */
        error module_call_plan(std::string_view name,module_exchange::plan value)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end()||value.capture_stages||!value.capture_environment.empty()||!module_exchange::valid(value))return error::invalid;
            auto key=value.route;bool clear=value.clear_after_copy;
            if(!found->second.module_call_plans.emplace(std::move(key),std::move(value)).second)return error::conflict;
            auto& entry=found->second;
            auto selected=clear?environment::input_policy::clear:environment::input_policy::retain;
            if(entry.module_call_plans.size()==1)entry.module_inputs=selected;
            else if(entry.module_inputs!=selected)entry.module_inputs=environment::input_policy::mixed;
            return error::success;
        }
        /** @brief Select input-carrier erasure for one registered route before activation.
         * @details Defaults to true. False deliberately leaves copied request
         * bytes in the sender's memory, including failure cleanup; it grants no
         * authority and does not disable broker/response-region erasure. This
         * trusted deployment setting is immutable once frozen. Guest payloads,
         * capabilities and debugger commands cannot change it.
         */
        error module_call_input_clear(std::string_view name,std::string_view route,bool clear_after_copy)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto plan=found->second.module_call_plans.find(route);if(plan==found->second.module_call_plans.end())return error::invalid;
            plan->second.clear_after_copy=clear_after_copy;
            // Configuration may rescan routes; the common all-clear/all-retain
            // hot path needs no second string lookup after exact admission.
            auto selected=clear_after_copy?environment::input_policy::clear:environment::input_policy::retain;
            for(auto const& [key,value]:found->second.module_call_plans)
            { (void)key;if(value.clear_after_copy!=clear_after_copy){selected=environment::input_policy::mixed;break;} }
            found->second.module_inputs=selected;return error::success;
        }
        /** @brief Opt in to one committed payload stage on an existing exact route.
         * @details Configure the private journal first. The selector is frozen
         * with deployment and supplies no messaging or query permissions.
         */
        error module_call_buffer_capture(std::string_view name,std::string_view route,module_buffer_audit::stage stage)
        {
            std::lock_guard lock{mutex};
            auto env=environments.find(name);
            if(frozen || retired || !(declarations_only?payload_journal.has_value():module_buffer_audit::output.enabled()) || env==environments.end() ||
                static_cast<unsigned>(stage)<1 || static_cast<unsigned>(stage)>4)return error::invalid;
            auto found=env->second.module_call_plans.find(route);
            if(found==env->second.module_call_plans.end())return error::invalid;
            auto& plan=found->second;auto mask=module_buffer_audit::bit(stage);
            if(plan.capture_stages & mask)return error::conflict;
            plan.capture_environment.assign(name.begin(),name.end());plan.capture_stages|=mask;
            return error::success;
        }
        /** @brief Register a source-authored child in one existing ROSv2 environment.
         * @details Trusted initialization only: there is deliberately no CLI or
         * Wasm registration operation. SDK metadata and dlopen/preloaddl cannot
         * add a module. No production platform is registered by this mechanism
         * alone; the corresponding typed builtin carrier must also exist.
         */
        error register_platform(std::string_view name,platform_extensions::definition const& value)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            return found->second.platform_children.register_module(value);
        }
        /** @brief Apply a monotonic traffic reduction to an already active parent grant.
         * @details Trusted deployment management only, not a Wasm or debugger
         * operation. This uses the existing authority account, including all
         * common and platform-child handles. Startup policy remains immutable;
         * live inspection reads the updated account's attenuation flag. The
         * method does not enable a child, start a provider, change scope, reset
         * usage, renew a lease or establish a physical safety state.
         */
        error attenuate(std::string_view name,std::string_view alias,scene::attenuation const& limits)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(retired||!frozen||found==environments.end()||!found->second.live)return error::denied;
            return found->second.live->permissions.attenuate(alias,limits);
        }
        /** @brief Configure a ROSv2 child; never establish a top-level permission owner.
         * @details Both forms require an existing parent environment. Selection
         * accepts only enable/disable; binding accepts one already granted
         * device alias. Unknown compiled modules, duplicate settings, wildcard
         * names and post-freeze changes fail closed. Parent grants remain zero
         * unless separately configured through the common policy.
         */
        error platform_setting(std::string_view name,std::string_view category,std::string_view module,std::string_view value)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(retired||found==environments.end())return error::invalid;
            if(category=="permission.report")
            {
                // Observation can only be reduced, including after launch.
                if(value!="deny")return error::invalid;
                auto separator=module.find('.');permission_report::family target;
                if(!permission_report::parse_family(module.substr(0,separator),target))return error::invalid;
                auto function=separator==module.npos?std::string_view{}:module.substr(separator+1);
                if(separator!=module.npos&&(!identifier(function)||function.empty()))return error::invalid;
                return found->second.observation.deny(target,function)?error::success:error::conflict;
            }
            if(frozen)return error::invalid;
            if(category=="platform_extension")
            {
                if(value!="enable"&&value!="disable")return error::invalid;
                return found->second.platform_children.select(module,value=="enable");
            }
            if(category=="platform_extension.bind")
            {
                auto grant=found->second.grants.find(value);
                if(grant==found->second.grants.end())return error::denied;
                return found->second.platform_children.bind(module,grant->second);
            }
            return error::invalid;
        }
        /** @brief Configure one communication environment before freezing any provider.
         * @details The callback is trusted CLI/launcher code. It cannot enroll a
         * guest or create authority; grant aliases are checked separately below.
         */
        template<class Configure>error communication_setting(std::string_view name,Configure&& configure)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            return configure(found->second.communication);
        }
        error communication_limit(std::string_view name,std::string_view field,std::uint64_t value)
        {return communication_setting(name,[&](auto& settings){return settings.limit(field,value);});}
        /** @brief Authenticate a finite trusted startup-definition file; no worker is launched. */
        error communication_worker(std::string_view name,std::string_view worker,std::string_view profile,std::string_view prefix,
            std::string_view path,std::uint64_t bytes,buffers::digest digest)
        {return communication_setting(name,[&](auto& settings){return settings.load_worker(worker,profile,prefix,path,bytes,digest);});}
        error communication_worker_limit(std::string_view name,std::string_view worker,std::string_view field,std::uint64_t value)
        {return communication_setting(name,[&](auto& settings){return settings.worker_limit(worker,field,value);});}
        /** @brief Register a deployment-owned ROS process and its fixed-direction channels. */
        error communication_channel(std::string_view name,std::string_view worker,communication_attachment::registration value)
        {return communication_setting(name,[&](auto& settings){return settings.worker_setting(worker,[&](auto& attachment){return attachment.channel(value);});});}
        /** @brief Bind an exact sealed definition to an already granted logical alias. */
        error communication_binding(std::string_view name,std::string_view alias,std::string_view worker,std::string_view profile)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto grant=found->second.grants.find(alias);if(grant==found->second.grants.end())return error::denied;
            return found->second.communication.bind(grant->second,worker,profile);
        }
        error communication_pair(std::string_view name,std::string_view pair,std::string_view left,std::string_view right,std::uint64_t retirement_ns)
        {return communication_setting(name,[&](auto& settings){return settings.pair_workers(pair,left,right,retirement_ns);});}
        /** @brief Configure a local readiness binding without granting its sources or starting a worker. */
        error communication_event(std::string_view name,std::string_view alias,std::string_view domain,communication::profile profile,std::array<std::uint64_t,7> const& values)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto grant=found->second.grants.find(alias);if(grant==found->second.grants.end())return error::denied;
            return found->second.communication.event_plan(grant->second,domain,std::move(profile),values);
        }
        /** @brief Set one explicit global parameter reservation; this grants no key or node. */
        error parameter_budget_limit(std::string_view name,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            return found->second.parameters.budget_limit(field,value);
        }
        /** @brief Select one exact parameter profile and originating node grant.
         * @details The node must already have a native-communication binding.
         * Complete profile equality is rechecked at freeze and every open.
         */
        error parameter_profile(std::string_view name,std::string_view alias,std::string_view node_alias,buffers::profile selected,
                                buffers::profile node,parameter_values::algebra algebra)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto const& grants=found->second.grants;auto cap=grants.find(alias),parent=grants.find(node_alias);
            if(cap==grants.end()||cap->second.kind!=purpose::parameter||cap->second.binding!="local-parameters"||
                parent==grants.end()||parent->second.kind!=purpose::node||parent->second.binding!="native-communication"||
                !parent->second.actions.contains("node.open")||!found->second.communication.contains(node_alias))return error::denied;
            return found->second.parameters.profile(::uwvm2::utils::container::std_string{alias},::uwvm2::utils::container::std_string{node_alias},selected,node,algebra);
        }
        /** @brief Change an existing parameter setting without publishing partial failure. */
        template<class Change>error parameter_setting(std::string_view name,std::string_view alias,Change&& change)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            return found->second.parameters.change(alias,std::forward<Change>(change));
        }
        error parameter_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {return parameter_setting(name,alias,[&](auto& setting){return setting.limit(field,value);});}
        error parameter_definition(std::string_view name,std::string_view alias,bytes value)
        {return parameter_setting(name,alias,[&](auto& setting){return setting.definition(value);});}
        /** @brief Authenticate a bounded regular configuration file before decoding; no guest filesystem authority. */
        error parameter_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t size,buffers::digest digest)
        {
            return parameter_setting(name,alias,[&](auto& setting)
            {
                std::uint64_t ceiling{};auto status=setting.definition_limit(ceiling);if(status!=error::success)return status;
                byte_buffer owned;status=configuration_file::read(path,size,digest,static_cast<std::size_t>(ceiling),owned);
                return status==error::success?setting.definition(owned.view()):status;
            });
        }
        /** @brief Decode an explicitly bounded lowercase hex definition using recoverable storage. */
        error parameter_definition_hex(std::string_view name,std::string_view alias,std::string_view hex)
        {
            return parameter_setting(name,alias,[&](auto& setting)
            {
                std::uint64_t ceiling{};auto status=setting.definition_limit(ceiling);if(status!=error::success)return status;
                if(hex.empty()||hex.size()%2)return error::invalid;
                if(hex.size()/2>ceiling)return error::too_large;
                auto nibble=[](char c)noexcept->int{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
                for(char c:hex)if(nibble(c)<0)return error::invalid;
                byte_buffer owned;owned.resize_for_overwrite(hex.size()/2);auto output=owned.writable();
                for(std::size_t i{};i<output.size();++i)output[i]=static_cast<std::uint8_t>((nibble(hex[2*i])<<4)|nibble(hex[2*i+1]));
                return setting.definition(owned.view());
            });
        }
        /** @brief Create an empty environment with zero interfaces and grants. */
        error create(::uwvm2::utils::container::std_string name)
        {
            std::lock_guard lock{mutex};if(frozen||retired||!identifier(name))return error::invalid;
            environment entry;entry.name=name;entry.identity=environment_identity();if(!entry.identity)return error::too_large;
            return environments.emplace(std::move(name),std::move(entry)).second?error::success:error::conflict;
        }
        /** @brief Atomically bind one exact module; overlapping environments fail.
         * @details The membership index is the sole owner of this relationship.
         * A recoverable allocation failure cannot publish a partial binding or
         * make an exposed interface visible. No duplicate per-environment set
         * is maintained. Unbound modules remain hidden; there is no fallback.
         */
        error bind(std::string_view name,::uwvm2::utils::container::std_string module)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||module.empty()||module.size()>1024||module.find('\0')!=module.npos)return error::invalid;
            if(memberships.contains(module))return error::conflict;
            memberships.emplace(std::move(module),it->first);return error::success;
        }
        /** @brief Set one exact imported function to hidden, deny or allow. */
        error expose(std::string_view name,::uwvm2::utils::container::std_string action,exposure state)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||!callable(action)||state>exposure::allow)return error::invalid;
            return it->second.interfaces.emplace(std::move(action),state).second?error::success:error::conflict;
        }
        /** @brief Add a validated resource grant, without enabling its imports. */
        error allow(std::string_view name,grant value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||!valid(value))return error::invalid;
            if(!implemented(value))return error::unsupported;
            auto alias=value.alias;return it->second.grants.emplace(std::move(alias),std::move(value)).second?error::success:error::conflict;
        }
        /** @brief Assign a mandatory finite recording reservation without constructing a store. */
        error recording_budget_limit(std::string_view name,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto status=found->second.recording_budget.limit(field,value);
            if(status==error::success)found->second.recording_budget_set=true;
            return status;
        }
        /** @brief Change one trusted recording plan atomically; defaults never enable its alias. */
        template<typename Function> error recording_setting(std::string_view name,std::string_view alias,Function&& change)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto cap=found->second.grants.find(alias);
            if(cap==found->second.grants.end()||cap->second.kind!=purpose::recording||cap->second.binding!="private-recording")return error::denied;
            try
            {
                auto current=found->second.recording_plans.find(alias);recording::configuration next;
                if(current!=found->second.recording_plans.end())next=current->second;
                auto status=change(next);if(status!=error::success)return status;
                found->second.recording_plans.insert_or_assign(::uwvm2::utils::container::std_string(alias),std::move(next));return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        /** @brief Select one exact hashed semantic definition under an existing recording grant. */
        error recording_profile(std::string_view name,std::string_view alias,buffers::profile profile)
        {return recording_setting(name,alias,[&](auto& setting){return setting.profile(profile);});}
        /** @brief Set one finite storage/definition/count ceiling; duplicate fields fail closed. */
        error recording_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {return recording_setting(name,alias,[&](auto& setting){return setting.limit(field,value);});}
        /** @brief Parse owned configuration bytes using only the compiled native Bool/String schemas. */
        error recording_definition(std::string_view name,std::string_view alias,bytes owned)
        {return recording_setting(name,alias,[&](auto& setting){auto registry=recording::codecs::registry();return setting.parse(owned,registry);});}
        /** @brief Decode bounded lowercase configuration bytes only after exact grant and reservation checks.
         * @details This is a trusted startup carrier, not guest file authority.
         * Invalid hex and excessive extents are rejected before allocating;
         * recoverable fast_io allocation failure leaves the plan unchanged.
         */
        error recording_definition_hex(std::string_view name,std::string_view alias,std::string_view hex)
        {
            return recording_setting(name,alias,[&](auto& setting)
            {
                std::uint64_t ceiling{};auto status=setting.definition_limit(ceiling);if(status!=error::success)return status;
                if(hex.empty()||hex.size()%2)return error::invalid;
                if(hex.size()/2>ceiling)return error::too_large;
                auto nibble=[](char c)noexcept->int{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
                for(char c:hex)if(nibble(c)<0)return error::invalid;
                byte_buffer owned;owned.resize_for_overwrite(hex.size()/2);auto destination=owned.writable();
                for(std::size_t i{};i<destination.size();++i)
                    destination[i]=static_cast<std::uint8_t>((nibble(hex[i*2])<<4)|nibble(hex[i*2+1]));
                auto registry=recording::codecs::registry();return setting.parse(owned.view(),registry);
            });
        }
        /** @brief Read a bounded authenticated trusted regular definition file, never a guest path.
         * @details Exact grant/profile/all capacity fields are required before
         * filesystem access. The reader rejects links, devices and changed
         * identities; no recording store or native ROS provider is activated.
         */
        error recording_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t size,buffers::digest digest)
        {
            return recording_setting(name,alias,[&](auto& setting)
            {
                std::uint64_t ceiling{};auto status=setting.definition_limit(ceiling);if(status!=error::success)return status;
                byte_buffer owned;status=configuration_file::read(path,size,digest,static_cast<std::size_t>(ceiling),owned);if(status!=error::success)return status;
                auto registry=recording::codecs::registry();return setting.parse(owned.view(),registry);
            });
        }
        /** @brief Bind replay once to one logical recording source, with an explicit finite handle count. */
        error replay_plan(std::string_view name,std::string_view alias,std::string_view source,buffers::profile profile,std::uint64_t handles)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto cap=found->second.grants.find(alias);
            if(cap==found->second.grants.end()||cap->second.kind!=purpose::replay||cap->second.binding!="private-replay")return error::denied;
            recording::replay_definition next;next.policy={profile,handles};std::uint64_t needed{};
            if(!identifier(source)||!next.source.assign(source)||!recording_dispatch::replay_storage(next.policy,needed))return error::invalid;
            try{return found->second.replay_plans.emplace(::uwvm2::utils::container::std_string(alias),next).second?error::success:error::conflict;}
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        /** @brief Set one finite global log/diagnostic reservation by its semantic name. */
        error log_diagnostic_budget_limit(std::string_view name,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto status=found->second.log_diagnostic_budget.limit(field,value);
            if(status==error::success)found->second.log_diagnostic_budget_set=true;
            return status;
        }
        /** @brief Apply an inert setting only to an explicitly granted private logger. */
        template<typename Function> error log_setting(std::string_view name,std::string_view alias,Function&& change)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto cap=found->second.grants.find(alias);
            if(cap==found->second.grants.end()||cap->second.kind!=purpose::log||cap->second.binding!="private-log")return error::denied;
            auto current=found->second.log_plans.find(alias);logs::configuration next;
            if(current!=found->second.log_plans.end())next=current->second;
            auto status=change(next);if(status!=error::success)return status;
            found->second.log_plans.insert_or_assign(::uwvm2::utils::container::std_string(alias),std::move(next));return error::success;
        }
        /** @brief Configure a complete encoded-record bound; not a rate restriction. */
        error log_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {return log_setting(name,alias,[&](auto& setting){return setting.limit(field,value);});}
        /** @brief Admit one exact literal logger name without wildcard descendants. */
        error log_logger(std::string_view name,std::string_view alias,std::string_view logger)
        {return log_setting(name,alias,[&](auto& setting){return setting.logger(logger);});}
        /** @brief Explicitly choose rejection or acknowledged loss when the finite log fills. */
        error log_loss(std::string_view name,std::string_view alias,std::string_view mode)
        {return log_setting(name,alias,[&](auto& setting){return setting.loss(mode);});}
        /** @brief Apply an inert diagnostic setting only to an explicitly granted alias. */
        template<typename Function> error diagnostic_setting(std::string_view name,std::string_view alias,Function&& change)
        {
            std::lock_guard lock{mutex};auto found=environments.find(name);
            if(frozen||retired||found==environments.end())return error::invalid;
            auto cap=found->second.grants.find(alias);
            if(cap==found->second.grants.end()||cap->second.kind!=purpose::diagnostic||cap->second.binding!="local-diagnostic")return error::denied;
            auto current=found->second.diagnostic_plans.find(alias);diagnostics::configuration next;
            if(current!=found->second.diagnostic_plans.end())next=current->second;
            auto status=change(next);if(status!=error::success)return status;
            found->second.diagnostic_plans.insert_or_assign(::uwvm2::utils::container::std_string(alias),std::move(next));return error::success;
        }
        /** @brief Set one mandatory finite snapshot capacity or closed counter mask. */
        error diagnostic_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {return diagnostic_setting(name,alias,[&](auto& setting){return setting.limit(field,value);});}
        /** @brief Bind one exact local log source; freeze separately checks its observe grant. */
        error diagnostic_source(std::string_view name,std::string_view alias,std::string_view source)
        {return diagnostic_setting(name,alias,[&](auto& setting){return setting.source(source);});}
        /** @brief Reserve aggregate retained input bytes and object slots explicitly.
         * @details Both defaults are zero. These finite capacities are startup
         * reservations, not request-rate attenuation. Handles do not own or
         * refund the retained snapshots. Duplicate/late configuration fails.
         */
        error input_budget(std::string_view name,std::uint64_t bytes,std::uint64_t objects)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.input_budget_set||bytes>static_cast<std::uint64_t>(INT64_MAX)||objects==UINT64_MAX)return error::invalid;
            it->second.input_bytes=bytes;it->second.input_objects=objects;it->second.input_budget_set=true;return error::success;
        }
        /** @brief Store an inert immutable-input plan for an already explicit grant.
         * @details No file is opened here or during validate. Plan identity is
         * fixed by its alias, purpose, resource, sealed-input binding, exact
         * length, generation and SHA256. No guest may replace this plan.
         */
        error input(std::string_view name,::uwvm2::utils::container::std_string alias,artifacts::input_plan plan)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||!artifacts::valid(plan))return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.binding!="sealed-input"||!artifacts::purpose_code(cap->second.kind))return error::denied;
            return it->second.input_plans.emplace(std::move(alias),std::move(plan)).second?error::success:error::conflict;
        }
        /** @brief Reserve session output bytes, object slots and exact retry memory.
         * @details Defaults are zero. Bounds are finite independent startup
         * reservations, not rate attenuation. Last-request buffers consume
         * transfer+96 bytes per output binding even before its first mutation.
         */
        error output_budget(std::string_view name,std::uint64_t bytes,std::uint64_t objects,std::uint64_t receipts)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.output_budget_set||bytes>static_cast<std::uint64_t>(INT64_MAX)||
               objects==UINT64_MAX||receipts>static_cast<std::uint64_t>(INT64_MAX))return error::invalid;
            it->second.output_budget_set=true;it->second.output_bytes=bytes;it->second.output_objects=objects;it->second.output_receipt_bytes=receipts;
            return error::success;
        }
        /** @brief Bind an existing cache grant to an inert finite volatile store.
         * @details No file path, implicit purpose conversion or durable-output
         * claim is accepted. Static validation neither allocates store payloads
         * nor initializes retry history. Execution prepares all bindings atomically.
         */
        error output(std::string_view name,::uwvm2::utils::container::std_string alias,outputs::plan plan)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||!outputs::valid(plan))return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::cache||cap->second.binding!="volatile-output")return error::denied;
            return it->second.output_plans.emplace(std::move(alias),plan).second?error::success:error::conflict;
        }
        /** @brief Reserve aggregate complete-publication carrier bytes and calls.
         * @details Both defaults are zero. These finite capacities bound live
         * copied requests, independently of retained output and optional rate
         * attenuation. A carrier charges its inline buffer plus any heap copy;
         * all reservations are returned on delivery, cancellation or failure.
         */
        error publication_budget(std::string_view name,std::uint64_t bytes,std::uint64_t calls)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.publication_budget_set||bytes>INT64_MAX||calls==UINT64_MAX)return error::invalid;
            it->second.publication_budget_set=true;it->second.publication_bytes=bytes;it->second.publication_calls=calls;
            return error::success;
        }
        /** @brief Add finite publication scratch to an already configured output.
         * @details Requires the output plan and grant to exist. Configuration
         * neither grants publish nor its begin/append/commit dependencies.
         * Repeated or late settings fail, including repeated zero reservations.
         * Admission checks all four exact rights before reserving or copying.
         */
        error publication(std::string_view name,::uwvm2::utils::container::std_string alias,std::uint64_t bytes,std::uint64_t calls)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||bytes>INT64_MAX||calls==UINT64_MAX)return error::invalid;
            auto& entry=it->second;auto found=entry.output_plans.find(alias);
            if(found==entry.output_plans.end())return error::denied;
            if(!entry.publication_plans.emplace(std::move(alias)).second)return error::conflict;
            found->second.publication_scratch_bytes=bytes;found->second.publication_scratch_calls=calls;
            return error::success;
        }
        /** @brief Reserve aggregate retained buffer data, objects, workspace and leases.
         * @details Defaults are zero. Reservations are finite startup policy,
         * independent of optional rate attenuation. Closed but leased buffers
         * retain their data/object charge. Repeated or late budgets are refused.
         */
        error buffer_budget(std::string_view name,std::uint64_t bytes,std::uint64_t objects,std::uint64_t workspace,std::uint64_t leases)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.buffer_budget_set||bytes>INT64_MAX||workspace>INT64_MAX||
               objects==UINT64_MAX||leases==UINT64_MAX)return error::invalid;
            auto& entry=it->second;entry.buffer_budget_set=true;entry.buffer_bytes=bytes;entry.buffer_objects=objects;
            entry.buffer_workspace=workspace;entry.buffer_leases=leases;return error::success;
        }
        /** @brief Bind an exact buffer schema and finite tensor/lease limits.
         * @details Requires an existing buffer/sealed-buffer grant. No payload
         * is allocated, no native type is loaded and no provenance is granted
         * during configuration or static inspection. Guest buffers always keep
         * guest provenance regardless of the supplied layout's schema name.
         */
        error buffer_plan(std::string_view name,::uwvm2::utils::container::std_string alias,buffers::plan plan)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||!buffers::valid(plan))return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::buffer||cap->second.binding!="sealed-buffer")return error::denied;
            return it->second.buffer_plans.emplace(std::move(alias),plan).second?error::success:error::conflict;
        }
        /** @brief Bind finite local lifecycle state, roles and permitted transitions.
         * @details Grant must already exist with the exact lifecycle purpose and
         * local-lifecycle binding. Zero limits remain zero, repeated plans fail,
         * and validation performs no native node/device work. Per-alias state
         * remains private even when resource labels coincide.
         */
        error managed_lifecycle_plan(std::string_view name,::uwvm2::utils::container::std_string alias,managed_lifecycle::plan plan)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||!managed_lifecycle::valid(plan))return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::lifecycle||cap->second.binding!="local-lifecycle")return error::denied;
            return it->second.managed_lifecycle_plans.emplace(std::move(alias),plan).second?error::success:error::conflict;
        }
        /** @brief Reserve finite directed bridge bindings and persistent storage; no I/O or grant. */
        error bridge_budget(std::string_view name,bridge_dispatch::limits capacity)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.bridge_budget_set||(capacity.bindings==UINT64_MAX||capacity.bindings>SIZE_MAX)||capacity.storage_bytes>INT64_MAX||capacity.supervisor_bytes>64u*1024u*1024u||capacity.poll_ns>100000000)return error::invalid;
            it->second.bridge_capacity=capacity;it->second.bridge_budget_set=true;return error::success;
        }
        /** @brief Reserve device/control storage, guarded supervisor stack and finite cleanup time; no grant or native effect. */
        error device_budget(std::string_view name,device_dispatch::limits capacity,device_supervisor::plan supervision)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.device_budget_set||capacity.devices==UINT64_MAX||capacity.controllers==UINT64_MAX||
                capacity.devices>SIZE_MAX||capacity.controllers>SIZE_MAX||capacity.retained_bytes>INT64_MAX||
                supervision.stack_bytes>64u*1024u*1024u||supervision.poll_ns>100000000||supervision.shutdown_ns>UINT64_C(5000000000))return error::invalid;
            it->second.device_capacity=capacity;it->second.device_supervision=supervision;it->second.device_budget_set=true;return error::success;
        }
        /** @brief Assign four directional startup descriptors without opening or probing them. */
        error device_endpoints(std::string_view name,std::string_view alias,std::array<std::uint64_t,4> endpoints)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.device_plans.find(alias);return setting==it->second.device_plans.end()?error::denied:setting->second.endpoints(endpoints);
        }
        /** @brief Choose a closed compiled schema under an existing exact device grant. */
        error device_profile(std::string_view name,::uwvm2::utils::container::std_string alias,buffers::profile profile)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::device||cap->second.binding!="framed-scalar")return error::denied;
            device_configuration setting;auto status=setting.profile(alias,profile);if(status!=error::success)return status;
            return it->second.device_plans.emplace(std::move(alias),std::move(setting)).second?error::success:error::conflict;
        }
        /** @brief Assign a mandatory capacity after the authenticated definition; rates are separate. */
        error device_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.device_plans.find(alias);return setting==it->second.device_plans.end()?error::denied:setting->second.limit(field,value);
        }
        /** @brief Verify exact profile semantics in owned bytes; no native endpoint is selected by those bytes. */
        error device_definition(std::string_view name,std::string_view alias,bytes owned)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.device_plans.find(alias);return setting==it->second.device_plans.end()?error::denied:setting->second.definition(owned);
        }
        /** @brief Decode a device definition only after the exact grant/profile and pending-state checks.
         * @details The fixed 16384-byte parser ceiling and all hex digits are
         * checked before recoverable fast_io allocation. Failure preserves the
         * previous configuration; no source descriptor is probed or activated.
         */
        error device_definition_hex(std::string_view name,std::string_view alias,std::string_view hex)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.device_plans.find(alias);if(setting==it->second.device_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::decode_hex(hex,16384,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Read a bounded, exact-hash trusted definition only before static freeze. */
        error device_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t bytes,buffers::digest digest)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.device_plans.find(alias);if(setting==it->second.device_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::read(path,bytes,digest,16384,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Choose a closed compiled schema under an existing exact control grant. */
        error control_profile(std::string_view name,::uwvm2::utils::container::std_string alias,buffers::profile profile)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::control||cap->second.binding!="framed-control")return error::denied;
            control_configuration setting;auto status=setting.profile(alias,profile);if(status!=error::success)return status;
            return it->second.control_plans.emplace(std::move(alias),std::move(setting)).second?error::success:error::conflict;
        }
        /** @brief Assign a mandatory capacity after the authenticated definition; rates are separate. */
        error control_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.control_plans.find(alias);return setting==it->second.control_plans.end()?error::denied:setting->second.limit(field,value);
        }
        /** @brief Verify exact profile semantics in owned bytes; no native endpoint is selected by those bytes. */
        error control_definition(std::string_view name,std::string_view alias,bytes owned)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.control_plans.find(alias);return setting==it->second.control_plans.end()?error::denied:setting->second.definition(owned);
        }
        /** @brief Decode a control definition only after the exact grant/profile and pending-state checks.
         * @details The fixed 16384-byte parser ceiling and all hex digits are
         * checked before recoverable fast_io allocation. Failure preserves the
         * previous configuration; no source descriptor is probed or activated.
         */
        error control_definition_hex(std::string_view name,std::string_view alias,std::string_view hex)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.control_plans.find(alias);if(setting==it->second.control_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::decode_hex(hex,16384,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Read a bounded, exact-hash trusted definition only before static freeze. */
        error control_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t bytes,buffers::digest digest)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.control_plans.find(alias);if(setting==it->second.control_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::read(path,bytes,digest,16384,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }

        /** @brief Choose a closed compiled schema under an existing exact bridge grant. */
        error bridge_profile(std::string_view name,::uwvm2::utils::container::std_string alias,buffers::profile profile)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::bridge||cap->second.binding!="topic-forwarder")return error::denied;
            bridge_configuration setting;auto status=setting.profile(alias,profile);if(status!=error::success)return status;
            return it->second.bridge_plans.emplace(std::move(alias),std::move(setting)).second?error::success:error::conflict;
        }
        /** @brief Assign a mandatory capacity after the authenticated definition; rates are separate. */
        error bridge_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.bridge_plans.find(alias);return setting==it->second.bridge_plans.end()?error::denied:setting->second.limit(field,value);
        }
        /** @brief Verify exact source semantics in owned bytes; no native endpoint is selected by those bytes. */
        error bridge_definition(std::string_view name,std::string_view alias,bytes owned)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.bridge_plans.find(alias);return setting==it->second.bridge_plans.end()?error::denied:setting->second.definition(owned);
        }
        /** @brief Decode a bridge definition only after the exact grant/profile and pending-state checks.
         * @details The fixed 4096-byte parser ceiling and all hex digits are
         * checked before recoverable fast_io allocation. Failure preserves the
         * previous configuration; no source descriptor is probed or activated.
         */
        error bridge_definition_hex(std::string_view name,std::string_view alias,std::string_view hex)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.bridge_plans.find(alias);if(setting==it->second.bridge_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::decode_hex(hex,4096,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Read a bounded, exact-hash trusted definition only before static freeze. */
        error bridge_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t bytes,buffers::digest digest)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.bridge_plans.find(alias);if(setting==it->second.bridge_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::read(path,bytes,digest,4096,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Reserve finite raw media bindings and persistent storage; no I/O or grant. */
        error media_budget(std::string_view name,media_dispatch::limits capacity)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.media_budget_set||(capacity.bindings==UINT64_MAX||capacity.bindings>SIZE_MAX)||capacity.storage_bytes>INT64_MAX)return error::invalid;
            it->second.media_capacity=capacity;it->second.media_budget_set=true;return error::success;
        }
        /** @brief Choose a closed compiled schema under an existing exact media grant. */
        error media_profile(std::string_view name,::uwvm2::utils::container::std_string alias,buffers::profile profile)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::media||cap->second.binding!="immutable-buffer-media")return error::denied;
            media_configuration setting;auto status=setting.profile(alias,profile);if(status!=error::success)return status;
            return it->second.media_plans.emplace(std::move(alias),std::move(setting)).second?error::success:error::conflict;
        }
        /** @brief Assign a mandatory capacity after the authenticated definition; rates are separate. */
        error media_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.media_plans.find(alias);return setting==it->second.media_plans.end()?error::denied:setting->second.limit(field,value);
        }
        /** @brief Verify exact source semantics in owned bytes; no native endpoint is selected by those bytes. */
        error media_definition(std::string_view name,std::string_view alias,bytes owned)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.media_plans.find(alias);return setting==it->second.media_plans.end()?error::denied:setting->second.definition(owned);
        }
        /** @brief Decode a media definition only after the exact grant/profile and pending-state checks.
         * @details The fixed 4096-byte parser ceiling and all hex digits are
         * checked before recoverable fast_io allocation. Failure preserves the
         * previous configuration; no source descriptor is probed or activated.
         */
        error media_definition_hex(std::string_view name,std::string_view alias,std::string_view hex)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.media_plans.find(alias);if(setting==it->second.media_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::decode_hex(hex,4096,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Read a bounded, exact-hash trusted definition only before static freeze. */
        error media_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t bytes,buffers::digest digest)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.media_plans.find(alias);if(setting==it->second.media_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::read(path,bytes,digest,4096,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Reserve finite receive-only bindings and retained native source bytes; no I/O or grant. */
        error sensor_budget(std::string_view name,sensor_dispatch::limits capacity)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.sensor_budget_set||capacity.bindings>32||capacity.storage_bytes>INT64_MAX)return error::invalid;
            it->second.sensor_capacity=capacity;it->second.sensor_budget_set=true;return error::success;
        }
        /** @brief Choose a closed compiled schema under an existing exact sensor grant. */
        error sensor_profile(std::string_view name,::uwvm2::utils::container::std_string alias,buffers::profile profile,std::string_view schema)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::sensor||cap->second.binding!="bounded-sensor")return error::denied;
            sensor_deployment_configuration setting;auto status=setting.profile(alias,profile,schema);if(status!=error::success)return status;
            return it->second.sensor_plans.emplace(std::move(alias),std::move(setting)).second?error::success:error::conflict;
        }
        /** @brief Assign a mandatory capacity after the authenticated definition; rates are separate. */
        error sensor_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.sensor_plans.find(alias);return setting==it->second.sensor_plans.end()?error::denied:setting->second.limit(field,value);
        }
        /** @brief Pin a trusted launch descriptor number once; only activation checks and duplicates it. */
        error sensor_descriptor(std::string_view name,std::string_view alias,std::uint64_t descriptor)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.sensor_plans.find(alias);return setting==it->second.sensor_plans.end()?error::denied:setting->second.descriptor(descriptor);
        }
        /** @brief Verify exact source semantics in owned bytes; no native endpoint is selected by those bytes. */
        error sensor_definition(std::string_view name,std::string_view alias,bytes owned)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.sensor_plans.find(alias);return setting==it->second.sensor_plans.end()?error::denied:setting->second.definition(owned);
        }
        /** @brief Decode a sensor definition only after the exact grant/profile and pending-state checks.
         * @details The fixed 4096-byte parser ceiling and all hex digits are
         * checked before recoverable fast_io allocation. Failure preserves the
         * previous configuration; no source descriptor is probed or activated.
         */
        error sensor_definition_hex(std::string_view name,std::string_view alias,std::string_view hex)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.sensor_plans.find(alias);if(setting==it->second.sensor_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::decode_hex(hex,4096,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Read a bounded, exact-hash trusted definition only before static freeze. */
        error sensor_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t bytes,buffers::digest digest)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.sensor_plans.find(alias);if(setting==it->second.sensor_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::read(path,bytes,digest,4096,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Reserve finite proposal providers, arbiters and exact retry/capture tables; zero remains zero. */
        error proposal_budget(std::string_view name,proposal_dispatch::limits capacity,proposals::guest_limits guest)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);std::uint64_t required{};
            if(frozen||retired||it==environments.end()||it->second.proposal_budget_set||capacity.proposals>32||capacity.arbiters>32||
               guest.retained_bytes>INT64_MAX||!proposal_guest_dispatch::required_storage(guest,required)||required>guest.retained_bytes)return error::invalid;
            it->second.proposal_capacity=capacity;it->second.proposal_guest_capacity=guest;it->second.proposal_budget_set=true;return error::success;
        }
        /** @brief Select exact proposal identity and an already configured model/task pair without granting either. */
        error proposal_profile(std::string_view name,::uwvm2::utils::container::std_string alias,buffers::profile profile,std::string_view model_alias)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto cap=it->second.grants.find(alias);auto model=it->second.model_plans.find(model_alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::proposal||cap->second.binding!="model-proposals"||model==it->second.model_plans.end())return error::denied;
            proposals::configuration setting;auto status=setting.profile(profile,model_alias,model->second.model,model->second.task);if(status!=error::success)return status;
            return it->second.proposal_plans.emplace(std::move(alias),std::move(setting)).second?error::success:error::conflict;
        }
        /** @brief Assign one mandatory proposal capacity independently of grant rates and target authority. */
        error proposal_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.proposal_plans.find(alias);return setting==it->second.proposal_plans.end()?error::denied:setting->second.limit(field,value);
        }
        /** @brief Authenticate model/source/tool semantics; every tool still requires current independent target authority. */
        error proposal_definition(std::string_view name,std::string_view alias,bytes owned)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.proposal_plans.find(alias);if(setting==it->second.proposal_plans.end())return error::denied;
            if(it->second.proposal_definitions.contains(alias))return error::conflict;
            auto next=setting->second;auto status=next.definition(owned);if(status!=error::success)return status;
            try{it->second.proposal_definitions.emplace(alias);}
            catch(std::bad_alloc const&){return error::no_memory;}
            setting->second=std::move(next);return error::success;
        }
        /** @brief Authenticate bounded proposal hex after exact model/proposal scope and pending-state checks.
         * @details Invalid or excessive hex allocates nothing. Decode and the
         * duplicate-definition receipt use recoverable fast_io allocation;
         * either failure leaves the plan available for an exact retry. No
         * model job, source read, arbiter decision or target operation occurs.
         */
        error proposal_definition_hex(std::string_view name,std::string_view alias,std::string_view hex)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.proposal_plans.find(alias);if(setting==it->second.proposal_plans.end())return error::denied;
            if(it->second.proposal_definitions.contains(alias))return error::conflict;
            byte_buffer owned;auto status=configuration_file::decode_hex(hex,proposals::maximum_definition_bytes,owned);if(status!=error::success)return status;
            auto next=setting->second;status=next.definition(owned.view());if(status!=error::success)return status;
            try{it->second.proposal_definitions.emplace(alias);}
            catch(std::bad_alloc const&){return error::no_memory;}
            setting->second=std::move(next);return error::success;
        }
        /** @brief Load one exact bounded proposal definition; duplicate/late requests are rejected before file access. */
        error proposal_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t bytes,buffers::digest digest)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.proposal_plans.find(alias);if(setting==it->second.proposal_plans.end())return error::denied;
            if(it->second.proposal_definitions.contains(alias))return error::conflict;
            byte_buffer owned;auto status=configuration_file::read(path,bytes,digest,proposals::maximum_definition_bytes,owned);if(status!=error::success)return status;
            auto next=setting->second;status=next.definition(owned.view());if(status!=error::success)return status;
            try{it->second.proposal_definitions.emplace(alias);}
            catch(std::bad_alloc const&){return error::no_memory;}
            setting->second=std::move(next);return error::success;
        }
        /** @brief Bind one independent arbiter to exact proposal/target aliases and an explicit tool mask. */
        error proposal_arbiter(std::string_view name,::uwvm2::utils::container::std_string alias,std::string_view source,std::string_view target,std::uint64_t mask)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.kind!=purpose::arbiter||cap->second.binding!="proposal-arbiter")return error::denied;
            proposals::arbiter_plan plan;
            if(!identifier(source)||!identifier(target)||!plan.proposal_alias.assign(source)||!plan.target_alias.assign(target)||!mask||mask>255)return error::invalid;
            plan.tool_mask=mask;return it->second.proposal_arbiter_plans.emplace(std::move(alias),plan).second?error::success:error::conflict;
        }
        /** @brief Reserve aggregate local transform capacity independently of optional rates. */
        error transform_budget(std::string_view name,transforms::budget capacity)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.transform_budget_set)return error::invalid;
            for(auto value:{capacity.sessions,capacity.queries,capacity.data_bytes,capacity.workspace_bytes,capacity.request_bytes,capacity.carrier_bytes})
                if(value>INT64_MAX)return error::invalid;
            it->second.transform_capacity=capacity;it->second.transform_budget_set=true;return error::success;
        }
        /** @brief Select a hash-pinned frame definition under a previously explicit grant. */
        error transform_profile(std::string_view name,::uwvm2::utils::container::std_string alias,buffers::profile profile)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto grant=it->second.grants.find(alias);
            if(grant==it->second.grants.end()||grant->second.kind!=purpose::transform||grant->second.binding!="local-transform")return error::denied;
            transforms::configuration setting;auto status=setting.profile(profile);if(status!=error::success)return status;
            return it->second.transform_plans.emplace(std::move(alias),std::move(setting)).second?error::success:error::conflict;
        }
        /** @brief Set one exact named transform reservation; missing fields never default to unlimited. */
        error transform_limit(std::string_view name,std::string_view alias,std::string_view field,std::uint64_t value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.transform_plans.find(alias);if(setting==it->second.transform_plans.end())return error::denied;
            return setting->second.limit(field,value);
        }
        /** @brief Parse an already owned canonical definition without host I/O. */
        error transform_definition(std::string_view name,std::string_view alias,bytes owned)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.transform_plans.find(alias);if(setting==it->second.transform_plans.end())return error::denied;
            return setting->second.definition(owned);
        }
        /** @brief Explicit trusted configuration loading, before policy freeze or guest activation.
         * @details The exact definition is limited to 64 KiB and SHA-verified.
         * This helper grants no guest file access and never selects a provider.
         * validate()/freeze() themselves remain free of file reads and I/O.
         */
        error transform_definition_file(std::string_view name,std::string_view alias,std::string_view path,std::uint64_t bytes,buffers::digest digest)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);if(frozen||retired||it==environments.end())return error::invalid;
            auto setting=it->second.transform_plans.find(alias);if(setting==it->second.transform_plans.end())return error::denied;
            if(!setting->second.definition_pending())return error::conflict;
            byte_buffer owned;auto status=configuration_file::read(path,bytes,digest,transforms::maximum_definition_bytes,owned);
            return status==error::success?setting->second.definition(owned.view()):status;
        }
        /** @brief Configure finite aggregate model reservations before any worker exists. */
        error model_budget(std::string_view name,models::budget capacity)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||it->second.model_budget_set||!models::valid(capacity))return error::invalid;
            it->second.model_capacity=capacity;it->second.model_budget_set=true;return error::success;
        }
        /** @brief Bind immutable model/task identity and a trusted transport codec.
         * @details The exact model purpose and local-model grant must already
         * exist. This does not attach or load a worker. Capacity is supplied
         * separately and must be explicitly set, including a deliberately
         * disabled all-zero plan. No URL, executable path or shell is accepted.
         */
        error model_plan(std::string_view name,::uwvm2::utils::container::std_string alias,models::plan plan)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end()||!models::valid(plan))return error::invalid;
            auto cap=it->second.grants.find(alias);
            if(cap==it->second.grants.end()||cap->second.binding!="local-model"||
               (cap->second.kind!=purpose::language_model&&cap->second.kind!=purpose::vision_model&&cap->second.kind!=purpose::world_model))return error::denied;
            return it->second.model_plans.emplace(std::move(alias),plan).second?error::success:error::conflict;
        }
        /** @brief Set model capacities once without altering identity or codec.
         * @details Fields are sessions, requests, pinned input bytes, maximum
         * input count, parameter bytes, output bytes/items, workspace bytes and
         * maximum steady deadline. Every reservation is finite; optional rate
         * attenuation remains in the independently configured grant.
         */
        error model_limits(std::string_view name,::uwvm2::utils::container::std_string alias,std::array<std::uint64_t,9> const& values)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end())return error::invalid;
            auto found=it->second.model_plans.find(alias);if(found==it->second.model_plans.end())return error::denied;
            auto plan=found->second;plan.sessions=values[0];plan.requests=values[1];plan.input_bytes=values[2];plan.input_count=values[3];
            plan.parameter_bytes=values[4];plan.output_bytes=values[5];plan.output_items=values[6];plan.workspace_bytes=values[7];plan.deadline_ns=values[8];
            if(!models::valid(plan))return error::invalid;
            if(!it->second.model_capacity_plans.emplace(std::move(alias)).second)return error::conflict;
            found->second=plan;return error::success;
        }
    private:
        template<typename Configure>
        error model_worker_setting(std::string_view name,std::string_view alias,Configure&& configure)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end())return error::invalid;
            if(!it->second.model_plans.contains(alias))return error::denied;
            auto found=it->second.model_worker_plans.find(alias);
            if(found!=it->second.model_worker_plans.end())return configure(found->second);
            model_attachment::configuration setting;auto status=configure(setting);
            if(status!=error::success)return status;
            it->second.model_worker_plans.emplace(::uwvm2::utils::container::std_string(alias),std::move(setting));return error::success;
        }
    public:
        /** @brief Register an existing external model channel; no worker launch or lifecycle authority. */
        error model_channel(std::string_view name,std::string_view alias,int fd,drivers::credentials peer,
                            model_attachment::protocol::binding binding,model_attachment::limits bounds)
        {return model_worker_setting(name,alias,[&](auto& setting){return setting.channel(fd,peer,binding,bounds,declarations_only);});}
        /** @brief Default native preparation fails closed for configured model workers. */
        struct unavailable_model_provider
        {
            error operator()(std::string_view,std::string_view,models::plan const&,model_attachment::configuration const&,std::uint64_t,model_dispatch&,
                             std::shared_ptr<models::provider_lifetime>&) const noexcept{return error::unsupported;}
        };
        /** @brief Production attachment factory; only a frozen external channel can attach. */
        struct external_model_provider
        {
            error operator()(std::string_view,std::string_view alias,models::plan const&,model_attachment::configuration const& setting,
                             std::uint64_t generation,model_dispatch& dispatcher,std::shared_ptr<models::provider_lifetime>& output) const noexcept
            {return setting.prepare(dispatcher,alias,generation,output);}
        };
        struct unavailable_communication_provider
        {error operator()(communication_deployment_configuration const&,broker&)const noexcept{return error::unsupported;}};
        /** @brief Production communication preparation with explicit pair supervisors.
         * @details The process-wide observation identity labels the real host
         * steady clock only; it grants no clock-read, ROS-time or device access.
         * Forked use is rejected. It is never presented as a DDS source stamp.
         */
        struct external_communication_provider
        {
            error operator()(communication_deployment_configuration const& settings,broker& target)const
            {
#if defined(__linux__)
                if(!target.communication_store)return error::invalid;
                struct observation_domain{pid_t process{::getpid()};handle clock{resources::allocate_identity()};};
                static observation_domain domain;
                if(domain.process!=::getpid()||!domain.clock)return error::denied;
                communication_dispatch::observation_clock observed{&domain,[](void* pointer,communication_contract::clock_epoch& epoch,std::int64_t& instant)noexcept
                {auto const& source=*static_cast<observation_domain*>(pointer);if(source.process!=::getpid()||!source.clock)return false;
                    epoch={source.clock,1};instant=resources::steady_now();return true;}};
                communication_deployment_provider::prepared prepared;
                auto status=communication_deployment_provider::prepare(settings,*target.communication_store,prepared,observed);if(status!=error::success)return status;
                for(auto const& [name,worker]:prepared.supervisors)target.communication_workers.emplace(name,worker);
                return prepared.start(settings.supervisor_stack_bytes());
#else
                (void)settings;(void)target;return error::unsupported;
#endif
            }
        };
        /** @brief Atomically prepare validated input snapshots, outputs and native workers before activation.
         * @details Providers are trusted attachment glue. All budgets and bindings
         * validate before any source read; --ros-scene-check never calls this.
         * A failure discards private prepared brokers and sealed descriptors;
         * no partially provisioned environment is selectable. No guest call
         * performs lazy file activation. Policy remains frozen after failure.
         */
        template<typename Provider,typename ModelProvider=unavailable_model_provider,typename CommunicationProvider=unavailable_communication_provider>
        error prepare_resources(Provider&& provider,ModelProvider model_provider={},CommunicationProvider communication_provider={})
        {
            std::lock_guard lock{mutex};if(declarations_only)return error::denied;
            if(auto status=freeze();status!=error::success)return status;
            if(providers_ready)return error::success;
#if defined(__linux__)
            ::uwvm2::utils::container::std_vector<std::pair<environment*,std::shared_ptr<broker>>> pending;
            for(auto& [name,entry]:environments)
            {
                std::shared_ptr<broker> next;
                if(auto status=configure(entry,next);status!=error::success)return status;
                for(auto const& [alias,plan]:entry.input_plans)
                {
                    auto const& cap=entry.grants.at(alias);storage::principal who{entry.identity,alias};
                    std::shared_ptr<storage::immutable_input> input;
                    if(auto status=provider(plan,who,cap.kind,input);status!=error::success)return status;
                    if(auto status=next->inputs.bind(who,cap.kind,cap.resource,cap.binding,plan,std::move(input));status!=error::success)return status;
                }
                for(auto const& [alias,plan]:entry.output_plans)
                {
                    auto const& cap=entry.grants.at(alias);
                    if(auto status=next->output_store.bind(alias,cap.resource,plan);status!=error::success)return status;
                }
                for(auto const& [alias,plan]:entry.model_plans)
                {
                    std::shared_ptr<models::provider_lifetime> worker;
                    auto status=model_provider(name,alias,plan,entry.model_worker_plans.at(alias),entry.identity,next->model_store,worker);
                    if(status!=error::success)return status;
                    if(!worker)return error::invalid;
                    next->model_workers.emplace(alias,std::move(worker));
                }
                for(auto const& [alias,setting]:entry.sensor_plans)
                {
                    (void)alias;auto prepared=setting;sensor_dispatch::configuration plan;int descriptor{};
                    if(auto status=prepared.freeze(plan,descriptor);status!=error::success)return status;
                    if(auto status=next->sensor_store->bind(plan,descriptor);status!=error::success)return status;
                }
                if(next->sensor_store)if(auto status=next->sensor_store->prepare();status!=error::success)return status;
                if(next->device_store)
                {
                    for(auto const& [alias,setting]:entry.device_plans)
                    {
                        (void)alias;device_dispatch::device_plan plan;std::array<int,4> endpoints;
                        if(auto status=setting.freeze(plan,endpoints);status!=error::success)return status;
                        if(auto status=next->device_store->bind(plan,endpoints);status!=error::success)return status;
                    }
                    for(auto const& [alias,setting]:entry.control_plans)
                    {
                        (void)alias;device_dispatch::control_plan plan;
                        if(auto status=setting.freeze(plan);status!=error::success)return status;
                        if(auto status=next->device_store->bind(plan);status!=error::success)return status;
                    }
                    if(!next->sensor_store)return error::invalid;
                    if(auto status=next->device_store->prepare(next->sensor_store->access());status!=error::success)return status;
                    if(next->device_store->has_active_bindings())
                        if(auto status=next->device_worker->start();status!=error::success)return status;
                }
                if(next->proposal_store)
                {
                    auto source=next->sensor_store?next->sensor_store->access():proposal_dispatch::source_access{};
                    if(auto status=next->proposal_store->prepare(source,next->proposal_target_store->access());status!=error::success)return status;
                }
                if(!entry.communication.empty())
                    if(auto status=communication_provider(entry.communication,*next);status!=error::success)return status;
                if(next->bridge_store)if(auto status=next->bridge_store->start_supervisor();status!=error::success)return status;
                pending.emplace_back(&entry,std::move(next));
            }
            for(auto& [entry,state]:pending){state->driver_store=entry->drivers;entry->live=std::move(state);}
            providers_ready=true;return error::success;
#else
            (void)provider;(void)model_provider;(void)communication_provider;return error::unsupported;
#endif
        }
        /** @brief Set a capacity or explicit optional attenuation field once.
         * @details Finite call/byte windows require delivery=may-reject.
         * CLI order is independent; complete validation happens before launch.
         * Unlimited wait still requires infinite-wait=allow. Stable authority
         * never silently installs a rate limit.
         */
        error limit(std::string_view name,std::string_view alias,::uwvm2::utils::container::std_string key,std::string_view value)
        {
            std::lock_guard lock{mutex};auto it=environments.find(name);
            if(frozen||retired||it==environments.end())return error::invalid;
            auto cap=it->second.grants.find(alias);if(cap==it->second.grants.end())return error::bad_handle;
            auto setting=std::pair{::uwvm2::utils::container::std_string(alias),key};if(it->second.assigned_limits.contains(setting))return error::conflict;
            auto updated=cap->second;std::uint64_t number{};
            if(key=="delivery")
            {if(value!="stable"&&value!="may-reject")return error::invalid;updated.limits.uncertain=value=="may-reject";}
            else if(key=="infinite-wait")
            {if(value!="allow"&&value!="deny")return error::invalid;updated.infinite_wait=value=="allow";}
            else
            {
                if(!parse_unsigned(value,number))return error::invalid;
                if(key=="max-handles")updated.max_handles=number;
                else if(key=="max-resources")updated.max_resources=number;
                else if(key=="max-wait-members")updated.max_wait_members=number;
                else if(key=="max-wait-ns")updated.max_wait_ns=number;
                else if(key=="calls-per-window")updated.limits.calls=number==UINT64_MAX?std::nullopt:std::optional{number};
                else if(key=="bytes-per-window")updated.limits.bytes=number==UINT64_MAX?std::nullopt:std::optional{number};
                else if(key=="window-ns")
                {if(!number||number>INT64_MAX)return error::invalid;updated.limits.window=std::chrono::nanoseconds(static_cast<std::int64_t>(number));}
                else return error::invalid;
            }
            cap->second=std::move(updated);it->second.assigned_limits.insert(std::move(setting));return error::success;
        }
        /** @brief Copy validated, frozen core authorization as one bounded JSON document.
         * @details Reads only configuration. Membership, import exposure, grants,
         * observation reductions and foundation selections are reported separately.
         * No provider, session, guest or native file descriptor is queried here.
         * On failure the caller receives no bytes and cannot publish a partial inventory.
         */
        error dump_effective_policy(::uwvm2::utils::container::std_string& document)const;
        /** @brief Validate all policy before execution, without native effects. */
        error validate(){std::lock_guard lock{mutex};return freeze();}
        exposure visible(std::string_view module,std::string_view action) const
        {
            std::lock_guard lock{mutex};if(retired)return exposure::hidden;
            auto owner=memberships.find(module);if(owner==memberships.end())return exposure::hidden;
            return interface_exposure(environments.at(owner->second),action).first;
        }
        /** @brief Check only observation policy; never activate or enroll a caller.
         * @details Explicit membership is the ROSv2 parent boundary. ABI
         * discovery and self reporting require no resource grant. The name comes
         * from the runtime's immediate caller identity, never from guest bytes.
         */
        error report_allowed(std::string_view module,permission_report::query const& query)const
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(retired)return error::denied;
            auto owner=memberships.find(module);if(owner==memberships.end())return error::denied;
            auto const& entry=environments.at(owner->second);auto exposed=entry.interfaces.find(permission_report::import_name);
            if(exposed!=entry.interfaces.end()&&exposed->second!=exposure::allow)return error::denied;
            return entry.observation.allows(query)?error::success:error::denied;
        }
        /** @brief Linearize report delivery with trusted observation reduction.
         * @details The callback is only the synchronous checked-memory sink;
         * it cannot escape the stack frame. Revocation waits for an in-flight
         * bounded copy, and no later copy may follow a successful reduction.
         */
        template<typename Deliver>error deliver_report(std::string_view module,permission_report::query const& query,Deliver&& deliver)const
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(retired)return error::denied;
            auto owner=memberships.find(module);if(owner==memberships.end())return error::denied;
            auto const& entry=environments.at(owner->second);auto exposed=entry.interfaces.find(permission_report::import_name);
            if((exposed!=entry.interfaces.end()&&exposed->second!=exposure::allow)||!entry.observation.allows(query))return error::denied;
            return deliver()?error::success:error::invalid;
        }
        /** @brief Read the caller's ROSv2 scope without exposing its other members.
         * @details Metadata remains observable before provider activation. No
         * freeze(), select(), enroll(), capability open or native provider is
         * called. Live grant accounting is observed without replenishing it.
         */
        error report_self(std::string_view module,permission_report::query const& query,permission_report::page& output)const
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto owner=memberships.find(module);
            if(owner==memberships.end())return output.add("self","module",0,module)&&output.finish(true,false)?error::success:error::too_large;
            auto const& entry=environments.at(owner->second);
            if(!query.function.empty())
            {
                if(!callable(query.function))return error::unsupported;
                auto state=retired?exposure::hidden:interface_exposure(entry,query.function).first;
                return output.add("interface",query.function,static_cast<unsigned>(state))&&output.finish(true,!retired)?error::success:error::too_large;
            }
            if(!output.add("self","module",0,module)||!output.add("self","permission-group",0,owner->second)||
               !output.add("self","retired",retired)||!output.add("self","frozen",frozen)||
               !output.add("self","providers-ready",providers_ready)||!output.add("interface","default",0))return error::too_large;
            for(auto name:bound_queries)
            {
                auto state=retired?exposure::hidden:interface_exposure(entry,name).first;
                if(!output.add("interface",name,static_cast<unsigned>(state)))return error::too_large;
            }
            for(auto const& [name,state]:entry.interfaces)
                if(!is_bound_query(name)&&!output.add("interface",name,static_cast<unsigned>(retired?exposure::hidden:state)))return error::too_large;
            if(entry.live)
            {if(auto status=entry.live->permissions.report_permissions(output);status!=error::success)return status;}
            else
            {
                // The configured policy is inspectable before execution; no
                // runtime account is synthesized just to satisfy a report.
                for(auto const& [alias,spec]:entry.grants)
                {
                    if(!output.add(alias,"resource",0,spec.resource)||!output.add(alias,"binding",0,spec.binding)||
                       !output.add(alias,"purpose",static_cast<unsigned>(spec.kind),purpose_names[static_cast<std::size_t>(spec.kind)])||
                       !output.add(alias,"max-handles",spec.max_handles)||!output.add(alias,"max-resources",spec.max_resources)||
                       !output.add(alias,"max-wait-members",spec.max_wait_members)||!output.add(alias,"max-wait-ns",spec.max_wait_ns)||
                       !output.add(alias,"infinite-wait",spec.infinite_wait)||!output.add(alias,"calls-per-window",spec.limits.calls.value_or(UINT64_MAX))||
                       !output.add(alias,"bytes-per-window",spec.limits.bytes.value_or(UINT64_MAX))||
                       !output.add(alias,"window-ns",static_cast<std::uint64_t>(spec.limits.window.count()))||!output.add(alias,"may-reject",spec.limits.uncertain))return error::too_large;
                    for(auto const& action:spec.actions)if(!output.add(alias,"configured-action",1,action))return error::too_large;
                }
            }
            return output.finish(true,!retired)?error::success:error::too_large;
        }
        /** @brief Resolve an exact other-module observation grant; default is deny.
         * @details No runtime enrollment, capability creation or quota mutation.
         * The target is fixed by trusted policy, never a guest-selected path or
         * wildcard. The callback may only copy metadata or deliver a checked
         * report. The lock serializes delivery with deployment retirement.
         */
        template<typename Observe>error observe_module(std::string_view module,std::string_view alias,std::string_view action,Observe&& observe,
            permission_report::query const* query=nullptr)const
        {
            if(action=="capability.revoke"||!permits(purpose::module_inspection,action))return error::denied;
            return observe_metadata(module,alias,action,purpose::module_inspection,std::forward<Observe>(observe),query);
        }
        /** @brief Read VM metadata only under a separate exact vm/runtime grant.
         * @details VM observation is never inherited from self observation or
         * module inspection. It cannot enumerate module names, load code,
         * inspect memory, change execution policy or obtain host identifiers.
         * Delivery holds the same revocation lease as module observation.
         */
        template<typename Observe>error observe_vm(std::string_view module,std::string_view alias,Observe&& observe)const
        {return observe_metadata(module,alias,permission_report::vm_import_name,purpose::vm_inspection,std::forward<Observe>(observe));}
    private:
        template<typename Observe>error observe_metadata(std::string_view module,std::string_view alias,std::string_view action,purpose kind,Observe&& observe,
            permission_report::query const* query=nullptr)const
        {
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(retired)return error::denied;
            auto owner=memberships.find(module);if(owner==memberships.end())return error::denied;
            auto const& entry=environments.at(owner->second);auto exposed=entry.interfaces.find(action);
            if(exposed==entry.interfaces.end()||exposed->second!=exposure::allow)return error::denied;
            if(query&&!entry.observation.allows(*query))return error::denied;
            auto grant=entry.grants.find(alias);
            if(grant==entry.grants.end()||grant->second.kind!=kind||grant->second.binding!="metadata"||
               !grant->second.actions.contains(action))return error::denied;
            lease held;
            if(entry.live)if(auto status=entry.live->permissions.observes(alias,action,kind,held);status!=error::success)return status;
            return observe(std::string_view{grant->second.resource});
        }
    public:
        /** @brief List membership only after separate module.group authorization.
         * @details Called under observe_module's deployment lock. Native dl
         * modules have their own identity and need not belong to a ROSv2 group.
         * This method is host-only; it never changes another group's policy.
         */
        error report_module_group_locked(std::string_view target,permission_report::page& output)const
        {
            auto owner=memberships.find(target);
            if(owner==memberships.end())return output.finish(true,false)?error::success:error::too_large;
            if(!output.add("target","permission-group",0,owner->second))return error::too_large;
            for(auto const& [member,group]:memberships)
                if(group==owner->second&&!output.add("member","module",0,member))return error::too_large;
            return output.finish(true,true)?error::success:error::too_large;
        }
        /** @brief Resolve only the checked caller's explicit environment. */
        error select(std::string_view module,caller who,std::shared_ptr<broker>& output)
        {
            std::lock_guard lock{mutex};if(auto result=freeze();result!=error::success)return result;
            if(!providers_ready)return error::denied;
            auto owner=memberships.find(module);if(owner==memberships.end())return error::denied;
            auto& entry=environments.at(owner->second);
            return activate(entry,who,output);
        }
        /** @brief Admit an exposed function and select its explicit environment atomically.
         * @details Guest entries use this single locked lookup. Hidden, denied,
         * unknown and unbound calls neither freeze policy nor enroll a caller or
         * bind an epoch. Allowing an interface grants no resource authority;
         * broker admission still checks the exact capability and operation.
         * Failure leaves the caller's output unchanged. A retired deployment
         * or a different generation always fails closed.
         */
        error select_exposed(std::string_view module,std::string_view action,caller who,std::shared_ptr<broker>& output)
        {
            std::lock_guard lock{mutex};if(retired)return error::denied;
            auto owner=memberships.find(module);if(owner==memberships.end())return error::denied;
            auto& entry=environments.at(owner->second);auto exposed=entry.interfaces.find(action);
            if(exposed==entry.interfaces.end()||exposed->second!=exposure::allow)return error::denied;
            if(auto result=freeze();result!=error::success)return result;
            if(!providers_ready)return error::denied;
            return activate(entry,who,output);
        }
        /** @brief Select an already prepared environment without lazy provisioning.
         * @details Registered messages require a previously opened capability.
         * Refusal before that point must not freeze configuration or prepare a
         * model, device, native worker or other provider. This closed path only
         * checks exposure and enrolls the authenticated incarnation in the
         * existing authority; its allocator remains recoverable. It grants no
         * capability and does not skip the caller's operation/route admission.
         */
        error select_active_exposed(std::string_view module,std::string_view action,caller who,std::shared_ptr<broker>& output)
        {
            std::lock_guard lock{mutex};if(retired||!frozen||!providers_ready)return error::denied;
            auto owner=memberships.find(module);if(owner==memberships.end())return error::denied;
            auto& entry=environments.at(owner->second);auto exposed=entry.interfaces.find(action);
            if(exposed==entry.interfaces.end()||exposed->second!=exposure::allow)return error::denied;
            return activate(entry,who,output);
        }
        /** @brief Preflight an attached driver without re-enrolling every polling call.
         * @details An established caller uses the authority membership check.
         * Only a denied first call attempts normal exposure-checked enrollment.
         * Revoked/stale callers still fail the final dispatcher check. No cached
         * permission survives revocation and no packet is sent here.
         */
        error preflight_driver(std::string_view module,caller who,handle cap,drivers::operation action,std::size_t bytes,std::shared_ptr<broker>& output)
        {
            if(static_cast<std::size_t>(action)>=drivers::names.size())return error::unsupported;
            std::lock_guard lock{mutex};if(retired||!frozen||!providers_ready||!who.instance||!who.generation)return error::denied;
            auto owner=memberships.find(module);if(owner==memberships.end())return error::denied;
            auto& entry=environments.at(owner->second);
            if(!entry.live||!entry.drivers||(entry.generation&&entry.generation!=who.generation))return error::denied;
            auto status=entry.drivers->preflight(entry.live->permissions,who,module,action,cap,bytes);
            if(status==error::denied)
            {
                auto visible=entry.interfaces.find(drivers::names[static_cast<std::size_t>(action)]);
                if(visible==entry.interfaces.end()||visible->second!=exposure::allow)return error::denied;
                std::shared_ptr<broker> selected;if(auto activated=activate(entry,who,selected);activated!=error::success)return activated;
                status=entry.drivers->preflight(selected->permissions,who,module,action,cap,bytes);
            }
            if(status==error::success)output=entry.live;
            return status;
        }
        /** @brief Preflight a registered message in an already prepared environment.
         * @details Never freezes policy, starts a provider or charges an
         * admission. Already enrolled callers take one indexed authority path.
         * First use preserves explicit group sharing: only an exactly bound
         * module with allowed exposure can enroll its authenticated incarnation.
         * Retired principals and mismatched epochs remain permanently denied. Frozen
         * exact action bits replace a duplicate string exposure lookup; the
         * authority still checks membership, handle, revocation, operation and
         * attenuation under its lease. Deployment-to-authority lock order
         * matches activation and retirement. Unknown actions fail before
         * indexing. Cleanup bypasses traffic metering, never authorization.
         * Refusal leaves the broker output unchanged and retains no lease.
         * After copying, the carrier must release this lease and repeat charged
         * admission before dispatch; preflight does not reserve permission.
         */
        error preflight_module_call(std::string_view module,caller who,handle capability,module_exchange::operation action,
                                    std::shared_ptr<broker>& output,lease& admitted,bool* clear_after_copy=nullptr)
        {
            admitted.release();
            if(static_cast<std::size_t>(action)>=module_exchange::names.size())return error::unsupported;
            std::lock_guard lock{mutex};if(retired||!frozen||!providers_ready||!who.instance||!who.generation)return error::denied;
            auto owner=memberships.find(module);if(owner==memberships.end())return error::denied;
            auto& entry=environments.at(owner->second);
            if((entry.generation&&entry.generation!=who.generation)||!entry.live)return error::denied;
            bool cleanup=action==module_exchange::operation::cancel||action==module_exchange::operation::acknowledge;
            lease checked;auto status=entry.live->permissions.admit(who,capability,action,0,checked,false,!cleanup);
            if(status==error::denied)
            {
                // An exposed group member may receive a group capability from
                // another member. Preserve its first-call enrollment, but do
                // not repeat exposure lookup and enrollment on successful calls.
                auto exposed=entry.interfaces.find(module_exchange::names[static_cast<std::size_t>(action)]);
                if(exposed==entry.interfaces.end()||exposed->second!=exposure::allow)return error::denied;
                if(auto enrolled=entry.live->permissions.enroll(who);enrolled!=error::success)return enrolled;
                entry.generation=who.generation;
                status=entry.live->permissions.admit(who,capability,action,0,checked,false,!cleanup);
            }
            if(status!=error::success)return status;
            if(checked.specification().kind!=purpose::module_call||checked.specification().binding!="registered-message")return error::denied;
            if(clear_after_copy)
            {
                bool clear=entry.module_inputs!=environment::input_policy::retain;
                if(entry.module_inputs==environment::input_policy::mixed)
                {
                    auto route=entry.module_call_plans.find(checked.specification().resource);
                    if(route==entry.module_call_plans.end())return error::denied;
                    clear=route->second.clear_after_copy;
                }
                *clear_after_copy=clear;
            }
            output=entry.live;admitted=std::move(checked);return error::success;
        }
        /** @brief Remove one incarnation without revoking other group members. */
        void retire_member(caller who)
        {std::lock_guard lock{mutex};for(auto& entry:environments)if(entry.second.live)
            {entry.second.live->permissions.retire_member(who);entry.second.live->module_calls.retire_member(who);if(entry.second.live->driver_store)entry.second.live->driver_store->retire_member(who);}}
        /** @brief Permanently retire all environments and wake all local waits. */
        void retire()
        {std::lock_guard lock{mutex};retired=true;for(auto& entry:environments)if(entry.second.live)entry.second.live->retire();}
        /** @brief Retire only authority used by the exact departing runtime epoch.
         * @details Runtime preparation retires an empty previous world before
         * loading the first guest. That event must not destroy unactivated CLI
         * configuration. The first checked caller binds an environment to its
         * epoch permanently; later epochs cannot inherit it. Once an active
         * environment's epoch retires, this deployment cannot be relaunched.
         */
        void retire_generation(std::uint64_t generation)
        {
            std::lock_guard lock{mutex};
            for(auto& [name,entry]:environments)
                if(entry.generation==generation&&entry.generation)
                {entry.live->retire();retired=true;}
        }
        /** @brief Read only bounded logical labels and stored resource counters.
         * @details Never freezes policy, enrolls callers, replenishes quotas,
         * consumes events or invokes providers. Locks are attempted once; busy
         * or oversized state yields an empty explicitly unavailable snapshot.
         * The host hook separately enforces debugger inspection permission.
         */
        void inspect(debug::request const& query,debug::view& output)
        {
            output={};output.selected=query.selected;
            auto fail=[&](debug::status reason){output={};output.selected=query.selected;output.result=reason;};
            if(!debug::valid(query)){fail(debug::status::invalid_request);return;}
            // The result journal has its own mutex and needs no environment lock.
            if(query.selected==debug::selection::audit)
            {
                if(!result_audit.inspect(output.audit)){fail(debug::status::busy);return;}
                output.result=debug::status::ok;
                output.total=1;
                if(!query.first){output.size=1;output.next=1;}
                return;
            }
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()){fail(debug::status::busy);return;}
            std::size_t scanned{};
            if(query.selected==debug::selection::policy)
            {
                if(query.operation.size&&!callable(query.operation.view())){fail(debug::status::invalid_request);return;}
                auto collect=[&](auto const& environment_item)
                {
                    auto const& [name,entry]=environment_item;
                    auto emit=[&](std::string_view action)
                    {
                        auto index=output.total++;
                        if(index<query.first)return;
                        if(output.size==query.count){output.more=true;return;}
                        auto const [mode,configured]=interface_exposure(entry,action);
                        auto label=[](exposure v){return v==exposure::allow?"allow":v==exposure::deny?"deny":"hidden";};
                        auto& item=output.policy[output.size++];
                        item.environment.assign(name);item.operation.assign(action);
                        item.configured=configured?label(mode):"unset";
                        item.effective=retired?"hidden":label(mode);
                        item.origin=retired?"retired":configured?"explicit":is_bound_query(action)?"self-default":"default";
                        output.next=index+1;
                    };
                    if(query.operation.size)
                    {
                        if(++scanned>debug::maximum_scan)return false;
                        emit(query.operation.view());
                    }
                    else
                    {
                        if(callable_names.size()>debug::maximum_scan-scanned)return false;
                        scanned+=callable_names.size();
                        for(auto action:callable_names)emit(action);
                    }
                    return true;
                };
                if(query.environment.size)
                {
                    auto found=environments.find(query.environment.view());
                    if(found!=environments.end()&&!collect(*found)){fail(debug::status::resource_limit);return;}
                }
                else for(auto const& item:environments)
                    if(!collect(item)){fail(debug::status::resource_limit);return;}
                output.result=debug::status::ok;return;
            }
            for(auto const& [name,entry]:environments)
            {
                if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                if(query.environment.size&&query.environment.view()!=name)continue;
                if(!entry.live)continue;
                auto platform_metadata=[&](std::string_view alias,debug::row& value)
                {
                    auto controlled=entry.control_plans.find(alias);
                    auto device=controlled==entry.control_plans.end()?alias:controlled->second.device_alias();
                    auto module=entry.platform_children.owner(device);value.platform.assign(module);
                    value.platform_enabled=!module.empty()&&entry.platform_children.enabled(module);
                };
                if(query.selected==debug::selection::authority)
                {
                    ::uwvm2::utils::container::std_vector<authority_snapshot> rows;
                    if(!entry.live->permissions.inspect(rows,debug::maximum_scan-scanned)){fail(debug::status::busy);return;}
                    for(auto const& row:rows)
                    {
                        ++scanned;debug::row value;value.kind=debug::row_kind::grant;value.environment.assign(name);
                        value.alias.assign(row.alias);value.resource.assign(row.resource);value.binding.assign(row.binding);
                        value.open_handles=row.open_handles;value.admissions=row.admissions;value.refused=row.refused;
                        value.revoked=row.revoked;value.attenuated=row.attenuated;
                        auto index=static_cast<std::size_t>(row.kind);
                        if(index>=purpose_names.size()){fail(debug::status::invalid_request);return;}
                        value.type.assign(purpose_names[index]);platform_metadata(row.alias,value);debug::add(query,output,value);
                    }
                }
                else
                {
                    ::uwvm2::utils::container::std_vector<module_exchange::inspection> calls;
                    if(!entry.live->module_calls.inspect(calls,debug::maximum_scan-scanned)){fail(debug::status::busy);return;}
                    for(auto const& row:calls)
                    {
                        ++scanned;debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(row.route);value.resource.assign(row.route);value.binding.assign("registered-message");
                        value.type.assign("module.call");value.members=row.slots;value.open_handles=row.occupied;
                        value.sample_count=row.delivered;value.query_count=row.quarantined;value.reserved_bytes=row.reserved;value.retired=row.retired;
                        value.clear_after_copy=row.clear_after_copy;
                        value.state.assign(row.retired?"retired":row.quarantined?"quarantined":row.occupied?"active":"ready");
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    ::uwvm2::utils::container::std_vector<resources::snapshot> rows;
                    if(!entry.live->runtime.inspect(rows,debug::maximum_scan-scanned)){fail(debug::status::busy);return;}
                    for(auto const& row:rows)
                    {
                        ++scanned;debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(row.grant);value.id=row.id;value.sequence=row.sequence;value.members=row.members;
                        value.retired=row.retired;value.armed=row.armed;
                        constexpr std::array names{"guard","clock","timer","wait"};value.type.assign(names[static_cast<std::size_t>(row.type)]);
                        debug::add(query,output,value);
                    }
                    ::uwvm2::utils::container::std_vector<artifacts::snapshot> artifact_rows;
                    if(!entry.live->inputs.inspect(artifact_rows,debug::maximum_scan-scanned)){fail(debug::status::busy);return;}
                    for(auto const& row:artifact_rows)
                    {
                        ++scanned;debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(row.alias);value.resource.assign(row.resource);value.binding.assign(row.binding);
                        value.type.assign("artifact");value.id=row.id;value.size_bytes=row.size_bytes;value.immutable=true;value.retired=row.retired;
                        debug::add(query,output,value);
                    }
                    ::uwvm2::utils::container::std_vector<outputs::snapshot> output_rows;
                    if(!entry.live->output_store.inspect(output_rows,debug::maximum_scan-scanned)){fail(debug::status::busy);return;}
                    for(auto const& row:output_rows)
                    {
                        ++scanned;debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(row.alias);value.resource.assign(row.resource);value.binding.assign(row.binding);value.id=row.id;
                        value.type.assign(row.receipt_only?"artifact.receipt":"artifact.output");
                        constexpr std::array states{"none","staged","published","aborted","removed"};
                        if(row.state>=states.size()){fail(debug::status::resource_limit);return;}
                        value.state.assign(states[row.state]);value.accepted_bytes=row.bytes;value.reserved_bytes=row.reserved;
                        value.retained_receipts=row.retained_receipts;value.published=row.published;value.retired=row.retired;
                        debug::add(query,output,value);
                    }
                    // inspect() copies cached metadata; state()/transitions()
                    // enforce deadlines and must never run from the debugger.
                    ::uwvm2::utils::container::std_vector<managed_lifecycle::inspection> lifecycle_rows;
                    if(!entry.live->managed_nodes.inspect(lifecycle_rows,debug::maximum_scan-scanned)){fail(debug::status::busy);return;}
                    for(auto const& row:lifecycle_rows)
                    {
                        ++scanned;debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(row.alias);value.type.assign("lifecycle");value.binding.assign("local-lifecycle");
                        auto grant=entry.grants.find(row.alias);
                        if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        value.resource.assign(grant->second.resource);value.open_handles=row.handles;value.retired=row.retired;
                        switch(row.current.state)
                        {
                        case 1:value.state.assign("unconfigured");break;case 2:value.state.assign("inactive");break;
                        case 3:value.state.assign("active");break;case 4:value.state.assign("finalized");break;
                        case 10:value.state.assign("configuring");break;case 11:value.state.assign("cleaning_up");break;
                        case 12:value.state.assign("shutting_down");break;case 13:value.state.assign("activating");break;
                        case 14:value.state.assign("deactivating");break;case 15:value.state.assign("error_processing");break;
                        default:fail(debug::status::resource_limit);return;
                        }
                        if(row.current.delivered>1){fail(debug::status::resource_limit);return;}
                        value.generation=row.current.generation;value.transition=row.current.transition;
                        value.deadline_ns=row.current.deadline;value.status_code=row.current.status;
                        value.callback_pending=row.current.request!=0;value.callback_delivered=row.current.delivered!=0;
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.communication.bindings())
                    {
                        (void)plan;if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        if(!entry.live->communication_store){fail(debug::status::busy);return;}
                        communication_dispatch::usage usage;
                        if(grant->second.kind==purpose::action)
                        {
                            communication_action_dispatch::usage action;
                            if(entry.live->communication_store->inspect_action(alias,action)!=error::success){fail(debug::status::busy);return;}
                            usage.configured=action.configured;usage.handles=action.handles;usage.orphaned=action.fenced;usage.ready=action.ready;usage.retired=action.retired;
                        }
                        else if(grant->second.kind==purpose::discovery)
                        {
                            communication_graph_dispatch::usage graph;
                            if(entry.live->communication_store->inspect_observation<communication_graph_dispatch>(alias,graph)!=error::success){fail(debug::status::busy);return;}
                            usage.configured=graph.configured;usage.handles=graph.watches;usage.ready=graph.ready;usage.retired=graph.retired;
                        }
                        else if(grant->second.kind==purpose::event)
                        {
                            communication_event_dispatch::usage event;
                            if(entry.live->communication_store->inspect_observation<communication_event_dispatch>(alias,event)!=error::success){fail(debug::status::busy);return;}
                            usage.configured=event.configured;usage.handles=event.streams;usage.ready=event.ready;usage.retired=event.retired;
                        }
                        else if(entry.live->communication_store->inspect(alias,usage)!=error::success){fail(debug::status::busy);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);value.alias.assign(alias);
                        value.resource.assign(grant->second.resource);value.binding.assign(grant->second.binding);value.type.assign("communication");
                        value.members=usage.configured;value.open_handles=usage.handles;value.retained_receipts=usage.orphaned;value.retired=usage.retired;
                        value.state.assign(usage.retired?"retired":usage.ready?"ready":"configured");
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.parameters.entries())
                    {
                        (void)plan;if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        parameters::usage usage;
                        if(!entry.live->parameter_store||!entry.live->parameter_store->try_inspect(alias,usage))
                        {fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);value.alias.assign(alias);
                        value.resource.assign(grant->second.resource);value.binding.assign("local-parameters");value.type.assign("parameter");
                        value.members=usage.retained_scopes;value.open_handles=usage.scopes;value.sample_count=usage.parameters;
                        value.query_count=usage.cursors;value.retained_bytes=usage.bytes;value.workspace_bytes=usage.workspace;
                        value.input_bytes=usage.event_bytes;value.output_items=usage.events;
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.buffer_plans)
                    {
                        (void)plan;
                        if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        buffers::usage usage;
                        if(entry.live->buffer_store.try_inspect(alias,usage)!=error::success){fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("buffer");value.binding.assign("sealed-buffer");
                        value.reserved_bytes=usage.bytes;value.members=usage.objects;value.open_handles=usage.active_handles;
                        value.lease_count=usage.leases;value.workspace_bytes=usage.workspace;
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.model_plans)
                    {
                        (void)plan;
                        if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        models::usage usage;
                        if(entry.live->model_store.try_inspect(alias,usage)!=error::success){fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("model");value.binding.assign("local-model");
                        value.members=usage.requests;value.open_handles=usage.sessions;value.input_bytes=usage.inputs;
                        value.output_bytes=usage.output_bytes;value.output_items=usage.output_items;value.workspace_bytes=usage.workspace;
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.transform_plans)
                    {
                        (void)plan;
                        if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        transforms::usage usage;
                        if(entry.live->transform_store.inspect(alias,usage)!=error::success){fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("transform");value.binding.assign("local-transform");
                        value.open_handles=usage.open_handles;value.members=usage.retained_sessions;value.retired=usage.retired;
                        value.reserved_bytes=usage.data_bytes;value.workspace_bytes=usage.workspace_bytes;
                        value.sample_count=usage.samples;value.query_count=usage.queries;value.pending_queries=usage.pending;
                        value.query_bytes=usage.request_bytes;value.carrier_bytes=usage.carrier_bytes;
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    auto recording_row=[&](std::string_view alias,bool replay)
                    {
                        if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return false;}
                        recording_dispatch::inspection usage;
                        if(!entry.live->recording_store||entry.live->recording_store->inspect(alias,usage)!=error::success)
                        {fail(debug::status::busy);return false;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()||usage.replay!=replay){fail(debug::status::resource_limit);return false;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);
                        value.type.assign(replay?"replay":"recording");value.binding.assign(replay?"private-replay":"private-recording");
                        value.open_handles=usage.open_handles;value.reserved_bytes=usage.reserved_bytes;value.retired=usage.retired;
                        value.state.assign(usage.retired?"retired":(replay?usage.open_handles!=0:usage.store.recording)?"active":"idle");
                        if(replay)value.members=usage.open_handles;
                        else
                        {
                            value.members=usage.store.sealed_segments;value.retained_bytes=usage.store.sealed_bytes;
                            value.sample_count=usage.store.active_messages;value.query_count=usage.store.replays;value.lease_count=usage.store.pins;
                        }
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return false;}
                        debug::add(query,output,value);return true;
                    };
                    for(auto const& [alias,plan]:entry.recording_plans){(void)plan;if(!recording_row(alias,false))return;}
                    for(auto const& [alias,plan]:entry.replay_plans){(void)plan;if(!recording_row(alias,true))return;}
                    for(auto const& alias:entry.driver_aliases)
                    {
                        if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        drivers::snapshot usage;
                        if(!entry.live->driver_store||!entry.live->driver_store->inspect(alias,usage)){fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);value.alias.assign(alias);
                        value.resource.assign(grant->second.resource);value.type.assign("driver");value.binding.assign("registered-driver");platform_metadata(alias,value);
                        value.sequence=usage.sequence;value.status_code=usage.status;value.query_count=usage.effect_unknown?1:0;
                        constexpr std::array<std::string_view,7> states{"prepared","handshake","ready","pending","result","closed","failed"};
                        value.state.assign(states[static_cast<std::size_t>(usage.phase)]);
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}debug::add(query,output,value);
                    }
                    auto device_row=[&](auto const& alias,bool control)
                    {
                        if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return false;}
                        device_dispatch::inspection usage;
                        if(!entry.live->device_store||entry.live->device_store->inspect(alias,usage)!=error::success){fail(debug::status::busy);return false;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return false;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);value.alias.assign(alias);value.resource.assign(grant->second.resource);
                        value.type.assign(control?"control":"device");value.binding.assign(control?"framed-control":"framed-scalar");
                        platform_metadata(alias,value);
                        value.open_handles=usage.handles;value.members=control?1:usage.handles;value.reserved_bytes=usage.retained_bytes;
                        value.sample_count=usage.requests;value.query_count=usage.unknown;value.lease_count=usage.pins;
                        value.generation=usage.controller_epoch;value.sequence=usage.sample_sequence;value.retired=usage.retired;
                        auto state=usage.retired?"retired":usage.unknown?"outcome_unknown":usage.stop_acknowledged?"stop_accepted":
                            usage.stop_pending?"stopping":usage.ready&&!usage.latched?"ready":"prepared";
                        value.state.assign(state);value.armed=value.state.view()=="ready";
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return false;}debug::add(query,output,value);return true;
                    };
                    for(auto const& [alias,plan]:entry.device_plans){(void)plan;if(!device_row(alias,false))return;}
                    for(auto const& [alias,plan]:entry.control_plans){(void)plan;if(!device_row(alias,true))return;}
                    for(auto const& [alias,plan]:entry.bridge_plans)
                    {
                        (void)plan;if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        bridge_dispatch::inspection usage;
                        if(!entry.live->bridge_store||entry.live->bridge_store->inspect(alias,usage)!=error::success)
                        {fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("bridge");value.binding.assign("topic-forwarder");
                        value.open_handles=usage.handles;value.members=1;value.reserved_bytes=usage.reserved_bytes;
                        value.sample_count=usage.pending;value.query_count=usage.unknown;value.output_items=usage.forwarded;value.lost_records=usage.losses;
                        value.busy=usage.busy;value.retired=usage.retired;
                        value.state.assign(usage.retired?"retired":usage.unknown?"outcome_unknown":usage.active?"running":"idle");
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.media_plans)
                    {
                        (void)plan;if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        media_dispatch::inspection usage;
                        if(!entry.live->media_store||entry.live->media_store->inspect(alias,usage)!=error::success)
                        {fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("media");value.binding.assign("immutable-buffer-media");
                        value.open_handles=usage.streams;value.members=usage.retained_streams;value.reserved_bytes=usage.retained_bytes;
                        value.sample_count=usage.pending;value.workspace_bytes=usage.workspace_bytes;value.output_items=usage.completed;value.exported_records=usage.delivered;
                        value.retired=usage.retired;value.state.assign(usage.retired?"retired":usage.pending?"active":"idle");
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.sensor_plans)
                    {
                        (void)plan;if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        sensor_dispatch::inspection usage;
                        if(!entry.live->sensor_store||entry.live->sensor_store->inspect(alias,usage)!=error::success)
                        {fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("sensor");value.binding.assign("bounded-sensor");
                        value.open_handles=usage.source.handles;value.members=usage.source.retained_handles;
                        value.retained_bytes=usage.source.payload_bytes;value.reserved_bytes=usage.source.reserved_bytes;
                        value.sample_count=usage.source.records;value.lease_count=usage.source.captures;
                        value.lost_records=usage.source.losses;value.refused=usage.source.full_refusals;value.counters_saturated=usage.source.counters_saturated;
                        value.workspace_bytes=usage.workspace_bytes;value.input_bytes=usage.native.bytes;value.output_items=usage.native.frames;
                        value.status_code=static_cast<std::uint64_t>(usage.native.failure);
                        value.busy=usage.source.ingesting||usage.source.delivering||usage.native.busy;value.retired=usage.source.retired||usage.native.retired;
                        value.state.assign(value.retired?"retired":usage.native.failure!=error::success?"failed":usage.native.active?"active":"idle");
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.proposal_plans)
                    {
                        (void)plan;if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        proposal_dispatch::usage usage;
                        if(!entry.live->proposal_store||entry.live->proposal_store->inspect(alias,usage)!=error::success)
                        {fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("model.proposal");value.binding.assign("model-proposals");
                        value.members=usage.proposals;value.lease_count=usage.readers;value.sample_count=usage.captures;value.query_count=usage.tools;
                        value.workspace_bytes=usage.workspace_bytes;value.retired=usage.retired;
                        value.state.assign(usage.retired?"retired":usage.proposals?"active":"idle");
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.log_plans)
                    {
                        (void)plan;if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        logs::usage usage;
                        if(!entry.live->log_diagnostic_store||entry.live->log_diagnostic_store->inspect_log(alias,usage)!=error::success)
                        {fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("log");value.binding.assign("private-log");
                        value.members=usage.retained;value.reserved_bytes=usage.reserved_bytes;value.retained_bytes=usage.retained_bytes;
                        value.emitted_records=usage.accepted;value.exported_records=usage.exported;value.lost_records=usage.lost;
                        value.refused=usage.full_refusals;value.copy_failures=usage.copy_failures;value.counters_saturated=usage.counters_saturated;
                        value.retired=usage.retired;value.busy=usage.draining;
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                    for(auto const& [alias,plan]:entry.diagnostic_plans)
                    {
                        (void)plan;if(++scanned>debug::maximum_scan){fail(debug::status::resource_limit);return;}
                        diagnostics::usage usage;
                        if(!entry.live->log_diagnostic_store||entry.live->log_diagnostic_store->inspect_diagnostic(alias,usage)!=error::success)
                        {fail(debug::status::busy);return;}
                        auto grant=entry.grants.find(alias);if(grant==entry.grants.end()){fail(debug::status::resource_limit);return;}
                        debug::row value;value.kind=debug::row_kind::resource;value.environment.assign(name);
                        value.alias.assign(alias);value.resource.assign(grant->second.resource);value.type.assign("diagnostic");value.binding.assign("local-diagnostic");
                        value.members=usage.rows;value.reserved_bytes=usage.reserved_bytes;value.pending_page=usage.pending;
                        value.busy=usage.busy;value.retired=usage.retired;
                        if(!debug::valid(value)){fail(debug::status::resource_limit);return;}
                        debug::add(query,output,value);
                    }
                }
            }
            output.result=debug::status::ok;
        }
    };
}

#include "policy_dump.h"
