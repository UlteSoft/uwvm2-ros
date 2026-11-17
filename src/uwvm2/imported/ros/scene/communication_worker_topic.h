/** @file
 * @brief Typed topic facade for a verified endpoint in an isolated ROS worker.
 * @details Host and native identities come from separate creation records. A
 * counted live node, immutable schema and bounded queues must be supplied before
 * adoption. Every call carries its own complete authority token. No permission
 * is inherited from the opener, and tokenless native entry points refuse calls.
 *
 * Publication, liveliness, QoS observations and reservation settlement are
 * nonblocking. The existing supervisor pumps the shared transport and reconciles
 * mutation completions with the host registry. A successful enqueue describes
 * local acceptance only. Unknown/malformed results retain their queue capacity.
 * Acknowledgement waiting owns an independent slot, so its deadline cannot
 * consume the publication receipt or the capacity reserved for receive release.
 *
 * All dynamic storage uses recoverable fast_io allocation. Configuration performs
 * no native I/O, starts no thread and charges no authority. CPU/NUMA placement,
 * process limits and positive native cleanup remain supervisor responsibilities.
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
    /** @brief Explicit aggregate reservation for one typed worker topic.
     * @details Storage covers one mutation slot plus encoder, one observation
     * slot plus detached encoder/reply, and a separate acknowledgement slot plus
     * encoder for publishers or a complete receive/settlement slot plus payload
     * for subscriptions. Zero bytes is disabled, never an unlimited allowance.
     */
    struct topic_capacity
    {std::uint64_t request_bytes{},reply_bytes{},reserved_bytes{};};

    class topic_endpoint final:public communication::native_topic
    {
        using error=communication::error;using effect=communication::effect;using token=communication::operation_token;
        using handle=communication::handle;using result=communication::native_result;
        struct state
        {
            // Owned admission context outlives all callback-bearing queues.
            std::shared_ptr<void> authority_owner;
            // Capacity also outlives every owned queue.
            communication_workspace::lease reservation;
            communication::topic_plan policy;communication::type_plan schema;communication::node_lease node;
            communication_jobs::plan queue;
            mutations effects,acknowledgements;queries observations;endpoint_receiver receiver;endpoint_closure closure;
            std::mutex acknowledgement_mutex;
            token acknowledgement;std::uint64_t acknowledgement_deadline{};result acknowledged;
            bool acknowledgement_ready{},acknowledgement_released{};
            std::atomic<bool> poisoned{};
        };
        ::uwvm2::utils::container::std_unique_ptr<state> owned;
        std::mutex configuration;
        std::atomic<bool> configured{},stopping{};
        pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        endpoint_closure::call enter()noexcept
        {return same_process()&&configured.load(std::memory_order_acquire)?owned->closure.enter():endpoint_closure::call{};}
        bool cohort(token const& origin)const noexcept
        {return origin.origins[1].resource==owned->node.identity()&&origin.origins[1].authority==owned->node.authority()&&
            origin.origins[2].resource==owned->node.context()&&origin.origins[3].authority==owned->schema.authority;}
        bool live()const noexcept{return same_process()&&configured.load(std::memory_order_acquire)&&!stopping.load()&&owned->node.live()&&!owned->poisoned.load();}
        bool valid(token const& origin,communication::permission action,bool request=false,bool recovery=false)const noexcept
        {
            if(!(recovery?(same_process()&&configured.load()&&!stopping.load()&&owned->node.live()):live())||
                !origin.valid(action,communication::operation_family::topic,owned->policy.authority,owned->queue.resource,request))return false;
            return cohort(origin);
        }
        std::uint64_t deadline()const noexcept
        {auto time=communication::now();return time>INT64_MAX-owned->policy.max_deadline_ns?0:time+owned->policy.max_deadline_ns;}
        static bool same_acknowledgement(token a,token b)noexcept
        {
            a.request=b.request=0;
            return a.caller_instance==b.caller_instance&&a.caller_generation==b.caller_generation&&a.authority==b.authority&&
                a.capability==b.capability&&a.resource==b.resource&&a.family==b.family&&a.action==b.action&&a.secondary==b.secondary&&
                a.session==b.session&&a.sequence==b.sequence&&a.watch==b.watch&&a.count==b.count&&a.origins==b.origins&&a.retained_origins==b.retained_origins;
        }
        static bool read_quality(wire::bytes bytes,communication::qos& quality,std::array<std::uint8_t,32>& gid,std::size_t& size)noexcept
        {
            wire::reader input{bytes};communication::qos decoded;std::uint64_t small{};
            for(auto* field:{&decoded.history,&decoded.reliability,&decoded.durability,&decoded.liveliness})
            {if(!input.u64(small)||small>UINT32_MAX)return false;*field=static_cast<std::uint32_t>(small);}
            if(!input.u64(decoded.depth))return false;
            for(auto* duration:{&decoded.deadline,&decoded.lifespan,&decoded.lease})
                if(!input.u64(duration->seconds)||!input.u64(duration->nanoseconds))return false;
            wire::bytes identity;
            if(!communication::valid(decoded)||!input.blob(identity)||identity.size()>gid.size()||!input.done()||
                (!identity.empty()&&std::all_of(identity.begin(),identity.end(),[](auto byte){return byte==0;})))return false;
            quality=decoded;gid={};std::copy(identity.begin(),identity.end(),gid.begin());size=identity.size();return true;
        }
        error read_completion(mutations& queue,token& origin,result& output)noexcept
        {
            std::array<std::uint8_t,160> body;std::size_t size{};
            auto status=queue.completion(origin,output,body,size);bool malformed=status==error::too_large;
            if(malformed)status=queue.completion(origin,output);
            if(status!=error::ok)return status;
            handle native{};
            if(malformed||!mutation_evidence({body.data(),size},output,native))
            {output={error::io,effect::unknown,origin.request,owned->queue.provider_generation,false};owned->poisoned=true;}
            return error::ok;
        }
    public:
        topic_endpoint()=default;topic_endpoint(topic_endpoint const&)=delete;topic_endpoint& operator=(topic_endpoint const&)=delete;
        ~topic_endpoint()override
        {if(!same_process()){std::construct_at(std::addressof(owned));return;}drain();}
        /** @brief Validate exact plans, native creation evidence and all reservations.
         * @details queue.resource is the opaque reserved HOST resource; binding
         * selects the sealed worker definition and native_endpoint is taken only
         * from its successful creation result. A failed allocation leaves this
         * facade unpublished. The caller retains native cleanup responsibility.
         */
        error configure(communication::topic_plan const& policy,communication::type_plan const& schema,communication::node_lease parent,
                        communication_jobs::plan queue,handle native_endpoint,topic_capacity capacity,
                        communication_jobs::admit admit,communication_jobs::charge charge,void* user,
                        communication_workspace::lease* reservation=nullptr,std::shared_ptr<void> authority_owner={})noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{configuration,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(configured.load()||stopping.load())return error::denied;
            if(authority_owner&&authority_owner.get()!=user)return error::invalid;
            if(!parent.live()||!policy.authority||policy.authority.environment!=parent.authority().environment||
                policy.type!=schema.selected||!schema.authority||schema.authority.environment!=policy.authority.environment||schema.revoked||!schema.validate||
                !communication::valid(policy.selected)||!communication::valid(policy.node)||!communication::valid(schema.selected)||
                !communication::absolute_name(policy.resolved_name)||!communication::valid(policy.quality)||
                (policy.direction!=communication::role::publisher&&policy.direction!=communication::role::subscription)||
                !policy.max_message_bytes||policy.max_message_bytes>4u*1024u*1024u||!policy.max_deadline_ns||policy.max_deadline_ns>INT64_MAX||
                queue.authority!=policy.authority||queue.family!=communication::operation_family::topic||!queue.resource||!queue.binding||
                !queue.provider_generation||!native_endpoint||queue.slots!=1||queue.max_deadline_ns!=policy.max_deadline_ns||!admit||!charge||
                !capacity.request_bytes||capacity.request_bytes>4u*1024u*1024u||capacity.reply_bytes<160||capacity.reply_bytes>4u*1024u*1024u||
                queue.request_bytes!=capacity.request_bytes||queue.reply_bytes!=capacity.reply_bytes)return error::invalid;
            if(policy.direction==communication::role::publisher&&capacity.request_bytes<policy.max_message_bytes+16)return error::too_large;
            auto mutation=2*capacity.request_bytes+capacity.reply_bytes;
            auto observation=2*(capacity.request_bytes+capacity.reply_bytes);
            auto receive=capacity.request_bytes+3*capacity.reply_bytes+policy.max_message_bytes;
            auto total=mutation+observation+(policy.direction==communication::role::publisher?mutation:receive)+endpoint_closure::reserved_bytes;
            if(capacity.reserved_bytes<total||queue.reserved_bytes!=capacity.reserved_bytes)return error::too_large;
            try
            {
                auto pending=::uwvm2::utils::container::make_unique<state>();
                pending->authority_owner=std::move(authority_owner);
                auto part=queue;part.reserved_bytes=mutation;
                auto status=pending->effects.configure(part,admit,charge,user);if(status!=error::ok)return status;
                part.reserved_bytes=observation;status=pending->observations.configure(part,admit,charge,user);if(status!=error::ok)return status;
                if(policy.direction==communication::role::publisher)
                {part.reserved_bytes=mutation;status=pending->acknowledgements.configure(part,admit,charge,user);}
                else
                {part.reserved_bytes=receive;status=pending->receiver.configure(part,{native_endpoint,queue.provider_generation,policy.max_message_bytes,
                    receive_direction::topic,schema.type_identity,schema.encoding,schema.validate},admit,charge,user);}
                if(status!=error::ok)return status;
                status=pending->closure.configure(queue,native_endpoint,admit,charge,user);if(status!=error::ok)return status;
                pending->queue=queue;pending->policy=policy;pending->schema=schema;pending->node=std::move(parent);
                if(reservation)pending->reservation=std::move(*reservation);
                owned=std::move(pending);configured.store(true,std::memory_order_release);return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::invalid;}
        }
        bool queued_mutations()const noexcept override{return true;}
        bool queued_receives()const noexcept override{return true;}
        bool queued_observations()const noexcept override{return true;}
        /// @brief Cached host identity for exact factory publication; never a native token.
        handle resource()const noexcept{return same_process()&&configured.load()?owned->queue.resource:0;}
        bool matches(communication::topic_plan const& policy,handle resource,communication::native_node const* parent)const noexcept
        {return live()&&owned->queue.resource==resource&&owned->policy.authority==policy.authority&&owned->policy.selected==policy.selected&&owned->node.get()==parent;}
        communication::endpoint_identity identity()const noexcept override
        {communication::endpoint_identity value;if(same_process()&&configured.load())value.provider_generation=owned->queue.provider_generation;return value;}
        result publish(wire::bytes,std::uint64_t)noexcept override{return {error::denied,effect::none};}
        result assert_liveliness()noexcept override{return {error::denied,effect::none};}
        error wait_acknowledged(std::uint64_t)noexcept override{return error::denied;}
        error inspect(communication::qos&,std::array<std::uint8_t,32>&,std::size_t&)noexcept override{return error::denied;}
        result publish(wire::bytes body,std::uint64_t until,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            if(!valid(origin,communication::permission::topic_publish,true)||owned->policy.direction!=communication::role::publisher)return {error::denied,effect::none};
            if(body.size()>owned->policy.max_message_bytes||!owned->schema.validate(body))return {error::invalid,effect::none};
            return owned->effects.publish(origin,body,until);
        }
        result assert_liveliness(token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            if(!valid(origin,communication::permission::qos_liveliness_assert,true)||owned->policy.direction!=communication::role::publisher)return {error::denied,effect::none};
            auto until=deadline();return until?owned->effects.liveliness(origin,until):result{error::too_large,effect::none};
        }
        error inspect(communication::qos& quality,std::array<std::uint8_t,32>& gid,std::size_t& size,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return error::cancelled;
            if(!valid(origin,communication::permission::qos_describe)||origin.request)return error::denied;
            auto until=deadline();if(!until)return error::too_large;
            return owned->observations.topic_qos(origin,until,[&](wire::bytes bytes){return read_quality(bytes,quality,gid,size)?error::ok:error::invalid;});
        }
        /** @brief Poll one finite acknowledgement observation using its exact deadline.
         * @details Repeated polls from the same actual origins/deadline reuse the
         * original host request and budget. A competing observation cannot replace
         * it. After completion a different deadline acknowledges the cached result.
         * A successful result reports observed_remote, never physical execution.
         */
        result wait_acknowledged(std::uint64_t until,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return {error::cancelled,effect::none};
            if(!valid(origin,communication::permission::qos_acknowledgements_wait,true,true)||owned->policy.direction!=communication::role::publisher)return {error::denied,effect::none};
            std::unique_lock lock{owned->acknowledgement_mutex,std::try_to_lock};if(!lock.owns_lock())return {error::again,effect::none};
            auto& current=owned->acknowledgement;
            if(current.request&&same_acknowledgement(current,origin)&&owned->acknowledgement_deadline==until)
                return owned->acknowledgement_ready?owned->acknowledged:result{error::again,effect::none};
            if(current.request&&!owned->acknowledgement_released)return {error::again,effect::none};
            if(owned->poisoned.load())return {error::cancelled,effect::none};
            auto accepted=owned->acknowledgements.acknowledged(origin,until);
            if(accepted.status!=error::ok||!accepted.deferred)return accepted;
            current=origin;owned->acknowledgement_deadline=until;owned->acknowledgement_ready=owned->acknowledgement_released=false;
            return {error::again,effect::none};
        }
        error reserve(communication::delivery_consumer who,std::uint64_t until,void* context,reservation_sink copy,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return error::cancelled;
            if(!copy||!valid(origin,communication::permission::topic_reserve)||owned->policy.direction!=communication::role::subscription)return error::denied;
            return owned->receiver.reserve(who,until,origin,[&](auto ref,auto deadline,communication::service_delivery const& record)
            {return copy(context,ref,deadline,communication::delivery{record.information,record.body});});
        }
        error settle(communication::delivery_consumer who,communication::delivery_reference ref,communication::request_key key,bool acknowledge,
                     communication::delivery_receipt& receipt,token const& origin)noexcept override
        {
            auto active=enter();if(!active)return error::cancelled;
            auto action=acknowledge?communication::permission::topic_acknowledge:communication::permission::topic_release;
            // Release may follow parent fencing. Its own original capability is
            // still checked by endpoint_receiver and the dispatcher callback.
            if(!same_process()||!configured.load()||owned->policy.direction!=communication::role::subscription||
                (acknowledge&&!valid(origin,action)))return error::denied;
            return owned->receiver.settle(who,ref,key,acknowledge,receipt,origin);
        }
        /** @brief Pump bounded queues outside guest/registry/authority locks. */
        error pump(communication_worker_async::transport& transport)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            bool progress{};
            for(unsigned i{};i!=4;++i)
            {
                auto status=i==3?owned->closure.pump(transport):i==0?owned->effects.pump(transport):i==1?owned->observations.pump(transport):
                    owned->policy.direction==communication::role::publisher?owned->acknowledgements.pump(transport):owned->receiver.pump(transport);
                if(status==error::ok)progress=true;
                else if(status!=error::again&&status!=error::busy)return status;
            }
            if(owned->policy.direction==communication::role::publisher)
            {
                std::unique_lock lock{owned->acknowledgement_mutex,std::try_to_lock};
                if(lock.owns_lock()&&owned->acknowledgement.request&&!owned->acknowledgement_ready)
                {
                    token completed;result actual;
                    auto status=read_completion(owned->acknowledgements,completed,actual);
                    if(status==error::ok)
                    {
                        if(!same_acknowledgement(completed,owned->acknowledgement)||completed.request!=owned->acknowledgement.request||
                            (actual.status==error::ok&&actual.observed!=effect::observed_remote))
                        {actual={error::io,effect::unknown,owned->acknowledgement.request,owned->queue.provider_generation,false};owned->poisoned=true;}
                        owned->acknowledged=actual;owned->acknowledgement_ready=true;
                        if(actual.observed!=effect::unknown)
                            owned->acknowledgement_released=owned->acknowledgements.release(completed)==error::ok;
                        progress=true;
                    }
                    else if(status!=error::again)return status;
                }
            }
            return progress?error::ok:error::again;
        }
        /** @brief Hand one validated completion to the host's retained mutation receipt.
         * @details The supervisor retains this token/result until registry
         * settlement succeeds, then calls release_completion. Neither method
         * grants guest access or infers native request IDs from host handles.
         */
        error completion(token& origin,result& output)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            return read_completion(owned->effects,origin,output);
        }
        error release_completion(token const& origin)noexcept
        {return !same_process()||!configured.load()?error::denied:owned->poisoned.load()?error::busy:owned->effects.release(origin);}
        /** @brief Close only after quiescence and verified exact native teardown.
         * @details The separately admitted close token retains exact origins but
         * needs no active parent/schema grant. The gate excludes every ordinary
         * call, including a detached delivery callback, before checking capacity.
         */
        error close(token const& origin)noexcept override
        {
            if(!same_process()||!configured.load()||!origin.valid(communication::permission::resource_close,communication::operation_family::topic,
                owned->policy.authority,owned->queue.resource,false)||!cohort(origin))return error::denied;
            return owned->closure.close(origin,[&]()noexcept
            {return !owned->poisoned.load()&&!owned->effects.occupied()&&!owned->observations.occupied()&&
                !(owned->policy.direction==communication::role::publisher?owned->acknowledgements.occupied():owned->receiver.occupied());});
        }
        bool cleanup_confirmed()const noexcept override
        {return same_process()&&configured.load()&&owned->closure.cleanup_confirmed();}
        bool occupied()noexcept override
        {return !same_process()||!configured.load()||owned->poisoned.load()||owned->effects.occupied()||owned->observations.occupied()||
            (owned->policy.direction==communication::role::publisher?owned->acknowledgements.occupied():owned->receiver.occupied())||owned->closure.occupied();}
        void request_stop()noexcept
        {
            if(!same_process())return;
            stopping=true;if(!configured.load())return;
            owned->effects.request_stop();owned->observations.request_stop();owned->acknowledgements.request_stop();owned->receiver.request_stop();owned->closure.request_stop();
        }
        /** @brief Drain only outside authority and registry locks before destruction. */
        void drain()noexcept
        {if(!same_process())return;request_stop();if(!configured.load())return;
            owned->effects.retire();owned->observations.retire();owned->acknowledgements.retire();owned->receiver.retire();owned->closure.retire();}
    };
#endif
}
