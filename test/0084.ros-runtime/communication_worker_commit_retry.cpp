/** @file @brief Inject EINTR at actual native node commitment without losing the worker. */
#define UWVM_COMMUNICATION_FACTORY_FIXTURE_ONLY
#include "communication_worker_provider.cpp"

namespace
{
    thread_local bool interrupt_commit{};
    thread_local unsigned interruptions{};
}
extern "C" int __real_poll(pollfd*,nfds_t,int);
extern "C" int __wrap_poll(pollfd* descriptors,nfds_t count,int timeout)
{
    if(interrupt_commit&&count==1&&timeout==0&&descriptors&&descriptors[0].events==POLLIN)
    {interrupt_commit=false;++interruptions;errno=EINTR;return -1;}
    return __real_poll(descriptors,count,timeout);
}
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);std::array<peer,2> peers;int at=1;
    for(auto& value:peers)value.parse(argc,argv,at);
    assert(at==argc);environment env{peers};w::paired_factory factory;
    assert(factory.configure(env.input,environment::admit,&env,5000000000ull)==c::error::ok);
    env.ready(factory);
    // Allocation is permitted before commitment, but destroying that provisional
    // object after EINTR must neither signal nor retire the retained process.
    interrupt_commit=true;
    assert(factory.create_node(env.input[0].node,env.contexts[0],env.nodes[0])==c::error::again);
    assert(interruptions==1&&!interrupt_commit&&!env.nodes[0]);
    auto const first=factory.inspect();assert(!first.stopping&&!first.link.active);
    assert(first.stages[0].phase==w::stage_phase::attached);
    assert(factory.create_node(env.input[0].node,env.contexts[0],env.nodes[0])==c::error::ok);
    assert(factory.create_node(env.input[1].node,env.contexts[1],env.nodes[1])==c::error::ok);
    assert(env.nodes[0]&&env.nodes[1]&&factory.inspect().link.active&&env.admitted==1);
    env.nodes[0].reset();env.nodes[1].reset();factory.retire();
    auto const final=factory.inspect();assert(final.drained&&!final.cleanup_confirmed);
    std::puts("PASS actual interrupted node commitment: retry retains both workers; remote cleanup debt retained");
}
