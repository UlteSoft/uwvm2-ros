/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <concepts>
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <memory>
# include <span>
# include <type_traits>
# include <utility>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/object/memory/linear/impl.h>
# include <uwvm2/extension/memtag/operations.h>
# include "random.h"
# include "native_pointer.h"
# include "deferred.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag
{
    enum class bridge_error : unsigned { none, invalid_owner, entropy_unavailable };
    template<::std::unsigned_integral Address>
    struct execution_result
    {
        ::uwvm2::extension::memtag::operation_result<Address> operation{};
        bridge_error error{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        { return error == bridge_error::none && static_cast<bool>(operation); }
    };

    namespace details
    {
        // Keep logging and host I/O out of the private integer checker's
        // inline budget. This cold path still proves the whole span first.
        template<bool Guest,typename View,typename Address>
        [[nodiscard,gnu::noinline,gnu::cold]] inline auto check_view_logged(
            View const& view,Address pointer,::std::uint64_t offset,
            ::std::uint64_t bytes,::std::uint64_t memory_index) noexcept
        {
            auto result{view.bounds(pointer,offset,bytes)};
            if(!result) { return result; }
            ::std::uint64_t done{};
            while(done!=bytes)
            {
                auto const remaining{16u-((result.address+done)&15u)};
                auto const chunk{bytes-done<remaining ? bytes-done:remaining};
                auto checked{view.check(pointer,offset+done,chunk)};
                checked=deferred::defer_if_requested(checked,pointer,memory_index,Guest);
                if(!checked) { return checked; }
                done+=chunk;
            }
            return result;
        }
        // The private interpreter emitter owns this three-u64 native bytecode
        // prefix. Decode it only on failure, while the complete owner span is
        // still pinned. No Guest address can supply or change this provenance.
        template<bool Guest,typename View,typename Address>
        [[nodiscard,gnu::noinline,gnu::cold]] inline ::uwvm2::extension::memtag::access_error check_view_with_origin(
            View const& view,Address pointer,::std::uint64_t offset,::std::uint64_t bytes,
            ::std::uint64_t memory_index,::uwvm2::extension::memtag::access_result const& initial,
            ::std::byte const* encoded) noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            if(!encoded) { return mt::access_error::memory_not_representable; }
            auto result{initial};
            ::std::uint64_t location[3]{};
            ::std::memcpy(location,encoded,sizeof(location));
            if(!mt::logging_requested())
            {
                if constexpr(Guest)
                {
                    deferred::record_llvm_at(memory_index,pointer,result.mismatch_granule,
                        result.pointer_tag,result.allocation_tag,location[0],location[1],location[2]);
                    result.error=mt::access_error::none;
                }
                return result.error;
            }
            result=view.bounds(pointer,offset,bytes);
            if(!result) { return result.error; }
            ::std::uint64_t done{};
            while(done!=bytes)
            {
                auto const remaining{16u-((result.address+done)&15u)};
                auto const chunk{bytes-done<remaining ? bytes-done:remaining};
                auto checked{view.check(pointer,offset+done,chunk)};
                if(checked.error==mt::access_error::tag_mismatch)
                {
                    deferred::record_llvm_at(memory_index,pointer,checked.mismatch_granule,
                        checked.pointer_tag,checked.allocation_tag,location[0],location[1],location[2]);
                }
                else if(!checked) { return checked.error; }
                done+=chunk;
            }
            return result.error;
        }
        template<bool Guest,typename View,typename Address>
        [[nodiscard,gnu::always_inline]] inline auto check_view(View const& view,Address pointer,
            ::std::uint64_t offset,::std::uint64_t bytes,::std::uint64_t memory_index,
            ::std::uint8_t comparison_mask=0xffu) noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            auto result{view.check(pointer,offset,bytes,comparison_mask)};
            // The complete normal proof is also the no-violation log fast path.
            // Only a mismatch needs the cold per-granule reporting walk; bounds
            // failures still return unchanged before any pointer is formed.
            if(result.error==mt::access_error::tag_mismatch && mt::logging_requested()) [[unlikely]]
            { return check_view_logged<Guest>(view,pointer,offset,bytes,memory_index); }
            if constexpr(Guest)
            { result=deferred::defer_if_requested(result,pointer,memory_index); }
            return result;
        }
        // The enclosing execution lease owns Memory. Imports resolve to this
        // same native owner before entry; a module index is never an identity.
        // A callback may neither retain a span/pointer nor invoke guest code or
        // trap. Errors are returned by value after the relocation pin releases.
        template<typename Memory, typename Function>
        inline auto with_snapshot(Memory const& memory, Function&& function) noexcept
        {
            static_assert(noexcept(function(::std::span<::std::byte>{})));
            auto checked{[&](::std::byte* begin, ::std::size_t bytes) noexcept
            { return function(::std::span<::std::byte>{begin,begin == nullptr ? 0u : bytes}); }};
            if constexpr(Memory::can_mmap)
            {
                if(memory.memory_length_p == nullptr) { return checked(nullptr,0u); }
                return checked(memory.memory_begin,memory.memory_length_p->load(::std::memory_order_acquire));
            }
            else if constexpr(Memory::support_multi_thread)
            {
                ::uwvm2::object::memory::linear::memory_operation_guard_t pin{
                    memory.growing_flag_p,memory.active_ops_p};
                return checked(memory.memory_begin,memory.memory_length);
            }
            else { return checked(memory.memory_begin,memory.memory_length); }
        }
        template<::std::unsigned_integral Address, bool Shared, typename Memory>
        [[nodiscard]] inline bool configured(Memory const& memory) noexcept
        {
            constexpr auto width{sizeof(Address)==4u ?
                ::uwvm2::extension::memtag::address_width::i32 : ::uwvm2::extension::memtag::address_width::i64};
            return memory.memtag_shadow.enabled() && memory.memtag_shadow.width()==width &&
                   memory.memtag_shadow.shared()==Shared &&
                   (!Shared || memory.memtag_shadow.shared_tag_lease_ready());
        }
    }

    namespace details
    {
        // Equivalent to with_snapshot + a private tag lease, flattened for the
        // interpreter register-ring handler. mmap pins its stable reservation;
        // the live acquired length and complete bounds/tag proof still precede
        // pointer formation. A private tag lease does not acquire a lock.
        template<::std::unsigned_integral Address,bool Guest,typename Memory,typename Function>
        [[nodiscard,gnu::always_inline]] inline auto with_private_origin_access(
            Memory const& memory,Address pointer,::std::uint64_t offset,::std::uint64_t bytes,
            Function&& function,::std::byte const* encoded_origin) noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            static_assert(Memory::can_mmap);
            static_assert(::std::is_nothrow_invocable_r_v<void,Function,::std::span<::std::byte>>);
            if constexpr(Guest) { deferred::poll(); }
            if(!configured<Address,false>(memory))
            { return mt::access_result{.error=mt::access_error::memory_not_representable}; }
            auto const length{memory.memory_length_p?memory.memory_length_p->load(::std::memory_order_acquire):0u};
            auto const view{memory.memtag_shadow.template view<Address,false>(length)};
            auto result{view.check(pointer,offset,bytes)};
            if(result.error==mt::access_error::tag_mismatch) [[unlikely]]
            {
                // Returning only the error keeps the original proved address
                // in SSA; the cold reporter never chooses an access address.
                result.error=check_view_with_origin<Guest>(view,pointer,offset,bytes,
                    memory.diagnostic_owner_memory_index,result,encoded_origin);
            }
            if(result)
            { ::std::forward<Function>(function)(::std::span<::std::byte>{native_pointer_unlocked(memory,result),static_cast<::std::size_t>(bytes)}); }
            return result;
        }
    }
    // Full-width offsets/lengths are preserved even on 32-bit hosts. The callback
    // receives only the proven byte range, and only while the memory is pinned.
    // It must perform host work synchronously and must not retain the span.
    template<::std::unsigned_integral Address, bool Shared, bool Guest, bool Enforcing = false, bool ExplicitOrigin = false, typename Memory, typename Function>
    [[nodiscard]] inline ::uwvm2::extension::memtag::access_result with_access_policy(
        Memory const& memory, Address pointer, ::std::uint64_t offset, ::std::uint64_t bytes,
        Function&& function,::std::byte const* encoded_origin=nullptr) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        static_assert(!ExplicitOrigin || (!Shared && !Enforcing));
        static_assert(::std::is_nothrow_invocable_r_v<void,Function,::std::span<::std::byte>>);
        if constexpr(ExplicitOrigin && Memory::can_mmap)
        {
            return details::with_private_origin_access<Address,Guest>(memory,pointer,offset,bytes,
                ::std::forward<Function>(function),encoded_origin);
        }
        if constexpr(Guest) { deferred::poll(); }
        if(!details::configured<Address,Shared>(memory))
        { return {.error=mt::access_error::memory_not_representable}; }
        return details::with_snapshot(memory,[&](::std::span<::std::byte> data) noexcept
        {
            return memory.memtag_shadow.template with_shared_tag_lease<Shared>([&]() noexcept
            {
                auto const view{memory.memtag_shadow.template view<Address,Shared>(data.size())};
                ::std::uint8_t comparison_mask{0xffu};
                if constexpr(Guest)
                {
                    if(memory.memtag_shadow.hardware_active() &&
                       mt::hardware_checked_access_requested())
                    { comparison_mask=sizeof(Address)==4u?0u:0xf0u; }
                }
                // Hardware sync/async checks the low nibble at the physical access.
                // memory64 still checks its upper four software tag bits here.
                mt::access_result result{};
                if constexpr(Enforcing)
                {
                    static_assert(!Guest);
                    // The trusted translator chooses this only for immutable
                    // sync/crash launch policy. Keep the exact complete proof;
                    // no report/delivery flag is read on each scalar access.
                    result=view.check(pointer,offset,bytes,comparison_mask);
                }
                else if constexpr(ExplicitOrigin)
                {
                    result=view.check(pointer,offset,bytes,comparison_mask);
                    if(result.error==mt::access_error::tag_mismatch) [[unlikely]]
                    { result.error=details::check_view_with_origin<Guest>(view,pointer,offset,bytes,
                        memory.diagnostic_owner_memory_index,result,encoded_origin); }
                }
                else { result=details::check_view<Guest>(view,pointer,offset,bytes,
                    memory.diagnostic_owner_memory_index,comparison_mask); }
                if(result)
                {
                    // [owner bytes ... proven range ...] end
                    // [safe                            ]
                    // Pointer formation follows the complete bounds/tag proof.
                    ::std::forward<Function>(function)(::std::span<::std::byte>{
                        native_pointer_unlocked(memory,result),static_cast<::std::size_t>(bytes)});
                }
                return result;
            });
        });
    }
    template<::std::unsigned_integral Address, bool Shared, typename Memory, typename Function>
    [[nodiscard]] inline auto with_checked_access(Memory const& memory,Address pointer,
        ::std::uint64_t offset,::std::uint64_t bytes,Function&& function) noexcept
    { return with_access_policy<Address,Shared,false>(memory,pointer,offset,bytes,::std::forward<Function>(function)); }
    template<::std::unsigned_integral Address, bool Shared, typename Memory, typename Function>
    [[nodiscard]] inline auto with_guest_access(Memory const& memory,Address pointer,
        ::std::uint64_t offset,::std::uint64_t bytes,Function&& function) noexcept
    {
        // Delivery policy is fixed before owners/code are published. Under
        // synchronous delivery the strict path retains the identical complete
        // owner/bounds/shadow proof without entering deferred-fault machinery.
        if(!::uwvm2::extension::memtag::asynchronous_requested() && !::uwvm2::extension::memtag::hardware_checked_access_requested()) [[likely]]
        { return with_access_policy<Address,Shared,false>(memory,pointer,offset,bytes,::std::forward<Function>(function)); }
        return with_access_policy<Address,Shared,true>(memory,pointer,offset,bytes,::std::forward<Function>(function));
    }
    template<::std::unsigned_integral Address, bool Shared, typename Memory>
    [[nodiscard]] inline ::uwvm2::extension::memtag::access_result check_access(
        Memory const& memory, Address pointer, ::std::uint64_t offset, ::std::uint64_t bytes) noexcept
    {
        return with_checked_access<Address,Shared>(memory,pointer,offset,bytes,
            [](::std::span<::std::byte>) noexcept {});
    }


    struct copy_access_result
    {
        ::uwvm2::extension::memtag::access_result access{};
        bool source_failed{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return static_cast<bool>(access); }
    };
    namespace details
    {
        template<typename Memory,typename Function>
        inline auto with_two_snapshots(Memory const& destination,Memory const& source,Function&& function) noexcept
        {
            // Aliases pin once. Distinct relocating owners always acquire pins
            // in the same total native-object order, including reverse copies.
            if(::std::addressof(destination)==::std::addressof(source))
            { return with_snapshot(destination,[&](::std::span<::std::byte> data) noexcept { return function(data,data); }); }
            if(reinterpret_cast<::std::uintptr_t>(::std::addressof(source))<
               reinterpret_cast<::std::uintptr_t>(::std::addressof(destination)))
            {
                return with_snapshot(source,[&](::std::span<::std::byte> source_data) noexcept
                { return with_snapshot(destination,[&](::std::span<::std::byte> destination_data) noexcept
                  { return function(destination_data,source_data); }); });
            }
            return with_snapshot(destination,[&](::std::span<::std::byte> destination_data) noexcept
            { return with_snapshot(source,[&](::std::span<::std::byte> source_data) noexcept
              { return function(destination_data,source_data); }); });
        }
        template<typename Memory,typename Function>
        inline auto with_two_tag_leases(Memory const& destination,Memory const& source,Function&& function) noexcept
        {
            auto one{[&](Memory const& memory,auto&& continuation) noexcept
            { return memory.memtag_shadow.template with_shared_tag_lease<true>(::std::forward<decltype(continuation)>(continuation)); }};
            if(::std::addressof(destination)==::std::addressof(source))
            { return one(destination,::std::forward<Function>(function)); }
            // with_two_snapshots already holds ALL relocation pins. Never
            // acquire a second pin after taking the first owner's tag lease.
            if(reinterpret_cast<::std::uintptr_t>(::std::addressof(source))<
               reinterpret_cast<::std::uintptr_t>(::std::addressof(destination)))
            { return one(source,[&]() noexcept { return one(destination,::std::forward<Function>(function)); }); }
            return one(destination,[&]() noexcept { return one(source,::std::forward<Function>(function)); });
        }
        template<bool Guest,::std::unsigned_integral Address,typename Memory>
        [[nodiscard]] inline ::uwvm2::extension::memtag::access_result check_snapshot(
            Memory const& memory,::std::span<::std::byte> data,Address pointer,::std::uint64_t bytes) noexcept
        {
            namespace mt=::uwvm2::extension::memtag;
            // This mixed-memory helper is only used by a caller that admits
            // both ordinary and tagged owners. Ordinary addresses keep every
            // original bit and have no tag-domain capacity restriction.
            if(!memory.memtag_shadow.enabled())
            {
                auto const address{static_cast<::std::uint64_t>(pointer)};
                if(bytes>data.size() || address>data.size()-bytes)
                { return {.error=mt::access_error::out_of_bounds,.address=address}; }
                return {.address=address};
            }
            if(memory.memtag_shadow.shared())
            {
                if(!configured<Address,true>(memory)) { return {.error=mt::access_error::memory_not_representable}; }
                return check_view<Guest>(memory.memtag_shadow.template view<Address,true>(data.size()),pointer,0u,bytes,memory.diagnostic_owner_memory_index);
            }
            if(!configured<Address,false>(memory)) { return {.error=mt::access_error::memory_not_representable}; }
            return check_view<Guest>(memory.memtag_shadow.template view<Address,false>(data.size()),pointer,0u,bytes,memory.diagnostic_owner_memory_index);
        }
    }
    template<bool Guest,::std::unsigned_integral Destination,::std::unsigned_integral Source,typename Memory,typename Function>
    [[nodiscard]] inline copy_access_result with_copy_policy(
        Memory const& destination_memory,Memory const& source_memory,Destination destination,Source source,
        ::std::uint64_t bytes,Function&& function) noexcept
    {
        static_assert(::std::is_nothrow_invocable_r_v<void,Function,::std::span<::std::byte>,::std::span<::std::byte>>);
        if constexpr(Guest) { deferred::poll(); }
        return details::with_two_snapshots(destination_memory,source_memory,
            [&](::std::span<::std::byte> destination_data,::std::span<::std::byte> source_data) noexcept
            {
                return details::with_two_tag_leases(destination_memory,source_memory,[&]() noexcept
                {
                    auto destination_result{details::check_snapshot<Guest>(destination_memory,destination_data,destination,bytes)};
                    if(!destination_result) { return copy_access_result{destination_result,false}; }
                    auto source_result{details::check_snapshot<Guest>(source_memory,source_data,source,bytes)};
                    if(!source_result) { return copy_access_result{source_result,true}; }
                    // Both complete ranges, including every intermediate granule,
                    // are proven before the first mutation. No pointer escapes.
                    function(::std::span<::std::byte>{native_pointer_unlocked(destination_memory,destination_result),static_cast<::std::size_t>(bytes)},
                             ::std::span<::std::byte>{native_pointer_unlocked(source_memory,source_result),static_cast<::std::size_t>(bytes)});
                    return copy_access_result{};
                });
            });
    }
    template<::std::unsigned_integral Destination,::std::unsigned_integral Source,typename Memory,typename Function>
    [[nodiscard]] inline copy_access_result with_checked_copy(Memory const& destination_memory,
        Memory const& source_memory,Destination destination,Source source,::std::uint64_t bytes,Function&& function) noexcept
    { return with_copy_policy<false>(destination_memory,source_memory,destination,source,bytes,::std::forward<Function>(function)); }
    template<::std::unsigned_integral Destination,::std::unsigned_integral Source,typename Memory,typename Function>
    [[nodiscard]] inline copy_access_result with_guest_copy(Memory const& destination_memory,
        Memory const& source_memory,Destination destination,Source source,::std::uint64_t bytes,Function&& function) noexcept
    {
        if(!::uwvm2::extension::memtag::asynchronous_requested()) [[likely]]
        { return with_copy_policy<false>(destination_memory,source_memory,destination,source,bytes,::std::forward<Function>(function)); }
        return with_copy_policy<true>(destination_memory,source_memory,destination,source,bytes,::std::forward<Function>(function));
    }

    template<bool Enabled, ::std::unsigned_integral Address, bool Shared, typename Memory>
    [[nodiscard]] inline execution_result<Address> execute(
        Memory const& memory, ::uwvm2::extension::memtag::opcode operation,
        ::std::span<Address const> arguments) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        using view_type = mt::shadow_view<Address,Shared>;
        execution_result<Address> result{};
        auto const* descriptor{mt::find_instruction(static_cast<::std::uint32_t>(operation))};
        if(descriptor == nullptr)
        { result.operation.error=mt::operation_error::unknown_opcode; return result; }
        if(arguments.size()!=descriptor->operand_count)
        { result.operation.error=mt::operation_error::wrong_arity; return result; }
        if(operation==mt::opcode::add)
        { result.operation.error=mt::operation_error::undefined_placeholder; return result; }
        if constexpr(Enabled)
        {
            if(!details::configured<Address,Shared>(memory))
            { result.error=bridge_error::invalid_owner; return result; }
        }
        ::std::uint8_t random{};
        auto generator{[&]() noexcept { return random; }};
        if constexpr(!Enabled)
        {
            // Disabled identity/query operations do not inspect the owner or its
            // shadow. The z instructions still have required zero-fill effects.
            if(!descriptor->zeroes_memory)
            {
                result.operation=mt::execute_operation<false,Address,Shared>(
                    operation,arguments,{},view_type{nullptr,0u,0u},generator);
                return result;
            }
        }
        if constexpr(Enabled)
        {
            bool const random_store{operation==mt::opcode::randomstore || operation==mt::opcode::randomstorez};
            if(random_store)
            {
                // Validate before consuming entropy, then release the pin before
                // a potentially blocking OS entropy refill. Recheck on execution:
                // memory.grow may relocate between these two snapshots.
                result.operation.access=details::with_snapshot(memory,[&](::std::span<::std::byte> data) noexcept
                {
                    return memory.memtag_shadow.template view<Address,Shared>(data.size()).bounds(
                        arguments[0],0u,static_cast<::std::uint64_t>(arguments[1]));
                });
                if(!result.operation.access)
                { result.operation.error=mt::operation_error::memory_access; return result; }
            }
            if(operation==mt::opcode::random || random_store)
            {
                if(!next_random_tag(random)) { result.error=bridge_error::entropy_unavailable; return result; }
            }
        }
        result.operation=details::with_snapshot(memory,[&](::std::span<::std::byte> data) noexcept
        {
            auto operation_body{[&]() noexcept
            {
                if constexpr(Enabled)
                {
                    return mt::execute_operation<true,Address,Shared>(operation,arguments,data,
                        memory.memtag_shadow.template view<Address,Shared>(data.size()),generator);
                }
                else
                {
                    return mt::execute_operation<false,Address,Shared>(
                        operation,arguments,data,view_type{nullptr,0u,0u},generator);
                }
            }};
            if constexpr(Shared)
            {
                if constexpr(Enabled)
                {
                    if(descriptor->writes_allocation_tags)
                    { return memory.memtag_shadow.template with_shared_tag_lease<true,true>(operation_body); }
                    if(descriptor->reads_allocation_tags)
                    { return memory.memtag_shadow.template with_shared_tag_lease<true>(operation_body); }
                }
                else if(descriptor->zeroes_memory)
                {
                    // An explicit identity-mode zero on a tagged owner still
                    // writes bytes; it cannot race a live allocation-tag change.
                    return memory.memtag_shadow.template with_shared_tag_lease<true>(operation_body);
                }
            }
            return operation_body();
        });
        return result;
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
