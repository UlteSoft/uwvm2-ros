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
CHECK(roswasmc_bounded_2_builtin_interfaces_msg_Duration,0);
CHECK(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
CHECK(roswasmc_bounded_2_control_msgs_msg_JointTolerance,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Point,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion,1);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Pose,1);
CHECK(roswasmc_bounded_2_std_msgs_msg_Header,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped,1);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Vector3,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Twist,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Wrench,0);
CHECK(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint,0);
CHECK(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request,1);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request,1);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request,0);
CHECK(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response,0);
CHECK(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal,0);
CHECK(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result,0);
CHECK(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback,0);
CHECK(roswasmc_bounded_2_ur_msgs_action_SendScript_Goal,1);
CHECK(roswasmc_bounded_2_ur_msgs_action_SendScript_Result,0);
CHECK(roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback,0);
CHECK(roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal,0);
CHECK(roswasmc_bounded_2_ur_msgs_action_ToolContact_Result,0);
CHECK(roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback,0);
CHECK(roswasmc_bounded_2_ur_msgs_msg_Analog,0);
CHECK(roswasmc_bounded_2_ur_msgs_msg_Digital,0);
CHECK(roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters,0);
CHECK(roswasmc_bounded_2_ur_msgs_msg_IOStates,0);
CHECK(roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg,0);
CHECK(roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg,0);
CHECK(roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg,0);
CHECK(roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request,1);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetIO_Request,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetIO_Response,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request,0);
CHECK(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response,0);
uwvm_ur_speed_request speed={0},saved;memset(&speed,0x55,sizeof speed);memcpy(&saved,&speed,sizeof speed);
assert(uwvm_ur_make_speed(NAN,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(-0.1,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(1.1,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(0,&speed)==0&&speed.UWVM_UR_SPEED_MEMBER==0);
assert(uwvm_ur_make_speed(1,&speed)==0&&speed.UWVM_UR_SPEED_MEMBER==1);
return 0;}
