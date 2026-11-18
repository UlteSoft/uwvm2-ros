/** @file @brief Bridge two named parameter reads across independent DDS service mappings.
 * The test provisions separate client and server domains. Two fixed reliable
 * topics use a third domain by default, or the selected legacy server domain. It accepts two named observations in
 * order. The upstream service supplies each result and a deadline bounds every wait.
 */
#include <stdint.h>
#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/msg/parameter_type.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <std_msgs/msg/string.hpp>
#include <rmw/rmw.h>

#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <thread>

using namespace std::chrono_literals;
using Clock = std::chrono::steady_clock;
using Service = rcl_interfaces::srv::GetParameters;
using Sample = std_msgs::msg::String;
using Value = rcl_interfaces::msg::ParameterValue;
constexpr auto string_type = rcl_interfaces::msg::ParameterType::PARAMETER_STRING;

// Early rclcpp releases obtain the node domain from the process environment.
template<class Options>
static auto assign_domain(Options& options, unsigned id, int) -> decltype(options.set_domain_id(id), void())
{
    options.set_domain_id(id);
}

template<class Options>
static void assign_domain(Options&, unsigned id, long)
{
    char const* configured = std::getenv("ROS_DOMAIN_ID");
    if (!configured || std::to_string(id) != configured)
        throw std::invalid_argument("this rclcpp requires one configured domain per process");
}

struct Domain
{
    std::shared_ptr<rclcpp::Context> context = std::make_shared<rclcpp::Context>();
    rclcpp::Node::SharedPtr node;
    std::unique_ptr<rclcpp::executors::SingleThreadedExecutor> executor;

    Domain(unsigned id, char const* name)
    {
        rclcpp::InitOptions init;
        assign_domain(init, id, 0);
        context->init(0, nullptr, init);
        rclcpp::NodeOptions options;
        options.context(context).start_parameter_services(false).start_parameter_event_publisher(false);
        node = rclcpp::Node::make_shared(name, options);
#if __has_include(<rclcpp/executor_options.hpp>)
        rclcpp::ExecutorOptions execution;
#else
        rclcpp::executor::ExecutorArgs execution;
#endif
        execution.context = context;
        executor = std::make_unique<rclcpp::executors::SingleThreadedExecutor>(execution);
        executor->add_node(node);
    }
    ~Domain() { executor.reset(); node.reset(); context->shutdown("relay finished"); }
};

template<class Ready>
static bool wait_for(Domain& domain, Ready ready, Clock::time_point end)
{
    while (!ready() && Clock::now() < end)
    {
        domain.executor->spin_some();
        std::this_thread::sleep_for(2ms);
    }
    return ready();
}

static bool transfer(bool ingress, std::string const& token, unsigned wire_domain)
{
    Domain local(ingress ? 61 : 62, ingress ? "wros_ingress" : "wros_egress");
    Domain wire(wire_domain, ingress ? "wros_ingress_transport" : "wros_egress_transport");
    auto send = wire.node->create_publisher<Sample>(ingress ? "/wros_relay/query" : "/wros_relay/value", 2);
    unsigned count = 0;
    bool failed = false;
    std::optional<std::string> incoming;
    auto expected = [&] { return token + (count == 0 ? "-a" : "-b"); };
    auto receive = wire.node->create_subscription<Sample>(ingress ? "/wros_relay/value" : "/wros_relay/query", 2,
        [&](Sample::ConstSharedPtr s) {
            std::string const allowed = (ingress ? "reply/" : "") + expected();
            if (incoming || count >= 2 || s->data != allowed) failed = true;
            else incoming = s->data;
        });
    auto publish = [&](std::string const& value) { Sample s; s.data = value; send->publish(s); };
    auto end = Clock::now() + 12s;
    if (!wait_for(wire, [&] { return send->get_subscription_count() == 1 && receive->get_publisher_count() == 1; }, end))
        return false;
    if (ingress)
    {
        auto service = local.node->create_service<Service>("/wros_interop/get_parameters",
            [&](std::shared_ptr<Service::Request> req, std::shared_ptr<Service::Response> res) {
                if (count >= 2 || req->names.size() != 1 || req->names.front() != expected())
                { failed = true; return; }
                publish(req->names.front());
                if (!wait_for(wire, [&] { return failed || incoming.has_value(); }, Clock::now() + 2s) || failed)
                { failed = true; return; }
                Value result;
                result.type = string_type;
                result.string_value = std::move(*incoming);
                res->values.emplace_back(std::move(result));
                incoming.reset();
                ++count;
            });
        wait_for(local, [&] { return failed || count == 2; }, end);
    }
    else
    {
        auto client = local.node->create_client<Service>("/wros_interop/get_parameters");
        if (!client->wait_for_service(3s)) return false;
        while (count < 2 && !failed && wait_for(wire, [&] { return failed || incoming.has_value(); }, end) && !failed)
        {
            auto query = std::make_shared<Service::Request>();
            query->names.push_back(std::move(*incoming));
            incoming.reset();
            auto response = client->async_send_request(query);
            auto completion = local.executor->spin_until_future_complete(response, 2s);
            if (completion != decltype(completion)::SUCCESS) return false;
            auto values = response.get();
            if (!values || values->values.size() != 1 || values->values.front().type != string_type ||
                values->values.front().string_value != "reply/" + query->names.front()) return false;
            publish(values->values.front().string_value);
            ++count;
        }
    }
    std::this_thread::sleep_for(600ms);
    return count == 2 && !failed;
}

int main(int argc, char** argv)
{
    if (argc != 5) return 1;
    std::string const wire_domain(argv[4]);
    if (wire_domain != "62" && wire_domain != "63") return 1;
    std::string role(argv[1]), token(argv[2]);
    if ((role != "ingress" && role != "egress") || token.empty() || token.size() > 9 ||
        token.find_first_not_of("0123456789") != std::string::npos) return 1;
    if (std::string(rmw_get_implementation_identifier()) != argv[3]) return 2;
    if (!transfer(role == "ingress", token, wire_domain == "62" ? 62U : 63U)) return 3;
    std::printf("GetParameters relay %s %s %s-a %s-b completed\n", role.c_str(), argv[3], token.c_str(), token.c_str());
}
