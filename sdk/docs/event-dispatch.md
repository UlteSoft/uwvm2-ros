# Caller-driven unified-event resources

The six language SDKs expose a reusable event resource in addition to the exact
ABI imports and codecs. This fills a practical gap: an application can register
real asynchronous service completion, an incoming request, Action feedback or
terminal state with a bounded guest-thread executor, without pretending that a
later blocking `service_call` is native asynchronous admission.

This is an additive source implementation. Its new regression fixtures and
updated lessons have not yet been compiled or executed. Existing dated SDK,
Source50 and middleware receipts describe their original bytes only.

## Explicit setup and independent ownership

Open the actual source alias and a **different** `event.observe` alias. Supply
both owners explicitly to the resource. The observer's trusted policy must name
that exact source alias/resource and list every permitted event kind and quota.
No constructor creates grants, discovers a resource, activates a legacy queue,
reads a clock, retries a timeout, starts a thread or changes WASI rights.

A source used for payload observation must not first be activated through the
legacy Subscription/Service factory. That would select another queue consumer;
the real Runtime returns17 rather than migrating consumers implicitly. Use a
plain Capability owner. Source and observer belong to the same selected backend
and principal. Correlation0 is used for topics, services, incoming servers and
metadata-only watches. Outgoing Action **data** uses its already owned goal ID;
feedback/result grants and cancel-own/cancel-any remain independent.

Every delivery validates the complete64-byte envelope, the exact source/backend,
the original Instance/session and a contiguous sequence starting at1. Validation
is transactional: foreign source/session, replay, length or sequence failure
changes no local binding and invokes no callback. Successful consumption commits
the sequence before user code, so an application callback failure never replays
an event. These local audit checks supplement the Runtime's actual authority and
ownership checks; they are not authentication implemented by a guest.

A resource-error or resource-closed record is a delivered event, not a successful
service result. Inspect kind, flags, error and correlation. Decode only with the
actual trusted source codec. The kind alone never permits a service response to
an incoming Action goal. Service requests and Action data retain their existing
ID prefix; service responses correlate through the envelope ID returned by a
successful `service_submit`. There is no automatic future/result map, timeout
retry, Goal-ID conversion or stronger cancellation grant.

| Language | Public resource | Registration | Ownership |
| --- | --- | --- | --- |
| C11 | `roswasmc_event_subscription` | `roswasmc_event_spin_some` | Borrows owners, fixed scratch, callback and user storage |
| C++20 | `EventStream` + `EventSubscription` | Existing `Executor::add` | Borrows stream/owners; entity owns fixed scratch and callback snapshot |
| Rust, no alloc | `events::Binding`, `events::Stream` | `Stream::spin_once` | Stream takes both unique Capability owners |
| Rust, alloc | Same Stream | `events::Executor::add` | Executor takes Streams; removal returns one Stream |
| TinyGo | `EventBinding`, `EventStream` | `EventExecutor.Add` | Borrows independent shared-retirement owners |
| AssemblyScript | `EventBinding`, `EventSubscription` | Existing `Executor.add` | Borrows owners; copied managed event/body |
| JavaScript | `EventSubscription` | Existing `Executor.add` | Borrows same-Session owners; owning copied event/body |
| Promise-facing JS | `AsyncEventSubscription.watch` | Existing `AsyncExecutor.add` | Same ownership; exact observer lease spans callback awaits |

The existing legacy Subscription executors keep their original receive behavior.
The C and TinyGo event executors are separate because their old registrations
represent legacy queues. Rust's distinct event executor also preserves the old
Subscription-removal API and unique-owner type. No existing ABI declaration or
backend dispatch table layout changes.

## Callback and retirement contract

Each explicit pass polls each selected registration at most once. Idle11 causes
no callback. Capacity90 retains the native record, preserving only a required
length actually written by the host. Scratch never grows automatically; choose a
new capacity only through explicit application lifecycle changes. Readiness is a
finite snapshot, not a reservation. A later submit may still return11 without
native dispatch. Admission success is not completion and may have external
side effects before a later timeout/error.

JS, AssemblyScript, Rust and TinyGo retain the observer's existing shared local
ownership lease before application callback invocation and throughout its
execution. JS/AS also hold it during polling/decode. Rust/Go perform the pure
transactional decode before entering that lease; no application code runs in
between. Direct observer close/revoke/poll and Node/Session cleanup refuse17
before mutation/import. JS awaits retain that same lease; rejection releases it. Rust exclusive borrows additionally prevent
safe same-object reentry. C++ and C reject dispatch/retirement through the event
facade while its callback runs. Their externally borrowed raw Capability owners
must not be moved, destroyed, closed or revoked directly during that borrow;
raw ABI calls bypass typed local guards. C callers must return normally and
must not `longjmp` through borrowed dispatcher state.

C++ exception-enabled callbacks unwind the local dispatch guard. JS rejection
and normal Go panic unwinding release their leases. Fatal Wasm traps/aborts do
not promise recovery or cleanup. The application remains responsible for finite
callback work and an external execution deadline. No SDK background callback
or clock permission enforces this deadline.

Synchronous JavaScript callbacks must complete synchronously and return normally;
their returned Promise is not awaited. Use AsyncEventSubscription with
AsyncExecutor for a Promise-returning handler, so its owner lease spans the full
awaited lifetime. Mixing an async handler with the synchronous entity is outside
this API contract, even if its code starts synchronously.

TinyGo applications must retain/register the returned EventStream pointer and
must not copy a live Stream value into independent audit states. Capability
copies share retirement state; that does not make duplicated per-watch sequence
state a second valid consumer.

JS and C++ retain a callback snapshot for the current invocation; replacing the
callback affects the next event. AssemblyScript snapshots its function reference
similarly and uses application classes/globals rather than unsupported lexical
closures. Existing JS/AS executors defer callback-time additions to the next
pass; C++/Go registration mutation is refused while spinning. C registration
arrays are caller-owned and must remain unchanged for the complete pass. Remove
registrations before moving/destroying borrowed resources. Never infer a native
resource close from removing a local registration.

`close` on an event resource closes only its observer; source ownership remains
independent, so explicitly permitted closed/error metadata can be read after
source retirement. Rust Stream owns both transferred capabilities; its explicit
`close` still closes only the observer, while `close_source` independently closes
the source. Rust event Executor's final `close` attempts both transferred owners,
retaining the first error while continuing cleanup. Go EventExecutor borrows its
Streams and closes their live observers only; close sources independently. JS,
AS and C++ generic executors drop registrations and do not close borrowed owners.
On Rust construction failure, transferred capabilities use normal best-effort
Drop cleanup; explicit checked cleanup is available after successful creation.

## Service, Action and lifecycle use

- For real native asynchronous service calls, use `submit`/`Submit` with the
  canonical selected request. Save the returned nonzero request ID only after
  status0, and match it with a later service-response/resource-error. Both
  response and error observations require explicit permission. A local deferred
  Future remains a separate synchronous scheduling API.
- For incoming services or Actions, register the advertise source as a unified
  event watch instead of activating a legacy advertiser queue. Invoke the
  independently owned respond capability with the exact incoming ID and correct
  generated/builtin codec. The observer cannot respond on its own.
- Observe outgoing feedback/result through their separately granted receive
  owners and the already owned goal ID. A metadata-only observer uses zero and
  acquires no payload consumer. Cancel-any never follows from cancel-own.
- A lifecycle/clock/query capability may be watched for explicitly permitted
  error/closed metadata. ROS2 lifecycle get/change still target the exact
  host-selected node/transition; the SDK does not add a local lifecycle state
  machine or emulate unsupported ROS1 lifecycle operations.

The updated runnable lessons are the existing C/C++ `11_unified_events`, Rust
`unified_events.rs`, TinyGo `unified_events`, shared AssemblyScript
`unified-events.ts`, and JS `09-unified-events.mjs`. The existing embedded
QuickJS lesson08 invokes that same JS application, now through the actual async
event entity/Executor. Its closed module list already includes all dependencies;
no new engine module, import or implicit permission is added. Every lesson opens
both explicit owners, performs a bounded pass, retires its source, checks the
permitted closed event and checks observer cleanup. A real native peer may not
have delivered before the first pass: idle11 remains a legitimate first outcome.

## Authored regression sources and actual invocation shapes

Run only under the selected Linux64GiB/swap0 cgroup and its existing external
resource/process owner. No command below has been executed for this change. All
output/cache/temp directories must be fresh owned `/tmp` paths. Replace tool,
sysroot/archive paths with the actually qualified selections; install/download
nothing implicitly. Keep serial builds or explicitly memory-budgeted jobs.

The maintained SDK model runner now includes the new C/C++ fixture, two Rust
binding tests, three Go dispatcher/executor tests and the20-assertion pure AS
fixture. Its normal source/installed-SDK/root guards and exact test inventories
remain mandatory. `sdk/tools/build_cgroup_matrix.py` includes the additional
AssemblyScript pure compile row; `run_cgroup_models.py` consumes its genuine
terminal compiler report before running the pure Wasm. The JS source/model
runners include the new sync/async contract file. Constructing a plan is not a
successful test.

For an individually selected native fixture, literal command shapes are:

```sh
"$CC" -std=c11 -O2 -UNDEBUG -I "$SOURCE/sdk/include" \
  "$SOURCE/sdk/tests/c/event_dispatch_contract.c" -o "$OUT/event-c"
"$CXX" -std=c++20 -O2 -UNDEBUG -I "$SOURCE/sdk/include" \
  "$SOURCE/sdk/tests/cpp/event_dispatch_contract.cpp" -o "$OUT/event-cpp"
"$OUT/event-c"
"$OUT/event-cpp"
"$NODE" "$SOURCE/sdk/javascript/tests/event_dispatch_contracts.mjs"
```

The AS compile row uses the same actual installed compiler as the maintained
matrix, without a NativeApi or ROS/WASI imports:

```sh
"$NODE" "$ASC_JS" "$SOURCE/sdk/assemblyscript/tests/event_dispatch.ts" \
  --runtime incremental --exportRuntime \
  --use abort=sdk/assemblyscript/api/abortGuest --outFile "$OUT/event-as.wasm"
"$WASM_TOOLS" validate "$OUT/event-as.wasm"
```

Execute `_start` once through the maintained pure-model runner and require
`completedChecks()==20`, zero imports, exact compiled source/validator evidence,
full input preservation and clean owned process teardown. These are local
finite-source models, not ROS delivery. TinyGo's native private dispatcher tests
use the real production state machine without importing raw ROS functions; real
Wasm application compilation uses the existing lesson:

```sh
"$TINYGO" build -target="$ACTUAL_WASM32_WASI_TARGET" -scheduler=none -tags=ros2 \
  -o "$OUT/event-go.wasm" "$SOURCE/sdk/examples/tinygo/unified_events"
```

`ACTUAL_WASM32_WASI_TARGET` is the qualified target JSON/triple selection used
by the existing TinyGo recipe, not an assumed builtin target alias. Keep its
actual target/compiler/stdlib dependency closure and artifact imports;
startup permissions depend on that actual binary and are never silently granted.
For ROS1 select only `ros1`; there is no automatic backend fallback.

For a real Rust executor Wasm regression, first build the current crate with the
qualified wasm32-unknown standard library and exactly `ros2,alloc`, no default
features, using a fresh offline Cargo target directory. Then consume that actual
reported rlib, not a guessed filename or an old Source50 object:

```sh
"$CARGO" build --offline --manifest-path "$SOURCE/sdk/rust/Cargo.toml" \
  --target wasm32-unknown-unknown --no-default-features --features ros2,alloc -j1
"$RUSTC" --edition=2021 --target wasm32-unknown-unknown --crate-type cdylib \
  -C panic=abort -C link-arg=--max-memory=67108864 \
  --extern "roswasm=$ACTUAL_RLIB" "$SOURCE/sdk/tests/rust/event_dispatch.rs" \
  -o "$OUT/event-rust.wasm"
"$WASM_TOOLS" validate "$OUT/event-rust.wasm"
"$NODE" "$SOURCE/sdk/tests/event_dispatch_wasm_runner.mjs" "$OUT/event-rust.wasm"
```

Set `CARGO_TARGET_DIR`, `CARGO_HOME`, TMPDIR and compiler output/cache locations
explicitly below the owned `/tmp` tree; supply the actual offline Rust sysroot
and linker exactly as in the maintained compile recipe. The finite mocked Wasm
host accepts only the actual test's ROS2 imports, calls `contract` once, checks
one real SDK Executor callback and both owner closes. It reaches no Gateway or
ROS network. Rust native binding tests separately reject wrong source/session,
replay/length and true u64 sequence exhaustion; safe Rust prevents alias-based
self-retirement during the same exclusive callback borrow.

For embedded JS, use the existing actual pinned `guest/prepare.py` selection
`08-unified-events-ros1` or `08-unified-events-ros2`, explicit full manifest,
QuickJS/source roots, genuine WASI compiler/sysroot/archives and validator. The
producer prints concrete compiler jobs; execute every prepared job unchanged,
retain full MD/link/import evidence, then use the actual qualified Runtime with
explicit Source/observer grants and a genuine native peer. Promise/Node models
do not qualify this Guest path. No new trusted engine route is introduced.

## Remaining unsupported behavior

This change does not claim native rclcpp/rclc/rclpy source compatibility, arbitrary
runtime IDL, native callbacks into Wasm, unbounded async request/future storage,
implicit clocks/timers, rollback of admitted ROS effects, guest ROS networking,
ROS1 lifecycle emulation or a cross-backend fallback. Generated bounded Service/
Action facades and codecs remain their separate reviewed APIs; event callbacks
can consume their actual source bytes but do not synthesize new native adapters.
Actual compilation and real ROS, revocation, overload, architecture and
performance acceptance remain required for these new source bytes.
