// Effective limits in the signed update measurement. Use a distinct hard limit
// for each initializer field so a swapped pair cannot produce the same result.
#include <uwvm2/uwvm/wasm/storage/module_policy.h>

static_assert([]
{
    ::uwvm2::parser::wasm::standard::wasm1::features::code_body_limits limits{SIZE_MAX, SIZE_MAX};
    ::std::size_t total{SIZE_MAX - 1u};
    return limits.append(total, 1u) && !limits.append(total, 1u) && total == SIZE_MAX;
}());

int main()
{
    namespace policy = ::uwvm2::uwvm::wasm::storage::module_policy;
    namespace resource = ::uwvm2::utils::allocator::resource;
    auto& process{::uwvm2::uwvm::runtime::initializer::initializer_limit};
    process.max_imported_functions = 101;
    process.max_imported_tables = 102;
    process.max_imported_memories = 103;
    process.max_imported_globals = 104;
    process.max_imported_tags = 105;
    process.max_local_defined_functions = 106;
    process.max_local_defined_codes = 107;
    process.max_local_defined_tables = 108;
    process.max_local_defined_memories = 109;
    process.max_local_defined_globals = 110;
    process.max_local_defined_tags = 111;
    process.max_local_defined_elements = 112;
    process.max_local_defined_datas = 113;
    process.max_element_expressions = 114;
    resource::initializer_allocation_bytes = 115;

    policy::parameters module{};
    policy::layer scoped{};
    for(::std::size_t i{}; i != ::std::size(policy::initializer_settings); ++i)
    {
        auto const& setting{policy::initializer_settings[i]};
        if(setting.effective(module.initializer_limits) != 101u + i ||
           !policy::set(scoped, setting.name, i % 2 ? u8"90" : u8"120")) { return 1; }
    }
    policy::apply(scoped, module);
    for(::std::size_t i{}; i != ::std::size(policy::initializer_settings); ++i)
    {
        if(policy::initializer_settings[i].effective(module.initializer_limits) != (i % 2 ? 90u : 101u + i)) { return 2; }
    }
    resource::module_file_bytes = 200;
    resource::parser_metadata_bytes = 201;
    resource::function_body_bytes = 202;
    resource::module_code_bytes = 203;
    auto& limits{policy::features::wasm_binfmt_ver1_wasm1_parameter(module.binfmt1_para).parser_limit};
    limits.max_section_storage_bytes = SIZE_MAX;
    auto measure = [&]
    {
        ::std::array<::std::size_t, ::std::size(policy::parser_counts)> words{};
        ::std::size_t i{};
        policy::visit_measured_parser_limits(module, [&](::std::size_t value) { words[i++] = value; });
        return ::std::array{words[policy::parser_input_index], words[policy::parser_limit_index(u8"type-metadata-bytes")],
            words[policy::parser_limit_index(u8"function-body-bytes")], words[policy::parser_limit_index(u8"module-code-bytes")],
            words[policy::parser_limit_index(u8"section_storage_bytes")]};
    };
    if(measure() != ::std::array{200uz, 201uz, 202uz, 203uz, static_cast<::std::size_t>(PTRDIFF_MAX)}) { return 3; }
    limits.max_input_bytes = 210;
    limits.max_type_metadata_bytes = 211;
    limits.max_function_body_bytes = 212;
    limits.max_module_code_bytes = 213;
    auto const unchanged{measure()};
    limits.max_input_bytes = 199;
    limits.max_type_metadata_bytes = 198;
    limits.max_function_body_bytes = 197;
    limits.max_module_code_bytes = 196;
    limits.max_section_storage_bytes = 195;
    if(unchanged != ::std::array{200uz, 201uz, 202uz, 203uz, static_cast<::std::size_t>(PTRDIFF_MAX)} ||
       measure() != ::std::array{199uz, 198uz, 197uz, 196uz, 195uz}) { return 4; }
}
