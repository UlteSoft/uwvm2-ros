/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
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

#ifndef UWVM_MODULE
// std
# include <cstdint>
# include <limits>
# include <optional>
# include "memory_limit.h"
# include <uwvm2/extension/memtag/execution_policy.h>
# include <uwvm2/uwvm/runtime/runtime_mode/storage.h>
# include <cstddef>
# include <cstring>
# include <new>
# include <memory>
# include <type_traits>
// macro
# include <uwvm2/utils/macro/push_macros.h>
// import
# include <fast_io.h>
# include <uwvm2/parser/wasm/concepts/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/type/impl.h>
# include <uwvm2/parser/wasm_custom/customs/impl.h>
# include <uwvm2/uwvm/wasm/base/impl.h>
# include <uwvm2/uwvm/wasm/feature/impl.h>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::wasm::type
{
    struct module_initializer_limits
    {
        static constexpr ::std::size_t inherited{(::std::numeric_limits<::std::size_t>::max)()};
        ::std::size_t imported_functions{inherited};
        ::std::size_t imported_tables{inherited};
        ::std::size_t imported_memories{inherited};
        ::std::size_t imported_globals{inherited};
        ::std::size_t imported_tags{inherited};
        ::std::size_t local_defined_functions{inherited};
        ::std::size_t local_defined_codes{inherited};
        ::std::size_t local_defined_tables{inherited};
        ::std::size_t local_defined_memories{inherited};
        ::std::size_t local_defined_globals{inherited};
        ::std::size_t local_defined_tags{inherited};
        ::std::size_t local_defined_elements{inherited};
        ::std::size_t local_defined_datas{inherited};
    };

    struct wasm_parameter_t
    {
        ::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_feature_parameter_storage_t binfmt1_para{};
        ::uwvm2::parser::wasm_custom::customs::name_parser_param_t name_parser{};
        module_initializer_limits initializer_limits{};
        ::uwvm2::uwvm::runtime::runtime_mode::module_translation_policy translation{};
        ::uwvm2::extension::memtag::module_memory_policy memory_policy{};
        // Only a fresh CLI load resolves host policy. Owned-source replay and
        // embedding APIs consume the caller's already-resolved snapshot.
        bool resolve_module_policy{};
        // Resolved module default for each locally defined memory. Indexed overrides belong to the owner.
        ::std::optional<module_memory_limit_t> memory_pages{};
        static_assert(::std::is_trivially_copyable_v<decltype(binfmt1_para)> && ::std::is_trivially_destructible_v<decltype(binfmt1_para)>);
    };

    static_assert(::std::is_trivially_copyable_v<wasm_parameter_t> && ::std::is_trivially_destructible_v<wasm_parameter_t>);
}  // namespace uwvm2::uwvm::wasm::storage

#ifndef UWVM_MODULE
// macro
# include <uwvm2/utils/macro/pop_macros.h>
#endif
