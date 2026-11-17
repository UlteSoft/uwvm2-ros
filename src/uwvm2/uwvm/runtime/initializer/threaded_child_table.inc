// Private-instance table operations shared by the interpreter and LLVM bridges.
// The caller holds the actual worker lease; no ordinary-runtime table view,
// source-module pointer, or debugger authority is borrowed here.
    struct threaded_table_operations
    {
        using world_type=ThreadedChildWorld;
        using table_type=::uwvm2::uwvm::runtime::storage::local_defined_table_storage_t;
        using ref=::uwvm2::uwvm::runtime::storage::gc_reference;
        using slot=::uwvm2::uwvm::runtime::storage::local_defined_table_elem_storage_t;
        enum class status : unsigned char { ok,bounds,invalid };
        world_type const& world;::std::size_t module_index;
        [[nodiscard]] static bool fits(::std::uint64_t at,::std::uint64_t n,::std::size_t size) noexcept
        {return at<=size&&n<=size-at;}
        [[nodiscard]] bool retain(table_type const& table,ref const& value) const noexcept
        {
            return ::uwvm2::uwvm::runtime::storage::retain_runtime_table_reference(&table,value)==
                ::uwvm2::uwvm::runtime::storage::gc_object_status::ok;
        }
        [[nodiscard]] status get(::std::size_t index,::std::uint64_t at,ref& output) const noexcept
        {
            namespace st=::uwvm2::uwvm::runtime::storage;
            auto* table=world.table(module_index,index);if(!table)return status::invalid;
            if(at>=table->elems.size())return status::bounds;
            auto const value=st::runtime_table_slot_to_gc_reference(table->elems.index_unchecked(static_cast<::std::size_t>(at)));
            if(st::uwvm2_gc_retain_reference(world.module(module_index)->gc_store.get(),&value)!=st::gc_object_status::ok)
                return status::invalid;
            output=value;return status::ok;
        }
        [[nodiscard]] status fill(::std::size_t index,::std::uint64_t at,::std::uint64_t count,ref const& value) const noexcept
        {
            namespace st=::uwvm2::uwvm::runtime::storage;
            auto* table=world.table(module_index,index);if(!table)return status::invalid;
            if(!fits(at,count,table->elems.size()))return status::bounds;
            if(!count)return status::ok;
            if(!retain(*table,value))return status::invalid;
            auto const converted=st::runtime_table_slot_from_gc_reference(value);
            for(::std::size_t i{};i<count;++i)table->elems.index_unchecked(static_cast<::std::size_t>(at)+i)=converted;
            return status::ok;
        }
        [[nodiscard]] status copy(::std::size_t destination,::std::size_t source,
            ::std::uint64_t dst,::std::uint64_t src,::std::uint64_t count) const noexcept
        {
            namespace st=::uwvm2::uwvm::runtime::storage;
            auto* to=world.table(module_index,destination);auto* from=world.table(module_index,source);
            if(!to||!from)return status::invalid;
            if(!fits(dst,count,to->elems.size())||!fits(src,count,from->elems.size()))return status::bounds;
            // Preflight both complete ranges and root transfers before touching
            // any destination. Overlap follows memmove, including table aliases.
            if(!count)return status::ok;
            if(to!=from)for(::std::size_t i{};i<count;++i)
                if(!retain(*to,st::runtime_table_slot_to_gc_reference(from->elems.index_unchecked(static_cast<::std::size_t>(src)+i))))
                    return status::invalid;
            ::std::memmove(to->elems.data()+static_cast<::std::size_t>(dst),from->elems.data()+static_cast<::std::size_t>(src),
                static_cast<::std::size_t>(count)*sizeof(slot));return status::ok;
        }
        [[nodiscard]] status drop(::std::size_t index) const noexcept
        {
            auto* module=const_cast<::uwvm2::uwvm::runtime::storage::wasm_module_storage_t*>(world.module(module_index));
            if(!module||index>=module->local_defined_element_vec_storage.size())return status::invalid;
            ::uwvm2::uwvm::runtime::storage::drop_wasm_element_segment_payload(module->local_defined_element_vec_storage.index_unchecked(index).element);
            return status::ok;
        }
        [[nodiscard]] ::std::uint64_t grow(::std::size_t index,::std::uint64_t count,ref const& value) const noexcept
        {
            namespace st=::uwvm2::uwvm::runtime::storage;
            auto* table=world.table(module_index,index);if(!table)return UINT64_MAX;
            auto const old=table->elems.size();if(!count)return old;
            if(!retain(*table,value))return UINT64_MAX;
            return world.grow_table(*table,count,st::runtime_table_slot_from_gc_reference(value))?old:UINT64_MAX;
        }
        [[nodiscard]] status init(::std::size_t index,::std::size_t segment,
            ::std::uint64_t dst,::std::uint32_t src,::std::uint32_t count) const noexcept
        {
            namespace st=::uwvm2::uwvm::runtime::storage;
            using kind=::uwvm2::object::global::wasm_ref_kind;
            auto const* module=world.module(module_index);auto* table=world.table(module_index,index);
            if(!module||!table||segment>=module->local_defined_element_vec_storage.size())return status::invalid;
            auto const& record=module->local_defined_element_vec_storage.index_unchecked(segment);
            auto const element=st::load_wasm_element_segment_payload(record.element);
            auto const family=st::runtime_element_family(record,*module);
            auto size=[](auto* begin,auto* end) noexcept -> ::std::size_t{return begin?static_cast<::std::size_t>(end-begin):0u;};
            ::std::size_t length{};
            if(family==st::runtime_table_reference_family::function)
                length=element.funcref_begin?size(element.funcref_begin,element.funcref_end):size(element.funcidx_begin,element.funcidx_end);
            else if(family==st::runtime_table_reference_family::gc)length=size(element.gc_ref_begin,element.gc_ref_end);
            else length=size(element.externref_begin,element.externref_end);
            if(!fits(src,count,length)||!fits(dst,count,table->elems.size()))return status::bounds;
            auto value=[&](::std::size_t i,ref& output) noexcept -> bool
            {
                if(family==st::runtime_table_reference_family::function)
                {
                    if(element.funcref_begin){output=st::runtime_table_slot_to_gc_reference(element.funcref_begin[i]);return true;}
                    auto const f=element.funcidx_begin[i];output={};if(f==UINT32_MAX)return true;
                    auto const imports=module->imported_function_vec_storage.size();
                    if(f<imports){output.kind=kind::wasm_func_imported;output.storage.ptr=const_cast<st::imported_function_storage_t*>(&module->imported_function_vec_storage.index_unchecked(f));}
                    else if(f-imports<module->local_defined_function_vec_storage.size())
                    {output.kind=kind::wasm_func_defined;output.storage.ptr=const_cast<st::local_defined_function_storage_t*>(&module->local_defined_function_vec_storage.index_unchecked(f-imports));}
                    else return false;
                }
                else if(family==st::runtime_table_reference_family::gc)output=element.gc_ref_begin[i];
                else
                {output={};output.storage.ptr=element.externref_begin[i];if(output.storage.ptr)output.kind=family==st::runtime_table_reference_family::exception?kind::wasm_exn:kind::wasm_extern;}
                return true;
            };
            for(::std::size_t i{};i<count;++i){ref r{};if(!value(src+i,r)||!retain(*table,r))return status::invalid;}
            for(::std::size_t i{};i<count;++i){ref r{};if(!value(src+i,r))return status::invalid;
                table->elems.index_unchecked(static_cast<::std::size_t>(dst)+i)=st::runtime_table_slot_from_gc_reference(r);}
            return status::ok;
        }
    };
