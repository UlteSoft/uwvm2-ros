// Licensed under the APL-2.0 License (see LICENSE file).
// Included inside the JIT details namespace after checked-memory primitives.
// All host owner addresses below are authenticated compiler relocations, never
// guest-provided authority. Native pins end before any diagnostic or unwind.
#include "memtag_shared_lease_ir.h"

template<bool ManagedTraps>
[[noreturn]] inline void llvm_jit_memtag_access_failure(runtime_native_memory_t const& memory,
    ::std::uint64_t raw,::std::uint64_t offset,::std::uint64_t bytes,
    ::uwvm2::extension::memtag::access_result const& failure) noexcept(!ManagedTraps)
{
    namespace mt=::uwvm2::extension::memtag;
    if(failure.error==mt::access_error::tag_mismatch)
    {
        if constexpr(ManagedTraps)
        { ::uwvm2::runtime::lib::llvm_jit_managed_memtag_trap(memory.diagnostic_owner_memory_index,
            raw,failure.mismatch_granule,failure.pointer_tag,failure.allocation_tag); }
        else
        { ::uwvm2::runtime::lib::llvm_jit_memtag_trap(memory.diagnostic_owner_memory_index,
            raw,failure.mismatch_granule,failure.pointer_tag,failure.allocation_tag); }
    }
    else if(failure.error==mt::access_error::out_of_bounds)
    {
        auto const length{::uwvm2::runtime::memtag::details::with_snapshot(memory,
            [](::std::span<::std::byte> data) noexcept { return data.size(); })};
        auto const size{bytes>SIZE_MAX ? SIZE_MAX : static_cast<::std::size_t>(bytes)};
        if constexpr(ManagedTraps)
        { ::uwvm2::runtime::lib::llvm_jit_managed_memory_trap(memory.diagnostic_owner_memory_index,
            offset,failure.address,0u,length,size,0u,0u); }
        else
        { ::uwvm2::runtime::lib::llvm_jit_memory_out_of_bounds_trap(memory.diagnostic_owner_memory_index,
            offset,failure.address,0u,length,size,0u,0u); }
    }
    if constexpr(ManagedTraps)
    { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(
        ::uwvm2::runtime::lib::llvm_jit_trap_kind::runtime_invariant_failure,0u,0u); }
    else
    { ::uwvm2::runtime::lib::llvm_jit_runtime_trap(
        ::uwvm2::runtime::lib::llvm_jit_trap_kind::runtime_invariant_failure,0u,0u); }
    ::fast_io::fast_terminate();
}

template<::uwvm2::extension::memtag::opcode Operation,typename Address,bool Shared,bool ManagedTraps=false>
[[nodiscard]] inline ::std::uint64_t llvm_jit_memtag_operation_bridge(::std::uintptr_t owner,
    ::std::uint64_t first,::std::uint64_t second,::std::uint64_t third,::std::uint64_t fourth) noexcept(!ManagedTraps)
{
    namespace mt=::uwvm2::extension::memtag;
    auto const* memory{reinterpret_cast<runtime_native_memory_t const*>(owner)};
    if(memory==nullptr) { ::fast_io::fast_terminate(); }
    constexpr auto descriptor{mt::find_instruction(static_cast<unsigned>(Operation))};
    static_assert(descriptor!=nullptr && Operation!=mt::opcode::add);
    // The emitter validates the selected address type then zero extends i32
    // values into these exact i64 ABI carriers on every host architecture.
    ::std::array<Address,4> arguments{static_cast<Address>(first),static_cast<Address>(second),
                                    static_cast<Address>(third),static_cast<Address>(fourth)};
    auto const result{::uwvm2::runtime::memtag::execute<true,Address,Shared>(*memory,Operation,
        ::std::span<Address const>{arguments.data(),descriptor->operand_count})};
    if(!result)
    {
        if(result.operation.error==mt::operation_error::memory_access)
        { llvm_jit_memtag_access_failure<ManagedTraps>(*memory,first,0u,descriptor->operand_count>1u ? second : 1u,result.operation.access); }
        if constexpr(ManagedTraps)
        { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(
            ::uwvm2::runtime::lib::llvm_jit_trap_kind::runtime_invariant_failure,0u,0u); }
        else
        { ::uwvm2::runtime::lib::llvm_jit_runtime_trap(
            ::uwvm2::runtime::lib::llvm_jit_trap_kind::runtime_invariant_failure,0u,0u); }
        ::fast_io::fast_terminate();
    }
    return result.operation.value;
}

template<typename Address,bool Shared,bool ManagedTraps=false>
inline void llvm_jit_memtag_fill_bridge(::std::uintptr_t owner,::std::uint64_t destination,
    ::std::uint64_t value,::std::uint64_t bytes) noexcept(!ManagedTraps)
{
    auto const* memory{reinterpret_cast<runtime_native_memory_t const*>(owner)};
    if(memory==nullptr) { ::fast_io::fast_terminate(); }
    auto const byte{static_cast<::std::byte>(value)};
    auto const checked{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(*memory,static_cast<Address>(destination),0u,bytes,
        [&](::std::span<::std::byte> data) noexcept
        {
            if constexpr(Shared)
            { for(auto& cell:data) { ::std::atomic_ref<::std::byte>(cell).store(byte,::std::memory_order_relaxed); } }
            else if(!data.empty()) { ::std::memset(data.data(),static_cast<int>(byte),data.size()); }
        })};
    if(!checked) { llvm_jit_memtag_access_failure<ManagedTraps>(*memory,destination,0u,bytes,checked); }
}

template<typename Destination,typename Source,bool ManagedTraps=false>
inline void llvm_jit_memtag_copy_bridge(::std::uintptr_t destination_owner,::std::uintptr_t source_owner,
    ::std::uint64_t destination,::std::uint64_t source,::std::uint64_t bytes) noexcept(!ManagedTraps)
{
    auto const* destination_memory{reinterpret_cast<runtime_native_memory_t const*>(destination_owner)};
    auto const* source_memory{reinterpret_cast<runtime_native_memory_t const*>(source_owner)};
    if(destination_memory==nullptr || source_memory==nullptr) { ::fast_io::fast_terminate(); }
    auto const checked{::uwvm2::runtime::memtag::with_guest_copy(*destination_memory,*source_memory,
        static_cast<Destination>(destination),static_cast<Source>(source),bytes,
        [&](::std::span<::std::byte> destination_data,::std::span<::std::byte> source_data) noexcept
        {
            if(destination_data.empty()) { return; }
            bool const destination_shared{::uwvm2::runtime::wasm_threads::is_shared(*destination_memory)};
            bool const source_shared{::uwvm2::runtime::wasm_threads::is_shared(*source_memory)};
            if(!destination_shared && !source_shared)
            { ::std::memmove(destination_data.data(),source_data.data(),destination_data.size());return; }
            auto transfer{[&](::std::size_t index) noexcept
            {
                auto const byte{source_shared ? ::std::atomic_ref<::std::byte>(source_data[index]).load(::std::memory_order_relaxed) :
                                                source_data[index]};
                if(destination_shared) { ::std::atomic_ref<::std::byte>(destination_data[index]).store(byte,::std::memory_order_relaxed); }
                else { destination_data[index]=byte; }
            }};
            auto const dst{reinterpret_cast<::std::uintptr_t>(destination_data.data())};
            auto const src{reinterpret_cast<::std::uintptr_t>(source_data.data())};
            if(dst>src && dst-src<destination_data.size())
            { for(auto index{destination_data.size()};index!=0u;) { transfer(--index); } }
            else { for(::std::size_t index{};index!=destination_data.size();++index) { transfer(index); } }
        })};
    if(!checked)
    { llvm_jit_memtag_access_failure<ManagedTraps>(checked.source_failed ? *source_memory : *destination_memory,
        checked.source_failed ? source : destination,0u,bytes,checked.access); }
}

template<typename Address,bool Shared,bool ManagedTraps=false>
inline void llvm_jit_memtag_init_bridge(::std::uintptr_t owner,::std::uintptr_t module_owner,
    ::std::uint64_t index,::std::uint64_t destination,::std::uint64_t source,::std::uint64_t bytes) noexcept(!ManagedTraps)
{
    auto const* memory{reinterpret_cast<runtime_native_memory_t const*>(owner)};
    auto const* module{reinterpret_cast<runtime_module_storage_t const*>(module_owner)};
    if(memory==nullptr || module==nullptr || index>=module->local_defined_data_vec_storage.size()) { ::fast_io::fast_terminate(); }
    auto const& data{module->local_defined_data_vec_storage.index_unchecked(static_cast<::std::size_t>(index)).data};
    auto const payload{::uwvm2::uwvm::runtime::storage::load_wasm_data_segment_payload(data)};
    if((payload.byte_begin==nullptr)!=(payload.byte_end==nullptr)) { ::fast_io::fast_terminate(); }
    auto const source_size{payload.byte_begin==nullptr ? 0u : static_cast<::std::size_t>(payload.byte_end-payload.byte_begin)};
    if(bytes>source_size || source>source_size-bytes)
    {
        auto const size{bytes>SIZE_MAX ? SIZE_MAX : static_cast<::std::size_t>(bytes)};
        if constexpr(ManagedTraps)
        { ::uwvm2::runtime::lib::llvm_jit_managed_memory_trap(memory->diagnostic_owner_memory_index,0u,source,0u,source_size,size,0u,0u); }
        else
        { ::uwvm2::runtime::lib::llvm_jit_memory_out_of_bounds_trap(memory->diagnostic_owner_memory_index,0u,source,0u,source_size,size,0u,0u); }
        ::fast_io::fast_terminate();
    }
    auto const checked{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(*memory,static_cast<Address>(destination),0u,bytes,
        [&](::std::span<::std::byte> destination_data) noexcept
        {
            if(destination_data.empty()) { return; }
            auto const begin{payload.byte_begin+static_cast<::std::size_t>(source)};
            if constexpr(Shared)
            { for(::std::size_t i{};i!=destination_data.size();++i)
              { ::std::atomic_ref<::std::byte>(destination_data[i]).store(begin[i],::std::memory_order_relaxed); } }
            else { ::std::memcpy(destination_data.data(),begin,destination_data.size()); }
        })};
    if(!checked) { llvm_jit_memtag_access_failure<ManagedTraps>(*memory,destination,0u,bytes,checked); }
}

template<unsigned Operation,typename Address,bool Shared,bool ManagedTraps=false>
[[nodiscard]] inline ::std::uint64_t llvm_jit_memtag_wait_notify_bridge(::std::uintptr_t owner,::std::uint64_t offset,
    ::std::uint64_t pointer,::std::uint64_t expected,::std::int64_t timeout) noexcept(!ManagedTraps)
{
    auto const* memory{reinterpret_cast<runtime_native_memory_t const*>(owner)};
    if(memory==nullptr) { ::fast_io::fast_terminate(); }
    constexpr ::std::size_t bytes{Operation==2u ? 8u : 4u};
    auto const result{::uwvm2::runtime::memtag::wait_notify<Operation,Address,Shared>(
        *memory,static_cast<Address>(pointer),offset,expected,timeout)};
    if(!result.access) { llvm_jit_memtag_access_failure<ManagedTraps>(*memory,pointer,offset,bytes,result.access); }
    namespace waiting=::uwvm2::runtime::wasm_threads;
    using trap=::uwvm2::runtime::lib::llvm_jit_trap_kind;
    trap failure{trap::runtime_invariant_failure};
    switch(result.wait.status)
    {
        case waiting::wait_status::notified:case waiting::wait_status::not_equal:case waiting::wait_status::timed_out:
            return Operation==0u ? result.wait.notified : static_cast<unsigned>(result.wait.status);
        case waiting::wait_status::unaligned:failure=trap::unaligned_atomic;break;
        case waiting::wait_status::out_of_bounds:
            llvm_jit_memtag_access_failure<ManagedTraps>(*memory,pointer,offset,bytes,
                {.error=::uwvm2::extension::memtag::access_error::out_of_bounds,.address=result.access.address});
        case waiting::wait_status::not_shared:failure=trap::atomic_wait_non_shared;break;
        case waiting::wait_status::cancelled:
            if constexpr(ManagedTraps) { ::uwvm2::runtime::lib::details::llvm_jit_debug_cancelled_wait_abi_bridge(); }
            failure=trap::atomic_wait_cancelled;break;
        case waiting::wait_status::unavailable:failure=trap::atomic_wait_unavailable;break;
        case waiting::wait_status::too_many_waiters:failure=trap::atomic_wait_limit;break;
    }
    if constexpr(ManagedTraps) { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(failure,0u,0u); }
    else { ::uwvm2::runtime::lib::llvm_jit_runtime_trap(failure,0u,0u); }
    ::fast_io::fast_terminate();
}
// Blocking leaves retain genuine Wasm debug activation while sleeping. Other
// MTE bridges retain the same foreign-host isolation as ordinary memory helpers.
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<1u,::std::uint32_t,false,false>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<1u,::std::uint32_t,false,true>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<1u,::std::uint32_t,true,false>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<1u,::std::uint32_t,true,true>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<1u,::std::uint64_t,false,false>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<1u,::std::uint64_t,false,true>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<1u,::std::uint64_t,true,false>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<1u,::std::uint64_t,true,true>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<2u,::std::uint32_t,false,false>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<2u,::std::uint32_t,false,true>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<2u,::std::uint32_t,true,false>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<2u,::std::uint32_t,true,true>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<2u,::std::uint64_t,false,false>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<2u,::std::uint64_t,false,true>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<2u,::std::uint64_t,true,false>>{true};
template<> inline constexpr bool llvm_jit_debug_control_bridge<llvm_jit_memtag_wait_notify_bridge<2u,::std::uint64_t,true,true>>{true};

// Checked leaves return faults by value. The ordinary wrapper and actual
// threaded ChildLease wrapper keep distinct trap/ownership authority; neither
// can unwind or report a group failure while a tag/relocation lease is held.
struct llvm_jit_memtag_data_result
{
    ::uwvm2::extension::memtag::access_result access{};
    ::std::uint64_t value{};
    bool unaligned{};
};
template<typename Address,bool Shared,unsigned ResultBits,::std::size_t Bytes,bool Signed,bool Store>
[[nodiscard]] inline llvm_jit_memtag_data_result llvm_jit_memtag_scalar_access(
    runtime_native_memory_t const& memory,::std::uint64_t offset,::std::uint64_t pointer,::std::uint64_t input) noexcept
{
    static_assert(ResultBits==32u || ResultBits==64u);
    static_assert(Bytes==1u || Bytes==2u || Bytes==4u || Bytes==8u);
    ::std::uint64_t result{};
    auto const checked{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(memory,static_cast<Address>(pointer),offset,Bytes,
        [&](::std::span<::std::byte> data) noexcept
        {
            ::std::array<::std::byte,Bytes> temporary{};
            auto* bytes{data.data()};
            if constexpr(Shared)
            {
                bytes=temporary.data();
                if constexpr(!Store) { for(::std::size_t i{};i!=Bytes;++i)
                    { bytes[i]=::std::atomic_ref<::std::byte>(data[i]).load(::std::memory_order_relaxed); } }
            }
            if constexpr(Store)
            {
                if constexpr(Bytes==1u) { llvm_jit_store_little_endian_integer(bytes,static_cast<::std::uint8_t>(input)); }
                else if constexpr(Bytes==2u) { llvm_jit_store_little_endian_integer(bytes,static_cast<::std::uint16_t>(input)); }
                else if constexpr(Bytes==4u) { llvm_jit_store_little_endian_integer(bytes,static_cast<::std::uint32_t>(input)); }
                else { llvm_jit_store_little_endian_integer(bytes,input); }
                if constexpr(Shared) { for(::std::size_t i{};i!=Bytes;++i)
                    { ::std::atomic_ref<::std::byte>(data[i]).store(bytes[i],::std::memory_order_relaxed); } }
            }
            else
            {
                if constexpr(Bytes==1u) { result=llvm_jit_load_little_endian_integer<::std::uint8_t>(bytes); }
                else if constexpr(Bytes==2u) { result=llvm_jit_load_little_endian_integer<::std::uint16_t>(bytes); }
                else if constexpr(Bytes==4u) { result=llvm_jit_load_little_endian_integer<::std::uint32_t>(bytes); }
                else { result=llvm_jit_load_little_endian_integer<::std::uint64_t>(bytes); }
            }
        })};
    if constexpr(!Store && Signed && Bytes*8u<ResultBits)
    {
        constexpr auto sign{::std::uint64_t{1u}<<(Bytes*8u-1u)};
        result=(result^sign)-sign;
    }
    if constexpr(ResultBits==32u) { result=static_cast<::std::uint32_t>(result); }
    return {checked,result,false};
}

template<typename Address,bool Shared,unsigned ResultBits,::std::size_t Bytes,bool Signed,bool Store,bool ManagedTraps=false>
[[nodiscard]] inline ::std::uint64_t llvm_jit_memtag_scalar_bridge(::std::uintptr_t owner,
    ::std::uint64_t offset,::std::uint64_t pointer,::std::uint64_t input) noexcept(!ManagedTraps)
{
    auto const* memory{reinterpret_cast<runtime_native_memory_t const*>(owner)};
    if(memory==nullptr) { ::fast_io::fast_terminate(); }
    auto const result{llvm_jit_memtag_scalar_access<Address,Shared,ResultBits,Bytes,Signed,Store>(*memory,offset,pointer,input)};
    if(!result.access) { llvm_jit_memtag_access_failure<ManagedTraps>(*memory,pointer,offset,Bytes,result.access); }
    return result.value;
}

template<llvm_jit_simd_code Op,typename Address,bool Shared,bool ManagedTraps=false>
inline void llvm_jit_memtag_simd_bridge(::std::uintptr_t owner,::std::uint64_t offset,::std::uint64_t pointer,
    ::std::uintptr_t vector_address,::std::uint64_t lane,::std::uintptr_t result_address) noexcept(!ManagedTraps)
{
    constexpr bool store{Op==llvm_jit_simd_code::v128_store || Op==llvm_jit_simd_code::v128_store8_lane ||
        Op==llvm_jit_simd_code::v128_store16_lane || Op==llvm_jit_simd_code::v128_store32_lane || Op==llvm_jit_simd_code::v128_store64_lane};
    constexpr bool lane_load{Op==llvm_jit_simd_code::v128_load8_lane || Op==llvm_jit_simd_code::v128_load16_lane ||
        Op==llvm_jit_simd_code::v128_load32_lane || Op==llvm_jit_simd_code::v128_load64_lane};
    constexpr auto width{llvm_jit_simd_details::simd_memory_access_size<Op>()};
    auto const* memory{reinterpret_cast<runtime_native_memory_t const*>(owner)};
    if(memory==nullptr) { ::fast_io::fast_terminate(); }
    runtime_wasm_v128 value{};
    if constexpr(store || lane_load) { value=llvm_jit_simd_read_bridge_value<runtime_wasm_v128>(vector_address); }
    auto const checked{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(*memory,static_cast<Address>(pointer),offset,width,
        [&](::std::span<::std::byte> data) noexcept
        {
            ::std::array<::std::byte,width> temporary{};
            auto* bytes{data.data()};
            if constexpr(Shared)
            {
                bytes=temporary.data();
                if constexpr(!store) { for(::std::size_t i{};i!=width;++i)
                    { bytes[i]=::std::atomic_ref<::std::byte>(data[i]).load(::std::memory_order_relaxed); } }
            }
            if constexpr(store)
            {
                llvm_jit_simd_details::eval_memory_store<Op>(bytes,value,static_cast<llvm_jit_simd_details::u8>(lane));
                if constexpr(Shared) { for(::std::size_t i{};i!=width;++i)
                    { ::std::atomic_ref<::std::byte>(data[i]).store(bytes[i],::std::memory_order_relaxed); } }
            }
            else { value=llvm_jit_simd_details::eval_memory_load<Op>(bytes,value,static_cast<llvm_jit_simd_details::u8>(lane)); }
        })};
    if(!checked) { llvm_jit_memtag_access_failure<ManagedTraps>(*memory,pointer,offset,width,checked); }
    if constexpr(!store) { llvm_jit_simd_write_bridge_value(result_address,value); }
}

template<typename Address,bool Shared>
[[nodiscard]] inline llvm_jit_memtag_data_result llvm_jit_memtag_atomic_access(
    runtime_native_memory_t const& memory,::std::uint64_t offset,::std::uint64_t pointer,
    ::std::uint64_t input,::std::uint64_t expected,::std::uint64_t opcode) noexcept
{
    namespace atomic=::uwvm2::validation::standard::wasm3;
    namespace operations=::uwvm2::runtime::compiler::shared::wasm_threads;
    using kind=atomic::atomic_instruction_kind;
    if(opcode<0x10u || opcode>0x4eu) { ::fast_io::fast_terminate(); }
    auto const descriptor{atomic::describe_atomic_instruction(static_cast<::std::uint32_t>(opcode))};
    auto const bytes{::std::size_t{1u}<<descriptor.natural_alignment};
    bool unaligned{};::std::uint64_t result{};
    auto const checked{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(memory,static_cast<Address>(pointer),offset,bytes,
        [&](::std::span<::std::byte> data) noexcept
        {
            unaligned=((pointer+offset)&(bytes-1u))!=0u;
            if(unaligned) { return; }
            auto execute{[&]<::std::size_t Bytes>() noexcept
            {
                switch(descriptor.kind)
                {
                    case kind::load:result=operations::atomic_load_le<Bytes>(data.data());break;
                    case kind::store:operations::atomic_store_le<Bytes>(data.data(),input);break;
                    case kind::add:result=operations::atomic_rmw_le<operations::atomic_rmw_operation::add,Bytes>(data.data(),input);break;
                    case kind::sub:result=operations::atomic_rmw_le<operations::atomic_rmw_operation::sub,Bytes>(data.data(),input);break;
                    case kind::and_:result=operations::atomic_rmw_le<operations::atomic_rmw_operation::and_,Bytes>(data.data(),input);break;
                    case kind::or_:result=operations::atomic_rmw_le<operations::atomic_rmw_operation::or_,Bytes>(data.data(),input);break;
                    case kind::xor_:result=operations::atomic_rmw_le<operations::atomic_rmw_operation::xor_,Bytes>(data.data(),input);break;
                    case kind::exchange:result=operations::atomic_rmw_le<operations::atomic_rmw_operation::exchange,Bytes>(data.data(),input);break;
                    case kind::compare_exchange:result=operations::atomic_rmw_le<operations::atomic_rmw_operation::compare_exchange,Bytes>(data.data(),input,expected);break;
                    default: ::fast_io::fast_terminate();
                }
            }};
            switch(bytes)
            {
                case 1u:execute.template operator()<1u>();break;case 2u:execute.template operator()<2u>();break;
                case 4u:execute.template operator()<4u>();break;case 8u:execute.template operator()<8u>();break;
                default: ::fast_io::fast_terminate();
            }
        })};
    return {checked,descriptor.result_i64 ? result : static_cast<::std::uint32_t>(result),unaligned};
}

template<typename Address,bool Shared,bool ManagedTraps=false>
[[nodiscard]] inline ::std::uint64_t llvm_jit_memtag_atomic_bridge(::std::uintptr_t owner,
    ::std::uint64_t offset,::std::uint64_t pointer,::std::uint64_t input,::std::uint64_t expected,::std::uint64_t opcode) noexcept(!ManagedTraps)
{
    auto const* memory{reinterpret_cast<runtime_native_memory_t const*>(owner)};
    if(memory==nullptr) { ::fast_io::fast_terminate(); }
    auto const result{llvm_jit_memtag_atomic_access<Address,Shared>(*memory,offset,pointer,input,expected,opcode)};
    auto const descriptor{::uwvm2::validation::standard::wasm3::describe_atomic_instruction(static_cast<::std::uint32_t>(opcode))};
    auto const bytes{::std::size_t{1u}<<descriptor.natural_alignment};
    if(!result.access) { llvm_jit_memtag_access_failure<ManagedTraps>(*memory,pointer,offset,bytes,result.access); }
    if(result.unaligned)
    {
        if constexpr(ManagedTraps)
        { ::uwvm2::runtime::lib::llvm_jit_managed_runtime_trap(::uwvm2::runtime::lib::llvm_jit_trap_kind::unaligned_atomic,0u,0u); }
        else { ::uwvm2::runtime::lib::llvm_jit_runtime_trap(::uwvm2::runtime::lib::llvm_jit_trap_kind::unaligned_atomic,0u,0u); }
        ::fast_io::fast_terminate();
    }
    return result.value;
}
