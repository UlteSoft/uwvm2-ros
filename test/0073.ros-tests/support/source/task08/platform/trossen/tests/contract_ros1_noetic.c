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
DECL(roswasmc_bounded_1_interbotix_slate_msgs_SetStringRequest,0);
DECL(roswasmc_bounded_1_interbotix_slate_msgs_SetStringResponse,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_ArmJoy,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_HexJoy,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_JointGroupCommand,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_JointSingleCommand,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_JointTemps,0);
DECL(roswasmc_bounded_1_time,0);
DECL(roswasmc_bounded_1_std_msgs_Header,0);
DECL(roswasmc_bounded_1_duration,0);
DECL(roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint,0);
DECL(roswasmc_bounded_1_trajectory_msgs_JointTrajectory,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_JointTrajectoryCommand,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_LocobotJoy,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_MotorGainsRequest,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_MotorGainsResponse,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_OperatingModesRequest,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_OperatingModesResponse,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_RebootRequest,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_RebootResponse,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_RegisterValuesRequest,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_RegisterValuesResponse,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_RobotInfoRequest,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_RobotInfoResponse,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_TorqueEnableRequest,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_TorqueEnableResponse,0);
DECL(roswasmc_bounded_1_interbotix_xs_msgs_TurretJoy,0);
DECL(roswasmc_bounded_1_xarm_msgs_ClearErrRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_ClearErrResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_ConfigToolModbusRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_ConfigToolModbusResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_GetAnalogIORequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_GetAnalogIOResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_GetControllerDigitalIORequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_GetControllerDigitalIOResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_GetDigitalIORequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_GetDigitalIOResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_GetErrRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_GetErrResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_GripperConfigRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_GripperConfigResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_GripperMoveRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_GripperMoveResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_GripperStateRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_GripperStateResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_IOState,0);
DECL(roswasmc_bounded_1_xarm_msgs_MoveRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_MoveResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_RobotMsg,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetAxisRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetAxisResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetDigitalIORequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetDigitalIOResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetInt16Request,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetInt16Response,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetLoadRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetLoadResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetToolModbusRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_SetToolModbusResponse,0);
DECL(roswasmc_bounded_1_xarm_msgs_TCPOffsetRequest,0);
DECL(roswasmc_bounded_1_xarm_msgs_TCPOffsetResponse,0);
int main(void){
check_roswasmc_bounded_1_interbotix_slate_msgs_SetStringRequest();
check_roswasmc_bounded_1_interbotix_slate_msgs_SetStringResponse();
check_roswasmc_bounded_1_interbotix_xs_msgs_ArmJoy();
check_roswasmc_bounded_1_interbotix_xs_msgs_HexJoy();
check_roswasmc_bounded_1_interbotix_xs_msgs_JointGroupCommand();
check_roswasmc_bounded_1_interbotix_xs_msgs_JointSingleCommand();
check_roswasmc_bounded_1_interbotix_xs_msgs_JointTemps();
check_roswasmc_bounded_1_time();
check_roswasmc_bounded_1_std_msgs_Header();
check_roswasmc_bounded_1_duration();
check_roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint();
check_roswasmc_bounded_1_trajectory_msgs_JointTrajectory();
check_roswasmc_bounded_1_interbotix_xs_msgs_JointTrajectoryCommand();
check_roswasmc_bounded_1_interbotix_xs_msgs_LocobotJoy();
check_roswasmc_bounded_1_interbotix_xs_msgs_MotorGainsRequest();
check_roswasmc_bounded_1_interbotix_xs_msgs_MotorGainsResponse();
check_roswasmc_bounded_1_interbotix_xs_msgs_OperatingModesRequest();
check_roswasmc_bounded_1_interbotix_xs_msgs_OperatingModesResponse();
check_roswasmc_bounded_1_interbotix_xs_msgs_RebootRequest();
check_roswasmc_bounded_1_interbotix_xs_msgs_RebootResponse();
check_roswasmc_bounded_1_interbotix_xs_msgs_RegisterValuesRequest();
check_roswasmc_bounded_1_interbotix_xs_msgs_RegisterValuesResponse();
check_roswasmc_bounded_1_interbotix_xs_msgs_RobotInfoRequest();
check_roswasmc_bounded_1_interbotix_xs_msgs_RobotInfoResponse();
check_roswasmc_bounded_1_interbotix_xs_msgs_TorqueEnableRequest();
check_roswasmc_bounded_1_interbotix_xs_msgs_TorqueEnableResponse();
check_roswasmc_bounded_1_interbotix_xs_msgs_TurretJoy();
check_roswasmc_bounded_1_xarm_msgs_ClearErrRequest();
check_roswasmc_bounded_1_xarm_msgs_ClearErrResponse();
check_roswasmc_bounded_1_xarm_msgs_ConfigToolModbusRequest();
check_roswasmc_bounded_1_xarm_msgs_ConfigToolModbusResponse();
check_roswasmc_bounded_1_xarm_msgs_GetAnalogIORequest();
check_roswasmc_bounded_1_xarm_msgs_GetAnalogIOResponse();
check_roswasmc_bounded_1_xarm_msgs_GetControllerDigitalIORequest();
check_roswasmc_bounded_1_xarm_msgs_GetControllerDigitalIOResponse();
check_roswasmc_bounded_1_xarm_msgs_GetDigitalIORequest();
check_roswasmc_bounded_1_xarm_msgs_GetDigitalIOResponse();
check_roswasmc_bounded_1_xarm_msgs_GetErrRequest();
check_roswasmc_bounded_1_xarm_msgs_GetErrResponse();
check_roswasmc_bounded_1_xarm_msgs_GripperConfigRequest();
check_roswasmc_bounded_1_xarm_msgs_GripperConfigResponse();
check_roswasmc_bounded_1_xarm_msgs_GripperMoveRequest();
check_roswasmc_bounded_1_xarm_msgs_GripperMoveResponse();
check_roswasmc_bounded_1_xarm_msgs_GripperStateRequest();
check_roswasmc_bounded_1_xarm_msgs_GripperStateResponse();
check_roswasmc_bounded_1_xarm_msgs_IOState();
check_roswasmc_bounded_1_xarm_msgs_MoveRequest();
check_roswasmc_bounded_1_xarm_msgs_MoveResponse();
check_roswasmc_bounded_1_xarm_msgs_RobotMsg();
check_roswasmc_bounded_1_xarm_msgs_SetAxisRequest();
check_roswasmc_bounded_1_xarm_msgs_SetAxisResponse();
check_roswasmc_bounded_1_xarm_msgs_SetDigitalIORequest();
check_roswasmc_bounded_1_xarm_msgs_SetDigitalIOResponse();
check_roswasmc_bounded_1_xarm_msgs_SetInt16Request();
check_roswasmc_bounded_1_xarm_msgs_SetInt16Response();
check_roswasmc_bounded_1_xarm_msgs_SetLoadRequest();
check_roswasmc_bounded_1_xarm_msgs_SetLoadResponse();
check_roswasmc_bounded_1_xarm_msgs_SetToolModbusRequest();
check_roswasmc_bounded_1_xarm_msgs_SetToolModbusResponse();
check_roswasmc_bounded_1_xarm_msgs_TCPOffsetRequest();
check_roswasmc_bounded_1_xarm_msgs_TCPOffsetResponse();
return 0;}
