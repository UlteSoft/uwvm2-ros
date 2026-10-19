/** @file Generic direct typed GO20/HG35 single-step sampled-q hold fixture.
 * Complete generated LowState→shared simulation_hold.h→family full CRC→actual
 * LowCmd imports. No SDK2 relay, walking/balance policy or hardware access setup.
 * Default mode checks both aliases13 before reading any positive setup/input.
 * Explicit positive UHL1/R/Q, STEP/STATE protocol is described in README.
 */
#include <roswasm/ros2.h>
#include <roswasm_unitree/crc.hpp>
#include <roswasm_unitree/simulation_hold.h>
#include "bounded_records.hpp"
#include "basic_io.h"
#include <fast_io.h>
#include "direct_text.h"
#ifndef UWVM_UNITREE_HOLD_ENABLED
#define UWVM_UNITREE_HOLD_ENABLED 0
#endif
#if UWVM_UNITREE_HOLD_FAMILY == 1
using Command=uwvm_custom::Ros2Unitree_goMsgLowCmd;
using State=uwvm_custom::Ros2Unitree_goMsgLowState;
constexpr unsigned carrier=20;
#elif UWVM_UNITREE_HOLD_FAMILY == 2
using Command=uwvm_custom::Ros2Unitree_hgMsgLowCmd;
using State=uwvm_custom::Ros2Unitree_hgMsgLowState;
constexpr unsigned carrier=35;
#else
#error "Explicit GO1 or HG2 HOLD_FAMILY required"
#endif
namespace {
template<class... Values> uint32_t line(Values const&... values) {
    auto text=uwvm_unitree_example_text::format(values...);
    return uwvm_unitree_example_output(text.data(),text.size());
}
uint32_t bits(float value){uint32_t result;memcpy(&result,&value,4);return result;}
}
int main(){
    if(uwvm_ros2_abi_version()!=1)return 95;roswasmc_context context{roswasmc_ros2_backend()};
    if(!UWVM_UNITREE_HOLD_ENABLED){
        uint32_t error=0;for(auto name:{"unitree-lowcmd","unitree-lowstate"}){roswasmc_capability cap;roswasmc_capability_init(&cap,&context);
            auto code=roswasmc_open(&cap,name,strlen(name));if(code!=13||roswasmc_is_open(&cap))error=22;
            if(roswasmc_is_open(&cap)){auto closed=roswasmc_close(&cap);if(!error)error=closed;}}
        if(!error)error=line("PASS_UNITREE_HOLD_DEFAULT_DENIED\n");return static_cast<int>(error);
    }
    uint8_t bytes[UWVM_UNITREE_HOLD_CONFIG_BYTES];uint32_t error=0;
    // Reuse the held basic I/O helper's <=12-byte bound, without changing it.
    for(size_t at=0;at<sizeof bytes&&!error;){size_t n=sizeof bytes-at;if(n>12)n=12;error=uwvm_unitree_example_input(bytes+at,n);at+=n;}
    uwvm_unitree_hold_config config{};if(!error)error=uwvm_unitree_hold_decode(bytes,sizeof bytes,carrier,&config);if(error)return static_cast<int>(error);
    roswasmcpp::Publisher<Command> publisher{context,"unitree-lowcmd"};if(publisher.error())return static_cast<int>(publisher.error());
    roswasmcpp::Subscription<State> subscription{context,"unitree-lowstate",roswasmcpp::Codec<State>::max_wire_bytes};error=subscription.error();uint32_t step=0;
    subscription.on_message([&](State const& state){
        if(error)return;if(step>=config.steps||state.tick!=step+1){error=22;return;}
#if UWVM_UNITREE_HOLD_FAMILY == 2
        // A trusted selected HG operating mode must also match the independently
        // received state; do not only copy it into the outgoing command.
        if(state.mode_machine!=config.mode_machine){error=22;return;}
#endif
        float q[35];for(unsigned i=0;i<carrier;++i){auto const& m=state.motor_state[i];
            if(!uwvm_unitree_finite(m.q)||!uwvm_unitree_finite(m.dq)||!uwvm_unitree_finite(m.tau_est)){error=22;return;}q[i]=m.q;}
        uwvm_unitree_hold_motor motors[35];error=uwvm_unitree_hold_step(&config,q,carrier,motors,35);if(error)return;
        Command command{};
#if UWVM_UNITREE_HOLD_FAMILY == 1
        command.head[0]=0xfe;command.head[1]=0xef;command.level_flag=0xff;
#else
        command.mode_pr=config.mode_pr;command.mode_machine=config.mode_machine;
#endif
        for(unsigned i=0;i<carrier;++i){auto& m=command.motor_cmd[i];m.mode=motors[i].mode;m.q=motors[i].q;m.dq=motors[i].dq;m.kp=motors[i].kp;m.kd=motors[i].kd;m.tau=motors[i].tau;}
#if UWVM_UNITREE_HOLD_FAMILY == 1
        error=roswasm_unitree::set_go_lowcmd_crc(command);
#else
        error=roswasm_unitree::set_hg_lowcmd_crc(command);
#endif
        if(error)return;error=publisher.publish(command);if(error)return;++step;
        error=line("UNITREE_STEP ",step," ",state.tick," ",command.crc," ",state.crc,"\n");if(error)return;
        // This record contains only two uint32 identifiers and the closed
        // scalar-word carrier. Ten decimal digits per uint32 plus each literal
        // fit the same 2048-byte bound without a per-sample heap allocation.
        char text[2048];
        static_assert((sizeof("UNITREE_STATE ")-1u)+10u+1u+10u+3u*carrier*11u+1u<=sizeof text);
        fast_io::obuffer_view sink{text,text+sizeof text};fast_io::print(sink,"UNITREE_STATE ",step," ",state.tick);
        for(unsigned i=0;i<carrier;++i){auto const& m=state.motor_state[i];fast_io::print(sink," ",bits(m.q)," ",bits(m.dq)," ",bits(m.tau_est));}
        fast_io::print(sink,"\n");error=uwvm_unitree_example_output(sink.data(),sink.size());
    });
    if(!error)error=line("READY_UNITREE_HOLD_V1\n");unsigned polls=0;
    while(!error&&step<config.steps&&polls++<4096){char gate=0;error=uwvm_unitree_example_input(&gate,1);if(error)break;if(gate!='R'){error=22;break;}
        auto code=subscription.poll_once();if(code==11){error=line("UNITREE_AGAIN\n");continue;}if(code)error=code;}
    if(!error&&step!=config.steps)error=110;
    if(!error){char gate=0;error=uwvm_unitree_example_input(&gate,1);if(!error&&gate!='Q')error=22;}
    if(subscription.is_open()){auto code=subscription.close();if(!error)error=code;}if(publisher.is_open()){auto code=publisher.close();if(!error)error=code;}
    if(!error)error=line("PASS_UNITREE_HOLD steps=",step,"\n");return static_cast<int>(error);
}
