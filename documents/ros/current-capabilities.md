# Current explicit ROS permissions and additive ABI

This guide supplements the preserved earlier capability and ABI documents.
The current implementation includes unified events, bounded asynchronous Service
submission, local queue backpressure, and compiled bounded custom Message,
Service and Action adapters. Earlier statements that backpressure or all custom
Service/Action adapters are unavailable do not describe this implementation.
Implementation support and actual acceptance results remain separate: consult
the current source-bound test report for executed outcomes.

## Start with no authority

A newly created Runtime Instance has no ROS authority. WASI Preview 1 permits
only basic stdin, stdout and stderr initially. There is no implicit network,
filesystem, clock, random, process or environment authority. Enabling a backend,
creating a module target, importing an ABI, constructing an SDK owner, installing
a native ROS package, or registering a codec does not grant an operation.

Use the independently scoped `--ros1-single-*`, `--ros1-group-*`,
`--ros1-global-*` and corresponding ROS2 CLI families. A Single target uses the
trusted module name; a Group target uses explicitly configured membership.
An alias cannot select a different Runtime Instance, ModuleGroup or backend.
WASI permissions use their separate `--wasip1-*` families. Selecting ROS1
permissions never selects ROS2 or WASI permissions.

For a Single target, each grant has this form:

```text
--ros2-single-grant MODULE ALIAS OPERATION /absolute/resource TYPE NATIVE_IDENTITY
--ros2-single-set MODULE ALIAS FIELD VALUE
```

Set every operation-specific quota, validator, audit and native QoS field before
execution. Configure the target's explicit Gateway socket/UID, connection
deadline, node/namespace, transport/domain, authentication and denial-audit
limits separately. The Gateway's trusted configuration must select the same
resource, operation, type, native identity and limits. The executable example
policies in `tests/ros_acceptance/run_events.py` and
`tests/ros_custom_types/bounded-v2/run_custom_bounded.py` show complete literal
inventories. Their numerical choices are examples, not permission defaults.

The authoritative field tables and final validation are in
`src/uwvm2/imported/ros/policy/schema.h`; the Gateway checks the same contracts.
Missing, unknown, malformed or conflicting fields cause refusal before an
endpoint is created. Keep-all history is refused because this profile requires
finite native history. ROS2 Action QoS selects eight fields independently for
each of goal-service, cancel-service, result-service, feedback and status.

## Unified event observation

Grant the actual source operation first. Grant a distinct observer using
`event.observe`, the same resource, type `builtin/event-v1` and identity
`builtin:ros-event-envelope-le64-v1`. Its complete explicit field inventory is:

| Field | Meaning |
| --- | --- |
| `event-source-alias` | Exact independently granted source alias |
| `event-kinds` | Nonempty duplicate-free CSV in the order below |
| `max-event-bytes` | Maximum complete record, including the 64-byte header |
| `max-event-queue-depth` | Finite record slots, at least three |
| `max-event-queue-bytes` | At least the record maximum plus two 64-byte control records |
| `max-inflight` | Finite observer/source work limit |
| `max-rate-hz` | Positive finite delivery/admission rate |
| `burst` | Explicit token burst, no greater than queue depth |
| `max-bytes-per-second` | Explicit bandwidth, at least one maximum record |
| `overflow-policy` | `reject`, `drop-oldest`, `drop-newest` or qualified `backpressure` |
| `audit` | Explicit audit mode and its associated finite rate/record/byte fields |

The seven kinds, in CSV order, are `topic-message`, `service-request`,
`service-response`, `action-feedback`, `action-result`, `resource-error`, and
`resource-closed`. An observer may select only kinds derived from its independently
granted source, plus explicitly selected error/closed metadata. Parameter
projections and incoming Action goals retain explicit envelope flags and their
source contracts. A metadata-only watch activates no native data endpoint.

Open both aliases, then call `event_watch(observer, source, correlation)`.
Outgoing Action data uses an already owned nonzero Goal ID. Other watches,
including Action metadata-only watches, use zero. A duplicate data consumer for
the same resource/type/owned Goal/receive kind returns `17`, including attempts
through another alias. Metadata-only observation can coexist with legacy reads.

The additive imports in both backend modules are:

| Import | Parameters |
| --- | --- |
| `event_watch` | observer:u64, source:u64, correlation:u64 |
| `event_poll_v1` | observer:u64, memory:u32, output:u64, capacity:u64, length-slot:u64 |
| `event_ready` | observer:u64, memory:u32, output:u64 |
| `service_submit` | source:u64, observer:u64, memory:u32, input:u64, length:u64, request-ID-slot:u64 |

Every call returns an i32 error. Current ROS1/ROS2 import counts are 38/43; ABI
version remains 1. The original import signatures remain available. See
[the complete ABI](../../sdk/docs/abi-v1.md) and
[unified event contracts](../../sdk/docs/unified-events.md) for exact unaligned
little-endian header, readiness record, flags, correlation and sequence rules.

`event_poll_v1` copies one complete record on the Guest execution thread.
Error `11` indicates idle or explicit quota refusal. A too-small buffer returns
`90`, writes a checked required length when applicable, and retains the record.
An overlapping output/length slot returns `22` before native peek or copying.
Source and observer credits are checked before exactly-once queue commit. A
transport failure after possible copying retires the session; it cannot replay
possibly delivered data. The legacy `event_poll` keeps its documented semantics.

## Asynchronous Service and bounded backpressure

`service_submit` requires an independently granted `service.call` source and an
observer selecting both `service-response` and `resource-error`. The returned
nonzero request ID is owned by that Instance/backend/session and is bounded by
the explicit inflight quota. A successful admission is not a successful native
response. Native completion or a bounded failure later arrives through that
observer with the same ID. Deadline/transport failure after native admission may
leave an external effect; the envelope says so explicitly. It is never retried
as a newly authorized request.

Backpressure is a nonblocking local bounded-queue policy. Explicitly set
`overflow-policy=backpressure`, `max-pending-events=1`, and
`max-pending-event-bytes` sufficient for one maximum complete source record.
These two conditional fields are forbidden under the other overflow policies.
Service request records include their eight-byte ticket; Action pending capacity
must cover the largest applicable Goal/feedback/result envelope. An event
observer's maximum record already includes its 64-byte header.

Full slots or byte credit cause refusal/pending credit according to that finite
policy; `event_ready` exposes four LE64 values: ready count, next record bytes,
available slots, and available bytes. Delivery returns credit. Queue depth and
high-water/refusal/drop counts refer to actual occupied local storage. This does
not promise native DDS network flow control or account for all middleware heap.
Native callbacks never enter Wasm. Close/revoke/Instance death retire associated
queues and owned work independently of data-rate token exhaustion.

## Compiled custom interfaces and SDK layers

The original fixed-v1 profile remains available. The additive bounded-v2 profile
supports complete trusted `.msg`, `.srv` and `.action` definitions with finite
scalar/array/string/sequence/nesting limits. Service and Action components use
their complete generated canonical codecs and normal ticket/Goal envelopes.
They do not reinterpret custom data as builtin String or Fibonacci payloads.

Select both the official native identity and explicit
`adapter-codec-identity`. The latter is a canonical fingerprint, not an MD5/RIHS
substitute. Native adapters verify official component definitions/type metadata
before endpoint activation. Definitions are generated during trusted build
preparation; an untrusted Guest cannot load native types or choose an adapter.
Unsafe, ambiguous, recursive or unsupported definitions receive an explicit
refusal reason. See [the bounded profile](../../sdk/custom-types/BOUNDED_V2.md)
and [official preparation](../../tests/ros_custom_types/bounded-v2/README.md).

C, C++, Rust, JavaScript, AssemblyScript and TinyGo expose the raw ABI, canonical
codecs, owned clients/servers, cooperative executors, timers, parameters, graph,
TF, logging, ROS time, and ROS2 lifecycle where the runtime supports them.
[SDK coverage](../../sdk/docs/client-library-coverage.md) describes the actual
contracts. Construction never adds rights. JavaScript's persistent Wasm relay
uses the same real Runtime/Instance and cannot invent authorization in its host.
Examples progress from imports to typed owners; native rclcpp/rclc/rclpy binary
ABI compatibility and arbitrary dynamic type loading are not claimed.

## Reproduce the current acceptance

Use [the serial acceptance workflow](../../tests/ros_acceptance/README.md) and
[testing guide](testing.md). Generate official types first, freeze complete
SourceB and the separately copied SDK, then compile and execute current products.
Current resumed Linux tests use the explicitly selected 64 GiB, zero-swap cgroup
incarnation. Earlier 48 GiB results retain their original boot, controller and
test scope. Local tests write only under `/tmp`. Each lane records its actual
process-tree budget, deadlines and admitted build parallelism; available cgroup
headroom must cover the selected work before any compiler is started.
Preparation, source models, historical results, documentation and SKIP/BLOCKED
outcomes do not count as current native delivery PASS.

The complete existing interpreter regression matrix has 3,073 required
configurations. Its serial chunks retain independent compiler/actor deadlines
and are aggregated only after every required configuration succeeds. For this
long serial matrix, the trusted parent may explicitly select a temporary QEMU
lease of up to 172,800 seconds; the default is still 7,200 seconds. This controls
only test orchestration duration and grants no Guest rights. The parent removes
its exact entry and checks registry restoration on success, failure or signal.
