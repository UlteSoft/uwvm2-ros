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
#define UWVM_AGIBOT_TEST_NOINLINE __attribute__((noinline))
#else
#define UWVM_AGIBOT_TEST_NOINLINE
#endif
/* One finite frame per record also works in unoptimized native CMake builds. */
#define DECL(P,D) static UWVM_AGIBOT_TEST_NOINLINE void check_##P(void){CHECK(P,D);}
DECL(roswasmc_bounded_2_builtin_interfaces_msg_Time,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_RequestHeader,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_CommonRequest,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_ResponseHeader,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_CommonState,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_CommonResponse,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_CommonTaskResponse,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_DomainErrorState,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_JointCommand,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_MessageHeader,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_JointCommandArray,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_JointState,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_JointStateArray,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_ModuleChannelInfo,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_ModuleRpcInfo,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_ModuleResourceInfo,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_ThreadResourceInfo,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_ProcessResourceInfo,0);
DECL(roswasmc_bounded_2_aimdk_msgs_msg_ProcessInfo,0);
int main(void){
check_roswasmc_bounded_2_builtin_interfaces_msg_Time();
check_roswasmc_bounded_2_aimdk_msgs_msg_RequestHeader();
check_roswasmc_bounded_2_aimdk_msgs_msg_CommonRequest();
check_roswasmc_bounded_2_aimdk_msgs_msg_ResponseHeader();
check_roswasmc_bounded_2_aimdk_msgs_msg_CommonState();
check_roswasmc_bounded_2_aimdk_msgs_msg_CommonResponse();
check_roswasmc_bounded_2_aimdk_msgs_msg_CommonTaskResponse();
check_roswasmc_bounded_2_aimdk_msgs_msg_DomainErrorState();
check_roswasmc_bounded_2_aimdk_msgs_msg_JointCommand();
check_roswasmc_bounded_2_aimdk_msgs_msg_MessageHeader();
check_roswasmc_bounded_2_aimdk_msgs_msg_JointCommandArray();
check_roswasmc_bounded_2_aimdk_msgs_msg_JointState();
check_roswasmc_bounded_2_aimdk_msgs_msg_JointStateArray();
check_roswasmc_bounded_2_aimdk_msgs_msg_ModuleChannelInfo();
check_roswasmc_bounded_2_aimdk_msgs_msg_ModuleRpcInfo();
check_roswasmc_bounded_2_aimdk_msgs_msg_ModuleResourceInfo();
check_roswasmc_bounded_2_aimdk_msgs_msg_ThreadResourceInfo();
check_roswasmc_bounded_2_aimdk_msgs_msg_ProcessResourceInfo();
check_roswasmc_bounded_2_aimdk_msgs_msg_ProcessInfo();
return 0;}
