#include <rclcpp/rclcpp.hpp>
#include <rclcpp/parameter_client.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <fast_io.h>
#include "../ros_resources/common.h"

using namespace ros_resources_probe;
namespace
{
    constexpr char remote_name[]{"/parameter_initialize/native"};
    core::capability_spec parameter(std::string alias,std::string operation,std::string name)
    {
        auto cap{resource(core::version::ros2,std::move(alias),std::move(operation),std::string{remote_name}+"/"+name)};
        cap.limits["parameter-node"]=remote_name;cap.limits["parameter-name"]=name;
        if(cap.operation!="param.describe")
        { cap.type="builtin/bool-v1";cap.type_identity="builtin:param-bool-v1";cap.limits["max-parameter-bytes"]="1"; }
        gateway::validate(core::version::ros2,cap);return cap;
    }
    struct executor_scope
    {
        rclcpp::executors::SingleThreadedExecutor executor;
        std::atomic<bool> stop{},failed{};
        std::thread spinner;
        executor_scope(rclcpp::Node::SharedPtr const& remote,rclcpp::Node::SharedPtr const& observer)
        {
            executor.add_node(remote);executor.add_node(observer);
            spinner=std::thread{[&] { try { while(!stop&&rclcpp::ok()) { executor.spin_once(std::chrono::milliseconds(10)); } }catch(...) { failed=true; } }};
        }
        ~executor_scope() { stop=true;executor.cancel();if(spinner.joinable()) { spinner.join(); } }
    };
    template<typename Future>
    auto ready(Future future)
    { require(future.wait_for(std::chrono::seconds(5))==std::future_status::ready,"actual official parameter service ready");return future.get(); }
}

int main(int argc,char** argv)
{
    try
    {
        require(argc==2,"usage parameter-initialize-native <gateway>");
        require(std::getenv("ROS_DOMAIN_ID")&&std::string_view{std::getenv("ROS_DOMAIN_ID")}=="202","isolated explicit ROS domain202");
        int ros_argc{1};rclcpp::init(ros_argc,argv);bool correct{};
        {
            rclcpp::NodeOptions options;options.enable_rosout(false).start_parameter_event_publisher(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            auto node{std::make_shared<rclcpp::Node>("native","/parameter_initialize",options)};
            node->declare_parameter("pending",rclcpp::ParameterType::PARAMETER_BOOL);
            node->declare_parameter<bool>("initialized",true);
            rcl_interfaces::msg::ParameterDescriptor dynamic;dynamic.dynamic_typing=true;
            node->declare_parameter("dynamic",rclcpp::ParameterValue{},dynamic);
            node->declare_parameter("wrong",rclcpp::ParameterType::PARAMETER_INTEGER);
            std::atomic<unsigned> pending_writes{},control_writes{},other_writes{};
            auto observed{node->add_on_set_parameters_callback([&](std::vector<rclcpp::Parameter> const& parameters) {
                for(auto const& value:parameters)
                { if(value.get_name()=="pending") { ++pending_writes; }else if(value.get_name()=="initialized") { ++control_writes; }else { ++other_writes; } }
                rcl_interfaces::msg::SetParametersResult result;result.successful=true;return result;
            })};
            auto observer{std::make_shared<rclcpp::Node>("observer","/parameter_initialize",options)};
            auto sdk{std::make_shared<rclcpp::AsyncParametersClient>(observer,remote_name)};
            executor_scope running{node,observer};require(sdk->wait_for_service(std::chrono::seconds(5)),"official parameter services discovered");
            auto raw{observer->create_client<rcl_interfaces::srv::GetParameters>(std::string{remote_name}+"/get_parameters")};
            auto query{std::make_shared<rcl_interfaces::srv::GetParameters::Request>()};query->names={"pending"};
            auto generated{ready(raw->async_send_request(query))};
            auto prior{ready(sdk->get_parameters({"pending"}))};
            auto descriptor{ready(sdk->describe_parameters({"pending"}))};
            fast_io::io::println("SDK_GET_RAW name=pending values_count=",generated->values.size()," sdk_count=",prior.size());
            require(generated->values.empty()&&prior.empty(),"actual Jazzy official static-uninitialized GetParameters returns empty");
            require(descriptor.size()==1&&descriptor[0].name=="pending"&&descriptor[0].type==1&&!descriptor[0].dynamic_typing&&!descriptor[0].read_only,"official uninitialized descriptor retains exact static Bool kind");
            fast_io::io::println("SDK_UNINITIALIZED name=pending declared=",node->has_parameter("pending")," value_count=",prior.size()," descriptor_tag=",static_cast<unsigned>(descriptor[0].type)," dynamic_typing=",descriptor[0].dynamic_typing," read_only=",descriptor[0].read_only);
            for(auto const& name:{"dynamic","wrong","missing"})
            {
                auto values{ready(sdk->get_parameters({name}))};auto descriptions{ready(sdk->describe_parameters({name}))};
                fast_io::io::println("SDK_PREFLIGHT name=",fast_io::mnp::os_c_str(name)," values_count=",values.size()," value_tag=",values.empty()?255u:static_cast<unsigned>(values[0].get_type())," descriptors_count=",descriptions.size()," descriptor_tag=",descriptions.empty()?255u:static_cast<unsigned>(descriptions[0].type)," dynamic_typing=",descriptions.empty()?false:descriptions[0].dynamic_typing);
            }
            fixture session{core::version::ros2,argv[1],[](gateway::configuration& config) {
                config.policy.connection["domain-id"]="202";config.policy.connection["node-name"]="gateway_initialize";config.policy.connection["namespace"]="/parameter_initialize";config.policy.capabilities.clear();
                for(auto const& name:{"pending","initialized","dynamic","wrong","missing"})
                { add(config,parameter(std::string{"get_"}+name,"param.get",name));add(config,parameter(std::string{"set_"}+name,"param.set",name)); }
                add(config,parameter("describe_pending","param.describe","pending"));
            }};
            ipc::client client;session.connect(client);
            auto before{client.call("param.get","get_pending",{},1500)};require(before.error==2&&before.payload.empty(),"uninitialized adapter getter is not-found without closing session");
            auto metadata{text(client.call("param.describe","describe_pending",{},1500))};require(metadata.find("type=1")!=std::string::npos,"adapter description retains static Bool metadata");
            for(auto const& name:{"dynamic","wrong","missing"})
            { auto reply{client.call("param.set",std::string{"set_"}+name,{1},1500)};require(reply.error==95&&reply.payload.empty(),"dynamic/wrong/missing initialization denied"); }
            require(other_writes==0&&!node->has_parameter("missing"),"denied preflights cause no setter or creation");
            require(!client.call("param.set","set_initialized",{0},1500).error&&control_writes==1&&!node->get_parameter("initialized").as_bool(),"initialized static Bool control still works");
            auto selected{client.call("param.set","set_pending",{1},1500)};
            auto after{client.call("param.get","get_pending",{},1500)};
            auto writes{pending_writes.load()};
            fast_io::io::println("OBSERVED_STATIC_INITIALIZE adapter_error=",selected.error," after_get_error=",after.error," native_setter_calls=",writes," expected_error=0 expected_setter_calls=1");
            correct=!selected.error&&selected.payload.empty()&&!after.error&&after.payload==std::vector<std::uint8_t>{1}&&writes==1;
            if(!correct)
            {
                auto unchanged{ready(sdk->get_parameters({"pending"}))};require(selected.error==95&&selected.payload.empty()&&after.error==2&&unchanged.empty()&&writes==0,"before adapter refuses existing static uninitialized parameter without mutation");
            }
            auto sdk_set{ready(sdk->set_parameters({rclcpp::Parameter("pending",true)}))};
            require(sdk_set.size()==1&&sdk_set[0].successful&&node->get_parameter("pending").as_bool()&&pending_writes==writes+1,"official SDK initializes same declared static parameter normally");
            auto final{client.call("param.get","get_pending",{},1500)};require(!final.error&&final.payload==std::vector<std::uint8_t>{1},"same authenticated client reads official SDK initialization");
            require(!running.failed,"native executor healthy");
            fast_io::io::println("SDK_SET_CONTROL successful=1 native_setter_calls=",pending_writes.load()," same_session_get_error=",final.error," initialized_control_calls=",control_writes.load()," denied_other_calls=",other_writes.load());
        }
        rclcpp::shutdown();
        if(!correct) { fast_io::io::println("FAIL declared static uninitialized parameter refused before native setter");return 1; }
        fast_io::io::println("PASS declared static uninitialized parameter initializes, exact static descriptor and missing/dynamic/wrong refusals retain session");return 0;
    }
    catch(std::exception const& error) { fast_io::io::perrln(fast_io::mnp::os_c_str(error.what()));rclcpp::shutdown();return 2; }
}
