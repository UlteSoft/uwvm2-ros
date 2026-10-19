#include "common.h"
#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <set>

// Execute this probe only on Linux in the established 64-GiB cgroup. It starts
// the real trusted Gateway and uses independently authenticated IPC sessions;
// no test callback invokes the production local-event helper directly.
namespace
{
    using namespace local_event_probe;
    struct observer
    {
        std::shared_ptr<rclcpp::Node> node;
        std::shared_ptr<rclcpp::Subscription<rcl_interfaces::msg::ParameterEvent>> events;
        rclcpp::executors::SingleThreadedExecutor executor;
        std::atomic<bool> stop{},failed{};std::atomic<unsigned> gateway_events{};std::thread spinner;
        observer()
        {
            rclcpp::NodeOptions options;options.enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            node=std::make_shared<rclcpp::Node>("observer",namespace_name,options);
            events=node->create_subscription<rcl_interfaces::msg::ParameterEvent>("/parameter_events",rclcpp::QoS(32).reliable().durability_volatile(),
                [this](rcl_interfaces::msg::ParameterEvent::ConstSharedPtr event) { if(event->node==gateway_name) { ++gateway_events; } });
            executor.add_node(node);spinner=std::thread{[this] {
                try { while(!stop&&rclcpp::ok()) { executor.spin_once(std::chrono::milliseconds(10)); } }catch(...) { failed=true; }
            }};
        }
        ~observer() { stop=true;executor.cancel();if(spinner.joinable()) { spinner.join(); } }
        std::set<std::string> writer_ids()
        {
            std::set<std::string> result;
            for(auto const& endpoint:node->get_publishers_info_by_topic("/parameter_events"))
            {
                auto const& bytes{endpoint.endpoint_gid()};result.insert(gateway::native_io::hexadecimal(bytes.data(),bytes.size()));
                require(endpoint.node_name()!="gateway"||endpoint.node_namespace()!=namespace_name,"Gateway creates no native ParameterEvent writer");
            }
            return result;
        }
        void visible()
        {
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds(8)};
            for(;;)
            {
                auto names{node->get_node_names()};if(std::find(names.begin(),names.end(),gateway_name)!=names.end()) { return; }
                require(!failed&&std::chrono::steady_clock::now()<until,"actual Gateway graph discovery");std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    };
    void configure(gateway::configuration& config)
    {
        config.policy.connection["node-name"]="gateway";config.policy.connection["namespace"]=namespace_name;
        config.policy.connection["domain-id"]="203";config.policy.capabilities.clear();
        for(std::string const kind:kinds)
        {
            auto name{name_of(kind)};
            for(std::string operation:{"declare","set","get","undeclare"})
            { add(config,parameter_cap(kind,kind+"_"+operation,"param."+operation,name)); }
            add(config,event_cap(kind,kind+"_event",name));add(config,event_cap("deletion",kind+"_deletion",name));
            add(config,event_cap(kind,kind+"_inactive",name));
            auto bytes{event_cap(kind,kind+"_bytes",name)};bytes.limits["max-parameter-bytes"]="1";add(config,std::move(bytes));
            auto other{event_cap(kind,kind+"_node",name)};other.limits["parameter-node"]="/local_parameter_events/other";
            other.resource=fast_io::concat_std("/local_parameter_events/other/",name);add(config,std::move(other));
            add(config,event_cap(kind,kind+"_name","unselected"));
            add(config,event_cap(kind=="bool"?"string":"bool",kind+"_type",name));
            if(ipc::numeric_parameter(sample(kind).type))
            {
                auto bounds{event_cap(kind,kind+"_bounds",name)};bounds.limits["parameter-min-value"]="0";bounds.limits["parameter-max-value"]="0";add(config,std::move(bounds));
            }
            if(ipc::array_parameter(sample(kind).type))
            { auto elements{event_cap(kind,kind+"_elements",name)};elements.limits["max-parameter-elements"]="1";add(config,std::move(elements)); }
        }
        auto legacy{event_cap("string","legacy",name_of("string"))};legacy.type="rcl_interfaces/msg/ParameterEvent";
        legacy.type_identity="RIHS01_043e627780fcad87a22d225bc2a037361dba713fca6a6b9f4b869a5aa0393204";add(config,std::move(legacy));
        for(std::string const overflow:{"drop-oldest","drop-newest","reject"})
        {
            auto cap{event_cap("int64",overflow,name_of("int64"))};cap.limits["max-event-queue-depth"]="2";
            cap.limits["max-event-queue-bytes"]=fast_io::concat_std(2*expected(name_of("int64"),sample("int64"),2,1,1).size());
            cap.limits["max-parameter-bytes"]=fast_io::concat_std(expected(name_of("int64"),sample("int64"),2,1,1).size());
            cap.limits["overflow-policy"]=overflow;add(config,std::move(cap));
        }
        for(auto const& cap:config.policy.capabilities) { gateway::validate(core::version::ros2,cap.second); }
    }
    void activate(ipc::client& client,std::string const& alias)
    { require(!client.call("param.subscribe-events",alias,{},2000).error,"explicit event activation"); }
    void legacy_receive(ipc::client& client,ipc::parameter_value const& value,unsigned kind)
    {
        auto result{client.call("event.poll","legacy",{},1000)};uwvm_ros::parameter_event event;
        require(!result.error&&uwvm_ros::decode(result.payload,event),"legacy String event remains decodable");
        require(event.node==gateway_name&&event.name==name_of("string")&&event.kind==kind&&event.value==(kind==3?std::string{}:value.string_value),"legacy String exact selected content");empty(client,"legacy");
    }
}
int main(int argc,char** argv)
{
    using namespace local_event_probe;
    try
    {
        require(argc==2,"usage local_parameter_events_native_ros2 <Gateway executable>");
        require(std::getenv("ROS_DOMAIN_ID")&&std::string_view{std::getenv("ROS_DOMAIN_ID")}=="203","isolated explicit domain203");
        int ros_argc{1};rclcpp::init(ros_argc,argv);unsigned complete{},rejections{},retirement{},overflow_checks{};
        {
            observer native;fixture session{core::version::ros2,argv[1],configure};native.visible();auto writers{native.writer_ids()};
            ipc::client sender,first,second;session.connect(sender);session.connect(first);session.connect(second);
            for(std::string const kind:kinds)
            {
                auto name{name_of(kind)},alias{kind+"_event"},deleted{kind+"_deletion"};activate(first,alias);activate(second,alias);activate(first,deleted);
                std::vector<std::string> negative{kind+"_bytes",kind+"_node",kind+"_name",kind+"_type"};
                if(ipc::numeric_parameter(sample(kind).type)) { negative.push_back(kind+"_bounds"); }
                if(ipc::array_parameter(sample(kind).type)) { negative.push_back(kind+"_elements"); }
                for(auto const& selected:negative) { activate(first,selected); }
                if(kind=="string") { activate(first,"legacy"); }
                for(unsigned phase:{0u,1u,2u})
                {
                    auto value{sample(kind,phase?1:0)};auto operation{phase==0?"declare":phase==1?"set":"undeclare"};
                    require(!sender.call(fast_io::concat_std("param.",fast_io::mnp::os_c_str(operation)),kind+"_"+operation,phase==2?std::vector<std::uint8_t>{}:request(value),2000).error,"actual local native mutation committed");
                    auto one{receive(first,alias,name,value,phase+1)},two{receive(second,alias,name,value,phase+1)};
                    require(one==two,"same shared-Node mutation has identical bytes in two independent sessions");++complete;
                    if(phase==2) { receive(first,deleted,name,value,3,true); }else { empty(first,deleted); }
                    for(auto const& selected:negative) { empty(first,selected);++rejections; }
                    auto inactive{first.call("event.poll",kind+"_inactive",{},1000)};require(inactive.error==9&&inactive.payload.empty(),"grant presence alone never activates a route");
                    auto absent{first.call("event.poll","ungranted",{},1000)};require(absent.error==13&&absent.payload.empty(),"absent authority rejected");
                    if(kind=="string") { legacy_receive(first,value,phase+1); }
                }
                for(auto const& selected:negative) { require(!first.call("capability.close",selected,{},1000).error,"negative alias closes"); }
                require(!first.call("capability.close",alias,{},1000).error&&!second.call("capability.close",alias,{},1000).error,"session-local event aliases close");
                require(!first.call("capability.close",deleted,{},1000).error,"deletion alias closes");if(kind=="string") { require(!first.call("capability.close","legacy",{},1000).error,"legacy alias closes"); }
            }
            // Close and revoke affect only their session's alias, including
            // when another session uses the identical authorized alias.
            auto name{name_of("bool")};activate(first,"bool_event");activate(second,"bool_event");
            require(!sender.call("param.declare","bool_declare",request(sample("bool")),2000).error,"retirement control declaration");
            receive(first,"bool_event",name,sample("bool"),1);receive(second,"bool_event",name,sample("bool"),1);
            require(!first.call("capability.close","bool_event",{},1000).error,"one owner's close succeeds");
            require(!sender.call("param.set","bool_set",request(sample("bool",1)),2000).error,"sibling mutation after close");receive(second,"bool_event",name,sample("bool",1),2);
            auto closed{first.call("event.poll","bool_event",{},1000)};require(closed.error==9&&closed.payload.empty(),"closed owner has no queue");++retirement;
            activate(first,"bool_event");require(!first.call("capability.revoke","bool_event",{},1000).error,"one owner's explicit revoke succeeds");
            require(!sender.call("param.set","bool_set",request(sample("bool")),2000).error,"sibling mutation after revoke");receive(second,"bool_event",name,sample("bool"),2);
            auto revoked{first.call("event.poll","bool_event",{},1000)};require(revoked.error==125&&revoked.payload.empty(),"revoked owner remains denied");++retirement;
            second.close();std::this_thread::sleep_for(std::chrono::milliseconds(100));session.connect(second);activate(second,"bool_event");empty(second,"bool_event");
            require(!sender.call("param.set","bool_set",request(sample("bool",1)),2000).error,"new session mutation");receive(second,"bool_event",name,sample("bool",1),2);++retirement;
            require(!sender.call("param.undeclare","bool_undeclare",{},2000).error,"retirement control cleanup");receive(second,"bool_event",name,sample("bool",1),3);
            // Three listeners share identical ordered mutations. Their only
            // different field is the already supported overflow policy.
            require(!sender.call("param.declare","int64_declare",request(sample("int64")),2000).error,"overflow control declaration");
            for(std::string const policy:{"drop-oldest","drop-newest","reject"}) { activate(second,policy); }
            for(unsigned index=1;index<=6;++index)
            { auto value{sample("int64")};value.integer_value=index;require(!sender.call("param.set","int64_set",request(value),2000).error,"ordered overflow mutation"); }
            for(std::string const policy:{"drop-oldest","drop-newest","reject"})
            {
                unsigned first_value{policy=="drop-oldest"?5u:1u};
                for(unsigned index=first_value;index<=first_value+1;++index)
                {
                    auto result{second.call("event.poll",policy,{},1000)};uwvm_ros::typed_parameter_event event;auto value{sample("int64")};value.integer_value=index;
                    require(!result.error&&uwvm_ros::decode(result.payload,event)&&result.payload==expected(name_of("int64"),value,2,event.seconds,event.nanoseconds),"overflow retains exact selected ordered events");
                }
                empty(second,policy);require(!second.call("capability.close",policy,{},1000).error,"overflow alias closed");++overflow_checks;
            }
            require(!sender.call("param.undeclare","int64_undeclare",{},2000).error,"overflow control cleanup");
            // This is a bounded observation, accompanied by actual graph GIDs;
            // it does not claim an unlimited proof about external publishers.
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds(1)};
            while(std::chrono::steady_clock::now()<until)
            { require(native.writer_ids()==writers&&!native.gateway_events&&!native.failed,"local events add no native writer or observed Gateway DDS traffic");std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
            fast_io::io::println("PASS local ROS2 parameter events complete_mutations=",complete," filtered_checks=",rejections," retirements=",retirement," overflow_policies=",overflow_checks,
                " exact SDK envelopes; two authenticated sessions; nine kinds; tag0 and legacy; no native ParameterEvent writer; observed_gateway_dds_events=",native.gateway_events.load());
        }
        rclcpp::shutdown();return 0;
    }
    catch(std::exception const& error) { fast_io::io::perrln(fast_io::mnp::os_c_str(error.what()));if(rclcpp::ok()) { rclcpp::shutdown(); }return 1; }
}
