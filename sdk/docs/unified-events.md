# Unified event v1 and bounded service admission

The additive imports are available in both `uwvm_ros1_v1` and
`uwvm_ros2_v1`. Their current inventories contain **38** and **43** functions.
ABI version remains `1`. Every original import name, width, native canonical body
and legacy receive behavior remains supported. Registration and SDK construction
create no authority: the default policy still grants no ROS rights.

## Explicit observation authority

Open the real source alias and a distinct `event.observe` alias before calling
`event_watch(observer, source, correlation)`. The observer's trusted configuration
must name `event-source-alias` exactly and use the same resource. Its type and
identity are `builtin/event-v1` and `builtin:ros-event-envelope-le64-v1`.
`event-kinds` is explicit, duplicate-free CSV in this enum order:

1. `topic-message`: topic subscription or explicitly flagged parameter projection.
2. `service-request`: service request, or explicitly flagged incoming Action goal.
3. `service-response`: response to an admitted asynchronous service request.
4. `action-feedback`: feedback for the watch's already owned goal.
5. `action-result`: terminal result for that already owned goal.
6. `resource-error`: body-free numeric error, never a reflected native exception.
7. `resource-closed`: body-free retirement of the exact watched source.

Only kinds derived from the independently granted source plus explicitly selected
error/closed metadata are allowed. An otherwise non-observable publisher, query,
clock or lifecycle owner can have a metadata-only observer. No topic permission
becomes service permission; incoming Action and parameter flags retain their
actual source type and operation. A metadata-only watch activates no native
publisher, subscription or RPC. Only watches selecting outgoing Action **data**
use nonzero `correlation`; Native validates exact goal ownership without consuming
its result. Metadata-only watches, including Action receive owners, require zero.
A data watch on a source already using a legacy consumer, or a duplicate watch,
returns `17`. A metadata-only watch may coexist with the existing legacy queue;
closing that observer never closes or consumes the independently owned queue.
Outgoing Action data consumers are exclusive by exact resource, native type,
owned goal ID and receive kind, across independently granted aliases. A second
data watch or a legacy receive for that same consumer returns17 before Source
quota debit or native queue consumption. Different goals and feedback/result
kinds remain separate consumers. Detaching the payload watch releases this
claim; correlation0 metadata does not acquire it.

All rate/burst/byte/audit and `max-inflight` fields remain explicit. Observer
limits are `max-event-bytes` (including header), `max-event-queue-depth` (at least
three), `max-event-queue-bytes` (at least record maximum plus128), and an explicit
`overflow-policy`. Source limits and native QoS remain independent. A practical
String lesson uses record256, depth8, queue2048, rate100, burst8, bandwidth8192;
choose separate Source String bounds and native QoS deliberately. These numbers
are examples, never defaults. Granting backpressure additionally requires exactly
`max-pending-events=1` and `max-pending-event-bytes` large enough for one complete
maximum record. The extra fields are forbidden under other overflow policies.
Native watch admission consumes the observer's explicit rate/burst before stream
creation. Native source creation has a separate creation counter derived from
the same explicit Source rate/burst, so a burst1 subscription retains its one
callback-data token. Close, revoke and detach are never blocked by these quotas.

## Complete unaligned wire record

Every integer is little endian. No native C/C++/Rust struct is a wire object.
Every memory index is i32; handles, offsets, lengths and IDs remain i64 on both
wasm32 and memory64. Memory-zero convenience wrappers retain their documented
pointer target restrictions; raw imports allow the explicitly selected memory.

| Offset | Width | Field |
| --- | --- | --- |
|0|4|Magic `UWRE` (`0x45525755`)|
|4|2|Event version1|
|6|2|Kind1 through7|
|8|1|ROS backend1 or2|
|9|1|Flags|
|10|2|Reserved zero|
|12|4|Unsigned numeric error; zero for data|
|16|8|Exact watched source handle|
|24|8|Owned request/goal correlation, or zero|
|32|8|Monotonic delivery sequence, starting at1|
|40|8|Trusted Runtime Instance integer, never a pointer|
|48|8|Original authenticated native session|
|56|8|Body byte length|
|64|variable|Unchanged canonical Source body|

Flags are source-retired1, admitted-external-effects-may-remain2,
incoming-Action4, parameter-projection8. Unknown combinations are refused.
Error/closed bodies are empty. Closed requires flag1. Topic correlation is zero;
request/response/feedback/result correlations are nonzero. Request, feedback and
result bodies retain the original owned ID8 and must agree with the header.
Service response bodies retain the selected service response codec; their header
ID belongs only to a successful `service_submit` acknowledgement. UTF-8 and
scalar/array bounds are always validated against the actual Source grant.

## Polling, credit and cleanup

`event_poll_v1(observer,memory,output,capacity,lengthSlot)` copies at most one
complete record. `11` means idle or explicit quota refusal. On capacity90 a
written eight-byte required length is diagnostic and the record is retained.
An admission/sequence90 can leave the slot untouched: use a sentinel and read
only a slot actually written. The original `event_poll` keeps its historical
potentially destructive-too-small behavior; do not infer new semantics for it.
The output range with nonzero capacity must be disjoint from the eight-byte
length slot in the selected memory. Overlap returns22 after memory bounds checks
and before native peek, payload copy or length-slot writes. Capacity0 has no
payload range and remains useful for a retained required-size query.

The Worker uses a private bounded peek/commit transaction. The existing native
slot stays pinned while both Source and observer byte/rate credits are checked
atomically under their live Instance/backend leases. A refused credit check
changes neither debit, consumes no completion and creates no extra payload queue.
Pinned records cannot be evicted or replaced by callbacks. After successful
controlled Guest copy, an exact sequence commit retires the native record once.
Copy/commit transport failure closes that session and retires its watches;
replaying possibly copied data is forbidden. No native callback enters Wasm.
The Gateway previews both Native delivery budgets before returning a peek and
debits them only at exact commit. An asynchronous service response consumes
Source response bytes; outgoing Action reads consume their independent receive
rate/body bytes. Incoming callback data was already charged to its Source queue
and is not charged a second time by Native commit. Worker delivery still checks
its own independent Instance/source/observer budgets.

`event_ready(observer,memory,output32)` returns four u64 counters: ready record
count, next record bytes, admission slots, admission bytes. It is a snapshot,
not a reservation. Callers still handle admission11. It grants no statistics,
clock or additional resource rights. Trusted native fixtures have separate
allocation-free queue snapshots for actual occupancy, high-water, pending,
reservation, enqueue/dequeue/drop/reject/backpressure and closure counters.

Drop-oldest/drop-newest/reject apply to eligible unpinned data. Backpressure
retains exactly one separately budgeted pending record and promotes it when a
slot becomes available. A second pending admission returns11 without blocking a
native executor. This is bounded local admission/credit; it does not claim that
DDS/TCPROS publishers are automatically flow-controlled or that reliable QoS
prevents application-level refusal. Outgoing Action feedback uses the same
pending/promote contract. Its receiver must match the sender-created immutable
queue and ROS2 endpoint QoS, rather than silently replacing that contract.

Source close/revoke clears sensitive queued/pending data and owned request
bookkeeping. A separately live observer can still receive only explicitly
permitted error/closed metadata, with the original source/Instance/session
binding. Observer close/revoke discards even metadata and detaches its native
watch; it does not revoke the independent source grant. Session death also
retires watches. A fresh authenticated connection never revives an old watch.
Metadata without a permitted mask bit is not synthesized. Callers which close a
source must retire their own local Future/deferred bookkeeping even when they
chose no closed-event permission. Error metadata is bounded/coalesced; audit
records remain the separate explicit audit authority.
Returned Native source-operation failures and Worker-local postvalidation errors
can create an explicitly selected uncorrelated `resource-error`. Ordinary
readiness/credit11 is not a fault. A Native invocation attempted before its
returned failure conservatively sets flag2; a pre-invocation refusal has flag0.
The bounded error slot coalesces unconsumed failures and cannot replace a pinned
front. It does not promise a notification for every middleware exception or
every malformed forged handle: errors without an existing exact live watch,
unselected errors, and observer-configuration failures still return only their
ordinary import status. Native connection death instead retires the stream.

## Real asynchronous service submission

`service_submit(source,observer,memory,input,length,requestIdSlot)` validates the
trusted Source byte limit before copying input. It requires explicit observation
of **both** service-response and resource-error, otherwise returns13 before native
side effects. It reserves copied input, task accounting and maximum response
credit within the observer queue and Source `max-inflight` before admission.
`0` writes a unique nonzero owned request ID and acknowledges admission only.
It does not wait for the synchronous native RPC. `11` before admission dispatches
no native service call. No automatic retry occurs for107/110 or a native error.

One owned worker per authenticated native session advances a finite queue of
bounded native calls. Queued work cannot start after Source retirement; work
admitted earlier may have external effects and its sealed-source completion is
discarded. Native timeout/error after an attempted invocation carries flag2;
expiry before invocation does not. Neither timeout nor cancellation promises
rollback in ROS peers. Session teardown joins the owned bounded worker, with no
detached jobs or unbounded `std::async` destruction. ROS1 uses a deadline-bound
TCPROS implementation with official generated serialization/identity; ROS2 uses
its bounded native future/removal path. Sensitive response delivery rechecks both
independent grants, native source type and the actually owned request ID.

## Six language entry points

| Language | Raw/codec/owned API | Runnable caller-driven lesson |
| --- | --- | --- |
|C|`ros1/abi.h`, `ros2/abi.h`; `roswasm/events.h`, explicit event backend and owners|`examples/c/11_unified_events.c`|
|C++20|Same raw imports; `roswasmcpp::EventStream`, caller-owned output and checked close|`examples/cpp/11_unified_events.cpp`|
|Rust|`raw::ros1/ros2`; `events::decode`; Capability watch/poll/readiness/submit (no alloc required)|`examples/rust/unified_events.rs`, feature exactly ros1 or ros2|
|TinyGo|RawEventWatch/PollV1/Ready/ServiceSubmit; complete decoder and Capability methods|`examples/tinygo/unified_events`, tags exactly ros1/ros2, scheduler none|
|AssemblyScript|Exact raw modules; copied event codecs; Capability methods with callback leases|`examples/assemblyscript/ros1/09-unified-events.ts` and ROS2 `examples/assemblyscript/09-unified-events.ts`|
|JavaScript|Complete raw adapter or real Guest relay; sync/async owned methods, copied codec and EventBinding|`examples/javascript/run.mjs` with `plan-unified-events.json`|

The lessons perform setup, real owned imports, one caller-driven read, explicit
Source close, permitted closed verification and checked observer cleanup. They
are runnable applications rather than exported no-op recipes. Native delivery
can be asynchronous, so the first read may legitimately be idle11. The native
publisher/server, explicit CLI grants, enabled matching Worker backend, controlled
memory and authenticated Gateway must be provided independently. Standard-language
startup imports must also be audited; no WASI clock/entropy/environment/thread
permission is silently added. TinyGo's scheduler-none removes hidden scheduler
clock/poll use, while its selected startup entropy requirement remains subject to
the actual artifact and an independent explicit WASI policy.

For these String lessons, the policy preparation tool accepts the **explicit**
combination `--operation subscribe --observe-sub`, in addition to its mandatory
backend, UID/PID, socket, node/namespace, resource and native connection inputs.
It emits `sub` and `subEvents` with visible reviewed bounds, applies no authority
and runs no process. A publisher-only selection with `--observe-sub` is refused;
there is no implicit source grant. The optional observer profile permits larger
records than the lesson's256-byte storage, so a long native message can return90
with a retained event. This example deliberately checks that result instead of
resizing or retrying. Source close remains permitted regardless of depleted
rate/byte credits and removes retained sensitive content.

Open the Source capability without invoking a legacy subscription factory before
a data watch. Those factories activate the legacy queue by design; a later data
watch on that same source returns17. Independently owned typed response/client
roles still use their ordinary methods with the received exact request/goal ID.
Decode the full event first, inspect the flags and select the actual Source codec:
topic1 uses String/Image/Twist or its registered bounded message, request2 uses
the Service request codec after ID8 (or Action goal codec when flag4), response3
uses the service response codec, feedback4 uses ID8 plus feedback, and result5
uses ID8/outcome8 plus result. Kinds6/7 have no payload decoder or native response.
The source alias and configured native identity fix this choice; kind2 alone
does not authorize service response to an incoming Action goal.

This document describes implemented source contracts. Compile, unit, real Guest,
Native ROS, overload, revocation and architecture acceptance run only in the
separate unified serial test stage. Historical34/39-function receipts remain
immutable and do not certify these additive imports or asynchronous events.

## Reusable callback/executor resources

Use the [six-language event resource guide](event-dispatch.md) for additive bound
EventSubscription/Stream resources, source/session/sequence audit state, actual
caller-driven executor registration and real service-submit completion. These
resources use this exact event ABI and existing canonical codecs; they open no
alias implicitly. Their new fixture sources and updated runnable lessons await
separate current-source guarded compilation and actual execution.
