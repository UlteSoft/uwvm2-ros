/** @file @brief Unknown namespace cleanup fences model startup and replacement. */
#define UWVM_MODEL_WORKER_NO_MAIN
#include "model_worker.cpp"
#include "worker_identity_fault_fixture.h"
int main(int argc,char** argv)
{
    assert(argc==10);assert(!::prctl(PR_SET_CHILD_SUBREAPER,1,0,0,0));
    uwvm2::utils::container::std_vector<x::descriptor> descriptors;
    auto manifest=worker_manifest(argv+1,descriptors);auto plan=model_plan();
    manifest.model=plan.model;manifest.task=plan.task;
    auto permissions=uwvm2::utils::container::make_shared<s::authority>();
    s::buffers::registry buffers;s::model_dispatch dispatcher{permissions,buffers};
    assert(dispatcher.bind("brain",plan)==error::success);
    std::shared_ptr<s::models::provider_lifetime> owner;
    inject_identity.store(true);
    auto first=s::model_worker::service::prepare(dispatcher,"brain",1,manifest,owner);
    drain_children();assert(injected.load()&&!owner&&first!=error::success);
    // Test-only cleanup cannot grant production the missing cleanup proof.
    for(unsigned retry{};retry<3;++retry)
        assert(s::model_worker::service::prepare(dispatcher,"brain",retry+2,manifest,owner)==error::denied&&!owner);
    assert(dispatcher.attach("brain",5)==error::denied);
    std::puts("PASS model identity refusal: startup not attached, unknown binding fenced, replacement denied");
}
