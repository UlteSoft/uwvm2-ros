/** @file
 * @brief Owned, nonblocking topic/service reservations across an isolated worker.
 * @details One exact native endpoint reserves a complete request slot, reply slot,
 * reconciliation buffer and retained record before any authority charge. A
 * trusted supervisor pumps the shared private transport. Guest-facing calls
 * never wait for a worker, and no native identifier is a guest resource handle.
 *
 * reserve() pins one original native record. A failed trusted decode/copy keeps
 * the same reference and payload; another caller or capability cannot take it.
 * acknowledge consumes that record, while release only returns it. Each has a
 * separate permission. Release uses an explicit cleanup grant, independent of
 * traffic attenuation and parent liveness, but cannot disclose retained data.
 *
 * The host settlement key and the native settlement key are separate. A known
 * no-effect failure consumes a host receipt key without advancing the native
 * key. Exact retries return the same receipt. A lost or contradictory reply is
 * unknown and fences the endpoint; teardown never relabels it as no-effect.
 * Storage uses recoverable fast_io allocation. No hidden thread, process,
 * unbounded history, automatic rate limit or default permission is introduced.
 */
#pragma once
#include "communication_jobs.h"
#include "communication_receive_reply.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class receives
    {
        using error=communication::error;
        using effect=communication::effect;
        using token=communication::operation_token;
        using permission=communication::permission;
        using operation=communication_worker_protocol::operation;
        enum class phase:std::uint8_t{idle,reserving,borrowed,settling,uncertain};
        communication_jobs::queue jobs;
        communication_jobs::plan limits;
        communication_jobs::admit admission{};
        communication_jobs::charge charging{};
        void* context{};
        receive_schema schema;
        ::uwvm2::utils::container::std_string type_name,encoding_name;
        ::uwvm2::utils::container::std_vector<std::uint8_t> scratch,retained;
        std::size_t retained_size{};
        token consumer,pending;
        std::uint64_t consumer_id{},deadline{},native_sequence{};
        receive_wire::reference reference,last_reference;
        communication::request_key pending_key;
        communication::receipt last;
        bool pending_ack{},last_ack{},configured{},poisoned{};
        phase current{};
        std::atomic<bool> stopping{};
        mutable std::mutex mutex;
        pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        static bool same_consumer(token const& a,token const& b)noexcept
        {
            if(a.caller_instance!=b.caller_instance||a.caller_generation!=b.caller_generation||a.authority!=b.authority||
                a.capability!=b.capability||a.resource!=b.resource||a.family!=b.family||a.count!=b.count||
                a.retained_origins!=b.retained_origins||a.secondary!=b.secondary||a.watch!=b.watch)return false;
            for(std::size_t i{1};i<a.origins.size();++i)if(a.origins[i]!=b.origins[i])return false;
            return true;
        }
        bool topic()const noexcept{return limits.family==communication::operation_family::topic;}
        bool valid(token const& value,permission expected)const noexcept
        {
            return !value.request&&!value.session&&!value.sequence&&value.valid(expected,limits.family,limits.authority,limits.resource,false)&&
                (!consumer.caller_instance||same_consumer(value,consumer));
        }
        error poison(error reason=error::io)noexcept
        {
            poisoned=true;current=phase::uncertain;jobs.request_stop();
            if(pending_key.sequence)
            {last={pending.request,limits.provider_generation,pending_key,reason,effect::unknown};last_reference=reference;last_ack=pending_ack;}
            return reason;
        }
        /** @brief Reconcile owned native evidence without granting access to its payload. */
        error harvest()noexcept
        {
            if(!pending.request)return error::again;
            token completed;communication::native_result result;std::size_t bytes{};
            auto status=jobs.completion(completed,result,scratch,bytes);if(status!=error::ok)return status;
            if(completed.request!=pending.request||!same_consumer(completed,pending)||completed.action!=pending.action||
                completed.session!=pending.session||completed.sequence!=pending.sequence||result.request_identity!=pending.request||
                result.provider_generation!=limits.provider_generation||!communication::known_status(result.status)||result.observed>effect::unknown)
                return poison();
            if(result.observed==effect::unknown)return poison(result.status==error::ok?error::io:result.status);
            if(current==phase::reserving)
            {
                // Reserving has no acknowledgement effect. A malformed response
                // may nevertheless have pinned a native record, so retain debt.
                if(result.observed!=effect::none)return poison();
                if(result.status==error::ok)
                {
                    received_record decoded;
                    if(!decode_received({scratch.data(),bytes},schema,deadline,decoded))return poison();
                    std::memcpy(retained.data(),scratch.data(),bytes);retained_size=bytes;
                    reference={limits.resource,limits.provider_generation,decoded.native_reference.sequence,decoded.native_reference.nonce};
                    current=phase::borrowed;
                }
                else current=phase::idle;
            }
            else if(current==phase::settling)
            {
                std::uint64_t native_receipt{};
                if(result.status==error::ok)
                {
                    if(result.observed!=effect::locally_accepted||
                        !decode_receive_settlement({scratch.data(),bytes},limits.provider_generation,result.status,result.observed,native_receipt))return poison();
                    ++native_sequence;current=phase::idle;retained_size=0;
                }
                else
                {
                    if(result.observed!=effect::none||bytes)return poison();
                    current=phase::borrowed;
                }
                last={pending.request,limits.provider_generation,pending_key,result.status,result.observed};
                last_reference=reference;last_ack=pending_ack;
            }
            else return poison();
            status=jobs.release(pending,pending.request);if(status!=error::ok)return poison(status);
            pending={};pending_key={};return result.status;
        }
        template<typename Decode> error deliver(token const& origin,Decode&& decode)noexcept
        {
            if(communication::now()>=deadline)return error::timeout;
            group_lease held;auto status=admission(context,origin,held);if(status!=error::ok)return status;
            received_record record;
            if(!decode_received({retained.data(),retained_size},schema,deadline,record))return poison();
            try{return decode(reference,record);}
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
            catch(...){return error::io;}
        }
    public:
        receives()=default;receives(receives const&)=delete;receives& operator=(receives const&)=delete;
        ~receives()
        {
            if(!same_process())
            {
                std::construct_at(std::addressof(type_name));std::construct_at(std::addressof(encoding_name));
                std::construct_at(std::addressof(scratch));std::construct_at(std::addressof(retained));
                std::construct_at(std::addressof(consumer));std::construct_at(std::addressof(pending));return;
            }
            retire();
        }
        /** @brief Reserve one immutable endpoint and all complete reconciliation storage.
         * @details slots must equal one; reserved_bytes covers request_bytes plus
         * three reply_bytes. The schema is copied with recoverable allocators.
         * native_endpoint and provider_generation must come from an independently
         * verified creation receipt. Configuration performs no I/O or admission.
         */
        error configure(communication_jobs::plan plan,receive_schema selected,communication_jobs::admit validate,
                        communication_jobs::charge charge,void* user)noexcept
        {
            if(!same_process())return error::denied;
            // Factories can hold registry locks. Do not wait for a receiver
            // whose fresh admission callback may acquire those same locks.
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(configured||stopping.load())return error::denied;
            if(plan.slots!=1||plan.request_bytes<32||plan.request_bytes>4u*1024u*1024u||plan.reply_bytes>4u*1024u*1024u||
                !selected.native_endpoint||!selected.provider_generation||selected.provider_generation!=plan.provider_generation||
                !selected.payload_bytes||selected.payload_bytes>4u*1024u*1024u||!selected.validate||
                selected.direction>receive_direction::service_response||selected.type.empty()||selected.type.size()>255||
                selected.encoding.empty()||selected.encoding.size()>255||!validate||!charge)return error::invalid;
            if(plan.family!=(selected.direction==receive_direction::topic?communication::operation_family::topic:communication::operation_family::service))return error::invalid;
            if(plan.reply_bytes<selected.payload_bytes+selected.type.size()+selected.encoding.size()+256||
                plan.reserved_bytes<plan.request_bytes+3*plan.reply_bytes)return error::too_large;
            try
            {
                ::uwvm2::utils::container::std_string type{selected.type},encoding{selected.encoding};
                ::uwvm2::utils::container::std_vector<std::uint8_t> temporary(plan.reply_bytes),record(plan.reply_bytes);
                auto id=resources::allocate_identity();if(!id)return error::too_large;
                auto queued=plan;queued.reserved_bytes-=2*plan.reply_bytes;
                auto status=jobs.configure(queued,validate,user);if(status!=error::ok)return status;
                type_name=std::move(type);encoding_name=std::move(encoding);scratch=std::move(temporary);retained=std::move(record);
                schema=selected;schema.type=type_name;schema.encoding=encoding_name;limits=plan;
                admission=validate;charging=charge;context=user;consumer_id=id;configured=true;return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::invalid;}
        }
        /** @brief Retain/recover one record without acknowledging it or charging a retry.
         * @details Decode is trusted, bounded host bookkeeping only: no guest
         * callback, native I/O or retained borrowed spans. It receives an explicit
         * host reference and separately labelled native evidence. The later VM
         * delivery must revalidate authority and map native service correlations.
         */
        template<typename Decode> error reserve(token const& origin,std::uint64_t until,Decode&& decode)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!configured||stopping.load()||poisoned)return error::cancelled;
            if(!valid(origin,topic()?permission::topic_reserve:permission::service_reserve))return error::denied;
            if(current==phase::settling)return error::busy;
            if(current!=phase::idle&&until!=deadline)return error::stale;
            if(current==phase::reserving)
            {auto status=harvest();if(status!=error::ok)return status;}
            if(current==phase::borrowed)return deliver(origin,std::forward<Decode>(decode));
            if(current!=phase::idle)return error::cancelled;
            auto request=resources::allocate_identity();if(!request)return error::too_large;
            auto candidate=origin.with_request(request);std::array<std::uint8_t,16> bytes;wire::writer writer{bytes};
            writer.u64(consumer_id);writer.u64(until);communication_jobs::receipt result;
            auto status=jobs.enqueue(candidate,topic()?operation::topic_reserve:operation::service_reserve,writer.written(),until,charging,context,result);
            if(status!=error::ok)return status;
            if(!consumer.caller_instance)consumer=origin;
            pending=std::move(candidate);deadline=until;current=phase::reserving;return error::again;
        }
        /** @brief Submit/recover an exact ACK or RELEASE with a separate host receipt key.
         * @details The caller preflights its full receipt output before calling.
         * The cleanup path uses the same preallocated IPC slot and zero traffic
         * charge. It needs a fresh finite IPC deadline after record expiry. ACK
         * cannot use that deadline to extend the original reservation lifetime.
         */
        error settle(token const& origin,receive_wire::reference selected,communication::request_key key,bool acknowledge,
                     std::uint64_t ipc_deadline,communication::receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            auto action=topic()?(acknowledge?permission::topic_acknowledge:permission::topic_release):
                (acknowledge?permission::service_acknowledge:permission::service_release);
            if(!configured||!valid(origin,action)||key.session!=limits.resource||!key.sequence)return error::denied;
            if(last.request&&key==last.key)
            {
                if(selected!=last_reference||acknowledge!=last_ack)return error::stale;
                group_lease held;auto status=admission(context,origin,held);if(status!=error::ok)return status;
                output=last;return error::ok;
            }
            if(stopping.load()||poisoned)return error::cancelled;
            if(current==phase::settling)
            {
                if(key!=pending_key||selected!=reference||acknowledge!=pending_ack)return error::stale;
                auto status=harvest();
                if(last.request&&last.key==key)
                {
                    group_lease held;auto allowed=admission(context,origin,held);if(allowed!=error::ok)return allowed;
                    output=last;return error::ok;
                }
                return status;
            }
            if(current!=phase::borrowed||selected!=reference)return error::denied;
            if(last.key.sequence==UINT64_MAX||key.sequence!=last.key.sequence+1||native_sequence==UINT64_MAX)return error::stale;
            if(acknowledge&&communication::now()>=deadline)return error::timeout;
            auto request=resources::allocate_identity();if(!request)return error::too_large;
            auto candidate=origin.with_request(request,key.session,key.sequence);
            std::array<std::uint8_t,32> bytes;wire::writer writer{bytes};
            writer.u64(consumer_id);writer.u64(native_sequence+1);writer.u64(reference.sequence);writer.u64(reference.nonce);
            auto rpc=topic()?(acknowledge?operation::topic_acknowledge:operation::topic_release):
                (acknowledge?operation::service_acknowledge:operation::service_release);
            communication_jobs::receipt result;
            auto status=jobs.enqueue(candidate,rpc,writer.written(),ipc_deadline,charging,context,result);if(status!=error::ok)return status;
            pending=std::move(candidate);pending_key=key;pending_ack=acknowledge;current=phase::settling;return error::again;
        }
        /** @brief Reconcile after revocation without publishing payloads or granting effects. */
        error reconcile()noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(poisoned)return error::cancelled;
            return harvest();
        }
        error pump(communication_worker_async::transport& transport)noexcept{return jobs.pump(transport);}
        error cancel(communication_worker_async::transport& transport,communication_jobs::receipt& result)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!pending.request)return error::bad_handle;
            return jobs.cancel(pending,pending.request,transport,result);
        }
        bool occupied()const noexcept
        {
            if(!same_process())return true;
            // Registry close/revoke may inspect occupancy while a receive is
            // validating that registry. Contention conservatively retains debt;
            // it must never wait or report the endpoint as safely disposable.
            std::unique_lock lock{mutex,std::try_to_lock};
            return !lock.owns_lock()||current!=phase::idle||jobs.occupied();
        }
        void request_stop()noexcept{if(same_process()){stopping.store(true,std::memory_order_release);jobs.request_stop();}}
        void retire()noexcept{if(same_process()){request_stop();jobs.retire();}}
    };
#endif
}
