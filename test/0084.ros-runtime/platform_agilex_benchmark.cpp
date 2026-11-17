/** @file @brief Measured codec and common-framing comparisons; no physical device throughput claim. */
#include <uwvm2/imported/ros/scene/platform_agilex_receiver.h>
#include <fast_io.h>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <string_view>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>
#include "protocol_v2/agilex_msg_parser_v2.h"
namespace s=uwvm2::imported::ros::scene;namespace d=s::devices;namespace p=d::protocol;
namespace a=s::platform_extensions::agilex;using d::error;
static constexpr std::size_t count=256;
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile out;assert(out.identity.assign(name));out.sha256.fill(12);return out;}
static d::scalar_schema schema()
{
    d::scalar_schema out;out.profile=profile("agilex.motion");assert(out.frame.assign("base"));out.count=4;
    for(std::size_t i{};i<4;++i){assert(out.fields[i].name.assign(a::motion_names[i]));out.fields[i].quantity=a::motion_units[i];out.fields[i].minimum=-2;out.fields[i].maximum=2;}
    return out;
}
struct fixture
{
    d::scalar_schema selected=schema();s::buffers::profile stop=profile("fixed.zero");
    s::control::lease lease{9,10,4,{1,11,3,12},1,1100};p::packet controller;
    std::array<p::packet,count> packets;std::array<d::scalar_command,count> values;
    fixture()
    {
        assert(p::controller(lease,selected,stop,1,controller)==error::success);
        for(std::size_t i{};i<count;++i)
        {
            d::work w;w.result={100+i,d::state::running,d::effect::locally_accepted,4,d::reason::none};w.deadline=1050;w.controller=lease;
            w.command.count=4;for(std::size_t j{};j<4;++j)w.command.values[j]=static_cast<double>(static_cast<int>((i+j)%17)-8)/8.;
            values[i]=w.command;assert(p::command(w,selected,i+1,packets[i])==error::success);
        }
    }
};
[[gnu::noinline]] static a::motion_frame upstream(d::scalar_command const& input)
{
    AgxMessage message{};message.type=AgxMsgMotionCommand;
    message.body.motion_command_msg.linear_velocity=input.values[0];message.body.motion_command_msg.angular_velocity=input.values[1];
    message.body.motion_command_msg.lateral_velocity=input.values[2];message.body.motion_command_msg.steering_angle=input.values[3];
    can_frame frame{};assert(EncodeCanFrameV2(&message,&frame));a::motion_frame out{frame.can_id,{}};
    std::copy_n(frame.data,8,out.payload.data());return out;
}
[[gnu::noinline]] static a::motion_frame protected_codec(a::motion_codec const& codec,d::scalar_command const& input)
{a::motion_frame out;assert(codec.encode(input,out)==error::success);return out;}
[[gnu::noinline]] static a::motion_frame native_common(p::receiver& receiver,p::packet const& frame)
{p::header header;d::scalar_command out;assert(receiver.command(frame.view(),1001,header,out)==error::success);return upstream(out);}
[[gnu::noinline]] static a::motion_frame platform_common(a::motion_receiver& receiver,p::packet const& frame)
{p::header header;a::motion_frame out;assert(receiver.command(frame.view(),1001,header,out)==error::success);return out;}
static void transfer(int fd,void* buffer,std::size_t size,bool write)
{
    auto p=static_cast<char*>(buffer);
    while(size){auto n=write ? ::write(fd,p,size) : ::read(fd,p,size);if(n<0&&errno==EINTR)continue;assert(n>0);size-=static_cast<std::size_t>(n);p+=n;}
}
/** @brief Alternate two real round trips through the same process/core/pipe pair.
 * @details Each receiver owns its own equal fencing state. Native/platform
 * order reverses on every sample pair, reducing drift from separate process
 * launches. Timing includes two clock reads. No sample or outlier is removed.
 */
static void paired(fixture const& data,bool isolated,std::size_t batches,std::size_t rounds)
{
    assert(rounds<=256);
    uwvm2::utils::container::std_vector<std::array<std::uint64_t,2>> samples(rounds*count);
    int to_child[2]{},from_child[2]{};pid_t child=-1;
    auto initialize=[&](p::receiver& native,a::motion_receiver& platform)
    {
        p::header header;
        assert(native.configure(4,data.selected,data.stop)==error::success&&native.rearm_after_stopped(4,1)==error::success&&native.controller(data.controller.view(),1001,header)==error::success);
        assert(platform.configure(4,data.selected,data.stop)==error::success&&platform.rearm_after_stopped(4,1)==error::success&&platform.controller(data.controller.view(),1001,header)==error::success);
    };
    if(isolated)
    {
        assert(::pipe(to_child)==0&&::pipe(from_child)==0);child=::fork();assert(child>=0);
        if(!child)
        {
            ::close(to_child[1]);::close(from_child[0]);cpu_set_t affinity;CPU_ZERO(&affinity);CPU_SET(10,&affinity);
            assert(::sched_setaffinity(0,sizeof(affinity),&affinity)==0);
            for(std::size_t batch{};batch<batches;++batch)for(std::size_t round{};round<rounds;++round)
            {
                p::receiver native;a::motion_receiver platform;initialize(native,platform);
                for(std::size_t i{};i<count;++i)for(std::size_t turn{};turn<2;++turn)
                {
                    bool extension=((i+batch+round)%2)^turn;
                    p::packet frame;frame.size=data.packets[i].size;transfer(to_child[0],frame.data.data(),frame.size,false);
                    auto result=extension?platform_common(platform,frame):native_common(native,frame);
                    transfer(from_child[1],&result,sizeof(result),true);
                }
            }
            ::_exit(0);
        }
        ::close(to_child[0]);::close(from_child[1]);
    }
    std::array<std::uint64_t,2> checksums{};
    for(std::size_t batch{};batch<batches;++batch)
    {
        for(std::size_t round{};round<rounds;++round)
        {
            p::receiver native;a::motion_receiver platform;
            if(!isolated)initialize(native,platform);
            for(std::size_t i{};i<count;++i)for(std::size_t turn{};turn<2;++turn)
            {
                bool extension=((i+batch+round)%2)^turn;auto started=std::chrono::steady_clock::now();a::motion_frame result;
                if(isolated)
                {transfer(to_child[1],const_cast<std::uint8_t*>(data.packets[i].data.data()),data.packets[i].size,true);transfer(from_child[0],&result,sizeof(result),false);}
                else result=extension?platform_common(platform,data.packets[i]):native_common(native,data.packets[i]);
                samples[round*count+i][extension]=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-started).count();
                checksums[extension]+=result.identifier;for(auto byte:result.payload)checksums[extension]+=byte;
            }
        }
        assert(checksums[0]==checksums[1]);
        for(std::size_t i{};i<samples.size();++i)
            fast_io::io::println("pair ",batch," ",i," ",samples[i][0]," ",samples[i][1]);
    }
    if(isolated){int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);::close(to_child[1]);::close(from_child[0]);}
}
int main(int argc,char** argv)
{
    assert(argc==4);std::string_view mode=argv[1];auto batches=std::strtoul(argv[2],nullptr,10),rounds=std::strtoul(argv[3],nullptr,10);
    assert(batches&&batches<=10000&&rounds&&rounds<=100000);
    bool latency=mode.ends_with("-latency");if(latency)mode.remove_suffix(8);
    // Reserve outside timed work. Each retained value measures one request;
    // throughput batches remain a separate, uninstrumented experiment.
    uwvm2::utils::container::std_vector<std::uint64_t> samples;
    if(latency){assert(rounds<=256);samples.resize(rounds*count);}
    auto data=uwvm2::utils::container::make_unique<fixture>();a::motion_codec codec;assert(codec.configure(data->selected)==error::success);
    for(std::size_t i{};i<count;++i)assert(upstream(data->values[i]).payload==protected_codec(codec,data->values[i]).payload);
    if(mode=="paired-isolated"||mode=="paired-framed")
    {paired(*data,mode=="paired-isolated",batches,rounds);return 0;}
    bool isolated=mode=="native-isolated"||mode=="platform-isolated";bool platform=mode=="platform-framed"||mode=="platform-isolated";
    assert(isolated||platform||mode=="native-framed"||mode=="native-codec"||mode=="platform-codec");
    int to_child[2]{},from_child[2]{};pid_t child=-1;
    if(isolated)
    {
        assert(::pipe(to_child)==0&&::pipe(from_child)==0);child=::fork();assert(child>=0);
        if(!child)
        {
            ::close(to_child[1]);::close(from_child[0]);cpu_set_t affinity;CPU_ZERO(&affinity);CPU_SET(10,&affinity);assert(::sched_setaffinity(0,sizeof(affinity),&affinity)==0);
            for(std::size_t batch{};batch<batches;++batch)for(std::size_t round{};round<rounds;++round)
            {
                p::receiver native;a::motion_receiver extension;p::header h;
                if(platform){assert(extension.configure(4,data->selected,data->stop)==error::success&&extension.rearm_after_stopped(4,1)==error::success&&extension.controller(data->controller.view(),1001,h)==error::success);}
                else {assert(native.configure(4,data->selected,data->stop)==error::success&&native.rearm_after_stopped(4,1)==error::success&&native.controller(data->controller.view(),1001,h)==error::success);}
                for(std::size_t i{};i<count;++i)
                {
                    p::packet frame;frame.size=data->packets[i].size;transfer(to_child[0],frame.data.data(),frame.size,false);
                    auto result=platform?platform_common(extension,frame):native_common(native,frame);transfer(from_child[1],&result,sizeof(result),true);
                }
            }
            ::_exit(0);
        }
        ::close(to_child[0]);::close(from_child[1]);
    }
    std::uint64_t checksum{};
    for(std::size_t batch{};batch<batches;++batch)
    {
        auto start=std::chrono::steady_clock::now();
        for(std::size_t round{};round<rounds;++round)
        {
            p::receiver native;a::motion_receiver extension;p::header h;
            if(!isolated)
            {
                if(mode=="platform-framed")assert(extension.configure(4,data->selected,data->stop)==error::success&&extension.rearm_after_stopped(4,1)==error::success&&extension.controller(data->controller.view(),1001,h)==error::success);
                if(mode=="native-framed")assert(native.configure(4,data->selected,data->stop)==error::success&&native.rearm_after_stopped(4,1)==error::success&&native.controller(data->controller.view(),1001,h)==error::success);
            }
            for(std::size_t i{};i<count;++i)
            {
                auto entered=latency?std::chrono::steady_clock::now():std::chrono::steady_clock::time_point{};
                a::motion_frame result;
                if(isolated){transfer(to_child[1],data->packets[i].data.data(),data->packets[i].size,true);transfer(from_child[0],&result,sizeof(result),false);}
                else if(mode=="native-codec")result=upstream(data->values[i]);
                else if(mode=="platform-codec")result=protected_codec(codec,data->values[i]);
                else if(platform)result=platform_common(extension,data->packets[i]);
                else result=native_common(native,data->packets[i]);
                if(latency)samples[round*count+i]=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-entered).count();
                checksum+=result.identifier;for(auto byte:result.payload)checksum+=byte;
            }
        }
        auto elapsed=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count();
        fast_io::io::println(mode," ",elapsed," ",rounds*count," ",checksum);
        if(latency)for(std::size_t i{};i<samples.size();++i)fast_io::io::println("sample ",batch," ",i," ",samples[i]);
    }
    if(isolated){int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);::close(to_child[1]);::close(from_child[0]);}
}
