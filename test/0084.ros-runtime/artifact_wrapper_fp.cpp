/** @file
 * @brief Qualify actual artifact wrapper FP guards with recoverable fast_io failures.
 * @details Runtime identity and checked-memory APIs are fixtures; the concrete
 * wrapper, deployment, authority, sealed-memfd reads and volatile cache are real.
 * Forty-eight x86 control profiles cover masks, rounding, denormals and x87
 * precision. Hostile identity/memory/cancellation/allocation callbacks change
 * controls before returning success or failure. These deliberately unguarded
 * memory fixtures stress the outer wrapper; actual runtime imported-memory
 * guards and tagged pointers require the separate VM qualification.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include "../0008.imported/wasi/wasip1/func/fp_control_probe.h"
#include <cassert>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/syscall.h>
namespace h=uwvm2::uwvm::imported::scene;
namespace s=uwvm2::imported::ros::scene;
namespace p=uwvm2test::wasip1_fp_control;
using op=uwvm2::imported::ros::artifact_contract::operation;
using error=s::error;
static std::array<std::uint8_t,32768> memory{};
static std::array<int,50> identities{};
static unsigned identity_index{},hostile_calls{},allocation_calls{};
static int hostile_stage{-1},failed_stage{-1},allocation_after{-1};
static bool allocation_fails{},pread_fails{};
static void poison() noexcept
{
    ++hostile_calls;
    unsigned short cw;__asm__ volatile("fnstcw %0":"=m"(cw)::"memory");
    cw=static_cast<unsigned short>((cw&~0x0f3fu)|0x0800u|0x003bu);
    __asm__ volatile("fnclex\n\tfldcw %0"::"m"(cw):"memory");
    _mm_setcsr(0x0000df40u); // Different rounding, FTZ/DAZ, division exception unmasked; no pending status.
}
static bool allocate_probe() noexcept
{
    if(allocation_after<0)return false;
    ++allocation_calls;
    if(allocation_after--!=0)return false;
    poison();if(allocation_fails){errno=ENOMEM;return true;}return false;
}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" ssize_t __real_pread(int,void*,std::size_t,off_t);
extern "C" void* __wrap_malloc(std::size_t n){return allocate_probe()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t size){return allocate_probe()?nullptr:__real_calloc(n,size);}
extern "C" void* __wrap_realloc(void* old,std::size_t n){return allocate_probe()?nullptr:__real_realloc(old,n);}
extern "C" ssize_t __wrap_pread(int fd,void* bytes,std::size_t n,off_t off)
{if(pread_fails){poison();errno=EIO;return -1;}return __real_pread(fd,bytes,n,off);}
static bool stage(int value) noexcept
{if(hostile_stage==value)poison();return failed_stage!=value;}
namespace uwvm2::runtime::lib
{
    bool ros_caller_identity_host_api(ros_caller_identity& out,bool) noexcept
    {out={&identities[identity_index],1,{u8"main"},false};return stage(0);}
    bool preload_memory_range_host_api(std::size_t index,std::uint_least64_t offset,std::size_t size) noexcept
    {return !index&&offset<=memory.size()&&size<=memory.size()-offset;}
    bool preload_memory_normalize_range_host_api(std::size_t index,std::uint_least64_t offset,std::size_t size,std::uint64_t& normalized) noexcept
    {normalized=offset;return stage(1)&&preload_memory_range_host_api(index,offset,size);}
    bool preload_memory_read_host_api(std::size_t index,std::uint_least64_t offset,void* out,std::size_t size) noexcept
    {if(!preload_memory_range_host_api(index,offset,size))return false;
     bool okay=stage(2);std::memcpy(out,memory.data()+offset,okay?size:size/2);return okay;}
    bool preload_memory_write_host_api(std::size_t index,std::uint_least64_t offset,void const* data,std::size_t size) noexcept
    {if(!preload_memory_range_host_api(index,offset,size)||!stage(offset==24576?3:4))return false;
     std::memcpy(memory.data()+offset,data,size);return true;}
}
struct controls
{
    int rounding{std::fegetround()};std::uint_least64_t arch{p::read_arch_control()};unsigned short x87{};
    controls(){__asm__ volatile("fnstcw %0":"=m"(x87)::"memory");}
    bool operator==(controls const&)const=default;
};
static void prepare(int rounding,bool flush,bool unmask,unsigned precision)
{
    assert(std::fesetenv(FE_DFL_ENV)==0&&std::fesetround(rounding)==0);if(flush)p::enable_flush_control();
    auto state=controls{};auto changed=static_cast<unsigned short>((state.x87&~0x0300u)|(precision<<8u));
    if(unmask)changed=static_cast<unsigned short>(changed&~4u);
    __asm__ volatile("fnclex\n\tfldcw %0"::"m"(changed):"memory");if(unmask)_mm_setcsr(_mm_getcsr()&~0x023fu);
}
static void put(std::size_t offset,std::uint64_t value){s::put(std::span{memory}.subspan(offset,8),0,value);}
static std::uint64_t word(std::size_t offset){return s::word(s::bytes{memory}.subspan(offset,8),0);}
static void request(std::initializer_list<std::uint64_t> words)
{std::size_t offset=128;for(auto n:words){put(offset,n);offset+=8;}put(24576,0xaabb);put(16384,0xccdd);}
template<op Action> static unsigned invoke(s::handle cap,unsigned expected,std::size_t capacity=8192,std::size_t output=16384,std::size_t length=24576)
{
    using wrapper=h::wrapper::native<&h::invoke_artifact<Action>,h::wrapper::artifact_open_name>;
    static_assert(wrapper::guards_fp_controls);
    typename wrapper::local_imported_function_type call{};
    uwvm2::utils::container::get<0>(call.params)=cap;uwvm2::utils::container::get<1>(call.params)=0;
    uwvm2::utils::container::get<2>(call.params)=128;
    uwvm2::utils::container::get<3>(call.params)=uwvm2::imported::ros::artifact_contract::operations[static_cast<std::size_t>(Action)].words*8u;
    uwvm2::utils::container::get<4>(call.params)=output;uwvm2::utils::container::get<5>(call.params)=capacity;
    uwvm2::utils::container::get<6>(call.params)=length;
    auto before=controls{};wrapper::call(call);auto after=controls{};
    auto result=static_cast<unsigned>(uwvm2::utils::container::get<0>(call.res));assert(after==before&&result==expected);return result;
}
static bool stop(void*)noexcept{poison();return true;}
int main()
{
    p::initial_state_restore restore;assert(restore.valid);assert(std::fesetenv(FE_DFL_ENV)==0);
    int fd=static_cast<int>(::syscall(SYS_memfd_create,"artifact-fp",MFD_CLOEXEC|MFD_ALLOW_SEALING));assert(fd>=0);
    std::array<std::uint8_t,8192> contents{};contents.fill(0x5a);assert(::write(fd,contents.data(),contents.size())==static_cast<ssize_t>(contents.size()));
    assert(::fcntl(fd,F_ADD_SEALS,F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL)==0);
    s::artifacts::input_plan plan;plan.path="/trusted-fixture";plan.generation=7;plan.bytes=contents.size();plan.transfer=contents.size();plan.digest=s::buffers::hash(contents);
    auto& config=h::configuration;assert(config.create("robot")==error::success&&config.bind("robot","main")==error::success);
    for(auto name:{"capability.open","artifact.open","artifact.read","artifact.describe"})assert(config.expose("robot",name,s::exposure::allow)==error::success);
    for(auto alias:{"input","denied","rate"})
    {
        s::grant grant;grant.alias=alias;grant.resource=alias;grant.binding="sealed-input";grant.kind=s::purpose::package;grant.max_resources=2;
        grant.actions={"artifact.open","artifact.read","artifact.describe"};if(grant.alias=="denied")grant.actions={"resource.close"};
        if(grant.alias=="rate"){grant.limits.uncertain=true;grant.limits.calls=0;}
        assert(config.allow("robot",grant)==error::success&&config.input("robot",alias,plan)==error::success);
    }
    s::grant cache;cache.alias="cache";cache.resource="cache";cache.binding="volatile-output";cache.kind=s::purpose::cache;
    cache.actions={"artifact.read"};assert(config.allow("robot",cache)==error::success);
    assert(config.output_budget("robot",8192,1,8192+96)==error::success&&config.output("robot","cache",{8192,1,8192,8192})==error::success);
    assert(config.input_budget("robot",3*8192,3)==error::success);
    assert(config.prepare_resources([&](auto const& input,auto const& principal,auto purpose,auto& out)
    {return static_cast<error>(s::storage::immutable_input::create(fd,principal,purpose,input.bytes,input.transfer,out));})==error::success);assert(::close(fd)==0);
    std::shared_ptr<s::broker> state;s::caller who{&identities[0],1};assert(config.select("main",who,state)==error::success);
    std::array<s::handle,4> caps{};unsigned index{};for(auto alias:{"input","denied","rate","cache"})assert(state->permissions.open(who,alias,caps[index++])==error::success);
    std::array<std::uint8_t,8216> output_request{};s::outputs::receipt receipt;
    s::put(output_request,0,1);s::put(output_request,1,8192);assert(state->output_store.mutate("cache",uwvm2::imported::ros::output_contract::operation::begin,{output_request.data(),16},1,receipt)==error::success);
    auto cache_id=receipt.object;s::put(output_request,0,2);s::put(output_request,1,cache_id);s::put(output_request,2,0);std::copy(contents.begin(),contents.end(),output_request.begin()+24);
    assert(state->output_store.mutate("cache",uwvm2::imported::ros::output_contract::operation::append,output_request,1,receipt)==error::success);
    s::put(output_request,0,3);s::put(output_request,2,8192);assert(state->output_store.mutate("cache",uwvm2::imported::ros::output_contract::operation::commit,{output_request.data(),24},1,receipt)==error::success);
    auto opening=[&]{request({7});std::copy(plan.digest.begin(),plan.digest.end(),memory.begin()+136);};
    unsigned profiles{};
    for(auto rounding:{FE_TONEAREST,FE_DOWNWARD,FE_UPWARD,FE_TOWARDZERO})for(auto flush:{false,true})for(auto unmask:{false,true})for(auto precision:{0u,2u,3u})
    {
        prepare(rounding,flush,unmask,precision);opening();invoke<op::open>(caps[0],90,7);assert(word(24576)==8);
        invoke<op::open>(caps[1],13);invoke<op::open>(caps[2],11);invoke<op::open>(UINT64_MAX,9);
        invoke<op::open>(caps[0],22,8192,16384,128);invoke<op::open>(caps[0],22,8192,32768);
        for(int phase:{0,1,2,3,4})
        {
            hostile_stage=failed_stage=phase;opening();invoke<op::open>(caps[0],phase==0?13:22);hostile_stage=failed_stage=-1;
            uwvm2::utils::container::std_vector<s::artifacts::snapshot> inspected;assert(state->inputs.inspect(inspected,4)&&inspected.empty());
        }
        opening();allocation_after=0;allocation_fails=true;auto calls=allocation_calls;invoke<op::open>(caps[0],12);allocation_after=-1;assert(allocation_calls>calls);
        opening();allocation_after=0;allocation_fails=false;calls=allocation_calls;invoke<op::open>(caps[0],0);allocation_after=-1;assert(allocation_calls>calls);auto id=word(16384);
        for(int phase:{0,1,2,3,4})
        {
            hostile_stage=phase;request({id});invoke<op::describe>(caps[0],0,64);assert(word(16384)==7&&word(16392)==8192);
            request({id,0,8192});invoke<op::read>(caps[0],0);assert(word(24576)==8192&&word(16384)==0x5a5a5a5a5a5a5a5aULL);hostile_stage=-1;
        }
        request({id});invoke<op::describe>(caps[0],90,63);assert(word(24576)==64);
        for(int phase:{2,3,4})
        {
            hostile_stage=failed_stage=phase;request({id});invoke<op::describe>(caps[0],22,64);
            request({id,0,8192});invoke<op::read>(caps[0],22);hostile_stage=failed_stage=-1;
        }
        request({id,0,8192});invoke<op::read>(caps[1],13);invoke<op::read>(caps[2],11);invoke<op::read>(caps[0],90,8191);
        request({id,8192,8192});invoke<op::read>(caps[0],0);assert(word(24576)==0);
        request({id,8193,1});invoke<op::read>(caps[0],90);request({id,UINT64_MAX,2});invoke<op::read>(caps[0],90);
        for(auto cap:{caps[0],caps[3]})
        {
            request({cap==caps[0]?id:cache_id,0,8192});allocation_after=0;allocation_fails=true;calls=allocation_calls;
            invoke<op::read>(cap,12);allocation_after=-1;assert(allocation_calls>calls);
            allocation_after=0;allocation_fails=false;calls=allocation_calls;invoke<op::read>(cap,0);allocation_after=-1;assert(allocation_calls>calls);
            request({cap==caps[0]?id:cache_id,0,8});invoke<op::read>(cap,0,8);
        }
        request({id,0,8});pread_fails=true;invoke<op::read>(caps[0],5,8);pread_fails=false;
        {uwvm2::imported::ros::synchronous_wait_context cancelled{stop,nullptr};invoke<op::read>(caps[0],125,8);}
        assert(state->inputs.close("input",id)==error::success);request({id});invoke<op::describe>(caps[0],9);
        identity_index=++profiles;allocation_after=0;allocation_fails=true;calls=allocation_calls;invoke<op::describe>(caps[0],12,64);allocation_after=-1;identity_index=0;assert(allocation_calls>calls);
    }
    assert(profiles==48&&hostile_calls>48&&std::fesetenv(FE_DFL_ENV)==0);
    std::printf("PASS 48 actual artifact-wrapper FP profiles; %u hostile callbacks, %u fast_io allocations; sealed pread/cache, OOM, rollback, short, denied, EOF, cancellation and failed memory delivery\n",hostile_calls,allocation_calls);
}
