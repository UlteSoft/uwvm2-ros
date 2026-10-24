/* Genuine Kortex headers and final-link witness, with no transport connection. */
#include "kortex_native.hpp"
#include <cassert>
#include <limits>
template<class Member> static void retain_member(Member value){
    /* Keep actual relocations to every implemented body in the final link.
     * No member is invoked and no native client/channel is constructed. */
    Member volatile retained=value;(void)retained;
}
int main(){
    using Native=uwvm_kinova::KortexNative;
    retain_member(&Native::actuator_count);retain_member(&Native::feedback);
    retain_member(&Native::servoing_mode);retain_member(&Native::set_servoing_mode);
    retain_member(&Native::clear_faults);retain_member(&Native::stop);
    retain_member(&Native::gripper);retain_member(&Native::action);
    retain_member(&Native::execute_action);retain_member(&Native::validate_waypoints);
    retain_member(&Native::execute_waypoints);retain_member(&Native::tool_configuration);
    retain_member(&Native::set_tool_configuration);retain_member(&Native::arm_state);
    retain_member(&Native::pause_action);retain_member(&Native::resume_action);
    retain_member(&Native::stop_action);
    Kinova::Api::Base::GripperCommand command;
    command.set_mode(Kinova::Api::Base::GRIPPER_POSITION);
    auto* finger=command.mutable_gripper()->add_finger();
    finger->set_finger_identifier(1);finger->set_value(0.25f);
    std::string wire;assert(command.SerializeToString(&wire));
    Kinova::Api::Base::GripperCommand decoded;
    assert(decoded.ParseFromString(wire));
    assert(decoded.gripper().finger(0).finger_identifier()==1);
    assert(decoded.gripper().finger(0).value()==0.25f);
    uwvm_kinova::KortexNativePolicy policy;
    assert(!policy.authenticated_owner&&!policy.maximum_rpc_ms&&!policy.maximum_message_bytes);
    for(bool enabled:policy.enabled)assert(!enabled);
    Kinova::Api::ControlConfig::ToolConfiguration tool;
    tool.mutable_tool_transform()->set_x(0.05f);
    tool.mutable_tool_mass_center()->set_z(0.02f);
    tool.set_tool_mass(0.25f);
    assert(!uwvm_kinova::valid_tool_configuration(tool,policy));
    policy.maximum_tool_mass=1;policy.maximum_tool_translation=0.5;policy.maximum_tool_rotation=180;
    assert(uwvm_kinova::valid_tool_configuration(tool,policy));
    tool.mutable_tool_transform()->set_theta_z(std::numeric_limits<float>::quiet_NaN());
    assert(!uwvm_kinova::valid_tool_configuration(tool,policy));
    tool.mutable_tool_transform()->set_theta_z(181);
    assert(!uwvm_kinova::valid_tool_configuration(tool,policy));
    tool.mutable_tool_transform()->set_theta_z(0);tool.set_tool_mass(2);
    assert(!uwvm_kinova::valid_tool_configuration(tool,policy));
    /* All adapter member bodies must type-check against the real original SDK.
     * No RPC/native controller, firmware or robot test credit follows. */
    return 0;
}
