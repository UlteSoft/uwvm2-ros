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
CHECK(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback,0);
CHECK(roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon,1);
CHECK(roswasmc_bounded_2_franka_msgs_action_Grasp_Goal,1);
CHECK(roswasmc_bounded_2_franka_msgs_action_Grasp_Result,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_Homing_Goal,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_Homing_Result,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_Homing_Feedback,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_Move_Goal,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_Move_Result,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_Move_Feedback,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal,0);
CHECK(roswasmc_bounded_2_franka_msgs_msg_TargetStatus,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result,0);
CHECK(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Vector3,0);
CHECK(roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators,0);
CHECK(roswasmc_bounded_2_franka_msgs_msg_Elbow,0);
CHECK(roswasmc_bounded_2_franka_msgs_msg_Errors,0);
CHECK(roswasmc_bounded_2_std_msgs_msg_Header,0);
CHECK(roswasmc_bounded_2_sensor_msgs_msg_JointState,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Wrench,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Inertia,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Point,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion,1);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Pose,1);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped,1);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Twist,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_TwistStamped,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Accel,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_AccelStamped,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped,0);
CHECK(roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState,1);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request,0);
CHECK(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_msg_SpineState,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request,0);
CHECK(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response,0);
uwvm_franka_grasp_goal goal={0},saved;memset(&goal,0x55,sizeof goal);memcpy(&saved,&goal,sizeof goal);
assert(uwvm_franka_make_grasp(NAN,0.02,1,0.005,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.09,0.02,1,0.005,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.04,0,1,0.005,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.04,0.02,INFINITY,0.005,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.04,0.02,1,-0.001,0.005,0.08,&goal)==22&&!memcmp(&goal,&saved,sizeof goal));
assert(uwvm_franka_make_grasp(0.04,0.02,1,0.005,0.005,0.08,&goal)==0);
assert(goal.width==0.04&&goal.speed==0.02&&goal.force==1&&goal.epsilon.inner==0.005&&goal.epsilon.outer==0.005);
return 0;}
