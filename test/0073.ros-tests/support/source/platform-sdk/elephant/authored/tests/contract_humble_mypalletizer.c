/* Authored whole-value contracts; never original official or robot commands. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "state.h"
#define INIT_0(P,v) ((void)0)
#define INIT_1(P,v) assert(P##_default(&(v))==0)
#define CHECK(P,D) do {P a={0},out,before;unsigned char wire[32769];size_t n=0,i;INIT_##D(P,a); assert(P##_encode(&a,wire,32768,&n)==0);assert(P##_decode(wire,n,&out)==0); memset(&out,0x55,sizeof out);memcpy(&before,&out,sizeof out); for(i=0;i<n;++i){assert(P##_decode(wire,i,&out)!=0);assert(!memcmp(&out,&before,sizeof out));} wire[n]=0xff;assert(P##_decode(wire,n+1,&out)!=0);assert(!memcmp(&out,&before,sizeof out)); if(n){memset(wire,0x66,n);size_t used=99;assert(P##_encode(&a,wire,n-1,&used)!=0);for(i=0;i<n;++i)assert(wire[i]==0x66);} }while(0)
int main(void){
CHECK(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request,0);
CHECK(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request,0);
CHECK(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request,0);
CHECK(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request,0);
CHECK(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_msg_MycobotAngles,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_msg_MycobotCoords,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_msg_MycobotGripperStatus,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_msg_MycobotSetAngles,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_msg_MycobotSetCoords,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Request,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Response,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Request,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Response,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Request,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Response,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Request,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Response,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Request,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Response,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Request,0);
CHECK(roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Response,0);
uwvm_elephant_state value={0},saved;double positions[UWVM_ELEPHANT_COMPONENTS],lo[UWVM_ELEPHANT_COMPONENTS],hi[UWVM_ELEPHANT_COMPONENTS];
for(size_t i=0;i<UWVM_ELEPHANT_COMPONENTS;++i){positions[i]=(double)(i+1)/4;lo[i]=-180;hi[i]=180;}
memset(&value,0x55,sizeof value);memcpy(&saved,&value,sizeof value);
assert(uwvm_elephant_make_observation(0,lo,hi,&value)==22&&!memcmp(&value,&saved,sizeof value));
assert(uwvm_elephant_make_observation(positions,0,hi,&value)==22&&!memcmp(&value,&saved,sizeof value));
assert(uwvm_elephant_make_observation(positions,lo,0,&value)==22&&!memcmp(&value,&saved,sizeof value));
assert(uwvm_elephant_make_observation(positions,lo,hi,0)==22);
for(size_t i=0;i<UWVM_ELEPHANT_COMPONENTS;++i){
 double original=positions[i];positions[i]=NAN;assert(uwvm_elephant_make_observation(positions,lo,hi,&value)==22&&!memcmp(&value,&saved,sizeof value));
 positions[i]=INFINITY;assert(uwvm_elephant_make_observation(positions,lo,hi,&value)==22&&!memcmp(&value,&saved,sizeof value));
 positions[i]=181;assert(uwvm_elephant_make_observation(positions,lo,hi,&value)==22&&!memcmp(&value,&saved,sizeof value));positions[i]=original;
 lo[i]=181;assert(uwvm_elephant_make_observation(positions,lo,hi,&value)==22&&!memcmp(&value,&saved,sizeof value));lo[i]=-180;
 hi[i]=NAN;assert(uwvm_elephant_make_observation(positions,lo,hi,&value)==22&&!memcmp(&value,&saved,sizeof value));hi[i]=180;
}
assert(uwvm_elephant_make_observation(positions,lo,hi,&value)==0);
for(size_t i=0;i<UWVM_ELEPHANT_COMPONENTS;++i)assert(uwvm_elephant_component(&value,i)==positions[i]);
memcpy(&saved,&value,sizeof value);positions[0]=1.00000004;lo[0]=positions[0];hi[0]=positions[0];assert(uwvm_elephant_make_observation(positions,lo,hi,&value)==22&&!memcmp(&value,&saved,sizeof value));
return 0;}
