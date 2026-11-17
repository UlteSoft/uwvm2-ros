/** @file @brief Authority-only revocation inside trusted callbacks cannot authorize later effects/copies. */
#include <uwvm2/imported/ros/scene/broker.h>
#include <cassert>
#include <iostream>
namespace s=uwvm2::imported::ros::scene;
namespace r=uwvm2::imported::ros::resources;
namespace a=uwvm2::imported::ros::artifact_contract;
namespace o=uwvm2::imported::ros::output_contract;
namespace b=uwvm2::imported::ros::buffer_contract;
namespace m=uwvm2::imported::ros::managed_lifecycle_contract;
namespace rt=uwvm2::imported::ros::runtime_contract;
using s::error;using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
static bytes words(std::initializer_list<std::uint64_t> values,std::string_view tail={})
{bytes out(values.size()*8+tail.size());std::size_t at{};for(auto value:values)s::put(out,at++,value);std::copy(tail.begin(),tail.end(),out.begin()+values.size()*8);return out;}
template<typename Fn> static bytes record(Fn fn)
{std::array<std::uint8_t,1024> data{};s::wire::writer out{data};fn(out);return {out.written().begin(),out.written().end()};}
struct fixture
{
    s::broker broker;int identity{};s::caller who{&identity,1};s::handle cap{};
    explicit fixture(s::purpose kind,std::string_view binding,std::initializer_list<std::string_view> actions)
    {
        s::grant value;value.alias="scope";value.resource="private";value.binding.assign(binding);value.kind=kind;value.max_resources=8;value.max_wait_ns=1000000000;
        for(auto name:actions){value.actions.emplace(name);assert(broker.permissions.expose(uwvm2::utils::container::std_string(name),s::exposure::allow)==error::success);}
        assert(broker.permissions.expose("capability.open",s::exposure::allow)==error::success);
        assert(broker.permissions.configure(value)==error::success&&broker.permissions.enroll(who)==error::success&&broker.permissions.open(who,"scope",cap)==error::success);
    }
    void revoke(){assert(broker.permissions.revoke("scope",[](auto){})==error::success);}
};
static void cache_mutation_revalidates_after_callback()
{
    for(unsigned stage=1;stage<=2;++stage)
    {
        fixture f{s::purpose::cache,"volatile-output",{"artifact.begin"}};
        assert(f.broker.output_store.bind("scope","private",{64,2,32,8})==error::success);unsigned calls{};bool copied{};std::size_t required{};
        auto cancel=[&]{if(++calls==stage)f.revoke();return false;};auto sink=[&](s::bytes){copied=true;return true;};
        assert(f.broker.invoke_output(f.who,o::operation::begin,f.cap,words({1,4}),72,required,sink,cancel)==error::denied&&!copied);
        s::outputs::receipt receipt;auto status=f.broker.output_store.receipt_for("scope",1,receipt);
        assert(stage==1?status==error::conflict:status==error::success);
    }
}
static void artifact_read_never_discloses_after_cancellation_reentry()
{
    for(unsigned stage=1;stage<=2;++stage)
    {
        fixture f{s::purpose::cache,"volatile-output",{"artifact.read"}};
        assert(f.broker.output_store.bind("scope","private",{64,2,32,8})==error::success);s::outputs::receipt receipt;
        assert(f.broker.output_store.mutate("scope",o::operation::begin,words({1,4}),2,receipt)==error::success);auto id=receipt.object;
        assert(f.broker.output_store.mutate("scope",o::operation::append,words({2,id,0},"data"),2,receipt)==error::success);
        assert(f.broker.output_store.mutate("scope",o::operation::commit,words({3,id,4}),2,receipt)==error::success&&!receipt.effect_status);
        unsigned calls{};bool copied{};std::size_t required{};auto cancel=[&]{if(++calls==stage)f.revoke();return false;};
        auto sink=[&](s::bytes){copied=true;return true;};
        assert(f.broker.invoke_artifact(f.who,a::operation::read,f.cap,words({id,0,4}),4,required,sink,cancel)==error::denied&&!copied);
        s::byte_buffer data;assert(f.broker.output_store.copy_read("scope",id,0,4,data)==error::success&&data.view().size()==4);
    }
}
static void buffer_create_and_write_revalidate()
{
    for(unsigned stage=1;stage<=2;++stage)
    {
        fixture f{s::purpose::buffer,"sealed-buffer",{"buffer.create","buffer.write"}};
        s::buffers::plan plan;assert(plan.schema.identity.assign("tensor"));plan.objects=2;plan.bytes=8;plan.object_bytes=4;plan.transfer_bytes=4;
        plan.workspace_bytes=8;plan.leases=2;plan.elements=4;plan.rank=1;plan.element_mask=1;plan.order_mask=1;plan.color_mask=1;
        assert(f.broker.buffer_store.bind("scope",plan)==error::success);
        s::buffers::layout layout;layout.schema=plan.schema;layout.rank=1;layout.shape[0]=4;layout.strides[0]=1;layout.bytes=4;
        auto request=record([&](auto& out){assert(b::encode_layout(out,layout));});unsigned calls{};bool copied{};std::size_t required{};
        auto cancel=[&]{if(++calls==stage)f.revoke();return false;};auto sink=[&](s::bytes,s::bytes){copied=true;return true;};
        auto result=f.broker.invoke_buffer(f.who,b::operation::create,f.cap,request,8,required,sink,cancel);
        assert(result==(stage==1?error::denied:error::io)&&!copied);
        s::buffers::usage usage;assert(f.broker.buffer_store.inspect("scope",usage)==error::success&&!usage.objects);
    }
}
static void lifecycle_open_rolls_back_before_disclosure()
{
    for(unsigned stage=1;stage<=2;++stage)
    {
        fixture f{s::purpose::lifecycle,"local-lifecycle",{"lifecycle.open"}};
        assert(f.broker.managed_nodes.bind("scope",{2,1000000000,s::managed_lifecycle::all_transitions,3})==error::success);
        unsigned calls{};bool copied{};std::size_t required{};auto cancel=[&]{if(++calls==stage)f.revoke();return false;};
        auto sink=[&](s::bytes){copied=true;return true;};
        auto result=f.broker.invoke_managed(f.who,m::operation::open,f.cap,words({s::managed_lifecycle::client}),8,required,sink,cancel);
        assert(result==(stage==1?error::denied:error::invalid)&&!copied);
        uwvm2::utils::container::std_vector<s::managed_lifecycle::inspection> rows;
        assert(f.broker.managed_nodes.inspect(rows,1)&&rows.size()==1&&!rows[0].handles);
    }
}
static void zero_output_validation_cannot_authorize_a_later_effect()
{
    fixture f{s::purpose::guard,"local",{"guard.trigger"}};s::handle id{};assert(f.broker.runtime.guard_create("scope",1,id)==error::success);
    std::size_t required{};auto sink=[&](s::bytes data){assert(data.empty());f.revoke();return true;};
    assert(f.broker.invoke(f.who,rt::operation::guard_trigger,f.cap,words({id}),0,required,sink)==error::denied);
    assert(f.broker.runtime.guard_deliver("scope",id,[](auto){assert(false);return true;})==error::again);
    fixture wait{s::purpose::wait,"local",{"wait.poll"}};assert(wait.broker.runtime.wait_set_create("scope",1,id)==error::success);
    auto cancel=[&]{wait.revoke();return false;};auto no_copy=[](s::bytes){assert(false);return true;};
    assert(wait.broker.invoke(wait.who,rt::operation::wait_set_wait,wait.cap,words({id,0}),32,required,no_copy,cancel)==error::denied);
}
int main()
{
    cache_mutation_revalidates_after_callback();artifact_read_never_discloses_after_cancellation_reentry();buffer_create_and_write_revalidate();
    lifecycle_open_rolls_back_before_disclosure();zero_output_validation_cannot_authorize_a_later_effect();
    std::cout<<"passed 5 scene callback authority security groups\n";
}
