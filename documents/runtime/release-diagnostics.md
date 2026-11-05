# RELEASE diagnostics

Trusted startup permanently seals full debugging in RELEASE, the default robot
profile. `--mode debug` and `--runtime-debug` request a bounded final diagnostic
record; they do not install a controller. All cold debugger, mutation, script,
checkpoint and source-replacement APIs still enforce the actual profile gate.
DEV requires its own explicit startup grant and authenticated debugger endpoint.

On Linux LLVM full, `--diagnostic-control-fd <fd>` grants an independent observer
on an inherited AF_UNIX SOCK_SEQPACKET socket. The launcher should enable
SO_PASSCRED on the child endpoint before exec/sending. The VM checks SO_PEERCRED,
pins the peer with a pidfd and authenticates PID/UID/GID on every packet. Passing
the peer socket does not delegate this grant. Passed descriptors, oversized or
truncated packets and changed credentials close observation. The socket and its
duplicates are excluded from guest descriptor admission. Update and diagnostic
endpoints are sealed separately and can coexist.

Commands are exact packet contents without newline, NUL or appended arguments:

| Command | Response |
|---|---|
| `status`, `modules`, `health`, `crash`, `update` | The same bounded schema-1 JSON metadata record |
| `help` | The accepted command list |
| `close` | Disconnect observation only |

Every other command is denied. In particular, no status wrapper, DAP command,
REPL expression, script, path, memory address, variable write, pause, restart,
rebuild or update permit is interpreted by this dispatcher. Observation does not
grant WASI/ROS interfaces or business-call authority.

The record includes runtime version/generation, counters, up to 16 initial module
SHA-256/size pairs and up to 16 innermost validated managed-fault frames (numeric
module/function/generation only). Names, guest memory/locals, arguments,
environment, paths, native addresses, nonces, contract details and credentials
are absent. `frame_offsets_available=false` explicitly declines offset evidence.
Initial module identities remain labeled initial; installed image, authenticated
commit epoch and admission state are separately observed update facts.

The crash record is collected after the genuine activation/source/generation
checks and managed execution stop. It does not fabricate a stack for arbitrary
host crashes, native corruption or unavailable activation evidence. A contained
fault cannot be resumed through diagnostics. Unmanaged fatal termination can
prevent delivery of a final JSON record; bounded trap output remains best effort.

Metadata copies and event publication use try-locks and fixed arrays; guest work
never waits for the observer. `status=unavailable` means no snapshot could be
obtained. `collection_lost=true`, `status=incomplete` and
`current_update_facts_available=false` mean that a producer could not record an
event: retained fields are historical, not proof of the current state. This loss
flag remains set for the process lifetime. `update_in_progress=true` separately
marks an ongoing management transition: update fields are last observed facts,
and `current_update_facts_available=false` until completion. This also prevents a
previous ACTIVE record from describing an in-flight drain as currently active.
No managed session likewise means no current update facts. Module/stack
truncation is explicit.
Root counters are sampled independently and are not an atomic accounting proof.

Reports are below 8 KiB with the current fixed schema. Sends are nonblocking;
a slow reader closes observation rather than delaying guest execution. The
observer checks completion at most every 50 ms, and normal completion attempts
one final nonblocking report. No diagnosis request pauses execution or enters
the update fence. Disconnecting diagnostics does not close the update channel,
rollback a commit, resume a fault, or stop the guest. Supervisor watchdogs and
device safety remain independent processes.

See [production updates](production-update.md) for the separately signed,
nonce-bound update protocol and its explicit supported execution boundary.
