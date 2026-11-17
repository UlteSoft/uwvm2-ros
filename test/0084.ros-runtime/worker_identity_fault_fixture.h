/** @file
 * @brief Test-only observer of a real namespace child before injected refusal.
 * @details The production owner never receives the independent cleanup pidfd.
 * No fake read, fork, pidfd or wait result is returned to production code.
 */
#pragma once
#include <atomic>
#include <cstddef>
static std::atomic<bool> inject_identity{}, injected{};
static std::atomic<int> observation_pidfd{-1};
static ::uwvm2::imported::ros::scene::model_sandbox::process* cancel_after_fork{};
extern "C" ssize_t __real_read(int,void*,std::size_t);
extern "C" ssize_t __wrap_read(int fd,void* data,std::size_t capacity)
{
    auto result=__real_read(fd,data,capacity);
    if(result>0&&inject_identity.load()&&std::string_view{static_cast<char*>(data),static_cast<std::size_t>(result)}.find("\"child-pid\":")!=std::string_view::npos)
    {
        inject_identity.store(false);
        // Independent fixture ownership precedes the refusal. This descriptor
        // is never given to production; only the test can clean a descendant
        // whose identity the product could not acquire.
        std::string_view text{static_cast<char*>(data),static_cast<std::size_t>(result)};
        auto offset=text.find("\"child-pid\":")+12;
        while(offset<text.size()&&text[offset]==' ')++offset;
        pid_t pid{};auto parsed=std::from_chars(text.data()+offset,text.data()+text.size(),pid);
        assert(parsed.ec==std::errc{}&&pid>0);
        auto observed=static_cast<int>(::syscall(SYS_pidfd_open,pid,0));assert(observed>=0);
        observation_pidfd.store(observed);
        std::array<sock_filter,7> filter{{
            BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)),
            BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0),
            BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
            BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)),
            BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_pidfd_open,0,1),
            BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|ENOSYS),
            BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW)}};
        sock_fprog program{static_cast<unsigned short>(filter.size()),filter.data()};
        assert(!::prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0));
        assert(!::syscall(SYS_seccomp,SECCOMP_SET_MODE_FILTER,0,&program));
        injected.store(true);
    }
    return result;
}
extern "C" pid_t __real_fork();
extern "C" pid_t __wrap_fork()
{
    auto result=__real_fork();
    if(result>0&&cancel_after_fork)cancel_after_fork->request_stop();
    return result;
}
static void drain_children()
{
    auto observed=observation_pidfd.exchange(-1);
    if(observed>=0)
    {
        assert(!::syscall(SYS_pidfd_send_signal,observed,SIGKILL,nullptr,0)||errno==ESRCH);
        ::close(observed);
    }
    auto until=::uwvm2::imported::ros::resources::steady_now()+2000000000;
    for(;;)
    {
        int status{};auto child=::waitpid(-1,&status,WNOHANG);
        if(child<0){assert(errno==ECHILD);return;}
        assert(::uwvm2::imported::ros::resources::steady_now()<until);if(!child)::usleep(1000);
    }
}
