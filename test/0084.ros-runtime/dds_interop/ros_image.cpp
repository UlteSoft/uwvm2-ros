/** @file @brief Exchange complete native ROS Image messages and check admission controls. */
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <sensor_msgs/msg/image.hpp>
#include <charconv>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <link.h>
#include <string_view>
#include <system_error>

namespace
{
using image = sensor_msgs::msg::Image;
using clock_type = std::chrono::steady_clock;
using namespace std::chrono_literals;
constexpr std::uint32_t samples_per_size = 36;
constexpr std::uint32_t sample_count = 2 * samples_per_size;
constexpr char request_topic[] = "/wros_image/request";
constexpr char reply_topic[] = "/wros_image/reply";

enum class scenario { reliable, incompatible, wrong_step };

/** @brief Emit executable library identities while the middleware is active. */
void loaded_libraries()
{
    dl_iterate_phdr([](dl_phdr_info* entry, std::size_t, void*) {
        std::string_view const name = entry->dlpi_name;
        if(!name.empty() && name.front() == '/')
            std::printf("LIB %s\n", entry->dlpi_name);
        return 0;
    }, nullptr);
}

std::uint8_t pixel(std::int32_t token, std::uint32_t sequence, std::uint32_t offset) noexcept
{
    std::uint32_t state = std::uint32_t(token) ^ (sequence * 0x9e3779b9u) ^ (offset * 0x85ebca6bu);
    state ^= state >> 13;
    state *= 0xc2b2ae35u;
    return static_cast<std::uint8_t>(state >> 24);
}

image frame(std::int32_t token, std::uint32_t sequence)
{
    image value;
    bool const small = sequence <= samples_per_size;
    value.width = small ? 120u : 640u;
    value.height = small ? 16u : 480u;
    value.step = value.width * 3u;
    value.encoding = "rgb8";
    value.header.frame_id = "wros_image_request";
    value.header.stamp.sec = token;
    value.header.stamp.nanosec = sequence;
    value.data.resize(value.height * value.step);
    for(std::uint32_t i = 0; i < value.data.size(); ++i)
        value.data[i] = pixel(token, sequence, i);
    return value;
}

/** @brief Check bounded image shape and exact content before constructing a reply. */
bool valid(image const& value, std::int32_t token, std::uint32_t sequence, bool echo,
           std::uint32_t step_delta = 0) noexcept
{
    if(sequence == 0 || sequence > sample_count)
        return false;
    bool const small = sequence <= samples_per_size;
    if(value.width != (small ? 120u : 640u) || value.height != (small ? 16u : 480u) ||
       value.step != value.width * 3u + step_delta || value.encoding != "rgb8" || value.is_bigendian != 0 ||
       value.header.stamp.sec != token || value.header.stamp.nanosec != sequence ||
       value.header.frame_id != (echo ? "wros_image_request" : "wros_image_reply") ||
       value.data.size() != value.height * value.width * 3u)
        return false;
    for(std::uint32_t i = 0; i < value.data.size(); ++i)
        if(value.data[i] != pixel(token, sequence, i))
            return false;
    return true;
}
}

int main(int argc, char** argv)
{
    if(argc != 5)
        return 1;
    std::string_view const role = argv[1], mode = argv[3], text = argv[4];
    bool const echo = role == "echo";
    if((!echo && role != "request") ||
       (mode != "reliable" && mode != "incompatible" && mode != "wrong-step"))
        return 1;
    scenario const test = mode == "reliable" ? scenario::reliable :
                          mode == "incompatible" ? scenario::incompatible : scenario::wrong_step;
    std::int32_t token{};
    auto const end = text.data() + text.size();
    auto const parsed = std::from_chars(text.data(), end, token);
    if(parsed.ec != std::errc{} || parsed.ptr != end || token < 1)
        return 1;
    rclcpp::init(0, nullptr);
    if(std::string_view(rmw_get_implementation_identifier()) != argv[2])
        return 2;

    auto node = rclcpp::Node::make_shared(echo ? "wros_image_echo" : "wros_image_request");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    rclcpp::QoS writer_qos(4);
    if(!echo && test == scenario::incompatible)
        writer_qos.best_effort();
    auto writer = node->create_publisher<image>(echo ? reply_topic : request_topic, writer_qos);
    std::uint32_t received{};
    bool failed{}, rejected{}, reliability_event{};
    rclcpp::SubscriptionOptions options;
    options.event_callbacks.incompatible_qos_callback = [&](rclcpp::QOSRequestedIncompatibleQoSInfo& info) {
        reliability_event |= info.last_policy_kind == RMW_QOS_POLICY_RELIABILITY;
    };
    auto reader = node->create_subscription<image>(echo ? request_topic : reply_topic, rclcpp::QoS(4),
        [&](image::ConstSharedPtr value) {
            std::uint32_t const sequence = received + 1;
            if(test == scenario::incompatible || (!echo && test == scenario::wrong_step))
            {
                failed = true;
                return;
            }
            if(echo && test == scenario::wrong_step)
            {
                rejected = !rejected && !valid(*value, token, 1, true) && valid(*value, token, 1, true, 1);
                failed = !rejected;
                return;
            }
            if(!valid(*value, token, sequence, echo))
            {
                failed = true;
                return;
            }
            ++received;
            if(echo)
            {
                image response = *value;
                response.header.frame_id = "wros_image_reply";
                writer->publish(response);
            }
        }, options);

    auto discovered = [&] {
        if(test != scenario::incompatible)
            return writer->get_subscription_count() == 1 && reader->get_publisher_count() == 1;
        // Verify graph visibility even though reliability prohibits the request match.
        return node->count_publishers(request_topic) == 1 && node->count_subscribers(request_topic) == 1 &&
               (echo ? writer->get_subscription_count() == 1 && reader->get_publisher_count() == 0 :
                       writer->get_subscription_count() == 0 && reader->get_publisher_count() == 1);
    };
    auto const discovery_deadline = clock_type::now() + 8s;
    while(!failed && !discovered() && clock_type::now() < discovery_deadline)
        executor.spin_once(5ms);
    bool complete = !failed && discovered();
    if(!complete)
        return 3;
    loaded_libraries();

    if(test == scenario::reliable)
    {
        if(echo)
        {
            auto const deadline = clock_type::now() + 18s;
            while(!failed && received < sample_count && clock_type::now() < deadline)
                executor.spin_once(5ms);
            // The requester independently validates delivery. Keep the reply
            // writer alive across the final publish and check for extra input.
            auto const linger = clock_type::now() + 500ms;
            while(!failed && received == sample_count && clock_type::now() < linger)
                executor.spin_once(5ms);
            complete = !failed && received == sample_count;
        }
        else
        {
            for(std::uint32_t sequence = 1; sequence <= sample_count && !failed; ++sequence)
            {
                auto const request = frame(token, sequence);
                auto const start = clock_type::now();
                writer->publish(request);
                while(!failed && received < sequence && clock_type::now() < start + 3s)
                    executor.spin_once(5ms);
                if(failed || received != sequence)
                    break;
                auto const time = std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - start);
                std::printf("FRAME %u %zu %lld\n", sequence, request.data.size(), static_cast<long long>(time.count()));
            }
            complete = !failed && received == sample_count;
        }
    }
    else
    {
        if(!echo)
        {
            image request = frame(token, 1);
            if(test == scenario::wrong_step)
                ++request.step;
            writer->publish(request);
            std::puts("PUBLISHED 1");
        }
        auto const deadline = clock_type::now() + 750ms;
        while(!failed && clock_type::now() < deadline)
            executor.spin_once(5ms);
        bool const confirmed = test == scenario::incompatible ? reliability_event : rejected;
        complete = !failed && received == 0 && (!echo || confirmed);
        if(echo && confirmed)
            std::puts(test == scenario::incompatible ? "RELIABILITY_MISMATCH" : "WRONG_STEP");
    }
    rclcpp::shutdown();
    if(!complete)
        return 5;
    std::printf("COMPLETE %s %s %u\n", argv[1], argv[3], received);
}
