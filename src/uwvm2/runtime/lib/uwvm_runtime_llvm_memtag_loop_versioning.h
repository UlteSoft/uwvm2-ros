// Experimental private-memory tag loop versioning. Not yet qualified for a
// complete runtime; the caller must retain the producer witness contract.
#pragma once
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <optional>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/AssumptionCache.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/ScalarEvolution.h>
#include <llvm/Analysis/ValueTracking.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Operator.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Support/KnownBits.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/Transforms/Utils/LoopUtils.h>

namespace uwvm_study::memtag_loop
{
    // Local experiment, deliberately not a default product policy. The host
    // selecting this mode must execute private owners with a single executor;
    // a lifetime lease or cold compiler TLS scope does not enforce exclusion.
    inline bool requested() noexcept
    {
        if(::uwvm2::extension::memtag::asynchronous_requested()) { return false; }
        auto const* value = std::getenv("UWVM_STUDY_MEMTAG_LOOP_VERSIONING");
        return value && std::strcmp(value,"exclusive-private") == 0;
    }
    constexpr char witness_name[]{"uwvm.memtag.private.minimum.v1"};
    inline bool has_private_witness(llvm::Module const& module)
    {
        for(auto const& global : module.globals())
            if(global.getMetadata(witness_name)) return true;
        return false;
    }
    struct extent { std::uint64_t first{}, last{}; };
    struct proof
    {
        llvm::LoadInst* load{};
        llvm::GlobalVariable* tags{};
        llvm::GlobalVariable* data{};
        llvm::Value* expected{};
        std::uint64_t minimum_bytes{};
        extent range{};
    };

    // All unsigned offsets must be within a known complete object prefix.
    // No inbounds, nuw/nsw or Wasm alignment hint is invented here.
    inline std::optional<extent> pointer_extent(llvm::Value* pointer, llvm::Value* base,
        std::uint64_t bound, llvm::ScalarEvolution& se, llvm::DataLayout const& dl)
    {
        if(bound == 0 || dl.getPointerSizeInBits() != 64 ||
           pointer->getType()->getPointerAddressSpace() != 0 ||
           base->getType()->getPointerAddressSpace() != 0) return {};
        extent result{};
        unsigned depth{};
        auto strip_bitcasts = [](llvm::Value* value)
        {
            while(auto* cast = llvm::dyn_cast<llvm::BitCastOperator>(value)) value = cast->getOperand(0);
            return value;
        };
        pointer = strip_bitcasts(pointer);
        while(pointer != base)
        {
            if(++depth > 8) return {};
            auto* gep = llvm::dyn_cast<llvm::GEPOperator>(pointer);
            if(!gep || gep->getPointerAddressSpace() != 0 ||
               dl.getIndexSizeInBits(gep->getPointerAddressSpace()) != 64) return {};
            llvm::SmallMapVector<llvm::Value*, llvm::APInt, 4> variables;
            llvm::APInt constant(64, 0);
            if(!gep->collectOffset(dl, 64, variables, constant) || constant.isNegative()) return {};
            auto const c = constant.getZExtValue();
            if(c >= bound || result.last >= bound - c) return {};
            result.first += c; result.last += c;
            for(auto const& entry : variables)
            {
                auto* value = entry.first;
                if(entry.second != 1 || !value->getType()->isIntegerTy() ||
                   value->getType()->getIntegerBitWidth() != 64) return {};
                auto const range = se.getUnsignedRange(se.getSCEV(value));
                auto const known = llvm::computeKnownBits(value, dl);
                auto const low = std::max(range.getUnsignedMin().getZExtValue(), known.getMinValue().getZExtValue());
                auto const high = std::min(range.getUnsignedMax().getZExtValue(), known.getMaxValue().getZExtValue());
                if(low > high || high >= bound || result.last >= bound - high) return {};
                result.first += low; result.last += high;
            }
            pointer = strip_bitcasts(gep->getPointerOperand());
        }
        if(result.last >= bound) return {};
        return result;
    }

    inline llvm::MDNode* witness(llvm::LoadInst& load)
    {
        if(auto* metadata = load.getMetadata(witness_name)) return metadata;
        auto* tags = llvm::dyn_cast<llvm::GlobalVariable>(llvm::getUnderlyingObject(load.getPointerOperand()));
        return tags ? tags->getMetadata(witness_name) : nullptr;
    }

    inline std::optional<proof> decode(llvm::LoadInst& load, llvm::Loop& loop,
        llvm::ScalarEvolution& se, llvm::DominatorTree& dt, llvm::AssumptionCache& ac)
    {
        auto* md = witness(load);
        if(!md || md->getNumOperands() != 4 || load.isAtomic() || load.isVolatile() ||
           !load.getType()->isIntegerTy(8)) return {};
        auto number = [](llvm::Metadata* metadata) -> llvm::ConstantInt*
        {
            auto* value = llvm::dyn_cast_or_null<llvm::ConstantAsMetadata>(metadata);
            return value ? llvm::dyn_cast<llvm::ConstantInt>(value->getValue()) : nullptr;
        };
        auto global = [](llvm::Metadata* metadata) -> llvm::GlobalVariable*
        {
            auto* value = llvm::dyn_cast_or_null<llvm::ValueAsMetadata>(metadata);
            return value ? llvm::dyn_cast<llvm::GlobalVariable>(value->getValue()) : nullptr;
        };
        auto* revision = number(md->getOperand(0));
        auto* minimum = number(md->getOperand(3));
        auto* tags = global(md->getOperand(1));
        auto* data = global(md->getOperand(2));
        if(!revision || !revision->equalsInt(1) || !minimum || !minimum->getType()->isIntegerTy(64) ||
           !tags || !data || tags == data || tags->getAddressSpace() != 0 || data->getAddressSpace() != 0) return {};
        auto const bytes = minimum->getZExtValue();
        if(bytes == 0 || bytes > (std::uint64_t(1) << 56)) return {};
        auto const count = bytes / 16 + (bytes % 16 != 0);
        auto range = pointer_extent(load.getPointerOperand(), tags, count, se, load.getModule()->getDataLayout());
        if(!range || range->last - range->first >= 4096) return {};
        llvm::Value* expected{};
        for(auto* user : load.users())
        {
            auto* compare = llvm::dyn_cast<llvm::ICmpInst>(user);
            if(!compare || !compare->isEquality()) continue;
            auto* other = compare->getOperand(compare->getOperand(0) == &load ? 1 : 0);
            if(!loop.isLoopInvariant(other) || !other->getType()->isIntegerTy(8)) continue;
            auto* insertion = loop.getLoopPreheader()->getTerminator();
            if(auto* instruction = llvm::dyn_cast<llvm::Instruction>(other); instruction && !dt.dominates(instruction, insertion)) continue;
            if(!llvm::isGuaranteedNotToBeUndefOrPoison(other, &ac, insertion, &dt)) continue;
            if(expected && expected != other) return {};
            expected = other;
        }
        if(!expected) return {};
        return proof{&load, tags, data, expected, bytes, *range};
    }

    inline bool readonly_integer_intrinsic(llvm::CallBase& call)
    {
        auto* intrinsic = llvm::dyn_cast<llvm::IntrinsicInst>(&call);
        if(!intrinsic || !call.doesNotAccessMemory() || !call.doesNotThrow()) return false;
        switch(intrinsic->getIntrinsicID())
        {
            case llvm::Intrinsic::fshl: case llvm::Intrinsic::fshr:
            case llvm::Intrinsic::bswap: case llvm::Intrinsic::ctpop:
            case llvm::Intrinsic::ctlz: case llvm::Intrinsic::cttz: return true;
            default: return false;
        }
    }

    inline bool access_stays_in_data(llvm::Value* pointer, llvm::Type* type, llvm::ArrayRef<proof> proofs,
        llvm::ScalarEvolution& se, llvm::DataLayout const& dl)
    {
        auto size = dl.getTypeStoreSize(type);
        if(size.isScalable() || size.getFixedValue() == 0) return false;
        auto const width = size.getFixedValue();
        auto* underlying = llvm::getUnderlyingObject(pointer);
        std::uint64_t bound{};
        for(auto const& proof : proofs)
        {
            if(underlying == proof.tags) return false;
            if(underlying == proof.data) bound = bound ? std::min(bound, proof.minimum_bytes) : proof.minimum_bytes;
        }
        if(!bound)
        {
            auto* alloca = llvm::dyn_cast<llvm::AllocaInst>(underlying);
            if(!alloca) return false;
            auto allocation_size = alloca->getAllocationSize(dl);
            if(!allocation_size || allocation_size->isScalable()) return false;
            bound = allocation_size->getFixedValue();
        }
        auto range = pointer_extent(pointer, underlying, bound, se, dl);
        return range && width <= bound - range->last;
    }

    inline unsigned trip_budget(llvm::Loop& loop, llvm::ScalarEvolution& se)
    {
        // Early MTE/bounds exits need not have a known count. The counted
        // latch is only a profitability filter, not a correctness premise.
        auto* latch = loop.getLoopLatch();
        return latch && loop.isLoopExiting(latch) ? se.getSmallConstantTripCount(&loop, latch) : 0;
    }

    inline bool qualify(llvm::Loop& loop, llvm::SmallVectorImpl<proof>& proofs,
        llvm::ScalarEvolution& se, llvm::DominatorTree& dt, llvm::AssumptionCache& ac)
    {
        if(!loop.getSubLoops().empty() || !loop.getLoopPreheader() || !loop.getLoopLatch() ||
           !loop.isLoopSimplifyForm() || trip_budget(loop, se) < 32768) return false;
        unsigned instructions{};
        for(auto* block : loop.blocks()) for(auto& instruction : *block)
        {
            if(++instructions > 512) return false;
            if(auto* load = llvm::dyn_cast<llvm::LoadInst>(&instruction))
            {
                if(load->isAtomic()) return false;
                if(witness(*load))
                {
                    auto proof = decode(*load, loop, se, dt, ac);
                    if(!proof) return false;
                    proofs.push_back(*proof);
                }
            }
            if(auto* call = llvm::dyn_cast<llvm::CallBase>(&instruction); call && !readonly_integer_intrinsic(*call)) return false;
            // LLVM treats volatile reads as writing for ordering purposes;
            // these are admitted only by the bounded private-data check below.
            if(instruction.mayWriteToMemory() && !llvm::isa<llvm::StoreInst, llvm::LoadInst>(&instruction)) return false;
            if(llvm::isa<llvm::FenceInst, llvm::AtomicCmpXchgInst, llvm::AtomicRMWInst,
                         llvm::IndirectBrInst, llvm::CallBrInst>(&instruction)) return false;
        }
        if(proofs.empty() || proofs.size() > 16) return false;
        auto const& dl = loop.getHeader()->getModule()->getDataLayout();
        for(auto* block : loop.blocks()) for(auto& instruction : *block)
        {
            if(auto* store = llvm::dyn_cast<llvm::StoreInst>(&instruction);
               store && (store->isAtomic() || !access_stays_in_data(store->getPointerOperand(),store->getValueOperand()->getType(),proofs,se,dl))) return false;
            if(auto* load = llvm::dyn_cast<llvm::LoadInst>(&instruction);
               load && load->isVolatile() && !access_stays_in_data(load->getPointerOperand(),load->getType(),proofs,se,dl)) return false;
        }
        return true;
    }

    inline bool version(llvm::Loop& loop, llvm::ArrayRef<proof> proofs, llvm::LoopInfo& li,
        llvm::DominatorTree& dt, llvm::ScalarEvolution& se)
    {
        struct group { llvm::GlobalVariable* tags; llvm::Value* expected; extent range; };
        llvm::SmallVector<group, 8> groups;
        for(auto const& p : proofs)
        {
            auto it = std::find_if(groups.begin(), groups.end(), [&](auto const& g) { return g.tags == p.tags && g.expected == p.expected; });
            if(it == groups.end()) groups.push_back({p.tags,p.expected,p.range});
            else { it->range.first=std::min(it->range.first,p.range.first); it->range.last=std::max(it->range.last,p.range.last); }
        }
        // Two individually small ranges may be far apart. Bound the merged
        // scan and the total work before mutating any part of the function.
        std::uint64_t total{};
        for(auto const& group : groups)
        {
            auto const length = group.range.last - group.range.first + 1;
            if(length > 4096 || total > 4096 - length) return false;
            total += length;
        }
        llvm::formLCSSA(loop, dt, &li, &se);
        llvm::SmallVector<llvm::BasicBlock*,8> exits;
        loop.getUniqueExitBlocks(exits);
        auto* check = loop.getLoopPreheader();
        auto* function = check->getParent();
        auto* slow_preheader = llvm::SplitBlock(check, check->getTerminator(), &dt, &li, nullptr, "memtag.slow.preheader");
        llvm::ValueToValueMapTy map;
        llvm::SmallVector<llvm::BasicBlock*,16> cloned;
        auto* fast = llvm::cloneLoopWithPreheader(slow_preheader,check,&loop,map,".memtag.proved",&li,&dt,cloned);
        llvm::remapInstructionsInBlocks(cloned,map);
        for(auto* exit : exits) for(auto& phi : exit->phis())
        {
            auto const n = phi.getNumIncomingValues();
            for(unsigned i=0;i!=n;++i)
            {
                auto* old_block = phi.getIncomingBlock(i);
                auto block = map.find(old_block);
                if(block == map.end() || !loop.contains(old_block)) continue;
                auto* value = phi.getIncomingValue(i);
                if(auto found=map.find(value); found!=map.end()) value=found->second;
                phi.addIncoming(value,llvm::cast<llvm::BasicBlock>(block->second));
            }
        }
        for(auto const& proof : proofs)
        {
            auto* load = llvm::cast<llvm::LoadInst>(map.lookup(proof.load));
            load->replaceAllUsesWith(proof.expected);
            load->eraseFromParent();
        }
        // The original loop remains the complete fallback. No failed preflight
        // reports a trap or touches guest data; it merely selects that fallback.
        check->getTerminator()->eraseFromParent();
        llvm::IRBuilder<> builder(check);
        for(auto const& group : groups)
        {
            auto* entry = builder.GetInsertBlock();
            auto* scan = llvm::BasicBlock::Create(function->getContext(),"memtag.preflight.scan",function,slow_preheader);
            auto* matched = llvm::BasicBlock::Create(function->getContext(),"memtag.preflight.matched",function,slow_preheader);
            auto* done = llvm::BasicBlock::Create(function->getContext(),"memtag.preflight.done",function,slow_preheader);
            builder.CreateBr(scan); builder.SetInsertPoint(scan);
            auto* index=builder.CreatePHI(builder.getInt64Ty(),2,"memtag.preflight.index");
            index->addIncoming(builder.getInt64(group.range.first),entry);
            auto* pointer=builder.CreateGEP(builder.getInt8Ty(),group.tags,index);
            auto* tag=builder.CreateLoad(builder.getInt8Ty(),pointer);tag->setAlignment(llvm::Align(1));
            builder.CreateCondBr(builder.CreateICmpEQ(tag,group.expected),matched,slow_preheader);
            builder.SetInsertPoint(matched);
            auto* next=builder.CreateAdd(index,builder.getInt64(1));
            index->addIncoming(next,matched);
            builder.CreateCondBr(builder.CreateICmpULT(next,builder.getInt64(group.range.last+1)),scan,done);
            builder.SetInsertPoint(done);
        }
        builder.CreateBr(fast->getLoopPreheader());
        function->addFnAttr("uwvm.memtag.loop.proved.v1");
        dt.recalculate(*function);
        return true;
    }

    // Admission describes the generated module, not only the compiler host.
    // Cross-target tools can run on x86-64 while consuming AArch64/ARM IR.
    inline bool supported_target(llvm::Module const& module)
    {
        auto const triple = llvm::Triple(module.getTargetTriple());
        auto const& layout = module.getDataLayout();
        return triple.getArch() == llvm::Triple::x86_64 && layout.isLittleEndian() &&
            layout.getPointerSizeInBits(0) == 64 && layout.getIndexSizeInBits(0) == 64;
    }

    struct pass : llvm::PassInfoMixin<pass>
    {
        llvm::PreservedAnalyses run([[maybe_unused]] llvm::Function& function, [[maybe_unused]] llvm::FunctionAnalysisManager& am)
        {
#if defined(__x86_64__) || defined(_M_X64)
            if(!supported_target(*function.getParent()) || function.isDeclaration() ||
               function.hasFnAttribute(llvm::Attribute::OptimizeNone) ||
               function.hasFnAttribute("uwvm.memtag.loop.proved.v1") ||
               function.hasFnAttribute("uwvm.native.debug.pcrel")) return llvm::PreservedAnalyses::all();
            auto& li=am.getResult<llvm::LoopAnalysis>(function);
            auto& dt=am.getResult<llvm::DominatorTreeAnalysis>(function);
            auto& se=am.getResult<llvm::ScalarEvolutionAnalysis>(function);
            auto& ac=am.getResult<llvm::AssumptionAnalysis>(function);
            llvm::Loop* selected{};unsigned best{};
            llvm::SmallVector<proof,16> selected_proofs;
            for(auto* loop : li.getLoopsInPreorder())
            {
                llvm::SmallVector<proof,16> proofs;
                if(!qualify(*loop,proofs,se,dt,ac)) continue;
                auto const count=trip_budget(*loop,se);
                if(count>best) {best=count;selected=loop;selected_proofs=std::move(proofs);}
            }
            if(selected)
            {
                if(version(*selected,selected_proofs,li,dt,se))
                    return llvm::PreservedAnalyses::none();
            }
#endif
            return llvm::PreservedAnalyses::all();
        }
    };
}
