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
CHECK(roswasmc_bounded_1_geometry_msgs_Point,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Quaternion,0);
CHECK(roswasmc_bounded_1_geometry_msgs_Pose,0);
CHECK(roswasmc_bounded_1_xarm_msgs_IOState,0);
CHECK(roswasmc_bounded_1_xarm_msgs_RobotMsg,0);
CHECK(roswasmc_bounded_1_xarm_msgs_ClearErrRequest,0);
CHECK(roswasmc_bounded_1_xarm_msgs_ClearErrResponse,0);
CHECK(roswasmc_bounded_1_xarm_msgs_GetAnalogIORequest,0);
CHECK(roswasmc_bounded_1_xarm_msgs_GetAnalogIOResponse,0);
CHECK(roswasmc_bounded_1_xarm_msgs_GetDigitalIORequest,0);
CHECK(roswasmc_bounded_1_xarm_msgs_GetDigitalIOResponse,0);
CHECK(roswasmc_bounded_1_xarm_msgs_MoveRequest,0);
CHECK(roswasmc_bounded_1_xarm_msgs_MoveResponse,0);
CHECK(roswasmc_bounded_1_xarm_msgs_SetAxisRequest,0);
CHECK(roswasmc_bounded_1_xarm_msgs_SetAxisResponse,0);
CHECK(roswasmc_bounded_1_xarm_msgs_SetDigitalIORequest,0);
CHECK(roswasmc_bounded_1_xarm_msgs_SetDigitalIOResponse,0);
CHECK(roswasmc_bounded_1_xarm_msgs_SetInt16Request,0);
CHECK(roswasmc_bounded_1_xarm_msgs_SetInt16Response,0);
CHECK(roswasmc_bounded_1_xarm_msgs_TCPOffsetRequest,0);
CHECK(roswasmc_bounded_1_xarm_msgs_TCPOffsetResponse,0);
CHECK(roswasmc_bounded_1_xarm_planner_exec_planRequest,0);
CHECK(roswasmc_bounded_1_xarm_planner_exec_planResponse,0);
CHECK(roswasmc_bounded_1_xarm_planner_joint_planRequest,0);
CHECK(roswasmc_bounded_1_xarm_planner_joint_planResponse,0);
CHECK(roswasmc_bounded_1_xarm_planner_pose_planRequest,0);
CHECK(roswasmc_bounded_1_xarm_planner_pose_planResponse,0);
CHECK(roswasmc_bounded_1_xarm_planner_single_straight_planRequest,0);
CHECK(roswasmc_bounded_1_xarm_planner_single_straight_planResponse,0);
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
