/** @file
 * @brief Allocation-free result contracts for the five robot provider families.
 * @details Decode complete immutable replies before publishing any fields to a
 * caller. Worker replies are untrusted: valid syntax is not a trusted physical
 * observation, a capability, or permission to issue a follow-up operation. The
 * owning provider separately authenticates worker generation, matches request
 * correlation, rechecks authority, and validates its qualified state observer.
 *
 * All 22 operations have closed result shapes. Byte/element budgets are supplied
 * explicitly; repeated fields share one whole-reply element reservation. This
 * layer allocates nothing, performs no I/O and leaves output unchanged on error.
 * It neither registers imports nor invents native support for a designed route.
 */
#pragma once
#include "component_fault.h"
#include "robot_provider_contract.h"
#include <initializer_list>

namespace uwvm2::imported::ros::scene::robot_contract
{
    /** @brief An explicit clock incarnation; security deadlines remain host-steady values. */
    struct stamp {std::uint64_t clock{},epoch{};std::int64_t nanoseconds{};};
    /** @brief Recovery evidence; success or cancellation does not assert physical stopping. */
    struct receipt
    {std::uint64_t request{},provider_generation{};std::string_view state,effect,reason;};
    /** @brief Qualified observer claim, to be authenticated against the actual provider session. */
    struct execution_evidence
    {std::string_view state;std::uint64_t provider_generation{},controller_epoch{};stamp observed;repeated observations;};
    /** @brief Cached independent watchdog evidence; stop_reservation is not a usable handle. */
    struct watchdog_description
    {profile selected;std::string_view kind;std::uint64_t provider_generation{},maximum_silence_ns{},stop_reservation{};bool available{},armed{};};
    /** @brief Sanitized device state and finite admission bounds without native descriptors. */
    struct device_description
    {
        profile selected,command_schema,stop_profile;std::string_view state;std::uint64_t provider_generation{};
        watchdog_description watchdog;
        std::uint64_t command_bytes{},pending{},observation_count{},dependency_count{},maximum_age_ns{},maximum_deadline_ns{},retained_unknown{};
    };
    /** @brief Original pinned input phase of one controller epoch and write deadline. */
    struct cycle_sample
    {std::uint64_t cycle{},sequence{},controller_epoch{},deadline{},missed{};stamp observed;repeated observations,dependencies;};
    /** @brief Source, arrival and loss remain distinct; absent native metadata stays absent. */
    struct metadata
    {
        std::uint64_t source{},generation{},sequence{},loss_count{};stamp source_stamp,received_stamp;
        wire::bytes publisher_identity;bool has_source_stamp{},has_received_stamp{},has_publisher_identity{};
    };
    /** @brief Cached lifecycle and accounting; exit evidence is not physical-state evidence. */
    struct execution_description
    {
        profile selected;std::string_view state;std::uint64_t generation{},edges{},processes{},retained_bytes{},pending{},lost{},unknown{};
        receipt last;bool has_last{},exit_observed{};std::uint64_t first_failure{};
    };
    /** @brief Closed borrowed result union; only fields selected by action are populated. */
    struct response
    {
        operation action{};std::uint64_t handle{},epoch{},provider_generation{},expires{},loss_count{};
        receipt outcome;execution_evidence evidence;bool has_evidence{};
        device_description device;cycle_sample sample;metadata origin;frame media;reference buffer;
        execution_description execution;
    };
    /** @brief Match a closed, statically supplied selector list without allocation. */
    inline bool one_of(std::string_view value,std::initializer_list<std::string_view> choices) noexcept
    {for(auto choice:choices)if(value==choice)return true;return false;}
    /** @brief Reject contradictory progress/effect labels without conflating cancellation with rollback.
     * @details Terminal failed/cancelled may retain a known partial or committed
     * effect. Unknown/partial uncertainty uses outcome_unknown; it is never a
     * successful result and never permits automatic physical retry.
     */
    inline bool valid_receipt(std::string_view state,std::string_view effect) noexcept
    {
        if(state=="admitted")return one_of(effect,{"none","locally_accepted"});
        if(state=="queued"||state=="running")return effect=="locally_accepted";
        if(state=="succeeded")return one_of(effect,{"committed","observed_remote"});
        if(state=="failed"||state=="cancelled")return one_of(effect,{"none","locally_accepted","committed","observed_remote","partial"});
        return state=="outcome_unknown"&&one_of(effect,{"unknown","partial"});
    }
    /** @brief Extend the shared budgeted reader with closed provider result shapes. */
    class response_decoder:public decoder
    {
        limits bounds_;
    public:
        response_decoder(wire::bytes bytes,limits bounds) noexcept:decoder(bytes,bounds),bounds_(bounds){}
        /** @brief Require a nonzero clock and epoch; negative ROS times remain representable. */
        bool timestamp(stamp& value) noexcept
        {return identity(value.clock)&&identity(value.epoch)&&input().i64(value.nanoseconds);}
        /** @brief Read a bounded sanitized receipt and reject contradictory progress/effect states. */
        bool outcome(receipt& value) noexcept
        {
            if(!identity(value.request)||!text(value.state,32)||!text(value.effect,32)||!identity(value.provider_generation)||
                !input().text(value.reason)||value.reason.size()>255||value.reason.size()>bounds_.text_bytes||
                value.reason.find('\0')!=value.reason.npos)return false;
            return valid_receipt(value.state,value.effect);
        }
        /** @brief Preserve absence rather than fabricate a successful prior receipt. */
        bool optional_outcome(receipt& value,bool& present) noexcept
        {return input().boolean(present)&&(!present||outcome(value));}
        /** @brief Decode a claimed observation; actual provenance authentication remains mandatory. */
        bool evidence(execution_evidence& value) noexcept
        {
            return text(value.state,32)&&one_of(value.state,{"execution_observed","partial_execution_observed","stopped_observed","outcome_unresolved"})&&
                identity(value.provider_generation)&&identity(value.controller_epoch)&&timestamp(value.observed)&&observations(value.observations)&&
                (value.state=="outcome_unresolved"||value.observations.count);
        }
        /** @brief Enforce armed-implies-available and a finite positive steady silence bound. */
        bool watchdog(watchdog_description& value) noexcept
        {
            if(!selected(value.selected)||!text(value.kind,32)||!one_of(value.kind,{"independent_controller","hardware_interlock","verified_receiver_lease"})||
                !identity(value.provider_generation)||!input().boolean(value.available)||!input().boolean(value.armed)||
                !deadline(value.maximum_silence_ns)||!identity(value.stop_reservation))return false;
            return !value.armed||value.available;
        }
        /** @brief Validate cached device support without performing a native probe or granting access. */
        bool device(device_description& value) noexcept
        {
            if(!selected(value.selected)||!selected(value.command_schema)||!selected(value.stop_profile)||!text(value.state,32)||
                !one_of(value.state,{"prepared","ready","stopping","stopped","faulted","retired","outcome_unknown"})||
                !identity(value.provider_generation)||!watchdog(value.watchdog)||!identity(value.command_bytes)||!identity(value.pending)||
                !number(value.observation_count)||!number(value.dependency_count)||!deadline(value.maximum_age_ns)||
                !deadline(value.maximum_deadline_ns)||!number(value.retained_unknown))return false;
            // These describe the admitted reservation, not unbounded sentinels.
            return value.command_bytes!=UINT64_MAX&&value.pending!=UINT64_MAX&&value.observation_count!=UINT64_MAX&&
                value.dependency_count!=UINT64_MAX&&value.retained_unknown<=value.pending&&
                (value.state!="ready"||(value.watchdog.available&&value.watchdog.armed));
        }
        /** @brief Decode original input/version references under the shared whole-reply element budget. */
        bool sample(cycle_sample& value) noexcept
        {return identity(value.cycle)&&identity(value.sequence)&&identity(value.controller_epoch)&&timestamp(value.observed)&&
            deadline(value.deadline)&&observations(value.observations)&&number(value.missed)&&dependencies(value.dependencies);}
        /** @brief Keep source/arrival clocks, publisher identity and loss separately optional/bounded. */
        bool origin(metadata& value) noexcept
        {
            if(!identity(value.source)||!identity(value.generation)||!identity(value.sequence)||!input().boolean(value.has_source_stamp)||
                (value.has_source_stamp&&!timestamp(value.source_stamp))||!input().boolean(value.has_received_stamp)||
                (value.has_received_stamp&&!timestamp(value.received_stamp))||!input().boolean(value.has_publisher_identity))return false;
            if(value.has_publisher_identity&&(!input().blob(value.publisher_identity)||value.publisher_identity.empty()||
                value.publisher_identity.size()>bounds_.blob_bytes))return false;
            return number(value.loss_count);
        }
        /** @brief Reject contradictory process-exit or pending/unknown accounting claims. */
        bool execution(execution_description& value,bool component) noexcept
        {
            if(!selected(value.selected)||!text(value.state,32)||!identity(value.generation))return false;
            if(component)
            {
                if(!one_of(value.state,{"prepared","starting","ready","stopping","exited","failed","retired","outcome_unknown"})||
                    !number(value.processes)||!number(value.retained_bytes)||!number(value.pending)||!number(value.unknown)||!input().boolean(value.exit_observed)||
                    !input().u64(value.first_failure)||!components::valid_fault(value.first_failure))return false;
                if(value.processes==UINT64_MAX||value.retained_bytes==UINT64_MAX||
                    (value.exit_observed&&value.processes)||(value.state=="exited"&&!value.exit_observed)||
                    (value.state=="failed"&&!value.first_failure)||(value.first_failure&&value.state!="failed"&&value.state!="retired"&&value.state!="outcome_unknown"))return false;
            }
            else if(!one_of(value.state,{"prepared","starting","running","draining","stopped","failed","retired","outcome_unknown"})||
                !identity(value.edges)||value.edges==UINT64_MAX||!number(value.pending)||!number(value.lost)||!number(value.unknown))return false;
            if(!optional_outcome(value.last,value.has_last))return false;
            return value.pending!=UINT64_MAX&&value.unknown<=value.pending;
        }
    };
    /** @brief Decode one whole result with no allocation and atomic publication.
     * @details This checks representation, not provider honesty. Receipt request,
     * returned resource origin and exact profile must still match the host's
     * retained request. Terminal physical evidence needs authenticated observation
     * and its own current authority; a valid reply alone can never rearm a latch.
     */
    inline bool decode_response(operation action,wire::bytes bytes,limits bounds,response& output) noexcept
    {
        if(static_cast<std::size_t>(action)>=operations.size()||bytes.empty()||!bounds.request_bytes||bytes.size()>bounds.request_bytes)return false;
        response_decoder in{bytes,bounds};response value;value.action=action;
        switch(action)
        {
        case operation::device_open:case operation::media_open:case operation::bridge_create:case operation::component_open:
            if(!in.identity(value.handle))return false;
            break;
        case operation::device_submit:case operation::device_stop:case operation::control_submit:case operation::control_result:
        case operation::control_stop:case operation::media_submit:case operation::media_close:case operation::bridge_start:
        case operation::bridge_stop:case operation::component_start:case operation::component_stop:
            if(!in.outcome(value.outcome))return false;
            break;
        case operation::device_result:
            if(!in.outcome(value.outcome)||!in.input().boolean(value.has_evidence)||
                (value.has_evidence&&(!in.evidence(value.evidence)||value.evidence.provider_generation!=value.outcome.provider_generation)))return false;
            break;
        case operation::device_describe:
            if(!in.device(value.device))return false;
            break;
        case operation::control_open:
            if(!in.identity(value.handle)||!in.identity(value.epoch)||!in.identity(value.provider_generation)||!in.deadline(value.expires))return false;
            break;
        case operation::control_sample:
            if(!in.sample(value.sample))return false;
            break;
        case operation::media_take:
            if(!in.outcome(value.outcome)||value.outcome.state!="succeeded"||!in.origin(value.origin)||!in.media(value.media)||
                !in.resource(value.buffer)||!in.number(value.loss_count))return false;
            break;
        case operation::bridge_describe:case operation::component_describe:
            if(!in.execution(value.execution,action==operation::component_describe))return false;
            break;
        }
        if(!in.done())return false;
        output=value;return true;
    }
}
