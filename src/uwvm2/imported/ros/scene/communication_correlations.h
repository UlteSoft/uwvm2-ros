/** @file
 * @brief Finite service correlation translation for one isolated native endpoint.
 * @details Native request identifiers never become guest handles. Client rows
 * start from a host-issued submission identity; server rows allocate a distinct
 * host handle before exposing a validated reservation. Exact endpoint/worker
 * identity, consumer, reservation nonce and settlement key guard every update.
 *
 * This table grants no authority and performs no I/O. A trusted adapter invokes
 * it only after the complete current operation group and native reply framing
 * have been validated. Rows reserve fixed capacity, independent of rate limits.
 * Known completion needs explicit reclamation; unknown outcomes retain their
 * row until deployment teardown. Cleanup cannot turn an unknown response into
 * permission to retry a robot command. All allocation is recoverable fast_io.
 */
#pragma once
#include "communication_receive_reply.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
    class correlations
    {
        using error=communication::error;
        using effect=communication::effect;
        using handle=communication::handle;
        using consumer=communication::delivery_consumer;
        using reference=receive_wire::reference;
    public:
        enum class role:std::uint8_t{client,server};
        enum class phase:std::uint8_t{free,submitting,waiting,borrowed,delivered,responding,complete,uncertain,abandoning};
        struct plan
        {communication::owner authority;handle endpoint{},native_endpoint{},provider_generation{};role direction{};std::uint64_t capacity{};};
        struct view{handle correlation{},native{},pending{};phase state{};};
    private:
        struct row{handle host{},native{},pending{};phase state{};consumer who;reference borrowed;};
        plan selected;
        ::uwvm2::utils::container::std_vector<row> rows;
        communication::receipt last;
        handle last_correlation{},high_host{};consumer last_consumer;reference last_reference;bool last_ack{};
        mutable std::mutex mutex;bool configured{},retired{};
#if defined(__linux__)
        pid_t process{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        row* host(handle id)noexcept{for(auto& value:rows)if(value.host==id&&id&&value.state!=phase::free)return &value;return nullptr;}
        row* native(handle id)noexcept{for(auto& value:rows)if(value.native==id&&id&&value.state!=phase::free)return &value;return nullptr;}
        row* available()noexcept{for(auto& value:rows)if(value.state==phase::free)return &value;return nullptr;}
        bool bound(reference ref)const noexcept
        {return receive_wire::valid(ref)&&ref.endpoint==selected.endpoint&&ref.provider_generation==selected.provider_generation;}
    public:
        ~correlations(){if(!same_process())std::construct_at(std::addressof(rows));}
        /// @brief Exact dynamic row reservation; invalid or disabled counts return zero.
        static constexpr std::uint64_t reservation_bytes(std::uint64_t capacity)noexcept
        {return capacity&&capacity<=65536?capacity*sizeof(row):0;}
        /** @brief Reserve all correlation rows before any native submission or delivery. */
        error configure(plan value)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(configured||retired)return error::denied;
            if(!value.authority||!value.endpoint||!value.native_endpoint||!value.provider_generation||value.direction>role::server||
                !value.capacity||value.capacity>65536)return error::invalid;
            try
            {::uwvm2::utils::container::std_vector<row> storage(static_cast<std::size_t>(value.capacity));rows=std::move(storage);selected=value;configured=true;return error::ok;}
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::invalid;}
        }
        /** @brief Reserve an actual host submission identity before publishing client work.
         * @details The host request is supplied by the trusted submission ledger,
         * not by guest payload. No native correlation is guessed. A failed
         * admission can abandon this still-unsent reservation without a refund.
         */
        error reserve_client(consumer who,handle request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};if(!configured||retired||selected.direction!=role::client||!who||!request)return error::denied;
            if(auto previous=host(request))return previous->who==who&&previous->state==phase::submitting?error::ok:error::stale;
            if(request<=high_host)return error::stale;
            auto slot=available();if(!slot)return error::again;
            *slot={request,0,request,phase::submitting,who,{}};high_host=request;return error::ok;
        }
        /** @brief Abandon only a proven unsent client reservation; its identity is never reused. */
        error abandon_client(consumer who,handle request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=host(request);
            if(!value||value->who!=who||value->state!=phase::submitting)return error::denied;
            *value={};return error::ok;
        }
        /** @brief Apply verified client submission evidence without publishing native IDs. */
        error submitted(consumer who,handle request,communication::native_result result,handle actual_native)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=host(request);
            if(!value||value->who!=who||value->state!=phase::submitting||selected.direction!=role::client)return error::denied;
            result=communication::checked_mutation(result,request);
            bool invalid=result.request_identity!=request||result.deferred||result.provider_generation!=selected.provider_generation||
                (result.status==error::ok?(!actual_native||native(actual_native)!=nullptr):actual_native!=0);
            if(invalid||result.observed==effect::unknown){value->state=phase::uncertain;return error::io;}
            value->pending=0;
            if(result.status==error::ok){value->native=actual_native;value->state=phase::waiting;}
            else value->state=phase::complete;
            return error::ok;
        }
        /** @brief Translate a validated original native record before guest delivery.
         * @details The reference uses the host endpoint and actual provider
         * generation. A server gets a fresh host correlation; a client must find
         * its original submitted request. Failed guest copies repeat this exact
         * mapping. Another consumer, nonce or native ID cannot replace a borrow.
         */
        error reserve_received(consumer who,reference ref,handle native_endpoint,handle native_correlation,handle& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!configured||retired||!who||!bound(ref)||native_endpoint!=selected.native_endpoint||!native_correlation)return error::denied;
            for(auto const& previous:rows)if(previous.state==phase::borrowed&&
                (previous.native!=native_correlation||previous.who!=who||previous.borrowed!=ref))return error::busy;
            auto value=native(native_correlation);
            if(value&&value->state==phase::borrowed)
            {
                if(value->who!=who||value->borrowed!=ref)return error::stale;
                output=value->host;return error::ok;
            }
            if(selected.direction==role::client)
            {
                if(!value||value->who!=who||value->state!=phase::waiting)return error::denied;
            }
            else
            {
                if(value)return error::busy;
                value=available();if(!value)return error::again;
                auto id=resources::allocate_identity();if(!id)return error::too_large;
                *value={id,native_correlation,0,phase::waiting,who,{}};high_host=std::max(high_host,id);
            }
            value->borrowed=ref;value->state=phase::borrowed;output=value->host;return error::ok;
        }
        /** @brief Reconcile one already validated host ACK/RELEASE receipt.
         * @details Exact replay is inert, including a released server row. The
         * next settlement key acknowledges the prior finite receipt. Known
         * no-effect refusal retains the borrow. Unknown or contradictory effects
         * fence that row; no reset or positive cleanup erases this uncertainty.
         */
        error settled(consumer who,handle correlation,reference ref,bool acknowledge,communication::receipt result)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            if(!configured||!who||!bound(ref)||!result.request||result.provider_generation!=selected.provider_generation||
                result.key.session!=selected.endpoint||!result.key.sequence||!communication::known_status(result.status)||result.observed>effect::unknown)return error::denied;
            if(last.request&&result.key==last.key)
                return who==last_consumer&&correlation==last_correlation&&ref==last_reference&&acknowledge==last_ack&&
                    result.request==last.request&&result.status==last.status&&result.observed==last.observed?error::ok:error::stale;
            if(last.key.sequence==UINT64_MAX||result.key.sequence!=last.key.sequence+1)return error::stale;
            auto value=host(correlation);
            if(!value||value->who!=who||value->state!=phase::borrowed||value->borrowed!=ref)return error::denied;
            bool uncertain=result.observed==effect::unknown||(result.status==error::ok?result.observed!=effect::locally_accepted:result.observed!=effect::none);
            if(uncertain)value->state=phase::uncertain;
            else if(result.status==error::ok)
            {
                if(acknowledge)value->state=selected.direction==role::client?phase::complete:phase::delivered;
                else if(selected.direction==role::client){value->state=phase::waiting;value->borrowed={};}
                else *value={};
            }
            last=result;last_consumer=who;last_correlation=correlation;last_reference=ref;last_ack=acknowledge;return error::ok;
        }
        /** @brief Resolve an acknowledged server request for one trusted response enqueue.
         * @details This translation grants no permission and does not mark the
         * request sent. The caller must atomically pair response_pending with its
         * queue publication under the provider's serialization guard.
         */
        error response_target(consumer who,handle correlation,handle& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=host(correlation);
            if(!configured||retired||selected.direction!=role::server||!value||value->who!=who||value->state!=phase::delivered)return error::denied;
            output=value->native;return error::ok;
        }
        error response_pending(consumer who,handle correlation,handle request)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=host(correlation);
            if(!configured||retired||selected.direction!=role::server||!request||!value||value->who!=who||value->state!=phase::delivered)return error::denied;
            value->pending=request;value->state=phase::responding;return error::ok;
        }
        /** @brief Complete the exact queued response, retaining unknown outcomes. */
        error responded(consumer who,handle correlation,communication::native_result result)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=host(correlation);
            if(!value||value->who!=who||value->state!=phase::responding||selected.direction!=role::server)return error::denied;
            result=communication::checked_mutation(result,value->pending);
            if(result.request_identity!=value->pending||result.deferred||result.provider_generation!=selected.provider_generation||result.observed==effect::unknown)
            {value->state=phase::uncertain;return error::io;}
            value->pending=0;value->state=result.status==error::ok?phase::complete:phase::delivered;return error::ok;
        }
        /** @brief Reserve cancellation of local client interest in a known request.
         * @details Only an unborrowed waiting request has a usable native target.
         * A server request, unknown submission, response being copied or foreign
         * consumer cannot be discarded. The adapter must pair this transition
         * with its independent bounded queue, and roll back a pre-effect refusal
         * through abandoned(). No remote execution cancellation is implied.
         */
        error abandon_pending(consumer who,handle correlation,handle request,handle& native_target)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=host(correlation);
            if(!configured||retired||selected.direction!=role::client||!request||!value||value->who!=who)return error::denied;
            if(value->state!=phase::waiting||!value->native)return error::busy;
            value->pending=request;value->state=phase::abandoning;native_target=value->native;return error::ok;
        }
        /** @brief Apply exact local-interest cancellation evidence.
         * @details Known local acceptance permits row reclamation only after
         * its receipt is retained. Failure with no effect restores waiting;
         * malformed, deferred or unknown evidence fences the row permanently.
         * The receipt never asserts that remote execution stopped.
         */
        error abandoned(consumer who,handle correlation,communication::native_result result)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=host(correlation);
            if(!value||value->who!=who||value->state!=phase::abandoning||selected.direction!=role::client)return error::denied;
            result=communication::checked_mutation(result,value->pending);
            if(result.request_identity!=value->pending||result.deferred||result.provider_generation!=selected.provider_generation||result.observed==effect::unknown||
                (result.status==error::ok&&result.observed!=effect::locally_accepted))
            {value->state=phase::uncertain;return error::io;}
            value->pending=0;value->state=result.status==error::ok?phase::complete:phase::waiting;return error::ok;
        }
        /** @brief Reclaim only a known completed mapping after its outer receipt is released. */
        error release(consumer who,handle correlation)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};auto value=host(correlation);
            if(!value||value->who!=who)return error::denied;
            if(value->state!=phase::complete)return error::busy;
            *value={};return error::ok;
        }
        error inspect(consumer who,handle correlation,view& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex};
            for(auto const& value:rows)if(value.host==correlation&&value.state!=phase::free&&value.who==who)
            {output={value.host,value.native,value.pending,value.state};return error::ok;}
            return error::denied;
        }
        /** @brief Check the frozen native/host identity without granting a correlation. */
        bool matches(communication::owner authority,handle endpoint,handle native_endpoint,handle generation,role direction)const noexcept
        {
            if(!same_process())return false;
            std::lock_guard lock{mutex};
            return configured&&!retired&&selected.authority==authority&&selected.endpoint==endpoint&&selected.native_endpoint==native_endpoint&&
                selected.provider_generation==generation&&selected.direction==direction;
        }
        /// @brief Conservative occupancy, including completed or uncertain rows pending explicit reclamation.
        bool occupied()const noexcept
        {
            if(!same_process())return true;
            std::unique_lock lock{mutex,std::try_to_lock};if(!lock.owns_lock())return true;
            return std::any_of(rows.begin(),rows.end(),[](auto const& value){return value.state!=phase::free;});
        }
        void retire()noexcept{if(same_process()){std::lock_guard lock{mutex};retired=true;}}
    };
}
