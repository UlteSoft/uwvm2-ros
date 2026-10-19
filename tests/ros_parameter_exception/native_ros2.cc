#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <fast_io.h>
#include "../ros_resources/common.h"

using namespace ros_resources_probe;
namespace
{
    constexpr char gateway_name[]{"/parameter_exception/gateway_immutable"};
    constexpr char pulse_topic[]{"/parameter_exception/pulse"};
    core::capability_spec parameter(std::string alias,std::string operation,std::string name)
    {
        auto cap{resource(core::version::ros2,std::move(alias),std::move(operation),std::string{gateway_name}+"/"+name)};
        cap.limits["parameter-node"]=gateway_name;cap.limits["parameter-name"]=name;
        if(cap.operation!="param.describe")
        { cap.type="builtin/bool-v1";cap.type_identity="builtin:param-bool-v1";cap.limits["max-parameter-bytes"]="1"; }
        gateway::validate(core::version::ros2,cap);return cap;
    }
    std::string body(ipc::call_result const& value)
    {
        require(!value.error&&value.payload.size()>=4&&ipc::get(value.payload.data(),4)==value.payload.size()-4,"canonical native String body");
        return {reinterpret_cast<char const*>(value.payload.data()+4),value.payload.size()-4};
    }
    struct native_loop
    {
        rclcpp::Node::SharedPtr node;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
        rclcpp::executors::SingleThreadedExecutor executor;
        std::atomic<bool> stop{},failed{};
        std::thread spinner;
        native_loop()
        {
            rclcpp::NodeOptions options;options.enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            node=std::make_shared<rclcpp::Node>("sdk_reference","/parameter_exception",options);
            publisher=node->create_publisher<std_msgs::msg::String>(pulse_topic,rclcpp::QoS(8).reliable().durability_volatile());
            executor.add_node(node);
            spinner=std::thread{[&] { try { while(!stop&&rclcpp::ok()) { executor.spin_once(std::chrono::milliseconds(10)); } }catch(...) { failed=true; } }};
        }
        ~native_loop() { stop=true;executor.cancel();if(spinner.joinable()) { spinner.join(); } }
        void publish(std::string const& value)
        { require(!failed&&rclcpp::ok(),"actual SDK executor healthy");std_msgs::msg::String message;message.data=value;publisher->publish(message); }
        void readers(std::size_t desired,std::string const& witness)
        {
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds(5)};
            while(publisher->get_subscription_count()!=desired)
            { publish(witness);require(std::chrono::steady_clock::now()<until,"actual DDS projection reader count");std::this_thread::sleep_for(std::chrono::milliseconds(20)); }
        }
        void receive(ipc::client& client,std::string const& expected)
        {
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds(5)};
            while(std::chrono::steady_clock::now()<until)
            {
                publish(expected);auto reply{client.call("event.poll","pulse",{},1000)};
                if(!reply.error&&body(reply)==expected) { return; }
                require(!reply.error||reply.error==11,"live sibling event poll remains usable");std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
            require(false,"actual DDS sibling String receipt");
        }
    };
    struct observation { std::string name;bool read_only{},dynamic{},value{};unsigned expected{}; };
}

int main(int argc,char** argv)
{
    try
    {
        require(argc==2,"usage parameter-exception-native <gateway>");
        require(std::getenv("ROS_DOMAIN_ID")&&std::string_view{std::getenv("ROS_DOMAIN_ID")}=="201","isolated explicit ROS domain201");
        int ros_argc{1};rclcpp::init(ros_argc,argv);
        bool correct{true};unsigned cases{},normal{};
        {
            native_loop native;
            std::vector<observation> observations;
            for(std::string const name:{"use_sim_time","start_type_description_service"})
            {
                auto descriptor{native.node->describe_parameter(name)};auto value{native.node->get_parameter(name).as_bool()};bool immutable{};
                try { native.node->undeclare_parameter(name); }
                catch(rclcpp::exceptions::InvalidParameterTypeException const& error)
                {
                    require(!descriptor.read_only&&!descriptor.dynamic_typing,"SDK static refusal has static non-read-only metadata");immutable=true;
                    fast_io::io::println("SDK_STATIC name=",name," exception=InvalidParameterTypeException reason=",fast_io::mnp::os_c_str(error.what()));
                }
                catch(rclcpp::exceptions::ParameterImmutableException const& error)
                {
                    require(descriptor.read_only,"SDK permission refusal has read-only metadata");immutable=true;
                    fast_io::io::println("SDK_IMMUTABLE name=",name," exception=ParameterImmutableException reason=",fast_io::mnp::os_c_str(error.what()));
                }
                require(immutable&&native.node->has_parameter(name)&&native.node->get_parameter(name).as_bool()==value,"actual SDK immutable refusal retains native value");
                require(descriptor.read_only||!descriptor.dynamic_typing,"native default immutable cause is explicit metadata");
                auto expected{descriptor.read_only?13u:95u};
                observations.push_back({name,descriptor.read_only,descriptor.dynamic_typing,value,expected});
                fast_io::io::println("SDK_DESCRIPTOR name=",name," type=",static_cast<unsigned>(descriptor.type)," read_only=",descriptor.read_only," dynamic_typing=",descriptor.dynamic_typing," expected_error=",expected);
            }
            fixture session{core::version::ros2,argv[1],[&](gateway::configuration& config) {
                config.policy.connection["domain-id"]="201";config.policy.connection["node-name"]="gateway_immutable";config.policy.connection["namespace"]="/parameter_exception";config.policy.capabilities.clear();
                for(auto const& item:observations)
                { add(config,parameter("get_"+item.name,"param.get",item.name));add(config,parameter("describe_"+item.name,"param.describe",item.name));add(config,parameter("remove_"+item.name,"param.undeclare",item.name)); }
                add(config,parameter("control_declare","param.declare","removable_control"));add(config,parameter("control_get","param.get","removable_control"));add(config,parameter("control_remove","param.undeclare","removable_control"));
                auto topic_cap{topic(core::version::ros2,"pulse","topic.subscribe",pulse_topic,"std_msgs/msg/String","RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18")};gateway::validate(core::version::ros2,topic_cap);add(config,std::move(topic_cap));
            }};
            ipc::client client;session.connect(client);
            for(auto const& item:observations)
            {
                require(!client.call("topic.subscribe","pulse",{},1000).error,"authorized independent DDS sibling subscription");native.readers(1,"ready");native.receive(client,"before:"+item.name);
                auto described{client.call("param.describe","describe_"+item.name,{},1000)};
                auto description{body(described)};require(description.find("read_only="+std::string{item.read_only?"true":"false"})!=std::string::npos,"real Gateway metadata agrees with the official SDK cause");
                auto read{client.call("param.get","get_"+item.name,{},1000)};
                require(!read.error&&read.payload==std::vector<std::uint8_t>{static_cast<std::uint8_t>(item.value)},"real Gateway default parameter value");
                require(!client.call("param.declare","control_declare",{1},1000).error,"normal dynamic declaration");
                require(!client.call("param.undeclare","control_remove",{},1000).error,"normal dynamic undeclare succeeds");
                auto absent{client.call("param.get","control_get",{},1000)};require(absent.error==2&&absent.payload.empty(),"normal removal has exact not-found result on live session");++normal;
                auto rejected{client.call("param.undeclare","remove_"+item.name,{},1000)};
                auto after{client.call("param.get","get_"+item.name,{},1000)};
                bool retained{rejected.error==item.expected&&rejected.payload.empty()&&!after.error&&after.payload==std::vector<std::uint8_t>{static_cast<std::uint8_t>(item.value)}};
                fast_io::io::println("OBSERVED_IMMUTABLE name=",item.name," actual_error=",rejected.error," expected_error=",item.expected," after_get_error=",after.error," same_client_retained=",retained);
                ++cases;correct=correct&&retained;
                if(retained)
                { native.readers(1,"after");native.receive(client,"after:"+item.name);require(!client.call("capability.close","pulse",{},1000).error,"normal sibling close");native.readers(0,"closed"); }
                else
                { native.readers(0,"after-exception");fast_io::io::println("OBSERVED_SESSION_CLOSED projection_readers=0 name=",item.name);session.connect(client); }
            }
        }
        rclcpp::shutdown();
        if(!correct) { fast_io::io::println("FAIL immutable parameter refusal closed authenticated IPC session cases=",cases," normal_dynamic_removals=",normal);return 1; }
        fast_io::io::println("PASS immutable parameter refusal preserves authenticated IPC session cases=",cases," normal_dynamic_removals=",normal," native_DDS_siblings_retained=2 static_InvalidParameterType95 readonly_ParameterImmutable13");return 0;
    }
    catch(std::exception const& error) { fast_io::io::perrln(fast_io::mnp::os_c_str(error.what()));rclcpp::shutdown();return 2; }
}
