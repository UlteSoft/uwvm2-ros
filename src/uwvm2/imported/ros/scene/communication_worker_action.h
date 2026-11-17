/** @file
 * @brief Typed isolated actions with bounded identity translation and retained ingress.
 * @details Submit, each response transition and cancellation carry the actual
 * caller and complete node/context/three-schema origin cohort. The owned effect
 * queue is the sole charging point. A queue acknowledgement cannot advance host
 * action state: the supervisor must settle the verified completion in the host
 * ledger before releasing it. Native goal/proposal/cancel IDs never escape as
 * host handles, and UUIDs never authorize an operation.
 *
 * One independent event query retains the complete native record across local
 * backpressure. Failed guest delivery is subsequently retained by the host core.
 * An independent close slot requires exact native destruction evidence. No
 * method starts a thread, waits for a worker or accesses a guest socket. All
 * owning allocations are recoverable fast_io; configuration reserves finite
 * queue, correlation and cleanup capacity before any operation is admitted.
 */
#pragma once
#include "communication_action_correlations.h"
#include "communication_mutations.h"
#include "communication_mutation_reply.h"
#include "communication_queries.h"
#include "communication_worker_closure.h"
#include "communication_workspace.h"
#include <uwvm2/utils/container/unique.h>
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class action_endpoint final:public communication::actions::native
    {
        using action=communication::actions::endpoint;using policy_type=communication::actions::plan;
        using role=communication::actions::role;using uuid=communication::actions::uuid;
        using error=communication::error;using effect=communication::effect;using handle=communication::handle;
        using token=communication::operation_token;using permission=communication::permission;using result=communication::native_result;
        struct retained{token origin;handle goal{};result outcome;bool ready{};};
        struct state
        {
            // Callback ownership outlives all callback-bearing queues below.
            std::shared_ptr<void> authority_owner;
            communication_workspace::lease reservation; // Destroyed after all queues and rows.
            policy_type policy;communication::node_lease node;communication_jobs::plan queue;
            mutations effects;queries events;endpoint_closure closure;action_correlations mapping;
            std::weak_ptr<action> receiver;
            std::mutex operation_mutex;retained pending;
            handle native{};std::uint64_t last_sequence{},last_loss{};
            std::atomic<bool> attached{},poisoned{};
        };
        ::uwvm2::utils::container::std_unique_ptr<state> owned;
        std::mutex configuration;std::atomic<bool> configured{},stopping{};pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        endpoint_closure::call enter()noexcept
        {return same_process()&&configured.load(std::memory_order_acquire)?owned->closure.enter():endpoint_closure::call{};}
        static communication::delivery_consumer caller(token const& value)noexcept
        {return {value.caller_instance,value.caller_generation,value.capability};}
        bool cohort(token const& origin)const noexcept
        {
            return origin.origins[1].resource==owned->node.identity()&&origin.origins[1].authority==owned->node.authority()&&
                origin.origins[2].resource==owned->node.context()&&origin.origins[3].authority==owned->policy.goal.authority&&
                origin.origins[4].authority==owned->policy.feedback.authority&&origin.origins[5].authority==owned->policy.result.authority;
        }
        bool valid(token const& origin,permission operation,bool request=false)const noexcept
        {return live()&&owned->attached.load(std::memory_order_acquire)&&origin.valid(operation,communication::operation_family::action,
            owned->policy.authority,owned->queue.resource,request)&&cohort(origin);}
        static bool same(token const& a,token const& b)noexcept
        {
            return a.caller_instance==b.caller_instance&&a.caller_generation==b.caller_generation&&a.authority==b.authority&&a.capability==b.capability&&
                a.resource==b.resource&&a.request==b.request&&a.family==b.family&&a.action==b.action&&a.secondary==b.secondary&&a.session==b.session&&
                a.sequence==b.sequence&&a.watch==b.watch&&a.count==b.count&&a.origins==b.origins&&a.retained_origins==b.retained_origins;
        }
        error finish()noexcept
        {
            auto& pending=owned->pending;if(!pending.origin.request||pending.ready)return error::again;
            std::array<std::uint8_t,160> bytes;std::size_t count{};token original;result value;
            auto status=owned->effects.completion(original,value,bytes,count);bool malformed=status==error::too_large;
            if(malformed)status=owned->effects.completion(original,value);
            if(status!=error::ok)return status;
            handle native{};
            if(malformed||!same(original,pending.origin)||!mutation_evidence({bytes.data(),count},value,native))
                value={error::io,effect::unknown,pending.origin.request,owned->queue.provider_generation,false};
            auto who=caller(pending.origin);auto mapped=pending.origin.action==permission::action_submit?
                owned->mapping.submitted(who,pending.goal,value,value.status==error::ok?native:0):(pending.origin.action==permission::action_cancel||pending.origin.action==permission::action_cancel_set)?
                owned->mapping.cancelled(who,pending.goal,value,value.status==error::ok?native:0):owned->mapping.responded(who,pending.goal,value);
            if(mapped!=error::ok)value={error::io,effect::unknown,pending.origin.request,owned->queue.provider_generation,false};
            if(value.observed==effect::unknown)owned->poisoned=true;
            pending.outcome=value;pending.ready=true;return error::ok;
        }
        error ingress(token const& origin,wire::bytes bytes)noexcept
        {
            namespace a=communication::actions;
            action_record record;
            if(!decode_action_record(bytes,owned->policy,owned->native,owned->queue.provider_generation,record)||
                record.information.sequence<=owned->last_sequence||record.information.loss_before<owned->last_loss)return error::invalid;
            auto core=owned->receiver.lock();if(!core)return error::cancelled;
            action_correlations::translated mapped;auto who=caller(origin);error status;
            // The mapping mutex is released before entering the host ledger.
            // Conversely a host mutation may call this facade while holding the
            // host ledger mutex. No provider lock spans both directions.
            if(record.kind==a::event_kind::proposal)
            {
                status=owned->mapping.proposal(who,record,mapped);if(status!=error::ok)return status;
                handle goal{};status=core->offer(record.identity,mapped.request,record.payload,goal,a::ingress_ownership::retained,mapped.goal,record.information.loss_before);
                if(status==error::ok&&goal!=mapped.goal)return error::io;
            }
            else if(record.kind==a::event_kind::cancel_request)
            {
                status=owned->mapping.cancellation(who,record,mapped);if(status!=error::ok)return status;
                handle goal{};status=core->offer_cancel(record.identity,mapped.request,goal,a::ingress_ownership::retained,record.information.loss_before);
                if(status==error::ok&&goal!=mapped.goal)return error::io;
            }
            else
            {
                status=owned->mapping.event(who,record,mapped);if(status!=error::ok)return status;
                status=core->observe(mapped.goal,record.kind,record.observed,record.payload,mapped.request,a::ingress_ownership::retained,record.information.loss_before);
            }
            if(status==error::ok){owned->last_sequence=record.information.sequence;owned->last_loss=record.information.loss_before;}
            return status;
        }
    public:
        action_endpoint()=default;action_endpoint(action_endpoint const&)=delete;action_endpoint& operator=(action_endpoint const&)=delete;
        ~action_endpoint()override{if(!same_process()){std::construct_at(std::addressof(owned));return;}retire();}
        /** @brief Exact adapter reservation; host action-ledger and middleware budgets are separate. */
        static std::uint64_t reservation_bytes(policy_type const& policy,std::uint64_t request,std::uint64_t reply)noexcept
        {
            auto rows=action_correlations::reservation_bytes(policy.max_goals,policy.max_cancels_per_goal);
            if(!rows||!request||request>4u*1024u*1024u||reply<160||reply>4u*1024u*1024u)return 0;
            return 2*request+reply+2*(request+reply)+rows+endpoint_closure::reserved_bytes;
        }
        /** @brief Freeze the exact trusted plan and native creation evidence without I/O.
         * @details A supplied preparation lease moves only after every allocation
         * and plan copy succeeds. Failed preparation leaves that lease with its
         * owner. The facade can be attached to exactly one matching host ledger.
         */
        error configure(policy_type const& policy,communication::node_lease parent,communication_jobs::plan queue,handle native,
                        communication_jobs::admit admit,communication_jobs::charge charge,void* context,
                        communication_workspace::lease* reservation=nullptr,std::shared_ptr<void> authority_owner={})noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{configuration,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(configured.load()||stopping.load())return error::denied;
            if(authority_owner&&authority_owner.get()!=context)return error::invalid;
            auto needed=reservation_bytes(policy,queue.request_bytes,queue.reply_bytes);
            if(!needed||!parent.live()||!native||!admit||!charge||!policy.authority||policy.authority.environment!=parent.authority().environment||
                !communication::valid(policy.selected)||!communication::absolute_name(policy.resolved_name)||policy.action_type.empty()||policy.direction>role::server||
                queue.authority!=policy.authority||queue.family!=communication::operation_family::action||!queue.resource||!queue.binding||!queue.provider_generation||
                queue.slots!=1||!policy.max_deadline_ns||policy.max_deadline_ns>INT64_MAX||queue.max_deadline_ns!=policy.max_deadline_ns||
                queue.reserved_bytes<needed||policy.transition_mask&~std::uint64_t{510})return error::invalid;
            for(auto const* schema:{&policy.goal,&policy.feedback,&policy.result})
                if(!schema->authority||schema->authority.environment!=policy.authority.environment||!communication::valid(schema->selected)||schema->revoked||
                    schema->type_identity.empty()||schema->type_identity.size()>255||schema->encoding.empty()||schema->encoding.size()>255||!schema->validate)return error::invalid;
            if(!policy.max_goal_bytes||!policy.max_result_bytes||policy.max_goal_bytes>4u*1024u*1024u||policy.max_feedback_bytes>4u*1024u*1024u||
                policy.max_result_bytes>4u*1024u*1024u||queue.request_bytes<std::max({policy.max_goal_bytes,policy.max_feedback_bytes,policy.max_result_bytes})+64||
                queue.reply_bytes<std::max({policy.max_goal_bytes,policy.max_feedback_bytes,policy.max_result_bytes})+1024)return error::too_large;
            try
            {
                auto pending=::uwvm2::utils::container::make_unique<state>();pending->policy=policy;pending->queue=queue;pending->native=native;
                pending->authority_owner=std::move(authority_owner);
                auto status=pending->mapping.configure({policy.authority,queue.resource,native,queue.provider_generation,policy.direction,policy.max_goals,policy.max_cancels_per_goal});
                if(status!=error::ok)return status;
                auto part=queue;part.reserved_bytes=2*queue.request_bytes+queue.reply_bytes;
                status=pending->effects.configure(part,admit,charge,context);if(status!=error::ok)return status;
                part.reserved_bytes=2*(queue.request_bytes+queue.reply_bytes);status=pending->events.configure(part,admit,charge,context);if(status!=error::ok)return status;
                status=pending->closure.configure(queue,native,admit,charge,context);if(status!=error::ok)return status;
                pending->node=std::move(parent);if(reservation)pending->reservation=std::move(*reservation);
                owned=std::move(pending);configured.store(true,std::memory_order_release);return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::invalid;}
        }
        bool live()const noexcept override
        {return same_process()&&configured.load(std::memory_order_acquire)&&!stopping.load()&&!owned->poisoned.load()&&owned->node.live();}
        error attach(std::shared_ptr<action> const& core)noexcept
        {
            if(!live()||!core||!core->owns(owned->policy.authority,owned->queue.resource))return error::denied;
            std::lock_guard lock{configuration};if(owned->attached.load())return error::denied;
            owned->receiver=core;owned->attached.store(true,std::memory_order_release);return error::ok;
        }
        bool queued_mutations()const noexcept override{return true;}
        bool queued_receives()const noexcept override{return true;}
        handle delivery_generation()const noexcept override
        {return same_process()&&configured.load(std::memory_order_acquire)?owned->queue.provider_generation:0;}
        /** @brief Transfer a pre-reserved slot after both facade and host ledger exist.
         * @details The factory retains this lease through recoverable host-ledger
         * allocation failures. This final bookkeeping step performs no allocation,
         * I/O or admission and may be used only before publishing the endpoint.
         */
        error retain_capacity(communication_workspace::lease& reservation)noexcept
        {
            if(!live()||!reservation)return error::denied;
            std::lock_guard lock{configuration};if(owned->reservation)return error::denied;
            owned->reservation=std::move(reservation);return error::ok;
        }
        handle resource()const noexcept{return same_process()&&configured.load()?owned->queue.resource:0;}
        result submit(handle,uuid const&,wire::bytes,std::uint64_t)noexcept override{return {error::denied,effect::none};}
        result respond(communication::actions::response const&,uuid const&,communication::actions::state,std::uint64_t)noexcept override{return {error::denied,effect::none};}
        result cancel(handle,handle,uuid const&,std::uint64_t)noexcept override{return {error::denied,effect::none};}
        result submit(handle goal,uuid const& id,wire::bytes body,std::uint64_t deadline,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            if(!valid(origin,permission::action_submit,true)||origin.request!=goal||owned->policy.direction!=role::client)return {error::denied,effect::none};
            if(body.size()>owned->policy.max_goal_bytes||!owned->policy.goal.validate(body))return {error::invalid,effect::none};
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||owned->pending.origin.request)return {error::again,effect::none};
            auto who=caller(origin);auto status=owned->mapping.begin_submit(who,goal,id);if(status!=error::ok)return {status,effect::none};
            auto accepted=owned->effects.action_submit(origin,id,body,deadline);
            if(accepted.status!=error::ok||!accepted.deferred){if(owned->mapping.abandon_submit(who,goal)!=error::ok)owned->poisoned=true;return accepted;}
            owned->pending={origin,goal,{},{}};return accepted;
        }
        result respond(communication::actions::response const& response,uuid const&,communication::actions::state,std::uint64_t deadline,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            if(!valid(origin,permission::action_respond,true)||owned->policy.direction!=role::server)return {error::denied,effect::none};
            auto transition=static_cast<std::uint64_t>(response.change);
            if(transition<1||transition>8||!(owned->policy.transition_mask&(std::uint64_t{1}<<transition))||
                origin.secondary!=static_cast<permission>(static_cast<unsigned>(permission::action_accept)+transition-1))return {error::denied,effect::none};
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||owned->pending.origin.request)return {error::again,effect::none};
            action_correlations::translated target;auto who=caller(origin);
            auto status=owned->mapping.begin_response(who,response.goal,response.request,origin.request,response.change==communication::actions::transition::cancel_response,target);
            if(status!=error::ok)return {status,effect::none};
            auto accepted=owned->effects.action_respond(origin,target.native_request,target.native_goal,transition,response.body,response.accept_cancel,deadline);
            if(accepted.status!=error::ok||!accepted.deferred)
            {if(owned->mapping.responded(who,response.goal,{accepted.status,effect::none,origin.request,owned->queue.provider_generation,false})!=error::ok)owned->poisoned=true;return accepted;}
            owned->pending={origin,response.goal,{},{}};return accepted;
        }
        result cancel(handle goal,handle request,uuid const&,std::uint64_t deadline,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            auto permission=origin.action==communication::permission::action_cancel_set?communication::permission::action_cancel_set:communication::permission::action_cancel;
            if(!valid(origin,permission,true)||origin.request!=request||owned->policy.direction!=role::client)return {error::denied,effect::none};
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||owned->pending.origin.request)return {error::again,effect::none};
            action_correlations::translated target;auto who=caller(origin);auto status=owned->mapping.begin_cancel(who,goal,request,target);
            if(status!=error::ok)return {status,effect::none};
            auto accepted=owned->effects.action_cancel(origin,target.native_goal,deadline);
            if(accepted.status!=error::ok||!accepted.deferred)
            {if(owned->mapping.cancelled(who,goal,{accepted.status,effect::none,request,owned->queue.provider_generation,false},0)!=error::ok)owned->poisoned=true;return accepted;}
            owned->pending={origin,goal,{},{}};return accepted;
        }
        error poll(token const& origin)noexcept override
        {
            auto active=enter();if(!active)return error::cancelled;
            if(!valid(origin,permission::action_take)||origin.request)return error::denied;
            {std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||owned->pending.origin.request)return error::again;}
            auto now=communication::now();if(now>INT64_MAX-owned->policy.max_deadline_ns)return error::too_large;
            auto status=owned->events.action_take(origin,now+owned->policy.max_deadline_ns,[&](wire::bytes bytes){return ingress(origin,bytes);});
            if(status==error::invalid||status==error::io||status==error::stale)owned->poisoned=true;
            return status;
        }
        /** @brief Advance owned nonblocking RPCs; never call the host ledger under this lock. */
        error pump(communication_worker_async::transport& transport)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            bool progress{};
            auto status=owned->effects.pump(transport);if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
            {std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(lock.owns_lock()){status=finish();if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;}}
            status=owned->events.pump(transport);if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
            status=owned->closure.pump(transport);if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
            return progress?error::ok:error::again;
        }
        error completion(token& origin,result& output)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||!owned->pending.ready)return error::again;
            origin=owned->pending.origin;output=owned->pending.outcome;return error::ok;
        }
        error release_completion(token const& origin)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!owned->pending.ready||!same(origin,owned->pending.origin))return error::denied;
            if(owned->pending.outcome.observed==effect::unknown||owned->poisoned.load())return error::busy;
            auto status=owned->effects.release(origin);if(status==error::ok)owned->pending={};return status;
        }
        error release_goal(handle goal)noexcept override
        {
            if(!live())return error::denied;
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||owned->pending.origin.request||owned->events.occupied())return error::again;
            return owned->mapping.release_terminal(goal);
        }
        bool requires_close_receipt()const noexcept override{return true;}
        error close(token const& origin)noexcept override
        {
            if(!same_process()||!configured.load()||!origin.valid(permission::resource_close,communication::operation_family::action,
                owned->policy.authority,owned->queue.resource,false)||!cohort(origin))return error::denied;
            return owned->closure.close(origin,[&]noexcept{return !owned->poisoned.load()&&!owned->effects.occupied()&&!owned->events.occupied()&&!owned->mapping.occupied();});
        }
        bool cleanup_confirmed()const noexcept override
        {return same_process()&&configured.load()&&owned->closure.cleanup_confirmed();}
        bool occupied()const noexcept override
        {return !same_process()||!configured.load()||owned->poisoned.load()||owned->effects.occupied()||owned->events.occupied()||owned->mapping.occupied()||owned->closure.occupied();}
        void request_stop()noexcept
        {if(same_process()){stopping=true;if(configured.load()){owned->effects.request_stop();owned->events.request_stop();owned->closure.request_stop();owned->mapping.retire();}}}
        void retire()noexcept override
        {
            if(!same_process())return;
            request_stop();if(!configured.load())return;
            owned->effects.retire();owned->events.retire();owned->closure.retire();std::lock_guard lock{owned->operation_mutex};
        }
    };
#endif
}
