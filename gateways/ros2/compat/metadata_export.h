/** @file
 * @brief Native-only metadata export for pre-RIHS ROS2 Message/Service/Action SDKs.
 *
 * This tool-side helper runs before any ROS Node/entity exists. The trusted
 * source preparer supplies the COMPLETE declaration graph from admitted .msg
 * definitions, including every transitive dependency and the official native
 * string/sequence capacities. It is never a canonical payload layout or a
 * Guest-selected graph. The installed type-support introspection is compared
 * independently, field by field, before deriving a RIHS01 value.
 *
 * The resulting identity is explicitly derived from actual native introspection;
 * it is not described as a hash baked into an old SDK. Hash-capable support is
 * refused here even if its pointer/hash is null or malformed: use its original
 * native type-hash/TypeDescription exporter instead. Service export additionally verifies actual ServiceMembers associations and
 * labels standard protocol/Event derivation separately from installed metadata.
 * Action roots require actual five-link typed support and complete native protocol proof below.
 *
 * Compile as C++17 against the actual selected ROS2 SDK and OpenSSL. The normal
 * whole-SDK, full-MD/link/ELF, stopped-process mapping, input/provenance and cgroup
 * owner checks still apply to the executable and its stdout receipt. This header
 * neither installs those checks nor treats JSON as proof of process ownership.
 * It grants no capability and adds no Runtime/Wasm import.
 */
#pragma once
#include "identity.h"
#if __has_include(<rosidl_typesupport_introspection_cpp/service_introspection.hpp>)
#include <rosidl_typesupport_introspection_cpp/service_introspection.hpp>
#include <builtin_interfaces/msg/time.hpp>
#define UWVM_ROS2_HAVE_SERVICE_INTROSPECTION_CPP 1
#else
#define UWVM_ROS2_HAVE_SERVICE_INTROSPECTION_CPP 0
#endif
#if __has_include(<rosidl_typesupport_cpp/action_type_support.hpp>) && __has_include(<unique_identifier_msgs/msg/uuid.hpp>) && __has_include(<action_msgs/srv/cancel_goal.hpp>) && __has_include(<action_msgs/msg/goal_status_array.hpp>)
#include <rosidl_typesupport_cpp/action_type_support.hpp>
#include <unique_identifier_msgs/msg/uuid.hpp>
#include <action_msgs/srv/cancel_goal.hpp>
#include <action_msgs/msg/goal_status_array.hpp>
#define UWVM_ROS2_HAVE_LEGACY_ACTION_TYPES 1
#else
#define UWVM_ROS2_HAVE_LEGACY_ACTION_TYPES 0
#endif
#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace uwvm2::ros_gateway::ros2_compat::legacy_metadata_export
{
    namespace ipc = ::uwvm2::imported::ros::ipc;

    /** Append the existing exporter's flattened, complete field description.
     * All names and declaration shapes were already matched to real native
     * metadata. rihs_json supplies a separate bounded ASCII name/number writer;
     * it never serializes native pointers, offsets or borrowed string capacity.
     */
    inline void record(rihs_json& json,ipc::custom_native_record const& value)
    {
        json.add("{\"type\": ");json.name(value.type);json.add(", \"fields\": [");
        for(std::size_t index{};index!=value.field_count;++index)
        {
            auto const& field{value.fields[index]};if(index) { json.add(", "); }
            unsigned kind{rihs_scalar(field.type)};bool const nested{kind==0};
            if(nested) { kind=1; }
            if(field.type=="string"&&field.string_bound) { kind=21; }
            if(field.is_array) { kind+=48; }
            else if(field.is_sequence) { kind+=field.sequence_bound?96:144; }
            json.add("{\"name\": ");json.name(field.name);
            json.add(", \"type_id\": ");json.number(kind);
            json.add(", \"capacity\": ");json.number(field.is_array?field.array_size:(field.is_sequence?field.sequence_bound:0));
            json.add(", \"string_capacity\": ");json.number(field.string_bound);
            json.add(", \"nested_type_name\": ");json.name(nested?field.type:std::string_view{});
            // The current complete native introspection comparison rejects all
            // default_value pointers; nonempty defaults are not projected away.
            json.add(", \"default_value\": \"\"}");
        }
        json.add("]}");
    }

    /** Return one exporter-compatible Message metadata object, or throw before
     * exposing an identity. Native must be the actual statically selected
     * generated ROS2 Message type. schema contains only its complete reachable
     * declaration graph; duplicate, cyclic, disconnected or mismatched records
     * are refused by independent native comparison and RIHS traversal.
     *
     * Put this exact object under the original exporter's `official` and
     * `components.message` roles. The outer exporter inventory and bounded I
     * gate remain the owner's responsibility. No root Message name is inferred
     * from a package/filename or rewritten to impersonate another ROS major.
     */
    template<class Native>
    std::string message(ipc::custom_native_schema schema,std::string_view root)
    {
        auto support{::rosidl_typesupport_cpp::get_message_type_support_handle<Native>()};
        if(!support) { throw std::runtime_error("native message type support unavailable"); }
        if constexpr(has_baked_hash<std::remove_pointer_t<decltype(support)>>::value)
        {
            // A modern provider must never fall back when its actual hash or
            // description is absent, invalid, unsupported or does not match.
            throw std::runtime_error("hash-capable native support requires its original exporter");
        }
        else
        {
            if(!matches_schema(message_members_for<Native>(),schema,root))
            { throw std::runtime_error("complete actual native introspection does not match"); }
            auto identity{rihs_identity(schema,root)};
            if(identity.empty()) { throw std::runtime_error("derived native RIHS unavailable"); }
            std::size_t selected{schema.record_count},count{};
            std::array<std::size_t,256> references{};
            // matches_schema and rihs_identity have already established the
            // finite <=256 complete graph and one unambiguous selected root.
            for(std::size_t index{};index!=schema.record_count;++index)
            { if(schema.records[index].type==root) { selected=index; }else { references[count++]=index; } }
            if(selected==schema.record_count) { throw std::runtime_error("native root missing"); }
            std::sort(references.begin(),references.begin()+count,[&](auto a,auto b) {
                return schema.records[a].type<schema.records[b].type;
            });
            rihs_json json;
            json.add("{\"native_identity\": \"");json.add(identity);
            json.add("\", \"identity_provenance\": \"LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01\", \"native_baked_hash_available\": false, \"root\": ");
            record(json,schema.records[selected]);json.add(", \"references\": [");
            for(std::size_t index{};index!=count;++index)
            { if(index) { json.add(", "); }record(json,schema.records[references[index]]); }
            json.add("]}");
            if(!json.valid()) { throw std::length_error("bounded native metadata export exceeded"); }
            return std::move(json).release();
        }
    }
    /** Build the exact reachable subgraph, without reinterpreting capacities.
     * This is a view into trusted compiled declarations, not native proof. The
     * independent matcher and whole-root RIHS traversal still check it later. */
    inline ipc::custom_native_schema reachable(ipc::custom_native_schema schema,
        std::string_view root,std::array<ipc::custom_native_record,256>& out)
    {
        if(!schema.records||!schema.record_count||schema.record_count>out.size())
        { throw std::runtime_error("finite complete native graph required"); }
        for(std::size_t i{};i!=schema.record_count;++i)
        {
            if(schema.records[i].field_count>4096||(!schema.records[i].fields&&schema.records[i].field_count))
            { throw std::runtime_error("finite native declaration fields required"); }
            for(std::size_t j{};j!=i;++j)
            { if(schema.records[i].type==schema.records[j].type) { throw std::runtime_error("duplicate declaration"); } }
        }
        std::size_t count{};
        auto append=[&](std::string_view name) {
            for(std::size_t i{};i!=count;++i) { if(out[i].type==name) { return; } }
            for(std::size_t i{};i!=schema.record_count;++i)
            { if(schema.records[i].type==name) { out[count++]=schema.records[i];return; } }
            throw std::runtime_error("native dependency missing");
        };
        append(root);
        for(std::size_t i{};i!=count;++i)
        { for(std::size_t j{};j!=out[i].field_count;++j)
          { if(!rihs_scalar(out[i].fields[j].type)) { append(out[i].fields[j].type); } } }
        return {out.data(),count};
    }

    /** Optional trailing native-default annotations remain forbidden in every
     * legacy declaration. The dependent trait keeps the original seven-field
     * schema ABI usable; no modern default is inferred from a null pointer. */
    template<class Field,class=void> struct has_declared_native_default : std::false_type {};
    template<class Field> struct has_declared_native_default<Field,
        std::void_t<decltype(std::declval<Field>().native_default)>> : std::true_type {};
    template<class Field> bool legacy_default_empty(Field const& value)
    { if constexpr(has_declared_native_default<Field>::value) {return value.native_default.empty();}else {(void)value;return true;} }

    inline bool same_field(ipc::custom_native_field const& a,ipc::custom_native_field const& b)
    { return a.name==b.name&&a.type==b.type&&a.array_size==b.array_size&&a.is_array==b.is_array&&
        a.string_bound==b.string_bound&&a.is_sequence==b.is_sequence&&a.sequence_bound==b.sequence_bound&&legacy_default_empty(a)&&legacy_default_empty(b); }

    template<std::size_t N>
    void require_record(ipc::custom_native_schema schema,std::string_view name,
        std::array<ipc::custom_native_field,N> const& fields)
    {
        for(std::size_t i{};i!=schema.record_count;++i)
        {
            auto const& value{schema.records[i]};if(value.type!=name) { continue; }
            if(value.field_count!=N||(!value.fields&&N)) { break; }
            for(std::size_t j{};j!=N;++j)
            { if(!same_field(value.fields[j],fields[j])) { throw std::runtime_error("standard Service protocol declaration differs"); } }
            return;
        }
        throw std::runtime_error("complete standard Service protocol declaration missing");
    }

    /** Serialize an already verified Service graph. Message's existing exporter
     * remains unchanged. The extra fields keep source protocol derivation and
     * actual installed Event introspection separate in the raw receipt. */
    inline std::string service_document(ipc::custom_native_schema schema,std::string_view root,bool event_native)
    {
        auto identity{rihs_identity(schema,root)};
        if(identity.empty()) { throw std::runtime_error("complete Service derived RIHS unavailable"); }
        std::array<std::size_t,256> refs{};std::size_t selected{schema.record_count},count{};
        for(std::size_t i{};i!=schema.record_count;++i)
        { if(schema.records[i].type==root) { selected=i; }else { refs[count++]=i; } }
        if(selected==schema.record_count) { throw std::runtime_error("Service root missing"); }
        std::sort(refs.begin(),refs.begin()+count,[&](auto a,auto b) {return schema.records[a].type<schema.records[b].type;});
        rihs_json json;json.add("{\"native_identity\": \"");json.add(identity);
        json.add("\", \"identity_provenance\": \"LEGACY_NATIVE_SERVICE_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01\", \"native_baked_hash_available\": false, \"standard_protocol\": \"ROS2_SERVICE_EVENT_V1\", \"protocol_event_native_introspection\": ");
        json.add(event_native?"true":"false");json.add(", \"root\": ");record(json,schema.records[selected]);json.add(", \"references\": [");
        for(std::size_t i{};i!=count;++i) { if(i) {json.add(", ");}record(json,schema.records[refs[i]]); }
        json.add("]}");if(!json.valid()) { throw std::length_error("bounded Service metadata exceeded"); }
        return std::move(json).release();
    }

#if UWVM_ROS2_HAVE_SERVICE_INTROSPECTION_CPP && UWVM_ROS2_HAVE_INTROSPECTION_CPP
    template<class Members,class=void> struct has_event_members : std::false_type {};
    template<class Members> struct has_event_members<Members,std::void_t<decltype(std::declval<Members>().event_members_)>> : std::true_type {};

    template<class Members>
    bool actual_event(Members const& members,ipc::custom_native_schema schema,std::string_view event)
    {
        if constexpr(has_event_members<Members>::value)
        {
            // A present but absent/malformed modern Event never becomes a
            // virtual old-SDK Event. Compare every actual native field instead.
            std::array<ipc::custom_native_record,256> rows{};auto subset{reachable(schema,event,rows)};
            if(!members.event_members_||!matches_schema(members.event_members_,subset,event))
            { throw std::runtime_error("actual native Service Event differs"); }
            return true;
        }
        else { (void)members;(void)schema;(void)event;return false; }
    }

    /** Export a real installed Service, not two independently named Messages.
     * The actual ServiceMembers associates typed Request/Response pointers.
     * Pre-Iron SDKs do not contain Event members: their complete Event/root
     * declaration is the reviewed standard protocol, explicitly labelled below.
     * It never creates an Event endpoint or claims that absent SDK type exists.
     * All metadata/lazy dispatch must finish BEFORE the owner's SIGSTOP. */
    template<class Native>
    std::string service(ipc::custom_native_schema schema,std::string_view root)
    {
        using Request=typename Native::Request;using Response=typename Native::Response;
        auto support{::rosidl_typesupport_cpp::get_service_type_support_handle<Native>()};
        auto request_support{::rosidl_typesupport_cpp::get_message_type_support_handle<Request>()};
        auto response_support{::rosidl_typesupport_cpp::get_message_type_support_handle<Response>()};
        auto time_support{::rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>()};
        if(!support||!request_support||!response_support||!time_support)
        { throw std::runtime_error("actual native Service/component support unavailable"); }
        if constexpr(has_baked_hash<std::remove_pointer_t<decltype(support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(request_support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(response_support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(time_support)>>::value)
        { throw std::runtime_error("hash-capable native Service requires original exporter"); }
        else
        {
            constexpr char id[]{"rosidl_typesupport_introspection_cpp"};
            if(!support->typesupport_identifier) {throw std::runtime_error("Service identifier absent");}
            auto selected{std::strcmp(support->typesupport_identifier,id)==0?support:(support->func?support->func(support,id):nullptr)};
            if(!selected||!selected->typesupport_identifier||std::strcmp(selected->typesupport_identifier,id)||!selected->data)
            {throw std::runtime_error("actual Service introspection unavailable");}
            auto actual{static_cast<::rosidl_typesupport_introspection_cpp::ServiceMembers const*>(selected->data)};
            if(!actual->service_namespace_||!actual->service_name_||root.size()>255)
            {throw std::runtime_error("native Service name unavailable");}
            auto slash{root.find('/')};auto last{root.rfind('/')};
            if(slash==std::string_view::npos||root.substr(slash,5)!="/srv/"||last!=slash+4||
                !native_string(actual->service_name_,root.substr(last+1)))
            {throw std::runtime_error("actual native Service name differs");}
            std::string space{root.substr(0,last)};
            for(std::size_t at{};(at=space.find('/',at))!=std::string::npos;) {space.replace(at,1,"::");at+=2;}
            if(!native_string(actual->service_namespace_,space)&&!native_string(actual->service_namespace_,root.substr(0,slash)))
            {throw std::runtime_error("actual native Service namespace differs");}
            std::string request{root};request+="_Request";std::string response{root};response+="_Response";
            std::string event{root};event+="_Event";
            std::array<ipc::custom_native_record,256> request_rows{},response_rows{},time_rows{};
            auto request_graph{reachable(schema,request,request_rows)};auto response_graph{reachable(schema,response,response_rows)};
            auto time_graph{reachable(schema,"builtin_interfaces/msg/Time",time_rows)};
            auto request_members{message_members_for<Request>()};auto response_members{message_members_for<Response>()};
            if(!request_members||!response_members||actual->request_members_!=request_members||actual->response_members_!=response_members||
                !matches_schema(request_members,request_graph,request)||!matches_schema(response_members,response_graph,response)||
                !matches_schema(message_members_for<::builtin_interfaces::msg::Time>(),time_graph,"builtin_interfaces/msg/Time"))
            {throw std::runtime_error("actual Service/component native closure differs");}
            using F=ipc::custom_native_field;
            require_record(schema,root,std::array<F,3>{{{"request_message",request},{"response_message",response},{"event_message",event}}});
            require_record(schema,event,std::array<F,3>{{{"info","service_msgs/msg/ServiceEventInfo"},
                {"request",request,0,false,0,true,1},{"response",response,0,false,0,true,1}}});
            require_record(schema,"service_msgs/msg/ServiceEventInfo",std::array<F,4>{{{"event_type","uint8"},
                {"stamp","builtin_interfaces/msg/Time"},{"client_gid","uint8",16,true},{"sequence_number","int64"}}});
            require_record(schema,"builtin_interfaces/msg/Time",std::array<F,2>{{{"sec","int32"},{"nanosec","uint32"}}});
            // Whole-root traversal refuses duplicate/disconnected/cyclic rows;
            // it occurs after independently checking actual installed payloads.
            bool const event_native{actual_event(*actual,schema,event)};
            return service_document(schema,root,event_native);
        }
    }
#else
    template<class Native>
    std::string service(ipc::custom_native_schema,std::string_view)
    { throw std::runtime_error("actual installed Service introspection header required"); }
#endif

    /** Complete external cancellation/status declarations. These are trusted
     * comparison views, never substitute SDK types, entities or capability IDs.
     * The canonical Action root excludes these shared runtime protocols; their
     * actual native metadata is preserved separately in protocol_associations. */
    inline ipc::custom_native_schema action_runtime_schema()
    {
        using F=ipc::custom_native_field;using R=ipc::custom_native_record;
        static constexpr F time[]{{"sec","int32"},{"nanosec","uint32"}};
        static constexpr F uuid[]{{"uuid","uint8",16,true}};
        static constexpr F info[]{{"goal_id","unique_identifier_msgs/msg/UUID"},{"stamp","builtin_interfaces/msg/Time"}};
        static constexpr F status[]{{"goal_info","action_msgs/msg/GoalInfo"},{"status","int8"}};
        static constexpr F statuses[]{{"status_list","action_msgs/msg/GoalStatus",0,false,0,true,0}};
        static constexpr F request[]{{"goal_info","action_msgs/msg/GoalInfo"}};
        static constexpr F response[]{{"return_code","int8"},{"goals_canceling","action_msgs/msg/GoalInfo",0,false,0,true,0}};
        static constexpr F event_info[]{{"event_type","uint8"},{"stamp","builtin_interfaces/msg/Time"},{"client_gid","uint8",16,true},{"sequence_number","int64"}};
        static constexpr F event[]{{"info","service_msgs/msg/ServiceEventInfo"},{"request","action_msgs/srv/CancelGoal_Request",0,false,0,true,1},{"response","action_msgs/srv/CancelGoal_Response",0,false,0,true,1}};
        static constexpr F root[]{{"request_message","action_msgs/srv/CancelGoal_Request"},{"response_message","action_msgs/srv/CancelGoal_Response"},{"event_message","action_msgs/srv/CancelGoal_Event"}};
        static constexpr R rows[]{{"builtin_interfaces/msg/Time",time,2},{"unique_identifier_msgs/msg/UUID",uuid,1},
            {"action_msgs/msg/GoalInfo",info,2},{"action_msgs/msg/GoalStatus",status,2},{"action_msgs/msg/GoalStatusArray",statuses,1},
            {"action_msgs/srv/CancelGoal_Request",request,1},{"action_msgs/srv/CancelGoal_Response",response,2},
            {"service_msgs/msg/ServiceEventInfo",event_info,4},{"action_msgs/srv/CancelGoal_Event",event,3},{"action_msgs/srv/CancelGoal",root,3}};
        return {rows,sizeof(rows)/sizeof(rows[0])};
    }

    inline void require_legacy_graph(ipc::custom_native_schema schema)
    {
        if(!schema.records||!schema.record_count||schema.record_count>256)
        {throw std::runtime_error("finite complete Action graph required");}
        for(std::size_t i{};i!=schema.record_count;++i)
        {
            auto const& row{schema.records[i]};if(row.field_count>4096||(!row.fields&&row.field_count))
            {throw std::runtime_error("finite complete Action fields required");}
            for(std::size_t j{};j!=i;++j) {if(row.type==schema.records[j].type) {throw std::runtime_error("duplicate Action declaration");}}
            for(std::size_t j{};j!=row.field_count;++j) {if(!legacy_default_empty(row.fields[j])) {throw std::runtime_error("legacy declared defaults are unsupported");}}
        }
    }

    inline void require_action_runtime(ipc::custom_native_schema schema)
    {
        require_legacy_graph(schema);auto expected{action_runtime_schema()};
        if(schema.record_count!=expected.record_count) {throw std::runtime_error("complete Action runtime inventory differs");}
        for(std::size_t i{};i!=expected.record_count;++i)
        {
            auto const& e{expected.records[i]};std::size_t selected{schema.record_count};
            for(std::size_t j{};j!=schema.record_count;++j) {if(schema.records[j].type==e.type) {selected=j;}}
            if(selected==schema.record_count||schema.records[selected].field_count!=e.field_count)
            {throw std::runtime_error("Action runtime record missing");}
            for(std::size_t j{};j!=e.field_count;++j) {if(!same_field(schema.records[selected].fields[j],e.fields[j])) {throw std::runtime_error("Action runtime declaration differs");}}
        }
    }

    inline std::string action_document(ipc::custom_native_schema schema,std::string_view root,
        std::array<std::pair<std::string_view,std::string>,7> const& protocols)
    {
        auto hash{rihs_identity(schema,root)};if(hash.empty()) {throw std::runtime_error("complete Action RIHS unavailable");}
        std::array<std::size_t,256> refs{};std::size_t selected{schema.record_count},count{};
        for(std::size_t i{};i!=schema.record_count;++i) {if(schema.records[i].type==root) {selected=i;}else {refs[count++]=i;}}
        if(selected==schema.record_count) {throw std::runtime_error("Action root missing");}
        std::sort(refs.begin(),refs.begin()+count,[&](auto a,auto b){return schema.records[a].type<schema.records[b].type;});
        rihs_json json;json.add("{\"native_identity\": \"");json.add(hash);
        json.add("\", \"identity_provenance\": \"LEGACY_NATIVE_ACTION_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01\", \"native_baked_hash_available\": false, \"standard_protocol\": \"ROS2_ACTION_EVENT_V1\", \"action_type_support_associations_verified\": true, \"root\": ");
        record(json,schema.records[selected]);json.add(", \"references\": [");
        for(std::size_t i{};i!=count;++i) {if(i) {json.add(", ");}record(json,schema.records[refs[i]]);}json.add("], \"protocol_associations\": {");
        for(std::size_t i{};i!=protocols.size();++i) {if(i) {json.add(", ");}json.name(protocols[i].first);json.add(": ");json.add(protocols[i].second);}
        json.add("}}");if(!json.valid()) {throw std::length_error("bounded Action metadata exceeded");}return std::move(json).release();
    }

#if UWVM_ROS2_HAVE_LEGACY_ACTION_TYPES && UWVM_ROS2_HAVE_SERVICE_INTROSPECTION_CPP && UWVM_ROS2_HAVE_INTROSPECTION_CPP
    template<class Native>
    std::string action_protocol_service(ipc::custom_native_schema schema,std::string_view root)
    {
        using Request=typename Native::Request;using Response=typename Native::Response;
        auto support{::rosidl_typesupport_cpp::get_service_type_support_handle<Native>()};
        auto request_support{::rosidl_typesupport_cpp::get_message_type_support_handle<Request>()};
        auto response_support{::rosidl_typesupport_cpp::get_message_type_support_handle<Response>()};
        auto time_support{::rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>()};
        if(!support||!request_support||!response_support||!time_support)
        { throw std::runtime_error("actual native Service/component support unavailable"); }
        if constexpr(has_baked_hash<std::remove_pointer_t<decltype(support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(request_support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(response_support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(time_support)>>::value)
        { throw std::runtime_error("hash-capable native Service requires original exporter"); }
        else
        {
            constexpr char id[]{"rosidl_typesupport_introspection_cpp"};
            if(!support->typesupport_identifier) {throw std::runtime_error("Service identifier absent");}
            auto selected{std::strcmp(support->typesupport_identifier,id)==0?support:(support->func?support->func(support,id):nullptr)};
            if(!selected||!selected->typesupport_identifier||std::strcmp(selected->typesupport_identifier,id)||!selected->data)
            {throw std::runtime_error("actual Service introspection unavailable");}
            auto actual{static_cast<::rosidl_typesupport_introspection_cpp::ServiceMembers const*>(selected->data)};
            if(!actual->service_namespace_||!actual->service_name_||root.size()>255)
            {throw std::runtime_error("native Service name unavailable");}
            auto slash{root.find('/')};auto last{root.rfind('/')};
            if(slash==std::string_view::npos||root.substr(slash,8)!="/action/"||last!=slash+7||
                !native_string(actual->service_name_,root.substr(last+1)))
            {throw std::runtime_error("actual native Service name differs");}
            std::string space{root.substr(0,last)};
            for(std::size_t at{};(at=space.find('/',at))!=std::string::npos;) {space.replace(at,1,"::");at+=2;}
            if(!native_string(actual->service_namespace_,space)&&!native_string(actual->service_namespace_,root.substr(0,slash)))
            {throw std::runtime_error("actual native Service namespace differs");}
            std::string request{root};request+="_Request";std::string response{root};response+="_Response";
            std::string event{root};event+="_Event";
            std::array<ipc::custom_native_record,256> request_rows{},response_rows{},time_rows{};
            auto request_graph{reachable(schema,request,request_rows)};auto response_graph{reachable(schema,response,response_rows)};
            auto time_graph{reachable(schema,"builtin_interfaces/msg/Time",time_rows)};
            auto request_members{message_members_for<Request>()};auto response_members{message_members_for<Response>()};
            if(!request_members||!response_members||actual->request_members_!=request_members||actual->response_members_!=response_members||
                !matches_schema(request_members,request_graph,request)||!matches_schema(response_members,response_graph,response)||
                !matches_schema(message_members_for<::builtin_interfaces::msg::Time>(),time_graph,"builtin_interfaces/msg/Time"))
            {throw std::runtime_error("actual Service/component native closure differs");}
            using F=ipc::custom_native_field;
            require_record(schema,root,std::array<F,3>{{{"request_message",request},{"response_message",response},{"event_message",event}}});
            require_record(schema,event,std::array<F,3>{{{"info","service_msgs/msg/ServiceEventInfo"},
                {"request",request,0,false,0,true,1},{"response",response,0,false,0,true,1}}});
            require_record(schema,"service_msgs/msg/ServiceEventInfo",std::array<F,4>{{{"event_type","uint8"},
                {"stamp","builtin_interfaces/msg/Time"},{"client_gid","uint8",16,true},{"sequence_number","int64"}}});
            require_record(schema,"builtin_interfaces/msg/Time",std::array<F,2>{{{"sec","int32"},{"nanosec","uint32"}}});
            // Whole-root traversal refuses duplicate/disconnected/cyclic rows;
            // it occurs after independently checking actual installed payloads.
            bool const event_native{actual_event(*actual,schema,event)};
            return service_document(schema,root,event_native);
        }
    }
    /** Prove the actual ActionSupport's five native protocol associations and
     * every typed payload before deriving the complete modern root grammar.
     * Old native Event absence is retained in each Service receipt; cancellation
     * and status metadata are additional native facts, not root-hash members. */
    template<class Native>
    std::string action(ipc::custom_native_schema schema,std::string_view root,ipc::custom_native_schema runtime)
    {
        using Send=typename Native::Impl::SendGoalService;using Get=typename Native::Impl::GetResultService;
        using Cancel=typename Native::Impl::CancelGoalService;using Feedback=typename Native::Impl::FeedbackMessage;
        using Status=typename Native::Impl::GoalStatusMessage;using UUID=::unique_identifier_msgs::msg::UUID;
        using Time=::builtin_interfaces::msg::Time;
        // These compile-time relationships tie the wrapper fields to the real
        // generated payload aliases; same-shaped substituted records do not pass.
        static_assert(std::is_same_v<decltype(std::declval<typename Send::Request>().goal),typename Native::Goal>);
        static_assert(std::is_same_v<decltype(std::declval<typename Get::Response>().result),typename Native::Result>);
        static_assert(std::is_same_v<decltype(std::declval<Feedback>().feedback),typename Native::Feedback>);
        static_assert(std::is_same_v<decltype(std::declval<typename Send::Request>().goal_id),UUID>);
        static_assert(std::is_same_v<decltype(std::declval<typename Get::Request>().goal_id),UUID>);
        static_assert(std::is_same_v<decltype(std::declval<Feedback>().goal_id),UUID>);
        static_assert(std::is_same_v<decltype(std::declval<typename Send::Response>().stamp),Time>);
        static_assert(std::is_same_v<Cancel,::action_msgs::srv::CancelGoal>);
        static_assert(std::is_same_v<Status,::action_msgs::msg::GoalStatusArray>);
        auto support{::rosidl_typesupport_cpp::get_action_type_support_handle<Native>()};
        auto send_support{::rosidl_typesupport_cpp::get_service_type_support_handle<Send>()};
        auto get_support{::rosidl_typesupport_cpp::get_service_type_support_handle<Get>()};
        auto cancel_support{::rosidl_typesupport_cpp::get_service_type_support_handle<Cancel>()};
        auto feedback_support{::rosidl_typesupport_cpp::get_message_type_support_handle<Feedback>()};
        auto status_support{::rosidl_typesupport_cpp::get_message_type_support_handle<Status>()};
        if(!support||!send_support||!get_support||!cancel_support||!feedback_support||!status_support)
        {throw std::runtime_error("actual Action/protocol support unavailable");}
        if constexpr(has_baked_hash<std::remove_pointer_t<decltype(support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(send_support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(get_support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(cancel_support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(feedback_support)>>::value||
            has_baked_hash<std::remove_pointer_t<decltype(status_support)>>::value)
        {throw std::runtime_error("hash-capable Action or protocol requires original exporter");}
        else
        {
            require_legacy_graph(schema);require_action_runtime(runtime);
            auto slash{root.find('/')};if(slash==std::string_view::npos||root.size()>255||
                root.substr(slash,8)!="/action/"||root.rfind('/')!=slash+7||root.size()==slash+8)
            {throw std::runtime_error("exact Action category/name required");}
            if(support->goal_service_type_support!=send_support||support->result_service_type_support!=get_support||
                support->cancel_service_type_support!=cancel_support||support->feedback_message_type_support!=feedback_support||
                support->status_message_type_support!=status_support)
            {throw std::runtime_error("actual ActionSupport typed protocol associations differ");}
            std::string goal{root};goal+="_Goal";std::string result{root};result+="_Result";std::string feedback{root};feedback+="_Feedback";
            std::string send{root};send+="_SendGoal";std::string get{root};get+="_GetResult";std::string wrapped{root};wrapped+="_FeedbackMessage";
            using F=ipc::custom_native_field;
            require_record(schema,root,std::array<F,6>{{{"goal",goal},{"result",result},{"feedback",feedback},
                {"send_goal_service",send},{"get_result_service",get},{"feedback_message",wrapped}}});
            require_record(schema,send+"_Request",std::array<F,2>{{{"goal_id","unique_identifier_msgs/msg/UUID"},{"goal",goal}}});
            require_record(schema,send+"_Response",std::array<F,2>{{{"accepted","bool"},{"stamp","builtin_interfaces/msg/Time"}}});
            require_record(schema,get+"_Request",std::array<F,1>{{{"goal_id","unique_identifier_msgs/msg/UUID"}}});
            require_record(schema,get+"_Response",std::array<F,2>{{{"status","int8"},{"result",result}}});
            require_record(schema,wrapped,std::array<F,2>{{{"goal_id","unique_identifier_msgs/msg/UUID"},{"feedback",feedback}}});
            require_record(schema,"unique_identifier_msgs/msg/UUID",std::array<F,1>{{{"uuid","uint8",16,true}}});
            std::array<ipc::custom_native_record,256> rows{};
            auto subset=[&](std::string_view name){return reachable(schema,name,rows);};
            (void)message<typename Native::Goal>(subset(goal),goal);
            (void)message<typename Native::Result>(subset(result),result);
            (void)message<typename Native::Feedback>(subset(feedback),feedback);
            auto send_text{action_protocol_service<Send>(subset(send),send)};
            auto get_text{action_protocol_service<Get>(subset(get),get)};
            auto feedback_text{message<Feedback>(subset(wrapped),wrapped)};
            auto uuid_text{message<UUID>(subset("unique_identifier_msgs/msg/UUID"),"unique_identifier_msgs/msg/UUID")};
            auto time_text{message<Time>(subset("builtin_interfaces/msg/Time"),"builtin_interfaces/msg/Time")};
            auto cancel_graph{reachable(runtime,"action_msgs/srv/CancelGoal",rows)};
            auto cancel_text{service<Cancel>(cancel_graph,"action_msgs/srv/CancelGoal")};
            auto status_graph{reachable(runtime,"action_msgs/msg/GoalStatusArray",rows)};
            auto status_text{message<Status>(status_graph,"action_msgs/msg/GoalStatusArray")};
            std::array<std::pair<std::string_view,std::string>,7> proofs{{{"send_goal",std::move(send_text)},
                {"get_result",std::move(get_text)},{"feedback_message",std::move(feedback_text)},
                {"cancel_goal",std::move(cancel_text)},{"status",std::move(status_text)},
                {"uuid",std::move(uuid_text)},{"time",std::move(time_text)}}};
            return action_document(schema,root,proofs);
        }
    }
#else
    template<class Native>
    std::string action(ipc::custom_native_schema,std::string_view,ipc::custom_native_schema)
    {throw std::runtime_error("actual installed Action typesupport/UUID/cancel/status/introspection required");}
#endif

}
