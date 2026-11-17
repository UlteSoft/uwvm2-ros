/** @file
 * @brief Native receiver death during identity release must not kill the host.
 * @details The poll wrapper kills real pinned launcher/namespace processes
 * immediately after the real liveness check. It does not forge poll results,
 * write results or process identities. The production release therefore sees
 * an actual kernel EPIPE. SIGPIPE has its default disposition and is unblocked;
 * a process-wide ignore or a lucky supervisor signal mask cannot hide the bug.
 */
#ifdef UWVM_RELEASE_COMMUNICATION
#define UWVM_COMMUNICATION_SANDBOX_NO_MAIN
#include "communication_worker_sandbox.cpp"
#else
#include "support/legacy_process/model_sandbox.h"
#include "model_worker_fixture.h"
#include <cassert>
#include <cstdio>
using x::error;
#endif

static bool armed{},injected{};
extern "C" int __real_poll(pollfd*,nfds_t,int);
extern "C" int __wrap_poll(pollfd* descriptors,nfds_t count,int timeout)
{
    auto result=__real_poll(descriptors,count,timeout);
    if(armed&&result==0&&count==2&&timeout==0&&descriptors[0].events==POLLIN&&descriptors[1].events==POLLIN)
    {
        armed=false;injected=true;
        // The actual identity check succeeded; both descriptors are pidfds.
        // Wait until all holders of the release-read endpoint have exited.
        for(nfds_t i{};i<count;++i)
            assert(!::syscall(SYS_pidfd_send_signal,descriptors[i].fd,SIGKILL,nullptr,0)||errno==ESRCH);
        for(nfds_t i{};i<count;++i)
        {pollfd dead{descriptors[i].fd,POLLIN,0};assert(__real_poll(&dead,1,3000)==1&&(dead.revents&POLLIN));}
    }
    return result;
}
int main(int argc,char** argv)
{
#ifdef UWVM_RELEASE_COMMUNICATION
    assert(argc==9);
#else
    assert(argc==10);
#endif
    assert(::signal(SIGPIPE,SIG_DFL)!=SIG_ERR);
    sigset_t pipe;assert(!::sigemptyset(&pipe)&&!::sigaddset(&pipe,SIGPIPE)&&!::pthread_sigmask(SIG_UNBLOCK,&pipe,nullptr));
    uwvm2::utils::container::std_vector<x::descriptor> ownership;
#ifdef UWVM_RELEASE_COMMUNICATION
    auto manifest=make_manifest(argv+1,ownership);
#else
    auto manifest=worker_manifest(argv+1,ownership);
#endif
    x::process child;assert(child.prepare(manifest)==error::success&&child.launch()==error::success);
    armed=true;auto until=uwvm2::imported::ros::resources::steady_now()+5000000000ll;error status;
    do{status=child.identify();if(status==error::again)::usleep(1000);assert(uwvm2::imported::ros::resources::steady_now()<until);}while(status==error::again);
    assert(injected&&status==error::io);
    // A failed release publishes no readiness. It latches stop and retains
    // exact ownership until actual launcher/namespace reaping completes.
    assert(child.identify()==error::cancelled);int exit{};
    do{status=child.reap(exit);if(status==error::again)::usleep(1000);assert(uwvm2::imported::ros::resources::steady_now()<until);}while(status==error::again);
    assert(status==error::success&&child.cleanup_confirmed());
    assert(WIFSIGNALED(exit)&&WTERMSIG(exit)==SIGKILL);
    sigset_t pending;assert(!::sigpending(&pending)&&!::sigismember(&pending,SIGPIPE));
    struct sigaction current{};assert(!::sigaction(SIGPIPE,nullptr,&current)&&current.sa_handler==SIG_DFL);
    std::puts("PASS namespace release: real receiver death, host survives EPIPE, no SIGPIPE policy mutation, no readiness, confirmed cleanup");
}
