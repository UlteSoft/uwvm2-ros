# Trusted control of a typed threaded Wasm cohort

This is the source-level contract for the optional B1 native embedding API.
The implementation and qualification sources have not yet been compiled or run.
It does not establish completed threaded CLI debugging, source stepping, locals,
unwinding, checkpoints or ROS2 callback debugging. The ordinary threaded `-Rdbg`
and checkpoint admission rules remain unchanged.

The API belongs to the trusted native Runtime controller. Applications in Wasm,
including Java, C#, Python, JavaScript and ROS compatibility libraries, receive
no debug-control import from this feature. Pausing an application does not grant
its controller a new ROS operation, native pointer or memory-inspection API.

## Configure before starting the real profile

Include `uwvm2/runtime/lib/uwvm_runtime_threaded_profile_api.h` in a supported
Linux native embedding build. The declarations require the actual threaded
Runtime feature configuration; inspect `UWVM2_HAS_THREADED_PROFILE_HOST_API`.
Use namespace `uwvm2::runtime::lib` for the functions and
`uwvm2::runtime::wasm_threads` for the policy and result types.

While the Runtime is quiescent, explicitly select
`threaded_debug_observation::readonly_metadata`, then configure control:

```cpp
namespace api = uwvm2::runtime::lib;
namespace wt = uwvm2::runtime::wasm_threads;

bool configure_debug_control(std::size_t admitted_control_bytes)
{
    if (!api::runtime_threaded_profile_debug_observation_configure_host_api(
            wt::threaded_debug_observation::readonly_metadata))
        return false;
    return api::runtime_threaded_profile_debug_control_configure_host_api(
        {wt::threaded_debug_control::cooperative_all_stop,
         admitted_control_bytes});
}
```

Check both results. Configuration during a live profile fails. Control cannot
implicitly enable observation, and observation cannot be removed while control
is selected. Both policies default to `none`; disabled control requires a zero
control quota. Enabled control requires a positive finite quota.

The quota accounts for the group's independent pause domain, actual vector
capacity and bounded roster scratch. These bytes also consume the existing
`threaded_profile_limits::max_live_storage_bytes`. Fixed object sizes are
charged by the existing profile accounting. Requested payload is distinct from
allocator overhead, process RSS, native stacks and the actual Linux cgroup limit.
Choose a budget using the actual target build and profile, then measure it.

The ordinary source-owned CLI loader, validator, actual profile limits and native
start API still establish the real application. Configuring debugging does not
replace any of those admissions.

## Request and release a stable stopped cohort

After the real typed profile starts, obtain
`runtime_threaded_profile_debug_control_session_host_api()` from an independent
native controller. The session weakly identifies that actual group and source
generation. An empty, old or expired session cannot address a replacement group.

`threaded_debug_all_stop` is default-constructible, noncopyable and nonmovable.
Only a successful `session.stop_until(deadline, guard)` fills it. A nonempty
issued guard must be explicitly reset before reuse, including after it becomes
stale. Use, reset and destruction of an issued guard must occur on its issuing
native thread; violating that Host invariant terminates the process.

Prepare bounded caller-owned row storage before requesting the stop. For example,
the following source fragment accepts at most eight workers and handles every
failure by releasing the guard:

```cpp
auto session = api::runtime_threaded_profile_debug_control_session_host_api();
std::array<wt::debug_stopped_worker, 8> rows{};
wt::threaded_debug_all_stop guard;
auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
auto status = session.stop_until(deadline, guard);
if (status == wt::debug_stop_status::stopped) {
    std::size_t needed{};
    auto observed = guard.read(rows, needed);
    if (observed == wt::debug_stop_status::stopped) {
        // Consume only rows[0..needed). These are detached diagnostic values.
    }
    // insufficient_capacity supplies needed; obtain a new bounded buffer and
    // a new stopped episode after releasing this one, rather than truncating.
    guard.reset();
}
```

This fragment also needs the standard `<array>` and `<chrono>` headers. It is an
uncompiled usage example. A timeout or busy result supplies no stopped proof.
`stop_until` rejects an infinite deadline and rechecks elapsed time before issuing
a guard. It is not a hard real-time bound for the whole call: synchronous
wait-shard wakes, mutex contention and scheduling can delay return. The embedding
needs an owned external process deadline and real stop/join recovery.

| Status | Meaning |
| --- | --- |
| `stopped` | The actual stable cohort was checked for this episode; `read` must still check current validity. |
| `busy` | Roster, compiler/Host/spawn activity, a prior resume ACK, or an issued output prevents this request. |
| `timeout` | No stopped authority was issued before the admitted cooperative deadline. |
| `stale` | The issued group, generation or stopped episode is no longer valid. |
| `unavailable` | Control was not enabled, or the session/guard is empty. |
| `insufficient_capacity` | `read` needs a larger bounded output span; this is not a truncated successful observation. |

## Interpret each worker's actual evidence

Rows identify the real participant, slot incarnation, native TID, Source
generation, validation epoch and backend. They do not expose a native stack
address or authorize dereferencing arbitrary process memory.

| Site | Available position |
| --- | --- |
| `before_instruction` | Actual interpreter module, function, decoded instruction and Wasm byte offset. |
| `function_boundary` | Actual LLVM typed poll's module/function; exact instruction and byte offset are unavailable. |
| `atomic_wait` | An actual linked wait is suspended with its shard unlocked; no saved frame or exact logical PC is supplied. |
| `native_boundary` | An actual native worker boundary; no invented function position. |
| `host_boundary` | Actual import admission or a fully returned Host boundary; this does not prove that the Host operation ran or completed. |

Unavailable numeric fields use the declared `SIZE_MAX` or `UINT64_MAX` sentinel.
Synthetic native provenance and disassembly are not managed-language source lines
or locals. CPython C DWARF, Mono runtime symbols and QuickJS C symbols remain
distinct from Python, C# and JavaScript frame debugging.

## Resume, stop and retirement

`guard.reset()` resumes only its own actual serial. The group waits for physical
returns from the prior pause polls before admitting another stopped episode.
Notifications alone do not count as resume acknowledgements. B1 refuses changing
rosters, compiler preparation/cleanup and native body-returned-but-unjoined slots.

Ordinary pause preserves the existing atomic wait's FIFO position, original
deadline and notification/cancellation arbitration. A pause is not an additional
Guest wait result. Workers park outside Host provider, ROS backend/client,
allocator/growth and wait-shard critical sections.

Terminal stop invalidates observation/control, publishes the genuine stop token
and closes the pause domain. Real pthread/TLS/compiler/code retirement still
requires physical join. A stale guard holds weak group/admission identity and
does not retain a provider-generation execution lease across a subsequent public
stop/drain. Each operation briefly locks its actual owners and releases them
before returning. Keep the same-thread rule even when an issued guard expires.

ROS gateways and external ROS peers continue running during this local cohort
pause. The existing signal-control pipe and cancellation descriptor retain their
own lifetime; pausing never consumes them or manufactures a canceled ROS call.
Grant checks, delivered payload validation, request/goal ownership and audit logs
remain independent. Stopping local Wasm workers does not cancel remote effects.

## Qualification required before use

The authored embedding/WAT sources exercise real two-worker identity, atomic
wait plus a looping worker, repeated resume/stop episodes, terminal join with a
held guard, stale/empty behavior, public drain and concurrent resume/stop. They
must run against the actual final Runtime; they are not model stand-ins or
executed results. A concurrent race case does not prove that a narrow scheduling
window was hit. The void public stop/drain API can refuse other management
contention, so a mere return is not a general completed-drain receipt.

Run all compilation, WAT generation and native/Guest/DBG/QEMU/VTune work only in
the authorized SSH Linux existing 64 GiB, zero-swap cgroup. Retain exact sources,
toolchain, flags, binaries, Wasm, actual membership/boot/inode, raw failures,
external deadlines, RSS/events and physical descendant cleanup. Test the actual
ROS2 topic/service/Action callbacks and independent native completion separately.

Measure default `none`, readonly observation and enabled control independently
with VTune hardware Hotspots and Microarchitecture Exploration. Default `none`
avoids dynamic pause allocation and debug locks/counters, but added branches and
fixed object fields are still costs. Enabled idle control skips linear position
mapping; that source change is not an observed performance improvement.
