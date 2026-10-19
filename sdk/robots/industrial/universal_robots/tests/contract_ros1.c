/* All complete selected records; authored SDK contracts, not official suites. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "io.h"
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
CHECK(roswasmc_bounded_1_geometry_msgs_Point,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Quaternion,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Pose,0);
CHECK(roswasmc_bounded_1_time,0);
CHECK(roswasmc_bounded_1_std_msgs_Header,0);
CHECK(roswasmc_bounded_1_geometry_msgs_PoseStamped,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Vector3,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Twist,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Wrench,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_ProgramState,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_RobotMode,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult,0);
CHECK(roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback,0);
CHECK(roswasmc_bounded_1_ur_msgs_Analog,0);
CHECK(roswasmc_bounded_1_ur_msgs_Digital,0);
CHECK(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest,0);
CHECK(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse,0);
CHECK(roswasmc_bounded_1_ur_msgs_IOStates,0);
CHECK(roswasmc_bounded_1_ur_msgs_MasterboardDataMsg,0);
CHECK(roswasmc_bounded_1_ur_msgs_RobotModeDataMsg,0);
CHECK(roswasmc_bounded_1_ur_msgs_RobotStateRTMsg,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetForceModeRequest,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetForceModeResponse,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetIORequest,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetIOResponse,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetPayloadRequest,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetPayloadResponse,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest,0);
CHECK(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse,0);
CHECK(roswasmc_bounded_1_ur_msgs_ToolDataMsg,0);
uwvm_ur_speed_request speed={0},saved;memset(&speed,0x55,sizeof speed);memcpy(&saved,&speed,sizeof speed);
assert(uwvm_ur_make_speed(NAN,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(-0.1,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(1.1,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(0,&speed)==0&&speed.UWVM_UR_SPEED_MEMBER==0);
assert(uwvm_ur_make_speed(1,&speed)==0&&speed.UWVM_UR_SPEED_MEMBER==1);
return 0;}
