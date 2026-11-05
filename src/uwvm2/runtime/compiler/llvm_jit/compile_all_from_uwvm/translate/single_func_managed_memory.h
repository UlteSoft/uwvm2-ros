// Same scalar ABI as the original memory helpers. All checked operations return
// out of their native snapshot/operation guards BEFORE a Guest trap can unwind.
template<bool Destination64, bool Source64>
inline void llvm_jit_managed_memory_copy_width(::std::uintptr_t destination_owner, ::std::uintptr_t source_owner,
    llvm_jit_bulk_address_t<Destination64> destination, llvm_jit_bulk_address_t<Source64> source, llvm_jit_bulk_address_t<Destination64 && Source64> length) UWVM_THROWS
{
    auto* dst{reinterpret_cast<runtime_native_memory_t*>(destination_owner)};
    auto* src{reinterpret_cast<runtime_native_memory_t*>(source_owner)};
    if(!dst || !src) { ::fast_io::fast_terminate(); }
    auto transfer{[&](::std::byte* db, ::std::size_t ds, ::std::byte* sb, ::std::size_t ss) noexcept
    { return ::uwvm2::runtime::compiler::shared::wasm_memory64::copy(db, ds, sb, ss,
        llvm_jit_bulk_unsigned(destination), llvm_jit_bulk_unsigned(source), llvm_jit_bulk_unsigned(length)) ==
        ::uwvm2::runtime::compiler::shared::wasm_memory64::bulk_error::none; }};
    bool ok{};
    if(dst == src) { ok = llvm_jit_with_bulk_memory_snapshot(*dst, [&](auto* b, auto n) noexcept { return transfer(b,n,b,n); }); }
    else
    {
        bool const destination_first{destination_owner < source_owner};
        auto& first{destination_first ? *dst : *src}; auto& second{destination_first ? *src : *dst};
        ok = llvm_jit_with_bulk_memory_snapshot(first, [&](auto* a, auto an) noexcept
        { return llvm_jit_with_bulk_memory_snapshot(second, [&](auto* b, auto bn) noexcept
            { return destination_first ? transfer(a,an,b,bn) : transfer(b,bn,a,an); }); });
    }
    if(!ok) { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(::uwvm2::runtime::lib::llvm_jit_trap_kind::memory_out_of_bounds, 0u, 0u); }
}
inline void llvm_jit_managed_memory_copy(::std::uintptr_t dst, ::std::uintptr_t src, runtime_wasm_i32 d, runtime_wasm_i32 s, runtime_wasm_i32 n) UWVM_THROWS
{ llvm_jit_managed_memory_copy_width<false,false>(dst,src,d,s,n); }
inline void llvm_jit_managed_memory_copy_one(::std::uintptr_t owner, runtime_wasm_i32 dst, runtime_wasm_i32 src, runtime_wasm_i32 length) UWVM_THROWS
{ llvm_jit_managed_memory_copy(owner,owner,dst,src,length); }
template<bool Wide>
inline void llvm_jit_managed_memory_fill_width(::std::uintptr_t owner, llvm_jit_bulk_address_t<Wide> destination, runtime_wasm_i32 value, llvm_jit_bulk_address_t<Wide> length) UWVM_THROWS
{
    auto* memory{reinterpret_cast<runtime_native_memory_t*>(owner)};
    if(!memory) { ::fast_io::fast_terminate(); }
    bool const ok{llvm_jit_with_bulk_memory_snapshot(*memory, [&](auto* b, auto n) noexcept
    { return ::uwvm2::runtime::compiler::shared::wasm_memory64::fill(b,n,llvm_jit_bulk_unsigned(destination),
        llvm_jit_wasm_i32_bits_to_u32(value),llvm_jit_bulk_unsigned(length)) ==
        ::uwvm2::runtime::compiler::shared::wasm_memory64::bulk_error::none; })};
    if(!ok) { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(::uwvm2::runtime::lib::llvm_jit_trap_kind::memory_out_of_bounds, 0u, 0u); }
}
inline void llvm_jit_managed_memory_fill(::std::uintptr_t owner, runtime_wasm_i32 d, runtime_wasm_i32 v, runtime_wasm_i32 n) UWVM_THROWS
{ llvm_jit_managed_memory_fill_width<false>(owner,d,v,n); }
inline void llvm_jit_managed_memory_init(::std::uintptr_t owner, ::std::uintptr_t module_owner, runtime_wasm_u32 data_index,
    runtime_wasm_i32 destination, runtime_wasm_i32 source, runtime_wasm_i32 length) UWVM_THROWS
{
    auto* memory{reinterpret_cast<runtime_native_memory_t*>(owner)};
    auto* module{reinterpret_cast<runtime_module_storage_t*>(module_owner)};
    if(!memory || !module || static_cast<::std::size_t>(data_index) >= module->local_defined_data_vec_storage.size()) { ::fast_io::fast_terminate(); }
    auto const payload{::uwvm2::uwvm::runtime::storage::load_wasm_data_segment_payload(
        module->local_defined_data_vec_storage.index_unchecked(static_cast<::std::size_t>(data_index)).data)};
    if((payload.byte_begin == nullptr) != (payload.byte_end == nullptr)) { ::fast_io::fast_terminate(); }
    auto const size{payload.byte_begin == nullptr ? 0uz : static_cast<::std::size_t>(payload.byte_end-payload.byte_begin)};
    bool const ok{llvm_jit_with_bulk_memory_snapshot(*memory, [&](auto* b, auto n) noexcept
    { return ::uwvm2::runtime::compiler::shared::wasm_memory64::copy(b,n,payload.byte_begin,size,
        llvm_jit_bulk_unsigned(destination),llvm_jit_bulk_unsigned(source),llvm_jit_bulk_unsigned(length)) ==
        ::uwvm2::runtime::compiler::shared::wasm_memory64::bulk_error::none; })};
    if(!ok) { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(::uwvm2::runtime::lib::llvm_jit_trap_kind::memory_out_of_bounds, 0u, 0u); }
}
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory_copy_bridge> { static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_copy_one}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_cross_native_memory_copy_bridge> { static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_copy}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory_fill_bridge> { static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_fill}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory_init_bridge> { static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_init}; };

// Runtime memories cannot shrink during an admitted Guest call. Validate with
// the actual snapshot, release its guard, then use the original pinned atomic
// implementation. No effect precedes this check and growth only enlarges bounds.
template<::std::size_t Bytes, bool Address64>
inline void llvm_jit_managed_atomic_check(::std::uintptr_t owner,
    llvm_jit_typed_atomic_address_t<Address64> fixed, llvm_jit_typed_atomic_address_t<Address64> address) UWVM_THROWS
{
    auto const effective{[&]() noexcept -> llvm_jit_wasm32_effective_offset_t
    { if constexpr(Address64) { auto low{address+fixed}; return {low,low<address}; }
      else { return llvm_jit_compute_wasm32_effective_offset(address,fixed); } }()};
    if((effective.offset & (Bytes-1u)) != 0u)
    { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(::uwvm2::runtime::lib::llvm_jit_trap_kind::unaligned_atomic,0u,0u); }
    auto* memory{reinterpret_cast<runtime_native_memory_t*>(owner)};
    if(!memory) { ::fast_io::fast_terminate(); }
    ::std::size_t length{};
    bool const valid{llvm_jit_try_checked_memory_access(*memory,effective,Bytes,length,[](auto*,auto) noexcept {})};
    if(!valid)
    { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(::uwvm2::runtime::lib::llvm_jit_trap_kind::memory_out_of_bounds,0u,0u); }
}
template<typename Value, ::std::size_t Bytes, bool Wide>
inline llvm_jit_atomic_value_carrier_t<Value> llvm_jit_managed_atomic_load(::std::uintptr_t owner,
    llvm_jit_typed_atomic_address_t<Wide> fixed, llvm_jit_typed_atomic_address_t<Wide> address) UWVM_THROWS
{ llvm_jit_managed_atomic_check<Bytes,Wide>(owner,fixed,address); return llvm_jit_atomic_load_bridge<Value,Bytes,Wide>(owner,fixed,address); }
template<typename Value, ::std::size_t Bytes, bool Wide>
inline void llvm_jit_managed_atomic_store(::std::uintptr_t owner,
    llvm_jit_typed_atomic_address_t<Wide> fixed, llvm_jit_typed_atomic_address_t<Wide> address, llvm_jit_atomic_value_carrier_t<Value> value) UWVM_THROWS
{ llvm_jit_managed_atomic_check<Bytes,Wide>(owner,fixed,address); llvm_jit_atomic_store_bridge<Value,Bytes,Wide>(owner,fixed,address,value); }
template<::uwvm2::runtime::compiler::shared::wasm_threads::atomic_rmw_operation Operation, typename Value, ::std::size_t Bytes, bool Wide>
inline llvm_jit_atomic_value_carrier_t<Value> llvm_jit_managed_atomic_rmw(::std::uintptr_t owner,
    llvm_jit_typed_atomic_address_t<Wide> fixed, llvm_jit_typed_atomic_address_t<Wide> address,
    llvm_jit_atomic_value_carrier_t<Value> value, llvm_jit_atomic_value_carrier_t<Value> expected) UWVM_THROWS
{ llvm_jit_managed_atomic_check<Bytes,Wide>(owner,fixed,address); return llvm_jit_atomic_rmw_bridge<Operation,Value,Bytes,Wide>(owner,fixed,address,value,expected); }
#define UWVM_MANAGED_ATOMIC_SIMPLE(Type, Bytes, Wide) \
template<> struct llvm_jit_managed_memory_target<llvm_jit_atomic_load_bridge<Type,Bytes,Wide>> \
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_atomic_load<Type,Bytes,Wide>}; }; \
template<> struct llvm_jit_managed_memory_target<llvm_jit_atomic_store_bridge<Type,Bytes,Wide>> \
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_atomic_store<Type,Bytes,Wide>}; };
#define UWVM_MANAGED_ATOMIC_RMW(Op, Type, Bytes, Wide) \
template<> struct llvm_jit_managed_memory_target<llvm_jit_atomic_rmw_bridge<::uwvm2::runtime::compiler::shared::wasm_threads::atomic_rmw_operation::Op,Type,Bytes,Wide>> \
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_atomic_rmw<::uwvm2::runtime::compiler::shared::wasm_threads::atomic_rmw_operation::Op,Type,Bytes,Wide>}; };
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i32,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i32,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i32,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i32,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i32,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i32,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i32,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i32,1uz,false)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i32,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i32,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i32,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i32,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i32,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i32,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i32,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i32,1uz,true)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i32,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i32,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i32,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i32,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i32,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i32,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i32,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i32,2uz,false)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i32,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i32,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i32,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i32,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i32,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i32,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i32,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i32,2uz,true)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i32,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i32,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i32,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i32,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i32,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i32,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i32,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i32,4uz,false)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i32,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i32,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i32,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i32,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i32,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i32,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i32,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i32,4uz,true)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i64,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i64,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i64,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i64,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i64,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i64,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i64,1uz,false)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i64,1uz,false)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i64,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i64,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i64,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i64,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i64,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i64,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i64,1uz,true)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i64,1uz,true)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i64,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i64,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i64,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i64,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i64,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i64,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i64,2uz,false)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i64,2uz,false)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i64,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i64,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i64,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i64,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i64,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i64,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i64,2uz,true)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i64,2uz,true)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i64,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i64,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i64,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i64,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i64,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i64,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i64,4uz,false)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i64,4uz,false)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i64,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i64,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i64,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i64,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i64,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i64,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i64,4uz,true)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i64,4uz,true)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i64,8uz,false)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i64,8uz,false)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i64,8uz,false)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i64,8uz,false)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i64,8uz,false)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i64,8uz,false)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i64,8uz,false)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i64,8uz,false)
UWVM_MANAGED_ATOMIC_SIMPLE(runtime_wasm_i64,8uz,true)
UWVM_MANAGED_ATOMIC_RMW(add,runtime_wasm_i64,8uz,true)
UWVM_MANAGED_ATOMIC_RMW(sub,runtime_wasm_i64,8uz,true)
UWVM_MANAGED_ATOMIC_RMW(and_,runtime_wasm_i64,8uz,true)
UWVM_MANAGED_ATOMIC_RMW(or_,runtime_wasm_i64,8uz,true)
UWVM_MANAGED_ATOMIC_RMW(xor_,runtime_wasm_i64,8uz,true)
UWVM_MANAGED_ATOMIC_RMW(exchange,runtime_wasm_i64,8uz,true)
UWVM_MANAGED_ATOMIC_RMW(compare_exchange,runtime_wasm_i64,8uz,true)
#undef UWVM_MANAGED_ATOMIC_SIMPLE
#undef UWVM_MANAGED_ATOMIC_RMW

inline void llvm_jit_managed_memory64_init(::std::uintptr_t owner, ::std::uintptr_t data_owner,
    ::std::uint64_t destination, llvm_jit_atomic_address_carrier_t source, llvm_jit_atomic_address_carrier_t length) UWVM_THROWS
{
    auto* memory{reinterpret_cast<runtime_native_memory_t*>(owner)};
    auto* data{reinterpret_cast<::uwvm2::uwvm::runtime::storage::local_defined_data_storage_t*>(data_owner)};
    if(!memory || !data) { ::fast_io::fast_terminate(); }
    auto const payload{::uwvm2::uwvm::runtime::storage::load_wasm_data_segment_payload(data->data)};
    if((payload.byte_begin == nullptr) != (payload.byte_end == nullptr)) { ::fast_io::fast_terminate(); }
    auto const size{payload.byte_begin == nullptr ? 0uz : static_cast<::std::size_t>(payload.byte_end-payload.byte_begin)};
    bool const ok{llvm_jit_with_bulk_memory_snapshot(*memory, [&](auto* b, auto n) noexcept
    { return ::uwvm2::runtime::compiler::shared::wasm_memory64::copy(b,n,payload.byte_begin,size,
        destination,static_cast<::std::uint32_t>(source),static_cast<::std::uint32_t>(length)) ==
        ::uwvm2::runtime::compiler::shared::wasm_memory64::bulk_error::none; })};
    if(!ok) { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(::uwvm2::runtime::lib::llvm_jit_trap_kind::memory_out_of_bounds,0u,0u); }
}
inline void llvm_jit_managed_memory64_module_init(::std::uintptr_t owner, ::std::uintptr_t module_owner,
    ::std::uint64_t index, ::std::uint64_t destination, llvm_jit_atomic_address_carrier_t source, llvm_jit_atomic_address_carrier_t length) UWVM_THROWS
{
    auto* module{reinterpret_cast<runtime_module_storage_t*>(module_owner)};
    if(!module || index >= module->local_defined_data_vec_storage.size()) { ::fast_io::fast_terminate(); }
    auto* data{::std::addressof(module->local_defined_data_vec_storage.index_unchecked(static_cast<::std::size_t>(index)))};
    llvm_jit_managed_memory64_init(owner,reinterpret_cast<::std::uintptr_t>(data),destination,source,length);
}
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_copy_bridge<false,false>>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_copy_width<false,false>}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_copy_bridge<false,true>>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_copy_width<false,true>}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_copy_bridge<true,false>>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_copy_width<true,false>}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_copy_bridge<true,true>>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_copy_width<true,true>}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_fill_bridge<false>>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_fill_width<false>}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_fill_bridge<true>>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory_fill_width<true>}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_init_bridge>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory64_init}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_module_init_bridge>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_managed_memory64_module_init}; };

// Notify uses the same bounds/alignment result path as wait, but does not
// suspend. Its selected managed variant may report a Guest trap after returning
// from the wait registry's internal guards.
template<> struct llvm_jit_managed_memory_target<llvm_jit_atomic_wait_notify_bridge<0u, false>>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_atomic_wait_notify_bridge<0u, true>}; };
template<> struct llvm_jit_managed_memory_target<llvm_jit_memory64_wait_notify_bridge<0u, false>>
{ static constexpr bool selected{true}; static constexpr auto value{llvm_jit_memory64_wait_notify_bridge<0u, true>}; };
