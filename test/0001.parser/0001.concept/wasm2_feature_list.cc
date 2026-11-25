/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @brief       WebAssembly 2.0 command-line feature policy test
 * @details     Checks individual Core feature overrides across the supported module baselines.
 * @author      MacroModel
 * @version     2.0.0
 * @date        2026-07-11
 * @copyright   APL-2.0 License
 */

#include <cstdlib>
#include <uwvm2/uwvm/wasm/storage/module_policy.h>
#include <uwvm2/validation/standard/wasm2/impl.h>

namespace policy = uwvm2::uwvm::wasm::storage::module_policy;
namespace wasm2 = uwvm2::parser::wasm::standard::wasm2::features;

static void expect(bool condition)
{
    if(!condition) { std::abort(); }
}

int main()
{
    struct feature_case { policy::option_view key; wasm2::wasm2_feature_kind kind; };
    constexpr feature_case features[]{
        {u8"parser.multi-value", wasm2::wasm2_feature_kind::multi_value},
        {u8"parser.reference-types", wasm2::wasm2_feature_kind::reference_types},
        {u8"parser.table-instructions", wasm2::wasm2_feature_kind::table_instructions},
        {u8"parser.multiple-tables", wasm2::wasm2_feature_kind::multiple_tables},
        {u8"parser.bulk-memory", wasm2::wasm2_feature_kind::bulk_memory},
        {u8"parser.sign-extension", wasm2::wasm2_feature_kind::sign_extension},
        {u8"parser.nontrapping-float-to-int", wasm2::wasm2_feature_kind::nontrapping_float_to_int},
        {u8"parser.simd", wasm2::wasm2_feature_kind::simd},
    };
    for(auto const& feature : features)
    {
        for(auto const baseline : {u8"mvp", u8"wasm1p1", u8"wasm2", u8"wasm3"})
        {
            for(auto const enabled : {false, true})
            {
                for(auto const baseline_first : {false, true})
                {
                    policy::layer settings{};
                    if(baseline_first) { expect(policy::set(settings, u8"parser.baseline", baseline)); }
                    expect(policy::set(settings, feature.key, enabled ? u8"on" : u8"off"));
                    if(!baseline_first) { expect(policy::set(settings, u8"parser.baseline", baseline)); }
                    expect(!policy::set(settings, feature.key, enabled ? u8"off" : u8"on"));
                    expect(!policy::set(settings, u8"parser.baseline", baseline));
                    policy::parameters parameters{};
                    policy::apply(settings, parameters);
                    auto const& parser = policy::features::wasm_binfmt_ver1_wasm1p1_parameter(parameters.binfmt1_para);
                    for(auto const& other : features)
                    {
                        bool const expected = other.kind == feature.kind ? enabled : policy::option_view{baseline} != u8"mvp";
                        expect(wasm2::feature_enabled(parser, other.kind) == expected);
                    }
                }
            }
        }
    }
    policy::parser_policy parser{};
    parser.controllable_allow_multi_result_vector = true;
    expect(!wasm2::feature_enabled(parser, wasm2::wasm2_feature_kind::multi_value));
    parser.controllable_allow_multi_table = true;
    expect(!wasm2::feature_enabled(parser, wasm2::wasm2_feature_kind::multiple_tables));
}
