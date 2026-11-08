// Shared deterministic tag palette for CLI and debugger memory previews.
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <cstddef>
# include <cstdint>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    struct rgb_color { unsigned red{}, green{}, blue{}; };
    // Odd multiplication permutes all 256 hues; low tag numbers stay separated.
    // Numeric/hex tags remain authoritative: colour is an additional cue.
    [[nodiscard]] inline constexpr rgb_color color_for_tag(::std::uint8_t tag) noexcept
    {
        auto const hue{static_cast<unsigned>(static_cast<::std::uint8_t>(static_cast<unsigned>(tag) * 137u)) * 6u};
        auto const rise{64u + (hue % 256u) * 191u / 255u};
        auto const fall{319u - rise};
        switch(hue / 256u)
        {
            case 0u: return {255u,rise,64u};
            case 1u: return {fall,255u,64u};
            case 2u: return {64u,255u,rise};
            case 3u: return {64u,fall,255u};
            case 4u: return {rise,64u,255u};
            default: return {255u,64u,fall};
        }
    }
    [[nodiscard]] inline constexpr ::std::array<char,7u> hex_color_for_tag(::std::uint8_t tag) noexcept
    {
        constexpr char digits[]{"0123456789abcdef"};
        auto const rgb{color_for_tag(tag)};
        return {'#',digits[rgb.red >> 4u],digits[rgb.red & 15u],
            digits[rgb.green >> 4u],digits[rgb.green & 15u],digits[rgb.blue >> 4u],digits[rgb.blue & 15u]};
    }
    struct ansi_color { ::std::array<char8_t,32u> bytes{}; ::std::size_t size{}; };
    [[nodiscard]] inline constexpr ansi_color ansi_color_for_tag(::std::uint8_t tag) noexcept
    {
        ansi_color result{};
        for(auto ch : u8"\033[38;2;") { if(ch) { result.bytes[result.size++] = ch; } }
        auto decimal{[&](unsigned value) constexpr noexcept
        {
            if(value >= 100u) { result.bytes[result.size++] = static_cast<char8_t>(u8'0' + value / 100u); }
            if(value >= 10u) { result.bytes[result.size++] = static_cast<char8_t>(u8'0' + value / 10u % 10u); }
            result.bytes[result.size++] = static_cast<char8_t>(u8'0' + value % 10u);
        }};
        auto const rgb{color_for_tag(tag)};
        decimal(rgb.red); result.bytes[result.size++] = u8';';
        decimal(rgb.green); result.bytes[result.size++] = u8';';
        decimal(rgb.blue); result.bytes[result.size++] = u8'm';
        return result;
    }
}
