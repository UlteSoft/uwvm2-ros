/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-06-30
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

module;
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/function_ref.h>
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/allocator/resource_limit.h>

// std
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <climits>
#include <concepts>
#include <memory>
#include <utility>
#include <type_traits>
#include <string_view>
#include <set>  /// @todo replace with btreemap
#include <map>  /// @todo replace with btreemap
// platform
#include <bizwen/deque.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <boost/unordered/unordered_node_map.hpp>
#include <boost/unordered/unordered_node_set.hpp>

export module uwvm2.utils.container:wrapper;



import fast_io;
import uwvm2.utils.hash;
import :allocator;

// Export the same global-module allocator-backed types used by header builds.
export namespace uwvm2::utils::container
{
    using ::uwvm2::utils::container::standard_allocator;
    using ::uwvm2::utils::container::std_vector;
    using ::uwvm2::utils::container::std_deque;
    using ::uwvm2::utils::container::std_list;
    using ::uwvm2::utils::container::std_map;
    using ::uwvm2::utils::container::std_set;
    using ::uwvm2::utils::container::standard_string;
    using ::uwvm2::utils::container::std_string;
    using ::uwvm2::utils::container::std_u8string;
    using ::uwvm2::utils::container::std_wstring;
    using ::uwvm2::utils::container::std_u16string;
    using ::uwvm2::utils::container::std_u32string;
    using ::uwvm2::utils::container::std_istringstream;
    using ::uwvm2::utils::container::std_ostringstream;
    using ::uwvm2::utils::container::copy_string;
    using ::uwvm2::utils::container::ostring_ref;
    using ::uwvm2::utils::container::u8ostring_ref;
    using ::uwvm2::utils::container::concat;
    using ::uwvm2::utils::container::u8concat;
    using ::uwvm2::utils::container::to_string;
    using ::uwvm2::utils::container::strict_object_deleter;
    using ::uwvm2::utils::container::adopt_shared;
    using ::uwvm2::utils::container::make_shared;
    using ::uwvm2::utils::container::strict_owned_ptr;
    using ::uwvm2::utils::container::std_unique_ptr;
    using ::uwvm2::utils::container::make_unique;
    using ::uwvm2::utils::container::make_unique_for_overwrite;
    using ::uwvm2::utils::container::function_ref;
}

#ifndef UWVM_MODULE
# define UWVM_MODULE
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT export
#endif

#include "wrapper.h"
