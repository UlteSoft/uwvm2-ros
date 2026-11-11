/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#include <uwvm2/utils/update/module_transition.h>
namespace uwvm2::utils::update
{
    inline constexpr ::std::size_t maximum_migration_ranges{64u}, maximum_migration_globals{64u};
    struct migration_memory_copy
    {
        ::std::uint64_t source_memory{}, destination_memory{}, source_offset{}, destination_offset{}, length{};
    };
    struct migration_global_copy { ::std::uint64_t source_index{}, destination_index{}; };
    struct migration_plan
    {
        ::std::vector<migration_memory_copy> memories{};
        ::std::vector<migration_global_copy> globals{};
    };
    namespace migration_detail
    {
        inline ::std::uint32_t fixed32(unsigned char const* p) noexcept
        {
            ::std::uint32_t n{}; for(unsigned i{}; i < 4u; ++i) { n |= static_cast<::std::uint32_t>(p[i]) << (i * 8u); } return n;
        }
        inline ::std::uint64_t fixed64(unsigned char const* p) noexcept
        {
            ::std::uint64_t n{}; for(unsigned i{}; i < 8u; ++i) { n |= static_cast<::std::uint64_t>(p[i]) << (i * 8u); } return n;
        }
        inline scalar_reset_record const* scalar(::std::span<scalar_reset_record const> records, ::std::uint64_t index) noexcept
        {
            for(auto const& record : records) { if(record.index == index) { return ::std::addressof(record); } }
            return nullptr;
        }
    }
    // UWM1, LE32 range count, LE32 global count, four reserved zero bytes;
    // then exact 5*LE64 memory rows and 2*LE64 global rows. No pointers, names,
    // optional fields, unsigned extensions or trailing bytes are admitted.
    inline status decode_migration(::std::span<unsigned char const> bytes, migration_plan& out) noexcept
    {
        out = {};
        if(bytes.size() < 16u || ::std::memcmp(bytes.data(), "UWM1", 4u) != 0) { return status::invalid_input; }
        auto const memories{migration_detail::fixed32(bytes.data() + 4u)}, globals{migration_detail::fixed32(bytes.data() + 8u)};
        if(memories > maximum_migration_ranges || globals > maximum_migration_globals ||
           (memories == 0u && globals == 0u) || migration_detail::fixed32(bytes.data() + 12u) != 0u ||
           bytes.size() != 16u + static_cast<::std::size_t>(memories) * 40u + static_cast<::std::size_t>(globals) * 16u)
        { return status::invalid_input; }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
#endif
        {
            migration_plan plan{}; plan.memories.reserve(memories); plan.globals.reserve(globals); ::std::size_t pos{16u};
            for(::std::uint32_t i{}; i < memories; ++i)
            {
                migration_memory_copy row{};
                for(auto* field : {&row.source_memory, &row.destination_memory, &row.source_offset, &row.destination_offset, &row.length})
                { *field = migration_detail::fixed64(bytes.data() + pos); pos += 8u; }
                plan.memories.push_back(row);
            }
            for(::std::uint32_t i{}; i < globals; ++i)
            {
                plan.globals.push_back({migration_detail::fixed64(bytes.data() + pos), migration_detail::fixed64(bytes.data() + pos + 8u)});
                pos += 16u;
            }
            out = ::std::move(plan); return status::ok;
        }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        catch(...) { return status::exhausted; }
#endif
    }
    inline status validate_migration(migration_plan const& migration, module_transition_plan const& module,
                                    ::std::span<scalar_reset_record const> scalars) noexcept
    {
        if(!module.reset_state || migration.memories.size() > maximum_migration_ranges || migration.globals.size() > maximum_migration_globals ||
           (migration.memories.empty() && migration.globals.empty())) { return status::invalid_input; }
        ::std::uint64_t total{};
        for(::std::size_t i{}; i < migration.memories.size(); ++i)
        {
            auto const& row{migration.memories[i]};
            if(row.source_memory >= module.memories.size() || row.destination_memory >= module.memories.size() || row.length == 0u)
            { return status::invalid_input; }
            auto const source_size{module.memories[row.source_memory].byte_size}, destination_size{module.memories[row.destination_memory].byte_size};
            if(row.source_offset > source_size || row.length > source_size - row.source_offset ||
               row.destination_offset > destination_size || row.length > destination_size - row.destination_offset ||
               row.length > maximum_image_bytes - total) { return status::scope_mismatch; }
            total += row.length;
            for(::std::size_t j{}; j < i; ++j)
            {
                auto const& other{migration.memories[j]};
                if(row.destination_memory == other.destination_memory && row.destination_offset < other.destination_offset + other.length &&
                   other.destination_offset < row.destination_offset + row.length) { return status::scope_mismatch; }
            }
        }
        for(::std::size_t i{}; i < migration.globals.size(); ++i)
        {
            auto const& row{migration.globals[i]};
            auto const* source{migration_detail::scalar(scalars, row.source_index)};
            auto const* destination{migration_detail::scalar(scalars, row.destination_index)};
            if(!source || !destination || source->immutable || destination->immutable || source->type != destination->type ||
               source->width != destination->width || !scalar_detail::scalar_type(source->type) ||
               source->width != ((source->type == 0x7fu || source->type == 0x7du) ? 4u : 8u)) { return status::scope_mismatch; }
            for(::std::size_t j{}; j < i; ++j)
            { if(migration.globals[j].destination_index == row.destination_index) { return status::scope_mismatch; } }
        }
        return status::ok;
    }
    // Caller holds the real drained old execution generation. The only source
    // views are trusted runtime-owned pure-guest memory and numeric globals;
    // the signed rows cannot introduce a host pointer/handle. All writes below
    // target the already initialized PRIVATE candidate, never the old world.
    template<typename ReadMutableGlobal>
    inline status apply_migration(migration_plan const& migration, module_transition_plan& module,
        ::std::span<scalar_reset_record> scalars, ::std::span<::std::span<unsigned char const> const> old_memories,
        ReadMutableGlobal&& read_mutable_global) noexcept
    {
        if(auto result{validate_migration(migration, module, scalars)}; result != status::ok) { return result; }
        if(old_memories.size() != module.memories.size()) { return status::scope_mismatch; }
        for(::std::size_t i{}; i < old_memories.size(); ++i)
        {
            if(old_memories[i].size() != module.memories[i].byte_size || module.memories[i].bytes.size() != module.memories[i].byte_size ||
               (old_memories[i].size() && old_memories[i].data() == nullptr)) { return status::scope_mismatch; }
        }
        ::std::array<::std::array<unsigned char, 8>, maximum_migration_globals> globals{};
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
#endif
        {
            // Read every approved scalar BEFORE mutating private memory, so an
            // invalid native source read leaves even the private plan unchanged.
            for(::std::size_t i{}; i < migration.globals.size(); ++i)
            {
                auto const* source{migration_detail::scalar(scalars, migration.globals[i].source_index)};
                if(!read_mutable_global(*source, globals[i])) { return status::validation_failed; }
                for(unsigned j{source->width}; j < 8u; ++j) { globals[i][j] = 0u; }
            }
            for(auto const& row : migration.memories)
            {
                ::std::memcpy(module.memories[row.destination_memory].bytes.data() + static_cast<::std::size_t>(row.destination_offset),
                    old_memories[row.source_memory].data() + static_cast<::std::size_t>(row.source_offset), static_cast<::std::size_t>(row.length));
            }
            for(::std::size_t i{}; i < migration.globals.size(); ++i)
            {
                for(auto& scalar : scalars)
                { if(scalar.index == migration.globals[i].destination_index) { scalar.new_bits = globals[i]; break; } }
            }
            return status::ok;
        }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        catch(...) { return status::validation_failed; }
#endif
    }
    // Commit the final candidate CONTENT to PreparedMeasurement as well as the
    // signed migration instructions. Values are canonical little-endian bytes;
    // no native pointer, allocator layout or host byte order enters the hash.
    inline bool measure_migrated_state(module_transition_plan const& module,
        ::std::span<scalar_reset_record const> scalars, digest& result) noexcept
    {
        if(!module.reset_state || module.memories.size() > 16u || scalars.size() > maximum_functions) { return false; }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
#endif
        {
            ::std::size_t total{};
            for(auto const& memory : module.memories)
            {
                if(memory.bytes.size() != memory.byte_size || memory.byte_size > maximum_image_bytes - total) { return false; }
                total += memory.byte_size;
            }
            constexpr char domain[]{"uwvm.module-migration.state.v2"};
            ::std::vector<unsigned char> bytes; bytes.reserve(total + 2048u);
            for(char c : domain) { bytes.push_back(static_cast<unsigned char>(c)); }
            append_u64(bytes, module.memories.size());
            for(auto const& memory : module.memories)
            {
                append_u64(bytes, memory.index); append_u64(bytes, memory.byte_size);
                bytes.insert(bytes.end(), memory.bytes.begin(), memory.bytes.end());
                if((!memory.tagged && !memory.allocation_tags.empty()) ||
                   (memory.tagged && memory.allocation_tags.size()!=memory.byte_size/16u)) { return false; }
                append_u64(bytes, memory.tagged); append_u64(bytes, memory.allocation_tags.size());
                for(auto tag:memory.allocation_tags) { if(tag>15u) { return false; } bytes.push_back(tag); }
            }
            append_u64(bytes, scalars.size());
            for(auto const& scalar : scalars)
            {
                if(scalar.width != 4u && scalar.width != 8u) { return false; }
                append_u64(bytes, scalar.index); bytes.push_back(scalar.type); bytes.push_back(scalar.width); bytes.push_back(scalar.immutable);
                bytes.insert(bytes.end(), scalar.new_bits.begin(), scalar.new_bits.begin() + scalar.width);
            }
            return hash(bytes, result);
        }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        catch(...) { return false; }
#endif
    }
}
