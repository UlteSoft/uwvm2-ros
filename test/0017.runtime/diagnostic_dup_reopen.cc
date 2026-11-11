// Actual Linux duplication and descriptor exhaustion, without throwing mocks.
#define UWVM
#include <uwvm2/uwvm_predefine/io/impl.h>
#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#if !defined(__linux__)
#error "Requires actual Linux descriptor and pending-signal semantics."
#endif
namespace io=uwvm2::uwvm::io;
namespace crash=uwvm2::runtime::diagnostic::crash;
static_assert(std::is_same_v<decltype(fast_io::u8out()),fast_io::u8native_io_observer>);
static_assert(std::is_same_v<decltype(fast_io::u8err()),fast_io::u8native_io_observer>);
static_assert(noexcept(io::diagnostic_file{fast_io::io_dup,fast_io::u8out()}));
static_assert(noexcept(io::diagnostic_file{fast_io::io_dup,fast_io::u8err()}));
static_assert(noexcept(std::declval<io::diagnostic_file&>().reopen(fast_io::io_dup,fast_io::u8out())));
static_assert(noexcept(std::declval<io::diagnostic_file&>().reopen(fast_io::io_dup,fast_io::u8err())));
static_assert(noexcept(io::diagnostic_file{fast_io::io_dup,std::declval<fast_io::u8native_io_observer const&>()}));
static_assert(noexcept(std::declval<io::diagnostic_file&>().reopen(
    fast_io::io_dup,std::declval<fast_io::u8native_io_observer const&>())));

static int child(unsigned mode,bool compiler_log,bool stderr_source)
{
    int null=::open("/dev/null",O_WRONLY|O_CLOEXEC);
    if(null<0 || ::dup2(null,1)<0 || ::dup2(null,2)<0)return 90;
    ::close(null);
    int const source=stderr_source?2:1;
    if(mode==1u)
    {
        if(::close(source)!=0)return 91;
        errno=0;
        if(::fcntl(source,F_GETFD)!=-1 || errno!=EBADF)return 92;
    }
    else if(mode==2u)
    {
        rlimit limit{};
        if(::getrlimit(RLIMIT_NOFILE,&limit)!=0 || limit.rlim_cur<8u)return 93;
        limit.rlim_cur=8u;
        if(::setrlimit(RLIMIT_NOFILE,&limit)!=0)return 94;
        while(::open("/dev/null",O_WRONLY|O_CLOEXEC)>=0){}
        if(errno!=EMFILE)return 95;
        if(::fcntl(source,F_DUPFD_CLOEXEC,3)!=-1 || errno!=EMFILE)return 96;
    }
    int const source_flags=mode==1u ? -1 : ::fcntl(source,F_GETFL);
    sigset_t pipe_signal{},before{},after{},pending{};
    ::sigemptyset(&pipe_signal);::sigaddset(&pipe_signal,SIGPIPE);
    if(::sigprocmask(SIG_BLOCK,&pipe_signal,nullptr)!=0 || ::raise(SIGPIPE)!=0 ||
       ::sigprocmask(SIG_SETMASK,nullptr,&before)!=0)return 97;
    auto const observer=stderr_source?fast_io::u8err():fast_io::u8out();
    errno=E2BIG;
    io::diagnostic_file direct{fast_io::io_dup,observer};
    if(errno!=E2BIG || (direct.native_handle()>=0)!=(mode==0u))return 98;
    auto& target=compiler_log?io::u8runtime_log_output:io::u8log_output;
    errno=E2BIG;
    target.reopen(fast_io::io_dup,observer);
    if(errno!=E2BIG || (target.handle.native_handle()>=0)!=(mode==0u))return 99;
    if(mode==0u)
    {
        int const owned=target.handle.native_handle();
        if(owned==source || owned<3 || (::fcntl(owned,F_GETFD)&FD_CLOEXEC)==0 ||
           (::fcntl(direct.native_handle(),F_GETFD)&FD_CLOEXEC)==0)return 100;
    }
    if(mode!=1u && ::fcntl(source,F_GETFL)!=source_flags)return 101;
    if(::sigprocmask(SIG_SETMASK,nullptr,&after)!=0 || ::sigpending(&pending)!=0 ||
       ::sigismember(&pending,SIGPIPE)!=1)return 102;
    for(int number=1;number<NSIG;++number)
        if(::sigismember(&before,number)!=::sigismember(&after,number))return 103;
    errno=E2BIG;
    fast_io::io::perrln(target,u8"best-effort explicit standard log");
    if(errno!=E2BIG)return 104;
    // Keep the caller's pre-existing pending SIGPIPE blocked. The component
    // checks its preservation, then exits with the classified runtime fault.
    crash::report(crash::category::memtag,"memory tag mismatch");
    if(errno!=E2BIG)return 105;
    return static_cast<int>(crash::exit_status());
}
int main()
{
    rlimit core{};if(::setrlimit(RLIMIT_CORE,&core)!=0)return 1;
    unsigned checks{};
    for(unsigned mode{};mode!=3u;++mode)
        for(bool compiler_log:{false,true})for(bool stderr_source:{false,true})
        {
            auto pid=::fork();if(pid<0)return 2;
            if(!pid)::_exit(child(mode,compiler_log,stderr_source));
            int status{};
            if(::waitpid(pid,&status,0)!=pid || !WIFEXITED(status) || WEXITSTATUS(status)!=122)
            {
                std::printf("FAIL mode=%u compiler=%d source=%d wait=%d\n",mode,compiler_log,stderr_source,status);
                return 3;
            }
            ++checks;
        }
    std::printf("PASS %u/12 actual constructor/reopen paths: healthy/EBADF/EMFILE; out/err; both log streams; errno/mask/pending SIGPIPE preserved; exit122\n",checks);
}
