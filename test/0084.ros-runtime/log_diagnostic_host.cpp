/** @file
 * @brief Exercise both real log/diagnostic carriers with controlled runtime memory.
 * @details Runtime identity/memory are fixtures. Production host, deployment,
 * authority, copied carriers and finite private stores run unchanged. This fixture
 * complements actual Wasm tagged-memory and floating-point boundary checks.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include <cassert>
#include <cstdio>
#include <cstring>
namespace h=uwvm2::uwvm::imported::scene;
namespace s=uwvm2::imported::ros::scene;
namespace c=uwvm2::imported::ros::log_diagnostic_contract;
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
static std::uint64_t word(std::size_t offset){return s::word(s::bytes{memory}.subspan(offset,8),0);}
template<typename Fn>static std::size_t request(Fn fn)
{s::wire::writer out{std::span{memory}.subspan(128,4096)};fn(out);return out.size();}
template<c::operation Op>static unsigned call(s::handle cap,std::size_t size,std::size_t capacity=1024,std::uint64_t output=8192,std::uint64_t length=32768)
{return h::invoke_log_diagnostic<Op>(cap,0,128,size,output,capacity,length);}
static std::size_t record(std::uint64_t sequence=1)
{
    std::array<char,800> content;content.fill('x');s::logs::request value;value.session=19;value.sequence=sequence;
    value.level=s::logs::severity::info;value.logger="robot";value.message={content.data(),content.size()};
    return request([&](auto& out){assert(s::logs::encode(out,value));});
}
int main()
{
    auto& config=h::configuration;assert(config.create("robot")==error::success&&config.bind("robot","main")==error::success);
    for(auto action:{"capability.open","log.emit","diagnostic.read"})assert(config.expose("robot",action,s::exposure::allow)==error::success);
    for(auto alias:{"log","rate","denied"})
    {
        s::grant value;value.alias=alias;value.resource=alias;value.kind=s::purpose::log;value.binding="private-log";
        value.actions.emplace("resource.observe");if(value.alias!="denied")value.actions.emplace("log.emit");
        if(value.alias=="rate"){value.limits.calls=0;value.limits.uncertain=true;}
        assert(config.allow("robot",std::move(value))==error::success);
        constexpr std::array<std::uint64_t,4> limits{2,1024,8192,31};
        for(std::size_t i{};i<limits.size();++i)assert(config.log_limit("robot",alias,s::logs::configuration::limit_names[i],limits[i])==error::success);
        assert(config.log_logger("robot",alias,"robot")==error::success&&config.log_loss("robot",alias,"reject")==error::success);
    }
    constexpr std::array<std::uint64_t,6> budgets{3,1,32768,1024,2048,2};
    for(std::size_t i{};i<budgets.size();++i)assert(config.log_diagnostic_budget_limit("robot",s::log_diagnostic_budget_configuration::names[i],budgets[i])==error::success);
    s::grant diagnostic;diagnostic.alias="health";diagnostic.resource="counters";diagnostic.kind=s::purpose::diagnostic;
    diagnostic.binding="local-diagnostic";diagnostic.actions.emplace("diagnostic.read");assert(config.allow("robot",diagnostic)==error::success);
    constexpr std::array<std::uint64_t,7> limits{37,7,3,2,512,60000000000,1000000000};
    for(std::size_t i{};i<limits.size();++i)assert(config.diagnostic_limit("robot","health",s::diagnostics::configuration::limit_names[i],limits[i])==error::success);
    assert(config.diagnostic_source("robot","health","log")==error::success);
    std::shared_ptr<s::broker> state;s::caller who{&identity,1};assert(config.select("main",who,state)==error::success);
    std::array<s::handle,4> caps{};unsigned at{};
    for(auto alias:{"log","rate","denied","health"})assert(state->permissions.open(who,alias,caps[at++])==error::success);
    auto size=record();assert(size>512);reads=large_attempts=0;reject_large=true;
    assert(call<c::operation::emit>(caps[0],size,47)==90&&word(32768)==48&&!reads&&!large_attempts);
    assert(call<c::operation::emit>(caps[1],size)==11&&!reads&&!large_attempts);
    assert(call<c::operation::emit>(caps[2],size)==13&&!reads&&!large_attempts);
    assert(call<c::operation::emit>(caps[0],size)==12&&!reads&&large_attempts==1);reject_large=false;
    s::logs::usage usage;assert(state->log_diagnostic_store->inspect_log("log",usage)==error::success&&!usage.accepted);
    fail_read=true;assert(call<c::operation::emit>(caps[0],size)==22);fail_read=false;
    assert(state->log_diagnostic_store->inspect_log("log",usage)==error::success&&!usage.accepted);
    assert(call<c::operation::emit>(caps[0],size,1024,8192,136)==22&&call<c::operation::emit>(caps[0],size,1024,8192,8200)==22);
    fail_write=true;assert(call<c::operation::emit>(caps[0],size)==5);fail_write=false;
    assert(state->log_diagnostic_store->inspect_log("log",usage)==error::success&&usage.accepted==1);
    assert(call<c::operation::emit>(caps[0],size)==0&&word(8208)==1&&word(8224)==size);
    size=record(2);assert(call<c::operation::emit>(caps[0],size,1024,128)==0&&word(144)==2);
    size=request([&](auto& out){assert(out.u64(37)&&out.u64(caps[0])&&out.boolean(false)&&out.u64(2));});
    assert(call<c::operation::read>(caps[3],size,95)==90&&word(32768)==96);
    fail_write=true;assert(call<c::operation::read>(caps[3],size)==5);fail_write=false;
    s::diagnostics::usage before,after;assert(state->log_diagnostic_store->inspect_diagnostic("health",before)==error::success&&before.pending);
    s::debug::request query;query.selected=s::debug::selection::resources;s::debug::view view;config.inspect(query,view);
    assert(view.result==s::debug::status::ok&&view.size==4);
    assert(state->log_diagnostic_store->inspect_diagnostic("health",after)==error::success&&after.revision==before.revision&&after.expires==before.expires&&after.pending);
    assert(call<c::operation::read>(caps[3],size)==0&&word(8200)==1);auto cursor=word(8208);
    size=request([&](auto& out){assert(out.u64(37)&&out.u64(caps[0])&&out.boolean(true)&&out.u64(cursor)&&out.u64(1));});
    assert(call<c::operation::read>(caps[3],size)==0&&word(8200)==0&&word(8208)==1);
    assert(state->revoke("log")==error::success&&call<c::operation::read>(caps[3],size)==13);
    std::puts("PASS real log/diagnostic carriers, copied input, preflight, fast_io allocation recovery, retained receipts/pages and detached inspection");
}
