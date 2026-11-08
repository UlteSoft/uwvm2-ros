// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace uwvm2::runtime::compiler::llvm_jit::details
{
    // Validate the complete unwind instruction area before the native unwinder
    // can read it. The caller authenticates the containing relocated section and,
    // for a generic entry, the separately owned personality destination.
    // This is an unwind bytecode extent check, not a generic LSDA parser.
    [[nodiscard]] inline bool arm_ehabi_unwind_extent_valid(void const* entry,
        ::std::size_t available, bool in_index) noexcept
    {
        if(available < 4u) { return false; }
        ::std::uint32_t first{};
        ::std::memcpy(&first, entry, 4u);
        if((first & 0x80000000u) != 0u)
        {
            // EHABI 7.3/10.2: bits 28..30 are zero; only personalities 0..2
            // are defined. The long formats carry a count of extra words.
            if((first & 0xf0000000u) != 0x80000000u) { return false; }
            auto const personality{(first >> 24u) & 15u};
            if(personality > 2u) { return false; }
            auto const extra{personality == 0u ? 0u : (first >> 16u) & 255u};
            if(in_index && extra != 0u) { return false; }
            return available >= 4u + 4u * extra;
        }
        if(in_index || available < 8u) { return false; }
        // The LLVM/GNU generic model starts with PREL31(personality), then
        // a size byte and three instructions, followed by that many words.
        ::std::uint32_t header{};
        ::std::memcpy(&header, static_cast<unsigned char const*>(entry) + 4u, 4u);
        return available >= 8u + 4u * (header >> 24u);
    }
}
