// Copyright (c) 2026 UlteSoft. Licensed under APL-2.0 (see LICENSE).
#pragma once
#include <cstddef>
#include <memory>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>

namespace uwvm2::runtime::compiler::llvm_jit::memory_minimum_proof
{
    // Private emitter witness, not a Wasm input or an authority token.
    // It is emitted ONLY for bounds reads of a stable mmap owner whose
    // effective minimum was committed and published before execution admission.
    inline constexpr char witness_name[]{"uwvm.bounds.minimum"};

    [[nodiscard]] inline bool redundant_bounds_load(::llvm::LoadInst const& load,
        ::llvm::DataLayout const& layout) noexcept
    {
        if(!load.use_empty() || load.isVolatile() ||
           load.getOrdering()!=::llvm::AtomicOrdering::Acquire ||
           (!load.getType()->isIntegerTy(32u) && !load.getType()->isIntegerTy(64u)) ||
           load.getType()->getIntegerBitWidth()!=layout.getPointerSizeInBits()) { return false; }
        auto const* proof{load.getMetadata(witness_name)};
        auto const* range{load.getMetadata(::llvm::LLVMContext::MD_range)};
        if(proof==nullptr || proof->getNumOperands()!=2u ||
           range==nullptr || range->getNumOperands()!=2u) { return false; }
        auto const read{[](::llvm::Metadata const* m) noexcept -> ::llvm::ConstantInt const*
        {
            auto const* c{::llvm::dyn_cast<::llvm::ConstantAsMetadata>(m)};
            return c==nullptr ? nullptr : ::llvm::dyn_cast<::llvm::ConstantInt>(c->getValue());
        }};
        auto const* version{read(proof->getOperand(0u).get())};
        auto const* minimum{read(proof->getOperand(1u).get())};
        auto const* lower{read(range->getOperand(0u).get())};
        auto const* upper{read(range->getOperand(1u).get())};
        return version!=nullptr && version->getType()->isIntegerTy(32u) && version->equalsInt(1u) &&
            minimum!=nullptr && lower!=nullptr && upper!=nullptr &&
            minimum->getType()==load.getType() && lower->getType()==load.getType() &&
            upper->getType()==load.getType() && !minimum->isZero() &&
            lower->getValue()==minimum->getValue() && upper->isZero();
    }

    [[nodiscard]] inline ::std::size_t discard_unused_bounds_reads(::llvm::Module& module) noexcept
    {
        ::std::size_t removed{};
        for(auto& function:module) for(auto& block:function)
        {
            for(auto at=block.begin();at!=block.end();)
            {
                auto* load{::llvm::dyn_cast<::llvm::LoadInst>(::std::addressof(*at++))};
                if(load!=nullptr && redundant_bounds_load(*load,module.getDataLayout()))
                { load->eraseFromParent(); ++removed; }
            }
        }
        return removed;
    }
    // This is a VM-specific lowering after LLVM has proved the current size
    // unnecessary; it is NOT a rule that unused acquires are generally dead.
    // An access to a grown suffix still needs its size result and keeps the
    // acquire publishing those pages. Explicit memory.size, Guest atomics,
    // shared tag loads, relocating/provider snapshots and checkpoint/host
    // admission loads never carry this witness.
}
