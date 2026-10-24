/** Unrun C11 contract compiled ONLY against actual complete generated product. */
#include "../../../../../../../../sdk/origin/robots/mobile_manipulation/fjt_goal.h"
#include "../../../../../../../../sdk/origin/robots/mobile_manipulation/examples/fjt_lesson.h"
static mm_fjt_goal source,target;
static mm_fjt_feedback full_feedback;
static mm_fjt_result full_result;
static unsigned char wire[32768];
int main(void) {
    double values[7]={0};size_t length=123456,used;uint32_t e;
    if(mm_make_gen3_goal(&source,values,7))return 1;
    e=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_encode(&source,wire,1,&length);
    if(e!=90||length!=123456)return 2;
    length=0;
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_encode(&source,wire,sizeof wire,&length))return 3;
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_decode(wire,length,&target))return 4;
    if(target.trajectory.joint_names.size!=7||target.trajectory.points.size!=1||
       target.trajectory.points.data[0].positions.size!=7||target.trajectory.points.data[0].time_from_start.sec!=2)return 5;
    used=length;wire[used++]=0;
    target.trajectory.points.data[0].positions.data[0]=19.0;
    if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_decode(wire,used,&target)||
       target.trajectory.points.data[0].positions.data[0]!=19.0)return 6;
    /* Compile the complete feedback and result APIs, not only a Goal prefix. */
    length=0;
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_encode(&full_feedback,wire,sizeof wire,&length))return 7;
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_encode(&full_result,wire,sizeof wire,&length))return 8;
    return 0;
}
