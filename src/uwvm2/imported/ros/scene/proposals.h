/** @file
 * @brief Production proposal storage retaining actual immutable source snapshots.
 * @details One provider owns one exact deployment alias and independently bound
 * producer/readers/tools. It uses the VM global identity allocator, canonical
 * tool profiles and real sealed buffer leases. Model results cannot invent
 * source provenance or gain arbiter/target authority. Acceptance remains inert;
 * actual target execution must revalidate authority/fencing at its effect point.
 * The integrating broker holds the complete origin/source/target group lease
 * across publication or acceptance. No provider callback runs under this
 * wrapper's mutex. Trusted teardown retires then joins callers before destroy.
 */
#pragma once
#include "model_jobs.h"
#include "model_dispatch.h"
#include "proposal_admission.h"

namespace uwvm2::imported::ros::scene {class proposal_dispatch;}

namespace uwvm2::imported::ros::scene::proposals
{
    namespace admission = proposal_admission;
    using result = admission::result;
    using identity = admission::identity;
    using core = admission::registry<16, 4096, 8, 8, 8, 16, resources::allocate_identity>;

    /** @brief Payload-free alias diagnostics; references are not allocation bytes.
     * @details Source/reader/tool counts include reserved tombstones. Snapshot
     * references count those held inside this provider only; external capture
     * tokens and actual retained bytes/leases remain visible in buffer accounting.
     */
    struct usage
    {
        std::uint64_t proposals{}, source_slots{}, reader_slots{}, tool_slots{}, snapshot_references{};
        bool retired{};
    };

    /** @brief Immutable trusted tool/schema text, never supplied by model output. */
    struct tool_profile
    {
        admission::tool semantics;
        buffers::profile profile, schema;
        buffers::text<96> argument_type;
        buffers::text<32> encoding;
        bool (*validate_arguments)(std::span<std::uint8_t const>) noexcept {};
    };

    /** @brief One exact source snapshot pinned independently of model cleanup. */
    struct source_snapshot
    {
        identity owner{};
        admission::observation metadata;
        buffers::text<96> frame;
        models::input_evidence evidence;
        buffers::registry::sealed_lease bytes;
    };

    class provider
    {
        struct source_slot
        {
            identity id{};
            std::shared_ptr<source_snapshot const> snapshot;
            bool fenced{};
        };

        struct tool_slot
        {
            identity id{};
            tool_profile value;
        };

        struct proposal_slot
        {
            identity id{};
            tool_profile selected;
            std::array<std::shared_ptr<source_snapshot const>, 8> inputs{};
            std::size_t input_count{};
        };

        mutable std::mutex mutex;
        core state;
        ::uwvm2::utils::container::std_string alias, model_alias;
        admission::limits capacity;
        identity producer{}, scope{}, identity_scope{};
        std::uint64_t reader_count{};
        std::array<source_slot, 16> sources{};
        std::array<tool_slot, 8> tools{};
        std::array<std::shared_ptr<proposal_slot>, 16> proposals{};
        bool configured{}, retired{};
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process == ::getpid();
#else
            return true;
#endif
        }

        bool live(std::string_view selected) const noexcept { return configured && !retired && selected == alias; }

        static result convert(resources::error value) noexcept
        {
            switch(value)
            {
                case resources::error::success: return result::success;
                case resources::error::bad_handle: return result::bad_handle;
                case resources::error::again: return result::again;
                case resources::error::timeout: return result::timeout;
                case resources::error::cancelled: return result::cancelled;
                case resources::error::too_large: return result::too_large;
                case resources::error::invalid: return result::invalid;
                case resources::error::conflict: return result::conflict;
                default: return result::denied;
            }
        }

        static bool text_valid(auto const& value) noexcept
        {
            return value.size && value.size <= value.bytes.size() && wire::utf8({reinterpret_cast<std::uint8_t const*>(value.bytes.data()), value.size}) &&
                   value.view().find('\0') == std::string_view::npos;
        }

        static result current(std::shared_ptr<source_snapshot const> const& value) noexcept
        {
            if(!value) { return result::bad_handle; }
            return convert(value->bytes.visit([&](auto, auto const&, auto const& digest) { return digest == value->evidence.sha256; }));
        }

        std::shared_ptr<proposal_slot> find(identity id) const noexcept
        {
            for(auto const& value: proposals)
            {
                if(value && value->id == id) { return value; }
            }
            return {};
        }

        static result all_current(proposal_slot const& value) noexcept
        {
            for(std::size_t i{}; i != value.input_count; ++i)
            {
                auto status = current(value.inputs[i]);
                if(status != result::success) { return status; }
            }
            return result::success;
        }

        static std::shared_ptr<source_snapshot> prepare(admission::observation metadata, std::string_view frame, buffers::registry::sealed_lease& bytes)
        {
            if(!bytes || bytes.identity() != metadata.buffer || !metadata.capability) { return {}; }
            buffers::text<96> owned_frame;
            if(!owned_frame.assign(frame) || !text_valid(owned_frame)) { return {}; }
            models::input_evidence evidence{bytes.identity(), {}};
            if(bytes.visit(
                   [&](auto, auto const&, auto const& digest)
                   {
                       evidence.sha256 = digest;
                       return true;
                   }) != resources::error::success)
            {
                return {};
            }
            auto value = ::uwvm2::utils::container::make_shared<source_snapshot>();
            value->metadata = metadata;
            value->frame = owned_frame;
            value->evidence = evidence;
            return value;
        }

    public:
        /** @brief Opaque shared pin captured when the native inference receives input.
         * @details The host adapter retains this token with its model job. A
         * future sample cannot alter it. This is lifetime evidence, not authority.
         */
        class snapshot_token
        {
            friend class provider;
            friend class ::uwvm2::imported::ros::scene::proposal_dispatch;
            std::shared_ptr<source_snapshot const> value;

        public:
            explicit operator bool() const noexcept { return static_cast<bool>(value); }

            identity buffer() const noexcept { return value ? value->metadata.buffer : 0; }
        };

        explicit provider(std::uint64_t (*clock)() noexcept = admission::steady_now) : state{clock} {}

        provider(provider const&) = delete;
        provider& operator= (provider const&) = delete;

        ~provider() { retire(); }

        /** @brief Bind one exact alias/model producer and finite fixed core capacities.
         * @details No readers, sources or tools are implied. Zero remains zero.
         * All owned allocation uses the recoverable strict fast_io adapters.
         */
        result configure(::uwvm2::utils::container::std_string name,
                         ::uwvm2::utils::container::std_string model,
                         admission::limits bounds,
                         identity producer_actor,
                         identity proposal_capability)
        {
            if(!same_process()) { return result::denied; }
            if(name.empty() || name.size() > 255 || name.find('\0') != name.npos || model.empty() || model.size() > 255 || model.find('\0') != model.npos)
            {
                return result::invalid;
            }
            std::lock_guard lock{mutex};
            if(configured || retired) { return result::denied; }
            auto status = state.configure(bounds, producer_actor, proposal_capability);
            if(status != result::success) { return status; }
            identity_scope = resources::allocate_identity();
            if(!identity_scope)
            {
                state.retire();
                retired = true;
                return result::again;
            }
            alias = std::move(name);
            model_alias = std::move(model);
            capacity = bounds;
            producer = producer_actor;
            scope = proposal_capability;
            configured = true;
            return result::success;
        }

        /** @brief Install exact tool profile and argument validator before publication. */
        result bind_tool(tool_profile value, identity& output)
        {
            if(!same_process()) { return result::denied; }
            if(!text_valid(value.profile.identity) || !text_valid(value.schema.identity) || !text_valid(value.argument_type) || !text_valid(value.encoding) ||
               !value.validate_arguments)
            {
                return result::invalid;
            }
            std::lock_guard lock{mutex};
            if(!configured || retired) { return result::denied; }
            for(auto const& existing: tools)
            {
                if(existing.id)
                {
                    if(existing.value.profile == value.profile) { return result::conflict; }
                    if(existing.value.semantics.schema == value.semantics.schema && existing.value.schema != value.schema) { return result::conflict; }
                }
            }
            tool_slot* empty{};
            for(std::size_t i{}; i != capacity.tools; ++i)
            {
                if(!tools[i].id)
                {
                    empty = &tools[i];
                    break;
                }
            }
            if(!empty) { return result::again; }
            auto status = state.bind_tool(value.semantics, output);
            if(status == result::success) { *empty = {output, value}; }
            return status;
        }

        /** @brief Bind an independently authorized capability as its known guest session.
         * @details The broker validates this capability/actor in its own typed
         * authority map. Resource and capability integers may be equal; this
         * method never infers ownership from numeric global uniqueness.
         */
        result bind_reader(admission::grant value, identity validated_capability, identity& output)
        {
            if(!same_process()) { return result::denied; }
            if(!validated_capability) { return result::invalid; }
            std::lock_guard lock{mutex};
            if(!configured || retired) { return result::denied; }
            auto status = state.bind_reader(value, output, validated_capability);
            if(status == result::success) { ++reader_count; }
            return status;
        }

        /** @brief Bind trusted provenance to the exact actual sealed allocation.
         * @details The host source adapter establishes timestamps/frames/epochs
         * under current source authority; guest buffer bytes cannot assign them.
         * On failure the caller retains its lease and no source slot is consumed.
         */
        result bind_source(admission::observation metadata, std::string_view frame, buffers::registry::sealed_lease& bytes, identity& output)
        {
            if(!same_process()) { return result::denied; }
            std::lock_guard lock{mutex};
            if(!configured || retired) { return result::denied; }
            source_slot* empty{};
            for(std::size_t i{}; i != capacity.sources; ++i)
            {
                if(!sources[i].id)
                {
                    empty = &sources[i];
                    break;
                }
            }
            if(!empty) { return result::again; }
            for(auto const& existing: sources)
            {
                if(existing.id && existing.snapshot->metadata.frame == metadata.frame && existing.snapshot->frame.view() != frame) { return result::invalid; }
            }
            auto prepared = prepare(metadata, frame, bytes);
            if(!prepared) { return result::invalid; }
            auto status = state.bind_source(metadata, output);
            if(status != result::success) { return status; }
            prepared->owner = identity_scope;
            prepared->bytes = std::move(bytes);
            *empty = {output, std::move(prepared), false};
            return result::success;
        }

        /** @brief Replace only the current sample while existing tokens pin old bytes.
         * @details Core lineage monotonicity/ABA checks reject rollback. Updates
         * do not relabel prior inference inputs or revive withdrawn authority.
         */
        result update_source(identity id, admission::observation metadata, std::string_view frame, buffers::registry::sealed_lease& bytes)
        {
            if(!same_process()) { return result::denied; }
            std::lock_guard lock{mutex};
            if(!configured || retired) { return result::denied; }
            for(auto& value: sources)
            {
                if(value.id == id)
                {
                    // Canonical frame spelling is bound to the same numeric frame.
                    if(value.snapshot->metadata.frame == metadata.frame && value.snapshot->frame.view() != frame) { return result::invalid; }
                    for(auto const& existing: sources)
                    {
                        if(existing.id && existing.snapshot->metadata.frame == metadata.frame && existing.snapshot->frame.view() != frame)
                        {
                            return result::invalid;
                        }
                    }
                    auto prepared = prepare(metadata, frame, bytes);
                    if(!prepared) { return result::invalid; }
                    auto status = state.update_source(id, metadata);
                    if(status != result::success) { return status; }
                    prepared->owner = identity_scope;
                    prepared->bytes = std::move(bytes);
                    value.snapshot = std::move(prepared);
                    value.fenced = false;
                    return result::success;
                }
            }
            return result::bad_handle;
        }

        /** @brief Capture an immutable input token before submitting native inference. */
        result capture(identity id, snapshot_token& output) const
        {
            if(!same_process()) { return result::denied; }
            std::lock_guard lock{mutex};
            if(!configured || retired) { return result::denied; }
            for(auto const& value: sources)
            {
                if(value.id == id)
                {
                    if(value.fenced) { return result::denied; }
                    auto status = current(value.snapshot);
                    if(status != result::success) { return status; }
                    output.value = value.snapshot;
                    return result::success;
                }
            }
            return result::bad_handle;
        }

    private:
        /** @brief Publish model data only with the exact inputs used by that completed job.
         * @details Argument validation runs outside locks. Captured observations
         * survive newer samples, subject to the selected bounded-age/latest
         * policy. The broker holds current model/source group authority; this
         * method does not authorize model text or infer a physical effect.
         */
        template<typename Evidence> result publish_evidence(identity model_request,
                       identity tool_id,
                       std::span<std::uint8_t const> arguments,
                       std::span<snapshot_token const> inputs,
                       std::uint64_t expiry,
                       identity& output,Evidence&& verify)
        {
            if(!same_process()) { return result::denied; }
            tool_profile selected;
            buffers::text<255> model;
            {
                std::lock_guard lock{mutex};
                if(!configured || retired) { return result::denied; }
                bool found{};
                for(auto const& value: tools)
                {
                    if(value.id == tool_id)
                    {
                        selected = value.value;
                        found = true;
                        break;
                    }
                }
                if(!found) { return result::bad_handle; }
                if(arguments.size() > capacity.argument_bytes || inputs.size() > capacity.observations) { return result::too_large; }
                if(!model.assign(model_alias)) { return result::invalid; }
            }
            if(!selected.validate_arguments(arguments)) { return result::invalid; }
            proposal_slot prepared;
            prepared.selected = selected;
            prepared.input_count = inputs.size();
            std::array<admission::observation, 8> observed{};
            std::array<models::input_evidence, 8> evidence{};
            for(std::size_t i{}; i != inputs.size(); ++i)
            {
                auto snapshot = inputs[i].value;
                auto status = current(snapshot);
                if(status != result::success) { return status; }
                if(snapshot->owner != identity_scope) { return result::denied; }
                prepared.inputs[i] = std::move(snapshot);
                observed[i] = prepared.inputs[i]->metadata;
                evidence[i] = prepared.inputs[i]->evidence;
            }
            auto checked = verify(model.view(),std::span<models::input_evidence const>{evidence.data(), inputs.size()});
            if(checked != resources::error::success) { return convert(checked); }
            // Allocation and authority/provider callbacks run without the storage
            // mutex: callbacks may synchronously revoke or inspect this provider.
            auto retained = ::uwvm2::utils::container::make_shared<proposal_slot>(std::move(prepared));
            checked=verify(model.view(),std::span<models::input_evidence const>{evidence.data(),inputs.size()});
            if(checked!=resources::error::success)return convert(checked);
            std::lock_guard lock{mutex};
            if(!configured || retired) { return result::denied; }
            std::shared_ptr<proposal_slot>* empty{};
            for(std::size_t i{}; i != capacity.proposals; ++i)
            {
                if(!proposals[i]) { empty=&proposals[i]; break; }
            }
            if(!empty) { return result::again; }
            auto status=all_current(*retained);
            if(status!=result::success)return status;
            // A reentrant validator may fence a target. Never publish the old
            // wrapper schema alongside a newly fenced core tool.
            bool same_tool{};
            for(auto const& entry:tools)
                if(entry.id==tool_id&&entry.value.semantics==selected.semantics&&entry.value.validate_arguments==selected.validate_arguments)
                    same_tool=true;
            if(!same_tool)return result::conflict;
            status = state.publish_snapshots(producer, model_request, tool_id, arguments, {observed.data(), inputs.size()}, expiry, output);
            if(status == result::success)
            {
                retained->id = output;
                *empty = std::move(retained);
            }
            return status;
        }

    public:
        /** @brief Existing trusted core publication fixture; frontends use publish_completed.
         * @details This route checks model input evidence only. It is retained
         * for separately trusted adapters/tests, never exposed as a guest import
         * or accepted as final-output provenance by the production dispatcher.
         */
        result publish(models::registry const& models,identity model_request,identity tool_id,std::span<std::uint8_t const> arguments,
                       std::span<snapshot_token const> inputs,std::uint64_t expiry,identity& output)
        {
            return publish_evidence(model_request,tool_id,arguments,inputs,expiry,output,
                [&](std::string_view alias,std::span<models::input_evidence const> expected){return models.check_inputs(alias,model_request,expected);});
        }
        /** @brief Retain proposal arguments only from an authenticated complete final envelope.
         * @details The dispatcher decodes and matches every envelope field before
         * calling this method. The noncopyable completion token was constructed
         * under actual model/source authority and is revalidated immediately
         * before the no-callback local commit. Exact captured inputs must match.
         */
        template<typename FinalAdmission>
        result publish_completed(model_dispatch::completion const& completion,identity tool_id,std::span<std::uint8_t const> arguments,
                                 std::span<snapshot_token const> inputs,std::uint64_t expiry,identity& output,FinalAdmission&& final_admission)
        {
            return publish_evidence(completion.request(),tool_id,arguments,inputs,expiry,output,
                [&](std::string_view alias,std::span<models::input_evidence const> expected)
                {
                    if(completion.alias()!=alias||!completion.matches_inputs(expected))return resources::error::denied;
                    auto status=final_admission();if(status!=resources::error::success)return status;
                    return completion.revalidate();
                });
        }

        /** @brief Deliver a detached record and pinned canonical provenance, nonconsuming. */
        template <typename Sink>
        result read(std::string_view selected, identity actor, identity reader, identity proposal, Sink&& sink)
        {
            if(!same_process()) { return result::denied; }
            std::shared_ptr<proposal_slot> held;
            {
                std::lock_guard lock{mutex};
                if(!live(selected)) { return result::denied; }
                held = find(proposal);
                if(!held) { return result::bad_handle; }
            }
            return state.read(actor,
                              reader,
                              scope,
                              proposal,
                              [&](auto const& record)
                              {
                                  if(all_current(*held) != result::success) { return false; }
                                  return sink(record, held->selected, std::span{held->inputs.data(), held->input_count});
                              });
        }

        /** @brief Admit only inert approval with independent actor and exact target rights.
         * @details Actual source leases are checked before and after the bounded
         * validator, which runs outside all wrapper/core locks. Receipt replay
         * remains possible after explicit proposal release and does no new work.
         */
        template <typename Validator>
        result accept(std::string_view selected,
                      identity actor,
                      admission::accept_request const& request,
                      admission::authorization_context context,
                      Validator&& validator,
                      admission::receipt& output)
        {
            if(!same_process()) { return result::denied; }
            if(!context.proposal_scope || !context.target_scope) { return result::denied; }
            {
                std::lock_guard lock{mutex};
                if(!live(selected)) { return result::denied; }
            }
            return state.accept(
                actor,
                request,
                [&](auto const& record)
                {
                    std::shared_ptr<proposal_slot> held;
                    {
                        std::lock_guard lock{mutex};
                        held = find(record.id);
                    }
                    if(!held) { return result::bad_handle; }
                    auto status = all_current(*held);
                    if(status != result::success) { return status; }
                    status = validator(record, held->selected);
                    if(status != result::success) { return status; }
                    return all_current(*held);
                },
                output,
                context);
        }

        result probe_retry(std::string_view selected,identity actor,admission::accept_request const& request,
                           admission::authorization_context context,admission::receipt& output,bool& found) noexcept
        {
            found=false;if(!same_process())return result::denied;
            {std::lock_guard lock{mutex};if(!live(selected))return result::denied;}
            return state.probe_retry(actor,request,context,output,found);
        }

        /** @brief Release pins only when no read/validator still borrows the proposal. */
        result release(std::string_view selected, identity proposal)
        {
            if(!same_process()) { return result::denied; }
            std::lock_guard lock{mutex};
            if(!live(selected)) { return result::denied; }
            for(auto& value: proposals)
            {
                if(value && value->id == proposal)
                {
                    if(value.use_count() != 1) { return result::again; }
                    auto status = state.release(proposal);
                    if(status == result::success) { value.reset(); }
                    return status;
                }
            }
            return result::bad_handle;
        }

        /** @brief Fence source permission independently of new sample publication. */
        result fence_source_authority(identity id, identity epoch)
        {
            if(!same_process()) { return result::denied; }
            std::lock_guard lock{mutex};
            auto status = state.fence_source_authority(id, epoch);
            if(status == result::success)
            {
                for(auto& value: sources)
                {
                    if(value.id == id) { value.fenced = true; }
                }
            }
            return status;
        }

        /** @brief Fence target authority/provider/controller ownership without effects. */
        result fence_target(identity id, identity generation, identity authority, identity controller)
        {
            if(!same_process()) { return result::denied; }
            std::lock_guard lock{mutex};
            auto status=state.fence_target(id, generation, authority, controller);
            if(status==result::success)for(auto& tool:tools)if(tool.id==id)
            {tool.value.semantics.provider_generation=generation;tool.value.semantics.target_authority_epoch=authority;tool.value.semantics.controller_lease_epoch=controller;}
            return status;
        }

        /** @brief Explicitly reduce arbiter rate while fencing in-flight validation.
         * @details Zero forbids new approvals. Existing inert receipts remain
         * recoverable; neither the actor nor target actions can be expanded.
         */
        result attenuate_reader(identity id, std::uint64_t calls, std::uint64_t window_ns, identity authority_epoch)
        {
            if(!same_process()) { return result::denied; }
            std::lock_guard lock{mutex};
            return state.attenuate_reader(id, calls, window_ns, authority_epoch);
        }

        /** @brief Permanently withdraw source admission, retaining tombstone identity. */
        void revoke_source(identity id)
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            state.revoke_source(id);
            for(auto& value: sources)
            {
                if(value.id == id) { value.fenced = true; }
            }
        }

        /** @brief Permanently withdraw one independently granted reader/arbiter. */
        void revoke_reader(identity id)
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            state.revoke_reader(id);
        }

        /** @brief Permanently withdraw an immutable tool without affecting physical state. */
        void revoke_tool(identity id)
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            state.revoke_tool(id);
        }

        /** @brief Nonblocking alias aggregate without payload, identity or state advancement. */
        result try_inspect(std::string_view selected, usage& output) const noexcept
        {
            if(!same_process()) { return result::denied; }
            std::unique_lock lock{mutex, std::try_to_lock};
            if(!lock.owns_lock()) { return result::again; }
            if(!configured || selected != alias) { return result::denied; }
            usage sampled;
            sampled.reader_slots = reader_count;
            sampled.retired = retired;
            for(auto const& value: proposals)
            {
                if(value)
                {
                    ++sampled.proposals;
                    sampled.snapshot_references += value->input_count;
                }
            }
            for(auto const& value: sources)
            {
                if(value.id)
                {
                    ++sampled.source_slots;
                    if(value.snapshot) { ++sampled.snapshot_references; }
                }
            }
            for(auto const& value: tools)
            {
                if(value.id) { ++sampled.tool_slots; }
            }
            output = sampled;
            return result::success;
        }

        /** @brief Retire local admission, then require caller/native-worker join. */
        void retire() noexcept
        {
            if(!same_process()) { return; }
            std::lock_guard lock{mutex};
            retired = true;
            state.retire();
        }
    };
}  // namespace uwvm2::imported::ros::scene::proposals
