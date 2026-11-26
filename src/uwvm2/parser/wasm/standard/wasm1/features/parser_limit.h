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
    };

    inline constexpr ::std::size_t checked_parser_total(::std::size_t previous, ::std::size_t increment, ::std::size_t limit,
                                                        ::fast_io::u8string_view name, ::std::byte const* at,
                                                        ::uwvm2::parser::wasm::base::error_impl& err) UWVM_THROWS
    {
        if(previous > limit || increment > limit - previous) [[unlikely]]
        {
            constexpr auto maximum{::std::numeric_limits<::std::size_t>::max()};
            err.err_curr = at;
            err.err_code = ::uwvm2::parser::wasm::base::wasm_parse_error_code::exceed_the_max_parser_limit;
            err.err_selectable.exceed_the_max_parser_limit = {name, increment > maximum - previous ? maximum : previous + increment, limit};
            ::uwvm2::parser::wasm::base::throw_wasm_parse_code(::fast_io::parse_code::invalid);
        }
        return previous + increment;
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
        }
    }
}

#ifndef UWVM_MODULE
// macro
# include <uwvm2/utils/macro/pop_macros.h>
#endif
