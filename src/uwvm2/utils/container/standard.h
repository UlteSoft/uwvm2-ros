/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/utils/allocator/fast_io_strict/std_allocator.h>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <charconv>
#include <system_error>
#include <string_view>
#include <concepts>
#include <limits>
#include <fast_io_unit/string.h>

namespace uwvm2::utils::allocator::fast_io_strict
{
    template<::std::integral Char>
    [[nodiscard]] inline constexpr ::std::true_type print_borrowed_scatter_source(
        ::fast_io::io_reserve_type_t<Char,
            ::std::basic_string<Char,::std::char_traits<Char>,native_std_allocator<Char>>>) noexcept
    { return {}; }
    // Our allocator is an associated namespace of the string. This borrowed
    // alias never allocates, mutates the string, or outlives the print call.
    template<::std::integral Char>
    [[nodiscard]] inline constexpr ::fast_io::basic_io_scatter_t<Char> print_alias_define(
        ::fast_io::io_alias_t,
        ::std::basic_string<Char, ::std::char_traits<Char>, native_std_allocator<Char>> const& value) noexcept
    { return {value.data(),value.size()}; }
}

namespace uwvm2::utils::container
{
    // Preserve the standard container interface while all backing allocation
    // uses the existing recoverable fast_io global allocator. Explicit resource
    // allocators remain explicit template arguments and retain their budgets.
    template<typename T> using standard_allocator = ::uwvm2::utils::allocator::fast_io_strict::native_std_allocator<T>;
    template<typename T, typename Alloc = standard_allocator<T>> using std_vector = ::std::vector<T,Alloc>;
    template<typename T, typename Alloc = standard_allocator<T>> using std_deque = ::std::deque<T,Alloc>;
    template<typename T, typename Alloc = standard_allocator<T>> using std_list = ::std::list<T,Alloc>;
    template<typename Key, typename Value, typename Compare = ::std::less<Key>,
        typename Alloc = standard_allocator<::std::pair<Key const,Value>>>
    using std_map = ::std::map<Key,Value,Compare,Alloc>;
    template<typename Key, typename Compare = ::std::less<Key>, typename Alloc = standard_allocator<Key>>
    using std_set = ::std::set<Key,Compare,Alloc>;
    template<typename Char> using standard_string = ::std::basic_string<Char,::std::char_traits<Char>,standard_allocator<Char>>;
    using std_string = standard_string<char>;
    using std_u8string = standard_string<char8_t>;
    using std_wstring = standard_string<wchar_t>;
    using std_u16string = standard_string<char16_t>;
    using std_u32string = standard_string<char32_t>;
    using std_istringstream = ::std::basic_istringstream<char,::std::char_traits<char>,standard_allocator<char>>;
    using std_ostringstream = ::std::basic_ostringstream<char,::std::char_traits<char>,standard_allocator<char>>;
    template<typename Text> [[nodiscard]] inline std_string copy_string(Text const& text)
    { return {text.data(),text.size()}; }
    using ostring_ref = ::fast_io::basic_ostring_ref_std<char,::std::char_traits<char>,standard_allocator<char>>;
    using u8ostring_ref = ::fast_io::basic_ostring_ref_std<char8_t,::std::char_traits<char8_t>,standard_allocator<char8_t>>;
    template<typename... Args> [[nodiscard]] inline constexpr std_string concat(Args&&... args)
    { return ::fast_io::basic_general_concat_compiler_constant_checked_entry<false,char,std_string>(::std::forward<Args>(args)...); }
    template<typename... Args> [[nodiscard]] inline constexpr std_u8string u8concat(Args&&... args)
    { return ::fast_io::basic_general_concat_compiler_constant_checked_entry<false,char8_t,std_u8string>(::std::forward<Args>(args)...); }
    template<::std::integral T> [[nodiscard]] std_string to_string(T value)
    {
        char buffer[::std::numeric_limits<T>::digits10 + 4]{};
        auto const result{::std::to_chars(buffer,buffer+sizeof(buffer),value)};
        if(result.ec != ::std::errc{}) { ::fast_io::fast_terminate(); }
        return {buffer,result.ptr};
    }
}
