/** @file
 * @brief Exercise seven real model/request carriers with controlled runtime memory.
 * @details Runtime identity/memory are fixtures. Production host, deployment,
 * authority, copied carriers and job lifecycle run unchanged. A trusted native
 * arithmetic consumer exercises the worker facade; this does not substitute
 * for real Wasm tagged-memory tests or isolated native process qualification.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include "model_deployment_fixture.h"
#include <cassert>
#include <cstdio>
#include <cstring>
namespace h=uwvm2::uwvm::imported::scene;
namespace s=uwvm2::imported::ros::scene;
namespace c=uwvm2::imported::ros::model_contract;
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
struct native_owner final:s::models::provider_lifetime
{
    void request_stop() noexcept override{}
    void retire() noexcept override{}
};
static std::uint64_t word(std::size_t offset){return s::word(s::bytes{memory}.subspan(offset,8),0);}
static void put(std::size_t offset,std::uint64_t value){s::put(std::span{memory}.subspan(offset,8),0,value);}
static s::models::plan policy()
{
    s::models::plan value;
    assert(value.model.identity.assign("bounded.model")&&value.task.identity.assign("bounded.task"));
    value.model.sha256.fill(11);value.task.sha256.fill(13);
    assert(value.parameter_type.assign("parameters")&&value.output_type.assign("result")&&s::models::select_codec("bytes",value));
    return value;
}
template<c::operation Op>static unsigned call(s::handle cap,std::size_t size,std::size_t capacity=512,std::uint64_t output=8192,std::uint64_t length=32768)
{return h::invoke_model<Op>(cap,0,128,size,output,capacity,length);}
static std::size_t open_request()
{s::wire::writer out{std::span{memory}.subspan(128,4096)};assert(c::write_profile(out,policy().model));return out.size();}
static std::size_t submit_request(s::handle session,std::uint64_t sequence,std::uint64_t deadline,std::size_t parameters)
{
    static std::array<std::uint8_t,2048> body{};body.fill(7);
    s::wire::writer out{std::span{memory}.subspan(128,4096)};
    assert(out.u64(session)&&out.u64(sequence)&&out.u64(session)&&c::write_profile(out,policy().task)&&
        out.text("parameters")&&out.text("bytes")&&out.blob({body.data(),parameters})&&out.u64(0)&&
        out.u64(16)&&out.u64(2)&&out.u64(4096)&&out.u64(deadline));return out.size();
}
int main()
{
    auto& config=h::configuration;assert(config.create("robot")==error::success&&config.bind("robot","main")==error::success);
    assert(config.expose("robot","capability.open",s::exposure::allow)==error::success);
    for(auto const& descriptor:c::operations)
        assert(config.expose("robot",uwvm2::utils::container::std_string(descriptor.name),s::exposure::allow)==error::success);
    for(auto alias:{"brain","rate","denied"})
    {
        s::grant grant;grant.alias=alias;grant.resource="inference";grant.binding="local-model";grant.kind=s::purpose::vision_model;
        for(auto const& descriptor:c::operations)grant.actions.emplace(descriptor.name);
        if(grant.alias=="rate"){grant.limits.calls=0;grant.limits.uncertain=true;}
        if(grant.alias=="denied")grant.actions.erase("model.submit");
        assert(config.allow("robot",std::move(grant))==error::success&&config.model_plan("robot",alias,policy())==error::success);
        assert(config.model_limits("robot",alias,{1,2,0,0,2048,32,4,16384,60000000000})==error::success);
        configure_inert_model_worker(config,"robot",alias);
    }
    assert(config.model_budget("robot",{3,6,0,96,12,49152})==error::success);
    auto no_inputs=[](auto const&,auto const&,auto,auto&){assert(false);return error::denied;};
    std::uint64_t provider_generation{};
    auto attach=[&](auto,auto alias,auto const&,auto const& manifest,std::uint64_t generation,s::model_dispatch& state,std::shared_ptr<s::models::provider_lifetime>& owner)
    {assert(manifest.frozen());provider_generation=generation;owner=uwvm2::utils::container::make_shared<native_owner>();return state.attach(alias,generation);};
    assert(config.prepare_resources(no_inputs,attach)==error::success);
    std::shared_ptr<s::broker> state;s::caller who{&identity,1};assert(config.select("main",who,state)==error::success);
    std::array<s::handle,3> caps{};unsigned index{};
    for(auto alias:{"brain","rate","denied"})assert(state->permissions.open(who,alias,caps[index++])==error::success);
    auto size=open_request();assert(call<c::operation::open>(caps[0],size,7)==90&&word(32768)==8&&!reads);
    fail_write=true;assert(call<c::operation::open>(caps[0],size)==5);fail_write=false;
    s::models::usage used;assert(state->model_store.try_inspect("brain",used)==error::success&&!used.sessions);
    assert(call<c::operation::open>(caps[0],size)==0);auto session=word(8192);assert(session);
    put(128,session);assert(call<c::operation::describe>(caps[0],8)==0);
    s::wire::reader description{s::bytes{memory}.subspan(8192,word(32768))};s::models::profile model;
    assert(c::read_profile(description,model)&&model==policy().model);
    auto deadline=s::models::now()+30000000000;size=submit_request(session,1,deadline,1024);
    reads=large_attempts=0;reject_large=true;
    assert(call<c::operation::submit>(caps[0],size,159)==90&&word(32768)==160&&!reads&&!large_attempts);
    assert(call<c::operation::submit>(caps[1],size)==11&&!reads&&!large_attempts);
    assert(call<c::operation::submit>(caps[2],size)==13&&!reads&&!large_attempts);
    assert(call<c::operation::submit>(caps[0],size)==12&&!reads&&large_attempts==1);reject_large=false;
    assert(state->model_store.try_inspect("brain",used)==error::success&&!used.workspace&&!used.requests);
    fail_read=true;assert(call<c::operation::submit>(caps[0],size)==22);fail_read=false;
    assert(state->model_store.try_inspect("brain",used)==error::success&&!used.workspace&&!used.requests);
    assert(call<c::operation::submit>(caps[0],size,512,8192,136)==22); // Length must not overwrite the request.
    assert(call<c::operation::submit>(caps[0],size,512,8192,8200)==22); // Or the result.
    fail_write=true;assert(call<c::operation::submit>(caps[0],size)==5);fail_write=false;
    assert(state->model_store.try_inspect("brain",used)==error::success&&used.requests==1&&used.workspace==4096);
    assert(call<c::operation::submit>(caps[0],size)==0);auto request=word(8192);assert(request);
    s::model_dispatch::work work;assert(state->model_store.claim("brain",provider_generation,work)==error::success&&work.identity()==request);
    assert(work.with_parameters([](auto bytes){return bytes.size()==1024&&bytes[0]==7;})==error::success);
    std::array<std::uint8_t,1> prediction{8};assert(work.finish(s::models::state::succeeded,prediction)==error::success);
    put(128,request);assert(call<c::operation::request_describe>(caps[0],8)==0);
    put(128,session);put(136,request);fail_write=true;assert(call<c::operation::take>(caps[0],16)==5);fail_write=false;
    assert(call<c::operation::take>(caps[0],16,512,128)==0); // Request/result alias uses the complete owned request.
    put(128,request);assert(call<c::operation::request_release>(caps[0],8)==0&&word(8192)==1);
    size=submit_request(session,1,deadline,1024);assert(call<c::operation::submit>(caps[0],size)==0&&word(8192)==request);
    size=submit_request(session,2,deadline,1);assert(call<c::operation::submit>(caps[0],size)==0);request=word(8192);
    put(128,session);put(136,request);assert(call<c::operation::cancel>(caps[0],16)==0);
    put(128,request);assert(call<c::operation::request_release>(caps[0],8)==0&&word(8192)==1);
    s::debug::request query;query.selected=s::debug::selection::resources;query.alias.assign("brain");s::debug::view view;
    config.inspect(query,view);assert(view.result==s::debug::status::ok&&view.size==1&&view.rows[0].type.view()=="model");
    assert(view.rows[0].members==1&&view.rows[0].output_bytes==16&&view.rows[0].workspace_bytes==4096);
    work={}; // Releasing a guest receipt cannot prematurely refund a native-held job.
    config.inspect(query,view);assert(view.result==s::debug::status::ok&&view.size==1);
    assert(view.rows[0].open_handles==1&&!view.rows[0].members&&!view.rows[0].input_bytes&&!view.rows[0].output_bytes&&!view.rows[0].workspace_bytes);
    std::puts("PASS seven model carriers, recoverable fast_io failure, exact replay, memory aliasing, cleanup and metadata-only inspection");
}
