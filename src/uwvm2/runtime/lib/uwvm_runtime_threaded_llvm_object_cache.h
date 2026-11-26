// Private to the group compiler. RuntimeDyld always relocates a new, checked
// object into the requesting child's own native arena.
        struct object_emission
        {
            ThreadedLlvmChild& child;
            bool require_symbol{};
            static ::llvm::Value* bind(void* context,::llvm::IRBuilder<>& at,::std::uintptr_t address,
                ::uwvm2::utils::container::u8string_view semantic) try
            {
                namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
                auto& self=*static_cast<object_emission*>(context);
                auto* module=at.GetInsertBlock()->getModule();
                auto name=::uwvm2::utils::container::u8concat_uwvm(u8"uwvm.threaded.object.",semantic);
                auto const ref=emit::get_llvm_string_ref(name);
                if(!address)return nullptr;
                auto literal=[&]{return self.require_symbol ? nullptr : emit::get_llvm_host_pointer_value(at,address,at.getPtrTy());};
                if(ref.size()>256u)return literal();
                auto* symbol=module->getNamedGlobal(ref);
                if(!symbol)
                {
                    if(module->getNamedValue(ref)||self.child.externals_.size()>=256u)return literal();
                    // Only the address of this declaration initializes a native
                    // relocation. The pointee is accessed through a loaded pointer.
                    symbol=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::make_global(
                        *module,at.getInt8Ty(),false,::llvm::GlobalValue::ExternalLinkage,nullptr,ref);
                    self.child.externals_.push_back({::uwvm2::utils::container::copy_string(ref),address});
                }
                else
                {
                    bool bound{};
                    for(auto const& native:self.child.externals_)
                        if(native.name==ref){bound=native.address==address;break;}
                    if(!bound)return literal();
                }
                // EH/GC lowering can later insert a new use *before* an
                // existing entry instruction. Each use initially dominates at
                // its actual insertion point. Hoist only in the completed IR.
                return emit::get_llvm_relocatable_host_symbol_pointer(at,symbol);
            }
            catch(::std::bad_alloc const&){return nullptr;}
        };

        // Only the cold compiler can create these private pointer carriers.
        // Match the actual mapping as well as the IR operand before specializing
        // a call whose first argument is a relocated native memory owner.
        [[nodiscard]] bool matches_bound_object(::llvm::Value const* operand,::std::uintptr_t expected) const noexcept
        {
            auto const* cast=::llvm::dyn_cast<::llvm::PtrToIntInst>(operand);
            auto const* load=cast ? ::llvm::dyn_cast<::llvm::LoadInst>(cast->getPointerOperand()) : nullptr;
            if(!load || !load->isVolatile() || load->isAtomic())return false;
            auto const* carrier=::llvm::dyn_cast<::llvm::GlobalVariable>(load->getPointerOperand());
            if(!carrier || !carrier->hasPrivateLinkage() || !carrier->hasInitializer() ||
               !carrier->getName().starts_with("uwvm.host.binding.uwvm.threaded.object."))return false;
            auto const* symbol=::llvm::dyn_cast<::llvm::GlobalVariable>(carrier->getInitializer());
            if(!symbol || !symbol->isDeclaration() || !symbol->getName().starts_with("uwvm.threaded.object."))return false;
            for(auto const& mapping:externals_)
                if(mapping.name==symbol->getName())return mapping.address==expected;
            return false;
        }

        static void hoist_object_bindings(::llvm::Module& module)
        {
            for(auto& carrier:module.globals())
            {
                if(!carrier.hasPrivateLinkage() || !carrier.hasInitializer() ||
                   !carrier.getName().starts_with("uwvm.host.binding.uwvm.threaded.object."))continue;
                auto const* symbol=::llvm::dyn_cast<::llvm::GlobalVariable>(carrier.getInitializer());
                if(!symbol || !symbol->isDeclaration() || !symbol->getName().starts_with("uwvm.threaded.object."))continue;
                ::llvm::DenseMap<::llvm::Function*,::llvm::LoadInst*> entries;
                for(auto it=carrier.user_begin();it!=carrier.user_end();)
                {
                    auto* load=::llvm::dyn_cast<::llvm::LoadInst>(*it++);
                    if(!load || !load->isVolatile() || load->isAtomic() || load->getPointerOperand()!=&carrier)continue;
                    auto* function=load->getFunction();
                    auto [first,fresh]=entries.try_emplace(function,load);
                    if(fresh)
                    {
                        // Only the loader-written pointer is immutable. Guest
                        // data, owner, bounds, TLS and stop reads stay at their
                        // original locations and retain their original ordering.
                        auto& entry=function->getEntryBlock();
                        auto where=entry.getFirstInsertionPt();
                        if(&*where!=load)load->moveBefore(entry,where);
                    }
                    else
                    {
                        load->replaceAllUsesWith(first->second);
                        load->eraseFromParent();
                    }
                }
            }
        }

        struct adm16_object_owner
        {
            void const* source{};
            ::std::uint_least64_t generation{},validation_epoch{};
            memory_type const* memory{};
            void const* base{};
            ::std::atomic_size_t const* length{};
            void const* tags{};
            void const* gate{};
            ::std::uint64_t maximum{},mask{};
            ::std::size_t reserved{};
            unsigned tag_start{};
            bool operator==(adm16_object_owner const&) const = default;
        };

        [[nodiscard]] bool qualify_adm16_object_owner(adm16_object_owner& result) const noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            namespace adm=::uwvm2::extension::adm;
            namespace st=::uwvm2::uwvm::runtime::storage;
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            // The preflight list consists of actual shared definitions reached
            // from this generation's validated imports. Its admission pins the
            // source until the group has retired every child and its cache.
            if(memories_.size()!=1u || memories_[0]->memtag_shadow.ignored_bits()!=16u ||
               world_->shared_memories_.size()!=1u || module_->imported_memory_vec_storage.size()!=1u ||
               !module_->local_defined_memory_vec_storage.empty() ||
               mt::asynchronous_requested() || mt::hardware_checked_access_requested() || mt::logging_requested())return false;
            auto const* record=world_->shared_memories_[0];
            auto const* memory=memories_[0];
            auto const& imported=st::runtime_memory_type(*module_,0u);
            auto const& shadow=memory->memtag_shadow;
            if(!record || !record->memory_type_ptr || &record->memory!=memory ||
               module_->imported_memory_vec_storage.index_unchecked(0u).target.defined_ptr!=record ||
               !memory64(*memory) || !::uwvm2::runtime::wasm_threads::is_shared(*memory) ||
               mt::is_tagged_memory(*record->memory_type_ptr)!=shadow.enabled() || mt::is_tagged_memory(imported)!=shadow.enabled() ||
               adm::ignored_bits(*record->memory_type_ptr)!=16u || adm::ignored_bits(imported)!=16u)return false;
            auto const info=emit::resolve_runtime_memory_access_info(*module_,0u);
            if(!shadow.shared() || shadow.hardware_selected() || shadow.width()!=mt::address_width::i64 ||
               shadow.address_mask()!=mt::adm_address_mask(mt::address_width::i64,16u) ||
               !adm::valid_tag_field(true,16u,shadow.tag_start_bit()) || shadow.maximum_memory_bytes()>shadow.address_mask()+1u ||
               info.memory_p!=memory || !info.stable_memory_begin || !info.stable_memory_length_p ||
               !info.stable_memory_reserved_span_bytes)return false;
            // Growth publishes prepared shadow storage before live length.
            // Neither mutable extent nor any tag byte is compiled into a plan.
            auto const live=info.stable_memory_length_p->load(::std::memory_order_acquire);
            if(live>shadow.maximum_memory_bytes() || live>info.stable_memory_reserved_span_bytes)return false;
            if(shadow.enabled())
            {
                if(!shadow.stable_address() || !shadow.tag_data() || !shadow.shared_tag_gate_ ||
                   live>shadow.prepared_memory_bytes())return false;
            }
            else if(shadow.tag_data() || shadow.shared_tag_gate_)return false;
            result={world_->source_owner().get(),world_->source_generation(),world_->admission().source_validation_epoch(),
                memory,info.stable_memory_begin,info.stable_memory_length_p,shadow.tag_data(),shadow.shared_tag_gate_,
                shadow.maximum_memory_bytes(),shadow.address_mask(),info.stable_memory_reserved_span_bytes,shadow.tag_start_bit()};
            return true;
        }

#include "uwvm_runtime_threaded_llvm_instance_template.h"
        struct object_scope
        {
            void const* admission{};
            void const* file{};
            ::std::size_t source_id{},instructions{},blocks{},relocations{};
            runtime_llvm_jit_full_pipeline_kind pipeline{};
            ::llvm::CodeGenOptLevel level{};
            bool scalar_lowering{},check_tags{};
            adm16_object_owner adm16{};
            bool operator==(object_scope const&) const = default;
        };
        struct prepared_object
        {
            object_scope scope;
            ::uwvm2::utils::container::std_string input,object;
            decltype(native_frames_) frames;
            decltype(native_call_names_) names;
            decltype(native_call_edges_) edges;
            ::uwvm2::utils::container::std_unique_ptr<instance_template> instance;
            bool known_graph{},polls{},strict_fp{};
            ::std::size_t retained_bytes{};
        };
        static constexpr ::std::size_t maximum_cached_object_storage=16u<<20u;

        void write_object_input(::llvm::Module const& module,::llvm::TargetMachine const& target,bounded_object_stream& key) const
        {
            // Exact compiler input, including any remaining process addresses or
            // immediate values. A newly introduced unsymbolized value can only
            // cause a miss. Source identity is checked independently.
            ::llvm::WriteBitcodeToFile(module,key);
            auto word=[&](::std::uint64_t n){key.write(reinterpret_cast<char const*>(&n),sizeof(n));};
            auto string=[&](::llvm::StringRef s){word(s.size());key.write(s.data(),s.size());};
            word(key.tell());
            string(target.getTargetCPU());
            string(target.getTargetFeatureString());
            word(externals_.size());
            for(auto const& binding:externals_)
            {
                auto name=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(binding.name);
                string(name);
                // Object addresses are rebound by this child's authenticated
                // emitter. Control transformations also inspect native *function*
                // identity, so include those actual mappings in the key.
                if(!name.starts_with("uwvm.threaded.object."))word(binding.address);
            }
        }
        [[nodiscard]] ::uwvm2::utils::container::std_unique_ptr<prepared_object> remember_object(object_scope scope,
            ::llvm::StringRef input,::llvm::StringRef object,::std::size_t limit,bool polls,bool strict_fp)
        {
            auto measure=[&](auto const& frames,auto const& names,auto const& edges,
                ::std::size_t key_bytes,::std::size_t object_bytes)->::std::size_t
            {
                // Charge the complete record and each retained allocation,
                // including string capacities and their terminators.
                ::std::size_t n=sizeof(prepared_object);
                auto add=[&](::std::size_t amount)
                { if(n>limit||amount>limit-n)return false;n+=amount;return true; };
                auto array=[&](auto const& v)
                { return v.capacity()<=limit/sizeof(v[0]) && add(v.capacity()*sizeof(v[0])); };
                if(!add(key_bytes)||!add(object_bytes)||!add(2u)||!array(frames)||!array(names)||!array(edges))return 0u;
                for(auto const& frame:frames)if(!add(frame.name.capacity()+1u))return 0u;
                for(auto const& name:names)if(!add(name.capacity()+1u))return 0u;
                return n;
            };
            if(!measure(native_frames_,native_call_names_,native_call_edges_,input.size(),object.size()))return {};
            auto saved=::uwvm2::utils::container::make_unique<prepared_object>();
            saved->scope=scope;saved->input=::uwvm2::utils::container::copy_string(input);
            saved->object=::uwvm2::utils::container::copy_string(object);
            saved->frames=native_frames_;saved->names=::std::move(native_call_names_);saved->edges=::std::move(native_call_edges_);
            // check_object repeats the correspondence of the observed final
            // MachineFunctions with this exact ELF's stack-size relocations.
            for(auto& f:saved->frames){f.section=UINT64_MAX;f.offset=UINT64_MAX;f.object_bytes=UINT64_MAX;}
            saved->known_graph=native_call_graph_known_;saved->polls=polls;saved->strict_fp=strict_fp;
            saved->retained_bytes=measure(saved->frames,saved->names,saved->edges,saved->input.capacity(),saved->object.capacity());
            if(!saved->retained_bytes)return {};
            return saved;
        }
        void remember_instance_template(prepared_object& saved,::llvm::StringRef inputs,::std::size_t bindings,
            bool shared_scalars,::std::size_t limit) const
        {
            if(inputs.empty() || !gc_scratch_.empty() || bindings>externals_.size())return;
            auto room=limit-saved.retained_bytes;
            auto fits=[&](::std::size_t n){if(n>room)return false;room-=n;return true;};
            if(!fits(sizeof(instance_template)) || !fits(inputs.size()+1u) || !fits(bindings*sizeof(instance_binding)))return;
            for(::std::size_t i{};i<bindings;++i)if(!fits(externals_[i].name.size()+1u))return;
            auto plan=::uwvm2::utils::container::make_unique<instance_template>();
            plan->inputs=::uwvm2::utils::container::copy_string(inputs);
            if(!capture_instance_bindings(*plan,bindings))return;
            plan->reference_dispatch=reference_dispatch_;plan->shared_scalars=shared_scalars;
            if(!fits(plan->inputs.capacity()-inputs.size()) ||
               !fits((plan->bindings.capacity()-bindings)*sizeof(instance_binding)))return;
            for(auto const& b:plan->bindings)if(!fits(b.name.capacity()-b.name.size()))return;
            saved.retained_bytes=limit-room;saved.instance=::std::move(plan);
        }
        [[nodiscard]] bool restore_object_witness(prepared_object const& saved)
        {
            native_frames_=saved.frames;native_call_names_=saved.names;native_call_edges_=saved.edges;
            native_call_graph_known_=saved.known_graph;
            auto add=[&](::llvm::StringRef name,::std::uintptr_t pointer)
            {
                if(externals_.size()>=256u)return false;
                for(auto const& binding:externals_)if(binding.name==name)return false;
                externals_.push_back({::uwvm2::utils::container::copy_string(name),pointer});return true;
            };
            namespace fp=::uwvm2::runtime::compiler::shared::strict_float_jit;
            return (!saved.polls || add("uwvm.threaded.poll.state.v1",address(&current_poll_state))) &&
                (!saved.strict_fp || add(fp::symbol_name,address(&fp::bridge)));
        }
