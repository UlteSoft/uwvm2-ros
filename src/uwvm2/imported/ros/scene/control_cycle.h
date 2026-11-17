/** @file
 * @brief One complete reserved control sample/write phase over an actual device ledger.
 * @details This host-only component binds an already committed exact controller
 * lease and an actual open device. It creates no authority, transport or thread.
 * The caller must reserve optional budget for a complete cycle before sample;
 * submit/result/stop within that admitted phase must not acquire a second rate
 * allowance. Permission and native-effect fencing are always checked anew by
 * the integrating provider. Capacity is fixed and independent of attenuation.
 *
 * Sample retains real sealed observation pins before exposing sequence/deadline.
 * Failed copies release the reservation without advancing sequence. Submit uses
 * that exact phase, revalidates current source proofs outside this mutex, then
 * submits to the real device ledger. Exact successful keys recover lost receipt
 * copies without duplicating work. Unknown outcomes and missed phases latch the
 * cycle and reserve both independent controller/device stop records. These are
 * bookkeeping obligations, not physical stop evidence or an autonomous watchdog.
 */
#pragma once
#include "device_core.h"
namespace uwvm2::imported::ros::scene::control
{
    class cycle
    {
    public:
        struct plan{std::uint64_t write_window_ns{};};
        enum class phase:std::uint8_t{idle,sampled,submitted,completed,latched};
        struct sample_record
        {handle cycle{},sequence{};std::uint64_t captured_ns{},deadline{},missed{};std::array<devices::observation,devices::maximum_observations> observations{};std::size_t count{};};
        struct status
        {handle cycle{},sequence{},request{},controller{},controller_epoch{},provider_generation{};std::uint64_t deadline{},missed{};phase current{};bool busy{},retired{};};
    private:
        arbitration& arbitration_;devices::core& device_;
        mutable std::mutex mutex_;
        principal owner_;devices::owner device_owner_;handle device_handle_{},id_{};
        lease controller_;plan policy_;devices::plan device_policy_;
        sample_record sample_;devices::observations evidence_;
        handle last_key_{},submitted_sequence_{},request_{};buffers::digest last_digest_{};devices::receipt submitted_,last_result_;
        phase phase_{};bool configured_{},busy_{},retired_{};
        std::uint64_t(*clock_)()noexcept;
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
        bool owned(principal value)const noexcept{return configured_&&value==owner_;}
        static std::uint64_t steady()noexcept{auto value=resources::steady_now();return value>0?static_cast<std::uint64_t>(value):0;}
        void release_evidence()noexcept{evidence_={};}
        void latch(devices::reason why)noexcept
        {
            phase_=phase::latched;handle stop{};arbitration_.stop(stop);devices::receipt receipt;device_.safety_stop(why,receipt);
            if(!busy_)release_evidence();
        }
        error valid_observations(devices::observations const& inputs,std::uint64_t now)const noexcept
        {
            if(inputs.count<device_policy_.minimum_observations||inputs.count>device_policy_.observations)return error::denied;
            for(std::size_t index{};index<inputs.count;++index)
            {
                auto const& value=inputs.values[index];if(!value.source||!value.source_generation||!value.capture||!value.authority_epoch||!value.buffer||
                    !value.captured_ns||value.captured_ns>now||now-value.captured_ns>device_policy_.maximum_age_ns||
                    value.frame!=device_policy_.schema.frame||inputs.pins[index].identity()!=value.buffer)return error::denied;
                for(std::size_t previous{};previous<index;++previous)if(inputs.values[previous].source==value.source)return error::invalid;
                auto result=inputs.pins[index].visit([&](auto,auto const&,auto const& digest)noexcept{return digest==value.digest;});if(result!=error::success)return result;
            }
            return error::success;
        }
        error refresh()noexcept
        {
            if(!request_)return error::again;
            auto result=device_.result(device_owner_,device_handle_,request_,last_result_);if(result!=error::success)return result;
            if(last_result_.status==devices::state::outcome_unknown){latch(devices::reason::stopped);return error::success;}
            if(last_result_.status==devices::state::succeeded||last_result_.status==devices::state::failed||last_result_.status==devices::state::cancelled)
                if(phase_!=phase::latched)phase_=phase::completed;
            return error::success;
        }
        static buffers::digest fingerprint(handle key,handle sequence,buffers::profile const& schema,wire::bytes body)noexcept
        {
            std::array<std::uint8_t,devices::maximum_command_bytes+256> storage;wire::writer out{storage};
            if(!out.u64(key)||!out.u64(sequence)||!out.text(schema.identity.view())||!out.blob(schema.sha256)||!out.blob(body))return {};
            return buffers::hash(out.written());
        }
    public:
        explicit cycle(arbitration& controller,devices::core& device,std::uint64_t(*clock)()noexcept=steady)
            :arbitration_(controller),device_(device),clock_(clock?clock:steady){}
        /** @brief Bind actual controller and device state after their independent grants/activation.
         * @details This never opens either resource or renews an expired lease.
         * Hardware-capable deployment must establish an independent watchdog
         * before constructing an enabled control binding around this core.
         */
        error configure(principal owner,devices::owner device_owner,handle device,lease const& controller,plan policy,std::uint64_t now)noexcept
        {
            if(!same_process())return error::denied;
            auto status=arbitration_.validate(owner,controller,now);if(status!=error::success)return status;
            devices::plan actual;handle provider{},next{};status=device_.observe_submission(device_owner,device,actual,provider,next);if(status!=error::success)return status;
            if(!policy.write_window_ns||policy.write_window_ns>actual.maximum_deadline_ns||provider!=controller.provider_generation||
                owner.environment!=device_owner.environment||owner.generation!=device_owner.generation)return error::invalid;
            std::lock_guard lock{mutex_};if(configured_||retired_)return error::denied;
            auto id=resources::allocate_identity();if(!id)return error::too_large;
            owner_=owner;device_owner_=device_owner;device_handle_=device;controller_=controller;policy_=policy;device_policy_=actual;id_=id;configured_=true;return error::success;
        }
        /** @brief Retain one exact sample phase before complete delivery; retries keep its sequence.
         * @details New sample after a terminal result acknowledges/releases that
         * prior internal device request. Another sample of an outstanding phase
         * returns the same record and does not adopt new inputs. Deadline failure
         * is explicit and latches the independent stop obligations.
         */
        template<typename Sink>error sample(principal owner,std::uint64_t now,devices::observations& inputs,Sink&& deliver)
        {
            if(!same_process())return error::denied;
            std::unique_lock lock{mutex_};if(!owned(owner)||retired_)return error::denied;if(busy_)return error::again;if(phase_==phase::latched)return error::cancelled;
            auto current=arbitration_.validate(owner,controller_,now);if(current!=error::success){latch(devices::reason::stopped);return current;}
            if(phase_==phase::submitted){auto result=refresh();if(result!=error::success)return result;if(phase_==phase::latched)return error::cancelled;if(phase_!=phase::completed)return error::again;}
            bool replay=phase_==phase::sampled;
            if(replay&&(!now||now>=sample_.deadline)){++sample_.missed;latch(devices::reason::deadline);return error::timeout;}
            sample_record next=sample_;
            if(!replay)
            {
                if(!now||now>INT64_MAX||policy_.write_window_ns>static_cast<std::uint64_t>(INT64_MAX)-now||
                    now+policy_.write_window_ns>controller_.expires)return error::timeout;
                if(sample_.sequence==UINT64_MAX)return error::too_large;
                auto valid=valid_observations(inputs,now);if(valid!=error::success)return valid;
                if(request_){auto result=device_.release(device_owner_,request_);if(result!=error::success)return result;request_=0;}
                next={id_,sample_.sequence+1,now,now+policy_.write_window_ns,sample_.missed,inputs.values,inputs.count};
                evidence_=std::move(inputs);inputs.count=0;
            }
            busy_=true;lock.unlock();bool copied{};
            try{copied=deliver(std::as_const(next));}catch(...){copied=false;}
            auto after=clock_();current=arbitration_.validate(owner,controller_,after);lock.lock();busy_=false;
            if(retired_||phase_==phase::latched){release_evidence();return error::cancelled;}
            if(current!=error::success){latch(devices::reason::stopped);return current;}
            if(after<now||after>=next.deadline){++sample_.missed;latch(devices::reason::deadline);return error::timeout;}
            if(!copied){if(!replay)release_evidence();return error::io;}
            sample_=next;phase_=phase::sampled;return error::success;
        }
        /** @brief Submit only this same sequence/deadline after a real source-proof recheck.
         * @details validate_sources is a borrowed trusted adapter; it must check
         * every original source and buffer capability under a complete authority
         * group held through this call. It runs outside the cycle mutex. Native
         * writes occur later, with another current group/controller/effect check.
         * A successful return is the actual device ledger's queued receipt.
         */
        template<typename Validator>error submit(principal owner,handle key,handle sequence,buffers::profile const& schema,wire::bytes input,
                                                std::uint64_t now,Validator&& validate_sources,devices::receipt& output)
        {
            if(!same_process())return error::denied;
            if(input.size()>devices::maximum_command_bytes||schema.identity.size>buffers::maximum_identity)return error::too_large;
            std::array<std::uint8_t,devices::maximum_command_bytes> body;std::copy(input.begin(),input.end(),body.begin());wire::bytes bytes{body.data(),input.size()};
            auto digest=fingerprint(key,sequence,schema,bytes);
            std::unique_lock lock{mutex_};if(!owned(owner))return error::denied;
            if(key&&key==last_key_){if(digest!=last_digest_)return error::conflict;output=submitted_;return error::success;}
            if(retired_||phase_==phase::latched)return error::cancelled;
            if(busy_)return error::again;
            if(!key||last_key_==UINT64_MAX||key!=last_key_+1||phase_!=phase::sampled||sequence!=sample_.sequence)return error::conflict;
            if(!now||now>=sample_.deadline){++sample_.missed;latch(devices::reason::deadline);return error::timeout;}
            auto status=valid_observations(evidence_,now);if(status!=error::success)return status;
            busy_=true;lock.unlock();
            try{status=validate_sources(std::as_const(evidence_),now);}catch(...){status=error::denied;}
            auto after=clock_();auto controller=arbitration_.validate(owner,controller_,after);lock.lock();busy_=false;
            if(retired_||phase_==phase::latched){release_evidence();return error::cancelled;}
            if(status!=error::success)return status;
            if(controller!=error::success){latch(devices::reason::stopped);return controller;}
            if(after<now||after>=sample_.deadline){++sample_.missed;latch(devices::reason::deadline);return error::timeout;}
            status=valid_observations(evidence_,after);if(status!=error::success)return status;
            devices::plan actual;handle provider{},device_key{};status=device_.observe_submission(device_owner_,device_handle_,actual,provider,device_key);
            if(status!=error::success)return status;
            if(provider!=controller_.provider_generation||actual.schema.profile!=device_policy_.schema.profile)return error::conflict;
            devices::submission command{device_handle_,device_key,sample_.deadline,controller_,schema,bytes};
            status=device_.submit(device_owner_,command,evidence_,after,output);if(status!=error::success)return status;
            last_key_=key;last_digest_=digest;submitted_=last_result_=output;request_=output.request;submitted_sequence_=sequence;phase_=phase::submitted;return error::success;
        }
        /** @brief Read the actual device receipt, retaining unknown outcomes and stop debt. */
        error result(principal owner,handle sequence,devices::receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!owned(owner))return error::denied;if(!sequence||sequence!=submitted_sequence_)return error::bad_handle;
            if(request_){auto status=refresh();if(status!=error::success)return status;}output=last_result_;return error::success;
        }
        /** @brief Explicit control stop uses reserved controller/device stop bookkeeping. */
        error stop(principal owner,devices::receipt& output)noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!owned(owner))return error::denied;latch(devices::reason::stopped);return device_.pending_stop(output);
        }
        error retire()noexcept
        {if(!same_process())return error::denied;std::lock_guard lock{mutex_};if(!configured_)return error::denied;retired_=true;latch(devices::reason::retired);return error::success;}
        /** @brief Cached diagnostics do not refresh devices, expire phases or send stop bytes. */
        error inspect(status& output)const noexcept
        {
            if(!same_process())return error::denied;
            std::lock_guard lock{mutex_};if(!configured_)return error::denied;
            output={id_,sample_.sequence,request_,controller_.id,controller_.epoch,controller_.provider_generation,sample_.deadline,sample_.missed,phase_,busy_,retired_};return error::success;
        }
    };
}
