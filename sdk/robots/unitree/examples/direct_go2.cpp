/** @file Direct typed Guest Go2 low-level controller, NOT an SDK2 relay.
 *
 * Every LowState is consumed through the real Runtime event_poll import and
 * decoded by the complete generated codec. Every LowCmd uses the real Runtime
 * topic_publish import. Root separately owns native DDS/ROS/MuJoCo witnesses.
 * Never use this simulator fixture as a physical robot driver.
 *
 * Default UWVM_UNITREE_DIRECT_ENABLED=0 requires13 for each alias and performs
 * no poll/publication. Positive mode requires an explicit compile selection,
 * pinned generated GO catalog, and independent Gateway grants for exactly
 * unitree-lowcmd/topic.publish and unitree-lowstate/topic.subscribe. No rights
 * are requested or inferred by the example. Gateway domain1/loopback and the
 * exact Go2 model/actuator mapping are trusted controller prerequisites.
 *
 * Positive stdin: UCK1 + countLE32(1..32) + dt_nsLE32(100000..20000000),
 * then one R per caller-paced poll (at most4096), then Q only after count
 * publications. Initial actual state tick1 precedes command1. The independent
 * simulator emits tickN+1 after applying commandN. No wall clock is imported.
 * Command motor order: FR,FL,RR,RL; hip/thigh/calf; slots12..19 official stop.
 */
#include <roswasm/ros2.h>
#include <roswasm_unitree/crc.hpp>
#include "bounded_records.hpp"
#include "basic_io.h"
#include <roswasm_unitree/simulation_control.h>
#include <fast_io.h>
#include "direct_text.h"
#ifndef UWVM_UNITREE_MODULAR_MOTION
#define UWVM_UNITREE_MODULAR_MOTION 0
#endif
#ifndef UWVM_UNITREE_MODULAR_PERCEPTION
#define UWVM_UNITREE_MODULAR_PERCEPTION 0
#endif
#if UWVM_UNITREE_MODULAR_MOTION && UWVM_UNITREE_MODULAR_PERCEPTION
#include <unitree_modular_control.h>
#elif UWVM_UNITREE_MODULAR_MOTION
#include <unitree_motion_abi.h>
#endif
#ifndef UWVM_UNITREE_DIRECT_ENABLED
#define UWVM_UNITREE_DIRECT_ENABLED 0
#endif
using Command=uwvm_custom::Ros2Unitree_goMsgLowCmd;
using State=uwvm_custom::Ros2Unitree_goMsgLowState;
namespace {
template<class... Values> uint32_t line(Values const&... values) {
    auto text=uwvm_unitree_example_text::format(values...);
    return uwvm_unitree_example_output(text.data(),text.size());
}
uint32_t bits(float value){uint32_t result;memcpy(&result,&value,4);return result;}
}
int main(){
    if(uwvm_ros2_abi_version()!=1)return 95;
    roswasmc_context context{roswasmc_ros2_backend()};
    if(!UWVM_UNITREE_DIRECT_ENABLED){
        uint32_t error=0;
        for(auto name:{"unitree-lowcmd","unitree-lowstate"}){
            roswasmc_capability cap;roswasmc_capability_init(&cap,&context);
            auto status=roswasmc_open(&cap,name,strlen(name));
            if(status!=13||roswasmc_is_open(&cap))error=22;
            if(roswasmc_is_open(&cap)){auto cleanup=roswasmc_close(&cap);if(!error)error=cleanup;}
        }
        if(!error)error=line("PASS_UNITREE_DIRECT_DEFAULT_DENIED\n");return static_cast<int>(error);
    }
    uint8_t setup[12];uint32_t error=uwvm_unitree_example_input(setup,sizeof setup);
    if(error)return static_cast<int>(error);
    auto count=uwvm_unitree_get32(setup+4),dt_ns=uwvm_unitree_get32(setup+8);
    if(memcmp(setup,"UCK1",4)||count<1||count>32||dt_ns<100000||dt_ns>20000000)return 22;
    roswasmcpp::Publisher<Command> publisher{context,"unitree-lowcmd"};
    if(publisher.error())return static_cast<int>(publisher.error());
    roswasmcpp::Subscription<State> subscription{context,"unitree-lowstate",roswasmcpp::Codec<State>::max_wire_bytes};
    error=subscription.error();uint32_t step=0;
    subscription.on_message([&](State const& state){
        if(error)return;
        if(step>=count||state.tick!=step+1){error=22;return;}
        // The complete generated decoder already rejects every nonfinite
        // scalar in all20 slots. Keep the controller's active-input contract
        // explicit as well; a rejected state cannot produce a command.
        for(unsigned i=0;i<12;++i){auto const& m=state.motor_state[i];
            if(!uwvm_unitree_finite(m.q)||!uwvm_unitree_finite(m.dq)||!uwvm_unitree_finite(m.tau_est)){error=22;return;}}
        uwvm_unitree_fixture_motor motors[20];
#if UWVM_UNITREE_MODULAR_MOTION
#if UWVM_UNITREE_MODULAR_PERCEPTION
        // Actual LowState words enter the pure perception instance before
        // motion calls its separate twelve-joint Wasm callback instance.
        uint32_t observed[36];
        for(unsigned i=0;i<12;++i){auto const& m=state.motor_state[i];
            observed[3*i]=bits(m.q);observed[3*i+1]=bits(m.dq);observed[3*i+2]=bits(m.tau_est);}
        error=uwvm_unitree_modular_observe(step+1,observed);
        if(!error)error=uwvm_unitree_modular_compute(step+1,dt_ns,motors);
#else
        // Scalar-only imported motion→callbacks lives in independent memories.
        // Inspect status after EVERY unrestricted word; zero is valid float
        // data, never an error sentinel. End every acquired generation even on
        // copy failure. All library frames unwind before CRC/any ROS call.
        auto generation=uwvm_unitree_motion_begin(step+1,dt_ns);
        error=uwvm_unitree_motion_status();if(!generation&&!error)error=22;
        for(unsigned i=0;generation&&!error&&i<20;++i){uint32_t words[6];
            for(unsigned field=0;!error&&field<6;++field){words[field]=uwvm_unitree_motion_word(generation,i,field);error=uwvm_unitree_motion_status();}
            if(error)break;if(words[0]>255){error=22;break;}motors[i].mode=words[0];
            memcpy(&motors[i].q,&words[1],4);memcpy(&motors[i].dq,&words[2],4);
            memcpy(&motors[i].kp,&words[3],4);memcpy(&motors[i].kd,&words[4],4);memcpy(&motors[i].tau,&words[5],4);
        }
        if(generation){auto cleanup=uwvm_unitree_motion_end(generation);auto after=uwvm_unitree_motion_status();if(!error)error=cleanup?cleanup:after;}
#endif
#else
        error=static_cast<uint32_t>(uwvm_unitree_go2_fixture_step(step+1,static_cast<double>(dt_ns)/1000000000.0,motors));
#endif
        if(error)return;
        Command command{};command.head[0]=0xfe;command.head[1]=0xef;command.level_flag=0xff;
        for(unsigned i=0;i<20;++i){auto& m=command.motor_cmd[i];m.mode=static_cast<uint8_t>(motors[i].mode);m.q=motors[i].q;m.dq=motors[i].dq;m.kp=motors[i].kp;m.kd=motors[i].kd;m.tau=motors[i].tau;}
        error=roswasm_unitree::set_go_lowcmd_crc(command);if(error)return;
        error=publisher.publish(command);if(error)return;++step;
        error=line("UNITREE_STEP ",step," ",state.tick," ",command.crc," ",state.crc,"\n");if(error)return;
        // Decimal uint32 bit patterns preserve exact binary32 observations,
        // independent of text float rounding, locale or host double precision.
        // This record contains only two uint32 identifiers and the closed
        // scalar-word carrier. Ten decimal digits per uint32 plus each literal
        // fit the same 2048-byte bound without a per-sample heap allocation.
        char text[2048];
        static_assert((sizeof("UNITREE_STATE ")-1u)+10u+1u+10u+36u*11u+1u<=sizeof text);
        fast_io::obuffer_view sink{text,text+sizeof text};
        fast_io::print(sink,"UNITREE_STATE ",step," ",state.tick);
        for(unsigned i=0;i<12;++i){auto const& m=state.motor_state[i];fast_io::print(sink," ",bits(m.q)," ",bits(m.dq)," ",bits(m.tau_est));}
        fast_io::print(sink,"\n");error=uwvm_unitree_example_output(sink.data(),sink.size());
    });
    if(!error)error=line("READY_UNITREE_DIRECT_V1\n");
    unsigned polls=0;
    while(!error&&step<count&&polls++<4096){
        char gate=0;error=uwvm_unitree_example_input(&gate,1);if(error)break;if(gate!='R'){error=22;break;}
        auto polled=subscription.poll_once();if(polled==11){error=line("UNITREE_AGAIN\n");continue;}if(polled)error=polled;
    }
    if(!error&&step!=count)error=110;
    if(!error){char gate=0;error=uwvm_unitree_example_input(&gate,1);if(!error&&gate!='Q')error=22;}
    if(subscription.is_open()){auto cleanup=subscription.close();if(!error)error=cleanup;}
    if(publisher.is_open()){auto cleanup=publisher.close();if(!error)error=cleanup;}
    if(!error)error=line("PASS_UNITREE_DIRECT steps=",step,"\n");
    return static_cast<int>(error);
}
