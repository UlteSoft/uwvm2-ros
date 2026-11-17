/** @file
 * @brief Fixed controller ownership, final-effect fencing and independent stop bookkeeping.
 * @details This core has no transport, actuator, clock thread or watchdog. The
 * integrating broker authenticates principals and immutable priority/lease policy;
 * the final native receiver must independently enforce the returned fencing.
 * Prepared ownership does not become live until its complete handle copy commits.
 * Expiry denies new effects; it does not assert that equipment stopped.
 *
 * A stop slot is independent of the sole normal-effect slot, including unknown
 * outcomes. Acknowledgement and independently witnessed stopped state are separate
 * facts. Rearm is a trusted management operation, never a guest import, capability
 * reopening or lifecycle activation. A process-local ledger is not durable crash
 * recovery: a real backend must reconstruct state before a replacement VM enables
 * commands. Every hot operation is fixed-storage and allocates nothing.
 */
#pragma once
#include "buffers.h"
#include <mutex>
namespace uwvm2::imported::ros::scene::control
{
    using error=resources::error;using handle=resources::handle;
    struct principal
    {handle environment{},grant{},generation{},authority_epoch{};explicit operator bool()const noexcept{return environment&&grant&&generation&&authority_epoch;}friend bool operator==(principal const&,principal const&)=default;};
    struct plan
    {std::uint64_t maximum_lease_ns{},prepare_ns{},maximum_priority{};bool preemption{};};
    /** @brief Host-created ownership receipt, never transferable authority. */
    struct lease
    {
        handle id{},epoch{},provider_generation{};principal owner;
        std::uint64_t priority{},expires{};
        friend bool operator==(lease const&,lease const&)=default;
    };
    struct request{principal owner;std::uint64_t priority{},duration_ns{};};
    struct prepared{handle ticket{};lease candidate;};
    enum class effect_result:std::uint8_t{no_effect,executed,unknown};
    struct status
    {lease controller;handle effect{},stop{},provider_generation{},stop_observation{};bool prepared{},outcome_unknown{},latched{},stop_acknowledged{};};
    class arbitration
    {
        mutable std::mutex mutex_;
        plan policy_;
        handle environment_{},generation_{},provider_{},epoch_{},pending_effect_{},pending_stop_{},stopped_observation_{};
        handle last_reconciliation_{},reserved_stop_{};
        lease active_;
        prepared staged_;
        handle staged_prior_{};std::uint64_t staged_expires_{};
        bool configured_{},retired_{},unknown_{},latched_{},acknowledged_{};
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static bool future(std::uint64_t time,std::uint64_t duration)noexcept
        {return time&&time<=INT64_MAX&&duration&&duration<=static_cast<std::uint64_t>(INT64_MAX)-time;}
        void clear_prepared()noexcept{staged_={};staged_prior_=staged_expires_=0;}
        error permitted(principal owner,lease const& claimed,std::uint64_t now)const noexcept
        {
            if(!configured_||retired_||!owner||owner.environment!=environment_||owner.generation!=generation_)return error::denied;
            if(latched_)return error::cancelled;
            if(!active_.id||claimed!=active_||owner!=active_.owner||claimed.provider_generation!=provider_)return error::conflict;
            if(!now||now>=active_.expires)return error::timeout;
            return error::success;
        }
    public:
        arbitration()=default;arbitration(arbitration const&)=delete;arbitration&operator=(arbitration const&)=delete;
        /** @brief Configure exact environment/boot ownership and explicit finite lease rules. */
        error configure(handle environment,handle generation,handle provider,plan policy)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(configured_||retired_)return error::denied;
            if(!environment||!generation||!provider||!policy.maximum_lease_ns||policy.maximum_lease_ns>INT64_MAX||!policy.prepare_ns||
                policy.prepare_ns>policy.maximum_lease_ns||policy.maximum_priority>UINT32_MAX)return error::invalid;
            auto stop_slot=resources::allocate_identity();if(!stop_slot)return error::too_large;
            reserved_stop_=stop_slot;environment_=environment;generation_=generation;provider_=provider;policy_=policy;configured_=true;epoch_=1;return error::success;
        }
        /** @brief Stage one exact principal without changing the current controller.
         * @details Priority comes only from a trusted immutable control profile.
         * Reopening the same active principal is not renewal. Expired ownership
         * cannot be renewed here; stop/reconciliation/rearm is a separate flow.
         * Prepared tickets have a finite deadline and may be explicitly discarded.
         */
        error prepare(request const& wanted,std::uint64_t now,prepared& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};
            if(!configured_||retired_||!wanted.owner||wanted.owner.environment!=environment_||wanted.owner.generation!=generation_)return error::denied;
            if(latched_)return error::cancelled;
            if(!future(now,wanted.duration_ns)||wanted.duration_ns>policy_.maximum_lease_ns||wanted.priority>policy_.maximum_priority||!future(now,policy_.prepare_ns))return error::invalid;
            if(staged_.ticket&&now>=staged_expires_)clear_prepared();
            if(staged_.ticket||pending_effect_)return error::again;
            if(active_.id)
            {
                if(now>=active_.expires)return error::timeout;
                if(active_.owner==wanted.owner)return error::conflict;
                if(!policy_.preemption||wanted.priority<=active_.priority)return error::denied;
            }
            if(epoch_==UINT64_MAX)return error::too_large;
            auto ticket=resources::allocate_identity(),id=resources::allocate_identity();if(!ticket||!id)return error::too_large;
            staged_={ticket,{id,epoch_+1,provider_,wanted.owner,wanted.priority,now+wanted.duration_ns}};staged_prior_=active_.id;staged_expires_=now+policy_.prepare_ns;
            output=staged_;return error::success;
        }
        /** @brief Commit only the exact prepared receipt after successful guest copy.
         * @details The caller holds the complete current control/device authority
         * through this commit. A failed commit discards this staging reservation;
         * the previous controller remains valid. An in-flight physical effect
         * cannot be preempted solely because its host submit returned.
         */
        error commit(prepared const& value,std::uint64_t now)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!configured_||retired_)return error::denied;
            if(!value.ticket||value.ticket!=staged_.ticket||value.candidate!=staged_.candidate)return error::bad_handle;
            auto result=latched_?error::cancelled:pending_effect_?error::again:
                !now||now>=staged_expires_||now>=staged_.candidate.expires||(active_.id&&now>=active_.expires)?error::timeout:
                active_.id!=staged_prior_||staged_.candidate.provider_generation!=provider_?error::conflict:error::success;
            if(result==error::success){active_=staged_.candidate;epoch_=active_.epoch;}
            clear_prepared();return result;
        }
        error discard(handle ticket)noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(!ticket||ticket!=staged_.ticket)return error::bad_handle;clear_prepared();return error::success;}
        /** @brief Observe current exact ownership without reserving an effect or renewing it. */
        error validate(principal owner,lease const& value,std::uint64_t now)const noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};return permitted(owner,value,now);}
        /** @brief Final normal-effect admission, not enqueue admission or permission creation. */
        error begin_effect(principal owner,lease const& value,std::uint64_t now,handle& effect)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto result=permitted(owner,value,now);if(result!=error::success)return result;
            if(pending_effect_)return error::again;
            auto id=resources::allocate_identity();if(!id)return error::too_large;pending_effect_=id;effect=id;return error::success;
        }
        /** @brief Record actual native cleanup; unknown outcomes retain their effect slot.
         * @details This is a trusted provider completion, never inferred from a
         * socket write. Failed/no-effect requires evidence that no effect occurred.
         * Unknown latches new commands and preemption until trusted reconciliation.
         */
        error finish_effect(handle effect,effect_result result)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!configured_||!effect||effect!=pending_effect_)return error::bad_handle;
            if(static_cast<unsigned>(result)>2)return error::invalid;
            if(unknown_)return error::conflict;
            if(result==effect_result::unknown){unknown_=latched_=true;clear_prepared();}
            else pending_effect_=0;
            return error::success;
        }
        /** @brief Reserve/coalesce a stop independently of every normal/unknown effect.
         * @details This only reserves bookkeeping; a separate qualified backend
         * must deliver and execute the fixed stop profile. Guest stop authority
         * or an independent deployment watchdog is checked outside this core. Its
         * identity is reserved before normal ownership can be enabled; rearm
         * must replenish that reservation before clearing the safety latch.
         */
        error stop(handle& id)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!configured_)return error::denied;
            if(pending_stop_){id=pending_stop_;return error::success;}
            if(!reserved_stop_)return error::conflict;
            auto next=std::exchange(reserved_stop_,0);
            latched_=true;clear_prepared();pending_stop_=next;acknowledged_=false;stopped_observation_=0;id=next;return error::success;
        }
        /** @brief Trusted acknowledgement is separate from independently observed stopped state. */
        error acknowledge_stop(handle provider,handle stop_id)noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(provider!=provider_||!stop_id||stop_id!=pending_stop_)return error::conflict;acknowledged_=true;return error::success;}
        error observe_stopped(handle provider,handle stop_id,handle trusted_observation)noexcept
        {
            if(!same_process()){return error::denied;}
            std::lock_guard lock{mutex_};
            if(provider!=provider_||!stop_id||stop_id!=pending_stop_||!trusted_observation)return error::conflict;
            stopped_observation_=trusted_observation;return error::success;
        }
        /** @brief Resolve one unknown effect from fresh trusted external state evidence.
         * @details Evidence IDs are host-side monotonically increasing records
         * from the bound receiver/state observer, never guest-provided integers.
         * Reconciliation frees the effect slot but deliberately retains the latch.
         */
        error reconcile_effect(handle provider,handle effect,handle evidence)noexcept
        {
            if(!same_process()){return error::denied;}
            std::lock_guard lock{mutex_};
            if(provider!=provider_||!unknown_||!effect||effect!=pending_effect_||!evidence||evidence<=last_reconciliation_)return error::conflict;
            last_reconciliation_=evidence;unknown_=false;pending_effect_=0;return error::success;
        }
        /** @brief Explicit trusted rearm after both stop acknowledgement and stopped evidence.
         * @details Not registered as an import. A backend unable to establish
         * independent stopped state must keep this core latched. Rearm clears the
         * previous controller instead of extending its authority or lease.
         */
        error rearm(handle provider,handle stop_id,handle observation)noexcept
        {
            if(!same_process()){return error::denied;}
            std::lock_guard lock{mutex_};
            if(retired_||provider!=provider_||!stop_id||stop_id!=pending_stop_||!acknowledged_||!observation||observation!=stopped_observation_)return error::denied;
            if(pending_effect_||unknown_)return error::again;
            if(epoch_==UINT64_MAX)return error::too_large;
            auto reserved=resources::allocate_identity();if(!reserved)return error::too_large;
            reserved_stop_=reserved;++epoch_;active_={};clear_prepared();pending_stop_=stopped_observation_=0;latched_=acknowledged_=false;return error::success;
        }
        /** @brief Reconnect fences old receipts while retaining unresolved effects and latch. */
        error provider_changed(handle next)noexcept
        {
            if(!same_process()){return error::denied;}
            std::lock_guard lock{mutex_};
            if(!configured_||!next||next<=provider_)return error::conflict;
            provider_=next;latched_=true;clear_prepared();stopped_observation_=0;acknowledged_=false;
            if(pending_effect_)unknown_=true;
            return error::success;
        }
        error retire()noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};retired_=latched_=true;clear_prepared();return error::success;}
        error inspect(status& output)const noexcept
        {
            if(!same_process()){return error::denied;}
            std::lock_guard lock{mutex_};if(!configured_)return error::denied;
            output={active_,pending_effect_,pending_stop_,provider_,stopped_observation_,bool(staged_.ticket),unknown_,latched_,acknowledged_};return error::success;
        }
    };
}
