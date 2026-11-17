/** @file @brief Interpret and install the exact communication syscall fences. */
#include <uwvm2/imported/ros/scene/communication_worker_syscalls.h>
#include <atomic>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <thread>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/wait.h>
namespace x=uwvm2::imported::ros::scene::communication_sandbox;
namespace w=uwvm2::imported::ros::scene::communication_worker;
static std::uint32_t decision(x::syscall_filter const& policy,seccomp_data const& data)
{
    auto p=policy.view();assert(!p.empty());std::uint32_t value{};
    for(std::size_t at{},steps{};at<p.size()&&steps<4096;++at,++steps)
    {
        auto i=p[at];switch(i.code)
        {
        case BPF_LD|BPF_W|BPF_ABS:assert(i.k<=sizeof(data)-4);std::memcpy(&value,reinterpret_cast<unsigned char const*>(&data)+i.k,4);break;
        case BPF_ALU|BPF_AND|BPF_K:value&=i.k;break;
        case BPF_JMP|BPF_JEQ|BPF_K:at+=value==i.k?i.jt:i.jf;break;
        case BPF_JMP|BPF_JSET|BPF_K:at+=(value&i.k)?i.jt:i.jf;break;
        case BPF_RET|BPF_K:return i.k;
        default:assert(false);
        }
    }
    assert(false);return 0;
}
static void interpreted()
{
    using phase=x::syscall_phase;std::array<w::role_fd,4> pipes{};for(std::size_t i{};i<4;++i)pipes[i]={w::roles[i],static_cast<int>(10+i)};
    std::array<w::internal_fd,3> notes{{{20,w::io_direction::both,w::notification_kind::event},{21,w::io_direction::both,w::notification_kind::poll},{22,w::io_direction::read,w::notification_kind::pipe}}};
    x::syscall_filter startup{phase::startup},confined{phase::confined},active{phase::active,pipes,notes};seccomp_data call{};call.arch=AUDIT_ARCH_X86_64;
    auto allows=[&](x::syscall_filter const& p,long number,unsigned long argument=0)
    {call.nr=static_cast<int>(number);call.args[0]=argument;return decision(p,call)==SECCOMP_RET_ALLOW;};
    auto combined=[&](long n,unsigned long arg=0){return allows(startup,n,arg)&&allows(confined,n,arg)&&allows(active,n,arg);};
    for(auto n:{SYS_socket,SYS_connect,SYS_bind,SYS_sendto,SYS_sendmsg,SYS_execve,SYS_clone,SYS_clone3,SYS_fork,SYS_vfork,SYS_ptrace,SYS_process_vm_readv,SYS_io_uring_setup,SYS_memfd_create,SYS_openat,SYS_dup,SYS_dup2,SYS_dup3,SYS_prctl,SYS_seccomp,SYS_mremap,SYS_personality})assert(!combined(n));
    assert(allows(startup,SYS_personality,0xfffffffful)&&!allows(startup,SYS_personality,0)&&!allows(startup,SYS_personality,(1ull<<32)|0xfffffffful));
    for(auto fd:{0,10,12,20,22})assert(combined(SYS_read,fd)&&combined(SYS_readv,fd));
    for(auto fd:{1,2,11,13,20})assert(combined(SYS_write,fd)&&combined(SYS_writev,fd));
    for(auto fd:{1,2,3,9,11,13,21})assert(!combined(SYS_read,fd));
    for(auto fd:{0,3,9,10,12,21,22})assert(!combined(SYS_write,fd));
    assert(!combined(SYS_read,(1ull<<32)|10));
    assert(combined(SYS_epoll_wait,21)&&combined(SYS_epoll_ctl,21));assert(!combined(SYS_epoll_wait,20));
    assert(allows(startup,SYS_recvmsg,3)&&allows(confined,SYS_recvmsg,3)&&!allows(active,SYS_recvmsg,3));
    assert(!allows(startup,SYS_recvmsg,4));
    auto flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID;
    assert(allows(startup,SYS_clone,flags));for(auto denied:{0u,static_cast<unsigned>(flags&~CLONE_THREAD),static_cast<unsigned>(flags|CLONE_NEWUSER),static_cast<unsigned>(flags|17)})assert(!allows(startup,SYS_clone,denied));
    call.args[2]=PROT_READ|PROT_WRITE;assert(combined(SYS_mmap));call.args[2]=PROT_READ|PROT_EXEC;assert(!combined(SYS_mmap)&&!combined(SYS_mprotect));call.args[2]=0;
    call.args[1]=F_GETFL;assert(combined(SYS_fcntl,10));call.args[1]=F_DUPFD;assert(!combined(SYS_fcntl,10));call.args[1]=0;
    auto duplicate=pipes;duplicate[3].descriptor=10;assert(x::syscall_filter(phase::active,duplicate,notes).view().empty());
    duplicate=pipes;duplicate[3].role=w::pipe_role::user_rx;assert(x::syscall_filter(phase::active,duplicate,notes).view().empty());
    assert(x::syscall_filter(phase::active,{},notes).view().empty());
    call.arch=AUDIT_ARCH_I386;call.nr=SYS_read;assert(decision(startup,call)==SECCOMP_RET_KILL_PROCESS);
    call.arch=AUDIT_ARCH_X86_64;call.nr=SYS_read|0x40000000;assert(decision(startup,call)==SECCOMP_RET_KILL_PROCESS);
}
static void kernel()
{
    int control=::eventfd(0,EFD_CLOEXEC);assert(control>=3); // Reserve the bootstrap receive-only control slot.
    std::array<std::array<int,2>,4> pipes{};for(auto& p:pipes)assert(!::pipe2(p.data(),O_CLOEXEC|O_NONBLOCK));
    char seed='r';assert(::write(pipes[0][1],&seed,1)==1&&::write(pipes[2][1],&seed,1)==1);
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        using phase=x::syscall_phase;std::array<w::role_fd,4> roles{};
        for(std::size_t i{};i<4;++i){bool rx=w::receiving(w::roles[i]);roles[i]={w::roles[i],pipes[i][rx?0:1]};::close(pipes[i][rx?1:0]);}
        int event=::eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK);if(event<0)::_exit(10);
        std::array<w::internal_fd,1> notes{{{event,w::io_direction::both,w::notification_kind::event}}};
        x::syscall_filter startup{phase::startup};if(!startup.install())::_exit(11);
        std::atomic<bool> waiting{},released{};std::atomic<int> failure{};
        std::thread thread{[&]
        {
            waiting=true;while(!released.load())::sched_yield();
            errno=0;if(::syscall(SYS_socket,AF_INET,SOCK_DGRAM,0)!=-1||errno!=ENOSYS)failure=1;
            char byte{};errno=0;if(::read(roles[1].descriptor,&byte,1)!=-1||errno!=ENOSYS)failure=2;
            errno=0;if(::write(roles[0].descriptor,&seed,1)!=-1||errno!=ENOSYS)failure=3;
            if(::read(roles[0].descriptor,&byte,1)!=1||byte!='r')failure=4;
            if(::write(roles[1].descriptor,&seed,1)!=1)failure=5;
        }};
        while(!waiting.load())::sched_yield();
        x::syscall_filter confined{phase::confined},active{phase::active,roles,notes};
        if(!confined.install())::_exit(12);
        if(!active.install())::_exit(13);
        released=true;thread.join();if(failure.load())::_exit(20+failure.load());
        std::uint64_t value=1;if(::write(event,&value,8)!=8||::read(event,&value,8)!=8||value!=1)::_exit(30);
        errno=0;if(::syscall(SYS_clone,CLONE_VM|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD,0,0,0,0)!=-1||errno!=ENOSYS)::_exit(31);
        errno=0;if(::syscall(SYS_openat,AT_FDCWD,"/etc/passwd",O_RDONLY)!=-1||errno!=ENOSYS)::_exit(32);
        errno=0;if(::syscall(SYS_mmap,0,4096,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_ANONYMOUS,-1,0)!=-1||errno!=ENOSYS)::_exit(33);
        errno=0;if(::syscall(SYS_mremap,0,0,0,0)!=-1||errno!=ENOSYS)::_exit(34);
        ::_exit(0);
    }
    int status{};assert(::waitpid(child,&status,0)==child);if(!WIFEXITED(status)||WEXITSTATUS(status))std::fprintf(stderr,"worker filter child status=%d\n",status);
    assert(WIFEXITED(status)&&WEXITSTATUS(status)==0);char byte{};assert(::read(pipes[1][0],&byte,1)==1&&byte=='r');for(auto p:pipes)for(auto fd:p)::close(fd);::close(control);
}
int main(){interpreted();kernel();std::puts("PASS communication BPF directions, closed syscalls, pthread startup and actual two-stage all-thread TSYNC");}
