/** @file
 * @brief Bind the regression to both actual ROS1 Action correlation domains.
 * Compile against the selected genuine ROS1 SDK and current complete sources.
 * No ROS initialization, Master, Node, network or robot operation is performed.
 */
#include "../../../../../src/ros/gateway/ros1/action.h"
#include "../../../../../src/ros/gateway/ros1/custom_actions.h"
#include "action_contract.h"

int main(int argc,char** argv)
{
    try
    {
        ticket_action_test::options(argc,argv);
        namespace builtin=uwvm2::ros_gateway::ros1_action;
        namespace custom=uwvm2::ros_gateway::ros1_custom_actions;
        ticket_action_test::need(builtin::registry.empty(),"builtin ROS1 goals must start empty");
        ticket_action_test::domain(builtin::allocate_id);
        ticket_action_test::domain(custom::allocate_id);
        ticket_action_test::need(builtin::registry.empty(),"ID allocation must not create a native goal");
        ros_probe::print_fixture(STDOUT_FILENO,
            "PASS ticket-Action ROS1 domains=2 sequential_per_domain=",ticket_action_test::completed,
            " parallel_per_domain=131072 no_ROS_or_Guest_execution\n");return 0;
    }
    catch(std::exception const& error)
    { try {ros_probe::print_fixture(STDERR_FILENO,"FAIL ticket-Action ROS1 ",error.what(),"\n");}catch(...) {}return 1; }
}
