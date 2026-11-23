// ThreadedLlvmChild private member fragment. No ordinary compiler option/IR.
        template<typename Fn>[[nodiscard]] static ::std::uintptr_t address(Fn fn) noexcept
        {return reinterpret_cast<::std::uintptr_t>(fn);}
        [[nodiscard]] bool add_external(::llvm::Function& f,::std::uintptr_t pointer)
        {
            if(pointer==0u||!f.isDeclaration()||f.isIntrinsic()||f.getName().size()>256u||externals_.size()>=256u)return false;
            for(auto const& old:externals_)if(old.name==f.getName())return old.address==pointer;
            externals_.push_back({::uwvm2::utils::container::copy_string(f.getName()),pointer});return true;
        }
        [[nodiscard]] static bool owns_gc_module(::std::uintptr_t module) noexcept
        {
            auto* const e=active_;
            return e!=nullptr&&e->child!=nullptr&&e->lease!=nullptr&&
                e->lease->owns_current_native_thread()&&
                ::std::addressof(e->lease->world())==e->child->world_.get()&&
                ::std::addressof(e->lease->admission())==::std::addressof(e->child->world_->admission())&&
                module==reinterpret_cast<::std::uintptr_t>(e->child->module_);
        }
        [[nodiscard]] static ThreadedLlvmChild* reference_owner(::std::uintptr_t record,::std::size_t& function,bool& imported) noexcept
        {
            if(!active_||!record)return nullptr;
            auto* graph=active_->child;
            for(::std::size_t m{};m<graph->world_->module_count();++m)
            {
                auto* child=graph->compiled_module(m);if(!child)continue;
                auto member=[&](auto const& records,::std::size_t& index) noexcept
                {
                    auto const begin=reinterpret_cast<::std::uintptr_t>(records.data());
                    if(!begin||record<begin||(record-begin)%sizeof(*records.data()))return false;
                    index=(record-begin)/sizeof(*records.data());return index<records.size();
                };
                if(member(child->module_->imported_function_vec_storage,function))
                {imported=true;return child;}
                if(member(child->module_->local_defined_function_vec_storage,function))
                {function+=child->imports_;imported=false;return child;}
            }
            return nullptr;
        }
        // A raw target context names an exact function record in the retained
        // private graph. It never contains a native entry supplied by a journal
        // or by guest memory. Keep the real call-site identity when a funcref
        // invokes another module's declared builtin import.
        static void UWVM2_RUNTIME_LLVM_JIT_RAW_ENTRY_PTR_ABI reference_call(::std::uintptr_t context,
            ::std::uintptr_t result,::std::size_t result_bytes,::std::uintptr_t params,::std::size_t param_bytes) noexcept
        {
            auto* e=active_;if(!e||stopped())return;
            ::std::size_t function{};bool imported{};auto* callee=reference_owner(context,function,imported);
            if(!callee||!owns_gc_module(reinterpret_cast<::std::uintptr_t>(e->child->module_))||
               (result_bytes&&!result)||(param_bytes&&!params))
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
            auto const* type=imported?callee->module_->imported_function_vec_storage.index_unchecked(function).import_type_ptr->imports.storage.function:
                callee->module_->local_defined_function_vec_storage.index_unchecked(function-callee->imports_).function_type_ptr;
            auto const abi=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::get_runtime_wasm_call_abi_layout(*type);
            if(!abi.valid||abi.result_bytes!=result_bytes||abi.parameter_bytes!=param_bytes)
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
            if(imported)
            {
                details::scoped_llvm_wasm_host_fp_environment_restore fp{e->fp_active};
                if(!fp.ready()){fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
                auto const status=::uwvm2::runtime::wasm_threads::dispatch_import(*e->lease,*e->child->world_,
                    e->child->module_index_,e->function,function,{reinterpret_cast<::std::byte*>(result),result_bytes},
                    {reinterpret_cast<::std::byte const*>(params),param_bytes},e->cross_module||callee!=e->child,callee->module_index_);
                using s=::uwvm2::runtime::wasm_threads::host_dispatch_status;
                if(status==s::rejected||(status==s::group_stopped&&!e->lease->cancellation().stop_requested()))
                    fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));
                return;
            }
            auto* caller=e->child;auto const saved_function=e->function;auto const saved_crossing=e->cross_module;
            e->child=callee;e->cross_module|=callee!=caller;
            using entry=void(UWVM2_RUNTIME_LLVM_JIT_RAW_ENTRY_PTR_ABI*)(::std::uintptr_t,::std::uintptr_t,::std::size_t,::std::uintptr_t,::std::size_t);
            auto const raw=callee->entries_[function-callee->imports_].raw;
            if(!raw)fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));
            else reinterpret_cast<entry>(raw)(0u,result,result_bytes,params,param_bytes);
            e->child=caller;e->function=saved_function;e->cross_module=saved_crossing;
        }
        [[nodiscard]] static ::std::uintptr_t resolve_reference(::std::uintptr_t module,::std::uintptr_t input,::std::uintptr_t output) noexcept
        {
            namespace st=::uwvm2::uwvm::runtime::storage;
            using kind=::uwvm2::object::global::wasm_ref_kind;
            if(!owns_gc_module(module)||!input||!output||output%alignof(target_type))
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return 1u;}
            st::gc_reference reference{};::std::memcpy(&reference,reinterpret_cast<void const*>(input),sizeof(reference));
            if(reference.kind==kind::wasm_null)return 1u;
            ::std::size_t function{};bool imported{};
            auto* owner=reference_owner(reinterpret_cast<::std::uintptr_t>(reference.storage.ptr),function,imported);
            if(!owner||reference.kind!=(imported?kind::wasm_func_imported:kind::wasm_func_defined))
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return 1u;}
            ::std::uintptr_t record=reinterpret_cast<::std::uintptr_t>(reference.storage.ptr);
            if(imported)
            {
                ::std::size_t target_module{},target_function{};
                if(owner->world_->resolve_wasm_import(owner->module_index_,function,target_module,target_function))
                {
                    owner=owner->compiled_module(target_module);function=target_function;imported=false;
                    if(!owner||function<owner->imports_||function-owner->imports_>=owner->entries_.size())
                    {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return 1u;}
                    record=reinterpret_cast<::std::uintptr_t>(&owner->module_->local_defined_function_vec_storage.index_unchecked(function-owner->imports_));
                }
            }
            target_type target{};target.entry_address=address(&reference_call);target.context_address=record;
            target.encoded_type_id=active_->child->reference_type_ids_[owner->module_index_][function];
            // Foreign raw entry must establish its own active module. A typed
            // shortcut here would execute with the caller's memory/GC authority.
            target.typed_entry_address=0u;
            ::std::memcpy(reinterpret_cast<void*>(output),&target,sizeof(target));return 0u;
        }
        [[nodiscard]] static ::std::uintptr_t resolve_tail_reference(::std::uintptr_t module,::std::uintptr_t input) noexcept
        {
            target_type checked{};
            if(resolve_reference(module,input,reinterpret_cast<::std::uintptr_t>(&checked))!=0u)
                fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::null_reference));
            // The compiler's owning tail adapter preserves argument/result
            // storage after the outgoing Wasm frame retires. Its checked raw
            // target establishes the actual private module; returning a bare
            // foreign typed address would skip that ownership transition.
            return 0u;
        }
#include "uwvm_runtime_threaded_llvm_tables.h"
        static void gc_local_index(::std::uintptr_t module,::std::uintptr_t output) noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            if(owns_gc_module(module)) { emit::llvm_jit_gc_local_index_bridge(module,output);return; }
            ::std::uintptr_t const empty[2]{};
            if(output!=0u) { ::std::memcpy(reinterpret_cast<void*>(output),empty,sizeof(empty)); }
        }
        [[nodiscard]] static ::std::uint32_t retain_global(::std::uintptr_t target,::std::uintptr_t value) noexcept
        {
            namespace st=::uwvm2::uwvm::runtime::storage;
            auto* e=active_;
            if(!e||!owns_gc_module(reinterpret_cast<::std::uintptr_t>(e->child->module_))||!value)
                return static_cast<::std::uint32_t>(st::gc_object_status::invalid_store);
            // The emitter supplies a storage address, never a Guest selector.
            // Authenticate it against this instance before even reading it.
            for(::std::size_t m{};m<e->child->world_->module_count();++m)
                for(auto const& global:e->child->world_->module(m)->local_defined_global_vec_storage)
                    if(target==reinterpret_cast<::std::uintptr_t>(&global.global)&&
                       global.global.ref_lease_store==e->child->world_->module(m)->gc_store.get())
                        return ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_global_retain_reference_bridge(target,value);
            return static_cast<::std::uint32_t>(st::gc_object_status::invalid_store);
        }
        [[nodiscard]] static ::std::uint32_t reference_test(::std::uintptr_t module,::std::uintptr_t reference,
            ::std::int_least64_t heap,::std::uint32_t nullable) noexcept
        {
            if(!owns_gc_module(module)||!reference)return 0u;
            using h=::uwvm2::uwvm::runtime::storage::gc_type::abstract_heap_type;
            if(heap==static_cast<::std::int_least64_t>(h::exn)||heap==static_cast<::std::int_least64_t>(h::noexn))
            {
                // Escaped EH objects belong to the private engine arena, not
                // the ordinary GC store. Authenticate the complete carrier
                // against that arena before testing the exception hierarchy.
                eh_reference ref{};::std::memcpy(&ref,reinterpret_cast<void const*>(reference),sizeof(ref));
                if(ref.kind==eh_kind::wasm_null)return nullable!=0u;
                if(!active_->child->reference_token(ref))
                {exception_failure(eh_failure::invalid);return 0u;}
                return heap==static_cast<::std::int_least64_t>(h::exn);
            }
            return ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_gc_ref_test_bridge(
                module,reference,heap,nullable);
        }
        [[nodiscard]] static ::std::uintptr_t immutable_struct_cast(::std::uintptr_t module,
            ::std::uint32_t kind,::std::uintptr_t payload,::std::uint32_t type,::std::uint32_t nullable) noexcept
        {
            // This ABI returns either zero, a success sentinel, or a checked
            // immutable carrier witness. An error enum is NOT a witness.
            if(!owns_gc_module(module))return 0u;
            return ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_gc_immutable_struct_cast_bridge(
                module,kind,payload,type,nullable);
        }
        template<auto Bridge, bool PackedStatus, typename... Arguments>
        [[nodiscard]] static auto gc_scalar(::std::uintptr_t module, Arguments... arguments) noexcept
        {
            using result_type=decltype(Bridge(module,arguments...));
            namespace st=::uwvm2::uwvm::runtime::storage;
            if(!owns_gc_module(module))
            {
                if constexpr(PackedStatus)
                {
                    static_assert(sizeof(result_type)==sizeof(::std::uint64_t));
                    return static_cast<result_type>(st::gc_object_status::invalid_store)<<32u;
                }
                else { return static_cast<result_type>(st::gc_object_status::invalid_store); }
            }
            // Original checked scalar operations retain membership/type checks,
            // foreign-owner leases and mutable locks. No raw GC address escapes;
            // add_control still polls after the call before guest effects resume.
            return Bridge(module,arguments...);
        }
        static ::std::uint_least32_t gc_aggregate(::std::uintptr_t module,::std::uint_least32_t op,
            ::std::uint_least32_t first,::std::uint_least32_t second,::std::uintptr_t inputs,
            ::std::size_t count,::std::uintptr_t output) noexcept
        {
            namespace st=::uwvm2::uwvm::runtime::storage;
            auto* e=active_;
            if(!e || !e->lease || !e->lease->owns_current_native_thread() ||
               module!=reinterpret_cast<::std::uintptr_t>(e->child->module_) ||
               count>e->child->world_->admission().limits().max_value_stack_bytes/sizeof(st::gc_object_value))
            { return static_cast<::std::uint_least32_t>(st::gc_object_status::invalid_store); }
            if(!output || (count && !inputs) || count>e->child->gc_scratch_.size())
            { return static_cast<::std::uint_least32_t>(st::gc_object_status::invalid_value); }
            auto* scratch=e->child->gc_scratch_.data();
            for(::std::size_t i{};i<count;++i)
            { ::std::memcpy(scratch+i,reinterpret_cast<void const*>(inputs+i*sizeof(st::gc_object_value)),sizeof(st::gc_object_value)); }
            st::gc_object_value result{};
            auto const outcome=::uwvm2::runtime::compiler::shared::gc_aggregate(*e->child->module_,op,first,second,scratch,count,result);
            for(::std::size_t i{};i<count;++i) { scratch[i]={}; }
            if(outcome==st::gc_object_status::ok) { ::std::memcpy(reinterpret_cast<void*>(output),&result,sizeof(result)); }
            return static_cast<::std::uint_least32_t>(outcome);
        }
        [[nodiscard]] bool bind_emitted_bridges()
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            for(auto const& bridge:emitted_bridges_)
            {
                if(bridge.declaration==nullptr)return false;
                ::std::uintptr_t replacement{memtag_bridge_replacement(*bridge.declaration,bridge.original)};
                if(!replacement)replacement=linear_memory_bridge_replacement(bridge.original);
                if(bridge.original==address(&instruction_point)&&world_->admission().instruction_control())replacement=address(&instruction_point);
                else if(bridge.original==address(&emit::llvm_jit_gc_aggregate_bridge<>))replacement=address(&gc_aggregate);
                else if(bridge.original==address(&emit::llvm_jit_gc_struct_get32_bridge<false>))
                    replacement=address(&gc_scalar<&emit::llvm_jit_gc_struct_get32_bridge<false>,true,::std::uint32_t,::std::uintptr_t,::std::uint32_t>);
                else if(bridge.original==address(&emit::llvm_jit_gc_struct_get32_bridge<true>))
                    replacement=address(&gc_scalar<&emit::llvm_jit_gc_struct_get32_bridge<true>,true,::std::uint32_t,::std::uintptr_t,::std::uint32_t>);
                else if(bridge.original==address(&emit::llvm_jit_gc_array_get32_bridge<false>))
                    replacement=address(&gc_scalar<&emit::llvm_jit_gc_array_get32_bridge<false>,true,::std::uint32_t,::std::uintptr_t,::std::uint32_t>);
                else if(bridge.original==address(&emit::llvm_jit_gc_array_get32_bridge<true>))
                    replacement=address(&gc_scalar<&emit::llvm_jit_gc_array_get32_bridge<true>,true,::std::uint32_t,::std::uintptr_t,::std::uint32_t>);
                else if(bridge.original==address(&emit::llvm_jit_gc_array_length32_bridge))
                    replacement=address(&gc_scalar<&emit::llvm_jit_gc_array_length32_bridge,true,::std::uint32_t,::std::uintptr_t>);
                else if(bridge.original==address(&emit::uwvm2_llvm_jit_gc_struct_set32_wide_bridge_r1))
                    replacement=address(&gc_scalar<&emit::uwvm2_llvm_jit_gc_struct_set32_wide_bridge_r1,false,::std::uintptr_t,::std::uintptr_t,::std::uintptr_t,::std::uintptr_t>);
                else if(bridge.original==address(&emit::uwvm2_llvm_jit_gc_array_set32_bridge_r1))
                    replacement=address(&gc_scalar<&emit::uwvm2_llvm_jit_gc_array_set32_bridge_r1,false,::std::uint32_t,::std::uintptr_t,::std::uint32_t,::std::uint32_t>);
                else if(bridge.original==address(&emit::llvm_jit_gc_local_index_bridge))replacement=address(&gc_local_index);
                else if(bridge.original==address(&emit::llvm_jit_global_retain_reference_bridge))replacement=address(&retain_global);
                else if(bridge.original==address(&emit::llvm_jit_gc_ref_test_bridge))replacement=address(&reference_test);
                else if(bridge.original==address(&emit::llvm_jit_gc_immutable_struct_cast_bridge))replacement=address(&immutable_struct_cast);
                else if(bridge.original==address(&emit::llvm_jit_gc_convert_bridge))
                    replacement=address(&gc_scalar<&emit::llvm_jit_gc_convert_bridge,false,::std::uint32_t,::std::uintptr_t,::std::uintptr_t>);
                else if(bridge.original==address(&llvm_jit_runtime_trap))replacement=address(&trap);
                else if(bridge.original==address(&::uwvm2::runtime::memtag::deferred::boundary))replacement=bridge.original;
                else if(bridge.original==address(&::uwvm2::runtime::memtag::deferred::record_llvm))replacement=bridge.original;
                // Compiler-private numeric evidence and entry-pinned pending
                // flag only. Neither bridge imports Guest authority or memory.
                else if(bridge.original==address(&::uwvm2::runtime::memtag::deferred::record_llvm_at))replacement=bridge.original;
                else if(bridge.original==address(&::uwvm2::runtime::memtag::deferred::pending_flag_address))replacement=bridge.original;
                else if(bridge.original==address(&llvm_jit_memory_out_of_bounds_trap))replacement=address(&memory_trap);
                else if(bridge.original==address(&details::llvm_jit_call_raw_from_generated_wasm_abi_bridge))replacement=address(&import_call);
                else if(bridge.original==address(&details::llvm_jit_resolve_call_ref_target_abi_bridge))
                {replacement=address(&resolve_reference);reference_dispatch_=true;}
                else if(bridge.original==address(&details::llvm_jit_debug_call_indirect_target_abi_bridge))
                {replacement=address(&indirect_target);reference_dispatch_=true;}
                else if(bridge.original==address(&details::llvm_jit_resolve_call_ref_tail_target_abi_bridge))replacement=address(&resolve_tail_reference);
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                else if(bridge.original==address(&llvm_jit_seed_ros_provenance))replacement=address(&seed_ros_provenance);
                else if(bridge.original==address(&llvm_jit_publish_ros_provenance))replacement=address(&publish_ros_provenance);
#endif
                else if(bridge.original==address(&emit::llvm_jit_atomic_wait_notify_bridge<0u>))replacement=address(&wait_notify<0u>);
                else if(bridge.original==address(&emit::llvm_jit_atomic_wait_notify_bridge<1u>))replacement=address(&wait_notify<1u>);
                else if(bridge.original==address(&emit::llvm_jit_atomic_wait_notify_bridge<2u>))replacement=address(&wait_notify<2u>);
                else if(bridge.original==address(&emit::llvm_jit_memory_copy_bridge))replacement=address(&bulk_copy);
                else if(bridge.original==address(&emit::llvm_jit_memory_fill_bridge))replacement=address(&bulk_fill);
                else if(bridge.original==address(&emit::llvm_jit_memory_grow_bridge))replacement=address(&memory_grow);
                else if(bridge.original==address(&emit::llvm_jit_memory_size_bridge))replacement=address(&memory_size);
                else if(bridge.original==address(&emit::llvm_jit_memory_init_bridge))replacement=address(&memory_init);
                else if(bridge.original==address(&emit::llvm_jit_data_drop_bridge))replacement=address(&data_drop);
                else if(bridge.original==address(&emit::llvm_jit_table_get_bridge<emit::runtime_wasm_i32>))replacement=address(&table_get<emit::runtime_wasm_i32>);
                else if(bridge.original==address(&emit::llvm_jit_table_get_bridge<emit::runtime_wasm_i64>))replacement=address(&table_get<emit::runtime_wasm_i64>);
                else if(bridge.original==address(&emit::llvm_jit_table_set_bridge<emit::runtime_wasm_i32>))replacement=address(&table_set<emit::runtime_wasm_i32>);
                else if(bridge.original==address(&emit::llvm_jit_table_set_bridge<emit::runtime_wasm_i64>))replacement=address(&table_set<emit::runtime_wasm_i64>);
                else if(bridge.original==address(&emit::llvm_jit_table_fill_bridge<emit::runtime_wasm_i32>))replacement=address(&table_fill<emit::runtime_wasm_i32>);
                else if(bridge.original==address(&emit::llvm_jit_table_fill_bridge<emit::runtime_wasm_i64>))replacement=address(&table_fill<emit::runtime_wasm_i64>);
                else if(bridge.original==address(&emit::llvm_jit_table_init_bridge<emit::runtime_wasm_i32>))replacement=address(&table_init<emit::runtime_wasm_i32>);
                else if(bridge.original==address(&emit::llvm_jit_table_init_bridge<emit::runtime_wasm_i64>))replacement=address(&table_init<emit::runtime_wasm_i64>);
                else if(bridge.original==address(&emit::llvm_jit_table_size_bridge<emit::runtime_wasm_i32>))replacement=address(&table_size<emit::runtime_wasm_i32>);
                else if(bridge.original==address(&emit::llvm_jit_table_size_bridge<emit::runtime_wasm_i64>))replacement=address(&table_size<emit::runtime_wasm_i64>);
                else if(bridge.original==address(&emit::llvm_jit_table_grow_bridge<emit::runtime_wasm_i32>))replacement=address(&table_grow<emit::runtime_wasm_i32>);
                else if(bridge.original==address(&emit::llvm_jit_table_grow_bridge<emit::runtime_wasm_i64>))replacement=address(&table_grow<emit::runtime_wasm_i64>);
                else if(bridge.original==address(&emit::llvm_jit_table_copy_bridge<emit::runtime_wasm_i32,emit::runtime_wasm_i32>))replacement=address(&table_copy<emit::runtime_wasm_i32,emit::runtime_wasm_i32>);
                else if(bridge.original==address(&emit::llvm_jit_table_copy_bridge<emit::runtime_wasm_i32,emit::runtime_wasm_i64>))replacement=address(&table_copy<emit::runtime_wasm_i32,emit::runtime_wasm_i64>);
                else if(bridge.original==address(&emit::llvm_jit_table_copy_bridge<emit::runtime_wasm_i64,emit::runtime_wasm_i32>))replacement=address(&table_copy<emit::runtime_wasm_i64,emit::runtime_wasm_i32>);
                else if(bridge.original==address(&emit::llvm_jit_table_copy_bridge<emit::runtime_wasm_i64,emit::runtime_wasm_i64>))replacement=address(&table_copy<emit::runtime_wasm_i64,emit::runtime_wasm_i64>);
                else if(bridge.original==address(&emit::llvm_jit_elem_drop_bridge))replacement=address(&element_drop);
                else if(bridge.original==address(&threaded_exception_bridge_api::pending))replacement=address(&threaded_exception_bridge_api::pending);
                else if(bridge.original==address(&threaded_exception_bridge_api::publish))replacement=address(&threaded_exception_bridge_api::publish);
                else if(bridge.original==address(&threaded_exception_bridge_api::matches))replacement=address(&threaded_exception_bridge_api::matches);
                else if(bridge.original==address(&threaded_exception_bridge_api::copy))replacement=address(&threaded_exception_bridge_api::copy);
                else if(bridge.original==address(&threaded_exception_bridge_api::make_reference))replacement=address(&threaded_exception_bridge_api::make_reference);
                else if(bridge.original==address(&threaded_exception_bridge_api::consume))replacement=address(&threaded_exception_bridge_api::consume);
                else if(bridge.original==address(&threaded_exception_bridge_api::throw_reference))replacement=address(&threaded_exception_bridge_api::throw_reference);
                // Fail CLOSED, including runtime-global table/ref adapters,
                // native EH/GC, provider callbacks and memory64. No grow-silently.
                // No original address is accepted just because it was emitted.
                if(replacement==0u||!add_external(*bridge.declaration,replacement))return false;
            }
            return true;
        }
        [[nodiscard]] static ::std::size_t native_function_index(::llvm::Function const& f,
            ::uwvm2::utils::container::std_vector<entries> const& entries,::std::size_t imports) noexcept
        {
            for(::std::size_t i{};i<entries.size();++i)
                if(f.getName()==entries[i].typed_name||f.getName()==entries[i].raw_name)return imports+i;
            // Host tail adapters are compiler-created bodies whose buffers must
            // survive the outgoing Wasm frame. Never exempt them from polling
            // or native stack accounting, or admit arbitrary helper names.
            auto const* identity=f.getMetadata("uwvm.wasm.tail.adapter.owner");
            if(identity==nullptr||identity->getNumOperands()!=1u||
               !f.hasInternalLinkage()||f.getCallingConv()!=::llvm::CallingConv::Tail)return SIZE_MAX;
            auto const* value=::llvm::dyn_cast<::llvm::ValueAsMetadata>(identity->getOperand(0u).get());
            auto const* owner=value==nullptr ? nullptr : ::llvm::dyn_cast<::llvm::Function>(value->getValue());
            if(owner==nullptr||owner==::std::addressof(f)||owner->getParent()!=f.getParent()||f.use_empty())return SIZE_MAX;
            for(auto const* use:f.users())
            {
                auto const* call=::llvm::dyn_cast<::llvm::CallInst>(use);
                if(call==nullptr||!call->isMustTailCall()||call->getCalledFunction()!=::std::addressof(f)||
                   call->getFunction()!=owner)return SIZE_MAX;
            }
            for(::std::size_t i{};i<entries.size();++i)
                if(owner->getName()==entries[i].typed_name)return imports+i;
            return SIZE_MAX;
        }
        static void return_carrier(::llvm::IRBuilder<>& b,::llvm::Function& f)
        {
            // This carrier is unreachable by successful guest continuation:
            // each caller tests group stop before consuming its call result.
            if(f.getReturnType()->isVoidTy())b.CreateRetVoid();
            else b.CreateRet(::llvm::Constant::getNullValue(f.getReturnType()));
        }
        // Simplification only returns an existing SSA value or a constant. A
        // single pass over scalar integer operations keeps every original
        // block, call, memory access and trap in place for instrumentation.
        static void simplify_control_inputs(::llvm::Function& function)
        {
            ::llvm::SimplifyQuery const query{function.getDataLayout()};
            for(auto& block:function)for(auto it=block.begin();it!=block.end();)
            {
                auto& operation=*it++;
                if(!operation.getType()->isIntegerTy()||
                   !::llvm::isa<::llvm::BinaryOperator,::llvm::ICmpInst,::llvm::CastInst,::llvm::SelectInst>(operation)||
                   !::std::all_of(operation.op_begin(),operation.op_end(),
                       [](::llvm::Use const& operand){return operand->getType()->isIntegerTy();}))continue;
                if(!operation.use_empty())
                {
                    auto* const simpler=::llvm::simplifyInstruction(&operation,query.getWithInstruction(&operation));
                    if(simpler==nullptr)continue;
                    operation.replaceAllUsesWith(simpler);
                }
                operation.eraseFromParent();
            }
        }
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
        // The caller inserts the owner comparison and select ahead of every
        // original instruction except static allocas. Reuse those two pure
        // definitions for original host/Wasm calls as well as control calls.
        // Each expression contains exactly one use of the function-local input.
        static void reuse_ros_token_operation(::llvm::Value& input,::llvm::Instruction& operation)
        {
            for(auto it=input.use_begin();it!=input.use_end();)
            {
                auto* const candidate=::llvm::dyn_cast<::llvm::Instruction>((it++)->getUser());
                if(candidate==nullptr||candidate==&operation||!operation.isIdenticalTo(candidate))continue;
                candidate->replaceAllUsesWith(&operation);
                candidate->eraseFromParent();
            }
        }
        [[nodiscard]] static ::llvm::Value* prepare_control_ros_token(::llvm::Instruction*& body_begin)
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            ::llvm::IRBuilder<> builder(body_begin);
            auto* const token=emit::llvm_jit_ros_provenance(builder);
            if(auto* select=::llvm::dyn_cast_or_null<::llvm::SelectInst>(token))
            {
                auto* const comparison=::llvm::cast<::llvm::ICmpInst>(select->getCondition());
                reuse_ros_token_operation(*comparison->getOperand(0u),*comparison);
                reuse_ros_token_operation(*comparison,*select);
                // The original first instruction may itself have been a copy.
                body_begin=select->getNextNode();
            }
            return token;
        }
#endif
        static constexpr char poll_name[]{"uwvm.threaded.poll.v1"},enter_name[]{"uwvm.threaded.enter.v1"};
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
        static constexpr char ros_poll_name[]{"uwvm.threaded.poll.ros.v1"},ros_enter_name[]{"uwvm.threaded.enter.ros.v1"};
#endif
        [[nodiscard]] bool add_control(::llvm::Module& module)
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            for(auto const& global:module.globals())if(global.isThreadLocal())return false;
            auto& c=module.getContext();auto* word=::llvm::Type::getIntNTy(c,sizeof(::std::uintptr_t)*CHAR_BIT);
            auto* poll_type=::llvm::FunctionType::get(word,{word},false);
            auto* leave_type=::llvm::FunctionType::get(::llvm::Type::getVoidTy(c),{},false);
            auto declare=[&](char const* name,::llvm::FunctionType* type,::std::uintptr_t target)->::llvm::Function*
            {
                if(module.getNamedValue(name)!=nullptr)return nullptr;
                auto* f=::llvm::Function::Create(type,::llvm::Function::ExternalLinkage,name,module);
                f->addFnAttr(::llvm::Attribute::NoUnwind);f->addFnAttr(::llvm::Attribute::NoInline);
                f->addFnAttr(::llvm::Attribute::Convergent);f->addFnAttr(::llvm::Attribute::NoDuplicate);
                emit::apply_llvm_jit_host_calling_conv(*f);
                return add_external(*f,target)?f:nullptr;
            };
            auto const instruction_control=world_->admission().instruction_control();
            auto* poll_fn=declare(poll_name,poll_type,instruction_control?address(&instruction_control_poll):address(&poll));
            auto* enter_fn=declare(enter_name,poll_type,instruction_control?address(&instruction_control_enter):address(&enter));
            auto* leave_fn=declare("uwvm.threaded.leave.v1",leave_type,address(&leave));
            if(poll_fn==nullptr||enter_fn==nullptr||leave_fn==nullptr)return false;
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
            auto* ros_poll_type=::llvm::FunctionType::get(word,{word,word},false);
            auto* ros_leave_type=::llvm::FunctionType::get(::llvm::Type::getVoidTy(c),{word},false);
            auto* ros_poll_fn=declare(ros_poll_name,ros_poll_type,instruction_control?
                address(&ros_control_probe<&instruction_control_poll>):address(&ros_control_probe<&poll>));
            auto* ros_enter_fn=declare(ros_enter_name,ros_poll_type,instruction_control?
                address(&ros_control_probe<&instruction_control_enter>):address(&ros_control_probe<&enter>));
            auto* ros_leave_fn=declare("uwvm.threaded.leave.ros.v1",ros_leave_type,address(&ros_control_leave));
            if(ros_poll_fn==nullptr||ros_enter_fn==nullptr||ros_leave_fn==nullptr)return false;
#endif
            for(auto& f:module)
            {
                if(f.isDeclaration())continue;
                auto const function=native_function_index(f,entries_,imports_);if(function==SIZE_MAX)return false;
                if(f.hasPersonalityFn()||f.isVarArg())return false;
                // Keep fixed native-frame allocations in the true entry block.
                // Splitting before them turns LLVM's static allocas into dynamic
                // allocas and correctly fails the later stack witness. Hoist only
                // already-static entry allocas; no Guest load/call/effect may run
                // ahead of the genuine entry/depth/cancellation check.
                auto& entry=f.getEntryBlock();
                ::llvm::SmallVector<::llvm::AllocaInst*,16> frame_allocas;
                ::llvm::SmallVector<::llvm::AllocaInst*,16> private_locals;
                for(auto& instruction:entry)
                    if(auto* allocation=::llvm::dyn_cast<::llvm::AllocaInst>(&instruction))
                    {
                        if(!allocation->isStaticAlloca()||allocation->getAlign().value()>16u)return false;
                        // Promote only frame slots used by ordinary loads and
                        // stores. In particular, a lifetime intrinsic or a
                        // host-visible address retains its original probe site.
                        auto const simple_access=[](::llvm::User const* user) noexcept
                        {
                            if(auto const* load=::llvm::dyn_cast<::llvm::LoadInst>(user))return load->isSimple();
                            if(auto const* store=::llvm::dyn_cast<::llvm::StoreInst>(user))return store->isSimple();
                            return false;
                        };
                        if(::llvm::isAllocaPromotable(allocation)&&
                           ::std::all_of(allocation->user_begin(),allocation->user_end(),simple_access))
                            private_locals.push_back(allocation);
                        else frame_allocas.push_back(allocation);
                    }
                if(!private_locals.empty())
                {
                    ::llvm::DominatorTree dominators{f};
                    ::llvm::PromoteMemToReg(private_locals,dominators);
                }
                simplify_control_inputs(f);
                for(auto current=frame_allocas.rbegin();current!=frame_allocas.rend();++current)
                {
                    auto point=entry.getFirstInsertionPt();
                    if(point==entry.end())return false;
                    if(::std::addressof(*point)!=*current)(*current)->moveBefore(entry,point);
                }
                ::uwvm2::utils::container::std_vector<::llvm::CallInst*> calls;
                ::uwvm2::utils::container::std_vector<::llvm::BasicBlock*> blocks;
                ::uwvm2::utils::container::std_vector<::llvm::CallInst*> traps;
                for(auto& block:f)
                {
                    blocks.push_back(::std::addressof(block));
                    for(auto& i:block)
                    {
                        if(::llvm::isa<::llvm::InvokeInst,::llvm::ResumeInst,::llvm::LandingPadInst,::llvm::CatchSwitchInst,
                            ::llvm::CatchReturnInst,::llvm::CleanupReturnInst,::llvm::IndirectBrInst,::llvm::CallBrInst>(i))return false;
                        auto* call=::llvm::dyn_cast<::llvm::CallInst>(::std::addressof(i));if(call==nullptr)continue;
                        auto* target=call->getCalledFunction();
                        if(call->isInlineAsm())
                        {
                            auto* a=::llvm::cast<::llvm::InlineAsm>(call->getCalledOperand());
                            if(!a->getAsmString().empty()||a->getConstraintString()!="~{memory}")return false;
                            continue;
                        }
                        if(target!=nullptr&&target->isIntrinsic())
                        {
                            if(target->getIntrinsicID()==::llvm::Intrinsic::trap||target->getIntrinsicID()==::llvm::Intrinsic::debugtrap)return false;
                            // LLVM intrinsics lower to finite scalar/vector/memory
                            // work. External libcalls are separately checked in the
                            // actual generated object before code loading.
                        }
                        bool is_trap{};
                        for(auto const& bridge:emitted_bridges_)if(bridge.declaration==target&&
                            (bridge.original==address(&llvm_jit_runtime_trap)||bridge.original==address(&llvm_jit_memory_out_of_bounds_trap)||
                             bridge.original==address(&llvm_jit_memtag_trap)))is_trap=true;
                        if(is_trap)traps.push_back(call);else calls.push_back(call);
                    }
                }
                // Every failed check in this function returns the same carrier.
                // Create its exit after collecting the original blocks so it
                // gets one leave below and cannot acquire a poll of its own.
                auto* const stop_exit=::llvm::BasicBlock::Create(c,"threaded.stop",::std::addressof(f));
                ::llvm::IRBuilder<> stop_builder(stop_exit);
                return_carrier(stop_builder,f);

                // Record the exact trap at its original site before leaving.
                for(auto* call:traps)
                {
                    if(!call->getType()->isVoidTy())return false;
                    auto* block=call->getParent();auto it=call->getIterator();++it;
                    while(it!=block->end())
                    {
                        auto& dead=*it++;
                        if(!dead.use_empty())return false;
                        dead.eraseFromParent();
                    }
                    call->removeFnAttr(::llvm::Attribute::NoReturn);
                    ::llvm::IRBuilder<> b(block);b.CreateBr(stop_exit);
                }
                auto entry_begin=entry.getFirstInsertionPt();
                while(entry_begin!=entry.end()&&::llvm::isa<::llvm::AllocaInst>(*entry_begin))++entry_begin;
                if(entry_begin==entry.end())return false;
                auto* body_begin=::std::addressof(*entry_begin);
                auto* entry_control=enter_fn;
                auto* poll_control=poll_fn;
                auto* leave_control=leave_fn;
                ::llvm::SmallVector<::llvm::Value*,2> probe_arguments{::llvm::ConstantInt::get(word,function)};
                ::llvm::SmallVector<::llvm::Value*,1> leave_arguments;
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                if(auto* token=prepare_control_ros_token(body_begin))
                {
                    // A raw thunk has no token. Every token-bearing entry,
                    // including a tail adapter, uses the publishing ABI even
                    // when the token is zero and must fail authentication.
                    entry_control=ros_enter_fn;
                    poll_control=ros_poll_fn;
                    leave_control=ros_leave_fn;
                    probe_arguments.push_back(token);
                    leave_arguments.push_back(token);
                }
#endif
                auto control_call=[](::llvm::IRBuilder<>& builder,::llvm::Function* target,::llvm::ArrayRef<::llvm::Value*> arguments)
                {
                    return emit::apply_llvm_jit_calling_conv(builder.CreateCall(target->getFunctionType(),target,arguments),
                        emit::get_llvm_jit_host_calling_conv());
                };
                auto probe=[&](::llvm::Instruction* before,::llvm::Function* probe_fn)
                {
                    auto* block=before->getParent();
                    auto* continued=block->splitBasicBlock(before->getIterator(),"threaded.cont");
                    block->getTerminator()->eraseFromParent();
                    ::llvm::IRBuilder<> b(block);
                    auto* observed=control_call(b,probe_fn,probe_arguments);
                    observed->setTailCallKind(::llvm::CallInst::TCK_NoTail);
                    // Branch on the original native-word result. Zero is the
                    // sole continuing case; every other value takes stop_exit.
                    b.CreateSwitch(observed,stop_exit,1u)->addCase(::llvm::ConstantInt::get(word,0u),continued);
                };
                for(auto* block:blocks)
                {
                    if(block->getTerminator()->getMetadata("uwvm.gc.scalar.local.region")!=nullptr)
                    {
                        // One finite checked numeric instruction. No raw GC
                        // header or held mutation lock may cross a debug poll.
                        // Refuse calls/allocas/returns in this private region;
                        // the post-optimization cycle proof still requires a
                        // real stop check on every cycle of the whole function.
                        if(block==::std::addressof(f.getEntryBlock())||
                           !::llvm::isa<::llvm::BranchInst>(block->getTerminator()))return false;
                        for(auto const& instruction:*block)
                            if(::llvm::isa<::llvm::CallBase,::llvm::AllocaInst>(instruction))return false;
                        continue;
                    }
                    bool const entry_block=block==::std::addressof(f.getEntryBlock());
                    auto first=entry_block?body_begin->getIterator():block->getFirstInsertionPt();
                    if(first==block->end())return false;
                    probe(::std::addressof(*first),entry_block?entry_control:poll_control);
                }
                for(auto* call:calls)
                {
                    if(call->isMustTailCall())continue;
                    auto* next=call->getNextNode();if(next==nullptr)return false;
                    // Tail hint becomes ordinary call because the stop check must
                    // precede SSA use / memory effects. musttail stays untouched.
                    call->setTailCallKind(::llvm::CallInst::TCK_NoTail);probe(next,poll_control);
                }
                for(auto& block:f)if(auto* ret=::llvm::dyn_cast<::llvm::ReturnInst>(block.getTerminator()))
                {
                    auto* before=static_cast<::llvm::Instruction*>(ret);
                    auto* previous=ret->getPrevNode();auto* tail=::llvm::dyn_cast_or_null<::llvm::CallInst>(previous);
                    if(tail!=nullptr&&tail->isMustTailCall())before=tail;
                    ::llvm::IRBuilder<> b(before);control_call(b,leave_control,leave_arguments);
                }
            }
            return !::llvm::verifyModule(module);
        }
        static constexpr char control_target_section[]{".rodata.uwvm.threaded.targets"};

        // One ELF relocation binds the private cell to the same audited external
        // declaration. A function with repeated probes loads its target once.
        // Every call, its arguments, its return check and its position survive.
        static void reuse_control_target(::llvm::Function& target)
        {
            auto& module=*target.getParent();
            ::llvm::GlobalVariable* cell{};
            for(auto& function:module)
            {
                ::llvm::SmallVector<::llvm::CallInst*,8> calls;
                for(auto& block:function)for(auto& instruction:block)
                    if(auto* call=::llvm::dyn_cast<::llvm::CallInst>(&instruction);
                       call!=nullptr&&call->getCalledFunction()==&target&&call->getCallingConv()==target.getCallingConv())
                        calls.push_back(call);
                // Small call sites cost less as direct branches. AArch64 may
                // need two relocations to materialize the local cell address.
                if(calls.size()<4u)continue;
                if(cell==nullptr)
                {
                    cell=new ::llvm::GlobalVariable(module,target.getType(),true,
                        ::llvm::GlobalValue::PrivateLinkage,&target,"uwvm.threaded.control.target");
                    cell->setSection(control_target_section);
                    cell->setAlignment(::llvm::Align{alignof(void*)});
                }
                auto& entry=function.getEntryBlock();
                ::llvm::IRBuilder<> builder(&entry,entry.getFirstInsertionPt());
                auto* loaded=builder.CreateLoad(target.getType(),cell,"threaded.control.target");
                loaded->setAlignment(::llvm::Align{alignof(void*)});
                loaded->setVolatile(true);
                for(auto* call:calls)
                {
                    // These function attributes also apply at an indirect site.
                    for(auto kind:{::llvm::Attribute::NoUnwind,::llvm::Attribute::NoInline,
                                   ::llvm::Attribute::Convergent,::llvm::Attribute::NoDuplicate})
                        if(target.hasFnAttribute(kind))call->addFnAttr(kind);
                    call->setCalledOperand(loaded);
                }
            }
        }
        void reuse_control_targets(::llvm::Module& module) const
        {
            for(auto const& binding:externals_)
            {
                if(binding.address!=address(&poll)&&binding.address!=address(&instruction_control_poll)
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                   &&binding.address!=address(&publish_ros_provenance)
                   &&binding.address!=address(&ros_control_probe<&poll>)
                   &&binding.address!=address(&ros_control_probe<&instruction_control_poll>)
#endif
                )continue;
                auto* target=module.getFunction(::uwvm2::runtime::compiler::llvm_jit::fast_io_support::ref(binding.name));
                if(target!=nullptr&&target->isDeclaration())reuse_control_target(*target);
            }
        }
        // Both the cycle proof and the native stack graph inspect the final IR.
        // Only the exact function in an immutable private control cell can
        // qualify as a known target. Guest table dispatch stays indirect.
        [[nodiscard]] static ::llvm::Function const* fixed_call_target(::llvm::CallBase const& call) noexcept
        {
            if(auto const* direct=call.getCalledFunction())return direct;
            auto const* load=::llvm::dyn_cast<::llvm::LoadInst>(call.getCalledOperand());
            if(load==nullptr||!load->isVolatile())return nullptr;
            auto const* cell=::llvm::dyn_cast<::llvm::GlobalVariable>(load->getPointerOperand());
            if(cell==nullptr||cell->getParent()!=call.getModule()||!cell->hasPrivateLinkage()||
               !cell->isConstant()||!cell->hasInitializer()||cell->getSection()!=control_target_section)return nullptr;
            auto const* target=::llvm::dyn_cast<::llvm::Function>(cell->getInitializer());
            return target!=nullptr&&target->isDeclaration()&&target->getFunctionType()==call.getFunctionType()&&
                target->getCallingConv()==call.getCallingConv()?target:nullptr;
        }
        [[nodiscard]] static bool prove_stop_cycles(::llvm::Module const& module)
        {
            // After the actual optimizer: remove blocks with real poll/enter
            // calls. Kahn's algorithm must consume the remaining entire CFG,
            // including unreachable / irreducible cycles, without recursion.
            for(auto const& f:module)
            {
                if(f.isDeclaration())continue;
                ::uwvm2::utils::container::std_vector<::llvm::BasicBlock const*> nodes;
                for(auto const& block:f)
                {
                    bool has_probe{};
                    for(auto const& i:block)if(auto const* call=::llvm::dyn_cast<::llvm::CallInst>(&i))
                        if(auto const* target=fixed_call_target(*call))
                        {
                            auto const name=target->getName();
                            has_probe|=name==poll_name||name==enter_name;
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2) || defined(UWVM_IMPORT_ROSV2)
                            has_probe|=name==ros_poll_name||name==ros_enter_name;
#endif
                        }
                    if(!has_probe)nodes.push_back(&block);
                }
                ::llvm::DenseMap<::llvm::BasicBlock const*,::std::size_t> degree;
                for(auto* block:nodes)degree.try_emplace(block,0u);
                for(auto* block:nodes)for(auto* successor : ::llvm::successors(block))
                    if(auto it=degree.find(successor);it!=degree.end())++it->second;
                ::uwvm2::utils::container::std_vector<::llvm::BasicBlock const*> ready;
                for(auto const& value:degree)if(value.second==0u)ready.push_back(value.first);
                ::std::size_t consumed{};
                while(!ready.empty())
                {
                    auto* block=ready.back();ready.pop_back();++consumed;
                    for(auto* successor : ::llvm::successors(block))if(auto it=degree.find(successor);it!=degree.end())
                        if(--it->second==0u)ready.push_back(successor);
                }
                if(consumed!=nodes.size())return false;
            }
            return true;
        }
        static void bulk_copy(::std::uintptr_t a,::std::int32_t d,::std::int32_t s,::std::int32_t n) noexcept
        {bulk_copy_wide<false,false>(a,a,d,s,n);}
        static void bulk_fill(::std::uintptr_t a,::std::int32_t d,::std::int32_t v,::std::int32_t n) noexcept
        {bulk_fill_wide<false>(a,d,v,n);}

        static void data_drop(::std::uintptr_t module_address,::std::uint32_t index) noexcept
        {
            auto* e=active_;if(e==nullptr||stopped())return;
            if(module_address!=reinterpret_cast<::std::uintptr_t>(e->child->module_)||
               index>=e->child->module_->local_defined_data_vec_storage.size())
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
            // The World interface is const metadata; this exact runtime instance
            // owns mutable drop state. No parent instance or parser is selected.
            auto& module=*const_cast<module_type*>(e->child->module_);
            ::uwvm2::uwvm::runtime::storage::drop_wasm_data_segment_payload(
                module.local_defined_data_vec_storage.index_unchecked(index).data);
        }
        static void memory_init(::std::uintptr_t memory_address,::std::uintptr_t module_address,::std::uint32_t index,
            ::std::int32_t destination,::std::int32_t source,::std::int32_t bytes) noexcept
        {memtag_init(memory_address,module_address,index,static_cast<::std::uint32_t>(destination),
            static_cast<::std::uint32_t>(source),static_cast<::std::uint32_t>(bytes));}
