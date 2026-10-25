/**
 * @file ros_resource_peer.cc
 * @brief Measure actual ROS graph and parameter APIs against authorized IPC.
 * @details A separate native peer owns every observed endpoint and parameter.
 * The native comparison retains its node, clients and executor, and produces
 * the same bounded canonical result as the IPC path. Discovery and warm-up
 * precede the measurement barrier. No mock response or robot graph is used.
 */
#include "ros_resource_contract.h"
#include <gateway/common/config.h>
#include <uwvm2/imported/ros/common/manifest.h>
#ifndef UWVM_ROS_BENCH_VERSION
# define UWVM_ROS_BENCH_VERSION 2
#endif
#if UWVM_ROS_BENCH_VERSION == 1
# include <ros/ros.h>
# include <ros/master.h>
# include <std_msgs/String.h>
# include <std_srvs/Trigger.h>
#else
# include <rclcpp/rclcpp.hpp>
# include <rclcpp/parameter_client.hpp>
# include <std_msgs/msg/string.hpp>
# include <std_srvs/srv/trigger.hpp>
#endif
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <set>
#include <stdexcept>
#include <thread>

namespace
{
    namespace gateway = uwvm2::ros_gateway;
    namespace ipc = uwvm2::imported::ros::ipc;
    using clock_type = std::chrono::steady_clock;
    using bytes = std::vector<std::uint8_t>;

    /** @brief Enforce release-build fixture invariants with a useful label. */
    void require(bool condition, char const* label)
    {
        if(!condition) { throw std::runtime_error(label); }
    }

    /** @brief Keep canonical text construction equivalent across measured paths. */
    bytes text_result(std::string const& text)
    {
        require(text.size() <= 16380u, "native result budget");
        bytes result;
        ipc::put(result, text.size(), 4);
        result.insert(result.end(), text.begin(), text.end());
        return result;
    }

    /** @brief Encode only the requested peer rows, in stable sorted order. */
    struct graph_rows
    {
        unsigned which;
        std::set<std::string> values;

        void add(std::string const& name, std::string const& type = {})
        {
            std::string const prefix{UWVM_BENCH_SCOPE "/"};
            if(name.compare(0, prefix.size(), prefix) != 0) { return; }
            if(which == 2u && name != UWVM_BENCH_SCOPE "/topic") { return; }
            if(which == 4u && name != UWVM_BENCH_NODE) { return; }
            if(which == 6u && name != UWVM_BENCH_SCOPE "/trigger") { return; }
            values.insert(name + (type.empty() ? "" : "\t" + type) + "\n");
        }

        bytes result() const
        {
            std::string text;
            for(auto const& row: values) { text += row; }
            return text_result(text);
        }
    };

    /** @brief Wait only for graph discovery, before any measured query. */
    template <class Query>
    void discover(unsigned which, Query&& query)
    {
        if(which > 6u) { return; }
        auto const deadline{clock_type::now() + std::chrono::seconds(10)};
        unsigned probes{};
        for(;;)
        {
            auto const result{query()};
            ++probes;
            if(!result.error && resource_valid(which, UWVM_ROS_BENCH_VERSION, result.payload.data(), result.payload.size())) { break; }
            require((!result.error || result.error == 2u) && clock_type::now() < deadline, "graph discovery failed");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::cerr << "BENCH_DISCOVERY_PROBES=" << probes << '\n';
    }

    /** @brief Measure a validated query loop with the shared profiler barrier. */
    template <class Query>
    void measure(unsigned count, unsigned which, Query&& query)
    {
        discover(which, query);
        auto exchange = [&]
        {
            auto const response{query()};
            if(response.error) { std::cerr << "BENCH_ROS_ERROR=" << response.error << '\n'; }
            require(!response.error && resource_valid(which, UWVM_ROS_BENCH_VERSION, response.payload.data(), response.payload.size()),
                    "native resource reply mismatch");
        };
        for(unsigned index{}; index != 20u; ++index) { exchange(); }
        std::cerr << "BENCH_READY=" << count << '\n' << std::flush;
        require(std::cin.get() == 'R', "measurement barrier");
        auto const begin{clock_type::now()};
        for(unsigned index{}; index != count; ++index) { exchange(); }
        auto const elapsed{std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - begin).count()};
        std::cerr << "BENCH_BODY_NS=" << elapsed << "\nBENCH_ROS_COUNT=" << count << "\nBENCH_CHECKSUM=" << count << '\n';
    }

    /** @brief Resolve only an enumerated public resource measurement. */
    unsigned case_number(std::string name)
    {
        if(name.compare(0, 10, "parameter-") == 0) { name.replace(0, 10, "param."); }
        else if(name.compare(0, 6, "graph-") == 0) { name[5] = '.'; }
        for(unsigned which{1}; which != 13u; ++which)
        {
            if(name == resource_operation(which)) { return which; }
        }
        throw std::runtime_error("unknown resource case");
    }

#if UWVM_ROS_BENCH_VERSION == 1
    /** @brief Query the official ROS1 API without the VM or gateway adapter. */
    ipc::call_result native_query(unsigned which)
    {
        if(which == 7u)
        {
            std::string value;
            require(ros::param::get(UWVM_BENCH_VALUE, value), "native getParam");
            return {0, text_result(value)};
        }
        if(which == 8u)
        {
            ros::param::set(UWVM_BENCH_VALUE, std::string{"ok"});
            return {};
        }
        if(which == 11u) { return {0, {static_cast<std::uint8_t>(ros::param::has(UWVM_BENCH_VALUE))}}; }
        if(which == 12u)
        {
            // This exact-name policy does not authorize searching parent nodes.
            require(ros::param::has(UWVM_BENCH_VALUE), "native exact parameter search");
            return {0, text_result(UWVM_BENCH_VALUE)};
        }
        graph_rows rows{which, {}};
        if(which == 9u)
        {
            std::vector<std::string> names;
            require(ros::param::getParamNames(names), "native getParamNames");
            for(auto const& name: names) { rows.add(name); }
        }
        else
        {
            XmlRpc::XmlRpcValue args, response, values;
            args[0] = ros::this_node::getName();
            bool const topics{which == 1u || which == 2u};
            require(ros::master::execute(topics ? "getTopicTypes" : "getSystemState", args, response, values, false),
                    "native graph query");
            require(values.getType() == XmlRpc::XmlRpcValue::TypeArray, "native graph shape");
            if(topics)
            {
                for(int index{}; index != values.size(); ++index)
                {
                    require(values[index].size() == 2, "native topic row");
                    rows.add(static_cast<std::string>(values[index][0]), static_cast<std::string>(values[index][1]));
                }
            }
            else
            {
                require(values.size() == 3, "native system state shape");
                for(int category{}; category != 3; ++category)
                {
                    for(int index{}; index != values[category].size(); ++index)
                    {
                        auto& row{values[category][index]};
                        require(row.size() == 2, "native system state row");
                        if((which == 5u || which == 6u) && category == 2)
                        {
                            rows.add(static_cast<std::string>(row[0]), "not-applicable");
                        }
                        if(which == 3u || which == 4u)
                        {
                            for(int node{}; node != row[1].size(); ++node) { rows.add(static_cast<std::string>(row[1][node])); }
                        }
                    }
                }
            }
        }
        return {0, rows.result()};
    }
#else
    /** @brief Require the real asynchronous native operation before inspection. */
    template <class Future>
    auto completed(Future future)
    {
        require(future.wait_for(std::chrono::seconds(10)) == std::future_status::ready, "native parameter deadline");
        return future.get();
    }

    /** @brief Query retained original ROS2 nodes and parameter clients. */
    ipc::call_result native_query(unsigned which, rclcpp::Node::SharedPtr const& node,
                                  rclcpp::AsyncParametersClient::SharedPtr const& parameters)
    {
        if(which == 7u)
        {
            auto values{completed(parameters->get_parameters({"value"}))};
            require(values.size() == 1u && values[0].get_type() == rclcpp::ParameterType::PARAMETER_STRING, "native parameter type");
            return {0, text_result(values[0].as_string())};
        }
        if(which == 8u)
        {
            auto values{completed(parameters->set_parameters({rclcpp::Parameter{"value", "ok"}}))};
            require(values.size() == 1u && values[0].successful, "native parameter set");
            return {};
        }
        if(which == 10u)
        {
            auto values{completed(parameters->describe_parameters({"value"}))};
            require(values.size() == 1u, "native parameter descriptor count");
            auto const& value{values[0]};
            return {0, text_result("name=" + value.name + "\ntype=" + std::to_string(value.type) + "\nread_only=" +
                                    (value.read_only ? "true" : "false") + "\ndescription=" + value.description +
                                    "\nconstraints=" + value.additional_constraints)};
        }
        graph_rows rows{which, {}};
        if(which == 9u)
        {
            auto values{completed(parameters->list_parameters({}, 10))};
            for(auto name: values.names)
            {
                std::replace(name.begin(), name.end(), '.', '/');
                rows.add(std::string{UWVM_BENCH_NODE "/"} + name);
            }
        }
        else if(which == 3u || which == 4u)
        {
            for(auto const& name: node->get_node_names()) { rows.add(name); }
        }
        else
        {
            auto names{which == 1u || which == 2u ? node->get_topic_names_and_types() : node->get_service_names_and_types()};
            for(auto const& item: names)
            {
                for(auto const& type: item.second) { rows.add(item.first, type); }
            }
        }
        return {0, rows.result()};
    }
#endif
}  // namespace

int main(int argc, char** argv)
try
{
    require(argc >= 2, "benchmark mode");
    std::string const mode{argv[1]};
    if(mode == "ipc")
    {
        require(argc == 5, "IPC arguments");
        auto const count{static_cast<unsigned>(gateway::integer(argv[2], 1000000))};
        auto const which{static_cast<unsigned>(gateway::integer(argv[4], 12))};
        require(count != 0u && which != 0u, "nonzero measurement count/case");
        auto const policy{gateway::load(argv[3])};
        require(policy.policy.capabilities.at("client").operation == resource_operation(which), "exact operation policy");
        ipc::client connection;
        require(connection.connect(policy.socket, UWVM_ROS_BENCH_VERSION, policy.worker_uid, 1000u) == 0u, "IPC connect");
        require(connection.call("session.bind", "target", uwvm2::imported::ros::serialize_manifest(policy.policy), 10000u).error == 0u,
                "IPC authenticated policy bind");
        bytes const request{which == 8u ? bytes{2, 0, 0, 0, 'o', 'k'} : bytes{}};
        measure(count, which, [&] { return connection.call(resource_operation(which), "client", request, 10000u); });
        require(connection.call("capability.close", "client", {}, 10000u).error == 0u, "IPC close");
        return 0;
    }
    unsigned const count{mode == "server" ? 0u : static_cast<unsigned>(gateway::integer(argc >= 3 ? argv[2] : "0", 1000000))};
    unsigned const which{mode == "server" ? 0u : case_number(argc >= 4 ? argv[3] : "")};
    require(mode == "server" || (mode == "native" && count != 0u), "native arguments");
#if UWVM_ROS_BENCH_VERSION == 1
    require(setenv("ROS_NAMESPACE", mode == "server" ? UWVM_BENCH_SCOPE : "/uwvm_perf/client", 1) == 0, "native namespace");
    ros::init(argc, argv, mode == "server" ? "resource_peer" : "resource_native", ros::init_options::NoRosout);
    ros::NodeHandle node;
    if(mode == "server")
    {
        ros::param::set(UWVM_BENCH_VALUE, std::string{"ok"});
        auto publisher{node.advertise<std_msgs::String>(UWVM_BENCH_SCOPE "/topic", 10)};
        std::uint64_t calls{};
        auto callback{[&](std_srvs::Trigger::Request&, std_srvs::Trigger::Response& response)
        {
            ++calls;
            response.success = true;
            response.message = "ok";
            return true;
        }};
        auto service{node.advertiseService<std_srvs::Trigger::Request, std_srvs::Trigger::Response>(UWVM_BENCH_SCOPE "/trigger", callback)};
        require(static_cast<bool>(service) && static_cast<bool>(publisher), "native peer endpoints");
        std::cout << "BENCH_SERVER_READY=1\n" << std::flush;
        ros::spin();
        std::cout << "BENCH_PEER_CALLS=" << calls << '\n' << std::flush;
    }
    else { measure(count, which, [&] { return native_query(which); }); }
    ros::shutdown();
#else
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    options.enable_rosout(false);
    options.start_parameter_services(mode == "server");
    options.start_parameter_event_publisher(false);
    auto node{std::make_shared<rclcpp::Node>(mode == "server" ? "resource_peer" : "resource_native",
                                            mode == "server" ? UWVM_BENCH_SCOPE : "/uwvm_perf/client", options)};
    if(mode == "server")
    {
        node->declare_parameter<std::string>("value", "ok");
        auto publisher{node->create_publisher<std_msgs::msg::String>(UWVM_BENCH_SCOPE "/topic", rclcpp::QoS{10}.reliable())};
        std::uint64_t calls{};
        auto service{node->create_service<std_srvs::srv::Trigger>(UWVM_BENCH_SCOPE "/trigger",
            [&](std::shared_ptr<std_srvs::srv::Trigger::Request>, std::shared_ptr<std_srvs::srv::Trigger::Response> response)
            {
                ++calls;
                response->success = true;
                response->message = "ok";
            })};
        std::cout << "BENCH_SERVER_READY=1\n" << std::flush;
        rclcpp::spin(node);
        std::cout << "BENCH_PEER_CALLS=" << calls << '\n' << std::flush;
    }
    else
    {
        auto parameters{std::make_shared<rclcpp::AsyncParametersClient>(node, UWVM_BENCH_NODE)};
        rclcpp::executors::SingleThreadedExecutor executor;
        executor.add_node(node);
        std::thread spinner{[&] { executor.spin(); }};
        try
        {
            if(which >= 7u) { require(parameters->wait_for_service(std::chrono::seconds(10)), "native parameter discovery"); }
            measure(count, which, [&] { return native_query(which, node, parameters); });
        }
        catch(...)
        {
            executor.cancel();
            spinner.join();
            rclcpp::shutdown();
            throw;
        }
        executor.cancel();
        spinner.join();
    }
    rclcpp::shutdown();
#endif
    return 0;
}
catch(std::exception const& error)
{
    std::cerr << "FAIL_RESOURCE_BENCHMARK " << error.what() << '\n';
    return 1;
}
