// Private ThreadedLlvmChild members, after fixed_call_target.
        using native_poll_witnesses=::llvm::DenseMap<::llvm::LoadInst const*,::llvm::CallInst const*>;

        // A fast success may bypass only the original zero-result edge. No
        // guest effect, instruction using the result, or intermediate computation
        // may sit between the checked call and its branch.
        [[nodiscard]] static ::llvm::BasicBlock* poll_continuation(::llvm::CallInst& call) noexcept
        {
            if(!call.hasOneUse())return nullptr;
            auto* next=call.getNextNode();
            if(auto* sw=::llvm::dyn_cast_or_null<::llvm::SwitchInst>(next))
            {
                if(sw->getCondition()!=&call||sw->getNumCases()!=1u)return nullptr;
                auto item=sw->case_begin();
                return item->getCaseValue()->isZero()&&item->getCaseSuccessor()!=sw->getDefaultDest() ?
                    item->getCaseSuccessor() : nullptr;
            }
            auto* cmp=::llvm::dyn_cast_or_null<::llvm::ICmpInst>(next);
            if(!cmp||!cmp->isEquality()||!cmp->hasOneUse())return nullptr;
            auto* zero=::llvm::dyn_cast<::llvm::ConstantInt>(cmp->getOperand(1u));
            auto* branch=::llvm::dyn_cast_or_null<::llvm::CondBrInst>(cmp->getNextNode());
            if(cmp->getOperand(0u)!=&call||!zero||!zero->isZero()||!branch||
               branch->getCondition()!=cmp||branch->getSuccessor(0u)==branch->getSuccessor(1u))return nullptr;
            return branch->getSuccessor(cmp->getPredicate()==::llvm::CmpInst::ICMP_EQ?0u:1u);
        }

        void inline_cancellation_loops(::llvm::Module& module,
            ::uwvm2::runtime::wasm_threads::threaded_llvm_module_limits room,native_poll_witnesses& witnesses,bool& attempted)
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            auto const& admission=world_->admission();
            if(!::std::atomic_ref<unsigned char>::is_always_lock_free||CHAR_BIT!=8||
               admission.debug_observation()!=::uwvm2::runtime::wasm_threads::threaded_debug_observation::none||
               admission.instruction_control())return;
            auto& context=module.getContext();
            ::llvm::IRBuilder<> types(context);
            auto* ptr=types.getPtrTy();
            auto* byte=types.getInt8Ty();
            auto* word=types.getIntNTy(sizeof(::std::uintptr_t)*CHAR_BIT);
            auto* ordinary=module.getFunction(poll_name);
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
            auto* ros=module.getFunction(ros_poll_name);
#endif
            char const state_name[]{"uwvm.threaded.poll.state.v1"};
            if(module.getNamedValue(state_name)||externals_.size()>=256u)return;
            ::llvm::Function* state_fn{};
            auto* likely=::llvm::MDBuilder{context}.createBranchWeights(2000u,1u);
            struct site { ::llvm::CallInst* call; ::llvm::BasicBlock* continued; };
            for(auto& function:module)
            {
                if(function.isDeclaration())continue;
                ::llvm::DominatorTree dominators{function};
                ::llvm::LoopInfo loops{dominators};
                ::llvm::SmallVector<site,16> sites;
                for(auto& block:function)
                {
                    if(!loops.getLoopFor(&block))continue;
                    for(auto& inst:block)
                    {
                        auto* call=::llvm::dyn_cast<::llvm::CallInst>(&inst);
                        if(!call)continue;
                        auto* target=fixed_call_target(*call);
                        if(!target||(target!=ordinary
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                           &&target!=ros
#endif
                        ))continue;
                        if(auto* continued=poll_continuation(*call))sites.push_back({call,continued});
                    }
                }
                if(sites.empty())continue;
                // One context call, GEP and pointer load per native activation.
                // Each site below adds two blocks and at most 32 instructions.
                if(room.instructions<35u||room.blocks<2u)return;
                attempted=true;
                if(!state_fn)
                {
                    state_fn=::llvm::Function::Create(::llvm::FunctionType::get(ptr,false),
                        ::llvm::Function::ExternalLinkage,state_name,module);
                    state_fn->addFnAttr(::llvm::Attribute::NoUnwind);
                    emit::apply_llvm_jit_host_calling_conv(*state_fn);
                    if(!add_external(*state_fn,address(&current_poll_state))){state_fn->eraseFromParent();return;}
                }
                auto& entry=function.getEntryBlock();
                ::llvm::IRBuilder<> start(&entry,entry.getFirstInsertionPt());
                auto* state=emit::apply_llvm_jit_calling_conv(start.CreateCall(state_fn,{}),emit::get_llvm_jit_host_calling_conv());
                auto* stop_byte=start.CreateLoad(ptr,start.CreateConstGEP1_64(byte,state,offsetof(native_poll_state,stop_byte)));
                room.instructions-=3u;
                for(auto const& [call,continued]:sites)
                {
                    if(room.instructions<32u||room.blocks<2u)return;
                    auto* block=call->getParent();
                    auto* slow=block->splitBasicBlock(call->getIterator(),"threaded.poll.slow");
                    auto* fast=::llvm::BasicBlock::Create(context,"threaded.poll.acquire",&function,slow);
                    block->getTerminator()->eraseFromParent();
                    ::llvm::IRBuilder<> b(block);
                    auto field=[&](::llvm::Type* type,::std::size_t offset)
                    {
                        auto* load=b.CreateLoad(type,b.CreateConstGEP1_64(byte,state,offset));
                        load->setVolatile(true);
                        return load;
                    };
                    // Only elide a publication of the identical function. A
                    // changed PC, rejection or provenance miss stays in the
                    // original host poll, including its fault-before-PC order.
                    auto* ready=b.CreateAnd(b.CreateIsNull(field(byte,offsetof(native_poll_state,rejected))),
                        b.CreateICmpEQ(field(word,offsetof(native_poll_state,function)),call->getArgOperand(0u)));
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                    if(call->arg_size()==2u)
                    {
                        auto* owner=b.CreateAnd(call->getArgOperand(1u),::llvm::ConstantInt::get(word,~::std::uintptr_t{1u}));
                        auto* valid_owner=b.CreateAnd(b.CreateIsNotNull(owner),
                            b.CreateICmpEQ(owner,field(word,offsetof(native_poll_state,authenticated_ros_owner))));
                        auto* same_child=b.CreateICmpEQ(field(ptr,offsetof(native_poll_state,child)),
                            field(ptr,offsetof(native_poll_state,authenticated_ros_child)));
                        ready=b.CreateAnd(ready,b.CreateAnd(valid_owner,same_child));
                    }
#endif
                    b.CreateCondBr(ready,fast,slow,likely);
                    b.SetInsertPoint(fast);
                    auto* requested=b.CreateAlignedLoad(byte,stop_byte,::llvm::Align{1u});
                    requested->setAtomic(::llvm::AtomicOrdering::Acquire);
                    requested->setVolatile(true);
                    b.CreateCondBr(b.CreateIsNull(requested),continued,slow,likely);
                    // splitBasicBlock already transferred the former incoming
                    // edge to slow. The success edge carries the same values.
                    for(auto& phi:continued->phis())phi.addIncoming(phi.getIncomingValueForBlock(slow),fast);
                    witnesses.try_emplace(requested,call);
                    room.instructions-=32u;
                    room.blocks-=2u;
                }
            }
        }
