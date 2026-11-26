// Private to the group compiler. RuntimeDyld always relocates a new, checked
// object into the requesting child's own native arena.
        struct object_emission
        {
            ThreadedLlvmChild& child;
            explicit object_emission(ThreadedLlvmChild& owner):child{owner}{}
            static ::llvm::Value* bind(void* context,::llvm::IRBuilder<>& at,::std::uintptr_t address,
                ::uwvm2::utils::container::u8string_view semantic) try
            {
                namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
                auto& self=*static_cast<object_emission*>(context);
                auto* module=at.GetInsertBlock()->getModule();
                auto name=::uwvm2::utils::container::u8concat_uwvm(u8"uwvm.threaded.object.",semantic);
                auto const ref=emit::get_llvm_string_ref(name);
                if(!address)return nullptr;
                auto literal=[&]{return emit::get_llvm_host_pointer_value(at,address,at.getPtrTy());};
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

#include "uwvm_runtime_threaded_llvm_instance_template.h"
        struct object_scope
        {
            void const* admission{};
            void const* file{};
            ::std::size_t source_id{},instructions{},blocks{},relocations{};
            runtime_llvm_jit_full_pipeline_kind pipeline{};
            ::llvm::CodeGenOptLevel level{};
            bool scalar_lowering{},check_tags{};
            bool operator==(object_scope const&) const = default;
        };
        struct object_evidence
        {
            ::std::uint64_t sections{},relocations{},symbols{},charge{3u*native_arena_granule},arena_bytes{};
            object_stack_shape stack;
            bool complete{},crel{};
        };
        struct object_entries { ::std::size_t typed{},raw{}; };
        static constexpr ::std::size_t object_binding_limit=256u;
        using object_import_mask=::std::array<bool,object_binding_limit>;
        struct prepared_object
        {
            object_scope scope;
            object_evidence evidence;
            ::uwvm2::utils::container::std_string input,object;
            decltype(native_frames_) frames;
            ::uwvm2::utils::container::std_vector<object_entries> entries;
            ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_string> imports;
            ::uwvm2::utils::container::std_unique_ptr<instance_template> instance;
            bool polls{},strict_fp{};
            ::std::size_t retained_bytes{};
        };
        static constexpr ::std::size_t maximum_cached_object_storage=16u<<20u;
        struct object_load_pin
        {
            object_cache* cache{};
            ::uwvm2::utils::container::std_unique_ptr<prepared_object> object;
            void take(object_cache& owner) noexcept
            {cache=&owner;object=::std::move(owner.object);}
            ~object_load_pin()
            {
                // The group compiler serializes cache access. Restore its slot after
                // loader teardown, including unwinding a failed or cancelled child.
                if(object&&!cache->object)cache->object=::std::move(object);
            }
        };

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
            ::llvm::StringRef input,::llvm::StringRef object,::std::size_t limit,bool polls,bool strict_fp,
            object_evidence const& evidence,native_frame_index const& by_name,object_import_mask const& required) const
        {
            // Include the exact retained capacities, including every string terminator.
            auto add=[&](::std::size_t& n,::std::size_t bytes)
            {if(n>limit||bytes>limit-n)return false;n+=bytes;return true;};
            auto base=[&](auto const& frames,::std::size_t key_bytes,::std::size_t object_bytes,
                ::std::size_t entries,::std::size_t imports)->::std::size_t
            {
                ::std::size_t n=sizeof(prepared_object);
                if(frames.capacity()>limit/sizeof(native_frame_record) || entries>limit/sizeof(object_entries) ||
                   imports>limit/sizeof(::uwvm2::utils::container::std_string) ||
                   !add(n,frames.capacity()*sizeof(native_frame_record)) || !add(n,entries*sizeof(object_entries)) ||
                   !add(n,imports*sizeof(::uwvm2::utils::container::std_string)) ||
                   !add(n,key_bytes)||!add(n,object_bytes)||!add(n,2u))return 0u;
                for(auto const& frame:frames)if(!add(n,frame.name.capacity()+1u))return 0u;
                return n;
            };
            auto const imports=static_cast<::std::size_t>(::std::count(required.begin(),required.end(),true));
            auto estimate=base(native_frames_,input.size(),object.size(),entries_.size(),imports);
            if(!estimate || externals_.size()>required.size())return {};
            for(::std::size_t i{};i<externals_.size();++i)
                if(required[i]&&!add(estimate,externals_[i].name.size()+1u))return {};
            auto saved=::uwvm2::utils::container::make_unique<prepared_object>();
            saved->scope=scope;saved->evidence=evidence;
            saved->input=::uwvm2::utils::container::copy_string(input);
            saved->object=::uwvm2::utils::container::copy_string(object);
            // These frames contain only object section indices, offsets, sizes and names.
            saved->frames=native_frames_;
            saved->entries.reserve(entries_.size());
            for(auto const& entry:entries_)
            {
                auto const typed=by_name.find(::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(entry.typed_name));
                auto const raw=by_name.find(::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(entry.raw_name));
                if(typed==by_name.end()||raw==by_name.end())return {};
                saved->entries.push_back({typed->second,raw->second});
            }
            saved->imports.reserve(imports);
            for(::std::size_t i{};i<externals_.size();++i)
                if(required[i])saved->imports.push_back(externals_[i].name);
            saved->polls=polls;saved->strict_fp=strict_fp;
            auto actual=base(saved->frames,saved->input.capacity(),saved->object.capacity(),saved->entries.capacity(),saved->imports.capacity());
            if(!actual)return {};
            for(auto const& name:saved->imports)if(!add(actual,name.capacity()+1u))return {};
            saved->retained_bytes=actual;
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
            native_frames_=saved.frames;
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
