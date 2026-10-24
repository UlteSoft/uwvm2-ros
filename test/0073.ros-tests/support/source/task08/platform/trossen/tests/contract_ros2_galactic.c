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
#define UWVM_TROSSEN_TEST_NOINLINE __attribute__((noinline))
#else
#define UWVM_TROSSEN_TEST_NOINLINE
#endif
/* One finite frame per record also works in unoptimized native CMake builds. */
#define DECL(P,D) static UWVM_TROSSEN_TEST_NOINLINE void check_##P(void){CHECK(P,D);}
DECL(roswasmc_bounded_2_builtin_interfaces_msg_Duration,0);
DECL(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_msg_ArmJoy,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_msg_HexJoy,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_msg_JointGroupCommand,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_msg_JointSingleCommand,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_msg_JointTemps,0);
DECL(roswasmc_bounded_2_std_msgs_msg_Header,0);
DECL(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint,0);
DECL(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_msg_JointTrajectoryCommand,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_msg_LocobotJoy,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_msg_TurretJoy,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_MotorGains_Request,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_MotorGains_Response,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_OperatingModes_Request,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_OperatingModes_Response,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_Reboot_Request,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_Reboot_Response,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_RegisterValues_Request,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_RegisterValues_Response,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_RobotInfo_Request,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_RobotInfo_Response,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_TorqueEnable_Request,0);
DECL(roswasmc_bounded_2_interbotix_xs_msgs_srv_TorqueEnable_Response,0);
int main(void){
check_roswasmc_bounded_2_builtin_interfaces_msg_Duration();
check_roswasmc_bounded_2_builtin_interfaces_msg_Time();
check_roswasmc_bounded_2_interbotix_xs_msgs_msg_ArmJoy();
check_roswasmc_bounded_2_interbotix_xs_msgs_msg_HexJoy();
check_roswasmc_bounded_2_interbotix_xs_msgs_msg_JointGroupCommand();
check_roswasmc_bounded_2_interbotix_xs_msgs_msg_JointSingleCommand();
check_roswasmc_bounded_2_interbotix_xs_msgs_msg_JointTemps();
check_roswasmc_bounded_2_std_msgs_msg_Header();
check_roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint();
check_roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory();
check_roswasmc_bounded_2_interbotix_xs_msgs_msg_JointTrajectoryCommand();
check_roswasmc_bounded_2_interbotix_xs_msgs_msg_LocobotJoy();
check_roswasmc_bounded_2_interbotix_xs_msgs_msg_TurretJoy();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_MotorGains_Request();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_MotorGains_Response();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_OperatingModes_Request();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_OperatingModes_Response();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_Reboot_Request();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_Reboot_Response();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_RegisterValues_Request();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_RegisterValues_Response();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_RobotInfo_Request();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_RobotInfo_Response();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_TorqueEnable_Request();
check_roswasmc_bounded_2_interbotix_xs_msgs_srv_TorqueEnable_Response();
return 0;}
