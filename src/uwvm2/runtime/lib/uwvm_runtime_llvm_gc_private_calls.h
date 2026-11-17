/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

#pragma once
#include <uwvm2/utils/container/standard.h>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/MDBuilder.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/Transforms/Utils/CodeExtractor.h>

namespace uwvm2::runtime::lib::details
{
    struct runtime_llvm_jit_gc_private_clone
    {
        ::std::size_t function_index{};
        ::uwvm2::utils::container::std_string name{};
        ::std::uintptr_t loaded_address{};
        bool wrapper_entry{};
    };

    // A compiler-created decision, still present before the optimization
    // pipeline. If pre-link optimization already removed it, do not guess.
    [[nodiscard]] inline ::llvm::StoreInst* runtime_llvm_jit_gc_private_decision(::llvm::Function& function) noexcept
    {
        ::llvm::StoreInst* result{};
        for(auto& block: function) for(auto& instruction: block)
        {
            if(instruction.getMetadata("uwvm.gc.private.decision.v1")==nullptr) { continue; }
            auto* store=::llvm::dyn_cast<::llvm::StoreInst>(&instruction);
            if(store==nullptr || result!=nullptr || store->isVolatile() || store->isAtomic() ||
               !store->getValueOperand()->getType()->isIntegerTy(1u)) { return nullptr; }
            auto* slot=::llvm::dyn_cast<::llvm::AllocaInst>(store->getPointerOperand());
            if(slot==nullptr || slot->getParent()!=&function.getEntryBlock() ||
               !slot->getAllocatedType()->isIntegerTy(1u)) { return nullptr; }
            result=store;
        }
        return result;
    }

    [[nodiscard]] inline ::llvm::ConstantInt* runtime_llvm_jit_gc_leaf_index(::llvm::Function& function) noexcept
    {
        auto const* proof=function.getMetadata("uwvm.gc.numeric.leaf.v1");
        if(function.isDeclaration() || proof==nullptr || proof->getNumOperands()!=1u) { return nullptr; }
        auto* index=::llvm::mdconst::dyn_extract<::llvm::ConstantInt>(proof->getOperand(0u));
        if(index==nullptr || index->getBitWidth()!=64u ||
           index->getZExtValue()>(::std::numeric_limits<::std::size_t>::max)()) { return nullptr; }
        return index;
    }

    // Specialize edges, never the shared/public entry. Every reference in an
    // admitted caller originates in that invocation: no reference parameters,
    // globals/tables, imported references, extern conversions or callbacks.
    // Every validated direct target, including unreachable calls, must have a
    // complete numeric no-safepoint leaf proof. The leaf can neither publish
    // those objects nor invoke a collector/guest/host callback. Consequently
    // different invocations cannot alias this caller's mutable GC objects.
    //
    // Authentication, type/null/element checks, fallback and precise roots all
    // remain. Only object locks inside this privately reached clone disappear.
    // Full/debug/checkpoint/threaded/patchable distinctions are proved by the
    // emitter; missing evidence always keeps the ordinary implementation.
    [[nodiscard]] inline ::uwvm2::utils::container::std_vector<runtime_llvm_jit_gc_private_clone>
        specialize_runtime_llvm_jit_gc_private_calls(::llvm::Module& module)
    {
        ::uwvm2::utils::container::std_vector<runtime_llvm_jit_gc_private_clone> bindings{};
        ::llvm::SmallVector<::llvm::Function*, 16u> callers{};
        for(auto& function: module)
        { if(!function.isDeclaration() && function.getMetadata("uwvm.gc.closed.caller.v1")!=nullptr) { callers.push_back(&function); } }
        ::llvm::DenseMap<::llvm::Function*, ::llvm::Function*> clones{};
        for(auto* caller: callers)
        {
            auto const* proof=caller->getMetadata("uwvm.gc.closed.caller.v1");
            ::llvm::SmallPtrSet<::llvm::Function*, 8u> targets{};
            bool complete=true;
            for(auto const& operand: proof->operands())
            {
                auto const* name=::llvm::dyn_cast<::llvm::MDString>(operand.get());
                auto* target=name==nullptr?nullptr:module.getFunction(name->getString());
                if(target==nullptr || target==caller || runtime_llvm_jit_gc_leaf_index(*target)==nullptr)
                { complete=false; break; }
                targets.insert(target);
            }
            if(!complete || targets.empty()) { continue; }
            ::llvm::SmallVector<::llvm::CallBase*, 8u> edges{};
            for(auto& block: *caller) for(auto& instruction: block)
            {
                if(instruction.getMetadata("uwvm.gc.direct.call.v1")==nullptr) { continue; }
                auto* call=::llvm::dyn_cast<::llvm::CallBase>(&instruction);
                auto* target=call==nullptr?nullptr:call->getCalledFunction();
                if(target==nullptr || !targets.contains(target) ||
                   (::llvm::isa<::llvm::CallInst>(call) && ::llvm::cast<::llvm::CallInst>(call)->isMustTailCall()) ||
                   call->getFunctionType()!=target->getFunctionType() ||
                   call->getCallingConv()!=target->getCallingConv())
                { complete=false; break; }
                edges.push_back(call);
            }
            if(!complete || edges.empty()) { continue; }
            bool specialized{};
            ::llvm::SmallVector<::llvm::CallBase*, 8u> selected_edges{};
            for(auto* edge: edges)
            {
                auto* target=edge->getCalledFunction();
                auto* decision=runtime_llvm_jit_gc_private_decision(*target);
                if(decision==nullptr) { continue; }
                auto*& clone=clones[target];
                if(clone==nullptr)
                {
                    ::uwvm2::utils::container::std_string name{target->getName().data(),target->getName().size()};
                    name += ".gc.private.v1";
                    if(module.getNamedValue(::llvm::StringRef{name.data(),name.size()})!=nullptr) { continue; }
                    ::llvm::ValueToValueMapTy values{};
                    clone=::llvm::CloneFunction(target,values);
                    if(clone==nullptr) { continue; }
                    auto* private_decision=::llvm::dyn_cast_or_null<::llvm::StoreInst>(values.lookup(decision));
                    if(clone->getParent()!=&module || private_decision==nullptr ||
                       clone->getFunctionType()!=target->getFunctionType() ||
                       clone->getCallingConv()!=target->getCallingConv())
                    { clone->eraseFromParent(); clone=nullptr; continue; }
                    clone->setName(::llvm::StringRef{name.data(),name.size()});
                    // Retain the exact parameter/result types and a standalone
                    // native extent/FDE. External hidden linkage prevents dead-
                    // argument elimination; public/raw dispatch uses target.
                    clone->setLinkage(::llvm::GlobalValue::ExternalLinkage);
                    clone->setVisibility(::llvm::GlobalValue::HiddenVisibility);
                    clone->setDSOLocal(true);
                    // This entry is reached only by the selected ordinary
                    // direct edges below, never by raw/public dispatch or a
                    // return_call. A private fast convention avoids tailcc's
                    // callee-pop argument area on these non-tail leaf calls.
                    // The function type and all parameter/result attributes
                    // are unchanged; change both sides of this private ABI.
                    clone->setCallingConv(::llvm::CallingConv::Fast);
                    clone->addFnAttr(::llvm::Attribute::NoInline);
                    clone->addFnAttr(::llvm::Attribute::NoMerge);
                    clone->addFnAttr("nooutline");
                    private_decision->setOperand(0u,::llvm::ConstantInt::getTrue(module.getContext()));
                    auto const index=runtime_llvm_jit_gc_leaf_index(*target)->getZExtValue();
                    bindings.push_back({static_cast<::std::size_t>(index),name,0u});
                }
                edge->setCalledFunction(clone);
                edge->setCallingConv(clone->getCallingConv());
                selected_edges.push_back(edge);
                specialized=true;
            }
            if(specialized)
            {
                // The exact leaf proof already permits removal of the leaf's
                // root frame: no collector, root visitor, callback or debugger
                // can observe a caller frame inside this call. State only that
                // separation, not "readnone"/"nosync": object/registry accesses
                // and cold fallback locks remain ordinary memory operations.
                // LLVM may coalesce invariant root stores across these calls;
                // every allocation/unknown call remains an aliasing boundary.
                ::llvm::MDBuilder metadata{module.getContext()};
                auto* domain=metadata.createAnonymousAliasScopeDomain("uwvm.gc.caller.roots");
                auto* scope=metadata.createAnonymousAliasScope(domain,"precise native frame");
                auto* scopes=::llvm::MDNode::get(module.getContext(),scope);
                for(auto& block: *caller) for(auto& instruction: block)
                {
                    if(instruction.getMetadata("uwvm.gc.root.store.v1")!=nullptr &&
                       ::llvm::isa<::llvm::StoreInst>(instruction))
                    { instruction.setMetadata(::llvm::LLVMContext::MD_alias_scope,scopes); }
                }
                for(auto* edge: selected_edges)
                { edge->setMetadata(::llvm::LLVMContext::MD_noalias,scopes); }
                // The same complete provenance proof covers the caller's own
                // accesses; its allocation and call root snapshots are kept.
                if(auto* decision=runtime_llvm_jit_gc_private_decision(*caller))
                { decision->setOperand(0u,::llvm::ConstantInt::getTrue(module.getContext())); }
            }
        }
        return bindings;
    }
    // Keep cold checked operations from forcing their register-save set onto
    // every successful numeric leaf call. This runs AFTER the ordinary IR
    // pipeline, and only on the exact private clones created above. No guard,
    // memory effect or failure path is removed or moved across another effect.
    [[nodiscard]] inline bool outline_runtime_llvm_jit_gc_private_fallbacks(
        ::llvm::Module& module, ::uwvm2::utils::container::std_vector<runtime_llvm_jit_gc_private_clone>& bindings)
    {
        auto const original_count=bindings.size();
        for(::std::size_t index{}; index!=original_count; ++index)
        {
            auto const binding=bindings[index];
            auto* function=module.getFunction(::llvm::StringRef{binding.name.data(),binding.name.size()});
            if(function==nullptr || function->isDeclaration()) { return false; }
            ::llvm::SmallVector<::llvm::WeakTrackingVH,8u> candidates{};
            for(auto& block:*function)
            {
                if(block.getName().starts_with("gc.local.checked")) { candidates.push_back(&block); }
            }
            for(auto const& candidate:candidates)
            {
                auto* entry=::llvm::dyn_cast_or_null<::llvm::BasicBlock>(candidate);
                if(entry==nullptr || entry->getParent()!=function || entry==&function->getEntryBlock()) { continue; }
                ::llvm::DominatorTree dominators{*function};
                if(!dominators.isReachableFromEntry(entry)) { continue; }
                ::llvm::SmallVector<::llvm::BasicBlock*,16u> region{entry};
                for(auto& block:*function)
                {
                    if(&block!=entry && dominators.dominates(entry,&block)) { region.push_back(&block); }
                }
                bool eligible=true;
                ::llvm::SmallPtrSet<::llvm::BasicBlock*,8u> exits{};
                bool has_call{};
                for(auto* block:region)
                {
                    if(block->isEHPad()) { eligible=false; break; }
                    for(auto& instruction:*block)
                    {
                        // Do not introduce a caller stack object, move an EH
                        // edge, or alter a guaranteed tail-call activation.
                        if(::llvm::isa<::llvm::AllocaInst>(instruction) ||
                           ::llvm::isa<::llvm::InvokeInst>(instruction) ||
                           ::llvm::isa<::llvm::CallBrInst>(instruction) ||
                           ::llvm::isa<::llvm::ReturnInst>(instruction)) { eligible=false; break; }
                        if(auto* call=::llvm::dyn_cast<::llvm::CallInst>(&instruction))
                        {
                            if(call->isMustTailCall()) { eligible=false; break; }
                            has_call=true;
                        }
                    }
                    for(auto* successor : ::llvm::successors(block))
                    { if(!dominators.dominates(entry,successor)) { exits.insert(successor); } }
                }
                if(!eligible || !has_call || exits.size()!=1u) { continue; }
                ::llvm::CodeExtractor extractor{region,&dominators,false,nullptr,nullptr,nullptr,false,false,nullptr};
                if(!extractor.isEligible()) { continue; }
                ::llvm::SetVector<::llvm::Value*> inputs{},outputs{},allocas{};
                extractor.findInputsOutputs(inputs,outputs,allocas);
                // Returning values through newly allocated output slots can
                // penalize the fast path. Leave such regions unchanged.
                if(!outputs.empty()) { continue; }
                ::llvm::CodeExtractorAnalysisCache analysis{*function};
                auto* helper=extractor.extractCodeRegion(analysis);
                if(helper==nullptr) { continue; }
                helper->setName(function->getName()+".gc.checked");
                helper->setLinkage(::llvm::GlobalValue::ExternalLinkage);
                helper->setVisibility(::llvm::GlobalValue::HiddenVisibility);
                helper->setDSOLocal(true);
                helper->addFnAttr(::llvm::Attribute::NoInline);
                helper->addFnAttr(::llvm::Attribute::NoMerge);
                helper->addFnAttr(::llvm::Attribute::Cold);
                helper->addFnAttr("nooutline");
                helper->setUWTableKind(function->getUWTableKind());
                // Keep the original Wasm activation visible to the unwinder.
                // The helper is a compiler wrapper, never a second Wasm call.
                for(auto* user:helper->users())
                {
                    auto* call=::llvm::dyn_cast<::llvm::CallInst>(user);
                    if(call==nullptr || call->getFunction()!=function) { return false; }
                    call->setTailCallKind(::llvm::CallInst::TCK_NoTail);
                }
                bindings.push_back({binding.function_index,::uwvm2::utils::container::copy_string(helper->getName()),0u,true});
                if(::llvm::verifyFunction(*function) || ::llvm::verifyFunction(*helper)) { return false; }
            }
        }
        return true;
    }
}
