// Included inside ThreadedLlvmChild. A group owns one unopened native object.
// The instance template contains process code addresses and recipes for data
// relocations, never a pointer into a previous worker's private instance.
        enum class instance_slot : unsigned char { function, module, memory, base, length, global, reference };
        struct instance_binding
        {
            ::uwvm2::utils::container::std_string name;
            instance_slot slot{};
            ::std::uintptr_t operand{}; // process function or Wasm index
        };
        struct instance_template
        {
            ::uwvm2::utils::container::std_string inputs;
            ::uwvm2::utils::container::std_vector<instance_binding> bindings;
            bool reference_dispatch{},shared_scalars{};
        };

        // For this closed instruction family the frontend reads immutable Wasm
        // declarations, the selected target/profile, and one plain shared
        // native memory. All other instance data is accessed by relocation.
        [[nodiscard]] bool write_instance_inputs(::llvm::TargetMachine const& target,bounded_object_stream& key) const
        {
            namespace mt=::uwvm2::extension::memtag;
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            auto const& mod=*module_;
            if(mt::asynchronous_requested() || mt::hardware_checked_access_requested() || mt::logging_requested() ||
               mod.imported_memory_vec_storage.size()!=1u || !mod.local_defined_memory_vec_storage.empty() ||
               !mod.imported_global_vec_storage.empty() || !mod.imported_table_vec_storage.empty() ||
               !mod.imported_tag_vec_storage.empty() || !mod.local_defined_tag_vec_storage.empty() ||
               mod.type_section_storage.requires_gc || mod.type_section_storage.requires_exceptions ||
               mod.type_section_storage.requires_simd || memories_.size()!=1u)return false;
            auto const info=emit::resolve_runtime_memory_access_info(mod,0u);
            auto* memory=info.memory_p;
            if(!memory || memory!=memories_[0] || !::uwvm2::runtime::wasm_threads::is_shared(*memory) ||
               memory->memtag_shadow.enabled() || memory->memtag_shadow.ignored_bits() ||
               !info.effective_page_limits || !info.stable_memory_length_p ||
               !info.stable_memory_begin || info.local_imported_module_ptr)return false;
            // The only live-memory specialization is the shared scalar minimum.
            // Establish it even when that family is absent from this module.
            auto const shift=info.custom_page_size_log2;
            auto const pages=info.effective_page_limits->min;
            if(shift>=::std::numeric_limits<::std::size_t>::digits || pages>(info.stable_memory_reserved_span_bytes>>shift) ||
               (pages<<shift)>info.stable_memory_length_p->load(::std::memory_order_acquire))return false;

            auto word=[&](::std::uint64_t v){key.write(reinterpret_cast<char const*>(&v),sizeof(v));};
            auto string=[&](::llvm::StringRef s){word(s.size());key.write(s.data(),s.size());};
            word(address(world_->source_.get()));word(world_->source_generation());word(world_->admission().source_validation_epoch());
            word(world_->module_count());
            for(::std::size_t i{};i<world_->module_count();++i)
            {
                auto const* original=world_->original_module(i);
                auto const* file=world_->file(i);auto const* peer=world_->module(i);
                if(!original || !file || !peer || !world_->feature_parameter(i) || peer->module_name!=file->module_name)return false;
                word(world_->source_module_id(i));word(address(original));word(address(file));word(address(world_->feature_parameter(i)));
                word(world_->initialization_module_at(i));word(static_cast<unsigned>(peer->memory_policy.backend));word(peer->memory_policy.check_tags);
                string(emit::get_llvm_string_ref(peer->module_name));
                auto policy=[&](auto choice){word(choice?1u+static_cast<unsigned>(*choice):0u);};
                policy(peer->translation.llvm_policy);policy(peer->translation.llvm_full_policy);
            }
            // Local tables are dispatched through the current lease. Numeric
            // globals always load/store the current world's active union member.
            for(::std::size_t i{};i<mod.local_defined_global_vec_storage.size();++i)
            {
                auto const g=emit::resolve_runtime_global_access_info(mod,static_cast<::std::uint32_t>(i));
                auto const type=emit::get_runtime_wasm_value_type_encoding(g.value_type);
                if(!g.defined_storage_ptr || !g.storage_ptr || type<0x7cu || type>0x7fu)return false;
            }
            word(address(memory));word(address(info.stable_memory_begin));word(address(info.stable_memory_length_p));
            word(info.stable_memory_reserved_span_bytes);word(info.max_limit_memory_length);word(info.declared_minimum_byte_length);
            word(info.direct_memory_is_shared);word(info.custom_page_size_log2);word(memory64(*memory));
            word(info.mmap_requires_dynamic_bounds);word(info.mmap_covers_adm_domain);word(info.mmap_partial_guard_limit);
            word(info.mmap_data_capacity);word(info.mmap_covers_wasm32_effective_domain);
            for(auto n:info.effective_page_limits->policy_identity())word(n);
            string(target.getTargetCPU());string(target.getTargetFeatureString());string(target.getTargetTriple().str());
            string(target.createDataLayout().getStringRepresentation());
            return key.valid();
        }

        [[nodiscard]] ::std::uintptr_t instance_address(instance_binding const& b) const noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            switch(b.slot)
            {
                case instance_slot::function:return b.operand;
                case instance_slot::module:return address(module_);
                case instance_slot::memory:return address(memories_[0]);
                case instance_slot::base:return address(memories_[0]->memory_begin);
                case instance_slot::length:return address(memories_[0]->memory_length_p);
                case instance_slot::global:
                    if(b.operand<module_->local_defined_global_vec_storage.size())
                        return address(&module_->local_defined_global_vec_storage.index_unchecked(b.operand).global.storage);
                    break;
                case instance_slot::reference:
                    if(b.operand>=imports_ && b.operand-imports_<entries_.size())
                    {
                        auto const ref=emit::llvm_jit_funcref_from_table_elem(
                            emit::llvm_jit_resolve_table_elem_from_func_index(*module_,static_cast<::std::uint32_t>(b.operand)));
                        return address(ref.ref.storage.ptr);
                    }
                    break;
            }
            return 0u;
        }

        [[nodiscard]] bool instance_functions_witnessed() const noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            for(auto const& binding:externals_)
            {
                auto const name=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(binding.name);
                if(name.starts_with("uwvm.threaded.object."))continue;
                auto const target=binding.address;
                if((name==poll_name && target==address(&cancellation_poll)) ||
                   (name==enter_name && target==address(&cancellation_enter)) ||
                   (name=="uwvm.threaded.leave.v1" && target==address(&leave)))continue;
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                if((name==ros_poll_name && target==address(&ros_cancellation_poll)) ||
                   (name==ros_enter_name && target==address(&ros_control_probe<&cancellation_enter>)) ||
                   (name=="uwvm.threaded.leave.ros.v1" && target==address(&ros_control_leave)))continue;
#endif
                // Require both the declaration created by this emitter and a
                // process-static endpoint accepted by bind_emitted_bridges.
                // Additional external mappings require a new witness here.
                bool recognized{};
                for(auto const& bridge:emitted_bridges_)
                {
                    if(bridge.declaration->getName()!=name)continue;
                    recognized=target==linear_memory_bridge_replacement(bridge.original) ||
                        target==address(&trap) || target==address(&memory_trap) || target==address(&import_call) ||
                        (bridge.original==address(&threaded_exception_bridge_api::pending) && target==bridge.original) ||
                        target==address(&indirect_target) || target==address(&resolve_reference) ||
                        target==address(&resolve_tail_reference) || target==address(&bulk_copy) || target==address(&bulk_fill) ||
                        target==address(&memory_grow) || target==address(&memory_size) || target==address(&memory_init) ||
                        target==address(&data_drop) || target==address(&wait_notify<0u>) ||
                        target==address(&wait_notify<1u>) || target==address(&wait_notify<2u>) ||
                        target==address(&table_get<emit::runtime_wasm_i32>) || target==address(&table_get<emit::runtime_wasm_i64>) ||
                        target==address(&table_set<emit::runtime_wasm_i32>) || target==address(&table_set<emit::runtime_wasm_i64>);
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                    recognized|=target==address(&seed_ros_provenance) || target==address(&publish_ros_provenance);
#endif
                    break;
                }
                if(!recognized)
                {
                    if(::uwvm2::uwvm::io::show_verbose)
                        ::fast_io::perrln("uwvm: [verbose] WASI Threads LLVM instance template skipped for external ",
                            ::fast_io::mnp::strvw(name.begin(),name.end()));
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] bool capture_instance_bindings(instance_template& plan,::std::size_t count) const
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            auto const prefix=emit::get_llvm_runtime_module_symbol_prefix(*module_);
            plan.bindings.reserve(count);
            for(::std::size_t i{};i<count;++i)
            {
                auto const& emitted=externals_[i];
                instance_binding slot{emitted.name,instance_slot::function,emitted.address};
                auto name=::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(emitted.name);
                if(name.consume_front("uwvm.threaded.object."))
                {
                    if(!name.consume_front(emit::get_llvm_string_ref(prefix)))return false;
                    slot.operand=0u;
                    if(name=="_runtime_module")slot.slot=instance_slot::module;
                    else if(name=="_memory_0_native_memory")slot.slot=instance_slot::memory;
                    else if(name=="_memory0_begin")slot.slot=instance_slot::base;
                    else if(name=="_memory0_length")slot.slot=instance_slot::length;
                    else if(name.consume_front("_global_") && name.consume_back("_storage") && !name.getAsInteger(10,slot.operand))
                        slot.slot=instance_slot::global;
                    else if(name.consume_front("_function_") && name.consume_back("_reference_record") && !name.getAsInteger(10,slot.operand))
                        slot.slot=instance_slot::reference;
                    else return false;
                    // The name alone cannot select a binding. Its generating
                    // endpoint must have issued exactly this current-world address.
                    if(instance_address(slot)!=emitted.address)return false;
                }
                plan.bindings.push_back(::std::move(slot));
            }
            return true;
        }

        [[nodiscard]] bool bind_instance_template(instance_template const& plan)
        {
            // Resolve the complete set before publishing any mapping.
            for(auto const& b:plan.bindings)if(!instance_address(b))return false;
            for(auto const& b:plan.bindings)externals_.push_back({b.name,instance_address(b)});
            reference_dispatch_=plan.reference_dispatch;
            return true;
        }
