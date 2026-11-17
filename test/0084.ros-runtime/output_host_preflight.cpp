/** @file
 * @brief Exercise the real output host wrapper before request allocation/copy.
 * @details Only runtime identity and checked linear-memory access are fixtures.
 * The production host, deployment, authority and provider execute unchanged.
 * Large-allocation failure proves refused or undersized requests cannot start
 * the variable append copy. This does not substitute for tagged-memory VM tests.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

namespace host=uwvm2::uwvm::imported::scene;
namespace scene=uwvm2::imported::ros::scene;
using op=uwvm2::imported::ros::output_contract::operation;
using error=scene::error;
static bool reject_large{};
static bool fail_partial_read{};
static std::size_t large_attempts{},request_reads{};
static std::array<std::uint8_t,65536> guest_memory{};
static int runtime_identity{};

void* operator new(std::size_t size)
{
    if(reject_large&&size>=1024){++large_attempts;throw std::bad_alloc{};}
    if(auto result=std::malloc(size?size:1))return result;
    throw std::bad_alloc{};
}
void* operator new[](std::size_t size){return ::operator new(size);}
[[gnu::noinline]] static void release_allocation(void* value) noexcept{std::free(value);}
void operator delete(void* value) noexcept{release_allocation(value);}
void operator delete[](void* value) noexcept{release_allocation(value);}
void operator delete(void* value,std::size_t) noexcept{release_allocation(value);}
void operator delete[](void* value,std::size_t) noexcept{release_allocation(value);}

namespace uwvm2::runtime::lib
{
    bool ros_caller_identity_host_api(ros_caller_identity& out,bool) noexcept
    {out={&runtime_identity,1,{u8"main"},false};return true;}
    bool preload_memory_range_host_api(std::size_t memory,std::uint_least64_t offset,std::size_t size) noexcept
    {return !memory&&offset<=guest_memory.size()&&size<=guest_memory.size()-offset;}
    bool preload_memory_normalize_range_host_api(std::size_t memory,std::uint_least64_t offset,
                                                 std::size_t size,std::uint64_t& normalized) noexcept
    {normalized=offset;return preload_memory_range_host_api(memory,offset,size);}
    bool preload_memory_read_host_api(std::size_t memory,std::uint_least64_t offset,void* target,std::size_t size) noexcept
    {
        if(!preload_memory_range_host_api(memory,offset,size))return false;
        ++request_reads;
        if(fail_partial_read)
        {std::memcpy(target,guest_memory.data()+offset,size/2);return false;}
        std::memcpy(target,guest_memory.data()+offset,size);return true;
    }
    bool preload_memory_write_host_api(std::size_t memory,std::uint_least64_t offset,void const* source,std::size_t size) noexcept
    {
        if(!preload_memory_range_host_api(memory,offset,size))return false;
        std::memcpy(guest_memory.data()+offset,source,size);return true;
    }
}

static std::uint64_t word(std::size_t offset){return scene::word(scene::bytes{guest_memory}.subspan(offset,8),0);}
static void put(std::size_t offset,std::uint64_t value){scene::put(std::span{guest_memory}.subspan(offset,8),0,value);}
static void prepare_request()
{
    guest_memory.fill(0x5a);put(128,1);put(136,9999);put(144,0);
    put(8192,0x123456789abcdef);put(16384,0x456789abcdef);
    request_reads=large_attempts=0;
}

int main()
{
    auto& config=host::configuration;
    assert(config.create("robot")==error::success&&config.bind("robot","main")==error::success);
    for(auto action:{"capability.open","artifact.append","artifact.status"})
        assert(config.expose("robot",action,scene::exposure::allow)==error::success);
    for(auto alias:{"cache","calls","bytes","denied"})
    {
        scene::grant grant;grant.alias=alias;grant.resource=alias;grant.kind=scene::purpose::cache;
        grant.binding="volatile-output";grant.actions={"artifact.status"};
        if(grant.alias!="denied")grant.actions.insert("artifact.append");
        if(grant.alias=="calls"){grant.limits.uncertain=true;grant.limits.calls=0;}
        if(grant.alias=="bytes"){grant.limits.uncertain=true;grant.limits.bytes=0;}
        assert(config.allow("robot",grant)==error::success);
        assert(config.output("robot",alias,{65536,1,65536,4096})==error::success);
    }
    assert(config.output_budget("robot",4*65536,4,4*4192)==error::success);
    unsigned provider_calls{};
    assert(config.prepare_resources([&](auto const&,auto const&,auto,auto&){++provider_calls;return error::denied;})==error::success);
    assert(!provider_calls);
    scene::caller who{&runtime_identity,1};std::shared_ptr<scene::broker> state;
    assert(config.select_exposed("main","artifact.append",who,state)==error::success);
    std::array<scene::handle,4> caps{};std::size_t index{};
    for(auto alias:{"cache","calls","bytes","denied"})
        assert(state->permissions.open(who,alias,caps[index++])==error::success);
    auto invoke=[](scene::handle cap,std::size_t request_size,std::size_t capacity)
    {return host::invoke_output<op::append>(cap,0,128,request_size,8192,capacity,16384);};
    auto no_effect=[&]
    {
        scene::outputs::receipt receipt;
        assert(state->output_store.receipt_for("cache",1,receipt)==error::conflict);
        assert(word(8192)==0x123456789abcdef);
    };

    prepare_request();reject_large=true;
    auto result=invoke(caps[0],4120,71);reject_large=false;
    assert(result==90&&word(16384)==72&&!request_reads&&!large_attempts);no_effect();

    // Positive injection control: sufficient output reaches the actual variable
    // request allocation. Failure still cannot read data or retain a mutation.
    prepare_request();reject_large=true;
    result=invoke(caps[0],4120,72);reject_large=false;
    assert(result==12&&!request_reads&&large_attempts==1);no_effect();

    prepare_request();reject_large=true;
    result=invoke(caps[0],4121,72);reject_large=false;
    assert(result==90&&word(16384)==0&&!request_reads&&!large_attempts);no_effect();

    for(std::size_t limited=1;limited!=caps.size();++limited)
    {
        prepare_request();reject_large=true;
        result=invoke(caps[limited],4120,72);reject_large=false;
        assert(result==(limited==3?13u:11u)&&!request_reads&&!large_attempts);no_effect();
        if(limited<3)
        {
            prepare_request();reject_large=true;
            result=invoke(caps[limited],4120,71);reject_large=false;
            assert(result==90&&word(16384)==72&&!request_reads&&!large_attempts);no_effect();
        }
    }
    prepare_request();reject_large=true;
    result=invoke(UINT64_MAX,4120,72);reject_large=false;
    assert(result==9&&!request_reads&&!large_attempts);no_effect();

    // A reader can initialize only a prefix before reporting failure. Neither
    // stack nor heap overwrite storage may be parsed or delivered in that case.
    // Probe both sides of the 512-byte local-buffer boundary as well as a full
    // transfer; the next mutation key and both output canaries must survive.
    for(auto size:{32u,512u,513u,4120u})
    {
        prepare_request();fail_partial_read=true;
        result=invoke(caps[0],size,72);fail_partial_read=false;
        assert(result==22&&request_reads==1&&word(16384)==0x456789abcdef);
        no_effect();
    }

    // The same first key remains available after every preflight refusal.
    // Unknown object is an admitted effect failure with a recoverable receipt.
    prepare_request();assert(invoke(caps[0],4120,72)==0&&request_reads==1);
    assert(word(16384)==72&&word(8192)==1&&word(8192+24)==9);
    scene::outputs::receipt receipt;
    assert(state->output_store.receipt_for("cache",1,receipt)==error::success&&receipt.key==1&&receipt.effect_status==9);
    std::puts("PASS real output host preflight, allocation failure, partial-copy rejection, zero-copy refusal and retained-key recovery");
}
