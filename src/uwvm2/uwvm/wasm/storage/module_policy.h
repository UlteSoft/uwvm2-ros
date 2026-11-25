// Cold, host-supplied Wasm module policies. These keys are independent of
// host-import permissions and of process-wide address-space reservations.
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <limits>
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

    struct initializer_setting { option_view name; ::std::size_t init_limits::* member; };
    inline constexpr initializer_setting initializer_settings[]{
        {u8"init.imported-memories", &init_limits::imported_memories},
        {u8"init.local-defined-memories", &init_limits::local_defined_memories},
        {u8"init.imported-globals", &init_limits::imported_globals},
        {u8"init.local-defined-globals", &init_limits::local_defined_globals}
    };

    struct layer
    {
        ::std::array<::std::optional<bool>, ::std::size(feature_settings)> features{};
        init_limits limits{};
        ::std::optional<bool> check_tags{};
        ::std::optional<backend> default_backend{};
    };

    inline layer global{};
    inline named_map<layer> groups{}, singles{};
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

    [[nodiscard]] inline constexpr bool set(layer& target, option_view key, option_view value) noexcept
    {
        for(auto const& field : initializer_settings)
        {
            if(key != field.name) { continue; }
            auto& cap{target.limits.*field.member};
            if(cap != init_limits::inherited || value.empty()) { return false; }
            ::std::size_t number{};
            auto const parsed{::fast_io::parse_by_scan(value.data(), value.data() + value.size(), number)};
            if(parsed.code != ::fast_io::parse_code::ok || parsed.iter != value.data() + value.size() || number == init_limits::inherited)
            { return false; }
            cap = number;
            return true;
        }
        if(key == u8"runtime.memtag-backend")
        {
            if(target.default_backend) { return false; }
            target.default_backend = parse_backend(value);
            return target.default_backend.has_value();
        }
        if(value != u8"on" && value != u8"off") { return false; }
        auto const enabled{value == u8"on"};
        if(key == u8"runtime.memtag-checks")
        {
            if(target.check_tags.has_value()) { return false; }
            target.check_tags = enabled;
            return true;
        }
        for(::std::size_t i{}; i != ::std::size(feature_settings); ++i)
        {
            if(feature_settings[i].name != key) { continue; }
            if(target.features[i].has_value()) { return false; }
            target.features[i] = enabled;
            return true;
        }
        return false;
    }

    inline constexpr void apply(layer const& from, ::uwvm2::uwvm::wasm::type::wasm_parameter_t& to) noexcept
    {
        auto& parser{::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(to.binfmt1_para)};
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
        for(auto const& field : initializer_settings)
        {
            if(auto cap{from.limits.*field.member}; cap != init_limits::inherited) { to.initializer_limits.*field.member = cap; }
        }
        if(from.check_tags) { to.memory_policy.check_tags = *from.check_tags; }
        if(from.default_backend) { to.memory_policy.backend = *from.default_backend; }
    }

    inline constexpr void resolve(name_view module, ::uwvm2::uwvm::wasm::type::wasm_parameter_t& parameters) noexcept
    {
        apply(global, parameters);
        if(auto member{membership.find(module)}; member != membership.end())
        {
            if(auto group{groups.find(member->second)}; group != groups.end()) { apply(group->second, parameters); }
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
