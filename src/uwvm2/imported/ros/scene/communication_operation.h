/** @file
 * @brief Owned, closed authority token for one queued native communication call.
 * @details The dispatcher supplies the ACTUAL current caller/capability and
 * complete resource origins on every call. A provider may not substitute the
 * first opener's capability or infer authority from an owner, profile, endpoint
 * name or native handle. This object is never accepted from Wasm memory.
 *
 * Immutable retained_origins keeps the dispatcher's resource-origin record alive
 * until stop/drain. A supervisor callback revalidates each caller incarnation,
 * exact capability, owner and resource under one current authority group before
 * nonblocking send and again before guest delivery. The token is not a lease.
 * No native wait may hold that group. Closed permission enums avoid retaining
 * guest strings or silently accepting an undeclared secondary action.
 */
#pragma once
#include "communication_identity.h"
#include <array>
#include <memory>
#include <string_view>
namespace uwvm2::imported::ros::scene::communication
{
    enum class operation_family:std::uint8_t{unconfigured,context,node,type,topic,service,action,graph};
    enum class permission:std::uint8_t
    {
        unconfigured,context_open,context_describe,context_shutdown,node_open,node_describe,type_describe,
        topic_open,topic_publish,topic_take,topic_describe,qos_describe,qos_liveliness_assert,qos_acknowledgements_wait,
        service_open,service_submit,service_take,service_respond,service_available,service_abandon,
        action_open,action_submit,action_take,action_respond,action_cancel,action_describe,
        action_accept,action_reject,action_execute,action_feedback,action_succeed,action_abort,action_finish_cancelled,action_cancel_respond,
        graph_snapshot,graph_watch,resource_close,resource_observe,communication_open_take,communication_open_cancel,request_describe,request_release,service_describe,
        topic_reserve,topic_acknowledge,topic_release,service_reserve,service_acknowledge,service_release,action_cancel_set
    };
    inline constexpr std::array permission_names{
        std::string_view{"unconfigured"},std::string_view{"context.open"},std::string_view{"context.describe"},std::string_view{"context.shutdown"},
        std::string_view{"node.open"},std::string_view{"node.describe"},std::string_view{"type.describe"},std::string_view{"topic.open"},
        std::string_view{"topic.publish"},std::string_view{"topic.take"},std::string_view{"topic.describe"},std::string_view{"qos.describe"},
        std::string_view{"qos.liveliness.assert"},std::string_view{"qos.acknowledgements.wait"},std::string_view{"service.open"},
        std::string_view{"service.submit"},std::string_view{"service.take"},std::string_view{"service.respond"},std::string_view{"service.available"},
        std::string_view{"service.abandon"},std::string_view{"action.open"},std::string_view{"action.submit"},std::string_view{"action.take"},
        std::string_view{"action.respond"},std::string_view{"action.cancel"},std::string_view{"action.describe"},std::string_view{"action.accept"},
        std::string_view{"action.reject"},std::string_view{"action.execute"},std::string_view{"action.feedback"},std::string_view{"action.succeed"},
        std::string_view{"action.abort"},std::string_view{"action.finish.cancelled"},std::string_view{"action.cancel.respond"},
        std::string_view{"graph.snapshot"},std::string_view{"graph.watch"},std::string_view{"resource.close"},std::string_view{"resource.observe"},
        std::string_view{"communication.open.take"},std::string_view{"communication.open.cancel"},std::string_view{"request.describe"},std::string_view{"request.release"},std::string_view{"service.describe"},
        std::string_view{"topic.reserve"},std::string_view{"topic.acknowledge"},std::string_view{"topic.release"},
        std::string_view{"service.reserve"},std::string_view{"service.acknowledge"},std::string_view{"service.release"},std::string_view{"action.cancel.set"}};
    static_assert(permission_names.size()==static_cast<std::size_t>(permission::action_cancel_set)+1);
    inline permission parse_permission(std::string_view name)noexcept
    {for(std::size_t i{1};i<permission_names.size();++i)if(permission_names[i]==name)return static_cast<permission>(i);return permission::unconfigured;}
    inline std::string_view permission_name(permission value)noexcept
    {auto i=static_cast<std::size_t>(value);return i<permission_names.size()?permission_names[i]:std::string_view{};}
    struct operation_origin
    {
        owner authority;std::uint64_t capability{},resource{};permission action{};
        friend bool operator==(operation_origin const&,operation_origin const&)=default;
    };
    struct operation_token
    {
        void const* caller_instance{};std::uint64_t caller_generation{};
        owner authority;std::uint64_t capability{},resource{},request{},session{},sequence{};
        /// @brief Exact graph watch incarnation for readiness; zero for all other calls.
        std::uint64_t watch{};
        operation_family family{};permission action{},secondary{};
        std::array<operation_origin,8> origins{};std::size_t count{};
        std::shared_ptr<void const> retained_origins;
        /** @brief Bind the registry's actual receipt ID and already-validated caller key. */
        operation_token with_request(std::uint64_t id,std::uint64_t caller_session=0,std::uint64_t caller_sequence=0)const noexcept
        {auto result=*this;result.request=id;result.session=caller_session;result.sequence=caller_sequence;return result;}
        /** @brief Check exact complete fixed-role origins without granting authority.
         * @details Order is primary, node, context, schemas, secondary. Node has
         * primary/context; context/type have only primary. Topic needs one schema,
         * service two, action three. Graph adds the explicitly selected context's
         * describe capability after the node's original context.open capability;
         * both must name the same context resource and owner. action.respond adds exactly one explicit
         * tag-specific secondary claim on the same action capability/resource.
         * A linked service.available observation includes its independently
         * authorized client service.submit handle last. The client contributes
         * validation only and must own this exact endpoint and native profile.
         * A missing context, omitted schema, truncation or nonempty tail rejects.
         */
        bool valid(permission expected,operation_family expected_family,owner expected_owner,std::uint64_t expected_resource,
                   bool require_request=true)const noexcept
        {
            if(!caller_instance||!caller_generation||!authority||authority!=expected_owner||!capability||resource!=expected_resource||
                family!=expected_family||family==operation_family::unconfigured||family>operation_family::graph||action!=expected||
                action==permission::unconfigured||permission_name(action).empty()||!retained_origins||(require_request&&!request))return false;
            bool response=action==permission::action_respond;
            if((family==operation_family::graph&&action==permission::resource_observe)?!watch:watch!=0)return false;
            bool linked=family==operation_family::service&&action==permission::service_available&&secondary==permission::service_submit;
            if(response?(family!=operation_family::action||secondary<permission::action_accept||secondary>permission::action_cancel_respond):!linked&&secondary!=permission::unconfigured)return false;
            std::size_t schemas=family==operation_family::topic?1:family==operation_family::service?2:family==operation_family::action?3:0;
            std::size_t base=family==operation_family::context||family==operation_family::type?1:family==operation_family::node?2:3;
            auto expected_count=base+schemas+static_cast<std::size_t>(response||linked)+static_cast<std::size_t>(family==operation_family::graph);
            if(count!=expected_count||count>origins.size())return false;
            if(origins[0]!=operation_origin{authority,capability,resource,action})return false;
            for(std::size_t i{1};i<count;++i)
            {
                auto const& origin=origins[i];if(!origin.authority||origin.authority.environment!=authority.environment||!origin.capability)return false;
                auto required=(response||linked)&&i==count-1?secondary:family==operation_family::node?permission::context_open:i==1?permission::node_open:i==2?permission::context_open:
                    family==operation_family::graph?permission::context_describe:permission::type_describe;
                if(origin.action!=required)return false;
                if(required==secondary&&response)
                {if(origin.authority!=authority||origin.capability!=capability||origin.resource!=resource)return false;}
                else if(linked&&i==count-1)
                {if(origin.authority!=authority||origin.capability==capability||origin.resource!=resource)return false;}
                else if(required==permission::type_describe){if(origin.resource)return false;}
                else if(!origin.resource)return false;
            }
            if(family==operation_family::graph&&(origins[2].authority!=origins[3].authority||origins[2].resource!=origins[3].resource))return false;
            for(std::size_t i{count};i<origins.size();++i)if(origins[i]!=operation_origin{})return false;
            return true;
        }
    };
}
