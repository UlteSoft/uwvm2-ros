/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <mutex>
#include "../policy/schema.h"

namespace uwvm2::imported::ros
{
    enum class goal_phase : ::std::uint8_t { pending, active, cancelling, succeeded, aborted, cancelled };

    // Ownership primitive for a future generated Action adapter. These methods
    // neither create a ROS action nor make unsupported Action grants admissible.
    class goal_ownership_table
    {
        struct goal
        {
            void const* owner{};
            ::std::uint64_t runtime_generation{};
            version ros_version{};
            ::std::string resource{};
            ::std::string backend_goal_id{};
            ::std::uint32_t generation{1};
            goal_phase phase{goal_phase::pending};
            bool live{true};
        };
        ::std::mutex mutex{};
        ::std::map<::std::uint32_t, goal> goals{};
        ::std::uint64_t next_slot{1};
        static bool terminal(goal_phase p) noexcept { return p == goal_phase::succeeded || p == goal_phase::aborted || p == goal_phase::cancelled; }
        goal* lookup(::std::uint64_t handle)
        {
            auto it{goals.find(static_cast<::std::uint32_t>(handle))};
            if(it == goals.end() || !it->second.live || it->second.generation != handle >> 32) { return nullptr; }
            return &it->second;
        }
    public:
        result create(void const* owner, ::std::uint64_t runtime_generation, version v, ::std::string_view resource,
                      ::std::string_view backend_goal_id, ::std::uint64_t maximum_active, ::std::uint64_t& handle)
        {
            handle = 0; ::std::lock_guard guard{mutex};
            if(!owner || !valid_version(v) || !canonical_resource(resource) || backend_goal_id.empty() || backend_goal_id.size() > 256 || !maximum_active || maximum_active > 1048576)
            { return {errc::invalid_argument, "trusted owner, exact resource, generated goal identity and explicit active limit required"}; }
            ::std::uint64_t active{};
            for(auto const& item : goals)
            {
                auto const& g{item.second};
                if(g.live && g.ros_version == v && g.resource == resource && g.backend_goal_id == backend_goal_id) { return {errc::conflict, "backend goal already has an owner"}; }
                if(g.live && !terminal(g.phase) && g.owner == owner && g.runtime_generation == runtime_generation && g.ros_version == v && g.resource == resource) { ++active; }
            }
            if(active >= maximum_active) { return {errc::rate_limited, "explicit active goal limit exhausted"}; }
            if(next_slot > ::std::numeric_limits<::std::uint32_t>::max()) { return {errc::too_large, "goal namespace exhausted"}; }
            auto slot{static_cast<::std::uint32_t>(next_slot++)};
            goals.emplace(slot, goal{owner, runtime_generation, v, ::std::string(resource), ::std::string(backend_goal_id)});
            handle = (1ULL << 32) | slot;
            return {};
        }
        result cancel(void const* caller, ::std::uint64_t runtime_generation, version v, ::std::uint64_t handle, capability_spec const& cancel_cap)
        {
            ::std::lock_guard guard{mutex}; auto g{lookup(handle)};
            if(!g || g->ros_version != v) { return {errc::bad_handle, "unknown or cross-version goal"}; }
            if(!caller || cancel_cap.resource != g->resource) { return {errc::permission_denied, "goal resource differs from explicit cancellation grant"}; }
            bool own{cancel_cap.operation == "action.cancel-own-goal"};
            bool any{cancel_cap.operation == "action.cancel-any-goal"};
            auto scope{cancel_cap.limits.find("cancel-scope")};
            if((!own && !any) || scope == cancel_cap.limits.end() || scope->second != (own ? "own" : "any"))
            { return {errc::permission_denied, "separate explicit cancellation capability required"}; }
            if(own && (g->owner != caller || g->runtime_generation != runtime_generation)) { return {errc::permission_denied, "goal belongs to another runtime instance"}; }
            if(terminal(g->phase)) { return {errc::conflict, "terminal goal cannot be cancelled"}; }
            g->phase = goal_phase::cancelling;
            return {};
        }
        result deliver(void const* caller, ::std::uint64_t runtime_generation, version v, ::std::uint64_t handle,
                       capability_spec const& receive_cap)
        {
            ::std::lock_guard guard{mutex}; auto g{lookup(handle)};
            if(!g || g->ros_version != v) { return {errc::bad_handle, "unknown or cross-version goal"}; }
            if(g->owner != caller || g->runtime_generation != runtime_generation || receive_cap.resource != g->resource ||
               (receive_cap.operation != "action.receive-feedback" && receive_cap.operation != "action.receive-result"))
            { return {errc::permission_denied, "goal event requires its actual owner and independent receive capability"}; }
            if(receive_cap.operation == "action.receive-feedback" && terminal(g->phase)) { return {errc::revoked, "feedback after terminal result forbidden"}; }
            return {};
        }
        result transition(::std::uint64_t handle, goal_phase next)
        {
            ::std::lock_guard guard{mutex}; auto g{lookup(handle)};
            if(!g) { return {errc::bad_handle, "stale goal"}; }
            bool allowed{(g->phase == goal_phase::pending && (next == goal_phase::active || next == goal_phase::aborted)) ||
                         (g->phase == goal_phase::active && (next == goal_phase::succeeded || next == goal_phase::aborted)) ||
                         (g->phase == goal_phase::cancelling && (next == goal_phase::cancelled || next == goal_phase::succeeded || next == goal_phase::aborted))};
            if(!allowed) { return {errc::conflict, "invalid generated goal lifecycle transition"}; }
            g->phase = next;
            return {};
        }
        result close(void const* owner, ::std::uint64_t runtime_generation, version v, ::std::uint64_t handle)
        {
            ::std::lock_guard guard{mutex}; auto g{lookup(handle)};
            if(!g || g->ros_version != v) { return {errc::bad_handle, "unknown or stale goal"}; }
            if(g->owner != owner || g->runtime_generation != runtime_generation) { return {errc::permission_denied, "only actual owner may close goal"}; }
            if(!terminal(g->phase)) { return {errc::conflict, "active goal must complete or cancel before release"}; }
            // Slots are never reused, so no stale goal can become valid again.
            goals.erase(static_cast<::std::uint32_t>(handle));
            return {};
        }
        void teardown(void const* owner, ::std::uint64_t runtime_generation)
        {
            ::std::lock_guard guard{mutex};
            for(auto it{goals.begin()}; it != goals.end();)
            { if(it->second.owner == owner && it->second.runtime_generation == runtime_generation) { it = goals.erase(it); } else { ++it; } }
        }
    };
}
