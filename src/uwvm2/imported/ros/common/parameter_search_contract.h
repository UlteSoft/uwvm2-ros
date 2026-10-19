/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include "types.h"

namespace uwvm2::imported::ros
{
    /**
     * Parent search is an additional, explicitly selected ROS1 authority.
     * Existing grants retain exact-name behavior when this field is absent
     * or false. The Worker and Gateway validate its spelling before binding.
     */
    inline bool parameter_search_parents_enabled(capability_spec const& cap) noexcept
    {
        auto field{cap.limits.find("search-parent-namespaces")};
        return cap.operation == "param.search" && field != cap.limits.end() && field->second == "true";
    }

    /**
     * Check a search candidate or returned name without allocating memory.
     *
     * The caller supplies an immutable, validated ROS1 search capability.
     * Its canonical absolute resource is the initial candidate. The final
     * component is the fixed search key; preceding components define the
     * initial namespace. Only that same key in strict ancestor namespaces
     * is additionally permitted when parent search is explicitly enabled.
     * Every candidate must also lie within the segment-aware result prefix.
     * An allowed-name-prefix alone never enables parent search.
     */
    inline bool parameter_search_name_allowed(capability_spec const& cap, ::std::string_view name) noexcept
    {
        if(cap.operation != "param.search") { return false; }
        auto prefix_field{cap.limits.find("allowed-name-prefix")};
        if(prefix_field == cap.limits.end()) { return false; }
        ::std::string_view prefix{prefix_field->second}, initial{cap.resource};
        if(initial.empty() || initial.front() != '/' || (initial.size() != 1 && initial.back() == '/')) { return false; }
        bool within{prefix == "/" || name == prefix ||
            (name.size() > prefix.size() && name.substr(0, prefix.size()) == prefix && name[prefix.size()] == '/')};
        if(!within) { return false; }
        if(name == initial) { return true; }
        if(initial == "/" || !parameter_search_parents_enabled(cap)) { return false; }
        auto separator{initial.rfind('/')};
        auto key{initial.substr(separator)}; // Includes the separator before the fixed leaf key.
        if(name.size() < key.size() || name.substr(name.size() - key.size()) != key) { return false; }
        auto original_namespace{initial.substr(0, separator)};
        auto candidate_namespace{name.substr(0, name.size() - key.size())};
        return candidate_namespace.empty() ||
            (original_namespace.size() > candidate_namespace.size() &&
             original_namespace.substr(0, candidate_namespace.size()) == candidate_namespace &&
             original_namespace[candidate_namespace.size()] == '/');
    }

    /**
     * Advance a canonical candidate to the same leaf key in its parent
     * namespace. This performs one bounded in-place erase; the root-level
     * candidate has no successor. The caller must check name_allowed before
     * each native query, including the candidate returned by this function.
     */
    inline bool parameter_search_next_candidate(::std::string& candidate)
    {
        auto separator{candidate.rfind('/')};
        if(separator == 0 || separator == ::std::string::npos) { return false; }
        auto parent{candidate.rfind('/', separator - 1)};
        if(parent == ::std::string::npos) { return false; }
        candidate.erase(parent, separator - parent);
        return true;
    }
}
