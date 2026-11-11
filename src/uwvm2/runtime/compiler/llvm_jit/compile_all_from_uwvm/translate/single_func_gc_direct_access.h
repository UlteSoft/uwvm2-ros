// Authenticate an ordinary local GC object in generated code. A bucket miss,
// collision, foreign owner, subtype or contended lock keeps the complete checked
// scalar operation. The optional local witness is compiler-private and never
// a Wasm value: it is revoked before control-flow changes, calls, allocation,
// or safepoints. Local writes preserve only equality of the original SSA values,
// never equality of local slots. Debug/checkpoint/threaded profiles do not use it.
inline void llvm_jit_gc_local_index_bridge(::std::uintptr_t module_address,
    ::std::uintptr_t output_address) noexcept
{
    namespace storage=::uwvm2::uwvm::runtime::storage;
    ::std::uintptr_t words[2]{};
    if(module_address!=0u)
    {
        auto const* module=reinterpret_cast<storage::wasm_module_storage_t const*>(module_address);
        if(auto const* store=module->gc_store.get();store!=nullptr) { store->jit_local_index_domain(words); }
    }
    if(output_address!=0u) { ::std::memcpy(reinterpret_cast<void*>(output_address),words,sizeof(words)); }
}

class llvm_jit_gc_direct_access_plan
{
    ::llvm::IRBuilder<>* builder_{};
    ::llvm::BasicBlock* done_{};
    ::llvm::BasicBlock* fast_end_{};
    ::llvm::Value* fast_value_{};
    runtime_local_func_llvm_jit_emit_state_t* witness_state_{};
    runtime_local_func_llvm_jit_emit_state_t::gc_local_access_witness witness_{};
    ::llvm::BasicBlock* checked_fallback_{};
    bool prefer_local_layout_{};
    void mark(::llvm::Instruction* terminal) const
    {
        auto& context=terminal->getContext();
        terminal->setMetadata("uwvm.gc.scalar.local.region",::llvm::MDNode::get(context,{}));
        auto* const branch=::llvm::dyn_cast<::llvm::BranchInst>(terminal);
        if(prefer_local_layout_ && branch!=nullptr && branch->isConditional() &&
           branch->getSuccessor(1u)==checked_fallback_)
        {
            // Layout information only: every failed authentication, bound or
            // try-lock still reaches the same complete checked operation.
            // Do not bias the private/shared choice or a debug control edge.
            auto* const integer=::llvm::Type::getInt32Ty(context);
            branch->setMetadata(::llvm::LLVMContext::MD_prof,::llvm::MDNode::get(context,{
                ::llvm::MDString::get(context,"branch_weights"),
                ::llvm::ConstantAsMetadata::get(::llvm::ConstantInt::get(integer,2000u)),
                ::llvm::ConstantAsMetadata::get(::llvm::ConstantInt::get(integer,1u))}));
        }
    }

    // Prove the actually emitted path from an earlier release to this insertion
    // point. Only unconditional forward edges and side-effect-free SSA work
    // are admitted. In particular a potentially trapping numeric operation
    // has a conditional exit and fails this proof; even a pure-looking call
    // is refused. A compiler witness alone is not permission to retain a lock.
    [[nodiscard]] static bool can_join_locked_regions(::llvm::StoreInst* release,
        ::llvm::IRBuilder<> const& builder) noexcept
    {
        if(release==nullptr || release->getOrdering()!=::llvm::AtomicOrdering::Release ||
           release->getParent()==nullptr || builder.GetInsertBlock()==nullptr ||
           release->getFunction()!=builder.GetInsertBlock()->getParent()) { return false; }
        auto* block=release->getParent();
        auto iterator=release->getIterator();
        ++iterator;
        ::llvm::SmallPtrSet<::llvm::BasicBlock*,8u> visited{};
        for(;;)
        {
            if(!visited.insert(block).second) { return false; }
            for(;iterator!=block->end();++iterator)
            {
                if(block==builder.GetInsertBlock() && iterator==builder.GetInsertPoint()) { return true; }
                auto& instruction=*iterator;
                if(auto* branch=::llvm::dyn_cast<::llvm::BranchInst>(::std::addressof(instruction)))
                {
                    if(!branch->isUnconditional()) { return false; }
                    break;
                }
                if(instruction.isTerminator() || ::llvm::isa<::llvm::CallBase>(instruction) ||
                   instruction.mayReadOrWriteMemory() || instruction.mayHaveSideEffects()) { return false; }
            }
            if(iterator==block->end())
            {
                return block==builder.GetInsertBlock() && iterator==builder.GetInsertPoint();
            }
            block=::llvm::cast<::llvm::BranchInst>(*iterator).getSuccessor(0u);
            iterator=block->begin();
        }
    }
public:
    // This compiler-private region is acyclic and has no call/return/GC poll.
    // Threaded control checks both properties before keeping it indivisible at
    // Wasm instruction granularity. A failed try-lock branches without holding
    // a lock into the ordinary checked helper, which preserves cancellation.
    [[nodiscard]] bool prepare(runtime_local_func_llvm_jit_emit_state_t& state,
        ::uwvm2::uwvm::runtime::storage::wasm_module_storage_t const& module,
        ::llvm::Value* kind,::llvm::Value* token,::llvm::Value* index,
        ::llvm::Value* input,::std::uint32_t expected_type,unsigned width,
        bool array,bool length_only,bool mutable_,bool signed_,::llvm::Value* reference_identity) noexcept
    {
        namespace storage=::uwvm2::uwvm::runtime::storage;
        namespace types=::uwvm2::parser::wasm::standard::wasm3::type;
        auto& b=*state.ir_builder;
        auto* const function=state.llvm_function;
        if(function==nullptr||width==0u||width>16u||(!length_only&&index==nullptr)) { return false; }
        auto* const word=b.getIntNTy(sizeof(::std::uintptr_t)*CHAR_BIT);
        auto* const ptr=b.getPtrTy();
        if(state.gc_local_index_owner==nullptr)
        {
            auto& entry=function->getEntryBlock();
            ::llvm::IRBuilder<> eb(&entry,entry.getFirstInsertionPt());
            state.gc_private_access_slot=eb.CreateAlloca(eb.getInt1Ty(),nullptr,"gc.private.access");
            state.gc_private_access_decision=eb.CreateStore(eb.getFalse(),state.gc_private_access_slot);

            // Ordinary compiled code already retains its genuine module and
            // that module's store. These two domain identities are immutable
            // after store construction; collection only changes atomic bucket
            // HEADS. Bind the actual allocations with relocations, never an
            // individual object, guest token, or serialized process address.
            // Cache hits rebuild these bindings for the current module.
            ::std::uintptr_t domain[2]{};
            if(state.gc_local_value_keys_enabled() && !state.route_wasm_calls_through_runtime_bridge &&
               state.debug_full_patchable_typed_target_base_address==0u)
            {
                if(auto const* store=module.gc_store.get();store!=nullptr)
                { store->jit_local_index_domain(domain); }
            }
            if(domain[0]!=0u && domain[1]!=0u)
            {
                auto const prefix=get_llvm_runtime_module_symbol_prefix(module);
                auto const owner_name=::uwvm2::utils::container::u8concat_uwvm(prefix,u8"_gc_local_owner_v2");
                auto const buckets_name=::uwvm2::utils::container::u8concat_uwvm(prefix,u8"_gc_local_buckets_v2");
                auto* const owner=get_llvm_external_host_object_pointer(eb,domain[0],eb.getInt8Ty(),
                    ::uwvm2::utils::container::u8string_view{owner_name.data(),owner_name.size()});
                auto* const bucket_type=::llvm::ArrayType::get(ptr,storage::gc_object_store::jit_local_bucket_mask+1u);
                auto* const buckets=get_llvm_external_host_object_pointer(eb,domain[1],bucket_type,
                    ::uwvm2::utils::container::u8string_view{buckets_name.data(),buckets_name.size()});
                if(owner==nullptr || buckets==nullptr) { return false; }
                state.gc_local_index_owner=eb.CreatePtrToInt(owner,word,"gc.local.owner");
                state.gc_local_index_buckets=eb.CreatePtrToInt(buckets,word,"gc.local.buckets");
            }
            else
            {
                // Debug, checkpoint, threaded, lazy/bridge and unavailable
                // domain profiles keep the original checked runtime binding.
                auto* const slots=eb.CreateAlloca(word,eb.getInt32(2u),"gc.local.domain.slots");
                slots->setAlignment(::llvm::Align(alignof(::std::uintptr_t)));
                auto const name=get_llvm_runtime_module_object_symbol_name(module);
                auto* const address=get_llvm_external_host_object_address(eb,
                    reinterpret_cast<::std::uintptr_t>(::std::addressof(module)),
                    ::uwvm2::utils::container::u8string_view{name.data(),name.size()});
                auto* const abi=::llvm::FunctionType::get(eb.getVoidTy(),{word,word},false);
                // Exact leaf: copies immutable owner/index identities, cannot
                // call back, allocate a GC object or collect.
                auto* const callee=get_llvm_runtime_bridge_function_symbol_value_unwrapped<llvm_jit_gc_local_index_bridge>(
                    eb,abi,::uwvm2::utils::container::u8string_view{u8"gc_local_index_domain_v1"});
                if(address==nullptr||callee==nullptr) { return false; }
                auto* const call=eb.CreateCall(abi,callee,{address,eb.CreatePtrToInt(slots,word)});
                apply_llvm_jit_calling_conv(call,get_llvm_jit_host_calling_conv());
                call->setDoesNotThrow();
                state.gc_local_index_owner=eb.CreateLoad(word,slots,"gc.local.owner");
                state.gc_local_index_buckets=eb.CreateLoad(word,eb.CreateGEP(word,slots,eb.getInt32(1u)),"gc.local.buckets");
            }
        }
        auto const block=[&](char const* name)
        { return ::llvm::BasicBlock::Create(b.getContext(),name,function); };
        bool const witnesses=state.gc_local_value_keys_enabled();
        auto const previous=state.gc_local_access;
        auto* const reference_key=state.gc_local_value_key(reference_identity);
        auto* const index_key=state.gc_local_value_key(index);
        bool const reuse=witnesses && previous.header!=nullptr &&
            reference_key!=nullptr && previous.reference==reference_key &&
            previous.array==array && (length_only || previous.type==expected_type);
        bool const reuse_element=reuse && !length_only && previous.element!=nullptr &&
            previous.index==index_key && previous.width==width;
        bool const join_lock=reuse_element && mutable_ && can_join_locked_regions(previous.release,b);
        if(witnesses && !length_only)
        {
            witness_state_=::std::addressof(state);
            witness_={reference_key,index_key,nullptr,nullptr,expected_type,width,array};
        }
        auto* const guard=block("gc.local.guard");
        auto* const bucket=reuse?nullptr:block("gc.local.bucket");
        auto* const header=reuse?nullptr:block("gc.local.header");
        auto* const geometry=block("gc.local.geometry");
        auto* const access=block("gc.local.access");
        auto* const slow=block("gc.local.checked");
        checked_fallback_=slow;
        prefer_local_layout_=witnesses && !state.route_wasm_calls_through_runtime_bridge &&
            state.debug_full_patchable_typed_target_base_address==0u;
        done_=block("gc.local.done");
        b.CreateBr(guard);b.SetInsertPoint(guard);
        auto const nonzero=[&](::llvm::Value* v)
        { return b.CreateICmpNE(v,::llvm::Constant::getNullValue(v->getType())); };
        auto* const owner=state.gc_local_index_owner;
        auto* const buckets=state.gc_local_index_buckets;
        auto const ref_kind=array?::uwvm2::object::global::wasm_ref_kind::wasm_array:
            ::uwvm2::object::global::wasm_ref_kind::wasm_struct;
        ::llvm::Value* head{};
        auto const member=[&](::std::size_t offset)
        { return b.CreateGEP(b.getInt8Ty(),head,::llvm::ConstantInt::get(word,offset)); };
        if(reuse)
        {
            head=previous.header;
            mark(b.CreateCondBr(nonzero(head),geometry,slow));
            b.SetInsertPoint(geometry);
        }
        else
        {
        auto* const valid=b.CreateAnd(b.CreateAnd(nonzero(owner),nonzero(buckets)),
            b.CreateAnd(nonzero(token),b.CreateICmpEQ(kind,b.getInt32(static_cast<unsigned>(ref_kind)))));
        mark(b.CreateCondBr(valid,bucket,slow));b.SetInsertPoint(bucket);
        auto* const mixed=b.CreateXor(token,b.CreateLShr(token,17u));
        auto* const hash=b.CreateAnd(b.CreateXor(mixed,b.CreateLShr(mixed,9u)),
            ::llvm::ConstantInt::get(word,storage::gc_object_store::jit_local_bucket_mask));
        head=b.CreateAlignedLoad(ptr,b.CreateGEP(ptr,b.CreateIntToPtr(buckets,ptr),hash),::llvm::Align(sizeof(void*)));
        ::llvm::cast<::llvm::LoadInst>(head)->setAtomic(::llvm::AtomicOrdering::Acquire);
        mark(b.CreateCondBr(nonzero(head),header,slow));b.SetInsertPoint(header);
        auto* const actual_token=b.CreateLoad(word,member(storage::gc_object_store::jit_local_token_offset));
        auto* const actual_owner=b.CreateLoad(word,member(storage::gc_object_store::jit_local_owner_offset));
        auto* const actual_kind=b.CreateLoad(b.getInt32Ty(),member(storage::gc_object_store::jit_local_kind_offset));
        ::llvm::Value* exact=b.CreateAnd(b.CreateICmpEQ(actual_token,token),b.CreateAnd(
            b.CreateICmpEQ(actual_owner,owner),b.CreateICmpEQ(actual_kind,b.getInt32(static_cast<unsigned>(
                array?types::composite_kind::array:types::composite_kind::struct_)))));
        if(!length_only)
        {
            auto* const actual_type=b.CreateLoad(b.getInt32Ty(),member(storage::gc_object_store::jit_local_type_offset));
            exact=b.CreateAnd(exact,b.CreateICmpEQ(actual_type,b.getInt32(expected_type)));
        }
        mark(b.CreateCondBr(exact,geometry,slow));b.SetInsertPoint(geometry);
        }
        witness_.header=head;
        auto* const length=reuse_element?nullptr:b.CreateLoad(word,member(storage::gc_object_store::jit_local_length_offset));
        if(length_only)
        {
            mark(b.CreateCondBr(b.CreateICmpULE(length,::llvm::ConstantInt::get(word,UINT32_MAX)),access,slow));
            b.SetInsertPoint(access);fast_value_=b.CreateZExtOrTrunc(length,b.getInt32Ty());
        }
        else
        {
            auto* const wide_index=b.CreateZExtOrTrunc(index,word);
            if(reuse_element) { mark(b.CreateBr(access)); }
            else { mark(b.CreateCondBr(b.CreateICmpULT(wide_index,length),access,slow)); }
            b.SetInsertPoint(access);
            auto* const flag=member(storage::gc_object_store::jit_local_lock_offset);
            ::llvm::Value* private_access{};
            if(mutable_ && join_lock)
            {
                // A nonnull reused witness comes only from the earlier fast
                // edge. That edge acquired THIS object's lock (or proved it
                // private). Same element/type/width means no new bounds edge,
                // and the IR path proof excluded every intervening exit/call.
                private_access=b.CreateLoad(b.getInt1Ty(),state.gc_private_access_slot);
            }
            else if(mutable_)
            {
                auto* const locking=block("gc.local.lock");
                auto* const locked=block("gc.local.locked");
                private_access=b.CreateLoad(b.getInt1Ty(),state.gc_private_access_slot);
                mark(b.CreateCondBr(private_access,locked,locking));b.SetInsertPoint(locking);
                auto* const acquired=b.CreateAtomicCmpXchg(flag,b.getInt8(0u),b.getInt8(1u),::llvm::Align(1u),
                    ::llvm::AtomicOrdering::Acquire,::llvm::AtomicOrdering::Monotonic);
                acquired->setWeak(true);
                mark(b.CreateCondBr(b.CreateExtractValue(acquired,1u),locked,slow));b.SetInsertPoint(locked);
            }
            auto* const data=reuse_element?nullptr:b.CreateLoad(ptr,member(storage::gc_object_store::jit_local_values_offset));
            unsigned stride=sizeof(storage::gc_object_value);
            if(array) { stride=width; }
            auto* const element=reuse_element?previous.element:
                b.CreateGEP(b.getInt8Ty(),data,b.CreateMul(wide_index,::llvm::ConstantInt::get(word,stride)));
            witness_.element=element;
            // Every genuine object tail starts at object_value_offset, aligned
            // for gc_object_value in ordinary, slab and staged allocations.
            // Packed elements retain only the alignment shared with the stride.
            // This describes the authenticated allocation; it is not a guest
            // alignment promise and never makes an unchecked address usable.
            auto const alignment=::llvm::commonAlignment(::llvm::Align(alignof(storage::gc_object_value)),stride);
            if(input!=nullptr)
            {
                // Canonical numeric carriers zero the unused bytes just like
                // gc_object_value::i32/f32. Packed arrays own only width bytes.
                auto* const bits=b.CreateTruncOrBitCast(input,b.getIntNTy(width*CHAR_BIT));
                auto* const value=b.CreateZExtOrTrunc(bits,b.getIntNTy(stride*CHAR_BIT));
                b.CreateAlignedStore(value,element,alignment);
            }
            else
            {
                auto* const bits=b.CreateAlignedLoad(b.getIntNTy(width*CHAR_BIT),element,alignment);
                fast_value_=width<4u?(signed_?b.CreateSExt(bits,b.getInt32Ty()):b.CreateZExt(bits,b.getInt32Ty())):static_cast<::llvm::Value*>(bits);
            }
            if(mutable_)
            {
                auto* const unlocked=block("gc.local.unlock");
                auto* const released=block("gc.local.released");
                mark(b.CreateCondBr(private_access,released,unlocked));b.SetInsertPoint(unlocked);
                auto* const release=b.CreateAlignedStore(b.getInt8(0u),flag,::llvm::Align(1u));
                release->setAtomic(::llvm::AtomicOrdering::Release);
                witness_.release=release;
                if(join_lock)
                {
                    // Retire the first release only after the matching final
                    // release exists. The helper/failed-authentication edge has
                    // a null witness and NEVER owns the earlier lock.
                    previous.release->eraseFromParent();
                }
                mark(b.CreateBr(released));b.SetInsertPoint(released);
            }
        }
        fast_end_=b.GetInsertBlock();mark(b.CreateBr(done_));b.SetInsertPoint(slow);builder_=::std::addressof(b);
        return true;
    }
    [[nodiscard]] ::llvm::Value* finish(::llvm::Value* slow_value) noexcept
    {
        auto& b=*builder_;auto* const slow_end=b.GetInsertBlock();b.CreateBr(done_);b.SetInsertPoint(done_);
        if(witness_state_!=nullptr)
        {
            // Only the successful direct edge carries a borrowed address.
            // The complete helper edge carries null, never an address across
            // a native call. The next use must authenticate this witness key.
            auto const merge=[&](::llvm::Value* fast)
            {
                auto* value=b.CreatePHI(b.getPtrTy(),2u,"gc.local.witness");
                value->addIncoming(fast,fast_end_);
                value->addIncoming(::llvm::ConstantPointerNull::get(b.getPtrTy()),slow_end);
                return value;
            };
            witness_.header=merge(witness_.header);
            witness_.element=merge(witness_.element);
            witness_state_->gc_local_access=witness_;
            ++witness_state_->gc_local_access_sequence;
        }
        if(slow_value==nullptr) { return nullptr; }
        auto* const result=b.CreatePHI(slow_value->getType(),2u,"gc.local.result");
        result->addIncoming(fast_value_,fast_end_);result->addIncoming(slow_value,slow_end);return result;
    }
};
