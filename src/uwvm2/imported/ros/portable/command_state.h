// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "wire.h"
#include <limits>

namespace uwvm2::imported::ros::portable
{
    // Native data keys, never Guest credentials. The actual Host must obtain
    // these from its admitted Runtime, policy and provider leases. Comparing
    // them cannot create authority, validate a pointer, or authorize an effect.
    struct owner_key
    {
        void const* instance{};
        ::std::uint64_t generation{};
        digest wasm_sha256{};
        friend constexpr bool operator==(owner_key const&,owner_key const&) noexcept = default;
    };
    struct operation_binding
    {
        owner_key owner{};
        ::std::uint64_t mapping_epoch{}, policy_epoch{}, native_generation{};
        digest adapter_sha256{}, mapping_sha256{}, native_provenance{};
        friend constexpr bool operator==(operation_binding const&,operation_binding const&) noexcept = default;
    };
    enum class native_terminal : unsigned { succeeded, rejected, cancelled };
    struct terminal_observation
    {
        ::std::uint64_t goal_id{}, observed_ns{};
        native_terminal result{};
        ::std::int32_t status{};
    };

    // One finite step per owner generation. Retain this journal after close;
    // a new handle must not clear it or replay an uncertain command. The Host
    // serializes access under its real owner/provider lease, validates every
    // Guest span/capacity before reserve, hashes the exact canonical request,
    // and performs the native send at most once after reserve succeeds.
    //
    // This class has no native callbacks, pointer dereferences, permissions,
    // import registration, I/O or claim that its supplied observations are real.
    // Only the concrete admitted native backend may supply those observations.
    class command_state final
    {
    public:
        enum class phase : unsigned { empty, reserved, accepted, succeeded, rejected, cancelled, indeterminate };
        static constexpr ::std::uint64_t maximum_wait_ns{2000000000};
    private:
        operation_binding binding_{};
        command command_{};
        descriptor mapping_{};
        digest command_sha256_{};
        ::std::uint64_t reserved_ns_{}, accepted_ns_{}, deadline_ns_{}, native_goal_id_{};
        phase phase_{};
        receipt receipt_{};
        bool closed_{}, revoked_{};

        constexpr error active(operation_binding const& binding) const noexcept
        {
            if(phase_==phase::empty || closed_ || revoked_ || binding.owner!=binding_.owner) { return error::bad_handle; }
            return binding==binding_ ? error::ok : error::stale;
        }
        constexpr error before_deadline(::std::uint64_t now_ns) noexcept
        {
            if(now_ns<reserved_ns_) { phase_=phase::indeterminate;return error::malformed; }
            if(now_ns>=deadline_ns_)
            {
                phase_=phase::indeterminate;
                return error::deadline;
            }
            return error::ok;
        }
    public:
        command_state() noexcept = default;
        command_state(command_state const&) = delete;
        command_state& operator=(command_state const&) = delete;
        command_state(command_state&&) = delete;
        command_state& operator=(command_state&&) = delete;
        [[nodiscard]] constexpr phase current_phase() const noexcept { return phase_; }
        [[nodiscard]] constexpr bool consumed() const noexcept { return phase_!=phase::empty; }
        [[nodiscard]] constexpr bool closed() const noexcept { return closed_; }

        [[nodiscard]] constexpr error reserve(operation_binding const& binding, command const& request,
            descriptor const& mapping,state const& sample,digest const& exact_command_sha256,
            ::std::uint64_t now_ns,::std::uint64_t wait_ns) noexcept
        {
            if(consumed() || closed_) { return error::bad_handle; }
            if(binding.owner.instance==nullptr || !binding.owner.generation || !binding.policy_epoch ||
               !binding.native_generation || !wait_ns || wait_ns>maximum_wait_ns ||
               now_ns>::std::numeric_limits<::std::uint64_t>::max()-wait_ns) { return error::malformed; }
            if(binding.mapping_epoch!=mapping.epoch || binding.mapping_sha256!=mapping.mapping) { return error::stale; }
            if(auto result{validate_command_data(request,mapping,sample,now_ns)};result!=error::ok) { return result; }
            binding_=binding;command_=request;mapping_=mapping;command_sha256_=exact_command_sha256;
            reserved_ns_=now_ns;deadline_ns_=now_ns+wait_ns;phase_=phase::reserved;
            return error::ok;
        }

        // Failure after reserve may follow an externally visible native send.
        // Never undo consumption, even when no accepted goal ID was returned.
        [[nodiscard]] constexpr error mark_indeterminate(operation_binding const& binding) noexcept
        {
            if(auto result{active(binding)};result!=error::ok) { return result; }
            if(phase_!=phase::reserved && phase_!=phase::accepted) { return error::bad_handle; }
            phase_=phase::indeterminate;return error::deadline;
        }

        [[nodiscard]] constexpr error accepted(operation_binding const& binding,::std::uint64_t native_goal_id,
            ::std::uint64_t now_ns) noexcept
        {
            if(auto result{active(binding)};result!=error::ok) { return result; }
            if(phase_!=phase::reserved) { return error::bad_handle; }
            // Preserve an observed goal ID even when its acceptance came after
            // the deadline. It is diagnostic state, never authority to resend.
            native_goal_id_=native_goal_id;
            if(auto result{before_deadline(now_ns)};result!=error::ok) { return result; }
            if(!native_goal_id) { phase_=phase::indeterminate;return error::malformed; }
            accepted_ns_=now_ns;phase_=phase::accepted;
            receipt_={mapping_.epoch,command_.id,command_.based_on_sequence,now_ns,
                      outcome::accepted,0,command_sha256_};
            return error::ok;
        }

        [[nodiscard]] constexpr error finish(operation_binding const& binding,terminal_observation const& terminal,
            state const& confirmation,::std::uint64_t now_ns) noexcept
        {
            if(auto result{active(binding)};result!=error::ok) { return result; }
            if(phase_!=phase::accepted) { return error::bad_handle; }
            if(auto result{before_deadline(now_ns)};result!=error::ok) { return result; }
            if(terminal.goal_id!=native_goal_id_ || terminal.observed_ns<accepted_ns_ || terminal.observed_ns>now_ns)
            { return error::malformed; }
            outcome result{};phase next{};::std::uint64_t sequence{};
            switch(terminal.result)
            {
                case native_terminal::succeeded:
                {
                    if(terminal.status!=0 || !valid(confirmation)) { return error::malformed; }
                    if(confirmation.epoch!=mapping_.epoch) { return error::stale; }
                    if(confirmation.count!=mapping_.count) { return error::unsupported; }
                    if(confirmation.sequence<=command_.based_on_sequence || confirmation.observed_ns<terminal.observed_ns ||
                       confirmation.observed_ns>now_ns || now_ns>=confirmation.valid_until_ns) { return error::no_fresh_state; }
                    for(::std::uint32_t i{};i!=mapping_.count;++i)
                    {
                        auto const& limits{mapping_.joints[i]};auto actual{confirmation.positions[i]},target{command_.targets[i]};
                        if(actual<limits.lower || actual>limits.upper) { return error::unsupported; }
                        auto distance{actual<target ? target-actual : actual-target};
                        if(static_cast<::std::uint64_t>(distance)>limits.tolerance) { return error::no_fresh_state; }
                    }
                    result=outcome::succeeded;next=phase::succeeded;sequence=confirmation.sequence;break;
                }
                case native_terminal::rejected:result=outcome::rejected;next=phase::rejected;break;
                case native_terminal::cancelled:result=outcome::cancelled;next=phase::cancelled;break;
                default:return error::malformed;
            }
            receipt_={mapping_.epoch,command_.id,sequence,now_ns,result,terminal.status,command_sha256_};
            phase_=next;return error::ok;
        }

        // Output is untouched on failure. ACCEPTED remains distinct from
        // SUCCEEDED; the caller must inspect the returned outcome.
        [[nodiscard]] constexpr error snapshot(operation_binding const& binding,::std::uint64_t now_ns,receipt& output) noexcept
        {
            if(auto result{active(binding)};result!=error::ok) { return result; }
            if(phase_==phase::indeterminate) { return error::deadline; }
            if(phase_==phase::reserved || phase_==phase::accepted)
            { if(auto result{before_deadline(now_ns)};result!=error::ok) { return result; } }
            if(phase_==phase::reserved) { return error::no_fresh_state; }
            output=receipt_;return error::ok;
        }

        // Real Host retirement notifications call this under the owner lease.
        // No rollback or later matching old binding can restore the journal.
        [[nodiscard]] constexpr error revoke(owner_key const& owner) noexcept
        {
            if(!consumed() || closed_ || owner!=binding_.owner) { return error::bad_handle; }
            revoked_=true;return error::ok;
        }

        // Caller-owned cleanup remains possible after policy/mapping changes.
        // Closing never cancels a native goal or discards its no-replay state.
        [[nodiscard]] constexpr error close(owner_key const& owner) noexcept
        {
            if(!consumed() || closed_ || owner!=binding_.owner) { return error::bad_handle; }
            closed_=true;return error::ok;
        }
    };
}
