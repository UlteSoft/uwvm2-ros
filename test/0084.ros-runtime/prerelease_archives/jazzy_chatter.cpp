/** Observe the legacy publisher or publish a run-specific String for its subscriber. */
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <set>
#include <string>
#include <string_view>
#include <thread>

int main(int argc, char **argv)
{
    using namespace std::chrono_literals;
    using message = std_msgs::msg::String;
    if (argc != 3) return 1;
    bool receiving = std::string_view(argv[1]) == "receive";
    if (!receiving && std::string_view(argv[1]) != "send") return 1;
    std::string payload = argv[2];
    rclcpp::init(argc,argv);
    if (std::string_view(rmw_get_implementation_identifier()) != "rmw_fastrtps_cpp") return 2;
    auto node = rclcpp::Node::make_shared("legacy_string_witness");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    rclcpp::Subscription<message>::SharedPtr reader;
    rclcpp::Publisher<message>::SharedPtr writer;
    std::set<std::string> seen;
    if (receiving) {
        reader = node->create_subscription<message>("/chatter",rclcpp::QoS(10).reliable(),
            [&](message::ConstSharedPtr incoming) {
                if (incoming->data.size() <= 256 && seen.insert(incoming->data).second) {
                    std::fputs("received ",stdout);
                    for (unsigned char c : incoming->data) std::printf("%02x",c);
                    std::putchar('\n');
                    std::fflush(stdout);
                }
            });
    } else {
        writer = node->create_publisher<message>("/chatter",rclcpp::QoS(10).reliable());
    }
    unsigned sent = 0;
    auto start = std::chrono::steady_clock::now();
    auto next_send = start + 1s;
    auto deadline = start + 8s;
    size_t max_matches = 0;
    while (std::chrono::steady_clock::now() < deadline && rclcpp::ok()) {
        if (receiving && seen.size() >= 3) break;
        if (writer) {
            max_matches = std::max(max_matches, writer->get_subscription_count());
        }
        if (writer && std::chrono::steady_clock::now() >= next_send) {
            message m;
            m.data = payload;
            writer->publish(m);
            ++sent;
            next_send += 500ms;
            if (sent == 10) break;
        }
        executor.spin_some();
        std::this_thread::sleep_for(40ms);
    }
    if (writer) std::printf("sent %u\nmatched subscriptions %zu\n",sent,max_matches);
    rclcpp::shutdown();
    return (receiving ? seen.size() >= 3 : sent == 10) ? 0 : 3;
}
