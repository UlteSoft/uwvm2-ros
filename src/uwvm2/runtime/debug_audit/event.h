/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <array>
#include <cstdint>
#include <limits>

namespace uwvm2::runtime::debug_audit
{
    // This is a native producer contract, never a guest ABI or an authority
    // reconstructed from a command's integer selectors. The runtime must mint
    // these identities from its live instance/policy/publication owners.
    struct principal
    {
        std::array<std::uint8_t, 32> wasm_sha256{};
        std::uint64_t instance{}, generation{}, policy_generation{}, trust_domain{};
        friend constexpr bool operator==(principal const&, principal const&) noexcept = default;
    };

    [[nodiscard]] constexpr bool valid(principal const& value) noexcept
    {
        bool digest{};
        for(auto byte : value.wasm_sha256) { digest = digest || byte != 0; }
        return digest && value.instance && value.generation && value.policy_generation && value.trust_domain;
    }

    // Separate categories are deliberate: linear memory is not all Wasm state.
    enum class feature : std::uint64_t
    {
        call_direct = 1ull << 0, call_indirect = 1ull << 1, call_reference = 1ull << 2,
        call_tail = 1ull << 3, call_import = 1ull << 4, call_callback = 1ull << 5,
        call_exception = 1ull << 6,
        store_scalar = 1ull << 7, store_simd = 1ull << 8, store_atomic = 1ull << 9,
        rmw_atomic = 1ull << 10, compare_exchange = 1ull << 11,
        memory_copy = 1ull << 12, memory_fill = 1ull << 13, memory_init = 1ull << 14,
        memory_initialization = 1ull << 15, memory_growth = 1ull << 16,
        host_wasi_write = 1ull << 17, host_ros_write = 1ull << 18,
        wasi_operation = 1ull << 19, ros_operation = 1ull << 20,
        gc_mutation = 1ull << 21, global_mutation = 1ull << 22, table_mutation = 1ull << 23
    };
    using coverage = std::uint64_t;
    [[nodiscard]] constexpr coverage bit(feature f) noexcept { return static_cast<coverage>(f); }
    inline constexpr coverage calls{(1ull << 7) - 1};
    inline constexpr coverage writes{((1ull << 19) - 1) & ~calls};
    inline constexpr coverage host_operations{bit(feature::wasi_operation) | bit(feature::ros_operation)};
    inline constexpr coverage other_mutations{bit(feature::gc_mutation) | bit(feature::global_mutation) | bit(feature::table_mutation)};
    inline constexpr coverage known_features{calls | writes | host_operations | other_mutations};

    enum class capability : std::uint32_t
    {
        observe_calls = 1u, observe_writes = 2u, observe_host = 4u, observe_state_mutations = 8u,
        read_values = 16u, write_memory = 32u, evaluate = 64u, replace_code = 128u,
        observe_other_instance = 256u
    };
    using capabilities = std::uint32_t;
    [[nodiscard]] constexpr capabilities bit(capability c) noexcept { return static_cast<capabilities>(c); }
    inline constexpr capabilities known_capabilities{511u};

    struct authorization
    {
        // Created only after authentication by the manager. No grant inheritance
        // from node name, identical module bytes or the attached debug session.
        principal subject{}, observer{};
        std::uint64_t session{}, authorization_generation{};
        capabilities granted{};
    };
    enum class admission
    { accepted, invalid_identity, stale_owner, missing_capability, unsupported_coverage, invalid_stream, invalid_policy };

    [[nodiscard]] constexpr admission authorize(authorization const& auth, principal const& live,
                                                coverage requested, coverage producer_support) noexcept
    {
        if(!valid(live) || !valid(auth.subject) || !valid(auth.observer) || !auth.session || !auth.authorization_generation ||
           (auth.granted & ~known_capabilities)) { return admission::invalid_identity; }
        if(auth.subject != live) { return admission::stale_owner; }
        if(!requested || (requested & ~known_features) || (producer_support & ~known_features) ||
           (requested & producer_support) != requested) { return admission::unsupported_coverage; }
        capabilities required{};
        if(requested & calls) { required |= bit(capability::observe_calls); }
        if(requested & writes) { required |= bit(capability::observe_writes); }
        if(requested & host_operations) { required |= bit(capability::observe_host); }
        if(requested & other_mutations) { required |= bit(capability::observe_state_mutations); }
        if(auth.observer != live) { required |= bit(capability::observe_other_instance); }
        return (auth.granted & required) == required ? admission::accepted : admission::missing_capability;
    }

    enum class phase : std::uint8_t
    {
        attempted, entered, returned, trapped, exceptional_exit, tail_replaced,
        committed, no_write, denied, completed, failed
    };
    struct site
    {
        std::uint64_t function{}, function_generation{}, wasm_offset{}, native_variant{};
    };
    struct memory_range
    {
        // Local index and actual backing identity are both retained. Two indices
        // may intentionally alias. Growth keeps the backing identity and changes
        // its generation; no native pointer is stored in this contract.
        std::uint64_t identity{}, generation{}, index{}, offset{}, length{}, size_bytes{};
    };
    struct event
    {
        feature category{feature::call_direct};
        phase outcome{phase::attempted};
        site location{};
        std::uint64_t operation{}, causal_call{}, parent_call{}, related_operation{};
        memory_range memory{};
        std::uint64_t resource{}, resource_generation{}, actual_count{};
        std::int64_t result{}; // Status/errno only, never a guest return value.
        // No values, raw addresses, strings or arbitrary payload storage.
    };

    [[nodiscard]] constexpr bool valid(event const& value) noexcept
    {
        auto const category{bit(value.category)};
        if(!category || (category & (category - 1)) || (category & ~known_features) ||
           !value.operation || !value.location.function_generation || !value.location.native_variant) { return false; }
        if(category & calls)
        {
            switch(value.outcome)
            {
                case phase::attempted: case phase::entered: case phase::returned:
                case phase::trapped: case phase::exceptional_exit: return true;
                case phase::tail_replaced: return value.category == feature::call_tail && value.related_operation != 0;
                default: return false;
            }
        }
        if(category & writes)
        {
            if(!value.memory.identity || !value.memory.generation || !value.causal_call) { return false; }
            bool const atomic{value.category == feature::store_atomic || value.category == feature::rmw_atomic ||
                              value.category == feature::compare_exchange};
            bool const scalar{value.category == feature::store_scalar};
            bool const simd{value.category == feature::store_simd};
            bool const bulk{value.category == feature::memory_copy || value.category == feature::memory_fill ||
                            value.category == feature::memory_init || value.category == feature::memory_initialization ||
                            value.category == feature::host_wasi_write || value.category == feature::host_ros_write};
            if(value.outcome == phase::committed || value.outcome == phase::no_write)
            {
                auto const width{value.memory.length};
                if((atomic || scalar || simd) && (!width || (width & (width - 1)) || width > (simd ? 16u : 8u))) { return false; }
                if(atomic && value.memory.offset % width) { return false; }
            }
            switch(value.outcome)
            {
                case phase::attempted: case phase::trapped: case phase::denied: case phase::failed: return true;
                case phase::no_write:
                    // A failed grow need not describe an existing destination.
                    // CAS mismatch and zero-length bulk operations still perform
                    // their normal bounds checks before reporting no write.
                    return value.category == feature::memory_growth ||
                        ((value.category == feature::compare_exchange || (bulk && value.memory.length == 0)) &&
                         value.memory.offset <= value.memory.size_bytes &&
                         value.memory.length <= value.memory.size_bytes - value.memory.offset);
                case phase::committed:
                    return value.memory.length != 0 && value.memory.offset <= value.memory.size_bytes &&
                        value.memory.length <= value.memory.size_bytes - value.memory.offset;
                default: return false;
            }
        }
        // These categories need their own actual runtime witnesses. Their enum
        // presence never advertises that a producer has implemented them.
        switch(value.outcome)
        {
            case phase::attempted: case phase::completed: case phase::denied: case phase::failed:
                return (category & host_operations) != 0;
            case phase::committed: case phase::trapped: return (category & other_mutations) != 0;
            default: return false;
        }
    }
}
