/** Independent pure Wasm callback instance for the simulator motion preload.
 * No includes from a native ROS SDK, host imports, clocks, allocation or I/O.
 * The owner and generation are an explicit application protocol. They do not
 * authenticate a caller and must never be presented as a Runtime capability.
 */
#include "unitree_motion_abi.h"
static uint32_t active,generation,step_id,count,checksum;
static uint32_t previous_generation;

UWVM_UM_EXTERN UWVM_UM_EXPORT("joint_begin") uint32_t joint_begin(uint32_t owner,uint32_t next,uint32_t step) {
    if(owner!=UWVM_UM_OWNER||!next||step<1||step>10000)return 22;
    if(active)return 17;
    if(next<=previous_generation)return 9;
    previous_generation=next;generation=next;step_id=step;count=0;
    checksum=UINT32_C(2166136261);active=1;return 0;
}
/** Preserve all callback state on a rejected joint. The increasing slot check
 * proves twelve actual calls rather than one final claimed aggregate value.
 * All five float words must be finite; no interpretation of pointer bits.
 */
UWVM_UM_EXTERN UWVM_UM_EXPORT("on_joint") uint32_t on_joint(uint32_t owner,uint32_t gen,uint32_t step,uint32_t slot,
    uint32_t mode,uint32_t q,uint32_t dq,uint32_t kp,uint32_t kd,uint32_t tau) {
    uint32_t words[10]={owner,gen,step,slot,mode,q,dq,kp,kd,tau};unsigned i,j;uint32_t next;
    if(!active||owner!=UWVM_UM_OWNER||gen!=generation||step!=step_id)return 9;
    if(count>=UWVM_UM_ACTIVE_JOINTS||slot!=count||mode!=1)return 22;
    for(i=5;i<10;++i)if((words[i]&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 22;
    /* Independent byte recurrence; do not checksum sizeof a native struct. */
    next=checksum;for(i=0;i<10;++i)for(j=0;j<4;++j){next^=(words[i]>>(8*j))&255u;next*=UINT32_C(16777619);}
    checksum=next;++count;return 0;
}
UWVM_UM_EXTERN UWVM_UM_EXPORT("joint_end") uint32_t joint_end(uint32_t owner,uint32_t gen,uint32_t step,
    uint32_t expected_count,uint32_t expected_checksum) {
    if(!active||owner!=UWVM_UM_OWNER||gen!=generation||step!=step_id)return 9;
    if(count!=UWVM_UM_ACTIVE_JOINTS||expected_count!=count||expected_checksum!=checksum)return 22;
    active=0;return 0;
}
/** Checked retirement after an earlier callback failure. This never retries a
 * joint or hides the primary error. A stale token cannot retire another run.
 */
UWVM_UM_EXTERN UWVM_UM_EXPORT("joint_abort") uint32_t joint_abort(uint32_t owner,uint32_t gen) {
    if(!active||owner!=UWVM_UM_OWNER||gen!=generation)return 9;
    active=0;return 0;
}
