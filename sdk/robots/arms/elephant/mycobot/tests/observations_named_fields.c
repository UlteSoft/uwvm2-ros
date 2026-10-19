/* SPDX-License-Identifier: Apache-2.0
 * Independent named-field checks for the model observation constructor.
 * Each expected field is named from the unchanged original declaration. The
 * helper's matching getter is intentionally not used to establish field order.
 * These arbitrary exact binary fractions are data tests, not physical poses.
 */
#include <assert.h>
#include <string.h>
#include <math.h>
#include "state.h"
int main(void){
 uwvm_elephant_state value={0},saved;
 double x[UWVM_ELEPHANT_COMPONENTS],lo[UWVM_ELEPHANT_COMPONENTS],hi[UWVM_ELEPHANT_COMPONENTS];
 for(size_t i=0;i<UWVM_ELEPHANT_COMPONENTS;++i){x[i]=(double)(i+1)/4;lo[i]=-4;hi[i]=8;}
 assert(uwvm_elephant_make_observation(x,lo,hi,&value)==0);
#if UWVM_ELEPHANT_SELECTION==1
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==2
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
#elif UWVM_ELEPHANT_SELECTION==3
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==4
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
assert((double)value.joint_7==1.75);
#elif UWVM_ELEPHANT_SELECTION==5
assert((double)value.joint_0==0.25);
assert((double)value.joint_1==0.5);
assert((double)value.joint_2==0.75);
assert((double)value.joint_3==1.0);
assert((double)value.joint_4==1.25);
assert((double)value.joint_5==1.5);
assert((double)value.joint_6==1.75);
assert((double)value.joint_7==2.0);
assert((double)value.joint_8==2.25);
assert((double)value.joint_9==2.5);
assert((double)value.joint_10==2.75);
assert((double)value.joint_11==3.0);
assert((double)value.joint_12==3.25);
#elif UWVM_ELEPHANT_SELECTION==6
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==7
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==8
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
#elif UWVM_ELEPHANT_SELECTION==9
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
#elif UWVM_ELEPHANT_SELECTION==10
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==11
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
assert((double)value.joint_7==1.75);
#elif UWVM_ELEPHANT_SELECTION==12
assert((double)value.joint_0==0.25);
assert((double)value.joint_1==0.5);
assert((double)value.joint_2==0.75);
assert((double)value.joint_3==1.0);
assert((double)value.joint_4==1.25);
assert((double)value.joint_5==1.5);
assert((double)value.joint_6==1.75);
assert((double)value.joint_7==2.0);
assert((double)value.joint_8==2.25);
assert((double)value.joint_9==2.5);
assert((double)value.joint_10==2.75);
assert((double)value.joint_11==3.0);
assert((double)value.joint_12==3.25);
#elif UWVM_ELEPHANT_SELECTION==13
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==14
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==15
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==16
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
#elif UWVM_ELEPHANT_SELECTION==17
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
#elif UWVM_ELEPHANT_SELECTION==18
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==19
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==20
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==21
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
#elif UWVM_ELEPHANT_SELECTION==22
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==23
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==24
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==25
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
#elif UWVM_ELEPHANT_SELECTION==26
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==27
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==28
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
assert((double)value.joint_5==1.25);
assert((double)value.joint_6==1.5);
#elif UWVM_ELEPHANT_SELECTION==29
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
#elif UWVM_ELEPHANT_SELECTION==30
assert((double)value.joint_1==0.25);
assert((double)value.joint_2==0.5);
assert((double)value.joint_3==0.75);
assert((double)value.joint_4==1.0);
#endif
 memcpy(&saved,&value,sizeof value);
 x[0]=NAN;assert(uwvm_elephant_make_observation(x,lo,hi,&value)==22);
 assert(!memcmp(&value,&saved,sizeof value));
 x[0]=1;lo[0]=2;assert(uwvm_elephant_make_observation(x,lo,hi,&value)==22);
 assert(!memcmp(&value,&saved,sizeof value));
 return 0;
}
