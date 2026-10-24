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
#define UWVM_LEJU_TEST_NOINLINE __attribute__((noinline))
#else
#define UWVM_LEJU_TEST_NOINLINE
#endif
/* One finite frame per record also works in unoptimized native CMake builds. */
#define DECL(P,D) static UWVM_LEJU_TEST_NOINLINE void check_##P(void){CHECK(P,D);}
DECL(roswasmc_bounded_1_kuavo_sdk_qv,0);
DECL(roswasmc_bounded_1_kuavo_sdk_fullBodyTargetTrajectories,0);
int main(void){
check_roswasmc_bounded_1_kuavo_sdk_qv();
check_roswasmc_bounded_1_kuavo_sdk_fullBodyTargetTrajectories();
return 0;}
