// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <array>
# include <type_traits>
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <limits>
# include <uwvm2/extension/memtag/instruction.h>
# include <uwvm2/runtime/memtag/host_fault.h>
# include <uwvm2/runtime/memtag/bridge.h>
# include <uwvm2/runtime/memtag/native_pointer.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::memory
{
    template<typename Memory>
    [[nodiscard]] inline constexpr bool declared_shared_bytes(Memory const& memory) noexcept
    {
        if constexpr(requires {memory.sequentially_consistent_size;})
        {return memory.sequentially_consistent_size || memory.memtag_shadow.shared();}
        else {return memory.memtag_shadow.shared();}
    }

    struct checked_guest_span
    {
        ::uwvm2::extension::memtag::access_result access{};
        ::std::byte* data{};
        ::std::size_t bytes{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        { return static_cast<bool>(access); }
    };

    // The caller must retain the existing memory::lock_memory guard throughout
    // use of this span. Do not acquire another relocation pin here: a pending
    // grow can otherwise wait for the outer pin while the inner pin waits for it.
    // A returned pointer has no tag lease. Use it only for preflight; shared
    // tagged byte effects must use with_checked_guest_bytes_unlocked below.
    // Observers instantiate RecordFault=false and cannot change guest execution.
    template<bool RecordFault = true, typename Memory>
    [[nodiscard]] inline checked_guest_span checked_span_unlocked(
        Memory const& memory, ::std::uint64_t raw, ::std::uint64_t bytes) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        checked_guest_span span{};
        span.access = {.error = mt::access_error::out_of_bounds, .address = raw};
        if constexpr(RecordFault)
        {
            // A failed nested decode cannot become a zero-valued, apparently
            // valid pointer in a later preflight of the same host operation.
            if(::uwvm2::runtime::memtag::pending_host_fault.pending) { return span; }
        }
        if(memory.memory_begin == nullptr) { return span; }
        ::std::size_t length{};
        if constexpr(requires { memory.memory_length_p; })
        {
            if(memory.memory_length_p == nullptr) { return span; }
            length = memory.memory_length_p->load(::std::memory_order_acquire);
        }
        else { length = memory.memory_length; }
        if(memory.memtag_shadow.enabled())
        {
            auto check{[&]<typename Address>() noexcept -> mt::access_result
            {
                if(raw > (::std::numeric_limits<Address>::max)())
                { return {.error = mt::access_error::out_of_bounds, .address = raw}; }
                auto checked{[&](auto const& view) noexcept
                {
                    if constexpr(RecordFault)
                    {
                        ::std::uint64_t index{};
                        if constexpr(requires{memory.diagnostic_owner_memory_index;})
                        { index=memory.diagnostic_owner_memory_index; }
                        return ::uwvm2::runtime::memtag::details::check_view<false>(
                            view,static_cast<Address>(raw),0u,bytes,index);
                    }
                    else { return view.check(static_cast<Address>(raw),0u,bytes); }
                }};
                if(declared_shared_bytes(memory))
                { return checked(memory.memtag_shadow.template view<Address,true>(length)); }
                return checked(memory.memtag_shadow.template view<Address,false>(length));
            }};
            if(memory.memtag_shadow.width() == mt::address_width::i32)
            { span.access = check.template operator()<::std::uint32_t>(); }
            else if(memory.memtag_shadow.width() == mt::address_width::i64)
            { span.access = check.template operator()<::std::uint64_t>(); }
            else { span.access.error = mt::access_error::memory_not_representable; }
        }
        else if(raw <= length && bytes <= length - raw)
        { span.access = {.address = raw}; }
        if(!span.access)
        {
            if constexpr(RecordFault)
            { ::uwvm2::runtime::memtag::record_host_fault(0u,raw,span.access); }
            return span;
        }
        // Both narrowing and native pointer formation follow the complete raw
        // pointer, tag and range proof. Untagged owners preserve every input bit.
        span.data = ::uwvm2::runtime::memtag::native_pointer_unlocked(memory,span.access);
        span.bytes = static_cast<::std::size_t>(bytes);
        return span;
    }

    // The caller retains the existing relocation pin. This helper never pins
    // again: a nested pin can deadlock against a pending memory.grow. The callback
    // must finish all byte effects before returning and must not retain the
    // pointer, acquire another tag lease, enter Guest/debugger code, block on
    // Host I/O, or raise a trap/longjmp. Crash-mode delayed fault recording only updates TLS. Explicit log mode may
    // write diagnostic metadata while the owner is pinned; that sink never
    // reenters Guest code or acquires a memory owner/relocation lease.
    template<bool RecordFault = true, typename Memory, typename Function>
    [[nodiscard]] inline ::uwvm2::extension::memtag::access_result
    with_checked_guest_bytes_unlocked(
        Memory const& memory, ::std::uint64_t raw, ::std::uint64_t bytes,
        Function&& function) noexcept
    {
        static_assert(noexcept(function(static_cast<::std::byte*>(nullptr), ::std::size_t{})));
        static_assert(::std::is_void_v<decltype(function(static_cast<::std::byte*>(nullptr), ::std::size_t{}))>);
        auto body{[&]() noexcept
        {
            auto const span{checked_span_unlocked<RecordFault>(memory,raw,bytes)};
            if(span) { function(span.data,span.bytes); }
            return span.access;
        }};
        if(memory.memtag_shadow.enabled() && memory.memtag_shadow.shared())
        {
            if(!memory.memtag_shadow.shared_tag_lease_ready())
            { return {.error = ::uwvm2::extension::memtag::access_error::memory_not_representable, .address = raw}; }
            return memory.memtag_shadow.template with_shared_tag_lease<true>(body);
        }
        return body();
    }

    template<bool Write, bool RecordFault = true, typename Memory>
    [[nodiscard]] inline bool copy_guest_bytes_unlocked(
        Memory const& memory, ::std::uint64_t raw, void* host, ::std::size_t bytes,
        bool declared_shared = false) noexcept
    {
        if(bytes != 0u && host == nullptr) { return false; }
        return static_cast<bool>(with_checked_guest_bytes_unlocked<RecordFault>(
            memory,raw,bytes,[&](::std::byte* data, ::std::size_t count) noexcept
            {
                if(count == 0u) { return; }
                if(declared_shared || declared_shared_bytes(memory))
                {
                    auto* native{static_cast<::std::byte*>(host)};
                    for(::std::size_t i{}; i != count; ++i)
                    {
                        ::std::atomic_ref<::std::byte> cell{data[i]};
                        if constexpr(Write) { cell.store(native[i],::std::memory_order_relaxed); }
                        else { native[i] = cell.load(::std::memory_order_relaxed); }
                    }
                }
                else if constexpr(Write) { ::std::memcpy(data,host,count); }
                else { ::std::memcpy(host,data,count); }
            }));
    }

    template<typename WasmType, typename Memory>
    [[nodiscard]] inline WasmType tagged_read_value_unlocked(
        Memory const& memory, ::std::uint64_t raw) noexcept
    {
        using U = ::std::make_unsigned_t<WasmType>;
        ::std::array<::std::byte,sizeof(WasmType)> bytes{};
        if(!copy_guest_bytes_unlocked<false>(memory,raw,bytes.data(),bytes.size())) { return {}; }
        U value{};
        for(::std::size_t i{}; i != bytes.size(); ++i)
        { value |= static_cast<U>(::std::to_integer<unsigned char>(bytes[i])) << (8u*i); }
        return static_cast<WasmType>(value);
    }
    template<typename WasmType, typename Memory>
    inline void tagged_store_value_unlocked(
        Memory const& memory, ::std::uint64_t raw, WasmType value) noexcept
    {
        using U = ::std::make_unsigned_t<WasmType>;
        auto bits{static_cast<U>(value)};
        ::std::array<::std::byte,sizeof(WasmType)> bytes{};
        for(::std::size_t i{}; i != bytes.size(); ++i)
        { bytes[i] = static_cast<::std::byte>(bits >> (8u*i)); }
        static_cast<void>(copy_guest_bytes_unlocked<true>(memory,raw,bytes.data(),bytes.size()));
    }
    template<typename Memory>
    inline void tagged_clear_unlocked(
        Memory const& memory, ::std::uint64_t raw, ::std::size_t bytes) noexcept
    {
        static_cast<void>(with_checked_guest_bytes_unlocked(
            memory,raw,bytes,[&](::std::byte* data, ::std::size_t count) noexcept
            {
                if(declared_shared_bytes(memory))
                {
                    for(::std::size_t i{}; i != count; ++i)
                    { ::std::atomic_ref<::std::byte>{data[i]}.store(::std::byte{},::std::memory_order_relaxed); }
                }
                else if(count != 0u) { ::std::memset(data,0,count); }
            }));
    }

}
