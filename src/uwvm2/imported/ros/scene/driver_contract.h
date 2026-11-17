/** @file
 * @brief Attachment-only driver imports under uwvm_ros_v2, default hidden.
 * @details Same seven-argument checked carrier as other scene imports. Each
 * capability selects one immutable external operation contract. Connect and
 * disconnect affect only a module's channel, never driver process lifetime.
 */
#pragma once
#define UWVM_ROS_DRIVER_FUNCTIONS(X) \
 X(driver_connect,connect,"driver.connect") \
 X(driver_submit,submit,"driver.submit") \
 X(driver_result,result,"driver.result") \
 X(driver_describe,describe,"driver.describe") \
 X(driver_disconnect,disconnect,"driver.disconnect")
#ifndef UWVM_SCENE_DRIVER_CONTRACT_MACROS_ONLY
#include <array>
#include <string_view>
namespace uwvm2::imported::ros::scene::drivers
{
    enum class operation:unsigned{connect,submit,result,describe,disconnect};
    inline constexpr std::array<std::string_view,5> names{"driver.connect","driver.submit","driver.result","driver.describe","driver.disconnect"};
}
#endif
