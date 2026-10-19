/** Actual complete generated FJT values, with finite explicit convenience goals.
 * Full interfaces/tolerances/MultiDOF remain available; these builders do not
 * change the declaration, native metadata, bounds or canonical identity.
 */
#ifndef UWVM_MOBILE_FJT_GOAL_H
#define UWVM_MOBILE_FJT_GOAL_H
#include "fjt_client.h"
#include <math.h>
#include <string.h>
static inline uint32_t mm_make_goal(mm_fjt_goal* output,char const* const* names,
    double const* positions,size_t count) {
    mm_fjt_goal next={0};size_t i,n;
    if(!output||!names||!positions||!count)return 22;
    /* Real selected Humble Quaternion defaults are preserved by the generated
       full-closure default factory. Empty sequences remain empty. */
    if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_default(&next))return 22;
    if(count>sizeof next.trajectory.joint_names.data/sizeof next.trajectory.joint_names.data[0]||
       count>sizeof next.trajectory.points.data[0].positions.data/sizeof next.trajectory.points.data[0].positions.data[0]||
       sizeof next.trajectory.points.data/sizeof next.trajectory.points.data[0]<1)return 90;
    next.trajectory.joint_names.size=(uint32_t)count;
    next.trajectory.points.size=1;
    next.trajectory.points.data[0].positions.size=(uint32_t)count;
    next.trajectory.points.data[0].time_from_start.sec=2;
    for(i=0;i<count;++i) {
        if(!names[i]||!isfinite(positions[i]))return 22;
        n=strlen(names[i]);if(n>sizeof next.trajectory.joint_names.data[i].data)return 90;
        next.trajectory.joint_names.data[i].size=(uint32_t)n;
        memcpy(next.trajectory.joint_names.data[i].data,names[i],n);
        next.trajectory.points.data[0].positions.data[i]=positions[i];
    }
    if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_valid(&next))return 22;
    *output=next;return 0;
}
static inline uint32_t mm_make_gen3_goal(mm_fjt_goal* output,double const* positions,size_t count) {
    static char const* const names[]={"joint_1","joint_2","joint_3","joint_4","joint_5","joint_6","joint_7"};
    if(count!=6&&count!=7)return 22;return mm_make_goal(output,names,positions,count);
}
static inline uint32_t mm_make_stretch_lift_goal(mm_fjt_goal* output,double position_m) {
    static char const* const names[]={"joint_lift"};
    return mm_make_goal(output,names,&position_m,1);
}
#endif
