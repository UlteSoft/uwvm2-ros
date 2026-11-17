/** @file
 * @brief Bounded endpoint close with exclusion of concurrent guest operations.
 * @details The gate admits ordinary calls until a closer proves quiescence.
 * Close then owns an independent reserved RPC slot and forbids new work. Native
 * success is accepted only with the exact endpoint identity from the verified
 * creation receipt. Unknown, malformed or lost replies retain cleanup debt;
 * neither a signal nor a deadline is proof of native resource destruction.
 *
 * Only an independently admitted resource.close capability can enqueue cleanup.
 * Its authority callback must verify the retained resource identity while
 * allowing closed parent/schema grants and exhausted traffic budgets. The
 * supervisor pumps nonblocking IPC; no guest call waits or creates a thread.
 * All allocation is recoverable fast_io, and the fixed queue payload reservation
 * is additional to publication/query/receive capacity.
 */
#pragma once
#include "communication_jobs.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class endpoint_closure
    {
        using error=communication::error;using effect=communication::effect;using token=communication::operation_token;
        static constexpr std::uint64_t closing=std::uint64_t{1}<<63;
        std::atomic<std::uint64_t> entries{};
        communication_jobs::queue jobs;communication_jobs::plan plan;
        communication_jobs::admit admission{};communication_jobs::charge charging{};void* context{};
        token original;std::uint64_t native_endpoint{};
        bool pending{},complete{},confirmed{},debt{};error outcome{error::again};
        std::atomic<bool> configured{},stopping{};mutable std::mutex mutex;pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        static bool same(token a,token b)noexcept
        {
            a.request=b.request=0;
            return a.caller_instance==b.caller_instance&&a.caller_generation==b.caller_generation&&a.authority==b.authority&&
                a.capability==b.capability&&a.resource==b.resource&&a.session==b.session&&a.sequence==b.sequence&&a.watch==b.watch&&
                a.family==b.family&&a.action==b.action&&a.secondary==b.secondary&&a.count==b.count&&a.origins==b.origins&&a.retained_origins==b.retained_origins;
        }
    public:
        /** @brief One ordinary operation pin; held through detached guest delivery.
         * @details Closing cannot proceed while any pin is live. Nested calls
         * remain bounded by the integer counter and never block on a close. A
         * fork child does not mutate the parent's inherited bookkeeping.
         */
        class call
        {
            friend class endpoint_closure;endpoint_closure* owner{};
            explicit call(endpoint_closure* value)noexcept:owner(value){}
        public:
            call()=default;call(call const&)=delete;call& operator=(call const&)=delete;
            call(call&& other)noexcept:owner(std::exchange(other.owner,nullptr)){}
            ~call(){if(owner&&owner->same_process())owner->entries.fetch_sub(1,std::memory_order_release);}
            explicit operator bool()const noexcept{return owner!=nullptr;}
        };
        static constexpr std::uint64_t reserved_bytes=9; // One empty-request slot plus the exact eight-byte reply.
        endpoint_closure()=default;endpoint_closure(endpoint_closure const&)=delete;endpoint_closure& operator=(endpoint_closure const&)=delete;
        ~endpoint_closure(){if(!same_process()){std::construct_at(std::addressof(original));return;}retire();}
        /** @brief Prepare one independent cleanup slot without authority or I/O.
         * @details selected uses the actual host resource, worker binding and
         * incarnation. The native endpoint comes only from verified creation.
         * The enclosing deployment explicitly reserves reserved_bytes separately.
         */
        error configure(communication_jobs::plan selected,std::uint64_t native,communication_jobs::admit admit,
                        communication_jobs::charge charge,void* argument)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(configured.load()||stopping.load()||!native||!admit||!charge)return error::denied;
            selected.slots=1;selected.request_bytes=1;selected.reply_bytes=8;selected.reserved_bytes=reserved_bytes;
            auto status=jobs.configure(selected,admit,argument);if(status!=error::ok)return status;
            plan=selected;native_endpoint=native;admission=admit;charging=charge;context=argument;configured=true;return error::ok;
        }
        call enter()noexcept
        {
            if(!same_process()||!configured.load(std::memory_order_acquire)||stopping.load())return {};
            auto current=entries.load(std::memory_order_acquire);
            while(!(current&closing)&&current<closing-1)
                if(entries.compare_exchange_weak(current,current+1,std::memory_order_acquire))
                {if(!stopping.load())return call{this};entries.fetch_sub(1,std::memory_order_release);return {};}
            return {};
        }
        /** @brief Start or recover exact close after excluding ordinary operations.
         * @details quiescent is trusted cached occupancy inspection only, with no
         * callbacks or native I/O. A busy endpoint remains open. The copied RPC
         * and exact reply capacity precede the cleanup admission callback. Known
         * pre-effect failure permits a later retry; unknown failure fences forever.
         */
        template<typename Quiescent> error close(token const& origin,Quiescent&& quiescent)noexcept
        {
            if(!same_process()||!configured.load()||!origin.valid(communication::permission::resource_close,plan.family,plan.authority,plan.resource,false)||
                origin.request||origin.session||origin.sequence)return error::denied;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(pending)
            {
                if(!same(original,origin))return error::denied;
                group_lease current;auto status=admission(context,origin,current);if(status!=error::ok)return status;
                if(!complete)return error::again;
                auto observed=outcome;
                if(!confirmed&&!debt){pending=complete=false;original={};}
                return observed;
            }
            if(stopping.load())return error::cancelled;
            std::uint64_t idle{};if(!entries.compare_exchange_strong(idle,closing,std::memory_order_acq_rel))return error::again;
            if(!quiescent()){entries.store(0,std::memory_order_release);return error::busy;}
            auto time=communication::now();auto request=resources::allocate_identity();
            if(!request||time>INT64_MAX-plan.max_deadline_ns){entries.store(0,std::memory_order_release);return error::too_large;}
            auto candidate=origin.with_request(request);communication_jobs::receipt receipt;
            auto status=jobs.enqueue(candidate,communication_worker_protocol::operation::resource_close,{},time+plan.max_deadline_ns,charging,context,receipt);
            if(status!=error::ok){entries.store(0,std::memory_order_release);return status;}
            original=std::move(candidate);pending=true;return error::again;
        }
        /** @brief Reconcile close evidence on the existing shared nonblocking transport. */
        error pump(communication_worker_async::transport& transport)noexcept
        {
            if(!same_process()||!configured.load())return error::denied;
            auto status=jobs.pump(transport);if(status!=error::ok&&status!=error::again&&status!=error::busy)return status;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!pending||complete)return status;
            std::array<std::uint8_t,8> body;std::size_t required{};token actual;communication::native_result result;
            auto copied=jobs.completion(actual,result,body,required);bool malformed=copied==error::too_large;
            if(malformed)copied=jobs.completion(actual,result);
            if(copied!=error::ok)return copied==error::again?status:copied;
            std::uint64_t native{};wire::reader input{body};
            bool valid=!malformed&&same(actual,original)&&actual.request==original.request&&result.provider_generation==plan.provider_generation&&
                communication::known_status(result.status)&&!result.deferred&&result.observed==effect::none&&
                (result.status==error::ok?(required==8&&input.u64(native)&&native==native_endpoint&&input.done()):required==0);
            outcome=valid?result.status:error::io;debt=!valid;confirmed=valid&&result.status==error::ok;complete=true;
            if(!debt)
            {
                auto released=jobs.release(original,original.request);
                if(released!=error::ok){debt=true;confirmed=false;outcome=error::io;return error::ok;}
                if(!confirmed)entries.store(0,std::memory_order_release);
            }
            return error::ok;
        }
        bool cleanup_confirmed()const noexcept
        {if(!same_process())return false;std::unique_lock lock{mutex,std::try_to_lock};return lock.owns_lock()&&confirmed;}
        bool cleanup_debt()const noexcept
        {if(!same_process())return true;std::unique_lock lock{mutex,std::try_to_lock};return !lock.owns_lock()||debt;}
        bool occupied()const noexcept
        {if(!same_process())return true;std::unique_lock lock{mutex,std::try_to_lock};return !lock.owns_lock()||debt||(pending&&!confirmed);}
        void request_stop()noexcept
        {if(same_process()){stopping=true;entries.fetch_or(closing,std::memory_order_acq_rel);jobs.request_stop();}}
        /// @brief Drain outside caller authority and resource-registry locks.
        void retire()noexcept
        {if(same_process()){request_stop();jobs.retire();std::lock_guard lock{mutex};}}
    };
#endif
}
