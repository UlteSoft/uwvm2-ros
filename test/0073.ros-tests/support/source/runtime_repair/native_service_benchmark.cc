/**
 * @file native_service_benchmark.cc
 * @brief Compare native ROS2 service calls and the actual production adapter.
 * @details The server is a separate native process. Rotated, bounded samples
 * distinguish a retained native client, a retained production adapter client,
 * and an adapter client retired before each request. No QEMU timing is valid.
 * This measures the native adapter, excluding VM admission and authenticated IPC.
 */
#define main uwvm_ros2_gateway_entry_for_benchmark
#include <gateway/ros2/main.cc>
#undef main

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

namespace
{
    using clock_type = std::chrono::steady_clock;
    using service_type = std_srvs::srv::Trigger;

    /** @brief Stop and reap only the native peer created by this measurement. */
    struct peer_owner
    {
        pid_t pid;

        ~peer_owner()
        {
            kill(pid, SIGTERM);
            int status{};
            while(waitpid(pid, &status, 0) == -1 && errno == EINTR) {}
        }
    };

    /** @brief Keep the production executor alive through every measured reply. */
    struct executor_owner
    {
        rclcpp::Executor& executor;
        std::thread& thread;

        ~executor_owner()
        {
            executor.cancel();
            thread.join();
            rclcpp::shutdown();
        }
    };
}  // namespace

int main(int argc, char** argv)
{
    auto const name_space{std::string("/uwvm_native_bench_") + std::to_string(getpid())};
    int readiness[2];
    assert(pipe(readiness) == 0);
    auto const child{fork()};
    assert(child >= 0);
    if(child == 0)
    {
        close(readiness[0]);
        rclcpp::init(argc, argv);
        auto peer{std::make_shared<rclcpp::Node>("peer", name_space)};
        unsigned count{};
        auto server{peer->create_service<service_type>("trigger",
                                                       [&count](std::shared_ptr<service_type::Request>, std::shared_ptr<service_type::Response> reply)
                                                       {
                                                           ++count;
                                                           reply->success = true;
                                                           reply->message = "observed";
                                                       })};
        assert(write(readiness[1], "R", 1) == 1);
        close(readiness[1]);
        rclcpp::spin(peer);
        std::cout << "NATIVE_PEER_CALLS=" << count << '\n' << std::flush;
        rclcpp::shutdown();
        _exit(0);
    }
    peer_owner own_peer{child};
    close(readiness[1]);
    char ready{};
    assert(read(readiness[0], &ready, 1) == 1 && ready == 'R');
    close(readiness[0]);
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    options.enable_rosout(false);
    options.start_parameter_services(false);
    options.start_parameter_event_publisher(false);
    auto node{std::make_shared<gateway::ros2_compat::node_type>("gateway", name_space, options)};
    auto native_node{std::make_shared<rclcpp::Node>("native_client", name_space, options)};
    auto native{native_node->create_client<service_type>("trigger")};
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node->get_node_base_interface());
    executor.add_node(native_node);
    std::thread spin{[&] { executor.spin(); }};
    executor_owner stop{executor, spin};
    assert(native->wait_for_service(std::chrono::seconds(2)));
    auto cleanup{std::make_shared<gateway::ros2_action::cleanup_pool>(1)};
    auto custom_cleanup{std::make_shared<gateway::ros2_custom_actions::cleanup_pool>(1)};
    ros2_adapter adapter{node, cleanup, custom_cleanup};
    capability cap{
        "retained",
        "service.call",
        name_space + "/trigger",
        "std_srvs/srv/Trigger",
        "RIHS01_eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f",
        {{"timeout-ms", "2000"},
          {"max-request-bytes", "0"},
          {"max-response-bytes", "64"},
          {"qos.depth", "10"},
          {"qos.reliability", "reliable"},
          {"qos.durability", "volatile"},
          {"qos.deadline", "infinite"},
          {"qos.lifespan", "infinite"},
          {"qos.liveliness", "automatic"},
          {"qos.liveliness-lease-duration", "infinite"}}
    };
    auto fresh{cap};
    fresh.alias = "fresh";
    std::array<std::vector<double>, 3> samples;
    for(unsigned round{}; round != 105u; ++round)
    {
        for(unsigned offset{}; offset != 3u; ++offset)
        {
            auto const mode{(round + offset) % 3u};
            auto const begin{clock_type::now()};
            if(mode == 0u)
            {
                auto pending{native->async_send_request(std::make_shared<service_type::Request>())};
                assert(pending.wait_for(std::chrono::seconds(2)) == std::future_status::ready);
                auto reply{pending.get()};
                assert(reply && reply->success && reply->message == "observed");
            }
            else
            {
                if(mode == 2u) { adapter.close(fresh.alias); }
                auto result{adapter.service(mode == 1u ? cap : fresh, {}, 2000)};
                assert(result.error == 0u && result.payload.size() == 13u && result.payload[0] == 1u);
                assert(std::string(result.payload.begin() + 5, result.payload.end()) == "observed");
            }
            auto const elapsed{std::chrono::duration<double, std::micro>(clock_type::now() - begin).count()};
            if(round >= 5u) { samples[mode].push_back(elapsed); }
        }
    }
    char const* labels[]{"native-retained", "adapter-retained", "adapter-recreated"};
    for(unsigned mode{}; mode != 3u; ++mode)
    {
        auto& values{samples[mode]};
        assert(values.size() == 100uz);
        std::sort(values.begin(), values.end());
        std::cout << "{\"mode\":\"" << labels[mode] << "\",\"count\":100,\"median_us\":" << (values[49] + values[50]) / 2 << ",\"p95_us\":" << values[94]
                  << ",\"min_us\":" << values[0] << ",\"max_us\":" << values[99] << "}\n";
    }
    std::cout << "PASS_NATIVE_SERVICE_BENCHMARK callbacks=315\n" << std::flush;
}
