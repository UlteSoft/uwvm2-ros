#include <assert.h>
#include <string.h>
#include "gen3_goal.h"
int main(void){
    unsigned i;uwvm_gen3_joint_limits limits={0};
    roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal goal={0};
    limits.model=6;limits.maximum_trajectory_ns=2000000000u;
    goal.trajectory.joint_names.size=6;goal.trajectory.points.size=1;
    goal.trajectory.points.data[0].positions.size=6;
    goal.trajectory.points.data[0].time_from_start.sec=1;
    for(i=0;i<6;++i){
        limits.lower_rad[i]=-1;limits.upper_rad[i]=1;limits.velocity_rps[i]=1;
        limits.acceleration_rps2[i]=1;
        goal.trajectory.joint_names.data[i].size=7;
        memcpy(goal.trajectory.joint_names.data[i].data,"joint_1",7);
        goal.trajectory.joint_names.data[i].data[6]=(unsigned char)('1'+i);
    }
    assert(!uwvm_gen3_validate_goal(&limits,&goal));
    goal.trajectory.joint_names.data[5].data[6]='5';
    assert(uwvm_gen3_validate_goal(&limits,&goal)==22);
    goal.trajectory.joint_names.data[5].data[6]='6';
    goal.trajectory.points.data[0].positions.data[0]=1.01;
    assert(uwvm_gen3_validate_goal(&limits,&goal)==22);
    goal.trajectory.points.data[0].positions.data[0]=0;
    goal.trajectory.points.data[0].effort.size=6;
    assert(uwvm_gen3_validate_goal(&limits,&goal)==95);
    goal.trajectory.points.data[0].effort.size=0;
    if(sizeof goal.trajectory.points.data/sizeof goal.trajectory.points.data[0]>=2){
        goal.trajectory.points.size=2;
        memcpy((unsigned char*)goal.trajectory.points.data+sizeof goal.trajectory.points.data[0],
               &goal.trajectory.points.data[0],sizeof goal.trajectory.points.data[0]);
        assert(uwvm_gen3_validate_goal(&limits,&goal)==22);
    }
    return 0;
}
