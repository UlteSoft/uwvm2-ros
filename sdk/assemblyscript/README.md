# AssemblyScript roswasm SDK

AssemblyScript compiles these sources into real Wasm guests. The two `raw.ts`
files declare every registered ABI v1 import using `@external`: 38 in
`uwvm_ros1_v1` and 43 in `uwvm_ros2_v1`. Select one version explicitly. The
SDK needs no native ROS headers or libraries inside the guest.

## Learning path and build

Read the four entries in [examples/assemblyscript](../examples/assemblyscript):
`01-raw.ts` opens/closes one alias with exact imports, `02-codecs.ts` publishes a
canonical String, `03-node.ts` introduces typed Node/executor concepts, and
`04-resources.ts` calls a service and reads a typed parameter/clock. Each entry
exports `_start()` plus optional diagnostic exports. The matching ROS 1 entries
are in `examples/assemblyscript/ros1/`; their advanced parameter example uses
canonical Int32 explicitly, while the ROS 2 entry uses Int64. The command runtime
does not
automatically print these diagnostics or grant native logging permission.

With an already installed AssemblyScript compiler, compile from repository root:

```sh
asc sdk/examples/assemblyscript/03-node.ts \
  --outFile /tmp/roswasm-as-node.wasm \
  --runtime incremental \
  --use abort=sdk/assemblyscript/api/abortGuest
```

The abort override traps locally instead of importing `env.abort`. Avoid using
`console`, `Date`, randomness, timers, filesystem or DOM APIs in a standalone
guest: those require additional unsupported or explicitly authorized imports.
Inspect the resulting binary before running it and check all imports against the
[authoritative ABI manifest](../abi/abi-v1.json). Compilation alone proves no ROS
behavior. This command is a prepared build recipe; it has not yet been executed
for this SDK revision because the authorized Linux cgroup host is offline.

Compiler syntax, explicit external names and UTF-8 helpers follow the official
[AssemblyScript concepts](https://www.assemblyscript.org/concepts.html),
[compiler](https://www.assemblyscript.org/compiler.html),
[runtime](https://www.assemblyscript.org/runtime.html) and
[String documentation](https://www.assemblyscript.org/stdlib/string.html).

## Authority and memory

Starting a guest provides no ROS authority. An import declaration or `new Node`
opens no capability. A runtime operator provisions exact alias/resource/type
identity/operation/limit grants through CLI configuration. Without a grant,
`Capability.open()` returns 13, and the SDK preserves that denial. Example alias
names such as `greeting-pub` are policy labels, not automatically created topics.
Native endpoint names and authentication remain controlled by the runtime policy.

Raw ABI memory indices are u32/i32. Handles, offsets, lengths and capacities are
u64/i64 even in wasm32. Managed wrappers use memory 0 and convert `dataStart`
addresses to u64 explicitly. Strings are UTF-16 managed objects; first encode
UTF-8 into a managed byte array, then pass its data address. Rooted managed arrays
remain alive during a synchronous import, which copies the bytes. Native ROS
never retains the guest pointer or calls a guest function pointer.

Every handle/length output slot is eight little-endian bytes. Open initializes its
handle slot to zero and accepts every nonzero opaque bit pattern. Request/poll
length slots use `UINT64_MAX`. On error 90, `Reply.requiredValid` is true only if the slot
was overwritten. The event may already be consumed, so there is no automatic
buffer growth, retry, replay or timeout restart. Choose the output capacity before
calling, within the runtime's byte quota. `MAX_WIRE` is a conservative request-data
bound, not proof of the configured alias's quota.

Close each owned capability explicitly. After one close attempt, the SDK releases
ownership even if a backend error follows runtime handle retirement. `Node.close`
closes still-owned handles once. There is no destructor/GC import, native thread,
default clock or implicit log authority. An admitted timeout may leave native
effects in progress; application recovery must respect that uncertainty.

## Public layers

| Layer | Import | Usage |
| --- | --- | --- |
| Raw | `ros1/raw` or `ros2/raw` | Exact imports and explicit memory selection |
| Codec | `codecs` | Canonical bounded values; inspect `.valid` before using decoded structures |
| Managed | `ros1/roswasm` or `ros2/roswasm` | `NativeApi`, Node, typed publisher/subscription and executor |
| Resources | Same managed module | Client/Service, ActionClient/Server, Parameters, Graph, Clock, Logger, Tf and Lifecycle |

`Executor.spinOnce` processes at most one event per registered entity. A handler
is ordinary guest code invoked after copying and validating the event. The caller
explicitly schedules subsequent iterations. The pass snapshots its registration
count at entry. A handler may register more entities, but they run only in the
next outer pass. Nested `spinOnce` returns `[Status.Conflict]` without polling
anything; it cannot recursively redispatch a handler. Unrecoverable guest traps
terminate a pass rather than becoming runtime status codes. Graph/list responses contain one
length-prefixed string of newline/tab rows; they are not parameter String arrays.
The native alias fixes parameter names, TF frame pair, clock kind, lifecycle node
and allowed transition. `Resource` exposes every remaining registered operation
at the canonical byte level without introducing wildcard authority.

An ActionClient needs only send authority. Cancellation, feedback and result
aliases are optional and opened only when explicitly supplied. `cancelAll()` uses
the ABI's special zero goal selector only through a separately granted
`action.cancel-any-goal` capability; own-goal cancellation and receive operations
still reject zero. Close/revoke each owned handle once and release local ownership
even when a backend error follows runtime retirement.

Service and Action factories acquire all explicitly requested handles first,
then activate an advertiser. If any open fails, they close every earlier handle
once, preserve the original initialization status, and commit no handles to Node
ownership. A failed optional-but-requested Action alias rolls back the whole
factory. Inspect server/client `.status` before use; failed construction never
activates an advertiser through a missing respond capability.

`Parameters.getReply()` and `readTextReply()` preserve exact runtime status and
known/unknown required-length information. Decode only successful bytes, using
`Parameters.decode(reply)` for typed values. Convenience methods that return an
invalid value or empty text cannot by themselves distinguish denial, a capacity
refusal and disconnection; retain the Reply for application recovery decisions.

Named factories validate their operation family before opening an alias. A Graph
cannot be constructed through a Clock selector, and a Clock cannot route an
authorized parameter-delete operation. Directly constructed named facades also
recheck their family before dispatch. Wrong families and wrong named method
selectors return Invalid (22) locally. `Parameters.getReply` requires get, `set`
requires set, `declareValue` requires declare, metadata reads require list/describe/search,
and explicit delete/undeclare methods require their own selector. TF lookup and
broadcast, and Lifecycle get-state/change-state, similarly require their exact
selector. `Resource` remains the intentionally generic canonical byte-level API.

Use `Parameters.declareValue(value)` for the explicitly granted ROS 2 declaration
operation. The earlier facade spelling `Parameters.declare(value)` is rejected
by AssemblyScript because `declare` is a reserved keyword; update source callers
to `declareValue`. The raw `parameter_declare` import, its Wasm signature and the
`Operation.parameter_declare` selector remain unchanged. This facade rename does
not provide declaration authority: the runtime still requires the exact trusted
grant, and a mismatched operation or value kind returns Invalid (22) locally.

Parameter events use `Operation.parameter_subscribe_events` with the dedicated
subscribe/poll methods; they are not a seven-argument request operation. ROS 1
NativeApi does not register that selector, so its factory rejects it as unsupported.

Builtin typed Fibonacci Action adapters are implemented for both ROS 1 and ROS 2.
Select a runtime profile that includes the intended backend and configure its
supported native type and exact identity. Unsupported types or operations can
return 95; importing a backend absent from the selected runtime is a separate
module-loading failure. ROS 2-specific parameter/lifecycle operations are not
silently substituted into ROS 1. NativeApi checks the registered operation before
an exact resource factory opens an alias. Supported client concepts do not imply
complete `rclcpp` API parity, arbitrary native node creation or custom IDL support.

## Prepared verification fixtures

`tests/coverage_ros1.ts` and `coverage_ros2.ts` force every raw import to remain
reachable for binary signature inspection. Their `invokeRaw` branch intentionally
passes zero arguments and must **not** be executed with real grants. `_start` is
empty. `tests/highlevel_coverage.ts` makes managed generic APIs reachable through
an unavailable Api base. `tests/codec_vectors.ts` prepares all 57 positive and 11
negative independent wire fixtures and imports no ROS functions.

`tests/factory_transactions.ts` and `adapter_contracts.ts` prepare guest-local fake
models for acquisition rollback, optional privileges and output slots. They are
not native ROS authorization tests, and their actual Wasm execution remains pending.

`tests/executor_mutation.ts` prepares self-registration, deferred new entities and
nested-spin checks using pure guest-local Pollables. It adds no ROS import or
native callback and has not yet been compiled or executed for this revision.

`tests/resource_roles.ts` prepares wrong-family/direct-constructor/method-selector
rejections with guest-local host-call counters. It distinguishes intentionally
generic Resource calls from named read/control operations and has not yet been
compiled or executed for this revision.

`tests/unicode_strings.ts` prepares literal leading-U+FEFF roundtrips for Strings,
parameter arrays, service responses and typed parameter events. A framed String
must preserve that code point rather than consume it as a stream BOM. This fixture
imports no ROS functions and remains uncompiled/unexecuted for this revision.

Current compiler, Wasm import/signature inspection and local fake-fixture
execution are coordinated serially in separate `/tmp` outputs under the
parent's low-memory guard. No agent launches parallel compilation or native ROS
locally. Historical64GiB receipts retain their original frozen-source/budget
scope. Real ROS/AArch64 acceptance remains a separate lane in the original
Linux64GiB zero-swap cgroup when available; a local fake fixture is not that
acceptance. Static inventory matching alone proves no compiler or execution
PASS. No toolchain download is needed to read or review these sources.

## Bounded ownership and local scheduling increment

`Session(api, maximum)` checks ABI version1 and a1..1024 local live-capability
budget without opening anything. `session.node(label)` attaches factories to
that group; it is local organization, not native Context/node identity or a
connection credential. Native policy remains default deny. Session.close and
Node.close mark closed first and return every live-handle close status exactly
once. Inspect all statuses even after an operation fails; retained typed wrappers
and later factories report9. No GC finalizer imports ROS. Typed resources expose
revoke and checked full-u64 rawHandle; servers borrow two independently granted
IDs. Alias UTF-8 length is checked before open and embedded NUL is refused.

Executor has bounded checked registration, removal and explicit close. It owns
only local Pollable registrations, so close the Node/Session for native cleanup.
Closed operations return9. Nested spin/removal/close is17; handler additions
remain deferred to the next outer pass, preserving existing mutation semantics.
No native callback/function pointer enters AssemblyScript. Service handlers
retain the incoming ID and complete validated response; Action replies retain
the incoming goal ID. A trap aborts the Guest and is not a successful outcome.

`Context` is a separate bounded local deferred queue plus caller-time timers,
not a native async ROS implementation. `defer` returns a one-shot Future of a
copied Reply. Pending/running is11; consumed is9; cancellation of not-started
work completes with local125 without revoking a host alias. CallbackGroup guards
same-group reentry with17. Timers use explicit caller-domain u64 timestamps,
positive periods, checked overflow and at most one callback per pass; Context
refuses backwards time before effects. No clock, thread, sleep, environment or
grant is implicit. Shutdown cancels queued work/timers; Session.close is separate.
AssemblyScript function references are used; no JavaScript closure support is
assumed. A deferred Client still performs its synchronous host RPC during that
later step and cannot be interrupted by local cancellation once started.

`tests/local_schedule.ts` prepares local result, timer, budget, closed9 and
checked failed-close contracts using an unavailable-native/fake Api. The existing
factory rollback test retains all earlier assertions and adds closed-Node9;
its second acquisition now uses a fresh Node because closed organizers cannot
reopen. Current verification is coordinated locally with one job and `/tmp`
outputs. The old64GiB receipts above describe historical evidence only. Source
preparation, native Go/Rust tests, and successful Wasm compilation each have
different scopes; none establishes native ROS delivery for these new sources.

## Additive unified event source contract

[Unified event v1](../docs/unified-events.md) specifies the separately granted
observer, complete64-byte envelope, seven kinds, Source/observer lease checks,
capacity-retaining poll, finite readiness and real bounded `service_submit`.
The new APIs are additive; local deferred tasks still advance synchronously
when explicitly scheduled, and do not gain native asynchronous authority.
No implicit alias, clock, thread, retry or stronger cancellation grant is added.
The six runnable unified-event lessons and source fixtures are prepared for the
unified serial test stage; dated historical34/39 receipts certify their original
revisions only.
