// Independent binary/signature oracle: trcrsired/llvm-project mt-2
// 3b5342c56f38dea910d9ffe1264add1aaa3bcd36,
// llvm/test/CodeGen/WebAssembly/memtag-addr{32,64}.ll and
// llvm/lib/Target/WebAssembly/WebAssemblyInstrMemtag.td.
#include <uwvm2/extension/memtag/impl.h>
#include <cstdint>
#include <string_view>

namespace
{
    namespace m = ::uwvm2::extension::memtag;
    struct reference_instruction
    {
        ::std::uint_least32_t subopcode;
        ::std::u8string_view mnemonic;
        unsigned operands;
        unsigned results;
    };
    constexpr reference_instruction reference[]{
        {0x20u, u8"memtag.status", 0, 1}, {0x21u, u8"memtag.extract", 1, 1},
        {0x22u, u8"memtag.insert", 2, 1}, {0x23u, u8"memtag.tagbits", 0, 1},
        {0x24u, u8"memtag.startbit", 0, 1}, {0x25u, u8"memtag.copy", 2, 1},
        {0x26u, u8"memtag.sub", 2, 1}, {0x27u, u8"memtag.load", 1, 1},
        {0x28u, u8"memtag.untag", 1, 1}, {0x29u, u8"memtag.untagstore", 2, 1},
        {0x2au, u8"memtag.untagstorez", 2, 1}, {0x2bu, u8"memtag.store", 2, 0},
        {0x2cu, u8"memtag.storez", 2, 0}, {0x2du, u8"memtag.random", 1, 1},
        {0x2eu, u8"memtag.randomstore", 2, 1}, {0x2fu, u8"memtag.randomstorez", 2, 1},
        {0x33u, u8"memtag.add", 3, 1}, {0x36u, u8"memtag.hint", 3, 1},
        {0x37u, u8"memtag.hintstore", 4, 1}, {0x38u, u8"memtag.hintstorez", 4, 1}
    };
    constexpr bool contract_matches_reference()
    {
        if(m::instruction_count != sizeof(reference) / sizeof(reference[0]) || m::opcode_prefix != 0xfcu) { return false; }
        for(auto const& expected : reference)
        {
            auto const* actual{m::find_instruction(expected.subopcode)};
            if(actual == nullptr || static_cast<::std::uint_least32_t>(actual->operation) != expected.subopcode ||
               actual->mnemonic != expected.mnemonic || actual->operand_count != expected.operands || actual->result_count != expected.results)
            { return false; }
            for(auto width : {m::address_width::i32, m::address_width::i64})
            {
                auto const signature{m::signature_of(*actual, width)};
                if(signature.value_width != width || signature.operand_count != expected.operands || signature.result_count != expected.results)
                { return false; }
            }
        }
        // The extension does not consume any holes, official 0xfc instruction,
        // or malformed subopcode with nonzero upper bits.
        for(::std::uint_least32_t value{}; value != 256u; ++value)
        {
            bool expected{};
            for(auto const& item : reference) { if(item.subopcode == value) { expected = true; } }
            if((m::find_instruction(value) != nullptr) != expected) { return false; }
        }
        return m::find_instruction(0x10020u) == nullptr && m::find_instruction(0xffffffffu) == nullptr;
    }
    constexpr bool effects_match_reference()
    {
        for(auto const& instruction : m::instructions)
        {
            auto const code{static_cast<unsigned>(instruction.operation)};
            bool const store{code == 0x29u || code == 0x2au || code == 0x2bu || code == 0x2cu ||
                             code == 0x2eu || code == 0x2fu || code == 0x37u || code == 0x38u};
            bool const zero{code == 0x2au || code == 0x2cu || code == 0x2fu || code == 0x38u};
            if(instruction.writes_allocation_tags != store || instruction.zeroes_memory != zero ||
               instruction.reads_allocation_tags != (code == 0x27u) ||
               instruction.randomizes_tag != (code == 0x2du || code == 0x2eu || code == 0x2fu)) { return false; }
            if(store && instruction.operands[1] != m::operand_role::byte_length) { return false; }
            for(unsigned index{}; index != instruction.operand_count; ++index)
            {
                if(instruction.operands[index] == m::operand_role::none) { return false; }
            }
        }
        auto const& add{*m::find_instruction(0x33u)};
        return add.operands[0] == m::operand_role::address && add.operands[1] == m::operand_role::address_offset &&
               add.operands[2] == m::operand_role::tag_offset;
    }
    static_assert(contract_matches_reference());
    static_assert(effects_match_reference());
    static_assert(m::granule_size == 16u && m::granule_size_log2 == 4u);
    static_assert(m::memory32_geometry.tag_bits == 4u && m::memory32_geometry.start_bit == 28u);
    static_assert(m::memory32_geometry.address_mask == 268435455ull);
    static_assert(m::memory32_geometry.pointer_tag_mask == 4026531840ull);
    static_assert(m::memory32_geometry.tag_value_mask == 15u);
    static_assert(m::memory64_software_geometry.tag_bits == 8u && m::memory64_software_geometry.start_bit == 56u);
    static_assert(m::memory64_software_geometry.address_mask == 72057594037927935ull);
    static_assert(m::memory64_software_geometry.pointer_tag_mask == 18374686479671623680ull);
    static_assert(m::memory64_software_geometry.tag_value_mask == 255u);
    static_assert(m::memory64_arm_mte_geometry.tag_bits == 4u && m::memory64_arm_mte_geometry.start_bit == 56u);
    static_assert(m::memory64_arm_mte_geometry.pointer_tag_mask == 1080863910568919040ull);
    static_assert((m::memory64_arm_mte_geometry.address_mask ^ m::memory64_arm_mte_geometry.pointer_tag_mask) == 18446744073709551615ull);
    static_assert(m::memory64_arm_mte_geometry.tag_value_mask == 15u);
    static_assert(m::globals_section_version == 1u && m::globals_memory_index == 0u);
    static_assert(::std::u8string_view{m::globals_section_name} == u8"memtag.globals");
    static_assert(m::memory_immediate{0xffffffffu}.memory_index == 0xffffffffu);
}
int main() { return contract_matches_reference() && effects_match_reference() ? 0 : 1; }
