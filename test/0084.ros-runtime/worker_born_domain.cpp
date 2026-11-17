/** @file
 * @brief Real launcher death and late pidfd refusal cannot lose the owned process tree.
 * @details The read wrapper observes the real sealed launcher's startup report.
 * Independent fixture pidfds identify its blocked child through the host procfs;
 * they never enter production. A kernel refusal or real launcher kill then
 * occurs before production consumes that report. Cleanup is checked before any
 * test-only rescue. Both model/component and DDS launchers use this fixture.
 */
#ifdef UWVM_RELEASE_COMMUNICATION
#define UWVM_COMMUNICATION_SANDBOX_NO_MAIN
#include "communication_worker_sandbox.cpp"
#else
#include "model_worker_fixture.h"
#include <cstdio>
using x::error;
#endif
#include <cstring>

static x::process* target{};
static pid_t owner{};
static int observed{-1},mask_calls{};
static bool armed{},injected{},cancel_birth{};
static std::string_view selected;
extern "C" ssize_t __real_read(int,void*,std::size_t);
extern "C" int __real_sigprocmask(int,sigset_t const*,sigset_t*);

static pid_t number(std::string_view data,std::string_view key={})
{
    auto at=key.empty()?0:data.find(key);if(at==data.npos)return -1;at+=key.size();
    while(at<data.size()&&(data[at]==' '||data[at]=='\t'))++at;
    pid_t result{};auto parsed=std::from_chars(data.data()+at,data.data()+data.size(),result);
    return parsed.ec==std::errc{}?result:-1;
}
static std::size_t information(char const* path,std::span<char> data)
{
    auto fd=::open(path,O_RDONLY|O_CLOEXEC);assert(fd>=0);
    auto count=__real_read(fd,data.data(),data.size());::close(fd);assert(count>0);
    return static_cast<std::size_t>(count);
}
static int pin_descendant(int root)
{
    std::array<char,128> path{};std::array<char,4096> data{};
    assert(std::snprintf(path.data(),path.size(),"/proc/self/fdinfo/%d",root)>0);
    auto parent=number({data.data(),information(path.data(),data)},"\nPid:");assert(parent>0);
    assert(std::snprintf(path.data(),path.size(),"/proc/%d/task/%d/children",parent,parent)>0);
    auto child=number({data.data(),information(path.data(),data)});assert(child>0);
    auto identity=static_cast<int>(::syscall(SYS_pidfd_open,child,0));assert(identity>=0);
    assert(std::snprintf(path.data(),path.size(),"/proc/%d/status",child)>0);
    assert(number({data.data(),information(path.data(),data)},"\nPPid:")==parent);
    std::array<pollfd,2> live{{{root,POLLIN,0},{identity,POLLIN,0}}};assert(::poll(live.data(),live.size(),0)==0);
    return identity;
}
static void deny_syscall(unsigned call)
{
    std::array<sock_filter,7> filter{{
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
        BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)),
        BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,call,0,1),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|ENOSYS),
        BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW)}};
    sock_fprog program{static_cast<unsigned short>(filter.size()),filter.data()};
    assert(!::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0));assert(!::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,0,&program));
}
static void kill(int identity)
{assert(!::syscall(SYS_pidfd_send_signal,identity,SIGKILL,nullptr,0)||errno==ESRCH);}
static bool exited(int identity,int milliseconds)
{pollfd item{identity,POLLIN,0};auto result=::poll(&item,1,milliseconds);return result==1&&(item.revents&POLLIN);}

extern "C" int __wrap_sigprocmask(int how,sigset_t const* next,sigset_t* previous)
{
    auto result=__real_sigprocmask(how,next,previous);
    if(!result&&cancel_birth&&::getpid()==owner&&how==SIG_SETMASK&&++mask_calls==2)
    {target->request_stop();injected=true;}
    return result;
}
extern "C" ssize_t __wrap_read(int fd,void* data,std::size_t capacity)
{
    auto count=__real_read(fd,data,capacity);
    if(count>0&&armed&&std::string_view{static_cast<char*>(data),static_cast<std::size_t>(count)}.find("\"child-pid\":")!=std::string_view::npos)
    {
        armed=false;observed=pin_descendant(target->process_fd());
        if(selected=="launcher-death"){kill(target->process_fd());assert(exited(target->process_fd(),3000));}
        else{assert(selected=="late-pidfd-refusal");deny_syscall(SYS_pidfd_open);}
        injected=true;
    }
    return count;
}

int main(int argc,char** argv)
{
#ifdef UWVM_RELEASE_COMMUNICATION
    assert(argc==10);
#else
    assert(argc==11);
#endif
    assert(!::prctl(PR_SET_CHILD_SUBREAPER,1,0,0,0));owner=::getpid();selected=argv[argc-1];
    uwvm2::utils::container::std_vector<x::descriptor> ownership;
#ifdef UWVM_RELEASE_COMMUNICATION
    auto manifest=make_manifest(argv+1,ownership);
#else
    auto manifest=worker_manifest(argv+1,ownership);
#endif
    x::process child;target=&child;assert(child.prepare(manifest)==error::success);
    sigset_t previous{},after{};assert(!__real_sigprocmask(SIG_SETMASK,nullptr,&previous));
    if(selected=="clone-refusal")deny_syscall(SYS_clone3);
    cancel_birth=selected=="cancel-after-birth";
    auto launch=child.launch();
    assert(!__real_sigprocmask(SIG_SETMASK,nullptr,&after));
    for(int signal=1;signal<NSIG;++signal)assert(::sigismember(&previous,signal)==::sigismember(&after,signal));
#ifdef UWVM_RELEASE_COMMUNICATION
    auto launched=child.launch_started();
#else
    auto launched=child.was_launched();
#endif
    if(selected=="clone-refusal")
    {assert(launch!=error::success&&!launched&&child.cleanup_confirmed());std::puts("PASS denied clone has no fork fallback");return 0;}
    error identity=error::again;
    if(!cancel_birth)
    {
        assert(launch==error::success&&launched);armed=true;
        auto until=uwvm2::imported::ros::resources::steady_now()+5000000000ll;
        do{identity=child.identify();if(identity==error::again)::usleep(1000);assert(uwvm2::imported::ros::resources::steady_now()<until);}while(identity==error::again);
    }
    child.request_stop();int status{};error reaped;auto until=uwvm2::imported::ros::resources::steady_now()+3000000000ll;
    do{reaped=child.reap(status);if(reaped==error::again)::usleep(1000);assert(uwvm2::imported::ros::resources::steady_now()<until);}while(reaped==error::again);
    assert(reaped==error::success&&injected);
    bool complete=child.cleanup_confirmed();bool descendant_gone=observed<0||exited(observed,0);
    // Rescue only after recording product evidence. Reference runs are expected
    // to fail below, but must not leak the deliberately orphaned namespace.
    if(observed>=0){kill(observed);assert(exited(observed,3000));::close(observed);}
    for(;;){auto result=::waitpid(-1,&status,0);if(result<0){assert(errno==ECHILD);break;}}
    assert(complete&&descendant_gone);
    if(cancel_birth)assert(launch==error::cancelled&&launched);
    else assert((identity==error::success)==(selected=="late-pidfd-refusal"));
    assert(child.identify()==error::cancelled&&child.launch()==error::cancelled);
    std::printf("PASS %s: born domain cleanup, inert cancellation, unchanged caller signal mask\n",argv[argc-1]);
}
