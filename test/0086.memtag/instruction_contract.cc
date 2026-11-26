// Operation/signature oracle (not the obsolete binary encoding): trcrsired/llvm-project mt-2
// 3b5342c56f38dea910d9ffe1264add1aaa3bcd36,
// llvm/test/CodeGen/WebAssembly/memtag-addr{32,64}.ll and
// llvm/lib/Target/WebAssembly/WebAssemblyInstrMemtag.td.
#include <uwvm2/extension/memtag/impl.h>
#include <uwvm2/extension/adm/types.h>
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
        {0x37u, u8"memtag.hintstore", 4, 1}, {0x38u, u8"memtag.hintstorez", 4, 1},
        {0x40u, u8"memtag.claim", 2, 1},
        {0x41u, u8"memtag.claimz", 2, 1},
        {0x42u, u8"memtag.reserve", 2, 1},
        {0x43u, u8"memtag.activate", 2, 1},
        {0x44u, u8"memtag.release", 2, 1}
    };
    constexpr bool contract_matches_reference()
    {
        if(m::instruction_count != sizeof(reference) / sizeof(reference[0]) || m::opcode_prefix != 0xffu) { return false; }
        for(auto const& expected : reference)
        {
            auto const* actual{m::find_instruction(expected.subopcode)};
            if(actual == nullptr || static_cast<::std::uint_least32_t>(actual->operation) != expected.subopcode ||
               m::encode_opcode(actual->operation) != (0x01010000u | expected.subopcode) ||
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
                             code == 0x2eu || code == 0x2fu || code == 0x37u || code == 0x38u || (code >= 0x40u && code <= 0x44u)};
            bool const zero{code == 0x2au || code == 0x2cu || code == 0x2fu || code == 0x38u || code == 0x41u};
            if(instruction.writes_allocation_tags != store || instruction.zeroes_memory != zero ||
               instruction.reads_allocation_tags != (code == 0x27u || (code >= 0x40u && code <= 0x44u)) ||
               instruction.randomizes_tag != (code == 0x2du || code == 0x2eu || code == 0x2fu || (code >= 0x40u && code <= 0x42u))) { return false; }
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
    namespace adm = ::uwvm2::extension::adm;
    static_assert(adm::memory_type_flag == 0x10u && m::memory_type_flag == 0x20u);
    static_assert(m::adm_address_mask(m::address_width::i32,0u) == UINT32_MAX);
    static_assert(m::adm_address_mask(m::address_width::i64,0u) == UINT64_MAX);

    template<typename Address>
    constexpr bool field_matches(m::tag_geometry geometry, unsigned minimum, unsigned start,
                                 Address address_mask, Address tag_mask, Address raw, unsigned tag)
    {
        auto const width{sizeof(Address) == 8u ? m::address_width::i64 : m::address_width::i32};
        if(geometry.tag_bits != 4u || geometry.tag_value_mask != 15u || geometry.start_bit != start ||
           geometry.address_mask != address_mask || geometry.pointer_tag_mask != tag_mask) { return false; }
        for(unsigned k{}; k < static_cast<unsigned>(width); ++k)
        {
            if(adm::valid_tag_field(width == m::address_width::i64,k,start) != (k >= minimum)) { return false; }
        }
        // The same ADM16 pointer has the same dynamic address in every layout.
        // Tag operations preserve the metadata in the other twelve ignored bits.
        m::shadow_view<Address> view{nullptr,0,0,nullptr,nullptr,static_cast<Address>(m::adm_address_mask(width,16u)),start};
        return view.canonical_address(raw) == 0x1234u && view.pointer_tag(raw) == tag &&
               view.clear_tag(raw) == (raw & ~tag_mask) && view.with_tag(raw,31u) == (raw | tag_mask) &&
               view.with_tag(view.clear_tag(raw),tag) == raw;
    }
    constexpr ::std::uint32_t raw32{0xbda51234u};
    constexpr ::std::uint64_t raw64{UINT64_C(0xbda5000000001234)};
    static_assert(field_matches(m::memory32_geometry,4,28,UINT32_C(0x0fffffff),UINT32_C(0xf0000000),raw32,0xb));
    static_assert(field_matches(m::memory32_arm_mte_geometry,8,24,UINT32_C(0x00ffffff),UINT32_C(0x0f000000),raw32,0xd));
    static_assert(field_matches(m::memory32_intel_mtt_geometry,7,25,UINT32_C(0x01ffffff),UINT32_C(0x1e000000),raw32,0xe));
    static_assert(field_matches(m::memory64_software_geometry,4,60,UINT64_C(0x0fffffffffffffff),UINT64_C(0xf000000000000000),raw64,0xb));
    static_assert(field_matches(m::memory64_arm_mte_geometry,8,56,UINT64_C(0x00ffffffffffffff),UINT64_C(0x0f00000000000000),raw64,0xd));
    static_assert(field_matches(m::memory64_intel_mtt_geometry,7,57,UINT64_C(0x01ffffffffffffff),UINT64_C(0x1e00000000000000),raw64,0xe));
    static_assert(m::encoding_version == 1u && m::linear_encoding_domain == 1u);
    static_assert(m::linear_globals_section_version == 2u);
    static_assert(::std::u8string_view{m::linear_globals_section_name} == u8"uwvm.memtag.linear.globals");
    static_assert(::std::u8string_view{m::obsolete_globals_section_name} == u8"memtag.globals");
    static_assert(m::memory_immediate{0xffffffffu}.memory_index == 0xffffffffu);
}
int main() { return contract_matches_reference() && effects_match_reference() ? 0 : 1; }
