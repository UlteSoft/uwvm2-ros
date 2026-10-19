/* Complete-value authored contracts, not the upstream official suite. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "state.h"
#define INIT_0(P,v) ((void)0)
#define INIT_1(P,v) assert(P##_default(&(v))==0)
#define CHECK(P,D) do {P a={0},out,before;unsigned char wire[32769];size_t n=0,i; INIT_##D(P,a); \
 assert(P##_encode(&a,wire,32768,&n)==0);assert(P##_decode(wire,n,&out)==0); \
 memset(&out,0x55,sizeof out);memcpy(&before,&out,sizeof out); \
 for(i=0;i<n;++i){assert(P##_decode(wire,i,&out)!=0);assert(!memcmp(&out,&before,sizeof out));} \
 wire[n]=0xff;assert(P##_decode(wire,n+1,&out)!=0);assert(!memcmp(&out,&before,sizeof out)); \
 if(n){memset(wire,0x66,n);size_t used=99;assert(P##_encode(&a,wire,n-1,&used)!=0);for(i=0;i<n;++i)assert(wire[i]==0x66);} \
}while(0)
int main(void){
CHECK(roswasmc_bounded_1_franka_example_controllers_JointTorqueComparison,0);
CHECK(roswasmc_bounded_1_franka_gripper_GraspEpsilon,0);
CHECK(roswasmc_bounded_1_franka_gripper_GraspGoal,0);
CHECK(roswasmc_bounded_1_franka_gripper_GraspResult,0);
CHECK(roswasmc_bounded_1_franka_gripper_GraspFeedback,0);
CHECK(roswasmc_bounded_1_franka_gripper_HomingGoal,0);
CHECK(roswasmc_bounded_1_franka_gripper_HomingResult,0);
CHECK(roswasmc_bounded_1_franka_gripper_HomingFeedback,0);
CHECK(roswasmc_bounded_1_franka_gripper_MoveGoal,0);
CHECK(roswasmc_bounded_1_franka_gripper_MoveResult,0);
CHECK(roswasmc_bounded_1_franka_gripper_MoveFeedback,0);
CHECK(roswasmc_bounded_1_franka_gripper_StopGoal,0);
CHECK(roswasmc_bounded_1_franka_gripper_StopResult,0);
CHECK(roswasmc_bounded_1_franka_gripper_StopFeedback,0);
CHECK(roswasmc_bounded_1_franka_msgs_ErrorRecoveryGoal,0);
CHECK(roswasmc_bounded_1_franka_msgs_ErrorRecoveryResult,0);
CHECK(roswasmc_bounded_1_franka_msgs_ErrorRecoveryFeedback,0);
CHECK(roswasmc_bounded_1_franka_msgs_Errors,0);
CHECK(roswasmc_bounded_1_time,0);
CHECK(roswasmc_bounded_1_std_msgs_Header,0);
CHECK(roswasmc_bounded_1_franka_msgs_FrankaState,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceRequest,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetCartesianImpedanceResponse,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetEEFrameRequest,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetEEFrameResponse,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorRequest,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetForceTorqueCollisionBehaviorResponse,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorRequest,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetFullCollisionBehaviorResponse,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetJointImpedanceRequest,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetJointImpedanceResponse,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetKFrameRequest,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetKFrameResponse,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetLoadRequest,0);
CHECK(roswasmc_bounded_1_franka_msgs_SetLoadResponse,0);
uwvm_franka_grasp_goal goal={0},saved;memset(&goal,0x55,sizeof goal);memcpy(&saved,&goal,sizeof goal);
assert(uwvm_franka_make_grasp(NAN,0.02,1,0.005,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.09,0.02,1,0.005,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.04,0,1,0.005,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.04,0.02,INFINITY,0.005,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.04,0.02,1,-0.001,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.04,0.02,1,0.005,0.005,0.08,&goal)==0);
assert(goal.width==0.04&&goal.speed==0.02&&goal.force==1&&goal.epsilon.inner==0.005&&goal.epsilon.outer==0.005);
return 0;}
