/** @file
 * @brief Real namespace-tree cleanup, parent-death and launch-refusal tests.
 * @details Uses the actual kernel-born domain and immediate executable replacement, not a process-provider mock.
 * Fixture descendants pass their own pidfds to the outer observer through a
 * test-only socket, allowing every observed generation's death to be checked.
 * Production never receives those independent observation descriptors.
 */
#include "support/legacy_process/linux_process_domain.h"
#include <cassert>
#include <cstring>
#include <cstdio>
#include <chrono>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <cstddef>
#include <sys/mman.h>
#include <sys/stat.h>
namespace d=uwvm2::imported::ros::scene::linux_process_domain;

static void transfer(int channel,int fd)
{
    char marker='p';iovec vector{&marker,1};
    alignas(cmsghdr) std::array<char,CMSG_SPACE(sizeof(int))> bytes{};
    msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;message.msg_control=bytes.data();message.msg_controllen=bytes.size();
    auto header=CMSG_FIRSTHDR(&message);header->cmsg_level=SOL_SOCKET;header->cmsg_type=SCM_RIGHTS;header->cmsg_len=CMSG_LEN(sizeof(int));
    std::memcpy(CMSG_DATA(header),&fd,sizeof(fd));assert(::sendmsg(channel,&message,MSG_NOSIGNAL)==1);
}
static int receive(int channel)
{
    pollfd item{channel,POLLIN,0};assert(::poll(&item,1,3000)==1&&(item.revents&POLLIN));
    char marker{};iovec vector{&marker,1};alignas(cmsghdr) std::array<char,CMSG_SPACE(sizeof(int))> bytes{};
    msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;message.msg_control=bytes.data();message.msg_controllen=bytes.size();
    assert(::recvmsg(channel,&message,MSG_CMSG_CLOEXEC)==1&&marker=='p'&&!(message.msg_flags&(MSG_TRUNC|MSG_CTRUNC)));
    auto header=CMSG_FIRSTHDR(&message);assert(header&&header->cmsg_level==SOL_SOCKET&&header->cmsg_type==SCM_RIGHTS&&header->cmsg_len==CMSG_LEN(sizeof(int)));
    int fd{};std::memcpy(&fd,CMSG_DATA(header),sizeof(fd));return fd;
}
static void died(int identity)
{pollfd item{identity,POLLIN,0};assert(::poll(&item,1,3000)==1&&(item.revents&POLLIN));::close(identity);}
static void observe_self(int channel)
{auto fd=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(fd>=0);transfer(channel,fd);::close(fd);}
[[noreturn]] static void tree(int channel,bool crash,bool nested)
{
    observe_self(channel);
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        assert(::setsid()>=0);observe_self(channel);auto grandchild=::fork();assert(grandchild>=0);
        if(!grandchild)
        {
            observe_self(channel);
            if(nested)
            {
                assert(!::unshare(CLONE_NEWUSER|CLONE_NEWPID));auto deeper=::fork();assert(deeper>=0);
                if(!deeper)observe_self(channel);
            }
        }
        for(;;)::pause();
    }
    if(crash){::usleep(100000);::_exit(71);}
    for(;;)::pause();
}

struct started{pid_t child;int identity;};
static started start(int output,bool crash,bool nested,bool authorize=true)
{
    auto parent=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(parent>=0);
    auto executable=::open("/proc/self/exe",O_RDONLY|O_CLOEXEC);assert(executable>=0);
    int gates[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,gates));
    int identity{-1};bool restored{};auto child=d::spawn(identity,restored);assert(child>=0);
    if(!child)
    {
        if(!d::enter(parent))::_exit(126);
        ::close(gates[0]);std::uint64_t permit{};
        if(::recv(gates[1],&permit,sizeof(permit),MSG_TRUNC)!=sizeof(permit)||permit!=1)::_exit(126);
        if(::dup2(output,3)<0)::_exit(126);
        if(::dup3(executable,4,O_CLOEXEC)<0||::syscall(SYS_close_range,5u,UINT_MAX,0))::_exit(126);
        char first[]{"domain"},worker[]{"worker"},crashing[]{"crash"},quiet[]{"wait"},nest[]{"nested"},simple[]{"simple"};
        char* arguments[]{first,worker,crash?crashing:quiet,nested?nest:simple,nullptr};char* environment[]{nullptr};
        ::syscall(SYS_execveat,4,"",arguments,environment,AT_EMPTY_PATH);::_exit(126);
    }
    assert(restored&&identity>=0);::close(parent);::close(executable);::close(gates[1]);
    assert(d::map_identity(child,identity,::geteuid(),::getegid()));
    if(authorize){std::uint64_t permit=1;assert(::send(gates[0],&permit,sizeof(permit),MSG_NOSIGNAL)==sizeof(permit));}
    else assert(!::shutdown(gates[0],SHUT_WR));
    ::close(gates[0]);return {child,identity};
}
static void group(bool crash,bool nested)
{
    int channel[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,channel));
    auto root=start(channel[1],crash,nested);std::array<int,4> identities{};auto count=nested?4:3;
    for(int i=0;i<count;++i)identities[i]=receive(channel[0]);
    if(!crash)assert(!::syscall(SYS_pidfd_send_signal,root.identity,SIGKILL,nullptr,0));
    died(root.identity);
    for(int i=0;i<count;++i)died(identities[i]);
    int status{};assert(::waitpid(root.child,&status,0)==root.child);
    assert(crash?(WIFEXITED(status)&&WEXITSTATUS(status)==71):(WIFSIGNALED(status)&&WTERMSIG(status)==SIGKILL));
    ::close(channel[0]);::close(channel[1]);
}
static void owner_death()
{
    int channel[2],control[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,channel));
    assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,control));
    auto owner=::fork();assert(owner>=0);
    if(!owner){auto root=start(channel[1],false,true);transfer(control[1],root.identity);for(;;)::pause();}
    auto owner_identity=static_cast<int>(::syscall(SYS_pidfd_open,owner,0));assert(owner_identity>=0);
    auto root=receive(control[0]);std::array<int,4> children{};for(auto& item:children)item=receive(channel[0]);
    assert(!::syscall(SYS_pidfd_send_signal,owner_identity,SIGKILL,nullptr,0));died(owner_identity);died(root);for(auto item:children)died(item);
    int status{};assert(::waitpid(owner,&status,0)==owner);for(auto fd:{channel[0],channel[1],control[0],control[1]})::close(fd);
}
int main(int argc,char** argv)
{
    if(argc==4)
    {
        assert(std::string_view(argv[1])=="worker"&&::getpid()==1);
        // A 1 GiB parent reservation must disappear after exec, not remain in a
        // permanent forked guardian. Native test processes have no VM pointers.
        auto status=::open("/proc/self/statm",O_RDONLY|O_CLOEXEC);assert(status>=0);
        std::array<char,128> data{};auto count=::read(status,data.data(),data.size());assert(count>0);::close(status);
        unsigned long pages{};assert(std::from_chars(data.data(),data.data()+count,pages).ec==std::errc{});
        assert(pages*static_cast<unsigned long>(::sysconf(_SC_PAGESIZE))<256ul*1024*1024);
        tree(3,std::string_view(argv[2])=="crash",std::string_view(argv[3])=="nested");
    }
    assert(argc==1);assert(!::prctl(PR_SET_CHILD_SUBREAPER,1,0,0,0));
    auto reservation=::mmap(nullptr,1024ul*1024*1024,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);assert(reservation!=MAP_FAILED);
    // SIGCHLD=ignore in the caller must not leak into the fixed init.
    struct sigaction ignored{},previous{};ignored.sa_handler=SIG_IGN;assert(!::sigemptyset(&ignored.sa_mask));
    group(false,false);group(true,false);group(false,true);owner_death();
    assert(!::sigaction(SIGCHLD,&ignored,&previous));
    int channel[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,channel));
    auto root=start(channel[1],true,false);std::array<int,3> children{};for(auto& item:children)item=receive(channel[0]);
    died(root.identity);for(auto item:children)died(item);assert(!::sigaction(SIGCHLD,&previous,nullptr));
    ::close(channel[0]);::close(channel[1]);
    // Rejected launch creates no fixture descendants and is actually reaped.
    auto rejected=start(-1,false,false,false);died(rejected.identity);int status{};
    assert(::waitpid(rejected.child,&status,0)==rejected.child&&WIFEXITED(status)&&WEXITSTATUS(status)==126);
    assert(!::munmap(reservation,1024ul*1024*1024));
    for(;;){int status{};auto child=::waitpid(-1,&status,0);if(child<0){assert(errno==ECHILD);break;}}
    std::puts("PASS born process domain and immediate exec: killed init, launcher failure, nested/daemonized descendants, owner death, inherited SIGCHLD ignore and denied gate");
}
