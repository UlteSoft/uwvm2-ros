// SPDX-License-Identifier: Apache-2.0
// Copyright 2026 UWVM contributors.
// Target-specific original-rclcpp platform transport. This is not a POSIX
// signal implementation or a source of ROS/Host management permissions.
#ifndef UWVM_RMW_WASI_SIGNAL_HPP
#define UWVM_RMW_WASI_SIGNAL_HPP
#include <cstdint>
namespace uwvm_rmw::wasi_signal {
inline constexpr std::uint32_t profile_version=1;
inline constexpr std::uint32_t interrupt_mask=1;
inline constexpr std::uint32_t terminate_mask=2;
inline constexpr std::uint64_t maximum_slice_ns=100000000;
inline constexpr std::uint64_t maximum_frame_count=1048576;
inline constexpr unsigned frame_bytes=32;
using handler=void(*)(int);
using failure_notification=void(*)() noexcept;

// The Root-owned pipe read end must already be installed at this exact fd by
// the actual Runtime owner. It is neither stdin nor a pathname/alias to open.
// Host retains the only write end. Actual FD_READ|POLL_FD_READWRITE and genuine
// WASI monotonic-clock permissions remain independently enforced at imports.
// Root must preserve O_NONBLOCK/OFD identity and the map entry through physical
// Group retirement. Cookie is a source association, not an authority secret.
struct profile {
 std::uint32_t version{};
 std::uint32_t read_fd{};
 std::uint64_t source_cookie{};
 std::uint64_t maximum_frames{};
 std::uint64_t poll_slice_ns{};
};
enum class phase:std::uint32_t { cold,prepared,starting,active,stopping,retired,poisoned };
struct diagnostics {
 phase state{};
 std::uint32_t first_error{},last_error{},last_wasi_errno{},last_native_error{};
 std::uint64_t polls{},reads{},accepted_frames{},interrupts{},terminations{};
 std::uint64_t last_sequence{},last_userdata{};
 std::uint32_t partial_bytes{},active_callbacks{};
 bool start_attempted{},thread_created{},guest_join_completed{},failure_notified{};
};

// Exactly one immutable source-lifetime profile and one activation are admitted.
// Allocations and a genuine non-consuming FD+timer poll precede publication.
// Missing feature/profile/rights is a real error; no fd is opened or granted.
// Every output remains untouched on failure. No reset or process-reuse API.
std::uint32_t prepare(profile const&) noexcept;

// Internal rclcpp-port registration, not a replacement for C signal()/raise().
// SIGINT/SIGTERM alone are supported. Registration is refused while a receiver
// could call the handler. Original private handler storage is fully set before
// activate; after stop_and_join success it can safely be restored.
std::uint32_t exchange_handler(int signal,handler desired,handler*previous) noexcept;

// The selected nonnull handlers and failure wake are snapshotted before the real
// receiver pthread can dispatch. Failure wake only wakes the original deferred
// worker; it never fabricates signal_received or invokes Host configuration.
std::uint32_t activate(std::uint32_t mask,failure_notification) noexcept;

// Stop admission, join the genuine receiver, and finish every actual callback.
// No callbacks occur after success. Self-join is refused. A failed join poisons
// the owner and retains all storage; it is never detached or reported retired.
// Guest join is not proof of Runtime native/TLS/Source physical retirement.
std::uint32_t stop_and_join() noexcept;

// Failure is sticky for this source lifetime, independent from cleanup facts.
// A terminal input/protocol failure wakes the original deferred worker exactly
// once; the port treats it as platform failure, never SIGINT/SIGTERM success.
// True only on the genuine receiver entry's lexical private TLS owner. Used
// before original install_mutex acquisition; no caller can supply/set this role.
bool in_receiver_thread() noexcept;
std::uint32_t terminal_error() noexcept;
std::uint32_t observe(diagnostics*) noexcept;
}
#endif
