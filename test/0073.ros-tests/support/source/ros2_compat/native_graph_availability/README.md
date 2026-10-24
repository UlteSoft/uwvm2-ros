# Actual retained-client graph-event availability

This is a source-only successor to the integrated direct-RCL service Registry
and private Gateway route. No configure, compiler, probe, model, Native, Guest,
ROS or SSH command has run for this cohort. Nothing here establishes a selected
SDK/RMW's runtime behavior or full upstream application compatibility.

The existing twelve service-import tuples, UMRC bytes, the original81 ABI,
permission operations, event masks and native/canonical identities are unchanged.
The internal Native C++ endpoint interface gains an additive virtual method and
admission tail: ALL Native Registry/factory/adapter consumers must source-recompile
coherently. No old binary vtable/layout compatibility is asserted. An external
factory that does not implement real positive wait retains the default95 path.
A positive `service.available` request remains separately authorized from its
original `service.call` owner. It now has a real implementation when the selected
original SDK passes an additional production compile/link gate. Missing support
returns95. It does not add graph inspection or management authority to a Guest.

## Exact original API basis

The primary DATA package binds original ros2/rclcpp commit
`53fa7aeea86e8ed484f9465fa14059faf13a71f1` and actual Git tree
`8ba691f837b5c0368b0d6d8b3900e7b111d9257c`, with complete original bodies and
verified Git blob hashes. The source paths are:

- `rclcpp/src/rclcpp/client.cpp`, ClientBase::service_is_ready and
  ClientBase::wait_for_service_nanoseconds. The former calls
  `rcl_service_server_is_available` with its original retained RCL client. The
  latter reuses a registered graph event, waits with the real NodeGraph method,
  clears the event, rechecks that native predicate even without a graph event,
  and computes remaining time from the real steady clock. Its100ms bound also
  handles the documented Connext notification/readiness race.
- `rclcpp/src/rclcpp/node_interfaces/node_graph.cpp` and its public interface.
  `get_graph_event` registers a weak loan and lazily starts the original graph
  listener. `wait_for_graph_change` verifies that the event belongs to that
  Node and uses its actual condition-variable timed wait; context shutdown is
  in the wait predicate. Expired weak loans are pruned only on graph changes.
- `rclcpp/include/rclcpp/event.hpp` and `rclcpp/src/rclcpp/event.cpp`. Clearing is
  an actual atomic exchange, not a readiness result.
- Original `graph_listener.cpp` and `graph_listener.hpp`: one listener belongs
  to the actual Context, uses its actual RCL wait set and guard conditions,
  and owns a real native thread plus original shutdown/join handling.

Those primary bodies are not SDK substitutes or proof that another installed
version has the same ABI. The new production probe compiles AND links the real
Node get/wait, Event clear, Context path, RCL predicate and Linux poll symbols,
retaining their addresses with volatile pointers. No version spelling, fabricated
header, `try_run`, future pin or externally set feature result enables the path.
The original service/QoS/availability probe bodies and their preexisting target-type
semantics are retained. Only the NEW graph probe sets
`CMAKE_TRY_COMPILE_TARGET_TYPE=EXECUTABLE` inside its function scope, after the
old two checks. A caller toolchain's STATIC_LIBRARY shortcut cannot stand in
for this new link proof; caller state is restored when the function returns.
Feature0 omits optional NodeBase/Context/utilities/Event includes and calls,
and its actual `check` branch returns95. It does not reference an absent API
and hope a runtime refusal can repair a compile error.

## One actual Node domain, no replacement client

A `native_rcl_graph_wait::domain` is allocated only for the explicit existing
owned-service profile. It retains the exact actual Node. Merely allocating it
registers no event and starts no listener thread. The Node retains all original
NodeOptions behavior; this cohort adds no quiet/default-endpoint suppression.

Only an independently authorized positive request whose first SAME-client RCL
predicate was false may register the domain's event. All current source/peer
and independent query grants, actual QoS/GID association, and native lifetime
must already be validated. No new RCL/rclcpp Client, Service, Publisher or DDS
participant is created for the query. Starting the SDK's GraphListener does use
its genuine infrastructure thread/wait set/guard conditions. These resources
need Root's actual process, memory, mapping and shutdown admission; they are not
free or a Wasm Threads implementation.

Exactly one event is kept for this Node/profile's whole lifetime, including
across retired endpoints and reconnects. A per-query or per-session event could
otherwise leave arbitrarily many expired weak loans on a quiet NodeGraph.
Concurrent waits may clear the shared event and miss an immediate notification.
That cannot decide readiness: every wait is at most a real100ms slice and every
wake/timeout rechecks the same retained RCL client. This bounds the resulting
notification delay by that slice, subject to actual scheduling/foreign API cost.

The event shared_ptr has exactly one initialization writer. An acquire/release
phase publishes it immutably to all readers. A simultaneous first preparation
returns17, preserving the existing busy/error semantics; no thread waits on an
unbounded domain initialization mutex. Registration attempt storage exists
before the original SDK call. Any allocation/start/registration exception
permanently poisons that same domain with its independent first error. There
is no retry through unknown partial original NodeGraph state and no assertion
that cleanup succeeded. Partial infrastructure remains owned by the retained
actual Node/Context and is reclaimed only through their real lifecycle.

The event is not reset during individual endpoint retirement. Domain destruction
occurs only after its borrowed synchronous calls are done. Original NodeGraph
removal, Context shutdown and GraphListener join remain original SDK operations.
Terminal Gateway audit records the domain counters/error after session workers
quiesce. Concurrent counter observations are monotonic facts, not an atomic
transactional snapshot or a substitute for terminal raw evidence.

## True time and cancellation

Zero duration calls the old `available` method unchanged. It neither requires a
graph domain nor registers an event. Positive duration is at most the actual
independent grant's `max-wait-ns` and60e9. The requested deadline is computed
once from real `steady_clock` time. The transport deadline is independently
bounded by the actual RPC deadline and query grant's `timeout-ms`.

Before and after the actual native getters, predicate, event registration and
real graph wait, the implementation checks:

1. The borrowed connected native session socket for HUP/RDHUP/ERR/NVAL, without
   reading it or consuming queued requests.
2. The actual pinned worker pidfd for lifetime termination.
3. The actual Gateway's stable lock-free atomic signal stop flag and the real Node Context.
4. The real transport deadline.

The connected socket, worker pidfd, stop pointer and Node-domain pointer are
Native-only admission facts. They never appear in UMRC, a Guest struct, aliases
or numeric handles. The authenticated server lends them for the whole synchronous
call; the implementation neither closes nor adopts descriptors. They cannot
manufacture capabilities. The shared flag is `std::atomic<std::sig_atomic_t>{0}`,
not volatile: connection workers and the main/pidfd path are native C++ threads.
The actual Linux probe and production static assertions require
`is_always_lock_free`. Signal handlers perform only a relaxed store; all main,
worker and wait reads use relaxed loads. The flag carries only cancellation,
not publication of any other mutable data. Initialization precedes handler
installation and the borrowed atomic remains alive until all workers join.
No locking library fallback or unsafe volatile variant is supplied. Common
server and both ROS Gateway consumers must source-recompile with the new
pointer type; this is not a claim of old private C++ layout compatibility.
The separately retained C++17 n4659 source permits these plain lock-free atomic
operations in signal handlers; actual selected-platform qualification is still
pending. A direct Native caller must meet the same lifetime
contract; absent native facts return95 for positive waiting.

Each timed wait uses the lesser of the real requested remaining time, real
transport remaining time and100ms. Slice count is never time. There is no sleep,
request send, callback spin, graph-name match, new probe client or fake ready hook.
A final false SAME-client native predicate after the requested deadline is the
only elapsed-unavailable success (`0,false`). A real ready predicate yields
`0,true`. RPC expiry is110, disconnect/dead worker107, Context/stop cancellation125,
other interrupted poll4 and genuine unsupported APIs95. All errors preserve the
caller's bool and wire output. Error/unknown effects are not permission to replay.
The existing Host holds all three real leases through the whole operation.

Timed CV waiting is real but does not hard-preempt a foreign RMW getter/query,
NodeGraph lock, thread startup or SDK destructor. Post-return time checks do not
make those calls interruptible. Root must retain its independent finite native
process deadline and once-only retirement evidence. Callback threads and original
SDK shutdown must actually retire before a whole execution can pass.

## Authored source contracts

`value_contract.cc` has13 unrun groups (14 in the forced feature0 target) using actual production helpers. It covers
requested/transport/slice bounds, absent admission, actual Linux socketpair and
self-pidfd liveness, ignored socket data with an unchanged peeked byte, actual
peer HUP, stale descriptors, stop and real expired deadlines, plus an actual
native-thread atomic store/production cancellation read. A separately compiled
forced-feature0 target uses the same real SDK and same production header to
assert the explicit95 branch. Only our generated feature switch is forced;
there are no fake ROS headers or capability results. It supplies no fake
ROS entity or time provider and makes no Native ROS credit claim.

`native_contract.cc` is a real selected-SDK component for either actual Empty or
AddTwoInts. It loads the genuine14-field trusted profile and warms the complete
actual11-identity CPP Registry; there is no published fixture/default hash.
It creates exactly one direct original RCL client over real generated types.
Its logical full64 owner/incarnation values are explicitly Native fixture DATA,
not actual Host handle leases or a Guest authorization proof.

Required CLI:

```
native-graph-wait-contract --owned-native-graph-wait ACTUAL_PROFILE \
  empty|add /ACTUAL_ISOLATED_RESOURCE \
  absent|ready|disconnect|stop|transport ACTUAL_INHERITED_SOCKET_FD ACTUAL_PEER_PID
```

Root must independently admit the real inherited connected UNIX stream socket
and its actual peer process. The witness checks socket type, UNIX connection,
same UID and the actual SO_PEERCRED PID, then opens that actual pidfd. Root owns
both ends/processes and all descriptors. It keeps stdin open, waits for the
`NATIVE_GRAPH_WAIT_READY` marker, and supplies exactly one `G` byte to start.

Initially the selected resource must be genuinely absent. The witness first
checks a real zero-duration false query and confirms zero graph registration,
then checks wrong-source and absent-domain refusals without registration.

- `absent`: no service is started. A120ms real requested deadline must elapse
  with an actual false native predicate and result0/false.
- `ready`: after `G`, Root starts the separately owned real original service
  peer after a deliberate controlled delay. The source witness requires at
  least one REAL graph wait, so an immediate discovery race cannot fake coverage.
- `disconnect`: after `G`, Root closes its actual session peer (or retires the
  actual pinned peer worker), and the witness requires107 with bool unchanged.
- `stop`: after `G`, Root sends SIGUSR1 only to this owned witness. Its minimal
  signal handler performs only a relaxed store to the actual always-lock-free atomic stop flag; the actual wait must return125.
- `transport`: no service is started. Requested wait is2s but actual transport
  allowance is25ms; the outcome must be110 with bool unchanged, never false.

Every mode checks actual client init happened once, actual readiness calls were
made, no business send/take/respond occurred, one event registration occurred,
and actual fini occurred exactly once. Original NodeOptions defaults stay in
the Native witness. Successful Context shutdown is checked explicitly. These
are unrun source assertions, not observed results. A separate real server's
handler-call counter must also be captured as zero during future qualification;
local client counters alone do not prove another process's callback count.

The explicit OFF-by-default CMake project only builds when Root later selects
`UWVM_BUILD_NATIVE_GRAPH_AVAILABILITY_COMPONENT=ON` and an absolute complete
`UWVM_NATIVE_GRAPH_WAIT_ROOT`. It requires actual original packages/targets,
full CPP codecs, OpenSSL, Threads, vendored fast_io and production feature probe.
It registers no automatic tests, probe execution or native server startup.
All configured compiler/link commands and complete MDs must be captured later.

## Remaining real qualification

No platform or SDK/RMW is accepted by this source package. Root still needs the
actual complete-source freeze and selected SDK feature/link/MD evidence, both
families in each witness mode, actual worker death and Context shutdown faults,
shared-event simultaneous clients/cross-session wake-clear races, registration
failure/partial-start poisoning, actual Native thread/mapping/memory admission,
and once-only teardown under the external hard deadline. End-to-end default-deny,
revoke/close during waits, Host triple leases, original upstream client APIs and
Guest Wasm behavior require their own actual complete integration runs.
