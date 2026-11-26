#pragma once
#ifndef UWVM_MODULE
# include <memory>
# include <fast_io.h>
# include <uwvm2/utils/cmdline/impl.h>
# include <uwvm2/uwvm/io/impl.h>
# include <uwvm2/uwvm/wasm/storage/module_policy.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params
{
    namespace details
    {
        enum class policy_action { global, group, single, add_module, memory_backend };

        template<policy_action Action>
        inline constexpr ::uwvm2::utils::cmdline::parameter_return_type wasm_policy_callback(
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results* current,
            ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
        {
            namespace policy = ::uwvm2::uwvm::wasm::storage::module_policy;
            using result = ::uwvm2::utils::cmdline::parameter_return_type;
            using kind = ::uwvm2::utils::cmdline::parameter_parsing_results_type;
            constexpr int count{Action == policy_action::global || Action == policy_action::add_module ? 2 : 3};
            auto error = [&]() noexcept
            {
                ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] Invalid or duplicate Wasm policy setting for ",
                    current->str, u8". ", ::uwvm2::utils::cmdline::print_usage(*current->para));
                return result::return_m1_imme;
            };
            if(end - current <= count) { return error(); }
            for(int i{1}; i <= count; ++i)
            { if(current[i].type != kind::arg || current[i].str.empty()) { return error(); } }
            auto name = [&](unsigned i) { return policy::name_view{current[i].str}; };
            auto option = [&](unsigned i) { return policy::option_view{current[i].str.data(), current[i].str.size()}; };
            if constexpr(Action == policy_action::add_module)
            {
                if(!policy::membership.try_emplace(policy::owned_name{name(2)}, policy::owned_name{name(1)}).second) { return error(); }
                policy::groups.try_emplace(policy::owned_name{name(1)}).first->second.has_members = true;
            }
            else if constexpr(Action == policy_action::memory_backend)
            {
                auto const requested{policy::parse_backend(option(3))};
                for(auto ch : option(2)) { if(ch < u8'0' || ch > u8'9') { return error(); } }
                ::std::size_t index{};
                auto const parsed{::fast_io::parse_by_scan(current[2].str.cbegin(), current[2].str.cend(), index)};
                if(!requested || parsed.code != ::fast_io::parse_code::ok || parsed.iter != current[2].str.cend()) { return error(); }
                auto const owner{policy::owner_backends.try_emplace(policy::owned_name{name(1)}).first};
                if(!owner->second.try_emplace(index, *requested).second) { return error(); }
            }
            else if constexpr(Action == policy_action::global)
            { if(!policy::set(policy::global, option(1), option(2))) { return error(); } }
            else if constexpr(Action == policy_action::group)
            {
                auto const target{policy::groups.try_emplace(policy::owned_name{name(1)}).first};
                if(!policy::set(target->second.settings, option(2), option(3))) { return error(); }
                target->second.has_settings = true;
            }
            else
            {
                auto const target{policy::singles.try_emplace(policy::owned_name{name(1)}).first};
                if(!policy::set(target->second, option(2), option(3))) { return error(); }
            }
            for(int i{1}; i <= count; ++i) { current[i].type = kind::occupied_arg; }
            return result::def;
        }
    }

    inline constexpr ::uwvm2::utils::cmdline::parameter wasm_policy_global{
        .name{u8"--wasm-policy-global"}, .describe{u8"Set a default Wasm parser, initializer or runtime policy before loading any module."},
        .usage{u8"<key> <value>"}, .handle = &details::wasm_policy_callback<details::policy_action::global>,
        .cate = ::uwvm2::utils::cmdline::categorization::wasm};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasm_policy_group{
        .name{u8"--wasm-policy-group"}, .describe{u8"Override a Wasm policy setting for members of a named group."},
        .usage{u8"<group> <key> <value>"}, .handle = &details::wasm_policy_callback<details::policy_action::group>,
        .cate = ::uwvm2::utils::cmdline::categorization::wasm};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasm_policy_group_add_module{
        .name{u8"--wasm-policy-group-add-module"}, .describe{u8"Bind a Wasm module name to one policy group. Membership is independent of WASI."},
        .usage{u8"<group> <module>"}, .handle = &details::wasm_policy_callback<details::policy_action::add_module>,
        .cate = ::uwvm2::utils::cmdline::categorization::wasm};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasm_policy_single{
        .name{u8"--wasm-policy-single"}, .describe{u8"Override one Wasm policy setting for a named module after its group settings."},
        .usage{u8"<module> <key> <value>"}, .handle = &details::wasm_policy_callback<details::policy_action::single>,
        .cate = ::uwvm2::utils::cmdline::categorization::wasm};
    inline constexpr ::uwvm2::utils::cmdline::parameter wasm_policy_memory_backend{
        .name{u8"--wasm-policy-memory-backend"}, .describe{u8"Override the tag backend for one local memory definition. The default auto policy prefers a usable checked hardware mapping."},
        .usage{u8"<module> <local-definition-index> <software|auto|arm-mte|x86-mtt>"},
        .handle = &details::wasm_policy_callback<details::policy_action::memory_backend>,
        .cate = ::uwvm2::utils::cmdline::categorization::wasm};

    [[nodiscard]] inline constexpr bool is_wasm_module_policy_parameter(::uwvm2::utils::cmdline::parameter const* parameter) noexcept
    {
        return parameter == &wasm_policy_global || parameter == &wasm_policy_group || parameter == &wasm_policy_group_add_module ||
            parameter == &wasm_policy_single || parameter == &wasm_policy_memory_backend;
    }
}
