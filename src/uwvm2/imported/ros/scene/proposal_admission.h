/** @file
 * @brief Bounded inert model proposals and independent local admission.
 * @details This isolated provider core is not a registered import and does not
 * invoke a model, native ROS endpoint or actuator. Trusted deployment explicitly
 * sets finite capacities, producer identity, exact tool profiles and readers /
 * arbiters. Default construction grants nothing. A handle, model output,
 * confidence score or receipt cannot install or expand those grants.
 *
 * The producer publication path is trusted: it must pin sealed input buffers
 * and validate canonical arguments against the bound schema before publish().
 * The core copies immutable bytes and host-owned observation records; guest
 * bytes cannot assign provenance. accept() performs an independent actor,
 * capability, tool/action, target, generation, clock/frame and freshness check.
 * A side-effect-free validator runs outside the mutex. Reentry, revocation,
 * expiry and observation/target changes are checked again before admission.
 * No native callback is allowed to execute commands during validation.
 *
 * Success retains one exact retry receipt per arbiter and marks the proposal
 * admitted. Failed validation does not consume the request sequence, proposal
 * or optional rate; failed read delivery is nonconsuming. A failed guest copy
 * after accept commits does not undo admission: exact retry recovers the same
 * receipt without a second charge. Receipt replay is recovery only; execution must
 * independently recheck the current authority, target epoch, schema/physical
 * bounds, observations and steady deadline in the target provider's atomic
 * admission transaction. This core deliberately provides no execute() method.
 *
 * All storage is compile-time bounded and runtime capacities may only reduce
 * it. After construction no operation allocates. Capacity and optional rate
 * attenuation are separate; absent rate is unlimited inside the grant, zero
 * explicitly allows no new admissions. A caller must hold the broker authority
 * lease across every effect. Registry retirement/revocation does not physically
 * stop a robot and is not an independent watchdog or certified stop mechanism.
 */
#pragma once
#include "proposal_contract.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <span>
#include <utility>
#if defined(__linux__)
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::proposal_admission
{
    /** @brief Semantic core outcomes, not external carrier result numbers. */
    enum class result : std::uint8_t {success,denied,invalid,bad_handle,conflict,again,too_large,timeout,cancelled,delivery_failed};
    using identity=std::uint64_t;
    using action=proposal_contract::target_action;
    /** @brief Trusted effect classification independent of the ROS method name.
     * @details A topic/service/action/parameter/lifecycle operation may cause a
     * physical effect just like device/control submission. The immutable target
     * profile must classify it and bind the same physical scope/fencing rules.
     * Unclassified targets cannot be bound; model text cannot choose this field.
     */
    enum class effect_class : std::uint8_t {unclassified,non_physical,physical};
    /** @brief Explicit observation policy selected only by trusted deployment.
     * @details latest_required requires the exact current immutable snapshot.
     * bounded_age permits an older pinned sample only within its age/expiry and
     * the same source incarnation, authority, clock/frame and calibration /
     * transform generations. Unconfigured and unknown policies grant nothing.
     */
    enum class observation_policy : std::uint8_t {unconfigured,latest_required,bounded_age};
    /// @brief Owned arbiter session identity and strict next request sequence.
    struct request_key
    {identity session{},sequence{};friend bool operator==(request_key const&,request_key const&)=default;};
    /// @brief Exact proposed_record request fields, before canonical wire encoding.
    struct accept_request
    {
        request_key key{};
        identity proposal_capability{},proposal{},target_capability{},target{},deadline{};
        friend bool operator==(accept_request const&,accept_request const&)=default;
    };
    /** @brief Trusted logical scope evidence, separate from canonical guest handles.
     * @details The production broker resolves environment/grant/generation-bound
     * scopes only after complete authority group admission. It must never hash
     * guest aliases/integers into scopes. Zero selects the original isolated-test
     * path; production adapters reject zero instead of falling back.
     */
    struct authorization_context {identity proposal_scope{},target_scope{};};
    /** @brief Trusted sealed input reference plus immutable provenance.
     * @details capability/buffer are the catalogue buffer_ref; source and
     * source_generation identify the source incarnation, retaining the catalogue
     * source_generation meaning. clock/epoch/source_ns and frame identify the
     * canonical optional stamp/frame. sample_sequence, calibration_generation,
     * transform_generation, source_authority_epoch and captured_steady_ns are
     * host-only metadata and never come from model text. This
     * restrictive core requires a stamp and frame; canonical optional fields
     * without those values require a future explicitly bound schema contract.
     */
    struct observation
    {
        identity capability{},buffer{},source{},source_generation{},clock{},epoch{},frame{};
        std::int64_t source_ns{};
        std::uint64_t captured_steady_ns{};
        identity sample_sequence{},calibration_generation{},transform_generation{},source_authority_epoch{};
        friend bool operator==(observation const&,observation const&)=default;
    };
    /** @brief Host immutable profile references and exact approved target.
     * @details profile/schema/frame refer to canonical deployment-owned entries
     * whose identity and SHA256 digest must be retained by the integrating
     * registry. They must never be populated from arbitrary guest integers.
     * Updating the provider epoch retires old proposals; it does not mutate
     * an immutable tool's target, schema, action or physical constraints.
     */
    struct tool
    {
        identity profile{},schema{},target_capability{},target{},provider_generation{},frame{};
        action operation{};
        std::uint64_t max_observation_age_ns{};
        effect_class effect{};
        identity target_authority_epoch{},physical_scope{},controller_lease_epoch{};
        std::size_t minimum_observations{};
        observation_policy observations{};
        friend bool operator==(tool const&,tool const&)=default;
    };
    /// @brief Finite deployment capacities; every zero is a literal zero allowance.
    struct limits
    {
        std::size_t proposals{},argument_bytes{},observations{},readers{},tools{},sources{};
        std::uint64_t max_lifetime_ns{},max_accept_deadline_ns{};
    };
    /** @brief Exact trusted read/arbiter grant, with no implied target actions.
     * @details actor is the host caller identity, capability the independent
     * arbiter scope, and proposal_capability the separately readable source.
     * accept requires read and an exact target capability/action bit. A model
     * producer actor is never permitted to accept, even if misconfigured here.
     */
    struct grant
    {
        identity actor{},capability{},proposal_capability{},target_capability{},authority_epoch{};
        std::uint64_t target_actions{};
        bool read{},accept{};
        std::optional<std::uint64_t> calls;
        std::uint64_t window_ns{};
    };
    /** @brief Inert historical admission receipt, never an execution token. */
    struct receipt
    {
        identity request{},provider_generation{};
        request_key key{};
        identity proposal{};
        // Host-only evidence, not additional proposed_record wire fields and
        // never transferable authority to perform a later physical effect.
        identity arbiter_authority_epoch{},target_authority_epoch{},controller_lease_epoch{};
        friend bool operator==(receipt const&,receipt const&)=default;
    };
    /** @brief Core-local monotonic identities; not yet the VM resource registry.
     * @details Integration must map them through the VM's kind/owner-checked
     * handle registry. They never grant authority and are not external handles.
     */
    inline std::atomic<identity> next_identity{1};
    inline identity allocate_identity() noexcept
    {
        auto value=next_identity.load(std::memory_order_relaxed);
        while(value&&value!=UINT64_MAX)
            if(next_identity.compare_exchange_weak(value,value+1,std::memory_order_relaxed))return value;
        return 0;
    }
    inline std::uint64_t steady_now() noexcept
    {
        auto value=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        return value<0?0:static_cast<std::uint64_t>(value);
    }

    /** @brief Fixed storage selected by trusted deployment, not by the guest.
     * @details Hard template bounds cap memory before runtime configuration.
     * Configure/bind/publish/update/revoke/retire are trusted-only management
     * methods. Read/accept additionally require external broker lease admission.
     * The registry must outlive calls and callbacks; teardown retires it and
     * joins callers before destruction. Forked children fail before locking.
     * IdentityAllocator defaults to the isolated test namespace; the production
     * sealed-buffer wrapper supplies the VM's global resource allocator. It
     * must return a never-reused nonzero identity or zero on exhaustion.
     */
    template<std::size_t ProposalSlots=16,std::size_t ArgumentBytes=4096,std::size_t ObservationSlots=8,
             std::size_t ReaderSlots=8,std::size_t ToolSlots=8,std::size_t SourceSlots=16,
             identity (*IdentityAllocator)() noexcept=allocate_identity>
    class registry
    {
    public:
        /// @brief Detached bounded proposal content with trusted copied provenance.
        struct record
        {
            identity id{},model_request{};
            tool selected_tool;
            std::array<std::uint8_t,ArgumentBytes> arguments{};
            std::size_t argument_size{};
            std::array<observation,ObservationSlots> observations{};
            std::size_t observation_count{};
            std::uint64_t expiry{};
        };
    private:
        struct entry {record value;identity tool_id{};bool admitted{},busy{};};
        struct reader
        {
            identity id{};grant scope;bool revoked{},busy{};
            accept_request last_request;receipt last_receipt;
            std::uint64_t window_started{},charged{};
        };
        struct tool_entry {identity id{};tool value;bool revoked{};};
        struct source_entry {identity id{};observation value;bool revoked{};};
        mutable std::mutex mutex;
        std::array<entry,ProposalSlots> proposals{};
        std::array<reader,ReaderSlots> readers{};
        std::array<tool_entry,ToolSlots> tools{};
        std::array<source_entry,SourceSlots> sources{};
        limits capacity;
        identity producer{},proposal_scope{};
        bool configured{},retired{};
        std::uint64_t (*clock)() noexcept;
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
        static bool future(std::uint64_t deadline,std::uint64_t time,std::uint64_t maximum) noexcept
        {return deadline>time&&deadline-time<=maximum;}
        static bool observation_valid(observation const& value) noexcept
        {return value.capability&&value.buffer&&value.source&&value.source_generation&&value.clock&&value.epoch&&value.frame&&
                value.sample_sequence&&value.calibration_generation&&value.transform_generation&&value.source_authority_epoch;}
        static bool same_observation_lineage(observation const& current,observation const& previous) noexcept
        {
            return current.capability==previous.capability&&current.source==previous.source&&
                current.source_generation==previous.source_generation&&current.clock==previous.clock&&current.epoch==previous.epoch&&
                current.frame==previous.frame&&current.calibration_generation==previous.calibration_generation&&
                current.transform_generation==previous.transform_generation&&current.source_authority_epoch==previous.source_authority_epoch&&
                current.sample_sequence>=previous.sample_sequence;
        }
        reader* find_reader(identity actor,identity id) noexcept
        {
            if(!configured||retired)return nullptr;
            for(auto& value:readers)if(value.id==id&&value.scope.actor==actor&&!value.revoked)return &value;
            return nullptr;
        }
        entry* find_proposal(identity id) noexcept
        {for(auto& value:proposals)if(value.value.id==id&&id)return &value;return nullptr;}
        tool_entry* find_tool(identity id) noexcept
        {for(auto& value:tools)if(value.id==id&&!value.revoked&&id)return &value;return nullptr;}
        source_entry* find_source(identity id) noexcept
        {for(auto& value:sources)if(value.id==id&&!value.revoked&&id)return &value;return nullptr;}
        bool readable(reader const& owner,identity scope) const noexcept
        {return owner.scope.read&&scope==proposal_scope&&owner.scope.proposal_capability==scope;}
        result observations_current(record const& value,std::uint64_t time) noexcept
        {
            for(std::size_t i{};i!=value.observation_count;++i)
            {
                auto const& previous=value.observations[i];bool found{};
                for(auto const& source:sources)
                {
                    if(!source.id||source.revoked||source.value.source!=previous.source)continue;
                    switch(value.selected_tool.observations)
                    {
                    case observation_policy::latest_required:found=source.value==previous;break;
                    case observation_policy::bounded_age:found=same_observation_lineage(source.value,previous);break;
                    default:return result::denied;
                    }
                    break;
                }
                if(!found)return result::conflict;
                if(previous.frame!=value.selected_tool.frame||previous.captured_steady_ns>time)return result::invalid;
                if(time-previous.captured_steady_ns>value.selected_tool.max_observation_age_ns)return result::timeout;
            }
            return result::success;
        }
        bool quota_available(reader const& owner,std::uint64_t time) const noexcept
        {
            if(!owner.scope.calls)return true;
            if(!*owner.scope.calls)return false;
            if(time<owner.window_started)return false;
            return time-owner.window_started>=owner.scope.window_ns||owner.charged<*owner.scope.calls;
        }
        void charge(reader& owner,std::uint64_t time) noexcept
        {
            if(!owner.scope.calls)return;
            if(time-owner.window_started>=owner.scope.window_ns){owner.window_started=time;owner.charged=0;}
            ++owner.charged;
        }
        result admission(reader const& owner,entry const& selected,accept_request const& request,std::uint64_t time,authorization_context context) noexcept
        {
            auto proposal=context.proposal_scope?context.proposal_scope:request.proposal_capability;
            auto target=context.target_scope?context.target_scope:request.target_capability;
            if(!readable(owner,proposal)||!owner.scope.accept||owner.scope.actor==producer)return result::denied;
            auto const& profile=selected.value.selected_tool;
            if(owner.scope.capability==proposal_scope||owner.scope.target_capability!=target||
               target!=profile.target_capability||request.target!=profile.target||
               !(owner.scope.target_actions&proposal_contract::action_bit(profile.operation)))return result::denied;
            auto bound=find_tool(selected.tool_id);
            if(!bound||bound->value!=profile)return result::conflict;
            if(!future(request.deadline,time,capacity.max_accept_deadline_ns)||time>=selected.value.expiry||request.deadline>selected.value.expiry)
                return result::timeout;
            auto freshness=observations_current(selected.value,time);if(freshness!=result::success)return freshness;
            return quota_available(owner,time)?result::success:result::again;
        }
    public:
        /// @brief Construct with no authority; clock is a trusted monotonic host clock.
        explicit registry(std::uint64_t (*trusted_clock)() noexcept=steady_now) noexcept:clock(trusted_clock?trusted_clock:steady_now){}
        registry(registry const&)=delete;
        registry& operator=(registry const&)=delete;
        /** @brief Bind a finite zero-default scope once; no authority is inferred. */
        result configure(limits value,identity producer_actor,identity proposal_capability) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};
            if(configured||retired)return result::conflict;
            if(value.proposals>ProposalSlots||value.argument_bytes>ArgumentBytes||value.observations>ObservationSlots||
               value.readers>ReaderSlots||value.tools>ToolSlots||value.sources>SourceSlots||
               value.max_lifetime_ns>INT64_MAX||value.max_accept_deadline_ns>INT64_MAX)return result::too_large;
            if(!producer_actor||!proposal_capability)return result::invalid;
            capacity=value;producer=producer_actor;proposal_scope=proposal_capability;configured=true;return result::success;
        }
        /** @brief Trusted deployment pins one classified tool and its target.
         * @details Call after configure and before publishing its proposals.
         * Canonical profile/schema digests and physical limits are owned by the
         * external registry. This metadata cannot be changed by model output.
         * A revoked slot remains a tombstone; binding cannot restore its name.
         */
        result bind_tool(tool value,identity& output) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired)return result::denied;
            if(!value.profile||!value.schema||!value.target_capability||!value.target||!value.provider_generation||!value.frame||
               !proposal_contract::valid(value.operation)||value.max_observation_age_ns>INT64_MAX||!value.target_authority_epoch||
               value.minimum_observations>capacity.observations||
               (value.observations!=observation_policy::latest_required&&value.observations!=observation_policy::bounded_age)||
               (value.effect!=effect_class::physical&&value.effect!=effect_class::non_physical)||
               (value.effect==effect_class::physical&&(!value.physical_scope||!value.controller_lease_epoch||!value.minimum_observations))||
               (value.effect==effect_class::non_physical&&(value.physical_scope||value.controller_lease_epoch)))return result::invalid;
            for(auto const& existing:tools)if(existing.id&&existing.value.profile==value.profile)return result::conflict;
            for(std::size_t i{};i!=capacity.tools;++i)if(!tools[i].id)
            {auto id=IdentityAllocator();if(!id)return result::again;tools[i]={id,value,false};output=id;return result::success;}
            return result::again;
        }
        /** @brief Trusted deployment installs an exact reader or independent arbiter.
         * @details No guest open/create path calls this method. The producer
         * cannot be an accepting actor. Action bits/capabilities are exact.
         * external_session is trusted-only: production supplies an already
         * validated arbiter capability as the guest-known request-key session.
         * Capability and resource integers have different namespaces; ownership
         * is checked through typed maps rather than numeric global uniqueness.
         * The external authority lease remains mandatory. Slots never recycle,
         * so closing/reopening cannot reset a request ledger or finite rate.
         */
        result bind_reader(grant value,identity& output,identity external_session=0) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired)return result::denied;
            if(!value.actor||!value.capability||!value.authority_epoch||value.proposal_capability!=proposal_scope||
               value.target_actions&~proposal_contract::all_actions||
               (value.accept&&(!value.read||!value.target_capability||value.actor==producer||value.capability==proposal_scope))||
               (value.calls&&(!value.window_ns||value.window_ns>INT64_MAX)))return result::invalid;
            for(auto const& existing:readers)if(existing.id&&existing.scope.capability==value.capability)return result::conflict;
            for(std::size_t i{};i!=capacity.readers;++i)if(!readers[i].id)
            {
                auto id=external_session?external_session:IdentityAllocator();if(!id)return result::again;
                for(auto const& existing:readers)if(existing.id==id)return result::conflict;
                readers[i].id=id;readers[i].scope=value;readers[i].window_started=clock();output=id;return result::success;
            }
            return result::again;
        }
        /** @brief Explicitly attenuate an existing arbiter without adding rights.
         * @details A missing original rate allows execution inside its grant;
         * installing a finite rate starts a new restricted window. Subsequent
         * attenuation may only reduce calls and lengthen the window, retaining
         * its start/count so no consumed capacity is refunded. Explicit zero
         * forbids new admissions. The trusted authority epoch must advance and
         * in-flight validations then fail for retry. Historical receipts remain
         * recoverable inert data. This method cannot change actors or targets.
         */
        result attenuate_reader(identity id,std::uint64_t calls,std::uint64_t window_ns,identity authority_epoch) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired)return result::denied;
            for(auto& value:readers)if(value.id==id&&!value.revoked)
            {
                if(!window_ns||window_ns>INT64_MAX||authority_epoch<=value.scope.authority_epoch)return result::invalid;
                if(value.scope.calls&&(calls>*value.scope.calls||window_ns<value.scope.window_ns))return result::denied;
                if(!value.scope.calls){value.window_started=clock();value.charged=0;}
                value.scope.calls=calls;value.scope.window_ns=window_ns;value.scope.authority_epoch=authority_epoch;
                return result::success;
            }
            return result::bad_handle;
        }
        /** @brief Register host provenance only; it does not pin the real buffer.
         * @details Integrators retain the sealed buffer lease separately. Source
         * identities are unique. A sample advances independently of incarnation,
         * calibration/transform generations and authority epoch. Only the trusted
         * profile can choose bounded-age reuse, and actual buffer/source leases
         * still need current adapter checks. Revoked slots remain tombstones.
         */
        result bind_source(observation value,identity& output) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired)return result::denied;
            if(!observation_valid(value)||value.captured_steady_ns>clock())return result::invalid;
            for(auto const& existing:sources)if(existing.id&&existing.value.source==value.source)return result::conflict;
            for(std::size_t i{};i!=capacity.sources;++i)if(!sources[i].id)
            {auto id=IdentityAllocator();if(!id)return result::again;sources[i]={id,value,false};output=id;return result::success;}
            return result::again;
        }
        /** @brief Trusted ingress advances a sample or fences a source incarnation.
         * @details Within one incarnation sample_sequence must strictly increase;
         * calibration, transform and authority generations cannot decrease even
         * across source incarnations. A
         * source restart requires a higher source_generation and may reset the
         * sample sequence. Capture time never moves backwards. A capability
         * change requires a newer source authority epoch. A frame change must
         * strictly advance transform_generation; changing clock identity must
         * strictly advance source_generation. These requirements prevent an
         * A-to-B-to-A frame/clock switch from reviving an old proposal. For the
         * same clock identity its epoch cannot decrease across incarnations.
         * Latest-required proposals reject every new sample. Bounded-age ones
         * retain their immutable sample only while all lineage/authority fields
         * match. On configuration changes the adapter must update/fence the
         * source immediately, not wait until a later acceptance or hardware effect.
         */
        result update_source(identity id,observation value) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired)return result::denied;
            auto old=find_source(id);if(!old)return result::bad_handle;
            auto const& previous=old->value;
            if(!observation_valid(value)||value.source!=previous.source||value.source_generation<previous.source_generation||
               value.captured_steady_ns<previous.captured_steady_ns||value.captured_steady_ns>clock()||
               value.calibration_generation<previous.calibration_generation||value.transform_generation<previous.transform_generation||
               value.source_authority_epoch<previous.source_authority_epoch||
               (value.clock==previous.clock&&value.epoch<previous.epoch)||
               (value.capability!=previous.capability&&value.source_authority_epoch==previous.source_authority_epoch)||
               (value.frame!=previous.frame&&value.transform_generation==previous.transform_generation)||
               (value.clock!=previous.clock&&value.source_generation==previous.source_generation))return result::invalid;
            if(value.source_generation==previous.source_generation&&
               value.sample_sequence<=previous.sample_sequence)return result::invalid;
            old->value=value;return result::success;
        }
        /** @brief Fence source permission changes independently of new samples.
         * @details Trusted authority management advances this epoch before a
         * withdrawal or attenuation can affect use. Existing proposals fail both
         * policies; captured buffers do not inherit new authority automatically.
         * Future publications still require current source permission and sealed
         * buffer leases from the integrating adapter. Revocation uses revoke_source
         * when no source access remains. No sample timestamp or sequence changes.
         */
        result fence_source_authority(identity id,identity authority_epoch) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired)return result::denied;
            auto source=find_source(id);if(!source)return result::bad_handle;
            if(authority_epoch<=source->value.source_authority_epoch)return result::invalid;
            source->value.source_authority_epoch=authority_epoch;return result::success;
        }
        /** @brief Trusted target authority/reconnect/controller fencing.
         * @details At least one of the three independent epochs must increase;
         * none can decrease. Provider restart, permission change and controller
         * lease ownership change are separate events. Old proposals stay inert,
         * and validation in flight cannot commit across any changed epoch.
         */
        result fence_target(identity id,identity generation,identity authority_epoch,identity controller_epoch) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired)return result::denied;
            auto bound=find_tool(id);if(!bound)return result::bad_handle;
            auto& current=bound->value;
            if(generation<current.provider_generation||authority_epoch<current.target_authority_epoch||controller_epoch<current.controller_lease_epoch||
               (generation==current.provider_generation&&authority_epoch==current.target_authority_epoch&&controller_epoch==current.controller_lease_epoch)||
               (current.effect==effect_class::non_physical&&controller_epoch))return result::invalid;
            current.provider_generation=generation;current.target_authority_epoch=authority_epoch;current.controller_lease_epoch=controller_epoch;
            return result::success;
        }
        /** @brief Copy a schema-validated trusted model result into bounded storage.
         * @details model_request is a host-owned inference receipt. Tool and
         * source references are resolved from trusted bindings; arguments are
         * copied before publication and cannot modify provenance or grants.
         * Zero byte/source/proposal capacity is literal. No heap allocation,
         * source mutation, target command or arbitrary codec runs here.
         */
        result publish(identity actor,identity model_request,identity tool_id,std::span<std::uint8_t const> arguments,
                       std::span<identity const> observation_ids,std::uint64_t expiry,identity& output) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired||actor!=producer)return result::denied;
            if(!model_request)return result::invalid;
            auto bound=find_tool(tool_id);if(!bound)return result::bad_handle;
            if(arguments.size()>capacity.argument_bytes||observation_ids.size()>capacity.observations)return result::too_large;
            if(observation_ids.size()<bound->value.minimum_observations)return result::invalid;
            auto time=clock();if(!future(expiry,time,capacity.max_lifetime_ns))return result::timeout;
            entry* available{};
            for(std::size_t i{};i!=capacity.proposals;++i)if(!proposals[i].value.id){available=&proposals[i];break;}
            if(!available)return result::again;
            record prepared;prepared.model_request=model_request;prepared.selected_tool=bound->value;prepared.expiry=expiry;
            prepared.argument_size=arguments.size();std::copy(arguments.begin(),arguments.end(),prepared.arguments.begin());
            for(auto id:observation_ids)
            {
                auto source=find_source(id);if(!source)return result::bad_handle;
                for(std::size_t i{};i!=prepared.observation_count;++i)
                    if(prepared.observations[i].source==source->value.source)return result::invalid;
                prepared.observations[prepared.observation_count++]=source->value;
            }
            auto freshness=observations_current(prepared,time);if(freshness!=result::success)return freshness;
            prepared.id=IdentityAllocator();if(!prepared.id)return result::again;
            available->value=prepared;available->tool_id=tool_id;output=prepared.id;return result::success;
        }
        /** @brief Publish the exact immutable observations used by completed inference.
         * @details The production adapter supplies captured host snapshots and
         * retains their actual sealed leases. A newer sample must not be relabeled
         * as the input of an older model result. The bound latest/bounded-age
         * policy checks these snapshots against current source lineage, authority,
         * clocks, frame/calibration/transform generations and freshness.
         */
        result publish_snapshots(identity actor,identity model_request,identity tool_id,std::span<std::uint8_t const> arguments,
                                 std::span<observation const> observations,std::uint64_t expiry,identity& output) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};if(!configured||retired||actor!=producer)return result::denied;
            if(!model_request)return result::invalid;
            auto bound=find_tool(tool_id);if(!bound)return result::bad_handle;
            if(arguments.size()>capacity.argument_bytes||observations.size()>capacity.observations)return result::too_large;
            if(observations.size()<bound->value.minimum_observations)return result::invalid;
            auto time=clock();if(!future(expiry,time,capacity.max_lifetime_ns))return result::timeout;
            entry* available{};
            for(std::size_t i{};i!=capacity.proposals;++i)if(!proposals[i].value.id){available=&proposals[i];break;}
            if(!available)return result::again;
            record prepared;prepared.model_request=model_request;prepared.selected_tool=bound->value;prepared.expiry=expiry;
            prepared.argument_size=arguments.size();std::copy(arguments.begin(),arguments.end(),prepared.arguments.begin());
            for(auto const& observation:observations)
            {
                if(!observation_valid(observation))return result::invalid;
                for(std::size_t i{};i!=prepared.observation_count;++i)
                    if(prepared.observations[i].source==observation.source)return result::invalid;
                prepared.observations[prepared.observation_count++]=observation;
            }
            auto freshness=observations_current(prepared,time);if(freshness!=result::success)return freshness;
            prepared.id=IdentityAllocator();if(!prepared.id)return result::again;
            available->value=prepared;available->tool_id=tool_id;output=prepared.id;return result::success;
        }
        /** @brief Copy a detached immutable proposal; sink failure consumes nothing.
         * @details Caller must preflight its complete output span before copying.
         * The external lease controls disclosure; this method holds no mutex
         * while the sink runs. A sink may reenter but may not retain references.
         */
        template<typename Sink> result read(identity actor,identity reader_id,identity scope,identity id,Sink&& sink)
        {
            if(!same_process())return result::denied;
            record copy;
            {
                std::lock_guard lock{mutex};auto owner=find_reader(actor,reader_id);
                if(!owner||!readable(*owner,scope))return result::denied;
                auto value=find_proposal(id);if(!value)return result::bad_handle;copy=value->value;
            }
            try {return sink(std::as_const(copy))?result::success:result::delivery_failed;}
            catch(...) {return result::delivery_failed;}
        }
        /** @brief Reserve, validate and locally admit one inert proposal.
         * @details validator(record) returns a semantic result and MUST have no
         * effect on the target. It must be bounded/nonblocking, executes outside the registry mutex and may
         * query a native provider under that provider's own observation rules.
         * Failed/throwing validation, concurrent/reentrant acceptance, revocation
         * and timeout never consume a sequence or rate. The output receipt is
         * assigned only after a successful commit or exact previous replay.
         * The caller handles output-copy failure by retrying the identical key;
         * it must not rerun a target command using this receipt as authority.
         */
        template<typename Validator> result accept(identity actor,accept_request const& request,Validator&& validator,receipt& output,authorization_context context={})
        {
            if(!same_process())return result::denied;
            record copy;identity validated_authority_epoch{};
            {
                std::lock_guard lock{mutex};auto owner=find_reader(actor,request.key.session);
                if(!owner||!readable(*owner,context.proposal_scope?context.proposal_scope:request.proposal_capability)||!owner->scope.accept||actor==producer)return result::denied;
                if(owner->last_request.key.sequence&&request==owner->last_request){output=owner->last_receipt;return result::success;}
                if(!request.key.sequence||owner->last_request.key.sequence==UINT64_MAX||request.key.sequence!=owner->last_request.key.sequence+1)
                    return result::conflict;
                auto selected=find_proposal(request.proposal);if(!selected)return result::bad_handle;
                if(owner->busy||selected->busy)return result::again;
                if(selected->admitted)return result::conflict;
                auto admitted=admission(*owner,*selected,request,clock(),context);if(admitted!=result::success)return admitted;
                owner->busy=selected->busy=true;copy=selected->value;validated_authority_epoch=owner->scope.authority_epoch;
            }
            result validated{result::denied};
            try {validated=validator(std::as_const(copy));} catch(...) {validated=result::denied;}
            std::lock_guard lock{mutex};
            // A busy proposal and reader cannot be removed or rebound while the
            // callback runs; revoked slots retain their reservation until here.
            reader* reserved{};for(auto& value:readers)if(value.id==request.key.session){reserved=&value;break;}
            auto selected=find_proposal(request.proposal);
            if(reserved)reserved->busy=false;
            if(selected)selected->busy=false;
            auto owner=find_reader(actor,request.key.session);
            if(!owner||!selected)return result::cancelled;
            if(owner->scope.authority_epoch!=validated_authority_epoch)return result::conflict;
            auto time=clock();auto admitted=admission(*owner,*selected,request,time,context);
            if(admitted!=result::success)return admitted;
            if(validated!=result::success)return validated;
            auto id=IdentityAllocator();if(!id)return result::again;
            receipt committed{id,copy.selected_tool.provider_generation,request.key,request.proposal,
                              owner->scope.authority_epoch,copy.selected_tool.target_authority_epoch,copy.selected_tool.controller_lease_epoch};
            selected->admitted=true;owner->last_request=request;owner->last_receipt=committed;charge(*owner,time);output=committed;
            return result::success;
        }
        /** @brief Recover only the exact previous inert receipt without fresh target/source access.
         * @details The integrating broker still requires its current arbiter
         * and proposal-reader scopes. A changed canonical request conflicts;
         * absence changes no ledger, deadline or quota. No proposal is executed.
         */
        result probe_retry(identity actor,accept_request const& request,authorization_context context,receipt& output,bool& found) noexcept
        {
            found=false;if(!same_process())return result::denied;
            std::lock_guard lock{mutex};auto owner=find_reader(actor,request.key.session);
            if(!owner||!context.proposal_scope||!context.target_scope||!readable(*owner,context.proposal_scope)||!owner->scope.accept||
               owner->scope.target_capability!=context.target_scope||actor==producer)return result::denied;
            if(owner->last_request.key.sequence&&request.key.sequence==owner->last_request.key.sequence)
            {
                if(request!=owner->last_request)return result::conflict;
                output=owner->last_receipt;found=true;
            }
            return result::success;
        }
        /** @brief Trusted explicit retirement; it cannot revoke a hardware effect.
         * @details Busy records cannot be erased. IDs never recycle. Removing a
         * proposal does not erase a retained exact retry receipt or refund rate.
         */
        result release(identity id) noexcept
        {
            if(!same_process())return result::denied;
            std::lock_guard lock{mutex};auto selected=find_proposal(id);if(!selected)return result::bad_handle;
            if(selected->busy)return result::again;
            *selected={};return result::success;
        }
        /** @brief Trusted revocation retires one exact reader/arbiter permanently.
         * @details Pending validation cannot commit; historical receipts are no
         * longer readable through that grant. Metadata is retained in its fixed
         * slot, preventing identity reuse, ledger reset or rate replenishment.
         */
        void revoke_reader(identity id) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};for(auto& value:readers)if(value.id==id)value.revoked=true;
        }
        /** @brief Trusted source revocation invalidates all dependent proposals.
         * @details The bounded source tombstone persists. Integrators separately
         * retire its sealed buffer lease only after all users release it.
         */
        void revoke_source(identity id) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};for(auto& value:sources)if(value.id==id)value.revoked=true;
        }
        /** @brief Trusted tool withdrawal prevents publication and admission.
         * @details It does not undo an earlier external effect. Tool metadata
         * stays reserved; its canonical profile cannot silently be rebound.
         */
        void revoke_tool(identity id) noexcept
        {
            if(!same_process())return;
            std::lock_guard lock{mutex};for(auto& value:tools)if(value.id==id)value.revoked=true;
        }
        /** @brief Retire the whole core before joining callers and destroying it.
         * @details This cancels local admission only, never a physical robot
         * command. In-flight validators must return before storage is destroyed.
         */
        void retire() noexcept
        {if(!same_process())return;std::lock_guard lock{mutex};retired=true;}
    };
}
