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
CHECK(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Point,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Quaternion,1);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Pose,1);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Vector3,0);
CHECK(roswasmc_bounded_2_geometry_msgs_msg_Twist,0);
CHECK(roswasmc_bounded_2_std_msgs_msg_Header,0);
CHECK(roswasmc_bounded_2_std_msgs_msg_String,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_RobotMode,0);
CHECK(roswasmc_bounded_2_urinterfaces_action_SetMode_Goal,1);
CHECK(roswasmc_bounded_2_urinterfaces_action_SetMode_Result,0);
CHECK(roswasmc_bounded_2_urinterfaces_action_SetMode_Feedback,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_ActiveTCP,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_Analog,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_AnalogIO,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_AnalogIOStateStamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_AnalogStamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_BoolStamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_ControlCycle,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_Current,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_Digital,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_DigitalIO,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_DigitalIOStateStamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_DigitalStamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_DoubleMultiArrayStamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_Float32Stamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_Float64Stamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_JointMode,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_JointTemperature,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_JointVoltage,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_JsonStringType,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_RuntimeState,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_SafetyStatus,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_SharedVariablesEditResponse,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_SpeedSliderFraction,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_TCPState,1);
CHECK(roswasmc_bounded_2_urinterfaces_msg_ToolOutputMode,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_VariableUpdate,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_VoltageCurrentStamped,0);
CHECK(roswasmc_bounded_2_urinterfaces_msg_WorldModelNames,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Request,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_SetAnalogOutput_Response,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Request,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_SetDigitalOutput_Response,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Request,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_SetSpeedFraction_Response,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Request,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_SharedVariablesEditor_Response,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Request,0);
CHECK(roswasmc_bounded_2_urinterfaces_srv_VariablesViewer_Response,0);
uwvm_ur_speed_request speed={0},saved;memset(&speed,0x55,sizeof speed);memcpy(&saved,&speed,sizeof speed);
assert(uwvm_ur_make_speed(NAN,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(-0.1,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(1.1,&speed)==22&&!memcmp(&speed,&saved,sizeof speed));
assert(uwvm_ur_make_speed(0,&speed)==0&&speed.UWVM_UR_SPEED_MEMBER==0);
assert(uwvm_ur_make_speed(1,&speed)==0&&speed.UWVM_UR_SPEED_MEMBER==1);
return 0;}
