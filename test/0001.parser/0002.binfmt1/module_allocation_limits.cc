#include <cstddef>
#include <cstdint>
#include <fast_io.h>
#include <uwvm2/parser/wasm/standard/wasm1/features/binfmt.h>
#include <uwvm2/parser/wasm/standard/wasm1p1/features/impl.h>

namespace
{
    namespace wasm1 = ::uwvm2::parser::wasm::standard::wasm1::features;
    namespace wasm1p1 = ::uwvm2::parser::wasm::standard::wasm1p1::features;
    namespace base = ::uwvm2::parser::wasm::base;
    using wasm = wasm1::wasm1;
    using proposals = wasm1p1::wasm1p1;

    template<typename... Features, ::std::size_t N, typename Configure>
    bool check(::std::uint8_t const (&bytes)[N], Configure configure, ::fast_io::u8string_view key = {},
               ::std::size_t requested = 0, ::std::size_t limit = 0)
    {
        ::uwvm2::parser::wasm::concepts::feature_parameter_t<Features...> parameters{};
        configure(parameters);
        auto const* begin{reinterpret_cast<::std::byte const*>(bytes)};
        base::error_impl error{};
        try
        {
            (void)::uwvm2::parser::wasm::binfmt::ver1::wasm_binfmt_ver1_handle_func<Features...>(begin, begin + N, error, parameters);
            return key.empty() && error.err_code == base::wasm_parse_error_code::ok;
        }
        catch(::fast_io::error const&)
        {
            if(error.err_code != base::wasm_parse_error_code::exceed_the_max_parser_limit) { return false; }
            auto const& details{error.err_selectable.exceed_the_max_parser_limit};
            return details.name == key && details.value == requested && details.maxval == limit;
        }
    }
}

int main()
{
    // A global and an active data offset each retain one opcode. Both end bytes are free.
    constexpr ::std::uint8_t offsets[]{
        0,97,115,109,1,0,0,0, 5,3,1,0,1, 6,6,1,0x7f,0,0x41,0,0x0b, 11,6,1,0,0x41,0,0x0b,0};
    auto const opcodes = [](unsigned limit)
    {
        return [limit](auto& parameters)
        { ::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<wasm>(parameters).parser_limit.max_const_expr_opcodes = limit; };
    };
    if(!check<wasm>(offsets, opcodes(2)) || !check<wasm>(offsets, opcodes(1), u8"const_expr_opcodes", 2, 1)) { return 1; }
    if(!check<wasm, proposals>(offsets, opcodes(2)) || !check<wasm, proposals>(offsets, opcodes(1), u8"const_expr_opcodes", 2, 1)) { return 2; }
    auto const mvp_offsets = [&](auto& parameters)
    { opcodes(1)(parameters); wasm1p1::get_wasm1p1_parameter(parameters).disable_extended_const = true; };
    if(!check<wasm, proposals>(offsets, mvp_offsets, u8"const_expr_opcodes", 2, 1)) { return 3; }
    constexpr ::std::uint8_t sum[]{0,97,115,109,1,0,0,0, 6,9,1,0x7f,0,0x41,1,0x41,2,0x6a,0x0b};
    auto const extended = [&](unsigned limit)
    {
        return [&, limit](auto& parameters)
        { wasm1p1::get_wasm1p1_parameter(parameters).disable_extended_const = false; opcodes(limit)(parameters); };
    };
    if(!check<wasm, proposals>(sum, extended(3)) || !check<wasm, proposals>(sum, extended(2), u8"const_expr_opcodes", 3, 2)) { return 4; }
}
