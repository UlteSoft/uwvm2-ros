/** @file
 * @brief Explicit asynchronous worker creation receipts, never overloaded handles.
 * @details node.open/topic.open/service.open/action.open append a nonzero
 * monotonically increasing creation key and finite absolute steady deadline to
 * their existing semantic body. The request receipt contains that key, a distinct
 * creation-request identity, its state and observed effect. A creation request is
 * never usable as a node/topic/service/action resource. context.open may remain
 * synchronous because it opens only a local logical context.
 *
 * communication.open.take and communication.open.cancel require their own exact
 * action grant on the original node/topic/service/action purpose. They take one
 * creation-request identity. request.describe/release use the same identity and
 * are independently admitted on that purpose. Take returns an explicit endpoint
 * kind and published resource handle; lost output can be copied again from the
 * same retained receipt. Release cannot discard pending work, an active endpoint,
 * an unknown effect or unconfirmed native cleanup debt. Cancellation cleanup is
 * independent of the original creation-rate budget and never refunds that charge.
 */
#pragma once
#include <array>
#include <string_view>
#define UWVM_ROS_COMMUNICATION_OPEN_FUNCTIONS(X) \
    X(communication_open_take, take, "communication.open.take") \
    X(communication_open_cancel, cancel, "communication.open.cancel")
#ifndef UWVM_SCENE_COMMUNICATION_OPEN_CONTRACT_MACROS_ONLY
#include "communication.h"
#include "wire.h"
namespace uwvm2::imported::ros::communication_open_contract
{
    namespace wire=scene::wire;
    enum class operation:std::uint8_t{take,cancel};
    struct descriptor{operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_COMMUNICATION_OPEN_DESCRIPTOR(symbol,action,name) descriptor{operation::action,name},
        UWVM_ROS_COMMUNICATION_OPEN_FUNCTIONS(UWVM_COMMUNICATION_OPEN_DESCRIPTOR)
#undef UWVM_COMMUNICATION_OPEN_DESCRIPTOR
    };
    enum class endpoint_kind:std::uint64_t{node=1,topic,service,action};
    enum class state:std::uint64_t{reserved=1,queued,sending,waiting,ready,published,cancelled,failed,released};
    struct request{std::uint64_t request_id{};};
    struct creation{std::uint64_t key{},deadline{};};
    struct receipt
    {
        std::uint64_t key{},request_id{};
        state status{state::reserved};scene::communication::error result{scene::communication::error::again};
        scene::communication::effect observed{scene::communication::effect::none};
        bool cleanup_debt{};
    };
    struct published{receipt creation;endpoint_kind kind{};std::uint64_t handle{};};
    inline constexpr std::size_t receipt_bytes=48,published_bytes=64;
    inline bool valid(endpoint_kind kind)noexcept{return kind>=endpoint_kind::node&&kind<=endpoint_kind::action;}
    /** @brief Decode the mandatory trailer after the family-specific open body. */
    inline bool read_creation(wire::reader& input,creation& output)noexcept
    {creation value;if(!input.u64(value.key)||!value.key||!input.u64(value.deadline)||!value.deadline||value.deadline>INT64_MAX)return false;output=value;return true;}
    /** @brief Split the required trailer before invoking the existing semantic decoder.
     * @details This accepts no legacy carrier: the complete owned open input
     * must contain a nonzero key/deadline trailer, and its remaining semantic
     * body must separately pass the selected node/topic/service/action decoder.
     * The full original carrier, including this trailer, is the retry identity.
     */
    inline bool split_creation(wire::bytes input,wire::bytes& semantic,creation& output)noexcept
    {if(input.size()<16)return false;wire::reader suffix{input.last(16)};creation value;if(!read_creation(suffix,value)||!suffix.done())return false;semantic=input.first(input.size()-16);output=value;return true;}
    inline bool decode(operation action,wire::bytes body,request& output)noexcept
    {if(action!=operation::take&&action!=operation::cancel)return false;wire::reader input{body};request value;if(!input.u64(value.request_id)||!value.request_id||!input.done())return false;output=value;return true;}
    inline bool encode(wire::writer& out,receipt const& input)noexcept
    {return input.key&&input.request_id&&input.status>=state::reserved&&input.status<=state::released&&
        input.observed<=scene::communication::effect::unknown&&out.u64(input.key)&&out.u64(input.request_id)&&out.u64(static_cast<std::uint64_t>(input.status))&&
        out.u64(static_cast<std::uint64_t>(input.result))&&out.u64(static_cast<std::uint64_t>(input.observed))&&out.boolean(input.cleanup_debt);}
    inline bool encode(wire::writer& out,published const& input)noexcept
    {return valid(input.kind)&&input.handle&&input.creation.status==state::published&&input.creation.result==scene::communication::error::ok&&
        !input.creation.cleanup_debt&&input.creation.observed!=scene::communication::effect::unknown&&encode(out,input.creation)&&out.u64(static_cast<std::uint64_t>(input.kind))&&out.u64(input.handle);}
    /** @brief Preserve the shared request.describe canonical receipt shape.
     * @details provider_generation is the actual sealed worker incarnation,
     * never a guest key, capability generation, synthetic zero or request ID.
     * In-flight unknown effects remain running; terminal unknown stays explicitly
     * outcome_unknown. Cleanup confirmation alone cannot erase unknown outcome.
     */
    inline bool encode_description(wire::writer& out,receipt const& input,std::uint64_t provider_generation)noexcept
    {
        using error=scene::communication::error;using effect=scene::communication::effect;
        if(!input.request_id||!provider_generation||input.status<state::reserved||input.status>state::released)return false;
        bool running=input.status==state::sending||input.status==state::waiting;
        bool queued=input.status==state::reserved||input.status==state::queued;
        auto phase=queued?"queued":running?"running":input.observed==effect::unknown?"outcome_unknown":
            input.status==state::ready?"admitted":input.result==error::ok?"succeeded":input.result==error::cancelled?"cancelled":"failed";
        auto observed=input.observed==effect::unknown?"unknown":input.observed==effect::observed_remote?"observed_remote":
            input.observed==effect::locally_accepted?"locally_accepted":"none";
        auto reason=input.observed==effect::unknown&&!running?"outcome.unknown":input.cleanup_debt?"native.cleanup.pending":
            input.result==error::timeout?"deadline.elapsed":input.result==error::cancelled?"request.cancelled":
            input.result==error::ok||queued||running?"none":"provider.error";
        return out.u64(input.request_id)&&out.text(phase)&&out.text(observed)&&out.u64(provider_generation)&&out.text(reason);
    }
    inline bool encode_release(wire::writer& out,bool released)noexcept{return out.boolean(released);}
}
#endif
