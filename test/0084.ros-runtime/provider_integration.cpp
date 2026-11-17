/** @file
 * @brief Exercise real buffer/publication broker, deployment and compound authority.
 * @details No native provider or Wasm behavior is inferred from these tests.
 * The same production decode/authority/registry paths are used, including
 * failed delivery, cancellation, hidden dependencies and bounded debug state.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <atomic>
#include <barrier>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
namespace s=uwvm2::imported::ros::scene;
namespace bc=uwvm2::imported::ros::buffer_contract;
namespace oc=uwvm2::imported::ros::output_contract;
using s::error;
static s::buffers::plan buffer_plan()
{
    s::buffers::plan value;assert(value.schema.identity.assign("test.tensor"));value.schema.sha256.fill(19);
    value.bytes=128;value.objects=2;value.object_bytes=64;value.transfer_bytes=64;value.workspace_bytes=64;
    value.leases=2;value.elements=64;value.rank=2;value.element_mask=1;value.order_mask=1;value.color_mask=1;return value;
}
static s::buffers::layout buffer_layout()
{s::buffers::layout value;value.schema=buffer_plan().schema;value.rank=1;value.shape[0]=4;value.strides[0]=1;value.bytes=4;return value;}
static s::grant buffer_grant(std::string_view alias="data")
{
    s::grant value;value.alias=alias;value.resource="tensor";value.kind=s::purpose::buffer;value.binding="sealed-buffer";
    value.max_handles=4;value.max_resources=2;value.actions={"buffer.create","buffer.write","buffer.seal","buffer.read","buffer.describe","resource.close"};return value;
}
static s::grant output_grant()
{
    s::grant value;value.alias="cache";value.resource="output";value.kind=s::purpose::cache;value.binding="volatile-output";
    value.max_handles=4;value.max_resources=2;value.actions={"artifact.publish","artifact.begin","artifact.append","artifact.commit","artifact.read","artifact.receipt"};return value;
}
static std::vector<std::uint8_t> words(std::initializer_list<std::uint64_t> values)
{std::vector<std::uint8_t> bytes(values.size()*8);std::size_t index{};for(auto value:values)s::put(bytes,index++,value);return bytes;}
static std::vector<std::uint8_t> create_request()
{std::array<std::uint8_t,512> bytes{};s::wire::writer out{bytes};assert(bc::encode_layout(out,buffer_layout()));return {out.written().begin(),out.written().end()};}
static std::vector<std::uint8_t> blob_request(std::initializer_list<std::uint64_t> prefix,s::bytes bytes)
{auto result=words(prefix);auto size=words({bytes.size()});result.insert(result.end(),size.begin(),size.end());result.insert(result.end(),bytes.begin(),bytes.end());return result;}
struct fixture
{
    s::broker state;int instance{};s::caller who{&instance,7};s::handle data{},cache{};
    explicit fixture(s::grant data_policy=buffer_grant(),s::grant output_policy=output_grant())
    {
        auto expose=[&](std::string_view action){assert(state.permissions.expose(uwvm2::utils::container::std_string(action),s::exposure::allow)==error::success);};
        expose("capability.open");expose("capability.close");expose("resource.close");
        for(auto const& entry:bc::operations)expose(entry.name);
        for(auto const& entry:oc::operations)expose(entry.name);
        expose("artifact.read");
        assert(state.permissions.configure(std::move(data_policy))==error::success);
        assert(state.permissions.configure(std::move(output_policy))==error::success);
        assert(state.buffer_store.bind("data",buffer_plan())==error::success);
        assert(state.output_store.bind("cache","output",{128,2,64,64,1024,2})==error::success);
        assert(state.permissions.enroll(who)==error::success);
        assert(state.permissions.open(who,"data",data)==error::success&&state.permissions.open(who,"cache",cache)==error::success);
    }
    std::uint64_t admissions(std::string_view alias="data")
    {uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(state.permissions.inspect(rows,8));for(auto const& row:rows)if(row.alias==alias)return row.admissions;assert(false);return 0;}
    error buffer(bc::operation action,s::bytes input,std::vector<std::uint8_t>& output,std::size_t capacity=512,
                 s::buffer_sink custom={},uwvm2::utils::container::function_ref<bool()> cancelled={})
    {
        auto copy=[&](s::bytes first,s::bytes second){output.assign(first.begin(),first.end());output.insert(output.end(),second.begin(),second.end());return true;};
        std::size_t required{};return state.invoke_buffer(who,action,data,input,capacity,required,custom?custom:s::buffer_sink(copy),cancelled);
    }
    s::handle create()
    {std::vector<std::uint8_t> out;assert(buffer(bc::operation::create,create_request(),out)==error::success);return s::word(out,0);}
};
static void buffer_authority_and_delivery()
{
    fixture f;std::vector<std::uint8_t> output;auto request=create_request();std::size_t required{};unsigned copied{};
    auto sink=[&](s::bytes,s::bytes){++copied;return true;};
    assert(f.state.invoke_buffer(f.who,bc::operation::create,f.data,request,7,required,sink)==error::too_large&&required==8);
    assert(!f.admissions()&&!copied);
    assert(f.state.invoke_buffer({&f.instance,8},bc::operation::create,f.data,request,8,required,sink)==error::denied);
    assert(f.state.invoke_buffer(f.who,bc::operation::create,f.cache,request,8,required,sink)==error::denied);
    assert(f.state.invoke_buffer(f.who,static_cast<bc::operation>(255),f.data,request,8,required,sink)==error::unsupported);
    auto failed=[](s::bytes,s::bytes){return false;};
    assert(f.buffer(bc::operation::create,request,output,8,failed)==error::io);
    s::buffers::usage usage;assert(f.state.buffer_store.inspect("data",usage)==error::success&&usage.objects==0);
    bool cancel{};auto during=[&](s::bytes,s::bytes){cancel=true;return true;};auto cancelled=[&]{return cancel;};
    assert(f.buffer(bc::operation::create,request,output,8,during,cancelled)==error::io);
    assert(f.state.buffer_store.inspect("data",usage)==error::success&&usage.objects==0);
    auto id=f.create();assert(id&&f.admissions()==3);
    request.push_back(0);auto before=f.admissions();assert(f.buffer(bc::operation::create,request,output)==error::invalid&&f.admissions()==before);
}
static void buffer_data_and_recovery()
{
    fixture f;auto id=f.create();std::vector<std::uint8_t> output;constexpr std::array<std::uint8_t,4> bytes{1,2,3,4};
    assert(f.buffer(bc::operation::read,words({id,0,4}),output)==error::too_large); // Nothing initialized yet.
    auto request=blob_request({id,0},bytes);auto failed=[](s::bytes,s::bytes){return false;};
    assert(f.buffer(bc::operation::write,request,output,8,failed)==error::invalid);
    assert(f.buffer(bc::operation::write,request,output)==error::success&&s::word(output,0)==4);
    auto changed=request;changed.back()^=1;assert(f.buffer(bc::operation::write,changed,output)==error::conflict);
    auto hash=s::buffers::hash(bytes);auto seal=blob_request({id},hash);
    assert(f.buffer(bc::operation::seal,seal,output,16,failed)==error::invalid);
    assert(f.buffer(bc::operation::seal,seal,output)==error::success&&s::word(output,0)==f.data&&s::word(output,1)==id);
    assert(f.buffer(bc::operation::write,request,output)==error::denied);
    assert(f.buffer(bc::operation::read,words({id,0,4}),output)==error::success&&s::word(output,0)==4);
    assert(std::equal(bytes.begin(),bytes.end(),output.begin()+8));
    auto before=f.admissions();assert(f.buffer(bc::operation::read,words({id,0,UINT64_MAX}),output)==error::invalid&&f.admissions()==before);
    assert(f.buffer(bc::operation::describe,words({id}),output)==error::success);
    s::wire::reader description{output};s::buffers::layout format;bool sealed{};std::string_view provenance;
    assert(bc::decode_layout(description,format)&&description.boolean(sealed)&&description.text(provenance)&&description.done()&&sealed&&provenance=="guest");
    s::buffers::registry::sealed_lease held;assert(f.state.buffer_store.pin("data",id,held)==error::success);
    auto close=words({id});std::size_t required{};auto sink=[](s::bytes){return true;};
    assert(f.state.invoke(f.who,uwvm2::imported::ros::runtime_contract::operation::resource_close,f.data,close,0,required,sink)==error::success);
    s::buffers::usage usage;assert(f.state.buffer_store.inspect("data",usage)==error::success&&usage.objects==1&&usage.active_handles==0&&usage.leases==1);
    held={};assert(f.state.buffer_store.inspect("data",usage)==error::success&&usage.objects==0&&usage.bytes==0);
}
static void output_compound_delivery()
{
    fixture f;constexpr std::array<std::uint8_t,3> bytes{'x','m','l'};auto request=blob_request({1},bytes);
    std::vector<std::uint8_t> output;std::size_t required{};
    auto sink=[&](s::bytes data){output.assign(data.begin(),data.end());return true;};
    assert(f.state.invoke_output(f.who,oc::operation::publish,f.cache,request,71,required,sink)==error::too_large&&required==72&&!f.admissions("cache"));
    auto failed=[](s::bytes){return false;};
    assert(f.state.invoke_output(f.who,oc::operation::publish,f.cache,request,72,required,failed)==error::invalid);
    assert(f.state.invoke_output(f.who,oc::operation::publish,f.cache,request,72,required,sink)==error::success);
    assert(s::word(output,0)==1&&s::word(output,1)==8&&!s::word(output,3)&&s::word(output,4)==3&&s::word(output,7)==2);
    auto original=output;assert(f.state.invoke_output(f.who,oc::operation::publish,f.cache,request,72,required,sink)==error::success&&output==original);
    unsigned checks{};auto cancel=[&]{return ++checks==2;};auto next=blob_request({2},bytes);
    assert(f.state.invoke_output(f.who,oc::operation::publish,f.cache,next,72,required,sink,cancel)==error::cancelled&&checks==2);
    s::outputs::receipt receipt;assert(f.state.output_store.receipt_for("cache",2,receipt)==error::conflict); // Cancel did not consume the next key.
    assert(f.state.invoke_output(f.who,oc::operation::publish,f.cache,next,72,required,sink)==error::success&&s::word(output,0)==2);
    s::outputs::registry::publication_scratch a,b,c;
    assert(f.state.reserve_output_publication(f.who,f.cache,request.size(),a)==error::success);
    assert(f.state.reserve_output_publication(f.who,f.cache,request.size(),b)==error::success);
    assert(f.state.reserve_output_publication(f.who,f.cache,request.size(),c)==error::again);
}
static void bounded_carrier_workspace()
{
    s::buffers::registry store;auto plan=buffer_plan();plan.workspace_bytes=64;
    assert(store.bind("data",plan)==error::success);s::handle id{};
    assert(store.create("data",buffer_layout(),2,[&](auto value){id=value;return true;})==error::success);
    constexpr std::array<std::uint8_t,4> bytes{1,2,3,4};std::uint64_t accepted{};
    assert(store.write("data",id,0,bytes,accepted)==error::success);
    s::buffers::registry::workspace_lease first,second;
    assert(store.reserve_workspace("data",0,first)==error::invalid&&!first);
    assert(store.reserve_workspace("data",61,first)==error::success&&first.live()&&first.reserved_bytes()==61);
    assert(store.reserve_workspace("data",1,first)==error::invalid);
    assert(store.reserve_workspace("data",4,second)==error::again&&!second);
    assert(store.read("data",id,0,4,[](auto){assert(false);return true;})==error::again);
    auto moved=std::move(first);assert(!first&&moved.live());moved.reset();
    assert(store.read("data",id,0,4,[&](auto input){assert(input.size()==4);
        s::buffers::usage used;assert(store.try_inspect("data",used)==error::success&&used.workspace==4);
        assert(store.reserve_workspace("data",61,second)==error::again);
        assert(store.reserve_workspace("data",60,second)==error::success);return true;})==error::success);
    assert(second.reserved_bytes()==60);store.revoke("data");assert(!second.live());second.reset();
    s::buffers::registry::workspace_lease surviving;
    {s::buffers::registry scoped;assert(scoped.bind("data",plan)==error::success&&scoped.reserve_workspace("data",64,surviving)==error::success);}
    assert(!surviving.live());surviving.reset();
    fixture f;assert(f.state.reserve_buffer_workspace(f.who,bc::operation::write,f.data,513,first)==error::too_large);
    assert(f.state.reserve_buffer_workspace(f.who,bc::operation::create,f.data,513,first)==error::too_large);
    assert(f.state.reserve_buffer_workspace(f.who,bc::operation::write,f.cache,513,first)==error::denied);
    assert(!f.admissions());
}
static void compound_atomic_authority()
{
    auto source=buffer_grant();source.limits.uncertain=true;source.limits.calls=2;source.limits.bytes=8;
    auto target=output_grant();target.limits.uncertain=true;target.limits.calls=0;
    fixture f{source,target};std::array claims{s::admission_claim{f.data,"buffer.read",4},s::admission_claim{f.cache,"artifact.read",0}};
    s::group_lease lease;assert(f.state.permissions.admit_group(f.who,claims,lease)==error::again&&!lease&&!f.admissions()&&!f.admissions("cache"));
    claims[1]={f.data,"buffer.read",4};assert(f.state.permissions.admit_group(f.who,claims,lease,false,true)==error::success&&lease.size()==2&&!f.admissions());lease.release();
    assert(f.state.permissions.admit_group(f.who,claims,lease)==error::success&&lease.specification(0)==lease.specification(1)&&!lease.specification(2));lease.release();
    assert(f.admissions()==2&&f.state.permissions.admit_group(f.who,claims,lease)==error::again);
    assert(f.state.permissions.admit_group(f.who,claims,lease,false,false)==error::success);lease.release();
    assert(f.state.permissions.admit_group({&f.instance,8},claims,lease)==error::denied);
    assert(f.state.permissions.admit_group(f.who,{},lease)==error::invalid);
    std::array<s::admission_claim,s::maximum_admission_claims+1> excessive{};
    assert(f.state.permissions.admit_group(f.who,excessive,lease)==error::invalid);
    fixture overflow;claims={s::admission_claim{overflow.data,"buffer.read",UINT64_MAX},s::admission_claim{overflow.data,"buffer.read",1}};
    assert(overflow.state.permissions.admit_group(overflow.who,claims,lease)==error::too_large&&!overflow.admissions());
    claims[0]={overflow.cache,"artifact.publish",0};assert(overflow.state.permissions.admit_group(overflow.who,claims,lease)==error::unsupported);
}
static void compound_concurrency_and_revocation()
{
    auto source=buffer_grant();source.limits.uncertain=true;source.limits.calls=2;
    fixture f{source};std::array claims{s::admission_claim{f.data,"buffer.read",4},s::admission_claim{f.data,"buffer.read",4}};
    std::barrier begin{2};std::atomic<unsigned> success{},refused{};
    auto run=[&]{begin.arrive_and_wait();s::group_lease lease;auto result=f.state.permissions.admit_group(f.who,claims,lease);
        if(result==error::success)++success;else{assert(result==error::again);++refused;}};
    std::thread first{run},second{run};first.join();second.join();assert(success==1&&refused==1&&f.admissions()==2);
    s::group_lease held;assert(f.state.permissions.admit_group(f.who,claims,held,false)==error::success);
    std::atomic<bool> entered{},done{};std::thread revoke{[&]{entered=true;f.state.revoke("data");done=true;}};
    while(!entered.load())std::this_thread::yield();
    assert(!done.load());held.release();revoke.join();
    assert(f.state.permissions.admit_group(f.who,claims,held,false)==error::denied);
}
static void deployment_configuration_and_debug()
{
    s::deployment config;assert(config.create("robot")==error::success&&config.bind("robot","main")==error::success);
    assert(config.allow("robot",buffer_grant())==error::success&&config.allow("robot",output_grant())==error::success);
    assert(config.buffer_plan("robot","data",buffer_plan())==error::success);
    assert(config.output("robot","cache",{128,2,64,64})==error::success);
    assert(config.publication("robot","missing",512,1)==error::denied);
    assert(config.publication("robot","cache",1024,2)==error::success&&config.publication("robot","cache",1024,2)==error::conflict);
    assert(config.validate()==error::invalid);
    assert(config.output_budget("robot",128,2,160)==error::success&&config.buffer_budget("robot",128,2,64,2)==error::success);
    assert(config.validate()==error::invalid);
    assert(config.publication_budget("robot",1024,2)==error::success&&config.publication_budget("robot",1024,2)==error::invalid);
    assert(config.expose("robot","capability.open",s::exposure::allow)==error::success);
    for(auto const& entry:bc::operations)assert(config.expose("robot",uwvm2::utils::container::std_string(entry.name),s::exposure::allow)==error::success);
    assert(config.validate()==error::success);
    unsigned input_calls{};assert(config.prepare_resources([&](auto const&,auto const&,auto,std::shared_ptr<s::storage::immutable_input>&){++input_calls;return error::io;})==error::success&&!input_calls);
    int instance{};s::caller who{&instance,9};std::shared_ptr<s::broker> state;assert(config.select("main",who,state)==error::success);
    s::handle cap{};assert(state->permissions.open(who,"data",cap)==error::success);
    std::size_t required{};s::handle id{};auto sink=[&](s::bytes first,s::bytes){id=s::word(first,0);return true;};
    assert(state->invoke_buffer(who,bc::operation::create,cap,create_request(),8,required,sink)==error::success&&id);
    s::debug::request query;query.selected=s::debug::selection::resources;query.alias.assign("data");s::debug::view view;
    config.inspect(query,view);assert(view.result==s::debug::status::ok&&view.size==1);
    auto const& row=view.rows[0];assert(row.type.view()=="buffer"&&row.members==1&&row.reserved_bytes==4&&row.open_handles==1&&!row.id&&!row.lease_count&&!row.workspace_bytes);
    query.selected=s::debug::selection::authority;config.inspect(query,view);
    assert(view.result==s::debug::status::ok&&view.size==1&&view.rows[0].type.view()=="buffer");
}
int main()
{
    buffer_authority_and_delivery();buffer_data_and_recovery();output_compound_delivery();compound_atomic_authority();
    compound_concurrency_and_revocation();deployment_configuration_and_debug();bounded_carrier_workspace();
    std::cout<<"passed 7 provider integration security groups\n";
}
