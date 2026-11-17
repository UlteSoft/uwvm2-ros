/** @file
 * @brief Typed service endpoint with isolated IPC and finite host correlations.
 * @details A service holds a counted node lease, independent request/response
 * schemas and an immutable worker incarnation. Host submissions, worker requests
 * and receive reservations have separate identities. Server responses require
 * an acknowledged request belonging to the exact consumer. No socket or native
 * pointer is exposed and no authority is inherited from endpoint creation.
 *
 * Submit/respond, observation, receive settlement and local-interest cancellation
 * have separate pre-reserved queues. Guest calls enqueue or recover; the existing
 * supervisor pumps bounded nonblocking IPC outside authority/registry locks.
 * Unknown native effects retain both receipt and correlation capacity. A known
 * local abandonment never means remote execution was cancelled or physically
 * stopped. All owning allocation uses recoverable fast_io containers.
 */
#pragma once
#include "communication_workspace.h"
#include "communication_mutations.h"
#include "communication_queries.h"
#include "communication_worker_receiver.h"
#include "communication_mutation_reply.h"
#include "communication_worker_closure.h"
#include <uwvm2/utils/container/unique.h>
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    /** @brief Explicit payload and correlation storage reservation for one service.
     * @details Includes two mutation slots/encoders, one query with detached
     * reply, one receive/settlement queue with detached payload and every mapping
     * row. Native middleware storage is separately bounded by the service plan.
     * An absent/zero reservation disables creation; it never means unlimited.
     */
    struct service_capacity{std::uint64_t request_bytes{},reply_bytes{},reserved_bytes{};};

    class service_endpoint final:public communication::native_service
    {
        using error=communication::error;using effect=communication::effect;using token=communication::operation_token;
        using handle=communication::handle;using result=communication::native_result;using consumer=communication::delivery_consumer;
        struct retained
        {
            token origin;handle correlation{};result outcome;
            bool ready{},released{};
        };
        struct state
        {
            // Owned admission context outlives all callback-bearing queues.
            std::shared_ptr<void> authority_owner;
            // Capacity also outlives every owned queue.
            communication_workspace::lease reservation;
            communication::service_plan policy;communication::type_plan request_schema,response_schema;communication::node_lease node;
            communication_jobs::plan queue;
            mutations effects,cancellations;queries observations;endpoint_receiver receiver;endpoint_closure closure;
            std::shared_ptr<correlations> mapping;
            std::mutex operation_mutex;retained pending,abandonment;
            std::atomic<bool> poisoned{};
        };
        ::uwvm2::utils::container::std_unique_ptr<state> owned;
        std::mutex configuration;std::atomic<bool> configured{},stopping{};pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        endpoint_closure::call enter()noexcept
        {return same_process()&&configured.load(std::memory_order_acquire)?owned->closure.enter():endpoint_closure::call{};}
        bool cohort(token const& origin)const noexcept
        {return origin.origins[1].resource==owned->node.identity()&&origin.origins[1].authority==owned->node.authority()&&
            origin.origins[2].resource==owned->node.context()&&origin.origins[3].authority==owned->request_schema.authority&&
            origin.origins[4].authority==owned->response_schema.authority;}
        bool live()const noexcept{return same_process()&&configured.load(std::memory_order_acquire)&&!stopping.load()&&owned->node.live()&&!owned->poisoned.load();}
        static consumer caller(token const& value)noexcept{return {value.caller_instance,value.caller_generation,value.capability};}
        bool valid(token const& origin,communication::permission action,bool request=false,bool recovery=false)const noexcept
        {
            if(!(recovery?(same_process()&&configured.load()&&!stopping.load()&&owned->node.live()):live())||
                !origin.valid(action,communication::operation_family::service,owned->policy.authority,owned->queue.resource,request))return false;
            return cohort(origin);
        }
        std::uint64_t deadline()const noexcept
        {auto time=communication::now();return time>INT64_MAX-owned->policy.max_deadline_ns?0:time+owned->policy.max_deadline_ns;}
        static bool same_origin(token a,token b,bool ignore_request=false)noexcept
        {
            if(ignore_request)a.request=b.request=0;
            return a.caller_instance==b.caller_instance&&a.caller_generation==b.caller_generation&&a.authority==b.authority&&
                a.capability==b.capability&&a.resource==b.resource&&a.request==b.request&&a.family==b.family&&a.action==b.action&&
                a.secondary==b.secondary&&a.session==b.session&&a.sequence==b.sequence&&a.watch==b.watch&&a.count==b.count&&
                a.origins==b.origins&&a.retained_origins==b.retained_origins;
        }
        error finish(mutations& queue,retained& pending,bool abandon)noexcept
        {
            if(!pending.origin.request||pending.ready)return error::again;
            std::array<std::uint8_t,160> body;std::size_t size{};token original;result actual;
            auto status=queue.completion(original,actual,body,size);bool malformed=status==error::too_large;
            if(malformed)status=queue.completion(original,actual);
            if(status!=error::ok)return status;
            handle native{};
            if(malformed||!same_origin(original,pending.origin)||!mutation_evidence({body.data(),size},actual,native))
                actual={error::io,effect::unknown,pending.origin.request,owned->queue.provider_generation,false};
            auto who=caller(pending.origin);
            auto mapped=abandon?owned->mapping->abandoned(who,pending.correlation,actual):
                pending.origin.action==communication::permission::service_submit?
                    owned->mapping->submitted(who,pending.correlation,actual,actual.status==error::ok?native:0):
                    owned->mapping->responded(who,pending.correlation,actual);
            if(mapped!=error::ok)actual={error::io,effect::unknown,pending.origin.request,owned->queue.provider_generation,false};
            if(actual.observed==effect::unknown)owned->poisoned=true;
            pending.outcome=actual;pending.ready=true;
            // Cancellation has its own cached public recovery receipt. Its
            // queue may be released only after known local completion is saved.
            if(abandon&&actual.observed!=effect::unknown)
            {
                if(actual.status==error::ok&&owned->mapping->release(who,pending.correlation)!=error::ok)
                {pending.outcome={error::io,effect::unknown,pending.origin.request,owned->queue.provider_generation,false};owned->poisoned=true;return error::ok;}
                pending.released=queue.release(original)==error::ok;
            }
            return error::ok;
        }
    public:
        service_endpoint()=default;service_endpoint(service_endpoint const&)=delete;service_endpoint& operator=(service_endpoint const&)=delete;
        ~service_endpoint()override{if(!same_process()){std::construct_at(std::addressof(owned));return;}drain();}
        /** @brief Compute all dynamic queue payload and mapping reservations safely.
         * @details Returns zero for invalid/disabled inputs. Object/control-block
         * overhead is bounded by one endpoint per immutable configured plan and
         * the enclosing deployment/process memory reservation.
         */
        static std::uint64_t reservation_bytes(communication::service_plan const& policy,std::uint64_t request_bytes,std::uint64_t reply_bytes)noexcept
        {
            if(!request_bytes||request_bytes>4u*1024u*1024u||reply_bytes<160||reply_bytes>4u*1024u*1024u||
                !policy.max_request_bytes||policy.max_request_bytes>4u*1024u*1024u||!policy.max_response_bytes||policy.max_response_bytes>4u*1024u*1024u||
                !correlations::reservation_bytes(policy.pending))return 0;
            auto payload=policy.direction==communication::service_role::client?policy.max_response_bytes:policy.max_request_bytes;
            return 2*(2*request_bytes+reply_bytes)+2*(request_bytes+reply_bytes)+request_bytes+3*reply_bytes+payload+
                correlations::reservation_bytes(policy.pending)+endpoint_closure::reserved_bytes;
        }
        /** @brief Freeze actual node, exact schemas and verified native endpoint.
         * @details This creates no worker/thread, sends no message and charges
         * no grant. Copying all plans occurs inside the recoverable allocation
         * boundary. Failed preparation leaves the facade unpublished; native
         * creation cleanup responsibility remains with the owning factory.
         */
        error configure(communication::service_plan const& policy,communication::type_plan const& request_schema,
                        communication::type_plan const& response_schema,communication::node_lease node,communication_jobs::plan queue,
                        handle native_endpoint,service_capacity capacity,communication_jobs::admit admit,communication_jobs::charge charge,void* context,
                        communication_workspace::lease* reservation=nullptr,std::shared_ptr<void> authority_owner={})noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{configuration,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(configured.load()||stopping.load())return error::denied;
            if(authority_owner&&authority_owner.get()!=context)return error::invalid;
            auto needed=reservation_bytes(policy,capacity.request_bytes,capacity.reply_bytes);
            if(!node.live()||!policy.authority||policy.authority.environment!=node.authority().environment||
                !communication::valid(policy.selected)||!communication::valid(policy.node)||!communication::valid(policy.quality)||
                !communication::absolute_name(policy.resolved_name)||policy.service_type.empty()||policy.direction>communication::service_role::server||
                !policy.max_deadline_ns||policy.max_deadline_ns>INT64_MAX||!needed||!native_endpoint||!admit||!charge||
                queue.authority!=policy.authority||queue.family!=communication::operation_family::service||!queue.resource||!queue.binding||
                !queue.provider_generation||queue.slots!=1||queue.max_deadline_ns!=policy.max_deadline_ns||
                queue.request_bytes!=capacity.request_bytes||queue.reply_bytes!=capacity.reply_bytes||queue.reserved_bytes!=capacity.reserved_bytes)return error::invalid;
            auto schema_valid=[&](communication::type_plan const& schema,communication::profile const& expected)
            {return schema.selected==expected&&communication::valid(schema.selected)&&schema.authority&&schema.authority.environment==policy.authority.environment&&
                !schema.revoked&&schema.validate&&!schema.type_identity.empty()&&!schema.encoding.empty();};
            if(!schema_valid(request_schema,policy.request_type)||!schema_valid(response_schema,policy.response_type))return error::denied;
            auto sending=policy.direction==communication::service_role::client?policy.max_request_bytes:policy.max_response_bytes;
            if(capacity.reserved_bytes<needed||capacity.request_bytes<sending+24)return error::too_large;
            try
            {
                auto pending=::uwvm2::utils::container::make_unique<state>();
                pending->authority_owner=std::move(authority_owner);
                auto mapping=::uwvm2::utils::container::make_shared<correlations>();bool client=policy.direction==communication::service_role::client;
                auto status=mapping->configure({policy.authority,queue.resource,native_endpoint,queue.provider_generation,
                    client?correlations::role::client:correlations::role::server,policy.pending});if(status!=error::ok)return status;
                auto part=queue;part.reserved_bytes=2*capacity.request_bytes+capacity.reply_bytes;
                status=pending->effects.configure(part,admit,charge,context);if(status!=error::ok)return status;
                status=pending->cancellations.configure(part,admit,charge,context);if(status!=error::ok)return status;
                part.reserved_bytes=2*(capacity.request_bytes+capacity.reply_bytes);
                status=pending->observations.configure(part,admit,charge,context);if(status!=error::ok)return status;
                auto const& received=client?response_schema:request_schema;auto payload=client?policy.max_response_bytes:policy.max_request_bytes;
                part.reserved_bytes=capacity.request_bytes+3*capacity.reply_bytes+payload;
                status=pending->receiver.configure(part,{native_endpoint,queue.provider_generation,payload,
                    client?receive_direction::service_response:receive_direction::service_request,received.type_identity,received.encoding,received.validate},
                    admit,charge,context,mapping);if(status!=error::ok)return status;
                status=pending->closure.configure(queue,native_endpoint,admit,charge,context);if(status!=error::ok)return status;
                pending->queue=queue;pending->policy=policy;pending->request_schema=request_schema;pending->response_schema=response_schema;
                pending->node=std::move(node);pending->mapping=std::move(mapping);
                if(reservation)pending->reservation=std::move(*reservation);
                owned=std::move(pending);
                configured.store(true,std::memory_order_release);return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::invalid;}
        }
        bool queued_mutations()const noexcept override{return true;}
        bool queued_receives()const noexcept override{return true;}
        bool queued_observations()const noexcept override{return true;}
        handle resource()const noexcept{return same_process()&&configured.load()?owned->queue.resource:0;}
        bool matches(communication::service_plan const& policy,handle resource,communication::native_node const* parent)const noexcept
        {return live()&&owned->queue.resource==resource&&owned->policy.authority==policy.authority&&owned->policy.selected==policy.selected&&owned->node.get()==parent;}
        communication::endpoint_identity identity()const noexcept override
        {communication::endpoint_identity value;if(same_process()&&configured.load())value.provider_generation=owned->queue.provider_generation;return value;}
        result submit(handle,wire::bytes,std::uint64_t)noexcept override{return {error::denied,effect::none};}
        result respond(handle,wire::bytes,std::uint64_t)noexcept override{return {error::denied,effect::none};}
        error take(void*,communication::service_sink)noexcept override{return error::denied;}
        error available(bool&)noexcept override{return error::denied;}
        error inspect(communication::qos&)noexcept override{return error::denied;}
        result abandon(handle)noexcept override{return {error::denied,effect::none};}
        result submit(handle request,wire::bytes body,std::uint64_t until,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            if(!valid(origin,communication::permission::service_submit,true)||request!=origin.request||owned->policy.direction!=communication::service_role::client)
                return {error::denied,effect::none};
            if(body.size()>owned->policy.max_request_bytes||!owned->request_schema.validate(body))return {error::invalid,effect::none};
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||owned->pending.origin.request)return {error::again,effect::none};
            auto who=caller(origin);auto status=owned->mapping->reserve_client(who,request);if(status!=error::ok)return {status,effect::none};
            auto accepted=owned->effects.service_submit(origin,body,until);
            if(!accepted.deferred)
            {if(owned->mapping->abandon_client(who,request)!=error::ok)owned->poisoned=true;return accepted;}
            owned->pending={origin,request,{},{},{}};return accepted;
        }
        result respond(handle correlation,wire::bytes body,std::uint64_t until,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            if(!valid(origin,communication::permission::service_respond,true)||owned->policy.direction!=communication::service_role::server)return {error::denied,effect::none};
            if(body.size()>owned->policy.max_response_bytes||!owned->response_schema.validate(body))return {error::invalid,effect::none};
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||owned->pending.origin.request)return {error::again,effect::none};
            auto who=caller(origin);handle native{};auto status=owned->mapping->response_target(who,correlation,native);
            if(status!=error::ok)return {status,effect::none};
            status=owned->mapping->response_pending(who,correlation,origin.request);if(status!=error::ok)return {status,effect::none};
            auto accepted=owned->effects.service_respond(origin,native,body,until);
            if(!accepted.deferred)
            {if(owned->mapping->responded(who,correlation,{accepted.status,effect::none,origin.request,owned->queue.provider_generation,false})!=error::ok)owned->poisoned=true;return accepted;}
            owned->pending={origin,correlation,{},{},{}};return accepted;
        }
        error reserve(consumer who,std::uint64_t until,void* context,communication::service_reservation_sink sink,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return error::cancelled;
            if(!sink||!valid(origin,communication::permission::service_reserve))return error::denied;
            return owned->receiver.reserve(who,until,origin,[&](auto reference,auto deadline,auto const& record){return sink(context,reference,deadline,record);});
        }
        error settle(consumer who,communication::delivery_reference ref,communication::request_key key,bool acknowledge,
                     communication::delivery_receipt& output,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return error::cancelled;
            if(!same_process()||!configured.load()||(acknowledge&&!valid(origin,communication::permission::service_acknowledge)))return error::denied;
            return owned->receiver.settle(who,ref,key,acknowledge,output,origin);
        }
        error available(bool& output,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return error::cancelled;
            if(!valid(origin,communication::permission::service_available)||origin.request||owned->policy.direction!=communication::service_role::client)return error::denied;
            auto until=deadline();if(!until)return error::too_large;
            return owned->observations.service_available(origin,until,[&](wire::bytes bytes)
            {wire::reader input{bytes};bool value{};if(!input.boolean(value)||!input.done())return error::invalid;output=value;return error::ok;});
        }
        /** @brief Read the worker's verified equal request/response QoS profile.
         * @details The worker rejects unequal actual legs rather than choosing
         * one. This collapsed observation is not used to fabricate independently
         * cached channel evidence or a native GID in service.describe.
         */
        error inspect(communication::qos& output,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return error::cancelled;
            if(!valid(origin,communication::permission::qos_describe)||origin.request)return error::denied;
            auto until=deadline();if(!until)return error::too_large;
            return owned->observations.service_qos(origin,until,[&](wire::bytes bytes)
            {
                wire::reader input{bytes};communication::qos value;std::uint64_t small{};
                for(auto* field:{&value.history,&value.reliability,&value.durability,&value.liveliness})
                {if(!input.u64(small)||small>UINT32_MAX)return error::invalid;*field=static_cast<std::uint32_t>(small);}
                if(!input.u64(value.depth))return error::invalid;
                for(auto* duration:{&value.deadline,&value.lifespan,&value.lease})if(!input.u64(duration->seconds)||!input.u64(duration->nanoseconds))return error::invalid;
                wire::bytes gid;if(!communication::valid(value)||!input.blob(gid)||!gid.empty()||!input.done())return error::invalid;
                output=value;return error::ok;
            });
        }
        /** @brief Cancel only local interest using independent reserved capacity.
         * @details Exact origin/correlation retries recover the retained result
         * even after the cancellation budget is exhausted. A competing request
         * cannot replace an in-flight or unknown result. Remote execution may
         * still occur after known local abandonment; no new submit is inferred.
         */
        result abandon(handle correlation,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            if(!valid(origin,communication::permission::service_abandon,true,true)||owned->policy.direction!=communication::service_role::client||origin.session||origin.sequence)
                return {error::denied,effect::none};
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock())return {error::again,effect::none};
            auto& pending=owned->abandonment;
            if(pending.origin.request&&pending.correlation==correlation&&same_origin(pending.origin,origin,true))
                return pending.ready?pending.outcome:result{error::again,effect::none};
            if(pending.origin.request&&!pending.released)return {error::again,effect::none};
            if(owned->poisoned.load())return {error::cancelled,effect::none};
            auto until=deadline();if(!until)return {error::too_large,effect::none};
            handle native{};auto who=caller(origin);auto status=owned->mapping->abandon_pending(who,correlation,origin.request,native);
            if(status!=error::ok)return {status,effect::none};
            auto accepted=owned->cancellations.service_abandon(origin,native,until);
            if(!accepted.deferred)
            {if(owned->mapping->abandoned(who,correlation,{accepted.status,effect::none,origin.request,owned->queue.provider_generation,false})!=error::ok)owned->poisoned=true;return accepted;}
            pending={origin,correlation,{},{},{}};return {error::again,effect::none};
        }
        /** @brief Pump bounded queues and translate mutation evidence before receive delivery. */
        error pump(communication_worker_async::transport& transport)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            bool progress{};
            for(unsigned i{};i!=5;++i)
            {
                auto status=i==0?owned->effects.pump(transport):i==1?owned->cancellations.pump(transport):
                    i==2?owned->observations.pump(transport):i==3?owned->receiver.pump(transport):owned->closure.pump(transport);
                if(status==error::ok)progress=true;else if(status!=error::again&&status!=error::busy)return status;
                if(i<2)
                {
                    std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock())continue;
                    status=i==0?finish(owned->effects,owned->pending,false):finish(owned->cancellations,owned->abandonment,true);
                    if(status==error::ok)progress=true;else if(status!=error::again)return status;
                }
            }
            return progress?error::ok:error::again;
        }
        /** @brief Recover completion until the host registry accepts the exact result. */
        error completion(token& origin,result& output)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock()||!owned->pending.ready)return error::again;
            origin=owned->pending.origin;output=owned->pending.outcome;return error::ok;
        }
        /** @brief Release known queue evidence after host settlement; unknown effects stay owned. */
        error release_completion(token const& origin)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            std::unique_lock lock{owned->operation_mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto& pending=owned->pending;if(!pending.ready||!same_origin(origin,pending.origin))return error::denied;
            if(pending.outcome.observed==effect::unknown||owned->poisoned.load())return error::busy;
            auto status=owned->effects.release(origin);if(status!=error::ok)return status;
            bool reclaim=origin.action==communication::permission::service_submit?pending.outcome.status!=error::ok:pending.outcome.status==error::ok;
            if(reclaim&&owned->mapping->release(caller(origin),pending.correlation)!=error::ok)
            {owned->poisoned=true;return error::io;}
            pending={};return error::ok;
        }
        /** @brief Admit exact destruction only when no service correlation or receipt is occupied.
         * @details Closing never abandons a pending remote command implicitly.
         * Independent local abandonment or receive settlement must run first.
         * Closed parents and traffic attenuation cannot block explicit cleanup.
         */
        error close(token const& origin)noexcept override
        {
            if(!same_process()||!configured.load()||!origin.valid(communication::permission::resource_close,communication::operation_family::service,
                owned->policy.authority,owned->queue.resource,false)||!cohort(origin))return error::denied;
            return owned->closure.close(origin,[&]()noexcept
            {return !owned->poisoned.load()&&!owned->effects.occupied()&&!owned->cancellations.occupied()&&!owned->observations.occupied()&&
                !owned->receiver.occupied()&&!owned->mapping->occupied();});
        }
        bool cleanup_confirmed()const noexcept override
        {return same_process()&&configured.load()&&owned->closure.cleanup_confirmed();}
        bool occupied()noexcept override
        {return !same_process()||!configured.load()||owned->poisoned.load()||owned->effects.occupied()||owned->cancellations.occupied()||
            owned->observations.occupied()||owned->receiver.occupied()||owned->mapping->occupied()||owned->closure.occupied();}
        void request_stop()noexcept
        {
            if(!same_process())return;
            stopping=true;if(!configured.load())return;
            owned->effects.request_stop();owned->cancellations.request_stop();owned->observations.request_stop();owned->receiver.request_stop();owned->mapping->retire();owned->closure.request_stop();
        }
        /// @brief Nonblocking authority fence; never wait for a pump needing that authority.
        void retire()noexcept override{request_stop();}
        /// @brief Drain supervisor users outside authority/registry locks before callback teardown.
        void drain()noexcept
        {
            if(!same_process())return;
            request_stop();if(!configured.load())return;
            owned->effects.retire();owned->cancellations.retire();owned->observations.retire();owned->receiver.retire();owned->closure.retire();
            std::lock_guard lock{owned->operation_mutex};
        }
    };
#endif
}
