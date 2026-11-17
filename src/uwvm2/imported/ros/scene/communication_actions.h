/** @file
 * @brief Bounded owned action protocol with deferred server decisions and recovery.
 * @details An action UUID identifies a protocol goal, never authority. The broker
 * supplies an exact endpoint owner and holds the primary and transition-specific
 * authority leases through mutation. Trusted deployment fixes the action role,
 * three schemas, node attachment, transition mask, retention and independent
 * critical/feedback storage. No endpoint is implicitly accepted or executed.
 *
 * Native adapters use the standard ROS action services/topics. Their callbacks
 * enter only the trusted ingress methods below and must not invoke ingress
 * synchronously from a native mutation call. Native calls retain no borrowed
 * request bytes. Native exceptions after possible effects become latched unknown
 * outcomes: close/reopen must not erase them. A cancel is a proposal, not a stop.
 * This live-process ledger does not establish durable or physical completion.
 */
#pragma once
#include "communication.h"
#include "wire.h"

namespace uwvm2::imported::ros::scene::communication::actions
{
    using uuid = std::array<std::uint8_t, 16>;
    enum class role : std::uint8_t
    {
        client,
        server
    };
    enum class state : std::uint64_t
    {
        pending,
        accepted,
        executing,
        cancelling,
        succeeded,
        cancelled,
        aborted,
        rejected
    };
    enum class transition : std::uint64_t
    {
        accept = 1,
        reject,
        execute,
        feedback,
        succeed,
        abort,
        cancelled,
        cancel_response
    };
    enum class event_kind : std::uint64_t
    {
        proposal = 1,
        acceptance,
        status,
        feedback,
        result,
        cancel_request,
        cancel_response
    };
    /** @brief Ownership of an ingress record when local delivery is full.
     * @details A retained record stays in an admitted bounded provider slot and
     * is retried exactly; callback input may disappear on return and therefore
     * preserves the existing loss/unknown reporting on failed enqueue.
     */
    enum class ingress_ownership:std::uint8_t{callback,retained};

    inline bool terminal(state s) noexcept { return s == state::succeeded || s == state::cancelled || s == state::aborted || s == state::rejected; }

    struct plan
    {
        owner authority;
        profile selected;
        ::uwvm2::utils::container::std_string resolved_name, action_type;
        role direction{};
        qos goal_quality, result_quality, cancel_quality, feedback_quality, status_quality;
        type_plan goal, feedback, result;
        std::size_t max_goals{}, max_cancels_per_goal{}, feedback_slots{}, max_goal_bytes{}, max_feedback_bytes{}, max_result_bytes{}, reserved_bytes{};
        std::uint64_t transition_mask{}, max_deadline_ns{}, retention_ns{};
    };

    struct response
    {
        handle request{}, goal{};
        transition change{};
        std::span<std::uint8_t const> body;
        bool accept_cancel{};
    };

    struct delivery
    {
        metadata information;
        handle goal{}, request{};
        event_kind kind{};
        state observed{};
        std::span<std::uint8_t const> body;
        uuid goal_uuid{};
    };

    using sink = bool (*)(void*, delivery const&);

    struct native
    {
        virtual ~native() = default;
        /** @brief Trusted immutable reservation-first mutation accounting mode. */
        virtual bool queued_mutations()const noexcept{return false;}
        virtual bool queued_receives()const noexcept{return false;}
        /** @brief Actual delivery incarnation, independent of the host grant generation.
         * @details An isolated adapter returns its verified worker generation.
         * Local callback providers return zero to select the configured local
         * incarnation. The value is frozen before any ingress becomes visible.
         */
        virtual handle delivery_generation()const noexcept{return 0;}
        virtual bool live() const noexcept = 0;
        virtual native_result submit(handle, uuid const&, std::span<std::uint8_t const>, std::uint64_t) noexcept = 0;
        virtual native_result respond(response const&, uuid const&, state, std::uint64_t) noexcept = 0;
        virtual native_result cancel(handle, handle, uuid const&, std::uint64_t) noexcept = 0;
        virtual native_result submit(handle goal,uuid const& id,std::span<std::uint8_t const> body,std::uint64_t deadline,operation_token const&) noexcept
        {return submit(goal,id,body,deadline);}
        virtual native_result respond(response const& value,uuid const& id,state next,std::uint64_t deadline,operation_token const&) noexcept
        {return respond(value,id,next,deadline);}
        virtual native_result cancel(handle goal,handle request,uuid const& id,std::uint64_t deadline,operation_token const&) noexcept
        {return cancel(goal,request,id,deadline);}
        virtual error poll(operation_token const&) noexcept {return error::ok;}
        /// @brief Forget an expired host goal only after the core has drained its events.
        virtual error release_goal(handle)noexcept{return error::ok;}
        virtual bool requires_close_receipt()const noexcept{return false;}
        virtual error close(operation_token const&)noexcept{return error::ok;}
        virtual bool cleanup_confirmed()const noexcept{return true;}
        virtual bool occupied() const noexcept {return false;}
        virtual void retire() noexcept = 0;
    };

    struct goal_info
    {
        handle goal{}, proposal_request{}, cancel_request{};
        state observed{};
        bool delivered{}, cancel_delivered{}, uncertain{};
        std::uint64_t retention_until{};
        uuid goal_uuid{};
    };

    struct endpoint_info
    {
        profile selected;
        ::uwvm2::utils::container::std_string name, type;
        role direction{};
        std::uint64_t max_goals{}, max_cancels_per_goal{}, retention_ns{};
        bool uncertain{};
    };

    /** @brief One explicitly owned cancellation candidate and its individual admission receipt.
     * @details A zero request means this candidate had no effect. An accepted
     * native request remains a proposal; action.take reports the actual server
     * decision and terminal goal state separately.
     */
    struct cancellation_item {handle goal{};receipt result;};
    inline constexpr std::size_t maximum_cancellation_selection=64;
    struct cancellation_selection
    {receipt batch;std::array<cancellation_item,maximum_cancellation_selection> items{};std::size_t count{};};

    class endpoint
    {
        struct goal_slot
        {
            struct cancel_slot
            {
                handle request{},provider_generation{};
                bool delivered{}, responded{}, pending{};
            };

            goal_info info;
            uuid identity{};
            ::uwvm2::utils::container::std_vector<std::uint8_t> goal;
            ::uwvm2::utils::container::std_vector<cancel_slot> cancels;
            std::size_t goal_size{}, queued{};
            handle pending_submit{},pending_response{},pending_correlation{},submit_generation{},response_generation{};
            state pending_state{};
            transition pending_transition{};
        };

        std::mutex mutex;
        plan configuration;
        std::shared_ptr<native> provider;
        std::shared_ptr<receive_queue> queue;
        ::uwvm2::utils::container::std_vector<goal_slot> goals;
        ::uwvm2::utils::container::std_vector<std::uint8_t> scratch, previous_body;
        handle identity{}, previous_goal{}, previous_request{}, delivery_incarnation{};
        std::size_t feedback_queued{}, previous_size{};
        std::uint64_t feedback_lost{},provider_loss{};
        request_key previous_key;
        uuid previous_uuid{};
        receipt previous_receipt;
        cancellation_selection previous_selection;
        std::uint64_t previous_deadline{}, previous_tag{};
        bool configured{}, retired{}, uncertainty{}, previous_accept{}, has_receipt{},queue_charged{},queue_received{},closing_call{};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process == ::getpid();
#else
            return true;
#endif
        }

        bool live() const noexcept { return configured && !retired && !closing_call && provider && provider->live(); }

        goal_slot* find(handle id) noexcept
        {
            for(auto& slot: goals)
            {
                if(slot.info.goal == id && id) { return &slot; }
            }
            return nullptr;
        }

        goal_slot* find(uuid const& id) noexcept
        {
            for(auto& slot: goals)
            {
                if(slot.info.goal && slot.identity == id) { return &slot; }
            }
            return nullptr;
        }

        goal_slot* free() noexcept
        {
            auto time = now();
            for(auto& slot: goals)
            {
                if(!slot.info.goal || (terminal(slot.info.observed) && !slot.info.uncertain && !slot.queued && !slot.pending_submit && !slot.pending_response &&
                                       std::none_of(slot.cancels.begin(),slot.cancels.end(),[](auto const& value){return value.pending;}) && time >= slot.info.retention_until &&
                                       (!has_receipt || slot.info.goal != previous_goal)))
                {
                    if(slot.info.goal&&provider->release_goal(slot.info.goal)!=error::ok)continue;
                    return &slot;
                }
            }
            return nullptr;
        }

        handle allocate() noexcept { return resources::allocate_identity(); }

        void retain(goal_slot& slot) noexcept
        {
            auto time = now();
            slot.info.retention_until = configuration.retention_ns > UINT64_MAX - time ? UINT64_MAX : time + configuration.retention_ns;
        }

        error emit(goal_slot& slot, event_kind kind, handle request, std::span<std::uint8_t const> body,
                   std::optional<state> observed={},ingress_ownership ownership=ingress_ownership::callback,
                   std::optional<std::uint64_t> native_loss={}) noexcept
        {
            if(kind == event_kind::feedback && feedback_queued == configuration.feedback_slots)
            {
                if(ownership==ingress_ownership::retained)return error::again;
                if(feedback_lost == UINT64_MAX)
                {
                    uncertainty = slot.info.uncertain = true;
                    return error::too_large;
                }
                ++feedback_lost;
                return error::again;
            }
            auto lost=native_loss.value_or(provider_loss);
            if(lost<provider_loss||feedback_lost>UINT64_MAX-lost)return error::invalid;
            wire::writer fields{scratch};
            if(!fields.u64(static_cast<std::uint64_t>(kind)) || !fields.u64(slot.info.goal) || !fields.u64(request) ||
               !fields.u64(static_cast<std::uint64_t>(observed.value_or(slot.info.observed))) || !fields.u64(feedback_lost+lost) || !fields.blob(body))
            {
                return error::too_large;
            }
            metadata information;
            information.source = identity;
            information.generation = delivery_incarnation;
            auto status = queue->push(information, fields.written(),ownership==ingress_ownership::retained);
            if(status != error::ok)
            {
                if(kind != event_kind::feedback&&ownership!=ingress_ownership::retained) { uncertainty = slot.info.uncertain = true; }
                return status;
            }
            ++slot.queued;
            provider_loss=lost;
            if(kind == event_kind::feedback) { ++feedback_queued; }
            return error::ok;
        }

        error admission(owner caller, request_key key, std::uint64_t deadline) noexcept
        {
            if(caller != configuration.authority || !live()) { return error::denied; }
            if(key.session != identity || !key.sequence || key.sequence == UINT64_MAX) { return error::invalid; }
            if((!has_receipt && key.sequence != 1) || (has_receipt && key != previous_key && key.sequence != previous_key.sequence + 1))
            {
                return error::stale;
            }
            if(has_receipt && key == previous_key) { return error::ok; }
            auto time = now();
            if(deadline <= time || deadline > INT64_MAX || deadline - time > configuration.max_deadline_ns) { return error::timeout; }
            return error::ok;
        }

        bool replay(request_key key,
                    handle goal,
                    handle request,
                    std::uint64_t tag,
                    uuid const& id,
                    std::span<std::uint8_t const> body,
                    std::uint64_t deadline,
                    bool accept,
                    receipt& output) noexcept
        {
            if(!has_receipt || key != previous_key || goal != previous_goal || request != previous_request || tag != previous_tag || id != previous_uuid ||
               deadline != previous_deadline || accept != previous_accept || body.size() != previous_size ||
               !std::equal(body.begin(), body.end(), previous_body.begin()))
            {
                return false;
            }
            output = previous_receipt;
            return true;
        }

        error commit(request_key key,
                     handle goal,
                     handle request,
                     std::uint64_t tag,
                     uuid const& id,
                     std::span<std::uint8_t const> body,
                     std::uint64_t deadline,
                     bool accept,
                     native_result result,
                     receipt& output) noexcept
        {
            if(result.observed == effect::none) { return result.status == error::ok ? error::io : result.status; }
            previous_key = key;
            previous_goal = goal;
            previous_request = request;
            previous_tag = tag;
            previous_uuid = id;
            previous_deadline = deadline;
            previous_accept = accept;
            previous_size = body.size();
            std::copy(body.begin(), body.end(), previous_body.begin());
            previous_receipt = {result.request_identity?result.request_identity:request ? request : goal,
                                result.provider_generation?result.provider_generation:configuration.authority.generation,key,result.status,result.observed};
            has_receipt = true;
            if(result.observed == effect::unknown)
            {
                uncertainty = true;
                if(auto slot = find(goal)) { slot->info.uncertain = true; }
            }
            output = previous_receipt;
            return error::ok;
        }

    public:
        ~endpoint()
        {
            if(!same_process())
            {
                std::construct_at(std::addressof(provider));
                std::construct_at(std::addressof(queue));
                std::construct_at(std::addressof(goals));
                std::construct_at(std::addressof(scratch));
                std::construct_at(std::addressof(previous_body));
                std::construct_at(std::addressof(configuration));
                return;
            }
            retire();
        }

        /** @brief Reserve all goal, recovery and critical-delivery storage first.
         * @details Critical capacity is (4 + 2 * max_cancels_per_goal) records
         * per goal, independently from feedback slots. Cancellation correlations
         * have their own fixed records; zero configured capacity denies cancel.
         * Rejection does not erase an earlier correlation or refund its lifetime
         * reservation. Deployments can reserve repeated lawful cancel requests.
         */
        error configure(plan selected, handle id, std::shared_ptr<native> actual)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(configured || retired) { return error::denied; }
            auto maximum = std::max({selected.max_goal_bytes, selected.max_feedback_bytes, selected.max_result_bytes, std::size_t{8}});
            if(!selected.authority || !valid(selected.selected) || !absolute_name(selected.resolved_name) || selected.action_type.empty() ||
               !selected.max_goals || selected.max_cancels_per_goal > (SIZE_MAX - 4) / 2 || maximum > (SIZE_MAX - 48) / 2 || !id || !actual ||
               !actual->live() || !selected.goal.validate || !selected.feedback.validate || !selected.result.validate || !selected.max_goal_bytes ||
               !selected.max_result_bytes || !selected.max_deadline_ns || selected.max_deadline_ns > INT64_MAX || !selected.retention_ns ||
               selected.retention_ns > INT64_MAX || (selected.transition_mask & ~std::uint64_t{510}) ||
               (selected.direction != role::client && selected.direction != role::server) || !valid(selected.goal_quality) || !valid(selected.result_quality) ||
               !valid(selected.cancel_quality) || !valid(selected.feedback_quality) || !valid(selected.status_quality))
            {
                return error::invalid;
            }
            for(auto const* schema: {&selected.goal, &selected.feedback, &selected.result})
            {
                if(!valid(schema->selected) || schema->authority.environment != selected.authority.environment || !schema->authority || schema->revoked ||
                   schema->type_identity.empty() || schema->encoding.empty())
                {
                    return error::invalid;
                }
            }
            auto critical = 4 + selected.max_cancels_per_goal * 2;
            if(selected.max_goals > (SIZE_MAX - selected.feedback_slots) / critical) { return error::too_large; }
            auto depth = selected.max_goals * critical + selected.feedback_slots;
            auto extent = maximum + 48;
            if(depth > SIZE_MAX / extent || selected.reserved_bytes < depth * extent || selected.max_goals > SIZE_MAX / selected.max_goal_bytes ||
               selected.reserved_bytes - depth * extent < selected.max_goals * selected.max_goal_bytes ||
               selected.reserved_bytes - depth * extent - selected.max_goals * selected.max_goal_bytes < extent + maximum)
            {
                return error::too_large;
            }
            try
            {
                auto staged_goals = ::uwvm2::utils::container::std_vector<goal_slot>(selected.max_goals);
                for(auto& slot: staged_goals)
                {
                    slot.goal.resize(selected.max_goal_bytes);
                    slot.cancels.resize(selected.max_cancels_per_goal);
                }
                auto staged_queue = ::uwvm2::utils::container::make_shared<receive_queue>(depth, extent, overflow::reject);
                auto incarnation=actual->delivery_generation();if(!incarnation)incarnation=selected.authority.generation;
                staged_queue->bind(id, incarnation);
                ::uwvm2::utils::container::std_vector<std::uint8_t> staged_scratch(extent), staged_previous(maximum);
                goals = std::move(staged_goals);
                queue = std::move(staged_queue);
                scratch = std::move(staged_scratch);
                previous_body = std::move(staged_previous);
                configuration = std::move(selected);
                delivery_incarnation=incarnation;
                queue_charged=actual->queued_mutations();
                queue_received=actual->queued_receives();
                provider = std::move(actual);
                identity = id;
                configured = true;
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                return error::no_memory;
            }
            catch(std::length_error const&)
            {
                return error::too_large;
            }
        }

        error submit(owner caller, request_key key, uuid const& id, std::span<std::uint8_t const> body, std::uint64_t deadline, receipt& output,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(operation&&!operation->valid(permission::action_submit,operation_family::action,caller,identity,false))return error::denied;
            auto checked = admission(caller, key, deadline);
            if(checked != error::ok) { return checked; }
            if(configuration.direction != role::client) { return error::denied; }
            if(has_receipt && key == previous_key) { return replay(key, previous_goal, 0, 0, id, body, deadline, false, output) ? error::ok : error::stale; }
            if(uncertainty) { return error::busy; }
            if(std::none_of(id.begin(), id.end(), [](auto b) { return b; }) || body.size() > configuration.max_goal_bytes || !configuration.goal.validate(body))
            {
                return error::invalid;
            }
            if(find(id)) { return error::stale; }
            auto slot = free();
            if(!slot) { return error::again; }
            auto goal = allocate();
            if(!goal) { return error::too_large; }
            auto result = operation?provider->submit(goal,id,body,deadline,operation->with_request(goal,key.session,key.sequence)):
                                  provider->submit(goal,id,body,deadline);
            result=checked_mutation(result,goal);
            if(result.observed != effect::none)
            {
                slot->info = {goal, goal, 0, state::pending, false, false, result.observed == effect::unknown, 0};
                slot->pending_submit=result.deferred?goal:0;slot->pending_response=slot->pending_correlation=0;
                slot->submit_generation=result.deferred?result.provider_generation:0;slot->response_generation=0;
                slot->identity = id;
                slot->info.goal_uuid = id;
                slot->goal_size = body.size();
                std::fill(slot->cancels.begin(), slot->cancels.end(), goal_slot::cancel_slot{});
                std::copy(body.begin(), body.end(), slot->goal.begin());
            }
            return commit(key, goal, 0, 0, id, body, deadline, false, result, output);
        }

        error respond(owner caller, request_key key, response const& value, std::uint64_t deadline, receipt& output,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(operation&&!operation->valid(permission::action_respond,operation_family::action,caller,identity,false))return error::denied;
            auto checked = admission(caller, key, deadline);
            if(checked != error::ok) { return checked; }
            if(configuration.direction != role::server) { return error::denied; }
            auto tag = static_cast<std::uint64_t>(value.change);
            if(tag < 1 || tag > 8 || !(configuration.transition_mask & (std::uint64_t{1} << tag))) { return error::denied; }
            auto slot = find(value.goal);
            if(!slot) { return error::bad_handle; }
            if(has_receipt && key == previous_key)
            {
                return replay(key, value.goal, value.request, tag, slot->identity, value.body, deadline, value.accept_cancel, output) ? error::ok
                                                                                                                                      : error::stale;
            }
            if(uncertainty || slot->info.uncertain || slot->pending_response) { return error::busy; }
            if(operation&&operation->secondary!=static_cast<permission>(static_cast<unsigned>(permission::action_accept)+tag-1))return error::denied;
            auto before = slot->info.observed, after = before;
            goal_slot::cancel_slot* cancellation{};
            if(value.change == transition::cancel_response)
            {
                for(auto& entry: slot->cancels)
                {
                    if(entry.request == value.request && value.request)
                    {
                        cancellation = &entry;
                        break;
                    }
                }
                if(!cancellation || !cancellation->delivered || cancellation->responded || (before != state::accepted && before != state::executing) ||
                   !value.body.empty())
                {
                    return error::denied;
                }
                if(value.accept_cancel) { after = state::cancelling; }
            }
            else
            {
                if(value.request != slot->info.proposal_request || !slot->info.delivered || value.accept_cancel) { return error::denied; }
                switch(value.change)
                {
                    case transition::accept:
                        if(before != state::pending) { return error::denied; }
                        after = state::accepted;
                        break;
                    case transition::reject:
                        if(before != state::pending) { return error::denied; }
                        after = state::rejected;
                        break;
                    case transition::execute:
                        if(before != state::accepted) { return error::denied; }
                        after = state::executing;
                        break;
                    case transition::feedback:
                        if(before != state::executing || value.body.size() > configuration.max_feedback_bytes || !configuration.feedback.validate(value.body))
                        {
                            return error::invalid;
                        }
                        break;
                    case transition::succeed:
                        if(before != state::executing && before != state::cancelling) { return error::denied; }
                        after = state::succeeded;
                        break;
                    case transition::abort:
                        if(before != state::executing && before != state::cancelling) { return error::denied; }
                        after = state::aborted;
                        break;
                    case transition::cancelled:
                        if(before != state::cancelling) { return error::denied; }
                        after = state::cancelled;
                        break;
                    default: return error::invalid;
                }
                if(terminal(after) && after != state::rejected)
                {
                    if(value.body.size() > configuration.max_result_bytes || !configuration.result.validate(value.body)) { return error::invalid; }
                }
                else if(value.change != transition::feedback && !value.body.empty()) { return error::invalid; }
            }
            auto operation_request=operation?allocate():0;if(operation&&!operation_request)return error::too_large;
            auto result = operation?provider->respond(value,slot->identity,after,deadline,operation->with_request(operation_request,key.session,key.sequence)):
                                  provider->respond(value,slot->identity,after,deadline);
            result=checked_mutation(result,operation_request);
            if(result.deferred&&result.observed!=effect::none)
            {
                slot->pending_response=result.request_identity?result.request_identity:operation_request;
                slot->response_generation=result.provider_generation;
                slot->pending_correlation=value.request;slot->pending_transition=value.change;slot->pending_state=after;
                if(!slot->pending_response){uncertainty=slot->info.uncertain=true;result={error::io,effect::unknown};}
            }
            if(!result.deferred && result.status==error::ok && result.observed != effect::none && result.observed != effect::unknown)
            {
                slot->info.observed = after;
                if(cancellation)
                {
                    cancellation->responded = true;
                    slot->info.cancel_delivered = false;
                }
                if(terminal(after)) { retain(*slot); }
            }
            return commit(key, value.goal, value.request, tag, slot->identity, value.body, deadline, value.accept_cancel, result, output);
        }

        /** @brief Propose cancellation of one owned UUID, including uncertain submit.
         * @details This independent control path remains available after an
         * uncertain submit. It does not erase its uncertainty latch, promise a
         * terminal state, or inherit authority to cancel unrelated goals.
         */
        error cancel(owner caller, request_key key, handle goal, std::uint64_t deadline, receipt& output,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(operation&&!operation->valid(permission::action_cancel,operation_family::action,caller,identity,false))return error::denied;
            auto checked = admission(caller, key, deadline);
            if(checked != error::ok) { return checked; }
            if(configuration.direction != role::client) { return error::denied; }
            auto slot = find(goal);
            if(!slot) { return error::bad_handle; }
            if(has_receipt && key == previous_key)
            {
                return replay(key, goal, previous_request, 9, slot->identity, {}, deadline, false, output) ? error::ok : error::stale;
            }
            if(terminal(slot->info.observed) || slot->info.observed == state::cancelling) { return error::denied; }
            goal_slot::cancel_slot* cancellation{};
            for(auto& entry: slot->cancels)
            {
                if(!entry.request)
                {
                    cancellation = &entry;
                    break;
                }
            }
            if(!cancellation) { return error::again; }
            auto request = allocate();
            if(!request) { return error::too_large; }
            auto result = operation?provider->cancel(goal,request,slot->identity,deadline,operation->with_request(request,key.session,key.sequence)):
                                  provider->cancel(goal,request,slot->identity,deadline);
            result=checked_mutation(result,request);
            if(result.observed != effect::none)
            {
                cancellation->request = request;cancellation->pending=result.deferred;cancellation->provider_generation=result.provider_generation;
                slot->info.cancel_request = request;
            }
            return commit(key, goal, request, 9, slot->identity, {}, deadline, false, result, output);
        }

        /** @brief Attempt a finite explicit set of this endpoint's owned goals once.
         * @details All identities and duplicates are checked before any native
         * call. Each candidate has its own current-authority token and native
         * request identity. Admission is intentionally not an atomic remote
         * cancellation: finite queue capacity or attenuation can refuse a later
         * candidate. The complete per-goal receipt records that distinction.
         * Exact retry only reads this retained receipt and never sends again.
         * A new key may explicitly retry only candidates with known no effect.
         * Unknown stops the batch and preserves the endpoint uncertainty latch.
         * No omitted UUID, timestamp wildcard, foreign principal or cancel-all
         * operation is inferred. Capacity is bounded by max_goals and 64; fixed
         * recovery storage is part of the endpoint object, without hot allocation.
         */
        error cancel_set(owner caller,request_key key,std::span<handle const> selected,std::uint64_t deadline,
                         cancellation_selection& output,operation_token const* operation=nullptr)
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(operation&&!operation->valid(permission::action_cancel_set,operation_family::action,caller,identity,false))return error::denied;
            auto status=admission(caller,key,deadline);if(status!=error::ok)return status;
            if(configuration.direction!=role::client)return error::denied;
            if(selected.empty()||selected.size()>maximum_cancellation_selection||selected.size()>configuration.max_goals)return error::too_large;
            if(has_receipt&&key==previous_key)
            {
                if(previous_tag!=10||previous_deadline!=deadline||previous_selection.count!=selected.size())return error::stale;
                for(std::size_t i{};i<selected.size();++i)if(previous_selection.items[i].goal!=selected[i])return error::stale;
                output=previous_selection;return error::ok;
            }
            std::array<goal_slot*,maximum_cancellation_selection> candidates{};
            std::array<handle,maximum_cancellation_selection> requests{};
            for(std::size_t i{};i<selected.size();++i)
            {
                if(!(candidates[i]=find(selected[i])))return error::bad_handle;
                for(std::size_t n{};n<i;++n)if(selected[i]==selected[n])return error::invalid;
            }
            // Reserve all correlation identities before any effect. Exhaustion
            // cannot leave a partly sent batch without its recovery record.
            auto aggregate=allocate();if(!aggregate)return error::too_large;
            for(std::size_t i{};i<selected.size();++i)if(!(requests[i]=allocate()))return error::too_large;
            cancellation_selection result;result.count=selected.size();
            result.batch={aggregate,delivery_incarnation,key,error::ok,effect::locally_accepted};
            bool stopped{};
            for(std::size_t i{};i<selected.size();++i)
            {
                auto* slot=candidates[i];auto& item=result.items[i];item.goal=selected[i];
                item.result={0,delivery_incarnation,key,error::again,effect::none};
                if(stopped){item.result.status=error::cancelled;continue;}
                if(now()>=deadline){item.result.status=error::timeout;continue;}
                if(terminal(slot->info.observed)||slot->info.observed==state::cancelling)
                {item.result.status=error::denied;continue;}
                goal_slot::cancel_slot* vacant{};
                for(auto& entry:slot->cancels)if(!entry.request){vacant=&entry;break;}
                if(!vacant)continue;
                auto request=requests[i];
                auto admitted=operation?provider->cancel(item.goal,request,slot->identity,deadline,operation->with_request(request,key.session,key.sequence)):
                    provider->cancel(item.goal,request,slot->identity,deadline);
                admitted=checked_mutation(admitted,request);
                item.result.status=admitted.status;item.result.observed=admitted.observed;
                if(admitted.observed==effect::none)continue;
                item.result.request=admitted.request_identity?admitted.request_identity:request;
                item.result.provider_generation=admitted.provider_generation?admitted.provider_generation:delivery_incarnation;
                vacant->request=request;vacant->pending=admitted.deferred;vacant->provider_generation=admitted.provider_generation;
                slot->info.cancel_request=request;
                if(admitted.observed==effect::unknown)
                {uncertainty=slot->info.uncertain=stopped=true;result.batch.observed=effect::unknown;result.batch.status=error::io;}
            }
            previous_key=key;previous_goal=0;previous_request=aggregate;previous_tag=10;previous_deadline=deadline;
            previous_uuid={};previous_accept=false;previous_size=0;previous_receipt=result.batch;
            previous_selection=result;has_receipt=true;output=result;return error::ok;
        }

        /** @brief Cached set receipt remains recoverable after revoke or retirement.
         * @details The caller's exact set authority is checked by the dispatcher;
         * this method performs no provider polling, admission or new effect.
         */
        error selection_for(owner caller,handle request,cancellation_selection& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!configured||caller!=configuration.authority||!has_receipt||previous_tag!=10||request!=previous_selection.batch.request)return error::denied;
            output=previous_selection;return error::ok;
        }

        /** @brief Trusted native server ingress; reserve before exposing proposal. */
        error offer(uuid const& id, handle request, std::span<std::uint8_t const> body, handle& goal,
                    ingress_ownership ownership=ingress_ownership::callback,handle reserved_goal=0,std::optional<std::uint64_t> native_loss={})
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(!live() || configuration.direction != role::server || uncertainty) { return error::denied; }
            if(!request || std::none_of(id.begin(), id.end(), [](auto b) { return b; }) || body.size() > configuration.max_goal_bytes ||
               !configuration.goal.validate(body))
            {
                return error::invalid;
            }
            if(find(id)) { return error::stale; }
            if(reserved_goal&&(ownership!=ingress_ownership::retained||find(reserved_goal)))return error::denied;
            auto slot = free();
            if(!slot) { return error::again; }
            auto allocated = reserved_goal?reserved_goal:allocate();
            if(!allocated) { return error::too_large; }
            slot->info = {allocated, request, 0, state::pending, false, false, false, 0};
            slot->pending_submit=slot->pending_response=slot->pending_correlation=0;
            slot->identity = id;
            slot->info.goal_uuid = id;
            slot->goal_size = body.size();
            std::fill(slot->cancels.begin(), slot->cancels.end(), goal_slot::cancel_slot{});
            std::copy(body.begin(), body.end(), slot->goal.begin());
            auto status = emit(*slot, event_kind::proposal, request, body,{},ownership,native_loss);
            if(status != error::ok)
            {
                slot->info = {};
                return status;
            }
            goal = allocated;
            return error::ok;
        }

        error offer_cancel(uuid const& id, handle request, handle& goal,ingress_ownership ownership=ingress_ownership::callback,
                           std::optional<std::uint64_t> native_loss={})
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(!live() || configuration.direction != role::server || uncertainty) { return error::denied; }
            auto slot = find(id);
            if(!request || !slot || (slot->info.observed != state::accepted && slot->info.observed != state::executing)) { return error::denied; }
            goal_slot::cancel_slot* cancellation{};
            for(auto& entry: slot->cancels)
            {
                if(entry.request == request) { return error::stale; }
                if(!entry.request && !cancellation) { cancellation = &entry; }
            }
            if(!cancellation) { return error::again; }
            auto status = emit(*slot, event_kind::cancel_request, request, {},{},ownership,native_loss);
            if(status == error::ok) { cancellation->request=request;slot->info.cancel_request=request;goal = slot->info.goal; }
            return status;
        }

        /** @brief Trusted client ingress from the exact native UUID correlation.
         * @details Native adapters already validate UUID and protocol response
         * shape. This method additionally checks legal state and full schemas.
         * A result/acceptance cannot overwrite an earlier terminal observation.
         * State, cancellation acknowledgement and retention are committed only
         * after the complete event is queued. Retained provider input may retry
         * queue pressure without inventing a drop or consuming a correlation.
         */
        error observe(handle goal, event_kind kind, state value, std::span<std::uint8_t const> body = {}, handle correlation = 0,
                      ingress_ownership ownership=ingress_ownership::callback,std::optional<std::uint64_t> native_loss={})
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(!live() || configuration.direction != role::client) { return error::denied; }
            auto slot = find(goal);
            if(!slot) { return error::bad_handle; }
            auto before = slot->info.observed;
            if(terminal(before)) { return error::stale; }
            goal_slot::cancel_slot* cancellation{};
            switch(kind)
            {
                case event_kind::acceptance:
                    if(before != state::pending || (value != state::accepted && value != state::rejected) || !body.empty()) { return error::invalid; }
                    break;
                case event_kind::status:
                    if(!body.empty())return error::invalid;
                    if(value == before) { return error::ok; }
                    if(!body.empty() || !((before == state::accepted && value == state::executing) ||
                                          ((before == state::accepted || before == state::executing) && value == state::cancelling)))
                    {
                        return error::invalid;
                    }
                    break;
                case event_kind::feedback:
                    if(before == state::pending || value != before || body.size() > configuration.max_feedback_bytes || !configuration.feedback.validate(body))
                    {
                        return error::invalid;
                    }
                    break;
                case event_kind::result:
                    if(before == state::pending || !terminal(value) || value == state::rejected || body.size() > configuration.max_result_bytes ||
                       !configuration.result.validate(body))
                    {
                        return error::invalid;
                    }
                    break;
                case event_kind::cancel_response:
                {
                    for(auto& entry: slot->cancels)
                    {
                        if(entry.request == correlation && correlation)
                        {
                            cancellation = &entry;
                            break;
                        }
                    }
                    wire::reader decoded{body};
                    std::uint64_t code{};
                    if(!cancellation || cancellation->responded || !decoded.u64(code) || !decoded.done() || code > 3 ||
                       (code == 0 ? value != state::cancelling : value != before))
                    {
                        return error::invalid;
                    }
                    break;
                }
                default: return error::denied;
            }
            auto status=emit(*slot,kind,kind==event_kind::cancel_response?correlation:slot->info.proposal_request,body,value,ownership,native_loss);
            if(status!=error::ok)return status;
            if(cancellation)cancellation->responded=true;
            slot->info.observed = value;
            if(terminal(value)) { retain(*slot); }
            return error::ok;
        }

        error take(owner caller, void* argument, sink deliver,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<receive_queue> selected;std::shared_ptr<native> actual;
            {
                std::lock_guard lock{mutex};
                if(caller != configuration.authority || !live() || !deliver) { return error::denied; }
                if(operation&&!operation->valid(permission::action_take,operation_family::action,caller,identity,false))return error::denied;
                selected = queue;actual=provider;
            }
            if(operation)
            {
                // A failed guest copy already owns the complete event. Retry
                // that event without consuming another native query slot.
                std::uint64_t revision{};bool ready{},terminal{};selected->observe(revision,ready,terminal);
                if(!ready||!queue_received){auto status=actual->poll(*operation);if(status!=error::ok&&status!=error::again)return status;}
            }
            return selected->take(
                [&](communication::delivery const& input)
                {
                    wire::reader fields{input.body};
                    std::uint64_t kind{}, goal{}, request{}, observed{}, loss{};
                    wire::bytes body;
                    if(!fields.u64(kind) || !fields.u64(goal) || !fields.u64(request) || !fields.u64(observed) || !fields.u64(loss) || !fields.blob(body) ||
                       !fields.done())
                    {
                        return false;
                    }
                    delivery record{input.information, goal, request, static_cast<event_kind>(kind), static_cast<state>(observed), body};
                    record.information.loss_before = loss;
                    {
                        std::lock_guard lock{mutex};
                        auto slot = find(goal);
                        if(!live() || !slot) { return false; }
                        record.goal_uuid = slot->identity;
                    }
                    if(!deliver(argument, record)) { return false; }
                    std::lock_guard lock{mutex};
                    if(!live()) { return false; }
                    auto slot = find(goal);
                    if(!slot || !slot->queued) { return false; }
                    if(record.kind == event_kind::proposal) { slot->info.delivered = true; }
                    if(record.kind == event_kind::cancel_request)
                    {
                        for(auto& entry: slot->cancels)
                        {
                            if(entry.request == request)
                            {
                                entry.delivered = true;
                                break;
                            }
                        }
                        slot->info.cancel_delivered = true;
                    }
                    if(record.kind == event_kind::feedback) { --feedback_queued; }
                    --slot->queued;
                    return true;
                });
        }

        /** @brief Observe bounded readiness without consuming action events.
         * @details Origin admission belongs to the dispatcher. Resource identity
         * and provider generation are independently checked here; readiness does
         * not reveal goal UUIDs, payloads or native handles.
         */
        error observe(owner caller,handle resource,handle generation,std::uint64_t& revision,bool& ready,bool& terminal) noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(caller!=configuration.authority||resource!=identity||generation!=configuration.authority.generation||!live())return error::denied;
            queue->observe(revision,ready,terminal);return error::ok;
        }

        error describe(owner caller, handle goal, goal_info& output)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(caller != configuration.authority || !live()) { return error::denied; }
            auto slot = find(goal);
            if(!slot) { return error::bad_handle; }
            output = slot->info;
            return error::ok;
        }

        error describe(owner caller, endpoint_info& output)
        {
            if(!same_process()) { return error::denied; }
            std::lock_guard lock{mutex};
            if(caller != configuration.authority || !live()) { return error::denied; }
            output = {configuration.selected,
                      configuration.resolved_name,
                      configuration.action_type,
                      configuration.direction,
                      configuration.max_goals,
                      configuration.max_cancels_per_goal,
                      configuration.retention_ns,
                      uncertainty};
            return error::ok;
        }

        /** @brief Cached immutable accounting mode; never a provider callback. */
        bool queued_submission()noexcept
        {if(!same_process())return false;std::lock_guard lock{mutex};return queue_charged;}
        bool queued_delivery()noexcept
        {if(!same_process())return false;std::lock_guard lock{mutex};return queue_received;}
        /** @brief Recover the last exact observed result without reexecuting it.
         * @details The dispatcher separately admits the recovering caller. This
         * cached lookup remains available after retirement and never grants new
         * action authority, polls a worker or advances a goal's state.
         */
        error receipt_for(owner caller,handle request,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!configured||configuration.authority!=caller||!has_receipt||previous_receipt.request!=request)return error::denied;
            output=previous_receipt;return error::ok;
        }
        /** @brief Settle one retained native queue receipt, never a guest-provided result.
         * @details A queue acknowledgement leaves server state unchanged. Only a
         * complete worker reply can apply the reserved transition. Known no-effect
         * failures retain the prior state; partial writes and lost replies latch
         * uncertainty. This callback runs after releasing queue/transport locks.
         * Revocation cannot erase pending results or turn cleanup into success.
         */
        error settle_operation(handle request,native_result result) noexcept
        {
            if(!same_process()||!request||result.deferred||!known_status(result.status))return error::invalid;
            std::lock_guard lock{mutex};if(!configured)return error::denied;
            for(auto& slot:goals)
            {
                bool submit=slot.pending_submit==request,response=slot.pending_response==request;
                goal_slot::cancel_slot* cancel{};
                for(auto& value:slot.cancels)if(value.request==request&&value.pending){cancel=&value;break;}
                if(!submit&&!response&&!cancel)continue;
                auto generation=submit?slot.submit_generation:response?slot.response_generation:cancel->provider_generation;
                if(result.request_identity!=request||!generation||result.provider_generation!=generation||result.observed>effect::unknown)return error::denied;
                result=checked_mutation(result,request);
                bool unknown=result.observed==effect::unknown || (result.status==error::ok&&result.observed==effect::none);
                bool applied=result.status==error::ok&&result.observed!=effect::none&&!unknown;
                if(submit)
                {
                    slot.pending_submit=0;
                    if(result.observed==effect::none)
                    {
                        if(slot.info.observed!=state::pending)unknown=true;
                        else{slot.info.observed=state::rejected;retain(slot);}
                    }
                }
                if(response)
                {
                    if(applied)
                    {
                        slot.info.observed=slot.pending_state;
                        if(slot.pending_transition==transition::cancel_response)
                            for(auto& value:slot.cancels)if(value.request==slot.pending_correlation){value.responded=true;slot.info.cancel_delivered=false;break;}
                        if(terminal(slot.info.observed))retain(slot);
                    }
                    else if(result.observed!=effect::none)unknown=true;
                    slot.pending_response=slot.pending_correlation=0;
                }
                if(cancel)
                {
                    cancel->pending=false;
                    if(result.observed==effect::none)
                    {
                        if(cancel->responded)unknown=true;
                        else{if(slot.info.cancel_request==cancel->request)slot.info.cancel_request=0;*cancel={};}
                    }
                }
                if(unknown)uncertainty=slot.info.uncertain=true;
                if(has_receipt&&previous_receipt.request==request)
                {previous_receipt.status=result.status;previous_receipt.observed=result.observed;}
                if(has_receipt&&previous_tag==10)
                    for(std::size_t i{};i<previous_selection.count;++i)if(previous_selection.items[i].result.request==request)
                    {
                        auto& item=previous_selection.items[i].result;item.status=result.status;item.observed=result.observed;
                        if(unknown){previous_receipt.observed=previous_selection.batch.observed=effect::unknown;
                            previous_receipt.status=previous_selection.batch.status=error::io;}
                    }
                return error::ok;
            }
            return error::stale;
        }

        /** @brief Fence an asynchronous malformed/lost native protocol outcome.
         * @details A failed response decoder or a failed automatic result request
         * is not a successful remote completion. The exact goal remains owned,
         * its uncertainty is inspectable, and only the separately admitted cancel
         * path can continue. This latch cannot be cleared by another native event.
         */
        void mark_unknown(handle goal) noexcept
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            if(auto slot = find(goal))
            {
                slot->info.uncertain = true;
                uncertainty = true;
            }
        }

        /** @brief Match the actual host endpoint before attaching an isolated facade. */
        bool owns(owner caller,handle resource)noexcept
        {if(!same_process())return false;std::lock_guard lock{mutex};return configured&&!retired&&configuration.authority==caller&&identity==resource;}

        error close(owner caller,operation_token const* operation=nullptr)
        {
            if(!same_process()) { return error::denied; }
            std::shared_ptr<native> selected;
            {
                std::lock_guard lock{mutex};
                if(!configured || caller != configuration.authority) { return error::denied; }
                if(retired) { return error::ok; }
                if(closing_call)return error::again;
                if(operation&&!operation->valid(permission::resource_close,operation_family::action,caller,identity,false))return error::denied;
                bool isolated=provider&&provider->requires_close_receipt();
                if(uncertainty||(!isolated&&provider&&provider->occupied()))return error::busy;
                if(isolated&&!operation&&!provider->cleanup_confirmed())return error::busy;
                for(auto const& goal: goals)
                {
                    if(goal.info.goal && (!terminal(goal.info.observed)||goal.pending_submit||goal.pending_response||
                        std::any_of(goal.cancels.begin(),goal.cancels.end(),[](auto const& value){return value.pending;}))) { return error::busy; }
                }
                closing_call=true;selected = provider;
            }
            auto status=operation?selected->close(*operation):error::ok;
            {
                std::lock_guard lock{mutex};closing_call=false;
                if(status!=error::ok)return status;
                if(!selected->cleanup_confirmed())return error::busy;
                retired=true;queue->retire();
            }
            selected->retire();
            return error::ok;
        }

        /** @brief Trusted deployment debt check; usable after revocation.
         * @details The owning deployment must retain a retired endpoint/profile
         * while this is true and refuse reconnect/replacement. Destroying an
         * object is not reconciliation; durable restart fencing belongs to the
         * deployment journal. This observation grants no guest authority.
         */
        bool cleanup_debt() noexcept
        {
            if(!same_process()) { return true; }
            std::lock_guard lock{mutex};
            if(uncertainty||(provider&&provider->occupied()))return true;
            for(auto const& slot:goals)if(slot.pending_submit||slot.pending_response||
                std::any_of(slot.cancels.begin(),slot.cancels.end(),[](auto const& value){return value.pending;}))return true;
            return false;
        }

        void retire() noexcept
        {
            if(!same_process()) { return; }
            std::shared_ptr<native> selected;
            {
                std::lock_guard lock{mutex};
                if(retired || !configured) { return; }
                retired = true;
                for(auto& goal: goals)
                {
                    if(goal.info.goal && !terminal(goal.info.observed)) { uncertainty = goal.info.uncertain = true; }
                }
                queue->retire();
                selected = provider;
            }
            selected->retire();
        }
    };
}  // namespace uwvm2::imported::ros::scene::communication::actions
