/* Genuine Kortex headers and final-link witness, with no transport connection. */
#include "kortex_native.hpp"
#include <cassert>
int main(){
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
    /* All adapter member bodies must type-check against the real original SDK.
     * No RPC/native controller, firmware or robot test credit follows. */
    return 0;
}
