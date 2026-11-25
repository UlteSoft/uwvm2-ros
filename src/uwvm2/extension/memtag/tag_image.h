// Portable tag-image validation has no dependency on a native owner.
#pragma once
#ifndef UWVM_MODULE
#include <array>
#include <span>
#include <cstddef>
#include <cstdint>
#include "allocation_neighborhood.h"
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
        if(tag_bits!=4u ||
           memory_bytes>(::std::uint64_t{1u}<<(address_bits==32u?28u:48u))) { return false; }
        auto const count{memory_bytes/16u+(memory_bytes%16u!=0u)};
        if(count!=tags.size() || (count!=0u && tags.data()==nullptr)) { return false; }
        for(auto value:tags) { if(::std::to_integer<unsigned>(value)>15u) { return false; } }
        return true;
    }

    // A memory's directory revision and record count occupy two formerly zero
    // words. The payload is the complete shadow followed by ordered records.
    inline constexpr ::std::uint64_t checkpoint_directory_revision{1u};
    inline constexpr ::std::size_t checkpoint_extent_bytes{19u};

    inline void encode_checkpoint_extent(::std::span<::std::byte,checkpoint_extent_bytes> row,
                                         allocation_extent const& item) noexcept
    {
        for(unsigned i{}; i != 8u; ++i)
        {
            row[i] = static_cast<::std::byte>(item.first >> (8u*i));
            row[8u+i] = static_cast<::std::byte>(item.last >> (8u*i));
        }
        row[16] = static_cast<::std::byte>(item.tag);
        row[17] = static_cast<::std::byte>(item.parent);
        row[18] = static_cast<::std::byte>(unsigned(item.active) | (unsigned(item.permanent) << 1u));
    }

    struct checkpoint_tag_image
    {
        ::std::span<::std::byte const> tags{}, records{};

        template<typename Visitor>
        [[nodiscard]] bool visit_allocations(Visitor visitor) const noexcept
        {
            auto rows{records};
            if(rows.size()%checkpoint_extent_bytes || (!rows.empty() && !rows.data())) { return false; }
            while(!rows.empty())
            {
                allocation_extent item{};
                for(unsigned i{}; i != 8u; ++i)
                {
                    item.first |= ::std::to_integer<::std::uint64_t>(rows[i]) << (8u*i);
                    item.last |= ::std::to_integer<::std::uint64_t>(rows[8u+i]) << (8u*i);
                }
                item.tag = ::std::to_integer<::std::uint8_t>(rows[16]);
                item.parent = ::std::to_integer<::std::uint8_t>(rows[17]);
                auto const flags{::std::to_integer<unsigned>(rows[18])};
                if(flags != 0u && flags != 1u && flags != 3u) { return false; }
                item.active = (flags & 1u) != 0u;
                item.permanent = (flags & 2u) != 0u;
                if(!visitor(item)) { return false; }
                rows = rows.subspan(checkpoint_extent_bytes);
            }
            return true;
        }

        [[nodiscard]] bool valid(unsigned address_bits, unsigned tag_bits, ::std::uint64_t memory_bytes,
                                 bool hardware = false) const noexcept
        {
            if(!valid_tag_image(address_bits,tag_bits,memory_bytes,tags)) { return false; }
            if(records.empty()) { return true; }
            if(!tag_bits) { return false; }
            unsigned const mask{hardware || tag_bits == 4u ? 15u : 255u};
            ::std::uint64_t next{};
            ::std::array<::std::uint64_t,256u> color_ends{};
            return visit_allocations([&](allocation_extent const& item) noexcept
            {
                unsigned const color{item.tag & mask};
                if(!color || (tag_bits == 4u && item.tag > 15u) || item.parent != 0u ||
                   item.first < next || item.last < item.first || item.last >= tags.size() ||
                   (!item.permanent && item.last >= memory_bytes/granule_size)) { return false; }
                // Reservations do not yet appear in the shadow. Check prior
                // registrations of this color as well as neighboring raw tags.
                if(color_ends[color] && allocations_near(color_ends[color]-1u,color_ends[color]-1u,item.first,item.last))
                { return false; }
                next = item.last+1u;
                color_ends[color] = next;
                auto const expected{static_cast<::std::byte>(item.active ? item.tag : item.parent)};
                for(auto i{item.first}; i != next; ++i)
                { if(tags[static_cast<::std::size_t>(i)] != expected) { return false; } }
                return visit_allocation_neighbors(item.first,item.last,tags.size(),[&](::std::uint64_t i) noexcept
                { return (::std::to_integer<unsigned>(tags[static_cast<::std::size_t>(i)]) & mask) != color; });
            });
        }
    };

    // Bound the record suffix before converting its count to a native extent.
    [[nodiscard]] inline bool split_checkpoint_tag_image(unsigned tag_bits, ::std::uint64_t revision,
        ::std::uint64_t count, ::std::span<::std::byte const> payload, checkpoint_tag_image& out) noexcept
    {
        if(revision > checkpoint_directory_revision || (!tag_bits && revision) || (!revision && count) ||
           count > payload.size()/checkpoint_extent_bytes || (!payload.empty() && !payload.data())) { return false; }
        auto const bytes{static_cast<::std::size_t>(count)*checkpoint_extent_bytes};
        out = {payload.first(payload.size()-bytes),payload.last(bytes)};
        return true;
    }
}
