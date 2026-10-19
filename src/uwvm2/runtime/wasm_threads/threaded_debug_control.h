#pragma once
// Host-only B1 control. These objects convey no Guest, checkpoint, mutation,
// native-stack or ROS permission. Only the actual Runtime/Group can issue them.
#ifndef UWVM_MODULE
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <span>
#include <thread>
#include "threaded_debug_session.h"
#include "admission.h"
#include <uwvm2/utils/thread/cooperative_pause_domain.h>
#endif
#if defined(__linux__) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
namespace uwvm2::runtime::wasm_threads
{
    enum class threaded_debug_control : unsigned { none, cooperative_all_stop };
    struct threaded_debug_control_limits
    {
        threaded_debug_control kind{threaded_debug_control::none};
        // Positive finite requested-payload quota for independent P + bounded
        // roster scratch. Also charged to the existing live-storage total.
        ::std::size_t maximum_control_storage_bytes{};
    };
    enum class debug_stop_status : unsigned { stopped, busy, timeout, stale, unavailable, insufficient_capacity };
    enum class debug_stop_site : unsigned
    { native_boundary, before_instruction, function_boundary, host_boundary, atomic_wait };
    struct debug_stopped_worker
    {
        ::std::uint64_t participant{}, incarnation{}, native_tid{}, source_generation{}, validation_epoch{};
        ::std::size_t slot{}, module{SIZE_MAX}, function{SIZE_MAX}, instruction{SIZE_MAX};
        ::std::uint64_t wasm_offset{UINT64_MAX};
        debug_backend backend{};
        debug_stop_site site{debug_stop_site::native_boundary};
        // Only before_instruction has an exact logical position. None of these
        // rows claims a source line, locals, unwind or a saved machine context.
    };
    class threaded_debug_all_stop final
    {
        friend class ThreadedGroup;
        friend class threaded_debug_control_session;
        // Observer identity only. The real Root/native/compiler owners retain
        // live code/World/Source until physical join. Never keep a provider
        // generation lease across a later public stop/drain through this guard.
        ::std::weak_ptr<ThreadedGroup> group_;
        ::std::weak_ptr<ThreadedSpawnAdmission const> admission_;
        ::std::uint64_t serial_{};
        ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket ticket_;
        ::std::thread::id owner_{};
    public:
        threaded_debug_all_stop() noexcept = default;
        threaded_debug_all_stop(threaded_debug_all_stop const&) = delete;
        threaded_debug_all_stop& operator=(threaded_debug_all_stop const&) = delete;
        threaded_debug_all_stop(threaded_debug_all_stop&&) = delete;
        threaded_debug_all_stop& operator=(threaded_debug_all_stop&&) = delete;
        ~threaded_debug_all_stop();
        void reset() noexcept;
        [[nodiscard]] debug_stop_status read(::std::span<debug_stopped_worker>, ::std::size_t& needed) const noexcept;
    };
    class threaded_debug_control_session final
    {
        friend class ThreadedGroup;
        ::std::weak_ptr<ThreadedGroup> group_;
        ::std::uint64_t epoch_{};
        threaded_debug_control_session(::std::weak_ptr<ThreadedGroup> group, ::std::uint64_t epoch) noexcept
            : group_{::std::move(group)}, epoch_{epoch} {}
    public:
        threaded_debug_control_session() noexcept = default;
        // Deadline bounds cooperative waiting, with a final elapsed recheck.
        // request_pause synchronously wakes actual wait shards; mutex/waker
        // contention can delay this call beyond deadline. It is NOT a hard
        // real-time bound. Embedders still require an owned outer deadline and
        // real stop/physical-join handling; timeout conveys no stopped proof.
        [[nodiscard]] debug_stop_status stop_until(::std::chrono::steady_clock::time_point,
            threaded_debug_all_stop&) const noexcept;
    };
}
#endif
