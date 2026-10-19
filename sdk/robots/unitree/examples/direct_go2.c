/** @file Caller-paced C11 Go2 controller using the real ROS2 Guest C API.
 *
 * This executable is for the admitted MuJoCo Go2 model only. It is not a
 * physical robot driver. Compile the complete generated GO family beside
 * this file as bounded_records.h, and link direct_go2_output.cpp into this
 * SAME Wasm. The latter only formats records with fast_io; all ROS operations,
 * generated decoding/encoding and ownership below remain ordinary C code.
 *
 * The default build opens each selected alias once and requires denial13.
 * Positive compilation selects UWVM_UNITREE_DIRECT_ENABLED=1, but grants no
 * authority. The trusted Runtime must separately select only LowCmd publish
 * and LowState subscribe on the isolated domain1/loopback Gateway.
 *
 * Input is UCK1/countLE32/dt_nsLE32, then one R per poll, then Q after exactly
 * count publications. A real initial LowState tick1 precedes command1. No
 * clock, random source, filesystem, socket, thread or native ROS relay exists
 * in this Guest. Each genuine callback-equivalent poll consumes one complete
 * generated message and calls the same SDK C kernel as the native baseline.
 */
#include <roswasm/ros2.h>
#include <roswasm_unitree/crc.h>
#include <roswasm_unitree/simulation_control.h>
#include "bounded_records.h"
#include "basic_io.h"
#ifndef UWVM_UNITREE_DIRECT_ENABLED
#define UWVM_UNITREE_DIRECT_ENABLED 0
#endif
#ifndef UWVM_UNITREE_MODULAR_MOTION
#define UWVM_UNITREE_MODULAR_MOTION 0
#endif
#ifndef UWVM_UNITREE_MODULAR_PERCEPTION
#define UWVM_UNITREE_MODULAR_PERCEPTION 0
#endif
#if UWVM_UNITREE_MODULAR_MOTION && !UWVM_UNITREE_MODULAR_PERCEPTION
#error "This language's explicit modular recipe requires the complete three-provider graph"
#endif
#if UWVM_UNITREE_MODULAR_MOTION
#include <unitree_modular_control.h>
#endif
#if UWVM_UNITREE_DIRECT_ENABLED != 0 && UWVM_UNITREE_DIRECT_ENABLED != 1
#error "Select the default-denied or explicit positive simulator recipe"
#endif
typedef roswasmc_bounded_2_unitree_go_msg_LowCmd Command;
typedef roswasmc_bounded_2_unitree_go_msg_LowState State;
/* This Guest-local C ABI formatter imports only basic stdout. It owns no ROS
 * capability and cannot publish, poll, select a handle or alter the policy. */
extern uint32_t uwvm_unitree_direct_record(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t const[36]);

static uint32_t close_owned(roswasmc_capability* subscription,
                            roswasmc_capability* publisher,uint32_t first) {
    uint32_t code;
    if(roswasmc_is_open(subscription)){code=roswasmc_close(subscription);if(!first)first=code;}
    if(roswasmc_is_open(publisher)){code=roswasmc_close(publisher);if(!first)first=code;}
    return first;
}
static uint32_t denied(roswasmc_context* context) {
    char const* names[2]={"unitree-lowcmd","unitree-lowstate"};
    uint32_t first=0;
    for(unsigned i=0;i<2;++i){
        roswasmc_capability owner;roswasmc_capability_init(&owner,context);
        uint32_t code=roswasmc_open(&owner,names[i],strlen(names[i]));
        if(code!=13||roswasmc_is_open(&owner)){if(!first)first=code?code:22;}
        if(roswasmc_is_open(&owner)){code=roswasmc_close(&owner);if(!first)first=code;}
    }
    if(!first)first=uwvm_unitree_example_output("PASS_UNITREE_DIRECT_DEFAULT_DENIED\n",sizeof("PASS_UNITREE_DIRECT_DEFAULT_DENIED\n")-1);
    return first;
}
/** A finite copied poll is the C equivalent of the C++ subscription callback.
 * Decode every full-carrier field before using active motor words. Never
 * increment the step or emit a success record until publish actually succeeds.
 */
int main(void) {
    roswasmc_context context={roswasmc_ros2_backend()};
    roswasmc_capability publisher,subscription;
    static State state;static Command command;
    static unsigned char incoming[ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWSTATE_MAX_WIRE_BYTES];
    static unsigned char outgoing[ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWCMD_MAX_WIRE_BYTES];
    uint8_t setup[12];uint32_t error=0,step=0,polls=0;
    if(uwvm_ros2_abi_version()!=1)return 95;
    if(!UWVM_UNITREE_DIRECT_ENABLED)return (int)denied(&context);
    error=uwvm_unitree_example_input(setup,sizeof setup);if(error)return (int)error;
    uint32_t count=uwvm_unitree_get32(setup+4),dt_ns=uwvm_unitree_get32(setup+8);
    if(memcmp(setup,"UCK1",4)||count<1||count>32||dt_ns<100000||dt_ns>20000000)return 22;
    roswasmc_capability_init(&publisher,&context);roswasmc_capability_init(&subscription,&context);
    error=roswasmc_open(&publisher,"unitree-lowcmd",sizeof("unitree-lowcmd")-1);
    if(!error)error=roswasmc_open(&subscription,"unitree-lowstate",sizeof("unitree-lowstate")-1);
    if(!error)error=roswasmc_subscribe(&subscription);
    if(!error)error=uwvm_unitree_example_output("READY_UNITREE_DIRECT_V1\n",sizeof("READY_UNITREE_DIRECT_V1\n")-1);
    while(!error&&step<count&&polls++<4096){
        char gate=0;error=uwvm_unitree_example_input(&gate,1);
        if(error)break;if(gate!='R'){error=22;break;}
        roswasmc_result result=roswasmc_poll(&subscription,incoming,sizeof incoming);
        if(result.error==11){error=uwvm_unitree_example_output("UNITREE_AGAIN\n",sizeof("UNITREE_AGAIN\n")-1);continue;}
        error=result.error;if(error)break;
        if(!result.has_length||result.length>sizeof incoming){error=22;break;}
        error=roswasmc_bounded_2_unitree_go_msg_LowState_decode(incoming,(size_t)result.length,&state);
        if(error)break;if(state.tick!=step+1){error=22;break;}
        uint32_t words[36];
        for(unsigned i=0;i<12;++i){
            if(!uwvm_unitree_finite(state.motor_state[i].q)||!uwvm_unitree_finite(state.motor_state[i].dq)||
               !uwvm_unitree_finite(state.motor_state[i].tau_est)){error=22;break;}
            memcpy(words+3*i,&state.motor_state[i].q,4);memcpy(words+3*i+1,&state.motor_state[i].dq,4);
            memcpy(words+3*i+2,&state.motor_state[i].tau_est,4);
        }
        if(error)break;
        uwvm_unitree_fixture_motor motors[20];
#if UWVM_UNITREE_MODULAR_MOTION
        error=uwvm_unitree_modular_observe(step+1,words);
        if(!error)error=uwvm_unitree_modular_compute(step+1,dt_ns,motors);
#else
        error=(uint32_t)uwvm_unitree_go2_fixture_step(step+1,(double)dt_ns/1000000000.0,motors);
#endif
        if(error)break;
        uwvm_unitree_go_lowcmd view={0};uint8_t packed[UWVM_UNITREE_GO_LOWCMD_BYTES];size_t packed_size=0,length=0;
        memset(&command,0,sizeof command);view.head[0]=command.head[0]=0xfe;view.head[1]=command.head[1]=0xef;
        view.level_flag=command.level_flag=0xff;
        for(unsigned i=0;i<20;++i){
            view.motor_cmd[i].mode=command.motor_cmd[i].mode=(uint8_t)motors[i].mode;
            view.motor_cmd[i].q=command.motor_cmd[i].q=motors[i].q;
            view.motor_cmd[i].dq=command.motor_cmd[i].dq=motors[i].dq;
            view.motor_cmd[i].kp=command.motor_cmd[i].kp=motors[i].kp;
            view.motor_cmd[i].kd=command.motor_cmd[i].kd=motors[i].kd;
            view.motor_cmd[i].tau=command.motor_cmd[i].tau=motors[i].tau;
        }
        error=uwvm_unitree_go_lowcmd_pack_crc(&view,packed,sizeof packed,&packed_size,&command.crc);
        if(!error&&packed_size!=sizeof packed)error=22;
        if(!error)error=roswasmc_bounded_2_unitree_go_msg_LowCmd_encode(&command,outgoing,sizeof outgoing,&length);
        if(!error)error=roswasmc_publish(&publisher,outgoing,length);
        if(!error){++step;error=uwvm_unitree_direct_record(step,state.tick,command.crc,state.crc,words);}
    }
    if(!error&&step!=count)error=110;
    if(!error){char gate=0;error=uwvm_unitree_example_input(&gate,1);if(!error&&gate!='Q')error=22;}
    error=close_owned(&subscription,&publisher,error);
    /* Reuse the bounded shared formatter for the final decimal count without
     * implementing a second number-to-text algorithm in the C example. */
    if(!error)error=uwvm_unitree_direct_record(step,0,0,0,NULL);
    return (int)error;
}
