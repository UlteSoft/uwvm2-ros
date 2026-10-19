/* Authored complete-value contracts, not an upstream official suite. */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "state.h"
#define INIT_0(P,v) ((void)0)
#define INIT_1(P,v) assert(P##_default(&(v))==0)
#define CHECK(P,D) do {P a={0},out,before;unsigned char wire[32769];size_t n=0,i;INIT_##D(P,a); \
assert(P##_encode(&a,wire,32768,&n)==0);assert(P##_decode(wire,n,&out)==0); \
memset(&out,0x55,sizeof out);memcpy(&before,&out,sizeof out); \
for(i=0;i<n;++i){assert(P##_decode(wire,i,&out)!=0);assert(!memcmp(&out,&before,sizeof out));} \
wire[n]=0xff;assert(P##_decode(wire,n+1,&out)!=0);assert(!memcmp(&out,&before,sizeof out)); \
if(n){memset(wire,0x66,n);size_t used=99;assert(P##_encode(&a,wire,n-1,&used)!=0);for(i=0;i<n;++i)assert(wire[i]==0x66);} \
}while(0)
int main(void){
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback,0);
CHECK(roswasmc_bounded_2_builtin_interfaces_msg_Duration,0);
CHECK(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
CHECK(roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo,0);
CHECK(roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection,0);
CHECK(roswasmc_bounded_2_std_msgs_msg_Header,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus,0);
CHECK(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_Power,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature,0);
CHECK(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request,0);
CHECK(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request,0);
CHECK(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response,0);
uwvm_clearpath_drive value={0},saved;memset(&value,0x55,sizeof value);memcpy(&saved,&value,sizeof value);
assert(uwvm_clearpath_make_wheel_velocity(NAN,0,1,&value)==22&&!memcmp(&value,&saved,sizeof value));
assert(uwvm_clearpath_make_wheel_velocity(0,INFINITY,1,&value)==22&&!memcmp(&value,&saved,sizeof value));
assert(uwvm_clearpath_make_wheel_velocity(0,0,0,&value)==22&&!memcmp(&value,&saved,sizeof value));
assert(uwvm_clearpath_make_wheel_velocity(2,0,1,&value)==22&&!memcmp(&value,&saved,sizeof value));
assert(uwvm_clearpath_make_wheel_velocity(0,-2,1,&value)==22&&!memcmp(&value,&saved,sizeof value));
assert(uwvm_clearpath_make_wheel_velocity(0,0,1,0)==22);
assert(uwvm_clearpath_make_wheel_velocity(0.5,-0.25,1,&value)==0);
assert(value.mode==0&&value.drivers[0]==0.5f&&value.drivers[1]==-0.25f);
return 0;}
