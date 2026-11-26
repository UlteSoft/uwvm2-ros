// Cold, host-supplied Wasm module policies. These keys are independent of
// host-import permissions and of process-wide address-space reservations.
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <uwvm2/uwvm/runtime/macro/push_macros.h>
# include <initializer_list>
# include <utility>
# include <optional>
# include <string_view>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/extension/memtag/execution_policy.h>
# include <uwvm2/uwvm/wasm/type/para.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::wasm::storage::module_policy
{
    using name_view = ::uwvm2::utils::container::u8string_view;
    using owned_name = ::uwvm2::utils::container::u8string;
    using option_view = ::std::u8string_view;
    using parser_policy = ::uwvm2::parser::wasm::standard::wasm1p1::features::wasm_binfmt1p1_feature_parameter;
    namespace runtime_mode = ::uwvm2::uwvm::runtime::runtime_mode;
    namespace features = ::uwvm2::uwvm::wasm::feature;
    namespace wasm1p1 = ::uwvm2::parser::wasm::standard::wasm1p1::features;
    using parameters = ::uwvm2::uwvm::wasm::type::wasm_parameter_t;
    using baseline = wasm1p1::wasm_feature_cli_mode;
    using init_limits = ::uwvm2::uwvm::wasm::type::module_initializer_limits;
    using backend = ::uwvm2::extension::memtag::owner_backend;
    template<typename T>
    using named_map = ::uwvm2::utils::container::unordered_flat_map<owned_name, T,
        ::uwvm2::utils::container::pred::u8string_view_hash, ::uwvm2::utils::container::pred::u8string_view_equal>;

    struct feature_setting
    {
        option_view name;
        bool parser_policy::* disabled;
        bool parser_policy::* explicit_on{};
        bool parser_policy::* explicit_off{};
    };

    inline constexpr feature_setting feature_settings[]{
#define UWVM_MODULE_POLICY_FEATURE(name, member) {u8"parser." name, &parser_policy::disable_##member, &parser_policy::explicit_enable_##member, &parser_policy::explicit_disable_##member}
        UWVM_MODULE_POLICY_FEATURE("multi-value", multi_value),
        UWVM_MODULE_POLICY_FEATURE("reference-types", reference_types),
        UWVM_MODULE_POLICY_FEATURE("table-instructions", table_instructions),
        UWVM_MODULE_POLICY_FEATURE("multiple-tables", multiple_tables),
        UWVM_MODULE_POLICY_FEATURE("bulk-memory", bulk_memory),
        UWVM_MODULE_POLICY_FEATURE("sign-extension", sign_extension),
        UWVM_MODULE_POLICY_FEATURE("nontrapping-float-to-int", nontrapping_float_to_int),
        UWVM_MODULE_POLICY_FEATURE("simd", simd),
        UWVM_MODULE_POLICY_FEATURE("extended-const", extended_const),
        UWVM_MODULE_POLICY_FEATURE("table-initializer", table_initializer),
        UWVM_MODULE_POLICY_FEATURE("relaxed-simd", relaxed_simd),
        UWVM_MODULE_POLICY_FEATURE("multi-memory", multi_memory),
        UWVM_MODULE_POLICY_FEATURE("threads", threads),
        UWVM_MODULE_POLICY_FEATURE("tail-call", tail_call),
        UWVM_MODULE_POLICY_FEATURE("memory64", memory64),
        UWVM_MODULE_POLICY_FEATURE("custom-page-sizes", custom_page_sizes),
        UWVM_MODULE_POLICY_FEATURE("table64", table64),
        UWVM_MODULE_POLICY_FEATURE("function-references", function_references),
        UWVM_MODULE_POLICY_FEATURE("gc", gc),
        UWVM_MODULE_POLICY_FEATURE("exceptions", exceptions),
#undef UWVM_MODULE_POLICY_FEATURE
        {u8"parser.adm", &parser_policy::disable_adm_extension},
        {u8"parser.memtag", &parser_policy::disable_memtag_extension}
    };

    struct parser_count_setting
    {
        option_view name;
        ::std::size_t& (*get)(parameters&) noexcept;
    };

    inline constexpr parser_count_setting parser_counts[]{
#define UWVM_PARSER_COUNT(name, limits, member) {u8"" name, [](parameters& p) noexcept -> ::std::size_t& { return (limits).member; }}
        UWVM_PARSER_COUNT("codesec_codes", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_code_sec_codes),
        UWVM_PARSER_COUNT("const_expr_opcodes", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_const_expr_opcodes),
        UWVM_PARSER_COUNT("section_storage_bytes", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_section_storage_bytes),
        UWVM_PARSER_COUNT("custom_sections", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_custom_sections),
        UWVM_PARSER_COUNT("code_locals", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_code_locals),
        UWVM_PARSER_COUNT("datacountsec_count", features::wasm_binfmt_ver1_wasm1p1_parameter(p.binfmt1_para).parser_limit, max_data_count_sec_count),
        UWVM_PARSER_COUNT("datasec_entries", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_data_sec_entries),
        UWVM_PARSER_COUNT("tagsec_count", features::wasm_binfmt_ver1_wasm1p1_parameter(p.binfmt1_para).parser_limit, max_tag_sec_entries),
        UWVM_PARSER_COUNT("elemsec_expr", features::wasm_binfmt_ver1_wasm1p1_parameter(p.binfmt1_para).parser_limit, max_elem_sec_expr),
        UWVM_PARSER_COUNT("elemsec_funcidx", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_elem_sec_funcidx),
        UWVM_PARSER_COUNT("elemsec_elems", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_elem_sec_elems),
        UWVM_PARSER_COUNT("exportsec_exports", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_export_sec_exports),
        UWVM_PARSER_COUNT("globalsec_globals", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_global_sec_globals),
        UWVM_PARSER_COUNT("importsec_imports", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_import_sec_imports),
        UWVM_PARSER_COUNT("memorysec_memories", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_memory_sec_memories),
        UWVM_PARSER_COUNT("tablesec_tables", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_table_sec_tables),
        UWVM_PARSER_COUNT("typesec_types", features::wasm_binfmt_ver1_wasm1_parameter(p.binfmt1_para).parser_limit, max_type_sec_types),
        UWVM_PARSER_COUNT("custom_name_funcnames", p.name_parser.name_parser_limit, max_name_sec_function_names),
        UWVM_PARSER_COUNT("custom_name_codelocal_funcs", p.name_parser.name_parser_limit, max_name_sec_code_local_functions),
        UWVM_PARSER_COUNT("custom_name_codelocal_name_per_funcs", p.name_parser.name_parser_limit, max_name_sec_code_local_names_per_function),
        UWVM_PARSER_COUNT("custom_name_local_names", p.name_parser.name_parser_limit, max_local_names),
        UWVM_PARSER_COUNT("custom_name_diagnostics", p.name_parser.name_parser_limit, max_diagnostics),
#undef UWVM_PARSER_COUNT
    };

    struct initializer_setting { option_view name; ::std::size_t init_limits::* member; };
    inline constexpr initializer_setting initializer_settings[]{
        {u8"init.imported-functions", &init_limits::imported_functions},
        {u8"init.imported-tables", &init_limits::imported_tables},
        {u8"init.imported-memories", &init_limits::imported_memories},
        {u8"init.imported-globals", &init_limits::imported_globals},
        {u8"init.imported-tags", &init_limits::imported_tags},
        {u8"init.local-defined-functions", &init_limits::local_defined_functions},
        {u8"init.local-defined-codes", &init_limits::local_defined_codes},
        {u8"init.local-defined-tables", &init_limits::local_defined_tables},
        {u8"init.local-defined-memories", &init_limits::local_defined_memories},
        {u8"init.local-defined-globals", &init_limits::local_defined_globals},
        {u8"init.local-defined-tags", &init_limits::local_defined_tags},
        {u8"init.local-defined-elements", &init_limits::local_defined_elements},
        {u8"init.local-defined-datas", &init_limits::local_defined_datas},
        {u8"init.element-expressions", &init_limits::element_expressions},
    };

    struct layer
    {
        ::std::array<::std::optional<bool>, ::std::size(feature_settings)> features{};
        ::std::array<::std::optional<::std::size_t>, ::std::size(parser_counts)> counts{};
        ::std::array<::std::optional<::std::size_t>, ::std::size(initializer_settings)> limits{};
        ::std::optional<baseline> preset{};
        ::std::optional<::uwvm2::uwvm::wasm::type::module_memory_limit_t> memory_pages{};
        runtime_mode::module_translation_policy translation{};
        ::std::optional<bool> check_tags{};
        ::std::optional<backend> default_backend{};
    };

    inline layer global{};
    struct group_policy { layer settings{}; bool has_members{}; bool has_settings{}; };
    inline named_map<group_policy> groups{};
    inline named_map<layer> singles{};
    inline named_map<owned_name> membership{};
    using memory_overrides = ::uwvm2::utils::container::unordered_flat_map<::std::size_t, backend>;
    inline named_map<memory_overrides> owner_backends{};

    [[nodiscard]] inline constexpr ::std::optional<backend> parse_backend(option_view name) noexcept
    {
        if(name == u8"software") { return backend::software; }
        if(name == u8"auto") { return backend::automatic; }
        if(name == u8"arm-mte") { return backend::arm_mte; }
        if(name == u8"x86-mtt") { return backend::x86_mtt; }
        return {};
    }

    [[nodiscard]] inline constexpr ::std::optional<::std::size_t> parse_count(option_view value) noexcept
    {
        if(value.empty()) { return {}; }
        for(auto ch : value) { if(ch < u8'0' || ch > u8'9') { return {}; } }
        ::std::size_t number{};
        auto const parsed{::fast_io::parse_by_scan(value.data(), value.data() + value.size(), number)};
        if(parsed.code != ::fast_io::parse_code::ok || parsed.iter != value.data() + value.size()) { return {}; }
        return number;
    }

    [[nodiscard]] inline constexpr bool set_count(::std::optional<::std::size_t>& field, option_view value) noexcept
    {
        if(field) { return false; }
        field = parse_count(value);
        return field.has_value();
    }

    template<typename T>
    [[nodiscard]] inline constexpr bool set_enum(::std::optional<T>& target, option_view value,
        ::std::initializer_list<::std::pair<option_view, T>> choices) noexcept
    {
        if(target) { return false; }
        for(auto const& [name, choice] : choices)
        { if(value == name) { target = choice; return true; } }
        return false;
    }

    [[nodiscard]] inline constexpr bool set(layer& target, option_view key, option_view value) noexcept
    {
        constexpr option_view parser_prefix{u8"parser.limit."};
        if(key.starts_with(parser_prefix))
        {
            auto const name{key.substr(parser_prefix.size())};
            for(::std::size_t i{}; i != ::std::size(parser_counts); ++i)
            { if(name == parser_counts[i].name) { return set_count(target.counts[i], value); } }
            return false;
        }
        for(::std::size_t i{}; i != ::std::size(initializer_settings); ++i)
        { if(key == initializer_settings[i].name) { return set_count(target.limits[i], value); } }
        if(key == u8"runtime.memory-pages")
        {
            if(target.memory_pages) { return false; }
            auto const separator{value.find(u8':')};
            auto const minimum{parse_count(value.substr(0, separator))};
            if(!minimum) { return false; }
            ::uwvm2::uwvm::wasm::type::module_memory_limit_t pages{.min = *minimum};
            if(separator != option_view::npos)
            {
                auto const maximum{parse_count(value.substr(separator + 1u))};
                if(!maximum || *maximum < *minimum) { return false; }
                pages.max = *maximum;
                pages.present_max = true;
            }
            target.memory_pages = pages;
            return true;
        }
        if(key == u8"parser.baseline")
        {
            return set_enum(target.preset, value, {{u8"mvp", baseline::direct_mvp}, {u8"wasm1p1", baseline::direct_wasm1p1},
                {u8"wasm2", baseline::direct_wasm2}, {u8"wasm3", baseline::direct_wasm3}});
        }
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER) && defined(UWVM_ENABLE_UWVM_INT_LOOP_UNWIND)
        if(key == u8"runtime.interpreter.loop-unwind-max-size") { return set_count(target.translation.loop_unwind_max_size, value); }
#endif
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER) && defined(UWVM_ENABLE_UWVM_INT_COMBINE_OPS)
        if(key == u8"runtime.interpreter.opcode-combine")
        {
            using level = runtime_mode::runtime_uwvm_int_opcode_conbination_level_t;
            return set_enum(target.translation.opcode_combine, value, {{u8"disable", level::disable}, {u8"soft", level::soft},
# if defined(UWVM_ENABLE_UWVM_INT_HEAVY_COMBINE_OPS)
                {u8"heavy", level::heavy},
# endif
# if defined(UWVM_ENABLE_UWVM_INT_EXTRA_HEAVY_COMBINE_OPS)
                {u8"extra", level::extra},
# endif
            });
        }
#endif
#if defined(UWVM_RUNTIME_LLVM_JIT)
        if(key == u8"runtime.llvm.policy")
        {
            using strategy = runtime_mode::runtime_llvm_jit_policy_t;
            return set_enum(target.translation.llvm_policy, value, {{u8"default", strategy::default_policy}, {u8"debug", strategy::debug},
                {u8"fast-compile", strategy::fast_compile}, {u8"balanced", strategy::balanced}, {u8"max", strategy::max}});
        }
        if(key == u8"runtime.llvm.full-policy")
        {
            using strategy = runtime_mode::runtime_llvm_jit_full_policy_t;
            return set_enum(target.translation.llvm_full_policy, value, {{u8"auto", strategy::auto_policy}, {u8"debug", strategy::debug},
                {u8"legacy-light", strategy::legacy_light}, {u8"pb-o1", strategy::passbuilder_o1},
                {u8"pb-o2", strategy::passbuilder_o2}, {u8"pb-o3", strategy::passbuilder_o3}});
        }
#endif
        if(key == u8"runtime.memtag-backend")
        {
            if(target.default_backend) { return false; }
            target.default_backend = parse_backend(value);
            return target.default_backend.has_value();
        }
        if(value != u8"on" && value != u8"off") { return false; }
        auto const enabled{value == u8"on"};
        auto flag = [enabled](::std::optional<bool>& field) noexcept
        {
            if(field.has_value()) { return false; }
            field = enabled;
            return true;
        };
        if(key == u8"runtime.memtag-checks") { return flag(target.check_tags); }
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER) && defined(UWVM_ENABLE_UWVM_INT_LOOP_UNWIND)
        if(key == u8"runtime.interpreter.loop-unwind") { return flag(target.translation.loop_unwind); }
#endif
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER) && defined(UWVM_ENABLE_UWVM_INT_COMBINE_OPS) && defined(UWVM_ENABLE_UWVM_INT_DELAY_LOCAL_SOFT)
        if(key == u8"runtime.interpreter.delay-local") { return flag(target.translation.delay_local); }
#endif
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER) && defined(UWVM_ENABLE_UWVM_INT_INSTRUCTION_REORDER)
        if(key == u8"runtime.interpreter.instruction-reorder") { return flag(target.translation.instruction_reorder); }
#endif
        for(::std::size_t i{}; i != ::std::size(feature_settings); ++i)
        {
            if(feature_settings[i].name != key) { continue; }
            return flag(target.features[i]);
        }
        return false;
    }

    inline constexpr void apply(layer const& from, ::uwvm2::uwvm::wasm::type::wasm_parameter_t& to) noexcept
    {
        auto& parser{::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(to.binfmt1_para)};
        if(from.preset)
        {
            auto const selected{*from.preset};
            parser.explicit_feature_mvp = selected == baseline::direct_mvp;
            parser.explicit_feature_wasm1p1 = selected == baseline::direct_wasm1p1;
            parser.explicit_feature_wasm2 = selected == baseline::direct_wasm2;
            parser.explicit_feature_wasm3 = selected == baseline::direct_wasm3;
            wasm1p1::apply_feature_baseline(parser, selected);
        }
        for(::std::size_t i{}; i != ::std::size(feature_settings); ++i)
        {
            auto const override{from.features[i]};
            if(!override) { continue; }
            auto const& feature{feature_settings[i]};
            parser.*feature.disabled = !*override;
            if(feature.disabled == &parser_policy::disable_multi_value) { parser.controllable_allow_multi_result_vector = !*override; }
            if(feature.disabled == &parser_policy::disable_multiple_tables) { parser.controllable_allow_multi_table = !*override; }
            if(feature.explicit_on) { parser.*feature.explicit_on = *override; parser.*feature.explicit_off = !*override; }
        }
        for(::std::size_t i{}; i != ::std::size(parser_counts); ++i)
        { if(from.counts[i]) { parser_counts[i].get(to) = *from.counts[i]; } }
        for(::std::size_t i{}; i != ::std::size(initializer_settings); ++i)
        { if(from.limits[i]) { to.initializer_limits.*initializer_settings[i].member = *from.limits[i]; } }
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
        if(from.translation.loop_unwind) { to.translation.loop_unwind = from.translation.loop_unwind; }
        if(from.translation.loop_unwind_max_size) { to.translation.loop_unwind_max_size = from.translation.loop_unwind_max_size; }
        if(from.translation.opcode_combine) { to.translation.opcode_combine = from.translation.opcode_combine; }
        if(from.translation.delay_local) { to.translation.delay_local = from.translation.delay_local; }
        if(from.translation.instruction_reorder) { to.translation.instruction_reorder = from.translation.instruction_reorder; }
#endif
#if defined(UWVM_RUNTIME_LLVM_JIT)
        if(from.translation.llvm_policy) { to.translation.llvm_policy = from.translation.llvm_policy; }
        if(from.translation.llvm_full_policy) { to.translation.llvm_full_policy = from.translation.llvm_full_policy; }
#endif
        if(from.memory_pages) { to.memory_pages = from.memory_pages; }
        if(from.check_tags) { to.memory_policy.check_tags = *from.check_tags; }
        if(from.default_backend) { to.memory_policy.backend = *from.default_backend; }
    }

    inline constexpr void resolve(name_view module, ::uwvm2::uwvm::wasm::type::wasm_parameter_t& parameters) noexcept
    {
        apply(global, parameters);
        if(auto member{membership.find(module)}; member != membership.end())
        {
            if(auto group{groups.find(member->second)}; group != groups.end()) { apply(group->second.settings, parameters); }
        }
        if(auto single{singles.find(module)}; single != singles.end()) { apply(single->second, parameters); }
        parameters.resolve_module_policy = false;
    }

    [[nodiscard]] inline constexpr backend resolve_backend(name_view module, ::std::size_t local_index, backend inherited) noexcept
    {
        if(auto owner{owner_backends.find(module)}; owner != owner_backends.end())
        {
            auto requested{owner->second.find(local_index)};
            if(requested != owner->second.end()) { return requested->second; }
        }
        return inherited;
    }
}

#ifndef UWVM_MODULE
# include <uwvm2/uwvm/runtime/macro/pop_macros.h>
#endif
