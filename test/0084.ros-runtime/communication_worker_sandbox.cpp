/** @file @brief Real sealed communication startup, exact four-pipe activation and pidfd cleanup. */
#include "support/legacy_process/communication_worker_sandbox.h"
#include <cassert>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
namespace x=s::communication_sandbox;
namespace w=s::communication_worker;
using x::error;
static x::image_spec image(int fd,std::string_view destination,x::image_role role)
{
    assert(fd>=0);struct stat info{};assert(!::fstat(fd,&info)&&info.st_size>=0);
    fast_io::sha256_context hashing{};std::array<std::byte,65536> bytes{};std::uint64_t offset{};
    while(offset<static_cast<std::uint64_t>(info.st_size)){auto n=::pread(fd,bytes.data(),bytes.size(),offset);assert(n>0);hashing.update(bytes.data(),bytes.data()+n);offset+=n;}
    x::image_spec value;value.source=fd;value.bytes=offset;value.role=role;assert(value.destination.assign(destination));hashing.do_final();hashing.digest_to_byte_ptr(reinterpret_cast<std::byte*>(value.sha256.data()));return value;
}
static x::manifest make_manifest(char** paths,uwvm2::utils::container::std_vector<x::descriptor>& ownership,unsigned mode=0)
{
    x::manifest result;
    auto add=[&](char const* path,std::string_view destination,x::image_role role)
    {ownership.emplace_back(::open(path,O_RDONLY|O_CLOEXEC));return image(ownership.back().get(),destination,role);};
    result.launcher=add(paths[7],"/worker/launcher",x::image_role::runtime);
    result.files.push_back(add(paths[0],"/worker/bootstrap",x::image_role::bootstrap));result.files.push_back(add(paths[1],"/worker/backend.so",x::image_role::runtime));
    constexpr std::array names{"/lib64/ld-linux-x86-64.so.2","/lib/x86_64-linux-gnu/libc.so.6","/lib/x86_64-linux-gnu/libstdc++.so.6","/lib/x86_64-linux-gnu/libgcc_s.so.1","/lib/x86_64-linux-gnu/libm.so.6"};
    for(std::size_t i{};i<names.size();++i)result.files.push_back(add(paths[i+2],names[i],x::image_role::runtime));
    ownership.emplace_back(static_cast<int>(::syscall(SYS_memfd_create,"communication-fixture-profile",MFD_CLOEXEC|MFD_ALLOW_SEALING)));
    std::array<std::uint8_t,8> profile{};profile[0]=static_cast<std::uint8_t>(mode);assert(::write(ownership.back().get(),profile.data(),profile.size())==8);
    auto config=image(ownership.back().get(),"/worker/profile",x::image_role::configuration);result.configuration.sha256=config.sha256;result.files.push_back(config);
    assert(result.configuration.identity.assign("fixture-one-approved-node")&&result.ros_prefix.assign("/opt/ros/jazzy"));
    result.bound={128u*1024u*1024u,512u*1024u*1024u,5,4096,128,65536,0,4096,4096,4096,5000000000ull,1000000,1024u*1024u,8,3};
    assert(x::valid(result));return result;
}
struct pipe_set
{
    std::array<x::descriptor,4> worker,broker;std::array<w::role_fd,4> roles{};
    pipe_set()
    {for(std::size_t i{};i<4;++i){int fds[2];assert(!::pipe2(fds,O_CLOEXEC|O_NONBLOCK));auto rx=w::receiving(w::roles[i]);worker[i].reset(fds[rx?0:1]);broker[i].reset(fds[rx?1:0]);roles[i]={w::roles[i],worker[i].get()};}}
};
[[maybe_unused]] static void diagnostics(x::process& child)
{std::array<char,4096> buffer{};auto n=::read(child.stderr_fd(),buffer.data(),buffer.size());if(n>0)assert(::write(2,buffer.data(),static_cast<std::size_t>(n))==n);}
[[maybe_unused]] static error startup(x::process& child,bool attached=false)
{
    auto until=uwvm2::imported::ros::resources::steady_now()+6000000000ll;auto status=error::again;
    while(status==error::again&&uwvm2::imported::ros::resources::steady_now()<until)
    {status=attached?child.readiness(true):child.identify();if(status==error::success&&!attached)status=child.readiness();if(status==error::again)::poll(nullptr,0,1);}
    if(status!=error::success)diagnostics(child);
    return status;
}
[[maybe_unused]] static int reaped(x::process& child,bool kill=true)
{
    if(kill)child.request_stop();
    int status{};auto until=uwvm2::imported::ros::resources::steady_now()+3000000000ll;
    for(;;){auto result=child.reap(status);if(result==error::success)return status;assert(result==error::again&&uwvm2::imported::ros::resources::steady_now()<until);::poll(nullptr,0,1);}
}
[[maybe_unused]] static void transfer(x::process& child,int fd,std::span<std::uint8_t> bytes,bool writing)
{
    auto until=uwvm2::imported::ros::resources::steady_now()+3000000000ll;std::size_t done{};
    while(done<bytes.size())
    {auto n=writing?::write(fd,bytes.data()+done,bytes.size()-done) : ::read(fd,bytes.data()+done,bytes.size()-done);if(n>0){done+=static_cast<std::size_t>(n);continue;}
     if(n==0||!(errno==EAGAIN||errno==EINTR)){diagnostics(child);std::fprintf(stderr,"transfer failed n=%zd errno=%d\n",n,errno);assert(false);}
     assert(uwvm2::imported::ros::resources::steady_now()<until);pollfd wait{fd,static_cast<short>(writing?POLLOUT:POLLIN),0};::poll(&wait,1,1);}
}
#ifndef UWVM_COMMUNICATION_SANDBOX_NO_MAIN
int main(int argc,char** argv)
{
    assert(argc==9);::signal(SIGPIPE,SIG_IGN);uwvm2::utils::container::std_vector<x::descriptor> ownership;auto manifest=make_manifest(argv+1,ownership);
    // Pure manifest rejection precedes sealing/spawning; reserves include both image copies.
    auto invalid=manifest;invalid.bound.thread_count=0;assert(!x::valid(invalid));invalid=manifest;invalid.bound.scratch_slots=1;assert(!x::valid(invalid));
    invalid=manifest;invalid.files.back().sha256[0]^=1;assert(!x::valid(invalid));invalid=manifest;invalid.files.push_back(invalid.files.back());assert(!x::valid(invalid));
    invalid=manifest;invalid.bound.image_bytes=manifest.launcher.bytes+1;assert(!x::valid(invalid));invalid=manifest;invalid.configuration.identity.bytes[0]=char(0xff);assert(!x::valid(invalid));
    // The launcher and every image are rehashed and sealed before a child exists.
    invalid=manifest;invalid.launcher.sha256[0]^=1;x::process broken;assert(broken.prepare(invalid)==error::conflict);
    pipe_set pipes;x::process child;assert(!child.namespace_identity_confirmed());assert(child.activate(pipes.roles)==error::conflict);assert(child.prepare(manifest)==error::success);assert(child.launch()==error::success);
    assert(child.activate(pipes.roles)==error::conflict);auto ready=startup(child);std::fprintf(stderr,"ready=%u\n",unsigned(ready));assert(ready==error::success&&child.namespace_identity_confirmed());
    pollfd waiting{child.output_fd(),POLLIN,0};assert(::poll(&waiting,1,20)==0); // No attach or application output while authority is zero.
    auto wrong=pipes.roles;std::swap(wrong[0],wrong[1]);assert(child.activate(wrong)==error::denied);
    wrong=pipes.roles;wrong[0].descriptor=pipes.broker[0].get();assert(child.activate(wrong)==error::denied);
    wrong=pipes.roles;wrong[2].descriptor=pipes.worker[0].get();assert(child.activate(wrong)==error::invalid);
    int blocking[2];assert(!::pipe2(blocking,O_CLOEXEC));x::descriptor b0{blocking[0]},b1{blocking[1]};wrong=pipes.roles;wrong[0].descriptor=b0.get();assert(child.activate(wrong)==error::denied);
    wrong=pipes.roles;wrong[0].descriptor=ownership.front().get();assert(child.activate(wrong)==error::denied);
    assert(child.activate(pipes.roles)==error::success&&child.activate(pipes.roles)==error::conflict&&startup(child,true)==error::success);
    std::array<std::uint8_t,1> first{7},second{13};transfer(child,pipes.broker[0].get(),first,true);transfer(child,pipes.broker[2].get(),second,true);
    transfer(child,pipes.broker[1].get(),first,false);transfer(child,pipes.broker[3].get(),second,false);assert(first[0]==8&&second[0]==14);
    std::array<std::uint8_t,1> command{'p'};transfer(child,child.input_fd(),command,true);std::array<std::uint8_t,8> response{};transfer(child,child.output_fd(),response,false);
    s::wire::reader reader{response};std::uint64_t mask{};assert(reader.u64(mask)&&mask==65535);command[0]='x';transfer(child,child.input_fd(),command,true);
    auto status=reaped(child,false);assert(WIFEXITED(status)&&WEXITSTATUS(status)==0);
    // Rejected startup must never publish a live endpoint; actual namespace pidfd confirms cleanup.
    for(unsigned mode=1;mode<=10;++mode)
    {
        uwvm2::utils::container::std_vector<x::descriptor> owned;auto selected=make_manifest(argv+1,owned,mode);
        if(mode==4)selected.bound.thread_count=1;
        if(mode==5)selected.bound.startup_ns=50000000;
        x::process failing;assert(failing.prepare(selected)==error::success&&failing.launch()==error::success);auto result=startup(failing);
        if(mode==7){assert(result==error::success&&failing.activate(pipes.roles)==error::success);result=startup(failing,true);}
        assert(result!=error::success);auto exited=reaped(failing,mode<8);assert(WIFEXITED(exited)||WIFSIGNALED(exited));
        if(mode>=8){assert(WIFEXITED(exited)&&WEXITSTATUS(exited)==125);std::printf("unsafe executable mapping mode=%u rejected by maps check, exit=125\n",mode);}
    }
    // Explicit revocation kills and reaps a live, attached worker and both existing pump threads.
    x::process revoked;assert(revoked.prepare(manifest)==error::success&&revoked.launch()==error::success&&startup(revoked)==error::success);
    assert(revoked.activate(pipes.roles)==error::success&&startup(revoked,true)==error::success);status=reaped(revoked);assert(WIFSIGNALED(status)&&WTERMSIG(status)==SIGKILL);
    std::puts("PASS sealed communication process: 14 probes on all 3 threads plus 2 main-thread checks, four directions, 10 failed startups including RWX/anonymous/unapproved-file execution, pidfd cleanup");
}
#endif
