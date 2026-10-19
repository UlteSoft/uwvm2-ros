/** Simulator-only Unitree Go2 trajectory in a distinct Wasm linear memory.
 * The exact SDK kernel is also used by root main's inline variant and the
 * independent native fixture. A selected real WASI libm supplies tanh; no
 * copied approximation, CRT initializer or host math import is accepted.
 * This module must have only the four callbacks function imports below.
 */
#define UWVM_UNITREE_MOTION_IMPLEMENTATION 1
#include "unitree_motion_abi.h"
#include <roswasm_unitree/simulation_control.h>
#include <string.h>
UWVM_UM_EXTERN UWVM_UM_IMPORT("callbacks","joint_begin") uint32_t joint_begin(uint32_t,uint32_t,uint32_t);
UWVM_UM_EXTERN UWVM_UM_IMPORT("callbacks","on_joint") uint32_t on_joint(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
UWVM_UM_EXTERN UWVM_UM_IMPORT("callbacks","joint_end") uint32_t joint_end(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
UWVM_UM_EXTERN UWVM_UM_IMPORT("callbacks","joint_abort") uint32_t joint_abort(uint32_t,uint32_t);
static uwvm_unitree_fixture_motor motors[UWVM_UM_MOTORS];
static uint32_t generation,active,busy,poisoned,last_status,step_id,checksum;
static uint32_t bits(float value){uint32_t result;memcpy(&result,&value,4);return result;}
static uint32_t field_word(unsigned slot,unsigned field) {
    uwvm_unitree_fixture_motor const* m=motors+slot;
    switch(field){case 0:return m->mode;case 1:return bits(m->q);case 2:return bits(m->dq);
    case 3:return bits(m->kp);case 4:return bits(m->kd);default:return bits(m->tau);}
}
uint32_t uwvm_unitree_motion_begin(uint32_t step,uint32_t dt_ns) {
    uint32_t error,next,i,j;uwvm_unitree_fixture_motor computed[UWVM_UM_MOTORS];
    if(busy||active||poisoned){last_status=17;return 0;}
    if(step<1||step>10000||dt_ns<100000||dt_ns>20000000){last_status=22;return 0;}
    if(generation==UINT32_MAX){last_status=75;return 0;}
    error=(uint32_t)uwvm_unitree_go2_fixture_step(step,(double)dt_ns/1000000000.0,computed);
    if(error){last_status=error;return 0;}
    next=++generation;busy=1;error=joint_begin(UWVM_UM_OWNER,next,step);
    if(error){busy=0;last_status=error;return 0;}
    checksum=UINT32_C(2166136261);
    for(i=0;i<UWVM_UM_ACTIVE_JOINTS&&!error;++i){
        uint32_t words[10]={UWVM_UM_OWNER,next,step,i,computed[i].mode,bits(computed[i].q),bits(computed[i].dq),
            bits(computed[i].kp),bits(computed[i].kd),bits(computed[i].tau)};
        error=on_joint(words[0],words[1],words[2],words[3],words[4],words[5],words[6],words[7],words[8],words[9]);
        if(!error)for(j=0;j<10;++j)checksum=uwvm_um_checksum_word(checksum,words[j]);
    }
    if(error){uint32_t cleanup=joint_abort(UWVM_UM_OWNER,next);busy=0;last_status=error;
        /* Preserve the primary callback refusal. A failed abort poisons this
         * motion instance so it cannot admit a new, potentially orphaned run. */
        if(cleanup)poisoned=1;return 0;}
    for(i=0;i<UWVM_UM_MOTORS;++i)motors[i]=computed[i];
    step_id=step;active=1;busy=0;last_status=0;return next;
}
uint32_t uwvm_unitree_motion_status(void){return last_status;}
uint32_t uwvm_unitree_motion_word(uint32_t gen,uint32_t slot,uint32_t field) {
    if(busy){last_status=17;return 0;}
    if(!active||gen!=generation){last_status=9;return 0;}
    if(slot>=UWVM_UM_MOTORS||field>=UWVM_UM_FIELDS){last_status=22;return 0;}
    last_status=0;return field_word(slot,field);
}
uint32_t uwvm_unitree_motion_end(uint32_t gen) {
    uint32_t error,cleanup=0;
    if(busy){last_status=17;return 17;}
    if(!active||gen!=generation){last_status=9;return 9;}
    busy=1;error=joint_end(UWVM_UM_OWNER,gen,step_id,UWVM_UM_ACTIVE_JOINTS,checksum);
    if(error)cleanup=joint_abort(UWVM_UM_OWNER,gen);
    busy=0;active=0;if(cleanup)poisoned=1;last_status=error?error:cleanup;return last_status;
}
