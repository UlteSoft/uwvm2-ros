// SPDX-License-Identifier: Apache-2.0
// Trusted native test observer for the existing preload host API. This is not
// an SDK Wasm library or a substitute for a Wasm library consumer.
#include <uwvm2/uwvm/wasm/type/dl.h>
#include <uwvm2/uwvm/wasm/type/preload_api.h>
#include <cstdint>
#include <cstring>
#include <limits>

namespace ty = ::uwvm2::uwvm::wasm::type;
static ty::uwvm_preload_host_api_v1 const* host{};

extern "C" void uwvm_set_preload_host_api_v1(ty::uwvm_preload_host_api_v1 const* api) noexcept
{
    host = api;
}

extern "C" ty::capi_module_name_t uwvm_get_module_name() noexcept
{
    return {"host-memory-bounds", 18};
}

static void probe(::std::byte* result, ::std::byte* parameters)
{
    ::std::uint32_t index{}, permitted{}, failures{};
    ::std::uint64_t expected_bytes{};
    ::std::memcpy(&index, parameters, 4);
    ::std::memcpy(&expected_bytes, parameters + 4, 8);
    ::std::memcpy(&permitted, parameters + 12, 4);
    if(host == nullptr || host->struct_size < sizeof(*host) || host->abi_version != 1 ||
       host->memory_descriptor_count == nullptr || host->memory_descriptor_at == nullptr ||
       host->memory_read == nullptr || host->memory_write == nullptr)
    {
        failures = UINT32_MAX;
        ::std::memcpy(result, &failures, 4);
        return;
    }

    bool found{};
    auto const count{host->memory_descriptor_count()};
    for(::std::size_t i{}; i != count; ++i)
    {
        ty::uwvm_preload_memory_descriptor_t descriptor{};
        if(host->memory_descriptor_at(i, &descriptor) && descriptor.memory_index == index)
        {
            found = true;
            failures += permitted && descriptor.byte_length != expected_bytes;
        }
    }
    failures += found != (permitted != 0);
    auto const maximum{(::std::numeric_limits<::std::uint64_t>::max)()};
    ::std::byte byte{};
    if(permitted)
    {
        failures += !host->memory_read(index, 0, nullptr, 0);
        failures += !host->memory_write(index, 0, nullptr, 0);
        failures += !host->memory_read(index, expected_bytes, nullptr, 0);
        failures += !host->memory_write(index, expected_bytes, nullptr, 0);
        if(expected_bytes != maximum)
        {
            failures += host->memory_read(index, expected_bytes + 1, nullptr, 0);
            failures += host->memory_write(index, expected_bytes + 1, nullptr, 0);
        }
        failures += host->memory_read(index, maximum, nullptr, 0);
        failures += host->memory_write(index, maximum, nullptr, 0);
        failures += host->memory_read(index, expected_bytes, &byte, 1);
        failures += host->memory_write(index, expected_bytes, &byte, 1);
        failures += host->memory_read(index, 0, nullptr, 1);
        failures += host->memory_write(index, 0, nullptr, 1);
        if(expected_bytes != 0)
        {
            ::std::byte original{}, observed{};
            ::std::byte const marker{0x5a};
            failures += !host->memory_read(index, expected_bytes - 1, &original, 1);
            failures += !host->memory_write(index, expected_bytes - 1, &marker, 1);
            failures += !host->memory_read(index, expected_bytes - 1, &observed, 1);
            failures += observed != marker;
            failures += !host->memory_write(index, expected_bytes - 1, &original, 1);
        }
    }
    else
    {
        failures += host->memory_read(index, 0, nullptr, 0);
        failures += host->memory_write(index, 0, nullptr, 0);
        failures += host->memory_read(index, 0, &byte, 1);
        failures += host->memory_write(index, 0, &byte, 1);
    }
    ::std::memcpy(result, &failures, 4);
}

extern "C" ty::capi_function_vec_t uwvm_function() noexcept
{
    static ::std::uint_least8_t const parameters[]{0x7f, 0x7e, 0x7f};
    static ::std::uint_least8_t const results[]{0x7f};
    static ty::capi_function_t const functions[]{
        {"probe", 5, parameters, 3, results, 1, &probe}};
    return {functions, 1};
}
