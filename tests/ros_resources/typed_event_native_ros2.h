#pragma once
#include "typed_event_witness_ros2.h"

inline void ros2_native_typed_events(rclcpp::Node::SharedPtr const& node,char const* executable)
{
    using namespace typed_event_probe;
    auto publisher{node->create_publisher<rcl_interfaces::msg::ParameterEvent>("/parameter_events",
        rclcpp::QoS{rclcpp::KeepLast{32}}.reliable().durability_volatile())};
    fixture session{core::version::ros2,executable,[](gateway::configuration& config) {
        config.policy.connection["domain-id"]="75";config.policy.connection["node-name"]="uwvm_typed_events_gateway_probe";
        config.policy.capabilities.clear();
        for(std::string const kind:{"bool","int64","float64"})
        {
            add(config,capability(kind,kind,kind));
            add(config,capability(kind,kind+"_auto","typed_auto_"+kind));
            auto tiny{capability(kind,kind+"_tiny","quota_"+kind)};tiny.limits["max-parameter-bytes"]="1";add(config,std::move(tiny));
            for(std::string const overflow:{"drop-oldest","drop-newest","reject"})
            {
                auto name{queue_name(kind,overflow)};auto cap{capability(kind,kind+"_"+overflow,name)};
                cap.limits["max-event-queue-depth"]="2";
                cap.limits["burst"]="2";
                auto envelope{23+std::string{"/uwvm_resources/native"}.size()+name.size()+ipc::parameter_width(scalar_kind(core::version::ros2,kind))};
                cap.limits["max-parameter-bytes"]=std::to_string(envelope);
                cap.limits["max-event-queue-bytes"]=std::to_string(2*envelope);
                cap.limits["overflow-policy"]=overflow;add(config,std::move(cap));
            }
            if(kind!="bool")
            {
                auto narrow{capability(kind,kind+"_narrow","narrow_"+kind)};
                narrow.limits["parameter-min-value"]=kind=="int64"?"-9007199254740995":"-2";
                narrow.limits["parameter-max-value"]=kind=="int64"?"9007199254740995":"2";add(config,std::move(narrow));
            }
        }
    }};
    ipc::client client;session.connect(client);
    reader_observation readers{node,publisher,"/uwvm_test/uwvm_typed_events_gateway_probe"};readers.capture_baseline(true);
    auto matched=[&](bool expected) {
        readers.wait_projection(expected?1:0);
    };
    auto subscribe=[&](std::string const& alias) {
        matched(false);require(!client.call("param.subscribe-events",alias,{},2500).error,"typed-event subscription explicitly authorized");matched(true);
    };
    auto close=[&](std::string const& alias) { require(!client.call("capability.close",alias,{},1000).error,"typed-event alias closed");matched(false); };
    auto publish=[&](rcl_interfaces::msg::ParameterEvent const& message) { publisher->publish(message);std::this_thread::sleep_for(std::chrono::milliseconds(25)); };
    auto receive=[&](std::string const& alias,std::vector<std::uint8_t> const& expected) {
        auto until{std::chrono::steady_clock::now()+std::chrono::seconds(2)};
        while(std::chrono::steady_clock::now()<until)
        {
            auto result{client.call("event.poll",alias,{},1000)};
            if(!result.error)
            {
                ipc::typed_parameter_event decoded;require(!ipc::decode_typed_parameter_event(result.payload,decoded)&&result.payload==expected,
                    "actual typed-event complete canonical bytes include native exact kind/value/stamp");return;
            }
            require(result.error==11&&result.payload.empty(),"empty typed-event queue exposes no bytes");
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        throw std::runtime_error{"native typed-event exact receipt deadline"};
    };
    auto absent=[&](std::string const& alias) {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));auto result{client.call("event.poll",alias,{},1000)};
        require(result.error==11&&result.payload.empty(),"unselected/malformed/over-budget native typed event is not disclosed");
    };
    for(std::string const kind:{"bool","int64","float64"})
    {
        subscribe(kind);
        unsigned stamp{1};
        for(auto const item:std::vector<std::pair<unsigned,std::uint8_t>>{{0,1},{1,2},{2,2},{2,3}})
        { auto native_value{value(kind,item.first)};publish(event(kind,native_value,item.second,stamp));receive(kind,bytes(kind,native_value,item.second,stamp++)); }
        if(kind=="float64")
        {
            for(double exact:{std::numeric_limits<double>::min(),-std::numeric_limits<double>::min()})
            { auto selected{value(kind,1)};selected.double_value=exact;publish(event(kind,selected,2,stamp));receive(kind,bytes(kind,selected,2,stamp++)); }
        }
        for(std::string const filter:{"wrong-node","wrong-name","wrong-kind","not-set","fields","stamp"})
        {
            auto message{event(kind,value(kind,1),filter=="not-set"?3:2,40)};
            auto& selected{filter=="not-set"?message.deleted_parameters.front():message.changed_parameters.front()};
            if(filter=="wrong-node") { message.node="/uwvm_resources/other"; }
            if(filter=="wrong-name") { selected.name="other"; }
            if(filter=="wrong-kind") { selected.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_STRING;selected.value.string_value="unselected"; }
            if(filter=="not-set") { selected.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_NOT_SET; }
            if(filter=="fields") { message.new_parameters.push_back(native("other",value(kind,1))); }
            if(filter=="stamp") { message.stamp.nanosec=1000000000U; }
            publish(message);absent(kind);
        }
        if(kind=="float64")
        {
            for(double invalid:{std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity()})
            {
                auto selected{value(kind,1)};selected.double_value=invalid;
                for(std::uint8_t event_kind:{2,3}) { publish(event(kind,selected,event_kind,41));absent(kind); }
            }
        }
        // The same reader still handles a valid marker after discarded input.
        publish(event(kind,value(kind,1),2,49));receive(kind,bytes(kind,value(kind,1),2,49));close(kind);
        if(kind!="bool")
        {
            auto alias{kind+"_narrow"},name{"narrow_"+kind};subscribe(alias);
            for(bool upper:{false,true})
            {
                auto invalid{value(kind,1)};invalid.integer_value=upper?9007199254740996LL:-9007199254740996LL;invalid.double_value=upper?3.0:-3.0;
                for(std::uint8_t event_kind:{1,2,3}) { publish(event(name,invalid,event_kind,50));absent(alias); }
            }
            publish(event(name,value(kind,1),2,59));receive(alias,bytes(name,value(kind,1),2,59));close(alias);
        }
        auto tiny_alias{kind+"_tiny"},tiny_name{"quota_"+kind};subscribe(tiny_alias);
        for(std::uint8_t event_kind:{1,2,3}) { publish(event(tiny_name,value(kind,1),event_kind,60));absent(tiny_alias); }
        close(tiny_alias);
        for(std::string const overflow:{"drop-oldest","drop-newest","reject"})
        {
            auto alias{kind+"_"+overflow},name{queue_name(kind,overflow)};subscribe(alias);
            for(unsigned n{1};n<=20;++n) { publish(event(name,value(kind,1),2,n)); }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto first{overflow=="drop-oldest"?19u:1u};
            receive(alias,bytes(name,value(kind,1),2,first));receive(alias,bytes(name,value(kind,1),2,first+1));absent(alias);close(alias);
        }
        // In addition to controlled generated messages, exercise the official
        // Node set_parameter automatic event path. Its native timestamp varies.
        auto automatic_name{"typed_auto_"+kind},automatic_alias{kind+"_auto"};
        rcl_interfaces::msg::ParameterDescriptor descriptor;descriptor.dynamic_typing=true;
        node->declare_parameter(automatic_name,ros2_scalar_native(value(kind,0)),descriptor);
        subscribe(automatic_alias);
        bool automatic{};auto until{std::chrono::steady_clock::now()+std::chrono::seconds(2)};
        while(!automatic&&std::chrono::steady_clock::now()<until)
        {
            auto changed{value(kind,1)};require(node->set_parameter(rclcpp::Parameter{automatic_name,ros2_scalar_native(changed)}).successful,
                "official native typed parameter mutation succeeded");
            std::this_thread::sleep_for(std::chrono::milliseconds(25));auto result{client.call("event.poll",automatic_alias,{},1000)};
            if(!result.error)
            {
                ipc::typed_parameter_event decoded;
                require(!ipc::decode_typed_parameter_event(result.payload,decoded)&&decoded.node=="/uwvm_resources/native"&&
                    decoded.name==automatic_name&&decoded.kind==2&&decoded.value.type==changed.type&&
                    scalar_bytes(decoded.value)==scalar_bytes(changed),"official Node automatic typed changed event exact kind/value and valid native timestamp");
                require(node->get_parameter(automatic_name).get_parameter_value()==ros2_scalar_native(changed),
                    "actual native typed value agrees with delivered automatic event");automatic=true;
            }
            else { require(result.error==11&&result.payload.empty(),"automatic typed event discovery wait exposes no bytes"); }
        }
        require(automatic,"official Node automatic scalar event receipt before deadline");close(automatic_alias);node->undeclare_parameter(automatic_name);
    }
    client.close();
    std::cout<<"PASS actual typed ParameterEvent Bool/int64/Float64 new/change/delete; exact extrema/float bits; node/name/native-kind/NOT_SET/stamp/fields/finite/bounds/envelope filters;9 explicit queue overflow contracts and native reader cleanup\n";
}
