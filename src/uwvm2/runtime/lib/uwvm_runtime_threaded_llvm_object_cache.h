// Private to the group compiler. RuntimeDyld always relocates a new, checked
// object into the requesting child's own native arena.
        struct object_emission
        {
            ThreadedLlvmChild& child;
            ::llvm::DenseMap<::std::pair<::llvm::Function*,::llvm::GlobalVariable*>,::llvm::Value*> loads{};
            explicit object_emission(ThreadedLlvmChild& owner):child{owner}{}
            static ::llvm::Value* bind(void* context,::llvm::IRBuilder<>& at,::std::uintptr_t address,
                ::uwvm2::utils::container::u8string_view semantic) try
            {
                namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
                auto& self=*static_cast<object_emission*>(context);
                auto* function=at.GetInsertBlock()->getParent();
                auto* module=function->getParent();
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
                auto [it,fresh]=self.loads.try_emplace({function,symbol},nullptr);
                if(fresh)
                {
                    // This private carrier is fixed by the loader for the life of
                    // this image. Load once per activation, outside guest loops.
                    auto& entry=function->getEntryBlock();
                    ::llvm::IRBuilder<> builder(&entry,entry.getFirstInsertionPt());
                    it->second=emit::get_llvm_relocatable_host_symbol_pointer(builder,symbol);
                }
                return it->second;
            }
            catch(::std::bad_alloc const&){return nullptr;}
        };

        struct object_scope
        {
            void const* admission{};
            void const* file{};
            ::std::size_t source_id{},instructions{},blocks{},relocations{};
            runtime_llvm_jit_full_pipeline_kind pipeline{};
            ::llvm::CodeGenOptLevel level{};
            bool scalar_lowering{};
            bool operator==(object_scope const&) const = default;
        };
        struct prepared_object
        {
            object_scope scope;
            ::uwvm2::utils::container::std_string input,object;
            decltype(native_frames_) frames;
            decltype(native_call_names_) names;
            decltype(native_call_edges_) edges;
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
