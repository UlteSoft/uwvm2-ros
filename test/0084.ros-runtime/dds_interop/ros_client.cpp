/** @file @brief Check the real rclcpp to independent DDS round trip. */
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/int32.hpp>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <string_view>
#include <thread>

int main(int argc, char** argv)
{
    if(argc != 4) return 1;
    bool const echo = std::string_view(argv[2]) == "echo";
    if(!echo && std::string_view(argv[2]) != "request") return 1;
    using namespace std::chrono_literals;
    using message = std_msgs::msg::Int32;
    auto const challenge = static_cast<std::int32_t>(std::strtol(argv[1], nullptr, 10));
    rclcpp::init(argc, argv);
    if(std::string_view(rmw_get_implementation_identifier()) != argv[3]) return 2;
    auto node = rclcpp::Node::make_shared(echo ? "wros_dds_echo" : "wros_dds_interop");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    bool received{}, unexpected{};
    auto writer = node->create_publisher<message>(echo ? "wros_dds_reply" : "wros_dds_request", 4);
    auto reader = node->create_subscription<message>(echo ? "wros_dds_request" : "wros_dds_reply", 4, [&](message::ConstSharedPtr value) {
        if(value->data != (echo ? challenge : challenge + 1)) { unexpected = true; return; }
        received = true;
        if(echo)
        {
            message response;
            response.data = challenge + 1;
            writer->publish(response);
        }
    });
    message request;
    request.data = challenge;
    auto deadline = std::chrono::steady_clock::now() + 12s;
    bool echoed{};
    while((echo || !received) && !unexpected && std::chrono::steady_clock::now() < deadline)
    {
        if(!echo) writer->publish(request);
        if(echo && received && !echoed) { deadline = std::chrono::steady_clock::now() + 2s; echoed = true; }
        executor.spin_some();
        std::this_thread::sleep_for(20ms);
    }
    rclcpp::shutdown();
    if(!received || unexpected) return 3;
    std::printf("rclcpp %s %s completed\n", argv[3], argv[2]);
}
