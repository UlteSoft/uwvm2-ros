/** @file
 * @brief External kernel notifications exercise the born-domain VM boundary.
 * @details A separate fixture pins the actual domain init and its blocked
 * descendant before refusing the VM's startup-pin duplication, killing the
 * launcher, or killing it immediately before the namespace-release send.
 * It never reads guest memory or injects VM code. Independent pidfds check
 * cleanup before any test-only rescue, and a success marker requires real
 * product-owned teardown. All processes inherit the existing test cgroup.
 */
#include <array>
#include <cassert>
#include <cerrno>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
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

#if defined(UWVM_TEST_DOMAIN_RELEASE_FAILURE)
static constexpr unsigned fault=2;
#elif defined(UWVM_TEST_DOMAIN_LAUNCHER_FAILURE)
static constexpr unsigned fault=1;
#else
static constexpr unsigned fault=0;
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
static pid_t descriptor_pid(pid_t thread,int fd)
{
    std::array<char,128> path{};std::array<char,4096> data{};
    if(std::snprintf(path.data(),path.size(),"/proc/%d/fdinfo/%d",thread,fd)<=0)return -1;
    auto file=::open(path.data(),O_RDONLY|O_CLOEXEC);if(file<0)return -1;
    auto count=::read(file,data.data(),data.size());::close(file);if(count<=0)return -1;
    std::string_view value{data.data(),static_cast<std::size_t>(count)};auto at=value.find("\nPid:");if(at==value.npos)return -1;
    at+=5;while(at<value.size()&&(value[at]==' '||value[at]=='\t'))++at;
    pid_t result{};return std::from_chars(value.data()+at,value.data()+value.size(),result).ec==std::errc{}?result:-1;
}
static pid_t first_child(pid_t parent)
{
    std::array<char,128> path{};std::array<char,256> data{};
    if(std::snprintf(path.data(),path.size(),"/proc/%d/task/%d/children",parent,parent)<=0)return -1;
    auto file=::open(path.data(),O_RDONLY|O_CLOEXEC);if(file<0)return -1;
    auto count=::read(file,data.data(),data.size());::close(file);if(count<=0)return -1;
    pid_t result{};return std::from_chars(data.data(),data.data()+count,result).ec==std::errc{}?result:-1;
}
static bool dead(int fd)
{pollfd value{fd,POLLIN,0};return ::poll(&value,1,0)==1&&(value.revents&POLLIN);}
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
        if(observed<0&&request.data.nr==SYS_fcntl&&request.data.args[1]==F_DUPFD_CLOEXEC&&
           status_id(static_cast<pid_t>(request.pid),"\nTgid:")==vm)
        {
            auto parent=descriptor_pid(static_cast<pid_t>(request.pid),static_cast<int>(request.data.args[0]));
            if(parent>0&&parent_of(parent)==vm)
            {
                auto child=first_child(parent);
                if(child>0)
                {
                    launcher=static_cast<int>(::syscall(SYS_pidfd_open,parent,0));
                    observed=static_cast<int>(::syscall(SYS_pidfd_open,child,0));
                    if(launcher<0||observed<0||parent_of(child)!=parent||parent_of(parent)!=vm||dead(launcher)||dead(observed))return 130;
                    owner_thread=static_cast<pid_t>(request.pid);
                    if constexpr(fault==0){response.flags=0;response.error=-EMFILE;injected=true;}
                    else if constexpr(fault==1){if(!kill_and_wait(launcher)||!dead(observed))return 131;injected=true;}
                }
            }
        }
        if constexpr(fault==2)
        {
            if(!injected&&observed>=0&&static_cast<pid_t>(request.pid)==owner_thread&&
               request.data.nr==SYS_sendto&&request.data.args[2]==1&&request.data.args[3]==MSG_NOSIGNAL)
            {
                if(::ioctl(listener,SECCOMP_IOCTL_NOTIF_ID_VALID,&request.id))return 132;
                if(!kill_and_wait(launcher)||!dead(observed))return 133;
                injected=true;
            }
        }
        if(::ioctl(listener,SECCOMP_IOCTL_NOTIF_SEND,&response)&&errno!=ENOENT)return 125;
    }
    bool cleaned=observed>=0&&launcher>=0&&dead(observed)&&dead(launcher);
    // Rescue only after the product's independent observations were recorded.
    if(observed>=0){if(!kill_and_wait(observed))return 126;::close(observed);}
    if(launcher>=0){if(!kill_and_wait(launcher))return 127;::close(launcher);}
    ::close(root);::close(listener);
    constexpr std::string_view marker="FIXTURE_BORN_DOMAIN_FAULT_AND_PRODUCT_TREE_CLEANUP\n";
    if(injected&&cleaned)(void)::write(2,marker.data(),marker.size());
    return injected&&cleaned?0:128;
}

int main(int argc,char** argv)
{
    assert(argc>=2);int channels[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,channels));
    auto vm=::getpid();auto child=::fork();assert(child>=0);
    if(!child){::close(channels[0]);::_exit(supervise(channels[1],vm));}
    ::close(channels[1]);
    std::array<sock_filter,17> filter{{
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_fcntl,0,3),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[1])),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,F_DUPFD_CLOEXEC,8,0),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_sendto,0,7),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[2])+4),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,0,0,5),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[2])),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,1,0,3),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[3])),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,MSG_NOSIGNAL,0,1),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_USER_NOTIF),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW)}};
    sock_fprog program{static_cast<unsigned short>(filter.size()),filter.data()};
    assert(!::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0));
    auto listener=static_cast<int>(::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,SECCOMP_FILTER_FLAG_NEW_LISTENER,&program));assert(listener>=0);
    char byte{};iovec data{&byte,1};alignas(cmsghdr)std::array<char,CMSG_SPACE(sizeof(int))> ancillary{};
    msghdr message{};message.msg_iov=&data;message.msg_iovlen=1;message.msg_control=ancillary.data();message.msg_controllen=ancillary.size();
    auto header=CMSG_FIRSTHDR(&message);header->cmsg_level=SOL_SOCKET;header->cmsg_type=SCM_RIGHTS;header->cmsg_len=CMSG_LEN(sizeof(int));
    std::memcpy(CMSG_DATA(header),&listener,sizeof(listener));assert(::sendmsg(channels[0],&message,MSG_NOSIGNAL)==1);
    ::close(channels[0]);::close(listener);
    argv[0]=const_cast<char*>(UWVM_TEST_RUNTIME_PATH);::execv(UWVM_TEST_RUNTIME_PATH,argv);return 129;
}
