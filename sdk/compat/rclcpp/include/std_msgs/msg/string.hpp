/** Complete value surface used by the pinned official minimal String examples.
 * Opt-in include mapping only. This is not native ROS generated type support,
 * binary rclcpp ABI compatibility, an RIHS provider or a native SDK replacement.
 */
#pragma once
#include <memory>
#include <string>
namespace std_msgs::msg {
struct String {
    using SharedPtr=std::shared_ptr<String>;
    using ConstSharedPtr=std::shared_ptr<String const>;
    std::string data;
};
}
