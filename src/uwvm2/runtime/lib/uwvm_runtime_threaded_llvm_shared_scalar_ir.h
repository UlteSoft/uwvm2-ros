// ThreadedLlvmChild member fragment, also used by the native scalar IR regression.
        struct shared_scalar_shape { unsigned address_bits{},result_bits{},bytes{};bool sign{},store{}; };
        struct shared_scalar_layout
        {
            memory_type* const* owner_slot{};
            ::std::uint64_t mask{};
            ::std::size_t base{},length{},prepared{},gate{};
        };
        [[nodiscard]] static bool matches_shared_scalar_owner(::llvm::Value const* operand,::std::uintptr_t owner)
        {
            // Threaded emission materializes a native owner as a pointer, then
            // converts it to a word for the bridge ABI. Before the target/layout
            // pass, LLVM can retain both casts as one uniqued constant expression.
            auto& context=operand->getContext();
            auto* word=::llvm::Type::getIntNTy(context,sizeof(::std::uintptr_t)*CHAR_BIT);
            auto* literal=::llvm::ConstantInt::get(word,owner);
            if(operand==literal)return true;
            auto* pointer=::llvm::ConstantExpr::getIntToPtr(literal,::llvm::PointerType::getUnqual(context));
            return operand==::llvm::ConstantExpr::getPtrToInt(pointer,word);
        }
        static void emit_shared_scalar(::llvm::CallInst& call,::llvm::Value* state,::std::uintptr_t child,
            shared_scalar_layout layout,shared_scalar_shape shape)
        {
            namespace locks=::uwvm2::utils::mutex;
            static_assert(offsetof(locks::rwlock_t,bits)==0);
            auto& c=call.getContext();
            auto* function=call.getFunction();
            auto* origin=call.getParent();
            auto* slow=origin->splitBasicBlock(call.getIterator(),"shared.scalar.bridge");
            auto* done=slow->splitBasicBlock(call.getNextNode()->getIterator(),"shared.scalar.done");
            auto block=[&](char const* name){return ::llvm::BasicBlock::Create(c,name,function,slow);};
            auto* scope=block("shared.scalar.scope");
            auto* bounds=block("shared.scalar.bounds");
            auto* claim=block("shared.scalar.claim");
            auto* effect=block("shared.scalar.effect");
            auto* likely=::llvm::MDBuilder{c}.createBranchWeights(2000u,1u);
            origin->getTerminator()->eraseFromParent();
            ::llvm::IRBuilder<> b(origin);
            auto* ptr=b.getPtrTy();
            auto* byte=b.getInt8Ty();
            auto* word=b.getIntNTy(sizeof(::std::uintptr_t)*CHAR_BIT);
            auto* lock_type=b.getIntNTy(sizeof(unsigned)*CHAR_BIT);
            auto host=[&](::std::uintptr_t value){return b.CreateIntToPtr(::llvm::ConstantInt::get(word,value),ptr);};
            auto field=[&](::llvm::Value* from,::std::size_t at){return b.CreateConstGEP1_64(byte,from,at);};
            auto read=[&](::llvm::Type* type,::llvm::Value* address,bool live=false)
            {auto* load=b.CreateLoad(type,address);load->setVolatile(live);return load;};
            auto atomic=[&](::llvm::Type* type,::llvm::Value* address,::llvm::AtomicOrdering order,::llvm::Align align=::llvm::Align{1})
            {auto* load=b.CreateAlignedLoad(type,address,align,true);load->setAtomic(order);return load;};
            // The TLS operation at native entry proved the pthread, lease, world
            // and admission. Its execution cannot migrate or nest. Imports and
            // reference calls switch child/policy and restore them on return.
            // Re-read the live child, canonical owner slot, scoped check policy,
            // local rejection and group stop for this particular operation.
            b.CreateCondBr(b.CreateICmpEQ(read(ptr,field(state,offsetof(native_poll_state,child)),true),host(child)),scope,slow,likely);
            b.SetInsertPoint(scope);
            auto* owner=read(ptr,host(address(layout.owner_slot)),true);
            auto* accepted=b.CreateICmpEQ(b.CreatePtrToInt(owner,word),call.getArgOperand(0u));
            accepted=b.CreateAnd(accepted,b.CreateIsNull(read(byte,field(state,offsetof(native_poll_state,rejected)),true)));
            auto* checks=read(ptr,field(state,offsetof(native_poll_state,access_checks)));
            accepted=b.CreateAnd(accepted,b.CreateIsNull(read(byte,checks,true)));
            auto* stop=read(ptr,field(state,offsetof(native_poll_state,stop_byte)));
            accepted=b.CreateAnd(accepted,b.CreateIsNull(atomic(byte,stop,::llvm::AtomicOrdering::Acquire)));
#if defined(__linux__) && defined(__aarch64__)
            // Another newly configured physical owner can enable deferred-poll
            // machinery process-wide, including for a software memory.
            auto* hardware=host(address(::std::addressof(::uwvm2::extension::memtag::selected_hardware_owner)));
            accepted=b.CreateAnd(accepted,b.CreateIsNull(atomic(byte,hardware,::llvm::AtomicOrdering::Acquire)));
#endif
            b.CreateCondBr(accepted,bounds,slow,likely);
            b.SetInsertPoint(bounds);
            // These are actual C++ atomic objects. The target layout has not
            // been installed yet, so IRBuilder's default alignment can be
            // smaller than the native word and force an out-of-line atomic.
            auto const extent_align=::llvm::Align{alignof(::std::atomic_size_t)};
            auto* length=b.CreateZExtOrTrunc(atomic(word,read(ptr,field(owner,layout.length)),::llvm::AtomicOrdering::Acquire,extent_align),b.getInt64Ty());
            auto* prepared=b.CreateZExtOrTrunc(atomic(word,field(owner,layout.prepared),::llvm::AtomicOrdering::Acquire,extent_align),b.getInt64Ty());
            auto* effective=b.CreateAdd(b.CreateAnd(call.getArgOperand(2u),b.getInt64(layout.mask)),call.getArgOperand(1u));
            auto* fits=b.CreateICmpULE(b.CreateAdd(effective,b.getInt64(shape.bytes)),length);
            fits=b.CreateAnd(fits,b.CreateICmpULE(length,prepared));
            fits=b.CreateAnd(fits,b.CreateICmpULE(length,b.getInt64(layout.mask+1u)));
            auto* gate=read(ptr,field(owner,layout.gate));
            auto* old=atomic(lock_type,gate,::llvm::AtomicOrdering::Monotonic,::llvm::Align{alignof(::std::atomic_uint)});
            auto* available=b.CreateIsNull(b.CreateAnd(old,locks::rwlock_writer_mask()|locks::rwlock_pending_mask()));
            available=b.CreateAnd(available,b.CreateICmpULE(old,::llvm::ConstantInt::get(lock_type,UINT_MAX-locks::rwlock_reader_mask())));
            b.CreateCondBr(b.CreateAnd(fits,available),claim,slow,likely);
            b.SetInsertPoint(claim);
            auto* readers=::llvm::ConstantInt::get(lock_type,locks::rwlock_reader_mask());
            auto* acquired=b.CreateAtomicCmpXchg(gate,old,b.CreateAdd(old,readers),::llvm::Align{alignof(::std::atomic_uint)},
                ::llvm::AtomicOrdering::Acquire,::llvm::AtomicOrdering::Monotonic);
            b.CreateCondBr(b.CreateExtractValue(acquired,1u),effect,slow,likely);
            b.SetInsertPoint(effect);
            // No host call, poll or trap can interrupt this finite section.
            // Use the same one-byte relaxed atomic effects as the shared bridge.
            auto* base=read(ptr,field(owner,layout.base));
            auto* start=b.CreateGEP(byte,base,b.CreateZExtOrTrunc(effective,word));
            auto* bits=b.getIntNTy(shape.bytes*8u);
            ::llvm::Value* result=::llvm::ConstantInt::get(bits,0u);
            for(unsigned i{};i!=shape.bytes;++i)
            {
                auto* where=field(start,i);
                if(shape.store)
                {
                    auto* value=b.CreateTrunc(b.CreateLShr(call.getArgOperand(3u),i*8u),byte);
                    auto* store=b.CreateAlignedStore(value,where,::llvm::Align{1u},true);
                    store->setAtomic(::llvm::AtomicOrdering::Monotonic);
                }
                else result=b.CreateOr(result,b.CreateShl(b.CreateZExtOrTrunc(atomic(byte,where,::llvm::AtomicOrdering::Monotonic),bits),i*8u));
            }
            b.CreateAtomicRMW(::llvm::AtomicRMWInst::Sub,gate,readers,::llvm::Align{alignof(::std::atomic_uint)},::llvm::AtomicOrdering::Release);
            result=b.CreateIntCast(result,b.getIntNTy(shape.result_bits),shape.sign);
            result=b.CreateZExtOrTrunc(result,b.getInt64Ty());
            b.CreateBr(done);
            if(!call.use_empty())
            {
                b.SetInsertPoint(done,done->begin());
                auto* phi=b.CreatePHI(b.getInt64Ty(),2u);
                call.replaceAllUsesWith(phi);phi->addIncoming(&call,slow);phi->addIncoming(result,effect);
            }
        }
