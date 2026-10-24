#include "common.h"
#include "../../../../../src/ros/gateway/ros2/resources.h"

// This complementary native fixture checks direct trusted-adapter contracts.
// The separate native_ros2.cc checks authentication and real IPC ownership.
// This file is prepared source until Linux/cgroup compilation and execution.
int main(int argc,char** argv)
{
    using namespace local_event_probe;
    namespace resources=gateway::ros2_resources;
    try
    {
        require(std::getenv("ROS_DOMAIN_ID")&&std::string_view{std::getenv("ROS_DOMAIN_ID")}=="203","isolated contract domain203");
        rclcpp::init(argc,argv);unsigned checks{};
        {
            auto& registry{resources::local_parameter_events()};
            rclcpp::NodeOptions options;options.enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
            auto name{name_of("bool")};options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false),rclcpp::Parameter(name,true)});
            auto node{std::make_shared<rclcpp::Node>("gateway",namespace_name,options)};
            auto cap{event_cap("bool","event",name)};auto queue{std::make_shared<gateway::event_queue>(cap)};
            std::shared_ptr<rclcpp::SubscriptionBase> subscription;
            require(!resources::subscribe_events(node,cap,queue,gateway::ros2_qos::configured(cap),subscription).error,"explicit known grant with verified native QoS activates");++checks;
            auto declaration{parameter_cap("bool","declare","param.declare",name)};
            auto result{resources::call(node,declaration,request(sample("bool")),2000)};
            auto event{queue->poll()};uwvm_ros::typed_parameter_event decoded;
            require(!result.error&&!event.error&&node->get_parameter(name).as_bool()&&uwvm_ros::decode(event.payload,decoded)&&event.payload==expected(name,sample("bool",1),1,decoded.seconds,decoded.nanoseconds),
                "declaration event carries actual native override, not requested default");++checks;
            require(queue->poll().error==11,"override declaration has one event");++checks;
            {
                resources::local_parameter_commit_snapshot isolated{node,name};std::atomic<bool> native_success{};
                std::thread external{[&] { native_success=node->set_parameter(rclcpp::Parameter{name,false}).successful; }};external.join();
                require(native_success&&!isolated.owned->ready&&queue->poll().error==11,"other native writer is neither captured nor automatically observed");++checks;
            }
            auto setter{parameter_cap("bool","set","param.set",name)};
            auto delayed{node->add_on_set_parameters_callback([&](std::vector<rclcpp::Parameter> const& values) {
                for(auto const& value:values) { if(value.get_name()==name) { std::this_thread::sleep_for(std::chrono::milliseconds(1100)); } }
                rcl_interfaces::msg::SetParametersResult response;response.successful=true;return response;
            })};
            result=resources::call(node,setter,request(sample("bool")),1000);event=queue->poll();
            require(result.error==110&&result.payload.empty()&&!node->get_parameter(name).as_bool()&&!event.error&&uwvm_ros::decode(event.payload,decoded)&&event.payload==expected(name,sample("bool"),2,decoded.seconds,decoded.nanoseconds),
                "committed native set still emits exactly once after RPC timeout");++checks;
            delayed.reset();
            auto transformed{node->add_pre_set_parameters_callback([&](std::vector<rclcpp::Parameter>& values) {
                for(auto& value:values) { if(value.get_name()==name) { value=rclcpp::Parameter{name,true}; } }
            })};
            result=resources::call(node,setter,request(sample("bool")),2000);event=queue->poll();
            require(!result.error&&node->get_parameter(name).as_bool()&&!event.error&&uwvm_ros::decode(event.payload,decoded)&&event.payload==expected(name,sample("bool",1),2,decoded.seconds,decoded.nanoseconds),
                "set event snapshots actual native pre-set value");++checks;transformed.reset();
            auto wrong{cap};wrong.alias="different";std::shared_ptr<rclcpp::SubscriptionBase> rejected;
            require(resources::subscribe_events(node,wrong,queue,gateway::ros2_qos::configured(wrong),rejected).error==13&&!rejected,
                "queue grant cannot replace the explicitly authorized activation grant");++checks;
            queue->deactivate();resources::notify_local_parameter_event(node,name,2,rclcpp::ParameterValue{true});
            require(queue->poll().error==125,"late mutation cannot reactivate a closed queue");++checks;
            {
                std::lock_guard<std::mutex> lock{registry.mutex};registry.prune();require(registry.routes.empty(),"inactive queue route removed on dispatch");++checks;
            }
            // This is controlled registry occupancy, not 65,536 DDS readers or
            // an allocation/performance claim. It independently exercises the
            // implementation ceiling without quadratic activation setup.
            auto live{std::make_shared<gateway::event_queue>(cap)},extra{std::make_shared<gateway::event_queue>(cap)};
            {
                std::lock_guard<std::mutex> lock{registry.mutex};registry.routes.assign(resources::local_parameter_event_routes::maximum_routes,{node,live});
            }
            require(!resources::register_local_parameter_events(node,extra),"full weak registry rejects an additional activation");++checks;
            std::shared_ptr<rclcpp::SubscriptionBase> refused;
            require(resources::subscribe_events(node,cap,extra,gateway::ros2_qos::configured(cap),refused).error==90&&!refused&&extra->poll().error==11,
                "quota refusal clears new native owner and never enables delivery");++checks;
            live->deactivate();require(resources::register_local_parameter_events(node,extra),"pruning inactive routes restores bounded capacity");++checks;
            std::weak_ptr<gateway::event_queue> expired{extra};extra.reset();require(expired.expired(),"registry owns no queue lifetime");++checks;
            {
                std::lock_guard<std::mutex> lock{registry.mutex};registry.prune();require(registry.routes.empty(),"expired route does not accumulate");++checks;
            }
            auto remote_cap{event_cap("bool","remote",name)};remote_cap.limits["parameter-node"]="/local_parameter_events/other";
            remote_cap.resource=fast_io::concat_std("/local_parameter_events/other/",name);auto remote{std::make_shared<gateway::event_queue>(remote_cap)};
            require(resources::register_local_parameter_events(node,remote)&&registry.routes.empty(),"other-node event grant never acquires local mutation route");++checks;
            // An explicitly enabled native automatic publisher keeps the DDS
            // path alone; the helper must not add a duplicate memory route.
            rclcpp::NodeOptions published_options;published_options.enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(true);
            auto published{std::make_shared<rclcpp::Node>("published",namespace_name,published_options)};
            auto native_cap{cap};native_cap.limits["parameter-node"]=published->get_fully_qualified_name();native_cap.resource=fast_io::concat_std(fast_io::mnp::os_c_str(published->get_fully_qualified_name()),"/",name);
            auto native_queue{std::make_shared<gateway::event_queue>(native_cap)};
            require(resources::register_local_parameter_events(published,native_queue)&&registry.routes.empty(),"native automatic publication excludes duplicate local route");++checks;
            rclcpp::ParameterValue bounded;
            require(!resources::snapshot_native_parameter(rclcpp::ParameterValue{std::string(ipc::parameter_wire_limit,'x')},bounded)&&bounded.get_type()==rclcpp::ParameterType::PARAMETER_NOT_SET,
                "oversized actual String is refused before bridge snapshot copy");++checks;
            require(!resources::snapshot_native_parameter(rclcpp::ParameterValue{std::vector<std::uint8_t>(ipc::parameter_max_elements+1)},bounded),
                "actual array element ceiling precedes snapshot allocation");++checks;
            rcl_interfaces::msg::ParameterValue inactive;inactive.type=rcl_interfaces::msg::ParameterType::PARAMETER_BOOL;inactive.bool_value=true;
            inactive.string_value.assign(ipc::parameter_wire_limit+1,'x');
            require(resources::snapshot_native_parameter(rclcpp::ParameterValue{inactive},bounded)&&bounded.get<rclcpp::ParameterType::PARAMETER_BOOL>()&&bounded.to_value_msg().string_value.empty(),
                "bounded snapshot copies only active Bool, not oversized inactive String");++checks;
            for(unsigned native_tag:{1u,2u,3u,0u})
            {
                inactive.type=static_cast<std::uint8_t>(native_tag);inactive.integer_value=7;inactive.double_value=1.25;
                auto selected{native_tag==1?"bool":native_tag==2?"int64":native_tag==3?"float64":"deletion"};
                auto scalar_cap{event_cap(selected,"inactive_scalar",name)};auto inactive_native{rclcpp::ParameterValue{inactive}};ipc::parameter_value projected;
                require(!resources::project_event_value(scalar_cap,inactive_native,native_tag==1?1u:8u,true,projected)&&
                    projected.type==ipc::parameter_event_type(core::version::ros2,scalar_cap.type)&&projected.string_value.empty(),
                    "all four native scalar/NOT_SET projections avoid oversized inactive String copy");++checks;
                require(resources::snapshot_native_parameter(inactive_native,bounded)&&bounded.to_value_msg().string_value.empty(),
                    "all four scalar/NOT_SET snapshots reconstruct only their active native field");++checks;
            }
            // Same projection routine accepts either borrowed native values or
            // generated DDS values. Check rejection before any queued bytes.
            auto string_cap{event_cap("string","utf8",name_of("string"))};gateway::event_queue strings{string_cap};
            require(!resources::enqueue_parameter_event(strings,1,0,2,gateway_name,name_of("string"),rclcpp::ParameterValue{std::string{"\x80"}})&&strings.poll().error==11,
                "malformed local native String is rejected");++checks;
            auto array_cap{event_cap("string-array","array",name_of("string-array"))};gateway::event_queue arrays{array_cap};
            require(!resources::enqueue_parameter_event(arrays,1,0,2,gateway_name,name_of("string-array"),rclcpp::ParameterValue{std::vector<std::string>{std::string{"x\0y",3}}})&&arrays.poll().error==11,
                "native StringArray NUL is rejected before copying");++checks;
            auto float_cap{event_cap("float64","finite",name_of("float64"))};gateway::event_queue finite{float_cap};
            require(!resources::enqueue_parameter_event(finite,1,0,2,gateway_name,name_of("float64"),rclcpp::ParameterValue{std::numeric_limits<double>::quiet_NaN()})&&finite.poll().error==11,
                "nonfinite local Float64 is rejected");++checks;
            require(!resources::enqueue_parameter_event(finite,2147483648U,0,2,gateway_name,name_of("float64"),rclcpp::ParameterValue{0.5})&&
                !resources::enqueue_parameter_event(finite,1,1000000000U,2,gateway_name,name_of("float64"),rclcpp::ParameterValue{0.5})&&finite.poll().error==11,
                "invalid native timestamp creates no queued event");++checks;
            auto rate_cap{event_cap("bool","rate",name)};rate_cap.limits["max-rate-hz"]="0.001";rate_cap.limits["burst"]="1";gateway::event_queue rate{rate_cap};
            require(resources::enqueue_parameter_event(rate,1,0,2,gateway_name,name,rclcpp::ParameterValue{true})&&
                !resources::enqueue_parameter_event(rate,1,1,2,gateway_name,name,rclcpp::ParameterValue{false})&&rate.events.size()==1&&rate.dropped==1,
                "local projection shares the existing token bucket");++checks;
            node->undeclare_parameter(name);subscription.reset();std::weak_ptr<rclcpp::Node> weak_node{node};node.reset();
            require(weak_node.expired(),"registry owns no Node lifetime");++checks;
            fast_io::io::println("PASS local ROS2 parameter event trusted contracts checks=",checks,
                " actual declaration overrides; committed timeout110; exact queue grant; weak ownership and ceiling; no native-publication duplicate; UTF8/NUL/finite/stamp/rate");
        }
        rclcpp::shutdown();return 0;
    }
    catch(std::exception const& error) { fast_io::io::perrln(fast_io::mnp::os_c_str(error.what()));if(rclcpp::ok()) { rclcpp::shutdown(); }return 1; }
}
