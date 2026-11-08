// Portable tag-image validation has no dependency on a native owner.
#pragma once
#ifndef UWVM_MODULE
#include <span>
#include <cstddef>
#include <cstdint>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    [[nodiscard]] inline bool valid_tag_image(unsigned address_bits, unsigned tag_bits,
        ::std::uint64_t memory_bytes, ::std::span<::std::byte const> tags) noexcept
    {
        if(address_bits!=32u && address_bits!=64u) { return false; }
        if(tag_bits==0u) { return tags.empty(); }
        if(tag_bits!=(address_bits==32u?4u:8u) ||
           memory_bytes>(::std::uint64_t{1u}<<(address_bits==32u?28u:56u))) { return false; }
        auto const count{memory_bytes/16u+(memory_bytes%16u!=0u)};
        if(count!=tags.size() || (count!=0u && tags.data()==nullptr)) { return false; }
        if(tag_bits==4u)
        { for(auto value:tags) { if(::std::to_integer<unsigned>(value)>15u) { return false; } } }
        return true;
    }
}
