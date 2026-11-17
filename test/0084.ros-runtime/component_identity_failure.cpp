/** @file
 * @brief Real namespace-identity refusal must not fabricate process-tree exit.
 * @details The read wrapper only installs a real kernel pidfd refusal after
 * bubblewrap reports its blocked namespace child. Fork, launch, IPC and reap
 * execute normally. A test-owned pidfd and subreaper clean descendants independently of
 * the product's deliberately conservative observation. Cancellation controls
 * also exercise the gap between fork and pidfd publication.
 */
#define UWVM_COMPONENT_DISPATCH_TEST_MAIN previous_component_main
#include "component_dispatch.cpp"
#include <cstddef>

#include "worker_identity_fault_fixture.h"
template<typename Process> static bool cleanup_proven(Process const& child)
{
    // The reference header predates positive cleanup evidence.
    if constexpr(requires{child.cleanup_confirmed();})return child.cleanup_confirmed();
    else return true;
}
static void cancellation(char** paths,bool during_fork)
{
    uwvm2::utils::container::std_vector<x::descriptor> descriptors;
    auto manifest=worker_manifest(paths,descriptors);x::process child;
    assert(child.prepare(manifest)==error::success);
    if(during_fork)cancel_after_fork=&child;else child.request_stop();
    auto status=child.launch();cancel_after_fork=nullptr;
    auto launched=child.was_launched();child.request_stop();
    if(launched)
    {
        int exit{};auto until=c::now()+2000000000;error result;
        do{result=child.reap(exit);if(result==error::again)::usleep(1000);assert(c::now()<until);}while(result==error::again);
        assert(result==error::success);
    }
    drain_children();
    assert(status==error::cancelled&&launched==during_fork&&cleanup_proven(child));
    assert(child.input_fd()==-1&&child.output_fd()==-1);
    assert(child.identify()==error::cancelled&&child.enable_weights()==error::cancelled);
    assert(child.launch()==error::cancelled);
}
int main(int argc,char** argv)
{
    assert(argc==11);assert(!::prctl(PR_SET_CHILD_SUBREAPER,1,0,0,0));
    std::string_view mode{argv[10]};
    if(mode=="cancel-before"||mode=="cancel-fork")
    {cancellation(argv+1,mode=="cancel-fork");std::printf("PASS identity safety %s\n",argv[10]);return 0;}
    assert(mode=="identity-refusal");
    component_fixture fixture{argv+1,1};fixture.open();auto start=fixture.keyed();
    inject_identity.store(true);
    assert(fixture.call(cop::component_start,start)==error::success);
    auto until=c::now()+2000000000;
    while(!fixture.usage().failed){assert(c::now()<until);::usleep(1000);}
    fixture.component->retire();drain_children();assert(injected.load());
    auto check=[&]()
    {
        auto usage=fixture.usage();
        assert(usage.failed&&usage.first_failure==s::components::fault::startup_protocol);
        assert(!usage.exit_observed&&usage.processes==1&&usage.unknown==1&&usage.pending>=1);
        assert(!usage.ready&&!usage.messages&&!fixture.native->publisher->publications);
        assert(usage.retained_bytes==fixture.policy.retained_bytes);
        assert(fixture.call(cop::component_describe,resource(fixture.instance_id))==error::success);
        robot::response response;assert(robot::decode_response(cop::component_describe,fixture.response,{2048,2048,255,16},response));
        assert(response.execution.state=="outcome_unknown"&&!response.execution.exit_observed&&response.execution.first_failure==3);
    };
    check();
    for(unsigned retry{};retry<3;++retry)
    {
        assert(fixture.call(cop::component_start,start)==error::success);robot::response response;
        assert(robot::decode_response(cop::component_start,fixture.response,{2048,2048,255,16},response));
        assert(response.outcome.state=="outcome_unknown"&&response.outcome.effect=="unknown");check();
    }
    auto stop=fixture.keyed(true);
    for(unsigned retry{};retry<3;++retry)
    {
        assert(fixture.call(cop::component_stop,stop)==error::success);robot::response response;
        assert(robot::decode_response(cop::component_stop,fixture.response,{2048,2048,255,16},response));
        assert(response.outcome.state=="outcome_unknown"&&response.outcome.effect=="unknown"&&response.outcome.reason=="exit.unconfirmed");check();
    }
    std::puts("PASS identity safety identity-refusal: no publication, no fabricated exit, retained debt and inert retries");
}
