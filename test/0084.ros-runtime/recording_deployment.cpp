/** @file @brief Closed deployment, actual owned buffers, seven recording/replay operations and cached debugger metadata. */
#include "recording_deployment_fixture.h"
#include <iostream>
namespace f=recording_fixture;namespace s=f::s;using s::error;using op=uwvm2::imported::ros::recording_contract::operation;
static void zero_authority_and_missing_plans()
{
    s::deployment empty;f::environment(empty);assert(empty.validate()==error::success);
    int id{};std::shared_ptr<s::broker> broker;assert(empty.select("main",{&id,1},broker)==error::success&&!broker->recording_store);
    s::handle cap{};assert(broker->permissions.open({&id,1},"recording",cap)==error::denied);
    s::deployment denied;f::environment(denied);assert(denied.recording_profile("robot","recording",f::recording_profile())==error::denied);
    assert(denied.recording_definition_file("robot","recording","/must-not-be-opened",1,{})==error::denied);
    auto bad=f::grant(s::purpose::recording);bad.actions.emplace("artifact.read");assert(denied.allow("robot",bad)==error::unsupported);
    assert(denied.allow("robot",f::grant(s::purpose::recording))==error::success&&denied.validate()==error::invalid);
    for(std::size_t missing{};missing<f::budgets.size();++missing)
    {
        s::deployment d;f::environment(d);f::configure(d);f::budget(d,missing);assert(d.validate()==error::denied);
        assert(d.recording_budget_limit("robot",s::recording_budget_configuration::names[missing],f::budgets[missing])==error::success&&d.validate()==error::success);
    }
    for(std::size_t missing{};missing<f::limits.size();++missing)
    {
        s::deployment d;f::environment(d);f::configure(d,true,true,false,missing);f::budget(d);
        assert(d.recording_definition_file("robot","recording","/must-not-be-opened",1,{})==error::denied);
        assert(d.recording_definition_hex("robot","recording",f::hex(f::definition()))==error::denied&&d.validate()==error::denied);
        assert(d.recording_limit("robot","recording",s::recording::configuration::limit_names[missing],f::limits[missing])==error::success);
        assert(d.recording_definition("robot","recording",f::definition())==error::success&&d.validate()==error::success);
    }
}
static void exact_definitions_and_compound_scopes()
{
    s::deployment d;f::environment(d);f::configure(d,true,true,false);f::budget(d);
    for(auto text:{"0","GG","AA",""})assert(d.recording_definition_hex("robot","recording",text)==error::invalid);
    uwvm2::utils::container::std_string excessive(16386,'0');assert(d.recording_definition_hex("robot","recording",excessive)==error::too_large);
    auto wire=f::definition();wire.back()^=1;assert(d.recording_definition("robot","recording",wire)==error::conflict);
    assert(d.recording_definition_hex("robot","recording",f::hex(f::definition()))==error::success);
    assert(d.recording_definition_hex("robot","recording",f::hex(f::definition()))==error::conflict&&d.validate()==error::success);
    assert(d.recording_limit("robot","recording","segments",1)==error::invalid);
    for(bool input:{true,false})
    {s::deployment missing;f::environment(missing);f::configure(missing,!input,input);f::budget(missing);assert(missing.validate()==error::denied);}
    s::deployment no_exposure;f::environment(no_exposure,false);f::configure(no_exposure);f::budget(no_exposure);
    int id{};std::shared_ptr<s::broker> broker;assert(no_exposure.select("main",{&id,1},broker)==error::success);
    s::handle cap{};assert(broker->permissions.open({&id,1},"recording",cap)==error::denied);
}
struct live
{
    s::deployment deployment;int identity{};s::caller who{&identity,1};std::shared_ptr<s::broker> broker;
    s::handle recording{},replay{},input{},buffer{},writer{};f::bytes output;std::size_t required{};
    live()
    {
        f::environment(deployment);f::configure(deployment);f::budget(deployment);assert(deployment.select("main",who,broker)==error::success&&broker->recording_store);
        assert(broker->permissions.open(who,"recording",recording)==error::success&&broker->permissions.open(who,"replay",replay)==error::success&&broker->permissions.open(who,"input",input)==error::success);
        s::buffers::layout layout;layout.schema=f::buffer_profile();layout.rank=1;layout.shape[0]=layout.strides[0]=layout.bytes=1;
        assert(broker->buffer_store.create("input",layout,8,[&](auto id){buffer=id;return true;})==error::success);
        std::uint8_t value=1;std::uint64_t accepted{};assert(broker->buffer_store.write("input",buffer,0,{&value,1},accepted)==error::success);
        assert(broker->buffer_store.seal("input",buffer,s::buffers::hash({&value,1}))==error::success);
    }
    error call(op action,f::bytes const& body,std::size_t capacity=8192,s::recording_dispatch::sink sink={})
    {auto copy=[&](s::bytes data){output.assign(data.begin(),data.end());return true;};return broker->recording_store->invoke(who,action,action<=op::recording_stop?recording:replay,body,capacity,required,sink?sink:s::recording_dispatch::sink{copy});}
    void open()
    {auto p=f::recording_profile();assert(call(op::recording_open,f::encode([&](auto& out){assert(out.text(p.identity.view())&&out.blob(p.sha256));}))==error::success);writer=s::word(output,0);}
    auto append()
    {return f::encode([&](auto& out){std::uint8_t value=1;assert(out.u64(writer)&&out.u64(1)&&out.u64(1)&&out.u64(input)&&out.u64(buffer)&&out.u64(1)&&out.u64(5)&&out.u64(10)&&out.u64(20)&&out.u64(1)&&out.blob(s::buffers::hash({&value,1})));});}
    auto mutation(std::uint64_t key,bool stop=false)
    {return f::encode([&](auto& out){assert(out.u64(writer)&&out.u64(key)&&(!stop||out.boolean(false)));});}
    auto cursor(s::handle id,std::uint64_t count=1){return f::encode([&](auto& out){assert(out.u64(id)&&out.u64(count));});}
    std::uint64_t admissions(std::string_view alias)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(broker->permissions.inspect(rows,8));for(auto const& row:rows)if(row.alias==alias)return row.admissions;assert(false);return 0;}
};
static void seven_operations_and_nonconsuming_debug()
{
    live f;f.open();auto append=f.append();assert(f.call(op::recording_append,append,103)==error::too_large&&f.required==104);
    assert(f.call(op::recording_append,append,104,[](auto){return false;})==error::io);auto charged=f.admissions("recording");
    assert(f.call(op::recording_append,append)==error::success&&f.admissions("recording")==charged);
    assert(f.call(op::recording_checkpoint,f.mutation(2))==error::success);
    s::recording::object_info object;object.id=s::word(f.output,4);object.bytes=s::word(f.output,5);object.messages=s::word(f.output,6);std::copy_n(f.output.data()+64,32,object.sha256.begin());
    auto p=f::replay_profile();auto request=f::encode([&](auto& out){assert(out.u64(f.recording)&&out.text(p.identity.view())&&out.blob(p.sha256)&&out.u64(1)&&out.u64(object.id)&&out.u64(object.bytes)&&out.u64(object.messages)&&out.blob(object.sha256));});
    assert(f.call(op::replay_open,request)==error::success);auto replay=s::word(f.output,0);
    s::debug::request query;query.selected=s::debug::selection::resources;s::debug::view view;
    auto recording_count=f.admissions("recording"),replay_count=f.admissions("replay");f.deployment.inspect(query,view);
    assert(view.result==s::debug::status::ok&&view.size==3);bool saw_recording{},saw_replay{};
    for(std::size_t i{};i<view.size;++i)
    {
        auto const& row=view.rows[i];assert(s::debug::valid(row)&&!row.id&&!row.deadline_ns);
        if(row.type.view()=="recording"){saw_recording=true;assert(row.members==1&&row.lease_count==1&&row.query_count==1&&row.open_handles==2&&row.retained_bytes==object.bytes);}
        if(row.type.view()=="replay"){saw_replay=true;assert(row.members==1&&row.open_handles==1&&row.state.view()=="active");}
    }
    assert(saw_recording&&saw_replay&&f.admissions("recording")==recording_count&&f.admissions("replay")==replay_count);
    assert(f.call(op::replay_read,f.cursor(replay),56)==error::too_large&&f.required==57);
    assert(f.call(op::replay_read,f.cursor(replay))==error::success&&s::word(f.output,0)==0&&s::word(f.output,1)==1&&f.output.back()==1);
    assert(f.call(op::replay_seek,f.cursor(replay,0))==error::success&&f.call(op::replay_read,f.cursor(replay))==error::success&&s::word(f.output,0)==0);
    assert(f.call(op::recording_stop,f.mutation(3,true))==error::success);
    auto close=f::encode([&](auto& out){assert(out.u64(replay));});std::size_t required{};auto before=f.admissions("replay");
    assert(f.broker->invoke(f.who,uwvm2::imported::ros::runtime_contract::operation::resource_close,f.replay,close,0,required,[](auto){return false;})==error::invalid&&f.admissions("replay")==before);
    assert(f.broker->invoke(f.who,uwvm2::imported::ros::runtime_contract::operation::resource_close,f.replay,close,0,required,[](auto){return true;})==error::success&&f.admissions("replay")==before+1);
    f.deployment.inspect(query,view);assert(view.result==s::debug::status::ok);
}
static void attenuation_and_revocation()
{
    live f;f.open();assert(f.broker->revoke("input")==error::success&&f.call(op::recording_append,f.append())==error::denied);
    s::recording::usage usage;assert(f.broker->recording_store->inspect("recording",usage)==error::success&&!usage.active_messages);
    s::deployment d;f::environment(d);f::configure(d);f::budget(d);
    assert(d.limit("robot","recording","delivery","may-reject")==error::success&&d.limit("robot","recording","calls-per-window","0")==error::success);
    int id{};s::caller who{&id,1};std::shared_ptr<s::broker> broker;assert(d.select("main",who,broker)==error::success);s::handle cap{};assert(broker->permissions.open(who,"recording",cap)==error::success);
    auto p=f::recording_profile();auto request=f::encode([&](auto& out){assert(out.text(p.identity.view())&&out.blob(p.sha256));});std::size_t required{};bool delivered{};
    auto sink=[&](auto){delivered=true;return true;};assert(broker->recording_store->invoke(who,op::recording_open,cap,request,8,required,sink)==error::again&&!delivered);
    assert(broker->recording_store->inspect("recording",usage)==error::success&&!usage.recording);
}
int main()
{zero_authority_and_missing_plans();exact_definitions_and_compound_scopes();seven_operations_and_nonconsuming_debug();attenuation_and_revocation();std::cout<<"passed 4 recording deployment security groups\n";}
