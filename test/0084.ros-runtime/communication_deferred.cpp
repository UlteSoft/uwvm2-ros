/** @file @brief Queue acceptance is not a native action-state transition. */
#define main synchronous_action_fixture_entry
#include "communication_actions.cpp"
#undef main
struct deferred_backend:backend
{
    using backend::submit;using backend::respond;using backend::cancel;
    c::operation_token previous;
    c::native_result enqueue(c::operation_token const& token)noexcept
    {previous=token;++calls;return {c::error::ok,c::effect::locally_accepted,token.request,71,true};}
    c::native_result submit(c::handle,a::uuid const&,std::span<std::uint8_t const>,std::uint64_t,c::operation_token const& token)noexcept override{return enqueue(token);}
    c::native_result respond(a::response const&,a::uuid const&,a::state,std::uint64_t,c::operation_token const& token)noexcept override{return enqueue(token);}
    c::native_result cancel(c::handle,c::handle,a::uuid const&,std::uint64_t,c::operation_token const& token)noexcept override{return enqueue(token);}
};
c::operation_token origin(c::handle resource,c::permission action,c::permission secondary=c::permission::unconfigured)
{
    static int caller;c::operation_token result;result.caller_instance=&caller;result.caller_generation=1;
    result.authority={1,1,1};result.capability=11;result.resource=resource;result.family=c::operation_family::action;result.action=action;result.secondary=secondary;
    result.retained_origins=uwvm2::utils::container::make_shared<int>(1);result.count=secondary==c::permission::unconfigured?6:7;
    result.origins[0]={result.authority,11,resource,action};result.origins[1]={{1,2,1},12,21,c::permission::node_open};
    result.origins[2]={{1,3,1},13,22,c::permission::context_open};
    for(std::size_t i=3;i<6;++i)result.origins[i]={{1,i+1,1},11+i,0,c::permission::type_describe};
    if(result.count==7)result.origins[6]={result.authority,11,resource,secondary};
    return result;
}
static void contradictory_native_results()
{
    struct faulty:backend
    {
        c::native_result answer;
        c::native_result respond(a::response const&,a::uuid const&,a::state,std::uint64_t)noexcept override
        {++calls;return answer;}
    };
    std::array<c::native_result,7> invalid{{
        {static_cast<c::error>(999),c::effect::none},{c::error::ok,static_cast<c::effect>(255)},
        {c::error::denied,c::effect::locally_accepted},{c::error::io,c::effect::observed_remote},
        {c::error::ok,c::effect::none},{c::error::ok,c::effect::locally_accepted,777,71,false},
        {c::error::ok,c::effect::locally_accepted,0,0,true}}};
    for(auto answer:invalid)
    {
        auto native=uwvm2::utils::container::make_shared<faulty>();native->answer=answer;a::endpoint endpoint;
        assert(endpoint.configure(plan(a::role::server),703,native)==c::error::ok);
        a::uuid uuid{1};std::array<std::uint8_t,1> body{7};c::handle goal{};capture captured{true,{},{}};
        assert(endpoint.offer(uuid,22,body,goal)==c::error::ok&&endpoint.take({1,1,1},&captured,take)==c::error::ok);
        c::receipt result,retry;auto deadline=c::now()+1000000000;
        a::response response{22,goal,a::transition::accept,{},false};
        assert(endpoint.respond({1,1,1},{703,1},response,deadline,result)==c::error::ok&&
            result.status==c::error::io&&result.observed==c::effect::unknown&&result.request!=777);
        a::goal_info info;assert(endpoint.describe({1,1,1},goal,info)==c::error::ok&&info.observed==a::state::pending&&info.uncertain);
        assert(endpoint.respond({1,1,1},{703,1},response,deadline,retry)==c::error::ok&&retry.request==result.request&&native->calls==1);
        assert(endpoint.respond({1,1,1},{703,2},response,deadline,retry)==c::error::busy&&native->calls==1&&endpoint.cleanup_debt());
    }
}
int main()
{
    assert(synchronous_action_fixture_entry()==0);
    contradictory_native_results();
    auto server=uwvm2::utils::container::make_shared<deferred_backend>();a::endpoint endpoint;
    assert(endpoint.configure(plan(a::role::server),701,server)==c::error::ok);
    a::uuid uuid{1};std::array<std::uint8_t,1> body{7};c::handle goal{};
    assert(endpoint.offer(uuid,22,body,goal)==c::error::ok);capture captured{true,{},{}};
    assert(endpoint.take({1,1,1},&captured,take)==c::error::ok);
    auto token=origin(701,c::permission::action_respond,c::permission::action_accept);c::receipt receipt,retry;
    auto deadline=c::now()+1000000000;a::response response{22,goal,a::transition::accept,{},false};
    auto missing=token;missing.origins[2]={};assert(endpoint.respond({1,1,1},{701,1},response,deadline,receipt,&missing)==c::error::denied&&!server->calls);
    auto wrong=token;wrong.secondary=c::permission::action_reject;wrong.origins[6].action=wrong.secondary;
    assert(endpoint.respond({1,1,1},{701,1},response,deadline,receipt,&wrong)==c::error::denied&&!server->calls);
    assert(endpoint.respond({1,1,1},{701,1},response,deadline,receipt,&token)==c::error::ok&&receipt.provider_generation==71);
    auto first=receipt.request;assert(first!=22&&first!=goal&&server->previous.request==first);
    a::goal_info info;assert(endpoint.describe({1,1,1},goal,info)==c::error::ok&&info.observed==a::state::pending);
    assert(endpoint.respond({1,1,1},{701,1},response,deadline,retry,&token)==c::error::ok&&retry.request==first&&server->calls==1);
    assert(endpoint.respond({1,1,1},{701,2},response,deadline,retry,&token)==c::error::busy);
    assert(endpoint.cleanup_debt());
    assert(endpoint.settle_operation(first,{c::error::denied,c::effect::none,first,72,false})==c::error::denied);
    assert(endpoint.settle_operation(first,{c::error::denied,c::effect::none,first+1,71,false})==c::error::denied);
    assert(endpoint.settle_operation(first,{static_cast<c::error>(999),c::effect::none,first,71,false})==c::error::invalid&&endpoint.cleanup_debt());
    assert(endpoint.settle_operation(first,{c::error::denied,c::effect::none,first,71,false})==c::error::ok);
    assert(endpoint.receipt_for({1,1,1},first,retry)==c::error::ok&&retry.status==c::error::denied&&retry.observed==c::effect::none);
    assert(endpoint.receipt_for({1,2,1},first,retry)==c::error::denied);
    assert(endpoint.respond({1,1,1},{701,1},response,deadline,retry,&token)==c::error::ok&&retry.status==c::error::denied&&server->calls==1);
    assert(endpoint.describe({1,1,1},goal,info)==c::error::ok&&info.observed==a::state::pending&&!info.uncertain);
    assert(endpoint.respond({1,1,1},{701,2},response,deadline,receipt,&token)==c::error::ok&&receipt.request!=first);
    assert(endpoint.settle_operation(receipt.request,{c::error::ok,c::effect::locally_accepted,receipt.request,71,false})==c::error::ok);
    assert(endpoint.describe({1,1,1},goal,info)==c::error::ok&&info.observed==a::state::accepted);
    assert(endpoint.settle_operation(receipt.request,{c::error::ok,c::effect::locally_accepted})==c::error::stale);
    response.change=a::transition::execute;token.secondary=c::permission::action_execute;token.origins[6].action=token.secondary;
    assert(endpoint.respond({1,1,1},{701,3},response,deadline,receipt,&token)==c::error::ok);
    assert(endpoint.describe({1,1,1},goal,info)==c::error::ok&&info.observed==a::state::accepted);
    assert(endpoint.settle_operation(receipt.request,{c::error::io,c::effect::unknown,receipt.request,71,false})==c::error::ok);
    assert(endpoint.receipt_for({1,1,1},receipt.request,retry)==c::error::ok&&retry.status==c::error::io&&retry.observed==c::effect::unknown);
    assert(endpoint.describe({1,1,1},goal,info)==c::error::ok&&info.observed==a::state::accepted&&info.uncertain);
    assert(endpoint.close({1,1,1})==c::error::busy&&endpoint.cleanup_debt());
    auto client=uwvm2::utils::container::make_shared<deferred_backend>();a::endpoint submitting;
    assert(submitting.configure(plan(a::role::client),702,client)==c::error::ok);
    token=origin(702,c::permission::action_submit);
    assert(submitting.submit({1,1,1},{702,1},uuid,body,deadline,receipt,&token)==c::error::ok);
    goal=receipt.request;assert(goal!=first&&goal!=22&&submitting.cleanup_debt());
    assert(submitting.observe(goal,a::event_kind::acceptance,a::state::accepted)==c::error::ok);
    assert(submitting.settle_operation(goal,{c::error::ok,c::effect::locally_accepted,goal,71,false})==c::error::ok);
    token=origin(702,c::permission::action_cancel);
    assert(submitting.cancel({1,1,1},{702,2},goal,deadline,receipt,&token)==c::error::ok&&receipt.request!=goal);
    first=receipt.request;
    assert(submitting.settle_operation(first,{c::error::timeout,c::effect::none,first,71,false})==c::error::ok);
    assert(submitting.describe({1,1,1},goal,info)==c::error::ok&&!info.cancel_request&&!info.uncertain);
    assert(submitting.cancel({1,1,1},{702,3},goal,deadline,receipt,&token)==c::error::ok&&receipt.request!=first);
    assert(submitting.settle_operation(receipt.request,{c::error::io,c::effect::unknown,receipt.request,71,false})==c::error::ok);
    assert(submitting.close({1,1,1})==c::error::busy);
    std::puts("PASS deferred communication: globally distinct receipt/goal IDs, complete context/schema/transition origins, exact queue retry, state unchanged until native completion, known no-effect retry and retained unknown");
}
