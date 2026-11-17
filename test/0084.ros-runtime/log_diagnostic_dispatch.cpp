/** @file @brief Actual compound log/diagnostic authority, attenuation and lifetime failures. */
#include <uwvm2/imported/ros/scene/log_diagnostic_dispatch.h>
#include <cassert>
#include <iostream>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace c=uwvm2::imported::ros::log_diagnostic_contract;
using s::error;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
template<typename F> static bytes encode(F fn)
{std::array<std::uint8_t,2048> storage{};s::wire::writer out{storage};fn(out);return {out.written().begin(),out.written().end()};}
static s::logs::plan log_plan()
{
    s::logs::plan value;value.records=2;value.record_bytes=1024;value.logger_count=1;value.severity_mask=31;
    assert(value.loggers[0].assign("robot")&&s::logs::store::required_storage(value,value.storage_bytes));return value;
}
static s::diagnostics::plan diagnostic_plan()
{s::diagnostics::plan out{123,1|8|64,3,2,0,1000000000,500000000};assert(s::diagnostics::snapshot::required_storage(out,out.storage_bytes));return out;}
struct fixture
{
    std::shared_ptr<s::authority> authority=uwvm2::utils::container::make_shared<s::authority>();
    s::log_diagnostic_dispatch dispatch{authority,77,{2,1,16384,1024,2048,2}};
    int instance{};s::caller who{&instance,1};s::handle log{},other{},diagnostic{};
    explicit fixture(bool exposed=true,bool zero_log=false,bool zero_source=false,bool observe=true)
    {
        assert(authority->expose("capability.open",s::exposure::allow)==error::success);
        if(exposed)for(auto name:{"log.emit","diagnostic.read"})assert(authority->expose(name,s::exposure::allow)==error::success);
        s::grant lg;lg.alias="log";lg.resource="private";lg.binding="private-log";lg.kind=s::purpose::log;lg.actions={"log.emit"};
        if(observe)lg.actions.emplace("resource.observe");
        if(zero_log||zero_source){lg.limits.calls=0;lg.limits.uncertain=true;}
        assert(authority->configure(lg)==error::success);lg.alias="other";lg.limits.calls.reset();lg.limits.uncertain=false;
        assert(authority->configure(lg)==error::success);
        s::grant dg;dg.alias="health";dg.resource="private-health";dg.binding="local-diagnostic";dg.kind=s::purpose::diagnostic;dg.actions={"diagnostic.read"};
        assert(authority->configure(dg)==error::success&&authority->enroll(who)==error::success);
        assert(authority->open(who,"log",log)==error::success&&authority->open(who,"other",other)==error::success&&authority->open(who,"health",diagnostic)==error::success);
        auto logs=log_plan();auto diagnostics=diagnostic_plan();
        assert(dispatch.bind_log("log",logs)==error::success&&dispatch.bind_log("other",logs)==error::success);
        assert(dispatch.bind_diagnostic("health","log",diagnostics)==error::success);
        // Bind borrows only during the call. Mutating and destroying caller
        // storage must not change the admitted logger, capacity or source scope.
        assert(logs.loggers[0].assign("unapproved"));logs.records=0;logs.severity_mask=0;
        diagnostics.scope=999;diagnostics.fields=0;diagnostics.rows=0;
        assert(dispatch.prepare()==error::success);
    }
    bytes message(std::uint64_t sequence,std::string_view text="private log %n")
    {return encode([&](auto& out){s::logs::request request{17,sequence,s::logs::severity::info,"robot",text,{},{},{},0,false,false};assert(s::logs::encode(out,request));});}
    bytes query(std::optional<std::uint64_t> cursor={},std::uint64_t origin=0,std::uint64_t limit=2,std::uint64_t scope=123)
    {return encode([&](auto& out){assert(out.u64(scope)&&out.u64(origin?origin:log)&&out.boolean(cursor.has_value())&&(!cursor||out.u64(*cursor))&&out.u64(limit));});}
    error call(c::operation op,bytes const& input,bytes& output,std::size_t capacity=2048,s::log_diagnostic_dispatch::sink sink={},s::log_diagnostic_dispatch::cancelled cancel={})
    {
        std::size_t required{};auto copy=[&](s::wire::bytes value){output.assign(value.begin(),value.end());return true;};
        return dispatch.invoke(who,op,op==c::operation::emit?log:diagnostic,input,capacity,required,sink?sink:s::log_diagnostic_dispatch::sink{copy},cancel);
    }
    s::logs::usage usage(){s::logs::usage value;assert(dispatch.inspect_log("log",value)==error::success);return value;}
    s::diagnostics::usage diagnostic_usage(){s::diagnostics::usage value;assert(dispatch.inspect_diagnostic("health",value)==error::success);return value;}
    std::uint64_t admitted(std::string_view alias)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(authority->inspect(rows,3));for(auto const& row:rows)if(row.alias==alias)return row.admissions;assert(false);return 0;}
};
static void denial_and_uncharged_short_output()
{
    bytes out;fixture hidden{false};assert(hidden.call(c::operation::emit,hidden.message(1),out)==error::denied&&!hidden.usage().accepted);
    fixture zero{true,true};assert(zero.call(c::operation::emit,zero.message(1),out)==error::again&&!zero.usage().accepted);
    assert(zero.call(c::operation::emit,zero.message(1),out,47)==error::too_large&&!zero.admitted("log"));
    fixture f;assert(f.call(c::operation::emit,f.message(1),out,47)==error::too_large&&!f.admitted("log")&&!f.usage().accepted);
    assert(f.call(c::operation::read,f.query(),out,95)==error::too_large&&!f.admitted("health")&&!f.admitted("log")&&!f.diagnostic_usage().revision);
    auto original=f.who;f.who.generation=2;assert(f.call(c::operation::emit,f.message(1),out)==error::denied);f.who=original;
    auto malformed=f.message(1);malformed.push_back(0);assert(f.call(c::operation::emit,malformed,out)==error::invalid&&!f.usage().accepted);
}
static void exact_log_replay_and_isolation()
{
    fixture f;bytes out;auto request=f.message(1);
    assert(f.call(c::operation::emit,request,out,48,[](auto){return false;})==error::io&&f.usage().accepted==1);
    assert(f.call(c::operation::emit,request,out)==error::success);auto receipt=out;assert(out.size()==48&&f.usage().accepted==1);
    assert(f.call(c::operation::emit,request,out)==error::success&&out==receipt&&f.admitted("log")==3);
    assert(f.call(c::operation::emit,f.message(1,"changed"),out)==error::conflict&&f.usage().accepted==1);
    s::logs::usage other;assert(f.dispatch.inspect_log("other",other)==error::success&&!other.accepted);
    assert(f.call(c::operation::emit,f.message(2),out)==error::success);
    assert(f.call(c::operation::emit,f.message(3),out)==error::again&&f.usage().accepted==2&&f.usage().full_refusals==1);
}
static void source_rights_and_atomic_group_attenuation()
{
    bytes out;fixture no_observe{true,false,false,false};assert(no_observe.call(c::operation::read,no_observe.query(),out)==error::denied&&!no_observe.admitted("health"));
    fixture zero{true,false,true};assert(zero.call(c::operation::read,zero.query(),out)==error::again&&!zero.admitted("health")&&!zero.diagnostic_usage().revision);
    fixture f;assert(f.call(c::operation::emit,f.message(1),out)==error::success);
    auto primary=f.admitted("health"),source=f.admitted("log");
    assert(f.call(c::operation::read,f.query({},f.other),out)==error::denied&&f.admitted("health")==primary&&f.admitted("log")==source);
    assert(f.call(c::operation::read,f.query({},f.log,2,124),out)==error::denied&&!f.diagnostic_usage().revision);
    assert(f.call(c::operation::read,f.query(),out)==error::success&&f.admitted("health")==primary+1&&f.admitted("log")==source+1);
    s::wire::reader reader{out};std::uint64_t revision{},cursor{},count{},code{},value{};bool present{},available{},saturated{};
    assert(reader.u64(revision)&&revision==1&&reader.boolean(present)&&present&&reader.u64(cursor)&&reader.u64(count)&&count==2);
    assert(reader.u64(code)&&code==1&&reader.u64(value)&&value==1&&reader.boolean(available)&&available&&reader.boolean(saturated)&&!saturated);
    assert(reader.u64(code)&&code==4&&reader.u64(value)&&value==1&&reader.boolean(available)&&available&&reader.boolean(saturated)&&!saturated&&reader.done());
    assert(f.call(c::operation::emit,f.message(2),out)==error::success);
    assert(f.call(c::operation::read,f.query(cursor),out)==error::success);
    assert(f.diagnostic_usage().revision==1&&!f.diagnostic_usage().rows);
}
static void failed_page_cancel_and_reentrant_revocation()
{
    fixture f;bytes out,first;auto copy=[&](s::wire::bytes value){first.assign(value.begin(),value.end());return false;};
    assert(f.call(c::operation::read,f.query(),out,2048,copy)==error::io&&f.diagnostic_usage().pending);
    assert(f.call(c::operation::emit,f.message(1),out)==error::success);
    assert(f.call(c::operation::read,f.query(),out)==error::success&&out==first);
    fixture cancelled;bool stop{};auto sink=[&](s::wire::bytes){stop=true;return true;};auto test=[&]{return stop;};
    assert(cancelled.call(c::operation::read,cancelled.query(),out,2048,sink,test)==error::io&&cancelled.diagnostic_usage().pending);
    stop=false;assert(cancelled.call(c::operation::read,cancelled.query(),out)==error::success);
    fixture revoked;auto revoke=[&](s::wire::bytes)
    {assert(revoked.authority->revoke("log",[&](auto alias){revoked.dispatch.revoke(alias);})==error::success);return true;};
    assert(revoked.call(c::operation::read,revoked.query(),out,2048,revoke)==error::cancelled&&revoked.diagnostic_usage().retired);
    assert(revoked.call(c::operation::emit,revoked.message(1),out)==error::denied);
}
static void retained_carriers_and_fork()
{
    fixture f;s::log_diagnostic_dispatch::workspace_lease a,b,c;assert(f.dispatch.reserve_workspace(f.who,c::operation::emit,f.log,1024,a)==error::success);
    assert(f.dispatch.reserve_workspace(f.who,c::operation::emit,f.log,1024,b)==error::success);
    assert(f.dispatch.reserve_workspace(f.who,c::operation::emit,f.log,1,c)==error::again);a.reset();
    assert(f.dispatch.reserve_workspace(f.who,c::operation::emit,f.log,1,c)==error::success);
    auto pid=::fork();assert(pid>=0);if(!pid)
    {
        bytes out;bool good=f.call(c::operation::emit,f.message(1),out)==error::denied;
        a.reset();b.reset();c.reset();::_exit(good?0:1);
    }
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));
    b.reset();c.reset();f.dispatch.retire();bytes out;assert(f.call(c::operation::emit,f.message(1),out)==error::denied);
}
static void reentrant_cancellation_does_not_authorize_effects()
{
    // Deliberately revoke authority without the ordinary provider-retire hook:
    // holding a recursive admission lock must not disguise a revoked grant.
    for(unsigned stage=1;stage<=3;++stage)
    {
        fixture f;bytes out;unsigned calls{};bool copied{};
        auto check=[&]{if(++calls==stage)assert(f.authority->revoke("log",[](auto){})==error::success);return false;};
        auto sink=[&](s::wire::bytes){copied=true;return true;};
        assert(f.call(c::operation::emit,f.message(1),out,48,sink,check)==error::denied&&!copied);
        assert(f.usage().accepted==(stage==3?1u:0u));
    }
    fixture f;bytes out;unsigned calls{};bool copied{};
    auto check=[&]{if(++calls==3)assert(f.authority->revoke("log",[](auto){})==error::success);return false;};
    auto sink=[&](s::wire::bytes){copied=true;return true;};
    assert(f.call(c::operation::read,f.query(),out,2048,sink,check)==error::io&&!copied&&f.diagnostic_usage().pending);
    auto permission=uwvm2::utils::container::make_shared<s::authority>();
    for(auto limits:{s::log_diagnostic_dispatch::limits{4097,1,16384,1024,2048,2},
                     s::log_diagnostic_dispatch::limits{1,4097,16384,1024,2048,2},
                     s::log_diagnostic_dispatch::limits{1,1,UINT64_MAX,1024,2048,2}})
    {
        s::log_diagnostic_dispatch rejected{permission,1,limits};
        assert(rejected.bind_log("log",log_plan())==error::invalid);
        assert(rejected.bind_diagnostic("health","log",diagnostic_plan())==error::invalid);
        assert(rejected.prepare()==error::invalid);
    }
}
static void local_retirement_fences_delivery()
{
    // A local provider retirement does not have to mutate the authority table.
    // Reentrant cancellation may do either while reporting false.
    for(auto action:{c::operation::emit,c::operation::read})
        for(auto alias:{"log","health"})
        {
            if(action==c::operation::emit&&std::string_view{alias}=="health")continue;
            for(unsigned stage=1;stage<=4;++stage)
            {
                fixture f;bytes out;unsigned calls{};bool copied{};
                auto cancel=[&]{if(++calls==stage)f.dispatch.revoke(alias);return false;};
                auto sink=[&](s::wire::bytes){copied=true;return true;};
                auto status=f.call(action,action==c::operation::emit?f.message(1):f.query(),out,2048,sink,cancel);
                assert(status!=error::success);
                assert(copied==(stage==4));
                if(action==c::operation::emit)assert(f.usage().accepted==(stage>=3?1u:0u));
                else assert(f.diagnostic_usage().retired);
            }
        }
}
int main()
{
    denial_and_uncharged_short_output();exact_log_replay_and_isolation();source_rights_and_atomic_group_attenuation();
    failed_page_cancel_and_reentrant_revocation();retained_carriers_and_fork();reentrant_cancellation_does_not_authorize_effects();
    local_retirement_fences_delivery();
    std::cout<<"passed 7 log diagnostic dispatch security groups\n";
}
