/** @file
 * @brief Native policy/projection reference for the real Wasm observation benchmark.
 * @details Uses the same production deployment, projection, page encoding,
 * delivery copy and temporary erasure. The registry is a deterministic trusted
 * fixture, not a native loader. It deliberately excludes Wasm identity, memory
 * bounds and host-import dispatch, so the measured ratio includes that boundary.
 */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/imported/ros/scene/deployment.h>
#include <uwvm2/imported/ros/scene/erased_buffer.h>
#include <uwvm2/uwvm/imported/scene/module_observer.h>
#include <uwvm2/uwvm/imported/scene/vm_observer.h>
#include <cassert>
#include <charconv>
#include <chrono>
#include <cstdio>
#include <cstring>

namespace s=uwvm2::imported::ros::scene;
namespace p=s::permission_report;
namespace h=uwvm2::uwvm::imported::scene;
namespace registry=uwvm2::uwvm::wasm::storage;
namespace type=uwvm2::uwvm::wasm::type;
using E=s::error;
static s::deployment configuration;
static std::array<std::uint8_t,p::maximum_response> delivered;
static type::wasm_file_t main_file{1},worker_file{1};
static constexpr std::array actions{std::string_view{"permission.report"},std::string_view{"module.describe"},
    std::string_view{"module.imports"},std::string_view{"module.exports"},std::string_view{"module.group"},
    std::string_view{"module.permission.report"},std::string_view{"module.importers"},std::string_view{"vm.describe"}};

static void setup()
{
    using kind=type::module_type_t;
    registry::all_module.emplace(u8"main",type::all_module_t{{.wf=&main_file},kind::exec_wasm});
    registry::all_module.emplace(u8"worker",type::all_module_t{{.wf=&worker_file},kind::preloaded_wasm});
    registry::all_module.emplace(u8"uwvm_ros_v2",type::all_module_t{{.li=nullptr},kind::local_import});
    auto get_imports=[]<uwvm2::parser::wasm::concepts::wasm_feature... Fs>(uwvm2::utils::container::tuple<Fs...>)->decltype(auto)
    {return (uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<
        uwvm2::parser::wasm::standard::wasm1::features::import_section_storage_t<Fs...>>(main_file.wasm_module_storage.wasm_binfmt_ver1_storage.sections).imports);};
    auto& imports=get_imports(uwvm2::uwvm::wasm::feature::all_features);
    using item=typename std::remove_reference_t<decltype(imports)>::value_type;
    item declaration{};declaration.module_name=u8"worker";declaration.extern_name=u8"ping";imports.push_back(declaration);
    registry::all_module_export[u8"worker"].emplace(u8"ping",type::all_module_export_t{});
    assert(configuration.create("benchmark")==E::success&&configuration.bind("benchmark","main")==E::success);
    assert(configuration.create("worker-group")==E::success&&configuration.bind("worker-group","worker")==E::success);
    s::grant module;module.alias="worker-view";module.kind=s::purpose::module_inspection;module.resource="worker";module.binding="metadata";
    for(auto action:actions)if(action.starts_with("module."))
    {module.actions.emplace(action);assert(configuration.expose("benchmark",uwvm2::utils::container::std_string{action},s::exposure::allow)==E::success);}
    assert(configuration.allow("benchmark",module)==E::success);
    s::grant vm;vm.alias="runtime-view";vm.kind=s::purpose::vm_inspection;vm.resource="runtime";vm.binding="metadata";vm.actions={"vm.describe"};
    assert(configuration.allow("benchmark",vm)==E::success&&configuration.expose("benchmark","vm.describe",s::exposure::allow)==E::success);
}

[[gnu::noinline]] static std::uint64_t observe(unsigned operation)
{
    auto action=actions[operation];p::query query{p::family::rosv2,"permission.report",0};
    auto preflight=[&](auto&& callback)
    {
        if(operation==7)return configuration.observe_vm("main","runtime-view",callback);
        return configuration.observe_module("main","worker-view",action,callback,operation==5?&query:nullptr);
    };
    if(operation==0)assert(configuration.report_allowed("main",query)==E::success);
    else assert(preflight([](auto){return E::success;})==E::success);
#if defined(UWVM_SCENE_OBSERVATION_OWNED_PAGE)
    p::owned_page response{0};auto& page=response.get();
#else
    s::erased_buffer response;response.resize_for_overwrite(p::maximum_response);p::page page{response.writable(),0};
#endif
    auto deliver=[&]()noexcept {std::memcpy(delivered.data(),page.bytes().data(),page.bytes().size());return true;};
    if(operation==0)
    {
        assert(configuration.report_self("main",query,page)==E::success);
        assert(configuration.deliver_report("main",query,deliver)==E::success);
    }
    else if(operation==5)
    {
        assert(h::module_observer::identity("worker",page)==E::success);
        assert(configuration.report_self("worker",query,page)==E::success);
        assert(preflight([&](auto){return deliver()?E::success:E::invalid;})==E::success);
    }
    else assert(preflight([&](auto target)
    {
        E result;
        if(operation==7)result=h::vm_observer::read(page);
        else if(operation==4)result=configuration.report_module_group_locked(target,page);
        else
        {
            auto selected=operation==1?p::module_operation::describe:operation==2?p::module_operation::imports:
                operation==3?p::module_operation::exports:p::module_operation::importers;
            result=h::module_observer::read(target,selected,page);
        }
        return result==E::success&&deliver()?E::success:E::invalid;
    })==E::success);
    return s::word(delivered,4); // bounded row count; protects work from dead-code removal
}

static std::uint64_t number(char const* arg)
{std::uint64_t value{};auto end=arg+std::strlen(arg);auto parsed=std::from_chars(arg,end,value);assert(parsed.ec==std::errc{}&&parsed.ptr==end);return value;}
int main(int argc,char** argv)
{
    assert(argc==4);auto operation=number(argv[1]),iterations=number(argv[2]),batches=number(argv[3]);
    assert(operation<actions.size()&&iterations&&iterations<=10000000&&batches&&batches<=1000);setup();
    std::array<std::uint64_t,1002> times{};std::uint64_t checksum{};
    for(unsigned i{};i<1000;++i)checksum+=observe(static_cast<unsigned>(operation));
    auto begin=std::chrono::steady_clock::now();
    for(std::uint64_t batch{};batch<batches;++batch)
    {
        auto start=std::chrono::steady_clock::now();
        for(std::uint64_t i{};i<iterations;++i)checksum+=observe(static_cast<unsigned>(operation));
        times[batch]=static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count());
    }
    times[batches]=static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-begin).count());
    times[batches+1]=checksum;
    return std::fwrite(times.data(),8,static_cast<std::size_t>(batches+2),stdout)==batches+2?0:1;
}
