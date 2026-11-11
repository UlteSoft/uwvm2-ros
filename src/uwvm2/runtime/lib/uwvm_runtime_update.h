#pragma once
#include <chrono>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <span>
#include <uwvm2/utils/update/protocol.h>
#include <uwvm2/utils/update/scalar_state.h>
#include <uwvm2/utils/update/transition.h>
#include <uwvm2/utils/update/module_transition.h>
#include <uwvm2/utils/update/migration_state.h>
namespace uwvm2::runtime::lib
{
    // HOST-only serialization, never an update grant. Debug commands fail
    // promptly while the signed coordinator owns publication; parked Guest
    // frames retain their real execution leases and cannot authorize a drain.
    namespace update_debug_coordination
    {
        inline ::std::mutex management{};
        inline ::std::atomic<bool> configured{}, fenced{};
        using publication_callback = void(*)(void*, ::std::uint64_t, ::std::uint64_t,
            ::std::span<::std::byte const>, ::std::uint64_t) noexcept;
        inline publication_callback code_published{}; // Serialized cold launch only.
    }
    // Trusted cold setup and private compilation; public admission remains closed
    // until a nonce-bound launch permit. Fixed authority; no guest imports.
    // All management calls are serialized on the configuring native thread.
    extern "C++" ::uwvm2::utils::update::status runtime_update_configure_host_api(
        ::uwvm2::utils::update::configuration const&) noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_launch_host_api(
        ::uwvm2::utils::update::permit const&) noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_observe_host_api(
        ::uwvm2::utils::update::facts&) noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_prepare_host_api(
        ::uwvm2::utils::update::permit const&, ::std::span<unsigned char const> complete_target,
        ::std::span<::std::uint64_t const> local_functions, ::std::chrono::steady_clock::time_point deadline) noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_prepare_transition_host_api(
        ::uwvm2::utils::update::permit const&, ::std::span<unsigned char const> complete_target,
        ::std::span<unsigned char const> manifest, ::std::chrono::steady_clock::time_point deadline) noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_observe_transition_host_api(
        ::uwvm2::utils::update::transition_facts&) noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_read_policy_host_api(::std::vector<unsigned char>&) noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_ready_host_api() noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_cancel_private_host_api() noexcept;
    extern "C++" ::uwvm2::utils::update::status runtime_update_decide_host_api(
        ::uwvm2::utils::update::permit const&) noexcept;
    extern "C++" void runtime_update_disconnected_host_api() noexcept;
}
