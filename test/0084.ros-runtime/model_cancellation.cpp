/** @file @brief Reentrant cancellation cannot bypass model or originating source authority. */
#define main model_dispatch_fixture_main
#include "model_dispatch.cpp"
#undef main
static error invoke_cancel(fixture& value,c::operation action,s::wire::bytes input,s::model_dispatch::sink deliver,
                           uwvm2::utils::container::function_ref<bool()> cancel)
{std::size_t required{};return value.dispatch.invoke(value.who,action,value.model_cap,input,8192,required,deliver,cancel);}
static s::handle finished(fixture& value)
{
    auto request=value.request(value.inference());s::model_dispatch::work worker;assert(value.dispatch.claim("brain",8,worker)==error::success);
    constexpr std::array<std::uint8_t,4> result{4,3,2,1};assert(worker.finish(s::models::state::succeeded,result)==error::success);return request;
}
static void submit_rechecks_after_last_cancellation()
{
    for(auto alias:{"brain","data"})
    {
        fixture value;unsigned checks{};bool copied{};auto copy=[&](auto){copied=true;return true;};
        auto cancel=[&]{if(++checks==2)assert(value.permissions->revoke(alias,[](auto){})==error::success);return false;};
        assert(invoke_cancel(value,c::operation::submit,value.inference(),copy,cancel)==error::denied&&!copied);
        s::models::usage usage;assert(value.dispatch.try_inspect("brain",usage)==error::success&&!usage.requests&&!usage.inputs);
    }
}
static void take_checks_both_sides_of_delivery()
{
    for(auto alias:{"brain","data"})for(unsigned when:{2u,3u})
    {
        fixture value;auto request=finished(value);unsigned checks{};bool copied{};auto copy=[&](auto){copied=true;return true;};
        auto cancel=[&]{if(++checks==when)assert(value.permissions->revoke(alias,[](auto){})==error::success);return false;};
        assert(invoke_cancel(value,c::operation::take,words({value.session,request}),copy,cancel)!=error::success);
        assert(copied==(when==3));s::models::usage usage;assert(value.dispatch.try_inspect("brain",usage)==error::success&&usage.output_items==2);
    }
}
static void metadata_and_cleanup_never_disclose_after_revocation()
{
    for(auto action:{c::operation::describe,c::operation::request_describe,c::operation::request_release,c::operation::cancel})
    {
        fixture value;auto request=finished(value);unsigned checks{};bool copied{};auto copy=[&](auto){copied=true;return true;};
        auto at=action==c::operation::describe?1u:2u;
        auto cancel=[&]{if(++checks==at)assert(value.permissions->revoke("brain",[](auto){})==error::success);return false;};
        auto input=action==c::operation::describe?words({value.session}):action==c::operation::cancel?words({value.session,request}):words({request});
        assert(invoke_cancel(value,action,input,copy,cancel)!=error::success&&!copied);
    }
    fixture value;auto request=finished(value);assert(value.permissions->revoke("data",[](auto){})==error::success);
    std::vector<std::uint8_t> output;assert(value.call(c::operation::request_describe,words({request}),output)==error::success);
    assert(value.call(c::operation::request_release,words({request}),output)==error::success); // Cleanup remains possible.
}
static void opening_rolls_back_and_fresh_receipt_checks_sources()
{
    fixture opening_value{data_grant(),model_grant(),false};assert(opening_value.dispatch.attach("brain",8)==error::success);
    unsigned checks{};bool copied{};auto copy=[&](auto){copied=true;return true;};
    auto cancel=[&]{if(++checks==2)assert(opening_value.permissions->revoke("brain",[](auto){})==error::success);return false;};
    assert(invoke_cancel(opening_value,c::operation::open,opening(),copy,cancel)!=error::success&&!copied);
    s::models::usage usage;assert(opening_value.dispatch.try_inspect("brain",usage)==error::success&&!usage.sessions);
    fixture submitted;auto exact=submitted.inference();checks=0;auto revoke=[&]{if(++checks==3)assert(submitted.permissions->revoke("data",[](auto){})==error::success);return false;};
    assert(invoke_cancel(submitted,c::operation::submit,exact,copy,revoke)!=error::success&&!copied);
    assert(submitted.dispatch.try_inspect("brain",usage)==error::success&&usage.requests==1);
    s::model_dispatch::work denied;assert(submitted.dispatch.claim("brain",8,denied)==error::cancelled&&!denied);
    std::vector<std::uint8_t> output;assert(submitted.call(c::operation::submit,exact,output)==error::success); // Exact inert recovery remains possible.
}
int main()
{
    assert(model_dispatch_fixture_main()==0);submit_rechecks_after_last_cancellation();take_checks_both_sides_of_delivery();
    metadata_and_cleanup_never_disclose_after_revocation();opening_rolls_back_and_fresh_receipt_checks_sources();
    std::puts("passed 4 model cancellation security groups");return 0;
}
