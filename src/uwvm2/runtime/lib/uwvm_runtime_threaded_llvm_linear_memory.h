// Private threaded LLVM memory bridges. Native owners are compiled relocations
// authenticated against the active child, never addresses supplied by a guest.
        template<unsigned Bits,::std::size_t Bytes,bool Signed>
        [[nodiscard]] static ::std::uint64_t scalar_load64(::std::uintptr_t owner,::std::uint64_t offset,::std::uint64_t pointer) noexcept
        {
            auto* memory=actual_memory(owner);if(!memory||stopped())return 0u;
            ::std::uint64_t value{};
            auto const checked=memory_access(*memory,pointer,offset,Bytes,[&](::std::span<::std::byte> data) noexcept
            {for(::std::size_t i{};i<Bytes;++i)value|=static_cast<::std::uint64_t>(::std::to_integer<unsigned char>(
                ::std::atomic_ref<::std::byte>(data[i]).load(::std::memory_order_relaxed)))<<(8u*i);});
            if(!checked){memtag_access_failure(*memory,pointer,checked);return 0u;}
            if constexpr(Signed&&Bytes*8u<Bits){constexpr auto sign=UINT64_C(1)<<(Bytes*8u-1u);value=(value^sign)-sign;}
            if constexpr(Bits==32u)value=static_cast<::std::uint32_t>(value);
            return value;
        }
        template<::std::size_t Bytes>
        static void scalar_store64(::std::uintptr_t owner,::std::uint64_t offset,::std::uint64_t pointer,::std::uint64_t bits) noexcept
        {
            auto* memory=actual_memory(owner);if(!memory||stopped())return;
            auto const checked=memory_access(*memory,pointer,offset,Bytes,[&](::std::span<::std::byte> data) noexcept
            {for(::std::size_t i{};i<Bytes;++i)::std::atomic_ref<::std::byte>(data[i]).store(static_cast<::std::byte>(bits>>(8u*i)),::std::memory_order_relaxed);});
            if(!checked)memtag_access_failure(*memory,pointer,checked);
        }
        template<auto Op>
        static void simd_load64(::std::uintptr_t owner,::std::uint64_t offset,::std::uint64_t pointer,::std::uintptr_t old,
            ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_atomic_address_carrier_t lane,::std::uintptr_t result) noexcept
        {simd_memory<Op,false>(owner,offset,pointer,old,static_cast<::std::uint32_t>(lane),result);}
        template<auto Op>
        static void simd_store64(::std::uintptr_t owner,::std::uint64_t offset,::std::uint64_t pointer,::std::uintptr_t value,
            ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_atomic_address_carrier_t lane) noexcept
        {simd_memory<Op,false>(owner,offset,pointer,value,static_cast<::std::uint32_t>(lane),0u);}
        template<bool Wide>using bulk_address=::std::conditional_t<Wide,::std::int64_t,::std::int32_t>;
        template<bool Wide>[[nodiscard]] static ::std::uint64_t unsigned_bulk(bulk_address<Wide> value) noexcept
        {if constexpr(Wide)return static_cast<::std::uint64_t>(value);else return static_cast<::std::uint32_t>(value);}
        template<bool Destination64,bool Source64>
        static void bulk_copy_wide(::std::uintptr_t dst_owner,::std::uintptr_t src_owner,bulk_address<Destination64> destination,
            bulk_address<Source64> source,bulk_address<Destination64&&Source64> bytes) noexcept
        {memtag_copy(dst_owner,src_owner,unsigned_bulk<Destination64>(destination),unsigned_bulk<Source64>(source),unsigned_bulk<Destination64&&Source64>(bytes));}
        template<bool Wide>
        static void bulk_fill_wide(::std::uintptr_t owner,bulk_address<Wide> destination,::std::int32_t value,bulk_address<Wide> bytes) noexcept
        {memtag_fill(owner,unsigned_bulk<Wide>(destination),static_cast<::std::uint32_t>(value),unsigned_bulk<Wide>(bytes));}
        static void memory_init64(::std::uintptr_t owner,::std::uintptr_t module,::std::uint64_t index,::std::uint64_t destination,
            ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_atomic_address_carrier_t source,
            ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_atomic_address_carrier_t bytes) noexcept
        {memtag_init(owner,module,index,destination,static_cast<::std::uint32_t>(source),static_cast<::std::uint32_t>(bytes));}
        static void memory_data_init64(::std::uintptr_t owner,::std::uintptr_t data,::std::uint64_t destination,
            ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_atomic_address_carrier_t source,
            ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details::llvm_jit_atomic_address_carrier_t bytes) noexcept
        {
            if(!active_||stopped())return;
            auto const& module=*active_->child->module_;
            for(::std::size_t i{};i<module.local_defined_data_vec_storage.size();++i)
                if(data==reinterpret_cast<::std::uintptr_t>(&module.local_defined_data_vec_storage.index_unchecked(i)))
                {memory_init64(owner,reinterpret_cast<::std::uintptr_t>(&module),i,destination,source,bytes);return;}
            fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));
        }
        [[nodiscard]] static ::std::uintptr_t linear_memory_bridge_replacement(::std::uintptr_t original) noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            ::std::uintptr_t replacement{};
            if(original==address(&emit::llvm_jit_memory64_grow_bridge))replacement=address(&memory_grow64);
            else if(original==address(&emit::llvm_jit_memory64_size_bridge))replacement=address(&memory_size64);
            else if(original==address(&emit::llvm_jit_memory64_module_init_bridge))replacement=address(&memory_init64);
            else if(original==address(&emit::llvm_jit_memory64_init_bridge))replacement=address(&memory_data_init64);
            else if(original==address(&emit::llvm_jit_cross_native_memory_copy_bridge))replacement=address(&bulk_copy_wide<false,false>);
            auto copies=[&]<bool D,bool S>() noexcept
            {if(original==address(&emit::llvm_jit_memory64_copy_bridge<D,S>))replacement=address(&bulk_copy_wide<D,S>);};
            copies.template operator()<false,false>();copies.template operator()<false,true>();
            copies.template operator()<true,false>();copies.template operator()<true,true>();
            auto fills=[&]<bool W>() noexcept
            {if(original==address(&emit::llvm_jit_memory64_fill_bridge<W>))replacement=address(&bulk_fill_wide<W>);};
            fills.template operator()<false>();fills.template operator()<true>();
            auto scalar=[&]<unsigned Bits,::std::size_t Bytes>() noexcept
            {
                if(original==address(&emit::llvm_jit_memory64_scalar_load_bridge<Bits,Bytes,false>))replacement=address(&scalar_load64<Bits,Bytes,false>);
                if(original==address(&emit::llvm_jit_memory64_scalar_load_bridge<Bits,Bytes,true>))replacement=address(&scalar_load64<Bits,Bytes,true>);
                if(original==address(&emit::llvm_jit_memory64_scalar_store_bridge<Bytes>))replacement=address(&scalar_store64<Bytes>);
            };
            scalar.template operator()<32u,1u>();scalar.template operator()<32u,2u>();scalar.template operator()<32u,4u>();
            scalar.template operator()<64u,1u>();scalar.template operator()<64u,2u>();scalar.template operator()<64u,4u>();scalar.template operator()<64u,8u>();
            auto waiting=[&]<unsigned Op>() noexcept
            {if(original==address(&emit::llvm_jit_memory64_wait_notify_bridge<Op>))replacement=address(&wait_notify64<Op>);};
            waiting.template operator()<0u>();waiting.template operator()<1u>();waiting.template operator()<2u>();
            return replacement;
        }
