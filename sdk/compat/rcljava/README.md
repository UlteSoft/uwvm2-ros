# Pinned rcljava topic, composition, timer and service source migration

This is an opt-in source implementation with independently reviewed service
and Future additions. Actual Java 17 source compilation covered twelve complete
original application classes. Local checks passed for the previous topic/timer
profile and twenty-eight service groups, including the unchanged original
service main through a separate bootstrap. These local models establish no
native ROS delivery; the original client main still needs native availability.
It uses the current `sdk/java` ROS2 copied-buffer transport, actual
canonical codecs and checked capability ownership. It does not load JNI,
replace a native rcl library, fabricate native pointers or grant any ROS right.
Do not add this source root to a native rcljava application's classpath by
accident: it deliberately occupies the same Java package names for a separately
selected Wasm migration target. Never combine it with a native rcljava jar.

The selected API comes from actual official source at these immutable commits:

- [ros2_java d0e4e952](https://github.com/ros2-java/ros2_java/tree/d0e4e952bad1977ce9818d00b520d3ad0aeafd8d), actual Git tree `152e79bcb6ad7e3e0ef808acbd5ae47191cadb76`.
- [ros2_java_examples 3c1fa01f](https://github.com/ros2-java/ros2_java_examples/tree/3c1fa01f95fb96110953c0f7f196424aa0557b97), actual Git tree `cbae8811f7d7d918ab6a7a5ad3a57993d707812c`.

`upstream/` preserves every acquired official source, LICENSE, NOTICE, commit
response and tree response without changing any business-source byte. The
supplemental executor, Context, NodeOptions and WallTimer implementation sources
were fetched at that same first commit and checked against their actual Git
blob IDs. This is a selected-source acquisition, not a materialized whole
upstream checkout or an installed native SDK qualification. The complete native
upstream Node/Publisher/Subscription interfaces remain visible there; the
selected subset does not imply that all their methods are implemented.

## What can be selected

The POM selects the original PublisherLambda, PublisherMemberFunction,
SubscriberLambda, SubscriberMemberFunction, SubscriberNotComposable,
composition Composed/PublisherNode/SubscriberNode and TimerLambda/
TimerMemberFunction classes. All ten remain unchanged. The original spelling
`minmal_publisher` is intentionally preserved. TimerMemberFunction's original
main instantiates TimerLambda; that upstream behavior is not silently repaired.

`std_msgs.msg.String` implements the complete `data` field with the observed
`getData`/`setData` API. Encoding and decoding reuse `Codecs.STRING`, preserving
UTF-8, NUL, BOM and exact framing; they do not use CDR or JNI conversion handles.
MessageDefinition converter/destructor/type-support pointer methods throw
explicitly. Disposable.getHandle also throws: a Wasm capability is not a native
rcl pointer. Native String type identity remains the Host/Gateway's actual
independently qualified contract, never a value invented by this facade.

Plain Callback, Consumer, Disposable, MessageDefinition, ComposableNode,
BaseComposableNode, Timer, WallTimer, Context, Executor and NodeOptions source
signatures are retained from the pinned source. Node, Publisher and Subscription preserve the topic/timer profile. The separate
[Service/Future profile](services/README.md) adds complete selected service
values, two-alias endpoints and genuine deferred event completion. Its new source
has not yet executed; earlier topic/timer local results remain separate receipts.
Missing parameter, graph, Action, QoS-event and remaining generated-message APIs
are not success stubs. Native service availability still explicitly refuses95,
so the original client main is not an executed or complete acceptance route.
The NodeOptions overload, native RMW identity queries, extra contexts,
multithreaded executors and native pointer access explicitly throw before an
operation. These remain required work in `coverage.json`.

## Independent deployment binding, empty authority by default

The default Context uses a single selected Ros2Transport and an EMPTY mapping.
A Node is a local organizer, not a discovered native ROS node. Before calling
an unchanged main, a separate deployment bootstrap installs an immutable Profile
containing exact original node FQN, fully resolved topic, message type,
operation, QoS selection and alias. A missing local binding fails13 before an
open. A present local binding still cannot install or change a Host grant.
The Host must independently grant the alias for its exact native resource,
version, model, type identity, QoS and finite queue/rate/byte/deadline limits.

The topic/timer QoS subset retains its explicit `official-default-profile`
selection token. The additive service profile has a separate
`official-default-service-profile` token for the official SERVICES_DEFAULT path;
neither token invents or queries a native RMW profile. In upstream, defaultProfile queries the actual native RMW profile. This
migration target makes no guessed native query: the deployment must independently
qualify the selected native default profile against the actual Host alias.
The token selects that reviewed deployment contract; it cannot alter effective
QoS. `keepLast`, `sensorData`, system defaults and other native profile queries
are explicitly unavailable rather than silently accepted with another profile.
The included native witness uses explicit depth10/reliable/volatile QoS only as
its own test configuration. Root must prove that the selected deployment's
actual default contract equals it, or refuse that witness recipe and implement
a correctly qualified profile. No distro-wide equality is inferred.

`examples/OfficialBootstrap.java` installs aliases, then calls the actual original
main for one selected mode. The application source is not rewritten to use new
Node calls. Every mode is a fresh Guest with one lifecycle. The separate
DefaultDeniedBootstrap deliberately installs a LOCAL mapping but runs with EMPTY
Host grants, checks real ABI1/imports and requires actual open errno13 with no
owned handle. Guest-side missing-map13 alone is not real Host-denial evidence.

## Fixed ownership and recoverable failures

Before each open, the resource wrapper, WeakReference, cleanup record and exact
Node/global table positions exist. After Session.open returns, its actual owner
is committed with one plain field store. A subscription activation or timer
clock-read failure closes only that newly acquired owner once and removes the
exact reserved positions. All tables have explicit bounds; overflow fails before
another open or clock read. Session's own pending-capability/result-delivery
compensation remains the independently reviewed core implementation, including
its OpenCleanup diagnostic object.

Every callback/decode/poll pass leases the WHOLE Context. A callback may publish
through an existing owner, but cannot create resources, close another node or
alias, revoke an alias, shut down the Context or reenter an executor. This also
covers direct public Subscription.executeCallback/WallTimer.executeCallback.
The actual core Session is private, so a facade caller cannot accidentally
bypass the group lease using its close method. Revocation is an explicitly
selected migration helper, never an implicit response to a timeout.

All context/node retirement slots and failure exceptions are allocated before
cleanup effects. Every known owner is attempted in reverse global/node order,
even when an earlier close throws an Error. A close retires its local handle
before the import and is not replayed after110/107. Fixed CleanupAttempt slots
retain primary, actual result or failure and whether a close was attempted.
The context report also retains earlier retirement outcomes without making a
second native attempt. Supplementary suppressed exceptions are best effort;
self-suppression and allocation failures cannot replace an application/setup
primary. `Deployment.shutdown(primary)` is the launcher's finally helper.
Observe reports before starting another factory when longer retention is needed.

These are ordinary executable Java paths, not immunity to an unrecoverable VM
failure or a terminal Wasm trap. The outer actual Runtime owner must always
retain and drain its Guest/Host/native resources and preserve raw failures.

## Clock, phase and scheduling boundaries

Each WallTimer needs a separate `time_read_steady` alias bound to the original
node FQN. The constructor reads the actual eight-byte clock through the current
SDK. There is no System.nanoTime, Thread.sleep, hidden WASI clock, host callback
thread or default timer permission. The current timer profile accepts
nonnegative signed64 nanoseconds and checks period/conversion/phase overflow.
The phase advances past missed periods and invokes at most one callback per
pass; a backward clock starts a new phase. Caller supplied period changes and
cancel/reset remain guest-local over that explicitly granted clock.

SingleThreadedExecutor's explicit timeout0 is a bounded nonblocking round-robin
pass with at most one delivered callback. spinSome(0) polls every current source
at most once using fixed tables. This is a migration scheduling profile, not an
atomic native wait-set readiness snapshot. Positive native timeouts,
duration-bounded/exhaustive spin and multithreading refuse before polling.
Default/infinite spin requires an explicitly installed Driver. The Profile also
caps the total number of credits even if a custom Driver always returns true.
StdinCredits accepts only `R\n`, caps its reads, and ends at EOF/another byte.
These credits are caller-driven scheduling, not proof that ROS data is ready.
The native controller must pace credits independently of Guest counters.

## Concrete build and test staging

Only Root may execute the task's actual builds. First integrate this reviewed
new prefix with the exact current Java core, including the reviewed OpenCleanup
successor. Freeze the whole coherent source and retain its actual manifest;
older source-specific results do not cover these files. Copy the SDK layout
under an exclusive owned `/tmp` output. The POM's relative roots depend on that
layout. Admit actual Java/Maven/TeaVM0.13.1 tools and a private complete offline
Maven cache before starting a compiler. Do not fetch an arbitrary new dependency
or substitute a native JVM ROS bridge during a Wasm build.

The source recipe is:

```sh
# Under Root's current guarded Linux owner; one Maven process, no parallel build.
MAVEN_OPTS='-Xms128m -Xmx2g' mvn -o -T1 \
  -Dmaven.repo.local="$ACTUAL_PRIVATE_MAVEN_CACHE" \
  -Dmaven.test.skip=true -f "$SOURCE/sdk/compat/rcljava/pom.xml" package

# A separate fresh artifact exercises actual Host default denial.
MAVEN_OPTS='-Xms128m -Xmx2g' mvn -o -T1 \
  -Dmaven.repo.local="$ACTUAL_PRIVATE_MAVEN_CACHE" \
  -Dmaven.test.skip=true \
  -Dfixture.main=org.uwvm.rcljava.DefaultDeniedBootstrap \
  -Dfixture.file=rcljava-default-denied.wasm \
  -f "$SOURCE/sdk/compat/rcljava/pom.xml" package
```

The exact output tree must be fresh per build, including Maven target state.
Root supplies a reviewed staging copy/project directory for each command rather
than overwriting a previous artifact. All actual tools, inputs, cache closure,
logs, products, MD/native ELF/map proofs and normal owner finalization remain
required. Task Linux execution stays in the actual64GiB swap-zero leaf, selected
CPUs, Root's current16GiB aggregate and8GiB per-process ceilings, with one build
job and fresh memory/backing/output admission. These are requirements for the
future actual owner, not an execution result from this source package.

The linked target must be TeaVM WEBASSEMBLY_WASI/linear heap, never WasmGC.
Validate the complete actual Wasm and inspect all retained import signatures:

```sh
wasm-tools validate "$ACTUAL_WASM"
python3 -B "$SOURCE/sdk/tools/check_wasm_imports.py" "$ACTUAL_WASM" \
  --backend 2 --allow-module wasi_snapshot_preview1 --report "$NEW_IMPORT_REPORT"
```

Do not add `--require-all` to this typed entry unless its actual dedicated raw
fixture deliberately retains all43 functions. Source inventory completeness and
binary reachability are separate observations. JNI symbols or ROS1 fallback are
not allowed as alternate success routes.

For local JVM-only policy and source-compatibility tests, Root selects the actual
interop jar, constructs one explicit compiler response file containing current
SDK Java, this selected facade, both local test classes, the two bootstrap
classes and EXACTLY the ten original examples listed in coverage.json. Compile
with one javac process/`--release17`; run LocalPolicyTest and
OfficialSourceCompatTest in separate bounded JVM processes. Neither model
invokes Ros2Transport's actual imports. They are authored source until run, and
a JVM PASS proves neither target-Wasm buffer lifetime nor native ROS behavior.

## Real native peer and acceptance still required

`native/string_peer.cc` is a separate installed-SDK ROS2 witness. An actual Root
owner must compile it against the selected installed rclcpp/std_msgs, preserve
full MD/ELF/link/SDK identities and run it independently from the Guest.
`--observe /topic` validates the entire unchanged official publisher sequence
`Hello, world! N`; `--publish /topic` publishes the complete explicit Unicode
String. The Guest subscriber's original callback output must be associated with
the exact native publication, module/instance/owner and fresh live resources.
Its READY/publication markers do not prove delivery. Full final source/SDK,
raw values, cleanup and Host/Gateway/Guest observations are required.

`acceptance-plan.json` enumerates fresh default-denied, publisher, subscriber,
composition and timer cases with separate native observations, clock grants and
bounded credits. The original PublisherNotComposable depends on Thread.sleep;
the additive Service/Future producer explicitly compiles the unmodified original
server/client sources. The server route is implemented but unexecuted in this
successor; the full original client main still requires actual native availability
and returns95 at that boundary. It is not silently rewritten or counted as a
completed client route. Further rcljava versions, message families, actions,
parameters, graph, QoS events, remapping, native wait sets and distro/vendor
matrices remain required. No source/marker/model count completes those tasks.
