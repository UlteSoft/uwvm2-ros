// Caller must prove the complete data range before using this tag proof.
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <llvm/IR/IRBuilder.h>
# include <llvm/IR/Module.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details
{
    // One byte per16-byte granule. A <=16-byte access touches at most two
    // granules; check both BEFORE any data load/store, including unaligned SIMD.
    // offset is a nonwrapping u64, within the SAME stable native owner as tags.
    // Failure callback receives raw pointer, granule index, pointer/stored tags,
    // and must terminate its IR block. No data pointer escapes this helper.
    template<typename Failure>
    [[nodiscard]] inline bool emit_memtag_scalar_check(::llvm::IRBuilder<>& b,
        ::llvm::Value* tags, ::llvm::Value* raw, ::llvm::Value* offset,
        ::std::size_t bytes, bool shared, Failure&& fail) noexcept
    {
        if(!tags || !raw || !offset || bytes==0u || bytes>16u ||
           !tags->getType()->isPointerTy() || !offset->getType()->isIntegerTy(64u) ||
           (!raw->getType()->isIntegerTy(32u) && !raw->getType()->isIntegerTy(64u))) {return false;}
        auto const bits{b.GetInsertBlock()->getModule()->getDataLayout().getPointerSizeInBits()};
        if(bits!=32u && bits!=64u) {return false;}
        auto const tag{b.CreateTrunc(b.CreateLShr(raw,raw->getType()->isIntegerTy(32u)?28u:56u),
            b.getInt8Ty(),"memtag.pointer")};
        auto const first{b.CreateLShr(offset,4u,"memtag.first")};
        auto const check{[&](::llvm::Value* granule) noexcept
        {
            auto const index{b.CreateZExtOrTrunc(granule,b.getIntNTy(bits))};
            auto const address{b.CreateGEP(b.getInt8Ty(),tags,index,"memtag.address")};
            auto const stored{b.CreateLoad(b.getInt8Ty(),address,"memtag.allocation")};
            stored->setAlignment(::llvm::Align{1u});
            if(shared) {stored->setAtomic(::llvm::AtomicOrdering::Monotonic);}
            auto const fn{b.GetInsertBlock()->getParent()};
            auto const bad{::llvm::BasicBlock::Create(b.getContext(),"memtag.mismatch",fn)};
            auto const good{::llvm::BasicBlock::Create(b.getContext(),"memtag.matches",fn)};
            b.CreateCondBr(b.CreateICmpNE(tag,stored),bad,good);b.SetInsertPoint(bad);
            fail(b.CreateZExtOrTrunc(raw,b.getInt64Ty()),granule,
                b.CreateZExt(tag,b.getInt32Ty()),b.CreateZExt(stored,b.getInt32Ty()));
            if(!b.GetInsertBlock()->getTerminator()) {return false;}
            b.SetInsertPoint(good);return true;
        }};
        if(!check(first)) {return false;}
        if(bytes!=1u)
        {
            auto const last{b.CreateLShr(b.CreateAdd(offset,b.getInt64(bytes-1u)),4u,"memtag.last")};
            auto const fn{b.GetInsertBlock()->getParent()};
            auto const second{::llvm::BasicBlock::Create(b.getContext(),"memtag.second",fn)};
            auto const done{::llvm::BasicBlock::Create(b.getContext(),"memtag.checked",fn)};
            // Contained accesses perform one tag load even for atomic shadows.
            b.CreateCondBr(b.CreateICmpNE(first,last),second,done);b.SetInsertPoint(second);
            if(!check(last)) {return false;}
            b.CreateBr(done);b.SetInsertPoint(done);
        }
        return true;
    }
}
