/** @file @brief Exact canonical startup authority and weak paired callback lifetime. */
#define UWVM_COMMUNICATION_NODE_OPEN_FIXTURE_ONLY
#include "communication_node_open.cpp"
#include <uwvm2/imported/ros/scene/communication_worker_activation.h>
#include <uwvm2/imported/ros/scene/communication_worker_endpoint_open.h>
namespace worker=s::communication_worker;
static void exact_stage()
{
    pipeline f;c::context_plan context;c::node_plan node;
    assert(f.dispatch->describe_plan("context",context)==s::error::success&&f.dispatch->describe_plan("node",node)==s::error::success);
    c::node_stage_request original{f.who.instance,f.who.generation,f.cap,f.context_cap,f.context,context.authority,s::buffers::hash(f.carrier)};
    s::group_lease held;assert(f.dispatch->admit_node_activation(node,context,original,held)==s::error::denied&&!held);
    assert(f.open()==s::error::success&&f.dispatch->admit_node_activation(node,context,original,held)==s::error::success&&held.size()==2);held.release();
    for(unsigned field{};field<7;++field)
    {
        auto forged=original;
        switch(field){case 0:++forged.context;break;case 1:++forged.context_capability;break;case 2:++forged.node_capability;break;
            case 3:++forged.caller_generation;break;case 4:forged.caller_instance=&context;break;case 5:++forged.context_authority.grant;break;
            default:forged.request_digest[0]^=1;break;}
        assert(f.dispatch->admit_node_activation(node,context,forged,held)==s::error::denied&&!held);
    }
    auto foreign_node=node;foreign_node.name="foreign";assert(f.dispatch->admit_node_activation(foreign_node,context,original,held)==s::error::denied&&!held);
    auto foreign_context=context;++foreign_context.domain;assert(f.dispatch->admit_node_activation(node,foreign_context,original,held)==s::error::denied&&!held);
    auto child=::fork();assert(child>=0);if(!child){assert(f.dispatch->admit_node_activation(node,context,original,held)==s::error::denied);::_exit(0);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    assert(f.authority->close(f.who,f.context_cap)==s::error::success);
    assert(f.dispatch->admit_node_activation(node,context,original,held)!=s::error::success&&!held);
}
static void weak_lifetime_and_distinct_authority()
{
    pipeline a,b;assert(!a.dispatch->shares_authority(*b.dispatch)&&a.dispatch->shares_authority(*a.dispatch));
    worker::activation_admission separate{{a.dispatch,b.dispatch}};c::activation_guard held;std::array<c::activation_origin,2> origins{};
    assert(worker::activation_admission::admit(&separate,origins,held)==c::error::denied&&!held.participants[0]&&!held.participants[1]);
    auto owner=uwvm2::utils::container::make_shared<s::authority>();
    auto empty=uwvm2::utils::container::make_shared<s::communication_dispatch>(owner,19,3,s::communication_dispatch::limits{});
    std::weak_ptr<s::communication_dispatch> observed=empty;
    worker::activation_admission expired{{empty,empty}};worker::dispatch_admission endpoint{empty};empty.reset();assert(observed.expired());
    assert(worker::activation_admission::admit(&expired,origins,held)==c::error::denied);
    s::group_lease grant;c::operation_token token;creation::identity identity;
    assert(worker::dispatch_admission::admit(&endpoint,token,grant)==c::error::denied);
    assert(worker::dispatch_admission::charge(&endpoint,token,1,grant)==c::error::denied);
    assert(worker::dispatch_admission::admit_open(&endpoint,identity,grant)==c::error::denied);
    assert(worker::dispatch_admission::charge_open(&endpoint,identity,1,grant)==c::error::denied&&!grant);
}
int main(){exact_stage();weak_lifetime_and_distinct_authority();
    std::puts("PASS canonical worker activation: exact accepted startup, full plans and carrier digest, independent live authority, weak lifetime and fork denial");}
