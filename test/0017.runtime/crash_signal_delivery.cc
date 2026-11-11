// Actual POSIX delivery must never invoke an ordinary embedding callback.
#include <uwvm2/runtime/diagnostic/crash_policy.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <cstring>
#include <cstdio>
#if !defined(__linux__) || !defined(SYS_pidfd_open)
#error "Requires real Linux signal/socket/pidfd semantics; no stub pass."
#endif
namespace crash=uwvm2::runtime::diagnostic::crash;
static void unsafe_embedding_sink(void*,crash::event const&) noexcept {::_exit(99);}
static void handler(int) noexcept
{
    crash::report_signal(crash::category::memtag,"signal fixture",0u,
        UINT64_MAX,UINT64_MAX,UINT64_MAX,nullptr,true);
}
static int run(unsigned mode)
{
    int pair[2],logs[2];if(::socketpair(AF_UNIX,SOCK_SEQPACKET,0,pair)||::pipe(logs))return 1;
    int peer=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));if(peer<0)return 2;
    if(mode==2u){::close(pair[0]);pair[0]=-1;}
    auto child=::fork();if(child<0)return 3;
    if(!child)
    {
        if(pair[0]>=0)::close(pair[0]);::close(logs[0]);::dup2(logs[1],2);::close(logs[1]);
        ::signal(SIGPIPE,SIG_DFL);
        if(!crash::install_event_sink_host_api(unsafe_embedding_sink,nullptr))::_exit(90);
        if(mode && !crash::install_signal_endpoint_host_api(pair[1],peer))::_exit(91);
        struct sigaction action{};action.sa_handler=handler;::sigemptyset(&action.sa_mask);
        if(::sigaction(SIGUSR1,&action,nullptr)||::raise(SIGUSR1))::_exit(92);
        crash::event event{};
        if(!crash::snapshot_host_api(event)||event.status!=122u||!event.asynchronous||event.memory_available)::_exit(93);
        ::_exit(static_cast<int>(crash::exit_status()));
    }
    ::close(pair[1]);::close(logs[1]);::close(peer);int status{};
    if(::waitpid(child,&status,0)!=child)return 4;
    char log[4096]{};auto count=::read(logs[0],log,sizeof(log)-1u);::close(logs[0]);
    char event[4096]{};ssize_t bytes{};
    if(pair[0]>=0){bytes=::recv(pair[0],event,sizeof(event)-1u,MSG_DONTWAIT);::close(pair[0]);}
    if(!WIFEXITED(status)||WEXITSTATUS(status)!=122||count<=0||!std::strstr(log,"uwvm: [crash] memory-protection")||std::strstr(log,"schema"))return 5;
    if(mode==1u && (bytes<=0||!std::strstr(event,"\"schema\":\"uwvm.crash.v2\"")||!std::strstr(event,"\"module\":null")||!std::strstr(event,"\"asynchronous\":true")))return 6;
    if(mode==0u && bytes>0)return 7;
    return 0;
}
int main()
{
    rlimit zero{};::setrlimit(RLIMIT_CORE,&zero);
    for(unsigned mode{};mode!=3u;++mode)if(auto error=run(mode)){std::printf("FAIL mode %u error %d\n",mode,error);return error;}
    std::puts("PASS 3/3 real signal deliveries: arbitrary callback skipped; fixed diagnostic endpoint live/absent/disconnected; status 122 retained");
}
