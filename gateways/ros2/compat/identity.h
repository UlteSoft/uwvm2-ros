#pragma once
#include "features.h"
#include "rihs.h"
#include "../../common/native_io.h"
#include <uwvm2/imported/ros/common/types.h>
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <array>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#if __has_include(<rosidl_typesupport_introspection_cpp/message_introspection.hpp>)
#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/field_types.hpp>
#define UWVM_ROS2_HAVE_INTROSPECTION_CPP 1
#else
#define UWVM_ROS2_HAVE_INTROSPECTION_CPP 0
#endif

namespace uwvm2::ros_gateway::ros2_compat
{
    namespace identity_ipc = ::uwvm2::imported::ros::ipc;
    /* RIHS01 and TypeDescription entered ROS in Iron. Access is dependent so
     * older official type-support structs compile without imaginary members.
     * An absent native hash stays absent; never return a catalog hash here. */
    template<class Support,class = void> struct has_baked_hash : std::false_type {};
    template<class Support> struct has_baked_hash<Support,std::void_t<decltype(std::declval<Support>().get_type_hash_func)>> : std::true_type {};
    template<class Support> std::string baked_hash(Support const* support)
    {
        if constexpr(has_baked_hash<Support>::value)
        {
            if(!support||!support->get_type_hash_func) { return {}; }
            auto hash{support->get_type_hash_func(support)};
            if(!hash||hash->version!=1) { return {}; }
            return native_io::hexadecimal(hash->value,sizeof hash->value,"RIHS01_");
        }
        else { (void)support;return {}; }
    }

#if UWVM_ROS2_HAVE_INTROSPECTION_CPP
    using message_members = ::rosidl_typesupport_introspection_cpp::MessageMembers;
    template<class Native> message_members const* message_metadata()
    {
        auto root{::rosidl_typesupport_cpp::get_message_type_support_handle<Native>()};
        if(!root||!root->typesupport_identifier) { return nullptr; }
        constexpr char selected[]{"rosidl_typesupport_introspection_cpp"};
        auto actual{std::strcmp(root->typesupport_identifier,selected)==0?root:
            (root->func?root->func(root,selected):nullptr)};
        if(!actual||!actual->typesupport_identifier||std::strcmp(actual->typesupport_identifier,selected)!=0||!actual->data) { return nullptr; }
        return static_cast<message_members const*>(actual->data);
    }
    // Compatibility spelling offered to custom providers; no dynamic ROS name
    // or Guest-selected library is passed to the typesupport dispatcher.
    template<class Native> message_members const* message_members_for() { return message_metadata<Native>(); }
    inline bool native_string(char const* value,std::string_view expected)
    {
        if(!value||expected.size()>255) { return false; }
        for(std::size_t i{};i!=expected.size();++i) { if(!value[i]||value[i]!=expected[i]) { return false; } }
        return value[expected.size()]=='\0';
    }
    template<class Members> auto metadata_namespace(Members const& value,int)
        -> decltype(value.message_namespace_) { return value.message_namespace_; }
    template<class Members> auto metadata_namespace(Members const& value,long)
        -> decltype(value.package_name_) { return value.package_name_; }
    inline bool metadata_name(message_members const& value,std::string_view expected)
    {
        auto space{metadata_namespace(value,0)};if(!space||!value.message_name_) { return false; }
        std::string name{space};
        for(std::size_t at{};(at=name.find("::",at))!=std::string::npos;) { name.replace(at,2,"/");++at; }
        name+='/';name+=value.message_name_;
        if(name==expected) { return true; }
        // Pre-IDL generators used package/Name. Only remove the explicitly
        // selected standard category, never accept an arbitrary suffix match.
        auto slash{expected.find('/')};if(slash==std::string_view::npos) { return false; }
        for(auto category:{std::string_view{"/msg/"},std::string_view{"/srv/"},std::string_view{"/action/"}})
        { if(expected.substr(slash,category.size())==category&&name==std::string{expected.substr(0,slash)}+"/"+std::string{expected.substr(slash+category.size())}) { return true; } }
        return false;
    }
    inline unsigned scalar_type(std::string_view value)
    {
        namespace i=::rosidl_typesupport_introspection_cpp;
        if(value=="bool") { return i::ROS_TYPE_BOOLEAN; }
        if(value=="int8") { return i::ROS_TYPE_INT8; }if(value=="uint8") { return i::ROS_TYPE_UINT8; }
        if(value=="byte") { return i::ROS_TYPE_OCTET; }if(value=="char") { return i::ROS_TYPE_CHAR; }
        if(value=="int16") { return i::ROS_TYPE_INT16; }if(value=="uint16") { return i::ROS_TYPE_UINT16; }
        if(value=="int32") { return i::ROS_TYPE_INT32; }if(value=="uint32") { return i::ROS_TYPE_UINT32; }
        if(value=="int64") { return i::ROS_TYPE_INT64; }if(value=="uint64") { return i::ROS_TYPE_UINT64; }
        if(value=="float32") { return i::ROS_TYPE_FLOAT; }if(value=="float64") { return i::ROS_TYPE_DOUBLE; }
        if(value=="string") { return i::ROS_TYPE_STRING; }return 0;
    }
    inline bool matches_schema_record(message_members const* actual,identity_ipc::custom_native_schema schema,
        std::size_t selected,std::array<unsigned char,256>& colors,unsigned depth)
    {
        if(!actual||depth>16||selected>=schema.record_count||colors[selected]==1) { return false; }
        auto const& expected{schema.records[selected]};
        if(!metadata_name(*actual,expected.type)||actual->member_count_>4096||!actual->size_of_) { return false; }
        bool canonical_empty{expected.field_count==1&&expected.fields&&
            expected.fields[0].name=="structure_needs_at_least_one_member"&&expected.fields[0].type=="uint8"&&
            !expected.fields[0].array_size&&!expected.fields[0].is_array&&!expected.fields[0].is_sequence&&
            !expected.fields[0].sequence_bound&&!expected.fields[0].string_bound};
        if(expected.field_count==0||canonical_empty)
        {
            if(actual->member_count_==0) { colors[selected]=2;return true; }
            // Official rosidl C/C++ empty records carry one representation-only
            // uint8 member. It is not an interface field or a serialized byte.
            if(actual->member_count_!=1||!actual->members_) { return false; }
            auto const& dummy{actual->members_[0]};
            bool matched{native_string(dummy.name_,"structure_needs_at_least_one_member")&&dummy.offset_==0&&dummy.type_id_==scalar_type("uint8")&&
                !dummy.is_array_&&!dummy.is_upper_bound_&&!dummy.array_size_&&!dummy.string_upper_bound_&&!dummy.members_&&!dummy.default_value_};
            if(matched) { colors[selected]=2; }return matched;
        }
        if(actual->member_count_!=expected.field_count||!actual->members_||!expected.fields) { return false; }
        colors[selected]=1;
        for(std::size_t index{};index!=expected.field_count;++index)
        {
            auto const& a{actual->members_[index]};auto const& e{expected.fields[index]};
            if(!native_string(a.name_,e.name)||a.default_value_||a.offset_>=actual->size_of_||
                a.string_upper_bound_!=e.string_bound||a.is_array_!=(e.is_array||e.is_sequence)) { return false; }
            if(e.is_sequence)
            { if(a.array_size_!=e.sequence_bound||a.is_upper_bound_!=(e.sequence_bound!=0)) { return false; } }
            else if(a.array_size_!=(e.is_array?e.array_size:0)||a.is_upper_bound_) { return false; }
            auto type{scalar_type(e.type)};
            if(type)
            { if(a.type_id_!=type||a.members_) { return false; } }
            else
            {
                if(a.type_id_!=::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE||!a.members_||!a.members_->data) { return false; }
                std::size_t nested{};while(nested<schema.record_count&&schema.records[nested].type!=e.type) { ++nested; }
                if(nested==schema.record_count||!matches_schema_record(static_cast<message_members const*>(a.members_->data),schema,nested,colors,depth+1)) { return false; }
            }
        }
        colors[selected]=2;return true;
    }
    inline bool matches_schema(message_members const* actual,identity_ipc::custom_native_schema schema,std::string_view root)
    {
        if(!schema.records||!schema.record_count||schema.record_count>256) { return false; }
        // Official introspection generators may emit default_value_=nullptr
        // even for declared defaults. Null is UNKNOWN, not proof of absence
        // or a scalar value. A declared-default graph requires the separate
        // actual full TypeDescription path; no legacy RIHS fallback is valid.
        for(std::size_t i{};i!=schema.record_count;++i)
        {
            auto const& record{schema.records[i]};
            if(record.field_count>4096||(record.field_count&&!record.fields)) { return false; }
            for(std::size_t j{};j!=record.field_count;++j)
            { if(!record.fields[j].native_default.empty()) { return false; } }
        }
        std::size_t selected{};while(selected<schema.record_count&&schema.records[selected].type!=root) { ++selected; }
        std::array<unsigned char,256> colors{};
        if(selected==schema.record_count||!matches_schema_record(actual,schema,selected,colors,1)) { return false; }
        // Reject unrelated records as well as cycles and duplicated type rows.
        for(std::size_t i{};i!=schema.record_count;++i)
        {
            if(!colors[i]) { return false; }
            for(std::size_t j{};j!=i;++j) { if(schema.records[i].type==schema.records[j].type) { return false; } }
        }
        return true;
    }
#else
    struct message_members;
    inline unsigned scalar_type(std::string_view value)
    { for(auto name:{"bool","int8","uint8","byte","char","int16","uint16","int32","uint32","int64","uint64","float32","float64","string"}) { if(value==name) { return 1; } }return 0; }
    template<class Native> message_members const* message_metadata() { return nullptr; }
    template<class Native> message_members const* message_members_for() { return nullptr; }
    inline bool matches_schema(message_members const*,identity_ipc::custom_native_schema,std::string_view) { return false; }
#endif
    /* This catalog is a complete declaration-level description of official
     * builtin interfaces, not a hand-written native RIHS implementation. On
     * pre-Iron SDKs we compare all official introspection fields and nested
     * records to it, then validate the existing trusted ABI catalog selector.
     * The attestation is LEGACY_OFFICIAL_SCHEMA, never NATIVE_RIHS01. */
    namespace builtin
    {
        using field=identity_ipc::custom_native_field;using record=identity_ipc::custom_native_record;
        inline constexpr field string_fields[]{{"data","string"}};
        inline constexpr field bool_fields[]{{"data","bool"}};
        inline constexpr field time_fields[]{{"sec","int32"},{"nanosec","uint32"}};
        inline constexpr field header_fields[]{{"stamp","builtin_interfaces/msg/Time"},{"frame_id","string"}};
        inline constexpr field vector_fields[]{{"x","float64"},{"y","float64"},{"z","float64"}};
        inline constexpr field quaternion_fields[]{{"x","float64"},{"y","float64"},{"z","float64"},{"w","float64"}};
        inline constexpr field twist_fields[]{{"linear","geometry_msgs/msg/Vector3"},{"angular","geometry_msgs/msg/Vector3"}};
        inline constexpr field transform_fields[]{{"translation","geometry_msgs/msg/Vector3"},{"rotation","geometry_msgs/msg/Quaternion"}};
        inline constexpr field stamped_fields[]{{"header","std_msgs/msg/Header"},{"child_frame_id","string"},{"transform","geometry_msgs/msg/Transform"}};
        inline constexpr field image_fields[]{{"header","std_msgs/msg/Header"},{"height","uint32"},{"width","uint32"},{"encoding","string"},{"is_bigendian","uint8"},{"step","uint32"},{"data","uint8",0,false,0,true,0}};
        inline constexpr field response_fields[]{{"success","bool"},{"message","string"}};
        inline constexpr field empty_fields[]{{"structure_needs_at_least_one_member","uint8"}};
        inline constexpr field request_bool[]{{"data","bool"}};
        inline constexpr field state_fields[]{{"id","uint8"},{"label","string"}};
        inline constexpr field get_state_response[]{{"current_state","lifecycle_msgs/msg/State"}};
        inline constexpr field transition_fields[]{{"id","uint8"},{"label","string"}};
        inline constexpr field change_state_request[]{{"transition","lifecycle_msgs/msg/Transition"}};
        inline constexpr field success_fields[]{{"success","bool"}};
        inline constexpr field goal_fields[]{{"order","int32"}};
        inline constexpr field result_fields[]{{"sequence","int32",0,false,0,true,0}};
        inline constexpr field feedback_fields[]{{"partial_sequence","int32",0,false,0,true,0}};
        inline constexpr field parameter_value_fields[]{{"type","uint8"},{"bool_value","bool"},{"integer_value","int64"},{"double_value","float64"},{"string_value","string"},
            {"byte_array_value","byte",0,false,0,true,0},{"bool_array_value","bool",0,false,0,true,0},{"integer_array_value","int64",0,false,0,true,0},{"double_array_value","float64",0,false,0,true,0},{"string_array_value","string",0,false,0,true,0}};
        inline constexpr field parameter_fields[]{{"name","string"},{"value","rcl_interfaces/msg/ParameterValue"}};
        inline constexpr field parameter_event_fields[]{{"stamp","builtin_interfaces/msg/Time"},{"node","string"},{"new_parameters","rcl_interfaces/msg/Parameter",0,false,0,true,0},{"changed_parameters","rcl_interfaces/msg/Parameter",0,false,0,true,0},{"deleted_parameters","rcl_interfaces/msg/Parameter",0,false,0,true,0}};
        inline constexpr field service_info_fields[]{{"event_type","uint8"},{"stamp","builtin_interfaces/msg/Time"},{"client_gid","uint8",16,true},{"sequence_number","int64"}};
        inline constexpr field uuid_fields[]{{"uuid","uint8",16,true}};
        // Official rosidl_parser.definition.Service synthesizes exactly these
        // three root references and bounded-singleton event request/response.
        // They are canonical hash declarations, not new legacy ROS endpoints.
#define UWVM_ROS2_SERVICE_SCHEMA(Symbol,Name) \
        inline constexpr field Symbol##_root[]{{"request_message",Name "_Request"},{"response_message",Name "_Response"},{"event_message",Name "_Event"}}; \
        inline constexpr field Symbol##_event[]{{"info","service_msgs/msg/ServiceEventInfo"},{"request",Name "_Request",0,false,0,true,1},{"response",Name "_Response",0,false,0,true,1}};
        UWVM_ROS2_SERVICE_SCHEMA(trigger,"std_srvs/srv/Trigger")
        UWVM_ROS2_SERVICE_SCHEMA(set_bool,"std_srvs/srv/SetBool")
        UWVM_ROS2_SERVICE_SCHEMA(get_state,"lifecycle_msgs/srv/GetState")
        UWVM_ROS2_SERVICE_SCHEMA(change_state,"lifecycle_msgs/srv/ChangeState")
        UWVM_ROS2_SERVICE_SCHEMA(send_goal,"action_tutorials_interfaces/action/Fibonacci_SendGoal")
        UWVM_ROS2_SERVICE_SCHEMA(get_result,"action_tutorials_interfaces/action/Fibonacci_GetResult")
#undef UWVM_ROS2_SERVICE_SCHEMA
        inline constexpr field send_goal_request[]{{"goal_id","unique_identifier_msgs/msg/UUID"},{"goal","action_tutorials_interfaces/action/Fibonacci_Goal"}};
        inline constexpr field send_goal_response[]{{"accepted","bool"},{"stamp","builtin_interfaces/msg/Time"}};
        inline constexpr field get_result_request[]{{"goal_id","unique_identifier_msgs/msg/UUID"}};
        inline constexpr field get_result_response[]{{"status","int8"},{"result","action_tutorials_interfaces/action/Fibonacci_Result"}};
        inline constexpr field feedback_message[]{{"goal_id","unique_identifier_msgs/msg/UUID"},{"feedback","action_tutorials_interfaces/action/Fibonacci_Feedback"}};
        inline constexpr field action_root[]{{"goal","action_tutorials_interfaces/action/Fibonacci_Goal"},{"result","action_tutorials_interfaces/action/Fibonacci_Result"},{"feedback","action_tutorials_interfaces/action/Fibonacci_Feedback"},
            {"send_goal_service","action_tutorials_interfaces/action/Fibonacci_SendGoal"},{"get_result_service","action_tutorials_interfaces/action/Fibonacci_GetResult"},{"feedback_message","action_tutorials_interfaces/action/Fibonacci_FeedbackMessage"}};
        inline constexpr record records[]{
            {"std_msgs/msg/String",string_fields,1},{"std_msgs/msg/Bool",bool_fields,1},
            {"builtin_interfaces/msg/Time",time_fields,2},{"std_msgs/msg/Header",header_fields,2},
            {"geometry_msgs/msg/Vector3",vector_fields,3},{"geometry_msgs/msg/Quaternion",quaternion_fields,4},
            {"geometry_msgs/msg/Twist",twist_fields,2},{"geometry_msgs/msg/Transform",transform_fields,2},
            {"geometry_msgs/msg/TransformStamped",stamped_fields,3},{"sensor_msgs/msg/Image",image_fields,7},
            {"std_srvs/srv/Trigger_Request",empty_fields,1},{"std_srvs/srv/Trigger_Response",response_fields,2},
            {"std_srvs/srv/SetBool_Request",request_bool,1},{"std_srvs/srv/SetBool_Response",response_fields,2},
            {"lifecycle_msgs/msg/State",state_fields,2},{"lifecycle_msgs/msg/Transition",transition_fields,2},
            {"lifecycle_msgs/srv/GetState_Request",empty_fields,1},{"lifecycle_msgs/srv/GetState_Response",get_state_response,1},
            {"lifecycle_msgs/srv/ChangeState_Request",change_state_request,1},{"lifecycle_msgs/srv/ChangeState_Response",success_fields,1},
            {"action_tutorials_interfaces/action/Fibonacci_Goal",goal_fields,1},{"action_tutorials_interfaces/action/Fibonacci_Result",result_fields,1},{"action_tutorials_interfaces/action/Fibonacci_Feedback",feedback_fields,1},
            {"rcl_interfaces/msg/ParameterValue",parameter_value_fields,10},{"rcl_interfaces/msg/Parameter",parameter_fields,2},{"rcl_interfaces/msg/ParameterEvent",parameter_event_fields,5},
            {"service_msgs/msg/ServiceEventInfo",service_info_fields,4},{"unique_identifier_msgs/msg/UUID",uuid_fields,1},
#define UWVM_ROS2_SERVICE_RECORD(Symbol,Name) {Name,Symbol##_root,3},{Name "_Event",Symbol##_event,3},
            UWVM_ROS2_SERVICE_RECORD(trigger,"std_srvs/srv/Trigger")
            UWVM_ROS2_SERVICE_RECORD(set_bool,"std_srvs/srv/SetBool")
            UWVM_ROS2_SERVICE_RECORD(get_state,"lifecycle_msgs/srv/GetState")
            UWVM_ROS2_SERVICE_RECORD(change_state,"lifecycle_msgs/srv/ChangeState")
            UWVM_ROS2_SERVICE_RECORD(send_goal,"action_tutorials_interfaces/action/Fibonacci_SendGoal")
            UWVM_ROS2_SERVICE_RECORD(get_result,"action_tutorials_interfaces/action/Fibonacci_GetResult")
#undef UWVM_ROS2_SERVICE_RECORD
            {"action_tutorials_interfaces/action/Fibonacci_SendGoal_Request",send_goal_request,2},{"action_tutorials_interfaces/action/Fibonacci_SendGoal_Response",send_goal_response,2},
            {"action_tutorials_interfaces/action/Fibonacci_GetResult_Request",get_result_request,1},{"action_tutorials_interfaces/action/Fibonacci_GetResult_Response",get_result_response,2},
            {"action_tutorials_interfaces/action/Fibonacci_FeedbackMessage",feedback_message,2},{"action_tutorials_interfaces/action/Fibonacci",action_root,6}};
        // Only the reachable part of this fixed catalog belongs to each check.
        // Custom providers instead pass their exact generated closed schema.
        inline identity_ipc::custom_native_schema schema(std::string_view root,std::array<record,32>& selected)
        {
            std::size_t count{};
            auto append=[&](std::string_view name) {
                for(std::size_t i{};i!=count;++i) { if(selected[i].type==name) { return true; } }
                for(auto const& item:records) { if(item.type==name&&count<selected.size()) { selected[count++]=item;return true; } }
                return false;
            };
            if(!append(root)) { return {}; }
            for(std::size_t i{};i!=count;++i)
            { for(std::size_t j{};j!=selected[i].field_count;++j) { auto type{selected[i].fields[j].type};if(!scalar_type(type)&&!append(type)) { return {}; } } }
            return {selected.data(),count};
        }
    }
    template<class Native> bool builtin_record(std::string_view type)
    {
        std::array<builtin::record,32> selected{};auto schema{builtin::schema(type,selected)};
        return matches_schema(message_metadata<Native>(),schema,type);
    }
    inline bool builtin_rihs(std::string_view type,std::string_view expected)
    { std::array<builtin::record,32> selected{};auto schema{builtin::schema(type,selected)};return matches_rihs(schema,type,expected); }
    inline std::string_view builtin_catalog_identity(std::string_view type)
    {
        // Same immutable builtin selector as common/message.h, kept here to
        // avoid importing the C++26 Worker codec into the C++17 native SDK.
        if(type=="std_msgs/msg/String") { return "RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18"; }
        if(type=="std_msgs/msg/Bool") { return "RIHS01_feb91e995ff9ebd09c0cb3d2aed18b11077585839fb5db80193b62d74528f6c9"; }
        if(type=="geometry_msgs/msg/TransformStamped") { return "RIHS01_0a241f87d04668d94099cbb5ba11691d5ad32c2f29682e4eb5653424bd275206"; }
        if(type=="geometry_msgs/msg/Twist") { return "RIHS01_9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a"; }
        if(type=="sensor_msgs/msg/Image") { return "RIHS01_d31d41a9a4c4bc8eae9be757b0beed306564f7526c88ea6a4588fb9582527d47"; }
        if(type=="std_srvs/srv/Trigger") { return "RIHS01_eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f"; }
        if(type=="std_srvs/srv/SetBool") { return "RIHS01_abe9e4bb6b41b40e6789712c00ec8871923e089af3f667a79992a428cff2da0a"; }
        if(type=="lifecycle_msgs/srv/GetState") { return "RIHS01_800a0a5aae599782b02932de0caf563f6dc4e7e94b794eadde075ba2cbef9795"; }
        if(type=="lifecycle_msgs/srv/ChangeState") { return "RIHS01_356fe34f0475a43acf54542013af4167b0e729f77ea22ffb045c6ad8e20668e5"; }
        if(type=="rcl_interfaces/msg/ParameterEvent") { return "RIHS01_043e627780fcad87a22d225bc2a037361dba713fca6a6b9f4b869a5aa0393204"; }
        if(type=="action_tutorials_interfaces/action/Fibonacci") { return "RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0"; }
        return {};
    }
    inline bool catalog_identity(std::string_view type,std::string_view expected)
    { auto trusted{builtin_catalog_identity(type)};return !trusted.empty()&&trusted==expected; }
    template<class Native> bool builtin_message(std::string_view type,std::string_view expected)
    {
        if(!catalog_identity(type,expected)) { return false; }
        auto support{::rosidl_typesupport_cpp::get_message_type_support_handle<Native>()};
        if constexpr(has_baked_hash<std::remove_cv_t<std::remove_reference_t<decltype(*support)>>>::value)
        { auto hash{baked_hash(support)};return !hash.empty()&&hash==expected; }
        else { return builtin_record<Native>(type)&&builtin_rihs(type,expected); }
    }
    template<class Service> bool builtin_service(std::string_view type,std::string_view expected)
    {
        if(!catalog_identity(type,expected)) { return false; }
        auto support{::rosidl_typesupport_cpp::get_service_type_support_handle<Service>()};
        if constexpr(has_baked_hash<std::remove_cv_t<std::remove_reference_t<decltype(*support)>>>::value)
        { auto hash{baked_hash(support)};return !hash.empty()&&hash==expected; }
        else { return builtin_record<typename Service::Request>(std::string{type}+"_Request")&&builtin_record<typename Service::Response>(std::string{type}+"_Response")&&builtin_rihs(type,expected); }
    }
    template<class Action> bool builtin_action(std::string_view type,std::string_view expected)
    {
        // Action root support is checked by the caller when its installed
        // header exposes the hash. Legacy Action definitions are identified by
        // all three official component schemas, never by their names alone.
        return catalog_identity(type,expected)&&builtin_record<typename Action::Goal>(std::string{type}+"_Goal")&&
            builtin_record<typename Action::Result>(std::string{type}+"_Result")&&builtin_record<typename Action::Feedback>(std::string{type}+"_Feedback")&&builtin_rihs(type,expected);
    }
}
