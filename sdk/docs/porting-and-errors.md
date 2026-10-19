# Memory, lifetime and error handling

## Treat all handles and offsets as 64-bit values

Every ROS import uses `i64` offsets, byte lengths, capacities and handles, even
when a guest is compiled for wasm32. Memory index is `i32`; the selected linear
memory must exist. Handles and returned lengths use eight **little-endian**
bytes. Unaligned addresses are supported. The host copies data during the
call; it never retains a guest pointer across a later call or memory growth.

In JavaScript, pass i64 arguments as BigInt and decode with a little-endian
DataView operation. Never convert an opaque handle to Number. A Number cannot
represent all 64-bit integer values. The Rust/TinyGo/AssemblyScript APIs keep
fixed-width integers; any cast to a memory-zero pointer must first satisfy the
binding's documented address-width and memory-range constraints.

After memory growth, reacquire a JavaScript view of `memory.buffer`. Do not keep
a detached view or assume that a newly allocated managed string is UTF-8.
Managed-language wrappers must keep temporary canonical byte arrays live for
the entire synchronous import call. Raw multi-memory callers are responsible
for selecting the correct memory and supplying its offsets.

## Returned length is conditional

Initialize an eight-byte result-length slot to `UINT64_MAX` before poll/RPC.
On success, the Runtime writes the response length. Error 90 has two meanings:
an early byte/quota refusal may leave the slot untouched, while an admitted
response that exceeds your output capacity writes the required length first.
Only read a 90 result as a length if your sentinel was overwritten. Preserve
the caller's previous public output/length on every other refusal.

An undersized receive buffer can be discovered **after** a Gateway event was
dequeued. A service or Action can also have an admitted external effect before
its response is refused or a timeout is reported. Do not blindly resize and
repeat the call. Supply an explicit bounded capacity consistent with your
grant and design retry semantics at the application level.

The IPC payload ceiling is 4194304 bytes. Request data has a conservative hard
ceiling of 4193786 bytes after bounded operation/alias envelope overhead; many
typed adapters and grants have stricter limits. These ceilings are not a
default permission or a recommended automatic allocation size.

## Close and revoke do not restore authority

Close retires a handle generation before the last-handle Gateway cleanup call.
Consequently, a later disconnected/timeout cleanup error does not mean the
handle is still usable. An owner should relinquish its local ownership after a
close attempt, report the cleanup error, and avoid replaying a stale handle.
Destructors cannot reliably report such failures; call close explicitly if the
application needs that status.

Revoke applies to the entire alias in the current Instance/backend, not only
one local wrapper. It invalidates open siblings before Gateway notification;
subsequent old-handle calls return bad-handle 9, and reopening the revoked alias
returns revoked 125. A local Node/resource factory cannot undo revocation or
grant permission to another Instance/backend.

## Error codes

| Code | Application meaning | Suggested handling |
| --- | --- | --- |
| 0 | Success | Decode only the returned canonical bytes. |
| 2 | Missing resource | Distinguish an absent parameter/resource from permission denial. |
| 9 | Invalid, stale, foreign or wrong-backend handle | Retire local ownership; do not fabricate a replacement. |
| 11 | Empty queue or explicit quota refusal | Bound your work; do not busy-spin without a scheduling policy. |
| 13 | Permission/type/ownership refusal | Fix the explicit trusted policy or the selected alias. |
| 17 | Explicit contract/state conflict | Correct paired contracts; do not weaken a validator. |
| 22 | Invalid memory, argument or canonical wire | Check widths, lengths, UTF-8, tags and trailing bytes. |
| 90 | Byte/capacity quota | Apply the conditional length contract above. |
| 95 | Unsupported backend, operation or native type | Choose an implemented adapter; no fallback backend is used. |
| 107 | Gateway disconnected | Reconnect only as permitted by the application and policy. |
| 110 | Deadline expired | An admitted external effect may already exist. |
| 125 | Alias revoked | Trusted reconfiguration is required; opening it again cannot restore it. |

## Default-deny host setup

The guest SDK starts with no ROS resources. It does not generate a wildcard
grant, a master URI, DDS domain, QoS default, keystore, topic name or environment
inheritance policy. Supply every connection and grant field through the trusted
host command line and the Gateway configuration.

Current ROS-enabled Runtime configuration requires effective WASIp1 environment
isolation, which is now the WASIp1 default. Explicit noinherit options, such as
`--wasip1-global-noinherit-system-environment` or the relevant
Single/Group equivalent, can select that policy explicitly. Native
directory/socket preopens and the incompatible
plugin routes are rejected by the ROS isolation checks. The separate task to
validate the current **WASIp1 stdio-only defaults** in the real Linux native and
Guest matrix remains pending; installing this SDK does
not complete that validation. The minimal examples add no file, socket,
clock or threading imports implicitly.
