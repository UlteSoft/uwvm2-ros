# Pinned Jazzy Service source migration and genuine asynchronous Futures

This is an additive successor to the reviewed Python6 lifecycle implementation.
Actual copied-source checks under `/tmp` passed all 75 local models: the previous
49 ownership/lifecycle cases plus 26 service/Future cases, including the unchanged
original service main through its explicit bootstrap. These checks used a local
backend model; a new frozen Wasm producer and native service exchange remain
required. The [dated checkpoint](../../../../documents/ros/sdk-compatibility-checkpoint-20261018.md)
retains the exact source and result identities. It grants no authority, installs no Host policy and implements
no debugger, module update, management command, socket or native-pointer import.
A local model is not a Runtime/Gateway/native result. All prior Python6 and
Source64 artifacts/results retain their original source and time identities.

## Exact original sources and selected entry

The preserved originals under `upstream/service_future/` are actual official
Git blobs at these immutable Jazzy commits, with whole commit/tree JSON,
licenses, hashes and acquisition data. Downloading these bytes did not execute
them or qualify an installed SDK:

- `ros2/examples` `07008852303f2a35a91c65d78046b274a35477ea`;
- `ros2/rclpy` `4e607c09d624debc39a4ae5405680c06d87fac35`;
- `ros2/example_interfaces` `4ed406d1277df78be01a31005306d69e7592a993`;
- `ros2/common_interfaces` `a941f14bb318d8d904505ed935ccbb97f24a70a4`.

The complete official `service_member_function.py` is copied byte-for-byte into
its actual `examples_rclpy_minimal_service` package as well as the upstream
archive. `examples/official_service_bootstrap.py` installs an explicit deployment
profile and calls that original module's original `main()`. The class,
constructor, callback and main are not edited, projected into another message,
or replaced by a test callback. Its original AddTwoInts request/response and
callback log are retained. The original plain-function `service.py` and every
minimal-client source are preserved as whole original evidence too.

The plain-function server uses default `spin_once`, which presently requires a
native wait or a further explicit source-compatible driver facility. It remains
unselected. Every retained official client main first calls `wait_for_service`;
ABI1 has no native service-availability predicate. Those original mains remain
unselected until that real protocol is implemented. Graph listing, a submit
acknowledgement, a native peer's marker, a local flag and a caller-supplied bool
are not substituted for that predicate. This is required future work, not a
permanent exclusion or a claim that all original client examples work already.

## Complete records and actual native identity

`example_interfaces.srv.AddTwoInts` contains every original field: Request.a/b
and Response.sum are exact signed64 integers. Both extrema are retained; bool
and overflow are rejected by the current canonical profile. `std_srvs.srv`
contains the complete Empty, Trigger and SetBool Request/Response field sets:
zero fields really encode zero bytes; SetBool.data and response.success are
canonical bool; the entire response.message retains UTF8, NUL and BOM. No dummy
native empty-struct member becomes an invented wire byte.

These classes provide the selected public field/constructor/service nesting and
canonical codec API. They are not installed `rosidl_generator_py` native type-
support capsules or native struct addresses. The complete original `.srv`
definitions remain visible. Trigger and SetBool use the actual existing Runtime
builtin adapters. `interfaces/catalog.preparation.json` uses the existing
custom-bounded-v2 pipeline for exact `example_interfaces/srv/AddTwoInts`: Root
must export/bind the entire actual installed Service graph and native identities
with the reviewed vendor/native machinery, generate the real native registry,
and build the selected Runtime/Gateway coherently. No native MD5/RIHS is invented
or computed here. Their actual products/pins remain null until real preparation.

Empty has a complete record/codec but lacks an actual current Runtime/Gateway
service adapter. Its factory rejects before any open. `std_srvs` is reserved by
the custom catalog boundary; it cannot be shadowed to bypass that missing
builtin. A coherent native adapter/identity/protocol extension is required.

## Explicit profile, logger and ownership

A Service requires separate exact `service_advertise` and `service_respond`
bindings. An asynchronous Client requires separate exact `service_call` and
`event_observe` bindings. Keys contain operation, fully resolved resource and
full service type; QoS must match. The Host independently fixes aliases,
resource/type/native identity, QoS and finite native quotas. All aliases are
nonempty, distinct, strict UTF8, NUL-free and bounded before the first effect.
All local capacities and both owner positions are checked/reserved before open.
Default profile/Host authority remains empty; missing local binding13 does not
qualify actual Host denial. `examples/default_denied_service.py` installs local
bindings but requires a separate real EMPTY Host policy and an actual first
service capability_open13, no adopted handle and no fictitious failed-open close.

`Context.configure(..., prepare_callback_logging=True)` is a new explicit opt-in
(default false). The service/client factory also requires a separate exact
`log_write` binding for the Node FQN, validates its distinct alias and reserves
all three owner/rollback positions before any acquisition. The logger opens
before the service/client roles and becomes visible only with the committed
transaction. Failure retires all newly acquired roles once in reverse real
acquisition order and preserves the setup primary plus every cleanup outcome.
An existing logger is reused only with the exact selected alias. No flag creates
a Host grant. This permits the original callback's first info log without
opening anything inside a callback. The original Python6 `_Logger.info` live
check and default callback-time lazy-open17 behavior remain intact.

Paired owners, resource wrappers, callback/diagnostic slots and ordinary Node/
executor registrations exist before effects. Repeated owner-list positions are
reserved distinctly. Existing Python6 `_FactoryCleanup` is reused unchanged;
no append-after-open gap or cleanup list growth is introduced. Both role owners
are attempted on cleanup even when the first close fails, and registration
rollback removes only its exact reserved positions. The private core SDK
Session, factory, codecs, raw81 inventory and backend bridge are unchanged.

Every selected poll/decode/callback borrows the entire Context, including other
Nodes and source aliases. The current actual source's own SDK poller holds its
owners; all siblings are marked busy. Client observer polling explicitly holds
both client/observer owners as well. This covers direct Service/Client helper
polls, done callbacks, raw Session.close and alias sibling revocation. Factories,
new lazy logger/clock resources, native-owner/Node/Context/Service/Client
teardown and reentrant executor dispatch reject17 before mutation. Explicit local
timer cancellation/removal retains the existing scheduling-only behavior: it
does not retire the shared busy Clock owner or acquire any new capability. Existing publication/log writes remain usable; closing/regranting
is not part of a callback. All lease release is in finally. Timer clocks are
also borrowed, because the timer itself only owns a local timer busy flag.

Node/Context shutdown marks pending attached Futures with local125 and attempts
all already owned closes; this is not native cancellation. Callback failures
remain in fixed Future.callback_errors slots and do not skip later callbacks or
replace a recorded service outcome. All storage required by factory rollback,
reverse cleanup and admission-failure cleanup is reserved first. These are
ordinary executable Python recovery paths. A terminal Wasm trap/unrecoverable VM
failure still requires the genuine outer Runtime/Host owner to drain resources.

## Service callback and asynchronous protocol

The server delegates to the actual current core ServiceServer. It validates the
nonzero8LE request prefix, decodes every request field, calls the original
`callback(request, Response())`, requires the exact complete returned Response,
and sends one response with that unchanged owned ID through the other alias.
A callback/decode/encode failure is preserved; no fabricated error response or
automatic retry is emitted. Native RMW header pointers are not Guest IDs;
manual native-header `send_response` and introspection configuration explicitly
refuse until their complete selected protocol exists.

Client creation opens its actual service source and separate event observer,
then calls real `event_watch` for that same source with correlation0 before any
request is submitted. `call_async` preallocates Future, callback/error slots and
failure-close work; captures the complete request bytes once; then invokes real
`service_submit` exactly once. A successful acknowledgement must contain a
nonzero strictly increasing owned u64 ID. The Future is still pending. No code
path calls synchronous `service_call` and pretends its result is asynchronous.
Any failed/uncertain admission retires the pair once using reserved cleanup,
preserves the original Result/exception and never retries110.
Older pending requests retired as a consequence receive an explicitly local125
with their own request ID and the admission primary as cause. They are not
misreported as native110 completions of those older requests. Actual native
terminal events and local retirement carry distinct origin/event fields.

Completion arrives only through real `event_poll_v1` and the current full64-byte
header validator. It must match the watched source, backend, instance, session,
observer sequence and an admitted request correlation. Kind3 has the complete
response body, without an invented repeated ID prefix. Kind6 retains numeric
error, correlation, retired/external-effects flags and raw Event in a reserved
ServiceEventError. Retired/uncorrelated scope errors and kind7 close finish all
attached pending Futures and prevent later calls; they do not regrant authority.
Malformed source/sequence/kind/correlation cannot complete an unrelated Future.
A consumed response with bad typed bytes completes that exact Future with its
codec error and is never replayed. Detached/cancelled IDs remain internal
bounded tombstones until actual completion is drained, so local detachment does
not falsely release native inflight credit.

The selected Future keeps the pinned Jazzy meanings: `done()` means FINISHED,
`cancelled()` is a separate state, `result()` is None while no value/error is set,
and a no-executor late done callback is immediate. Cancellation/removal only
changes local delivery and claims no DDS/native cancellation. A Future has at
most32 registered callbacks over its lifetime and the Client at most32 pending
requests, failing90 before another effect. All callbacks are attempted even if
one raises. Positive-timeout waits, native threaded Executor/Task scheduling,
synchronous native waits, callback groups and full coroutine executor semantics
explicitly refuse; a `__await__` generator alone is not asyncio compatibility.

`spin_until_future_complete` drives only a Future of the exact Context, using an
explicit finite Driver or one timeout0 nonblocking pass. Context credit counts
are independently bounded even if a Driver always returns true. The selected
original server spin retains its existing first nonblocking pass plus at most64
explicit R-newline credits. No native clock, busy-loop availability inference or
implicit wait-set is installed.

## Future actual build, test and acceptance

Only Root executes actual tools. First integrate the exact reviewed successor
with its exact Python6 preimages and unchanged current SDK/bridge; freeze the
whole coherent source. Keep every old Source64/Python artifact associated with
its original source. Use a fresh copied SDK layout for each build/test. The
matching real CPython3.14.8 native build-Python, target archives/configuration,
WASI SDK24 and complete stdlib/builtin/tool/cache closure remain required.

The current `sdk/python/tools/freeze.py` collects explicit SDK/rclpy roots. The
two byte-exact `examples_rclpy_minimal_service` package bodies have their normal
module names and are frozen with the SDK. The bootstrap imports that frozen
module and calls original.main; it needs no target source-file loading or
filesystem preopen. Do not execute downloaded upstream native modules or use
host Python as the Guest substitute. The freeze manifest must retain every
source/bytecode hash; actual Guest startup/builtin support is still unqualified.

For future local tests, Root selects an actual bounded local Python and explicit
copied roots, for example with `PYTHONDONTWRITEBYTECODE=1` and
`PYTHONPATH="$COPY/sdk/python:$COPY/sdk/python/compat/rclpy"`, then runs the copied
SDK test discovery under its owner. The previous49 method sources remain
unchanged. `test_rclpy_services.py` adds26 authored methods, including invoking
the complete original server main/class/handler under a clearly labeled local
Model. Models are local logic, never native/Runtime/Wasm proof. No test has run
in this source package. Root reports the actual discovery/count/results.

Actual target builds then use the matching freeze output, real `_native.c` C
extension and actual libpython/WASI link closure, validate the entire Wasm and
inspect all actual imports. Source raw81 completeness and binary reachability
are distinct; no Host management/debug/update/policy import belongs to the
Guest. Real denial, granted server, genuine asynchronous client, Trigger and
SetBool routes must use actual independently prepared Runtime/Gateway/native
artifacts, explicit native QoS and all resource limits, complete values/IDs and
owned cleanup, not just printed counters. `service-acceptance-plan.json` is the
closed future recipe; its actual identities/results remain null.

`native/service_peer.cc` uses actual installed rclcpp/example_interfaces/std_srvs
headers and real native availability/futures; it includes no facade structs.
The no-callback Jazzy async result uses `.future.share()` and preserves the real
request_id for local pending removal on witness failure. That removal is not
native cancellation. Root must compile it separately with full MD/ELF/map/SDK
proofs and run it under a bounded pidfd/native deadline and <=64 stdin credits.
Printed readiness/admission/value lines are evidence inputs, not whole-route
acceptance; independently associate complete payloads through actual Host/
Gateway/Guest ownership and preserve every original raw failure/log/source pin.

## Revision r2: local Future retirement after explicit owner close

The separately preserved first source HOLD remains immutable. A Client whose
source and observer owners were explicitly closed outside dispatch may still
have attached local pending Futures. Destroying that facade now finishes those
Futures with local125 under the complete Context lease before removing the
scheduling source. Both native owners are already retired, so no close, submit,
watch, cancellation, reopen or response event is invented or repeated. The
additional authored model closes both real model owners first, then checks the
Future identity/origin, callback lease, complete scheduling removal and unchanged
two-close ledger even after later Context shutdown. Existing25 methods remain
AST-identical; all26 new-family methods remain unexecuted in this source package.
