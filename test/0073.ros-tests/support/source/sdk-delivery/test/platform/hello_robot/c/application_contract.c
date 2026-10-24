#include <assert.h>
#include <string.h>
#include "application.h"
int main(void){
    char const* joints[]={"joint_lift","wrist_extension","joint_head_pan"};
    uwvm_stretch_deployment profile={UWVM_STRETCH_SE3,UWVM_STRETCH_POSITION,1,0,0,joints,3,1000,100};
    roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal goal={0};
    goal.trajectory.joint_names.size=1;goal.trajectory.joint_names.data[0].size=10;
    memcpy(goal.trajectory.joint_names.data[0].data,"joint_lift",10);
    goal.trajectory.points.size=1;goal.trajectory.points.data[0].positions.size=1;
    assert(!uwvm_stretch_validate_goal(&profile,1100,&goal));
    assert(uwvm_stretch_validate_goal(&profile,1101,&goal)==22);
    profile.mode=UWVM_STRETCH_GAMEPAD;
    assert(uwvm_stretch_validate_goal(&profile,1001,&goal)==125);
    profile.mode=UWVM_STRETCH_POSITION;profile.runstopped=1;
    assert(uwvm_stretch_validate_goal(&profile,1001,&goal)==125);
    profile.runstopped=0;goal.trajectory.joint_names.data[0].size=11;
    memcpy(goal.trajectory.joint_names.data[0].data,"joint_wrist",11);
    assert(uwvm_stretch_validate_goal(&profile,1001,&goal)==95);
    goal.trajectory.joint_names.data[0].size=10;
    memcpy(goal.trajectory.joint_names.data[0].data,"joint_lift",10);
    goal.trajectory.points.data[0].time_from_start.sec=1;
    assert(uwvm_stretch_validate_goal(&profile,1001,&goal)==95);
    profile.mode=UWVM_STRETCH_TRAJECTORY;profile.maximum_trajectory_ns=2000000000u;
    assert(uwvm_stretch_validate_goal(&profile,1001,&goal)==95);
    if(sizeof goal.trajectory.points.data/sizeof goal.trajectory.points.data[0]>=2){
        /* Exercise multi-point success only with actual multi-point storage. */
        goal.trajectory.points.size=2;
        memcpy((unsigned char*)goal.trajectory.points.data+sizeof goal.trajectory.points.data[0],
               &goal.trajectory.points.data[0],sizeof goal.trajectory.points.data[0]);
        goal.trajectory.points.data[0].time_from_start.sec=0;
        assert(!uwvm_stretch_validate_goal(&profile,1001,&goal));
        goal.trajectory.points.data[0].time_from_start.sec=1;
        assert(uwvm_stretch_validate_goal(&profile,1001,&goal)==22);
        goal.trajectory.points.data[0].time_from_start.sec=0;
    }
    goal.goal_time_tolerance.sec=1;
    assert(uwvm_stretch_validate_goal(&profile,1001,&goal)==95);
    return 0;
}
