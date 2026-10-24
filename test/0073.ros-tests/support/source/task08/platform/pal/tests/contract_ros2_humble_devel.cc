#include "state.hpp"
#include "bounded_records.hpp"
#include "bounded_interfaces.hpp"
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
#define UWVM_PAL_TEST_NOINLINE __attribute__((noinline))
#else
#define UWVM_PAL_TEST_NOINLINE
#endif
/* One finite frame per record also works in unoptimized native CMake builds. */
#define DECL(P,D) static UWVM_PAL_TEST_NOINLINE void check_##P(void){CHECK(P,D);}
DECL(roswasmc_bounded_2_builtin_interfaces_msg_Duration,0);
DECL(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
DECL(roswasmc_bounded_2_std_msgs_msg_ColorRGBA,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedFixedColorParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedRainbowParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedFadeParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedBlinkParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedProgressParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedFlowParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedPreProgrammedParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedEffectViaTopicParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedDataArrayParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedEffectParams,0);
DECL(roswasmc_bounded_2_pal_device_msgs_action_DoTimedLedEffect_Goal,0);
DECL(roswasmc_bounded_2_pal_device_msgs_action_DoTimedLedEffect_Result,0);
DECL(roswasmc_bounded_2_pal_device_msgs_action_DoTimedLedEffect_Feedback,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_BatteryState,0);
DECL(roswasmc_bounded_2_std_msgs_msg_Header,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_Bumper,0);
DECL(roswasmc_bounded_2_pal_device_msgs_msg_LedGroup,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_CancelEffect_Request,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_CancelEffect_Response,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_ShutdownAndWakeUpRobot_Request,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_ShutdownAndWakeUpRobot_Response,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_TimedBlinkEffect_Request,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_TimedBlinkEffect_Response,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_TimedColourEffect_Request,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_TimedColourEffect_Response,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_TimedFadeEffect_Request,0);
DECL(roswasmc_bounded_2_pal_device_msgs_srv_TimedFadeEffect_Response,0);
int main(void){
check_roswasmc_bounded_2_builtin_interfaces_msg_Duration();
check_roswasmc_bounded_2_builtin_interfaces_msg_Time();
check_roswasmc_bounded_2_std_msgs_msg_ColorRGBA();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedFixedColorParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedRainbowParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedFadeParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedBlinkParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedProgressParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedFlowParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedPreProgrammedParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedEffectViaTopicParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedDataArrayParams();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedEffectParams();
check_roswasmc_bounded_2_pal_device_msgs_action_DoTimedLedEffect_Goal();
check_roswasmc_bounded_2_pal_device_msgs_action_DoTimedLedEffect_Result();
check_roswasmc_bounded_2_pal_device_msgs_action_DoTimedLedEffect_Feedback();
check_roswasmc_bounded_2_pal_device_msgs_msg_BatteryState();
check_roswasmc_bounded_2_std_msgs_msg_Header();
check_roswasmc_bounded_2_pal_device_msgs_msg_Bumper();
check_roswasmc_bounded_2_pal_device_msgs_msg_LedGroup();
check_roswasmc_bounded_2_pal_device_msgs_srv_CancelEffect_Request();
check_roswasmc_bounded_2_pal_device_msgs_srv_CancelEffect_Response();
check_roswasmc_bounded_2_pal_device_msgs_srv_ShutdownAndWakeUpRobot_Request();
check_roswasmc_bounded_2_pal_device_msgs_srv_ShutdownAndWakeUpRobot_Response();
check_roswasmc_bounded_2_pal_device_msgs_srv_TimedBlinkEffect_Request();
check_roswasmc_bounded_2_pal_device_msgs_srv_TimedBlinkEffect_Response();
check_roswasmc_bounded_2_pal_device_msgs_srv_TimedColourEffect_Request();
check_roswasmc_bounded_2_pal_device_msgs_srv_TimedColourEffect_Response();
check_roswasmc_bounded_2_pal_device_msgs_srv_TimedFadeEffect_Request();
check_roswasmc_bounded_2_pal_device_msgs_srv_TimedFadeEffect_Response();
return 0;}
