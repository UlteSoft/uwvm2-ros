/** @file Portable simulator-only sampled-q hold, shared by native and Wasm.
 * This is a bounded single-step pose/damping fixture, not walking, balance,
 * gravity compensation, actuator safety or firmware control. At each delivered
 * state it samples q again; it does NOT claim a persistent initial-pose target.
 * Native physics, real joint mapping and safe operation remain external duties.
 * No I/O, clock, allocator, ROS import, authority or mutable input is involved.
 */
#ifndef UWVM_UNITREE_SIMULATION_HOLD_H
#define UWVM_UNITREE_SIMULATION_HOLD_H
#include "crc.h"
#define UWVM_UNITREE_HOLD_CONFIG_BYTES 59u
typedef struct uwvm_unitree_hold_config {
    uint32_t steps,dt_ns;
    uint8_t carrier,active_count,mode_pr,mode_machine,active_slots[35];
    float kp,kd;
} uwvm_unitree_hold_config;
typedef struct uwvm_unitree_hold_motor {
    uint8_t mode;
    float q,dq,kp,kd,tau;
} uwvm_unitree_hold_motor;
/** Validate the complete trusted model-map configuration. The ordered list maps
 * each actual model actuator to exactly one carrier slot; it is not inferred
 * from the robot name or the carrier capacity. Unused list entries must255.
 * Gains are finite positive canonical binary32, kp<=1000 and kd<=100. These
 * fixture bounds are not a manufacturer's hardware limit recommendation.
 */
static inline int uwvm_unitree_hold_config_valid(uwvm_unitree_hold_config const* config) {
    uint64_t seen=0;unsigned i;
    if(!config||(config->carrier!=20&&config->carrier!=35)||config->steps<1||config->steps>32||
       config->dt_ns<100000||config->dt_ns>20000000||!config->active_count||config->active_count>config->carrier||
       !uwvm_unitree_finite(config->kp)||!uwvm_unitree_finite(config->kd)||config->kp<=0||config->kp>1000||config->kd<=0||config->kd>100)return 0;
    if(config->carrier==20&&(config->mode_pr||config->mode_machine))return 0;
    for(i=0;i<35;++i){uint8_t slot=config->active_slots[i];
        if(i>=config->active_count){if(slot!=255)return 0;continue;}
        if(slot>=config->carrier||(seen&(UINT64_C(1)<<slot)))return 0;seen|=UINT64_C(1)<<slot;
    }
    return 1;
}
/** Exact59-byte UHL1 format: magic4, stepsLE32, dt_nsLE32, carrier/active_count/
 * mode_pr/mode_machine4bytes, kpLE32/kdLE32, ordered35 slotbytes. On any22 the
 * caller config is unchanged. Native-object layout/padding is never serialized.
 */
static inline uint32_t uwvm_unitree_hold_decode(void const* input,size_t bytes,uint8_t expected_carrier,uwvm_unitree_hold_config* output) {
    uint8_t const* p=(uint8_t const*)input;uwvm_unitree_hold_config next;uint32_t value;unsigned i;
    if(!input||!output||bytes!=UWVM_UNITREE_HOLD_CONFIG_BYTES||
       uwvm_unitree_overlap(input,bytes,output,sizeof *output)||memcmp(input,"UHL1",4))return 22;
    memset(&next,0,sizeof next);next.steps=uwvm_unitree_get32(p+4);next.dt_ns=uwvm_unitree_get32(p+8);
    next.carrier=p[12];next.active_count=p[13];next.mode_pr=p[14];next.mode_machine=p[15];
    value=uwvm_unitree_get32(p+16);memcpy(&next.kp,&value,4);value=uwvm_unitree_get32(p+20);memcpy(&next.kd,&value,4);
    for(i=0;i<35;++i)next.active_slots[i]=p[24+i];
    if(next.carrier!=expected_carrier||!uwvm_unitree_hold_config_valid(&next))return 22;*output=next;return 0;
}
/** Emit one complete carrier without modifying config or sampled state.
 * Active slots use the current delivered state's exact q, dq/tau0 and selected
 * gains/mode1. Inactive GO slots use the official stop sentinel with gains/tau0;
 * inactive HG slots are allzero/mode0. The caller's fresh full LowCmd must keep
 * ALL named reserved fields0 and set the family CRC after copying these values.
 * This function commits output only after complete validation/capacity/overlap
 * checks; errors22/90 preserve it. It is not a persistent controller object.
 */
static inline uint32_t uwvm_unitree_hold_step(uwvm_unitree_hold_config const* config,float const* sampled_q,size_t state_count,uwvm_unitree_hold_motor* output,size_t capacity) {
    uwvm_unitree_hold_motor next[35];unsigned i;
    if(!uwvm_unitree_hold_config_valid(config)||!sampled_q||!output||state_count!=config->carrier)return 22;
    if(capacity<config->carrier)return 90;
    if(uwvm_unitree_overlap(output,config->carrier*sizeof *output,config,sizeof *config)||
       uwvm_unitree_overlap(output,config->carrier*sizeof *output,sampled_q,state_count*sizeof *sampled_q))return 22;
    memset(next,0,sizeof next);
    for(i=0;i<config->carrier;++i){
        if(!uwvm_unitree_finite(sampled_q[i]))return 22;
        next[i].mode=config->carrier==20?1:0;next[i].q=config->carrier==20?2.146E9f:0;
        next[i].dq=config->carrier==20?16000.0f:0;next[i].kp=0;next[i].kd=0;next[i].tau=0;
    }
    for(i=0;i<config->active_count;++i){unsigned slot=config->active_slots[i];
        next[slot].mode=1;next[slot].q=sampled_q[slot];next[slot].dq=0;next[slot].kp=config->kp;next[slot].kd=config->kd;
    }
    memcpy(output,next,config->carrier*sizeof *output);return 0;
}
#endif
