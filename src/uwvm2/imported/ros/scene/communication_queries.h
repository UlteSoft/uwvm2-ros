/** @file
 * @brief Bounded nonblocking observations through an isolated communication worker.
 * @details One immutable endpoint reserves one query slot, a complete encoder,
 * and a detached reply buffer before use. The actual caller and every original
 * node/context/schema capability travel with each request. A query returns again
 * while the supervisor pumps the private transport; no guest call waits for IPC.
 * An identical pending query resumes without charging twice. A different caller,
 * capability, operation or body cannot consume or replace its result.
 *
 * Native replies are untrusted. A fresh complete authority group protects the
 * bounded copy into owned storage, then a trusted decoder validates that storage.
 * Recoverable allocation failures retain the same complete reply. A decoder may
 * not perform guest I/O, native effects or retain the borrowed byte span. Unknown
 * results and malformed replies retain occupied capacity until explicit teardown;
 * they never become evidence of native completion. A successful query decode is
 * local receipt consumption, not an acknowledgement of a robot effect.
 *
 * No process or thread is owned here. The supervisor must stop/drain this object
 * before destroying its borrowed callbacks or transport. Storage uses recoverable
 * fast_io allocators. Zero capacity or missing callbacks grants no functionality.
 */
#pragma once
#include "communication_jobs.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class queries
    {
        using error=communication::error;
        using token=communication::operation_token;
        using operation=communication_worker_protocol::operation;
        communication_jobs::queue jobs;
        ::uwvm2::utils::container::std_vector<std::uint8_t> encoded,reply;
        communication_jobs::admit admission{};
        communication_jobs::charge charging{};
        void* context{};
        token pending;
        std::uint64_t pending_deadline{};
        operation pending_operation{};
        mutable std::mutex encoding;
        std::atomic<bool> stopping{};
        bool configured{},poisoned{};
        pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        static bool same_origin(token const& a,token const& b)noexcept
        {
            return a.caller_instance==b.caller_instance&&a.caller_generation==b.caller_generation&&a.authority==b.authority&&
                a.capability==b.capability&&a.resource==b.resource&&a.family==b.family&&a.action==b.action&&a.secondary==b.secondary&&
                a.session==b.session&&a.sequence==b.sequence&&a.watch==b.watch&&a.count==b.count&&a.origins==b.origins&&a.retained_origins==b.retained_origins;
        }
        static bool observation(operation value)noexcept
        {
            switch(value)
            {
                case operation::context_inspect:case operation::node_inspect:case operation::topic_qos:case operation::service_available:
                case operation::service_qos:case operation::action_describe:case operation::action_take:case operation::graph_capture:return true;
                default:return false;
            }
        }
        error consume()noexcept
        {
            token completed;communication::native_result result;auto status=jobs.completion(completed,result);
            if(status!=error::ok)return status;
            if(completed.request!=pending.request||!same_origin(completed,pending))
            {poisoned=true;jobs.request_stop();return error::io;}
            status=jobs.release(pending,pending.request);
            if(status==error::ok){pending={};pending_deadline=0;pending_operation={};}
            return status;
        }
        template<typename Encode,typename Decode>
        error query(operation action,token const& original,std::uint64_t deadline,Encode&& encode,Decode&& decode)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock serialized{encoding,std::try_to_lock};if(!serialized.owns_lock())return error::again;
            if(!configured||poisoned||stopping.load(std::memory_order_acquire))return error::cancelled;
            if(!observation(action)||!communication_jobs::permits(original,action)||original.request||
                !original.valid(original.action,original.family,original.authority,original.resource,false))return error::denied;
            wire::writer writer{encoded};if(!encode(writer))return error::too_large;
            if(pending.request&&(!same_origin(pending,original)||pending_operation!=action))return error::busy;
            auto candidate=original;
            if(pending.request)candidate=pending;
            else
            {
                candidate.request=resources::allocate_identity();if(!candidate.request)return error::too_large;
            }
            communication_jobs::receipt result;
            auto status=jobs.enqueue(candidate,action,writer.written(),pending.request?pending_deadline:deadline,charging,context,result);
            if(status!=error::ok)return status;
            if(!pending.request){pending=std::move(candidate);pending_deadline=deadline;pending_operation=action;}
            if(result.phase!=communication_jobs::state::complete)return error::again;
            group_lease held;status=admission(context,pending,held);if(status!=error::ok)return status;
            status=jobs.copy(pending,held,reply,result);held.release();if(status!=error::ok)return status;
            // Observation RPCs have no native mutation. An effect-bearing reply
            // is not silently relabelled as none or used as trusted query data.
            if(result.cleanup_debt||result.observed!=communication::effect::none)
            {poisoned=true;jobs.request_stop();return result.status==error::ok?error::io:result.status;}
            if(result.status!=error::ok)
            {
                status=consume();return status==error::ok?result.status:status;
            }
            try{status=decode(wire::bytes{reply.data(),static_cast<std::size_t>(result.bytes)});}
            catch(std::bad_alloc const&){status=error::no_memory;}
            catch(std::length_error const&){status=error::too_large;}
            catch(...){status=error::io;}
            if(status==error::no_memory||status==error::too_large||status==error::again)return status;
            if(status!=error::ok){poisoned=true;jobs.request_stop();return status;}
            return consume();
        }
    public:
        queries()=default;queries(queries const&)=delete;queries& operator=(queries const&)=delete;
        ~queries()
        {
            if(!same_process())
            {std::construct_at(std::addressof(encoded));std::construct_at(std::addressof(reply));std::construct_at(std::addressof(pending));return;}
            retire();
        }
        /** @brief Reserve a single query plus its detached encode/decode storage.
         * @details slots must be exactly one. reserved_bytes covers twice the
         * sum of request_bytes and reply_bytes; finite byte limits are checked
         * before allocation. Configuration never reads a path, starts a worker,
         * sends a message or charges a capability. Failed allocation is retryable.
         */
        error configure(communication_jobs::plan plan,communication_jobs::admit revalidate,communication_jobs::charge charge,void* argument)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{encoding,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(configured||stopping.load())return error::denied;
            if(plan.slots!=1||!plan.request_bytes||plan.request_bytes>4u*1024u*1024u||!plan.reply_bytes||plan.reply_bytes>4u*1024u*1024u||
                !revalidate||!charge)return error::invalid;
            auto detached=plan.request_bytes+plan.reply_bytes;
            if(plan.reserved_bytes<2*detached)return error::too_large;
            try
            {
                ::uwvm2::utils::container::std_vector<std::uint8_t> input(static_cast<std::size_t>(plan.request_bytes)),output(static_cast<std::size_t>(plan.reply_bytes));
                plan.reserved_bytes-=detached;auto status=jobs.configure(plan,revalidate,argument);if(status!=error::ok)return status;
                encoded=std::move(input);reply=std::move(output);admission=revalidate;charging=charge;context=argument;configured=true;return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::invalid;}
        }
        template<typename Decode> error context_describe(token const& origin,std::uint64_t deadline,Decode&& decode)noexcept
        {return query(operation::context_inspect,origin,deadline,[](auto&){return true;},std::forward<Decode>(decode));}
        template<typename Decode> error node_describe(token const& origin,std::uint64_t deadline,Decode&& decode)noexcept
        {return query(operation::node_inspect,origin,deadline,[](auto&){return true;},std::forward<Decode>(decode));}
        template<typename Decode> error topic_qos(token const& origin,std::uint64_t deadline,Decode&& decode)noexcept
        {return query(operation::topic_qos,origin,deadline,[](auto&){return true;},std::forward<Decode>(decode));}
        template<typename Decode> error service_qos(token const& origin,std::uint64_t deadline,Decode&& decode)noexcept
        {return query(operation::service_qos,origin,deadline,[](auto&){return true;},std::forward<Decode>(decode));}
        template<typename Decode> error service_available(token const& origin,std::uint64_t deadline,Decode&& decode)noexcept
        {return query(operation::service_available,origin,deadline,[](auto&){return true;},std::forward<Decode>(decode));}
        template<typename Decode> error action_describe(token const& origin,std::optional<std::uint64_t> native_goal,std::uint64_t deadline,Decode&& decode)noexcept
        {
            if(native_goal&&!*native_goal)return error::invalid;
            return query(operation::action_describe,origin,deadline,[&](auto& out){return out.boolean(native_goal.has_value())&&(!native_goal||out.u64(*native_goal));},std::forward<Decode>(decode));
        }
        /** @brief Retain a consumed native action event until host ingress accepts it.
         * @details This dequeue is not remotely replayed after a lost reply.
         * Unknown transport effects retain the occupied slot. Local backpressure
         * returns again from decode and retries the same complete owned record.
         * A decoder must not perform guest I/O or another native operation.
         */
        template<typename Decode> error action_take(token const& origin,std::uint64_t deadline,Decode&& decode)noexcept
        {return query(operation::action_take,origin,deadline,[](auto&){return true;},std::forward<Decode>(decode));}
        template<typename Decode> error graph_capture(token const& origin,std::uint64_t deadline,Decode&& decode)noexcept
        {return query(operation::graph_capture,origin,deadline,[](auto&){return true;},std::forward<Decode>(decode));}
        /** @brief Drain a revoked observer's completed read without disturbing the shared client.
         * @details The terminal status and effect must be settled. An uncertain result stays
         * fenced. A different observer can subsequently reserve its own native probe, and
         * cannot obtain the previous observer's response or acquire its grant.
         */
        error pump(communication_worker_async::transport& transport)noexcept
        {
            auto status=jobs.pump(transport);
            std::unique_lock lock{encoding,std::try_to_lock};
            if(!lock.owns_lock()||!pending.request||pending_operation!=operation::service_available||
               pending.secondary!=communication::permission::service_submit)return status;
            communication_jobs::receipt completed;
            if(jobs.inspect(pending,pending.request,completed)!=error::ok||completed.phase!=communication_jobs::state::complete||
               completed.cleanup_debt||completed.observed!=communication::effect::none)return status;
            group_lease held;auto current=admission(context,pending,held);
            if(current!=error::denied&&current!=error::cancelled&&current!=error::bad_handle)return status;
            auto discarded=consume();return discarded==error::ok?status:discarded;
        }
        /** @brief Cancel the retained query without granting observation or refunding work. */
        error cancel(communication_worker_async::transport& transport,communication_jobs::receipt& result)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{encoding,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!pending.request)return error::bad_handle;
            return jobs.cancel(pending,pending.request,transport,result);
        }
        bool occupied()const noexcept{return jobs.occupied();}
        void request_stop()noexcept{if(same_process()){stopping.store(true,std::memory_order_release);jobs.request_stop();}}
        void retire()noexcept{if(same_process()){request_stop();jobs.retire();}}
    };
#endif
}
