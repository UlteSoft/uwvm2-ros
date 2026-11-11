// Caller must prove the complete data range before using this tag proof.
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <llvm/IR/IRBuilder.h>
# include <llvm/IR/Module.h>
# include <llvm/IR/MDBuilder.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details
{
    // The caller proves ordinary bounds and supplies a canonical, untagged
    // native mapping. A true witness is native allocator metadata, never a
    // Guest alignment hint or pointer value. Integer addition is deliberately
    // wrapping IR: even invalid inputs cannot manufacture poison/inbounds UB.
    [[nodiscard]] inline ::llvm::Value* emit_memtag_hardware_pointer(::llvm::IRBuilder<>& b,
        ::llvm::Value* base,::llvm::Value* raw,::llvm::Value* checked_pointer,
        ::std::uint64_t static_offset,bool canonical_owner) noexcept
    {
        if(!base || !raw || !checked_pointer || !base->getType()->isPointerTy() ||
           !checked_pointer->getType()->isPointerTy() ||
           (!raw->getType()->isIntegerTy(32u) && !raw->getType()->isIntegerTy(64u)) ||
           b.GetInsertBlock()->getModule()->getDataLayout().getPointerSizeInBits()!=64u) { return nullptr; }
        auto* const i64{b.getInt64Ty()};
        if(canonical_owner && raw->getType()->isIntegerTy(64u))
        {
            auto* offset{b.CreateAnd(raw,b.getInt64(0x0fffffffffffffffull))};
            offset=b.CreateAdd(offset,b.getInt64(static_offset));
            return b.CreateIntToPtr(b.CreateAdd(b.CreatePtrToInt(base,i64),offset),
                checked_pointer->getType(),"memory.hardware.tagged");
        }
        auto* const tag{raw->getType()->isIntegerTy(32u) ?
            b.CreateZExt(b.CreateLShr(raw,28u),i64) :
            b.CreateAnd(b.CreateLShr(raw,56u),b.getInt64(15u))};
        auto* address{b.CreatePtrToInt(checked_pointer,i64)};
        if(!canonical_owner) { address=b.CreateAnd(address,b.getInt64(0x00ffffffffffffffull)); }
        return b.CreateIntToPtr(b.CreateOr(address,b.CreateShl(tag,b.getInt64(56u))),
            checked_pointer->getType(),"memory.hardware.tagged");
    }

    // One byte per16-byte granule. A <=16-byte access touches at most two
    // granules; check both BEFORE any data load/store, including unaligned SIMD.
    // offset is a nonwrapping u64, within the SAME stable native owner as tags.
    // Failure callback receives raw pointer, granule starting BYTE offset and
    // pointer/stored tags. Synchronous failure must terminate its IR block;
    // deferred failure may return and rejoins the checked data access below.
    // No data pointer escapes this helper.
    template<typename Failure>
    [[nodiscard]] inline bool emit_memtag_scalar_check(::llvm::IRBuilder<>& b,
        ::llvm::Value* tags, ::llvm::Value* raw, ::llvm::Value* offset,
        ::std::size_t bytes, bool shared, Failure&& fail,
        ::llvm::Value* private_data = nullptr, ::std::uint64_t immutable_minimum = 0,
        bool deferred_fault=false, ::std::uint8_t comparison_mask=0xffu) noexcept
    {
        if(!tags || !raw || !offset || bytes==0u || bytes>16u ||
           !tags->getType()->isPointerTy() || !offset->getType()->isIntegerTy(64u) ||
           (!raw->getType()->isIntegerTy(32u) && !raw->getType()->isIntegerTy(64u))) {return false;}
        auto const bits{b.GetInsertBlock()->getModule()->getDataLayout().getPointerSizeInBits()};
        if(bits!=32u && bits!=64u) {return false;}
        if(comparison_mask==0u) { return true; } // Authenticated active MTE mapping checks the complete 4-bit tag.
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
            // Translator-owned witness: tags/data are distinct stable native
            // reservations, private execution cannot retag concurrently, and
            // the declared minimum remains committed across restore/growth.
            // Guest custom metadata never supplies this witness.
            if(!deferred_fault && comparison_mask==0xffu && !shared && bits == 64u && private_data && immutable_minimum != 0u)
            {
                auto* tag_global = ::llvm::dyn_cast<::llvm::GlobalVariable>(tags->stripPointerCasts());
                auto* data_global = ::llvm::dyn_cast<::llvm::GlobalVariable>(private_data->stripPointerCasts());
                if(tag_global && data_global && tag_global != data_global)
                    tag_global->setMetadata("uwvm.memtag.private.minimum.v1", ::llvm::MDNode::get(b.getContext(), {
                        ::llvm::ConstantAsMetadata::get(b.getInt32(1u)),
                        ::llvm::ValueAsMetadata::get(tag_global),
                        ::llvm::ValueAsMetadata::get(data_global),
                        ::llvm::ConstantAsMetadata::get(b.getInt64(immutable_minimum))}));
            }
            auto const fn{b.GetInsertBlock()->getParent()};
            auto const bad{::llvm::BasicBlock::Create(b.getContext(),"memtag.mismatch",fn)};
            auto const good{::llvm::BasicBlock::Create(b.getContext(),"memtag.matches",fn)};
            // A partial hardware-assisted tag compares only the upper nibble.
            // XOR/TST avoids two separate masks while retaining the full byte
            // for diagnostics. The ordinary full-byte path keeps its exact IR.
            auto* const mismatch{comparison_mask==0xffu ? b.CreateICmpNE(tag,stored) :
                b.CreateICmpNE(b.CreateAnd(b.CreateXor(tag,stored),b.getInt8(comparison_mask)),b.getInt8(0u))};
            // This is layout/profile information, never a tag assumption.
            // Both successors and every diagnostic side effect remain present.
            b.CreateCondBr(mismatch,bad,good,
                ::llvm::MDBuilder{b.getContext()}.createBranchWeights(1u,2000u));
            b.SetInsertPoint(bad);
            // The common fault ABI uses the granule's starting BYTE offset,
            // matching shadow_view::check, not the shadow-array element index.
            fail(b.CreateZExtOrTrunc(raw,b.getInt64Ty()),b.CreateShl(granule,b.getInt64(4u)),
                b.CreateZExt(tag,b.getInt32Ty()),b.CreateZExt(stored,b.getInt32Ty()));
            // LLVM 23 getTerminator() requires a well-formed block and no
            // longer returns null for an unterminated block. Inspect the last
            // instruction without asking that API to assume the very property
            // being checked. This is portable to earlier LLVM providers too.
            auto const failure_block{b.GetInsertBlock()};
            if(failure_block == nullptr) { return false; }
            if(failure_block->empty() || !failure_block->back().isTerminator())
            {
                if(!deferred_fault) { return false; }
                b.CreateBr(good);
            }
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
