/** @file
 * @brief Bind the regression to the actual selected custom ROS2 Action domain.
 * The SDK must really provide Action support: an unavailable adapter is a build
 * failure, never a skipped test or a successful empty-registry qualification.
 * No rclcpp initialization, Node, DDS endpoint or robot operation is performed.
 */
#include "../../../../../src/ros/gateway/ros2/custom_actions.h"
#include "action_contract.h"
#if !UWVM_ROS2_HAVE_ACTION_CORE
#error "the ROS2 Action ID regression requires an actual Action-capable native SDK"
#endif

int main(int argc,char** argv)
{
    try
    {
        ticket_action_test::options(argc,argv);
        ticket_action_test::domain(uwvm2::ros_gateway::ros2_custom_actions::allocate_id);
        ros_probe::print_fixture(STDOUT_FILENO,
            "PASS ticket-Action ROS2 domains=1 sequential_per_domain=",ticket_action_test::completed,
            " parallel_per_domain=131072 no_ROS_or_Guest_execution\n");return 0;
    }
    catch(std::exception const& error)
    { try {ros_probe::print_fixture(STDERR_FILENO,"FAIL ticket-Action ROS2 ",error.what(),"\n");}catch(...) {}return 1; }
}
