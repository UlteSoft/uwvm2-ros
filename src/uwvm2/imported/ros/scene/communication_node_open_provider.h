/** @file
 * @brief Finite asynchronous node creation and independent receipt controls.
 * @details Configuration retains one exact host binding, its dispatcher and an
 * isolated provider lifetime. It performs no native work. node.open reserves and
 * charges one canonical creation receipt. Explicit supervisor pump() advances
 * one bounded startup step under fresh context/node authority; there is no hidden
 * thread, wait, retry after uncertainty, or native-token substitute.
 *
 * Successful startup produces opaque registry-node evidence. Creation take
 * publishes only that exact host resource under the original full scope. Cancel
 * of started work fences the retained provider's sessions without waiting.
 * For a paired worker deployment this includes both peer channels. The VM
 * supervisor drains its own users outside authority locks; it never signals or
 * reaps a native process. External attachment retirement retains remote cleanup
 * debt. Neither elapsed time nor logical node close proves native cleanup.
 * Unknown effects remain unknown even if external cleanup is later established.
 */
#pragma once
#include "communication_dispatch.h"
#include "communication_open_dispatch.h"
#include "communication_provider.h"
namespace uwvm2::imported::ros::scene
{
    class communication_node_open_provider final:public communication_open_dispatch::provider
    {
        using error=communication::error;
        using draft=communication_dispatch::opening;
        using contract=::uwvm2::imported::ros::communication_open_contract::endpoint_kind;
        std::shared_ptr<communication_dispatch> dispatch;
        std::shared_ptr<communication::provider_lifetime> lifetime;
        communication_creation::ledger ledger;
        communication_creation::plan policy;
        ::uwvm2::utils::container::std_string alias;
        mutable std::mutex mutex;
        std::mutex submitting,pumping;
        std::shared_ptr<draft> current;
        bool configured{};
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
        std::shared_ptr<draft> pending()noexcept
        {std::lock_guard lock{mutex};return current;}
        error own(caller who,handle request,communication_open_dispatch::observation& output)noexcept
        {
            if(!same_process()||!configured)return error::denied;
            auto status=ledger.inspect_origin(who,request,output.state,output.origin);
            if(status!=error::ok)return status;
            return output.origin.authority==policy.authority&&output.state.kind==contract::node?error::ok:error::denied;
        }
        static error charge(void* context,communication_creation::identity const& original,std::uint64_t bytes,group_lease& held)noexcept
        {return static_cast<error>(static_cast<communication_node_open_provider*>(context)->dispatch->admit_open(original,bytes,held,true));}
        static error enqueue(void* context,draft const& value,wire::bytes input,communication_creation::view& output)noexcept
        {
            auto& self=*static_cast<communication_node_open_provider*>(context);
            if(value.kind()!=contract::node||value.identity().authority!=self.policy.authority)return error::denied;
            std::shared_ptr<draft> retained;auto status=self.dispatch->retain_open(value.identity(),retained);
            if(status!=scene::error::success)return static_cast<error>(status);
            {std::lock_guard lock{self.mutex};self.current=std::move(retained);}
            return self.ledger.enqueue(value.identity(),input,charge,&self,output);
        }
        static error inspect_open(void* context,draft const& value,communication_creation::view& output)noexcept
        {return static_cast<communication_node_open_provider*>(context)->ledger.inspect(value.identity().who,value.request(),output);}
    public:
        /** @brief Configure one exact finite node ledger; defaults grant no operation.
         * @details The owner/profile must come from the bound dispatcher. The
         * sealed worker incarnation is supplied by trusted deployment, then
         * independently compared with actual node evidence on completion.
         * All fallible allocation precedes configuration publication.
         */
        error configure(::uwvm2::utils::container::std_string name,communication_creation::plan selected,
                        std::shared_ptr<communication_dispatch> target,std::shared_ptr<communication::provider_lifetime> worker)noexcept
        {
            if(!same_process()||!target||!worker||selected.kind!=contract::node)return error::denied;
            std::lock_guard lock{mutex};if(configured)return error::denied;
            try
            {
                communication::node_plan actual;auto status=target->describe_plan(name,actual);
                if(status!=scene::error::success||actual.authority!=selected.authority||actual.selected!=selected.selected)return error::denied;
                auto prepared=selected;auto configured_ledger=ledger.configure({&prepared,1},prepared.carrier_bytes*2);
                if(configured_ledger!=error::ok)return configured_ledger;
                alias=std::move(name);policy=std::move(prepared);dispatch=std::move(target);lifetime=std::move(worker);configured=true;return error::ok;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}
        }
        /** @brief Return the exact node creation receipt without native startup.
         * @details Failed output retains accepted work. Submissions are bounded
         * and reentrant calls return again. The guest receives no node handle
         * until a separately admitted communication.open.take succeeds.
         */
        scene::error invoke_open(caller who,handle cap,wire::bytes input,std::size_t capacity,std::size_t& required,
                                communication_dispatch::sink deliver,::uwvm2::utils::container::function_ref<bool()> cancelled={})noexcept
        {
            required=communication_open_contract::receipt_bytes;
            if(!same_process()||!configured)return scene::error::denied;
            std::unique_lock serial{submitting,std::try_to_lock};if(!serial.owns_lock())return scene::error::again;
            return dispatch->invoke_open(who,communication_contract::operation::node_open,cap,input,capacity,required,deliver,{this,enqueue,inspect_open},cancelled);
        }
        /** @brief Advance at most one admitted startup step; no guest polling loop or waiting.
         * @details The ledger marks potential native effects before startup.
         * A closed origin cancels and fences the provider; errors cannot reset
         * the same incarnation. Bounded pending work retains its original key
         * and deadline. Only actual typed success resolves startup uncertainty.
         */
        error pump()noexcept
        {
            if(!same_process()||!configured)return error::denied;
            std::unique_lock serial{pumping,std::try_to_lock};if(!serial.owns_lock())return error::again;
            auto value=pending();if(!value||!value->request())return error::again;
            communication_creation::view state;auto status=ledger.inspect(value->identity().who,value->request(),state);if(status!=error::ok)return status;
            using phase=communication_open_contract::state;
            if(state.receipt.status!=phase::queued&&state.receipt.status!=phase::sending&&state.receipt.status!=phase::waiting)return error::again;
            group_lease held;status=static_cast<error>(dispatch->admit_open(value->identity(),0,held));
            if(status==error::ok)
            {
                communication_creation::work work;status=ledger.begin(value->request(),held,work);
                if(status==error::busy)
                {
                    // Cancel may linearize after the cached phase observation
                    // and before begin. Report the retained terminal outcome,
                    // rather than an unrelated resource-capacity failure.
                    communication_creation::view actual;
                    if(ledger.inspect(value->identity().who,value->request(),actual)==error::ok&&actual.receipt.status==phase::cancelled)
                    {held.release();if(actual.receipt.cleanup_debt)lifetime->request_stop();return actual.receipt.result;}
                }
                if(status==error::ok)
                {
                    communication::node_attachment actual;status=static_cast<error>(dispatch->advance_node(value,actual));
                    if(status==error::again)return status;
                    if(status==error::ok)status=ledger.complete_node(value->request(),actual);
                    if(status==error::ok)return status;
                }
            }
            held.release();communication_creation::view cancelled;
            if(ledger.fail_node(value->identity().who,value->request(),status,cancelled)==error::ok&&cancelled.receipt.cleanup_debt)lifetime->request_stop();
            return status;
        }
        error inspect(caller who,handle request,communication_open_dispatch::observation& result)noexcept override{return own(who,request,result);}
        error validate(communication_creation::identity const& original)noexcept override
        {if(!same_process()||!configured||original.authority!=policy.authority)return error::denied;group_lease held;return static_cast<error>(dispatch->admit_open(original,0,held));}
        error prepare(caller who,handle request,handle& resource)noexcept override
        {
            communication_open_dispatch::observation result;auto status=own(who,request,result);if(status!=error::ok)return status;
            if(result.state.receipt.status!=communication_open_contract::state::ready)return error::busy;
            auto value=pending();if(!value||value->request()!=request||value->identity()!=result.origin||!value->resource())return error::denied;
            status=validate(result.origin);if(status==error::ok)resource=value->resource();return status;
        }
        error publish(caller who,handle request,group_lease const& held,handle resource,communication_open_contract::published& result)noexcept override
        {
            communication_open_dispatch::observation retained;auto status=own(who,request,retained);if(status!=error::ok)return status;
            auto value=pending();if(!value||value->request()!=request||value->resource()!=resource)return error::denied;
            status=ledger.publish(who,request,held,resource,result);return status==error::ok?static_cast<error>(dispatch->publish_open(value,result,held)):status;
        }
        error cancel(caller who,handle request,communication_creation::view& result)noexcept override
        {
            communication_open_dispatch::observation retained;auto status=own(who,request,retained);if(status!=error::ok)return status;
            if(retained.state.receipt.status==communication_open_contract::state::published)return error::busy;
            status=ledger.cancel(who,request,result);if(status==error::ok&&result.receipt.cleanup_debt)lifetime->request_stop();return status;
        }
        error release(caller who,handle request)noexcept override
        {
            communication_open_dispatch::observation retained;auto status=own(who,request,retained);if(status!=error::ok)return status;
            auto value=pending();if(!value||value->request()!=request)return error::denied;
            status=ledger.release(who,request);if(status!=error::ok)return status;
            status=ledger.inspect(who,request,retained.state);if(status!=error::ok)return status;
            return static_cast<error>(dispatch->release_open(value,retained.state));
        }
        /** @brief Fence this creation ledger when its supervisor or worker stops.
         * @details Call before draining the owning provider sessions. Every
         * accepted nonterminal request becomes a retained cancelled receipt, so
         * a stopped supervisor cannot leave take returning again indefinitely.
         * Enqueue and completion race with this fence under the ledger mutex:
         * late work cannot become accepted or ready. No native work, allocation,
         * guest delivery, authority debit or wait for process exit occurs here.
         * Uncertain effects and cleanup debt remain reserved; only the separate
         * reconcile_cleanup path can record trusted cleanup evidence. Existing
         * request.describe/cancel/release still require their own authority.
         */
        void request_stop()noexcept
        {if(same_process()&&configured)ledger.retire();}
        /** @brief Reconcile only after the retained provider confirms actual cleanup.
         * @details A disclosed external attachment currently cannot establish
         * remote cleanup through local retirement, so this returns busy. A new
         * trusted reconciliation protocol must qualify its evidence separately.
         * This method never waits, signals or reaps. Cancellation, connection
         * closure and expiry are insufficient. A published node must also have
         * closed; the ledger checks its actual registry evidence.
         */
        error reconcile_cleanup()noexcept
        {
            if(!same_process()||!configured)return error::denied;
            if(!lifetime->cleanup_confirmed())return error::busy;
            auto value=pending();if(!value||!value->request())return error::bad_handle;
            communication_creation::view state;auto status=ledger.inspect(value->identity().who,value->request(),state);if(status!=error::ok)return status;
            if(!state.published){status=static_cast<error>(dispatch->discard_private_node(value));if(status!=error::ok)return status;}
            return ledger.cleanup(value->request(),true);
        }
    };
}
