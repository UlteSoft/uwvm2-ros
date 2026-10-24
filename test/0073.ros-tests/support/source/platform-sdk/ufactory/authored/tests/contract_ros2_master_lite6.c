/* Authored complete-value, transactional, and selected-model contracts.
 * Synthetic values are test data, never physical motion or original test claims.
 */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "state.h"
#define INIT_0(P,v) ((void)0)
#define INIT_1(P,v) assert(P##_default(&(v))==0)
#define CHECK(P,D) do {P a={0},out,before;unsigned char wire[32769];size_t n=0,i;INIT_##D(P,a); assert(P##_encode(&a,wire,32768,&n)==0);assert(P##_decode(wire,n,&out)==0); memset(&out,0x55,sizeof out);memcpy(&before,&out,sizeof out); for(i=0;i<n;++i){assert(P##_decode(wire,i,&out)!=0);assert(!memcmp(&out,&before,sizeof out));} wire[n]=0xff;assert(P##_decode(wire,n+1,&out)!=0);assert(!memcmp(&out,&before,sizeof out)); if(n){memset(wire,0x66,n);size_t used=99;assert(P##_encode(&a,wire,n-1,&used)!=0);for(i=0;i<n;++i)assert(wire[i]==0x66);} }while(0)
int main(void){
CHECK(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Point,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion,1);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Pose,1);
CHECK(roswasmc_bounded_2_std_msgs_msg_Header,0);
CHECK(roswasmc_bounded_2_xarm_msgs_msg_CIOState,0);
CHECK(roswasmc_bounded_2_xarm_msgs_msg_IOState,0);
CHECK(roswasmc_bounded_2_xarm_msgs_msg_RobotMsg,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_BioGripperCtrl_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_BioGripperCtrl_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_BioGripperEnable_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_BioGripperEnable_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_Call_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_Call_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_FtCaliLoad_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_FtCaliLoad_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_FtForceConfig_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_FtForceConfig_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_FtForcePid_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_FtForcePid_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_FtImpedance_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_FtImpedance_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetAnalogIO_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetAnalogIO_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetDigitalIO_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetDigitalIO_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetFloat32_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetFloat32_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetFloat32List_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetFloat32List_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetInt16_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetInt16_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetInt16List_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetInt16List_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetInt32_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetInt32_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetInt32ByType_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetInt32ByType_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GripperMove_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_GripperMove_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_IdenLoad_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_IdenLoad_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_LinearTrackBackOrigin_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_LinearTrackBackOrigin_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_LinearTrackSetPos_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_LinearTrackSetPos_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveCartesian_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveCartesian_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveCircle_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveCircle_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveHome_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveHome_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveJoint_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveJoint_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveVelocity_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_MoveVelocity_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_PlanExec_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_PlanExec_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_PlanJoint_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_PlanJoint_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_PlanPose_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_PlanPose_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_PlanSingleStraight_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_PlanSingleStraight_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_RobotiqActivate_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_RobotiqActivate_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_RobotiqGetStatus_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_RobotiqGetStatus_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_RobotiqMove_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_RobotiqMove_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_RobotiqReset_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_RobotiqReset_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetAnalogIO_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetAnalogIO_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetDigitalIO_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetDigitalIO_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetFloat32_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetFloat32_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetFloat32List_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetFloat32List_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt16_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt16_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt16ById_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt16ById_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt16List_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt16List_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt32_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt32_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt32ByType_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetInt32ByType_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetModbusTimeout_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetModbusTimeout_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetTcpLoad_Request,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_SetTcpLoad_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_TrajCtrl_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_TrajCtrl_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_TrajPlay_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_TrajPlay_Response,0);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_VacuumGripperCtrl_Request,1);
CHECK(roswasmc_bounded_2_xarm_msgs_srv_VacuumGripperCtrl_Response,0);
uwvm_xarm_state in={0},out,saved;memset(&out,0x55,sizeof out);memcpy(&saved,&out,sizeof out);
in.angle.size=UWVM_XARM_COMPONENTS;
for(size_t i=0;i<UWVM_XARM_COMPONENTS;++i)in.angle.data[i]=(float)(i+1)/4;
for(size_t i=0;i<6;++i){in.pose[i]=(float)(i+1)*2;in.offset[i]=(float)(i+1)/8;}
assert(uwvm_xarm_accept_observation(0,&out)==22&&!memcmp(&out,&saved,sizeof out));
assert(uwvm_xarm_accept_observation(&in,0)==22);
in.angle.size=UWVM_XARM_COMPONENTS-1;assert(uwvm_xarm_accept_observation(&in,&out)==22&&!memcmp(&out,&saved,sizeof out));
in.angle.size=8;assert(uwvm_xarm_accept_observation(&in,&out)==22&&!memcmp(&out,&saved,sizeof out));in.angle.size=UWVM_XARM_COMPONENTS;
in.angle.data[0]=NAN;assert(uwvm_xarm_accept_observation(&in,&out)==22&&!memcmp(&out,&saved,sizeof out));in.angle.data[0]=0.25f;
in.pose[5]=INFINITY;assert(uwvm_xarm_accept_observation(&in,&out)==22&&!memcmp(&out,&saved,sizeof out));in.pose[5]=12;
in.offset[2]=NAN;assert(uwvm_xarm_accept_observation(&in,&out)==22&&!memcmp(&out,&saved,sizeof out));in.offset[2]=0.375f;
assert(uwvm_xarm_accept_observation(&in,&out)==0);
assert(out.angle.size==UWVM_XARM_COMPONENTS);
for(size_t i=0;i<UWVM_XARM_COMPONENTS;++i)assert(out.angle.data[i]==(float)(i+1)/4);
for(size_t i=0;i<6;++i){assert(out.pose[i]==(float)(i+1)*2);assert(out.offset[i]==(float)(i+1)/8);}
memcpy(&saved,&out,sizeof out);uwvm_xarm_storage scratch={0};uwvm_xarm_query_storage query={0};uwvm_xarm_query_response response;
assert(uwvm_xarm_poll_state(0,&out,&scratch)==22&&!memcmp(&out,&saved,sizeof out));
assert(uwvm_xarm_query_once(0,&response,&query)==22);
return 0;}
