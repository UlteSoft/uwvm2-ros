/** @file
 * @brief Bounded directional device execution with independent stop transport.
 * @details This provider adapter owns four checked nonblocking endpoints: command
 * transmit/acknowledgement and stop transmit/acknowledgement. A trusted caller
 * holds the complete current authority cohort through each normal pump and
 * supplies a source-provenance validator. Queue admission, a completed write,
 * an authenticated protocol acknowledgement and independently observed stopping
 * are different facts. Only an exact generation/sequence/digest-bound command
 * acknowledgement resolves a normal effect. No transport reply rearms safety.
 *
 * Storage is fixed and reserved before activation. The stop lane runs before the
 * normal lane and has its own endpoints, framing, sequence, timeout and ACK
 * buffer. It continues after normal authority is revoked or normal quotas are
 * exhausted. Its authority comes from the explicitly configured deployment
 * safety contract, never a guest-supplied descriptor. This class is nonblocking;
 * an independent qualified receiver lease/watchdog is still required to protect
 * a machine when the VM or its scheduler cannot run.
 */
#pragma once
#include "device_channel.h"

namespace uwvm2::imported::ros::scene::devices
{
    class execution
    {
    public:
        /** @brief First observed terminal failure of one native session.
         * @details These are cached transport/admission observations, never
         * proof of a physical stop. Process exit/signal details belong to the
         * owning launcher. A failed session has no reset or automatic retry.
         */
        enum class failure:std::uint8_t{none,transport_lost,protocol_violation,deadline_expired,authority_lost,effect_unknown,stop_unconfirmed,provider_failed};
        inline static constexpr std::array<std::string_view,8> failure_names{
            "","transport.lost","protocol.violation","deadline.expired","authority.lost","effect.unknown","stop.unconfirmed","provider.failed"};
        struct fault_record
        {failure cause{};std::uint64_t observed_ns{};friend bool operator==(fault_record const&,fault_record const&)=default;};
        struct plan {std::size_t maximum_write{};std::uint64_t stop_ack_ns{};};
        struct status
        {
            handle request{},stop_request{},command_sequence{},stop_sequence{};
            channel::transmission command{},stop{};
            bool active{},unknown{},stop_unknown{},retired{},controller_ready{},stop_armed{};
            fault_record fault;
        };
    private:
        core& ledger_;control::arbitration& arbitration_;scalar_schema schema_;buffers::profile stop_profile_;
        mutable std::mutex mutex_;
        channel::sender command_{protocol::kind::command},stop_{protocol::kind::stop},controller_{protocol::kind::controller};
        channel::acknowledgements command_ack_,stop_ack_;
        plan policy_;work pending_;control::lease controller_lease_;handle effect_{},command_sequence_{},stop_sequence_{},stop_request_{};
        std::uint64_t stop_deadline_{};bool bound_{},unknown_{},stop_unknown_{},crossed_{},retired_{},controller_ready_{},controller_unknown_{},stop_armed_{};
        fault_record fault_;
        void remember_failure(failure cause,std::uint64_t time)noexcept
        {if(cause!=failure::none&&fault_.cause==failure::none)fault_={cause,time};}
#if defined(__linux__)
        pid_t process_{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        /** @brief Mark only proven zero-byte failure as no effect; uncertainty retains its ledger and pins. */
        error finish(completion result)noexcept
        {
            if(!pending_.ticket)return error::again;
            auto status=ledger_.finish(pending_.ticket,result);
            if(effect_)
            {
                auto arb=arbitration_.finish_effect(effect_,result==completion::executed?control::effect_result::executed:
                    result==completion::unknown?control::effect_result::unknown:control::effect_result::no_effect);
                if(status==error::success)status=arb;
            }
            if(result==completion::unknown)
            {unknown_=true;handle stop{};(void)arbitration_.stop(stop);}
            else
            {pending_={};effect_=0;crossed_=false;(void)command_.discard_unwritten();}
            return status;
        }
        error refuse(error why,failure cause=failure::none,std::uint64_t time=0)noexcept
        {
            remember_failure(cause,time);
            channel::send_status sent;command_.inspect(sent);
            (void)finish(sent.written?completion::unknown:completion::no_effect);
            receipt stopped;(void)ledger_.safety_stop(reason::stopped,stopped);handle stop{};(void)arbitration_.stop(stop);
            return why;
        }
        error refuse_controller(error why,failure cause=failure::none,std::uint64_t time=0)noexcept
        {
            remember_failure(cause,time);
            channel::send_status sent;controller_.inspect(sent);
            controller_unknown_|=sent.written&&!controller_ready_;
            // Controller cancellation precedes the normal ACK pump. Retire
            // any claimed command here as well: after even one written byte
            // its effect is unknown, including an explicit (nonfault) stop.
            // Otherwise a live command would remain running indefinitely and
            // its arbitration effect could never be reconciled. A stop ACK
            // cannot establish the outcome of this earlier command.
            if(pending_.ticket&&!unknown_)
            {
                channel::send_status command;command_.inspect(command);
                (void)finish(command.written?completion::unknown:completion::no_effect);
            }
            receipt stopped;(void)ledger_.safety_stop(why==error::timeout?reason::deadline:reason::stopped,stopped);
            handle stop{};(void)arbitration_.stop(stop);return why;
        }
        template<class Validator>error pump_controller(std::uint64_t time,Validator&& validate)noexcept
        {
            if(controller_unknown_)return error::conflict;
            if(!controller_lease_.id)return error::again;
            auto status=arbitration_.validate(controller_lease_.owner,controller_lease_,time);
            if(retired_||status!=error::success)return refuse_controller(retired_?error::cancelled:status,
                // An explicitly latched stop is expected cancellation, not a
                // native-provider fault. Preserve the granted stop/inspection
                // path; a genuine transport fault keeps its earlier record.
                retired_||status==error::cancelled?failure::none:status==error::timeout?failure::deadline_expired:failure::authority_lost,time);
            if(controller_ready_)return error::success;
            channel::send_status sent;controller_.inspect(sent);
            if(sent.state==channel::transmission::queued||sent.state==channel::transmission::partial)
            {
                work installation;installation.controller=controller_lease_;
                try{status=validate(installation,time);}catch(...){status=error::denied;}
                if(status==error::again)return status;
                if(status!=error::success)return refuse_controller(status);
                status=controller_.pump(time);if(status!=error::success&&status!=error::again)return refuse_controller(status,
                    status==error::timeout?failure::deadline_expired:failure::transport_lost,time);
            }
            controller_.inspect(sent);if(sent.state!=channel::transmission::awaiting_ack)return error::again;
            status=command_ack_.pump();if(status!=error::success&&status!=error::again)return refuse_controller(status,
                status==error::invalid?failure::protocol_violation:failure::transport_lost,time);
            protocol::packet response;status=command_ack_.copy(response);if(status!=error::success)return status;
            protocol::acknowledgement result{};status=controller_.acknowledge(response.view(),1,result);
            if(status!=error::success||result!=protocol::acknowledgement::controller_installed)return refuse_controller(error::denied,failure::protocol_violation,time);
            (void)command_ack_.consume();controller_ready_=true;return error::success;
        }
        /** @brief Pump the reserved stop once; stop acceptance never fabricates an independent stopped witness. */
        error pump_stop(std::uint64_t time)noexcept
        {
            if(!stop_armed_)return error::denied;
            receipt wanted;if(ledger_.pending_stop(wanted)!=error::success)return error::again;
            if(stop_unknown_)return error::conflict;
            if(wanted.status==state::succeeded)return error::success;
            if(!stop_request_)
            {
                if(!time||time>INT64_MAX-policy_.stop_ack_ns||stop_sequence_==UINT64_MAX)return error::too_large;
                protocol::packet packet;auto status=protocol::stop(wanted,stop_profile_,stop_sequence_+1,packet);
                if(status!=error::success)return status;
                status=stop_.stage(packet);if(status!=error::success)return status;
                stop_request_=wanted.request;stop_deadline_=time+policy_.stop_ack_ns;
            }
            if(time>=stop_deadline_)
            {remember_failure(failure::stop_unconfirmed,time);stop_unknown_=true;(void)ledger_.stop_failed(wanted.provider_generation,wanted.request);return error::timeout;}
            channel::send_status sent;stop_.inspect(sent);
            if(sent.state==channel::transmission::queued||sent.state==channel::transmission::partial)
            {
                auto status=stop_.pump(time);if(status!=error::success&&status!=error::again)
                {remember_failure(failure::stop_unconfirmed,time);stop_unknown_=true;(void)ledger_.stop_failed(wanted.provider_generation,wanted.request);return status;}
            }
            stop_.inspect(sent);if(sent.state!=channel::transmission::awaiting_ack)return error::again;
            auto status=stop_ack_.pump();if(status!=error::success&&status!=error::again)
            {remember_failure(failure::stop_unconfirmed,time);stop_unknown_=true;(void)ledger_.stop_failed(wanted.provider_generation,wanted.request);return status;}
            protocol::packet packet;status=stop_ack_.copy(packet);if(status!=error::success)return status;
            protocol::acknowledgement outcome{};status=stop_.acknowledge(packet.view(),stop_sequence_+1,outcome);
            if(status!=error::success||(outcome!=protocol::acknowledgement::stop_accepted&&outcome!=protocol::acknowledgement::stopped))
            {remember_failure(failure::stop_unconfirmed,time);stop_unknown_=true;(void)ledger_.stop_failed(wanted.provider_generation,wanted.request);return error::io;}
            ++stop_sequence_;(void)stop_ack_.consume();status=ledger_.acknowledge_stop(wanted.provider_generation,wanted.request);
            control::status controller;if(arbitration_.inspect(controller)==error::success&&controller.stop)
                (void)arbitration_.acknowledge_stop(wanted.provider_generation,controller.stop);
            return status;
        }
    public:
        execution(core& ledger,control::arbitration& arbitration):ledger_(ledger),arbitration_(arbitration){}
        execution(execution const&)=delete;execution& operator=(execution const&)=delete;
        ~execution(){retire();}
        /** @brief Trusted activation duplicates four explicit endpoints and rejects shared lanes.
         * @details Descriptor validation performs no command write. Distinct PTY
         * pairs/pipes prevent normal-channel backpressure from occupying stop
         * framing. Deployment still reserves independent receiver execution and
         * verifies its lease/watchdog; descriptor separation alone cannot do so.
         */
        error bind(int commands,int command_ack,int stops,int stop_ack,scalar_schema const& schema,
                   buffers::profile const& stop_profile,plan policy)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(bound_||retired_)return error::denied;
            if(!valid(schema)||!valid_text(stop_profile.identity)||!policy.maximum_write||policy.maximum_write>4096||
                !policy.stop_ack_ns||policy.stop_ack_ns>INT64_MAX)return error::invalid;
#if defined(__linux__)
            for(auto fd:{commands,command_ack,stops,stop_ack})
            {
                ::termios mode{};if(::tcgetattr(fd,&mode)==0&&
                    ((mode.c_lflag&(ICANON|ECHO|ISIG|IEXTEN))||(mode.c_iflag&(BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON|IXOFF))||
                    (mode.c_oflag&OPOST)||(mode.c_cflag&CSIZE)!=CS8))return error::denied;
            }
#endif
            auto status=command_.bind(commands,policy.maximum_write);if(status!=error::success)return status;
            status=controller_.bind(commands,policy.maximum_write);if(status!=error::success)return status;
            status=command_ack_.bind(command_ack);if(status!=error::success)return status;
            status=stop_.bind(stops,policy.maximum_write);if(status!=error::success)return status;
            status=stop_ack_.bind(stop_ack);if(status!=error::success)return status;
            std::array sources{command_.source(),command_ack_.source(),stop_.source(),stop_ack_.source()};
            for(std::size_t i{};i<sources.size();++i)for(std::size_t j{};j<i;++j)
                if(sources[i]==sources[j])return error::denied;
            schema_=schema;stop_profile_=stop_profile;policy_=policy;bound_=true;return error::success;
        }
        /** @brief Stage one committed controller for actual receiver installation.
         * @details This does not write, renew, preempt or create authority. Each
         * native session accepts exactly one controller; a new session requires
         * an explicit stop/reconciliation and a new provider generation. Commands
         * remain queued until the receiver's exact installation ACK arrives.
         */
        error stage_controller(control::lease const& lease,std::uint64_t time)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!bound_||retired_)return error::denied;
            if(controller_lease_.id)return error::conflict;
            auto status=arbitration_.validate(lease.owner,lease,time);if(status!=error::success)return status;
            protocol::packet frame;status=protocol::controller(lease,schema_,stop_profile_,1,frame);if(status!=error::success)return status;
            status=controller_.stage(frame);if(status==error::success){controller_lease_=lease;stop_armed_=true;}return status;
        }
        /** @brief Enable only the fixed stop lane after explicit guest stop admission.
         * @details An unused prepared binding does not transmit on destruction.
         * Once a controller is staged its stop reservation is armed permanently
         * for this native session, independently of later guest revocation.
         */
        error arm_stop()noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(!bound_)return error::denied;stop_armed_=true;return error::success;}
        /** @brief At most one bounded step per lane with current source/controller checks.
         * @details validate is host-only and runs under this adapter's private
         * lock. It must not reenter the adapter. The integrating provider holds
         * authority before entering here; validation may inspect exact sources,
         * but cannot wait or grant authority. Failure after any byte write is
         * unknown. ACK cleanup and the independent stop never require more normal
         * budget. No new command is admitted after an unresolved effect.
         */
        template<class Validator>error pump(std::uint64_t time,Validator&& validate)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!bound_)return error::denied;
            (void)pump_stop(time);
            if(unknown_)return error::conflict;
            if(retired_&&!pending_.ticket)return error::cancelled;
            auto installed=pump_controller(time,validate);if(installed!=error::success)return installed;
            if(!pending_.ticket)
            {
                auto status=ledger_.claim(time,pending_);if(status!=error::success)return status;
                if(command_sequence_==UINT64_MAX)return refuse(error::too_large);
                protocol::packet packet;status=protocol::command(pending_,schema_,command_sequence_+1,packet);
                if(status!=error::success)return refuse(status);
                status=command_.stage(packet);if(status!=error::success)return refuse(status);
            }
            channel::send_status sent;command_.inspect(sent);
            if(!time||time>=pending_.deadline||time>=pending_.controller.expires)return refuse(error::timeout,failure::deadline_expired,time);
            if(sent.state==channel::transmission::queued||sent.state==channel::transmission::partial)
            {
                error status=error::denied;try{status=validate(std::as_const(pending_),time);}catch(...){status=error::denied;}
                if(!retired_&&status==error::again)return status;
                if(retired_||status!=error::success)return refuse(retired_?error::cancelled:status);
                status=arbitration_.validate(pending_.controller.owner,pending_.controller,time);if(status!=error::success)return refuse(status);
                if(!crossed_)
                {
                    status=arbitration_.begin_effect(pending_.controller.owner,pending_.controller,time,effect_);if(status!=error::success)return refuse(status);
                    status=ledger_.cross_effect(pending_.ticket,time);if(status!=error::success)return refuse(status);crossed_=true;
                }
                status=command_.pump(time);if(status!=error::success&&status!=error::again)return refuse(status,
                    status==error::timeout?failure::deadline_expired:failure::transport_lost,time);
            }
            command_.inspect(sent);if(sent.state!=channel::transmission::awaiting_ack)return error::again;
            auto status=command_ack_.pump();if(status!=error::success&&status!=error::again)return refuse(status,
                status==error::invalid?failure::protocol_violation:failure::transport_lost,time);
            protocol::packet packet;status=command_ack_.copy(packet);if(status!=error::success)return status;
            protocol::acknowledgement outcome{};status=command_.acknowledge(packet.view(),command_sequence_+1,outcome);
            if(status!=error::success)return refuse(status,failure::protocol_violation,time);
            ++command_sequence_;(void)command_ack_.consume();
            return finish(outcome==protocol::acknowledgement::executed?completion::executed:completion::no_effect);
        }
        /** @brief Quiesce commands and reserve stop; a scheduler must continue pumping stop until its bounded deadline. */
        void retire()noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex_};if(retired_)return;retired_=true;
            if(!bound_)return;
            if(pending_.ticket&&!unknown_)(void)refuse(error::cancelled);
            receipt stopped;(void)ledger_.safety_stop(reason::retired,stopped);handle id{};(void)arbitration_.stop(id);
        }
        /** @brief Record an owning supervisor's native failure before permanent retirement.
         * @details The caller must already hold the complete authority fence.
         * This performs no signal delivery, wait, allocation or physical effect.
         * A partial command remains unknown; independent stop pumping continues.
         */
        void fail(std::uint64_t time)noexcept
        {
            if(!same_process())return;
            {std::lock_guard lock{mutex_};remember_failure(failure::provider_failed,time);}
            retire();
        }
        error inspect(status& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_,std::try_to_lock};if(!lock.owns_lock())return error::again;
            channel::send_status command,stop;command_.inspect(command);stop_.inspect(stop);
            output={pending_.result.request,stop_request_,command_sequence_,stop_sequence_,command.state,stop.state,
                bound_,unknown_||controller_unknown_,stop_unknown_,retired_,controller_ready_,stop_armed_,fault_};return error::success;
        }
    };
}
