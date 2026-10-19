# ROS diagnostics in the DAP debugger

The adapter exposes the production Runtime's existing read-only ROS Host
snapshot through `uwvm/rosState` and three Variables scopes. It uses the same
authenticated broker and cooperative Runtime stop as other debugger requests.
It creates no ROS context, session, capability, queue, publisher or subscriber.
It cannot open, revoke, grant, refresh or consume a capability or event.

At a cooperative frame-zero stop the Variables view offers:

- **ROS capabilities (process-owned)**: stored capability metadata and counters.
- **ROS instances (backend sessions)**: actual Host instance/generation labels
  and separate ROS1/ROS2 connection states, using `info ros sessions`.
- **ROS event queues (metadata only)**: queue depths, pending/reserved bytes,
  retirement, drop/backpressure and completion counters, using `info ros queues`.

These are process-owned observations, independent of the selected DAP frame's
numeric module ordinal. An instance label is not a transferable handle. Empty
default-denied capability/queue tables remain empty. No synthetic row is added
to make an unavailable ROS backend appear enabled. ROS-disabled builds and a
contended Host lock produce explicit unavailable errors. Queue inspection never
polls an event or reads its envelope/payload, parameter value, certificate,
security path, session credential or native pointer.

Variables requests accept `start=0..4096` and `count=1..8`. Omitted/zero count
returns one bounded page of at most eight rows. The structured request below
provides cardinality and continuation metadata when a client needs further
pages; Variables does not add a fake capability to display pagination state.

## Structured read-only request

For example an IDE can send this custom DAP request after attaching:

```json
{"command":"uwvm/rosState","arguments":{"selection":"events","backend":"ros2","instance":3,"start":0,"count":8}}
```

The instance number above is illustrative, never a predefined principal. Obtain
actual identities from the current `instances`/`sessions` result first. Only
the following fields are accepted; unknown fields fail closed:

| Field | Meaning |
| --- | --- |
| `selection` | `capabilities` (default), `sessions`, `queues`, `services`, `actions`, `parameters`, `tf`, `graph`, `stats`, `trace`; `instances` aliases sessions and `events` aliases queues. |
| `backend` | `all` (default), `ros1` or `ros2`; no backend fallback. |
| `instance` | Exact actual nonzero unsigned 64-bit Host instance. |
| `module`, `alias` | Exact nonempty printable ASCII token, at most255 bytes; no inferred module lookup. |
| `start` | State row offset0..4096, default0; forbidden for trace. |
| `count` | State1..8 (default8), trace1..16 (default16). |
| `after` | Trace sequence cursor, unsigned64. |
| `error` | Exact trace errno filter, unsigned32. |
| `function`, `operation` | Exact trace import/operation token, at most64 bytes. |
| `ifStop` | Optional exact nonzero current Runtime stop assertion; never a request to pause. |

The result contains `scope`, `stopId`, canonical `selection`,
`requestedSelection`, `rows`, `start`, `total`, `next`, `more`, `gap`,
`overwritten`, `dropped_busy`, and explicit read-only scope flags. Text fields
are objects with a safe `text` display, exact `bytesBase64` and `truncated`.
Invalid UTF-8 is represented in the display with byte escapes; base64 preserves
the exact original bytes. A redacted resource is null with
`resourceRedacted=true`. Truncated metadata is never silently complete.

`services`, `actions`, `parameters`, `tf` and `graph` select locally authorized
capability families. They do not perform ROS graph discovery or native status
RPC. `trace` returns the bounded non-consuming Host-call metadata ring, whose
sequence cursor is different from a state row offset. Overflow/gap facts remain
visible; this is not an exhaustive network audit.

Every query refreshes actual controller status, requires a complete identified
cooperative participant cohort, sends the same stop ID in `if-stop`, parses
the complete bounded response, then refreshes status again. A running Runtime,
native-instruction stop, changed participant/generation/cohort, stale stop,
malformed/truncated packet or transport error refuses the result. Cached
Variables references are cleared on resume/stop replacement. Reaching the same
PC again never revives an old scope.

## Actual integration and architecture scope

`tests/ros_debugger/integration_runtime.py` now loads this exact adapter from
the admitted complete source manifest and executes its production query method
against the actual credential-checked inherited console. The existing ROS1,
ROS2 and dual jobs check repeated unchanged projections, instance/event alias
equivalence, running/stale/native-stop refusal, unchanged native Gateway log
windows and the independent Guest/native payload/quiet results. This path
proves projection over that real console; it does not claim the separate IDE
or secure-server broker transport executed.

The query protocol has no native-endian binary fields or native-address reads.
It is usable by each actually admitted target/QEMU Runtime in the existing
eight-architecture debugger lane, including LoongArch64. The host QEMU image,
foreign Runtime image, target loader, actual kernel stopped birth, inherited
control credentials and LLVM-full engine remain independently mandatory. A
source/parser model, interpreter matrix or native x86 result cannot substitute
for actual foreign JIT/debugger execution.

`test_dap_ros_state.py` supplies an unexecuted source-bound DATA factory for
packet and stop-fencing policies. Its mathematical broker is explicitly not
ROS or Runtime evidence. Root must execute both policy and real integration
jobs under the selected64GiB/swap-zero owner after the complete source freeze.

## Trusted replacement and ROS retirement

The Guest ROS import registry contains no Runtime update, debugger or policy
configuration entry point. Guest stdout/stderr and ROS log messages are data;
the authenticated control console must remain a separate channel. The actual
Runtime integration Guest prints `pause`, `quit`, `replace` and `wasm-script`
as WASI stdout and requires the real controller to remain running. That source
assertion becomes transport evidence only when the real job executes.

A genuine successful function replacement permanently retires ROS authority
for the affected compiled module's current Runtime incarnation. The Runtime
sets its module-local flag before publishing the new entry points. Its primary
ROS caller identity then fails closed. The observed identity remains available
for denial diagnostics and grants no authority. Failed preparation/commit does
not retire the module's ROS authority.

After leaving the stopped-domain publication mutex, while the Guest remains
paused, the native post-commit hook waits for earlier admitted Host leases,
invalidates handles, closes both backend connections and discards local queued,
pending and reserved event data. It clears watch request IDs, retains inert
diagnostic registrations/counters and issues no Gateway RPC. Other modules are
unaffected. Existing native ROS effects are not undone. ROS reuse requires a
fresh trusted launch with explicit CLI grants; no re-enable debugger request
or Guest import is supplied.

DAP references are invalidated **before** sending `replace` or `wasm-script`,
even for malformed/rejected requests and transport errors. Successful
`replace` synchronously reads complete authoritative status before publishing
a new stop. Failed refresh leaves all old references retired. A script
transcript cannot serve as a new stop; refresh status/scopes explicitly. The
same PC or the same numeric stop label cannot resurrect a reference from a
different generation/cohort. These adapter rules conservatively retire copied
references without asserting whether an uncertain transport committed an update.

[`README.management.md`](../../tests/ros_debugger/README.management.md) describes
the separate actual manager/Host components, injected DAP/raw-registry tests
and the remaining genuine Runtime replacement integration obligation. The SDK
boundary applies identically to every supported language; see
[`update-and-debug-boundary.md`](../../sdk/docs/update-and-debug-boundary.md).
