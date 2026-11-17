/** @file
 * @brief External native MuJoCo fault fixture for the actual ROSv2 device carrier.
 * @details This is test instrumentation, never a VM provider or a vendor driver.
 * The runner seals the executable/library/model closure, enters fresh namespaces
 * with the production startup filter, and passes only directional descriptors.
 * The production final filter precedes all command bytes. One thread owns
 * mjData, the receiver fence and each physical step. The selected scalar is a
 * bounded abdomen_z target angle; a fixed PD law drives the unmodified official
 * humanoid model. This is neither balance control nor a physical safety stop.
 *
 * Controller, command and stop frames use the production parser. Fault modes
 * deliberately crash, hang, withhold or corrupt a receipt AFTER one actual
 * physics step. A separate sensor pipe publishes that step's position. Stop
 * clears actuator inputs but never claims that the simulated body is stationary.
 * Watchdog service has the same limitation as an ordinary native driver: it
 * cannot run during a whole-process hang. A deployment needs an independent
 * machine watchdog for that failure, not a fabricated stopped acknowledgement.
 */
#include <uwvm2/imported/ros/scene/device_protocol.h>
#include "support/legacy_process/model_sandbox.h"
#include <mujoco/mujoco.h>
#include <cmath>
#include <cstdio>
#include <sys/socket.h>
#include <sys/ptrace.h>

namespace s=uwvm2::imported::ros::scene;
namespace d=s::devices;
namespace p=d::protocol;
using d::error;
static void require(bool ok,char const* what)
{if(!ok){std::fprintf(stderr,"fixture failure: %s errno=%d\n",what,errno);::_exit(101);}}
static std::uint64_t now()
{timespec t{};require(::clock_gettime(CLOCK_MONOTONIC,&t)==0,"clock");return std::uint64_t(t.tv_sec)*1000000000+t.tv_nsec;}
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile out;require(out.identity.assign(name),"profile");out.sha256=s::buffers::hash({reinterpret_cast<std::uint8_t const*>(name.data()),name.size()});return out;}
static void send(int fd,s::wire::bytes bytes)
{
    auto until=now()+1000000000;
    while(!bytes.empty())
    {
        auto n=::write(fd,bytes.data(),bytes.size());
        if(n<0){require((errno==EINTR||errno==EAGAIN)&&now()<until,"bounded write");timespec t{0,100000};::nanosleep(&t,nullptr);continue;}
        require(n>0,"write");bytes=bytes.subspan(static_cast<std::size_t>(n));
    }
}
struct ingress
{
    p::packet packet;std::size_t wanted{p::header_bytes};p::kind kind{};
    bool take(int fd)
    {
        auto n=::read(fd,packet.data.data()+packet.size,wanted-packet.size);
        if(n<0){require(errno==EAGAIN||errno==EINTR,"read");return false;}
        require(n>0,"peer lost");packet.size+=static_cast<std::size_t>(n);
        if(packet.size==p::header_bytes&&wanted==p::header_bytes)
        {
            s::wire::reader fields{packet.view()};std::uint64_t magic{},tag{};
            require(fields.u64(magic)&&fields.u64(tag)&&(tag==1||tag==2||tag==4),"kind");
            kind=static_cast<p::kind>(tag);p::header header;
            require(p::decode_header(packet.view(),kind,header)==error::success,"header");
            wanted=p::header_bytes+header.body_bytes;
        }
        return packet.size==wanted;
    }
    void clear(){packet.size=0;wanted=p::header_bytes;}
};
static void sensor(int fd,double position,std::uint64_t sequence)
{
    constexpr std::string_view schema="uwvm.ros.sensor.octets:nonempty opaque bytes; maximum=4194304; no implicit ROS message or physical schema";
    auto hash=s::buffers::hash({reinterpret_cast<std::uint8_t const*>(schema.data()),schema.size()});
    std::array<std::uint8_t,156> frame{};std::array<std::uint8_t,4> payload{};
    auto bits=std::bit_cast<std::uint32_t>(static_cast<float>(position));
    for(unsigned i{};i<4;++i)payload[i]=static_cast<std::uint8_t>(bits>>(i*8));
    s::wire::writer out{frame};for(auto word:{UINT64_C(0x524f534e45535255),UINT64_C(4),UINT64_C(101),UINT64_C(1),sequence,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0)})out.u64(word);
    require(out.blob(hash),"sensor schema");auto digest=p::content_hash(out.written(),payload);
    require(out.blob(digest)&&out.encoded(payload),"sensor frame");send(fd,out.written());
}
int main(int argc,char** argv)
{
    if(argc==2&&std::string_view(argv[1])=="--filter")
    {s::model_sandbox::syscall_filter filter;auto bytes=std::as_bytes(filter.view());send(1,{reinterpret_cast<std::uint8_t const*>(bytes.data()),bytes.size()});return 0;}
    require(argc==9,"arguments");
    std::string_view mode{argv[1]};std::array<int,5> fd{};
    for(unsigned i{};i<5;++i){std::string_view text{argv[i+2]};auto n=std::from_chars(text.data(),text.data()+text.size(),fd[i]);require(n.ec==std::errc{}&&n.ptr==text.data()+text.size()&&fd[i]>=0,"descriptor");}
    std::uint64_t generation{};std::string_view text{argv[7]};auto n=std::from_chars(text.data(),text.data()+text.size(),generation);require(n.ec==std::errc{}&&n.ptr==text.data()+text.size()&&generation,"generation");
    require(std::string_view(argv[8])=="official-humanoid","model selector");
    // Parse only the sealed, bounded model bytes. MuJoCo's generic filesystem
    // resolver is unnecessary and is not a reason to widen the syscall policy.
    std::array<char,65536> xml{};auto model_fd=::open("/fixture/model.xml",O_RDONLY|O_CLOEXEC);require(model_fd>=0,"model open");
    std::size_t extent{};
    for(;;){auto got=::read(model_fd,xml.data()+extent,xml.size()-extent-1);require(got>=0,"model read");if(!got)break;extent+=got;require(extent<xml.size()-1,"model bound");}
    ::close(model_fd);mjVFS files;::mj_defaultVFS(&files);require(::mj_addBufferVFS(&files,"model.xml",xml.data(),extent)==0,"model VFS");
    std::array<char,1024> diagnostic{};auto* model=::mj_loadXML("model.xml",&files,diagnostic.data(),diagnostic.size());::mj_deleteVFS(&files);
    require(model!=nullptr,diagnostic.data());auto* data=::mj_makeData(model);require(data,"data");
    auto actuator=::mj_name2id(model,mjOBJ_ACTUATOR,"abdomen_z"),joint=::mj_name2id(model,mjOBJ_JOINT,"abdomen_z");
    require(actuator>=0&&joint>=0&&model->jnt_type[joint]==mjJNT_HINGE,"fixed mapping");
    auto q=model->jnt_qposadr[joint],v=model->jnt_dofadr[joint];::mj_forward(model,data);
    auto* crash_page=::mmap(nullptr,4096,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);require(crash_page!=MAP_FAILED,"crash page");
    rlimit file{0,0};require(!::prlimit(0,RLIMIT_FSIZE,&file,nullptr),"file limit");
    s::model_sandbox::syscall_filter filter{true};require(filter.install(),"final filter");
    // Real attempted escapes, after confinement and before application traffic.
    require(::socket(AF_INET,SOCK_STREAM,0)==-1&&errno==ENOSYS,"socket denied");
    require(::ptrace(PTRACE_TRACEME,0,nullptr,nullptr)==-1&&errno==ENOSYS,"ptrace denied");
    require(::syscall(SYS_clone,0,nullptr,nullptr,nullptr,0)==-1&&errno==ENOSYS,"clone denied");
    require(::open("/escape",O_WRONLY|O_CREAT,0600)==-1&&errno==ENOSYS,"file create denied");
    require(::open("/etc/passwd",O_RDONLY)==-1&&errno==ENOENT,"host filesystem absent");
    require(::mmap(nullptr,4096,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0)==MAP_FAILED&&errno==ENOSYS,"executable memory denied");
    require(::prlimit(0,RLIMIT_AS,&file,nullptr)==-1&&errno==ENOSYS,"resource changes denied");
    auto execute=[&](double angle,std::uint64_t step,std::uint64_t sampled=0)
    {
        auto begin=now();data->ctrl[actuator]=std::clamp(2.0*(angle-data->qpos[q])-.1*data->qvel[v],-1.0,1.0);
        ::mj_step(model,data);auto elapsed=now()-begin;require(std::isfinite(data->qpos[q])&&std::isfinite(data->time),"finite physics");
        std::fprintf(stderr,"{\"kind\":\"effect\",\"step\":%llu,\"time\":%.17g,\"position\":%.17g,\"velocity\":%.17g,\"step_ns\":%llu,\"sample_to_effect_ns\":%llu}\n",
            static_cast<unsigned long long>(step),data->time,data->qpos[q],data->qvel[v],static_cast<unsigned long long>(elapsed),
            static_cast<unsigned long long>(sampled?begin-sampled:0));std::fflush(stderr);
    };
    std::fprintf(stderr,"{\"kind\":\"ready\",\"mujoco\":%d,\"actuators\":%d,\"escape_checks\":7}\n",::mj_version(),model->nu);std::fflush(stderr);
    if(mode=="direct"||mode=="direct-cycle")
    {for(unsigned i{1};i<=(mode=="direct"?1:128);++i)execute(i%2?.5:-.5,i);::mj_deleteData(data);::mj_deleteModel(model);return 0;}
    d::scalar_schema schema;schema.profile=profile("drive.schema");schema.frame.assign("logical");schema.count=1;
    schema.fields[0].name.assign("joint");schema.fields[0].quantity=d::unit::radian;schema.fields[0].minimum=-1;schema.fields[0].maximum=1;
    p::receiver receiver;require(receiver.configure(generation,schema,profile("drive.stop"))==error::success&&receiver.rearm_after_stopped(generation,1)==error::success,"receiver");
    ingress commands,stops;std::uint64_t effects{};auto until=now()+15000000000;
    auto stop=[&](char const* cause){::mju_zero(data->ctrl,model->nu);std::fprintf(stderr,"{\"kind\":\"stop\",\"cause\":\"%s\",\"effects\":%llu,\"stationary_claim\":false}\n",cause,static_cast<unsigned long long>(effects));std::fflush(stderr);};
    while(now()<until)
    {
        auto time=now();if(receiver.expire(time))stop("watchdog");
        // Priority is separate from command framing; no shared queue.
        if(stops.take(fd[2]))
        {
            p::header header;require(receiver.stop(stops.packet.view(),header)==error::success,"stop");
            stop("host");p::packet ack;require(p::acknowledge(header,p::acknowledgement::stop_accepted,header.sequence,ack)==error::success,"stop ACK");
            send(fd[3],ack.view());stops.clear();
        }
        if(commands.take(fd[0]))
        {
            p::header header;p::packet ack;
            if(commands.kind==p::kind::controller)
            {
                require(receiver.controller(commands.packet.view(),time,header)==error::success,"controller");
                require(p::acknowledge(header,p::acknowledgement::controller_installed,header.sequence,ack)==error::success,"controller ACK");send(fd[1],ack.view());
            }
            else
            {
                d::scalar_command command;require(receiver.command(commands.packet.view(),time,header,command)==error::success,"command");
                require(++effects<=(mode=="cycle"?128u:1u)&&command.count==1&&command.values[0]==(effects%2?.5:-.5),"exact ordered effects");
                require(header.deadline>=1000000000&&time>=header.deadline-1000000000,"sample clock domain");
                execute(command.values[0],effects,header.deadline-1000000000);sensor(fd[4],data->qpos[q],effects+1);
                if(mode=="crash"){*static_cast<volatile std::uint8_t*>(crash_page)=1;::_exit(102);}
                if(mode=="hang"){for(;;){asm volatile("" ::: "memory");}}
                if(mode!="withhold")
                {
                    if(mode=="corrupt")header.digest[0]^=1;
                    if(mode=="stale")++header.provider;
                    require(p::acknowledge(header,p::acknowledgement::executed,header.sequence,ack)==error::success,"command ACK");send(fd[1],ack.view());
                }
            }
            commands.clear();
        }
        timespec pause{0,100000};::nanosleep(&pause,nullptr);
    }
    ::mj_deleteData(data);::mj_deleteModel(model);return 0;
}
