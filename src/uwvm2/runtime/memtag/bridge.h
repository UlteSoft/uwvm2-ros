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
# include <memory>
# include <span>
# include <type_traits>
# include <utility>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/object/memory/linear/impl.h>
# include <uwvm2/extension/memtag/operations.h>
# include "random.h"
# include "native_pointer.h"
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

    // Full-width offsets/lengths are preserved even on 32-bit hosts. The callback
    // receives only the proven byte range, and only while the memory is pinned.
    // It must perform host work synchronously and must not retain the span.
    template<::std::unsigned_integral Address, bool Shared, typename Memory, typename Function>
    [[nodiscard]] inline ::uwvm2::extension::memtag::access_result with_checked_access(
        Memory const& memory, Address pointer, ::std::uint64_t offset, ::std::uint64_t bytes,
        Function&& function) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        static_assert(::std::is_nothrow_invocable_r_v<void,Function,::std::span<::std::byte>>);
        if(!details::configured<Address,Shared>(memory))
        { return {.error=mt::access_error::memory_not_representable}; }
        return details::with_snapshot(memory,[&](::std::span<::std::byte> data) noexcept
        {
            return memory.memtag_shadow.template with_shared_tag_lease<Shared>([&]() noexcept
            {
                auto const view{memory.memtag_shadow.template view<Address,Shared>(data.size())};
                auto result{view.check(pointer,offset,bytes)};
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
        template<::std::unsigned_integral Address,typename Memory>
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
                return memory.memtag_shadow.template view<Address,true>(data.size()).check(pointer,0u,bytes);
            }
            if(!configured<Address,false>(memory)) { return {.error=mt::access_error::memory_not_representable}; }
            return memory.memtag_shadow.template view<Address,false>(data.size()).check(pointer,0u,bytes);
        }
    }
    template<::std::unsigned_integral Destination,::std::unsigned_integral Source,typename Memory,typename Function>
    [[nodiscard]] inline copy_access_result with_checked_copy(
        Memory const& destination_memory,Memory const& source_memory,Destination destination,Source source,
        ::std::uint64_t bytes,Function&& function) noexcept
    {
        static_assert(::std::is_nothrow_invocable_r_v<void,Function,::std::span<::std::byte>,::std::span<::std::byte>>);
        return details::with_two_snapshots(destination_memory,source_memory,
            [&](::std::span<::std::byte> destination_data,::std::span<::std::byte> source_data) noexcept
            {
                return details::with_two_tag_leases(destination_memory,source_memory,[&]() noexcept
                {
                    auto const destination_result{details::check_snapshot(destination_memory,destination_data,destination,bytes)};
                    if(!destination_result) { return copy_access_result{destination_result,false}; }
                    auto const source_result{details::check_snapshot(source_memory,source_data,source,bytes)};
                    if(!source_result) { return copy_access_result{source_result,true}; }
                    // Both complete ranges, including every intermediate granule,
                    // are proven before the first mutation. No pointer escapes.
                    function(::std::span<::std::byte>{native_pointer_unlocked(destination_memory,destination_result),static_cast<::std::size_t>(bytes)},
                             ::std::span<::std::byte>{native_pointer_unlocked(source_memory,source_result),static_cast<::std::size_t>(bytes)});
                    return copy_access_result{};
                });
            });
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
