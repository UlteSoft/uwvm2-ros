/** @file
 * @brief Exercise actual registered-message wrappers with hostile FP callbacks.
 * @details Deployment, admission, routing and erasure are real. The runtime
 * identity and memory APIs are controlled fixtures which deliberately change
 * native controls, including on refusal and partial writes. Forty-eight x86
 * profiles cover rounding, masks, flush controls and x87 precision. Allocation
 * interposition tests recoverable fast_io failures. Real VM memory validation
 * is qualified separately; these fixtures do not claim to emulate that layer.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include "../0008.imported/wasi/wasip1/func/fp_control_probe.h"
#include <cassert>
#include <cstdio>
#include <cerrno>
#include <cstring>
namespace h=uwvm2::uwvm::imported::scene;
namespace s=uwvm2::imported::ros::scene;
namespace p=uwvm2test::wasip1_fp_control;
using op=s::module_exchange::operation;using E=s::error;
static std::array<std::array<std::uint8_t,32768>,2> memories{};
static std::array<int,2> identities{};
static unsigned actor{},poison_calls{},allocation_calls{};
static int failed_stage{-1},allocation_after{-1};
static bool hostile{},allocation_fails{};
static void poison()noexcept
{
    ++poison_calls;unsigned short cw;__asm__ volatile("fnstcw %0":"=m"(cw)::"memory");
    cw=static_cast<unsigned short>((cw&~0x0f3fu)|0x0800u|0x003bu);
    __asm__ volatile("fnclex\n\tfldcw %0"::"m"(cw):"memory");_mm_setcsr(0x0000df40u);
}
static bool allocation()noexcept
{
    if(allocation_after<0)return false;
    ++allocation_calls;if(allocation_after--!=0)return false;
    poison();if(allocation_fails){errno=ENOMEM;return true;}return false;
}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return allocation()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t size){return allocation()?nullptr:__real_calloc(n,size);}
extern "C" void* __wrap_realloc(void* old,std::size_t n){return allocation()?nullptr:__real_realloc(old,n);}
static bool stage(int value)noexcept{if(hostile)poison();return failed_stage!=value;}
namespace uwvm2::runtime::lib
{
    bool ros_caller_identity_host_api(ros_caller_identity& out,bool)noexcept
    {if(actor)out={&identities[actor],1,{u8"worker"},false};else out={&identities[actor],1,{u8"main"},false};return stage(0);}
    std::uint32_t with_ros_exchange_memory_host_api(ros_exchange_memory_callback callback,void* context)noexcept
    {ros_exchange_memory_access access;if(!ros_caller_identity_host_api(access.caller_,true))return 13;return callback(context,access);}
    bool ros_exchange_memory_access::capture(std::size_t index,std::uint64_t offset,std::size_t size,std::uint64_t& normalized)noexcept
    {
        normalized=offset;if(!stage(1)||index||offset>32768||size>32768-offset)return false;
        if(!memory_owner_){memory_owner_=memories[actor].data();memory_index_=index;}
        return memory_index_==index&&memory_owner_==memories[actor].data();
    }
    bool ros_exchange_memory_access::copy(std::size_t index,std::uint64_t offset,void* data,std::size_t size,bool write)noexcept
    {
        if(!memory_owner_||index!=memory_index_||memory_owner_!=memories[actor].data()||offset>32768||size>32768-offset)return false;
        bool okay=stage(write?3:2);auto* start=memories[actor].data()+offset;
        std::memcpy(write?start:data,write?data:start,okay?size:size/2);return okay;
    }
    bool ros_exchange_memory_access::write_frame(std::size_t index,std::uint64_t offset,
        void const* prefix,std::size_t prefix_size,void const* payload,std::size_t payload_size)noexcept
    {
        if((prefix_size&&!prefix)||(payload_size&&!payload)||payload_size>SIZE_MAX-prefix_size||
            !memory_owner_||index!=memory_index_||memory_owner_!=memories[actor].data()||
            offset>32768||prefix_size+payload_size>32768-offset)return false;
        bool okay=stage(3);auto* start=memories[actor].data()+offset;
        // This deliberately hostile runtime stub keeps the old partial-write
        // failure injection; production complete-range refusal is stricter.
        if(prefix_size)std::memcpy(start,prefix,okay?prefix_size:prefix_size/2);
        if(okay&&payload_size)std::memcpy(start+prefix_size,payload,payload_size);
        return okay;
    }
    bool ros_exchange_memory_erase_host_api(void const* instance,std::uint64_t generation,std::uint64_t index,std::uint64_t offset,std::uint64_t size,std::uint64_t length_offset,std::uint64_t length_size)noexcept
    {
        if(!stage(4)||generation!=1||index||offset>32768||size>32768-offset)return false;
        for(unsigned i{};i<2;++i)if(instance==&identities[i]){s::erase_bytes(std::span{memories[i]}.subspan(offset,size));if(length_size){if(length_offset>32768||length_size>32768-length_offset)return false;s::erase_bytes(std::span{memories[i]}.subspan(length_offset,length_size));}return true;}
        return false;
    }
    bool ros_exchange_memory_access::erase(std::uint64_t offset,std::uint64_t size)noexcept
    {return memory_owner_&&ros_exchange_memory_erase_host_api(caller_.runtime_instance,caller_.generation,memory_index_,offset,size);}
    bool ros_exchange_memory_access::read_input(std::size_t index,std::uint64_t offset,void* data,std::size_t size,bool clear_after_copy)noexcept
    {
        // Keep hostile partial-copy and erase-refusal negative controls. The
        // production primitive proves the entire extent before either effect.
        return copy(index,offset,data,size,false)&&(!clear_after_copy||erase(offset,size));
    }
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
static std::uint64_t word(std::size_t offset){return s::word(s::bytes{memories[actor]}.subspan(offset,8),0);}
static void request(std::uint64_t token,std::size_t size=0)
{
    auto& memory=memories[actor];s::put(std::span{memory}.subspan(128,16),0,token);s::put(std::span{memory}.subspan(128,16),1,size);
    if(size)std::memset(memory.data()+144,0xa5,size);
}
static void zero(unsigned owner,std::size_t start,std::size_t size)
{for(auto byte:std::span{memories[owner]}.subspan(start,size))assert(!byte);}
template<op Action> static void invoke(s::handle cap,unsigned expected,std::size_t size=16)
{
    using wrapper=h::wrapper::native<&h::invoke_module_call<Action>,h::wrapper::module_call_submit_name>;
    static_assert(wrapper::guards_fp_controls);
    typename wrapper::local_imported_function_type call{};
    uwvm2::utils::container::get<0>(call.params)=cap;uwvm2::utils::container::get<1>(call.params)=0;
    uwvm2::utils::container::get<2>(call.params)=128;uwvm2::utils::container::get<3>(call.params)=size;
    constexpr bool receipt=Action==op::respond||Action==op::cancel||Action==op::acknowledge;
    uwvm2::utils::container::get<4>(call.params)=receipt?8192:16384;uwvm2::utils::container::get<5>(call.params)=2048;
    uwvm2::utils::container::get<6>(call.params)=receipt?12000:24576;
    auto before=controls{};wrapper::call(call);auto after=controls{};
    auto result=static_cast<unsigned>(uwvm2::utils::container::get<0>(call.res));
    if(result!=expected)std::fprintf(stderr,"operation %u got %u expected %u\n",static_cast<unsigned>(Action),result,expected);
    assert(after==before&&result==expected);
}
static void check_input(unsigned owner,std::uint64_t token,std::size_t payload,bool clear)
{
    if(clear){zero(owner,128,payload+16);return;}
    auto const bytes=s::bytes{memories[owner]};
    assert(s::word(bytes.subspan(128,8),0)==token&&s::word(bytes.subspan(136,8),0)==payload);
    for(auto byte:bytes.subspan(144,payload))assert(byte==0xa5);
}
int main(int argc,char** argv)
{
    assert(argc==1||(argc==2&&!std::strcmp(argv[1],"--retain-input")));bool clear=argc==1;
    p::initial_state_restore restore;assert(restore.valid&&std::fesetenv(FE_DFL_ENV)==0);
    auto& config=h::configuration;assert(config.create("robot")==E::success&&config.bind("robot","main")==E::success&&config.bind("robot","worker")==E::success);
    assert(config.expose("robot","capability.open",s::exposure::allow)==E::success);
    for(auto name:s::module_exchange::names)assert(config.expose("robot",uwvm2::utils::container::std_string{name},s::exposure::allow)==E::success);
    // No capability is open: refusal cannot freeze or initialize the broker.
    request(0);invoke<op::submit>(0,13);assert(config.bind("robot","still-configurable")==E::success);
    assert(config.module_call_budget("robot",65536,1)==E::success);
    assert(config.module_call_plan("robot",{"route","main","worker","echo","octets","octets",1024,1024,1,60000000000})==E::success);
    assert(config.module_call_input_clear("robot","route",clear)==E::success);
    s::grant grant;grant.alias="exchange";grant.resource="route";grant.kind=s::purpose::module_call;grant.binding="registered-message";
    for(auto name:s::module_exchange::names)grant.actions.emplace(name);
    assert(config.allow("robot",grant)==E::success);
    std::shared_ptr<s::broker> broker;std::array<s::handle,2> caps{};
    for(unsigned i{};i<2;++i){assert(config.select(i?"worker":"main",{&identities[i],1},broker)==E::success);assert(broker->permissions.open({&identities[i],1},"exchange",caps[i])==E::success);}
    actor=1;request(0);invoke<op::take>(caps[1],11);
    unsigned profiles{};
    for(auto rounding:{FE_TONEAREST,FE_DOWNWARD,FE_UPWARD,FE_TOWARDZERO})for(auto flush:{false,true})for(auto unmask:{false,true})for(auto precision:{0u,2u,3u})
    {
        prepare(rounding,flush,unmask,precision);hostile=true;actor=0;
        for(auto [phase,code]:{std::pair{0,13u},{1,95u},{2,22u},{3,22u},{4,5u}})
        {if(!clear&&phase==4)continue;failed_stage=phase;request(0,1024);invoke<op::submit>(caps[0],code,1040);failed_stage=-1;if(!clear)check_input(0,0,1024,false);}
        request(0,1024);allocation_after=0;allocation_fails=true;auto allocations=allocation_calls;
        invoke<op::submit>(caps[0],12,1040);allocation_after=-1;assert(allocation_calls>allocations);check_input(0,0,1024,clear);
        request(0,1024);allocation_after=0;allocation_fails=false;invoke<op::submit>(caps[0],0,1040);allocation_after=-1;check_input(0,0,1024,clear);auto token=word(16384);
        actor=1;request(0);invoke<op::take>(caps[1],0);check_input(1,0,0,clear);assert(word(16400)==0xa5a5a5a5a5a5a5a5ULL);
        request(token,1024);invoke<op::respond>(caps[1],0,1040);check_input(1,token,1024,clear);zero(1,16400,2032);
        actor=0;request(token);invoke<op::result>(caps[0],0);check_input(0,token,0,clear);assert(word(16400)==0xa5a5a5a5a5a5a5a5ULL);
        // Cleanup output must not overlap the retained result: use a separate
        // temporary wrapper output area by cancelling from the opposite role.
        actor=1;request(token);invoke<op::cancel>(caps[1],0);zero(0,16384,2048);zero(0,24576,8);
        actor=0;request(token);invoke<op::acknowledge>(caps[0],0);check_input(0,token,0,clear);
        request(0);invoke<op::submit>(UINT64_MAX,9);hostile=false;++profiles;
    }
    assert(profiles==48&&poison_calls>48&&std::fesetenv(FE_DFL_ENV)==0);
    std::printf("PASS 48 registered-message FP profiles; %u hostile callbacks; %u recoverable allocations; cold denial, partial copy, erasure, OOM and cleanup\n",poison_calls,allocation_calls);
}
