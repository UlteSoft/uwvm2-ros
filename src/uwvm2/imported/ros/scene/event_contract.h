/** @file
 * @brief Allocation-free event-stream records under the canonical scene carrier.
 * @details Decoding supplies no authority: each source capability/resource must
 * be resolved to a trusted generation-fenced source with resource.observe leases.
 * A public source number is the one-based index in the immutable source_ref list,
 * not a native handle. Readiness snapshots do not promise every intermediate
 * edge; lost_before counts discarded retained records (none in this provider).
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#define UWVM_ROS_EVENT_FUNCTIONS(X)                                                                                                                            \
    X(event_stream_open, open, "event.stream.open")                                                                                                            \
    X(event_stream_poll, poll, "event.stream.poll")                                                                                                            \
    X(event_stream_close, close, "event.stream.close")
#ifndef UWVM_SCENE_EVENT_CONTRACT_MACROS_ONLY
# include "event_streams.h"
# include "wire.h"

namespace uwvm2::imported::ros::event_contract
{
    namespace wire = scene::wire;
    namespace events = scene::events;
    enum class operation : std::uint8_t
    {
        open,
        poll,
        close
    };

    struct descriptor
    {
        operation id;
        std::string_view name;
    };

    inline constexpr std::array operations{
# define UWVM_EVENT_DESCRIPTOR(symbol, action, name) descriptor{operation::action, name},
        UWVM_ROS_EVENT_FUNCTIONS(UWVM_EVENT_DESCRIPTOR)
# undef UWVM_EVENT_DESCRIPTOR
    };

    struct source_ref
    { std::uint64_t capability{}, resource{}; };

    struct request
    {
        std::array<source_ref, 64> sources{};
        std::size_t source_count{};
        std::string_view profile_identity;
        wire::bytes profile_digest;
        std::uint64_t items{}, bytes{}, workspace{}, stream{}, limit{}, deadline{};
    };

    inline bool decode(operation action, wire::bytes bytes, request& output) noexcept
    {
        wire::reader input{bytes};
        request value;
        if(action == operation::open)
        {
            std::uint64_t count{};
            if(!input.u64(count) || !count || count > value.sources.size()) { return false; }
            value.source_count = static_cast<std::size_t>(count);
            for(std::size_t i{}; i < value.source_count; ++i)
            {
                if(!input.u64(value.sources[i].capability) || !value.sources[i].capability || !input.u64(value.sources[i].resource) ||
                   !value.sources[i].resource)
                {
                    return false;
                }
            }
            if(!input.text(value.profile_identity) || value.profile_identity.empty() || value.profile_identity.size() > 255 ||
               !input.blob(value.profile_digest) || value.profile_digest.size() != 32 || !input.u64(value.bytes) || !input.u64(value.items) ||
               !input.u64(value.workspace))
            {
                return false;
            }
        }
        else if(action == operation::poll)
        {
            if(!input.u64(value.stream) || !value.stream || !input.u64(value.limit) || !value.limit || !input.u64(value.deadline) || !value.deadline)
            {
                return false;
            }
        }
        else if(action == operation::close)
        {
            if(!input.u64(value.stream) || !value.stream) { return false; }
        }
        else
        {
            return false;
        }
        if(!input.done()) { return false; }
        output = value;
        return true;
    }

    inline bool matches(request const& request, scene::communication::profile const& profile) noexcept
    {
        return request.profile_identity == profile.identity && request.profile_digest.size() == profile.digest.size() &&
               std::equal(request.profile_digest.begin(), request.profile_digest.end(), profile.digest.begin());
    }

    /** @brief Encode only explicitly selected readiness fields; no source bytes. */
    inline bool encode(wire::writer& output, std::span<events::event const> values) noexcept
    {
        if(!output.u64(values.size())) { return false; }
        for(auto const& value: values)
        {
            if(!value.sequence || !value.source_index || !value.source_generation || !value.fields || (value.fields & ~std::uint64_t{3})) { return false; }
            std::array<std::uint8_t, 24> bytes{};
            wire::writer payload{bytes};
            if(!payload.u64(value.fields) || !payload.boolean((value.fields & 1) && value.ready) || !payload.boolean((value.fields & 2) && value.terminal))
            {
                return false;
            }
            if(!output.u64(value.sequence) || !output.u64(value.source_index) || !output.u64(value.source_generation) || !output.text("readiness.snapshot") ||
               !output.boolean(false) || !output.u64(value.lost_before) || !output.text("uwvm/events/Readiness") || !output.text("scene-canonical") ||
               !output.blob(payload.written()))
            {
                return false;
            }
        }
        return true;
    }
}  // namespace uwvm2::imported::ros::event_contract
#endif
