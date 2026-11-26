// Private threaded compiler. The original authenticated bridge remains the cold
// edge, including its post-call stop/diagnostic probe.
        [[nodiscard]] static native_poll_state const* current_shared_scalar_state(::std::uintptr_t module) noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            auto* e=active_;
            if(e && e->child && e->stop_byte && !e->child->module_->memory_policy.check_tags &&
               !mt::asynchronous_requested() && !mt::hardware_checked_access_requested() &&
               owns_gc_module(module))return e;
            static constexpr native_poll_state unavailable{.rejected=1u};
            return &unavailable;
        }

        [[nodiscard]] shared_scalar_layout shared_scalar_owner(::llvm::CallInst const& call,shared_scalar_shape shape) const noexcept
        {
            auto const* owner=::llvm::dyn_cast<::llvm::ConstantInt>(call.getArgOperand(0u));
            auto const* offset=::llvm::dyn_cast<::llvm::ConstantInt>(call.getArgOperand(1u));
            if(!owner||!offset)return {};
            for(auto const& memory:memories_)
            {
                if(!memory||owner->getZExtValue()!=address(memory))continue;
                auto const& shadow=memory->memtag_shadow;
                if(!shadow.stable_address()||shadow.hardware_selected()||!shadow.shared()||!shadow.ignored_bits()||
                   shadow.tag_data()==nullptr||shadow.shared_tag_gate_==nullptr||!memory->memory_begin||!memory->memory_length_p||
                   static_cast<unsigned>(shadow.width())!=shape.address_bits||memory64(*memory)!=(shape.address_bits==64u)||
                   !::uwvm2::runtime::wasm_threads::is_shared(*memory))return {};
                auto const mask=shadow.address_mask();
                // With at least one ignored bit, both the canonical address and
                // offset + width - 1 fit the mask. Their exclusive end fits u64,
                // even for memory64. All other immediates stay at the full bridge.
                if(mask>UINT64_MAX/2u||shape.bytes-1u>mask||offset->getZExtValue()>mask-(shape.bytes-1u))return {};
                auto field=[&](auto const& value){return address(::std::addressof(value))-address(memory);};
                return {::std::addressof(memory),mask,field(memory->memory_begin),field(memory->memory_length_p),
                    field(shadow.prepared_memory_bytes_),field(shadow.shared_tag_gate_)};
            }
            return {};
        }

        [[nodiscard]] bool inline_shared_scalars(::llvm::Module& module,
            ::uwvm2::runtime::wasm_threads::threaded_llvm_module_limits room,bool& attempted)
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            namespace mt=::uwvm2::extension::memtag;
            auto const& admission=world_->admission();
            if(module_->memory_policy.check_tags||mt::asynchronous_requested()||mt::hardware_checked_access_requested()||
               admission.debug_observation()!=::uwvm2::runtime::wasm_threads::threaded_debug_observation::none||
               admission.instruction_control()||CHAR_BIT!=8||sizeof(bool)!=1u||
               !::std::atomic_ref<::std::byte>::is_always_lock_free||!::std::atomic_ref<unsigned char>::is_always_lock_free||
               ::std::atomic_ref<::std::byte>::required_alignment!=1u||::std::atomic_ref<unsigned char>::required_alignment!=1u||
               !::std::atomic_uint::is_always_lock_free||sizeof(::std::atomic_uint)!=sizeof(unsigned)||
               !::std::atomic_size_t::is_always_lock_free||sizeof(::std::atomic_size_t)!=sizeof(::std::uintptr_t))return false;
            ::llvm::DenseMap<::llvm::Function*,shared_scalar_shape> declarations;
            for(auto const& bridge:emitted_bridges_)
            {
                if(!emit::memtag_shared_lease_declaration(*bridge.declaration,emit::memtag_shared_access_family::scalar,
                    sizeof(::std::uintptr_t)*CHAR_BIT,emit::get_llvm_jit_host_calling_conv()))continue;
                shared_scalar_shape shape{};
                if(memtag_bridge_replacement(*bridge.declaration,bridge.original,&shape)&&shape.bytes)
                    declarations.try_emplace(bridge.declaration,shape);
            }
            char const name[]{"uwvm.threaded.scalar.state.v1"};
            if(declarations.empty()||module.getNamedValue(name)||externals_.size()>=256u)return false;
            ::llvm::Function* state_fn{};
            for(auto& function:module)
            {
                if(function.isDeclaration())continue;
                ::llvm::SmallVector<::llvm::CallInst*,16> sites;
                for(auto& block:function)for(auto& instruction:block)
                    if(auto* call=::llvm::dyn_cast<::llvm::CallInst>(&instruction);call && declarations.contains(call->getCalledFunction()))
                        sites.push_back(call);
                ::llvm::Value* state{};
                for(auto* call:sites)
                {
                    auto const shape=declarations.find(call->getCalledFunction())->second;
                    auto const layout=shared_scalar_owner(*call,shape);
                    if(!layout.owner_slot)continue;
                    // Six blocks and at most 64 fixed instructions plus six per
                    // byte, including the state call. The factory recounts exact
                    // IR and checks actual frame, relocation and object budgets.
                    auto const cost=64u+6u*shape.bytes;
                    if(room.instructions<cost||room.blocks<6u)return state_fn!=nullptr;
                    if(!state)
                    {
                        auto& entry=function.getEntryBlock();
                        ::llvm::IRBuilder<> b(&entry,entry.getFirstInsertionPt());
                        auto* word=b.getIntNTy(sizeof(::std::uintptr_t)*CHAR_BIT);
                        if(!state_fn)
                        {
                            state_fn=::llvm::Function::Create(::llvm::FunctionType::get(b.getPtrTy(),{word},false),
                                ::llvm::Function::ExternalLinkage,name,module);
                            state_fn->addFnAttr(::llvm::Attribute::NoUnwind);
                            emit::apply_llvm_jit_host_calling_conv(*state_fn);
                            if(!add_external(*state_fn,address(&current_shared_scalar_state))){state_fn->eraseFromParent();return false;}
                        }
                        state=emit::apply_llvm_jit_calling_conv(b.CreateCall(state_fn,{::llvm::ConstantInt::get(word,address(module_))}),emit::get_llvm_jit_host_calling_conv());
                    }
                    attempted=true;
                    emit_shared_scalar(*call,state,address(this),layout,shape);
                    room.instructions-=cost;room.blocks-=6u;
                }
            }
            return state_fn!=nullptr;
        }
