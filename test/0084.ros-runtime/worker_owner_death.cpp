/** @file
 * @brief Kill the actual worker owner at release, readiness and attachment.
 * @details The observer receives independent fixture pidfds over SCM_RIGHTS.
 * These descriptors never enter production. Exit is checked before any rescue;
 * closing an observer channel cannot stand in for the owner's death. Both the
 * model/component and DDS launchers execute their actual sealed bubblewrap and
 * bootstrap images. This proves process cleanup, not a physical stopped state.
 */
#ifdef UWVM_RELEASE_COMMUNICATION
#define UWVM_COMMUNICATION_SANDBOX_NO_MAIN
#include "communication_worker_sandbox.cpp"
#else
#include "model_worker_fixture.h"
#include <uwvm2/imported/ros/scene/model_worker_protocol.h>
#include <cstdio>
using x::error;
#endif
#include <cstring>

extern "C" ssize_t __real_read(int,void*,std::size_t);
static x::process* observed_process{};
static int report_socket{-1};
static bool intercept_release{};
// Keep the syscall-wrapped startup boundary separate from fixture control flow.
// GCC's interprocedural bounds analysis otherwise conflates the report reader
// with this test's nonreturning read interception. Production stays unchanged.
[[maybe_unused,gnu::noinline]] static error identify_worker(x::process& child)noexcept
{return child.identify();}

static pid_t number(std::string_view data,std::string_view key={})
{
    auto at=key.empty()?0:data.find(key);assert(at!=data.npos);at+=key.size();
    while(at<data.size()&&(data[at]==' '||data[at]=='\t'))++at;
    pid_t value{};auto parsed=std::from_chars(data.data()+at,data.data()+data.size(),value);
    assert(parsed.ec==std::errc{}&&value>0);return value;
}
static std::size_t information(char const* path,std::span<char> buffer)
{
    auto fd=::open(path,O_RDONLY|O_CLOEXEC);assert(fd>=0);
    auto count=__real_read(fd,buffer.data(),buffer.size());::close(fd);assert(count>0);
    return static_cast<std::size_t>(count);
}
static int descendant(int root)
{
    std::array<char,128> path{};std::array<char,4096> buffer{};
    assert(std::snprintf(path.data(),path.size(),"/proc/self/fdinfo/%d",root)>0);
    auto parent=number({buffer.data(),information(path.data(),buffer)},"\nPid:");
    assert(std::snprintf(path.data(),path.size(),"/proc/%d/task/%d/children",parent,parent)>0);
    auto child=number({buffer.data(),information(path.data(),buffer)});
    auto identity=static_cast<int>(::syscall(SYS_pidfd_open,child,0));assert(identity>=0);
    assert(std::snprintf(path.data(),path.size(),"/proc/%d/status",child)>0);
    assert(number({buffer.data(),information(path.data(),buffer)},"\nPPid:")==parent);
    return identity;
}
static bool exited(int identity,int timeout)
{pollfd item{identity,POLLIN,0};return ::poll(&item,1,timeout)==1&&(item.revents&POLLIN);}
static void kill_identity(int identity)
{assert(!::syscall(SYS_pidfd_send_signal,identity,SIGKILL,nullptr,0)||errno==ESRCH);}
[[noreturn]] static void report_and_wait()
{
    std::array<int,2> identities{observed_process->process_fd(),descendant(observed_process->process_fd())};
    for(auto fd:identities)assert(!exited(fd,0));
    char ready='r';iovec vector{&ready,1};
    alignas(cmsghdr)std::array<std::byte,CMSG_SPACE(sizeof(identities))> control{};
    msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;message.msg_control=control.data();message.msg_controllen=control.size();
    auto* header=CMSG_FIRSTHDR(&message);header->cmsg_level=SOL_SOCKET;header->cmsg_type=SCM_RIGHTS;header->cmsg_len=CMSG_LEN(sizeof(identities));
    std::memcpy(CMSG_DATA(header),identities.data(),sizeof(identities));
    assert(::sendmsg(report_socket,&message,MSG_NOSIGNAL)==1);
    for(;;)::pause();
}
extern "C" ssize_t __wrap_read(int fd,void* buffer,std::size_t size)
{
    auto count=__real_read(fd,buffer,size);
    if(count>0&&intercept_release&&std::string_view{static_cast<char*>(buffer),static_cast<std::size_t>(count)}.find("\"child-pid\":")!=std::string_view::npos)
        report_and_wait();
    return count;
}
#ifndef UWVM_RELEASE_COMMUNICATION
static void marker(x::process& child,std::string_view expected)
{
    std::array<char,64> buffer{};assert(expected.size()<=buffer.size());std::size_t copied{};
    auto until=uwvm2::imported::ros::resources::steady_now()+5000000000ll;
    while(copied<expected.size())
    {
        auto count=::read(child.output_fd(),buffer.data()+copied,expected.size()-copied);
        if(count>0)copied+=static_cast<std::size_t>(count);
        else {assert(count<0&&(errno==EINTR||errno==EAGAIN));::poll(nullptr,0,1);}
        assert(uwvm2::imported::ros::resources::steady_now()<until);
    }
    assert(std::string_view(buffer.data(),copied)==expected);
}
#endif
int main(int argc,char** argv)
{
#ifdef UWVM_RELEASE_COMMUNICATION
    assert(argc==10);
#else
    assert(argc==11);
#endif
    std::string_view stage{argv[argc-1]};assert(stage=="release"||stage=="ready"||stage=="attached");
    assert(!::prctl(PR_SET_CHILD_SUBREAPER,1,0,0,0));
    int channel[2];assert(!::socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_CLOEXEC,0,channel));
    auto owner=::fork();assert(owner>=0);
    if(!owner)
    {
        ::close(channel[0]);report_socket=channel[1];
        uwvm2::utils::container::std_vector<x::descriptor> ownership;
#ifdef UWVM_RELEASE_COMMUNICATION
        auto manifest=make_manifest(argv+1,ownership);
        pipe_set pipes;
#else
        auto manifest=worker_manifest(argv+1,ownership);
#endif
        x::process child;observed_process=&child;
        assert(child.prepare(manifest)==error::success&&child.launch()==error::success);
        intercept_release=stage=="release";
#ifdef UWVM_RELEASE_COMMUNICATION
        assert(startup(child)==error::success);
        if(stage=="attached")assert(child.activate(pipes.roles)==error::success&&startup(child,true)==error::success);
#else
        auto until=uwvm2::imported::ros::resources::steady_now()+5000000000ll;error status;
        do{status=identify_worker(child);if(status==error::again)::poll(nullptr,0,1);assert(uwvm2::imported::ros::resources::steady_now()<until);}while(status==error::again);
        assert(status==error::success);
        namespace protocol=uwvm2::imported::ros::scene::model_worker;
        marker(child,protocol::ready);
        if(stage=="attached"){assert(child.enable_weights()==error::success);marker(child,protocol::attached);}
#endif
        report_and_wait();
    }
    ::close(channel[1]);auto owner_identity=static_cast<int>(::syscall(SYS_pidfd_open,owner,0));assert(owner_identity>=0);
    pollfd available{channel[0],POLLIN,0};assert(::poll(&available,1,15000)==1&&(available.revents&POLLIN));
    char ready{};iovec vector{&ready,1};alignas(cmsghdr)std::array<std::byte,CMSG_SPACE(2*sizeof(int))> control{};
    msghdr message{};message.msg_iov=&vector;message.msg_iovlen=1;message.msg_control=control.data();message.msg_controllen=control.size();
    assert(::recvmsg(channel[0],&message,MSG_CMSG_CLOEXEC)==1&&ready=='r'&&!(message.msg_flags&(MSG_TRUNC|MSG_CTRUNC)));
    auto* header=CMSG_FIRSTHDR(&message);assert(header&&header->cmsg_level==SOL_SOCKET&&header->cmsg_type==SCM_RIGHTS&&header->cmsg_len==CMSG_LEN(2*sizeof(int))&&!CMSG_NXTHDR(&message,header));
    std::array<int,2> identities{};std::memcpy(identities.data(),CMSG_DATA(header),sizeof(identities));
    for(auto fd:identities)assert(!exited(fd,0));
    kill_identity(owner_identity);assert(exited(owner_identity,3000));
    bool root_gone=exited(identities[0],3000),worker_gone=exited(identities[1],3000);
    // Only rescue after both independent observations have been recorded.
    for(auto fd:identities){kill_identity(fd);assert(exited(fd,3000));::close(fd);}
    ::close(owner_identity);::close(channel[0]);int status{};
    assert(::waitpid(owner,&status,0)==owner&&WIFSIGNALED(status)&&WTERMSIG(status)==SIGKILL);
    for(;;){auto child=::waitpid(-1,&status,0);if(child<0){assert(errno==ECHILD);break;}}
    std::printf("owner-death stage=%s root-exited=%u worker-exited=%u before-rescue\n",argv[argc-1],unsigned(root_gone),unsigned(worker_gone));
    assert(root_gone&&worker_gone);
}
