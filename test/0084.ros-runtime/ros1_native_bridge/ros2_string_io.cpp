// One test-only ROS endpoint. Its parent provides a fixed 32-byte String on stdin
// or receives one on stdout. No CLI or payload can select a peer or topic.
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <string>
#include <thread>

namespace {
bool valid(std::string const& data)
{
    return data.size() == 32 && std::all_of(data.begin(), data.end(), [](char ch) {
        return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f');
    });
}
}

int main(int argc, char** argv)
{
    using namespace std::chrono_literals;
    if (argc != 2) return 1;
    std::string const role(argv[1]);
    if (role != "take" && role != "publish") return 1;
    auto const activate = std::fgetc(stdin);
    if (activate == EOF) return 0;
    if (activate != 'S') return 3;
    std::fputs("activating DDS endpoint\n", stderr);
    rclcpp::init(0, nullptr);
    if (std::string(rmw_get_implementation_identifier()) != "rmw_cyclonedds_cpp") return 2;
    auto node = rclcpp::Node::make_shared("wros_bounded_string_adapter");
    using Message = std_msgs::msg::String;
    auto const until = std::chrono::steady_clock::now() + 4s;
    if (role == "publish") {
        Message sample;
        sample.data.resize(32);
        if (std::fread(sample.data.data(), 1, 32, stdin) != 32 || !valid(sample.data)) return 3;
        auto publisher = node->create_publisher<Message>("/chatter", 4);
        while (!publisher->get_subscription_count()) {
            if (std::chrono::steady_clock::now() >= until) return 4;
            std::this_thread::sleep_for(5ms);
        }
        publisher->publish(sample);
        if (!publisher->wait_for_all_acked(1s)) return 5;
        if (std::fputc('P', stdout) == EOF) return 7;
    } else {
        std::string data;
        bool invalid{};
        auto reader = node->create_subscription<Message>("/chatter", 4, [&](Message const& sample) {
            if (!valid(sample.data) || (!data.empty() && data != sample.data)) invalid = true;
            else data = sample.data;
        });
        while (data.empty() && !invalid) {
            if (std::chrono::steady_clock::now() >= until) return 6;
            rclcpp::spin_some(node);
            std::this_thread::sleep_for(5ms);
        }
        if (invalid || std::fwrite(data.data(), 1, data.size(), stdout) != 32) return 7;
    }
    rclcpp::shutdown();
    return 0;
}
