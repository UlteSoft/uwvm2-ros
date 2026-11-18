/** @file
 * @brief Test deployment gateway; process ownership is deliberately outside the VM provider.
 * @details This fixture owns an original ROS sandbox and forwards its bounded
 * RPC through preprovisioned pipes. It is not a production deployer. The VM-side
 * factory gets only an authenticated gateway identity and directional endpoints.
 */
#pragma once
#include <uwvm2/imported/ros/scene/communication_attachment.h>
#include "support/legacy_process/communication_worker_sandbox.h"
#include <cassert>
#include <sys/wait.h>
#include <sys/prctl.h>
namespace external_fixture
{
    namespace s=uwvm2::imported::ros::scene;
    namespace resources=uwvm2::imported::ros::resources;
    namespace x=s::communication_sandbox;
    namespace a=s::communication_attachment;
    namespace w=s::communication_worker;
    inline volatile sig_atomic_t stopping{};
    inline void stop(int){stopping=1;}
    inline uwvm2::utils::container::std_vector<int> parent_endpoints;
    inline void gateway(int control,std::array<int,7> const& pipes,pid_t parent,x::manifest const& manifest,int startup,pid_t provisioner=0)
    {
        ::signal(SIGTERM,stop);::signal(SIGINT,stop);::signal(SIGPIPE,SIG_IGN);
        if(!provisioner)provisioner=parent;
        assert(!::prctl(PR_SET_PDEATHSIG,SIGTERM));if(::getppid()!=provisioner)stopping=1;
        x::process child;assert(child.prepare(manifest)==s::error::success&&child.launch()==s::error::success);
        auto until=resources::steady_now()+15000000000ll;
        auto wait=[&](bool identify,bool attached=false)
        {
            s::error status;
            do{status=identify?child.identify():child.readiness(attached);if(status==s::error::again)::poll(nullptr,0,1);}
            while(status==s::error::again&&resources::steady_now()<until&&!stopping);
            return status;
        };
        assert(wait(true)==s::error::success&&wait(false)==s::error::success);
        char done='R';assert(::write(startup,&done,1)==1);::close(startup);
        s::drivers::channel channel;assert(channel.bind(control,{static_cast<std::uint64_t>(parent),::getuid(),::getgid()})==s::error::success);::close(control);
        std::array<std::uint8_t,1200> packet{};std::array<std::uint8_t,1200> identity{};std::size_t identity_size{};
        auto receive=[&](unsigned expected)
        {
            std::size_t count{};s::error status;
            do{status=channel.receive(packet,count);if(status==s::error::again)::poll(nullptr,0,1);}while(status==s::error::again&&!stopping);
            if(status!=s::error::success||stopping)return false;
            s::wire::reader header{std::span{packet}.first(count)};std::uint64_t magic{},size{},kind{},flags{};
            if(!header.u64(magic)||magic!=a::magic||!header.u64(size)||size!=176||!header.u64(kind)||kind!=expected||!header.u64(flags)||flags)return false;
            if(expected==a::hello)
            {
                if(count<=176||count>identity.size())return false;
                s::wire::reader body{std::span{packet}.subspan(176,count-176)};std::string_view name;s::wire::bytes digest;
                if(!body.text(name)||name!=manifest.configuration.identity.view()||!body.blob(digest)||!std::ranges::equal(digest,manifest.configuration.sha256))return false;
                for(std::size_t i{};i<7;++i)
                {
                    std::uint64_t role{},device{},inode{};bool vm_writes{};struct stat info{};
                    if(!body.u64(role)||role!=i||!body.u64(device)||!body.u64(inode)||!body.boolean(vm_writes)||
                       vm_writes!=(i==0||i==3||i==5)||::fstat(pipes[i],&info)||device!=static_cast<std::uint64_t>(info.st_dev)||inode!=static_cast<std::uint64_t>(info.st_ino))return false;
                }
                std::uint64_t diagnostics{};if(!body.u64(diagnostics)||diagnostics!=65536||!body.done())return false;
                std::copy_n(packet.begin(),count,identity.begin());identity_size=count;
            }
            else
            {
                if(count!=identity_size)return false;
                for(std::size_t i{};i<count;++i)if(i<16||i>=24)if(packet[i]!=identity[i])return false;
            }
            return true;
        };
        auto reply=[&](unsigned kind)
        {
            s::wire::writer patch{std::span{packet}.subspan(16,8)};assert(patch.u64(kind));
            s::error status;do{status=channel.send(std::span{packet}.first(176),std::span{packet}.subspan(176,identity_size-176));if(status==s::error::again)::poll(nullptr,0,1);}while(status==s::error::again&&!stopping);
            return status==s::error::success;
        };
        bool active=receive(a::hello)&&reply(a::ready)&&receive(a::activate);
        if(active)
        {
            std::array<w::role_fd,4> endpoints;for(std::size_t i{};i<4;++i)endpoints[i]={w::roles[i],pipes[i+3]};
            auto status=child.activate(endpoints);assert(status==s::error::success&&wait(false,true)==s::error::success);
            active=reply(a::attached);
        }
        struct relay{int from,to;std::array<std::uint8_t,65536> bytes{};std::size_t size{},offset{};};
        std::array<relay,3> routes{{{pipes[0],child.input_fd()},{child.output_fd(),pipes[1]},{child.stderr_fd(),pipes[2]}}};
        while(!stopping&&active)
        {
            // Session closure must not be treated as permission to kill a peer.
            // The explicit fixture owner sends SIGTERM only after VM-side tests.
            std::size_t size{};auto status=channel.receive(packet,size);
            if(status==s::error::success){if(!size)active=false;else {s::wire::reader in{std::span{packet}.first(size)};std::uint64_t value{};in.u64(value);in.u64(value);in.u64(value);if(value==a::release)active=false;}}
            else if(status!=s::error::again)active=false;
            for(auto& route:routes)
            {
                if(!route.size)
                {auto count=::read(route.from,route.bytes.data(),route.bytes.size());if(count>0){route.size=static_cast<std::size_t>(count);route.offset=0;}else if(!count)active=false;else if(errno!=EINTR&&errno!=EAGAIN)active=false;}
                if(route.size)
                {auto count=::write(route.to,route.bytes.data()+route.offset,route.size-route.offset);if(count>0){route.offset+=static_cast<std::size_t>(count);if(route.offset==route.size)route.size=0;}else if(count<0&&errno!=EINTR&&errno!=EAGAIN)active=false;}
            }
            ::poll(nullptr,0,1);
        }
        while(!stopping)::poll(nullptr,0,1);
        child.request_stop();int status{};until=resources::steady_now()+10000000000ll;
        while(child.reap(status)==s::error::again){assert(resources::steady_now()<until);::poll(nullptr,0,1);}
        assert(child.cleanup_confirmed());::_exit(0);
    }
    struct owner
    {
        pid_t pid{};std::array<int,8> owned{};a::configuration attachment;
        explicit owner(x::manifest const& manifest)
        {
            owned.fill(-1);int control[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,control));
            int one=1;for(auto fd:control)assert(!::setsockopt(fd,SOL_SOCKET,SO_PASSCRED,&one,sizeof(one)));
            owned[0]=control[0];std::array<int,7> remote{};
            for(std::size_t i{};i<7;++i)
            {int fds[2];assert(!::pipe2(fds,O_NONBLOCK|O_CLOEXEC));bool vm_writes=i==0||i==3||i==5;owned[i+1]=fds[vm_writes?1:0];remote[i]=fds[vm_writes?0:1];}
            int startup[2];assert(!::pipe2(startup,O_CLOEXEC));auto parent=::getpid();pid=::fork();assert(pid>=0);
            if(!pid)
            {::close(startup[0]);for(auto fd:parent_endpoints)::close(fd);for(auto fd:owned)::close(fd);gateway(control[1],remote,parent,manifest,startup[1]);::_exit(99);}
            ::close(startup[1]);::close(control[1]);for(auto fd:remote)::close(fd);
            pollfd wait{startup[0],POLLIN,0};assert(::poll(&wait,1,16000)==1);char marker{};assert(::read(startup[0],&marker,1)==1&&marker=='R');::close(startup[0]);
            for(auto fd:owned)parent_endpoints.push_back(fd);
            a::registration setting;setting.control=owned[0];setting.peer={static_cast<std::uint64_t>(pid),::getuid(),::getgid()};
            for(std::size_t i{};i<7;++i)setting.pipes[i]=owned[i+1];
            setting.identity={{1,2},{3,4},{5,static_cast<std::uint64_t>(pid)},{7,8},{9,static_cast<std::uint64_t>(pid)},{11,12,13,14}};
            setting.startup_ns=10000000000ull;setting.session_until=static_cast<std::uint64_t>(resources::steady_now())+120000000000ull;setting.stderr_bytes=65536;
            assert(attachment.profile(manifest.configuration,manifest.ros_prefix.view())==s::error::success&&attachment.channel(setting)==s::error::success&&attachment.freeze()==s::error::success);
        }
        ~owner()
        {
            assert(::kill(pid,0)==0);assert(!::kill(pid,SIGTERM));int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
            for(auto fd:owned){::close(fd);auto at=std::find(parent_endpoints.begin(),parent_endpoints.end(),fd);assert(at!=parent_endpoints.end());parent_endpoints.erase(at);}
        }
    };
}
