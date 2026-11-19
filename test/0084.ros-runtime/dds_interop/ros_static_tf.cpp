/** @file @brief Publish bounded camera frame transforms using the selected native ROS installation. */
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <tf2_msgs/msg/tf_message.hpp>
#include <charconv>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <link.h>
#include <numbers>
#include <string_view>

namespace
{
using clock_type = std::chrono::steady_clock;
using namespace std::chrono_literals;
using tf_message = tf2_msgs::msg::TFMessage;
constexpr std::uint32_t live_count = 24;

/** @brief Identify the native type support and middleware while they are loaded. */
void loaded_libraries()
{
    dl_iterate_phdr([](dl_phdr_info* entry, std::size_t, void*) {
        std::string_view const name = entry->dlpi_name;
        if(!name.empty() && name.front() == '/')
            std::cout << "LIB " << name << '\n';
        return 0;
    }, nullptr);
}

/** @brief Build a fresh, deterministic two-link frame tree with exact binary fractions. */
tf_message sample(std::int32_t token, std::uint32_t sequence)
{
    geometry_msgs::msg::TransformStamped mount;
    mount.header.stamp.sec = token;
    mount.header.stamp.nanosec = sequence;
    mount.header.frame_id = "wros_base";
    mount.child_frame_id = "wros_camera_mount";
    mount.transform.translation.x = 1.0;
    mount.transform.translation.y = 2.0;
    mount.transform.translation.z = 3.0;
    mount.transform.rotation.z = std::numbers::sqrt2 / 2;
    mount.transform.rotation.w = std::numbers::sqrt2 / 2;

    auto optical = mount;
    optical.header.frame_id = mount.child_frame_id;
    optical.child_frame_id = "wros_camera_optical";
    optical.transform.translation.x = static_cast<double>(token) / 4096 + sequence;
    optical.transform.translation.y = 4.0;
    optical.transform.translation.z = 5.0;
    optical.transform.rotation.z = 0.0;
    optical.transform.rotation.w = 1.0;

    tf_message message;
    message.transforms = {mount, optical};
    return message;
}
}

int main(int argc, char** argv)
{
    if(argc != 4)
        return 1;
    std::string_view const token_text{argv[2]};
    std::int32_t token{};
    auto const end = token_text.data() + token_text.size();
    auto const parsed = std::from_chars(token_text.data(), end, token);
    if(parsed.ec != std::errc{} || parsed.ptr != end || token < 1)
        return 1;

    rclcpp::init(0, nullptr);
    if(std::string_view(rmw_get_implementation_identifier()) != argv[1])
        return 2;
    auto node = rclcpp::Node::make_shared("wros_static_tf_source");
    auto qos = rclcpp::QoS(1).reliable();
    std::string_view const durability{argv[3]};
    if(durability == "volatile")
        qos.durability_volatile();
    else if(durability == "transient")
        qos.transient_local();
    else
        return 3;
    auto writer = node->create_publisher<tf_message>("/tf_static", qos);
    if(writer->get_actual_qos().durability() != qos.durability())
        return 3;
    std::cout << "WRITER_QOS " << durability << '\n';
    loaded_libraries();

    auto publish = [&](std::uint32_t sequence) {
        auto const next = sample(token, sequence);
        auto const time = std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now().time_since_epoch());
        writer->publish(next);
        std::cout << "PUBLISHED " << sequence << ' ' << time.count() << std::endl;
    };
    if(writer->get_subscription_count() != 0)
        return 3;
    std::cout << "SUBSCRIBERS_BEFORE_HISTORY 0\n";
    publish(0);
    for(std::uint32_t sequence = 1; sequence <= live_count; ++sequence)
    {
        char command{};
        if(!(std::cin >> command) || command == 'x')
            break;
        if(command != 'p' || writer->get_subscription_count() != 1)
            return 4;
        publish(sequence);
    }
    rclcpp::shutdown();
    std::cout << "PUBLISHER_EXIT\n";
}
