#pragma once
#include "compat/node.h"
#include "qos.h"
#include "compat/identity.h"
#include "compat/parameters.h"
#include "../common/server.h"
#include "../common/bounded_text.h"
#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters.hpp>
#include <rcl_interfaces/srv/list_parameters.hpp>
#include <rcl_interfaces/srv/describe_parameters.hpp>
#include <rcl_interfaces/msg/parameter_type.hpp>
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <uwvm2/imported/ros/ipc/parameter_event.h>
#include <uwvm2/imported/ros/ipc/parameter.h>
#include <mutex>
#include <iomanip>
#if UWVM_ROS2_HAVE_ACTION_GRAPH
#include <rcl_action/graph.h>
#endif
#include <set>

namespace uwvm2::ros_gateway::ros2_resources
{
    inline bool prefix(std::string_view scope,std::string_view name)
    { return scope=="/" || name==scope || (name.size()>scope.size() && name.compare(0,scope.size(),scope)==0 && name[scope.size()]=='/'); }
    inline bool parameter_name(std::string const& name)
    {
        if(name.empty()||name.size()>255) { return false; }
        bool start{true};
        for(char c:name)
        {
            if(c=='.') { if(start) { return false; } start=true; continue; }
            bool alpha{(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_'};
            if(!alpha&&!(c>='0'&&c<='9'&&!start)) { return false; } start=false;
        }
        return !start;
    }
    inline std::string parameter_resource(std::string const& node,std::string name)
    { std::replace(name.begin(),name.end(),'.','/'); return node+"/"+name; }
    inline std::uint8_t native_kind(ipc::parameter_kind kind)
    {
        using type=rcl_interfaces::msg::ParameterType;
        if(kind==ipc::parameter_kind::string) { return type::PARAMETER_STRING; }
        if(kind==ipc::parameter_kind::boolean) { return type::PARAMETER_BOOL; }
        if(kind==ipc::parameter_kind::int64) { return type::PARAMETER_INTEGER; }
        if(kind==ipc::parameter_kind::float64) { return type::PARAMETER_DOUBLE; }
        if(kind==ipc::parameter_kind::boolean_array) { return type::PARAMETER_BOOL_ARRAY; }
        if(kind==ipc::parameter_kind::int64_array) { return type::PARAMETER_INTEGER_ARRAY; }
        if(kind==ipc::parameter_kind::float64_array) { return type::PARAMETER_DOUBLE_ARRAY; }
        if(kind==ipc::parameter_kind::string_array) { return type::PARAMETER_STRING_ARRAY; }
        if(kind==ipc::parameter_kind::byte_array) { return type::PARAMETER_BYTE_ARRAY; }
        return type::PARAMETER_NOT_SET;
    }
    inline bool known(ros::capability_spec const& cap)
    {
        if(cap.operation=="graph.list-topics"||cap.operation=="graph.list-nodes"||cap.operation=="graph.list-services"||cap.operation=="graph.inspect-topic"||cap.operation=="graph.inspect-node"||cap.operation=="graph.inspect-service"||cap.operation=="graph.inspect-action")
        { return cap.type=="builtin/graph-list-v1"&&cap.type_identity=="builtin:graph-list-v1"; }
        if(cap.operation=="param.subscribe-events")
        {
            bool const legacy{cap.type=="rcl_interfaces/msg/ParameterEvent"};
            if(!legacy&&ipc::validate_parameter_contract(ros::version::ros2,cap)) { return false; }
            if(!legacy&&ipc::parameter_event_type(ros::version::ros2,cap.type)==ipc::parameter_kind::unsupported) { return false; }
            auto node{cap.limits.find("parameter-node")},name{cap.limits.find("parameter-name")};
            constexpr char native_identity[]{"RIHS01_043e627780fcad87a22d225bc2a037361dba713fca6a6b9f4b869a5aa0393204"};
            return ros2_compat::builtin_message<rcl_interfaces::msg::ParameterEvent>("rcl_interfaces/msg/ParameterEvent",native_identity)&&
                   (legacy?cap.type_identity==native_identity:cap.type_identity==ipc::parameter_event_identity(ros::version::ros2,cap.type))&&
                   node!=cap.limits.end()&&name!=cap.limits.end()&&canonical(node->second)&&parameter_name(name->second)&&cap.resource==parameter_resource(node->second,name->second);
        }
        if(cap.operation=="param.get"||cap.operation=="param.set"||cap.operation=="param.declare"||cap.operation=="param.undeclare"||cap.operation=="param.describe"||cap.operation=="param.list")
        {
            if(ipc::scalar_parameter_operation(ros::version::ros2,cap.operation))
            { if(ipc::validate_parameter_contract(ros::version::ros2,cap)) { return false; } }
            else if(cap.type!="builtin/string-v1"||cap.type_identity!="builtin:param-string-v1") { return false; }
            auto node{cap.limits.find("parameter-node")},name{cap.limits.find("parameter-name")};
            if(node==cap.limits.end()||name==cap.limits.end()||!canonical(node->second)) { return false; }
            if(cap.operation=="param.list") { return name->second=="not-applicable"&&cap.resource==node->second; }
            return parameter_name(name->second)&&cap.resource==parameter_resource(node->second,name->second);
        }
        return false;
    }
    template<typename Container>
    inline std::uint32_t project_event_array(ros::capability_spec const& cap,ipc::parameter_kind kind,
        Container const& native,std::uint64_t budget,bool copy,ipc::parameter_value& output)
    {
        budget=std::min<std::uint64_t>(budget,ipc::parameter_wire_limit);
        if(native.size()>limit(cap,"max-parameter-elements")||native.size()>ipc::parameter_max_elements||budget<4) { return 90; }
        auto remaining{budget-4};
        if constexpr(std::is_same_v<typename Container::value_type,std::string>)
        {
            for(auto const& text:native)
            {
                if(remaining<4||text.size()>remaining-4) { return 90; }
                if(!ipc::parameter_array_string_valid(ros::version::ros2,text)) { return 22; }
                remaining-=4+text.size();
            }
        }
        else if(native.size()>remaining/ipc::parameter_element_width(kind)) { return 90; }
        if constexpr(std::is_same_v<typename Container::value_type,double>)
        {
            double lo{},hi{};ipc::parse_parameter_real(field(cap,"parameter-min-value"),lo);ipc::parse_parameter_real(field(cap,"parameter-max-value"),hi);
            for(auto item:native) { if(!std::isfinite(item)) { return 22; }if(item<lo||item>hi) { return 13; } }
        }
        else if constexpr(std::is_same_v<typename Container::value_type,std::int64_t>)
        {
            std::int64_t lo{},hi{};ipc::parse_parameter_integer(field(cap,"parameter-min-value"),lo);ipc::parse_parameter_integer(field(cap,"parameter-max-value"),hi);
            for(auto item:native) { if(item<lo||item>hi) { return 13; } }
        }
        ipc::parameter_value value;value.type=kind;
        if(copy)
        {
            if constexpr(std::is_same_v<typename Container::value_type,std::string>) { value.string_array_value=native; }
            else if constexpr(std::is_same_v<typename Container::value_type,double>) { value.double_array_value=native; }
            else if constexpr(std::is_same_v<typename Container::value_type,std::int64_t>) { value.int64_array_value=native; }
            else
            {
                if(kind==ipc::parameter_kind::boolean_array)
                { value.bool_array_value.reserve(native.size());for(auto item:native) { value.bool_array_value.push_back(item?1:0); } }
                else { value.byte_array_value.assign(native.begin(),native.end()); }
            }
        }
        output=std::move(value);return 0;
    }
    inline std::uint32_t project_event_value(ros::capability_spec const& cap,rcl_interfaces::msg::ParameterValue const& native,
        std::uint64_t budget,bool copy,ipc::parameter_value& output)
    {
        auto kind{ipc::parameter_event_type(ros::version::ros2,cap.type)};
        if(kind==ipc::parameter_kind::unsupported||native.type!=native_kind(kind)) { return 95; }
        if(kind==ipc::parameter_kind::deletion)
        {
            // NOT_SET has no prior value. Inactive generated fields do not
            // establish a prior kind, contents or numeric bounds.
            ipc::parameter_value value;value.type=kind;output=std::move(value);return 0;
        }
        if(kind==ipc::parameter_kind::byte_array) { return project_event_array(cap,kind,native.byte_array_value,budget,copy,output); }
        if(kind==ipc::parameter_kind::boolean_array) { return project_event_array(cap,kind,native.bool_array_value,budget,copy,output); }
        if(kind==ipc::parameter_kind::int64_array) { return project_event_array(cap,kind,native.integer_array_value,budget,copy,output); }
        if(kind==ipc::parameter_kind::float64_array) { return project_event_array(cap,kind,native.double_array_value,budget,copy,output); }
        if(kind==ipc::parameter_kind::string_array) { return project_event_array(cap,kind,native.string_array_value,budget,copy,output); }
        ipc::parameter_value value;value.type=kind;
        if(kind==ipc::parameter_kind::string)
        {
            budget=std::min<std::uint64_t>(budget,ipc::parameter_wire_limit);
            if(budget<4||native.string_value.size()>budget-4) { return 90; }
            if(!ipc::parameter_utf8(native.string_value)) { return 22; }
            if(copy) { value.string_value=native.string_value; }
        }
        else
        {
            if(budget<ipc::parameter_width(kind)) { return 90; }
            if(kind==ipc::parameter_kind::boolean) { value.bool_value=native.bool_value; }
            else if(kind==ipc::parameter_kind::int64) { value.integer_value=native.integer_value; }
            else if(kind==ipc::parameter_kind::float64) { value.double_value=native.double_value; }
            auto error{ipc::validate_parameter_event_value(ros::version::ros2,cap,value)};if(error) { return error; }
        }
        output=std::move(value);return 0;
    }
    inline std::uint32_t project_event_value(ros::capability_spec const& cap,::rclcpp::ParameterValue const& native,
        std::uint64_t budget,bool copy,ipc::parameter_value& output)
    {
        auto kind{ipc::parameter_event_type(ros::version::ros2,cap.type)};
        if(kind==ipc::parameter_kind::unsupported||static_cast<std::uint8_t>(native.get_type())!=native_kind(kind)) { return 95; }
        // Borrow native arrays and strings until the listener's complete wire
        // and element quotas have passed. A generated-message conversion would
        // copy these values before the listener's limits could reject them.
        if(kind==ipc::parameter_kind::byte_array) { return project_event_array(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_BYTE_ARRAY>(),budget,copy,output); }
        if(kind==ipc::parameter_kind::boolean_array) { return project_event_array(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_BOOL_ARRAY>(),budget,copy,output); }
        if(kind==ipc::parameter_kind::int64_array) { return project_event_array(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY>(),budget,copy,output); }
        if(kind==ipc::parameter_kind::float64_array) { return project_event_array(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY>(),budget,copy,output); }
        if(kind==ipc::parameter_kind::string_array) { return project_event_array(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_STRING_ARRAY>(),budget,copy,output); }
        if(kind==ipc::parameter_kind::string)
        {
            auto const& text{native.get<::rclcpp::ParameterType::PARAMETER_STRING>()};
            budget=std::min<std::uint64_t>(budget,ipc::parameter_wire_limit);
            if(budget<4||text.size()>budget-4) { return 90; }
            if(!ipc::parameter_utf8(text)) { return 22; }
            ipc::parameter_value value;value.type=kind;if(copy) { value.string_value=text; }
            output=std::move(value);return 0;
        }
        // Read only the active scalar. A wholesale generated-message conversion
        // could copy oversized inactive native strings/arrays even for Bool or
        // NOT_SET, before this projection's byte limits apply.
        ipc::parameter_value value;value.type=kind;
        if(kind==ipc::parameter_kind::deletion) { output=std::move(value);return 0; }
        if(budget<ipc::parameter_width(kind)) { return 90; }
        if(kind==ipc::parameter_kind::boolean) { value.bool_value=native.get<::rclcpp::ParameterType::PARAMETER_BOOL>(); }
        else if(kind==ipc::parameter_kind::int64) { value.integer_value=native.get<::rclcpp::ParameterType::PARAMETER_INTEGER>(); }
        else if(kind==ipc::parameter_kind::float64) { value.double_value=native.get<::rclcpp::ParameterType::PARAMETER_DOUBLE>(); }
        else { return 95; }
        auto error{ipc::validate_parameter_event_value(ros::version::ros2,cap,value)};if(error) { return error; }
        output=std::move(value);return 0;
    }
    inline std::uint8_t event_native_type(rcl_interfaces::msg::ParameterValue const& value) { return value.type; }
    inline std::uint8_t event_native_type(::rclcpp::ParameterValue const& value) { return static_cast<std::uint8_t>(value.get_type()); }
    inline std::string const& event_native_string(rcl_interfaces::msg::ParameterValue const& value) { return value.string_value; }
    inline std::string const& event_native_string(::rclcpp::ParameterValue const& value)
    {
        static std::string const empty;
        return value.get_type()==::rclcpp::ParameterType::PARAMETER_STRING?value.get<::rclcpp::ParameterType::PARAMETER_STRING>():empty;
    }
    template<typename NativeValue>
    inline bool enqueue_parameter_event(event_queue& queue,std::uint32_t seconds,std::uint32_t nanoseconds,std::uint8_t kind,
        std::string_view node,std::string_view name,NativeValue const& native)
    {
        auto const& cap{queue.capability};
        if(node!=field(cap,"parameter-node")||name!=field(cap,"parameter-name")||!limit(cap,"max-event-fields")||
           seconds>2147483647U||nanoseconds>=1000000000U||kind<1||kind>3) { return false; }
        auto selected{ipc::parameter_event_type(ros::version::ros2,cap.type)};
        if(selected!=ipc::parameter_kind::unsupported)
        {
            if(selected==ipc::parameter_kind::deletion&&kind!=3) { return false; }
            // Deletion omits the old payload, but a concrete prior tag still
            // attests its type, bounds, element quota, UTF-8 and hard wire cap.
            auto maximum{limit(cap,"max-parameter-bytes")};
            auto base{std::uint64_t{23}+node.size()+name.size()};if(base>maximum) { return false; }
            ipc::parameter_value projected;
            if(project_event_value(cap,native,kind==3?ipc::parameter_wire_limit:maximum-base,kind!=3,projected)) { return false; }
            std::vector<std::uint8_t> bytes;
            if(ipc::encode_typed_parameter_event_view(seconds,nanoseconds,kind,node,name,projected,bytes)) { return false; }
            return queue.enqueue(std::move(bytes));
        }
        auto type{event_native_type(native)};
        if(kind!=3&&type!=rcl_interfaces::msg::ParameterType::PARAMETER_STRING) { return false; }
        if(kind==3&&type!=rcl_interfaces::msg::ParameterType::PARAMETER_NOT_SET&&type!=rcl_interfaces::msg::ParameterType::PARAMETER_STRING) { return false; }
        auto const& text{event_native_string(native)};
        auto maximum{limit(cap,"max-parameter-bytes")};auto base{std::uint64_t{21}+node.size()+name.size()};
        if(base>maximum||(kind!=3&&text.size()>maximum-base)||!ipc::parameter_utf8(text)) { return false; }
        return queue.enqueue(ipc::encode_parameter_event_view(seconds,nanoseconds,kind,node,name,kind==3?std::string_view{}:std::string_view{text}));
    }
    inline bool active_parameter_event_queue(event_queue& queue)
    { std::lock_guard<std::mutex> guard{queue.mutex};return queue.active; }
    struct local_parameter_event_routes
    {
        // These references grant no lifetime or authority. The hard ceiling
        // bounds registry memory independently of the trusted policy size;
        // normal per-grant resource counts and session limits still apply.
        static constexpr std::size_t maximum_routes{65536};
        struct route { std::weak_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> node;std::weak_ptr<event_queue> queue; };
        std::mutex mutex;std::vector<route> routes;
        void prune()
        {
            routes.erase(std::remove_if(routes.begin(),routes.end(),[](route const& item) {
                auto queue{item.queue.lock()};return item.node.expired()||!queue||!active_parameter_event_queue(*queue);
            }),routes.end());
        }
    };
    inline local_parameter_event_routes& local_parameter_events()
    { static local_parameter_event_routes routes;return routes; }
    template<typename Container>
    inline bool snapshot_parameter_array(Container const& values,std::size_t width,::rclcpp::ParameterValue& output)
    {
        if(values.size()>ipc::parameter_max_elements) { return false; }
        auto remaining{ipc::parameter_wire_limit-4};
        if constexpr(std::is_same_v<typename Container::value_type,std::string>)
        {
            for(auto const& text:values)
            { if(remaining<4||text.size()>remaining-4) { return false; }remaining-=4+text.size(); }
        }
        else if(values.size()>remaining/width) { return false; }
        output=::rclcpp::ParameterValue{values};return true;
    }
    inline bool snapshot_native_parameter(::rclcpp::ParameterValue const& value,::rclcpp::ParameterValue& output)
    {
        using type=::rclcpp::ParameterType;
        // Reconstruct only the active native field. Copying a generated value
        // wholesale could also copy unrelated, oversized inactive fields.
        switch(value.get_type())
        {
        case type::PARAMETER_NOT_SET:output={};return true;
        case type::PARAMETER_BOOL:output=::rclcpp::ParameterValue{value.get<type::PARAMETER_BOOL>()};return true;
        case type::PARAMETER_INTEGER:output=::rclcpp::ParameterValue{value.get<type::PARAMETER_INTEGER>()};return true;
        case type::PARAMETER_DOUBLE:output=::rclcpp::ParameterValue{value.get<type::PARAMETER_DOUBLE>()};return true;
        case type::PARAMETER_STRING:
        {
            auto const& text{value.get<type::PARAMETER_STRING>()};if(text.size()>ipc::parameter_wire_limit-4) { return false; }
            output=::rclcpp::ParameterValue{text};return true;
        }
        case type::PARAMETER_BYTE_ARRAY:return snapshot_parameter_array(value.get<type::PARAMETER_BYTE_ARRAY>(),1,output);
        case type::PARAMETER_BOOL_ARRAY:return snapshot_parameter_array(value.get<type::PARAMETER_BOOL_ARRAY>(),1,output);
        case type::PARAMETER_INTEGER_ARRAY:return snapshot_parameter_array(value.get<type::PARAMETER_INTEGER_ARRAY>(),8,output);
        case type::PARAMETER_DOUBLE_ARRAY:return snapshot_parameter_array(value.get<type::PARAMETER_DOUBLE_ARRAY>(),8,output);
        case type::PARAMETER_STRING_ARRAY:return snapshot_parameter_array(value.get<type::PARAMETER_STRING_ARRAY>(),0,output);
        default:return false;
        }
    }
    struct local_parameter_commit_snapshot
    {
        struct capture
        {
            std::string name;std::thread::id writer;::rclcpp::ParameterValue value;bool ready{};
            explicit capture(std::string selected):name{std::move(selected)},writer{std::this_thread::get_id()} {}
        };
        std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> node;
        std::shared_ptr<capture> owned;
        ros2_compat::post_set_lease callback;
        local_parameter_commit_snapshot(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> current,std::string name):node{std::move(current)},owned{std::make_shared<capture>(std::move(name))}
        {
            // Jazzy declares and sets parameters under its native mutation
            // mutex before invoking this post-set callback. Capture only this
            // adapter call's selected value while it is still safe to borrow.
            // The callback owns its state, performs no queue/registry/native
            // calls, and cannot change a mutation if its bounded copy fails.
            callback=ros2_compat::post_set(node,[state=owned](std::vector<::rclcpp::Parameter> const& values) noexcept {
                if(std::this_thread::get_id()!=state->writer) { return; }
                for(auto const& value:values)
                {
                    if(value.get_name()!=state->name) { continue; }
                    try { state->ready=snapshot_native_parameter(value.get_parameter_value(),state->value); }
                    catch(...) { state->ready=false; }
                }
            });
        }
        void committed()
        {
            // Pre-Iron has no post-set API. Read the actual committed value
            // only AFTER a successful mutation, while local_parameters_mutex
            // still serializes every Gateway writer. Native parameter services
            // and the event publisher are disabled at Node construction. Never
            // announce success from an on-set validation callback.
            if(!callback&&!owned->ready)
            { try { owned->ready=snapshot_native_parameter(node->get_parameter(owned->name).get_parameter_value(),owned->value); }catch(...) { owned->ready=false; } }
        }
        local_parameter_commit_snapshot(local_parameter_commit_snapshot const&)=delete;
        local_parameter_commit_snapshot& operator=(local_parameter_commit_snapshot const&)=delete;
    };
    inline bool register_local_parameter_events(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const& node,std::shared_ptr<event_queue> const& queue)
    {
        if(ros2_compat::parameter_events_enabled(node)||field(queue->capability,"parameter-node")!=ros2_compat::full_name(node)) { return true; }
        auto& registry{local_parameter_events()};std::lock_guard<std::mutex> guard{registry.mutex};registry.prune();
        for(auto const& item:registry.routes) { if(item.node.lock()==node&&item.queue.lock()==queue) { return true; } }
        if(registry.routes.size()>=local_parameter_event_routes::maximum_routes) { return false; }
        registry.routes.push_back({node,queue});return true;
    }
    inline void notify_local_parameter_event(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const& node,std::string_view name,std::uint8_t kind,
        ::rclcpp::ParameterValue const& value) noexcept
    {
        // A committed mutation stays committed if its best-effort event drops,
        // allocation fails, the clock is outside the wire range, or an RPC
        // deadline subsequently expires. No native publisher is created here.
        try
        {
            if(ros2_compat::parameter_events_enabled(node)) { return; }
            auto stamp{node->get_clock()->now().nanoseconds()};if(stamp<0||stamp/1000000000LL>2147483647LL) { return; }
            auto seconds{static_cast<std::uint32_t>(stamp/1000000000LL)},nanoseconds{static_cast<std::uint32_t>(stamp%1000000000LL)};
            auto& registry{local_parameter_events()};std::lock_guard<std::mutex> guard{registry.mutex};registry.prune();
            for(auto const& item:registry.routes)
            {
                if(item.node.lock()!=node) { continue; }auto queue{item.queue.lock()};if(!queue||!active_parameter_event_queue(*queue)) { continue; }
                try
                {
                    // A local undeclare knows its prior value. Concrete grants
                    // independently attest it; tag0 grants deliberately receive
                    // NOT_SET without inferring any prior type or value.
                    if(kind==3&&ipc::parameter_event_type(ros::version::ros2,queue->capability.type)==ipc::parameter_kind::deletion)
                    { enqueue_parameter_event(*queue,seconds,nanoseconds,kind,ros2_compat::full_name(node),name,::rclcpp::ParameterValue{}); }
                    else { enqueue_parameter_event(*queue,seconds,nanoseconds,kind,ros2_compat::full_name(node),name,value); }
                }
                catch(...) {} // One failed listener cannot suppress its siblings.
            }
        }
        catch(...) {} // Event delivery never changes an already committed RPC.
    }
    inline ipc::call_result subscribe_events(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const& node,ros::capability_spec const& cap,
        std::shared_ptr<event_queue> queue,ros2_compat::qos_type const& qos,std::shared_ptr<::rclcpp::SubscriptionBase>& subscription)
    {
        if(!known(cap)||cap.operation!="param.subscribe-events"||!ros2_qos::supports_entity_qos<::rclcpp::Subscription<rcl_interfaces::msg::ParameterEvent>>::value) { return {95,{}}; }
        if(!queue||queue->capability.alias!=cap.alias||queue->capability.operation!=cap.operation||queue->capability.resource!=cap.resource||
           queue->capability.type!=cap.type||queue->capability.type_identity!=cap.type_identity||queue->capability.limits!=cap.limits) { return {13,{}}; }
        std::weak_ptr<event_queue> weak{queue};
        auto verified{std::make_shared<std::atomic<bool>>(false)};
        auto callback=[weak,verified](rcl_interfaces::msg::ParameterEvent::ConstSharedPtr message)
        {
            if(!verified->load(std::memory_order_acquire)) { return; }
            auto queue{weak.lock()};if(!queue||!active_parameter_event_queue(*queue)) { return; }auto const& cap{queue->capability};
            if(message->node!=field(cap,"parameter-node")||message->stamp.sec<0||message->stamp.nanosec>=1000000000U) { return; }
            auto maximum_fields{limit(cap,"max-event-fields")};
            if(message->new_parameters.size()>maximum_fields||message->changed_parameters.size()>maximum_fields-message->new_parameters.size()||message->deleted_parameters.size()>maximum_fields-message->new_parameters.size()-message->changed_parameters.size()) { return; }
            auto process=[&](auto const& values,std::uint8_t kind)
            {
                for(auto const& value:values)
                {
                    enqueue_parameter_event(*queue,static_cast<std::uint32_t>(message->stamp.sec),message->stamp.nanosec,
                        kind,message->node,value.name,value.value);
                }
            };
            process(message->new_parameters,1);process(message->changed_parameters,2);process(message->deleted_parameters,3);
        };
        subscription=ros2_compat::create_subscription<rcl_interfaces::msg::ParameterEvent>(node,"/parameter_events",qos,std::move(callback));
        auto discard=[&] {
            subscription.reset();
            // As with adapter::close, the blocking wait set may retain this
            // entity after our owner drops it. Wake the executor outside any
            // registry/queue lock; cleanup failure never changes the refusal.
            try { ros2_compat::notify(node); }catch(...) {}
        };
        auto refuse=[&](std::uint32_t error)->ipc::call_result { discard();return {error,{}}; };
        // A weak local route becomes visible only after the explicit native
        // subscription exists and its complete actual QoS has been verified.
        try
        {
            if(!subscription||!ros2_qos::entity(*subscription,qos)) { return refuse(95); }
            if(!register_local_parameter_events(node,queue)) { return refuse(90); }
            verified->store(true,std::memory_order_release);
        }
        catch(...) { discard();throw; } // Preserve the existing native exception mapping after cleanup.
        return {};
    }
    inline ipc::call_result string_result(std::string const& value,std::uint64_t maximum)
    {
        return bounded_text::result(maximum,{value});
    }
    template<typename Descriptor>
    inline ipc::call_result descriptor_result(Descriptor const& descriptor,std::uint64_t maximum)
    {
        native_io::fixed_text<3> type;
        if(!type.number(static_cast<unsigned>(descriptor.type))) { return {22,{}}; }
        return bounded_text::result(maximum,{"name=",descriptor.name,"\ntype=",type.view(),
            "\nread_only=",descriptor.read_only?"true":"false","\ndescription=",descriptor.description,
            "\nconstraints=",descriptor.additional_constraints});
    }
    inline ::rclcpp::ParameterValue native_value(ipc::parameter_value const& value)
    {
        if(value.type==ipc::parameter_kind::string) { return ::rclcpp::ParameterValue{value.string_value}; }
        if(value.type==ipc::parameter_kind::boolean) { return ::rclcpp::ParameterValue{value.bool_value}; }
        if(value.type==ipc::parameter_kind::int64) { return ::rclcpp::ParameterValue{value.integer_value}; }
        if(value.type==ipc::parameter_kind::float64) { return ::rclcpp::ParameterValue{value.double_value}; }
        if(value.type==ipc::parameter_kind::boolean_array)
        {
            std::vector<bool> values;values.reserve(value.bool_array_value.size());
            for(auto item:value.bool_array_value) { values.push_back(item!=0); }return ::rclcpp::ParameterValue{values};
        }
        if(value.type==ipc::parameter_kind::int64_array) { return ::rclcpp::ParameterValue{value.int64_array_value}; }
        if(value.type==ipc::parameter_kind::float64_array) { return ::rclcpp::ParameterValue{value.double_array_value}; }
        if(value.type==ipc::parameter_kind::string_array) { return ::rclcpp::ParameterValue{value.string_array_value}; }
        if(value.type==ipc::parameter_kind::byte_array) { return ::rclcpp::ParameterValue{value.byte_array_value}; }
        return {};
    }
    template<typename Container>
    inline std::uint32_t native_array_quota(ros::capability_spec const& cap,ipc::parameter_kind kind,Container const& values)
    {
        auto maximum{limit(cap,"max-parameter-bytes")};
        if(values.size()>limit(cap,"max-parameter-elements")||maximum<4) { return 90; }
        auto remaining{maximum-4};
        if constexpr(std::is_same_v<typename Container::value_type,std::string>)
        {
            for(auto const& text:values)
            {
                if(remaining<4||text.size()>remaining-4) { return 90; }
                if(!ipc::parameter_array_string_valid(ros::version::ros2,text)) { return 22; }
                remaining-=4+text.size();
            }
        }
        else if(values.size()>remaining/ipc::parameter_element_width(kind)) { return 90; }
        return 0;
    }
    template<typename Container>
    inline ipc::call_result array_result(ros::capability_spec const& cap,ipc::parameter_kind kind,Container const& native)
    {
        auto error{native_array_quota(cap,kind,native)};if(error) { return {error,{}}; }
        ipc::parameter_value value;value.type=kind;
        if constexpr(std::is_same_v<typename Container::value_type,std::string>) { value.string_array_value=native; }
        else if constexpr(std::is_same_v<typename Container::value_type,double>) { value.double_array_value=native; }
        else if constexpr(std::is_same_v<typename Container::value_type,std::int64_t>) { value.int64_array_value=native; }
        else
        {
            if(kind==ipc::parameter_kind::boolean_array)
            { value.bool_array_value.reserve(native.size());for(auto item:native) { value.bool_array_value.push_back(item?1:0); } }
            else
            { value.byte_array_value.reserve(native.size());for(auto item:native) { value.byte_array_value.push_back(static_cast<std::uint8_t>(item)); } }
        }
        std::vector<std::uint8_t> bytes;error=ipc::encode_parameter(value,bytes);
        if(!error) { error=ipc::validate_parameter_value(ros::version::ros2,cap,bytes.data(),bytes.size()); }
        return error?ipc::call_result{error,{}}:ipc::call_result{0,std::move(bytes)};
    }
    inline ipc::call_result parameter_string_result(ros::capability_spec const& cap,std::string_view native)
    {
        auto result{bounded_text::result(std::min<std::uint64_t>(limit(cap,"max-parameter-bytes"),ipc::parameter_wire_limit),{native})};
        if(result.error) { return result; }
        auto error{ipc::validate_parameter_value(ros::version::ros2,cap,result.payload.data(),result.payload.size())};
        return error?ipc::call_result{error,{}}:std::move(result);
    }
    inline ipc::call_result parameter_result(ros::capability_spec const& cap,rcl_interfaces::msg::ParameterValue const& native)
    {
        ipc::parameter_value value;value.type=ipc::parameter_type(ros::version::ros2,cap.type);
        if(native.type!=native_kind(value.type)) { return {native.type==rcl_interfaces::msg::ParameterType::PARAMETER_NOT_SET?2u:95u,{}}; }
        if(value.type==ipc::parameter_kind::boolean_array) { return array_result(cap,value.type,native.bool_array_value); }
        if(value.type==ipc::parameter_kind::int64_array) { return array_result(cap,value.type,native.integer_array_value); }
        if(value.type==ipc::parameter_kind::float64_array) { return array_result(cap,value.type,native.double_array_value); }
        if(value.type==ipc::parameter_kind::string_array) { return array_result(cap,value.type,native.string_array_value); }
        if(value.type==ipc::parameter_kind::byte_array) { return array_result(cap,value.type,native.byte_array_value); }
        if(value.type==ipc::parameter_kind::string)
        {
            return parameter_string_result(cap,native.string_value);
        }
        else if(value.type==ipc::parameter_kind::boolean) { value.bool_value=native.bool_value; }
        else if(value.type==ipc::parameter_kind::int64) { value.integer_value=native.integer_value; }
        else if(value.type==ipc::parameter_kind::float64) { value.double_value=native.double_value; }
        else { return {95,{}}; }
        std::vector<std::uint8_t> bytes;auto error{ipc::encode_parameter(value,bytes)};
        if(!error) { error=ipc::validate_parameter_value(ros::version::ros2,cap,bytes.data(),bytes.size()); }
        return error?ipc::call_result{error,{}}:ipc::call_result{0,std::move(bytes)};
    }
    inline ipc::call_result parameter_result(ros::capability_spec const& cap,::rclcpp::ParameterValue const& native)
    {
        auto kind{ipc::parameter_type(ros::version::ros2,cap.type)};
        if(static_cast<std::uint8_t>(native.get_type())!=native_kind(kind)) { return {native.get_type()==::rclcpp::ParameterType::PARAMETER_NOT_SET?2u:95u,{}}; }
        // Use references to the public native array accessors. Copying a full
        // ParameterValue into a generated message here would precede quotas.
        if(kind==ipc::parameter_kind::boolean_array) { return array_result(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_BOOL_ARRAY>()); }
        if(kind==ipc::parameter_kind::int64_array) { return array_result(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY>()); }
        if(kind==ipc::parameter_kind::float64_array) { return array_result(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY>()); }
        if(kind==ipc::parameter_kind::string_array) { return array_result(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_STRING_ARRAY>()); }
        if(kind==ipc::parameter_kind::byte_array) { return array_result(cap,kind,native.get<::rclcpp::ParameterType::PARAMETER_BYTE_ARRAY>()); }
        if(kind==ipc::parameter_kind::string)
        { return parameter_string_result(cap,native.get<::rclcpp::ParameterType::PARAMETER_STRING>()); }
        return parameter_result(cap,native.to_value_msg());
    }
    inline std::timed_mutex& local_parameters_mutex()
    {
        // All authenticated sessions share the gateway node. Serialize every
        // local parameter operation, including type-check plus undeclaration.
        static std::timed_mutex mutex;return mutex;
    }
    inline ipc::call_result result_before_deadline(std::chrono::steady_clock::time_point deadline,ipc::call_result result)
    {
        if(!result.error&&std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
        return result;
    }
    template<typename Service,typename Inspect>
    inline ipc::call_result invoke(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const& node,ros::capability_spec const& cap,std::string const& service,
                                  std::shared_ptr<typename Service::Request> request,std::chrono::steady_clock::time_point deadline,Inspect inspect)
    {
        if constexpr(!ros2_qos::supports_client_qos<::rclcpp::Client<Service>>::value) { return {95,{}}; }
        auto configured{ros2_qos::configured(cap)};
        auto client{ros2_compat::create_client<Service>(node,service,configured)};
        if(!ros2_qos::client(*client,configured)) { return {95,{}}; }
        auto left{deadline-std::chrono::steady_clock::now()};
        if(left<=decltype(left)::zero()||!client->wait_for_service(left)) { return {110,{}}; }
        if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
        auto pending{client->async_send_request(request)};
        if(pending.wait_until(deadline)!=std::future_status::ready) { ros2_compat::remove_pending(*client,pending); return {110,{}}; }
        if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
        return result_before_deadline(deadline,inspect(*pending.get()));
    }
    inline ipc::call_result call(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const& node,ros::capability_spec const& cap,
                                 std::vector<std::uint8_t> const& request,std::uint32_t timeout)
    {
        if(!known(cap)) { return {95,{}}; }
        auto deadline{std::chrono::steady_clock::now()+std::chrono::milliseconds(std::min(timeout,static_cast<std::uint32_t>(limit(cap,"timeout-ms"))))};
        bool graph{cap.operation.rfind("graph.",0)==0};
        auto maximum{limit(cap,graph?"max-graph-result-bytes":"max-parameter-bytes")};
        if(graph)
        {
            if(!request.empty()) { return {22,{}}; }
            bounded_text::sorted_rows rows{maximum};
            auto add=[&](std::string_view name,std::string_view type)->std::uint32_t
            {
                if(std::chrono::steady_clock::now()>=deadline) { return 110; }
                if(!canonical(name)||!prefix(cap.resource,name)||!prefix(field(cap,"allowed-name-prefix"),name)) { return 0; }
                if(cap.operation.rfind("graph.inspect-",0)==0&&name!=cap.resource) { return 0; }
                if(type.find_first_of("\t\r\n")!=std::string_view::npos) { return 0; }
                return rows.add(name,type)?0u:rows.error();
            };
            if(cap.operation=="graph.list-nodes"||cap.operation=="graph.inspect-node")
            { for(auto const& name:node->get_node_names()) { auto error{add(name,"")};if(error) { return {error,{}}; } } }
            else if(cap.operation=="graph.inspect-action")
            {
#if UWVM_ROS2_HAVE_ACTION_GRAPH
                auto names{rcl_get_zero_initialized_names_and_types()}; auto allocator{rcl_get_default_allocator()};
                auto result{rcl_action_get_names_and_types(node->get_node_base_interface()->get_rcl_node_handle(),&allocator,&names)};
                struct cleanup { rcl_names_and_types_t* names; ~cleanup() { auto result=rcl_names_and_types_fini(names); (void)result; } } guard{&names};
                if(result!=RCL_RET_OK) { return {107,{}}; }
                for(std::size_t i{};i<names.names.size;++i)
                { for(std::size_t j{};j<names.types[i].size;++j) { auto error{add(names.names.data[i],names.types[i].data[j])};if(error) { return {error,{}}; } } }
#else
                return {95,{}};
#endif
            }
            else
            {
                auto names{(cap.operation=="graph.list-topics"||cap.operation=="graph.inspect-topic")?node->get_topic_names_and_types():node->get_service_names_and_types()};
                for(auto const& item:names) { for(auto const& type:item.second) { auto error{add(item.first,type)};if(error) { return {error,{}}; } } }
            }
            if(cap.operation.rfind("graph.inspect-",0)==0&&rows.empty()) { return {2,{}}; }
            if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
            return result_before_deadline(deadline,rows.result());
        }
        auto remote{field(cap,"parameter-node")},name{field(cap,"parameter-name")};
        bool write{cap.operation=="param.set"||cap.operation=="param.declare"};
        ipc::parameter_value value;
        if(write)
        { auto error{ipc::validate_parameter_value(ros::version::ros2,cap,request.data(),request.size(),&value)};if(error) { return {error,{}}; } }
        else if(!request.empty()) { return {22,{}}; }
        if(remote==ros2_compat::full_name(node))
        {
            std::unique_lock<std::timed_mutex> guard{local_parameters_mutex(),std::defer_lock};
            if(!guard.try_lock_until(deadline)||std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
            try
            {
#if UWVM_ROS2_HAVE_PARAMETER_DECLARE
                if(cap.operation=="param.declare")
                {
                    if(node->has_parameter(name)) { return {17,{}}; }
                    rcl_interfaces::msg::ParameterDescriptor descriptor;ros2_compat::dynamic_typing(descriptor);
                    auto prepared{native_value(value)};
                    local_parameter_commit_snapshot committed{node,name};
                    if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
                    node->declare_parameter(name,prepared,descriptor);
                    committed.committed();
                    if(committed.owned->ready) { notify_local_parameter_event(node,name,1,committed.owned->value); }
                    return result_before_deadline(deadline,{});
                }
#else
                if(cap.operation=="param.declare"||cap.operation=="param.undeclare") { return {95,{}}; }
#endif
                if(cap.operation=="param.list")
                {
                    auto listed{node->list_parameters({},limit(cap,"list-depth"))};bounded_text::sorted_rows names{maximum};
                    for(auto const& parameter:listed.names)
                    {
                        if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
                        if(parameter_name(parameter))
                        {
                            auto actual{parameter_resource(remote,parameter)};
                            if(prefix(cap.resource,actual)&&prefix(field(cap,"allowed-name-prefix"),actual)&&!names.add(actual)) { return {names.error(),{}}; }
                        }
                    }
                    if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }return result_before_deadline(deadline,names.result());
                }
                if(!node->has_parameter(name)) { return {2,{}}; }
                if(cap.operation=="param.get")
                { return result_before_deadline(deadline,parameter_result(cap,node->get_parameter(name).get_parameter_value())); }
                if(cap.operation=="param.set")
                {
                    if(static_cast<std::uint8_t>(node->get_parameter(name).get_type())!=native_kind(value.type)) { return {95,{}}; }
                    rclcpp::Parameter prepared{name,native_value(value)};
                    local_parameter_commit_snapshot committed{node,name};
                    if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
                    auto result{node->set_parameter(prepared)};
                    if(result.successful) { committed.committed(); }
                    if(result.successful&&committed.owned->ready) { notify_local_parameter_event(node,name,2,committed.owned->value); }
                    return result_before_deadline(deadline,result.successful?ipc::call_result{}:ipc::call_result{13,{}});
                }
#if UWVM_ROS2_HAVE_PARAMETER_DECLARE
                if(cap.operation=="param.undeclare")
                {
                    auto prior{node->get_parameter(name)};
                    auto checked{parameter_result(cap,prior.get_parameter_value())};if(checked.error) { return {checked.error,{}}; }
                    if(std::chrono::steady_clock::now()>=deadline) { return {110,{}}; }
                    node->undeclare_parameter(name);notify_local_parameter_event(node,name,3,prior.get_parameter_value());
                    return result_before_deadline(deadline,{});
                }
#endif
                if(cap.operation=="param.describe")
                {
                    auto descriptor{node->describe_parameter(name)};return result_before_deadline(deadline,descriptor_result(descriptor,maximum));
                }
            }
#if UWVM_ROS2_HAVE_PARAMETER_DECLARE
            catch(::rclcpp::exceptions::ParameterNotDeclaredException const&) { return {2,{}}; }
            catch(::rclcpp::exceptions::ParameterAlreadyDeclaredException const&) { return {17,{}}; }
            catch(::rclcpp::exceptions::InvalidParameterTypeException const&) { return {95,{}}; }
            catch(::rclcpp::exceptions::ParameterImmutableException const&) { return {13,{}}; }
#else
            catch(std::out_of_range const&) { return {2,{}}; }
#endif
        }
        if(cap.operation=="param.declare"||cap.operation=="param.undeclare")
        {
            // ROS2 has no remote declaration service. Official deletion uses
            // SetParameters/NOT_SET for dynamic parameters, but it has no
            // conditional prior-kind/value operation. This typed adapter does
            // not offer remote deletion without those atomic preconditions.
            return {95,{}};
        }
        if(cap.operation=="param.get")
        {
            auto query{std::make_shared<rcl_interfaces::srv::GetParameters::Request>()}; query->names={name};
            return invoke<rcl_interfaces::srv::GetParameters>(node,cap,remote+"/get_parameters",query,deadline,[&](auto const& response) {
                if(response.values.empty()) { return ipc::call_result{2,{}}; }
                if(response.values.size()!=1) { return ipc::call_result{22,{}}; }
                return parameter_result(cap,response.values[0]);
            });
        }
        if(cap.operation=="param.set")
        {
            if(value.type!=ipc::parameter_kind::string)
            {
                // Generated SetParameters does not conditionally enforce the
                // previous type for dynamic parameters. Typed writes require an
                // existing exact static descriptor; an uninitialized static
                // parameter may have no GetParameters value. Its kind must
                // still be established by DescribeParameters. Every preflight
                // shares the original deadline and explicitly configured QoS.
                auto get{std::make_shared<rcl_interfaces::srv::GetParameters::Request>()};get->names={name};
                auto checked{invoke<rcl_interfaces::srv::GetParameters>(node,cap,remote+"/get_parameters",get,deadline,[&](auto const& response) {
                    if(response.values.empty()) { return ipc::call_result{}; }
                    if(response.values.size()!=1) { return ipc::call_result{22,{}}; }
                    auto kind{response.values[0].type};
                    return ipc::call_result{kind==rcl_interfaces::msg::ParameterType::PARAMETER_NOT_SET||kind==native_kind(value.type)?0u:95u,{}};
                })};if(checked.error) { return checked; }
                auto describe{std::make_shared<rcl_interfaces::srv::DescribeParameters::Request>()};describe->names={name};
                checked=invoke<rcl_interfaces::srv::DescribeParameters>(node,cap,remote+"/describe_parameters",describe,deadline,[&](auto const& response) {
                    if(response.descriptors.empty()) { return ipc::call_result{95,{}}; }
                    if(response.descriptors.size()!=1||response.descriptors[0].name!=name) { return ipc::call_result{22,{}}; }
                    auto const& descriptor{response.descriptors[0]};return ipc::call_result{!ros2_compat::is_dynamic_typing(descriptor)&&descriptor.type==native_kind(value.type)?0u:95u,{}};
                });if(checked.error) { return checked; }
            }
            auto query{std::make_shared<rcl_interfaces::srv::SetParameters::Request>()};rcl_interfaces::msg::Parameter p;p.name=name;p.value=native_value(value).to_value_msg();query->parameters.push_back(std::move(p));
            return invoke<rcl_interfaces::srv::SetParameters>(node,cap,remote+"/set_parameters",query,deadline,[](auto const& response) {
                if(response.results.size()!=1) { return ipc::call_result{22,{}}; } return response.results[0].successful?ipc::call_result{}:ipc::call_result{13,{}};
            });
        }
        if(cap.operation=="param.describe")
        {
            auto query{std::make_shared<rcl_interfaces::srv::DescribeParameters::Request>()}; query->names={name};
            return invoke<rcl_interfaces::srv::DescribeParameters>(node,cap,remote+"/describe_parameters",query,deadline,[&](auto const& response) {
                if(response.descriptors.size()!=1||response.descriptors[0].name!=name) { return ipc::call_result{22,{}}; }
                auto const& descriptor{response.descriptors[0]};
                // Return exact parameter metadata, not neighboring parameter names.
                return descriptor_result(descriptor,maximum);
            });
        }
        if(cap.operation=="param.list")
        {
            auto query{std::make_shared<rcl_interfaces::srv::ListParameters::Request>()}; query->prefixes={}; query->depth=limit(cap,"list-depth");
            return invoke<rcl_interfaces::srv::ListParameters>(node,cap,remote+"/list_parameters",query,deadline,[&](auto const& response) {
                bounded_text::sorted_rows names{maximum};
                for(auto const& parameter:response.result.names)
                {
                    if(std::chrono::steady_clock::now()>=deadline) { return ipc::call_result{110,{}}; }
                    if(parameter_name(parameter))
                    {
                        auto actual{parameter_resource(remote,parameter)};
                        if(prefix(cap.resource,actual)&&prefix(field(cap,"allowed-name-prefix"),actual)&&!names.add(actual)) { return ipc::call_result{names.error(),{}}; }
                    }
                }
                if(std::chrono::steady_clock::now()>=deadline) { return ipc::call_result{110,{}}; }return names.result();
            });
        }
        return {95,{}};
    }
}
