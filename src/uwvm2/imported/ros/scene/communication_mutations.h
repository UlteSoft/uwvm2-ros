/** @file
 * @brief Typed, bounded asynchronous mutations for an isolated ROS worker.
 * @details Deployment supplies an exact immutable worker binding, incarnation,
 * host resource, finite reservations and trusted current-authority callbacks.
 * Every request is encoded into recoverable fast_io storage reserved at prepare
 * time. The owned queue copies the entire record before it calls the charging
 * callback. There is no native I/O, allocation or reply wait in a guest mutation.
 *
 * A successful submission reports only locally_accepted and deferred=true.
 * pump() belongs to the supervisor, outside guest/authority locks; each write
 * obtains fresh complete authority. completion() returns the actual numeric
 * native result and a separate worker body to trusted bookkeeping. Worker goal,
 * request and resource identifiers in that body are never host identities.
 * Mapping those identifiers requires the endpoint's bounded incarnation-bound
 * correlation registry. Unknown outcomes retain debt and cannot be released.
 *
 * This object owns no process or thread. Its owner must stop and drain it before
 * destroying either the borrowed authority callbacks or the retained transport.
 * Public receipt delivery requires fresh authority through copy(), never the
 * supervisor-only completion path. No generic socket, file or plugin call is
 * exposed by this protocol.
 */
#pragma once
#include "communication_jobs.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class mutations
    {
        using cerror=communication::error;
        using token=communication::operation_token;
        using operation=communication_worker_protocol::operation;
        using permission=communication::permission;
        communication_jobs::queue jobs;
        ::uwvm2::utils::container::std_vector<std::uint8_t> encoded;
        communication_jobs::charge charging{};
        void* context{};
        std::mutex encoding;
        bool configured{};
        pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        template<typename Encode>
        communication::native_result submit(operation action,token const& original,std::uint64_t deadline,Encode&& encode)noexcept
        {
            if(!same_process())return {cerror::denied,communication::effect::none};
            std::unique_lock serialized{encoding,std::try_to_lock};
            if(!serialized.owns_lock())return {cerror::again,communication::effect::none};
            if(!configured||!communication_jobs::permits(original,action))return {cerror::denied,communication::effect::none};
            wire::writer writer{encoded};
            if(!encode(writer))return {cerror::too_large,communication::effect::none};
            communication_jobs::receipt receipt;
            auto status=jobs.enqueue(original,action,writer.written(),deadline,charging,context,receipt);
            if(status!=cerror::ok)return {status,communication::effect::none};
            bool deferred=receipt.phase!=communication_jobs::state::complete&&receipt.phase!=communication_jobs::state::released;
            return {deferred?cerror::ok:receipt.status,receipt.observed,receipt.request,receipt.provider_generation,deferred};
        }
    public:
        mutations()=default;mutations(mutations const&)=delete;mutations& operator=(mutations const&)=delete;
        ~mutations()
        {
            if(!same_process()){std::construct_at(std::addressof(encoded));return;}
            retire();
        }
        /** @brief Freeze one endpoint's queue and independent encoder reservation.
         * @details reserved_bytes covers slots*(request_bytes+reply_bytes) plus
         * one complete encoder buffer. Zero capacity never means unlimited.
         * No worker starts, request is sent or authority is charged here. Both
         * allocation stages use recoverable fast_io and roll back on failure.
         */
        cerror configure(communication_jobs::plan plan,communication_jobs::admit revalidate,
                         communication_jobs::charge charge,void* argument)noexcept
        {
            if(!same_process())return cerror::denied;
            // Factories can configure while holding a registry lock. Never
            // block on an encoder whose admission callback may need that
            // registry: a concurrent prepare/submit simply retries.
            std::unique_lock lock{encoding,std::try_to_lock};if(!lock.owns_lock())return cerror::again;
            if(configured)return cerror::denied;
            if(!charge||!revalidate||!plan.slots||plan.slots>65536||!plan.request_bytes||plan.request_bytes>4u*1024u*1024u||
                !plan.reply_bytes||plan.reply_bytes>4u*1024u*1024u)return cerror::invalid;
            auto per_slot=plan.request_bytes+plan.reply_bytes;
            if(plan.reserved_bytes<plan.request_bytes||plan.slots>(plan.reserved_bytes-plan.request_bytes)/per_slot)return cerror::too_large;
            try
            {
                ::uwvm2::utils::container::std_vector<std::uint8_t> prepared(static_cast<std::size_t>(plan.request_bytes));
                plan.reserved_bytes-=plan.request_bytes;
                auto status=jobs.configure(plan,revalidate,argument);if(status!=cerror::ok)return status;
                encoded=std::move(prepared);charging=charge;context=argument;configured=true;return cerror::ok;
            }
            catch(std::bad_alloc const&){return cerror::no_memory;}catch(...){return cerror::invalid;}
        }
        communication::native_result publish(token const& origin,wire::bytes body,std::uint64_t deadline)noexcept
        {return submit(operation::topic_publish,origin,deadline,[&](auto& out){return out.blob(body)&&out.u64(deadline);});}
        communication::native_result liveliness(token const& origin,std::uint64_t deadline)noexcept
        {return submit(operation::topic_liveliness,origin,deadline,[](auto&){return true;});}
        communication::native_result acknowledged(token const& origin,std::uint64_t deadline)noexcept
        {return submit(operation::topic_acknowledged,origin,deadline,[&](auto& out){return out.u64(deadline);});}
        communication::native_result service_submit(token const& origin,wire::bytes body,std::uint64_t deadline)noexcept
        {return submit(operation::service_submit,origin,deadline,[&](auto& out){return out.u64(0)&&out.blob(body)&&out.u64(deadline);});}
        /** @brief Respond only with an already resolved worker-side correlation.
         * @details The owning service adapter must validate the host request and
         * map it within this exact worker incarnation before calling this method.
         */
        communication::native_result service_respond(token const& origin,std::uint64_t native_request,wire::bytes body,std::uint64_t deadline)noexcept
        {
            if(!native_request)return {cerror::invalid,communication::effect::none};
            return submit(operation::service_respond,origin,deadline,[&](auto& out){return out.u64(native_request)&&out.blob(body)&&out.u64(deadline);});
        }
        communication::native_result service_abandon(token const& origin,std::uint64_t native_request,std::uint64_t deadline)noexcept
        {
            if(!native_request)return {cerror::invalid,communication::effect::none};
            return submit(operation::service_abandon,origin,deadline,[&](auto& out){return out.u64(native_request);});
        }
        communication::native_result action_submit(token const& origin,wire::bytes uuid,wire::bytes body,std::uint64_t deadline)noexcept
        {
            if(uuid.size()!=16)return {cerror::invalid,communication::effect::none};
            return submit(operation::action_submit,origin,deadline,[&](auto& out){return out.blob(uuid)&&out.blob(body)&&out.u64(deadline);});
        }
        /** @brief Encode the separately admitted exact action transition.
         * @details The closed transition index must match the token's independent
         * secondary permission. Native request/goal values must already belong
         * to this endpoint's finite correlation table. Host IDs are never passed
         * through as an assumed native identity.
         */
        communication::native_result action_respond(token const& origin,std::uint64_t native_request,std::uint64_t native_goal,
            std::uint64_t transition,wire::bytes body,bool accept_cancel,std::uint64_t deadline)noexcept
        {
            if(!native_request||!native_goal||transition<1||transition>8||
                origin.secondary!=static_cast<permission>(static_cast<unsigned>(permission::action_accept)+transition-1))
                return {cerror::invalid,communication::effect::none};
            return submit(operation::action_respond,origin,deadline,[&](auto& out)
            {return out.u64(native_request)&&out.u64(native_goal)&&out.u64(transition)&&out.blob(body)&&out.boolean(accept_cancel)&&out.u64(deadline);});
        }
        communication::native_result action_cancel(token const& origin,std::uint64_t native_goal,std::uint64_t deadline)noexcept
        {
            if(!native_goal)return {cerror::invalid,communication::effect::none};
            return submit(operation::action_cancel,origin,deadline,[&](auto& out){return out.u64(native_goal)&&out.u64(deadline);});
        }
        communication::native_result shutdown(token const& origin,std::uint64_t deadline)noexcept
        {return submit(operation::context_shutdown,origin,deadline,[](auto&){return true;});}
        /** @brief Advance one nonblocking supervisor step outside authority groups. */
        cerror pump(communication_worker_async::transport& transport)noexcept{return jobs.pump(transport);}
        cerror completion(token& origin,communication::native_result& output,std::span<std::uint8_t> body,std::size_t& required)noexcept
        {return jobs.completion(origin,output,body,required);}
        /** @brief Consume only terminal metadata after a trusted decoder rejects body size.
         * @details This grants no guest access and does not release the receipt.
         * The typed supervisor must mark an oversized canonical body as unknown
         * and retain capacity; silently truncating it into a valid result is forbidden.
         */
        cerror completion(token& origin,communication::native_result& output)noexcept
        {return jobs.completion(origin,output);}
        cerror inspect(token const& origin,communication_jobs::receipt& output)noexcept{return jobs.inspect(origin,origin.request,output);}
        cerror copy(token const& origin,group_lease const& current,std::span<std::uint8_t> body,communication_jobs::receipt& result)noexcept
        {return jobs.copy(origin,current,body,result);}
        cerror cancel(token const& origin,communication_worker_async::transport& transport,communication_jobs::receipt& output)noexcept
        {return jobs.cancel(origin,origin.request,transport,output);}
        cerror release(token const& origin)noexcept{return jobs.release(origin,origin.request);}
        bool occupied()const noexcept{return jobs.occupied();}
        void request_stop()noexcept{jobs.request_stop();}
        void retire()noexcept{jobs.retire();}
    };
#endif
}
