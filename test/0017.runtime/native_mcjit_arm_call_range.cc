// ARM ELF MCJIT must not wrap a call to a stub beyond the BL range.
// The padding is never executed. It makes the real emitted code section exceed
// 32 MiB without spending minutes optimizing thousands of checkpoint bodies.
#define UWVM_RUNTIME_LLVM_JIT 1
#include <uwvm2/runtime/compiler/llvm_jit/fast_io_support.h>
#include <uwvm2/runtime/compiler/llvm_jit/mcjit_target_support.h>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/translate/section_memory_manager.h>
#include <uwvm2/uwvm/runtime/macro/pop_macros.h>
#include <uwvm2/utils/macro/pop_macros.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/JITEventListener.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <fast_io.h>
#include <string_view>
namespace support = ::uwvm2::runtime::compiler::llvm_jit::details;
namespace storage = ::uwvm2::runtime::compiler::llvm_jit::fast_io_support;
#define CHECK(x) do { if(!(x)) { ::fast_io::io::perrln("ARM MCJIT call-range failure line=",__LINE__); return 1; } } while(false)
static unsigned host_callback(unsigned value) noexcept { return value + 3u; }
struct object_capture final : ::llvm::JITEventListener
{
    char const* path{};
    ::std::uint64_t text_size{};
    void notifyObjectLoaded(ObjectKey,::llvm::object::ObjectFile const& object,
        ::llvm::RuntimeDyld::LoadedObjectInfo const&) override
    {
        for(auto const& section: object.sections())
        { if(section.isText() && section.getSize()>text_size) { text_size=section.getSize(); } }
        if(path)
        {
            auto const bytes{object.getMemoryBufferRef().getBuffer()};
            ::fast_io::native_file output{::fast_io::mnp::os_c_str(path),::fast_io::open_mode::out};
            ::fast_io::io::print(output,::fast_io::mnp::strvw(::std::string_view{bytes.data(),bytes.size()}));
        }
    }
};
int main(int argc,char** argv)
{
    CHECK(argc<=3);
    ::llvm::Triple const triple{::llvm::sys::getProcessTriple()};
    if(!triple.isOSBinFormatELF() || triple.getArch()!=::llvm::Triple::arm)
    { ::fast_io::io::println("SKIP: ARM ELF code-range execution requires an ARM process"); return 77; }
    bool const legacy{argc==3 && ::std::string_view{argv[2]}=="legacy"};
    CHECK(argc!=3 || legacy);
    CHECK(!::llvm::InitializeNativeTarget());
    CHECK(!::llvm::InitializeNativeTargetAsmPrinter());
    CHECK(!::llvm::InitializeNativeTargetAsmParser());
    auto context{storage::make_context()};
    auto module{storage::make_module("arm-call-range",*context)};
    ::llvm::IRBuilder<> ir{*context};
    auto* const signature{::llvm::FunctionType::get(ir.getInt32Ty(),{ir.getInt32Ty()},false)};
    auto* const host{::llvm::Function::Create(signature,::llvm::GlobalValue::ExternalLinkage,"host_callback",*module)};
    auto* const entry{::llvm::Function::Create(signature,::llvm::GlobalValue::ExternalLinkage,"entry",*module)};
    auto* const tail{::llvm::Function::Create(signature,::llvm::GlobalValue::ExternalLinkage,"tail",*module)};
    auto* const indirect{::llvm::Function::Create(::llvm::FunctionType::get(ir.getInt32Ty(),
        {ir.getInt32Ty(),ir.getPtrTy()},false),::llvm::GlobalValue::ExternalLinkage,"indirect",*module)};
    auto* const padding_type{::llvm::FunctionType::get(ir.getVoidTy(),{},false)};
    auto* const padding{::llvm::Function::Create(padding_type,::llvm::GlobalValue::ExternalLinkage,"padding",*module)};
    auto* const callee{::llvm::Function::Create(signature,::llvm::GlobalValue::ExternalLinkage,"callee",*module)};
    ir.SetInsertPoint(::llvm::BasicBlock::Create(*context,"entry",entry));
    auto* const intermediate{ir.CreateCall(host,{entry->getArg(0u)})};
    ir.CreateRet(ir.CreateCall(callee,{intermediate}));
    ir.SetInsertPoint(::llvm::BasicBlock::Create(*context,"entry",tail));
    auto* const tail_call{ir.CreateCall(callee,{tail->getArg(0u)})};
    tail_call->setTailCallKind(::llvm::CallInst::TCK_MustTail);ir.CreateRet(tail_call);
    ir.SetInsertPoint(::llvm::BasicBlock::Create(*context,"entry",indirect));
    ir.CreateRet(ir.CreateCall(signature,indirect->getArg(1u),{indirect->getArg(0u)}));
    ir.SetInsertPoint(::llvm::BasicBlock::Create(*context,"entry",padding));
    ir.CreateCall(::llvm::InlineAsm::get(padding_type,".space 34603008, 0","",true));ir.CreateRetVoid();
    ir.SetInsertPoint(::llvm::BasicBlock::Create(*context,"entry",callee));
    ir.CreateRet(ir.CreateAdd(callee->getArg(0u),ir.getInt32(7u)));
    CHECK(!::llvm::verifyModule(*module));
    ::llvm::SmallVector<::std::string,32u> attributes{}; // External LLVM API string ABI.
    for(auto const& [name,enabled]: ::llvm::sys::getHostCPUFeatures())
    { attributes.emplace_back((enabled ? "+" : "-")+name.str()); }
    if(!legacy) { support::llvm_jit_mcjit_append_call_range_features(triple,attributes); }
    ::llvm::EngineBuilder selector{};
    selector.setEngineKind(::llvm::EngineKind::JIT).setOptLevel(::llvm::CodeGenOptLevel::None);
    support::llvm_jit_mcjit_configure_code_model(selector,triple);
    support::llvm_jit_mcjit_configure_host_unwind_abi(selector);
    auto target{::uwvm2::utils::container::delete_owned_ptr<::llvm::TargetMachine>{
        selector.selectTarget(triple,{},::llvm::sys::getHostCPUName(),attributes)}};
    CHECK(target);
    module->setTargetTriple(target->getTargetTriple());module->setDataLayout(target->createDataLayout());
    for(auto& function: *module)
    { function.addFnAttr("target-features",target->getTargetFeatureString()); }
    auto manager{::uwvm2::utils::container::make_unique<support::runtime_llvm_jit_section_memory_manager>()};
    auto engine{::uwvm2::utils::container::delete_owned_ptr<::llvm::ExecutionEngine>{
        ::llvm::EngineBuilder{::std::unique_ptr<::llvm::Module>{module.release()}}
        .setEngineKind(::llvm::EngineKind::JIT).setOptLevel(::llvm::CodeGenOptLevel::None)
        .setMCJITMemoryManager(::std::unique_ptr<::llvm::RTDyldMemoryManager>{manager.release()})
        .create(target.release())}};
    CHECK(engine);engine->addGlobalMapping(host,reinterpret_cast<void*>(&host_callback));
    object_capture capture{};capture.path=argc>=2 ? argv[1] : nullptr;
    engine->RegisterJITEventListener(&capture);engine->finalizeObject();engine->UnregisterJITEventListener(&capture);
    CHECK(!engine->hasError());CHECK(capture.text_size>33554432u);
    auto const entry_address{engine->getFunctionAddress("entry")};
    auto const tail_address{engine->getFunctionAddress("tail")};
    auto const indirect_address{engine->getFunctionAddress("indirect")};
    auto const callee_address{engine->getFunctionAddress("callee")};
    CHECK(entry_address && tail_address && indirect_address && callee_address);
    CHECK(callee_address>entry_address && callee_address-entry_address>33554432u);
    CHECK(reinterpret_cast<unsigned(*)(unsigned)>(entry_address)(19u)==29u);
    CHECK(reinterpret_cast<unsigned(*)(unsigned)>(tail_address)(19u)==26u);
    CHECK(reinterpret_cast<unsigned(*)(unsigned,void*)>(indirect_address)(19u,reinterpret_cast<void*>(callee_address))==26u);
    ::fast_io::io::println("PASS ARM far direct, host, musttail and indirect calls; text_bytes=",capture.text_size);
}
