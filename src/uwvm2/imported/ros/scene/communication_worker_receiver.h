/** @file
 * @brief Token-aware endpoint delivery over an isolated retained receive queue.
 * @details This adapter separates trusted IPC decoding from the guest-facing
 * copy. One complete payload buffer is reserved in addition to all receives
 * storage. Native source and service correlations are translated into exact
 * host endpoint identities before publication. The guest callback runs without
 * the adapter or transport mutex; reentrant delivery and settlement return again.
 *
 * Every request uses its current full operation token. The owned RPC queue is
 * the sole traffic charging point; copying an accepted record is validation-only.
 * A failed copy retains the original record. ACK/RELEASE have independent rights,
 * keys and reserved IPC capacity. Release remains possible after record expiry
 * or parent fencing, but never overrides primary capability revocation. Unknown
 * results remain occupied and cannot be converted into successful cleanup.
 *
 * Configuration performs no I/O, grant or thread creation. Recoverable fast_io
 * allocation owns every byte. A supervisor pumps this object on its existing
 * bounded transport and stops/drains it before destroying authority callbacks.
 */
#pragma once
#include "communication_receives.h"
#include "communication_correlations.h"
#include <uwvm2/utils/container/shared.h>
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class endpoint_receiver
    {
        using error=communication::error;
        using token=communication::operation_token;
        using consumer=communication::delivery_consumer;
        receives transport;
        std::shared_ptr<correlations> mapping;
        ::uwvm2::utils::container::std_vector<std::uint8_t> payload;
        communication_jobs::plan policy;
        receive_direction direction{};
        communication_jobs::admit admission{};void* context{};
        communication::metadata metadata;
        receive_wire::reference reference;
        communication::handle correlation{},last_correlation{};
        communication::receipt last;
        receive_wire::reference last_reference;consumer last_consumer;bool last_ack{};
        std::size_t size{};
        bool configured{},delivering{},poisoned{};
        std::atomic<bool> stopping{};
        mutable std::mutex mutex;
        pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        static consumer caller(token const& value)noexcept{return {value.caller_instance,value.caller_generation,value.capability};}
        bool valid(consumer who,token const& origin,communication::permission action)const noexcept
        {return who==caller(origin)&&origin.valid(action,policy.family,policy.authority,policy.resource,false)&&!origin.request&&!origin.session&&!origin.sequence;}
    public:
        endpoint_receiver()=default;endpoint_receiver(endpoint_receiver const&)=delete;endpoint_receiver& operator=(endpoint_receiver const&)=delete;
        ~endpoint_receiver()
        {
            if(!same_process())
            {std::construct_at(std::addressof(payload));std::construct_at(std::addressof(mapping));return;}
            retire();
        }
        /** @brief Reserve detached delivery storage and freeze exact creation evidence.
         * @details reserved_bytes covers request_bytes + 3*reply_bytes plus one
         * complete payload. A service requires the independently configured
         * correlation table for this exact host/native endpoint and worker.
         * Topic receivers reject a service table. Zero/missing capacity fails.
         */
        error configure(communication_jobs::plan selected,receive_schema schema,communication_jobs::admit validate,
                        communication_jobs::charge charge,void* user,std::shared_ptr<correlations> correlations={})noexcept
        {
            if(!same_process())return error::denied;
            // A factory may hold the registry while configuring. Runtime
            // admission takes registry locks in the opposite direction, so
            // configuration must never wait for an in-flight adapter operation.
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(configured||stopping.load())return error::denied;
            if(!validate||!charge||!schema.payload_bytes||schema.payload_bytes>4u*1024u*1024u||selected.reserved_bytes<schema.payload_bytes)return error::invalid;
            bool topic=schema.direction==receive_direction::topic;
            if(topic?bool(correlations):!correlations)return error::invalid;
            if(correlations&&!correlations->matches(selected.authority,selected.resource,schema.native_endpoint,selected.provider_generation,
                schema.direction==receive_direction::service_response?communication_worker::correlations::role::client:communication_worker::correlations::role::server))return error::denied;
            try
            {
                ::uwvm2::utils::container::std_vector<std::uint8_t> buffer(static_cast<std::size_t>(schema.payload_bytes));
                auto queued=selected;queued.reserved_bytes-=schema.payload_bytes;
                auto status=transport.configure(queued,schema,validate,charge,user);if(status!=error::ok)return status;
                payload=std::move(buffer);mapping=std::move(correlations);policy=selected;direction=schema.direction;
                admission=validate;context=user;configured=true;return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::invalid;}
        }
        /** @brief Deliver a validated detached record without consuming the original.
         * @details The sink is called with host identities only and no provider
         * mutex held. Its bytes remain valid only for that call. Failed/throwing
         * copies, revocation and stopping preserve the native reservation.
         */
        template<typename Sink> error reserve(consumer who,std::uint64_t deadline,token const& origin,Sink&& sink)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()||delivering)return error::again;
            if(!configured||stopping.load()||poisoned)return error::cancelled;
            bool topic=direction==receive_direction::topic;
            if(!valid(who,origin,topic?communication::permission::topic_reserve:communication::permission::service_reserve))return error::denied;
            auto status=transport.reserve(origin,deadline,[&](receive_wire::reference selected,received_record const& record)
            {
                if(record.payload.size()>payload.size())return error::too_large;
                communication::handle translated{};
                if(mapping)
                {
                    auto result=mapping->reserve_received(who,selected,record.native_reference.endpoint,record.native_correlation,translated);
                    if(result!=error::ok)return result;
                }
                if(!record.payload.empty())std::memcpy(payload.data(),record.payload.data(),record.payload.size());
                size=record.payload.size();metadata=record.native_metadata;metadata.source=selected.endpoint;metadata.generation=selected.provider_generation;
                reference=selected;correlation=translated;return error::ok;
            });
            if(status!=error::ok)return status;
            communication::delivery_reference public_ref{reference.endpoint,reference.provider_generation,reference.sequence,reference.nonce};
            communication::service_delivery value{metadata,correlation,direction==receive_direction::service_response,{payload.data(),size}};
            delivering=true;lock.unlock();
            group_lease held;status=admission(context,origin,held);
            if(status==error::ok)
            {
                if(stopping.load())status=error::cancelled;
                else try{if(!sink(public_ref,deadline,value))status=error::io;}
                    catch(std::bad_alloc const&){status=error::no_memory;}catch(...){status=error::io;}
                if(status==error::ok&&stopping.load())status=error::cancelled;
            }
            held.release();
            if(status==error::ok){group_lease current;status=admission(context,origin,current);}
            lock.lock();delivering=false;return status;
        }
        /** @brief Recover exact asynchronous settlement evidence without a second debit.
         * @details Fresh finite IPC time does not extend the original ACK
         * deadline. Client correlation capacity is reclaimed only after a known
         * successful ACK; the exact settlement receipt survives that reclamation.
         * Server correlations remain until their independently authorized reply.
         */
        error settle(consumer who,communication::delivery_reference selected,communication::request_key key,bool acknowledge,
                     communication::delivery_receipt& output,token const& origin)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock()||delivering)return error::again;
            bool topic=direction==receive_direction::topic;
            auto action=topic?(acknowledge?communication::permission::topic_acknowledge:communication::permission::topic_release):
                (acknowledge?communication::permission::service_acknowledge:communication::permission::service_release);
            if(!configured||!valid(who,origin,action))return error::denied;
            if(poisoned)
            {
                receive_wire::reference ref{selected.source,selected.generation,selected.sequence,selected.nonce};
                if(!last.request||last.key!=key||last_reference!=ref||last_consumer!=who||last_ack!=acknowledge)return error::io;
                group_lease current;auto status=admission(context,origin,current);if(status!=error::ok)return status;
                output={last.request,last.provider_generation,last.key,last.status,last.observed};return error::ok;
            }
            auto now=communication::now();if(now>INT64_MAX-policy.max_deadline_ns)return error::too_large;
            receive_wire::reference ref{selected.source,selected.generation,selected.sequence,selected.nonce};communication::receipt receipt;
            auto status=transport.settle(origin,ref,key,acknowledge,now+policy.max_deadline_ns,receipt);if(status!=error::ok)return status;
            if(mapping)
            {
                auto translated=last.request&&last.key==key?last_correlation:correlation;
                status=mapping->settled(who,translated,ref,acknowledge,receipt);
                if(status==error::ok&&(!last.request||last.key!=key)&&acknowledge&&receipt.status==error::ok&&
                    direction==receive_direction::service_response)status=mapping->release(who,translated);
                if(status!=error::ok)
                {poisoned=true;transport.request_stop();receipt.status=error::io;receipt.observed=communication::effect::unknown;}
                last_correlation=translated;
            }
            last=receipt;last_reference=ref;last_consumer=who;last_ack=acknowledge;
            output={receipt.request,receipt.provider_generation,receipt.key,receipt.status,receipt.observed};return error::ok;
        }
        error pump(communication_worker_async::transport& selected)noexcept{return transport.pump(selected);}
        error cancel(communication_worker_async::transport& selected,communication_jobs::receipt& result)noexcept{return transport.cancel(selected,result);}
        bool occupied()const noexcept
        {
            if(!same_process())return true;
            std::unique_lock lock{mutex,std::try_to_lock};return !lock.owns_lock()||delivering||poisoned||transport.occupied();
        }
        void request_stop()noexcept{if(same_process()){stopping.store(true,std::memory_order_release);transport.request_stop();}}
        void retire()noexcept{if(same_process()){request_stop();transport.retire();}}
    };
#endif
}
