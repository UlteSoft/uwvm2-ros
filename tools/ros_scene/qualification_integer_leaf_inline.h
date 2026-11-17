#pragma once
// Qualification-only compiler experiment. No live runtime includes this file.
#include <uwvm2/utils/container/standard.h>
#include <span>
#include <llvm/IR/Constants.h>
#include <llvm/IR/ConstantRange.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/Utils/Cloning.h>
namespace uwvm2::runtime::qualification::integer_leaf_inline
{
    inline constexpr ::std::size_t maximum_instructions{32u};
    inline constexpr ::std::size_t maximum_entries{65536u};
    inline constexpr ::std::size_t maximum_edges{65536u};
    enum class status { complete, invalid_owner, resource_limit, allocation_failure, invalid_ir, inline_failure };
    struct result
    {
        status state{status::complete};
        ::std::size_t leaves{}, edges{}, rejected_attributes{}, rejected_metadata{}, rejected_tail{}, rejected_shape{};
    };
    [[nodiscard]] inline bool integer_type(::llvm::Type const* type) noexcept
    { return type!=nullptr && type->isIntegerTy() && type->getIntegerBitWidth()<=64u; }
    [[nodiscard]] inline bool signature_type(::llvm::Type const* type) noexcept
    { return type!=nullptr && (type->isIntegerTy(32u) || type->isIntegerTy(64u)); }

    /** @brief Admit the exact boolean return-range fact only with a structural witness.
     * @details Actual optimized isalpha returns zext i1 to i32 and LLVM adds
     * range(i32 0, 2). This helper does not trust an arbitrary inferred range:
     * the sole attribute must describe [0,2), and the actual return operand
     * must be a local zero-extension of i1 to the exact result type. Remaining
     * body checks still reject memory, poison flags, calls and all instruction
     * metadata. Public and cloned attributes remain intact; call-site
     * attributes/metadata remain separately forbidden. Poison on a used input
     * is not sanitized or declared noundef by this qualification experiment.
     */
    [[nodiscard]] inline bool return_attributes_proved(::llvm::Function const& function) noexcept
    {
        auto attributes=function.getAttributes().getRetAttrs();
        if(!attributes.hasAttributes()) { return true; }
        if(attributes.getNumAttributes()!=1u || !attributes.hasAttribute(::llvm::Attribute::Range) ||
           function.size()!=1u || !signature_type(function.getReturnType())) { return false; }
        auto const& range=attributes.getAttribute(::llvm::Attribute::Range).getRange();
        if(range.getBitWidth()!=function.getReturnType()->getIntegerBitWidth() ||
           !range.getLower().isZero() || range.getUpper()!=2u) { return false; }
        auto const* returned=::llvm::dyn_cast_or_null<::llvm::ReturnInst>(function.front().getTerminator());
        auto const* extended=returned?::llvm::dyn_cast_or_null<::llvm::ZExtInst>(returned->getReturnValue()):nullptr;
        return extended!=nullptr && extended->getFunction()==&function && extended->getSrcTy()->isIntegerTy(1u) &&
            extended->getDestTy()==function.getReturnType();
    }

    /** @brief Admit only total, bounded, memory-free integer leaf bodies.
     * @details This runs after the ordinary optimizer, on actual compiler-owned
     * public entries supplied by the owning materializer. It never trusts guest
     * names or a caller-provided numeric address. One basic block and at most 32
     * instructions exclude loops; the closed opcode/operand lists exclude memory,
     * allocation, FP, division, calls, traps, GC, pointer carriers and observers.
     * Poison-generating flags and undef/poison/constant-expression operands are
     * rejected. No public entry attribute, body or raw wrapper is changed.
     */
    [[nodiscard]] inline bool eligible(::llvm::Function const& function) noexcept
    {
        if(function.isDeclaration() || function.size()!=1u || function.isVarArg() || function.arg_size()>8u ||
           !signature_type(function.getReturnType()) || function.hasPersonalityFn() || function.hasGC() ||
           function.getSubprogram()!=nullptr || function.hasFnAttribute(::llvm::Attribute::Convergent) ||
           function.hasFnAttribute(::llvm::Attribute::ReturnsTwice) || function.hasFnAttribute(::llvm::Attribute::Naked) ||
           function.hasFnAttribute(::llvm::Attribute::OptimizeNone) ||
           !return_attributes_proved(function)) { return false; }
        for(auto const& argument:function.args())
        {
            if(!signature_type(argument.getType()) ||
               function.getAttributes().getParamAttrs(argument.getArgNo()).hasAttributes()) { return false; }
        }
        ::std::size_t count{};
        for(auto const& instruction:function.front())
        {
            if(++count>maximum_instructions || instruction.hasMetadata() || instruction.hasPoisonGeneratingFlags()) { return false; }
            switch(instruction.getOpcode())
            {
                case ::llvm::Instruction::Ret: break;
                case ::llvm::Instruction::Add: case ::llvm::Instruction::Sub:
                case ::llvm::Instruction::And: case ::llvm::Instruction::Or: case ::llvm::Instruction::Xor:
                case ::llvm::Instruction::ICmp: case ::llvm::Instruction::Select:
                case ::llvm::Instruction::ZExt: case ::llvm::Instruction::SExt: case ::llvm::Instruction::Trunc:
                    if(!integer_type(instruction.getType())) { return false; }
                    break;
                default: return false;
            }
            for(auto const& operand:instruction.operands())
            {
                auto const* value=operand.get();
                if(!integer_type(value->getType())) { return false; }
                if(auto const* argument=::llvm::dyn_cast<::llvm::Argument>(value))
                { if(argument->getParent()!=&function) { return false; } }
                else if(auto const* input=::llvm::dyn_cast<::llvm::Instruction>(value))
                { if(input->getFunction()!=&function) { return false; } }
                else if(!::llvm::isa<::llvm::ConstantInt>(value)) { return false; }
            }
        }
        return count!=0u && ::llvm::isa<::llvm::ReturnInst>(function.front().getTerminator());
    }

    /** @brief Inline private temporary copies on exact ordinary direct edges.
     * @details The caller passes the same materializer-owned typed entry list
     * for target and caller selection. Raw wrappers and arbitrary helper functions
     * are absent. Tail/invoke/callbr, any call-site attributes, bundles or metadata
     * are refused. Public/indirect targets remain the original function. Each
     * private copy is deleted before returning; no private address or range can
     * escape into publication. The outer owner must reject any non-complete
     * result before entry publication and unwind its actual private engine state.
     * All component-owned dynamic containers use recoverable fast_io allocators;
     * LLVM's own internal allocation policy remains an upstream qualification limit.
     */
    [[nodiscard]] inline result run(::llvm::Module& module,::std::span<::llvm::Function* const> entries) noexcept
    {
        result outcome{};
        if(entries.size()>maximum_entries) { outcome.state=status::resource_limit;return outcome; }
        try
        {
            ::uwvm2::utils::container::std_set<::llvm::Function*> actual{};
            for(auto* entry:entries)
            {
                if(entry==nullptr || entry->getParent()!=&module || !actual.insert(entry).second)
                { outcome.state=status::invalid_owner;return outcome; }
            }
            if(::llvm::verifyModule(module)) { outcome.state=status::invalid_ir;return outcome; }
            ::uwvm2::utils::container::std_set<::llvm::Function*> leaves{};
            for(auto* entry:entries) { if(eligible(*entry)) { leaves.insert(entry); } }
            outcome.leaves=leaves.size();
            ::uwvm2::utils::container::std_vector<::llvm::CallInst*> calls{};
            for(auto* caller:entries) for(auto& block:*caller) for(auto& instruction:block)
            {
                auto* call=::llvm::dyn_cast<::llvm::CallBase>(&instruction);
                if(call==nullptr || !leaves.contains(call->getCalledFunction())) { continue; }
                auto* ordinary=::llvm::dyn_cast<::llvm::CallInst>(call);
                if(ordinary==nullptr || ordinary->isTailCall()) { ++outcome.rejected_tail;continue; }
                if(!call->getAttributes().isEmpty()) { ++outcome.rejected_attributes;continue; }
                if(call->hasMetadata() || call->hasOperandBundles()) { ++outcome.rejected_metadata;continue; }
                auto* target=call->getCalledFunction();
                if(target==caller || call->getFunctionType()!=target->getFunctionType() ||
                   call->getCallingConv()!=target->getCallingConv() || call->arg_size()!=target->arg_size())
                { ++outcome.rejected_shape;continue; }
                if(calls.size()==maximum_edges) { outcome.state=status::resource_limit;return outcome; }
                calls.push_back(ordinary);
            }
            for(auto* call:calls)
            {
                auto* target=call->getCalledFunction();
                ::llvm::ValueToValueMapTy values{};
                auto* body=::llvm::CloneFunction(target,values);
                if(body==nullptr || body->getParent()!=&module) { outcome.state=status::invalid_ir;return outcome; }
                body->setName(target->getName()+".integer.inline.body");
                body->setLinkage(::llvm::GlobalValue::InternalLinkage);
                body->removeFnAttr(::llvm::Attribute::NoInline);
                body->removeFnAttr(::llvm::Attribute::NoMerge);
                body->removeFnAttr("nooutline");
                body->addFnAttr(::llvm::Attribute::AlwaysInline);
                call->setCalledFunction(body);
                ::llvm::InlineFunctionInfo info{};
                if(!::llvm::InlineFunction(*call,info,false).isSuccess())
                {
                    call->setCalledFunction(target);
                    if(body->use_empty()) { body->eraseFromParent(); }
                    outcome.state=status::inline_failure;return outcome;
                }
                if(!body->use_empty()) { outcome.state=status::invalid_ir;return outcome; }
                body->eraseFromParent();++outcome.edges;
            }
            if(::llvm::verifyModule(module)) { outcome.state=status::invalid_ir; }
        }
        catch(...) { outcome.state=status::allocation_failure; }
        return outcome;
    }
}
