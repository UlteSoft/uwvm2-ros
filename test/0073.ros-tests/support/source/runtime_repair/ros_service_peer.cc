/**
 * @file ros_service_peer.cc
 * @brief Provide a native ROS1 or ROS2 peer and equivalent comparison clients.
 * @details All clients use the same actual Trigger server and exact response.
 * The direct client retains its client and executor. The IPC client uses the
 * production authenticated transport and the complete immutable policy manifest;
 * it isolates gateway overhead from Wasm admission without bypassing gateway
 * validation. A supervisor-controlled input barrier excludes startup from PMU
 * collection. This executable is test infrastructure, never an SDK component.
 */
#include <gateway/common/config.h>
#include <uwvm2/imported/ros/common/manifest.h>
#ifndef UWVM_ROS_BENCH_VERSION
# define UWVM_ROS_BENCH_VERSION 2
#endif
#if UWVM_ROS_BENCH_VERSION == 1
# include <ros/ros.h>
# include <std_srvs/Trigger.h>
#else
# include <rclcpp/rclcpp.hpp>
# include <std_srvs/srv/trigger.hpp>
#endif
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
    using clock_type = std::chrono::steady_clock;
#if UWVM_ROS_BENCH_VERSION == 2
    using service_type = std_srvs::srv::Trigger;
#endif
    namespace gateway = uwvm2::ros_gateway;
    namespace ipc = uwvm2::imported::ros::ipc;

    /** @brief Reject incorrect replies and timeouts in release builds as well. */
    void require(bool condition)
    {
        if(!condition) { throw std::runtime_error("Native ROS benchmark invariant failed"); }
    }

    /** @brief Apply an identical warm-up, collection barrier and measured loop. */
    template <class Exchange>
    void measure(unsigned count, Exchange&& exchange)
    {
        for(unsigned index{}; index != 20u; ++index) { exchange(); }
        std::cerr << "BENCH_READY=" << count << '\n' << std::flush;
        require(std::cin.get() == 'R');
        auto const begin{clock_type::now()};
        for(unsigned index{}; index != count; ++index) { exchange(); }
        auto const duration{std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - begin).count()};
        std::cerr << "BENCH_BODY_NS=" << duration << "\nBENCH_ROS_COUNT=" << count << "\nBENCH_CHECKSUM=" << count << '\n';
    }
}  // namespace

int main(int argc, char** argv)
try
{
    require(argc >= 2);
    std::string const mode{argv[1]};
    if(mode == "ipc")
    {
        require(argc == 4);
        auto const count{static_cast<unsigned>(gateway::integer(argv[2], 1000000))};
        require(count != 0u);
        auto const policy{gateway::load(argv[3])};
        auto const& operation{policy.policy.capabilities.at("client").operation};
        ipc::client connection;
        require(connection.connect(policy.socket, UWVM_ROS_BENCH_VERSION, policy.worker_uid, 1000u) == 0u);
        require(connection.call("session.bind", "target", uwvm2::imported::ros::serialize_manifest(policy.policy), 10000u).error == 0u);
        measure(count,
                [&]
                {
                    auto const response{connection.call(operation, "client", {}, 10000u)};
                    require(response.error == 0u);
                    if(operation.rfind("time.read-", 0) == 0)
                    {
                        require(response.payload.size() == 8u && ipc::get(response.payload.data(), 8) != 0u);
                    }
                    else
                    {
                        require(operation == "service.call" && response.payload == std::vector<std::uint8_t>{1, 2, 0, 0, 0, 'o', 'k'});
                    }
                });
        require(connection.call("capability.close", "client", {}, 10000u).error == 0u);
        return 0;
    }
#if UWVM_ROS_BENCH_VERSION == 1
    ros::init(argc, argv, mode == "server" ? "benchmark_peer" : "benchmark_native", ros::init_options::NoRosout);
    ros::NodeHandle node;
    if(mode == "server")
    {
        std::uint64_t calls{};
        auto callback{[&calls](std_srvs::Trigger::Request&, std_srvs::Trigger::Response& response)
        {
            ++calls;
            response.success = true;
            response.message = "ok";
            return true;
        }};
        auto service{node.advertiseService<std_srvs::Trigger::Request, std_srvs::Trigger::Response>("/uwvm_perf/trigger", callback)};
        require(static_cast<bool>(service));
        std::cout << "BENCH_SERVER_READY=1\n" << std::flush;
        ros::spin();
        std::cout << "BENCH_PEER_CALLS=" << calls << '\n' << std::flush;
    }
    else
    {
        require(mode == "native" && (argc == 3 || argc == 4));
        auto const count{static_cast<unsigned>(gateway::integer(argv[2], 1000000))};
        require(count != 0u);
        if(argc == 4)
        {
            std::string const operation{argv[3]};
            require(operation == "time-ros" || operation == "time-wall");
            measure(count, [&]
            {
                auto const ns{operation == "time-ros" ? ros::Time::now().toNSec() : ros::WallTime::now().toNSec()};
                require(ns != 0u);
            });
            ros::shutdown();
            return 0;
        }
        // Persistent native connections exclude repeated TCPROS discovery from
        // the direct baseline, just as the ROS2 client retains its executor.
        auto client{node.serviceClient<std_srvs::Trigger>("/uwvm_perf/trigger", true)};
        require(client.waitForExistence(ros::Duration(10)));
        measure(count, [&]
        {
            std_srvs::Trigger call;
            require(client.call(call) && call.response.success && call.response.message == "ok");
        });
    }
    ros::shutdown();
#else
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    options.enable_rosout(false);
    options.start_parameter_services(false);
    options.start_parameter_event_publisher(false);
    auto node{std::make_shared<rclcpp::Node>(mode == "server" ? "benchmark_peer" : "benchmark_native", "/uwvm_perf", options)};
    if(mode == "server")
    {
        std::uint64_t calls{};
        auto service{node->create_service<service_type>("/uwvm_perf/trigger",
                                                        [&calls](std::shared_ptr<service_type::Request>, std::shared_ptr<service_type::Response> response)
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
        require(mode == "native" && (argc == 3 || argc == 4));
        auto const count{static_cast<unsigned>(gateway::integer(argv[2], 1000000))};
        require(count != 0u);
        if(argc == 4)
        {
            std::string const operation{argv[3]};
            require(operation == "time-ros" || operation == "time-system" || operation == "time-steady");
            measure(count, [&]
            {
                std::int64_t ns{};
                if(operation == "time-ros") { ns = node->get_clock()->now().nanoseconds(); }
                else if(operation == "time-system")
                {
                    ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                }
                else
                {
                    ns = std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now().time_since_epoch()).count();
                }
                require(ns > 0);
            });
            rclcpp::shutdown();
            return 0;
        }
        auto client{node->create_client<service_type>("/uwvm_perf/trigger")};
        require(client->wait_for_service(std::chrono::seconds(10)));
        rclcpp::executors::SingleThreadedExecutor executor;
        executor.add_node(node);
        measure(count,
                [&]
                {
                    auto pending{client->async_send_request(std::make_shared<service_type::Request>())};
                    require(executor.spin_until_future_complete(pending, std::chrono::seconds(3)) == rclcpp::FutureReturnCode::SUCCESS);
                    auto const response{pending.get()};
                    require(response && response->success && response->message == "ok");
                });
    }
    rclcpp::shutdown();
#endif
    return 0;
}
catch(std::exception const& error)
{
    std::cerr << "BENCH_FAILURE=" << error.what() << '\n';
    return 1;
}
