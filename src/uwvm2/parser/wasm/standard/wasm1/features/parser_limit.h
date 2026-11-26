/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @brief       WebAssembly Release 1.0 (2019-07-20)
 * @details     antecedent dependency: null
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-04-16
 * @copyright   APL-2.0 License
 */

/****************************************
 *  _   _ __        ____     __ __  __  *
 * | | | |\ \      / /\ \   / /|  \/  | *
 * | | | | \ \ /\ / /  \ \ / / | |\/| | *
 * | |_| |  \ V  V /    \ V /  | |  | | *
 *  \___/    \_/\_/      \_/   |_|  |_| *
 *                                      *
 ****************************************/

#pragma once

#if !(__cpp_pack_indexing >= 202311L)
# error "UWVM requires at least C++26 standard compiler. See https://en.cppreference.com/w/cpp/feature_test#cpp_pack_indexing"
#endif

#ifndef UWVM_MODULE
// std
# include <cstddef>
# include <cstdint>
# include <limits>
# include <climits>
# include <concepts>
# include <type_traits>
# include <utility>
# include <memory>
# include <compare>
# include <algorithm>
# include <compare>
// macro
# include <uwvm2/utils/macro/push_macros.h>
// import
# include "feature_def.h"
# include <uwvm2/utils/allocator/resource_limit.h>
# include <fast_io.h>
# include <uwvm2/parser/wasm/base/impl.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/utils/hash/impl.h>
# include <uwvm2/utils/debug/impl.h>
# include <uwvm2/parser/wasm/concepts/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/type/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/section/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/opcode/impl.h>
# include <uwvm2/parser/wasm/binfmt/binfmt_ver1/impl.h>
# include <uwvm2/parser/wasm/text_format/impl.h>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::parser::wasm::standard::wasm1::features
{

    template <::std::unsigned_integral From>
    inline constexpr ::std::size_t saturating_cast_size_t(From value) noexcept
    {
        if constexpr(::std::numeric_limits<From>::max() <= ::std::numeric_limits<::std::size_t>::max()) { return static_cast<::std::size_t>(value); }
        else
        {
            constexpr auto max_val{static_cast<From>(::std::numeric_limits<::std::size_t>::max())};
            return value > max_val ? ::std::numeric_limits<::std::size_t>::max() : static_cast<::std::size_t>(value);
        }
    }

    inline constexpr ::std::size_t default_max_code_sec_codes{saturating_cast_size_t(262144u)};
    inline constexpr ::std::size_t default_max_code_locals{saturating_cast_size_t(65536u)};
    inline constexpr ::std::size_t default_max_data_sec_entries{saturating_cast_size_t(262144u)};
    inline constexpr ::std::size_t default_max_elem_sec_funcidx{saturating_cast_size_t(262144u)};
    inline constexpr ::std::size_t default_max_elem_sec_elems{saturating_cast_size_t(262144u)};
    inline constexpr ::std::size_t default_max_export_sec_exports{saturating_cast_size_t(262144u)};
    inline constexpr ::std::size_t default_max_global_sec_globals{saturating_cast_size_t(262144u)};
    inline constexpr ::std::size_t default_max_import_sec_imports{saturating_cast_size_t(262144u)};
    inline constexpr ::std::size_t default_max_memory_sec_memories{saturating_cast_size_t(1024u)};
    inline constexpr ::std::size_t default_max_table_sec_tables{saturating_cast_size_t(1024u)};
    inline constexpr ::std::size_t default_max_type_sec_types{saturating_cast_size_t(262144u)};

    inline constexpr ::std::size_t default_max_const_expr_opcodes{saturating_cast_size_t(1048576u)};

    inline constexpr ::std::size_t default_max_section_storage_bytes{saturating_cast_size_t(256ull << 20u)};
    inline constexpr ::std::size_t default_max_custom_sections{saturating_cast_size_t(65536u)};

    struct wasm1_parser_limit_t
    {
        ::std::size_t max_code_sec_codes{default_max_code_sec_codes};
        ::std::size_t max_code_locals{default_max_code_locals};
        ::std::size_t max_data_sec_entries{default_max_data_sec_entries};
        ::std::size_t max_elem_sec_funcidx{default_max_elem_sec_funcidx};
        ::std::size_t max_elem_sec_elems{default_max_elem_sec_elems};
        ::std::size_t max_export_sec_exports{default_max_export_sec_exports};
        ::std::size_t max_global_sec_globals{default_max_global_sec_globals};
        ::std::size_t max_import_sec_imports{default_max_import_sec_imports};
        ::std::size_t max_memory_sec_memories{default_max_memory_sec_memories};
        ::std::size_t max_table_sec_tables{default_max_table_sec_tables};
        ::std::size_t max_type_sec_types{default_max_type_sec_types};
        ::std::size_t max_const_expr_opcodes{default_max_const_expr_opcodes};
        ::std::size_t max_section_storage_bytes{default_max_section_storage_bytes};
        ::std::size_t max_custom_sections{default_max_custom_sections};
        // The host resource thresholds apply when a module has no tighter override.
        inline static constexpr auto inherit_resource_limit{::std::numeric_limits<::std::size_t>::max()};
        ::std::size_t max_type_metadata_bytes{inherit_resource_limit};
        ::std::size_t max_function_body_bytes{inherit_resource_limit};
        ::std::size_t max_module_code_bytes{inherit_resource_limit};
        ::std::size_t max_input_bytes{inherit_resource_limit};
    };

    struct code_body_limits
    {
        ::std::size_t function_bytes;
        ::std::size_t module_bytes;

        // Encoded body length includes local declarations and the final end opcode.
        // The surrounding code count and body-length LEB belong to the section.
        [[nodiscard]] constexpr bool append(::std::size_t& total, ::std::size_t bytes) const noexcept
        {
            if(bytes > function_bytes || total > module_bytes || bytes > module_bytes - total) { return false; }
            total += bytes;
            return true;
        }

        template<typename SizeAt>
        [[nodiscard]] constexpr bool admits(::std::size_t functions, SizeAt size_at) const noexcept
        {
            ::std::size_t total{};
            for(::std::size_t i{}; i != functions; ++i)
            { if(!append(total, size_at(i))) { return false; } }
            return true;
        }
    };

    template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
    [[nodiscard]] inline constexpr code_body_limits effective_code_body_limits(
        ::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const& parameters) noexcept
    {
        code_body_limits result{saturating_cast_size_t(8ull << 20u), saturating_cast_size_t(64ull << 20u)};
        if !consteval
        {
            result = {::uwvm2::utils::allocator::resource::function_body_bytes, ::uwvm2::utils::allocator::resource::module_code_bytes};
        }
        if constexpr((::std::same_as<wasm1, Fs> || ...))
        {
            auto const& limits{::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<wasm1>(parameters).parser_limit};
            result.function_bytes = (::std::min)(result.function_bytes, limits.max_function_body_bytes);
            result.module_bytes = (::std::min)(result.module_bytes, limits.max_module_code_bytes);
        }
        return result;
    }

    inline constexpr ::std::size_t checked_parser_total(::std::size_t previous, ::std::size_t count, ::std::size_t limit,
                                                        ::fast_io::u8string_view name, ::std::byte const* at,
                                                        ::uwvm2::parser::wasm::base::error_impl& err, ::std::size_t width = 1uz) UWVM_THROWS
    {
        if(previous > limit || count > (limit - previous) / width) [[unlikely]]
        {
            constexpr auto maximum{::std::numeric_limits<::std::size_t>::max()};
            auto const requested{count > (maximum - previous) / width ? maximum : previous + count * width};
            err.err_curr = at;
            err.err_code = ::uwvm2::parser::wasm::base::wasm_parse_error_code::exceed_the_max_parser_limit;
            err.err_selectable.exceed_the_max_parser_limit = {name, requested, limit};
            ::uwvm2::parser::wasm::base::throw_wasm_parse_code(::fast_io::parse_code::invalid);
        }
        return previous + count * width;
    }

    template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
    inline constexpr ::std::size_t section_storage_limit(::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const& parameters) noexcept
    {
        ::std::size_t limit{default_max_section_storage_bytes};
        if constexpr((::std::same_as<wasm1, Fs> || ...))
        { limit = ::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<wasm1>(parameters).parser_limit.max_section_storage_bytes; }
        return ::std::min(limit, static_cast<::std::size_t>(::std::numeric_limits<::std::ptrdiff_t>::max()));
    }

    template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
    inline constexpr void count_section_storage(::uwvm2::parser::wasm::binfmt::ver1::wasm_binfmt_ver1_module_extensible_storage_t<Fs...>& module,
                                                ::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const& parameters,
                                                ::std::size_t count, ::std::size_t width, ::std::byte const* at,
                                                ::uwvm2::parser::wasm::base::error_impl& err) UWVM_THROWS
    {
        module.section_storage_bytes = checked_parser_total(module.section_storage_bytes, count, section_storage_limit(parameters),
                                                           u8"section_storage_bytes", at, err, width);
    }

    template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
    inline constexpr ::std::size_t type_storage_limit(::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const& parameters) noexcept
    {
        auto limit{saturating_cast_size_t(512ull << 20u)};
        if !consteval { limit = ::uwvm2::utils::allocator::resource::parser_metadata_bytes; }
        if constexpr((::std::same_as<wasm1, Fs> || ...))
        { limit = ::std::min(limit, ::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<wasm1>(parameters).parser_limit.max_type_metadata_bytes); }
        return limit;
    }

    template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
    inline constexpr ::std::size_t remaining_type_storage(::uwvm2::parser::wasm::binfmt::ver1::wasm_binfmt_ver1_module_extensible_storage_t<Fs...> const& module,
                                                         ::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const& parameters) noexcept
    {
        auto const& types{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<type_section_storage_t<Fs...>>(module.sections)};
        auto const type_limit{type_storage_limit(parameters)};
        auto const section_limit{section_storage_limit(parameters)};
        if(types.type_storage_bytes >= type_limit || module.section_storage_bytes >= section_limit) { return 0uz; }
        return ::std::min(type_limit - types.type_storage_bytes, section_limit - module.section_storage_bytes);
    }

    template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
    inline constexpr void count_type_storage(::uwvm2::parser::wasm::binfmt::ver1::wasm_binfmt_ver1_module_extensible_storage_t<Fs...>& module,
                                             ::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const& parameters,
                                             ::std::size_t count, ::std::size_t width, ::std::byte const* at,
                                             ::uwvm2::parser::wasm::base::error_impl& err) UWVM_THROWS
    {
        auto& types{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<type_section_storage_t<Fs...>>(module.sections)};
        auto const total{checked_parser_total(types.type_storage_bytes, count, type_storage_limit(parameters), u8"type-metadata-bytes", at, err, width)};
        count_section_storage(module, parameters, count, width, at, err);
        types.type_storage_bytes = total;
    }

    template <typename Char>
    inline constexpr void check_encoded_count(::std::size_t count, ::std::size_t minimum_record_bytes, Char const* after_count,
                                              ::std::byte const* end, ::uwvm2::parser::wasm::base::wasm_parse_error_code invalid_count,
                                              ::uwvm2::parser::wasm::base::error_impl& err) UWVM_THROWS
    {
        auto const* records{reinterpret_cast<::std::byte const*>(after_count)};
        if(count > static_cast<::std::size_t>(end - records) / minimum_record_bytes) [[unlikely]]
        {
            err.err_curr = records;
            err.err_code = invalid_count;
            ::uwvm2::parser::wasm::base::throw_wasm_parse_code(::fast_io::parse_code::invalid);
        }
    }

    template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
    inline constexpr void count_const_expr_opcode(::uwvm2::parser::wasm::binfmt::ver1::wasm_binfmt_ver1_module_extensible_storage_t<Fs...>& module,
                                                  ::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const& parameters,
                                                  ::std::byte const* opcode, ::uwvm2::parser::wasm::base::error_impl& err) UWVM_THROWS
    {
        if constexpr((::std::same_as<wasm1, Fs> || ...))
        {
            auto& count{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<global_section_storage_t<Fs...>>(
                module.sections).const_expr_opcode_count};
            auto const limit{::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<wasm1>(parameters).parser_limit.max_const_expr_opcodes};
            count = checked_parser_total(count, 1uz, limit, u8"const_expr_opcodes", opcode, err);
            using expr = final_wasm_const_expr<Fs...>;
            count_section_storage(module, parameters, 1uz, sizeof(typename decltype(expr::opcodes)::value_type), opcode, err);
        }
    }
}

#ifndef UWVM_MODULE
// macro
# include <uwvm2/utils/macro/pop_macros.h>
#endif
