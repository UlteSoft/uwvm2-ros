/** @file @brief Exact private-log deployment, compound diagnostic rights and detached inspection. */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <iostream>
namespace s=uwvm2::imported::ros::scene;
namespace c=uwvm2::imported::ros::log_diagnostic_contract;
using s::error;using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
template<typename Function> static bytes encode(Function fn)
{std::array<std::uint8_t,2048> data{};s::wire::writer writer{data};fn(writer);return {writer.written().begin(),writer.written().end()};}
static void environment(s::deployment& value,std::string_view name="robot",std::string_view module="main")
{
    assert(value.create(uwvm2::utils::container::std_string(name))==error::success&&value.bind(name,uwvm2::utils::container::std_string(module))==error::success);
    for(auto action:{"capability.open","log.emit","diagnostic.read"})assert(value.expose(name,action,s::exposure::allow)==error::success);
}
static s::grant grant(bool diagnostic,bool observe=true)
{
    s::grant out;out.alias=diagnostic?"health":"log";out.resource=diagnostic?"counters":"records";
    out.kind=diagnostic?s::purpose::diagnostic:s::purpose::log;out.binding=diagnostic?"local-diagnostic":"private-log";
    out.actions.emplace(diagnostic?"diagnostic.read":"log.emit");if(!diagnostic&&observe)out.actions.emplace("resource.observe");return out;
}
static void budget(s::deployment& value,std::string_view name="robot",std::size_t skip=6)
{
    constexpr std::array<std::uint64_t,6> values{1,1,16384,1024,2048,2};
    for(std::size_t i{};i<values.size();++i)if(i!=skip)
        assert(value.log_diagnostic_budget_limit(name,s::log_diagnostic_budget_configuration::names[i],values[i])==error::success);
}
static void configure(s::deployment& value,std::string_view name="robot",std::size_t skip=4,bool observe=true)
{
    assert(value.allow(name,grant(false,observe))==error::success&&value.allow(name,grant(true))==error::success);
    constexpr std::array<std::uint64_t,4> logs{2,1024,8192,31};
    for(std::size_t i{};i<logs.size();++i)if(i!=skip)assert(value.log_limit(name,"log",s::logs::configuration::limit_names[i],logs[i])==error::success);
    assert(value.log_logger(name,"log","robot")==error::success&&value.log_loss(name,"log","reject")==error::success);
    constexpr std::array<std::uint64_t,7> diagnostics{37,7,3,2,512,1000000000,100000000};
    for(std::size_t i{};i<diagnostics.size();++i)assert(value.diagnostic_limit(name,"health",s::diagnostics::configuration::limit_names[i],diagnostics[i])==error::success);
    assert(value.diagnostic_source(name,"health","log")==error::success);
}
static bytes record(std::uint64_t sequence=1,std::string_view logger="robot")
{
    s::logs::request value;value.session=19;value.sequence=sequence;value.level=s::logs::severity::info;
    value.logger=logger;value.message="literal %n and {} never become formatting";
    return encode([&](auto& out){assert(s::logs::encode(out,value));});
}
static bytes read(s::handle source,std::optional<std::uint64_t> cursor={},std::uint64_t count=2)
{return encode([&](auto& out){assert(out.u64(37)&&out.u64(source)&&out.boolean(cursor.has_value())&&(!cursor||out.u64(*cursor))&&out.u64(count));});}
struct live
{
    s::deployment deployment;int identity{};s::caller who{&identity,1};std::shared_ptr<s::broker> broker;s::handle log{},diagnostic{};
    live()
    {
        environment(deployment);configure(deployment);budget(deployment);
        assert(deployment.validate()==error::success&&deployment.select("main",who,broker)==error::success&&broker->log_diagnostic_store);
        assert(broker->permissions.open(who,"log",log)==error::success&&broker->permissions.open(who,"health",diagnostic)==error::success);
    }
};
static void explicit_closed_configuration()
{
    s::deployment empty;environment(empty);assert(empty.validate()==error::success);
    int id{};std::shared_ptr<s::broker> selected;assert(empty.select("main",{&id,1},selected)==error::success&&!selected->log_diagnostic_store);
    s::handle cap{};assert(selected->permissions.open({&id,1},"log",cap)==error::denied);
    s::deployment value;environment(value);
    assert(value.log_limit("robot","log","records",1)==error::denied&&value.diagnostic_source("robot","health","log")==error::denied);
    auto invalid=grant(false);invalid.actions.emplace("resource.close");assert(value.allow("robot",invalid)==error::unsupported);
    configure(value);budget(value);
    assert(value.log_logger("robot","log","robot")==error::conflict&&value.log_loss("robot","log","drop-oldest")==error::conflict);
    assert(value.log_limit("robot","log","unknown",1)==error::invalid&&value.diagnostic_limit("robot","health","fields",2048)==error::conflict);
    assert(value.validate()==error::success&&value.log_logger("robot","log","late")==error::invalid);
}
static void missing_configuration_never_freezes_partial_policy()
{
    constexpr std::array<std::uint64_t,6> amounts{1,1,16384,1024,2048,2};
    for(std::size_t missing{};missing<6;++missing)
    {
        s::deployment value;environment(value);configure(value);budget(value,"robot",missing);
        assert(value.validate()==error::denied);
        assert(value.log_diagnostic_budget_limit("robot",s::log_diagnostic_budget_configuration::names[missing],amounts[missing])==error::success&&value.validate()==error::success);
    }
    s::deployment value;environment(value);configure(value);budget(value);environment(value,"second","other");configure(value,"second",0);budget(value,"second");
    assert(value.validate()==error::denied);
    assert(value.log_limit("second","log","records",2)==error::success&&value.validate()==error::success);
    s::deployment missing;environment(missing);assert(missing.allow("robot",grant(false))==error::success&&missing.validate()==error::invalid);
    s::deployment no_observe;environment(no_observe);configure(no_observe,"robot",4,false);budget(no_observe);assert(no_observe.validate()==error::denied);
}
static void complete_receipts_and_compound_pages()
{
    live value;auto& store=*value.broker->log_diagnostic_store;bytes output;std::size_t required{};
    auto sink=[&](s::bytes data){output.assign(data.begin(),data.end());return true;};
    auto request=record();assert(store.invoke(value.who,c::operation::emit,value.log,request,47,required,sink)==error::too_large&&required==48&&output.empty());
    s::logs::usage usage;assert(store.inspect_log("log",usage)==error::success&&!usage.accepted);
    assert(store.invoke(value.who,c::operation::emit,value.log,request,48,required,sink)==error::success&&s::word(output,1)==19&&s::word(output,2)==1);
    auto receipt=output;assert(store.invoke(value.who,c::operation::emit,value.log,request,48,required,sink)==error::success&&output==receipt);
    assert(store.invoke(value.who,c::operation::emit,value.log,record(2,"robot.child"),48,required,sink)==error::denied);
    assert(store.invoke(value.who,c::operation::read,value.diagnostic,read(value.log),96,required,sink)==error::success);
    assert(s::word(output,1)==1&&s::word(output,3)==2&&s::word(output,4)==1&&s::word(output,5)==1);auto cursor=s::word(output,2);
    assert(store.invoke(value.who,c::operation::read,value.diagnostic,read(value.log,cursor,1),64,required,sink)==error::success&&s::word(output,1)==0&&s::word(output,2)==1);
    s::diagnostics::usage before,after;assert(store.inspect_diagnostic("health",before)==error::success);
    s::debug::request query;query.selected=s::debug::selection::resources;query.count=8;s::debug::view view;value.deployment.inspect(query,view);
    assert(view.result==s::debug::status::ok&&view.size==2);
    for(std::size_t i{};i<view.size;++i)assert(s::debug::valid(view.rows[i])&&!view.rows[i].id&&!view.rows[i].deadline_ns);
    assert(view.rows[0].type.view()=="log"&&view.rows[0].emitted_records==1&&view.rows[0].members==1);
    assert(store.inspect_diagnostic("health",after)==error::success&&before.expires==after.expires&&before.revision==after.revision&&before.rows==after.rows);
}
static void attenuation_and_source_revocation_are_independent()
{
    live value;auto& store=*value.broker->log_diagnostic_store;std::size_t required{};bool copied{};auto sink=[&](s::bytes){copied=true;return true;};
    assert(value.broker->revoke("log")==error::success);
    assert(store.invoke(value.who,c::operation::read,value.diagnostic,read(value.log),96,required,sink)==error::denied&&!copied);
    s::diagnostics::usage state;assert(store.inspect_diagnostic("health",state)==error::success&&state.retired);
    s::deployment limited;environment(limited);configure(limited);budget(limited);
    assert(limited.limit("robot","log","delivery","may-reject")==error::success&&limited.limit("robot","log","calls-per-window","0")==error::success);
    int id{};s::caller who{&id,1};std::shared_ptr<s::broker> broker;assert(limited.select("main",who,broker)==error::success);s::handle cap{};
    assert(broker->permissions.open(who,"log",cap)==error::success&&broker->log_diagnostic_store->invoke(who,c::operation::emit,cap,record(),48,required,sink)==error::again&&!copied);
    s::logs::usage usage;assert(broker->log_diagnostic_store->inspect_log("log",usage)==error::success&&!usage.accepted);
}
int main()
{
    explicit_closed_configuration();missing_configuration_never_freezes_partial_policy();complete_receipts_and_compound_pages();attenuation_and_source_revocation_are_independent();
    std::cout<<"passed 4 log diagnostic deployment security groups\n";
}
