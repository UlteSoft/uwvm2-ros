/* SPDX-License-Identifier: Apache-2.0
 * Authored local preflight contracts. No ROS peer, mock import or robot call.
 * Compile and execute only within the admitted Linux resource owner.
 */
#include "trajectory_preflight.h"
#include <stdio.h>
#include <stdlib.h>

typedef roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal Goal;
static Goal goal, original;
static uwvm_yaskawa_trajectory_contract contract;
static uwvm_yaskawa_preflight_report report;
static unsigned checks;
static unsigned char const names[7][3] = {"j1", "j2", "j3", "j4", "j5", "j6", "j7"};
static uwvm_yaskawa_joint_name joint_names[7];
static double lower[7], upper[7], current[7];

#define CHECK(expression) do { ++checks; if(!(expression)) { \
    fprintf(stderr, "preflight contract failed at line %u: %s\n", \
        (unsigned)__LINE__, #expression); return 1; } } while(0)

static void fixture(enum uwvm_yaskawa_route route, size_t count)
{
    size_t j, p;
    memset(&goal, 0, sizeof goal);
    memset(&contract, 0, sizeof contract);
    for(j = 0; j < count; ++j) {
        joint_names[j].data = names[j]; joint_names[j].size = 2;
        lower[j] = -3; upper[j] = 3; current[j] = 0;
        goal.trajectory.joint_names.data[j].size = 2;
        memcpy(goal.trajectory.joint_names.data[j].data, names[j], 2);
    }
    goal.trajectory.joint_names.size = (uint32_t)count;
    goal.trajectory.points.size = 2;
    for(p = 0; p < 2; ++p) {
        goal.trajectory.points.data[p].positions.size = (uint32_t)count;
        goal.trajectory.points.data[p].velocities.size = (uint32_t)count;
        goal.trajectory.points.data[p].accelerations.size = (uint32_t)count;
        goal.trajectory.points.data[p].time_from_start.sec = (int32_t)p;
        for(j = 0; j < count; ++j) goal.trajectory.points.data[p].positions.data[j] = p ? .1 : 0;
    }
    contract.route = route;
    contract.joint_count = count;
    contract.joint_names = joint_names;
    contract.lower_position = lower; contract.upper_position = upper; contract.current_position = current;
    contract.first_position_tolerance = .001;
    contract.maximum_state_age_ns = 1000;
    contract.maximum_points = 16;
    original = goal;
}

static uint32_t validate(void)
{
    return uwvm_yaskawa_validate_trajectory(&goal, &contract, &report);
}

int main(void)
{
    fixture(uwvm_yaskawa_motoros2_dx200, 6);
    CHECK(validate() == 0 && report.reason == uwvm_yaskawa_preflight_ok);
    CHECK(!memcmp(&goal, &original, sizeof goal));
    CHECK(uwvm_yaskawa_validate_trajectory(&goal, &contract,
        (uwvm_yaskawa_preflight_report*)(void*)&goal) == 22);
    CHECK(!memcmp(&goal, &original, sizeof goal));
    CHECK(uwvm_yaskawa_validate_trajectory(NULL, &contract, &report) == 22);
    CHECK(uwvm_yaskawa_validate_trajectory(&goal, NULL, &report) == 22);
    CHECK(uwvm_yaskawa_validate_trajectory(&goal, &contract, NULL) == 0);
    contract.route = (enum uwvm_yaskawa_route)0;
    CHECK(validate() == 95);
    contract.route = uwvm_yaskawa_motoros2_dx200;
    contract.state_age_ns = 1001;
    CHECK(validate() == 110 && report.reason == uwvm_yaskawa_preflight_stale_state);
    contract.state_age_ns = 1000;
    CHECK(validate() == 0);
    contract.maximum_state_age_ns = 0;
    CHECK(validate() == 22);
    contract.maximum_state_age_ns = 1000;
    contract.maximum_points = 1;
    CHECK(validate() == 90);
    contract.maximum_points = 16;
    goal.trajectory.joint_names.data[0].data[1] = '9';
    CHECK(validate() == 22 && report.reason == uwvm_yaskawa_preflight_joint_order);
    goal = original;
    joint_names[1] = joint_names[0];
    goal.trajectory.joint_names.data[1] = goal.trajectory.joint_names.data[0];
    CHECK(validate() == 22);
    fixture(uwvm_yaskawa_motoros2_dx200, 6);
    goal.trajectory.points.size = 0;
    CHECK(validate() == 90);
    goal = original;
    goal.trajectory.points.data[0].positions.size = 0;
    CHECK(validate() == 22 && report.reason == uwvm_yaskawa_preflight_array_shape);
    goal = original;
    goal.trajectory.points.data[1].velocities.size = 1;
    CHECK(validate() == 22);
    goal = original;
    goal.trajectory.points.data[1].positions.data[0] = 4;
    CHECK(validate() == 34 && report.point == 1 && report.joint == 0);
    goal = original;
    lower[0] = 4;
    CHECK(validate() == 22);
    lower[0] = -3;
    current[0] = NAN;
    CHECK(validate() == 22);
    current[0] = 0;
    goal.trajectory.points.data[1].positions.data[0] = NAN;
    CHECK(validate() == 22 && report.reason == uwvm_yaskawa_preflight_source_value);
    goal = original;
    goal.trajectory.points.data[1].time_from_start.sec = 0;
    CHECK(validate() == 22 && report.reason == uwvm_yaskawa_preflight_duration);
    goal = original;
    goal.trajectory.points.data[1].time_from_start.nanosec = 1000000000;
    CHECK(validate() == 22);
    goal = original;
    goal.goal_time_tolerance.sec = -1;
    CHECK(validate() == 22);
    goal = original;
    goal.trajectory.header.stamp.nanosec = 1000000000;
    CHECK(validate() == 22);
    goal = original;
    goal.multi_dof_trajectory.joint_names.size = 1;
    CHECK(validate() == 95);
    goal = original;
    goal.component_path_tolerance.size = 1;
    CHECK(validate() == 95);
    goal = original;
    goal.path_tolerance.size = 1;
    goal.path_tolerance.data[0].name.size = 2;
    memcpy(goal.path_tolerance.data[0].name.data, "j1", 2);
    goal.path_tolerance.data[0].position = -1;
    CHECK(validate() == 0);
    goal.path_tolerance.data[0].position = -.5;
    CHECK(validate() == 22 && report.reason == uwvm_yaskawa_preflight_tolerance);
    goal.path_tolerance.data[0].position = 0;
    goal.path_tolerance.size = 2;
    goal.path_tolerance.data[1] = goal.path_tolerance.data[0];
    CHECK(validate() == 22);
    goal.path_tolerance.size = 1;
    goal.path_tolerance.data[0].name.data[1] = '9';
    CHECK(validate() == 22);
    goal = original;
    goal.trajectory.points.data[1].velocities.data[0] = .1;
    CHECK(validate() == 22);
    goal = original;
    goal.trajectory.points.data[1].accelerations.data[0] = .1;
    CHECK(validate() == 22);
    goal = original;
    goal.trajectory.points.data[0].positions.data[0] = .1;
    CHECK(validate() == 22);
    goal = original;
    goal.path_tolerance.size = UINT32_MAX;
    CHECK(validate() == 22);
    goal = original;
    CHECK(validate() == 0 && !memcmp(&goal, &original, sizeof goal));
    fixture(uwvm_yaskawa_motoros2_yrc1000, 6);
    CHECK(validate() == 0);
    fixture(uwvm_yaskawa_motoros2_yrc1000micro, 6);
    CHECK(validate() == 0);
    printf("yaskawa: %u authored local preflight checks passed; ROS/Wasm/native/controller credit=0\n", checks);
    return 0;
}
