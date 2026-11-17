/** @file
 * @brief Exercise all eight observation wrappers under hostile native controls.
 * @details Production policy, registry projections and erasure run unchanged.
 * Controlled identity, imported-memory and allocation callbacks alter x87 and
 * MXCSR controls even when returning failure or a partial copy. The test covers
 * 48 initial control profiles, recoverable fast_io OOM and deallocation. These
 * callbacks are test doubles; real Wasm memory and backend checks are separate.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/imported/scene/host.h>
#include "../0008.imported/wasi/wasip1/func/fp_control_probe.h"
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>

namespace h=uwvm2::uwvm::imported::scene;
namespace s=uwvm2::imported::ros::scene;
namespace r=s::permission_report;
namespace p=uwvm2test::wasip1_fp_control;
namespace registry=uwvm2::uwvm::wasm::storage;
namespace type=uwvm2::uwvm::wasm::type;
using E=s::error;
static std::array<std::uint8_t,131072> memory{};
static int identity,failed_stage{-1},allocation_after{-1};
static unsigned poison_calls{},allocation_calls{},free_calls{};
static bool hostile{},allocation_fails{},hostile_free{};
static type::wasm_file_t main_file{1},worker_file{1};
static constexpr std::array actions{std::string_view{"permission.report"},std::string_view{"module.describe"},
    std::string_view{"module.imports"},std::string_view{"module.exports"},std::string_view{"module.group"},
    std::string_view{"module.permission.report"},std::string_view{"module.importers"},std::string_view{"vm.describe"}};

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
extern "C" void __real_free(void*);
extern "C" void* __wrap_malloc(std::size_t n){return allocation()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t size){return allocation()?nullptr:__real_calloc(n,size);}
extern "C" void* __wrap_realloc(void* old,std::size_t n){return allocation()?nullptr:__real_realloc(old,n);}
extern "C" void __wrap_free(void* pointer){if(hostile_free){++free_calls;poison();}__real_free(pointer);}
static bool stage(int value)noexcept{if(hostile)poison();return failed_stage!=value;}
namespace uwvm2::runtime::lib
{
    bool ros_caller_identity_host_api(ros_caller_identity& out,bool)noexcept
    {out={&identity,1,{u8"main"},false};return stage(0);}
    bool preload_memory_range_host_api(std::size_t index,std::uint_least64_t offset,std::size_t size)noexcept
    {return index==0&&offset<=memory.size()&&size<=memory.size()-offset;}
    bool preload_memory_normalize_range_host_api(std::size_t index,std::uint_least64_t offset,std::size_t size,std::uint64_t& normalized)noexcept
    {normalized=offset;return stage(1)&&preload_memory_range_host_api(index,offset,size);}
    bool preload_memory_read_host_api(std::size_t index,std::uint_least64_t offset,void* target,std::size_t size)noexcept
    {
        if(!preload_memory_range_host_api(index,offset,size))return false;
        bool okay=stage(2);std::memcpy(target,memory.data()+offset,okay?size:size/2);return okay;
    }
    bool preload_memory_write_host_api(std::size_t index,std::uint_least64_t offset,void const* source,std::size_t size)noexcept
    {
        if(!preload_memory_range_host_api(index,offset,size))return false;
        bool okay=stage(offset==60000?3:4);std::memcpy(memory.data()+offset,source,okay?size:size/2);return okay;
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
static std::size_t request(unsigned operation,std::string_view family="rosv2",std::string_view function="permission.report")
{
    s::wire::writer out{std::span{memory}.subspan(128,1024)};
    if(operation)assert(out.text(operation==7?"runtime-view":"worker-view"));
    if(operation==0||operation==5)assert(out.text(family)&&out.text(function));
    assert(out.u64(0));std::memset(memory.data()+16384,0x6a,32768);std::memset(memory.data()+60000,0x6b,8);return out.size();
}
template<auto Fn>static void invoke(std::size_t size,unsigned expected)
{
    using wrapper=h::wrapper::native<Fn,h::wrapper::permission_report_name>;
    static_assert(wrapper::guards_fp_controls);
    typename wrapper::local_imported_function_type call{};
    uwvm2::utils::container::get<0>(call.params)=0;uwvm2::utils::container::get<1>(call.params)=0;
    uwvm2::utils::container::get<2>(call.params)=128;uwvm2::utils::container::get<3>(call.params)=size;
    uwvm2::utils::container::get<4>(call.params)=16384;uwvm2::utils::container::get<5>(call.params)=32768;
    uwvm2::utils::container::get<6>(call.params)=60000;
    auto before=controls{};wrapper::call(call);auto after=controls{};
    auto result=static_cast<unsigned>(uwvm2::utils::container::get<0>(call.res));
    if(result!=expected)std::fprintf(stderr,"observation got %u expected %u\n",result,expected);
    assert(after==before&&result==expected);
    if(expected==12||expected==13)
    {
        for(auto byte:std::span{memory}.subspan(16384,32768))assert(byte==0x6a);
        for(auto byte:std::span{memory}.subspan(60000,8))assert(byte==0x6b);
    }
}
static constexpr std::array callbacks{
    &invoke<&h::invoke_permission_report>,
    &invoke<&h::invoke_module_observation<r::module_operation::describe>>,
    &invoke<&h::invoke_module_observation<r::module_operation::imports>>,
    &invoke<&h::invoke_module_observation<r::module_operation::exports>>,
    &invoke<&h::invoke_module_observation<r::module_operation::group>>,
    &invoke<&h::invoke_module_permission_report>,
    &invoke<&h::invoke_module_observation<r::module_operation::importers>>,
    &invoke<&h::invoke_module_observation<r::module_operation::describe,true>>};
static void setup()
{
    using kind=type::module_type_t;
    registry::all_module.emplace(u8"main",type::all_module_t{{.wf=&main_file},kind::exec_wasm});
    registry::all_module.emplace(u8"worker",type::all_module_t{{.wf=&worker_file},kind::preloaded_wasm});
    auto get_imports=[]<uwvm2::parser::wasm::concepts::wasm_feature... Fs>(uwvm2::utils::container::tuple<Fs...>)->decltype(auto)
    {return (uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<
        uwvm2::parser::wasm::standard::wasm1::features::import_section_storage_t<Fs...>>(main_file.wasm_module_storage.wasm_binfmt_ver1_storage.sections).imports);};
    auto& imports=get_imports(uwvm2::uwvm::wasm::feature::all_features);
    using item=typename std::remove_reference_t<decltype(imports)>::value_type;
    item declaration{};declaration.module_name=u8"worker";declaration.extern_name=u8"ping";imports.push_back(declaration);
    registry::all_module_export[u8"worker"].emplace(u8"ping",type::all_module_export_t{});
    auto& config=h::configuration;assert(config.create("robot")==E::success&&config.bind("robot","main")==E::success);
    assert(config.create("worker-group")==E::success&&config.bind("worker-group","worker")==E::success);
    s::grant module;module.alias="worker-view";module.kind=s::purpose::module_inspection;module.resource="worker";module.binding="metadata";
    for(auto action:actions)if(action.starts_with("module."))
    {module.actions.emplace(action);assert(config.expose("robot",uwvm2::utils::container::std_string{action},s::exposure::allow)==E::success);}
    assert(config.allow("robot",module)==E::success);
    s::grant vm;vm.alias="runtime-view";vm.kind=s::purpose::vm_inspection;vm.resource="runtime";vm.binding="metadata";vm.actions={"vm.describe"};
    assert(config.allow("robot",vm)==E::success&&config.expose("robot","vm.describe",s::exposure::allow)==E::success);
}
int main()
{
    p::initial_state_restore restore;assert(restore.valid);setup();unsigned profiles{};
    for(auto rounding:{FE_TONEAREST,FE_DOWNWARD,FE_UPWARD,FE_TOWARDZERO})for(auto flush:{false,true})for(auto unmask:{false,true})for(auto precision:{0u,2u,3u})
    {
        prepare(rounding,flush,unmask,precision);hostile=true;hostile_free=true;
        for(unsigned i{};i<callbacks.size();++i)
        {
            auto size=request(i);callbacks[i](size,0);
            for(auto phase:{0,1,2,3,4}){failed_stage=phase;size=request(i);callbacks[i](size,phase==0?13:22);failed_stage=-1;}
            size=request(i);auto calls=allocation_calls;allocation_after=0;allocation_fails=true;callbacks[i](size,12);
            assert(allocation_calls==calls+1&&allocation_after==-1);allocation_fails=false;
            size=request(i);allocation_after=0;callbacks[i](size,0);assert(allocation_after==-1);
        }
        for(auto family:r::names)
        {
            auto size=request(0,family,"");unsigned expected{};
            if(family=="wasip1")
            {
#if !defined(UWVM_IMPORT_WASI_WASIP1) || defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                expected=95;
#endif
            }
            if(family=="preloaddl")
            {
#if !defined(UWVM_SUPPORT_PRELOAD_DL)
                expected=95;
#endif
            }
            if(family=="ros1v1")
            {
#if !defined(UWVM_IMPORT_ROS1)
                expected=95;
#endif
            }
            if(family=="ros2v1")
            {
#if !defined(UWVM_IMPORT_ROS2)
                expected=95;
#endif
            }
            callbacks[0](size,expected);
        }
        hostile=hostile_free=false;++profiles;
    }
    assert(profiles==48&&poison_calls>48&&free_calls>48&&std::fesetenv(FE_DFL_ENV)==0);
    assert(h::configuration.bind("robot","after-observation")==E::success);
    std::printf("PASS 48 observation FP profiles across eight wrappers; %u hostile callbacks, %u allocations, %u frees; no freeze or enrollment\n",poison_calls,allocation_calls,free_calls);
}
