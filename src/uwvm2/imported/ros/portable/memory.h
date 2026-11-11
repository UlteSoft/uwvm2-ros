// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "wire.h"
#include <limits>

namespace uwvm2::imported::ros::portable
{
    struct memory_shape
    {
        ::std::uint64_t bytes{};
        ::std::uint32_t count{}, index{};
        bool shared{};
    };
    struct memory_span { ::std::uint64_t offset{}, bytes{}; };
    constexpr bool contains(memory_shape const& memory,memory_span span) noexcept
    {
        constexpr auto address_limit{::std::numeric_limits<::std::size_t>::max()};
        return span.offset<=memory.bytes && span.bytes<=memory.bytes-span.offset &&
            span.offset<=address_limit && span.bytes<=address_limit-span.offset;
    }
    constexpr bool overlaps(memory_span first,memory_span second) noexcept
    {
        if(!first.bytes || !second.bytes) { return false; }
        return first.offset<=second.offset ? second.offset-first.offset<first.bytes : first.offset-second.offset<second.bytes;
    }
    constexpr error valid_memory(memory_shape const& memory,::std::uint32_t selector) noexcept
    {
        // Shared instances use Runtime-owned atomic snapshots; this check only
        // validates shape/ranges and never authorizes direct native pointers.
        if(memory.count!=1) { return error::unsupported; }
        return selector==0 && memory.index==0 ? error::ok : error::malformed;
    }
    // These checks precede any output write or native operation. Even on a size
    // error the length word must not alias an input or the proposed output. The
    // Host obtains memory_shape from the genuine active caller descriptor and
    // separately verifies read/write rights; Guest-supplied shapes are invalid.
    constexpr error preflight_open(memory_shape const& memory,::std::uint32_t selector,
                                   memory_span alias,::std::uint64_t handle_out) noexcept
    {
        if(auto result{valid_memory(memory,selector)};result!=error::ok) { return result; }
        memory_span result{handle_out,8};
        if(!alias.bytes || alias.bytes>255 || !contains(memory,alias) || !contains(memory,result) || overlaps(alias,result))
        { return error::malformed; }
        return error::ok;
    }
    constexpr error preflight_output(memory_shape const& memory,::std::uint32_t selector,
                                     memory_span output,::std::uint64_t length_out,::std::size_t required,
                                     memory_span request = {}) noexcept
    {
        if(auto result{valid_memory(memory,selector)};result!=error::ok) { return result; }
        memory_span length{length_out,8};
        if(!contains(memory,output) || !contains(memory,length) || !contains(memory,request) ||
           overlaps(output,length) || overlaps(request,length) || overlaps(request,output)) { return error::malformed; }
        return output.bytes<required ? error::too_small : error::ok;
    }
}
