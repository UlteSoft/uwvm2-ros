/** @file
 * @brief Canonical communication records without native handles or ambient ROS authority.
 * @details Parsing borrows only the carrier's owned immutable request. The
 * broker resolves every profile/node/capability and holds independent source
 * authority leases through provider effects. RMW timestamp numbers do not prove
 * a clock identity or epoch: they remain absent unless a trusted profile supplies
 * the corresponding mapping. Publisher IDs are opaque observations, not peers'
 * authentication credentials. A successful receipt never implies actuator safety.
 */
#pragma once
#include <array>
#include <string_view>
#define UWVM_ROS_COMMUNICATION_FUNCTIONS(X)                                                                                                                    \
    X(context_open, context_open, "context.open")                                                                                                              \
    X(context_describe, context_describe, "context.describe")                                                                                                  \
    X(context_shutdown, context_shutdown, "context.shutdown")                                                                                                  \
    X(node_open, node_open, "node.open")                                                                                                                       \
    X(node_describe, node_describe, "node.describe")                                                                                                           \
    X(type_describe, type_describe, "type.describe")                                                                                                           \
    X(topic_open, topic_open, "topic.open")                                                                                                                    \
    X(topic_publish, topic_publish, "topic.publish")                                                                                                           \
    X(topic_take, topic_take, "topic.take")                                                                                                                    \
    X(topic_describe, topic_describe, "topic.describe")                                                                                                        \
    X(qos_describe, qos_describe, "qos.describe")                                                                                                              \
    X(qos_liveliness_assert, qos_liveliness_assert, "qos.liveliness.assert")                                                                                   \
    X(qos_acknowledgements_wait, qos_acknowledgements_wait, "qos.acknowledgements.wait")                                                                       \
    X(service_open, service_open, "service.open")                                                                                                              \
    X(service_submit, service_submit, "service.submit")                                                                                                        \
    X(service_take, service_take, "service.take")                                                                                                              \
    X(service_respond, service_respond, "service.respond")                                                                                                     \
    X(service_available, service_available, "service.available")                                                                                               \
    X(service_abandon, service_abandon, "service.abandon")                                                                                                    \
    X(service_describe, service_describe, "service.describe")
#ifndef UWVM_SCENE_COMMUNICATION_CONTRACT_MACROS_ONLY
# include "communication.h"
# include "wire.h"

namespace uwvm2::imported::ros::communication_contract
{
    namespace wire = scene::wire;
    namespace communication = scene::communication;
    enum class operation : std::uint8_t
    {
# define UWVM_COMMUNICATION_ENUM(symbol, action, name) action,
        UWVM_ROS_COMMUNICATION_FUNCTIONS(UWVM_COMMUNICATION_ENUM)
# undef UWVM_COMMUNICATION_ENUM
    };

    struct descriptor
    {
        operation id;
        std::string_view name;
    };

    inline constexpr std::array operations{
# define UWVM_COMMUNICATION_DESCRIPTOR(symbol, action, name) descriptor{operation::action, name},
        UWVM_ROS_COMMUNICATION_FUNCTIONS(UWVM_COMMUNICATION_DESCRIPTOR)
# undef UWVM_COMMUNICATION_DESCRIPTOR
    };

    struct profile_view
    {
        std::string_view identity;
        wire::bytes digest;
    };

    struct message_view
    {
        std::string_view identity, encoding;
        wire::bytes body;
    };

    struct endpoint_view
    {
        std::uint64_t node_capability{},node{};
        std::array<std::uint64_t,3> schema_capabilities{};
        std::size_t schema_count{};
        profile_view profile;
        std::string_view name, type, role;
    };

    inline bool read_profile(wire::reader& input, profile_view& output) noexcept
    {
        return input.text(output.identity) && !output.identity.empty() && output.identity.size() <= 255 && input.blob(output.digest) &&
               output.digest.size() == 32;
    }

    inline bool read_message(wire::reader& input, message_view& output) noexcept
    {
        return input.text(output.identity) && !output.identity.empty() && output.identity.size() <= 255 && input.text(output.encoding) &&
               !output.encoding.empty() && output.encoding.size() <= 255 && input.blob(output.body);
    }

    inline bool read_endpoint(wire::reader& input, endpoint_view& output) noexcept
    {
        std::uint64_t count{};
        if(!input.u64(output.node_capability)||!output.node_capability||!input.u64(output.node)||!output.node||
           !input.u64(count)||!count||count>output.schema_capabilities.size())return false;
        output.schema_count=static_cast<std::size_t>(count);
        for(std::size_t i{};i<output.schema_count;++i)if(!input.u64(output.schema_capabilities[i])||!output.schema_capabilities[i])return false;
        return read_profile(input, output.profile) && input.text(output.name) &&
               communication::absolute_name(output.name) && input.text(output.type) && !output.type.empty() && output.type.size() <= 255 &&
               input.text(output.role);
    }

    inline bool matches(profile_view const& input, communication::profile const& actual) noexcept
    {
        return input.identity == actual.identity && input.digest.size() == actual.digest.size() &&
               std::equal(input.digest.begin(), input.digest.end(), actual.digest.begin());
    }

    struct request
    {
        communication::request_key key;
        std::uint64_t resource{}, origin_capability{}, correlation{}, deadline{}, limit{};
        profile_view profile;
        endpoint_view endpoint;
        message_view message;
        std::string_view type;
        std::optional<std::uint64_t> cursor;
    };

    inline bool decode(operation action, wire::bytes bytes, request& output) noexcept
    {
        request value;
        wire::reader input{bytes};
        auto resource = [&] { return input.u64(value.resource) && value.resource; };
        auto key = [&] { return input.u64(value.key.session) && value.key.session && input.u64(value.key.sequence) && value.key.sequence; };
        bool valid{};
        switch(action)
        {
            case operation::context_open: valid = read_profile(input, value.profile); break;
            case operation::node_open:
                valid = input.u64(value.origin_capability) && value.origin_capability && resource() && read_profile(input, value.profile);
                break;
            case operation::context_shutdown:
            case operation::qos_liveliness_assert: valid = key() && resource(); break;
            case operation::topic_open:
            case operation::service_open:
                valid = read_endpoint(input, value.endpoint) && value.endpoint.schema_count==(action==operation::topic_open?1u:2u) &&
                        (action == operation::topic_open ? (value.endpoint.role == "publisher" || value.endpoint.role == "subscription")
                                                         : (value.endpoint.role == "client" || value.endpoint.role == "server"));
                break;
            case operation::topic_publish:
            case operation::service_submit:
                valid = key() && resource() && read_message(input, value.message) && input.u64(value.deadline) && value.deadline;
                break;
            case operation::service_respond:
                valid = key() && resource() && input.u64(value.correlation) && value.correlation && read_message(input, value.message) &&
                        input.u64(value.deadline) && value.deadline;
                break;
            case operation::qos_acknowledgements_wait: valid = resource() && input.u64(value.deadline) && value.deadline; break;
            case operation::service_abandon: valid = resource() && input.u64(value.correlation) && value.correlation; break;
            case operation::type_describe:
            {
                bool present{};
                std::uint64_t cursor{};
                valid = input.text(value.type) && !value.type.empty() && value.type.size() <= 255 && input.boolean(present) &&
                        (!present || (input.u64(cursor) && cursor)) && input.u64(value.limit) && value.limit;
                if(present) { value.cursor = cursor; }
                break;
            }
            case operation::context_describe:
            case operation::node_describe:
            case operation::topic_take:
            case operation::topic_describe:
            case operation::qos_describe:
            case operation::service_take:
            case operation::service_describe: valid = resource(); break;
            case operation::service_available: valid = resource() && input.u64(value.origin_capability); break;
            default: return false;
        }
        if(!valid || !input.done()) { return false; }
        output = value;
        return true;
    }

    inline bool encode_profile(wire::writer& output, communication::profile const& value) noexcept
    { return output.text(value.identity) && output.blob(value.digest); }

    inline bool encode_message(wire::writer& output, message_view const& value) noexcept
    { return output.text(value.identity) && output.text(value.encoding) && output.blob(value.body); }

    inline bool encode_receipt(wire::writer& output, communication::receipt const& value) noexcept
    {
        using effect = communication::effect;
        using error = communication::error;
        std::string_view observed = value.observed == effect::unknown            ? "unknown"
                                    : value.observed == effect::observed_remote  ? "observed_remote"
                                    : value.observed == effect::locally_accepted ? "locally_accepted"
                                                                                 : "none";
        std::string_view state = value.observed == effect::unknown                                        ? "outcome_unknown"
                                 : value.status != error::ok                                              ? "failed"
                                 : value.observed == effect::observed_remote                               ? "succeeded"
                                 : value.observed == effect::none                                         ? "failed"
                                                                                                          : "admitted";
        std::string_view reason = value.observed == effect::unknown ? "outcome.unknown"
                                  : value.status == error::ok       ? "none"
                                  : value.status == error::timeout  ? "deadline.elapsed"
                                                                    : "provider.error";
        return value.request && value.provider_generation && output.u64(value.request) && output.text(state) && output.text(observed) &&
               output.u64(value.provider_generation) && output.text(reason);
    }

    struct clock_epoch
    {
        std::uint64_t clock{}, epoch{};

        explicit operator bool() const noexcept { return clock != 0; }
    };

    struct stamp_mapping
    { clock_epoch source, received; };

    inline bool encode_stamp(wire::writer& output, clock_epoch origin, std::int64_t nanoseconds) noexcept
    { return origin && output.u64(origin.clock) && output.u64(origin.epoch) && output.i64(nanoseconds); }

    inline bool encode_metadata(wire::writer& output, communication::metadata const& value, stamp_mapping mapping = {}) noexcept
    {
        bool source = bool(value.source_nanoseconds) && bool(mapping.source), received = bool(value.received_nanoseconds) && bool(mapping.received);
        return value.source && value.generation && value.sequence && value.publisher_bytes <= value.publisher.size() && output.u64(value.source) &&
               output.u64(value.generation) && output.u64(value.sequence) && output.boolean(source) &&
               (!source || encode_stamp(output, mapping.source, *value.source_nanoseconds)) && output.boolean(received) &&
               (!received || encode_stamp(output, mapping.received, *value.received_nanoseconds)) && output.boolean(value.publisher_bytes != 0) &&
               (!value.publisher_bytes || output.blob(std::span{value.publisher}.first(value.publisher_bytes))) && output.u64(value.loss_before);
    }

    inline bool encode_delivery(wire::writer& output,
                                communication::metadata const& metadata,
                                std::uint64_t correlation,
                                std::string_view kind,
                                message_view const& message,
                                stamp_mapping stamps = {}) noexcept
    { return encode_metadata(output, metadata, stamps) && output.u64(correlation) && output.text(kind) && encode_message(output, message); }

    inline bool encode_context(wire::writer& output, communication::context_info const& value) noexcept
    { return encode_profile(output, value.selected) && output.text(value.active ? "active" : "inactive") && output.u64(value.generation); }

    inline bool encode_node(wire::writer& output, communication::node_info const& value) noexcept
    { return output.text(value.name) && output.text(value.name_space) && encode_profile(output, value.selected) && output.u64(value.generation); }

    inline bool encode_topic(wire::writer& output, communication::topic_info const& value) noexcept
    {
        auto const& native=value.native;
        return communication::valid(native)&&output.u64(value.node) && encode_profile(output, value.selected) && output.text(value.name) && output.text(value.type) &&
               output.text(value.direction == communication::role::publisher ? "publisher" : "subscription") &&
               output.boolean(native.provider_generation.has_value())&&(!native.provider_generation||output.u64(*native.provider_generation))&&
               output.boolean(native.size!=0)&&(!native.size||output.blob({native.native.data(),native.size}));
    }

    /** @brief Encode the immutable eleven-u64 effective QoS vocabulary.
     * @details history/reliability/durability/liveliness/depth are followed by
     * deadline, lifespan and liveliness-lease (seconds,nanoseconds) pairs.
     */
    inline bool qos_body(wire::writer& output, communication::qos const& value) noexcept
    {
        if(!communication::valid(value) || !output.u64(value.history) || !output.u64(value.reliability) || !output.u64(value.durability) ||
           !output.u64(value.liveliness) || !output.u64(value.depth))
        {
            return false;
        }
        for(auto duration: {value.deadline, value.lifespan, value.lease})
        {
            if(!output.u64(duration.seconds) || !output.u64(duration.nanoseconds)) { return false; }
        }
        return true;
    }

    inline bool encode_qos(wire::writer& output, communication::qos const& requested, std::optional<communication::qos> const& effective,
                           bool supported=true) noexcept
    {
        std::array<std::uint8_t, 88> body{};
        wire::writer profile{body};
        if(!qos_body(profile, requested) || !encode_message(output, {"uwvm/qos/Profile", "scene-canonical", profile.written()}) ||
           !output.boolean(effective.has_value()))
        {
            return false;
        }
        if(effective)
        {
            wire::writer actual{body};
            if(!qos_body(actual, *effective) || !encode_message(output, {"uwvm/qos/Profile", "scene-canonical", actual.written()})) { return false; }
        }
        if(!output.u64(supported?8:0)) { return false; }
        if(!supported)return true;
        for(auto name: {"history", "reliability", "durability", "liveliness", "depth", "deadline", "lifespan", "lease"})
        {
            if(!output.text(name)) { return false; }
        }
        return true;
    }

    /** @brief Encode independent cached native service legs and optional identity. */
    inline bool encode_service(wire::writer& output,communication::service_info const& value)noexcept
    {
        auto const& native=value.native;bool client=value.direction==communication::service_role::client;
        if(!communication::valid(native)||!output.u64(value.node)||!encode_profile(output,value.selected)||!output.text(value.name)||
            !output.text(value.type)||!output.text(client?"client":"server")||!output.boolean(native.provider_generation.has_value())||
            (native.provider_generation&&!output.u64(*native.provider_generation))||!output.boolean(native.size!=0)||
            (native.size&&!output.blob({native.native.data(),native.size}))||!output.u64(2))return false;
        return output.text(client?"request_transmit":"request_receive")&&encode_qos(output,value.requested,value.channels[0],value.channels[0].has_value())&&
            output.text(client?"response_receive":"response_transmit")&&encode_qos(output,value.requested,value.channels[1],value.channels[1].has_value());
    }
}  // namespace uwvm2::imported::ros::communication_contract
#endif
