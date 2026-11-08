// Verify the actual disabled lowering emits no tag machinery even at -O0.
#include <uwvm2/runtime/compiler/llvm_jit/compile_all_from_uwvm/translate/memtag_disabled_ir.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Module.h>
#include <array>
#include <cstdlib>
static void check(bool value) { if(!value) { std::abort(); } }
#include <cstdio>
namespace mt = uwvm2::extension::memtag;
namespace jit = uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
int main()
{
    llvm::LLVMContext context;
    llvm::Module module{"memtag-disabled", context};
    unsigned checks{};
    for(unsigned bits : {32u, 64u})
    {
        llvm::IRBuilder<> b{context};
        auto const address_type{b.getIntNTy(bits)};
        auto const fn{llvm::Function::Create(llvm::FunctionType::get(b.getVoidTy(),
            {address_type,address_type,address_type,address_type},false),llvm::Function::ExternalLinkage,
            bits == 32u ? "disabled32" : "disabled64", module)};
        auto const block{llvm::BasicBlock::Create(context,"entry",fn)};
        b.SetInsertPoint(block);
        std::array<llvm::Value*,4> args{fn->getArg(0),fn->getArg(1),fn->getArg(2),fn->getArg(3)};
        for(unsigned raw{}; raw != 256u; ++raw)
        {
            auto const* descriptor{mt::find_instruction(raw)};
            if(!descriptor) { continue; }
            unsigned zero_calls{};
            auto zero = [&](llvm::Value* address,llvm::Value* length) noexcept {
                check(address == args[0] && length == args[1]); ++zero_calls;
                // A test sentinel represents the selected normal bounds-checked
                // memory.fill path; no tag-only declaration may be introduced.
                auto type = llvm::FunctionType::get(b.getVoidTy(),{address_type,address_type},false);
                auto callee = module.getOrInsertFunction(bits == 32u ? "checked_fill32" : "checked_fill64",type);
                b.CreateCall(callee,{address,length}); return true;
            };
            auto const before{block->size()};
            auto const op{static_cast<mt::opcode>(raw)};
            auto const result{jit::emit_disabled_memtag_ir(b,op,address_type,
                std::span<llvm::Value* const>{args.data(),descriptor->operand_count},zero)};
            if(op == mt::opcode::add)
            { check(!result.valid && block->size() == before && zero_calls == 0u); ++checks; continue; }
            check(result.valid);
            check(zero_calls == static_cast<unsigned>(descriptor->zeroes_memory));
            auto const expected{static_cast<unsigned>(descriptor->zeroes_memory || op == mt::opcode::sub)};
            check(block->size() == before + expected);
            if(descriptor->result_count == 0u) { check(result.value == nullptr); }
            else if(op == mt::opcode::status || op == mt::opcode::extract || op == mt::opcode::tagbits)
            { check(llvm::cast<llvm::ConstantInt>(result.value)->isZero()); }
            else if(op == mt::opcode::startbit)
            { check(llvm::cast<llvm::ConstantInt>(result.value)->getZExtValue() == bits); }
            else if(op == mt::opcode::sub)
            { check(llvm::cast<llvm::Instruction>(result.value)->getOpcode() == llvm::Instruction::Sub); }
            else { check(result.value == args[0]); }
            ++checks;
        }
        auto const before{block->size()};
        auto reject_zero = [](llvm::Value*,llvm::Value*) noexcept { check(false); return false; };
        check(!jit::emit_disabled_memtag_ir(b,mt::opcode::random,address_type,{},reject_zero).valid);
        std::array<llvm::Value*,1> wrong{b.getInt8(0)};
        check(!jit::emit_disabled_memtag_ir(b,mt::opcode::random,address_type,wrong,reject_zero).valid);
        check(!jit::emit_disabled_memtag_ir(b,static_cast<mt::opcode>(0x32u),address_type,{},reject_zero).valid);
        check(block->size() == before);
        b.CreateRetVoid();
    }
    check(!llvm::verifyModule(module,&llvm::errs()));
    // Two generated functions, two normal fill declarations; no MTE helpers.
    check(module.size() == 4u);
    std::printf("PASS disabled LLVM IR: %u opcode/width cases, no tag instructions or helpers, zeroing preserved\n",checks);
}
