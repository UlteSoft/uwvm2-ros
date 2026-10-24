#include "state.hpp"
/* Authored complete records and transactional malformed/capacity cases.
 * These are SDK contracts, not original upstream or hardware tests. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "state.h"
#define INIT_0(P,v) ((void)0)
#define INIT_1(P,v) assert(P##_default(&(v))==0)
#define CHECK(P,D) do{P a={0},out,before;unsigned char wire[32769];size_t n=0,i;INIT_##D(P,a);assert(P##_encode(&a,wire,32768,&n)==0);assert(P##_decode(wire,n,&out)==0);memset(&out,0x55,sizeof out);memcpy(&before,&out,sizeof out);for(i=0;i<n;++i){assert(P##_decode(wire,i,&out)!=0);assert(!memcmp(&out,&before,sizeof out));}wire[n]=0xff;assert(P##_decode(wire,n+1,&out)!=0);assert(!memcmp(&out,&before,sizeof out));if(n){size_t used=99;memset(wire,0x66,n);assert(P##_encode(&a,wire,n-1,&used)!=0);for(i=0;i<n;++i)assert(wire[i]==0x66);}}while(0)
#if defined(__GNUC__)
#define UWVM_FLEXIV_TEST_NOINLINE __attribute__((noinline))
#else
#define UWVM_FLEXIV_TEST_NOINLINE
#endif
/* One finite frame per record also works in unoptimized native CMake builds. */
#define DECL(P,D) static UWVM_FLEXIV_TEST_NOINLINE void check_##P(void){CHECK(P,D);}
DECL(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
DECL(roswasmc_bounded_2_flexiv_msgs_action_Grasp_Goal,0);
DECL(roswasmc_bounded_2_flexiv_msgs_action_Grasp_Result,0);
DECL(roswasmc_bounded_2_flexiv_msgs_action_Grasp_Feedback,0);
DECL(roswasmc_bounded_2_flexiv_msgs_action_Move_Goal,0);
DECL(roswasmc_bounded_2_flexiv_msgs_action_Move_Result,0);
DECL(roswasmc_bounded_2_flexiv_msgs_action_Move_Feedback,0);
DECL(roswasmc_bounded_2_flexiv_msgs_msg_Digital,0);
DECL(roswasmc_bounded_2_flexiv_msgs_msg_GPIOStates,0);
DECL(roswasmc_bounded_2_flexiv_msgs_msg_JointPosVel,0);
DECL(roswasmc_bounded_2_flexiv_msgs_msg_Mode,0);
DECL(roswasmc_bounded_2_std_msgs_msg_Header,0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_Point,0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_Quaternion,1);
DECL(roswasmc_bounded_2_geometry_msgs_msg_Pose,0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped,0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_Vector3,0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_Twist,0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_TwistStamped,0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_Wrench,0);
DECL(roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped,0);
DECL(roswasmc_bounded_2_flexiv_msgs_msg_RobotStates,0);
int main(void){
check_roswasmc_bounded_2_builtin_interfaces_msg_Time();
check_roswasmc_bounded_2_flexiv_msgs_action_Grasp_Goal();
check_roswasmc_bounded_2_flexiv_msgs_action_Grasp_Result();
check_roswasmc_bounded_2_flexiv_msgs_action_Grasp_Feedback();
check_roswasmc_bounded_2_flexiv_msgs_action_Move_Goal();
check_roswasmc_bounded_2_flexiv_msgs_action_Move_Result();
check_roswasmc_bounded_2_flexiv_msgs_action_Move_Feedback();
check_roswasmc_bounded_2_flexiv_msgs_msg_Digital();
check_roswasmc_bounded_2_flexiv_msgs_msg_GPIOStates();
check_roswasmc_bounded_2_flexiv_msgs_msg_JointPosVel();
check_roswasmc_bounded_2_flexiv_msgs_msg_Mode();
check_roswasmc_bounded_2_std_msgs_msg_Header();
check_roswasmc_bounded_2_geometry_msgs_msg_Point();
check_roswasmc_bounded_2_geometry_msgs_msg_Quaternion();
check_roswasmc_bounded_2_geometry_msgs_msg_Pose();
check_roswasmc_bounded_2_geometry_msgs_msg_PoseStamped();
check_roswasmc_bounded_2_geometry_msgs_msg_Vector3();
check_roswasmc_bounded_2_geometry_msgs_msg_Twist();
check_roswasmc_bounded_2_geometry_msgs_msg_TwistStamped();
check_roswasmc_bounded_2_geometry_msgs_msg_Wrench();
check_roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped();
check_roswasmc_bounded_2_flexiv_msgs_msg_RobotStates();
return 0;}
