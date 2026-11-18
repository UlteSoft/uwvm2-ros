/** @file
 * @brief Bounded owned native operations with current per-call authority.
 * @details Configuration reserves all request/reply/receipt slots with recoverable
 * fast_io allocation. reserve() owns the complete request before first charging;
 * commit() accepts the dispatcher's complete charged group. Exact retry preserves
 * the same request and never enqueues or charges twice. A host queue acceptance
 * is not a native completion. Inspection preserves the actual worker generation.
 *
 * pump() obtains fresh validation-only admission without holding the queue lock,
 * performs one nonblocking write under that group, then releases all leases before
 * reading. No endpoint callback or response wait runs under queue/authority locks.
 * An unsent expiry is known no-effect. Any partially sent operation whose reply is
 * lost is unknown and retains an unreleasable receipt; confirmed worker cleanup
 * settles lifetime debt only. Cancellation has no creation/traffic rate charge.
 */
#pragma once
#include "communication_worker_async.h"
#include "communication_operation.h"
namespace uwvm2::imported::ros::scene::communication_jobs
{
#if defined(__linux__)
    namespace c=communication;namespace rpc=communication_worker_protocol;namespace async=communication_worker_async;
    using error=c::error;using effect=c::effect;
    enum class state:std::uint8_t{free,reserved,queued,staged,sending,waiting,complete,released};
    struct plan
    {
        c::owner authority;c::operation_family family{};
        std::uint64_t resource{},binding{},provider_generation{},slots{},request_bytes{},reply_bytes{},max_deadline_ns{},reserved_bytes{};
    };
    struct receipt
    {
        std::uint64_t request{},provider_generation{};state phase{};error status{error::again};effect observed{};
        std::uint64_t bytes{};bool cleanup_debt{};
    };
    using admit = error(*)(void*,c::operation_token const&,group_lease&)noexcept;
    using charge = error(*)(void*,c::operation_token const&,std::uint64_t,group_lease&)noexcept;
    using claim_set=std::array<admission_claim,8>;
    /** @brief Release returns an existing record without reading or acknowledging it.
     * @details The dispatcher still verifies the exact retained resource cohort.
     * Only the current primary cleanup grant is leased: closed parent/schema
     * capabilities and exhausted traffic budgets must not prevent cleanup.
     */
    inline bool receive_cleanup(c::permission action)noexcept
    {return action==c::permission::topic_release||action==c::permission::service_release;}
    /** @brief Non-expanding cleanup, independent of traffic attenuation and parent grants.
     * @details A resource close must still name its exact retained original
     * resource cohort, but can only destroy that resource. It cannot publish,
     * receive, acknowledge a record or open another endpoint.
     */
    inline bool cleanup(c::permission action)noexcept
    {return receive_cleanup(action)||action==c::permission::resource_close;}
    /** @brief Account for one full request and the reserved maximum complete reply.
     * @details Both private RPC envelopes are included. Charging the reply bound
     * before publication prevents a response-size-dependent effect from escaping
     * optional byte attenuation. Unused allowance is not refunded. Capacity is a
     * separate reservation. A pump or delivery revalidation uses zero bytes and
     * never charges again; the outer dispatcher must not charge this operation twice.
     */
    inline bool submission_cost(std::uint64_t request,std::uint64_t reply,std::uint64_t& bytes)noexcept
    {
        if(request>4u*1024u*1024u||!reply||reply>4u*1024u*1024u)return false;
        bytes=request+reply+2*rpc::header_bytes;return true;
    }
    /** @brief Derive exact claim roles from a trusted, already validated origin token.
     * @details The primary operation is charged. Retained node/context/schema
     * origins are validation-only, so creation rates are not charged again.
     * action.respond adds one charged grant-only transition permission with zero
     * extra bytes. It cannot use the primary permission as a substitute. This
     * helper does not validate resource ownership or itself grant authority.
     */
    inline std::span<admission_claim const> admission_claims(c::operation_token const& token,claim_set& output,std::uint64_t bytes=0)noexcept
    {
        if(!token.count||token.count>output.size())return {};
        if(cleanup(token.action))
        {output[0]={token.capability,c::permission_name(token.action),0,false,false,true};return {output.data(),1};}
        for(std::size_t i{};i<token.count;++i)
        {
            bool secondary=token.action==c::permission::action_respond&&i==token.count-1;
            output[i]={token.origins[i].capability,c::permission_name(token.origins[i].action),i?0:bytes,
                secondary||(i==0&&token.action==c::permission::resource_observe),i!=0&&!secondary};
        }
        return {output.data(),token.count};
    }
    /** @brief Closed mapping prevents a valid token being reused for another RPC. */
    inline bool permits(c::operation_token const& token,rpc::operation operation)noexcept
    {
        using P=c::permission;using R=rpc::operation;
        switch(operation)
        {
            case R::context_inspect:return token.family==c::operation_family::context&&token.action==P::context_describe;
            case R::context_shutdown:return token.family==c::operation_family::context&&token.action==P::context_shutdown;
            case R::node_inspect:return token.family==c::operation_family::node&&token.action==P::node_describe;
            case R::topic_publish:return token.family==c::operation_family::topic&&token.action==P::topic_publish;
            case R::topic_take:return token.family==c::operation_family::topic&&token.action==P::topic_take;
            case R::topic_reserve:return token.family==c::operation_family::topic&&token.action==P::topic_reserve;
            case R::topic_acknowledge:return token.family==c::operation_family::topic&&token.action==P::topic_acknowledge;
            case R::topic_release:return token.family==c::operation_family::topic&&token.action==P::topic_release;
            case R::topic_qos:return token.family==c::operation_family::topic&&token.action==P::qos_describe;
            case R::topic_liveliness:return token.family==c::operation_family::topic&&token.action==P::qos_liveliness_assert;
            case R::topic_acknowledged:return token.family==c::operation_family::topic&&token.action==P::qos_acknowledgements_wait;
            case R::service_submit:return token.family==c::operation_family::service&&token.action==P::service_submit;
            case R::service_respond:return token.family==c::operation_family::service&&token.action==P::service_respond;
            case R::service_take:return token.family==c::operation_family::service&&token.action==P::service_take;
            case R::service_reserve:return token.family==c::operation_family::service&&token.action==P::service_reserve;
            case R::service_acknowledge:return token.family==c::operation_family::service&&token.action==P::service_acknowledge;
            case R::service_release:return token.family==c::operation_family::service&&token.action==P::service_release;
            case R::service_available:return token.family==c::operation_family::service&&token.action==P::service_available;
            case R::service_qos:return token.family==c::operation_family::service&&token.action==P::qos_describe;
            case R::service_abandon:return token.family==c::operation_family::service&&token.action==P::service_abandon;
            case R::action_submit:return token.family==c::operation_family::action&&token.action==P::action_submit;
            case R::action_respond:return token.family==c::operation_family::action&&token.action==P::action_respond;
            case R::action_take:return token.family==c::operation_family::action&&token.action==P::action_take;
            case R::action_cancel:return token.family==c::operation_family::action&&(token.action==P::action_cancel||token.action==P::action_cancel_set);
            case R::action_describe:return token.family==c::operation_family::action&&token.action==P::action_describe;
            case R::graph_capture:return token.family==c::operation_family::graph&&(token.action==P::graph_snapshot||token.action==P::graph_watch||token.action==P::resource_observe);
            case R::resource_close:return token.action==P::resource_close;
            case R::topic_open:case R::service_open:case R::action_open:return false; // Separate creation-key ledger.
        }
        return false;
    }
    class queue
    {
        struct job
        {
            c::operation_token token;rpc::operation operation{};std::uint64_t deadline{},order{};
            ::uwvm2::utils::container::std_vector<std::uint8_t> request,reply;
            std::size_t request_size{};receipt result;async::ticket wire;
            bool completion_pending{},managed_submission{};
        };
        plan selected;::uwvm2::utils::container::std_vector<job> jobs;
        admit admission{};void* admission_context{};std::uint64_t order{},high_request{},high_sequence{};
        bool configured{};std::atomic<bool> stopped{};
        mutable std::mutex mutex;std::mutex pumping,submitting;pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        static bool same(c::operation_token const& a,c::operation_token const& b)noexcept
        {
            return a.caller_instance==b.caller_instance&&a.caller_generation==b.caller_generation&&a.authority==b.authority&&a.capability==b.capability&&
                a.resource==b.resource&&a.request==b.request&&a.session==b.session&&a.sequence==b.sequence&&a.family==b.family&&a.action==b.action&&
                a.secondary==b.secondary&&a.watch==b.watch&&a.count==b.count&&a.origins==b.origins&&a.retained_origins==b.retained_origins;
        }
        job* find(std::uint64_t request)noexcept
        {for(auto& value:jobs)if(value.result.request==request&&request&&value.result.phase!=state::free)return &value;return nullptr;}
        static bool terminal(job const& value)noexcept{return value.result.phase==state::complete||value.result.phase==state::released;}
        static void finish(job& value,error status,effect observed,std::uint64_t size,bool debt)noexcept
        {value.result.phase=state::complete;value.result.status=status;value.result.observed=observed;value.result.bytes=size;value.result.cleanup_debt=debt;value.completion_pending=true;}
        error retain_reply(job& value,async::transport& transport)noexcept
        {
            async::result reply;auto status=transport.copy(value.wire,value.reply,reply);if(status!=error::ok)return status;
            finish(value,reply.status,reply.observed,reply.bytes,reply.observed==effect::unknown);
            // A failed/fenced incarnation is deliberately not reused.
            if(transport.inspect().state==async::phase::ready){auto acknowledged=transport.acknowledge(value.wire);if(acknowledged!=error::ok)return acknowledged;}
            return error::ok;
        }
    public:
        /** @brief Freeze one exact endpoint and reserve finite complete payload slots.
         * @details The worker/transport must outlive stop/drain of this queue. No
         * background thread or hidden capacity is created. The owner periodically
         * pumps without authority leases and drains trusted completions separately.
         */
        ~queue(){if(!same_process()){std::construct_at(std::addressof(jobs));return;}retire();}
        error configure(plan value,admit callback,void* context)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(configured||stopped.load())return error::denied;
            if(!value.authority||value.family==c::operation_family::unconfigured||value.family>c::operation_family::graph||!value.resource||!value.binding||
                !value.provider_generation||!value.slots||value.slots>65536||!value.request_bytes||value.request_bytes>4u*1024u*1024u||
                !value.reply_bytes||value.reply_bytes>4u*1024u*1024u||!value.max_deadline_ns||value.max_deadline_ns>60000000000ull||!callback)return error::invalid;
            auto bytes=value.request_bytes+value.reply_bytes;
            if(value.slots>SIZE_MAX/bytes||value.reserved_bytes<value.slots*bytes)return error::too_large;
            try
            {
                ::uwvm2::utils::container::std_vector<job> prepared(static_cast<std::size_t>(value.slots));
                for(auto& entry:prepared){entry.request.resize(static_cast<std::size_t>(value.request_bytes));entry.reply.resize(static_cast<std::size_t>(value.reply_bytes));}
                jobs=std::move(prepared);selected=value;admission=callback;admission_context=context;configured=true;return error::ok;
            }
            catch(...){return error::no_memory;}
        }
        /** @brief Copy full canonical RPC parameters before the first authority charge.
         * @details Existing request IDs and caller session/sequence keys cannot
         * be rebound to another caller, capability, origin or body. Every retry
         * compares owned bytes, not a digest. Released IDs remain tombstones until
         * a later globally unique request acknowledges their finite slot; the
         * retained high-water marks permanently reject forgotten request IDs and
         * mutation keys. Request IDs must come from resources::allocate_identity.
         */
    private:
        error reserve_impl(bool managed,c::operation_token const& token,rpc::operation operation,wire::bytes body,std::uint64_t deadline,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!configured||!token.valid(token.action,selected.family,selected.authority,selected.resource)||!permits(token,operation))return error::denied;
            for(auto& value:jobs)
            {
                bool same_key=value.result.phase!=state::free&&token.session&&token.sequence&&value.token.session==token.session&&value.token.sequence==token.sequence;
                if(value.result.request==token.request||same_key)
                {
                    if(value.managed_submission!=managed||!same(value.token,token)||value.operation!=operation||value.deadline!=deadline||value.request_size!=body.size()||
                        !std::equal(body.begin(),body.end(),value.request.begin()))return error::stale;
                    output=value.result;return error::ok;
                }
            }
            if(stopped.load())return error::cancelled;
            if(token.request<=high_request||(token.sequence&&token.sequence<=high_sequence))return error::stale;
            if(bool(token.session)!=bool(token.sequence)||(token.session&&token.session!=selected.resource))return error::invalid;
            auto now=c::now();if(deadline<=now)return error::timeout;
            if(deadline>INT64_MAX||deadline-now>selected.max_deadline_ns||body.size()>selected.request_bytes)return error::too_large;
            if(order==UINT64_MAX)return error::too_large;
            job* available{};
            for(auto& value:jobs)if(value.result.phase==state::free){available=&value;break;}
            // The global request high-water mark fences all forgotten IDs. A
            // later keyed mutation also passes the per-resource sequence mark.
            // Unkeyed reads can reuse released slots without unbounded history.
            if(!available)for(auto& value:jobs)if(value.result.phase==state::released&&
                token.caller_instance==value.token.caller_instance&&token.caller_generation==value.token.caller_generation&&
                token.capability==value.token.capability){available=&value;break;}
            if(!available)return error::again;
            auto& value=*available;value.token=token;value.operation=operation;value.deadline=deadline;value.request_size=body.size();value.managed_submission=managed;
            if(!body.empty())std::memcpy(value.request.data(),body.data(),body.size());
            value.order=++order;value.wire={};value.completion_pending=false;
            value.result={token.request,selected.provider_generation,state::reserved,error::again,effect::none,0,false};output=value.result;return error::ok;
        }
        /** @brief Commit the pre-reserved operation under the exact complete charged group. */
        error commit_impl(bool managed,c::operation_token const& token,group_lease& held,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(token.request);
            if(!value||value->managed_submission!=managed||!same(value->token,token))return error::denied;
            std::uint64_t bytes{};if(!submission_cost(value->request_size,selected.reply_bytes,bytes))return error::invalid;
            claim_set claims;auto expected=admission_claims(token,claims,bytes);caller who{token.caller_instance,token.caller_generation};
            if(!held.matches(who,expected))return error::denied;
            if(value->result.phase!=state::reserved){output=value->result;return error::ok;}
            if(stopped.load())return error::cancelled;
            if(c::now()>=value->deadline)return error::timeout;
            if(token.request<=high_request||(token.sequence&&token.sequence<=high_sequence))return error::stale;
            if(!held.consume(who,expected))return error::denied;
            high_request=token.request;if(token.sequence)high_sequence=token.sequence;
            value->result.phase=state::queued;value->result.observed=effect::locally_accepted;output=value->result;return error::ok;
        }
        error abandon_impl(bool managed,c::operation_token const& token)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(token.request);
            if(!value||value->managed_submission!=managed||!same(value->token,token))return error::denied;
            if(value->result.phase!=state::reserved)return error::busy;
            value->token={};value->result={};return error::ok;
        }
    public:
        error reserve(c::operation_token const& token,rpc::operation operation,wire::bytes body,std::uint64_t deadline,receipt& output)noexcept
        {return reserve_impl(false,token,operation,body,deadline,output);}
        error commit(c::operation_token const& token,group_lease& held,receipt& output)noexcept
        {return commit_impl(false,token,held,output);}
        error abandon(c::operation_token const& token)noexcept{return abandon_impl(false,token);}
        /** @brief Atomically coordinate owned reservation, exact charging and publication.
         * @details This is the adapter entry for new queued effects. It reserves
         * complete request/reply storage before calling the trusted admission
         * function, then consumes exactly that complete charged group. Concurrent
         * or reentrant submissions return again rather than waiting under an
         * authority lease. Exact accepted retries do not call admission again.
         * Low-level reserve/commit/abandon cannot take over a managed submission.
         *
         * No native I/O occurs. The charge callback runs without the queue mutex;
         * it must revalidate all current resource origins and return one charged
         * group for submission_cost, including the response bound/envelopes.
         * The outer dispatcher must not charge this same effect independently.
         * A failed charge discards only the tentative reservation. A revocation
         * that wins publication may retain its own cancellation receipt; no
         * refund, replacement worker or claim of native completion is implied.
         */
        error enqueue(c::operation_token const& token,rpc::operation operation,wire::bytes body,std::uint64_t deadline,
                      charge admit_submission,void* context,receipt& output)noexcept
        {
            if(!same_process()||!admit_submission)return error::denied;
            std::unique_lock serial{submitting,std::try_to_lock};if(!serial.owns_lock())return error::again;
            receipt pending;auto status=reserve_impl(true,token,operation,body,deadline,pending);if(status!=error::ok)return status;
            if(pending.phase!=state::reserved){output=pending;return error::ok;}
            std::uint64_t cost{};
            if(!submission_cost(body.size(),selected.reply_bytes,cost)){abandon_impl(true,token);return error::invalid;}
            group_lease held;status=admit_submission(context,token,cost,held);
            if(status==error::ok)status=commit_impl(true,token,held,pending);
            held.release();
            if(status!=error::ok){abandon_impl(true,token);return status;}
            output=pending;return error::ok;
        }
        /** @brief Perform finite queue progress; caller holds no authority lease.
         * @details Admission callback is invoked with neither queue mutex held.
         * It must validate the exact full token and return all claims without a
         * second charge. Concurrent revoke/cancel wins before any later write.
         */
        error pump(async::transport& transport)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock serial{pumping,std::try_to_lock};if(!serial.owns_lock())return error::again;
            c::operation_token token;std::uint64_t request{};
            {
                std::lock_guard lock{mutex};job* value{};
                for(auto& row:jobs)if(row.result.phase>=state::queued&&row.result.phase<=state::waiting&&(!value||row.order<value->order))value=&row;
                if(!value)return error::again;
                if(value->result.phase==state::queued)
                {
                    if(stopped.load()||c::now()>=value->deadline){finish(*value,stopped.load()?error::cancelled:error::timeout,effect::none,0,false);return error::ok;}
                    auto status=transport.stage(value->operation,selected.binding,{value->request.data(),value->request_size},value->deadline,value->wire);
                    if(status==error::busy||status==error::again)return error::again;
                    if(status!=error::ok){finish(*value,status,effect::none,0,false);return error::ok;}
                    value->result.phase=state::staged;
                }
                if(value->result.phase==state::waiting)
                {transport.receive(value->wire);return retain_reply(*value,transport);}
                token=value->token;request=value->result.request;
            }
            group_lease held;auto admitted=admission(admission_context,token,held);
            std::lock_guard lock{mutex};auto value=find(request);if(!value||terminal(*value))return error::again;
            claim_set claims;
            if(admitted==error::ok&&!held.matches({token.caller_instance,token.caller_generation},admission_claims(token,claims)))admitted=error::denied;
            if(admitted!=error::ok||stopped.load())
            {
                auto snapshot=transport.inspect();bool sent=snapshot.current==value->wire&&snapshot.sent;
                if(!sent){auto discarded=transport.discard(value->wire);if(discarded==error::again)return error::again;if(discarded!=error::ok){transport.request_stop();}}
                else transport.request_stop();
                finish(*value,admitted==error::ok?error::cancelled:admitted,sent?effect::unknown:effect::none,0,sent);return error::ok;
            }
            auto status=transport.send(value->wire,held);auto snapshot=transport.inspect();held.release();
            if(snapshot.state==async::phase::failed)return retain_reply(*value,transport);
            value->result.phase=snapshot.state==async::phase::header||snapshot.state==async::phase::body?state::waiting:state::sending;
            return status;
        }
        /** @brief Cached identity-bound observation; performs no I/O or expiry mutation. */
        error inspect(c::operation_token const& identity,std::uint64_t request,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(request);
            if(!value||identity.caller_instance!=value->token.caller_instance||identity.caller_generation!=value->token.caller_generation||
                identity.authority!=selected.authority||identity.resource!=selected.resource)return error::denied;
            output=value->result;return error::ok;
        }
        /** @brief Copy retained reply under freshly revalidated original authority.
         * @details The caller preflights guest output. This copies owned bytes and
         * does not consume the receipt or invoke a guest callback under the lock.
         */
        error copy(c::operation_token const& token,group_lease const& held,std::span<std::uint8_t> output,receipt& result)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(token.request);
            claim_set claims;
            if(!value||!same(value->token,token)||!held.matches({token.caller_instance,token.caller_generation},admission_claims(token,claims)))return error::denied;
            if(!terminal(*value))return error::again;
            if(output.size()<value->result.bytes)return error::too_large;
            if(value->result.bytes)std::memcpy(output.data(),value->reply.data(),static_cast<std::size_t>(value->result.bytes));
            result=value->result;return error::ok;
        }
        /** @brief Stop an exact operation without applying traffic attenuation. */
        error cancel(c::operation_token const& identity,std::uint64_t request,async::transport& transport,receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(request);
            if(!value||identity.caller_instance!=value->token.caller_instance||identity.caller_generation!=value->token.caller_generation||
                identity.authority!=selected.authority||identity.resource!=selected.resource)return error::denied;
            if(terminal(*value)){output=value->result;return error::ok;}
            bool sent{};
            if(value->wire.request)
            {
                auto current=transport.inspect();if(current.current!=value->wire)return error::io;
                if(current.state==async::phase::ready||current.state==async::phase::failed)
                {auto status=retain_reply(*value,transport);if(status!=error::ok)return status;output=value->result;return error::ok;}
                sent=current.sent!=0;
                if(sent)transport.request_stop();else{auto status=transport.discard(value->wire);if(status!=error::ok)return status;}
            }
            finish(*value,error::cancelled,sent?effect::unknown:effect::none,0,sent);output=value->result;return error::ok;
        }
        /** @brief Deliver one detached completion to trusted endpoint bookkeeping.
         * @details The owner calls this outside all authority groups and queue
         * locks. It returns the complete immutable operation identity and actual
         * receipt, never remote goal IDs masquerading as host request IDs.
         */
        error completion(c::operation_token& token,c::native_result& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            for(auto& value:jobs)if(value.completion_pending)
            {value.completion_pending=false;token=value.token;output={value.result.status,value.result.observed,value.result.request,value.result.provider_generation,false};return error::ok;}
            return error::again;
        }
        /** @brief Copy a terminal worker body for trusted host bookkeeping.
         * @details A completion is consumed only after the complete body fits.
         * Storage belongs to the supervisor and must be pre-reserved. No guest
         * callback, allocation, I/O or authority acquisition occurs under this
         * lock. The caller must not expose these bytes to Wasm: public delivery
         * uses copy() with fresh complete authority. Exact native correlations
         * can therefore be reconciled after revocation without granting data
         * access or inventing successful effects.
         */
        error completion(c::operation_token& token,c::native_result& output,std::span<std::uint8_t> body,std::size_t& required)noexcept
        {
            required=0;if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            for(auto& value:jobs)if(value.completion_pending)
            {
                required=static_cast<std::size_t>(value.result.bytes);if(body.size()<required)return error::too_large;
                if(required)std::memcpy(body.data(),value.reply.data(),required);
                token=value.token;output={value.result.status,value.result.observed,value.result.request,value.result.provider_generation,false};
                value.completion_pending=false;return error::ok;
            }
            return error::again;
        }
        error release(c::operation_token const& identity,std::uint64_t request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(request);
            if(!value||identity.caller_instance!=value->token.caller_instance||identity.caller_generation!=value->token.caller_generation||
                identity.authority!=selected.authority||identity.resource!=selected.resource)return error::denied;
            if(!terminal(*value)||value->result.cleanup_debt||value->result.observed==effect::unknown||value->completion_pending)return error::busy;
            // A readiness result is a single, unkeyed observation with an internal
            // request ID. Once its exact origin consumes the terminal result, a
            // different independently admitted observer can use the shared client.
            // high_request continues to reject replay of every retired request.
            if(value->operation==rpc::operation::service_available&&!value->token.session&&!value->token.sequence&&
               value->result.observed==effect::none)
            {
                if(!same(value->token,identity))return error::denied;
                erase_bytes({value->request.data(),value->request_size});
                erase_bytes({value->reply.data(),static_cast<std::size_t>(value->result.bytes)});
                value->token={};value->result={};value->request_size=0;
                return error::ok;
            }
            value->result.phase=state::released;return error::ok;
        }
        /** @brief Trusted positive namespace/endpoint teardown evidence only. */
        error cleanup(std::uint64_t request,bool confirmed)noexcept
        {if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=find(request);if(!value)return error::bad_handle;if(!confirmed)return error::busy;value->result.cleanup_debt=false;return error::ok;}
        bool occupied()const noexcept
        {if(!same_process())return true;std::lock_guard lock{mutex};for(auto const& value:jobs)if(value.result.phase!=state::free&&value.result.phase!=state::released)return true;return false;}
        void request_stop()noexcept{if(same_process())stopped.store(true,std::memory_order_release);}
        /** @brief Drain the short pump before owner destroys callbacks or transport.
         * @details Pending rows are retained. Pump/cancel and confirmed supervisor
         * cleanup settle them; destructor/timeout never claims native no-effect.
         */
        void retire()noexcept{if(!same_process())return;request_stop();std::lock_guard lock{pumping};}
    };
#endif
}
