/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#ifndef UWVM_MODULE
# include "checkpoint_state.h"
# include <algorithm>
# include <atomic>
# include <utility>
# include <memory>
# include <span>
# include <vector>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::checkpoint
{
    struct checkpoint_index
    {
        ::std::uint64_t identifier{}, parent{}, instruction{}, event_cursor{};
        friend bool operator==(checkpoint_index const&, checkpoint_index const&) = default;
    };
    struct reverse_plan
    {
        error status{error::unavailable_capability};
        checkpoint_index origin{};
        ::std::uint64_t target_instruction{};
    };
    // Planning is not execution or restore authorization. Reverse execution is
    // restore(the nearest ancestor checkpoint) followed by deterministic forward
    // replay to an instruction boundary, as in rr/QEMU. Native PCs and debugger
    // observation-only trace entries cannot satisfy this contract.
    [[nodiscard]] inline reverse_plan plan_reverse(::std::span<checkpoint_index const> catalog,
        ::std::uint64_t current_checkpoint, ::std::uint64_t current_instruction, ::std::uint64_t target_instruction) noexcept
    {
        if(catalog.empty() || target_instruction >= current_instruction) { return {}; }
        ::std::uint64_t previous{};
        for(auto const& item : catalog)
        {
            if(item.identifier <= previous || item.parent >= item.identifier) { return {error::malformed}; }
            previous = item.identifier;
        }
        auto lookup = [&](::std::uint64_t id) -> checkpoint_index const*
        {
            auto const position{::std::lower_bound(catalog.begin(), catalog.end(), id,
                [](checkpoint_index const& item, ::std::uint64_t key) { return item.identifier < key; })};
            if(position == catalog.end() || position->identifier != id) { return nullptr; }
            // Same live catalog owner; checked iterator borrow is local only.
            return ::std::addressof(*position);
        };
        auto const* current{lookup(current_checkpoint)};
        if(current == nullptr || current->instruction > current_instruction) { return {error::malformed}; }
        for(::std::size_t visited{}; current != nullptr && visited != catalog.size(); ++visited)
        {
            if(current->instruction <= target_instruction) { return {error::none, *current, target_instruction}; }
            if(current->parent == 0u) { return {}; }
            auto const* parent{lookup(current->parent)};
            if(parent == nullptr || parent->instruction > current->instruction || parent->event_cursor > current->event_cursor)
            { return {error::malformed}; }
            current = parent; // checked same-catalog ancestry borrow, no wire pointer.
        }
        return {error::malformed};
    }
    struct replay_request
    {
        ::std::uint64_t sequence{}, instruction{}, logical_thread{}, adapter_operation{};
        ::std::uint16_t event_kind{};
        ::std::span<object_id const> capability_ids{};
        ::std::span<value const> arguments{};
    };
    struct replay_result
    {
        error status{error::replay_diverged};
        ::std::span<value const> results{};
        ::std::span<::std::byte const> adapter_payload{};
        ::std::uint64_t error_bits{}, effect_ordinal{};
        // Keep returned views valid across a subsequent bind or log destruction.
        ::std::shared_ptr<void const> lifetime{};
    };
    // Owns copied immutable event data. It grants no host capability, never
    // invokes an import and never writes an external resource. A runtime import
    // adapter must validate and apply the returned payload to its detached
    // virtual resource/guest memory under the real world-stop/replay owner.
    class replay_log
    {
        using image = ::uwvm2::utils::container::std_vector<object>;
        ::std::shared_ptr<image const> events_{};
        ::std::atomic<::std::size_t> cursor_{};
        ::std::atomic<::std::size_t> count_{};
        ::std::atomic_flag applying_{};
        // Never wait for a nested provider/adapter callback: recursive or
        // concurrent operations decline before reading or mutating event data.
        class operation
        {
            ::std::atomic_flag* held_{};
        public:
            explicit operation(::std::atomic_flag& flag) noexcept
            { if(!flag.test_and_set(::std::memory_order_acquire)) { held_ = ::std::addressof(flag); } }
            operation(operation const&) = delete;
            operation& operator=(operation const&) = delete;
            ~operation() { if(held_) { held_->clear(::std::memory_order_release); } }
            explicit operator bool() const noexcept { return held_ != nullptr; }
        };
        [[nodiscard]] replay_result match(replay_request const& request) const noexcept
        {
            auto const cursor{cursor_.load(::std::memory_order_relaxed)};
            if(!events_ || cursor >= events_->size()) { return {}; }
            auto const& item{(*events_)[cursor]};
            if(item.words[0] != request.sequence || item.words[1] != request.instruction || item.words[2] != request.logical_thread ||
               item.words[3] != request.adapter_operation || item.flags != request.event_kind ||
               item.links.size() != request.capability_ids.size() || item.words[4] != request.arguments.size()) { return {}; }
            for(::std::size_t i{}; i != item.links.size(); ++i) { if(item.links[i] != request.capability_ids[i]) { return {}; } }
            for(::std::size_t i{}; i != request.arguments.size(); ++i) { if(item.values[i] != request.arguments[i]) { return {}; } }
            // bind validated the complete counts. The immutable image pins both
            // typed results and payload even after the transaction releases.
            auto const arguments{static_cast<::std::size_t>(item.words[4])};
            return {error::none, ::std::span<value const>{item.values}.subspan(arguments),
                item.bytes, item.words[6], item.words[7], events_};
        }
    public:
        replay_log() = default;
        replay_log(replay_log const&) = delete;
        replay_log& operator=(replay_log const&) = delete;
        [[nodiscard]] error bind(state const& snapshot, limits const& cap = {})
        {
            operation held{applying_};
            if(!held) { return error::unavailable_capability; }
            if(auto const status{validate_graph(snapshot, cap)}; status != error::none) { return status; }
            for(auto const& item : snapshot.objects)
            { if(item.kind == object_kind::host_resource && item.flags == 3u) { return error::non_replayable_import; } }
            auto candidate{::uwvm2::utils::container::make_shared<image>()};
            for(auto const& item : snapshot.objects) { if(item.kind == object_kind::event) { candidate->push_back(item); } }
            if(snapshot.replay_event_cursor > candidate->size()) { return error::invalid_shape; }
            events_ = ::std::move(candidate);
            count_.store(events_->size(), ::std::memory_order_release);
            cursor_.store(static_cast<::std::size_t>(snapshot.replay_event_cursor), ::std::memory_order_release);
            return error::none;
        }
        [[nodiscard]] ::std::size_t cursor() const noexcept { return cursor_.load(::std::memory_order_acquire); }
        [[nodiscard]] bool exhausted() const noexcept { return cursor() == count_.load(::std::memory_order_acquire); }

        // Runtime adapters preflight ALL guest ranges and detached-resource
        // changes before their nonthrowing commit. No external operation is
        // executed here. A rejected/throwing preparation consumes no event;
        // commit and cursor publication form one serialized transaction.
        template<typename Prepare, typename Commit>
        [[nodiscard]] replay_result apply(replay_request const& request, Prepare&& prepare, Commit&& commit)
        {
            static_assert(noexcept(::std::forward<Commit>(commit)(::std::declval<replay_result const&>())));
            operation held{applying_};
            if(!held) { return {error::unavailable_capability}; }
            auto matched{match(request)};
            if(matched.status != error::none) { return matched; }
            auto const status{::std::forward<Prepare>(prepare)(::std::as_const(matched))};
            if(status != error::none) { return {status}; }
            ::std::forward<Commit>(commit)(::std::as_const(matched));
            cursor_.fetch_add(1u, ::std::memory_order_release);
            return matched;
        }
        // Data-only consumers do not apply guest/resource effects. Production
        // adapters must use apply so payload rejection cannot skip an event.
        [[nodiscard]] replay_result consume(replay_request const& request) noexcept
        {
            return apply(request, [](replay_result const&) noexcept { return error::none; },
                [](replay_result const&) noexcept {});
        }
    };
}
