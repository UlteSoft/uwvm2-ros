/* SPDX-License-Identifier: Apache-2.0
 * A reusable real typed consumer, not a fake telemetry producer. The caller
 * receives native_proto through a successfully decoded SpotState ROS response,
 * separately checks provider_error/unknown_outcome, then calls this function.
 * Bank storage and all borrowed values stay owned by that caller. */
#include "robot_state.h"
#include <stdint.h>
#include <string.h>
typedef struct uwvm_spot_state_summary {
    size_t joints,batteries,feet,estops,faults,frames,service_faults;
    double first_voltage,first_temperature,first_foot_height;
    int32_t motor_power;
    bool has_kinematics,has_arm,unsafe_to_sit;
} uwvm_spot_state_summary;
uint32_t uwvm_spot_inspect_state(uwvm_spot_proto_document *document,
                                void const *bytes,size_t size,
                                uwvm_spot_state_summary *out) {
    if(!out) return 22;
    uint32_t code=uwvm_spot_robot_state_decode(document,bytes,size);
    if(code) return code;
    bosdyn_api_RobotState const *s=uwvm_spot_robot_state_value(document);
    if(!s) return 5;
    uwvm_spot_state_summary next={0};
    next.batteries=s->battery_states_count;
    next.feet=s->foot_state_count;
    next.estops=s->estop_states_count;
    next.has_arm=s->manipulator_state!=NULL;
    if(s->power_state&&s->power_state->motor_power_state)
        next.motor_power=*s->power_state->motor_power_state;
    if(s->kinematic_state) {
        next.has_kinematics=true;next.joints=s->kinematic_state->joint_states_count;
        if(s->kinematic_state->transforms_snapshot)
            next.frames=s->kinematic_state->transforms_snapshot->child_to_parent_edge_map_count;
    }
    if(s->system_fault_state) next.faults=s->system_fault_state->faults_count;
    if(s->service_fault_state) next.service_faults=s->service_fault_state->faults_count;
    if(s->battery_states_count) {
        bosdyn_api_BatteryState const *b=&s->battery_states[0];
        if(b->voltage&&b->voltage->value) next.first_voltage=*b->voltage->value;
        if(b->temperatures_count) next.first_temperature=b->temperatures[0];
        /* b->identifier is pb_bytes_array_t*: explicit size, not strlen.
         * Thus a genuine protobuf identifier containing NUL is preserved. */
    }
    if(s->foot_state_count&&s->foot_state[0].foot_position_rt_body&&
       s->foot_state[0].foot_position_rt_body->z)
        next.first_foot_height=*s->foot_state[0].foot_position_rt_body->z;
    if(s->terrain_state&&s->terrain_state->is_unsafe_to_sit)
        next.unsafe_to_sit=*s->terrain_state->is_unsafe_to_sit;
    *out=next;return 0;
}
