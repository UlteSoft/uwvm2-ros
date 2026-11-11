// ThreadedLlvmChild private MTE bridges. Every owner is reauthenticated
// against this actual ChildLease; no ordinary runtime-global trap is called.
// Pins and memory guards end before diagnostics or group cancellation.
        static void memtag_trap(::std::size_t memory_index,::std::uint64_t pointer,
            ::std::uint64_t granule,::std::uint32_t pointer_tag,::std::uint32_t allocation_tag) noexcept
        {
            if(stopped())return;
            bool owned{};
            for(auto* memory:active_->child->memories_)
                owned|=memory!=nullptr&&memory->diagnostic_owner_memory_index==memory_index;
            if(!owned||pointer_tag>255u||allocation_tag>255u)
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
            print_memtag_fault(memory_index,pointer,granule,pointer_tag,allocation_tag);
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            namespace crash=::uwvm2::runtime::diagnostic::crash;
            crash::memory_detail const evidence{memory_index,pointer,granule,pointer_tag,allocation_tag};
            crash::report(crash::category::memtag,"memory tag mismatch",2u,UINT64_MAX,UINT64_MAX,UINT64_MAX,&evidence);
#endif
            fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::memtag_violation));
        }
        static void memtag_access_failure(memory_type const& memory,::std::uint64_t pointer,
            ::uwvm2::extension::memtag::access_result const& result) noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            if(result.error==mt::access_error::tag_mismatch)
                memtag_trap(memory.diagnostic_owner_memory_index,pointer,result.mismatch_granule,
                    result.pointer_tag,result.allocation_tag);
            else fail(static_cast<::std::uint32_t>(result.error==mt::access_error::out_of_bounds?
                llvm_jit_trap_kind::memory_out_of_bounds:llvm_jit_trap_kind::runtime_invariant_failure));
        }
        [[nodiscard]] static bool memtag_u32(::std::uint64_t value) noexcept
        {
            if(value<=UINT32_MAX)return true;
            fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return false;
        }
        [[nodiscard]] static memory_type* actual_tagged_memory(::std::uintptr_t owner) noexcept
        {
            auto* e=active_;
            if(e==nullptr)return nullptr;
            if(e->lease==nullptr||e->child==nullptr||!e->lease->owns_current_native_thread()||
               ::std::addressof(e->lease->world())!=e->child->world_.get()||
               ::std::addressof(e->lease->admission())!=::std::addressof(e->child->world_->admission()))
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return nullptr;}
            auto* memory=actual_memory(owner);
            if(memory==nullptr||stopped())return nullptr;
            if(!::uwvm2::runtime::memtag::details::configured<::std::uint32_t,true>(*memory))
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return nullptr;}
            return memory;
        }
        template<unsigned ResultBits,::std::size_t Bytes,bool Signed,bool Store>
        [[nodiscard]] static ::std::uint64_t memtag_scalar(::std::uintptr_t owner,
            ::std::uint64_t offset,::std::uint64_t pointer,::std::uint64_t input) noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            auto* memory=actual_tagged_memory(owner);if(memory==nullptr||!memtag_u32(pointer))return 0u;
            auto const result=emit::llvm_jit_memtag_scalar_access<::std::uint32_t,true,ResultBits,Bytes,Signed,Store>(
                *memory,offset,pointer,input);
            if(!result.access){memtag_access_failure(*memory,pointer,result.access);return 0u;}
            return result.value;
        }
        [[nodiscard]] static ::std::uint64_t memtag_atomic(::std::uintptr_t owner,
            ::std::uint64_t offset,::std::uint64_t pointer,::std::uint64_t input,
            ::std::uint64_t expected,::std::uint64_t opcode) noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            auto* memory=actual_tagged_memory(owner);if(memory==nullptr||!memtag_u32(pointer))return 0u;
            if(opcode<0x10u||opcode>0x4eu)
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return 0u;}
            auto const result=emit::llvm_jit_memtag_atomic_access<::std::uint32_t,true>(
                *memory,offset,pointer,input,expected,opcode);
            if(!result.access){memtag_access_failure(*memory,pointer,result.access);return 0u;}
            if(result.unaligned){fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::unaligned_atomic));return 0u;}
            return result.value;
        }
        template<::uwvm2::extension::memtag::opcode Operation>
        [[nodiscard]] static ::std::uint64_t memtag_operation(::std::uintptr_t owner,
            ::std::uint64_t first,::std::uint64_t second,::std::uint64_t third,::std::uint64_t fourth) noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            auto* memory=actual_memory(owner);
            if(memory==nullptr||stopped()||!memtag_u32(first)||!memtag_u32(second)||!memtag_u32(third)||!memtag_u32(fourth))return 0u;
            constexpr auto descriptor=mt::find_instruction(static_cast<unsigned>(Operation));
            static_assert(descriptor!=nullptr&&Operation!=mt::opcode::add);
            ::std::array<::std::uint32_t,4u> arguments{static_cast<::std::uint32_t>(first),static_cast<::std::uint32_t>(second),
                static_cast<::std::uint32_t>(third),static_cast<::std::uint32_t>(fourth)};
            auto const result=::uwvm2::runtime::memtag::execute<true,::std::uint32_t,true>(*memory,Operation,
                ::std::span<::std::uint32_t const>{arguments.data(),descriptor->operand_count});
            if(!result)
            {
                if(result.operation.error==mt::operation_error::memory_access)
                    memtag_access_failure(*memory,first,result.operation.access);
                else fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));
                return 0u;
            }
            return result.operation.value;
        }
        static void memtag_fill(::std::uintptr_t owner,::std::uint64_t destination,
            ::std::uint64_t value,::std::uint64_t bytes) noexcept
        {
            auto* memory=actual_memory(owner);
            if(memory==nullptr||stopped()||!memtag_u32(destination)||!memtag_u32(value)||!memtag_u32(bytes))return;
            auto const checked=::uwvm2::runtime::memtag::with_guest_access<::std::uint32_t,true>(
                *memory,static_cast<::std::uint32_t>(destination),0u,bytes,[&](::std::span<::std::byte> data) noexcept
                {
                    ::std::size_t at{};
                    while(at<data.size()&&!stopped())
                    {
                        auto const end=at+(::std::min)(data.size()-at,::std::size_t{4096u});
                        for(;at!=end;++at)::std::atomic_ref<::std::byte>(data[at]).store(
                            static_cast<::std::byte>(value),::std::memory_order_relaxed);
                    }
                });
            if(!checked)memtag_access_failure(*memory,destination,checked);
        }
        static void memtag_copy(::std::uintptr_t destination_owner,::std::uintptr_t source_owner,
            ::std::uint64_t destination,::std::uint64_t source,::std::uint64_t bytes) noexcept
        {
            auto* dst=actual_memory(destination_owner);auto* src=actual_memory(source_owner);
            if(dst==nullptr||src==nullptr||stopped()||!memtag_u32(destination)||!memtag_u32(source)||!memtag_u32(bytes))return;
            auto const checked=::uwvm2::runtime::memtag::with_guest_copy(*dst,*src,
                static_cast<::std::uint32_t>(destination),static_cast<::std::uint32_t>(source),bytes,
                [&](::std::span<::std::byte> d,::std::span<::std::byte> s) noexcept
                {
                    // Use canonical Guest offsets for overlap, never differently
                    // tagged native integer pointers into the same owner.
                    auto const da=dst->memtag_shadow.enabled()?destination&UINT64_C(0x0fffffff):destination;
                    auto const sa=src->memtag_shadow.enabled()?source&UINT64_C(0x0fffffff):source;
                    bool const backward=dst==src&&da>sa&&da-sa<bytes;
                    ::std::size_t remaining=d.size();
                    while(remaining!=0u&&!stopped())
                    {
                        auto const count=(::std::min)(remaining,::std::size_t{4096u});
                        auto const offset=backward?remaining-count:d.size()-remaining;
                        for(::std::size_t j{};j!=count;++j)
                        {
                            auto const at=offset+(backward?count-1u-j:j);
                            auto const value=::std::atomic_ref<::std::byte>(s[at]).load(::std::memory_order_relaxed);
                            ::std::atomic_ref<::std::byte>(d[at]).store(value,::std::memory_order_relaxed);
                        }
                        remaining-=count;
                    }
                });
            if(!checked)memtag_access_failure(checked.source_failed?*src:*dst,
                checked.source_failed?source:destination,checked.access);
        }
        static void memtag_init(::std::uintptr_t owner,::std::uintptr_t module_owner,
            ::std::uint64_t index,::std::uint64_t destination,::std::uint64_t source,::std::uint64_t bytes) noexcept
        {
            auto* e=active_;auto* memory=actual_memory(owner);
            if(e==nullptr||memory==nullptr||stopped()||!memtag_u32(destination)||!memtag_u32(source)||!memtag_u32(bytes))return;
            if(module_owner!=reinterpret_cast<::std::uintptr_t>(e->child->module_)||
                index>=e->child->module_->local_defined_data_vec_storage.size())
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
            auto const payload=::uwvm2::uwvm::runtime::storage::load_wasm_data_segment_payload(
                e->child->module_->local_defined_data_vec_storage.index_unchecked(static_cast<::std::size_t>(index)).data);
            if((payload.byte_begin==nullptr)!=(payload.byte_end==nullptr))
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
            auto const size=payload.byte_begin==nullptr?0u:static_cast<::std::size_t>(payload.byte_end-payload.byte_begin);
            if(bytes>size||source>size-bytes)
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::memory_out_of_bounds));return;}
            auto const checked=::uwvm2::runtime::memtag::with_guest_access<::std::uint32_t,true>(
                *memory,static_cast<::std::uint32_t>(destination),0u,bytes,[&](::std::span<::std::byte> data) noexcept
                {
                    ::std::size_t at{};
                    while(at<data.size()&&!stopped())
                    {
                        auto const end=at+(::std::min)(data.size()-at,::std::size_t{4096u});
                        for(;at!=end;++at)::std::atomic_ref<::std::byte>(data[at]).store(
                            payload.byte_begin[static_cast<::std::size_t>(source)+at],::std::memory_order_relaxed);
                    }
                });
            if(!checked)memtag_access_failure(*memory,destination,checked);
        }
        template<unsigned Operation>
        [[nodiscard]] static ::std::uint64_t memtag_wait_notify(::std::uintptr_t owner,::std::uint64_t offset,
            ::std::uint64_t pointer,::std::uint64_t expected,::std::int64_t timeout) noexcept
        {
            static_assert(Operation<=2u);
            auto* memory=actual_memory(owner);if(memory==nullptr||stopped()||!memtag_u32(pointer))return 0u;
            auto const result=::uwvm2::runtime::memtag::wait_notify<Operation,::std::uint32_t,true>(
                *memory,static_cast<::std::uint32_t>(pointer),offset,expected,timeout);
            if(!result.access){memtag_access_failure(*memory,pointer,result.access);return 0u;}
            namespace wait=::uwvm2::runtime::wasm_threads;
            switch(result.wait.status)
            {
                case wait::wait_status::notified:case wait::wait_status::not_equal:case wait::wait_status::timed_out:
                    return Operation==0u?result.wait.notified:static_cast<unsigned>(result.wait.status);
                case wait::wait_status::cancelled:
                    if(!active_->lease->cancellation().stop_requested())fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::atomic_wait_cancelled));
                    return 0u; // The mandatory post-call poll prevents Guest result use.
                case wait::wait_status::out_of_bounds:fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::memory_out_of_bounds));break;
                case wait::wait_status::unaligned:fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::unaligned_atomic));break;
                case wait::wait_status::not_shared:fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::atomic_wait_non_shared));break;
                default:fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::atomic_wait_unavailable));break;
            }
            return 0u;
        }
        template<auto Op,bool Tagged>
        static void simd_memory(::std::uintptr_t owner,::std::uint64_t offset,::std::uint64_t pointer,
            ::std::uintptr_t vector_address,::std::uint64_t lane,::std::uintptr_t result_address) noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            using code=emit::llvm_jit_simd_code;
            constexpr bool store=Op==code::v128_store||Op==code::v128_store8_lane||
                Op==code::v128_store16_lane||Op==code::v128_store32_lane||Op==code::v128_store64_lane;
            constexpr bool lane_load=Op==code::v128_load8_lane||Op==code::v128_load16_lane||
                Op==code::v128_load32_lane||Op==code::v128_load64_lane;
            constexpr bool lane_store=store&&Op!=code::v128_store;
            constexpr auto width=emit::llvm_jit_simd_details::simd_memory_access_size<Op>();
            auto* memory=actual_memory(owner);if(memory==nullptr||stopped()||!memtag_u32(pointer))return;
            if(((store||lane_load)&&vector_address==0u)||(!store&&result_address==0u)||
               ((lane_load||lane_store)&&lane>=16u/width))
            {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
            // Native vector slots are compiler-created static frame allocas,
            // authenticated by the existing final frame/IR witness. Guest
            // operands cannot select either slot address.
            emit::runtime_wasm_v128 value{};
            if constexpr(store||lane_load)value=emit::llvm_jit_simd_read_bridge_value<emit::runtime_wasm_v128>(vector_address);
            auto access=[&](::std::span<::std::byte> data) noexcept
            {
                ::std::array<::std::byte,width> detached{};
                if constexpr(!store)for(::std::size_t i{};i!=width;++i)
                    detached[i]=::std::atomic_ref<::std::byte>(data[i]).load(::std::memory_order_relaxed);
                if constexpr(store)
                {
                    emit::llvm_jit_simd_details::eval_memory_store<Op>(detached.data(),value,static_cast<emit::llvm_jit_simd_details::u8>(lane));
                    for(::std::size_t i{};i!=width;++i)::std::atomic_ref<::std::byte>(data[i]).store(detached[i],::std::memory_order_relaxed);
                }
                else value=emit::llvm_jit_simd_details::eval_memory_load<Op>(detached.data(),value,static_cast<emit::llvm_jit_simd_details::u8>(lane));
            };
            ::uwvm2::extension::memtag::access_result checked{};
            if constexpr(Tagged)checked=::uwvm2::runtime::memtag::with_guest_access<::std::uint32_t,true>(
                *memory,static_cast<::std::uint32_t>(pointer),offset,width,access);
            else
            {
                if(memory->memtag_shadow.enabled())
                {fail(static_cast<::std::uint32_t>(llvm_jit_trap_kind::runtime_invariant_failure));return;}
                checked=::uwvm2::runtime::memtag::details::with_snapshot(*memory,[&](::std::span<::std::byte> data) noexcept
                {
                    using result_type=::uwvm2::extension::memtag::access_result;
                    if(offset>data.size()||pointer>data.size()-offset||width>data.size()-offset-pointer)
                        return result_type{.error=::uwvm2::extension::memtag::access_error::out_of_bounds};
                    access(data.subspan(static_cast<::std::size_t>(offset+pointer),width));return result_type{};
                });
            }
            if(!checked){memtag_access_failure(*memory,pointer,checked);return;}
            if constexpr(!store)emit::llvm_jit_simd_write_bridge_value(result_address,value);
        }
        template<auto Op>
        static void simd_load(::std::uintptr_t owner,::std::uint32_t offset,::std::int32_t pointer,
            ::std::uintptr_t old_value,::std::uint32_t lane,::std::uintptr_t result) noexcept
        {simd_memory<Op,false>(owner,offset,static_cast<::std::uint32_t>(pointer),old_value,lane,result);}
        template<auto Op>
        static void simd_store(::std::uintptr_t owner,::std::uint32_t offset,::std::int32_t pointer,
            ::std::uintptr_t value,::std::uint32_t lane) noexcept
        {simd_memory<Op,false>(owner,offset,static_cast<::std::uint32_t>(pointer),value,lane,0u);}
        [[nodiscard]] static bool check_memtag_shared_lease_ir(::llvm::Module const& module) noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            return emit::validate_memtag_shared_lease_ir(module,sizeof(::std::uintptr_t)*CHAR_BIT,
                emit::get_llvm_jit_host_calling_conv());
        }
        [[nodiscard]] static ::std::uintptr_t memtag_bridge_replacement(::llvm::Function const& declaration,::std::uintptr_t original) noexcept
        {
            namespace emit=::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            namespace mt=::uwvm2::extension::memtag;
            ::std::uintptr_t replacement{};
            auto scalar=[&]<unsigned Bits,::std::size_t Bytes,bool Signed,bool Store>() noexcept
            {
                if(original==address(&emit::llvm_jit_memtag_scalar_bridge<::std::uint32_t,true,Bits,Bytes,Signed,Store,false>)&&
                   emit::memtag_shared_lease_declaration(declaration,emit::memtag_shared_access_family::scalar,
                       sizeof(::std::uintptr_t)*CHAR_BIT,emit::get_llvm_jit_host_calling_conv()))
                    replacement=address(&memtag_scalar<Bits,Bytes,Signed,Store>);
            };
            auto width=[&]<unsigned Bits,::std::size_t Bytes>() noexcept
            {
                scalar.template operator()<Bits,Bytes,false,false>();
                scalar.template operator()<Bits,Bytes,true,false>();
                scalar.template operator()<Bits,Bytes,false,true>();
            };
            width.template operator()<32u,1u>();width.template operator()<32u,2u>();width.template operator()<32u,4u>();
            width.template operator()<64u,1u>();width.template operator()<64u,2u>();width.template operator()<64u,4u>();width.template operator()<64u,8u>();
            if(original==address(&emit::llvm_jit_memtag_atomic_bridge<::std::uint32_t,true,false>)&&
               emit::memtag_shared_lease_declaration(declaration,emit::memtag_shared_access_family::atomic,
                   sizeof(::std::uintptr_t)*CHAR_BIT,emit::get_llvm_jit_host_calling_conv()))
                replacement=address(&memtag_atomic);
            auto operation=[&]<mt::opcode Op>() noexcept
            {
                if(original==address(&emit::llvm_jit_memtag_operation_bridge<Op,::std::uint32_t,true,false>))
                    replacement=address(&memtag_operation<Op>);
            };
            operation.template operator()<mt::opcode::status>();
            operation.template operator()<mt::opcode::extract>();
            operation.template operator()<mt::opcode::insert>();
            operation.template operator()<mt::opcode::tagbits>();
            operation.template operator()<mt::opcode::startbit>();
            operation.template operator()<mt::opcode::copy>();
            operation.template operator()<mt::opcode::sub>();
            operation.template operator()<mt::opcode::load>();
            operation.template operator()<mt::opcode::untag>();
            operation.template operator()<mt::opcode::untagstore>();
            operation.template operator()<mt::opcode::untagstorez>();
            operation.template operator()<mt::opcode::store>();
            operation.template operator()<mt::opcode::storez>();
            operation.template operator()<mt::opcode::random>();
            operation.template operator()<mt::opcode::randomstore>();
            operation.template operator()<mt::opcode::randomstorez>();
            operation.template operator()<mt::opcode::hint>();
            operation.template operator()<mt::opcode::hintstore>();
            operation.template operator()<mt::opcode::hintstorez>();

            using code=emit::llvm_jit_simd_code;
            auto simd=[&]<code Op>() noexcept
            {
                constexpr bool store=Op==code::v128_store||Op==code::v128_store8_lane||
                    Op==code::v128_store16_lane||Op==code::v128_store32_lane||Op==code::v128_store64_lane;
                if(original==address(&emit::llvm_jit_memtag_simd_bridge<Op,::std::uint32_t,true,false>))
                    replacement=address(&simd_memory<Op,true>);
                if constexpr(store)
                {if(original==address(&emit::llvm_jit_simd_memory_store_bridge<Op>))replacement=address(&simd_store<Op>);}
                else
                {if(original==address(&emit::llvm_jit_simd_memory_load_bridge<Op>))replacement=address(&simd_load<Op>);}
            };
            simd.template operator()<code::v128_load>();
            simd.template operator()<code::v128_load8x8_s>();
            simd.template operator()<code::v128_load8x8_u>();
            simd.template operator()<code::v128_load16x4_s>();
            simd.template operator()<code::v128_load16x4_u>();
            simd.template operator()<code::v128_load32x2_s>();
            simd.template operator()<code::v128_load32x2_u>();
            simd.template operator()<code::v128_load8_splat>();
            simd.template operator()<code::v128_load16_splat>();
            simd.template operator()<code::v128_load32_splat>();
            simd.template operator()<code::v128_load64_splat>();
            simd.template operator()<code::v128_store>();
            simd.template operator()<code::v128_load8_lane>();
            simd.template operator()<code::v128_load16_lane>();
            simd.template operator()<code::v128_load32_lane>();
            simd.template operator()<code::v128_load64_lane>();
            simd.template operator()<code::v128_store8_lane>();
            simd.template operator()<code::v128_store16_lane>();
            simd.template operator()<code::v128_store32_lane>();
            simd.template operator()<code::v128_store64_lane>();
            simd.template operator()<code::v128_load32_zero>();
            simd.template operator()<code::v128_load64_zero>();
            if(original==address(&llvm_jit_memtag_trap))replacement=address(&memtag_trap);
            else if(original==address(&emit::llvm_jit_memtag_fill_bridge<::std::uint32_t,true,false>))replacement=address(&memtag_fill);
            else if(original==address(&emit::llvm_jit_memtag_copy_bridge<::std::uint32_t,::std::uint32_t,false>))replacement=address(&memtag_copy);
            else if(original==address(&emit::llvm_jit_memtag_init_bridge<::std::uint32_t,true,false>))replacement=address(&memtag_init);
            else if(original==address(&emit::llvm_jit_memtag_wait_notify_bridge<0u,::std::uint32_t,true,false>))replacement=address(&memtag_wait_notify<0u>);
            else if(original==address(&emit::llvm_jit_memtag_wait_notify_bridge<1u,::std::uint32_t,true,false>))replacement=address(&memtag_wait_notify<1u>);
            else if(original==address(&emit::llvm_jit_memtag_wait_notify_bridge<2u,::std::uint32_t,true,false>))replacement=address(&memtag_wait_notify<2u>);
            return replacement;
        }
