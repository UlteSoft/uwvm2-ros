// Private table member fragment; callable only through authenticated emitted bridges.
        using table_operations=::uwvm2::uwvm::runtime::initializer::threaded_table_operations;
        using table_status=table_operations::status;
        static bool table_result(table_status status) noexcept
        {
            if(status==table_status::ok)return true;
            fail(static_cast<::std::uint32_t>(status==table_status::bounds?llvm_jit_trap_kind::table_out_of_bounds:
                llvm_jit_trap_kind::runtime_invariant_failure));return false;
        }
        template<typename A> static ::std::uint64_t table_bits(A value) noexcept
        {return static_cast<::std::make_unsigned_t<A>>(value);}
        template<typename A> static void table_get(::std::uintptr_t module,::std::uint32_t table,A index,::std::uintptr_t output) noexcept
        {
            if(!owns_gc_module(module)||!output){table_result(table_status::invalid);return;}
            table_operations::ref value{};table_operations operations{*active_->child->world_,active_->child->module_index_};
            table_result(operations.get(table,table_bits(index),value));::std::memcpy(reinterpret_cast<void*>(output),&value,sizeof(value));
        }
        template<typename A> static void table_set(::std::uintptr_t module,::std::uint32_t table,A index,::std::uintptr_t input) noexcept
        {
            if(!owns_gc_module(module)||!input){table_result(table_status::invalid);return;}
            table_operations::ref value{};::std::memcpy(&value,reinterpret_cast<void const*>(input),sizeof(value));
            table_operations operations{*active_->child->world_,active_->child->module_index_};table_result(operations.fill(table,table_bits(index),1u,value));
        }
        template<typename A> static A table_size(::std::uintptr_t module,::std::uint32_t index) noexcept
        {
            if(!owns_gc_module(module)){table_result(table_status::invalid);return 0;}
            auto const* table=active_->child->world_->table(active_->child->module_index_,index);
            if(!table){table_result(table_status::invalid);return 0;}return static_cast<A>(table->elems.size());
        }
        template<typename A> static A table_grow(::std::uintptr_t module,::std::uint32_t table,::std::uintptr_t input,A count) noexcept
        {
            if(!owns_gc_module(module)||!input){table_result(table_status::invalid);return -1;}
            table_operations::ref value{};::std::memcpy(&value,reinterpret_cast<void const*>(input),sizeof(value));
            table_operations operations{*active_->child->world_,active_->child->module_index_};
            auto const* actual=active_->child->world_->table(active_->child->module_index_,table);
            if(!actual){table_result(table_status::invalid);return -1;}
            return static_cast<A>(::uwvm2::runtime::wasm_threads::journal_table_grow(table,table_bits(count),actual->elems.size(),
                [&]() noexcept{return operations.grow(table,table_bits(count),value);}));
        }
        template<typename A> static void table_fill(::std::uintptr_t module,::std::uint32_t table,A index,::std::uintptr_t input,A count) noexcept
        {
            if(!owns_gc_module(module)||!input){table_result(table_status::invalid);return;}
            table_operations::ref value{};::std::memcpy(&value,reinterpret_cast<void const*>(input),sizeof(value));
            table_operations operations{*active_->child->world_,active_->child->module_index_};
            table_result(operations.fill(table,table_bits(index),table_bits(count),value));
        }
        template<typename D,typename S,typename N=::std::conditional_t<(sizeof(D)==8&&sizeof(S)==8),::std::int64_t,::std::int32_t>>
        static void table_copy(::std::uintptr_t module,::std::uint32_t to,::std::uint32_t from,D dst,S src,N count) noexcept
        {
            if(!owns_gc_module(module)){table_result(table_status::invalid);return;}
            table_operations operations{*active_->child->world_,active_->child->module_index_};
            table_result(operations.copy(to,from,table_bits(dst),table_bits(src),table_bits(count)));
        }
        template<typename A> static void table_init(::std::uintptr_t module,::std::uint32_t segment,::std::uint32_t table,A dst,::std::int32_t src,::std::int32_t count) noexcept
        {
            if(!owns_gc_module(module)){table_result(table_status::invalid);return;}
            table_operations operations{*active_->child->world_,active_->child->module_index_};
            table_result(operations.init(table,segment,table_bits(dst),static_cast<::std::uint32_t>(src),static_cast<::std::uint32_t>(count)));
        }
        static void element_drop(::std::uintptr_t module,::std::uint32_t segment) noexcept
        {
            if(!owns_gc_module(module)){table_result(table_status::invalid);return;}
            table_operations operations{*active_->child->world_,active_->child->module_index_};table_result(operations.drop(segment));
        }
        static ::std::uintptr_t indirect_target(::std::uintptr_t module,::std::uintptr_t table,::std::uint64_t index,::std::uintptr_t output) noexcept
        {
            if(!owns_gc_module(module)||!output||output%alignof(target_type)){table_result(table_status::invalid);return 1u;}
            table_operations operations{*active_->child->world_,active_->child->module_index_};table_operations::ref value{};
            auto const result=operations.get(table,index,value);if(result!=table_status::ok){table_result(result);return 1u;}
            if(value.kind==::uwvm2::object::global::wasm_ref_kind::wasm_null)
            {target_type empty{};::std::memcpy(reinterpret_cast<void*>(output),&empty,sizeof(empty));return 0u;}
            return resolve_reference(module,reinterpret_cast<::std::uintptr_t>(&value),output);
        }
