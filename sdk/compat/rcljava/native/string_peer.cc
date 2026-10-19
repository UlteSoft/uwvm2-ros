// Independent installed-ROS2 full-value witness. No Wasm SDK, fake transport,
// native identity constant, robot motion or authority installation occurs here.
// Root must run this only inside its reviewed native owner/SDK/memory/deadline
// envelope. R-newline input grants one bounded native scheduling pass. Markers
// alone never prove delivery; preserve the actual complete values and source,
// native SDK, MD/ELF and Host/Gateway/Guest association independently.
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

int main(int argc,char** argv)
{
    bool initialized{};
    try
    {
        if(argc!=3 || (std::string{argv[1]}!="--observe" && std::string{argv[1]}!="--publish"))
            throw std::runtime_error("--observe|--publish /exact-topic");
        std::string const mode{argv[1]},topic{argv[2]};
        if(topic.empty() || topic.front()!='/')throw std::runtime_error("absolute selected topic required");
        int ros_argc{1};char* ros_argv[]{argv[0],nullptr};rclcpp::init(ros_argc,ros_argv);initialized=true;
        auto node{std::make_shared<rclcpp::Node>("rcljava_source_peer")};
        // This is explicit peer QoS, not a guessed discovery of rcljava's RMW
        // default. Root must prove the deployment's actual default-profile
        // contract equals these values or refuse this particular test recipe.
        auto qos{rclcpp::QoS{10}.reliable().durability_volatile()};
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
        unsigned received{},sent{};bool wrong{};
        if(mode=="--observe")
            subscription=node->create_subscription<std_msgs::msg::String>(topic,qos,[&](std_msgs::msg::String::ConstSharedPtr message)
            {
                auto const expected{std::string{"Hello, world! "}+std::to_string(received)};
                if(received>=64 || message->data!=expected)wrong=true;
                std::cout<<"NATIVE_FULL_VALUE "<<message->data.size()<<" "<<message->data<<'\n';
                ++received;
            });
        else publisher=node->create_publisher<std_msgs::msg::String>(topic,qos);
        std::cout<<"NATIVE_READY "<<mode<<'\n'<<std::flush;
        std::string line;
        for(unsigned pass{};pass!=64 && std::getline(std::cin,line);++pass)
        {
            if(line!="R")throw std::runtime_error("exact native scheduling credit required");
            rclcpp::spin_some(node);
            if(publisher)
            {
                if(!publisher->get_subscription_count())continue;
                std_msgs::msg::String message;message.data="native-rcljava:机器人🙂";
                publisher->publish(message);++sent;
                std::cout<<"NATIVE_PUBLISHED "<<message.data.size()<<" "<<message.data<<'\n';
            }
            if(wrong)throw std::runtime_error("native full-value/counter mismatch");
            std::cout.flush();
        }
        rclcpp::spin_some(node);
        if(wrong || (mode=="--observe"?!received:!sent))throw std::runtime_error("no exact independent native value observed/admitted");
        subscription.reset();publisher.reset();node.reset();rclcpp::shutdown();initialized=false;
        std::cout<<"NATIVE_PEER_FINISHED source_or_observation_only_no_whole_lane_qualification\n";
        return 0;
    }
    catch(std::exception const& error)
    {
        if(initialized)rclcpp::shutdown();
        std::cerr<<error.what()<<'\n';return 1;
    }
}
