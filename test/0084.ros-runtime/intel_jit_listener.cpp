/** @file
 * @brief Exercise the optional listener with an actual independent MCJIT engine.
 * @details The Intel collector boundary is recorded; LLVM allocates, finalizes,
 * resolves and executes real code. This proves helper lifecycle only, not VM
 * integration, original guest source mapping, or native ROS performance parity.
 */
#include <uwvm2/runtime/lib/uwvm_runtime_intel_jit_listener.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/TargetSelect.h>
#include <cstdlib>
#include <iostream>
#include <set>
#include <atomic>
#include <thread>
#include <sys/wait.h>
#include <unistd.h>
namespace profile = uwvm2::runtime::lib::details::intel_jit_profile;
static unsigned queries{}, next_id{1u}, loads{}, unloads{};
static uwvm2::utils::container::std_set<unsigned> live;
static uwvm2::utils::container::std_vector<unsigned char> before_relocation,at_load;
static bool finalized{};
static bool fail_allocation{};
static unsigned rejected_allocations{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t size)
{ if(fail_allocation){++rejected_allocations;return nullptr;}return __real_malloc(size); }
extern "C" void* __wrap_calloc(std::size_t count,std::size_t size)
{ if(fail_allocation){++rejected_allocations;return nullptr;}return __real_calloc(count,size); }
extern "C" void* __wrap_realloc(void* pointer,std::size_t size)
{ if(fail_allocation){++rejected_allocations;return nullptr;}return __real_realloc(pointer,size); }
static void check(bool okay, char const* text) { if(!okay) { std::cerr << text << '\n'; std::abort(); } }
extern "C" std::uint64_t profile_external_target() { return 44; }
/** @brief Observe genuine MCJIT callback bytes, before resolveRelocations(). */
class object_capture final:public llvm::JITEventListener
{
public:
    void notifyObjectLoaded(ObjectKey,llvm::object::ObjectFile const& object,llvm::RuntimeDyld::LoadedObjectInfo const& loaded) override
    {
        for(auto const& sized:llvm::object::computeSymbolSizes(object))
        {
            auto name=sized.first.getName();check(static_cast<bool>(name),"loaded name");
            if(*name!="actual_mcjit_kernel")continue;
            auto section=sized.first.getSection();auto address=sized.first.getAddress();
            check(static_cast<bool>(section)&&static_cast<bool>(address)&&*section!=object.section_end(),"loaded section/address");
            auto base=loaded.getSectionLoadAddress(**section)+*address-(*section)->getAddress();
            check(base&&sized.second&&sized.second<4096,"bounded real text");
            auto* bytes=reinterpret_cast<unsigned char const*>(static_cast<std::uintptr_t>(base));
            before_relocation.assign(bytes,bytes+sized.second);
        }
    }
};
extern "C" iJIT_IsProfilingActiveFlags JITAPI iJIT_IsProfilingActive() { ++queries; return iJIT_SAMPLING_ON; }
extern "C" unsigned JITAPI iJIT_GetNewMethodID() { return next_id++; }
extern "C" int JITAPI iJIT_NotifyEvent(iJIT_JVM_EVENT type, void* payload)
{
    if(type == iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED_V2)
    {
        auto const& event{*static_cast<iJIT_Method_Load_V2*>(payload)};
        check(event.method_size > 0u && event.line_number_size == 0u && event.source_file_name == nullptr, "no fabricated guest source");
        check(std::string_view{event.method_name} == "actual_mcjit_kernel", "actual loaded symbol name");
        check(finalized,"LOAD must follow actual code finalization");
        check(live.insert(event.method_id).second, "new loaded identity");
        auto* bytes=static_cast<unsigned char const*>(event.method_load_address);
        at_load.assign(bytes,bytes+event.method_size);
        ++loads;
    }
    else
    {
        check(type == iJVM_EVENT_TYPE_METHOD_UNLOAD_START, "unload event");
        auto const& event{*static_cast<iJIT_Method_Load*>(payload)};
        check(live.erase(event.method_id) == 1u, "exact one unload per method");
        volatile auto byte{*static_cast<unsigned char const*>(event.method_load_address)}; static_cast<void>(byte);
        ++unloads;
    }
    return 1;
}
int main()
{
    check(!llvm::InitializeNativeTarget() && !llvm::InitializeNativeTargetAsmPrinter(), "native LLVM target");
    for(unsigned scenario : {0u, 1u, 2u, 3u})
    {
        finalized=false;before_relocation.clear();at_load.clear();
        auto context{std::make_unique<llvm::LLVMContext>()};
        auto module{std::make_unique<llvm::Module>("controlled-listener", *context)};
        auto* type{llvm::FunctionType::get(llvm::Type::getInt64Ty(*context), false)};
        auto* function{llvm::Function::Create(type, llvm::GlobalValue::ExternalLinkage, "actual_mcjit_kernel", *module)};
        llvm::IRBuilder<> builder{llvm::BasicBlock::Create(*context, "entry", function)};
        auto* external{llvm::Function::Create(type,llvm::GlobalValue::ExternalLinkage,"profile_external_target",*module)};
        builder.CreateRet(builder.CreateCall(external));
        std::string error;
        auto engine{std::unique_ptr<llvm::ExecutionEngine>{llvm::EngineBuilder{std::move(module)}
            .setEngineKind(llvm::EngineKind::JIT).setErrorStr(&error).create()}};
        check(engine != nullptr, error.c_str());
        engine->addGlobalMapping(external,reinterpret_cast<void*>(&profile_external_target));
        { auto const before{queries}; profile::listener disabled{*engine}; check(queries == before, "disabled listener has zero SDK calls"); }
        {
            auto const before{queries};fail_allocation=true;
            auto no_owner=profile::make_listener_owner(*engine,true,"allocation-denied");
            fail_allocation=false;
            check(!no_owner&&rejected_allocations&&queries==before,"real fast_io allocation failure returns before SDK activation");
            auto disabled=profile::make_listener_owner(*engine,false);
            check(!disabled&&queries==before,"disabled owner makes no allocation or SDK call");
        }
        {
            auto owned=profile::make_listener_owner(*engine,true,"controlled-listener");
            check(static_cast<bool>(owned),"recoverable owner creation");
            auto& listener=*owned;
            object_capture capture;engine->RegisterJITEventListener(&capture);
            engine->finalizeObject();
            engine->UnregisterJITEventListener(&capture);
            check(!engine->hasError() && !listener.observation_failed() && listener.methods() == 0u&&listener.pending_methods()==1u&&live.empty(), "object notification must only stage metadata");
            auto address{engine->getFunctionAddress("actual_mcjit_kernel")};
            if(scenario==2u)
            {
                owned.reset();
                check(live.empty()&&loads==unloads,"aborted pending owner emits no LOAD or UNLOAD");
                continue;
            }
            finalized=true;check(listener.publish_finalized()&&listener.methods()==1u&&!listener.pending_methods(),"finalized actual object reported");
            check(!before_relocation.empty()&&before_relocation!=at_load,"real external relocation changes machine-code bytes");
            auto* actual=reinterpret_cast<unsigned char const*>(static_cast<std::uintptr_t>(address));
            check(std::equal(at_load.begin(),at_load.end(),actual),"Intel LOAD contains finalized actual assembly");
            check(listener.publish_finalized()&&live.size()==1u,"repeated publication emits no duplicate LOAD");
            check(address != 0u && reinterpret_cast<std::uint64_t(*)()>(address)() == 44u, "registered actual code executes");
            if(scenario==0u)
            {
                std::atomic<bool> locked{},release{};
                std::thread holder{[&]{std::lock_guard guard{profile::sdk::mutex};locked.store(true,std::memory_order_release);
                    while(!release.load(std::memory_order_acquire)){std::this_thread::yield();}}};
                while(!locked.load(std::memory_order_acquire)){std::this_thread::yield();}
                auto const child=::fork();check(child>=0,"listener fork");
                if(child==0)
                {
                    ::alarm(3);auto const before=loads+unloads+queries;
                    check(!listener.publish_finalized(),"child cannot publish inherited code");
                    listener.detach_keep_code();listener.retire_code();owned.reset();
                    check(before==loads+unloads+queries,"child listener destructor makes zero SDK calls");
                    ::_exit(0);
                }
                int status{};check(::waitpid(child,&status,0)==child,"listener child joined");
                release.store(true,std::memory_order_release);holder.join();
                check(WIFEXITED(status)&&WEXITSTATUS(status)==0,"inherited SDK mutex never blocks child listener");
            }
            if(scenario==3u)
            {
                // A process-exit owner release must preserve live code events.
                // Reclaim the witness only after its explicit test execution drain.
                listener.detach_keep_code();auto* retained=owned.release();
                check(live.size()==1u&&reinterpret_cast<std::uint64_t(*)()>(address)()==44u,"retained owner/code produces no premature UNLOAD");
                owned.reset(retained);
            }
            if(scenario==1u)
            {
                listener.detach_keep_code();
                check(listener.methods() == 1u && live.size() == 1u, "loader detach retains live executable registration");
                check(reinterpret_cast<std::uint64_t(*)()>(address)() == 44u, "detached observer leaves code live");
                listener.retire_code();
                check(listener.methods() == 0u && live.empty(), "actual code drain precedes retirement");
                engine.reset(); // listener no longer holds this loader borrow
            }
        }
        check(live.empty() && loads == unloads, "destructor notifies before ordinary engine code release");
    }
    std::cout << "PASS actual MCJIT relocation-before-LOAD, fast_io OOM, pending abort, fork denial, retention and unload before code release\n";
}
