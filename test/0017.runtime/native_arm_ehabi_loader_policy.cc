// The actual ARM TargetMachine must refuse an old linked RuntimeDyld before
// compiling guest EH. This component executes no generated code or relocation.
#define UWVM_RUNTIME_LLVM_JIT 1
#include <uwvm2/runtime/compiler/llvm_jit/native_exception_symbols.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Verifier.h>
#include <llvm/TargetParser/Host.h>
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#define CHECK(x) do { if(!(x)) { ::fast_io::io::perrln("ARM loader policy failure line=",__LINE__);return 1; } } while(false)
int main(int argc,char** argv)
{
#if defined(__linux__) && defined(__arm__)
    CHECK(argc==2);
    auto const arg{::fast_io::cstring_view{::fast_io::mnp::os_c_str(argv[1])}};
    bool const qualified{arg=="qualified"};CHECK(qualified || arg=="unqualified");
    CHECK(!::llvm::InitializeNativeTarget());
    ::llvm::LLVMContext context{};auto module{::std::make_unique<::llvm::Module>("live-arm-loader-policy",context)};
    module->setTargetTriple(::llvm::Triple{::llvm::Triple::normalize(::llvm::sys::getDefaultTargetTriple())});
    auto* const owned_module{module.get()};
    ::llvm::EngineBuilder builder{::std::move(module)};
    ::std::unique_ptr<::llvm::TargetMachine> machine{builder.selectTarget()};CHECK(machine);
    CHECK(machine->getTargetTriple().getArch()==::llvm::Triple::arm);
    CHECK(::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols::details::asm_info(*machine)->getExceptionHandlingType()==::llvm::ExceptionHandling::ARM);
    CHECK(::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols::supports_itanium_dwarf_object(*machine)==qualified);
    if(qualified)
    {
        namespace eh=::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols;
        owned_module->setDataLayout(machine->createDataLayout());
        auto const symbols{eh::declare_itanium_dwarf_symbols(*owned_module,*machine)};
        CHECK(symbols.status==eh::error::ok && symbols.personality && symbols.host_personality);
        CHECK(eh::is_object_local_personality_definition(*symbols.personality));
        CHECK(symbols.host_personality->isDeclaration());
        auto* const call{::llvm::dyn_cast<::llvm::CallInst>(&symbols.personality->front().front())};
        CHECK(call && call->isMustTailCall() && call->getCalledFunction()==symbols.host_personality);
        CHECK(call->arg_size()==3u && !::llvm::verifyModule(*owned_module));
        auto const repeated{eh::declare_itanium_dwarf_symbols(*owned_module,*machine)};
        CHECK(repeated.status==eh::error::ok && repeated.personality==symbols.personality &&
              repeated.host_personality==symbols.host_personality);
        ::fast_io::io::println("PASS actual ARM TARGET2 ABI and object-local PREL31 personality");
    }
    else { ::fast_io::io::println("PASS actual ARM TargetMachine refuses the old linked loader before guest EH compilation"); }
#else
    ::fast_io::io::println("SKIP ARM32 linked loader policy");
#endif
}
