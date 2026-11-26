#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>
#include <uwvm2/utils/allocator/resource_limit.h>
#include <uwvm2/parser/wasm_custom/customs/name.h>
#include <uwvm2/validation/standard/wasm3/recursive_type_registry.h>
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

int main(int argc, char** argv)
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
    // Two empty custom sections with distinct names require two retained section records.
    constexpr ::std::uint8_t customs[]{0,97,115,109,1,0,0,0, 0,2,1,'a', 0,2,1,'b'};
    namespace resource = ::uwvm2::utils::allocator::resource;
    constexpr auto pool{static_cast<unsigned>(resource::pool::parser)};
    ::uwvm2::parser::wasm::concepts::feature_parameter_t<wasm> defaults{};
    auto const parse_customs = [&]
    {
        auto const* begin{reinterpret_cast<::std::byte const*>(customs)};
        base::error_impl error{};
        return ::uwvm2::parser::wasm::binfmt::ver1::wasm_binfmt_ver1_handle_func<wasm>(begin, begin + sizeof(customs), error, defaults);
    };
    auto const baseline{resource::live_bytes[pool].load()};
    {
        if(argc == 2 && ::std::strcmp(argv[1], "single") == 0) { resource::policies[pool].single = 0; }
        auto first{parse_customs()};
        auto const one{resource::live_bytes[pool].load()};
        if(one <= baseline) { return 6; }
        auto moved{::std::move(first)};
        if(resource::live_bytes[pool].load() != one) { return 7; }
        if(argc == 2 && ::std::strcmp(argv[1], "live") == 0) { resource::policies[pool].live = one; }
        { auto second{parse_customs()}; if(resource::live_bytes[pool].load() != one + (one - baseline)) { return 8; } }
        if(resource::live_bytes[pool].load() != one) { return 9; }
    }
    if(resource::live_bytes[pool].load() != baseline) { return 10; }
    namespace v3 = ::uwvm2::validation::standard::wasm3;
    namespace t3 = ::uwvm2::parser::wasm::standard::wasm3::type;
    {
        constexpr ::std::uint8_t structure[]{1, 0x5f, 0};
        auto cursor{reinterpret_cast<::std::byte const*>(structure)};
        t3::recursive_type_section section{};
        if(v3::scan_core3_type_section(cursor, cursor + sizeof(structure), section).error != v3::recursive_type_binary_error::ok) { return 11; }
        v3::recursive_type_registry registry{};
        ::uwvm2::utils::container::validation_vector<::std::size_t> ids{};
        if(registry.validate_and_intern(section, ids).error != v3::recursive_type_validation_error::ok) { return 12; }
        auto const after_insert{resource::live_bytes[pool].load()};
        if(registry.validate_and_intern(section, ids).error != v3::recursive_type_validation_error::ok ||
           ids.size() != 1 || ids.front_unchecked() != 0 || registry.size() != 1 ||
           resource::live_bytes[pool].load() != after_insert) { return 13; }
    }
    if(resource::live_bytes[pool].load() != baseline) { return 14; }
    // A Core 3 array has exactly one field record even though it carries no encoded field count.
    constexpr ::std::uint8_t array_payload[]{1, 0x5e, 0x7f, 0};
    constexpr auto array_bytes{sizeof(t3::recursive_group) + sizeof(t3::sub_type) + sizeof(t3::field_type)};
    auto const* array_begin{reinterpret_cast<::std::byte const*>(array_payload)};
    for(auto limit : {array_bytes, array_bytes - 1uz})
    {
        auto cursor{array_begin};
        t3::recursive_type_section section{};
        auto const result{v3::scan_core3_type_section(cursor, array_begin + sizeof(array_payload), section, 1uz, limit)};
        if(limit == array_bytes)
        {
            if(result.error != v3::recursive_type_binary_error::ok || result.metadata_bytes != array_bytes || section.type_count != 1u) { return 18; }
        }
        else if(result.error != v3::recursive_type_binary_error::resource_limit || result.requested != array_bytes ||
                result.limit != limit || cursor != array_begin || section.type_count != 0u) { return 19; }
    }
    constexpr ::std::uint8_t array_module[]{0,97,115,109,1,0,0,0, 1,4,1,0x5e,0x7f,0};
    using tsec = wasm1::type_section_storage_t<wasm, proposals>;
    constexpr auto full_array_bytes{array_bytes + sizeof(v3::recursive_type_record) + sizeof(decltype(tsec::types)::value_type) +
        sizeof(decltype(tsec::owned_signatures)::value_type) + sizeof(t3::composite_kind) + sizeof(wasm1::final_value_type_t<wasm, proposals>)};
    auto const gc_bytes = [](auto limit)
    {
        return [limit](auto& p)
        {
            ::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<wasm>(p).parser_limit.max_type_metadata_bytes = limit;
            wasm1p1::get_wasm1p1_parameter(p).disable_gc = false;
        };
    };
    if(!check<wasm, proposals>(array_module, gc_bytes(full_array_bytes)) ||
       !check<wasm, proposals>(array_module, gc_bytes(full_array_bytes - 1uz), u8"type-metadata-bytes", full_array_bytes, full_array_bytes - 1uz)) { return 20; }
    constexpr ::std::uint8_t bodies[]{0,97,115,109,1,0,0,0, 1,4,1,0x60,0,0, 3,3,2,0,0, 10,7,2,2,0,0x0b,2,0,0x0b};
    auto const code_bytes = [](auto body, auto module)
    {
        return [=](auto& p)
        {
            auto& limits{::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<wasm>(p).parser_limit};
            limits.max_function_body_bytes = body;
            limits.max_module_code_bytes = module;
        };
    };
    if(!check<wasm>(bodies, code_bytes(2, 4)) || !check<wasm>(bodies, code_bytes(1, 4), u8"function-body-bytes", 2, 1) ||
       !check<wasm, proposals>(bodies, code_bytes(2, 3), u8"module-code-bytes", 4, 3)) { return 21; }
    auto const saved_body{::std::exchange(resource::function_body_bytes, 1uz)};
    auto const host_wins{check<wasm>(bodies, code_bytes(2, 4), u8"function-body-bytes", 2, 1)};
    resource::function_body_bytes = saved_body;
    if(!host_wins) { return 22; }
    using custom_record = ::uwvm2::parser::wasm::standard::wasm1::section::custom_section;
    constexpr auto bytes{2uz * sizeof(custom_record)};
    auto const sections = [](auto count, auto storage_bytes)
    {
        return [=](auto& parameters)
        {
            auto& limits{::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<wasm>(parameters).parser_limit};
            limits.max_custom_sections = count;
            limits.max_section_storage_bytes = storage_bytes;
        };
    };
    if(!check<wasm>(customs, sections(2, bytes)) ||
       !check<wasm>(customs, sections(1, bytes), u8"custom_sections", 2, 1) ||
       !check<wasm, proposals>(customs, sections(2, bytes - 1), u8"section_storage_bytes", bytes, bytes - 1)) { return 5; }
    namespace names = ::uwvm2::parser::wasm_custom::customs;
    // Two distinct function-local name maps each contain one local.
    constexpr ::std::uint8_t subsection[]{2, 11, 2, 0, 1, 0, 1, 'a', 1, 1, 0, 1, 'b'};
    auto const* name_begin{reinterpret_cast<::std::byte const*>(subsection)};
    for(unsigned limit : {1u, 2u})
    {
        names::name_storage_t names_section{};
        ::uwvm2::utils::container::parser_vector<names::name_err_t> diagnostics{};
        names::name_parser_param_t parameters{};
        parameters.name_parser_limit.max_local_names = limit;
        parameters.name_parser_limit.max_diagnostics = 1;
        auto const parse = [&]{ names::parse_name_storage(names_section, name_begin, name_begin + sizeof(subsection), diagnostics, parameters); };
        parse();
        if(limit == 1)
        {
            if(diagnostics.size() != 1 || diagnostics.front_unchecked().type != names::name_err_type_t::exceed_the_max_name_parser_limit ||
               diagnostics.front_unchecked().err.exceed_the_max_name_parser_limit.name != u8"custom_name_local_names" ||
               names_section.code_local_name.size() != 1) { return 15; }
            parse();
            parse();
            if(diagnostics.size() != 2 || diagnostics.back_unchecked().type != names::name_err_type_t::exceed_the_max_name_parser_limit ||
               diagnostics.back_unchecked().err.exceed_the_max_name_parser_limit.name != u8"custom_name_diagnostics") { return 16; }
        }
        else if(!diagnostics.empty() || names_section.code_local_name.size() != 2) { return 17; }
    }
}
