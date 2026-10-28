/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "event.h"
#include <atomic>
#include <cstddef>

namespace uwvm2::runtime::debug_audit
{
    enum class pressure_policy { drop_new, fail_recording };
    enum class append_result { recorded, dropped, failed, sealed, invalid };
    enum class failure : std::uint32_t
    { overflow = 1u, sink = 2u, invalid_event = 4u, sequence_exhaustion = 8u, revoked = 16u };
    struct record { std::uint64_t sequence{}; event value{}; };
    struct interval_report
    {
        principal owner{};
        std::uint64_t session{}, authorization_generation{}, stream{}, thread{};
        coverage requested{}, producer_support{};
        std::uint64_t attempted{}, recorded{}, dropped{}, invalid{}, consumed{}, delivered{}, sink_failed{};
        std::uint32_t failures{};
        admission admitted{admission::invalid_identity};
        pressure_policy policy{};
        bool sealed{}, drained{}, transport_complete{};
        // transport_complete proves this producer stream only. The runtime's
        // boundary handshake and all participating streams must be checked to
        // establish complete execution coverage for an interval.
    };

    // Exactly one native producer and one consumer per instance. Not callable
    // from signals, recursive hooks or two producers; use another stream for a
    // reentrant host callback. All control changes require the producer's safe
    // point. Construction/authorization occurs before either thread starts.
    template<std::size_t Capacity>
    class stream
    {
        static_assert(Capacity > 0 && Capacity <= 65536);
        static_assert(std::atomic<std::uint64_t>::is_always_lock_free,
                      "This target needs a separately qualified bounded transport");
        std::array<record, Capacity> records_{};
        authorization authorization_{};
        coverage requested_{}, support_{};
        std::uint64_t stream_{}, thread_{};
        pressure_policy policy_{};
        admission admission_{};
        // Counters below are producer-owned until sealed_. Consumer counters
        // are only read by that consumer. No racy diagnostic snapshot API.
        std::uint64_t attempted_{}, dropped_{}, invalid_{};
        alignas(64) std::atomic<std::uint64_t> head_{};
        alignas(64) std::atomic<std::uint64_t> tail_{};
        std::uint64_t delivered_{}, sink_failed_{};
        std::atomic<std::uint32_t> failures_{};
        std::atomic<bool> sealed_{};

        void mark(failure why) noexcept
        { failures_.fetch_or(static_cast<std::uint32_t>(why), std::memory_order_relaxed); }
    public:
        stream(authorization auth, principal live, coverage requested, coverage support,
               std::uint64_t stream_id, std::uint64_t thread_id, pressure_policy policy) noexcept :
            authorization_{auth}, requested_{requested}, support_{support}, stream_{stream_id}, thread_{thread_id}, policy_{policy},
            admission_{authorize(auth, live, requested, support)}
        {
            if(!stream_id || !thread_id) { admission_ = admission::invalid_stream; }
            if(policy != pressure_policy::drop_new && policy != pressure_policy::fail_recording) { admission_ = admission::invalid_policy; }
        }
        stream(stream const&) = delete;
        stream& operator=(stream const&) = delete;
        [[nodiscard]] admission admitted() const noexcept { return admission_; }

        [[nodiscard]] append_result append(event const& value) noexcept
        {
            if(admission_ != admission::accepted) { return append_result::invalid; }
            if(sealed_.load(std::memory_order_acquire)) { return append_result::sealed; }
            if(attempted_ == std::numeric_limits<std::uint64_t>::max())
            { mark(failure::sequence_exhaustion); return append_result::failed; }
            auto const sequence{++attempted_};
            if(!valid(value) || !(requested_ & bit(value.category)))
            { ++invalid_; mark(failure::invalid_event); return append_result::invalid; }
            auto const failed{failures_.load(std::memory_order_acquire)};
            if((failed & (static_cast<std::uint32_t>(failure::sink) | static_cast<std::uint32_t>(failure::revoked))) ||
               (policy_ == pressure_policy::fail_recording && failed))
            { ++dropped_; return append_result::failed; }
            auto const head{head_.load(std::memory_order_relaxed)};
            if(head - tail_.load(std::memory_order_acquire) == Capacity)
            {
                ++dropped_; mark(failure::overflow);
                return policy_ == pressure_policy::fail_recording ? append_result::failed : append_result::dropped;
            }
            records_[head % Capacity] = {sequence, value};
            head_.store(head + 1, std::memory_order_release);
            return append_result::recorded;
        }

        // Consumer retains the slot until the sink accepts or explicitly fails.
        // A failed/partial sink must ack(false), never claim durable delivery.
        [[nodiscard]] bool peek(record& output) const noexcept
        {
            auto const tail{tail_.load(std::memory_order_relaxed)};
            if(tail == head_.load(std::memory_order_acquire)) { return false; }
            output = records_[tail % Capacity]; return true;
        }
        [[nodiscard]] bool acknowledge(std::uint64_t sequence, bool delivered) noexcept
        {
            auto const tail{tail_.load(std::memory_order_relaxed)};
            if(tail == head_.load(std::memory_order_acquire) || records_[tail % Capacity].sequence != sequence) { return false; }
            if(delivered) { ++delivered_; } else { ++sink_failed_; mark(failure::sink); }
            tail_.store(tail + 1, std::memory_order_release); return true;
        }
        // Native producer-only: external revocation first rendezvous all guest
        // participants, then seal. Never race a control thread with append.
        void seal() noexcept { sealed_.store(true, std::memory_order_release); }
        void revoke() noexcept { mark(failure::revoked); seal(); }

        // Consumer-only. Producer counters become visible only after seal. A
        // caller cannot interpret an in-flight snapshot as a complete interval.
        [[nodiscard]] interval_report report() const noexcept
        {
            interval_report result{};
            result.owner = authorization_.subject; result.session = authorization_.session;
            result.admitted = admission_; result.policy = policy_;
            result.authorization_generation = authorization_.authorization_generation;
            result.stream = stream_; result.thread = thread_; result.requested = requested_; result.producer_support = support_;
            result.sealed = sealed_.load(std::memory_order_acquire);
            result.recorded = head_.load(std::memory_order_acquire);
            result.consumed = tail_.load(std::memory_order_relaxed);
            result.delivered = delivered_; result.sink_failed = sink_failed_;
            result.failures = failures_.load(std::memory_order_relaxed);
            result.drained = result.sealed && result.recorded == result.consumed;
            if(result.sealed) { result.attempted = attempted_; result.dropped = dropped_; result.invalid = invalid_; }
            result.transport_complete = admission_ == admission::accepted && result.drained && !result.failures &&
                result.attempted == result.recorded && result.delivered == result.recorded;
            return result;
        }
    };
}
