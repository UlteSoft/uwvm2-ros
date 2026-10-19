#include "../ros_ipc/probe_common.h"
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
using namespace ros_probe;
int main(int argc,char** argv)
{
    try
    {
        require(argc==3,"usage: secure probe gateway-executable owned-keystore");
        std::string keys{argv[2]};
        ::setenv("ROS_DOMAIN_ID","74",1);::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1);
        ::setenv("ROS_SECURITY_ENABLE","true",1);::setenv("ROS_SECURITY_STRATEGY","Enforce",1);::setenv("ROS_SECURITY_KEYSTORE",keys.c_str(),1);
        char const* init[]{"secure-witness","--ros-args","--enclave","/witness"};rclcpp::init(4,init);
        rclcpp::NodeOptions options;options.enable_rosout(false);options.start_parameter_services(false);options.start_parameter_event_publisher(false);
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto node{std::make_shared<rclcpp::Node>("secure_witness","/uwvm_test",options)};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spin{[&]{executor.spin();}};
        struct joiner {rclcpp::Executor& executor;std::thread& thread;~joiner(){executor.cancel();thread.join();rclcpp::shutdown();}}cleanup{executor,spin};
        fixture f{core::version::ros2,argv[1],[&](gateway::configuration& c)
        {
            c.policy.connection["domain-id"]="74";c.policy.connection["security-mode"]="enforce";c.policy.connection["keystore"]=keys;c.policy.connection["enclave"]="/gateway";
            for(auto it=c.policy.capabilities.begin();it!=c.policy.capabilities.end();)
            {if(it->first!="pubString"&&it->first!="subString"){it=c.policy.capabilities.erase(it);}else{++it;}}
        }};
        ipc::client client;f.connect(client);std::atomic<unsigned> received{};
        auto subscriber{node->create_subscription<std_msgs::msg::String>("/uwvm_test/pubString",rclcpp::QoS{8}.reliable(),[&](std_msgs::msg::String::ConstSharedPtr m)
        {if(m->data=="uwvm real ROS exact payload"){++received;}})};
        auto publisher{node->create_publisher<std_msgs::msg::String>("/uwvm_test/subString",rclcpp::QoS{8}.reliable())};
        require(!client.call("topic.subscribe","subString",{},1000).error,"secure Gateway subscriber");
        std_msgs::msg::String m;m.data="uwvm real ROS exact payload";ipc::call_result incoming{11,{}};
        for(unsigned n{};n!=250&&(incoming.error||!received.load());++n)
        {
            require(!client.call("topic.publish","pubString",data(0),1000).error,"secure Gateway publication");publisher->publish(m);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));incoming=client.call("event.poll","subString",{},1000);
            require(!incoming.error||incoming.error==11,"secure incoming queue");
        }
        require(received.load()>0&&!incoming.error&&incoming.payload==data(0),"actual signed DDS-Security bidirectional exact content");
        bool refused{};
        try{auto denied{node->create_publisher<std_msgs::msg::String>("/uwvm_test/not_permitted",rclcpp::QoS{8})};denied->publish(m);}
        catch(std::exception const&){refused=true;}
        require(refused,"DDS-Security explicitly denied native publisher outside signed exact-resource permissions");
        std::cout<<"PASS actual FastDDS SROS2 Enforce, owned signed enclave identities, exact-resource bidirectional communication and denied native DDS publisher\n";return 0;
    }
    catch(std::exception const& e){std::cerr<<e.what()<<'\n';return 1;}
}
