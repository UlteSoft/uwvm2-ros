/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

// This is an experimental extension, not a member of any standard Wasm opcode
// enumeration. Encoding/signatures follow trcrsired/llvm-project mt-2 commit
// 3b5342c56f38dea910d9ffe1264add1aaa3bcd36, WebAssemblyInstrMemtag.td.
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    inline constexpr ::std::uint_least8_t opcode_prefix{0xfcu};
    inline constexpr ::std::size_t instruction_count{20u};
    inline constexpr ::std::uint_least8_t granule_size_log2{4u};
    inline constexpr ::std::uint_least64_t granule_size{16u};

    enum class opcode : ::std::uint_least32_t
    {
        status = 0x20u, extract = 0x21u, insert = 0x22u,
        tagbits = 0x23u, startbit = 0x24u, copy = 0x25u,
        sub = 0x26u, load = 0x27u, untag = 0x28u,
        untagstore = 0x29u, untagstorez = 0x2au,
        store = 0x2bu, storez = 0x2cu, random = 0x2du,
        randomstore = 0x2eu, randomstorez = 0x2fu,
        add = 0x33u, hint = 0x36u, hintstore = 0x37u,
        hintstorez = 0x38u
    };

    // All instructions have exactly one unsigned LEB32 memory-index immediate.
    // This remains u32 for memory64 and is not an alignment/offset memarg.
    struct memory_immediate
    {
        ::std::uint_least32_t memory_index{};
    };

    enum class address_width : ::std::uint_least8_t { i32 = 32u, i64 = 64u };
    enum class operand_role : ::std::uint_least8_t
    {
        none, address, byte_length, tag, tag_source, address_offset, tag_offset
    };

    struct instruction_descriptor
    {
        opcode operation{};
        char8_t const* mnemonic{};
        ::std::uint_least8_t operand_count{};
        ::std::uint_least8_t result_count{};
        // Roles are in Wasm stack order (the first operand is deepest).
        operand_role operands[4]{};
        bool reads_allocation_tags{};
        bool writes_allocation_tags{};
        bool zeroes_memory{};
        bool randomizes_tag{};
    };

    // Every operand and result has the selected memory's address type, including
    // status, tag counts, tag values and lengths. Length operands are byte counts;
    // published slides and the pinned VM differ for non-granule ranges.
    // Ordinary memory-access checks must cover every intersecting granule.
    inline constexpr instruction_descriptor instructions[instruction_count]{
        {opcode::status, u8"memtag.status", 0u, 1u, {}, false, false, false, false},
        {opcode::extract, u8"memtag.extract", 1u, 1u, {operand_role::address}, false, false, false, false},
        {opcode::insert, u8"memtag.insert", 2u, 1u, {operand_role::address, operand_role::tag}, false, false, false, false},
        {opcode::tagbits, u8"memtag.tagbits", 0u, 1u, {}, false, false, false, false},
        {opcode::startbit, u8"memtag.startbit", 0u, 1u, {}, false, false, false, false},
        {opcode::copy, u8"memtag.copy", 2u, 1u, {operand_role::address, operand_role::tag_source}, false, false, false, false},
        {opcode::sub, u8"memtag.sub", 2u, 1u, {operand_role::address, operand_role::address}, false, false, false, false},
        {opcode::load, u8"memtag.load", 1u, 1u, {operand_role::address}, true, false, false, false},
        {opcode::untag, u8"memtag.untag", 1u, 1u, {operand_role::address}, false, false, false, false},
        {opcode::untagstore, u8"memtag.untagstore", 2u, 1u, {operand_role::address, operand_role::byte_length}, false, true, false, false},
        {opcode::untagstorez, u8"memtag.untagstorez", 2u, 1u, {operand_role::address, operand_role::byte_length}, false, true, true, false},
        {opcode::store, u8"memtag.store", 2u, 0u, {operand_role::address, operand_role::byte_length}, false, true, false, false},
        {opcode::storez, u8"memtag.storez", 2u, 0u, {operand_role::address, operand_role::byte_length}, false, true, true, false},
        {opcode::random, u8"memtag.random", 1u, 1u, {operand_role::address}, false, false, false, true},
        {opcode::randomstore, u8"memtag.randomstore", 2u, 1u, {operand_role::address, operand_role::byte_length}, false, true, false, true},
        {opcode::randomstorez, u8"memtag.randomstorez", 2u, 1u, {operand_role::address, operand_role::byte_length}, false, true, true, true},
        // The encoding/signature is reserved in mt-2; the author's June 2026
        // CG slides, page 30, explicitly leave arithmetic semantics unspecified.
        {opcode::add, u8"memtag.add", 3u, 1u, {operand_role::address, operand_role::address_offset, operand_role::tag_offset}, false, false, false, false},
        {opcode::hint, u8"memtag.hint", 3u, 1u, {operand_role::address, operand_role::tag_source, operand_role::tag_offset}, false, false, false, false},
        {opcode::hintstore, u8"memtag.hintstore", 4u, 1u, {operand_role::address, operand_role::byte_length, operand_role::tag_source, operand_role::tag_offset}, false, true, false, false},
        {opcode::hintstorez, u8"memtag.hintstorez", 4u, 1u, {operand_role::address, operand_role::byte_length, operand_role::tag_source, operand_role::tag_offset}, false, true, true, false},
    };

    [[nodiscard]] inline constexpr instruction_descriptor const* find_instruction(::std::uint_least32_t subopcode) noexcept
    {
        if(subopcode >= 0x20u && subopcode <= 0x2fu) { return instructions + (subopcode - 0x20u); }
        if(subopcode == 0x33u) { return instructions + 16u; }
        if(subopcode >= 0x36u && subopcode <= 0x38u) { return instructions + (17u + subopcode - 0x36u); }
        return nullptr;
    }

    struct typed_signature
    {
        address_width value_width{};
        ::std::uint_least8_t operand_count{};
        ::std::uint_least8_t result_count{};
    };

    [[nodiscard]] inline constexpr typed_signature signature_of(instruction_descriptor const& instruction, address_width width) noexcept
    {
        return {width, instruction.operand_count, instruction.result_count};
    }

    struct tag_geometry
    {
        ::std::uint_least8_t tag_bits{};
        ::std::uint_least8_t start_bit{};
        ::std::uint_least64_t address_mask{};
        ::std::uint_least64_t pointer_tag_mask{};
        ::std::uint_least64_t tag_value_mask{};
    };

    // Geometry is a representation contract, not permission to truncate an
    // effective address. Bounds and immediate-offset overflow are checked before
    // a host/shadow address is formed, independently of these masks.
    inline constexpr tag_geometry memory32_geometry{4u, 28u, 0x0fffffffull, 0xf0000000ull, 0xfull};
    inline constexpr tag_geometry memory64_software_geometry{8u, 56u, 0x00ffffffffffffffull, 0xff00000000000000ull, 0xffull};
    inline constexpr tag_geometry memory64_arm_mte_geometry{4u, 56u, 0xf0ffffffffffffffull, 0x0f00000000000000ull, 0xfull};

    // Reference status values are ABI-observable. "full" must not be used as
    // permission to omit interior granules in this implementation.
    enum class runtime_status : ::std::uint_least8_t { disabled = 0u, software = 1u, software_full = 2u, arm_mte = 3u, arm_mte_irg = 4u };

    inline constexpr char8_t globals_section_name[]{u8"memtag.globals"};
    inline constexpr ::std::uint_least64_t globals_section_version{1u};
    inline constexpr ::std::uint_least32_t globals_memory_index{0u};
    struct global_descriptor
    {
        ::std::uint_least64_t address{};
        ::std::uint_least64_t byte_length{};
    };
    // Payload: ULEB64 version, ULEB64 count, then count repetitions of
    // (ULEB64 address, ULEB64 byte_length). No memory index occurs in the payload.
}
