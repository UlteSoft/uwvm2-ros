/** Standalone TRUSTED native process, never a Wasm library or Guest import.
 * Full native mros frames become NEW ROS2 bridge messages. Every network name,
 * SDK/model selection, finite window and command count is explicit. Default
 * executable has no SDK init or endpoint effects. This finite observation/
 * command process is not a hardware emergency-stop or native cancellation API.
 * Root owns the separate process, library/input identity and external controller
 * watchdog. The vendor SDK has no public ApiBase shutdown; its borrowed singleton
 * exists until this one process retires, never across Runtime Source generations.
 */
#include "conversion.hpp"
#include "report.hpp"
#include <limxsdk/pointfoot.h>
#include <limxsdk/humanoid.h>
#include <limxsdk/topic_channel.h>
#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <array>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#ifndef UWVM_LIMX_BRIDGE_ENABLE
#define UWVM_LIMX_BRIDGE_ENABLE 0
#endif
static_assert(UWVM_LIMX_BRIDGE_ENABLE==0||UWVM_LIMX_BRIDGE_ENABLE==1);
namespace {
using Clock=std::chrono::steady_clock;
struct Frame {
    std::array<unsigned char,uwvm_limx::maximum_native_packet> bytes{};
    std::size_t size{};bool pending{};Clock::time_point arrived{};
};
struct State {
    std::mutex mutex;
    static_assert(std::atomic<std::uint32_t>::is_always_lock_free);
    std::atomic<std::uint32_t> callback_error{};
    Frame state_frame,imu_frame;
    bool live{},poisoned{};
    std::uint32_t primary{},close_state{},close_imu{},close_command{},shutdown_error{},commands{},command_limit{},motors{};
    Clock::time_point until{},last_state{};
    bool have_state{};
    void* native_state{};void* native_imu{};void* native_command{};
    bool state_close_attempted{},imu_close_attempted{},command_close_attempted{};
    rclcpp::Publisher<uwvm_limx::bridge::JointState>::SharedPtr state_output;
    rclcpp::Publisher<uwvm_limx::bridge::ImuData>::SharedPtr imu_output;
    std::vector<unsigned char> command_scratch;
    std::vector<std::string> actual_motor_names;
    void fail(std::uint32_t code) noexcept {poisoned=true;if(!primary)primary=code;}
};
bool decimal(char const* s,std::uint32_t maximum,std::uint32_t& out) noexcept {
    if(!s||!*s)return false;std::uint32_t n{};
    for(;*s;++s){if(*s<'0'||*s>'9')return false;auto d=static_cast<unsigned>(*s-'0');if(n>(maximum-d)/10u)return false;n=n*10u+d;}
    if(!n||n>maximum)return false;out=n;return true;
}
bool absolute_topic(std::string const& name) noexcept {
    if(name.empty()||name.size()>255||name[0]!='/')return false;
    for(unsigned char c:name)if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='/'||c=='_'))return false;
    return true;
}
/** Actual RawFrameCallback must not block. Do not allocate, decode or publish
 * ROS in the SDK dispatch thread. A busy/full single slot is terminal105; it
 * never overwrites an unprocessed frame or masquerades as current telemetry.
 */
void capture(State& s,Frame& frame,unsigned char const* data,int size) noexcept {
    std::unique_lock lock{s.mutex,std::try_to_lock};
    if(!lock.owns_lock()){std::uint32_t expected{};(void)s.callback_error.compare_exchange_strong(expected,105);return;}
    if(!s.live||s.poisoned)return;
    if(size<0||size>static_cast<int>(frame.bytes.size())||(!data&&size)){s.fail(90);return;}
    if(frame.pending){s.fail(105);return;}
    if(size)std::memcpy(frame.bytes.data(),data,static_cast<std::size_t>(size));
    frame.size=static_cast<std::size_t>(size);frame.arrived=Clock::now();frame.pending=true;
}
void state_frame(unsigned char const* data,int size,void* user) noexcept {
    auto& s=*static_cast<State*>(user);capture(s,s.state_frame,data,size);
}
void imu_frame(unsigned char const* data,int size,void* user) noexcept {
    auto& s=*static_cast<State*>(user);capture(s,s.imu_frame,data,size);
}
bool borrowed_frame(State& s,Frame& source,Frame& local) noexcept {
    std::lock_guard lock{s.mutex};
    if(!s.live||s.poisoned||!source.pending)return false;
    local.size=source.size;local.arrived=source.arrived;
    if(local.size)std::memcpy(local.bytes.data(),source.bytes.data(),local.size);
    source.pending=false;return true;
}
void consume(State& s) noexcept {
    auto fail=[&](std::uint32_t code)noexcept{std::lock_guard lock{s.mutex};s.fail(code);};
    auto callback_error=s.callback_error.load();if(callback_error){fail(callback_error);return;}
    try {
        Frame frame;
        if(borrowed_frame(s,s.state_frame,frame)){
            uwvm_limx::native::JointState value{};limxsdk::codec::Reader reader{frame.bytes.data(),frame.size};
            if(!value.decode(reader)||!reader.exhausted()){fail(22);return;}
            uwvm_limx::bridge::JointState converted{};auto status=uwvm_limx::copy(value,converted);
            if(status||converted.na!=s.motors){fail(status?status:22);return;}
            s.state_output->publish(converted);
            std::lock_guard lock{s.mutex};s.last_state=frame.arrived;s.have_state=true;
        }
        if(borrowed_frame(s,s.imu_frame,frame)){
            uwvm_limx::native::IMUData value{};limxsdk::codec::Reader reader{frame.bytes.data(),frame.size};
            if(!value.decode(reader)||!reader.exhausted()){fail(22);return;}
            uwvm_limx::bridge::ImuData converted{};auto status=uwvm_limx::copy(value,converted);
            if(status){fail(status);return;}s.imu_output->publish(converted);
        }
    }catch(std::bad_alloc const&){fail(12);}catch(...){fail(5);}
}
template<class Native,class Message> void send(State& s,Message const& message) noexcept {
    std::lock_guard lock{s.mutex};if(!s.live||s.poisoned)return;
    if(auto error=s.callback_error.load()){s.fail(error);return;}
    auto now=Clock::now();
    if(now>=s.until||!s.have_state||now-s.last_state>std::chrono::milliseconds{100}||s.commands>=s.command_limit){s.fail(110);return;}
    try {
        Native value{};auto status=uwvm_limx::copy(message,value);
        if(status||value.na!=s.motors){s.fail(status?status:22);return;}
        if(!value.names.empty()&&value.names!=s.actual_motor_names){s.fail(13);return;}
        auto n=value.encodedSize();if(n>s.command_scratch.size()){s.fail(90);return;}
        limxsdk::codec::Writer writer{s.command_scratch.data(),n};
        if(!value.encode(writer)||writer.offset()!=n){s.fail(22);return;}
        if(Clock::now()>=s.until){s.fail(110);return;}
        ++s.commands; // Attempt admitted before actual effect; never replay.
        auto result=limxsdk::channel::publishRaw(s.native_command,s.command_scratch.data(),static_cast<int>(n));
        if(result!=0)s.fail(107);
    }catch(std::bad_alloc const&){s.fail(12);}catch(...){s.fail(5);}
}
/** closeSub's actual SDK contract drains callbacks before return. Never call
 * it from a callback or while holding its mutex. Every handle is relinquished
 * locally before the attempt. A throwing close has UNKNOWN completion; no retry
 * and no userdata destruction is safe. Caller then exits this owned process.
 */
bool close(State& s) noexcept {
    {std::lock_guard lock{s.mutex};s.live=false;}
    bool known=true;
    auto subscription=[&](void*& handle,bool& attempted,std::uint32_t& error){if(!handle)return;auto value=std::exchange(handle,nullptr);attempted=true;try{limxsdk::channel::closeSub(value);}catch(...){known=false;error=5;}};
    subscription(s.native_imu,s.imu_close_attempted,s.close_imu);subscription(s.native_state,s.state_close_attempted,s.close_state);
    if(s.native_command){auto value=std::exchange(s.native_command,nullptr);s.command_close_attempted=true;try{limxsdk::channel::closePub(value);}catch(...){known=false;s.close_command=5;}}
    return known;
}
} // namespace
int main(int argc,char** argv) {
    if(!UWVM_LIMX_BRIDGE_ENABLE)return 13;
    // CLASS MODEL IP native-state native-imu native-command ROS-prefix
    // window-ms maximum-commands expected-motors. No default domain/IP/firmware.
    if(argc!=11)return 22;
    std::string kind{argv[1]},model{argv[2]},ip{argv[3]},native_state{argv[4]},native_imu{argv[5]},native_command{argv[6]},prefix{argv[7]};
    std::uint32_t window{},count{},motors{};
    if((kind!="PointFoot"&&kind!="Humanoid")||model.empty()||model.size()>128||ip.empty()||
       !absolute_topic(native_state)||!absolute_topic(native_imu)||!absolute_topic(native_command)||!absolute_topic(prefix)||
       !decimal(argv[8],2000,window)||!decimal(argv[9],32,count)||!decimal(argv[10],64,motors))return 22;
    // The precise PF_P441A example is six motors in the official PointFoot
    // order. No claim that another label or firmware has this actuator map.
    if(kind=="PointFoot"&&(model!="PF_P441A"||motors!=6))return 95;
    State state;state.command_limit=count;state.motors=motors;state.command_scratch.resize(uwvm_limx::maximum_native_packet);
    bool initialized=false,close_known=true,unresolved_native_create=false;
    rclcpp::SubscriptionBase::SharedPtr command_input;
    try {
        rclcpp::init(0,nullptr);initialized=true;
        auto node=std::make_shared<rclcpp::Node>("uwvm_limx_bridge");
        state.state_output=node->create_publisher<uwvm_limx::bridge::JointState>(prefix+"/state",rclcpp::QoS(1).reliable());
        state.imu_output=node->create_publisher<uwvm_limx::bridge::ImuData>(prefix+"/imu",rclcpp::QoS(1).reliable());
        limxsdk::ApiBase* sdk=kind=="PointFoot"?static_cast<limxsdk::ApiBase*>(limxsdk::PointFoot::getInstance()):static_cast<limxsdk::ApiBase*>(limxsdk::Humanoid::getInstance());
        if(!sdk)throw std::runtime_error{"SDK owner unavailable"};
        unresolved_native_create=true;auto sdk_initialized=sdk->init(ip);unresolved_native_create=false;
        if(!sdk_initialized||sdk->getMotorNumber()!=motors)throw std::runtime_error{"SDK model/count admission failed"};
        state.actual_motor_names=sdk->getMotorNames();
        if(state.actual_motor_names.size()!=motors)throw std::runtime_error{"SDK complete motor map unavailable"};
        if(kind=="PointFoot"&&state.actual_motor_names!=std::vector<std::string>{"abad_L_Joint","hip_L_Joint","knee_L_Joint","abad_R_Joint","hip_R_Joint","knee_R_Joint"})throw std::runtime_error{"SDK PointFoot map differs"};
        using NState=uwvm_limx::native::JointState;using NImu=uwvm_limx::native::IMUData;
        unresolved_native_create=true;state.native_state=limxsdk::channel::openSub(native_state.c_str(),NState::TYPE,NState::MD5,NState::definition(),state_frame,&state);
        if(!state.native_state)throw std::runtime_error{"native state open failed"};unresolved_native_create=false;
        unresolved_native_create=true;state.native_imu=limxsdk::channel::openSub(native_imu.c_str(),NImu::TYPE,NImu::MD5,NImu::definition(),imu_frame,&state);
        if(!state.native_imu)throw std::runtime_error{"native IMU open failed"};unresolved_native_create=false;
        if(kind=="PointFoot") {
            using M=uwvm_limx::native::JointCmd;
            unresolved_native_create=true;state.native_command=limxsdk::channel::openPub(native_command.c_str(),M::TYPE,M::MD5,M::definition());
            if(!state.native_command)throw std::runtime_error{"native command open failed"};unresolved_native_create=false;
            command_input=node->create_subscription<uwvm_limx::bridge::JointCmd>(prefix+"/command",rclcpp::QoS(1).reliable(),[&](uwvm_limx::bridge::JointCmd::ConstSharedPtr value){if(value)send<M>(state,*value);});
        }else {
            using M=uwvm_limx::native::JointCmdLimx;
            unresolved_native_create=true;state.native_command=limxsdk::channel::openPub(native_command.c_str(),M::TYPE,M::MD5,M::definition());
            if(!state.native_command)throw std::runtime_error{"native command open failed"};unresolved_native_create=false;
            command_input=node->create_subscription<uwvm_limx::bridge::JointCmdLimx>(prefix+"/command",rclcpp::QoS(1).reliable(),[&](uwvm_limx::bridge::JointCmdLimx::ConstSharedPtr value){if(value)send<M>(state,*value);});
        }
        if(!state.native_command)throw std::runtime_error{"native command open failed"};
        {std::lock_guard lock{state.mutex};state.until=Clock::now()+std::chrono::milliseconds{window};state.live=true;}
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
        for(;;){
            {std::lock_guard lock{state.mutex};if(state.poisoned||Clock::now()>=state.until||state.commands>=count)break;}
            if(!rclcpp::ok())break;consume(state);executor.spin_once(std::chrono::milliseconds{1});
        }
        {std::lock_guard lock{state.mutex};if(state.commands<count&&!state.primary)state.fail(rclcpp::ok()?110:125);}
        executor.remove_node(node);command_input.reset();close_known=close(state)&&!unresolved_native_create;
    }catch(std::bad_alloc const&){{std::lock_guard lock{state.mutex};state.fail(12);}command_input.reset();close_known=close(state)&&!unresolved_native_create;}
    catch(...){{std::lock_guard lock{state.mutex};state.fail(5);}command_input.reset();close_known=close(state)&&!unresolved_native_create;}
    if(initialized)try{rclcpp::shutdown();}catch(...){state.shutdown_error=5;}
    auto result=state.primary?state.primary:state.close_imu?state.close_imu:state.close_state?state.close_state:state.close_command?state.close_command:state.shutdown_error;
    auto diagnostic=humanoid_native_report::emit("LIMX_NATIVE primary=",state.primary," close_imu=",state.close_imu," close_state=",state.close_state," close_command=",state.close_command," shutdown=",state.shutdown_error," imu_attempted=",static_cast<unsigned>(state.imu_close_attempted)," state_attempted=",static_cast<unsigned>(state.state_close_attempted)," command_attempted=",static_cast<unsigned>(state.command_close_attempted)," send_attempts=",state.commands," callback_error=",state.callback_error.load()," unresolved_create=",static_cast<unsigned>(unresolved_native_create));
    if(!result)result=diagnostic;
    if(!close_known)std::_Exit(static_cast<int>(result?result:5));
    return static_cast<int>(result);
}
