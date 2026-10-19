#include "common_ros2.h"

int main(int argc,char** argv) {
    using namespace expanded_event_probe;
    try {
        require(argc==2,"usage expanded native event probe gateway");
        auto domain=::getenv("ROS_DOMAIN_ID");require(domain&&std::string_view{domain}=="191","expanded event probe isolated domain191");
        int ros_argc=1;rclcpp::init(ros_argc,argv);
        auto node=std::make_shared<rclcpp::Node>("native","/typed_events");
        auto publisher=node->create_publisher<rcl_interfaces::msg::ParameterEvent>("/parameter_events",rclcpp::QoS{32}.reliable().durability_volatile());
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
        std::atomic<bool> stopped{},spin_failed{};std::thread spinner{[&] {
            try { while(!stopped&&rclcpp::ok()) executor.spin_once(std::chrono::milliseconds(10)); }
            catch(...) { spin_failed=true; }
        }};
        auto cleanup=[&] { stopped=true;executor.cancel();spinner.join();publisher.reset();rclcpp::shutdown(); };
        try {
            fixture session{core::version::ros2,argv[1],[](gateway::configuration& config) {
                config.policy.connection["domain-id"]="191";config.policy.connection["node-name"]="expanded_events_gateway";
                config.policy.connection["namespace"]="/typed_events";config.policy.capabilities.clear();
                for(std::string kind:kinds) {
                    auto name=kind;std::replace(name.begin(),name.end(),'-','_');
                    add(config,capability(kind,kind,name));add(config,capability(kind,fast_io::concat_std(kind,"_auto"),fast_io::concat_std("auto_",name)));
                    if(kind=="int64-array") {
                        auto narrow=capability(kind,"narrow","narrow");narrow.limits["parameter-min-value"]="-9007199254740995";
                        narrow.limits["parameter-max-value"]="9007199254740995";add(config,std::move(narrow));
                    }
                    auto envelope=bytes(name,value(kind),2,1).size();
                    auto bounded=capability(kind,fast_io::concat_std(kind,"_bytes"),fast_io::concat_std(name,"_bytes"));
                    bounded.limits["max-parameter-bytes"]=fast_io::concat_std(envelope+6);if(kind=="byte-array") bounded.limits["max-parameter-elements"]="5";add(config,std::move(bounded));
                    for(std::string overflow:{"drop-oldest","drop-newest","reject"}) {
                        auto queue_name=fast_io::concat_std("queue_",name,"_",overflow);std::replace(queue_name.begin(),queue_name.end(),'-','_');
                        auto queue=capability(kind,queue_name,queue_name);auto exact=bytes(queue_name,value(kind,2),2,1).size();
                        queue.limits["max-event-queue-depth"]="2";queue.limits["max-event-queue-bytes"]=fast_io::concat_std(2*exact);
                        queue.limits["max-parameter-bytes"]=fast_io::concat_std(exact);queue.limits["overflow-policy"]=overflow;
                        queue.limits["burst"]="2";add(config,std::move(queue));
                    }
                }
            }};
            ipc::client client;session.connect(client);
            typed_event_probe::reader_observation readers{node,publisher,"/typed_events/expanded_events_gateway"};readers.capture_baseline(false);
            auto subscribe=[&](std::string const& alias) {
                readers.wait_projection(0);require(!client.call("param.subscribe-events",alias,{},2500).error,"expanded event reader authorized");readers.wait_projection(1);
            };
            auto close=[&](std::string const& alias) { require(!client.call("capability.close",alias,{},1000).error,"expanded event last alias closed");readers.wait_projection(0); };
            auto receive=[&](std::string const& alias,std::vector<std::uint8_t> const& expected) {
                auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(3);
                while(std::chrono::steady_clock::now()<deadline) {
                    auto result=client.call("event.poll",alias,{},1000);
                    if(!result.error) {
                        if(result.payload!=expected) {
                            fast_io::io::perrln("MISMATCH alias=",alias," expected_bytes=",expected.size()," actual_bytes=",result.payload.size());
                            fast_io::io::perr("EXPECTED hex=");
                            for(auto byte:expected) fast_io::io::perr(fast_io::mnp::hex0x(static_cast<unsigned>(byte)),",");
                            fast_io::io::perrln("");fast_io::io::perr("ACTUAL hex=");
                            for(auto byte:result.payload) fast_io::io::perr(fast_io::mnp::hex0x(static_cast<unsigned>(byte)),",");
                            fast_io::io::perrln("");
                        }
                        require(result.payload==expected,"entire expanded native event agrees with independent SDK wire oracle");return;
                    }
                    require(result.error==11&&result.payload.empty(),"waiting expanded event has EAGAIN and no payload");std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
                throw std::runtime_error{"expanded event exact receipt deadline"};
            };
            unsigned positive{},rejection_batches{},automatic{},overflow_contracts{};
            for(std::string kind:kinds) {
                auto name=kind;std::replace(name.begin(),name.end(),'-','_');subscribe(kind);
                for(unsigned phase:{0u,1u}) for(unsigned event_kind:{1u,2u,3u}) {
                    auto selected=value(kind,phase);publisher->publish(event(name,selected,event_kind,10+positive));
                    receive(kind,bytes(name,selected,event_kind,10+positive));++positive;
                }
                for(auto const& scenario:scenarios(kind)) {
                    if(scenario=="bounds"&&kind=="int64-array") continue;
                    // A selected valid marker from the same ordered writer
                    // follows rejected inputs; any disclosure appears before it.
                    for(unsigned event_kind:{1u,2u,3u}) {
                        publisher->publish(rejected(kind,name,scenario,event_kind));
                        if(scenario=="bounds"||scenario=="nonfinite") {
                            auto bad=rejected(kind,name,scenario,event_kind);
                            auto& selected=(event_kind==1?bad.new_parameters:event_kind==2?bad.changed_parameters:bad.deleted_parameters).front();
                            if(scenario=="bounds") selected.value.double_array_value.back()=-3.;
                            else selected.value.double_array_value.back()=std::numeric_limits<double>::quiet_NaN();
                            publisher->publish(bad);
                        }
                    }
                    auto selected=value(kind,2);publisher->publish(event(name,selected,2,200+rejection_batches));
                    receive(kind,bytes(name,selected,2,200+rejection_batches));
                    auto empty=client.call("event.poll",kind,{},1000);require(empty.error==11&&empty.payload.empty(),"rejected native batch leaves no queued disclosure after its ordered marker");++rejection_batches;
                }
                close(kind);
                if(kind=="int64-array") {
                    subscribe("narrow");for(unsigned event_kind:{1u,2u,3u}) {
                        publisher->publish(rejected(kind,"narrow","bounds",event_kind));auto bad=rejected(kind,"narrow","bounds",event_kind);
                        auto& selected=(event_kind==1?bad.new_parameters:event_kind==2?bad.changed_parameters:bad.deleted_parameters).front();
                        selected.value.integer_array_value.back()=-9007199254740996LL;publisher->publish(bad);
                    }
                    auto selected=value(kind,2);publisher->publish(event("narrow",selected,2,500));receive("narrow",bytes("narrow",selected,2,500));++rejection_batches;close("narrow");
                }
                auto bounded_alias=fast_io::concat_std(kind,"_bytes"),bounded_name=fast_io::concat_std(name,"_bytes");subscribe(bounded_alias);
                auto large=value(kind);large.string_value.append(1,'x');large.byte_array_value.push_back(1);large.bool_array_value.push_back(1);
                large.int64_array_value.push_back(1);large.double_array_value.push_back(1);large.string_array_value.push_back("");
                for(unsigned event_kind:{1u,2u}) publisher->publish(event(bounded_name,large,event_kind,600));
                auto selected=value(kind);publisher->publish(event(bounded_name,selected,2,601));receive(bounded_alias,bytes(bounded_name,selected,2,601));++rejection_batches;
                publisher->publish(event(bounded_name,large,3,602));receive(bounded_alias,bytes(bounded_name,large,3,602));++positive;close(bounded_alias);
                for(std::string overflow:{"drop-oldest","drop-newest","reject"}) {
                    auto queue_name=fast_io::concat_std("queue_",name,"_",overflow);std::replace(queue_name.begin(),queue_name.end(),'-','_');subscribe(queue_name);
                    auto selected=value(kind,2);
                    for(unsigned stamp=1;stamp<=20;++stamp) { publisher->publish(event(queue_name,selected,2,stamp));std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    auto first=overflow=="drop-oldest"?19u:1u;
                    receive(queue_name,bytes(queue_name,selected,2,first));receive(queue_name,bytes(queue_name,selected,2,first+1));
                    auto empty=client.call("event.poll",queue_name,{},1000);require(empty.error==11&&empty.payload.empty(),"expanded exact byte/depth queue contains precisely two retained events");
                    positive+=2;++overflow_contracts;close(queue_name);
                }
                auto auto_alias=fast_io::concat_std(kind,"_auto"),auto_name=fast_io::concat_std("auto_",name);subscribe(auto_alias);
                rcl_interfaces::msg::ParameterDescriptor descriptor;descriptor.dynamic_typing=true;
                node->declare_parameter(auto_name,native(value(kind,0)),descriptor);
                // Native stamps are intentionally checked separately from
                // controlled publisher stamps by the SDK decoder.
                auto drain_auto=[&](unsigned expected_kind,ipc::parameter_value const& expected) {
                    auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(3);
                    while(std::chrono::steady_clock::now()<deadline) {
                        auto result=client.call("event.poll",auto_alias,{},1000);
                        if(!result.error) {
                            uwvm_ros::typed_parameter_event decoded;require(uwvm_ros::decode(result.payload,decoded),"official Node event canonical timestamp and shape");
                            auto exact=bytes(auto_name,expected,expected_kind,decoded.nanoseconds);for(unsigned i=0;i<4;++i) exact[i]=static_cast<std::uint8_t>(decoded.seconds>>(8*i));
                            require(decoded.node==node_name&&decoded.name==auto_name&&decoded.kind==expected_kind&&result.payload==exact,"official Node automatic expanded event exact full value");return;
                        }
                        require(result.error==11&&result.payload.empty(),"official Node automatic wait empty");std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    }
                    throw std::runtime_error{"official Node automatic expanded event deadline"};
                };
                drain_auto(1,value(kind,0));require(node->set_parameter(rclcpp::Parameter{auto_name,native(value(kind))}).successful,"official native expanded parameter mutation succeeds");
                drain_auto(2,value(kind));require(node->get_parameter(auto_name).get_parameter_value()==native(value(kind)),"actual native expanded value retained exact");
                ++automatic;close(auto_alias);node->undeclare_parameter(auto_name);
            }
            client.close();require(!spin_failed,"expanded event native executor retained");
            fast_io::io::println("PASS expanded ROS2 ParameterEvents native positive=",positive," rejection_batches=",rejection_batches," automatic=",automatic," overflow_contracts=",overflow_contracts,
                " exact SDK envelopes, six types empty/new/change/delete, ordered native rejection barriers, framework GID retention and reader teardown");
        } catch(...) { cleanup();throw; }
        cleanup();return 0;
    } catch(std::exception const& error) { fast_io::io::perrln(fast_io::mnp::os_c_str(error.what()));return 1; }
}
