# Complete client-library coverage for the current WasmROS ABI

The SDK provides six maintained client libraries and their runnable application
lessons: C11 `roswasmc`, C++20 `roswasmcpp`, Rust, JavaScript, AssemblyScript and
TinyGo. Every library starts at the exact ROS1/ROS2 import declarations, adds
canonical codecs and checked ownership, then exposes familiar Node, resource
and caller-driven executor patterns. The implementation covers all 38 ROS1 and
43 ROS2 imports registered by this Runtime. These are WasmROS client APIs;
they do not promise source or binary compatibility with native rclcpp, rclc,
rclpy, roscpp or their middleware/plugin ecosystem.

## Public layers and application families

| Family | C11 and C++20 | Rust | JavaScript | AssemblyScript | TinyGo |
| --- | --- | --- | --- | --- | --- |
| Exact import declarations and complete coverage probe | `abi/ros1.h`, `abi/ros2.h`, compatibility/freestanding headers | `raw::ros1`, `raw::ros2` | exact ABI metadata, raw and supplied bridge | `ros1/raw.ts`, `ros2/raw.ts` | backend-tagged raw declarations |
| Canonical values, complete integer widths and checked outputs | caller-buffer codecs and typed `wire` codecs | fixed-buffer codecs and `alloc` typed values | BigInt codecs and strict envelopes | checked typed codecs | fixed-buffer codecs and Message values |
| Unique/grouped resource lifetime | capability, Node, Session and checked cleanup | Capability, Node, Session | sync/async resources and Node groups | Capability, Node and owned resource factories | shared retirement state, Node, Session |
| Topics | typed publisher, subscriber and caller callbacks | Publisher, Subscription, Executor | sync/async Publisher, Subscription, Executor | Publisher, Subscription, Executor | Publish, Poll, checked Executor |
| Service request and exact-ID response | Client and separately granted ServiceServer roles | Client and Server | sync/async Client and Service | ServiceClient and ServiceServer | Client and Server |
| Fibonacci Action ownership and responses | single-role operations, four/five-role ActionClientBundle, ActionServer | ActionClient and ActionServer | sync/async ActionClient and ActionServer | ActionClient and ActionServer | ActionClient and ActionServer |
| Parameters and backend metadata | typed scalar/array values, operation-specific Parameters, ROS2 events | Parameters and typed events | typed Parameters and local event hooks | Parameters and typed events | Parameters and typed events |
| Graph, TF, clock, logger and ROS2 lifecycle | operation-specific resources and Node factories | typed resource methods | sync/async resource methods | typed resource methods | typed resource methods |
| Local deferred work | explicit Context, Future, bounded Executor | local Context and Future | Context, Future, bounded sync/async Executor | LocalContext, LocalFuture, LocalExecutor | Context, Future and explicit SpinOnce |
| Local timer and callback exclusion | caller-time Timer and CallbackGroup | Timer and CallbackGroup | explicit readiness Timer/AsyncTimer and CallbackGroup | LocalTimer and LocalCallbackGroup | Timer and CallbackGroup |
| Trusted bounded custom Topic/Service/Action codecs | reviewed generated C/C++ codecs | reviewed generated Rust codec | reviewed generated JS codec | reviewed generated AS codec | reviewed generated Go codec |

The exact aliases, method signatures, buffer lifetimes, checked cleanup and
backend restrictions are documented in the [C/C++ guide](c-cpp.md),
[Rust/TinyGo guide](rust-tinygo.md), [JavaScript API](../javascript/API.md),
[AssemblyScript support matrix](../assemblyscript/SUPPORT.md),
[Rust support matrix](../rust/SUPPORT.md) and
[TinyGo support matrix](../tinygo/SUPPORT.md). Compatibility forwarding paths
are retained; source-only CMake targets can be installed and relocated.

## Progressive runnable lessons

| Language | Application progression |
| --- | --- |
| C | Eleven entries: raw imports; encoded owner; Node/subscription; service/Action operation roles; query/event/control cases; delivery fixture; typed servers; Context/Future/Timer; Session ownership; complete Action client bundle |
| C++ | Eleven entries: raw imports; typed owner; Node/subscription; service/Action client roles; typed servers; query/control cases; delivery fixture; Context/Future/Timer; Session ownership; complete Action client bundle |
| Rust | Raw, typed, Node, resources, typed servers, Action/parameters, TF/graph/clock, local scheduling and the ROS2 event/lifecycle application; separate complete-import coverage probe |
| TinyGo | The same application families as Rust, with an explicitly selected backend build tag and a separate complete-import coverage probe |
| AssemblyScript | Nine ROS2 lessons and nine matching ROS1 lessons; raw, codecs, Node, resources, servers, backend query/event families, scheduling and Action/parameters; separate raw and pure-model fixtures |
| JavaScript | Nine lessons from raw/codec/Node/resource usage to supervised Runtime relay, complete async/sync application custom codecs and unified events; the command relay is a real Wasm module |

Follow the individual example READMEs in order. Every native operation needs a
matching native peer and explicit host policy. Source comments explain which
alias is used, when bytes remain borrowed, how a pending result returns to the
caller, and how to retain both operation and cleanup failures. A coverage probe
is a compiler fixture rather than an application that performs all operations.
Default tutorial branches and alternate compile-time selectors are all part of
the [serial local compilation matrix](local-testing.md).

## Authority, resource bounds and asynchronous behavior

Construction starts with no ROS authority. Context/Node/Session labels, codecs,
local queues, timers and Futures cannot enable a backend or create a grant.
Aliases select reviewed host capabilities; they do not select arbitrary native
names. Backend enablement and operation grants are separate explicit CLI inputs.
The Runtime remains authoritative for instance identity, type, native resource,
QoS, deadline, byte/rate/queue limits and goal/request ownership.

The C/C++ Action bundle takes four distinct explicit aliases for send,
cancel-own, feedback and result. A fifth cancel-any alias must be separately
supplied. Local goal storage is bounded to 1..1024 slots. A successful terminal
result retires its matching local goal; cancellation alone does not. Stronger
cancel-any/all is never inferred from send or cancel-own. Failed construction
closes prior acquisitions in reverse and reports every cleanup status.

Callbacks run only during explicit guest calls. Checked direct server/subscriber
facades reject callback-time recursive dispatch and destructive cleanup before
another dequeue or buffer reuse. Native C++ unwinding and JavaScript rejection
paths reset their local busy state; a fatal Wasm trap promises no cleanup. Raw
import calls and independently revoked sibling aliases remain outside these
local lifetime guards. A response still uses its exact acquired response role and the
request/goal ID delivered to that handler. Retain owned decoded values when
data must survive the next poll.

Local deferred service/Action work executes the existing bounded synchronous
import in a later explicit executor pass. Future cancellation cancels only
unstarted local work. Timers use caller-supplied time/readiness. There is no
implicit native asynchronous RPC, host clock, sleep, thread, retry, cancellation
or timeout rollback. Separately granted service_submit is an explicit bounded
Native admission path with event completion, described below. Errors 11/90/107/110 never cause effect replay. Checked
close and explicit destructive revoke remain separate operations.

Compiler runtimes can add WASIp1 imports independently of this SDK. Inspect
the binary import table and review each permission explicitly. The supplied
TinyGo recipes use `-scheduler=none`; startup entropy can still require an
explicit WASIp1 random permission. An import dependency never changes the
default-deny policy or proves portable operation without that permission.

## Backend and evidence boundaries

ROS1 includes parameter delete/exists/search and wall clock. ROS2 includes
parameter describe/declare/undeclare/events, Action graph inspection, lifecycle
and system/steady clocks. Version-absent operations return 95 and acquire no
replacement resource. The [ABI reference](abi-v1.md) specifies these differences
and the selected builtin type profiles.

Trusted generated bounded Topics, Services and Actions have a separate
[bounded profile](../custom-types/BOUNDED_V2.md), reviewed codecs and explicitly
registered native adapters. Builtin Trigger/Fibonacci factories do not silently
select arbitrary custom types. Dynamic IDL/CDR loading, unregistered native
interfaces and middleware plugins remain outside the supported interface.
Current new profile acceptance is source-only until the unified test stage;
historical fixed-record receipts retain their original scope.

Local source/native/pure-Wasm/relay models and actual Wasm compilation have
separate scopes. Use the [local verifier](local-testing.md) for frozen `/tmp`
inputs, one build job, bounded RSS and preserved failure logs. A model pass
does not establish native ROS delivery, authenticated principal admission,
toolchain dependency closure, Linux cgroup acceptance or aarch64/QEMU behavior.
The earlier [verification history](verification.md) remains tied to its frozen
revisions; the original Runtime acceptance task is not completed by installing
these client libraries.

## Additive unified event source contract

[Unified event v1](unified-events.md) specifies the separately granted
observer, complete64-byte envelope, seven kinds, Source/observer lease checks,
capacity-retaining poll, finite readiness and real bounded `service_submit`.
The new APIs are additive; local deferred tasks still advance synchronously
when explicitly scheduled, and do not gain native asynchronous authority.
No implicit alias, clock, thread, retry or stronger cancellation grant is added.
The six language families provide runnable unified-event lessons and source fixtures are prepared for the
unified serial test stage; dated historical34/39 receipts certify their original
revisions only.
