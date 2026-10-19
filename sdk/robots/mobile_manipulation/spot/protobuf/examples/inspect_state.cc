/* SPDX-License-Identifier: Apache-2.0 */
#include "document.hpp"
#include <cstddef>
#include <cstdint>
namespace uwvm::spot::protobuf {
struct StateSummary {
    std::size_t batteries{},joints{},feet{},estops{},frames{},faults{},service_faults{};
    double voltage{},temperature{},foot_height{},gripper_open{};
    bool has_arm{},unsafe_to_sit{};
};
/* The complete generated object remains accessible through state.get();
 * summary fields here are examples, not a replacement/projected schema. */
std::uint32_t inspect_state(RobotState& state,void const *bytes,std::size_t size,
                           StateSummary& output) {
    auto code=state.decode(bytes,size);
    if(code) return code;
    auto const& value=*state.get();
    StateSummary next{};
    next.batteries=value.battery_states_size();
    next.joints=value.kinematic_state().joint_states_size();
    next.feet=value.foot_state_size();next.estops=value.estop_states_size();
    next.frames=value.kinematic_state().transforms_snapshot().child_to_parent_edge_map_size();
    next.faults=value.system_fault_state().faults_size();
    next.service_faults=value.service_fault_state().faults_size();
    next.has_arm=value.has_manipulator_state();
    next.gripper_open=value.manipulator_state().gripper_open_percentage();
    next.unsafe_to_sit=value.terrain_state().is_unsafe_to_sit();
    if(value.battery_states_size()) {
        auto const& battery=value.battery_states(0);
        next.voltage=battery.voltage().value();
        if(battery.temperatures_size()) next.temperature=battery.temperatures(0);
        // identifier().size() preserves NUL; do not pass c_str() to strlen.
    }
    if(value.foot_state_size()) next.foot_height=value.foot_state(0).foot_position_rt_body().z();
    output=next;return 0;
}
}
