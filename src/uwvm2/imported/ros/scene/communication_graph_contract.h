/** @file
 * @brief Canonical bounded graph snapshot/watch records, without ambient discovery.
 * @details The profile and context identify a preconfigured filtered observer.
 * Decoding never grants context.describe, graph.snapshot or graph.watch; the
 * broker must hold their independent admission leases through the observation.
 * Cursors are opaque provider-owned revision/owner/filter/expiry references.
 * An entity body contains kind, approved node, endpoint and optional type text;
 * unavailable or ungranted type information stays empty, never guessed.
 */
#pragma once
#include <array>
#include <string_view>
#define UWVM_ROS_GRAPH_FUNCTIONS(X)                                                                                                                            \
    X(graph_snapshot, snapshot, "graph.snapshot")                                                                                                              \
    X(graph_watch, watch, "graph.watch")
#ifndef UWVM_SCENE_GRAPH_CONTRACT_MACROS_ONLY
# include "communication_graph.h"

namespace uwvm2::imported::ros::graph_contract
{
    namespace wire = scene::wire;
    namespace graph = scene::communication::graph;
    enum class operation : std::uint8_t
    {
        snapshot,
        watch
    };

    struct descriptor
    {
        operation id;
        std::string_view name;
    };

    inline constexpr std::array operations{
# define UWVM_GRAPH_DESCRIPTOR(symbol, action, name) descriptor{operation::action, name},
        UWVM_ROS_GRAPH_FUNCTIONS(UWVM_GRAPH_DESCRIPTOR)
# undef UWVM_GRAPH_DESCRIPTOR
    };

    struct request
    {
        std::uint64_t context_capability{}, context{}, observer_node_capability{},observer_node{}, limit{};
        std::string_view profile_identity;
        wire::bytes profile_digest;
        std::optional<std::uint64_t> cursor;
    };

    inline bool decode(operation action, wire::bytes bytes, request& output) noexcept
    {
        wire::reader input{bytes};
        request value;
        if(!input.u64(value.context_capability) || !value.context_capability || !input.u64(value.context) || !value.context ||
           !input.u64(value.observer_node_capability)||!value.observer_node_capability||!input.u64(value.observer_node)||!value.observer_node||
           !input.text(value.profile_identity) || value.profile_identity.empty() ||
           value.profile_identity.size() > 255 || !input.blob(value.profile_digest) || value.profile_digest.size() != 32)
        {
            return false;
        }
        if(action == operation::snapshot)
        {
            bool has_cursor{};
            std::uint64_t cursor{};
            if(!input.boolean(has_cursor) || (has_cursor && (!input.u64(cursor) || !cursor)) || !input.u64(value.limit) || !value.limit) { return false; }
            if(has_cursor) { value.cursor = cursor; }
        }
        else if(action != operation::watch) { return false; }
        if(!input.done()) { return false; }
        output = value;
        return true;
    }

    inline bool matches(request const& request, scene::communication::profile const& profile) noexcept
    {
        return request.profile_identity == profile.identity && request.profile_digest.size() == profile.digest.size() &&
               std::equal(request.profile_digest.begin(), request.profile_digest.end(), profile.digest.begin());
    }

    /** @brief Encode complete canonical page records into caller-reserved output.
     * @details A false result must fail the provider delivery sink, preserving
     * its pinned page/cursor. This encoder never performs an acknowledgement.
     */
    inline bool encode(wire::writer& output, graph::page const& value) noexcept
    {
        if(!value.revision || (value.cursor && !*value.cursor) || !output.u64(value.revision) || !output.boolean(value.cursor.has_value()) ||
           (value.cursor && !output.u64(*value.cursor)) || !output.u64(value.entities.size()))
        {
            return false;
        }
        for(auto const& row: value.entities)
        {
            auto kind = static_cast<std::uint64_t>(row.type);
            if(!kind || (kind & (kind - 1)) || (kind & ~std::uint64_t{31}) || row.node.size() > 255 || row.endpoint.size() > 255 ||
               row.message_type.size() > 255)
            {
                return false;
            }
            std::array<std::uint8_t, 797> body;
            wire::writer fields{body};
            if(!fields.u64(kind) || !fields.text(row.node) || !fields.text(row.endpoint) || !fields.text(row.message_type) ||
               !output.text("uwvm/graph/Entity") || !output.text("scene-canonical") || !output.blob(fields.written()))
            {
                return false;
            }
        }
        return true;
    }
}  // namespace uwvm2::imported::ros::graph_contract
#endif
