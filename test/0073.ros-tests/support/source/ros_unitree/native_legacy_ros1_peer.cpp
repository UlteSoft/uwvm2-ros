/** @file Independent native ROS1 legacy LowState/LowCmd transport witness.
 *
 * Test-only isolated loopback process; literal /uwvm_unitree_legacy topics are
 * never firmware /lowcmd or /lowstate endpoints. It uses the ACTUAL eleven-type
 * catkin package and roscpp. Root must qualify full-MD/link/ELF/maps/identities
 * and own a private loopback-only netns plus a finite native ROS Master. Native
 * ROS access is OS authority, never proof of Wasm permission enforcement.
 *
 * Boot prints LEGACY_NATIVE_GATE pid before reading any control. Root proves
 * this actual executable/birth/pidfd/cgroup/SDK maps first. Input setup is16B:
 * ULN1 + positiveLE32(0/1) + sequenceLE32(42) + reservedLE32(0). No ROS context
 * is explicitly initialized before that gate. M checks exact self/Guest graph
 * counts; P publishes one complete LowState and confirms a real self-delivery;
 * A arms and then observes one complete Guest LowCmd; Z after positive retirement or Q after
 * denied Guest completion observes two seconds/at least30 quiet samples; F
 * closes this native role. All controls are single-use, finite and checked.
 *
 * Every received field is compared through actual ROS1 generated serialization
 * against independently constructed native values, not C++ memory/padding.
 * Full canonical bytes and a diagnostic FNV1a64 are emitted for Root/Guest
 * comparison. The crc field is observed data, NOT hardware CRC authentication.
 * No async spinner/author-created callback thread, unbounded retry, robot
 * address, external service, SDK install, Guest grant or Runtime is created.
 */
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <ros/serialization.h>
#include <ros/message_traits.h>
#include <ros/message_event.h>
#include <ros/subscribe_options.h>
#include "native_legacy_ros1_values.h"
#include "native_legacy_ros1_contract.h"
#include <src/ros/gateway/common/native_io.h>
#include <array>
#include <cerrno>
#include <charconv>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <unistd.h>

namespace legacy_native {
namespace io=uwvm2::ros_gateway::native_io;
using Clock=std::chrono::steady_clock;
struct Failure {unsigned code;};
void require(bool value,unsigned code=22){if(!value)throw Failure{code};}
bool equal(char const* value,std::string_view expected){return value&&std::string_view{value}==expected;}
void environment(){
    require(equal(::getenv("ROS_IP"),"127.0.0.1"));
    require(::getenv("ROS_HOSTNAME")==nullptr&&::getenv("ROS_NAMESPACE")==nullptr&&::getenv("LD_PRELOAD")==nullptr);
    auto master=::getenv("ROS_MASTER_URI");require(master);std::string_view address{master};
    constexpr std::string_view prefix{"http://127.0.0.1:"};require(address.substr(0,prefix.size())==prefix);
    auto port=address.substr(prefix.size());if(!port.empty()&&port.back()=='/')port.remove_suffix(1);
    unsigned value{};auto parsed=std::from_chars(port.data(),port.data()+port.size(),value);
    require(!port.empty()&&parsed.ec==std::errc{}&&parsed.ptr==port.data()+port.size()&&value>=1024&&value<=65535);
    require(::signal(SIGPIPE,SIG_IGN)!=SIG_ERR,107);
    for(int fd:{STDIN_FILENO,STDOUT_FILENO}){auto flags=::fcntl(fd,F_GETFL,0);require(flags>=0&&::fcntl(fd,F_SETFL,flags|O_NONBLOCK)==0,107);}
}
void input(void* output,std::size_t bytes){
    require(bytes&&bytes<=16);auto end=Clock::now()+std::chrono::seconds{30};std::size_t used=0;unsigned attempts=0;
    while(used<bytes){
        require(++attempts<=128&&Clock::now()<end,110);auto ms=std::chrono::duration_cast<std::chrono::milliseconds>(end-Clock::now()).count();
        pollfd fd{STDIN_FILENO,POLLIN,0};auto ready=::poll(&fd,1,static_cast<int>(ms>0?ms:0));
        if(ready<0&&errno==EINTR)continue;require(ready>0&&!(fd.revents&(POLLERR|POLLNVAL)),107);
        auto count=::read(STDIN_FILENO,static_cast<unsigned char*>(output)+used,bytes-used);
        if(count<0&&(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK))continue;
        require(count>0&&static_cast<std::size_t>(count)<=bytes-used,107);used+=static_cast<std::size_t>(count);
    }
}
std::uint32_t le32(unsigned char const* p){return std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
void output(std::string_view text){
    require(!text.empty()&&text.size()<=4096,90);pollfd fd{STDOUT_FILENO,POLLOUT,0};int ready;unsigned tries=0;
    auto end=Clock::now()+std::chrono::seconds{2};
    do{require(++tries<=128&&Clock::now()<end,110);ready=::poll(&fd,1,10);}while((ready<0&&errno==EINTR)||ready==0);
    require(ready>0&&(fd.revents&POLLOUT)&&!(fd.revents&(POLLERR|POLLHUP|POLLNVAL)),107);require(io::write_all(STDOUT_FILENO,text),107);
}
template<class... T> void line(T const&... value){
    io::fixed_text<4096> text;
    auto one=[&](auto const& item){using V=std::decay_t<decltype(item)>;if constexpr(std::is_integral_v<V>)require(text.number(item),90);else require(text.append(std::string_view{item}),90);};
    (one(value),...);output(text.view());
}
struct Payload {std::array<std::uint8_t,1536> bytes{};std::uint32_t size{};};
template<class Message> Payload payload(Message const& value){
    Payload result;result.size=ros::serialization::serializationLength(value);require(result.size>0&&result.size<=result.bytes.size());
    ros::serialization::OStream stream{result.bytes.data(),result.size};ros::serialization::serialize(stream,value);require(stream.getLength()==0);return result;
}
bool same(Payload const& a,Payload const& b){return a.size==b.size&&!std::memcmp(a.bytes.data(),b.bytes.data(),a.size);}
void observation(char const* role,Payload const& value,std::uint32_t crc_field){
    std::uint64_t digest=UINT64_C(14695981039346656037);
    io::fixed_text<4096> text;require(text.append(role)&&text.append(" ")&&text.number(value.size)&&text.append(" ")&&text.number(crc_field)&&text.append(" "),90);
    constexpr char hex[]{"0123456789abcdef"};
    for(std::uint32_t i=0;i<value.size;++i){digest^=value.bytes[i];digest*=UINT64_C(1099511628211);char pair[]{hex[value.bytes[i]>>4],hex[value.bytes[i]&15]};require(text.append({pair,2}),90);}
    require(text.append(" ")&&text.number(digest)&&text.append("\n"),90);output(text.view());
}
// Keep this bounded polling delay distinct from the POSIX pause() function.
void poll_pause(){int r;do{r=::poll(nullptr,0,2);}while(r<0&&errno==EINTR);require(r==0,107);}
}

int main(int argc,char**){
    using namespace legacy_native;unsigned code=0;
    try{
        require(argc==1);environment();line("LEGACY_NATIVE_GATE ",::getpid(),"\n");
        unsigned char setup[16];input(setup,sizeof setup);
        require(!std::memcmp(setup,"ULN1",4)&&le32(setup+4)<=1&&le32(setup+8)==42&&le32(setup+12)==0);
        bool positive=le32(setup+4)!=0;
        ros::M_string remappings{{"__ns","/uwvm_unitree_legacy"}};ros::init(remappings,"native_peer",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);
        {
            ros::CallbackQueue queue;ros::NodeHandle node{"/uwvm_unitree_legacy"};node.setCallbackQueue(&queue);
            auto state=uwvm_legacy_ros1_fixture::state();auto command=uwvm_legacy_ros1_fixture::command();
            auto state_expected=payload(state);auto command_expected=payload(command);
            unsigned states=0,commands=0;bool allow_state=false,allow_command=false,callback_fault=false;
            auto publisher=node.advertise<unitree_legged_msgs::LowState>("lowstate",1,false);
            // Explicit callback signatures and queue avoid overload deduction and
            // global spinning. Never throw through roscpp's callback trampoline:
            // record a sticky failure, then fail in the owning pump immediately.
            auto header_matches=[](auto const& packet,char const* caller,auto const& message){
                using Message=std::decay_t<decltype(message)>;
                auto const headers=packet.getConnectionHeaderPtr();
                return headers&&packet.getPublisherName()==caller&&headers->count("callerid")&&
                    headers->at("callerid")==caller&&headers->count("md5sum")&&
                    headers->at("md5sum")==ros::message_traits::MD5Sum<Message>::value()&&
                    headers->count("type")&&headers->at("type")==ros::message_traits::DataType<Message>::value();
            };
            ros::SubscribeOptions state_options;
            state_options.initByFullCallbackType<ros::MessageEvent<unitree_legged_msgs::LowState const> const&>("lowstate",1,
                [&](ros::MessageEvent<unitree_legged_msgs::LowState const> const& packet){
                    auto const value=packet.getMessage();
                    if(!value||!allow_state||states!=0||!header_matches(packet,"/uwvm_unitree_legacy/native_peer",state))
                    {callback_fault=true;return;}
                    try{if(!same(payload(*value),state_expected)||value->crc!=uwvm_legacy_ros1_fixture::state_crc_field)
                        {callback_fault=true;return;}}
                    catch(...){callback_fault=true;return;}
                    ++states;
                });
            state_options.callback_queue=&queue;state_options.transport_hints=ros::TransportHints{}.tcpNoDelay().reliable();
            auto self=node.subscribe(state_options);
            ros::SubscribeOptions command_options;
            command_options.initByFullCallbackType<ros::MessageEvent<unitree_legged_msgs::LowCmd const> const&>("lowcmd",1,
                [&](ros::MessageEvent<unitree_legged_msgs::LowCmd const> const& packet){
                    auto const value=packet.getMessage();
                    if(!value||!allow_command||commands!=0||!header_matches(packet,"/uwvm_unitree_legacy/legacy_gateway",command))
                    {callback_fault=true;return;}
                    try{if(!same(payload(*value),command_expected)||value->crc!=uwvm_legacy_ros1_fixture::command_crc_field)
                        {callback_fault=true;return;}}
                    catch(...){callback_fault=true;return;}
                    ++commands;
                });
            command_options.callback_queue=&queue;command_options.transport_hints=ros::TransportHints{}.tcpNoDelay().reliable();
            auto subscriber=node.subscribe(command_options);
            require(publisher&&self&&subscriber);
            auto pump=[&]{queue.callAvailable(ros::WallDuration{0});require(!callback_fault&&ros::ok());};
            auto until=[&](auto condition,unsigned seconds){auto end=Clock::now()+std::chrono::seconds{seconds};unsigned attempts=0;while(!condition()){require(++attempts<=8192&&Clock::now()<end,110);pump();poll_pause();}};
            // A publisher alias does not advertise during cap.open: the actual
            // ROS1 Gateway advertises on first publish. M therefore requires
            // only the Guest state subscription, with command publishers0.
            auto matched=[&](bool guest,bool command_live=false){return publisher.getNumSubscribers()==(guest?2u:1u)&&self.getNumPublishers()==1u&&subscriber.getNumPublishers()==(command_live?1u:0u);};
            // Actual generated traits are observations, never substituted
            // constants. Root must compare all11 to the real Noetic exporter.
            uwvm_legacy_ros1_contract::identities([](char const* type,char const* md5){
                require(type&&md5&&std::strlen(type)<=128&&std::strlen(md5)==32);
                for(unsigned i=0;i<32;++i)require((md5[i]>='0'&&md5[i]<='9')||(md5[i]>='a'&&md5[i]<='f'));
                line("LEGACY_NATIVE_TYPE ",type," ",md5,"\n");
            });
            line("READY_NATIVE_LEGACY_V1 ",unsigned(positive),"\n");
            bool did_match=false,did_state=false,did_command=false,did_quiet=false;
            for(unsigned controls=0;controls<6;++controls){
                char token;input(&token,1);
                if(token=='M'){
                    require(!did_match);until([&]{return matched(positive);},5);did_match=true;
                    line("LEGACY_MATCHED ",unsigned(positive)," ",publisher.getNumSubscribers()," ",subscriber.getNumPublishers(),"\n");
                }else if(token=='P'){
                    require(positive&&did_match&&!did_state&&!did_quiet);allow_state=true;publisher.publish(state);until([&]{return states==1;},5);did_state=true;
                    observation("LEGACY_STATE",state_expected,state.crc);
                }else if(token=='A'){
                    require(positive&&did_match&&did_state&&!did_command&&!did_quiet);allow_command=true;line("LEGACY_ARMED_COMMAND\n");until([&]{return commands==1&&matched(true,true);},5);did_command=true;
                    observation("LEGACY_COMMAND",command_expected,command.crc);
                }else if(token=='Q'||token=='Z'){
                    require(did_match&&!did_quiet&&((token=='Z'&&positive&&did_state&&did_command)||(token=='Q'&&!positive&&!did_state&&!did_command)));
                    until([&]{return matched(false);},5);auto start=Clock::now();unsigned samples=0;
                    do{pump();require(matched(false)&&states==unsigned(positive)&&commands==unsigned(positive));++samples;poll_pause();require(samples<=4096,110);}while(Clock::now()-start<std::chrono::seconds{2}||samples<30);
                    did_quiet=true;line("LEGACY_QUIET ",unsigned(positive)," ",samples," ",std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-start).count()," ",states," ",commands,"\n");
                }else{
                    require(token=='F'&&did_quiet&&states==unsigned(positive)&&commands==unsigned(positive));
                    subscriber.shutdown();self.shutdown();publisher.shutdown();node.shutdown();line("PASS_NATIVE_LEGACY_WITNESS ",unsigned(positive),"\n");break;
                }
                require(controls!=5,110);
            }
        }
        ros::shutdown();ros::waitForShutdown();
    }catch(Failure const& failure){code=failure.code;}catch(...){code=1;}
    if(ros::isInitialized()&&ros::ok())ros::shutdown();return static_cast<int>(code);
}
