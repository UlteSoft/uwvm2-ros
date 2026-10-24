/* Retain the genuine SDK import-function table without invoking a ROS import.
 * Reading the copied backend's version opens no alias and grants no authority.
 */
#include <platform/ros2/c/roswasm/ros2.h>
static roswasmc_backend const *volatile selected;
int main(void) {
    selected = roswasmc_ros2_backend();
    return selected->version == 2 ? 0 : 22;
}
