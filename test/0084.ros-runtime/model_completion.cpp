/** @file @brief Authentic final output adoption after delivery loss, cleanup and authority changes. */
#define main model_dispatch_fixture_main
#include "model_dispatch.cpp"
#undef main
#include <type_traits>
static_assert(!std::is_default_constructible_v<s::model_dispatch::completion>);
static_assert(!std::is_copy_constructible_v<s::model_dispatch::completion>);
static constexpr std::array<std::uint8_t,4> completed{9,8,7,6},source_bytes{1,2,3,4};
static std::array<s::models::input_evidence,1> evidence(fixture const& value)
{return {{{value.input,s::buffers::hash(source_bytes)}}};}
static s::handle finish(fixture& value,s::models::state outcome=s::models::state::succeeded)
{
    auto request=value.request(value.inference());s::model_dispatch::work work;assert(value.dispatch.claim("brain",8,work)==error::success);
    assert(work.finish(outcome,outcome==s::models::state::succeeded?s::wire::bytes{completed}:s::wire::bytes{})==error::success);return request;
}
static error adopt(fixture& value,s::handle request,bool& called,s::wire::bytes output=completed)
{
    auto proof=evidence(value);return value.dispatch.with_completed_output("brain",request,proof,output,[&](auto const& token)
    {
        called=true;assert(token.request()==request&&token.alias()=="brain"&&token.origin().instance==value.who.instance);
        assert(token.policy().model==profile("local.identity")&&token.matches_inputs(proof));
        assert(std::equal(token.output().begin(),token.output().end(),output.begin(),output.end()));return token.revalidate();
    });
}
static void final_evidence_survives_lost_take()
{
    fixture value;auto request=finish(value);bool called{};
    assert(adopt(value,request,called)==error::success&&called);
    std::vector<std::uint8_t> output;auto failed=[](s::wire::bytes){return false;};
    assert(value.call(c::operation::take,words({value.session,request}),output,512,failed)==error::io);
    called=false;assert(adopt(value,request,called)==error::success&&called);
    assert(value.call(c::operation::take,words({value.session,request}),output)==error::success);
    called=false;assert(adopt(value,request,called)==error::success&&called);
    assert(value.call(c::operation::request_release,words({request}),output)==error::success);
    called=false;assert(adopt(value,request,called)==error::bad_handle&&!called);
}
static void exact_final_bytes_and_input_proof()
{
    fixture value;auto request=finish(value);auto proof=evidence(value);bool called{};
    auto consumer=[&](auto const&){called=true;return error::success;};
    auto corrupt=completed;corrupt[2]^=1;assert(adopt(value,request,called,corrupt)==error::conflict&&!called);
    assert(adopt(value,request,called,s::wire::bytes{completed}.first(3))==error::conflict&&!called);
    assert(adopt(value,request,called,{})==error::invalid&&!called);
    assert(value.dispatch.with_completed_output("other",request,proof,completed,consumer)==error::bad_handle&&!called);
    auto bad=proof;bad[0].buffer++;assert(value.dispatch.with_completed_output("brain",request,bad,completed,consumer)==error::conflict&&!called);
    bad=proof;bad[0].sha256[1]^=1;assert(value.dispatch.with_completed_output("brain",request,bad,completed,consumer)==error::conflict&&!called);
    assert(value.dispatch.with_completed_output("brain",request,{},completed,consumer)==error::conflict&&!called);
}
static void non_success_unknown_and_streaming_are_not_evidence()
{
    for(auto outcome:{s::models::state::failed,s::models::state::cancelled})
    {fixture value;auto request=finish(value,outcome);bool called{};assert(adopt(value,request,called)==error::conflict&&!called);}
    fixture value;auto request=value.request(value.inference());bool called{};assert(adopt(value,request,called)==error::conflict&&!called);
    s::model_dispatch::work work;assert(value.dispatch.claim("brain",8,work)==error::success&&work.emit(completed)==error::success);
    assert(adopt(value,request,called)==error::conflict&&!called);
    assert(value.dispatch.detach("brain",8)==error::success&&adopt(value,request,called)==error::conflict&&!called);
    assert(value.dispatch.reconcile("brain",request,8,s::models::state::succeeded)==error::success);
    assert(adopt(value,request,called)==error::conflict&&!called);
    fixture streaming;request=streaming.request(streaming.inference());s::model_dispatch::work streamed;
    assert(streaming.dispatch.claim("brain",8,streamed)==error::success&&streamed.emit(completed)==error::success&&streamed.finish(s::models::state::succeeded)==error::success);
    assert(adopt(streaming,request,called)==error::conflict&&!called);
}
static void terminal_cancellation_and_current_authority()
{
    fixture value;auto request=finish(value);std::vector<std::uint8_t> output;bool called{};
    assert(value.call(c::operation::cancel,words({value.session,request}),output)==error::success);
    assert(adopt(value,request,called)==error::success&&called); // Historical success cannot be undone by a late cancellation.
    assert(value.dispatch.close("brain",value.session)==error::success);called=false;
    assert(adopt(value,request,called)==error::conflict&&!called);
    for(auto alias:{"brain","data"})
    {
        fixture revoked;request=finish(revoked);called=false;
        assert(revoked.permissions->revoke(alias,[&](auto name){if(name=="brain")revoked.dispatch.revoke(name);else revoked.buffers.revoke(name);})==error::success);
        assert(adopt(revoked,request,called)==error::denied&&!called);
    }
    fixture closed;request=finish(closed);called=false;assert(closed.permissions->close(closed.who,closed.data_cap)==error::success);
    assert(adopt(closed,request,called)==error::bad_handle&&!called);
    fixture retired;request=finish(retired);called=false;retired.permissions->retire_member(retired.who);
    assert(adopt(retired,request,called)==error::denied&&!called);
}
static void reentrant_revocation_and_fork_fencing()
{
    fixture value;auto request=finish(value);auto proof=evidence(value);
    assert(value.dispatch.with_completed_output("brain",request,proof,completed,[&](auto const& token)
    {
        assert(value.permissions->revoke("data",[&](auto alias){value.buffers.revoke(alias);})==error::success);
        return token.revalidate();
    })==error::denied);
    fixture parent;request=finish(parent);proof=evidence(parent);
    assert(parent.dispatch.with_completed_output("brain",request,proof,completed,[&](auto const& token)
    {
        auto pid=::fork();assert(pid>=0);
        if(!pid)::_exit(token.revalidate()==error::denied?0:4);
        int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0);return token.revalidate();
    })==error::success);
}
int main()
{
    assert(model_dispatch_fixture_main()==0);
    final_evidence_survives_lost_take();exact_final_bytes_and_input_proof();non_success_unknown_and_streaming_are_not_evidence();
    terminal_cancellation_and_current_authority();reentrant_revocation_and_fork_fencing();
    std::puts("passed 5 model completion adoption security groups");
}
