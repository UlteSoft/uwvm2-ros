/** @file
 * @brief Canonical model/request records with bounded borrowed input views.
 * @details These helpers allocate nothing and grant no authority. The host
 * owns the complete carrier request, then independently validates every source
 * capability/provenance and pins its sealed buffer before job admission.
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#define UWVM_ROS_MODEL_FUNCTIONS(X)                                                                                                                            \
    X(model_open, open, "model.open")                                                                                                                          \
    X(model_submit, submit, "model.submit")                                                                                                                    \
    X(model_take, take, "model.take")                                                                                                                          \
    X(model_cancel, cancel, "model.cancel")                                                                                                                    \
    X(model_describe, describe, "model.describe")
#define UWVM_ROS_REQUEST_FUNCTIONS(X) \
    X(request_describe, request_describe, "request.describe") \
    X(request_release, request_release, "request.release")

#ifndef UWVM_SCENE_MODEL_CONTRACT_MACROS_ONLY
# include "model_jobs.h"
# include "wire.h"

namespace uwvm2::imported::ros::model_contract
{
    namespace model = scene::models;
    namespace wire = scene::wire;
    enum class operation : std::uint8_t
    {
        open,
        submit,
        take,
        cancel,
        describe,
        request_describe,
        request_release
    };

    struct descriptor
    {
        operation id;
        std::string_view name;
    };

    inline constexpr std::array operations{
# define UWVM_MODEL_DESCRIPTOR(symbol, action, name) descriptor{operation::action, name},
        UWVM_ROS_MODEL_FUNCTIONS(UWVM_MODEL_DESCRIPTOR)
        UWVM_ROS_REQUEST_FUNCTIONS(UWVM_MODEL_DESCRIPTOR)
# undef UWVM_MODEL_DESCRIPTOR
    };

    /** @brief Canonical observation syntax only; provenance must come from host registry. */
    struct observation
    {
        std::uint64_t capability{}, buffer{}, source{}, source_generation{}, clock{}, epoch{};
        std::int64_t nanoseconds{};
        bool stamped{}, framed{};
        scene::buffers::text<96> frame;
    };

    struct request
    {
        model::request_key key;
        std::uint64_t session{}, request_id{}, deadline{};
        model::profile selected;
        scene::buffers::text<96> parameter_type;
        scene::buffers::text<32> encoding;
        std::span<std::uint8_t const> parameters;
        std::array<observation, 64> inputs{};
        std::size_t input_count{};
        model::capacity reservation;
    };

    /** @brief Decode exactly one bounded canonical immutable profile. */
    inline bool read_profile(wire::reader& input, model::profile& output) noexcept
    {
        model::profile value;
        std::string_view name;
        wire::bytes digest;
        if(!input.text(name) || name.empty() || !value.identity.assign(name) || !input.blob(digest) || digest.size() != 32) { return input.reject(); }
        std::copy(digest.begin(), digest.end(), value.sha256.begin());
        output = value;
        return true;
    }

    inline bool write_profile(wire::writer& output, model::profile const& value) noexcept
    { return value.identity.size <= scene::buffers::maximum_identity && output.text(value.identity.view()) && output.blob(value.sha256); }

    /** @brief Decode all fields without accepting unknown/trailing/oversized data. */
    inline bool decode(operation action, wire::bytes bytes, request& output) noexcept
    {
        wire::reader input{bytes};
        request value;
        switch(action)
        {
            case operation::open:
                if(!read_profile(input, value.selected)) { return false; }
                break;
            case operation::submit:
            {
                std::string_view text;
                std::uint64_t count{};
                if(!input.u64(value.key.session) || !input.u64(value.key.sequence) || !input.u64(value.session) || !read_profile(input, value.selected) ||
                   !input.text(text) || text.empty() || !value.parameter_type.assign(text) || !input.text(text) || text.empty() ||
                   !value.encoding.assign(text) || !input.blob(value.parameters) || value.parameters.size() > scene::buffers::maximum_transfer ||
                   !input.u64(count) || count > value.inputs.size())
                {
                    return false;
                }
                value.input_count = static_cast<std::size_t>(count);
                for(std::size_t i{}; i != value.input_count; ++i)
                {
                    auto& current = value.inputs[i];
                    if(!input.u64(current.capability) || !input.u64(current.buffer) || !input.u64(current.source) || !input.u64(current.source_generation) ||
                       !input.boolean(current.stamped))
                    {
                        return false;
                    }
                    if(current.stamped && (!input.u64(current.clock) || !input.u64(current.epoch) || !input.i64(current.nanoseconds))) { return false; }
                    if(!input.boolean(current.framed)) { return false; }
                    if(current.framed && (!input.text(text) || text.empty() || !current.frame.assign(text))) { return false; }
                }
                if(!input.u64(value.reservation.bytes) || !input.u64(value.reservation.items) || !input.u64(value.reservation.workspace_bytes) ||
                   !input.u64(value.deadline))
                {
                    return false;
                }
                break;
            }
            case operation::take:
            case operation::cancel:
                if(!input.u64(value.session) || !input.u64(value.request_id)) { return false; }
                break;
            case operation::describe:
                if(!input.u64(value.session)) { return false; }
                break;
            case operation::request_describe:
            case operation::request_release:
                if(!input.u64(value.request_id)) { return false; }
                break;
            default: return false;
        }
        if(!input.done()) { return false; }
        output = value;
        return true;
    }

    /** @brief Bound host copying before allocating a variable inference request. */
    inline std::uint64_t request_bound(operation action, std::uint64_t parameter_bytes, std::uint64_t inputs) noexcept
    {
        switch(action)
        {
            case operation::open: return 144;
            case operation::submit: return 512 + std::min(parameter_bytes, scene::buffers::maximum_transfer) + 184 * std::min(inputs, std::uint64_t{64});
            case operation::take:
            case operation::cancel: return 16;
            case operation::describe:
            case operation::request_describe:
            case operation::request_release: return 8;
            default: return 0;
        }
    }

    /** @brief Preserve the catalogue's closed receipt state/effect semantics. */
    inline bool encode_receipt(wire::writer& output, model::receipt const& value) noexcept
    {
        auto phase = static_cast<unsigned>(value.phase), reason = static_cast<unsigned>(value.why);
        if(phase >= model::state_names.size() || reason >= model::reason_names.size()) { return false; }
        std::string_view effect = value.phase == model::state::outcome_unknown                                  ? "unknown"
                                  : value.phase == model::state::succeeded                                      ? "committed"
                                  : value.phase == model::state::queued || value.phase == model::state::running ? "locally_accepted"
                                  : value.had_output                                                            ? "partial"
                                                                                                                : "none";
        return output.u64(value.request) && output.text(model::state_names[phase]) && output.text(effect) && output.u64(value.provider_generation) &&
               output.text(model::reason_names[reason]);
    }

    /** @brief Encode a host-owned model output with explicitly absent native metadata.
     * @details Source/session and sequence are this local provider's identities.
     * Native timestamps and publisher identity are absent, not fabricated.
     * kind distinguishes a partial stream item from the terminal model result.
     */
    inline bool encode_take(wire::writer& output, std::uint64_t session, model::receipt const& receipt, model::output const* chunk) noexcept
    {
        if(!encode_receipt(output, receipt) || !output.boolean(chunk != nullptr)) { return false; }
        if(!chunk) { return true; }
        return output.u64(session) && output.u64(receipt.provider_generation) && output.u64(chunk->sequence) && output.boolean(false) &&
               output.boolean(false) && output.boolean(false) && output.u64(0) && output.u64(receipt.request) &&
               output.text(chunk->terminal ? "model.result" : "model.partial") && output.text(chunk->type.view()) && output.text(chunk->encoding.view()) &&
               output.blob(chunk->bytes);
    }

    /** @brief Describe only approved model/task and implemented execution features. */
    inline bool encode_description(wire::writer& output, model::profile const& model, model::profile const& task, bool streaming) noexcept
    {
        return write_profile(output, model) && output.u64(streaming ? 3 : 2) && output.text(task.identity.view()) && output.text("sealed-inputs") &&
               (!streaming || output.text("streaming"));
    }
}  // namespace uwvm2::imported::ros::model_contract
#endif
