#include <assert.h>
#include "application.h"
int main(void){
    uwvm_franka_gripper_limits limits={0.08,0.02,5,0.005};
    uwvm_franka_grasp_goal grasp={0};
    uwvm_franka_move_goal move={0};
    grasp.width=0.04;grasp.speed=0.01;grasp.force=1;
    assert(!uwvm_franka_validate_grasp(&grasp,&limits));
    grasp.force=6;assert(uwvm_franka_validate_grasp(&grasp,&limits)==22);
    grasp.force=1;grasp.epsilon.outer=0.006;
    assert(uwvm_franka_validate_grasp(&grasp,&limits)==22);
    move.width=0.08;move.speed=0.02;
    assert(!uwvm_franka_validate_move(&move,&limits));
    move.speed=0.021;assert(uwvm_franka_validate_move(&move,&limits)==22);
#if UWVM_FRANKA_ROS_VERSION==2
    {
        unsigned i;uwvm_franka_ptp_goal goal={0};uwvm_franka_joint_limits joint={0};
        goal.goal_joint_configuration.size=7;goal.maximum_joint_velocities.size=7;
        goal.goal_tolerance=0.01;joint.tolerance_rad=0.02;
        for(i=0;i<7;++i){joint.lower_rad[i]=-1;joint.upper_rad[i]=1;
            joint.velocity_rps[i]=0.2;goal.maximum_joint_velocities.data[i]=0.1;}
        assert(!uwvm_franka_validate_ptp(&goal,&joint));
        goal.goal_joint_configuration.data[6]=1.01;
        assert(uwvm_franka_validate_ptp(&goal,&joint)==22);
        goal.goal_joint_configuration.data[6]=0;goal.maximum_joint_velocities.size=6;
        assert(uwvm_franka_validate_ptp(&goal,&joint)==22);
    }
#endif
    return 0;
}
