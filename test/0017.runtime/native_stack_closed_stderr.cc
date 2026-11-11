// Actual guarded pthread exhaustion and setup failure with unusable stderr.
#include <uwvm2/runtime/lib/uwvm_runtime_native_stack_guard.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <cstdio>
#include <cstring>
#if !defined(__linux__)
#error "Requires actual Linux guarded pthread, SIGSEGV and SIGPIPE semantics."
#endif
namespace native=uwvm2::runtime::lib::native_stack;
using recurse_type=unsigned(*)(unsigned);
static unsigned recurse(unsigned);
static recurse_type volatile next_call=recurse;
[[gnu::noinline]] static unsigned recurse(unsigned value)
{
    volatile unsigned char space[512];space[value%sizeof(space)]=static_cast<unsigned char>(value);
    return next_call(value+1u)+space[value%sizeof(space)];
}
static void* exhaust(void*)
{
    native::scope guard;if(!guard.ready())::_exit(90);
    static_cast<void>(recurse(1));::_exit(91);
}
static int run(unsigned mode,bool setup)
{
    int channel[2];if(::pipe(channel))return 1;
    if(mode==1u){::close(channel[0]);channel[0]=-1;}
    auto child=::fork();if(child<0)return 2;
    if(!child)
    {
        ::signal(SIGPIPE,SIG_DFL);
        if(channel[0]>=0)::close(channel[0]);::dup2(channel[1],2);::close(channel[1]);
        if(mode==2u)::close(2);
        if(setup)native::setup_failed();
        pthread_attr_t attributes{};pthread_t worker{};
        if(::pthread_attr_init(&attributes)||::pthread_attr_setstacksize(&attributes,1024u*1024u)||::pthread_attr_setguardsize(&attributes,4096u)||
           ::pthread_create(&worker,&attributes,exhaust,nullptr))::_exit(92);
        ::pthread_attr_destroy(&attributes);::pthread_join(worker,nullptr);::_exit(93);
    }
    ::close(channel[1]);int status{};if(::waitpid(child,&status,0)!=child)return 3;
    char bytes[1024]{};ssize_t size{};
    if(channel[0]>=0){size=::read(channel[0],bytes,sizeof(bytes)-1u);::close(channel[0]);}
    unsigned expected=setup?126u:127u;
    if(!WIFEXITED(status)||WEXITSTATUS(status)!=expected)return 4;
    if(mode==0u && (size<=0 || !std::strstr(bytes,setup?"cannot establish native stack fault handling":"call stack exhausted (native stack guard)")))return 5;
    return 0;
}
int main()
{
    rlimit limit{};::setrlimit(RLIMIT_CORE,&limit);
    for(unsigned mode{};mode!=3u;++mode)for(bool setup:{false,true})if(auto error=run(mode,setup))
    {std::printf("FAIL mode %u setup %u error %d\n",mode,setup,error);return error;}
    std::puts("PASS 6/6 actual stack/setup paths with live, closed-pipe and closed-fd stderr; guard127/setup126 preserved");
}
