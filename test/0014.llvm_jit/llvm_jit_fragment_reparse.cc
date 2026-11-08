// Exercise the actual parallel-fragment reparse/link boundary with independent
// LLVMContexts. This is compiler DATA; it grants no runtime/debugger authority.
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/impl.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/DiagnosticInfo.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/resource.h>
namespace compiler = ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm;
namespace details = compiler::details;
using storage = compiler::llvm_jit_module_storage_t;
using fragments = ::uwvm2::utils::container::vector<storage>;

static void require(bool ok, char const* message)
{
    if(!ok) { ::std::fprintf(stderr, "fragment reparse: %s\n", message); ::std::abort(); }
}
static storage module(char const* name)
{
    storage result{};
    result.llvm_context_holder = ::uwvm2::utils::container::make_delete_owned<::llvm::LLVMContext>();
    result.llvm_module = ::uwvm2::utils::container::make_delete_owned<::llvm::Module>(name, *result.llvm_context_holder);
    result.emitted = true;
    return result;
}
static ::llvm::Function* make_fragment(storage& owner, unsigned ordinal, unsigned functions, unsigned instructions)
{
    auto& context{*owner.llvm_context_holder};
    auto& ir_module{*owner.llvm_module};
    ::llvm::DIBuilder debug{ir_module};
    auto const file{debug.createFile("fragment.cpp", "/component")};
    debug.createCompileUnit(::llvm::dwarf::DW_LANG_C_plus_plus, file, "uwvm fragment test", false, "", 0);
    ir_module.addModuleFlag(::llvm::Module::Warning, "Debug Info Version", ::llvm::DEBUG_METADATA_VERSION);
    auto const subroutine{debug.createSubroutineType(debug.getOrCreateTypeArray({}))};
    ::llvm::IRBuilder<> builder{context};
    auto const integer{builder.getInt32Ty()};
    auto const function_type{::llvm::FunctionType::get(integer, {integer}, false)};
    auto const helper_name{"helper." + ::std::to_string(ordinal)};
    auto const helper{::llvm::Function::Create(function_type, ::llvm::GlobalValue::ExternalLinkage, helper_name, ir_module)};
    builder.SetInsertPoint(::llvm::BasicBlock::Create(context, "entry", helper));
    builder.CreateRet(builder.CreateAdd(helper->getArg(0), builder.getInt32(ordinal + 1)));
    auto const global_name{"data." + ::std::to_string(ordinal)};
    auto const global{new ::llvm::GlobalVariable(ir_module, integer, false, ::llvm::GlobalValue::ExternalLinkage,
        builder.getInt32(ordinal + 3), global_name)};
    ir_module.getOrInsertNamedMetadata("uwvm.test.fragment")->addOperand(
        ::llvm::MDNode::get(context, ::llvm::MDString::get(context, global_name)));
    for(unsigned index{}; index != functions; ++index)
    {
        auto const name{"function." + ::std::to_string(ordinal) + "." + ::std::to_string(index)};
        auto const function{::llvm::Function::Create(function_type, ::llvm::GlobalValue::ExternalLinkage, name, ir_module)};
        auto const scope{debug.createFunction(file, name, name, file, index + 1, subroutine, index + 1,
            ::llvm::DINode::FlagZero, ::llvm::DISubprogram::SPFlagDefinition)};
        function->setSubprogram(scope);
        auto const entry{::llvm::BasicBlock::Create(context, "entry", function)};
        auto const then_block{::llvm::BasicBlock::Create(context, "then", function)};
        auto const else_block{::llvm::BasicBlock::Create(context, "else", function)};
        auto const merge{::llvm::BasicBlock::Create(context, "merge", function)};
        builder.SetInsertPoint(entry);
        builder.SetCurrentDebugLocation(::llvm::DILocation::get(context, index + 1, 3, scope));
        auto const condition{builder.CreateICmpEQ(function->getArg(0), builder.getInt32(0))};
        builder.CreateCondBr(condition, then_block, else_block);
        builder.SetInsertPoint(then_block);
        ::llvm::Value* value{builder.CreateCall(helper, {function->getArg(0)})};
        for(unsigned operation{}; operation != instructions; ++operation)
        {
            // Keep actual instructions: no constant folding or enormous constant graph.
            value = builder.CreateAdd(value, builder.CreateLoad(integer, global));
            value = builder.CreateXor(value, builder.getInt32(operation + index + 1));
        }
        builder.CreateBr(merge);
        builder.SetInsertPoint(else_block);
        auto const other{builder.CreateSub(function->getArg(0), builder.getInt32(ordinal + index))};
        builder.CreateBr(merge);
        builder.SetInsertPoint(merge);
        auto const result{builder.CreatePHI(integer, 2)};
        result->addIncoming(value, then_block);
        result->addIncoming(other, else_block);
        builder.CreateRet(result);
        builder.SetCurrentDebugLocation({});
    }
    debug.finalize();
    require(!::llvm::verifyModule(ir_module), "input fragment must verify");
    return helper;
}
static void edge_cases()
{
    fragments empty{};
    auto merged{module("empty")};
    require(details::link_llvm_jit_module_fragments(merged, empty), "empty fragment list");
    storage absent{};
    require(!details::link_llvm_jit_module_fragments(absent, empty), "missing merged owner");
    fragments broken{};
    broken.emplace_back(module("consumed-before-missing"));
    broken.emplace_back();
    require(!details::link_llvm_jit_module_fragments(merged, broken), "missing fragment");
    require(!broken[0].llvm_module && !broken[0].llvm_context_holder, "processed owner released on later failure");

    merged = module("duplicate-definitions");
    bool diagnosed{};
    merged.llvm_context_holder->setDiagnosticHandlerCallBack(
        +[](::llvm::DiagnosticInfo const* diagnostic, void* state)
        { if(diagnostic->getSeverity() == ::llvm::DS_Error) { *static_cast<bool*>(state) = true; } }, &diagnosed);
    fragments duplicates{};
    for(unsigned i{}; i != 2; ++i)
    {
        auto owned{module("same-name")};
        ::llvm::IRBuilder<> builder{*owned.llvm_context_holder};
        auto const function{::llvm::Function::Create(::llvm::FunctionType::get(builder.getInt32Ty(), false),
            ::llvm::GlobalValue::ExternalLinkage, "duplicate", *owned.llvm_module)};
        builder.SetInsertPoint(::llvm::BasicBlock::Create(*owned.llvm_context_holder, "entry", function));
        builder.CreateRet(builder.getInt32(i));
        duplicates.emplace_back(::std::move(owned));
    }
    require(!details::link_llvm_jit_module_fragments(merged, duplicates), "duplicate strong definition rejected");
    require(diagnosed, "link diagnostic retained");
    // A failed merged graph is never accepted. A fresh owner can link again.
    merged = module("fresh-after-failure");
    fragments fresh{};
    fresh.emplace_back(module("fresh"));
    make_fragment(fresh.back(), 0, 1, 1);
    require(details::link_llvm_jit_module_fragments(merged, fresh), "fresh attempt succeeds");
    require(!::llvm::verifyModule(*merged.llvm_module), "fresh result verifies");
}
int main(int argc, char** argv)
{
    unsigned count{4}, functions{16}, instructions{32};
    if(argc == 5)
    {
        count = static_cast<unsigned>(::std::strtoul(argv[2], nullptr, 10));
        functions = static_cast<unsigned>(::std::strtoul(argv[3], nullptr, 10));
        instructions = static_cast<unsigned>(::std::strtoul(argv[4], nullptr, 10));
        require(count && count <= 64 && functions && functions <= 8192 && instructions <= 8192, "test workload bounds");
    }
    else { require(argc == 1 || argc == 2, "optional output and workload"); }
    edge_cases();
    auto merged{module("merged")};
    fragments inputs{};
    for(unsigned i{}; i != count; ++i)
    {
        auto const long_name{::std::string(8192, 'x') + ::std::to_string(i)};
        inputs.emplace_back(module(long_name.c_str()));
        make_fragment(inputs.back(), i, functions, instructions);
        inputs.back().note_first_decline(compiler::llvm_jit_compiler_decline_stage::function_prepare, i, i + 1, 0x28);
    }
    auto const start{::std::chrono::steady_clock::now()};
    require(details::link_llvm_jit_module_fragments(merged, inputs), "link all");
    auto const elapsed{::std::chrono::duration<double>(::std::chrono::steady_clock::now() - start).count()};
    for(unsigned i{}; i != count; ++i)
    {
        auto const& input{inputs[i]};
        require(!input.llvm_module && !input.llvm_context_holder && !input.emitted, "consumed IR owner");
        require(input.first_decline.function_index == i && input.first_decline.wasm_offset == i + 1, "decline diagnostic retained");
        auto const name{"helper." + ::std::to_string(i)};
        require(merged.llvm_module->getFunction(name) != nullptr, "all helper definitions retained");
    }
    require(!::llvm::verifyModule(*merged.llvm_module), "linked module verifies with debug metadata");
    require(merged.llvm_module->size() == count * (functions + 1), "complete symbol set");
    require(merged.llvm_module->getNamedMetadata("uwvm.test.fragment")->getNumOperands() == count, "all fragment metadata retained");
    if(argc > 1)
    {
        ::uwvm2::utils::container::u8string bytes{};
        { details::raw_uwvm_string_ostream stream{bytes}; ::llvm::WriteBitcodeToFile(*merged.llvm_module, stream); }
        ::fast_io::native_file file{::fast_io::mnp::os_c_str(argv[1]), ::fast_io::open_mode::out};
        auto const first{reinterpret_cast<::std::byte const*>(bytes.data())};
        ::fast_io::operations::write_all_bytes(file, first, first + bytes.size());
    }
    struct rusage usage{};
    require(::getrusage(RUSAGE_SELF, &usage) == 0, "usage");
    ::std::printf("{\"status\":\"pass\",\"link_seconds\":%.9f,\"max_rss_kib\":%ld,\"fragments\":%u,\"functions\":%u,\"instructions\":%u}\n",
        elapsed, usage.ru_maxrss, count, functions, instructions);
}
