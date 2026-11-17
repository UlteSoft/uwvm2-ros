/** @file @brief Complete named policy, closed fields and real allocation rollback. */
#include <uwvm2/imported/ros/scene/log_diagnostic_configuration.h>
#include <cassert>
#include <iostream>
namespace s=uwvm2::imported::ros::scene;
using s::error;
static thread_local std::size_t fail_after{},allocations{};
static bool fail() noexcept{++allocations;return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return fail()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
static s::logs::plan log_plan()
{
    s::logs::configuration c;
    assert(c.limit("records",2)==error::success&&c.limit("record_bytes",1024)==error::success&&c.limit("storage_bytes",8192)==error::success&&c.limit("severity_mask",31)==error::success);
    assert(c.logger("robot")==error::success&&c.loss("reject")==error::success);s::logs::plan out;assert(c.freeze(out)==error::success);return out;
}
static s::diagnostics::plan diagnostic_plan()
{
    s::diagnostics::configuration c;assert(c.source("log")==error::success);
    std::array<std::uint64_t,7> values{1,7,3,2,512,1000000000,100000000};
    for(std::size_t i{};i<values.size();++i)assert(c.limit(c.limit_names[i],values[i])==error::success);
    s::diagnostics::plan out;s::logs::name source;assert(c.freeze(out,source)==error::success&&source.view()=="log");return out;
}
static void missing_duplicate_and_closed_configuration()
{
    s::log_diagnostic_budget_configuration budget;s::log_diagnostic_dispatch::limits sentinel{17},out=sentinel;
    assert(budget.freeze(out)==error::denied&&out.logs==17);
    std::array<std::uint64_t,6> values{1,1,16384,1024,2048,2};
    assert(budget.limit("unknown",1)==error::invalid&&budget.limit("logs",4097)==error::too_large&&budget.limit("logs",UINT64_MAX)==error::invalid);
    for(std::size_t i{};i<values.size();++i){assert(budget.limit(budget.names[i],values[i])==error::success);assert(budget.limit(budget.names[i],values[i])==error::conflict);}
    assert(budget.freeze(out)==error::success&&out.logs==1&&out.workspace_calls==2&&budget.limit("logs",1)==error::conflict);
    for(std::size_t missing{};missing<6;++missing)
    {
        s::log_diagnostic_budget_configuration partial;
        for(std::size_t i{};i<values.size();++i)if(i!=missing)assert(partial.limit(partial.names[i],values[i])==error::success);
        assert(partial.freeze(out)==error::denied);
    }
}
static void log_policy_has_no_implicit_logger_or_loss()
{
    std::array<std::uint64_t,4> values{2,1024,8192,31};
    for(std::size_t missing{};missing<4;++missing)
    {
        s::logs::configuration c;s::logs::plan out;
        for(std::size_t i{};i<values.size();++i)if(i!=missing)assert(c.limit(c.limit_names[i],values[i])==error::success);
        assert(c.logger("robot")==error::success&&c.loss("reject")==error::success&&c.freeze(out)==error::denied);
    }
    s::logs::configuration c;s::logs::plan out;
    for(std::size_t i{};i<values.size();++i)assert(c.limit(c.limit_names[i],values[i])==error::success);
    assert(c.loss("silent-drop")==error::invalid&&c.loss("drop-oldest")==error::success&&c.loss("reject")==error::conflict);
    assert(c.freeze(out)==error::invalid&&c.logger("")==error::invalid&&c.logger(std::string_view{"bad\0name",8})==error::invalid);
    assert(c.logger("robot")==error::success&&c.logger("robot")==error::conflict&&c.freeze(out)==error::success&&out.full==s::logs::overflow::drop_oldest);
    assert(c.logger("later")==error::conflict&&c.limit("records",2)==error::conflict);
    s::logs::configuration too_short;
    for(std::size_t i{};i<values.size();++i)assert(too_short.limit(too_short.limit_names[i],i==1?1:values[i])==error::success);
    assert(too_short.logger("robot")==error::success&&too_short.loss("reject")==error::success&&too_short.freeze(out)==error::too_large);
}
static void diagnostic_policy_is_exact_and_finite()
{
    std::array<std::uint64_t,7> values{1,7,3,2,512,1000000000,100000000};s::diagnostics::plan out;s::logs::name source;
    for(std::size_t missing{};missing<7;++missing)
    {
        s::diagnostics::configuration c;assert(c.source("log")==error::success);
        for(std::size_t i{};i<values.size();++i)if(i!=missing)assert(c.limit(c.limit_names[i],values[i])==error::success);
        assert(c.freeze(out,source)==error::denied);
    }
    s::diagnostics::configuration c;assert(c.source("../log")==error::invalid&&c.source("log")==error::success&&c.source("other")==error::conflict);
    assert(c.limit("fields",2048)==error::too_large&&c.limit("capture_ns",UINT64_MAX)==error::invalid);
    for(std::size_t i{};i<values.size();++i)assert(c.limit(c.limit_names[i],i==2?2:values[i])==error::success);
    assert(c.freeze(out,source)==error::invalid); // three enabled counters need three retained rows
    static_cast<void>(diagnostic_plan());
}
static void allocation_failure_never_commits_partial_bindings()
{
    auto lp=log_plan();auto dp=diagnostic_plan();std::uint64_t log_bytes{},diagnostic_bytes{};
    assert(s::logs::store::required_storage(lp,log_bytes)&&s::diagnostics::snapshot::required_storage(dp,diagnostic_bytes));
    auto permissions=uwvm2::utils::container::make_shared<s::authority>();
    std::array<std::size_t,2> injected{};
    for(unsigned binding{};binding<2;++binding)
    {
        bool finished{};
        for(std::size_t at=1;at<=12&&!finished;++at)
        {
            s::log_diagnostic_dispatch dispatch{permissions,1,{1,1,log_bytes+diagnostic_bytes,1024,2048,2}};
            if(binding)assert(dispatch.bind_log("log",lp)==error::success);
            allocations=0;fail_after=at;bool threw{};error status{};
            try{status=binding?dispatch.bind_diagnostic("health","log",dp):dispatch.bind_log("log",lp);}catch(std::bad_alloc const&){threw=true;}
            fail_after=0;
            if(!threw){assert(status==error::success&&allocations<at);finished=true;continue;}
            assert(allocations==at);++injected[binding];
            if(binding){s::diagnostics::usage value;assert(dispatch.inspect_diagnostic("health",value)==error::bad_handle&&dispatch.bind_diagnostic("health","log",dp)==error::success);}
            else{s::logs::usage value;assert(dispatch.inspect_log("log",value)==error::bad_handle&&dispatch.bind_log("log",lp)==error::success);}
            if(!binding)assert(dispatch.bind_diagnostic("health","log",dp)==error::success);
            allocations=0;fail_after=1;assert(dispatch.prepare()==error::no_memory);fail_after=0;assert(allocations==1&&dispatch.prepare()==error::success);
        }
        assert(finished&&injected[binding]>=3);
    }
    std::cout<<"real binding allocation faults "<<injected[0]<<"/"<<injected[1]<<'\n';
}
int main()
{
    missing_duplicate_and_closed_configuration();log_policy_has_no_implicit_logger_or_loss();diagnostic_policy_is_exact_and_finite();
    allocation_failure_never_commits_partial_bindings();std::cout<<"passed 4 log diagnostic configuration security groups\n";
}
