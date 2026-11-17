/** @file
 * @brief Exercise nine real transform carriers and shared request routing with controlled runtime memory.
 * @details Runtime identity/memory are fixtures. Production host, deployment,
 * authority, copied carriers and job lifecycle run unchanged. Real runtime clocks and finite TF storage back every entry. This fixture
 * complements actual Wasm tagged-memory and floating-point boundary checks.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include "transform_deployment_fixture.h"
#include <cassert>
#include <cstdio>
#include <cstring>
namespace h=uwvm2::uwvm::imported::scene;
namespace s=uwvm2::imported::ros::scene;
namespace c=uwvm2::imported::ros::transform_contract;
using s::error;
static std::array<std::uint8_t,65536> memory;
static int identity;
static unsigned reads{},copies{},large_attempts{};
static bool fail_read{},fail_write{},reject_large{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
static bool refuse(std::size_t size) noexcept{if(reject_large&&size>512){++large_attempts;return true;}return false;}
extern "C" void* __wrap_malloc(std::size_t size){return refuse(size)?nullptr:__real_malloc(size);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t size){return count&&size>SIZE_MAX/count?nullptr:refuse(count*size)?nullptr:__real_calloc(count,size);}
extern "C" void* __wrap_realloc(void* pointer,std::size_t size){return refuse(size)?nullptr:__real_realloc(pointer,size);}
namespace uwvm2::runtime::lib
{
    bool ros_caller_identity_host_api(ros_caller_identity& out,bool) noexcept
    {out={&identity,1,{u8"main"},false};return true;}
    bool preload_memory_range_host_api(std::size_t index,std::uint_least64_t offset,std::size_t size) noexcept
    {return !index&&offset<=memory.size()&&size<=memory.size()-offset;}
    bool preload_memory_normalize_range_host_api(std::size_t index,std::uint_least64_t offset,std::size_t size,std::uint64_t& normalized) noexcept
    {normalized=offset;return preload_memory_range_host_api(index,offset,size);}
    bool preload_memory_read_host_api(std::size_t index,std::uint_least64_t offset,void* output,std::size_t size) noexcept
    {
        if(!preload_memory_range_host_api(index,offset,size))return false;
        ++reads;std::memcpy(output,memory.data()+offset,fail_read?size/2:size);return !fail_read;
    }
    bool preload_memory_write_host_api(std::size_t index,std::uint_least64_t offset,void const* input,std::size_t size) noexcept
    {
        if(!preload_memory_range_host_api(index,offset,size))return false;
        ++copies;if(fail_write)return false;std::memcpy(memory.data()+offset,input,size);return true;
    }
}
namespace f=transform_fixture;
static std::uint64_t word(std::size_t offset){return s::word(s::bytes{memory}.subspan(offset,8),0);}
static void put(std::size_t offset,std::uint64_t value){s::put(std::span{memory}.subspan(offset,8),0,value);}
template<typename Fn>static std::size_t request(Fn fn)
{s::wire::writer out{std::span{memory}.subspan(128,4096)};fn(out);return out.size();}
template<c::operation Op>static unsigned call(s::handle cap,std::size_t size,std::size_t capacity=1024,std::uint64_t output=8192,std::uint64_t length=32768)
{return h::invoke_transform<Op>(cap,0,128,size,output,capacity,length);}
static c::name name(std::string_view text){c::name out;assert(out.assign(text));return out;}
static s::handle buffer,clock_id;
static std::size_t publication(std::uint64_t sequence)
{
    return request([&](auto& out)
    {
        assert(out.u64(buffer)&&out.u64(sequence)&&out.u64(buffer)&&out.u64(0)&&out.u64(6));
        for(std::int64_t i{};i<6;++i)
        {
            c::transform value{name("map"),name("base"),{clock_id,1,i*100000000},{{static_cast<double>(i),0,0},{}},false};
            assert(c::write_transform(out,value));
        }
    });
}
static void query(s::wire::writer& out,std::int64_t time)
{assert(out.text("map")&&out.boolean(true)&&c::write_stamp(out,{clock_id,1,time})&&out.text("base")&&out.boolean(false)&&out.boolean(false));}
static void debug_unchanged(std::shared_ptr<s::broker> const& state)
{
    s::debug::request query;query.selected=s::debug::selection::resources;s::debug::view view;
    auto& config=h::configuration;s::transforms::usage before,after;
    assert(state->transform_store.inspect("frames",before)==error::success);
    for(unsigned i{};i<3;++i)
    {
        config.inspect(query,view);assert(view.result==s::debug::status::ok);bool found{};
        for(std::size_t n{};n<view.size;++n)
        {
            auto const& row=view.rows[n];if(row.alias.view()!="frames"||row.type.view()!="transform")continue;found=true;
            assert(s::debug::valid(row)&&!row.id&&row.binding.view()=="local-transform"&&row.open_handles==before.open_handles&&
                row.members==before.retained_sessions&&row.sample_count==before.samples&&row.query_count==before.queries&&row.pending_queries==before.pending&&
                row.query_bytes==before.request_bytes&&row.carrier_bytes==before.carrier_bytes);
        }
        assert(found);
    }
    assert(state->transform_store.inspect("frames",after)==error::success&&after.pending==before.pending&&after.queries==before.queries&&after.samples==before.samples);
}
int main()
{
    auto& config=h::configuration;f::environment(config);f::configure(config);f::clock(config);
    for(auto alias:{"rate","denied"})
    {
        auto grant=f::grant(alias);if(grant.alias=="rate"){grant.limits.calls=0;grant.limits.uncertain=true;}else grant.actions.erase("transform.publish");
        assert(config.allow("robot",std::move(grant))==error::success&&config.transform_profile("robot",alias,f::profile())==error::success);
        for(std::size_t i{};i<f::limits.size();++i)assert(config.transform_limit("robot",alias,s::transforms::limit_names[i],f::limits[i])==error::success);
        assert(config.transform_definition("robot",alias,f::definition())==error::success);
    }
    assert(config.transform_budget("robot",{6,24,3000000,3000000,3000000,196608})==error::success);
    std::shared_ptr<s::broker> state;s::caller who{&identity,1};assert(config.select("main",who,state)==error::success);
    std::array<s::handle,3> caps{};unsigned at{};
    for(auto alias:{"frames","rate","denied"})assert(state->permissions.open(who,alias,caps[at++])==error::success);
    s::handle clock_cap{};assert(state->permissions.open(who,"clock",clock_cap)==error::success);
    assert(state->runtime.clock_create("clock",1,uwvm2::imported::ros::resources::clock_kind::ros,clock_id)==error::success&&
        state->runtime.clock_update("clock",clock_id,1000000000,true)==error::success);
    auto profile=f::profile();auto size=request([&](auto& out){assert(out.text(profile.identity.view())&&out.blob(profile.sha256)&&out.u64(clock_cap)&&out.u64(clock_id));});
    assert(call<c::operation::open>(caps[0],size,7)==90&&word(32768)==8&&!reads);
    fail_write=true;assert(call<c::operation::open>(caps[0],size)==5);fail_write=false;
    s::transforms::usage used;assert(state->transform_store.inspect("frames",used)==error::success&&!used.retained_sessions);
    assert(call<c::operation::open>(caps[0],size)==0);buffer=word(8192);assert(buffer);
    size=publication(1);assert(size>512);reads=large_attempts=0;reject_large=true;
    assert(call<c::operation::publish>(caps[0],size,159)==90&&word(32768)==160&&!reads&&!large_attempts);
    assert(call<c::operation::publish>(caps[1],size)==11&&!reads&&!large_attempts);
    assert(call<c::operation::publish>(caps[2],size)==13&&!reads&&!large_attempts);
    assert(call<c::operation::publish>(caps[0],size)==12&&!reads&&large_attempts==1);reject_large=false;
    assert(state->transform_store.inspect("frames",used)==error::success&&!used.carrier_bytes&&!used.samples);
    fail_read=true;assert(call<c::operation::publish>(caps[0],size)==22);fail_read=false;
    assert(state->transform_store.inspect("frames",used)==error::success&&!used.carrier_bytes&&!used.samples);
    assert(call<c::operation::publish>(caps[0],size,1024,8192,136)==22);
    assert(call<c::operation::publish>(caps[0],size,1024,8192,8200)==22);
    fail_write=true;assert(call<c::operation::publish>(caps[0],size)==5);fail_write=false;
    assert(state->transform_store.inspect("frames",used)==error::success&&!used.carrier_bytes&&used.samples==6);
    assert(call<c::operation::publish>(caps[0],size)==0); // exact mutation replay
    size=request([&](auto& out){assert(out.u64(buffer));query(out,250000000);});
    assert(call<c::operation::lookup>(caps[0],size,621)==90&&word(32768)==622);
    assert(call<c::operation::lookup>(caps[0],size)==0);
    c::transform transform;s::wire::reader lookup{s::bytes{memory}.subspan(8192,word(32768))};assert(c::read_transform(lookup,transform)&&lookup.done()&&transform.pose.translation.x==2.5);
    assert(call<c::operation::available>(caps[0],size)==0&&word(8192)==1);
    size=request([&](auto& out)
    {assert(out.u64(buffer)&&out.text("base")&&out.text("map")&&out.text("map")&&c::write_vector(out,{})&&out.text("base")&&c::write_stamp(out,{clock_id,1,500000000})&&out.u64(500000000));});
    assert(call<c::operation::velocity>(caps[0],size)==0);
    s::wire::reader velocity{s::bytes{memory}.subspan(8192,word(32768))};double speed{};assert(velocity.f64(speed)&&speed==10);
    size=request([&](auto& out){assert(out.u64(buffer)&&out.u64(2)&&out.u64(buffer));query(out,250000000);assert(out.u64(uwvm2::imported::ros::resources::steady_now()+500000000));});
    assert(call<c::operation::submit>(caps[0],size)==0);auto query_id=word(8192);
    put(128,buffer);put(136,query_id);fail_write=true;assert(call<c::operation::take>(caps[0],16)==5);fail_write=false;
    assert(call<c::operation::take>(caps[0],16,789)==90&&word(32768)==790);
    assert(call<c::operation::take>(caps[0],16)==0);
    put(128,query_id);namespace m=uwvm2::imported::ros::model_contract;
    assert(h::invoke_model<m::operation::request_describe>(caps[0],0,128,8,8192,160,32768)==0);
    assert(h::invoke_model<m::operation::request_release>(caps[0],0,128,8,8192,8,32768)==0&&word(8192)==1);
    size=request([&](auto& out){assert(out.u64(buffer)&&out.u64(3)&&out.u64(buffer));query(out,900000000);assert(out.u64(uwvm2::imported::ros::resources::steady_now()+500000000));});
    assert(call<c::operation::submit>(caps[0],size)==0);query_id=word(8192);debug_unchanged(state);
    put(128,buffer);put(136,query_id);assert(call<c::operation::cancel>(caps[0],16)==0);
    size=request([&](auto& out){assert(out.u64(buffer)&&out.u64(4)&&out.u64(buffer)&&out.u64(2));});
    assert(call<c::operation::clear>(caps[0],size)==0);
    size=request([&](auto& out){assert(out.u64(buffer));query(out,250000000);});
    // All request bytes are owned before the output overwrites the request.
    assert(call<c::operation::available>(caps[0],size,1024,128)==0&&word(128)==0);
    assert(state->transform_store.inspect("frames",used)==error::success&&!used.samples&&!used.carrier_bytes);
    assert(state->revoke("frames")==error::success);assert(call<c::operation::available>(caps[0],size)==13);
    std::puts("PASS nine actual transform carriers, shared requests, recoverable allocation, copied memory, receipt recovery and read-only debug");
}
