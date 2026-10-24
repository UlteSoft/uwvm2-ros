/**
 * @file native_service_reuse.cc
 * @brief Exercise the production ROS2 service adapter against a real native peer.
 * @details Count successful RCL client initializations, verify repeated responses,
 * retain a client across a request deadline, and retire it on alias close.
 */
#define main uwvm_ros2_gateway_entry_for_reuse_test
#include <gateway/ros2/main.cc>
#undef main

#include <atomic>
#include <cassert>
#include <dlfcn.h>

namespace
{
    std::atomic<unsigned> client_creations{};
    std::atomic<unsigned> peer_calls{};
    std::atomic<bool> slow_response{};
}  // namespace

/** @brief Observe the actual RCL boundary without replacing middleware behavior. */
extern "C" rcl_ret_t rcl_client_init(rcl_client_t* client,
                                     rcl_node_t const* node,
                                     rosidl_service_type_support_t const* type_support,
                                     char const* service_name,
                                     rcl_client_options_t const* options)
{
    using function = decltype(&rcl_client_init);
    static auto original{reinterpret_cast<function>(dlsym(RTLD_NEXT, "rcl_client_init"))};
    assert(original);
    auto result{original(client, node, type_support, service_name, options)};
    if(result == RCL_RET_OK) { ++client_creations; }
    return result;
}

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    options.enable_rosout(false);
    options.start_parameter_services(false);
    options.start_parameter_event_publisher(false);
    auto node{std::make_shared<gateway::ros2_compat::node_type>("client_reuse_gateway", "/uwvm_reuse", options)};
    auto peer{std::make_shared<rclcpp::Node>("client_reuse_peer", "/uwvm_reuse", options)};
    auto service{peer->create_service<std_srvs::srv::Trigger>(
        "trigger",
        [](std::shared_ptr<std_srvs::srv::Trigger::Request>, std::shared_ptr<std_srvs::srv::Trigger::Response> response)
        {
            ++peer_calls;
            if(slow_response.exchange(false)) { std::this_thread::sleep_for(std::chrono::milliseconds(80)); }
            response->success = true;
            response->message = "observed";
        })};
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node->get_node_base_interface());
    executor.add_node(peer);
    std::thread spin{[&] { executor.spin(); }};

    struct stop_executor
    {
        rclcpp::Executor& executor;
        std::thread& thread;

        ~stop_executor()
        {
            executor.cancel();
            thread.join();
            rclcpp::shutdown();
        }
    } stop{executor, spin};

    auto cleanup{std::make_shared<gateway::ros2_action::cleanup_pool>(1)};
    auto custom_cleanup{std::make_shared<gateway::ros2_custom_actions::cleanup_pool>(1)};
    ros2_adapter adapter{node, cleanup, custom_cleanup};
    capability cap{
        "trigger",
        "service.call",
        "/uwvm_reuse/trigger",
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
    auto call{[&]
              {
                  auto result{adapter.service(cap, {}, 2000)};
                  assert(result.error == 0u && result.payload.size() == 13u && result.payload[0] == 1u);
                  assert(std::string(result.payload.begin() + 5, result.payload.end()) == "observed");
              }};
    for(unsigned index{}; index != 21u; ++index) { call(); }
    assert(client_creations == 1u && peer_calls == 21u);

    slow_response = true;
    auto expired{adapter.service(cap, {}, 5)};
    assert(expired.error == 110u);
    call();
    assert(client_creations == 1u && peer_calls == 23u);

    adapter.close(cap.alias);
    call();
    assert(client_creations == 2u && peer_calls == 24u);
    adapter.close(cap.alias);
    gateway::native_io::diagnostic("PASS ", "retained-native-service-client: calls=24 clients=2 timeout=1 close=2");
}
