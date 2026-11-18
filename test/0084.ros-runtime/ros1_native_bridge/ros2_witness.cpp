// One explicit String topic for the native ROS 1 bridge qualification.
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <cstdio>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    if (argc != 4) return 1;
    std::string const role(argv[1]), challenge(argv[2]);
    int const seconds = std::stoi(argv[3]);
    if ((role != "send" && role != "receive") || challenge.size() != 32 || seconds < 1 || seconds > 15) return 1;
    rclcpp::init(argc, argv);
    if (std::string(rmw_get_implementation_identifier()) != "rmw_cyclonedds_cpp") return 2;
    using namespace std::chrono_literals;
    using Message = std_msgs::msg::String;
    auto node = rclcpp::Node::make_shared("wros_ros2_witness");
    bool received{}, unexpected{};
    rclcpp::Publisher<Message>::SharedPtr publisher;
    rclcpp::Subscription<Message>::SharedPtr reader;
    if (role == "send") publisher = node->create_publisher<Message>("chatter", 4);
    else reader = node->create_subscription<Message>("chatter", 4, [&](Message const& sample) {
        if (sample.data == challenge) received = true;
        else unexpected = true;
    });
    Message sample;
    sample.data = challenge;
    unsigned sent{};
    auto const until = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
    while (!received && !unexpected && std::chrono::steady_clock::now() < until) {
        if (publisher) {
            publisher->publish(sample);
            ++sent;
        }
        rclcpp::spin_some(node);
        std::this_thread::sleep_for(50ms);
    }
    rclcpp::shutdown();
    std::printf("role=%s sent=%u received=%d unexpected=%d value=%s rmw=rmw_cyclonedds_cpp\n",
                role.c_str(), sent, received, unexpected, (received || sent) ? challenge.c_str() : "");
    if (unexpected) return 4;
    return (received || sent) ? 0 : 3;
}
