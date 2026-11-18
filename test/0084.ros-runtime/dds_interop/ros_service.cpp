/** @file @brief Exercise ROS service discovery, correlated requests and typed replies across RMWs. */
#include <stdint.h> // Some generated ROS headers use the global uint8_t name.
#include <rcl_interfaces/msg/parameter_type.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>

#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

using namespace std::chrono_literals;
using Service = rcl_interfaces::srv::GetParameters;
using StringType = rcl_interfaces::msg::ParameterType;

static bool serve(rclcpp::Node::SharedPtr const& node, std::string const& token)
{
    std::vector<std::string> const expected{token + "-a", token + "-b"};
    unsigned completed{};
    bool rejected{};
    auto server = node->create_service<Service>(
        "/wros_interop/get_parameters",
        [&](std::shared_ptr<Service::Request> request, std::shared_ptr<Service::Response> reply) {
            if (completed >= expected.size() || request->names.size() != 1 ||
                request->names.front() != expected[completed])
            {
                rejected = true;
                return;
            }
            rcl_interfaces::msg::ParameterValue value;
            value.type = StringType::PARAMETER_STRING;
            value.string_value = "reply/" + request->names.front();
            reply->values.push_back(value);
            ++completed;
        });
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(node);
    auto const end = std::chrono::steady_clock::now() + 13s;
    while (completed < expected.size() && !rejected && std::chrono::steady_clock::now() < end)
    {
        exec.spin_some();
        std::this_thread::sleep_for(5ms);
    }
    if (completed != expected.size() || rejected)
    {
        std::fprintf(stderr, "service received %u of %zu expected requests, rejected=%d\n", completed, expected.size(), rejected);
        return false;
    }
    // Allow reliable DDS writers to complete delivery before removing the server.
    std::this_thread::sleep_for(500ms);
    return true;
}

static bool request(rclcpp::Node::SharedPtr const& node, std::string const& token)
{
    auto client = node->create_client<Service>("/wros_interop/get_parameters");
    if (!client->wait_for_service(5s))
    {
        std::fputs("service discovery timed out\n", stderr);
        return false;
    }
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(node);
    for (std::string const& suffix : {std::string("-a"), std::string("-b")})
    {
        auto input = std::make_shared<Service::Request>();
        input->names.push_back(token + suffix);
        auto pending = client->async_send_request(input);
        auto const completion = exec.spin_until_future_complete(pending, 3s);
        if (completion != decltype(completion)::SUCCESS)
        {
            std::fprintf(stderr, "service response missing for %s\n", input->names.front().c_str());
            return false;
        }
        auto reply = pending.get();
        if (!reply || reply->values.size() != 1 || reply->values.front().type != StringType::PARAMETER_STRING ||
            reply->values.front().string_value != "reply/" + input->names.front()) return false;
    }
    return true;
}

int main(int argc, char** argv)
{
    if (argc != 4) return 1;
    std::string_view const role(argv[2]);
    if (role != "request" && role != "echo") return 1;
    rclcpp::init(argc, argv);
    if (std::string_view(rmw_get_implementation_identifier()) != argv[3]) return 2;
    auto node = rclcpp::Node::make_shared(role == "echo" ? "wros_service_provider" : "wros_service_client");
    bool const passed = role == "echo" ? serve(node, argv[1]) : request(node, argv[1]);
    rclcpp::shutdown();
    if (!passed) return 3;
    std::printf("GetParameters %s %s %s-a %s-b completed\n", argv[3], argv[2], argv[1], argv[1]);
}
