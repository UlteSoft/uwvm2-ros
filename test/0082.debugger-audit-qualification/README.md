# Debug audit component qualification

This finite target implements a metadata-only native producer/consumer contract.
It does not enable instrumentation in a VM and does not claim complete Wasm,
WASIp1, ROS, debugger, IDE or performance qualification.

`src/uwvm2/runtime/debug_audit/event.h` distinguishes attempted calls, entered
frames, returned/trapped/exceptional exits and tail replacement. Memory events
distinguish attempted, committed, no-write, denied and failed operations. A
before-instruction callback cannot manufacture a committed event. CAS mismatch
and zero-length bulk operations are distinct from writes. Each stream retains
the actual instance, module digest, policy/runtime generations and native variant;
local memory indices and actual backing identities are separate. No payload or
native address field exists.

The runtime must supply an authenticated native authorization, bind producers
to live principals and attest the selected code variant's actual coverage.
The contract refuses stale owners, a second instance of identical Wasm without
an explicit cross-instance grant, missing observation permissions and unsupported
coverage. Values, expression evaluation, mutation and code replacement remain
separate capabilities; none implies observation authority. These component tests
exercise authorization decisions, not a hostile guest's complete runtime boundary.

`stream.h` is a fixed-capacity, single-producer/single-consumer queue. Its release
and acquire operations publish complete records and retain each slot until the
sink acknowledges it. Sequence numbers are per stream; they do not establish
cross-thread total order or the linearization order of shared atomic memory.
The operation/causal IDs must be bound by real producers. No allocation, clock,
I/O, blocking wait or mutex occurs in `append`. Targets without lock-free 64-bit
atomics are explicitly unsupported by this transport pending another backend.

With `drop_new`, a full queue retains old unread events, counts discarded new
ones, preserves sequence gaps and marks the interval incomplete. With
`fail_recording`, the first loss permanently fails that recording and every
subsequent append returns failure. The runtime integrating that mode must act
on failure at a safe boundary; this queue neither terminates a guest nor promises
lossless execution by itself. Sink errors always latch failure. A producer seals
at a real rendezvous; only then can the consumer read its counters. There is no
reset that could erase failure evidence or reuse sequence labels.

`transport_complete` means only that all events submitted to this stream were
accepted by the sink and the sealed stream was drained without error. It is not
an assertion that every runtime operation was submitted, that a sink is durable
without its own acknowledgement, or that all participants were covered. Attaching,
changing policy, replacement and revocation require a runtime-owned safe-point
handshake and retention of old variants until all active frames retire.

Run the whole folder in an admitted Linux source tree under the canonical lease:

```sh
xmake f -m release
xmake
xmake test
```

The 14 named-in-source case groups exercise authority separation, range overflow,
CAS/no-write semantics, phase rejection, explicit pressure policies, sink failure,
stale acknowledgement, revocation and 100,000 genuinely concurrent native events.
Public Xmake execution and direct compiler execution have separate evidence.
All required product modes remain open: unchanged-Wasm ordinary/idle/unhit
breakpoint/call/write/combined audit, active hit/step, code replacement, official
Wasmtime+LLDB/GDB comparisons and native PMU profiles. Source presence cannot
fill any of those results.
