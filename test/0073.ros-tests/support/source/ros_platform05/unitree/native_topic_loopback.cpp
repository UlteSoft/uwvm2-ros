/** @file Genuine generated Unitree ROS2 Request/Response loopback witness.
 * This is a test peer, not a robot/controller/SDK replacement. It only accepts
 * absolute topics below /uwvm_task05/. A separately isolated loopback namespace,
 * finite process owner and ROS_LOCALHOST_ONLY=1 are required by the runner.
 *
 * CLI: native_topic_loopback REQUEST_TOPIC RESPONSE_TOPIC EXPECTED_REQUEST
 *                            ACTUAL_RESPONSE
 * Files contain complete canonical records, never native struct/serialized
 * bytes. The actual generated native objects are populated field by field.
 * One exact native Request reception precedes exactly one native Response
 * publication. Generated full-message equality validates every Request field.
 * No incoming publication or reply is retried. A retained observer transcript
 * is still required alongside the actual Guest/Runtime/Gateway evidence.
 */
#include <rclcpp/rclcpp.hpp>
#include <unitree_api/msg/request.hpp>
#include <unitree_api/msg/response.hpp>
#include <fast_io.h>
#include "bounded_records.h"
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <string>
#include <string_view>
#include <thread>

namespace {
using Request = unitree_api::msg::Request;
using Response = unitree_api::msg::Response;
using Clock = std::chrono::steady_clock;
struct Failure {
    unsigned line;
};
void check(bool value, unsigned line) {
    if (!value)
        throw Failure{line};
}
#define require(value) check((value), __LINE__)
bool topic(char const *name) {
    std::string_view value{name};
    if (!value.starts_with("/uwvm_task05/") || value.size() > 192 ||
        value.back() == '/')
        return false;
    for (char byte : value)
        if (!((byte >= 'a' && byte <= 'z') || (byte >= 'A' && byte <= 'Z') ||
              (byte >= '0' && byte <= '9') || byte == '_' || byte == '/'))
            return false;
    return value.find("//") == value.npos;
}
Request load_request(char const *name) {
    fast_io::native_file_loader file{fast_io::mnp::os_c_str(name)};
    require(file.size() >= 37 &&
            file.size() <= ROSWASMC_BOUNDED_2_UNITREE_API_MSG_REQUEST_MAX_WIRE_BYTES);
    roswasmc_bounded_2_unitree_api_msg_Request value{};
    require(roswasmc_bounded_2_unitree_api_msg_Request_decode(
                file.data(), file.size(), &value) == 0);
    require(value.header.identity.id > 0 && value.header.identity.api_id > 0 &&
            !value.header.policy.noreply);
    Request result;
    result.header.identity.id = value.header.identity.id;
    result.header.identity.api_id = value.header.identity.api_id;
    result.header.lease.id = value.header.lease.id;
    result.header.policy.priority = value.header.policy.priority;
    result.header.policy.noreply = value.header.policy.noreply;
    result.parameter.assign(reinterpret_cast<char const *>(value.parameter.data),
                            value.parameter.size);
    result.binary.assign(value.binary.data, value.binary.data + value.binary.size);
    return result;
}
Response load_response(char const *name) {
    fast_io::native_file_loader file{fast_io::mnp::os_c_str(name)};
    require(file.size() >= 28 &&
            file.size() <= ROSWASMC_BOUNDED_2_UNITREE_API_MSG_RESPONSE_MAX_WIRE_BYTES);
    roswasmc_bounded_2_unitree_api_msg_Response value{};
    require(roswasmc_bounded_2_unitree_api_msg_Response_decode(
                file.data(), file.size(), &value) == 0);
    Response result;
    result.header.identity.id = value.header.identity.id;
    result.header.identity.api_id = value.header.identity.api_id;
    result.header.status.code = value.header.status.code;
    result.data.assign(reinterpret_cast<char const *>(value.data.data),
                       value.data.size);
    result.binary.assign(value.binary.data, value.binary.data + value.binary.size);
    return result;
}
} // namespace

int main(int argc, char **argv) {
    try {
        require(argc == 5 && topic(argv[1]) && topic(argv[2]) &&
                std::strcmp(argv[1], argv[2]) != 0);
        require(std::getenv("ROS_LOCALHOST_ONLY") &&
                std::strcmp(std::getenv("ROS_LOCALHOST_ONLY"), "1") == 0);
        require(std::getenv("ROS_DOMAIN_ID") && *std::getenv("ROS_DOMAIN_ID"));
        auto expected = load_request(argv[3]);
        auto response = load_response(argv[4]);
        require(expected.header.identity.id == response.header.identity.id &&
                expected.header.identity.api_id == response.header.identity.api_id);
        auto context = std::make_shared<rclcpp::Context>();
        context->init(0, nullptr);
        rclcpp::NodeOptions options;
        options.context(context)
            .use_global_arguments(false)
            .enable_rosout(false)
            .start_parameter_services(false)
            .start_parameter_event_publisher(false)
            .use_intra_process_comms(false);
        auto node = std::make_shared<rclcpp::Node>(
            "unitree_task05_loopback", "/uwvm_task05", options);
        rclcpp::ExecutorOptions executor_options;
        executor_options.context = context;
        rclcpp::executors::SingleThreadedExecutor executor{executor_options};
        executor.add_node(node);
        auto qos = rclcpp::QoS(rclcpp::KeepLast(1)).reliable().durability_volatile();
        auto publisher = node->create_publisher<Response>(argv[2], qos);
        std::size_t received{}, sent{};
        bool invalid{};
        auto subscription =
            node->create_subscription<Request>(argv[1], qos, [&](Request const &value) {
                ++received;
                if (received != 1 || value != expected) {
                    invalid = true;
                    return;
                }
                fast_io::io::println(fast_io::native_stdout(),
                                     "REQUEST_ALL_FIELDS_MATCHED id=",
                                     value.header.identity.id,
                                     " api=",
                                     value.header.identity.api_id);
            });
        fast_io::io::println(fast_io::native_stdout(), "READY");
        auto until = Clock::now() + std::chrono::seconds{20};
        while (publisher->get_subscription_count() != 1 ||
               subscription->get_publisher_count() != 1) {
            require(!invalid && Clock::now() < until);
            executor.spin_some(std::chrono::milliseconds{1});
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }
        fast_io::io::println(fast_io::native_stdout(), "MATCHED");
        while (!received) {
            require(!invalid && Clock::now() < until);
            executor.spin_some(std::chrono::milliseconds{1});
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }
        require(!invalid && received == 1 && publisher->get_subscription_count() == 1);
        ++sent;
        publisher->publish(response);
        fast_io::io::println(fast_io::native_stdout(), "RESPONSE_PUBLISHED_ONCE");
        // Explicit F arrives only after the parent has checked the COMPLETE
        // Guest response. It is not an automatic publisher retry/ack substitute.
        char credit{};
        fast_io::operations::read_all(fast_io::native_stdin(), &credit, &credit + 1);
        require(credit == 'F');
        executor.spin_some(std::chrono::milliseconds{1});
        require(!invalid && received == 1 && sent == 1);
        executor.remove_node(node);
        subscription.reset();
        publisher.reset();
        node.reset();
        context->shutdown("finite loopback peer complete");
        fast_io::io::println(fast_io::native_stdout(), "DONE received=1 sent=1");
        return 0;
    } catch (Failure const &error) {
        fast_io::io::println(
            fast_io::native_stderr(), "LOOPBACK_INVARIANT_LINE ", error.line);
        return 22;
    } catch (std::exception const &error) {
        fast_io::io::println(fast_io::native_stderr(),
                             "LOOPBACK_EXCEPTION ",
                             std::string_view{error.what()});
        return 22;
    } catch (...) {
        fast_io::io::println(fast_io::native_stderr(), "LOOPBACK_UNKNOWN_FAILURE");
        return 22;
    }
}
