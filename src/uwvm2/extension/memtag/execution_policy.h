// Host-selected execution policy. Guest instructions never mutate this state.
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <cstdint>
# include <cstddef>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    // Four internal states; the CLI exposes orthogonal backend and delivery axes.
    enum class fault_delivery : unsigned char { synchronous, software_async, hardware_async, hardware_sync };
    // Configured during cold startup, before any owner or generated code is
    // published. Atomic reads also make the policy safe for admitted workers.
    inline ::std::atomic<fault_delivery> selected_fault_delivery{fault_delivery::synchronous};
    inline ::std::atomic<unsigned> asynchronous_worker_count{1u};
    enum class report_action : unsigned char { crash, silent_log };
    inline ::std::atomic<report_action> selected_report_action{report_action::crash};
    // Dedicated cold report descriptor; never the general logger or a Guest FD.
    inline int report_descriptor{2}; // Standalone native host/test sink.
    // Cold fstat witness for this independently owned, immutable descriptor.
    // Guest FD operations cannot replace it. Nonregular/unknown sinks keep the
    // SIGPIPE-safe writer; no path spelling is treated as proof of file type.
    inline bool report_regular_file{};
    [[nodiscard]] inline bool logging_requested() noexcept
    { return selected_report_action.load(::std::memory_order_relaxed)==report_action::silent_log; }
    [[nodiscard]] constexpr bool is_asynchronous(fault_delivery mode) noexcept
    { return mode==fault_delivery::software_async || mode==fault_delivery::hardware_async; }
    [[nodiscard]] constexpr bool is_hardware(fault_delivery mode) noexcept
    { return mode==fault_delivery::hardware_sync || mode==fault_delivery::hardware_async; }
    [[nodiscard]] inline bool asynchronous_requested() noexcept
    { return is_asynchronous(selected_fault_delivery.load(::std::memory_order_relaxed)); }
    [[nodiscard]] inline bool hardware_requested() noexcept
    { return is_hardware(selected_fault_delivery.load(::std::memory_order_relaxed)); }
    [[nodiscard]] inline bool hardware_checked_access_requested() noexcept
    { return hardware_requested() && !logging_requested(); }
    // Cold host-only setters preserve the other axis regardless of CLI order.
    inline void select_backend(bool hardware) noexcept
    {
        bool const async{asynchronous_requested()};
        selected_fault_delivery.store(hardware ? (async?fault_delivery::hardware_async:fault_delivery::hardware_sync) :
            (async?fault_delivery::software_async:fault_delivery::synchronous),::std::memory_order_relaxed);
    }
    inline void select_delivery(bool async) noexcept
    {
        bool const hardware{hardware_requested()};
        selected_fault_delivery.store(hardware ? (async?fault_delivery::hardware_async:fault_delivery::hardware_sync) :
            (async?fault_delivery::software_async:fault_delivery::synchronous),::std::memory_order_relaxed);
    }
}
