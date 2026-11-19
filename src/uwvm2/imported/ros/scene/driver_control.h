/** @file
 * @brief Receiver-side ownership fencing for one externally controlled actuator.
 * @details This object lives in the external trusted receiver, never inside a
 * vendor library or independently in each VM. All channels for the SAME physical
 * resource must use the SAME instance. A deployment registers exact protocol
 * bindings and observed credentials for the owning deployment, freezes the
 * registry and explicitly grants control. Registration alone grants nothing.
 * Management methods are C++ integration points outside the client protocol.
 *
 * The receiver first authenticates kernel credentials and validates the complete
 * canonical command, physical bounds, units and dependencies. Immediately before
 * an effect it calls begin() with that channel's opaque registration, the decoded
 * header and controller epoch. Payload validation is not replaced by this gate.
 * The existing arbitration core reserves at most one normal effect globally.
 * Lost ownership, expiry, unknown outcomes and trusted stop evidence are separate
 * states; disconnect/release is never proof of physical stop. No process, cgroup,
 * namespace, vendor ABI, watchdog or deployment lifecycle is implemented here.
 *
 * The registry has an explicit finite entry limit, uses recoverable fast_io
 * allocation during setup and allocates nothing during ordinary admission. Its
 * mutex always precedes the arbitration mutex. No callbacks execute under either.
 */
#pragma once
#include "driver_channel.h"
#include "control_arbitration.h"
namespace uwvm2::imported::ros::scene::drivers
{
    class receiver_control
    {
        struct entry
        {
            resources::handle id{};protocol::binding origin;credentials peer;
            control::principal principal;std::uint64_t until{},request_ns{},sequence{};bool released{};
        };
        mutable std::mutex mutex_;
        ::uwvm2::utils::container::std_vector<entry> entries_;
        control::arbitration arbiter_;
        // Written only under mutex_ after a successful trusted grant/rearm.
        // This avoids an extra arbiter inspection; begin_effect still performs
        // the authoritative expiry, owner, latch and pending-effect checks.
        control::lease active_;
        protocol::identity driver_;std::array<std::uint64_t,4> contract_{};
        resources::handle environment_{},generation_{},provider_{},pending_route_{},pending_effect_{};
        std::size_t maximum_{};bool configured_{},frozen_{};
#if defined(__linux__)
        ::pid_t process_{::getpid()};
#endif
        protocol::identity deployment_;
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        entry* find(resources::handle id)noexcept
        {for(auto& row:entries_)if(row.id==id)return &row;return nullptr;}
    public:
        receiver_control()=default;receiver_control(receiver_control const&)=delete;
        receiver_control& operator=(receiver_control const&)=delete;
        /** @brief Establish the actuator's owning deployment, driver contract and lease policy.
         * @details The deployment identity and provider incarnation come from
         * trusted provisioning. A channel cannot select the owning deployment.
         * Fresh construction does not recover a crashed receiver's unknown effects;
         * a replacement must remain ungranted until external reconciliation.
         */
        error configure(protocol::identity deployment,protocol::identity driver,std::array<std::uint64_t,4> contract,
                        resources::handle provider,std::size_t sessions,control::plan policy)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(configured_)return error::conflict;
            if(!protocol::present(deployment)||!protocol::present(driver)||!provider||!sessions||
               !std::any_of(contract.begin(),contract.end(),[](auto value){return value!=0;}))return error::invalid;
            auto environment=resources::allocate_identity(),generation=resources::allocate_identity();
            if(!environment||!generation)return error::too_large;
            auto result=arbiter_.configure(environment,generation,provider,policy);if(result!=error::success)return result;
            deployment_=deployment;driver_=driver;contract_=contract;provider_=provider;maximum_=sessions;environment_=environment;generation_=generation;configured_=true;return error::success;
        }
        /** @brief Register one exact authenticated session without conferring ownership.
         * @details Several VMs in the owning deployment may share a resource.
         * The receiver assigns its own route handles, refuses duplicate sessions
         * and rejects other deployments before reserving a slot or a handle.
         */
        error bind(protocol::binding const& origin,credentials peer,std::uint64_t until,
                   std::uint64_t request_ns,resources::handle& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!configured_||frozen_)return error::denied;
            if(!protocol::valid(origin)||origin.driver!=driver_||origin.contract!=contract_||!peer.pid||
               peer.pid>INT32_MAX||peer.uid>UINT32_MAX||peer.gid>UINT32_MAX||!until||until>INT64_MAX||!request_ns||request_ns>INT64_MAX)return error::invalid;
            if(origin.deployment!=deployment_)return error::denied;
            for(auto const& row:entries_)if(row.origin.session==origin.session)return error::conflict;
            if(entries_.size()>=maximum_)return error::too_large;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            try{entries_.push_back({id,origin,peer,{environment_,id,generation_,1},until,request_ns,0,false});}
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
            output=id;return error::success;
        }
        error freeze()noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(!configured_||frozen_)return error::denied;frozen_=true;return error::success;}
        /** @brief Trusted ownership grant or policy-permitted handover; never guest-callable.
         * @details Ownership cannot outlive the registered session. Existing
         * unknown/in-flight effects prevent handover. Lease expiry requires the
         * independent stop/reconcile/rearm sequence before another grant.
         */
        error grant(resources::handle route,std::uint64_t priority,std::uint64_t duration,
                    std::uint64_t now,control::lease& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto* row=find(route);
            if(!frozen_||!row||row->released)return error::denied;
            if(!now||now>=row->until||!duration||duration>row->until-now)return error::timeout;
            control::prepared staged;auto result=arbiter_.prepare({row->principal,priority,duration},now,staged);
            if(result!=error::success)return result;result=arbiter_.commit(staged,now);
            if(result==error::success){active_=staged.candidate;output=active_;}return result;
        }
        /** @brief Last shared ownership check before one fully validated physical effect.
         * @details route is selected by the authenticated endpoint, not payload.
         * observed must be the kernel's packet credentials. Controller epochs
         * originate at this receiver, never from a VM-local arbiter. A fresh
         * authenticated request consumes its sequence even when ownership is
         * denied; replay never causes a second effect. Error replies must still
         * use the canonical result shape. Malformed identity/deadlines do not
         * advance state. The caller must hold the gate's effect until completion.
         */
        error begin(resources::handle route,credentials observed,protocol::header const& request,
                    std::size_t payload_bytes,std::uint64_t controller_epoch,std::uint64_t now,
                    resources::handle& effect)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};auto* row=find(route);
            if(!frozen_||!row||row->released||observed!=row->peer||request.bound!=row->origin)return error::denied;
            if(request.type!=protocol::kind::request||!protocol::valid(request,payload_bytes))return error::invalid;
            if(!now||now>=row->until||request.deadline<=now||request.deadline>row->until||request.deadline-now>row->request_ns)return error::timeout;
            if(request.sequence!=row->sequence+1)return error::conflict;
            row->sequence=request.sequence;
            if(!controller_epoch||controller_epoch!=active_.epoch)return error::conflict;
            if(request.deadline>active_.expires)return error::timeout;
            resources::handle next{};auto result=arbiter_.begin_effect(row->principal,active_,now,next);
            if(result==error::success){pending_route_=route;pending_effect_=next;effect=next;}return result;
        }
        /** @brief Record receiver-observed completion; unknown preserves the global slot. */
        error finish(resources::handle route,resources::handle effect,control::effect_result outcome)noexcept
        {
            if(!same_process())return error::denied;std::lock_guard lock{mutex_};
            if(route!=pending_route_||!effect||effect!=pending_effect_)return error::denied;
            auto result=arbiter_.finish_effect(effect,outcome);
            if(result==error::success&&outcome!=control::effect_result::unknown)pending_route_=pending_effect_=0;return result;
        }
        /** @brief Retire only this origin; lost active control latches the common actuator.
         * @details Transport loss cannot imply no effect. An in-flight request
         * becomes unknown. The stop slot is bookkeeping for an independent
         * qualified stop lane; this method does not send or claim a physical stop.
         */
        error release(resources::handle route)noexcept
        {
            if(!same_process())return error::denied;std::lock_guard lock{mutex_};auto* row=find(route);
            if(!frozen_||!row)return error::denied;if(row->released)return error::success;row->released=true;
            control::status state;auto result=arbiter_.inspect(state);if(result!=error::success)return result;
            if(pending_route_==route&&!state.outcome_unknown)(void)arbiter_.finish_effect(pending_effect_,control::effect_result::unknown);
            if(state.controller.owner==row->principal){resources::handle stop{};return arbiter_.stop(stop);}return error::success;
        }
        /** @brief Trusted stop reconciliation requires BOTH stop evidence and effect evidence.
         * @details Inputs must be independently authenticated observations; no
         * wire request reaches this method. A caller cannot clear unknown effects
         * merely by acknowledging a stop. Rearm leaves every client ungranted.
         */
        error reconcile_stopped(resources::handle stop,resources::handle observation,resources::handle effect_evidence)noexcept
        {
            if(!same_process())return error::denied;std::lock_guard lock{mutex_};control::status state;
            auto result=arbiter_.inspect(state);if(result!=error::success)return result;
            if(!stop||stop!=state.stop||!observation||!state.latched||
               (state.outcome_unknown&&!effect_evidence)||(!state.outcome_unknown&&effect_evidence))return error::denied;
            if(state.effect&&!state.outcome_unknown)return error::again;
            if(state.outcome_unknown){result=arbiter_.reconcile_effect(provider_,state.effect,effect_evidence);if(result!=error::success)return result;pending_route_=pending_effect_=0;}
            result=arbiter_.acknowledge_stop(provider_,stop);if(result!=error::success)return result;
            result=arbiter_.observe_stopped(provider_,stop,observation);if(result!=error::success)return result;
            result=arbiter_.rearm(provider_,stop,observation);if(result==error::success)active_={};return result;
        }
        error stop(resources::handle& ticket)noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(!frozen_)return error::denied;return arbiter_.stop(ticket);}
        /** @brief Detached local control state; this is not a guest disclosure grant. */
        error inspect(control::status& output)const noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};return arbiter_.inspect(output);}
    };
}
