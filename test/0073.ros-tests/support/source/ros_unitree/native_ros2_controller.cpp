/** @file Compiled native ROS2 simulator baseline for direct_go2/direct_hold.
 *
 * This is an ordinary rclcpp process, NOT a Guest, Runtime identity, SDK2 relay
 * or mathematical-only "native" label. It uses actual GO/HG LowState and
 * LowCmd endpoints, the SAME SDK-shipped simulation kernel/CRC and exact
 * binary32 field order as the typed Guest. No callback thread, timer, ROS clock,
 * random value, hidden retry or unbounded executor spin is created here.
 *
 * Root must qualify its actual compiler/full-MD/link/ELF/SDK closure and run it
 * only in domain1 with an explicit loopback Cyclone profile, owned finite pipes
 * and independent native/simulator witnesses. Native OS ROS access is not Wasm
 * capability enforcement; never use this fixture as a physical robot driver.
 *
 * CLI has no address/domain/robot overrides. Stand stdin is UCK1+countLE32+dt_nsLE32;
 * explicitly compiled GO/HG hold mode instead takes the shared exact59B UHL1.
 * then R per one nonblocking spin_once (max4096), Q after exactlycount commands.
 * READY_UNITREE_NATIVE_V1 and PASS_UNITREE_NATIVE steps=N distinguish this role.
 * UNITREE_STEP/STATE records match the Guest byte grammar: stand emits36 decimal
 * uint32 binary32 words from the first12 motor states; hold emits every carrier
 * motor triple (60GO/105HG words), with distinct HOLD_NATIVE role markers.
 */
#include <rclcpp/rclcpp.hpp>
#ifndef UWVM_UNITREE_NATIVE_HG
#define UWVM_UNITREE_NATIVE_HG 0
#endif
#ifndef UWVM_UNITREE_NATIVE_HOLD
#define UWVM_UNITREE_NATIVE_HOLD 0
#endif
#if UWVM_UNITREE_NATIVE_HG
#include <unitree_hg/msg/low_cmd.hpp>
#include <unitree_hg/msg/low_state.hpp>
#if !UWVM_UNITREE_NATIVE_HOLD
#error "HG native baseline requires explicit sampled-hold mode"
#endif
#else
#include <unitree_go/msg/low_cmd.hpp>
#include <unitree_go/msg/low_state.hpp>
#endif
#include <roswasm_unitree/crc.hpp>
#include <roswasm_unitree/simulation_control.h>
#include <roswasm_unitree/simulation_hold.h>
#include <src/ros/gateway/common/native_io.h>
#include <chrono>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <memory>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unistd.h>
#if __cplusplus >= 202002L && defined(__cpp_concepts)
#include <fast_io.h>
#include <fast_io_unit/string.h>
#endif

namespace unitree_native_controller {
using Clock=std::chrono::steady_clock;
#if UWVM_UNITREE_NATIVE_HG
using Command=unitree_hg::msg::LowCmd;
using State=unitree_hg::msg::LowState;
constexpr unsigned carrier=35;
#else
using Command=unitree_go::msg::LowCmd;
using State=unitree_go::msg::LowState;
constexpr unsigned carrier=20;
#endif
namespace io=uwvm2::ros_gateway::native_io;
struct Failure {uint32_t code;};
void require(bool condition,uint32_t code=22){if(!condition)throw Failure{code};}
bool equal(char const* value,std::string_view expected){return value&&std::string_view{value}==expected;}
/** No implicit native address/domain selection. The controller independently
 * binds the actual loopback XML bytes and selected middleware/tool roots.
 */
void environment(){
    require(equal(::getenv("ROS_DOMAIN_ID"),"1"));
    require(equal(::getenv("RMW_IMPLEMENTATION"),"rmw_cyclonedds_cpp"));
    auto profile=::getenv("CYCLONEDDS_URI");require(profile&&*profile);
    require(::getenv("ROS_NAMESPACE")==nullptr&&::getenv("LD_PRELOAD")==nullptr);
}
void pipes(){
    require(::signal(SIGPIPE,SIG_IGN)!=SIG_ERR,107);
    for(int fd:{STDIN_FILENO,STDOUT_FILENO}){auto flags=::fcntl(fd,F_GETFL,0);require(flags>=0&&::fcntl(fd,F_SETFL,flags|O_NONBLOCK)==0,107);}
}
/** Bounded native control I/O, unrelated to ROS message retries. */
void input(void* output,size_t bytes){
    require(bytes<=59);auto end=Clock::now()+std::chrono::seconds{30};size_t at=0;unsigned attempts=0;
    while(at<bytes){
        require(++attempts<=128&&Clock::now()<end,110);
        auto remaining=std::chrono::duration_cast<std::chrono::milliseconds>(end-Clock::now()).count();
        pollfd descriptor{STDIN_FILENO,POLLIN,0};int ready=::poll(&descriptor,1,static_cast<int>(remaining>0?remaining:0));
        if(ready<0&&errno==EINTR)continue;require(ready>=0,107);require(ready!=0,110);
        require(!(descriptor.revents&(POLLERR|POLLNVAL)),107);
        auto got=::read(STDIN_FILENO,static_cast<unsigned char*>(output)+at,bytes-at);
        if(got<0&&(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK))continue;
        require(got>0&&static_cast<size_t>(got)<=bytes-at,107);at+=static_cast<size_t>(got);
    }
}
class Text {
#if __cplusplus >= 202002L && defined(__cpp_concepts)
    std::string bytes_;
#else
    io::fixed_text<4096> bytes_;
    template<class T> void one(T const& value){
        if constexpr(std::is_same_v<std::remove_cv_t<T>,char>)require(bytes_.append({&value,1}),90);
        else if constexpr(std::is_integral_v<T>)require(bytes_.number(value),90);
        else require(bytes_.append(std::string_view{value}),90);
    }
#endif
public:
    Text(){
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        bytes_.reserve(2048);
#endif
    }
    template<class... Values> void append(Values const&... values){
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        fast_io::ostring_ref_std sink{std::addressof(bytes_)};fast_io::print(sink,values...);
#else
        (one(values),...);
#endif
    }
    std::string_view view()const noexcept {
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        return bytes_;
#else
        return bytes_.view();
#endif
    }
};
void output(std::string_view value){
    require(value.size()<=4096,90);pollfd descriptor{STDOUT_FILENO,POLLOUT,0};int ready=-1;unsigned attempts=0;
    auto end=Clock::now()+std::chrono::seconds{1};
    do{
        require(++attempts<=128&&Clock::now()<end,110);
        auto remaining=std::chrono::duration_cast<std::chrono::milliseconds>(end-Clock::now()).count();
        ready=::poll(&descriptor,1,static_cast<int>(remaining>0?remaining:0));
    }while(ready<0&&errno==EINTR);
    require(ready>0&&(descriptor.revents&POLLOUT)&&!(descriptor.revents&(POLLERR|POLLHUP|POLLNVAL)),107);
    require(io::write_all(STDOUT_FILENO,value),107);
}
template<class... Values> void line(Values const&... values){Text text;text.append(values...);output(text.view());}
uint32_t bits(float value){uint32_t result;std::memcpy(&result,&value,4);return result;}
}

int main(int argc,char**){
    using namespace unitree_native_controller;
    uint32_t status=0;std::shared_ptr<rclcpp::Context> context;
    try{
        require(argc==1);environment();pipes();
#if UWVM_UNITREE_NATIVE_HOLD
        uint8_t setup[UWVM_UNITREE_HOLD_CONFIG_BYTES];input(setup,sizeof setup);uwvm_unitree_hold_config hold{};
        require(uwvm_unitree_hold_decode(setup,sizeof setup,carrier,&hold)==0);auto count=hold.steps;
#else
        uint8_t setup[12];input(setup,sizeof setup);
        auto count=uwvm_unitree_get32(setup+4),dt_ns=uwvm_unitree_get32(setup+8);
        require(!std::memcmp(setup,"UCK1",4)&&count>=1&&count<=32&&dt_ns>=100000&&dt_ns<=20000000);
#endif
        context=std::make_shared<rclcpp::Context>();rclcpp::InitOptions init;context->init(0,nullptr,init);
        uint32_t step=0;
        {
            // These are real public NodeOptions APIs. CMake compile+link probes
            // the complete quiet construction; missing APIs fail before this
            // program can create hidden rosout/parameter/service endpoints.
            rclcpp::NodeOptions options;options.context(context).use_global_arguments(false).enable_rosout(false);
            options.start_parameter_services(false).start_parameter_event_publisher(false).use_intra_process_comms(false);
            options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
            auto node=std::make_shared<rclcpp::Node>("uwvm_unitree_native_controller","/uwvm_unitree",options);
            auto qos=rclcpp::QoS{rclcpp::KeepLast{1}}.reliable().durability_volatile();
            auto publisher=node->create_publisher<Command>("/lowcmd",qos);
            bool permitted=false,received=false;
            auto subscription=node->create_subscription<State>("/lowstate",qos,[&](State::ConstSharedPtr state){
                require(permitted&&!received&&state&&step<count&&state->tick==step+1);received=true;
#if UWVM_UNITREE_NATIVE_HG
                require(state->mode_machine==hold.mode_machine);
#endif
                constexpr unsigned checked_slots=UWVM_UNITREE_NATIVE_HOLD?carrier:12;
                for(unsigned i=0;i<checked_slots;++i){auto const& m=state->motor_state[i];require(uwvm_unitree_finite(m.q)&&uwvm_unitree_finite(m.dq)&&uwvm_unitree_finite(m.tau_est));}
#if UWVM_UNITREE_NATIVE_HOLD
                float q[35];for(unsigned i=0;i<carrier;++i)q[i]=state->motor_state[i].q;
                uwvm_unitree_hold_motor motors[35];require(uwvm_unitree_hold_step(&hold,q,carrier,motors,35)==0);
#else
                uwvm_unitree_fixture_motor motors[20];
                require(uwvm_unitree_go2_fixture_step(step+1,static_cast<double>(dt_ns)/1000000000.0,motors)==0);
#endif
#if UWVM_UNITREE_NATIVE_HG
                Command command{};command.mode_pr=hold.mode_pr;command.mode_machine=hold.mode_machine;
#else
                Command command{};command.head[0]=0xfe;command.head[1]=0xef;command.level_flag=0xff;
#endif
                for(unsigned i=0;i<carrier;++i){auto& m=command.motor_cmd[i];m.mode=static_cast<uint8_t>(motors[i].mode);m.q=motors[i].q;m.dq=motors[i].dq;m.kp=motors[i].kp;m.kd=motors[i].kd;m.tau=motors[i].tau;}
#if UWVM_UNITREE_NATIVE_HG
                require(roswasm_unitree::set_hg_lowcmd_crc(command)==0);
#else
                require(roswasm_unitree::set_go_lowcmd_crc(command)==0);
#endif
                publisher->publish(command);++step;
                line("UNITREE_STEP ",step," ",state->tick," ",command.crc," ",state->crc,"\n");
                Text text;text.append("UNITREE_STATE ",step," ",state->tick);
                for(unsigned i=0;i<checked_slots;++i){auto const& m=state->motor_state[i];text.append(" ",bits(m.q)," ",bits(m.dq)," ",bits(m.tau_est));}
                text.append("\n");output(text.view());
            });
            rclcpp::ExecutorOptions executor_options;executor_options.context=context;
            rclcpp::executors::SingleThreadedExecutor executor{executor_options};executor.add_node(node);
#if UWVM_UNITREE_NATIVE_HOLD
            line("READY_UNITREE_HOLD_NATIVE_V1\n");
#else
            line("READY_UNITREE_NATIVE_V1\n");
#endif
            unsigned polls=0;
            while(step<count&&polls++<4096){
                char gate=0;input(&gate,1);require(gate=='R');received=false;permitted=true;
                // Exactly one nonblocking executor operation, not spin/spin_all.
                // Guard/metadata wakeups may yield AGAIN; a fresh parent R is
                // required before another callback can publish a command.
                executor.spin_once(std::chrono::nanoseconds{0});permitted=false;
                if(!received)line("UNITREE_AGAIN\n");
            }
            require(step==count,110);char gate=0;input(&gate,1);require(gate=='Q');
            executor.remove_node(node);subscription.reset();publisher.reset();node.reset();
        }
        require(context->shutdown("Unitree native simulation controller complete"),107);context.reset();
#if UWVM_UNITREE_NATIVE_HOLD
        line("PASS_UNITREE_HOLD_NATIVE steps=",step,"\n");
#else
        line("PASS_UNITREE_NATIVE steps=",step,"\n");
#endif
    }catch(Failure const& error){status=error.code;}
    catch(std::exception const&){status=22;}
    catch(...){status=22;}
    if(context&&context->is_valid()){
        try{if(!context->shutdown("Unitree native controller failure cleanup")&&!status)status=107;}
        catch(...){if(!status)status=107;}
    }
    if(status){if(!io::write_all(STDERR_FILENO,"native Unitree ROS2 controller refused\n"))return 107;}
    return static_cast<int>(status);
}
