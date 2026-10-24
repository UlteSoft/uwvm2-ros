/* SPDX-License-Identifier: Apache-2.0
 * Authored full-value local contract, not an upstream or controller test.
 */
#include "move_joint_preflight.h"
#include <stdio.h>
#include <string.h>

static unsigned checks;
#define CHECK(expression) do { ++checks; if(!(expression)) { \
    fprintf(stderr, "Doosan preflight failed at line %u: %s\n", \
        (unsigned)__LINE__, #expression); return 1; } } while(0)

int main(void)
{
    unsigned i;
    roswasmc_bounded_2_dsr_msgs2_srv_MoveJoint_Request request = {0}, original;
    roswasmc_bounded_2_dsr_msgs2_srv_MoveStop_Request stop = {0};
    uwvm_doosan_move_joint_contract contract = {0};
    contract.controller_major = contract.drcf_build_major = 2;
    contract.controller_minor = 12;
    contract.maximum_velocity_degrees_per_second = 60;
    contract.maximum_acceleration_degrees_per_second_squared = 60;
    contract.maximum_blend_radius_mm = 20;
    contract.current_monotonic_ns = 100;
    contract.deadline_monotonic_ns = 200;
    contract.maximum_state_age_ns = 100;
    for(i = 0; i < 6; ++i) { contract.lower_degrees[i] = -180; contract.upper_degrees[i] = 180; }
    request.vel = request.acc = 10;
    original = request;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 0);
    CHECK(!memcmp(&request, &original, sizeof request));
    CHECK(uwvm_doosan_validate_move_joint(NULL, &contract) == 22);
    contract.drcf_build_major = 3;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 95);
    contract.controller_major = 3;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 0);
    contract.controller_major = contract.drcf_build_major = 2;
    contract.uses_moveit = 1; contract.controller_minor = 11;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 95);
    contract.controller_minor = 12;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 0);
    contract.state_age_ns = 101;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 110);
    contract.state_age_ns = 100;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 0);
    contract.current_monotonic_ns = 200;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 110);
    contract.current_monotonic_ns = 100;
    request.pos[5] = 181;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 34);
    request = original;
    request.mode = 1; request.pos[0] = 1; contract.current_degrees[0] = 180;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 34);
    request.pos[0] = -1;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 0);
    contract.current_degrees[0] = 0; request = original;
    request.vel = 61;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 34);
    request = original; request.radius = 21;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 34);
    request = original; request.sync_type = 2;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 22);
    request = original; request.mode = -1;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 22);
    request = original; request.blend_type = 2;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 22);
    request = original; request.vel = request.acc = 0;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 22);
    request.time = 2;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 0);
    request.pos[0] = NAN;
    CHECK(uwvm_doosan_validate_move_joint(&request, &contract) == 22);
    stop.stop_mode = 77;
    CHECK(uwvm_doosan_make_move_stop(-1, &stop) == 22 && stop.stop_mode == 77);
    CHECK(uwvm_doosan_make_move_stop(4, &stop) == 22 && stop.stop_mode == 77);
    CHECK(uwvm_doosan_make_move_stop(0, NULL) == 22);
    for(i = 0; i < 4; ++i) CHECK(uwvm_doosan_make_move_stop((int32_t)i, &stop) == 0 && stop.stop_mode == (int32_t)i);
    printf("Doosan profile %d: %u local preflight checks; native/Guest/controller credit=0\n", UWVM_DOOSAN_PROFILE, checks);
    return 0;
}
