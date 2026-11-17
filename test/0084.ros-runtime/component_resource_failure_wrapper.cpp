/** @file
 * @brief Refuse resource-limit readback in a real VM's owned pre-exec child.
 * @details A test-only external seccomp notifier distinguishes the VM from its
 * direct namespace-init child and pins that child's identity before refusal.
 * No VM memory, syscall implementation or driver code is replaced. Readiness
 * must never be published and the product must reap the child. Rescue follows
 * the independent exit observation and cannot turn a failed test into success.
 */
#include <array>
#include <cassert>
#include <cerrno>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstdio>
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
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

static pid_t parent_of(pid_t pid)
{
    std::array<char,64> path{};std::array<char,4096> data{};
    if(std::snprintf(path.data(),path.size(),"/proc/%d/status",pid)<=0)return -1;
    auto fd=::open(path.data(),O_RDONLY|O_CLOEXEC);if(fd<0)return -1;
    auto count=::read(fd,data.data(),data.size());::close(fd);if(count<=0)return -1;
    std::string_view value{data.data(),static_cast<std::size_t>(count)};
    auto at=value.find("\nPPid:");if(at==value.npos)return -1;at+=6;
    while(at<value.size()&&(value[at]==' '||value[at]=='\t'))++at;
    pid_t parent{};return std::from_chars(value.data()+at,value.data()+value.size(),parent).ec==std::errc{}?parent:-1;
}
static ino_t pid_namespace(pid_t pid)
{
    std::array<char,64> path{};struct stat information{};
    if(std::snprintf(path.data(),path.size(),"/proc/%d/ns/pid",pid)<=0||::stat(path.data(),&information))return 0;
    return information.st_ino;
}
static bool dead(int fd)
{pollfd value{fd,POLLIN,0};return ::poll(&value,1,0)==1&&(value.revents&POLLIN);}
static bool rescue(int fd)
{
    if(::syscall(SYS_pidfd_send_signal,fd,SIGKILL,nullptr,0)&&errno!=ESRCH)return false;
    pollfd stopped{fd,POLLIN,0};int ready;
    do{ready=::poll(&stopped,1,2000);}while(ready<0&&errno==EINTR);
    return ready==1&&(stopped.revents&POLLIN);
}
static int supervise(int channel,pid_t vm)
{
    auto root=static_cast<int>(::syscall(SYS_pidfd_open,vm,0));if(root<0)return 120;
    auto root_namespace=pid_namespace(vm);if(!root_namespace)return 121;
    char byte{};iovec bytes{&byte,1};alignas(cmsghdr)std::array<char,CMSG_SPACE(sizeof(int))> ancillary{};
    msghdr message{};message.msg_iov=&bytes;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
    if(::recvmsg(channel,&message,MSG_CMSG_CLOEXEC)!=1)return 122;
    auto header=CMSG_FIRSTHDR(&message);
    if(!header||header->cmsg_level!=SOL_SOCKET||header->cmsg_type!=SCM_RIGHTS||header->cmsg_len!=CMSG_LEN(sizeof(int)))return 123;
    int listener{};std::memcpy(&listener,CMSG_DATA(header),sizeof(listener));::close(channel);
    int observed=-1;bool injected{};
    for(;;)
    {
        std::array<pollfd,2> wait{{{root,POLLIN,0},{listener,POLLIN,0}}};
        if(::poll(wait.data(),wait.size(),1000)<0){if(errno==EINTR)continue;return 124;}
        if(wait[0].revents&POLLIN)break;
        if(!(wait[1].revents&POLLIN))continue;
        seccomp_notif request{};
        if(::ioctl(listener,SECCOMP_IOCTL_NOTIF_RECV,&request)){if(errno==EINTR||errno==ENOENT)continue;return 125;}
        seccomp_notif_resp response{};response.id=request.id;response.flags=SECCOMP_USER_NOTIF_FLAG_CONTINUE;
        auto pid=static_cast<pid_t>(request.pid);
        if(observed<0&&pid!=vm&&parent_of(pid)==vm&&request.data.nr==SYS_prlimit64&&
           !request.data.args[0]&&request.data.args[1]==RLIMIT_NOFILE&&!request.data.args[2]&&request.data.args[3])
        {
            auto child_namespace=pid_namespace(pid);
            if(child_namespace&&child_namespace!=root_namespace)
            {
                observed=static_cast<int>(::syscall(SYS_pidfd_open,pid,0));
                if(observed<0||dead(observed)||parent_of(pid)!=vm||pid_namespace(pid)!=child_namespace)return 126;
                if(::ioctl(listener,SECCOMP_IOCTL_NOTIF_ID_VALID,&request.id))return 127;
                response.flags=0;response.error=-EPERM;injected=true;
            }
        }
        if(::ioctl(listener,SECCOMP_IOCTL_NOTIF_SEND,&response)&&errno!=ENOENT)return 128;
    }
    bool cleaned=observed>=0&&dead(observed);
    if(observed>=0){if(!rescue(observed))return 129;::close(observed);}
    ::close(root);::close(listener);
    constexpr std::string_view marker="FIXTURE_RESOURCE_READBACK_REFUSED_AND_PRODUCT_CLEANUP\n";
    if(injected&&cleaned)(void)::write(2,marker.data(),marker.size());
    return injected&&cleaned?0:130;
}
int main(int argc,char** argv)
{
    assert(argc>=2);int channels[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,channels));
    auto vm=::getpid();auto child=::fork();assert(child>=0);
    if(!child){::close(channels[0]);::_exit(supervise(channels[1],vm));}
    ::close(channels[1]);
    std::array<sock_filter,12> filter{{
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_prlimit64,0,6),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[1])),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,RLIMIT_NOFILE,0,4),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[2])),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,0,0,2),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_USER_NOTIF),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW)}};
    sock_fprog program{static_cast<unsigned short>(filter.size()),filter.data()};
    assert(!::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0));
    auto listener=static_cast<int>(::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,SECCOMP_FILTER_FLAG_NEW_LISTENER,&program));assert(listener>=0);
    char byte{};iovec data{&byte,1};alignas(cmsghdr)std::array<char,CMSG_SPACE(sizeof(int))> ancillary{};
    msghdr message{};message.msg_iov=&data;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
    auto header=CMSG_FIRSTHDR(&message);header->cmsg_level=SOL_SOCKET;header->cmsg_type=SCM_RIGHTS;header->cmsg_len=CMSG_LEN(sizeof(int));
    std::memcpy(CMSG_DATA(header),&listener,sizeof(listener));assert(::sendmsg(channels[0],&message,MSG_NOSIGNAL)==1);
    ::close(channels[0]);::close(listener);
    argv[0]=const_cast<char*>(UWVM_TEST_RUNTIME_PATH);::execv(UWVM_TEST_RUNTIME_PATH,argv);return 131;
}
