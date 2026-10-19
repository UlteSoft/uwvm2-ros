/** @file Pure, independently instantiated observed-joint-state perception.
 *
 * This module imports no host API, memory, table or other module. It verifies
 * actual ordered finite LowState words; it does not invent camera input or
 * claim image vision. Rejected calls cannot replace/retire another generation.
 */
#define UWVM_UNITREE_PERCEPTION_IMPLEMENTATION 1
#include "unitree_perception_abi.h"
static uint32_t generation,active,count,checksum,last_status;
uint32_t uwvm_unitree_perception_begin(uint32_t step) {
    if(active){last_status=17;return 0;}
    if(step<1||step>32){last_status=22;return 0;}
    if(generation==UINT32_MAX){last_status=75;return 0;}
    ++generation;active=1;count=0;
    checksum=uwvm_um_checksum_word(UINT32_C(2166136261),step);
    last_status=0;return generation;
}
uint32_t uwvm_unitree_perception_status(void){return last_status;}
uint32_t uwvm_unitree_perception_word(uint32_t gen,uint32_t index,uint32_t word) {
    if(!active||gen!=generation){last_status=9;return 9;}
    if(index!=count||count>=36||(word&UINT32_C(0x7f800000))==UINT32_C(0x7f800000)){
        last_status=22;return 22;
    }
    /* Independent recurrence: never checksum native structs or host memory. */
    for(unsigned byte=0;byte<4;++byte){checksum^=(word>>(8*byte))&255u;checksum*=UINT32_C(16777619);}
    ++count;last_status=0;return 0;
}
uint32_t uwvm_unitree_perception_end(uint32_t gen,uint32_t expected) {
    if(!active||gen!=generation){last_status=9;return 9;}
    if(count!=36||checksum!=expected){last_status=22;return 22;}
    active=0;last_status=0;return 0;
}
uint32_t uwvm_unitree_perception_abort(uint32_t gen) {
    if(!active||gen!=generation){last_status=9;return 9;}
    active=0;last_status=0;return 0;
}
