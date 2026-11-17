/** @file
 * @brief Bounded internal CPU worker framing and borrowed backend invocation.
 * @details This is a trusted native adapter ABI, not a guest import or a model
 * tool protocol. Its exact executable/backend closure is digest pinned. The
 * bootstrap installs its final syscall filter before reading any IPC/model data.
 */
#pragma once
#include "wire.h"
#include <array>
#include <span>
#include <cstdint>
#include <string_view>

namespace uwvm2::imported::ros::scene::model_worker
{
    inline constexpr std::string_view ready = "uwvm.cpu.ready\n", attached = "uwvm.cpu.attached\n";
    enum class response : std::uint64_t
    {
        partial = 1,
        succeeded = 2,
        failed = 3
    };

    struct input_view
    { wire::bytes layout, data; };

    struct weight_view
    {
        int descriptor{-1};
        std::array<char,255> name{};
        std::size_t name_size{};
        std::uint64_t bytes{};
        std::string_view identity() const noexcept { return {name.data(),name_size}; }
    };

    struct invocation
    {
        std::uint64_t request{};
        wire::bytes parameters;
        std::array<input_view, 64> inputs{};
        std::size_t input_count{};
        std::span<weight_view const> weights;
    };

    using emit_function = bool (*)(void*, response, wire::bytes) noexcept;
    using backend_function = bool (*)(invocation const&, emit_function, void*) noexcept;

    /** @brief Decode complete owned bytes; views never escape the synchronous backend call. */
    inline bool decode(wire::bytes bytes, invocation& output) noexcept
    {
        wire::reader reader{bytes};
        invocation value;
        std::uint64_t count{};
        if(!reader.u64(value.request) || !value.request || !reader.blob(value.parameters) || !reader.u64(count) || count > value.inputs.size())
        {
            return false;
        }
        value.input_count = static_cast<std::size_t>(count);
        for(std::size_t i{}; i != value.input_count; ++i)
        {
            if(!reader.blob(value.inputs[i].layout) || !reader.blob(value.inputs[i].data)) { return false; }
        }
        if(!reader.done()) { return false; }
        output = value;
        return true;
    }

    /** @brief Validate one response before it enters the model job's own bounded output queue. */
    inline bool decode_response(wire::bytes bytes, response& kind, std::uint64_t& request, wire::bytes& body) noexcept
    {
        wire::reader reader{bytes};
        std::uint64_t tag{}, id{};
        wire::bytes payload;
        if(!reader.u64(tag) || tag < 1 || tag > 3 || !reader.u64(id) || !id || !reader.blob(payload) || !reader.done()) { return false; }
        if(tag == 3 && !payload.empty()) { return false; }
        kind = static_cast<response>(tag);
        request = id;
        body = payload;
        return true;
    }
}  // namespace uwvm2::imported::ros::scene::model_worker
