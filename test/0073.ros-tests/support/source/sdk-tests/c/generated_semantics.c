/* Native-only consumer checks for the documented representation/protocol boundary. */
#include "bounded_records.h"
#include <stdio.h>

#define CHECK(expression) do { ++checks; if (!(expression)) { \
    fprintf(stderr, "generated semantics check failed at line %d\n", __LINE__); \
    return 1; } } while (0)

static unsigned checks;

int main(void)
{
    roswasmc_bounded_2_geometry_msgs_msg_Quaternion q = {0};
    roswasmc_bounded_2_geometry_msgs_msg_Quaternion next_q = {0};
    roswasmc_bounded_2_sensor_msgs_msg_JointState state = {0};
    roswasmc_bounded_2_sensor_msgs_msg_JointState next_state = {0};
    roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result outcome = {0};
    roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result next_outcome = {0};
    roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request force_mode = {0};
    roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request slider = {0};
    roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response business = {0};
    roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response next_business = {0};
    roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request empty = {0};
    unsigned char quaternion_wire[32];
    unsigned char joint_wire[ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MAX_WIRE_BYTES];
    unsigned char result_wire[ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_MAX_WIRE_BYTES];
    unsigned char business_wire[1];
    uwvm_ros_u64 nan_bits = UWVM_ROS_U64_C(0x7ff8000000000000);
    size_t length = 0;
    unsigned i;

    CHECK(q.x == 0 && q.y == 0 && q.z == 0 && q.w == 0);
    CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(&q));
    CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_encode(&q, quaternion_wire, sizeof quaternion_wire, &length) == 0 && length == 32);
    for (i = 0; i < sizeof quaternion_wire; ++i) CHECK(quaternion_wire[i] == 0);
    CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&q) == 0);
    CHECK(q.x == 0 && q.y == 0 && q.z == 0 && q.w == 1);
    CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(0) == UWVM_BOUNDED_INVALID);
    CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_encode(&q, quaternion_wire, sizeof quaternion_wire, &length) == 0 && length == 32);
    CHECK(quaternion_wire[30] == 0xf0 && quaternion_wire[31] == 0x3f);
    for (i = 0; i < 30; ++i) CHECK(quaternion_wire[i] == 0);
    q.x = 2;
    CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(&q));
    CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_encode(&q, quaternion_wire, sizeof quaternion_wire, &length) == 0);
    CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion_decode(quaternion_wire, length, &next_q) == 0);
    CHECK(next_q.x == 2 && next_q.y == 0 && next_q.z == 0 && next_q.w == 1);
    uwvm_bounded_copy(&q.x, &nan_bits, sizeof q.x);
    CHECK(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(&q));

    state.name.size = 2;
    state.name.data[0].size = 1;
    state.name.data[0].data[0] = 'j';
    state.name.data[1].size = 1;
    state.name.data[1].data[0] = 'j';
    state.position.size = 1;
    state.position.data[0] = 1.25;
    CHECK(roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&state));
    CHECK(roswasmc_bounded_2_sensor_msgs_msg_JointState_encode(&state, joint_wire, sizeof joint_wire, &length) == 0);
    CHECK(roswasmc_bounded_2_sensor_msgs_msg_JointState_decode(joint_wire, length, &next_state) == 0);
    CHECK(next_state.name.size == 2 && next_state.position.size == 1);
    CHECK(next_state.name.data[0].data[0] == 'j' && next_state.name.data[1].data[0] == 'j');
    CHECK(next_state.position.data[0] == 1.25 && next_state.velocity.size == 0 && next_state.effort.size == 0);
    state.position.size = 0;
    CHECK(roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&state));

    CHECK(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_SUCCESSFUL == 0);
    CHECK(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_INVALID_GOAL == -1);
    CHECK(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_INVALID_JOINTS == -2);
    CHECK(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_OLD_HEADER_TIMESTAMP == -3);
    CHECK(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_PATH_TOLERANCE_VIOLATED == -4);
    CHECK(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_GOAL_TOLERANCE_VIOLATED == -5);
    outcome.error_code = -7;
    CHECK(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_valid(&outcome));
    CHECK(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_encode(&outcome, result_wire, sizeof result_wire, &length) == 0 && length == 8);
    CHECK(result_wire[0] == 0xf9 && result_wire[1] == 0xff && result_wire[2] == 0xff && result_wire[3] == 0xff);
    CHECK(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_decode(result_wire, length, &next_outcome) == 0);
    CHECK(next_outcome.error_code == -7 && next_outcome.error_string.size == 0);

    CHECK(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_default(&force_mode) == 0);
    CHECK(force_mode.type == 2);
    CHECK(force_mode.selection_vector_x == 0 && force_mode.selection_vector_y == 0 && force_mode.selection_vector_z == 0 && force_mode.selection_vector_rx == 0 && force_mode.selection_vector_ry == 0 && force_mode.selection_vector_rz == 0);
    CHECK(force_mode.task_frame.pose.orientation.w == 1);
    CHECK(force_mode.gain_scaling == (float)0.5);
    CHECK(force_mode.damping_factor == (float)0.025);
    for (i = 0; i < 6; ++i) CHECK(force_mode.deviation_limits[i] == (float)0.01);
    CHECK(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_valid(&force_mode));
    force_mode.gain_scaling = 3;
    force_mode.type = 0;
    CHECK(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_valid(&force_mode));
    slider.speed_slider_fraction = 2;
    CHECK(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_valid(&slider));
    CHECK(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_valid(&business));
    CHECK(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_encode(&business, business_wire, sizeof business_wire, &length) == 0 && length == 1 && business_wire[0] == 0);
    CHECK(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_decode(business_wire, length, &next_business) == 0 && next_business.success == 0);
    business.success = 2;
    CHECK(!roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_valid(&business));
    empty.local_empty_placeholder = 77;
    CHECK(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_wire_size(&empty) == 0);
    length = 99;
    CHECK(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_encode(&empty, 0, 0, &length) == 0 && length == 0);
    CHECK(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_decode(0, 0, &empty) == 0);
    printf("C_GENERATED_SEMANTIC_CHECKS=%u\n", checks);
    return 0;
}
