// Real Linux child regression: logging failure cannot replace a terminal status.
#include <uwvm2/runtime/diagnostic/crash_policy.h>
#include <fast_io.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/resource.h>
#if !defined(__linux__)
#error "This fixture requires real Linux process, signal, pipe and socket semantics."
#endif
namespace crash=uwvm2::runtime::diagnostic::crash;
static void retain(void* context,crash::event const& event) noexcept
{
    auto fd=*static_cast<int*>(context);
    if(::write(fd,&event,sizeof(event))!=static_cast<ssize_t>(sizeof(event)))::_exit(91);
}
static int run(unsigned mode)
{
    int event_pipe[2],log_pipe[2];
    if(::pipe(event_pipe)||::pipe(log_pipe))return 1;
    int pair[2]{-1,-1};
    if(mode==4u && ::socketpair(AF_UNIX,SOCK_STREAM,0,pair))return 2;
    auto child=::fork();if(child<0)return 3;
    if(!child)
    {
        ::close(event_pipe[0]);
        ::signal(SIGPIPE,SIG_DFL);
        sigset_t signal{};::sigemptyset(&signal);::sigaddset(&signal,SIGPIPE);
        ::sigprocmask(SIG_UNBLOCK,&signal,nullptr);
        if(mode==6u){::sigprocmask(SIG_BLOCK,&signal,nullptr);::raise(SIGPIPE);}
        if(mode==4u){::close(pair[1]);::dup2(pair[0],2);::close(pair[0]);}
        else if(mode==3u)::dup2(log_pipe[0],2);
        else ::dup2(log_pipe[1],2);
        ::close(log_pipe[0]);::close(log_pipe[1]);
        if(mode==2u)::close(2);
        if(mode==5u)
        {
            ::fcntl(2,F_SETFL,::fcntl(2,F_GETFL)|O_NONBLOCK);
            char fill[4096]{};while(::write(2,fill,sizeof(fill))>0){}
        }
        if(!crash::install_event_sink_host_api(retain,&event_pipe[1]))::_exit(92);
        if(!crash::process_policy.seal(crash::action::none))::_exit(93);
        sigset_t before{},after{},pending{};::sigprocmask(SIG_SETMASK,nullptr,&before);
        errno=E2BIG;
        // This reproduces the product's formerly vulnerable pre-report write.
        ::fast_io::io::perrln(crash::stderr_sink{},u8"uwvm: [fatal] Memory tag mismatch in memory ",3u);
        if(errno!=E2BIG)::_exit(94);
        crash::memory_detail detail{3u,0x7000000000000020ull,32u,7u,9u};
        bool const partial_hardware=mode==9u || mode==10u;
        if(partial_hardware)
        {detail.pointer=0u;detail.pointer_available=false;detail.pointer_tag_available=mode==10u;
         detail.tag_mask=15u;detail.allocation_tag_is_snapshot=true;}
        bool asynchronous=mode==7u || mode==8u;
        bool unknown=mode==7u;
        crash::report(crash::category::memtag,"memory tag mismatch",2u,
            unknown?UINT64_MAX:11u,unknown?UINT64_MAX:12u,unknown?UINT64_MAX:13u,
            unknown?nullptr:&detail,asynchronous);
        crash::event snapshot{};
        if(!crash::snapshot_host_api(snapshot) || snapshot.status!=122u || snapshot.backend!=2u ||
           snapshot.memory_available==unknown || snapshot.asynchronous!=asynchronous)::_exit(95);
        crash::report(crash::category::execution,"secondary fault");
        if(crash::exit_status()!=122u || !crash::snapshot_host_api(snapshot) || snapshot.status!=122u)::_exit(96);
        auto private_json=crash::format_event_json(snapshot,true);
        auto public_json=crash::format_event_json(snapshot);
        if(!std::strstr(public_json.bytes,"\"memory\":null") ||
           !std::strstr(public_json.bytes,"\"module\":null") ||
           !std::strstr(public_json.bytes,"\"function\":null") ||
           !std::strstr(public_json.bytes,"\"wasm_offset\":null") ||
           (!unknown && mode!=9u && !std::strstr(private_json.bytes,"\"pointer_tag\":7")) ||
           (partial_hardware && (!std::strstr(private_json.bytes,"\"pointer\":null") ||
            !std::strstr(private_json.bytes,"\"tag_mask\":15") ||
            !std::strstr(private_json.bytes,"\"allocation_tag_is_snapshot\":true"))) ||
           (mode==9u && !std::strstr(private_json.bytes,"\"pointer_tag\":null")))::_exit(97);
        ::sigprocmask(SIG_SETMASK,nullptr,&after);::sigpending(&pending);
        if(::sigismember(&before,SIGPIPE)!=::sigismember(&after,SIGPIPE) ||
           (::sigismember(&pending,SIGPIPE)==1)!=(mode==6u) || errno!=E2BIG)::_exit(98);
        ::_exit(static_cast<int>(crash::exit_status()));
    }
    ::close(event_pipe[1]);::close(log_pipe[1]);
    if(mode==1u || mode==4u){::close(log_pipe[0]);log_pipe[0]=-1;}
    if(mode==4u){::close(pair[0]);::close(pair[1]);}
    crash::event received{};auto bytes=::read(event_pipe[0],&received,sizeof(received));
    int status{};if(::waitpid(child,&status,0)!=child)return 4;
    char extra{};auto trailing=::read(event_pipe[0],&extra,1);::close(event_pipe[0]);
    bool text_ok=true;
    if(log_pipe[0]>=0)
    {
        char text[16384]{};auto n=::read(log_pipe[0],text,sizeof(text)-1u);::close(log_pipe[0]);
        if(mode==0u || mode>=6u)
            text_ok=n>0 && std::strstr(text,"uwvm: [crash] memory-protection\n  Reason:") &&
                std::strstr(text,"\n  Exit status: 122\n") && !std::strstr(text,"\"schema\"");
        if(mode==9u || mode==10u)
            text_ok=text_ok && std::strstr(text,"\n  Pointer: unavailable\n") &&
                std::strstr(text,"\n  Tag evidence mask: 15\n") &&
                std::strstr(text,"post-fault hardware snapshot");
    }
    if(!WIFEXITED(status)||WEXITSTATUS(status)!=122 || bytes!=sizeof(received) || trailing!=0 ||
       received.version!=2u || received.status!=122u || !text_ok)
    {std::printf("FAIL case %u wait=%d bytes=%zd trailing=%zd text=%d\n",mode,status,bytes,trailing,text_ok);return 5;}
    return 0;
}
int main()
{
    rlimit limit{};::setrlimit(RLIMIT_CORE,&limit);
    for(unsigned mode{};mode!=11u;++mode)if(auto failure=run(mode))return failure;
    std::puts("PASS 11/11: real Linux delivery failures; hardware/software async and partial-tag record semantics; immutable native event and redacted public event");
}
