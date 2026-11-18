#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <cstdio>
#include <string>
#include <thread>

using namespace std::chrono_literals;
using Message = std_msgs::msg::String;

int main(int argc, char** argv)
{
    if(argc < 4) return 1;
    std::string const role = argv[1], token = argv[2];
    bool const request = role == "request", denied = role == "observer";
    if(!request && !denied && role != "echo") return 1;
    rclcpp::init(argc, argv);
    if(std::string(rmw_get_implementation_identifier()) != "rmw_fastrtps_cpp") return 2;
    rclcpp::NodeOptions options;
    options.enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
    options.parameter_overrides({rclcpp::Parameter("start_type_description_service", false)});
    auto node = std::make_shared<rclcpp::Node>(request ? "uwvm_ros2_gateway_probe" : denied ? "observer" : "secure_witness",
                                              "/uwvm_test", options);
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    char const* const forward = "/uwvm_test/pubString";
    char const* const reverse = "/uwvm_test/subString";
    using Reader = rclcpp::Subscription<Message>::SharedPtr;
    if(denied)
    {
        Reader reader;
        unsigned received = 0;
        try
        {
            reader = node->create_subscription<Message>(forward, 4, [&](Message const&) { ++received; });
        }
        catch(std::exception const& error)
        {
            std::fprintf(stdout, "reader creation refused: %s\n", error.what());
        }
        std::puts("observer ready");
        std::fflush(stdout);
        auto end = std::chrono::steady_clock::now() + 7s;
        while(std::chrono::steady_clock::now() < end)
        {
            executor.spin_some();
            std::this_thread::sleep_for(20ms);
        }
        std::printf("observer reader=%s samples=%u\n", reader ? "created" : "refused", received);
        rclcpp::shutdown();
        return received || reader ? 3 : 0;
    }
    std::string const challenge = "secure-request-" + token, reply = "secure-reply-" + token;
    auto writer = node->create_publisher<Message>(request ? forward : reverse, 4);
    unsigned valid = 0, invalid = 0;
    auto reader = node->create_subscription<Message>(request ? reverse : forward, 4, [&](Message const& message) {
        if(message.data != (request ? reply : challenge)) { ++invalid; return; }
        if(!valid++) std::printf("received %s\n", message.data.c_str());
        if(!request)
        {
            Message response;
            response.data = reply;
            writer->publish(response);
        }
    });
    auto end = std::chrono::steady_clock::now() + 12s;
    std::chrono::steady_clock::time_point first{};
    while(std::chrono::steady_clock::now() < end && !invalid)
    {
        if(request)
        {
            Message message;
            message.data = challenge;
            writer->publish(message);
        }
        executor.spin_some();
        if(valid && first == std::chrono::steady_clock::time_point{}) first = std::chrono::steady_clock::now();
        if(valid && std::chrono::steady_clock::now() - first > (request ? 250ms : 600ms)) break;
        std::this_thread::sleep_for(20ms);
    }
    rclcpp::shutdown();
    return valid && !invalid ? 0 : 4;
}
