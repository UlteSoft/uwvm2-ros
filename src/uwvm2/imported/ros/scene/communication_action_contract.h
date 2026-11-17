/** @file
 * @brief Canonical owned action records, with distinct goal/request identities.
 * @details Outer delivery correlation is the request token; it is not the owned
 * goal handle. uwvm/action/Event contains goal handle, UUID, closed state and an
 * optional complete typed payload. UUIDs never confer authority. Failed complete
 * output encoding must fail the transactional sink so no event is consumed.
 * Mutation decoders do not grant action.respond or its tag-specific secondary
 * action. The broker verifies both and maps cancelled_goals only after ownership
 * and the configured cancel-set scope have been checked.
 */
#pragma once
#include <array>
#include <string_view>
#define UWVM_ROS_ACTION_FUNCTIONS(X)                                                                                                                           \
    X(action_open, open, "action.open")                                                                                                                        \
    X(action_submit, submit, "action.submit")                                                                                                                  \
    X(action_take, take, "action.take")                                                                                                                        \
    X(action_respond, respond, "action.respond")                                                                                                               \
    X(action_cancel, cancel, "action.cancel")                                                                                                                  \
    X(action_describe, describe, "action.describe")                                                                                                             \
    X(action_cancel_set, cancel_set, "action.cancel.set")
#ifndef UWVM_SCENE_ACTION_CONTRACT_MACROS_ONLY
# include "communication_contract.h"
# include "communication_actions.h"

namespace uwvm2::imported::ros::action_contract
{
    namespace communication = scene::communication;
    namespace actions = communication::actions;
    namespace wire = scene::wire;
    namespace common = communication_contract;
    enum class operation : std::uint8_t
    {
        open,
        submit,
        take,
        respond,
        cancel,
        describe,
        cancel_set
    };

    struct descriptor
    {
        operation id;
        std::string_view name;
    };

    inline constexpr std::array operations{
# define UWVM_ACTION_DESCRIPTOR(symbol, action, name) descriptor{operation::action, name},
        UWVM_ROS_ACTION_FUNCTIONS(UWVM_ACTION_DESCRIPTOR)
# undef UWVM_ACTION_DESCRIPTOR
    };
    inline constexpr std::array<std::string_view, 8>
        transition_names{"accept", "reject", "execute", "feedback", "succeed", "abort", "cancelled", "cancel_response"};
    inline constexpr std::array<std::string_view, 8> transition_actions{"action.accept",
                                                                        "action.reject",
                                                                        "action.execute",
                                                                        "action.feedback",
                                                                        "action.succeed",
                                                                        "action.abort",
                                                                        "action.finish.cancelled",
                                                                        "action.cancel.respond"};
    inline constexpr std::array<std::string_view, 7> event_names{"action.goal.proposal",
                                                                 "action.goal.acceptance",
                                                                 "action.goal.status",
                                                                 "action.feedback",
                                                                 "action.result",
                                                                 "action.cancel.request",
                                                                 "action.cancel.response"};

    struct request
    {
        common::endpoint_view endpoint;
        common::message_view message;
        communication::request_key key;
        std::uint64_t resource{}, goal{}, correlation{}, deadline{};
        wire::bytes uuid;
        std::array<std::uint64_t, 64> cancelled_goals{};
        std::size_t cancelled_count{};
        actions::transition transition{};
        bool has_message{}, has_goal{};
    };

    inline bool decode(operation action, wire::bytes bytes, request& output) noexcept
    {
        wire::reader input{bytes};
        request value;
        bool valid{};
        auto resource = [&] { return input.u64(value.resource) && value.resource; };
        auto key = [&] { return input.u64(value.key.session) && value.key.session && input.u64(value.key.sequence) && value.key.sequence; };
        switch(action)
        {
            case operation::open:
                valid = common::read_endpoint(input, value.endpoint) && value.endpoint.schema_count==3 &&
                        (value.endpoint.role == "client" || value.endpoint.role == "server");
                break;
            case operation::submit:
                valid = key() && resource() && input.blob(value.uuid) && value.uuid.size() == 16 && common::read_message(input, value.message) &&
                        input.u64(value.deadline) && value.deadline;
                break;
            case operation::take: valid = resource(); break;
            case operation::cancel: valid = key() && resource() && input.u64(value.goal) && value.goal && input.u64(value.deadline) && value.deadline; break;
            case operation::cancel_set:
            {
                std::uint64_t count{};valid=key()&&resource()&&input.u64(count)&&count&&count<=value.cancelled_goals.size();
                if(!valid)return false;
                value.cancelled_count=static_cast<std::size_t>(count);
                for(std::size_t i{};i<value.cancelled_count;++i)
                {
                    if(!input.u64(value.cancelled_goals[i])||!value.cancelled_goals[i])return false;
                    for(std::size_t n{};n<i;++n)if(value.cancelled_goals[n]==value.cancelled_goals[i])return false;
                }
                valid=input.u64(value.deadline)&&value.deadline;break;
            }
            case operation::describe: valid = resource() && input.boolean(value.has_goal) && (!value.has_goal || (input.u64(value.goal) && value.goal)); break;
            case operation::respond:
            {
                std::string_view name;
                std::uint64_t count{};
                valid = key() && resource() && input.u64(value.correlation) && value.correlation && input.u64(value.goal) && input.text(name) &&
                        input.boolean(value.has_message) && (!value.has_message || common::read_message(input, value.message)) && input.u64(count) &&
                        count <= value.cancelled_goals.size();
                if(!valid) { return false; }
                for(std::size_t i{}; i < transition_names.size(); ++i)
                {
                    if(name == transition_names[i])
                    {
                        value.transition = static_cast<actions::transition>(i + 1);
                        break;
                    }
                }
                if(!static_cast<std::uint64_t>(value.transition)) { return false; }
                value.cancelled_count = static_cast<std::size_t>(count);
                for(std::size_t i{}; i < value.cancelled_count; ++i)
                {
                    if(!input.u64(value.cancelled_goals[i]) || !value.cancelled_goals[i]) { return false; }
                    for(std::size_t j{}; j < i; ++j)
                    {
                        if(value.cancelled_goals[i] == value.cancelled_goals[j]) { return false; }
                    }
                }
                valid = input.u64(value.deadline) && value.deadline;
                break;
            }
            default: return false;
        }
        if(!valid || !input.done()) { return false; }
        output = value;
        return true;
    }

    /** @brief Translate the currently qualified exact-goal response scope.
     * @details Broad sets remain rejected until a separately configured provider
     * implements their bounded candidate/selection semantics. Empty selection is
     * a real rejection; one matching owned goal is acceptance. No other tag may
     * smuggle a cancel selection or an unrelated message schema.
     */
    inline bool exact_response(request const& input, actions::plan const& plan, actions::response& output) noexcept
    {
        if(!input.goal) { return false; }
        auto tag = input.transition;
        if(tag == actions::transition::cancel_response)
        {
            if(input.has_message || input.cancelled_count > 1 || (input.cancelled_count == 1 && input.cancelled_goals[0] != input.goal)) { return false; }
        }
        else
        {
            if(input.cancelled_count) { return false; }
            auto schema = tag == actions::transition::feedback                                                                                  ? &plan.feedback
                          : (tag == actions::transition::succeed || tag == actions::transition::abort || tag == actions::transition::cancelled) ? &plan.result
                                                                                                                                                : nullptr;
            if(schema)
            {
                if(!input.has_message || input.message.identity != schema->type_identity || input.message.encoding != schema->encoding) { return false; }
            }
            else if(input.has_message) { return false; }
        }
        output = {input.correlation, input.goal, tag, input.has_message ? input.message.body : wire::bytes{}, input.cancelled_count == 1};
        return true;
    }

    /** @brief Complete per-candidate cancellation evidence; zero request means no native effect. */
    inline bool encode_selection(wire::writer& output,actions::cancellation_selection const& value)noexcept
    {
        if(!value.count||value.count>actions::maximum_cancellation_selection||!common::encode_receipt(output,value.batch)||!output.u64(value.count))return false;
        for(std::size_t i{};i<value.count;++i)
        {
            auto const& item=value.items[i];auto const& receipt=item.result;
            if(!item.goal||!communication::known_status(receipt.status)||receipt.observed>communication::effect::unknown||
                !output.u64(item.goal)||!output.u64(receipt.request)||!output.u64(receipt.provider_generation)||
                !output.u64(static_cast<std::uint64_t>(receipt.status))||!output.u64(static_cast<std::uint64_t>(receipt.observed)))return false;
        }
        return true;
    }

    inline bool event_body(wire::writer& output, actions::delivery const& record, actions::plan const& plan) noexcept
    {
        common::message_view payload;
        bool present{};
        auto schema = record.kind == actions::event_kind::proposal   ? &plan.goal
                      : record.kind == actions::event_kind::feedback ? &plan.feedback
                      : record.kind == actions::event_kind::result   ? &plan.result
                                                                     : nullptr;
        if(schema)
        {
            payload = {schema->type_identity, schema->encoding, record.body};
            present = true;
        }
        else if(record.kind == actions::event_kind::cancel_response)
        {
            payload = {"uwvm/action/CancelResponse", "scene-canonical", record.body};
            present = true;
        }
        else if(!record.body.empty()) { return false; }
        return record.goal && output.u64(record.goal) && output.blob(record.goal_uuid) && output.u64(static_cast<std::uint64_t>(record.observed)) &&
               output.boolean(present) && (!present || common::encode_message(output, payload));
    }

    inline bool encode_delivery(wire::writer& output, actions::delivery const& record, actions::plan const& plan) noexcept
    {
        auto kind = static_cast<std::uint64_t>(record.kind);
        if(kind < 1 || kind > event_names.size()) { return false; }
        auto body = wire::writer::measuring(output.remaining());
        if(!event_body(body, record, plan)) { return false; }
        return common::encode_metadata(output, record.information) && output.u64(record.request) && output.text(event_names[kind - 1]) &&
               output.text("uwvm/action/Event") && output.text("scene-canonical") && output.u64(body.size()) && event_body(output, record, plan);
    }

    /** @brief Maximum complete event size for output preflight before isolated dequeue.
     * @details Measure real canonical prefixes with empty borrowed bodies, then
     * add each configured payload bound with checked arithmetic. No payload or
     * temporary allocation is required. Zero denotes an invalid/overflowing plan.
     */
    inline std::size_t maximum_delivery_bytes(actions::plan const& plan)noexcept
    {
        std::size_t maximum{};
        for(std::uint64_t kind{1};kind<=event_names.size();++kind)
        {
            actions::delivery record;record.information.source=record.information.generation=record.information.sequence=1;
            record.goal=record.request=1;record.kind=static_cast<actions::event_kind>(kind);
            auto measure=wire::writer::measuring(SIZE_MAX);
            if(!encode_delivery(measure,record,plan))return 0;
            auto payload=record.kind==actions::event_kind::proposal?plan.max_goal_bytes:record.kind==actions::event_kind::feedback?plan.max_feedback_bytes:
                record.kind==actions::event_kind::result?plan.max_result_bytes:record.kind==actions::event_kind::cancel_response?std::size_t{8}:0;
            if(payload>SIZE_MAX-measure.size())return 0;
            maximum=std::max(maximum,measure.size()+payload);
        }
        return maximum;
    }

    inline bool state_body(wire::writer& output, actions::endpoint_info const& endpoint, std::optional<actions::goal_info> const& goal) noexcept
    {
        if(!common::encode_profile(output, endpoint.selected) || !output.text(endpoint.name) || !output.text(endpoint.type) ||
           !output.text(endpoint.direction == actions::role::client ? "client" : "server") || !output.u64(endpoint.max_goals) ||
           !output.u64(endpoint.max_cancels_per_goal) || !output.u64(endpoint.retention_ns) || !output.boolean(endpoint.uncertain) ||
           !output.boolean(goal.has_value()))
        {
            return false;
        }
        if(!goal) { return true; }
        return output.u64(goal->goal) && output.blob(goal->goal_uuid) && output.u64(static_cast<std::uint64_t>(goal->observed)) &&
               output.u64(goal->proposal_request) && output.u64(goal->cancel_request) && output.boolean(goal->delivered) &&
               output.boolean(goal->cancel_delivered) && output.boolean(goal->uncertain) && output.u64(goal->retention_until);
    }

    inline bool encode_state(wire::writer& output, actions::endpoint_info const& endpoint, std::optional<actions::goal_info> const& goal) noexcept
    {
        auto body = wire::writer::measuring(output.remaining());
        if(!state_body(body, endpoint, goal)) { return false; }
        return output.text("uwvm/action/State") && output.text("scene-canonical") && output.u64(body.size()) && state_body(output, endpoint, goal);
    }
}  // namespace uwvm2::imported::ros::action_contract
#endif
