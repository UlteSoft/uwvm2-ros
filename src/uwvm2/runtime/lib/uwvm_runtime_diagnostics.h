// Bounded RELEASE observations. No pointer, live guest value or mutation request crosses this ABI.
#pragma once
#ifndef UWVM_MODULE
#include <array>
#include <cstdint>
#endif
namespace uwvm2::runtime::lib
{
    struct diagnostic_frame { ::std::uint64_t module{}, function{}, generation{}; };
    struct diagnostic_module { ::std::array<unsigned char,32> sha256{}; ::std::uint64_t bytes{}; };
    struct diagnostic_snapshot
    {
        ::std::uint64_t runtime_generation{}, sequence{}, roots_started{}, roots_completed{}, roots_active{};
        ::std::uint64_t installed_epoch{}, authenticated_commit_epoch{}, decision_sequence{};
        ::std::array<unsigned char,32> installed_sha256{};
        ::std::array<diagnostic_module,16> modules{};
        ::std::array<diagnostic_frame,16> frames{};
        ::std::uint32_t module_count{}, total_modules{}, frame_count{}, fault_kind{}, update_phase{};
        bool update_in_progress{}, module_truncated{}, stack_truncated{}, collection_lost{}, faulted{}, update_managed{}, active{}, coordinator_connected{};
    };
    // Trusted cold initialization only. This does not enable a debugger, root
    // entry, import, provider, policy change or update session.
    extern "C++" bool runtime_diagnostic_initialize_host_api() noexcept;
    // A bounded try-lock copy of previously recorded metadata; never stops Guest.
    extern "C++" bool runtime_diagnostic_snapshot_host_api(diagnostic_snapshot&) noexcept;
}
