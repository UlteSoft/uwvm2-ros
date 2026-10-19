/** @file Checked main-local composition of pure perception/motion/callbacks.
 *
 * C, C++ and the embedded-JavaScript C adapter use these same scalar calls.
 * The perception input is the real decoded LowState, not a host-generated
 * stand-in. The motion output is the exact simulation_control.h trajectory
 * also compiled by the independent native controller. Each motion begin calls
 * the separate callbacks module twelve times before exposing output words.
 * All providers have separate memories and zero ROS/WASI authority. Main
 * alone calculates the official CRC and publishes after every import returns.
 */
#ifndef UWVM_UNITREE_MODULAR_CONTROL_H
#define UWVM_UNITREE_MODULAR_CONTROL_H
#include "unitree_perception_abi.h"
#include <roswasm_unitree/simulation_control.h>
#include <string.h>
static inline uint32_t uwvm_unitree_modular_observe(uint32_t step,uint32_t const words[36]) {
    uint32_t gen=uwvm_unitree_perception_begin(step),error=uwvm_unitree_perception_status();
    uint32_t checksum=uwvm_um_checksum_word(UINT32_C(2166136261),step);
    if(!gen&&!error)error=22;
    for(unsigned i=0;gen&&!error&&i<36;++i){
        error=uwvm_unitree_perception_word(gen,i,words[i]);
        if(!error)checksum=uwvm_um_checksum_word(checksum,words[i]);
    }
    if(gen&&!error)error=uwvm_unitree_perception_end(gen,checksum);
    if(gen&&error){uint32_t cleanup=uwvm_unitree_perception_abort(gen);(void)cleanup;}
    /* The primary status is retained. A nonzero abort cannot fabricate success;
     * this command terminates, so no possibly active provider is reused. */
    return error;
}
static inline uint32_t uwvm_unitree_modular_compute(uint32_t step,uint32_t dt_ns,
                                                   uwvm_unitree_fixture_motor motors[20]) {
    uint32_t gen=uwvm_unitree_motion_begin(step,dt_ns),error=uwvm_unitree_motion_status();
    if(!gen&&!error)error=22;
    for(unsigned slot=0;gen&&!error&&slot<20;++slot){
        uint32_t words[6];
        for(unsigned field=0;!error&&field<6;++field){
            words[field]=uwvm_unitree_motion_word(gen,slot,field);error=uwvm_unitree_motion_status();
        }
        if(error)break;
        if(words[0]>255){error=22;break;}
        for(unsigned field=1;field<6;++field)if((words[field]&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))error=22;
        if(error)break;
        motors[slot].mode=words[0];memcpy(&motors[slot].q,words+1,4);memcpy(&motors[slot].dq,words+2,4);
        memcpy(&motors[slot].kp,words+3,4);memcpy(&motors[slot].kd,words+4,4);memcpy(&motors[slot].tau,words+5,4);
    }
    if(gen){uint32_t cleanup=uwvm_unitree_motion_end(gen),status=uwvm_unitree_motion_status();
        if(!error)error=cleanup?cleanup:status;}
    return error;
}
#endif
