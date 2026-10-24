#include <assert.h>
#include <string.h>
#include "application.h"
int main(void){
#if UWVM_TROSSEN_PROFILE<=7
    uwvm_trossen_robot_info info={0};UWVM_TROSSEN_COMMAND command={0};
    info.num_joints=1;info.mode.size=8;memcpy(info.mode.data,"position",8);
    info.joint_names.size=1;info.joint_names.data[0].size=5;
    memcpy(info.joint_names.data[0].data,"waist",5);
    info.joint_ids.size=1;info.joint_ids.data[0]=1;
    info.joint_state_indices.size=1;info.joint_state_indices.data[0]=0;
    info.joint_lower_limits.size=1;info.joint_lower_limits.data[0]=-1;
    info.joint_upper_limits.size=1;info.joint_upper_limits.data[0]=1;
    info.joint_velocity_limits.size=1;info.joint_velocity_limits.data[0]=0.2f;
    info.name.size=1;info.name.data[0].size=3;memcpy(info.name.data[0].data,"arm",3);
    command.name.size=3;memcpy(command.name.data,"arm",3);command.cmd.size=1;command.cmd.data[0]=0.5f;
    assert(!uwvm_trossen_validate_group(&info,&command,(unsigned char const*)"arm",3));
    assert(uwvm_trossen_validate_group(&info,&command,(unsigned char const*)"bad",3)==22);
    command.cmd.data[0]=1.01f;
    assert(uwvm_trossen_validate_group(&info,&command,(unsigned char const*)"arm",3)==22);
    memcpy(info.mode.data,"velocity",8);command.cmd.data[0]=0.1f;
    assert(!uwvm_trossen_validate_group(&info,&command,(unsigned char const*)"arm",3));
    command.cmd.data[0]=0.3f;
    assert(uwvm_trossen_validate_group(&info,&command,(unsigned char const*)"arm",3)==22);
    info.mode.size=3;memcpy(info.mode.data,"pwm",3);
    assert(uwvm_trossen_validate_group(&info,&command,(unsigned char const*)"arm",3)==95);
#else
    assert(UWVM_TROSSEN_ROS_VERSION==1);
#endif
    return 0;
}
