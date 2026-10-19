/*************************************************************
 * UWVM2 ROS Runtime Extension. APL-2.0.                     *
 *************************************************************/
#pragma once
// Worker-side formatting only. Shared C++17 wire headers do not include this.
#include <string>
#include <string_view>
#include <memory>
#ifndef UWVM_MODULE
#include <fast_io.h>
#endif
#ifndef UWVM_MODULE
#include <fast_io_unit/string.h>
#endif
#include <cstddef>
#include <limits>
#include <stdexcept>

namespace uwvm2::imported::ros
{
    // Borrowed for this formatting expression; byte-for-byte legacy JSON escaping.
    struct json_quoted { ::std::string_view text; };
    inline ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char,json_quoted>, json_quoted value)
    {
        if(value.text.size() > (::std::numeric_limits<::std::size_t>::max()-2u)/6u)
        {
#if defined(__cpp_exceptions)
            throw ::std::length_error{"ROS JSON string exceeds representable output size"};
#else
            ::fast_io::fast_terminate();
#endif
        }
        ::std::size_t size{2};
        for(unsigned char byte : value.text)
        { size += byte < 0x20 ? 6u : byte == '\"' || byte == '\\' ? 2u : 1u; }
        return size;
    }
    inline char* print_reserve_define(::fast_io::io_reserve_type_t<char,json_quoted>, char* out, json_quoted value) noexcept
    {
        constexpr char hex[]{"0123456789abcdef"};
        *out++='\"';
        for(unsigned char byte : value.text)
        {
            if(byte=='\"' || byte=='\\') { *out++='\\';*out++=static_cast<char>(byte); }
            else if(byte<0x20)
            { *out++='\\';*out++='u';*out++='0';*out++='0';*out++=hex[byte>>4];*out++=hex[byte&15]; }
            else { *out++=static_cast<char>(byte); }
        }
        *out++='\"';return out;
    }
}
