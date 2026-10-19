/** @file
 * Explicit complete pre-RIHS Service/Action admission from installed IDL.
 *
 * Only an independently admitted native build may specialize interface_traits.
 * Actual request/response metadata and all five Action type-support links must
 * match the exact compiled types. Every payload, protocol message and standard
 * dependency has its separate installed_source::traits proof. No ServiceEvent
 * is projected into a legacy SDK which does not contain it. Derived identities
 * describe this selected legacy declaration graph, never a baked native hash.
 * Modern hash-capable support cannot enter this path, even with a broken hash.
 * No Node, endpoint, registry, Guest import or authority is created.
 */
#pragma once
#include "installed_source.h"
#include <initializer_list>
#if __has_include(<rosidl_typesupport_introspection_cpp/service_introspection.hpp>)
#include <rosidl_typesupport_introspection_cpp/service_introspection.hpp>
#define UWVM_ROS2_INSTALLED_SERVICE_METADATA 1
#else
#define UWVM_ROS2_INSTALLED_SERVICE_METADATA 0
#endif
#if __has_include(<rosidl_typesupport_cpp/action_type_support.hpp>)
#include <rosidl_typesupport_cpp/action_type_support.hpp>
#define UWVM_ROS2_INSTALLED_ACTION_SUPPORT 1
#else
#define UWVM_ROS2_INSTALLED_ACTION_SUPPORT 0
#endif

namespace uwvm2::ros_gateway::ros2_compat::installed_source
{
    /** This trait is a trusted compiler input, never runtime/Guest DATA. */
    template<class Native> struct interface_traits
    {
        static constexpr bool enabled=false;
        static constexpr unsigned kind=0; // 1: Service, 2: Action.
        inline static constexpr std::string_view type{},source_proof_sha256{};
        static constexpr ipc::custom_native_schema schema() noexcept {return {};}
    };

    /** Cold bounded graph assembly. Virtual interface fields have stable local
     * storage; actual Message fields borrow immutable generated declarations. */
    struct protocol_graph
    {
        std::array<std::array<ipc::custom_native_field,6>,3> fields{};
        std::size_t used{};
        std::array<ipc::custom_native_record,256> rows{};
        std::size_t count{};
        bool append(ipc::custom_native_record row)
        {
            for(std::size_t i{};i<count;++i)if(rows[i].type==row.type)
                return same_schema({&rows[i],1},{&row,1});
            if(count==rows.size())return false;
            rows[count++]=row;return true;
        }
        bool add(std::string_view type,std::initializer_list<ipc::custom_native_field> values)
        {
            if(used==fields.size()||values.size()>fields[0].size())return false;
            auto& storage=fields[used++];std::size_t i{};
            for(auto const& value:values)storage[i++]=value;
            return append({type,storage.data(),values.size()});
        }
        template<class Native> bool message()
        {
            if constexpr(!traits<Native>::enabled)return false;
            else
            {
                if(!digest(traits<Native>::source_proof_sha256)||!traits<Native>::prove())return false;
                auto schema=traits<Native>::schema();
                if(!schema.records||!schema.record_count||schema.record_count>256)return false;
                for(std::size_t i{};i<schema.record_count;++i)if(!append(schema.records[i]))return false;
                return true;
            }
        }
        ipc::custom_native_schema view() const noexcept {return {rows.data(),count};}
    };

#if UWVM_ROS2_INSTALLED_SERVICE_METADATA
    using service_members=::rosidl_typesupport_introspection_cpp::ServiceMembers;
    template<class Members> auto service_namespace(Members const& value,int)
        -> decltype(value.service_namespace_) {return value.service_namespace_;}
    template<class Members> auto service_namespace(Members const& value,long)
        -> decltype(value.package_name_) {return value.package_name_;}
    template<class Native> service_members const* service_metadata()
    {
        auto root=::rosidl_typesupport_cpp::get_service_type_support_handle<Native>();
        if(!root||!root->typesupport_identifier)return nullptr;
        constexpr char selected[]="rosidl_typesupport_introspection_cpp";
        auto actual=std::strcmp(root->typesupport_identifier,selected)==0?root:
            (root->func?root->func(root,selected):nullptr);
        if(!actual||!actual->typesupport_identifier||std::strcmp(actual->typesupport_identifier,selected)||!actual->data)return nullptr;
        return static_cast<service_members const*>(actual->data);
    }
    template<class Native> bool service_members_match(service_members const* actual,std::string_view root)
    {
        using request=typename Native::Request;using response=typename Native::Response;
        if constexpr(!traits<request>::enabled||!traits<response>::enabled)return false;
        else
        {
            auto first=root.find('/'),last=root.rfind('/');
            if(!actual||root.size()>255||first==std::string_view::npos||first==last)return false;
            auto category=root.substr(first+1,last-first-1);
            if(category!="srv"&&category!="action")return false;
            auto space=std::string{root.substr(0,first)}+"::"+std::string{category};
            if(!native_string(actual->service_name_,root.substr(last+1))||
               (!native_string(service_namespace(*actual,0),space)&&
                !native_string(service_namespace(*actual,0),root.substr(0,first)))||
               traits<request>::type!=std::string{root}+"_Request"||
               traits<response>::type!=std::string{root}+"_Response")return false;
            return actual->request_members_&&actual->response_members_&&
                actual->request_members_==message_members_for<request>()&&
                actual->response_members_==message_members_for<response>();
        }
    }
    template<class Native> bool append_service(protocol_graph& graph,std::string_view root)
    {
        using request=typename Native::Request;using response=typename Native::Response;
        if constexpr(!traits<request>::enabled||!traits<response>::enabled)return false;
        else return service_members_match<Native>(service_metadata<Native>(),root)&&
            graph.add(root,{{"request_message",traits<request>::type},{"response_message",traits<response>::type}})&&
            graph.message<request>()&&graph.message<response>();
    }
#else
    template<class Native> bool append_service(protocol_graph&,std::string_view){return false;}
#endif

#if UWVM_ROS2_INSTALLED_ACTION_SUPPORT
    /** Compare the actual five-link root, not a copied success flag. */
    template<class Native,class Support> bool action_links(Support const* support)
    {
        using impl=typename Native::Impl;
        return support&&
            support->goal_service_type_support==::rosidl_typesupport_cpp::get_service_type_support_handle<typename impl::SendGoalService>()&&
            support->result_service_type_support==::rosidl_typesupport_cpp::get_service_type_support_handle<typename impl::GetResultService>()&&
            support->cancel_service_type_support==::rosidl_typesupport_cpp::get_service_type_support_handle<typename impl::CancelGoalService>()&&
            support->feedback_message_type_support==::rosidl_typesupport_cpp::get_message_type_support_handle<typename impl::FeedbackMessage>()&&
            support->status_message_type_support==::rosidl_typesupport_cpp::get_message_type_support_handle<typename impl::GoalStatusMessage>();
    }
    template<class Native> bool append_action(protocol_graph& graph,std::string_view root)
    {
        using impl=typename Native::Impl;using send=typename impl::SendGoalService;using result=typename impl::GetResultService;
        using cancel=typename impl::CancelGoalService;using feedback=typename impl::FeedbackMessage;using status=typename impl::GoalStatusMessage;
        if constexpr(!interface_traits<send>::enabled||!interface_traits<result>::enabled||
                     !interface_traits<cancel>::enabled||!traits<typename Native::Goal>::enabled||
                     !traits<typename Native::Result>::enabled||!traits<typename Native::Feedback>::enabled||
                     !traits<feedback>::enabled||!traits<status>::enabled)return false;
        else
        {
            if(!action_links<Native>(::rosidl_typesupport_cpp::get_action_type_support_handle<Native>())||
               traits<typename Native::Goal>::type!=std::string{root}+"_Goal"||
               traits<typename Native::Result>::type!=std::string{root}+"_Result"||
               traits<typename Native::Feedback>::type!=std::string{root}+"_Feedback"||
               traits<feedback>::type!=std::string{root}+"_FeedbackMessage"||
               interface_traits<send>::type!=std::string{root}+"_SendGoal"||
               interface_traits<result>::type!=std::string{root}+"_GetResult"||
               interface_traits<cancel>::type!="action_msgs/srv/CancelGoal"||
               traits<status>::type!="action_msgs/msg/GoalStatusArray")return false;
            // Cancel/status are standard protocol associations, not fields of
            // this Action's derived type description. Prove their full actual
            // source/metadata closure independently before accepting any link.
            protocol_graph cancellation;
            if(!append_service<cancel>(cancellation,interface_traits<cancel>::type)||
               !same_schema(cancellation.view(),interface_traits<cancel>::schema()))return false;
            protocol_graph statuses;if(!statuses.message<status>())return false;
            return graph.add(root,{{"goal",traits<typename Native::Goal>::type},
                {"result",traits<typename Native::Result>::type},{"feedback",traits<typename Native::Feedback>::type},
                {"send_goal_service",interface_traits<send>::type},{"get_result_service",interface_traits<result>::type},
                {"feedback_message",traits<feedback>::type}})&&
                graph.message<typename Native::Goal>()&&graph.message<typename Native::Result>()&&
                graph.message<typename Native::Feedback>()&&
                append_service<send>(graph,interface_traits<send>::type)&&
                append_service<result>(graph,interface_traits<result>::type)&&graph.message<feedback>();
        }
    }
#else
    template<class Native> bool append_action(protocol_graph&,std::string_view){return false;}
#endif

    template<class Native> bool protocol_proof()
    {
        if constexpr(!interface_traits<Native>::enabled)return false;
        else
        {
            using support=std::remove_pointer_t<decltype(interface_traits<Native>::support())>;
            if constexpr(has_baked_hash<support>::value)return false;
            else
            {
                protocol_graph graph;bool valid{};
                if constexpr(interface_traits<Native>::kind==1)valid=append_service<Native>(graph,interface_traits<Native>::type);
                else if constexpr(interface_traits<Native>::kind==2)valid=append_action<Native>(graph,interface_traits<Native>::type);
                return valid&&same_schema(graph.view(),interface_traits<Native>::schema());
            }
        }
    }
    template<class Native,class Support> std::string_view protocol_identity(Support const* support)
    {
        if constexpr(has_baked_hash<Support>::value||!interface_traits<Native>::enabled)return {};
        else
        {
            if(!support||support!=interface_traits<Native>::support())return {};
            // Immutable selected SDK/build inputs are qualified once, before
            // entities. Invalid caller DATA cannot poison this cached proof.
            static std::string const value=[] {
                if(!digest(interface_traits<Native>::source_proof_sha256)||!protocol_proof<Native>())return std::string{};
                return rihs_identity(interface_traits<Native>::schema(),interface_traits<Native>::type);
            }();
            return value;
        }
    }
    template<class Native,class Support> bool qualify_protocol(Support const* support,std::string_view root,std::string_view expected)
    {
        if constexpr(has_baked_hash<Support>::value||!interface_traits<Native>::enabled)return false;
        else return root==interface_traits<Native>::type&&!expected.empty()&&protocol_identity<Native>(support)==expected;
    }
}
