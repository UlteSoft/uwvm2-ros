// The group pins this one software owner, its mmap reservation, tag directory
// and fair gate until all workers retire. The reader count belongs to the active
// worker and is never transferred to a different native or Wasm activation.
        using scalar_gate_type=::uwvm2::utils::mutex::rwlock_t;
        static void leave_scalar_region() noexcept
        {
            if(auto* e=active_; e && e->scalar_gate)
            {
                auto* gate=::std::exchange(e->scalar_gate,nullptr);
                gate->bits.fetch_sub(::uwvm2::utils::mutex::rwlock_reader_mask(),::std::memory_order_release);
            }
        }

        [[nodiscard]] static ::std::uintptr_t enter_scalar_region(native_poll_state const* state,
            ThreadedLlvmChild const* child,memory_type const* memory,memory_type* const* slot) noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            namespace locks=::uwvm2::utils::mutex;
            auto* e=active_;
            if(!e || e!=state || e->child!=child || stopped(*e) ||
               child->memories_.size()!=1u || slot!=&child->memories_[0] || *slot!=memory ||
               mt::access_checks_enabled!=child->module_->memory_policy.check_tags ||
               mt::logging_requested() || mt::asynchronous_requested() || mt::hardware_checked_access_requested())return 0u;
            auto* gate=memory->memtag_shadow.shared_tag_gate_;
            if(!gate || e->scalar_gate)return 0u;
            auto old=gate->bits.load(::std::memory_order_relaxed);
            if((old&(locks::rwlock_writer_mask()|locks::rwlock_pending_mask())) ||
               old>UINT_MAX-locks::rwlock_reader_mask() ||
               !gate->bits.compare_exchange_strong(old,old+locks::rwlock_reader_mask(),
                    ::std::memory_order_acquire,::std::memory_order_relaxed))return 0u;
            e->scalar_gate=gate;
            return 1u;
        }

        static ::std::uintptr_t scalar_region_poll(::std::size_t function) noexcept
        { leave_scalar_region();return cancellation_poll(function); }
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
        static ::std::uintptr_t scalar_region_ros_poll(::std::size_t function,::std::uintptr_t token) noexcept
        { leave_scalar_region();return ros_cancellation_poll(function,token); }
#endif
        struct scalar_region_ir
        {
            ::llvm::Value *state{},*child{},*owner{},*slot{},*gate{},*held{},*stop{},*base{},*length{},*tags{};
            ::llvm::Function *enter{},*leave{};
            unsigned tag_start{};
        };

        static ::llvm::CallInst* scalar_region_call(::llvm::IRBuilder<>& b,::llvm::Function* fn,
            ::llvm::ArrayRef<::llvm::Value*> args={})
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            return emit::apply_llvm_jit_calling_conv(b.CreateCall(fn,args),emit::get_llvm_jit_host_calling_conv());
        }

        static ::llvm::Value* scalar_writer_requested(::llvm::IRBuilder<>& b,::llvm::Value* gate)
        {
            namespace locks=::uwvm2::utils::mutex;
            static_assert(offsetof(scalar_gate_type,bits)==0);
            auto* bits=b.CreateAlignedLoad(b.getIntNTy(sizeof(unsigned)*CHAR_BIT),gate,::llvm::Align{alignof(::std::atomic_uint)},true);
            bits->setAtomic(::llvm::AtomicOrdering::Monotonic);
            return b.CreateIsNotNull(b.CreateAnd(bits,locks::rwlock_pending_mask()|locks::rwlock_writer_mask()));
        }

        static void emit_scalar_region_access(::llvm::CallInst& call,scalar_region_ir const& r,
            shared_scalar_layout layout,shared_scalar_shape shape)
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            auto* entry=call.getParent();
            auto* slow=entry->splitBasicBlock(call.getIterator(),"tagged.scalar.bridge");
            auto* done=slow->splitBasicBlock(call.getNextNode()->getIterator(),"tagged.scalar.done");
            auto block=[&](char const* name){return ::llvm::BasicBlock::Create(call.getContext(),name,call.getFunction(),slow);};
            auto* lease=block("tagged.scalar.lease");
            auto* claim=block("tagged.scalar.claim");
            auto* effect=block("tagged.scalar.effect");
            auto* tags=r.tags ? block("tagged.scalar.tags") : effect;
            auto* likely=::llvm::MDBuilder{call.getContext()}.createBranchWeights(2000u,1u);
            entry->getTerminator()->eraseFromParent();
            ::llvm::IRBuilder<> b(entry);
            auto* byte=b.getInt8Ty();
            auto* word=b.getIntNTy(sizeof(::std::uintptr_t)*CHAR_BIT);
            auto* raw=call.getArgOperand(2u);
            auto* offset=b.CreateAdd(b.CreateAnd(raw,b.getInt64(layout.mask)),call.getArgOperand(1u));
            auto* end=b.CreateAdd(offset,b.getInt64(shape.bytes));
            // The group cannot shrink this reservation. Every successful grow
            // prepares all shadow/data pages before publishing this length.
            auto* live=b.CreateAlignedLoad(word,r.length,::llvm::Align{alignof(::std::atomic_size_t)});
            live->setAtomic(::llvm::AtomicOrdering::Acquire);
            auto* stop=b.CreateAlignedLoad(byte,r.stop,::llvm::Align{1u},true);
            stop->setAtomic(::llvm::AtomicOrdering::Acquire);
            auto* ready=b.CreateAnd(b.CreateIsNull(stop),b.CreateICmpULE(end,b.CreateZExtOrTrunc(live,b.getInt64Ty())));
            ready=b.CreateAnd(ready,b.CreateNot(scalar_writer_requested(b,r.gate)));
            b.CreateCondBr(ready,lease,slow,likely);
            b.SetInsertPoint(lease);
            b.CreateCondBr(b.CreateICmpEQ(b.CreateLoad(b.getPtrTy(),r.held,true),r.gate),tags,claim,likely);
            b.SetInsertPoint(claim);
            auto* acquired=scalar_region_call(b,r.enter,{r.state,r.child,r.owner,r.slot});
            b.CreateCondBr(b.CreateIsNotNull(acquired),tags,slow,likely);
            if(r.tags)
            {
                b.SetInsertPoint(tags);
                auto* expected=b.CreateAnd(b.CreateTrunc(b.CreateLShr(raw,r.tag_start),byte),b.getInt8(15u));
                auto matches=[&](::llvm::Value* position)
                {
                    auto* index=b.CreateLShr(b.CreateZExtOrTrunc(position,word),4u);
                    auto* tag=b.CreateAlignedLoad(byte,b.CreateGEP(byte,r.tags,index),::llvm::Align{1u});
                    tag->setAtomic(::llvm::AtomicOrdering::Monotonic);
                    return b.CreateICmpEQ(tag,expected);
                };
                auto* match=matches(offset);
                if(shape.bytes>1u)match=b.CreateAnd(match,matches(b.CreateAdd(offset,b.getInt64(shape.bytes-1u))));
                b.CreateCondBr(match,effect,slow,likely);
            }
            b.SetInsertPoint(effect);
            auto* at=b.CreateGEP(byte,r.base,b.CreateZExtOrTrunc(offset,word));
            auto* bits=b.getIntNTy(shape.bytes*8u);
            ::llvm::Value* value=shape.store ? b.CreateTruncOrBitCast(call.getArgOperand(3u),bits) : nullptr;
            // The same admitted x86 MOV as ordinary shared Wasm only touches
            // the proved span. Qualification below excludes unsupported targets.
            if(shape.bytes==1u)
            {
                if(shape.store)b.CreateAlignedStore(value,at,::llvm::Align{1u},true)->setAtomic(::llvm::AtomicOrdering::Monotonic);
                else
                {
                    auto* load=b.CreateAlignedLoad(byte,at,::llvm::Align{1u},true);
                    load->setAtomic(::llvm::AtomicOrdering::Monotonic);value=load;
                }
            }
            else
            {
                auto* move=emit::emit_threaded_scalar_move(b,at,bits,value);
                if(!shape.store)value=move;
            }
            auto* result=shape.store ? static_cast<::llvm::Value*>(b.getInt64(0u)) :
                b.CreateZExtOrTrunc(b.CreateIntCast(value,b.getIntNTy(shape.result_bits),shape.sign),b.getInt64Ty());
            b.CreateBr(done);
            if(!call.use_empty())
            {
                b.SetInsertPoint(done,done->begin());
                auto* phi=b.CreatePHI(b.getInt64Ty(),2u);
                call.replaceAllUsesWith(phi);phi->addIncoming(&call,slow);phi->addIncoming(result,effect);
            }
        }

        [[nodiscard]] bool inline_scalar_regions(::llvm::Module& module,
            ::llvm::DenseMap<::llvm::Function*,shared_scalar_shape> const& declarations,
            ::uwvm2::runtime::wasm_threads::threaded_llvm_module_limits room,bool& attempted)
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            auto& context=module.getContext();
            ::llvm::IRBuilder<> types(context);
            auto* ptr=types.getPtrTy();
            auto* word=types.getIntNTy(sizeof(::std::uintptr_t)*CHAR_BIT);
            // No region may be emitted without the exact-width data primitive.
            if(!emit::threaded_scalar_move(types.getInt64Ty(),false) || memories_.size()!=1u ||
               externals_.size()+6u>=256u)return false;
            auto declare=[&](char const* name,::llvm::FunctionType* signature,::std::uintptr_t address)
            {
                if(module.getNamedValue(name))return static_cast<::llvm::Function*>(nullptr);
                auto* f=::llvm::Function::Create(signature,::llvm::Function::ExternalLinkage,name,module);
                f->addFnAttr(::llvm::Attribute::NoUnwind);
                emit::apply_llvm_jit_host_calling_conv(*f);
                return add_external(*f,address) ? f : nullptr;
            };
            scalar_region_ir r{};
            ::llvm::Function* state_fn{};
            auto* poll=module.getFunction(poll_name);
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
            auto* ros_poll=module.getFunction(ros_poll_name);
#endif
            auto is_poll=[&](::llvm::CallInst const& c)
            {
                auto* target=c.getCalledFunction();
                return target && (target==poll
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                    || target==ros_poll
#endif
                );
            };
            // These are the only real calls whose native success edge the
            // later cancellation pass can bypass. Release on the actual call,
            // before ROS authentication, failure reporting or cancellation.
            for(auto& binding:externals_)
            {
                if(binding.name==poll_name && binding.address==address(&cancellation_poll))binding.address=address(&scalar_region_poll);
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                if(binding.name==ros_poll_name && binding.address==address(&ros_cancellation_poll))binding.address=address(&scalar_region_ros_poll);
#endif
            }
            for(auto& function:module)
            {
                if(function.isDeclaration())continue;
                struct site { ::llvm::CallInst* call;shared_scalar_layout layout;shared_scalar_shape shape; };
                ::llvm::SmallVector<site,16> sites;
                ::llvm::SmallVector<::llvm::Instruction*,32> boundaries,checkpoints;
                ::llvm::DenseMap<::llvm::BasicBlock*,unsigned> order;
                unsigned rank{};
                for(auto& block:function)order.try_emplace(&block,rank++);
                for(auto& block:function)
                {
                    // Every cycle, including an irreducible cycle, has a
                    // non-increasing edge in this fixed block order. Scalar-
                    // free loops must also yield to a pending exclusive writer.
                    for(auto* next: ::llvm::successors(&block))
                        if(order.lookup(next)<=order.lookup(&block))
                        {checkpoints.push_back(&*block.getFirstInsertionPt());break;}
                    for(auto& inst:block)
                    {
                        if(auto* call=::llvm::dyn_cast<::llvm::CallInst>(&inst))
                        {
                            if(is_poll(*call))continue;
                            boundaries.push_back(call);
                            auto it=declarations.find(call->getCalledFunction());
                            if(it==declarations.end())continue;
                            auto layout=shared_scalar_owner(*call,it->second,false,true);
                            if(layout.owner_slot==&memories_[0])sites.push_back({call,layout,it->second});
                        }
                        else if(inst.isTerminator() && !inst.getNumSuccessors() && !block.getTerminatingMustTailCall())
                            boundaries.push_back(&inst);
                    }
                }
                if(sites.empty())continue;
                auto const instructions=sites.size()*50u+boundaries.size()+checkpoints.size()*8u+16u;
                auto const blocks=sites.size()*6u+checkpoints.size()*2u;
                if(instructions>room.instructions || blocks>room.blocks)continue;
                room.instructions-=instructions;room.blocks-=blocks;
                if(!state_fn)
                {
                    state_fn=declare("uwvm.threaded.scalar.state.v1",::llvm::FunctionType::get(ptr,{word},false),address(&current_shared_scalar_state));
                    r.enter=declare("uwvm.threaded.scalar.enter.v1",::llvm::FunctionType::get(word,{ptr,ptr,ptr,ptr},false),address(&enter_scalar_region));
                    r.leave=declare("uwvm.threaded.scalar.leave.v1",::llvm::FunctionType::get(types.getVoidTy(),false),address(&leave_scalar_region));
                    if(!state_fn || !r.enter || !r.leave)return true;
                }
                auto& entry=function.getEntryBlock();
                ::llvm::IRBuilder<> b(&entry,entry.getFirstInsertionPt());
                object_emission objects{*this,true};
                auto const prefix=emit::get_llvm_runtime_module_symbol_prefix(*module_);
                auto bind=[&](auto pointer,::uwvm2::utils::container::u8string_view suffix)
                {
                    auto const name=::uwvm2::utils::container::u8concat_uwvm(prefix,suffix);
                    ::uwvm2::utils::container::u8string_view const semantic{name.data(),name.size()};
                    return object_emission::bind(&objects,b,address(pointer),semantic);
                };
                auto* mod=bind(module_,u8"_runtime_module");
                r.child=bind(this,u8"_threaded_child");
                r.owner=bind(memories_[0],u8"_memory_0_native_memory");
                r.slot=bind(&memories_[0],u8"_threaded_memory0_owner_slot");
                if(!mod || !r.child || !r.owner || !r.slot)return true;
                r.state=scalar_region_call(b,state_fn,{b.CreatePtrToInt(mod,word)});
                auto field=[&](::llvm::Value* p,::std::size_t offset){return b.CreateConstGEP1_64(b.getInt8Ty(),p,offset);};
                r.held=field(r.state,offsetof(native_poll_state,scalar_gate));
                r.stop=b.CreateLoad(ptr,field(r.state,offsetof(native_poll_state,stop_byte)));
                auto const layout=sites.front().layout;
                r.gate=b.CreateLoad(ptr,field(r.owner,layout.gate));
                r.length=b.CreateLoad(ptr,field(r.owner,layout.length));
                r.base=b.CreateLoad(ptr,field(r.owner,layout.base));
                if(module_->memory_policy.check_tags)
                {
                    auto const& shadow=memories_[0]->memtag_shadow;
                    r.tags=b.CreateLoad(ptr,field(r.owner,address(&shadow.data_)-address(memories_[0])));
                    r.tag_start=shadow.tag_start_bit();
                }
                for(auto* at:checkpoints)
                {
                    b.SetInsertPoint(at);
                    auto* requested=scalar_writer_requested(b,r.gate);
                    auto* then=::llvm::SplitBlockAndInsertIfThen(requested,at,false,
                        ::llvm::MDBuilder{context}.createBranchWeights(1u,2000u));
                    b.SetInsertPoint(then);scalar_region_call(b,r.leave);
                }
                for(auto const& s:sites)emit_scalar_region_access(*s.call,r,s.layout,s.shape);
                // The original scalar calls are now cold edges. A failing
                // proof drops the lease before the complete diagnostic bridge.
                for(auto* at:boundaries)
                {
                    b.SetInsertPoint(at);
                    scalar_region_call(b,r.leave);
                }
                attempted=true;
            }
            return state_fn!=nullptr;
        }
