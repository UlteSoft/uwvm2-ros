/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#include "scalar_state.h"

namespace uwvm2::utils::update
{
    struct memory_reset_record
    {
        ::std::uint64_t index{};
        ::std::size_t byte_size{};
        // Private complete initial memory, built before READY. Preserve plans
        // retain only byte_size and never allocate or publish reset bytes.
        ::std::vector<unsigned char> bytes{};
        // Prepared alongside reset bytes, before READY. Never an implicit copy
        // of the previous epoch's allocation tags. PRESERVE has no tag image.
        bool tagged{};
        ::std::vector<unsigned char> allocation_tags{};
    };
    struct active_data_record
    {
        ::std::uint32_t flags{}, memory_index{};
        ::std::uint64_t offset{};
        ::std::size_t payload_offset{}, payload_size{};
    };
    struct module_transition_plan
    {
        bool reset_state{};
        ::std::vector<function_body> bodies{};
        ::std::vector<scalar_reset_record> scalars{};
        ::std::vector<memory_reset_record> memories{};
        ::std::vector<active_data_record> data_segments{};
    };
    namespace module_transition_detail
    {
        inline status memories(::std::span<unsigned char const> bytes, bool reset,
                               ::std::vector<memory_reset_record>& records)
        {
            ::std::size_t pos{}, total{}; ::std::uint32_t count{};
            if(!u32(bytes,pos,count))return status::validation_failed;
            if(count>16u)return status::exhausted;
            for(::std::uint32_t i{};i<count;++i)
            {
                ::std::uint32_t flags{},minimum{},maximum{};
                if(!u32(bytes,pos,flags))return status::validation_failed;
                // Memory64/shared/custom pages have a different transition ABI.
                if(flags>1u)return status::unsupported;
                if(!u32(bytes,pos,minimum) || minimum>65536u ||
                   ((flags&1u) && (!u32(bytes,pos,maximum) || maximum<minimum || maximum>65536u)))
                    return status::validation_failed;
                auto const size=static_cast<::std::uint64_t>(minimum)*65536u;
                // A valid module can exceed the private reset-image budget.
                // It is not an unsupported Wasm feature or a compiler failure.
                if(size>maximum_image_bytes-total)return status::exhausted;
                total+=static_cast<::std::size_t>(size);
                memory_reset_record record{i,static_cast<::std::size_t>(size),{}};
                if(reset)record.bytes.resize(record.byte_size);
                records.push_back(::std::move(record));
            }
            return pos==bytes.size()?status::ok:status::validation_failed;
        }
        inline status active_data(::std::span<unsigned char const> bytes,bool apply,
            ::std::vector<memory_reset_record>& memories,::std::vector<active_data_record>& records)
        {
            ::std::size_t pos{};::std::uint32_t count{};
            if(!u32(bytes,pos,count))return status::validation_failed;
            if(count>4096u)return status::exhausted;
            records.reserve(count);
            for(::std::uint32_t i{};i<count;++i)
            {
                active_data_record record{};::std::uint64_t raw{};::std::uint32_t length{};
                if(!u32(bytes,pos,record.flags))return status::validation_failed;
                if(record.flags!=0u && record.flags!=2u)return status::unsupported;
                if((record.flags==2u && !u32(bytes,pos,record.memory_index)) ||
                   record.memory_index>=memories.size() || pos==bytes.size())return status::validation_failed;
                if(bytes[pos++]!=0x41u)return status::unsupported;
                if(!scalar_detail::signed_integer(bytes,pos,32u,raw) || pos==bytes.size() || bytes[pos++]!=0x0bu ||
                   !u32(bytes,pos,length) || length>bytes.size()-pos)return status::validation_failed;
                record.offset=raw;auto& memory=memories[record.memory_index];
                if(raw>memory.byte_size || length>memory.byte_size-static_cast<::std::size_t>(raw))return status::validation_failed;
                if(apply && length)
                {
                    if(memory.bytes.size()!=memory.byte_size)return status::invalid_state;
                    // Actual owned extents checked above; later active segments
                    // overwrite earlier overlaps exactly as Wasm initialization.
                    ::std::memcpy(memory.bytes.data()+static_cast<::std::size_t>(raw),bytes.data()+pos,length);
                }
                record.payload_offset=pos;record.payload_size=length;
                pos+=length;records.push_back(record);
            }
            return pos==bytes.size()?status::ok:status::validation_failed;
        }
    }

    // A complete, same-layout single-module transition. Unchanged sections are
    // the already validated actual module's exact bytes. New function bodies
    // still pass the real runtime validator/compiler; globals and active data
    // are completely parsed below and never executed as guest initializers.
    inline status compare_module_transition(::std::span<unsigned char const> base,
        ::std::span<unsigned char const> target, ::std::span<::std::uint64_t const> scopes,
        bool reset_state, module_transition_plan& out)
    {
        out = {};
        if(scopes.empty() || scopes.size() > maximum_functions) { return status::invalid_input; }
        for(::std::size_t i{1}; i < scopes.size(); ++i) { if(scopes[i] <= scopes[i-1u]) { return status::invalid_input; } }
        ::std::vector<section> old_sections, new_sections; ::std::vector<function_body> old_bodies;
        module_transition_plan plan{}; plan.reset_state = reset_state;
        if(!split(base, old_sections, old_bodies) || !split(target, new_sections, plan.bodies) ||
           old_sections.size() != new_sections.size() || old_bodies.size() != plan.bodies.size()) { return status::validation_failed; }
        if(!scalar_detail::closed_immutable_aliases(base, old_sections, old_bodies) ||
           !scalar_detail::closed_immutable_aliases(target, new_sections, plan.bodies)) { return status::unsupported; }
        ::std::vector<scalar_detail::value> old_globals, new_globals;
        ::std::span<unsigned char const> old_data{}, new_data{};
        bool seen_global{}, seen_memory{}, seen_data{}, seen_count{}, changed{}; ::std::uint32_t data_count{};
        for(::std::size_t i{}; i < old_sections.size(); ++i)
        {
            auto const& old{old_sections[i]}; auto const& next{new_sections[i]};
            if(old.id != next.id) { return status::scope_mismatch; }
            bool const different{old.size != next.size || ::std::memcmp(base.data() + old.offset, target.data() + next.offset, old.size) != 0};
            if(old.id != 6u && old.id != 10u && old.id != 11u && different) { return status::scope_mismatch; }
            if(!reset_state && old.id != 10u && different) { return status::scope_mismatch; }
            changed = changed || different;
            ::std::span<unsigned char const> a{}, b{};
            if(!scalar_detail::payload(base, old, a) || !scalar_detail::payload(target, next, b)) { return status::validation_failed; }
            if(old.id == 2u || old.id == 9u)
            { if(!scalar_detail::empty_counted_section(a)) { return status::unsupported; } }
            if(old.id == 6u)
            {
                if(seen_global || !scalar_detail::globals(a, old_globals) || !scalar_detail::globals(b, new_globals)) { return status::unsupported; }
                seen_global = true;
            }
            if(old.id == 5u)
            {
                if(seen_memory)return status::validation_failed;
                if(auto result=module_transition_detail::memories(b,reset_state,plan.memories);result!=status::ok)return result;
                seen_memory = true;
            }
            if(old.id == 11u)
            { if(seen_data) { return status::validation_failed; } seen_data = true; old_data = a; new_data = b; }
            if(old.id == 12u)
            {
                ::std::size_t pos{};
                if(seen_count || !u32(b, pos, data_count) || pos != b.size()) { return status::validation_failed; }
                seen_count = true;
            }
        }
        if(old_globals.size() != new_globals.size()) { return status::scope_mismatch; }
        if(scopes.size() != plan.bodies.size() + new_globals.size()) { return status::scope_mismatch; }
        for(::std::size_t i{}; i < plan.bodies.size(); ++i) { if(scopes[i] != i) { return status::scope_mismatch; } }
        for(::std::size_t i{}; i < new_globals.size(); ++i)
        {
            auto const& old{old_globals[i]}; auto const& next{new_globals[i]};
            if(old.type != next.type || old.mutability != next.mutability) { return status::scope_mismatch; }
            bool const immutable{next.mutability == 0u};
            if(!::std::binary_search(scopes.begin(), scopes.end(), (immutable ? immutable_global_tag : global_reset_tag) | i))
            { return status::scope_mismatch; }
            // An explicit reset installs every target initializer, even when
            // the two files match and business execution changed mutable state.
            if(reset_state) { plan.scalars.push_back({i, next.type, next.width, old.bits, next.bits, immutable}); }
        }
        if(seen_data)
        {
            ::std::vector<active_data_record> old_records;
            if(auto result=module_transition_detail::active_data(old_data,false,plan.memories,old_records);result!=status::ok)return result;
            if(auto result=module_transition_detail::active_data(new_data,reset_state,plan.memories,plan.data_segments);result!=status::ok)return result;
            if(old_records.size() != plan.data_segments.size()) { return status::scope_mismatch; }
            for(::std::size_t i{}; i < old_records.size(); ++i)
            {
                auto const& a{old_records[i]}; auto const& b{plan.data_segments[i]};
                if(a.flags != b.flags || a.memory_index != b.memory_index) { return status::scope_mismatch; }
                plan.data_segments[i].payload_offset += static_cast<::std::size_t>(new_data.data() - target.data());
            }
        }
        if(seen_count && data_count != plan.data_segments.size()) { return status::validation_failed; }
        // A signed reset of the identical image is meaningful. Preserve of the
        // identical image has no transition to perform.
        if(!reset_state && !changed) { return status::invalid_input; }
        out = ::std::move(plan); return status::ok;
    }
}
