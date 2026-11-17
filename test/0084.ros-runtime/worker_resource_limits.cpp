/** @file
 * @brief A failed limit setter/readback must stop the real worker before exec.
 * @details Link wrappers only inject at the owned namespace-init stage. The
 * kernel still performs all successful setters/readbacks. Original launcher
 * images run unchanged. A refused getter or corrupted soft/hard observation
 * must yield the fixed pre-exec exit code, without a readiness report or process
 * descendant. The older implementation is a negative control: it omits this
 * readback and wrongly reaches launcher readiness. This is not a vendor test.
 */
#ifdef UWVM_RELEASE_COMMUNICATION
#define UWVM_COMMUNICATION_SANDBOX_NO_MAIN
#include "communication_worker_sandbox.cpp"
#else
#include "model_worker_fixture.h"
#include <cstdio>
using x::error;
#endif
static pid_t preparing_owner{};
static std::string_view mode;
extern "C" int __real_setrlimit(__rlimit_resource_t,rlimit const*);
extern "C" int __real_getrlimit(__rlimit_resource_t,rlimit*);
static bool injecting(int resource)noexcept
{return ::getpid()!=preparing_owner&&resource==RLIMIT_NOFILE;}
extern "C" int __wrap_setrlimit(__rlimit_resource_t resource,rlimit const* value)
{
    if(injecting(resource)&&mode=="setter-denied"){errno=EPERM;return -1;}
    return __real_setrlimit(resource,value);
}
extern "C" int __wrap_getrlimit(__rlimit_resource_t resource,rlimit* value)
{
    if(injecting(resource)&&mode=="readback-denied"){errno=EPERM;return -1;}
    auto result=__real_getrlimit(resource,value);
    if(!result&&injecting(resource))
    {
        if(mode=="soft-mismatch")++value->rlim_cur;
        if(mode=="hard-mismatch")value->rlim_max=RLIM_INFINITY;
    }
    return result;
}
[[gnu::noinline]] static error identify_worker(x::process& worker)noexcept
{return worker.identify();}
int main(int argc,char** argv)
{
#ifdef UWVM_RELEASE_COMMUNICATION
    assert(argc==10);
#else
    assert(argc==11);
#endif
    preparing_owner=::getpid();mode=argv[argc-1];
    assert(mode=="success"||mode=="setter-denied"||mode=="readback-denied"||mode=="soft-mismatch"||mode=="hard-mismatch");
    assert(!::prctl(PR_SET_CHILD_SUBREAPER,1,0,0,0));
    rlimit before{};assert(!__real_getrlimit(RLIMIT_NOFILE,&before));
    uwvm2::utils::container::std_vector<x::descriptor> ownership;
#ifdef UWVM_RELEASE_COMMUNICATION
    auto manifest=make_manifest(argv+1,ownership);
#else
    auto manifest=worker_manifest(argv+1,ownership);
#endif
    x::process child;assert(child.prepare(manifest)==error::success);
    assert(child.launch()==error::success);
    auto until=uwvm2::imported::ros::resources::steady_now()+5000000000ll;
    error identity;
    do{identity=identify_worker(child);if(identity==error::again)::poll(nullptr,0,1);assert(uwvm2::imported::ros::resources::steady_now()<until);}while(identity==error::again);
    // Also clean the older negative control before reporting its false success.
    if(identity==error::success)child.request_stop();
    int status{};until=uwvm2::imported::ros::resources::steady_now()+5000000000ll;
    error reaped;
    do{reaped=child.reap(status);if(reaped==error::again)::poll(nullptr,0,1);assert(uwvm2::imported::ros::resources::steady_now()<until);}while(reaped==error::again);
    assert(reaped==error::success&&child.cleanup_confirmed());
    if(mode!="success")
        assert(identity!=error::success&&WIFEXITED(status)&&WEXITSTATUS(status)==126);
    else assert(identity==error::success);
    int abandoned{};assert(::waitpid(-1,&abandoned,WNOHANG)==-1&&errno==ECHILD);
    rlimit after{};assert(!__real_getrlimit(RLIMIT_NOFILE,&after));
    assert(before.rlim_cur==after.rlim_cur&&before.rlim_max==after.rlim_max);
    std::printf("PASS %s: exact worker ceilings or refusal before sealed exec; owner unchanged\n",argv[argc-1]);
}
