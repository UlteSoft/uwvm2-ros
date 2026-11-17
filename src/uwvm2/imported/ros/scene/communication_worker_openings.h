/** @file
 * @brief Bounded supervisor for asynchronous isolated topic/service/action creation.
 * @details One immutable table owns exact-key creation receipts and canonical
 * retry bytes. Submission reserves and charges once before any native effect.
 * The existing supervisor calls pump(): it performs bounded nonblocking IPC,
 * with current complete node/context/schema authority around each send only.
 * No caller waits for a worker while retaining an authority or registry lock.
 *
 * A verified native endpoint identity remains private until typed adoption and
 * explicit publication under fresh authority. Request, native and guest resource
 * identities are distinct. Cancellation after any written byte retains unknown
 * effect and cleanup debt; cancelling unsent work cannot stop unrelated workers.
 * No timer, successful pipe write or process signal is proof of native cleanup.
 * Unknown receipts remain unreleasable even after positive namespace teardown.
 *
 * The object creates no thread or process. The shared transport and admission
 * callbacks must outlive it. Factories reserve all storage with recoverable
 * fast_io allocation before use; zero plans/budget grants no functionality.
 */
#pragma once
#include "communication_creation.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class openings
    {
    public:
        using error=communication::error;
        using effect=communication::effect;
        using handle=communication::handle;
        using admit=error(*)(void*,communication_creation::identity const&,group_lease&)noexcept;
    private:
        communication_creation::ledger ledger;
        communication_creation::charge charging{};admit admission{};void* context{};
        communication_creation::work pending;
        communication_worker_async::ticket ticket;
        std::array<std::uint8_t,8> reply{};
        mutable std::mutex pumping;
        bool have_ticket{},poisoned{},reconciled{};
        std::atomic<bool> configured{},stopping{};
        std::uint64_t generation{};
        pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        static communication_worker_protocol::operation operation(communication_open_contract::endpoint_kind kind)noexcept
        {
            using op=communication_worker_protocol::operation;
            return kind==communication_open_contract::endpoint_kind::topic?op::topic_open:
                kind==communication_open_contract::endpoint_kind::service?op::service_open:op::action_open;
        }
        void clear()noexcept{pending={};ticket={};have_ticket=reconciled=false;}
        error finish(communication_worker_async::transport& transport)noexcept
        {
            auto status=transport.acknowledge(ticket);
            if(status==error::again)return status;
            if(status!=error::ok){poisoned=true;transport.request_stop();return status;}
            clear();return error::ok;
        }
        /** @brief Cancel exact owned work and discard only a proven unsent transport frame. */
        error abandon(communication_worker_async::transport& transport)noexcept
        {
            communication_creation::view cancelled;
            auto status=ledger.cancel(pending.original.who,pending.request,cancelled);if(status!=error::ok)return status;
            if(have_ticket)
            {
                auto actual=transport.inspect();
                if(actual.current!=ticket){poisoned=true;return error::io;}
                if(actual.sent)
                {
                    // A partial request cannot be removed from a stream. The
                    // supervisor must retire this exact worker incarnation.
                    transport.request_stop();poisoned=true;return error::cancelled;
                }
                status=transport.discard(ticket);if(status==error::again)return status;
                if(status!=error::ok){poisoned=true;return status;}
            }
            clear();return error::ok;
        }
    public:
        openings()=default;openings(openings const&)=delete;openings& operator=(openings const&)=delete;
        ~openings(){retire();}
        /** @brief Freeze endpoint bindings and exact retry storage without native I/O.
         * @details All plans must use this exact verified worker generation.
         * Nodes use the separate staged worker lifecycle and are rejected here.
         * carrier_bytes is the ledger's aggregate two-carrier reservation; the
         * supervisor's fixed eight-byte result buffer has no dynamic allocator.
         */
        error configure(std::span<communication_creation::plan const> plans,std::uint64_t carrier_bytes,
                        admit revalidate,communication_creation::charge charge,void* user)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{pumping,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(configured||stopping.load())return error::denied;
            if(plans.empty()||!revalidate||!charge)return error::invalid;
            auto selected=plans.front().provider_generation;
            for(auto const& plan:plans)if(!selected||plan.provider_generation!=selected||plan.kind==communication_open_contract::endpoint_kind::node)return error::invalid;
            auto status=ledger.configure(plans,carrier_bytes);if(status!=error::ok)return status;
            admission=revalidate;charging=charge;context=user;generation=selected;configured=true;return error::ok;
        }
        /** @brief Own and enqueue one complete checked semantic carrier before charging.
         * @details Semantic decoding and full output preflight belong to the
         * public dispatcher. Only host-derived owners and actual caller/capability
         * origins may be passed here. An exact retry neither queues nor charges
         * twice. No worker launch, send, receive or waiting occurs in this call.
         */
        error enqueue(communication_creation::identity const& origin,wire::bytes canonical,communication_creation::view& result)noexcept
        {
            if(!same_process()||stopping.load()||!configured)return error::denied;
            return ledger.enqueue(origin,canonical,charging,context,result);
        }
        /** @brief Perform at most one bounded send/receive step for this supervisor.
         * @details Fresh full authority is held only through the nonblocking
         * ledger send, never while receiving or copying a reply. A foreign or
         * malformed reply retains unknown native debt. Other users of the same
         * transport may progress between completed tickets; no second channel
         * or implicit thread is created to escape its capacity reservation.
         */
        error pump(communication_worker_async::transport& transport)noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{pumping,std::try_to_lock};if(!lock.owns_lock())return error::again;
            if(!configured||poisoned)return error::cancelled;
            if(reconciled)return finish(transport);
            if(stopping.load())return pending.request?abandon(transport):error::cancelled;
            if(!pending.request)
            {
                auto status=ledger.next(pending);if(status!=error::ok)return status;
            }
            communication_creation::view current;
            auto status=ledger.inspect(pending.original.who,pending.request,current);if(status!=error::ok)return status;
            using phase=communication_open_contract::state;
            if(current.receipt.status==phase::cancelled||current.receipt.status==phase::failed)return abandon(transport);
            if(current.receipt.status==phase::queued||current.receipt.status==phase::sending)
            {
                if(!have_ticket)
                {
                    status=transport.stage(operation(pending.binding->kind),pending.binding->worker_binding,{},pending.original.deadline,ticket);
                    if(status==error::busy||status==error::again)return error::again;
                    if(status!=error::ok){auto removed=abandon(transport);return removed==error::ok?status:removed;}
                    have_ticket=true;
                    if(ticket.incarnation!=generation)
                    {auto removed=abandon(transport);return removed==error::ok?error::denied:removed;}
                }
                group_lease held;status=admission(context,pending.original,held);
                if(status!=error::ok){auto removed=abandon(transport);return removed==error::ok?status:removed;}
                status=ledger.send(pending.request,held,transport,ticket);held.release();
                if(status==error::ok||status==error::again)return status;
                auto removed=abandon(transport);return removed==error::ok?status:removed;
            }
            if(current.receipt.status!=phase::waiting||!have_ticket)return error::invalid;
            status=transport.receive(ticket);if(status==error::again)return status;
            communication_worker_async::result result;
            auto copied=transport.copy(ticket,reply,result);
            if(copied==error::again)return copied;
            handle native{};wire::reader input{{reply.data(),copied==error::ok?static_cast<std::size_t>(result.bytes):0}};
            bool valid=copied==error::ok&&communication::known_status(result.status)&&result.observed<=effect::unknown&&
                (result.status==error::ok?(result.bytes==8&&input.u64(native)&&native&&input.done()&&result.observed!=effect::unknown):
                    (result.bytes==0&&result.observed==effect::none));
            auto observed=valid?result.observed:effect::unknown;auto outcome=valid?result.status:error::io;
            status=ledger.complete(pending.request,outcome,observed,valid?native:0);
            if(status!=error::ok)
            {
                // Cancellation won the race. Known reply bytes do not publish
                // an endpoint or erase the cancellation's cleanup obligation.
                auto stopped=abandon(transport);return stopped==error::ok?status:stopped;
            }
            reconciled=true;return finish(transport);
        }
        error inspect(caller who,handle request,communication_creation::view& result)noexcept{return ledger.inspect(who,request,result);}
        error inspect_origin(caller who,handle request,communication_creation::view& result,communication_creation::identity& origin)noexcept
        {return ledger.inspect_origin(who,request,result,origin);}
        error provider_result(handle request,communication_creation::work& origin,handle& native)noexcept{return ledger.provider_result(request,origin,native);}
        error cancel(caller who,handle request,communication_creation::view& result)noexcept{return ledger.cancel(who,request,result);}
        error publish(caller who,handle request,group_lease const& current,handle resource,communication_open_contract::published& result)noexcept
        {return ledger.publish(who,request,current,resource,result);}
        error release(caller who,handle request)noexcept{return ledger.release(who,request);}
        error cleanup(handle request,bool confirmed)noexcept{return ledger.cleanup(request,confirmed);}
        /** @brief Require supervisor-owned positive cleanup; signalling does not free debt. */
        void request_stop()noexcept
        {if(same_process()){stopping.store(true,std::memory_order_release);ledger.retire();}}
        /// @brief Drain the short pump outside registry/authority locks before callback teardown.
        void retire()noexcept
        {if(same_process()){request_stop();std::lock_guard lock{pumping};}}
    };
#endif
}
