/** @file
 * @brief External kernel-notification fault injector for the unmodified VM.
 * @details The original process execs the exact recorded VM. A separate test
 * process answers pidfd_open notifications, refusing only a namespace child
 * of a direct VM child. The launcher pidfd and debugger authentication remain
 * available. No test library or callback enters the VM. An independent pidfd
 * cleans the refused namespace after VM exit; that cleanup is never reported
 * to the VM as product evidence. In release-failure mode, acquisition succeeds;
 * the supervisor instead kills and waits for both pinned peers when the same
 * VM thread reaches its one-byte namespace release. Continuing that real
 * syscall delivers kernel EPIPE, after the VM's real successful liveness poll.
 * No guest memory is inspected and no successful syscall result is fabricated.
 * All processes inherit the test cgroup. This injector targets Linux x86-64.
 */
#include <array>
#include <cassert>
#include <cerrno>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <poll.h>
#include <signal.h>
#include <string_view>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <unistd.h>

#if defined(UWVM_TEST_NAMESPACE_RELEASE_FAILURE)
static constexpr bool release_failure=true;
#else
static constexpr bool release_failure=false;
#endif

static pid_t status_id(pid_t pid,std::string_view field)
{
    std::array<char,64> path{};constexpr std::string_view prefix="/proc/";
    std::memcpy(path.data(),prefix.data(),prefix.size());
    auto rendered=std::to_chars(path.data()+prefix.size(),path.data()+48,pid);
    if(rendered.ec!=std::errc{})return -1;
    std::memcpy(rendered.ptr,"/status",8);
    auto fd=::open(path.data(),O_RDONLY|O_CLOEXEC);if(fd<0)return -1;
    std::array<char,4096> data{};auto count=::read(fd,data.data(),data.size());::close(fd);if(count<=0)return -1;
    std::string_view text{data.data(),static_cast<std::size_t>(count)};auto at=text.find(field);
    if(at==text.npos)return -1;
    at+=field.size();while(at<text.size()&&(text[at]==' '||text[at]=='\t'))++at;
    pid_t result{};auto value=std::from_chars(text.data()+at,text.data()+text.size(),result);
    return value.ec==std::errc{}?result:-1;
}
static pid_t parent_of(pid_t pid){return status_id(pid,"\nPPid:");}
static bool kill_and_wait(int fd)
{
    if(::syscall(SYS_pidfd_send_signal,fd,SIGKILL,nullptr,0)&&errno!=ESRCH)return false;
    pollfd stopped{fd,POLLIN,0};int ready;
    do{ready=::poll(&stopped,1,2000);}while(ready<0&&errno==EINTR);
    return ready==1&&(stopped.revents&POLLIN);
}
static int supervise(int channel,pid_t vm)
{
    auto root=static_cast<int>(::syscall(SYS_pidfd_open,vm,0));if(root<0)return 120;
    char byte{};iovec bytes{&byte,1};alignas(cmsghdr)std::array<char,CMSG_SPACE(sizeof(int))> ancillary{};
    msghdr message{};message.msg_iov=&bytes;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
    if(::recvmsg(channel,&message,MSG_CMSG_CLOEXEC)!=1)return 121;
    auto header=CMSG_FIRSTHDR(&message);if(!header||header->cmsg_level!=SOL_SOCKET||header->cmsg_type!=SCM_RIGHTS||header->cmsg_len!=CMSG_LEN(sizeof(int)))return 122;
    int listener{};std::memcpy(&listener,CMSG_DATA(header),sizeof(listener));::close(channel);
    int observed=-1,launcher=-1;pid_t owner_thread{};bool injected{};
    for(;;)
    {
        std::array<pollfd,2> wait{{{root,POLLIN,0},{listener,POLLIN,0}}};
        if(::poll(wait.data(),wait.size(),1000)<0){if(errno==EINTR)continue;return 123;}
        if(wait[0].revents&POLLIN)break;
        if(!(wait[1].revents&POLLIN))continue;
        seccomp_notif request{};if(::ioctl(listener,SECCOMP_IOCTL_NOTIF_RECV,&request)){if(errno==EINTR||errno==ENOENT)continue;return 124;}
        seccomp_notif_resp response{};response.id=request.id;response.flags=SECCOMP_USER_NOTIF_FLAG_CONTINUE;
        auto target=static_cast<pid_t>(request.data.args[0]);auto parent=parent_of(target);
        if(observed<0&&request.data.nr==SYS_pidfd_open&&status_id(static_cast<pid_t>(request.pid),"\nTgid:")==vm&&
           target>0&&parent>0&&parent!=vm&&parent_of(parent)==vm)
        {
            auto candidate=static_cast<int>(::syscall(SYS_pidfd_open,target,0));
            if(candidate>=0)
            {
                pollfd alive{candidate,POLLIN,0};
                if(parent_of(target)==parent&&parent_of(parent)==vm&&::poll(&alive,1,0)==0)
                {
                    if constexpr(release_failure)
                    {
                        launcher=static_cast<int>(::syscall(SYS_pidfd_open,parent,0));
                        std::array<pollfd,2> peers{{{candidate,POLLIN,0},{launcher,POLLIN,0}}};
                        if(launcher<0||parent_of(target)!=parent||parent_of(parent)!=vm||::poll(peers.data(),peers.size(),0)!=0)return 130;
                        observed=candidate;owner_thread=static_cast<pid_t>(request.pid);
                    }
                    else{observed=candidate;injected=true;response.flags=0;response.error=-ENOSYS;}
                }
                else ::close(candidate);
            }
        }
        if constexpr(release_failure)
        {
            if(!injected&&observed>=0&&static_cast<pid_t>(request.pid)==owner_thread&&
               (request.data.nr==SYS_write||request.data.nr==SYS_sendto)&&request.data.args[2]==1)
            {
                if(::ioctl(listener,SECCOMP_IOCTL_NOTIF_ID_VALID,&request.id))return 131;
                if(!kill_and_wait(observed)||!kill_and_wait(launcher))return 132;
                injected=true;
            }
        }
        if(::ioctl(listener,SECCOMP_IOCTL_NOTIF_SEND,&response)&&errno!=ENOENT)return 125;
    }
    if(observed>=0)
    {
        if(!kill_and_wait(observed))return 126;
        ::close(observed);
    }
    if(launcher>=0){if(!kill_and_wait(launcher))return 127;::close(launcher);}
    ::close(root);::close(listener);
    constexpr std::string_view marker=release_failure?"FIXTURE_NAMESPACE_RELEASE_PEERS_EXITED_BEFORE_SEND\n":
        "FIXTURE_NAMESPACE_PIDFD_REFUSED_AND_EXTERNALLY_CLEANED\n";
    if(injected)(void)::write(2,marker.data(),marker.size());
    return injected?0:128;
}
int main(int argc,char** argv)
{
    assert(argc>=2);int channels[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,channels));
    auto vm=::getpid();auto child=::fork();assert(child>=0);
    if(!child){::close(channels[0]);::_exit(supervise(channels[1],vm));}
    ::close(channels[1]);
    std::array<sock_filter,7> filter{{
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_pidfd_open,0,1),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_USER_NOTIF),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW)}};
    sock_fprog program{static_cast<unsigned short>(filter.size()),filter.data()};
    std::array<sock_filter,14> release_filter{{
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_pidfd_open,7,0),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_write,2,0),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_sendto,1,0),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[2])+4),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,0,0,3),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[2])),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,1,0,1),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_USER_NOTIF),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW)}};
    if constexpr(release_failure)program={static_cast<unsigned short>(release_filter.size()),release_filter.data()};
    assert(!::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0));
    auto listener=static_cast<int>(::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,SECCOMP_FILTER_FLAG_NEW_LISTENER,&program));assert(listener>=0);
    char byte{};iovec data{&byte,1};alignas(cmsghdr)std::array<char,CMSG_SPACE(sizeof(int))> ancillary{};
    msghdr message{};message.msg_iov=&data;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
    auto header=CMSG_FIRSTHDR(&message);header->cmsg_level=SOL_SOCKET;header->cmsg_type=SCM_RIGHTS;header->cmsg_len=CMSG_LEN(sizeof(int));
    std::memcpy(CMSG_DATA(header),&listener,sizeof(listener));assert(::sendmsg(channels[0],&message,MSG_NOSIGNAL)==1);
    ::close(channels[0]);::close(listener);
    argv[0]=const_cast<char*>(UWVM_TEST_RUNTIME_PATH);::execv(UWVM_TEST_RUNTIME_PATH,argv);return 129;
}
