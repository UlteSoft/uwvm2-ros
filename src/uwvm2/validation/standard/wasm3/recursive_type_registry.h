/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <limits>
# include <type_traits>
# include <fast_io.h>
# include <utility>
# include "recursive_type_validation.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::validation::standard::wasm3
{
    // One registry is owned by a VM/linking domain. IDs are stable for its lifetime, cannot be reused,
    // and carry no guest pointer. Module parsing may run concurrently, but callers serialize insertion
    // and exclude it while reading this mutable builder. Execution will consume a frozen snapshot.
    // This class does not itself install a global process singleton or grant guest code registry access.
    class recursive_type_registry
    {
        struct record
        {
            ::uwvm2::parser::wasm::standard::wasm3::type::composite_kind kind{};
            ::std::size_t parent{recursive_validation_details::no_index}, depth{}, jump{};
        };
        using stored_key = ::uwvm2::utils::container::parser_vector<::std::uint_least64_t>;
        ::uwvm2::utils::container::parser_map<stored_key, ::std::size_t, recursive_validation_details::words_less> groups_{};
        ::uwvm2::utils::container::parser_vector<record> records_{};
        [[nodiscard]] static inline ::std::size_t ancestor(::std::size_t index, ::std::size_t depth, auto const& get_record) noexcept
        {
            // Internal precondition: index is a live registry ID and depth <= its stored depth.
            // Each hop strictly decreases depth. A low-bit jump skips a power-of-two suffix of the chain;
            // no recursive calls or quadratic arrays of ancestors are needed.
            while(get_record(index).depth > depth)
            {
                auto const& current{get_record(index)};
                auto const jump{current.jump};
                index = get_record(jump).depth >= depth ? jump : current.parent;
            }
            return index;
        }
    public:
        [[nodiscard]] inline ::std::size_t size() const noexcept { return records_.size(); }
        [[nodiscard]] inline bool matches(::std::size_t from, ::std::size_t to) const noexcept
        {
            if(from >= records_.size() || to >= records_.size()) { return false; }
            // The common exact-type path is one ID comparison, with no walk through recursive structures.
            if(from == to) { return true; }
            auto const& a{records_.index_unchecked(from)};
            auto const& b{records_.index_unchecked(to)};
            return a.kind == b.kind && a.depth >= b.depth && ancestor(from, b.depth, [&](::std::size_t index) noexcept -> record const& { return records_.index_unchecked(index); }) == to;
        }
        template<typename Output>
        [[nodiscard]] inline recursive_type_validation_result validate_and_intern(
            ::uwvm2::parser::wasm::standard::wasm3::type::recursive_type_section const& section,
            Output& module_ids) noexcept
        {
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            try
            {
                ::uwvm2::utils::allocator::resource::recoverable_parser_allocations admission{};
                return intern(section, module_ids);
            }
            catch(::std::bad_alloc const&) { return {recursive_type_validation_error::resource_limit}; }
#else
            return intern(section, module_ids);
#endif
        }
    private:
        template<typename Output>
        inline recursive_type_validation_result intern(
            ::uwvm2::parser::wasm::standard::wasm3::type::recursive_type_section const& section,
            Output& module_ids)
        {
            namespace d = recursive_validation_details;
            basic_recursive_type_context<::uwvm2::utils::container::parser_scratch_vector<recursive_type_record>> checked{};
            auto const validation{validate_core3_type_section(section, checked)};
            if(validation.error != recursive_type_validation_error::ok) { return validation; }
            // A failed validation does not mutate the registry or the caller's mapping. No borrowed section
            // objects are retained: keys replace external references with stable IDs, internal ones with binders.
            auto const count{checked.records.size()};
            auto const original_size{records_.size()};
            constexpr auto max_records{static_cast<::std::size_t>((::std::numeric_limits<::std::ptrdiff_t>::max)()) / sizeof(record)};
            ::uwvm2::utils::container::parser_scratch_vector<::std::size_t> ids{};
            ids.resize(count);
            decltype(groups_) new_groups{};
            ::uwvm2::utils::container::parser_scratch_vector<record> new_records{};
            auto const lookup_record = [&](::std::size_t id) noexcept -> record const&
            { return id < original_size ? records_.index_unchecked(id) : new_records.index_unchecked(id - original_size); };
            for(auto const& group : section.groups)
            {
                auto const first{static_cast<::std::size_t>(group.first_type_index)};
                auto key{d::group_key(group, ids)};
                auto const published{groups_.find(key)};
                auto canonical_first{original_size + new_records.size()};
                bool inserted{};
                if(published != groups_.end()) { canonical_first = published->second; }
                else
                {
                    auto position{new_groups.lower_bound(key)};
                    inserted = position == new_groups.end() || new_groups.key_comp()(key,position->first);
                    if(inserted)
                    {
                        if(group.types.size() > max_records - canonical_first)
                        { return {recursive_type_validation_error::resource_limit, group.binary_offset, first}; }
                        // Private map nodes are transferred to the registry only after all groups fit.
                        stored_key retained{::fast_io::freestanding::from_range_t{}, key};
                        new_groups.emplace_hint(position, ::std::move(retained), canonical_first);
                    }
                    else { canonical_first = position->second; }
                }
                for(::std::size_t local{}; local != group.types.size(); ++local)
                {
                    auto const index{canonical_first + local};
                    ids.index_unchecked(first + local) = index;
                    if(!inserted) { continue; }
                    auto const& type{group.types.index_unchecked(local)};
                    record item{.kind = type.kind, .jump = index};
                    if(!type.supertypes.empty())
                    {
                        item.parent = ids.index_unchecked(type.supertypes.index_unchecked(0));
                        item.depth = lookup_record(item.parent).depth + 1uz;
                        auto const depth{item.depth & (item.depth - 1uz)};
                        item.jump = ancestor(item.parent, depth, lookup_record);
                    }
                    new_records.push_back(item);
                }
            }
            using output_type = ::std::remove_reference_t<decltype(module_ids)>;
            output_type output{};
            if constexpr(::std::is_base_of_v<output_type, decltype(ids)>) { output = ::std::move(ids); }
            else { output = output_type{::fast_io::freestanding::from_range_t{}, ids}; }
            if(!new_records.empty())
            {
                auto const required{original_size + new_records.size()};
                if(required > records_.capacity())
                {
                    // Preserve amortized registry growth across modules. A tightly configured budget
                    // can still admit an exact-fit replacement if the speculative extra space is denied.
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
                    auto const capacity{records_.capacity()};
                    auto const target{::std::max(required, capacity > max_records / 2uz ? max_records : 2uz * capacity)};
                    if(target > required)
                    {
                        try { records_.reserve(target); }
                        catch(::std::bad_alloc const&) { records_.reserve(required); }
                    }
                    else
#endif
                    { records_.reserve(required); }
                }
                for(auto const& item : new_records) { records_.push_back_unchecked(item); }
                groups_.merge(new_groups);
            }
            module_ids = ::std::move(output);
            return {};
        }
    };
}
