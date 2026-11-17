/** @file
 * @brief Verify scene wrapper FP restoration around standard allocation hooks.
 * @details Native mode uses the actual standard allocator/new_handler under a
 * reduced process address-space limit. Injection mode deterministically calls
 * the installed standard new_handler at the next allocation to cover initial
 * caller enrollment and scheduling allocation. Both invoke wrapper::call,
 * not the raw broker or invoke function. Runtime identity/memory are fixtures.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include "../0008.imported/wasi/wasip1/func/fp_control_probe.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <sys/resource.h>
#include <unistd.h>

namespace host=uwvm2::uwvm::imported::scene;
namespace s=uwvm2::imported::ros::scene;
namespace probe=uwvm2test::wasip1_fp_control;
using output_op=uwvm2::imported::ros::output_contract::operation;
using runtime_op=uwvm2::imported::ros::runtime_contract::operation;
static std::array<std::uint8_t,8192> memory{};
static std::array<int,2> identities{};
static unsigned identity_index{},handler_calls{};
static void* reclaim{};
#ifdef UWVM_SCENE_ALLOCATOR_INJECTION
static bool fail_next{};
void* operator new(std::size_t size)
{
    if(fail_next){fail_next=false;auto handler=std::get_new_handler();assert(handler);handler();throw std::bad_alloc{};}
    if(auto value=std::malloc(size?size:1))return value;
    throw std::bad_alloc{};
}
void* operator new[](std::size_t size){return ::operator new(size);}
[[gnu::noinline]] static void release(void* value) noexcept{std::free(value);}
void operator delete(void* value) noexcept{release(value);}
void operator delete[](void* value) noexcept{release(value);}
void operator delete(void* value,std::size_t) noexcept{release(value);}
void operator delete[](void* value,std::size_t) noexcept{release(value);}
#endif
namespace uwvm2::runtime::lib
{
    bool ros_caller_identity_host_api(ros_caller_identity& out,bool) noexcept
    {out={&identities[identity_index],1,{u8"main"},false};return true;}
    bool preload_memory_range_host_api(std::size_t index,std::uint_least64_t offset,std::size_t size) noexcept
    {return !index&&offset<=memory.size()&&size<=memory.size()-offset;}
    bool preload_memory_normalize_range_host_api(std::size_t index,std::uint_least64_t offset,std::size_t size,std::uint64_t& normalized) noexcept
    {normalized=offset;return preload_memory_range_host_api(index,offset,size);}
    bool preload_memory_read_host_api(std::size_t index,std::uint_least64_t offset,void* target,std::size_t size) noexcept
    {if(!preload_memory_range_host_api(index,offset,size))return false;std::memcpy(target,memory.data()+offset,size);return true;}
    bool preload_memory_write_host_api(std::size_t index,std::uint_least64_t offset,void const* source,std::size_t size) noexcept
    {if(!preload_memory_range_host_api(index,offset,size))return false;std::memcpy(memory.data()+offset,source,size);return true;}
}
static void hostile_handler()
{
    ++handler_calls;assert(std::fesetround(FE_DOWNWARD)==0);probe::enable_flush_control();
    if(reclaim){std::free(reclaim);reclaim=nullptr;return;}
    throw std::bad_alloc{};
}
template<auto Fn,auto& Name> static unsigned invoke(s::handle cap,std::size_t request_size,std::size_t capacity,unsigned expected=12)
{
    using wrapper=host::wrapper::native<Fn,Name>;
    static_assert(wrapper::guards_fp_controls);
    typename wrapper::local_imported_function_type call{};
    uwvm2::utils::container::get<0>(call.params)=cap;
    uwvm2::utils::container::get<1>(call.params)=0;
    uwvm2::utils::container::get<2>(call.params)=128;
    uwvm2::utils::container::get<3>(call.params)=request_size;
    uwvm2::utils::container::get<4>(call.params)=5120;
    uwvm2::utils::container::get<5>(call.params)=capacity;
    uwvm2::utils::container::get<6>(call.params)=6000;
    auto before=probe::read_arch_control();auto rounding=std::fegetround();auto calls=handler_calls;
    wrapper::call(call);
    assert(handler_calls==calls+1&&std::fegetround()==rounding&&probe::read_arch_control()==before);
    auto status=static_cast<unsigned>(uwvm2::utils::container::get<0>(call.res));assert(status==expected);return status;
}
int main()
{
    probe::initial_state_restore restore;assert(restore.valid&&std::fesetenv(FE_DFL_ENV)==0);
    auto& config=host::configuration;assert(config.create("robot")==s::error::success&&config.bind("robot","main")==s::error::success);
    for(auto name:{"capability.open","artifact.begin","artifact.append","artifact.status","guard.create"})
        assert(config.expose("robot",name,s::exposure::allow)==s::error::success);
    s::grant cache;cache.alias="cache";cache.resource="cache";cache.binding="volatile-output";cache.kind=s::purpose::cache;
    cache.actions={"artifact.begin","artifact.append","artifact.status"};assert(config.allow("robot",cache)==s::error::success);
    assert(config.output_budget("robot",64u<<20,1,4192)==s::error::success);
    assert(config.output("robot","cache",{64u<<20,1,64u<<20,4096})==s::error::success);
    s::grant guard;guard.alias="guard";guard.resource="guard";guard.binding="local";guard.kind=s::purpose::guard;guard.actions={"guard.create"};
    assert(config.allow("robot",guard)==s::error::success);
    assert(config.prepare_resources([](auto const&,auto const&,auto,auto&){assert(false);return s::error::denied;})==s::error::success);
    std::shared_ptr<s::broker> broker;s::caller who{&identities[0],1};assert(config.select("main",who,broker)==s::error::success);
    s::handle cache_cap{},guard_cap{};assert(broker->permissions.open(who,"cache",cache_cap)==s::error::success);
    assert(broker->permissions.open(who,"guard",guard_cap)==s::error::success);
    s::put(memory,16,1);s::put(memory,17,64u<<20);
    auto previous=std::set_new_handler(hostile_handler);
#ifdef UWVM_SCENE_ALLOCATOR_INJECTION
    identity_index=1;fail_next=true;
    invoke<&host::invoke_output<output_op::status>,host::wrapper::artifact_status_name>(cache_cap,8,72);
    identity_index=0;fail_next=true;
    invoke<&host::invoke<runtime_op::guard_create>,host::wrapper::guard_create_name>(guard_cap,0,8);
    fail_next=true;invoke<&host::invoke_output<output_op::begin>,host::wrapper::artifact_begin_name>(cache_cap,16,72);
    fail_next=true;invoke<&host::invoke_output<output_op::append>,host::wrapper::artifact_append_name>(cache_cap,4120,72);
    std::puts("PASS real wrappers: allocation handler during initial enrollment, guard.create, artifact.begin and append scratch");
#else
    FILE* stat=std::fopen("/proc/self/statm","r");assert(stat);unsigned long pages{};assert(std::fscanf(stat,"%lu",&pages)==1);std::fclose(stat);
    rlimit old{};assert(::getrlimit(RLIMIT_AS,&old)==0);rlimit limited=old;
    limited.rlim_cur=pages*static_cast<unsigned long>(::sysconf(_SC_PAGESIZE))+(1u<<20);
    assert(::setrlimit(RLIMIT_AS,&limited)==0);
    invoke<&host::invoke_output<output_op::begin>,host::wrapper::artifact_begin_name>(cache_cap,16,72);
    assert(::setrlimit(RLIMIT_AS,&old)==0);
    reclaim=std::malloc(64u<<20);assert(reclaim);
    stat=std::fopen("/proc/self/statm","r");assert(stat);assert(std::fscanf(stat,"%lu",&pages)==1);std::fclose(stat);
    limited.rlim_cur=pages*static_cast<unsigned long>(::sysconf(_SC_PAGESIZE))+(1u<<20);assert(::setrlimit(RLIMIT_AS,&limited)==0);
    invoke<&host::invoke_output<output_op::begin>,host::wrapper::artifact_begin_name>(cache_cap,16,72,0);
    assert(::setrlimit(RLIMIT_AS,&old)==0&&!reclaim);
    std::puts("PASS real wrapper: standard allocator/new_handler throwing and reclaim-return-success paths preserve controls");
#endif
    std::set_new_handler(previous);
    s::outputs::receipt receipt;
#ifdef UWVM_SCENE_ALLOCATOR_INJECTION
    assert(broker->output_store.receipt_for("cache",1,receipt)==s::error::conflict);
#else
    assert(broker->output_store.receipt_for("cache",1,receipt)==s::error::success&&receipt.state==1&&receipt.retained_bytes==(64u<<20));
#endif
}
