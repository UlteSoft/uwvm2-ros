# Historical ROS communication sandbox fixture

Production VM configuration now uses [external communication attachment](ros-communication-attachment.md).
The launcher and sandbox described below remain test-deployment components.
Their process lifecycle operations are not VM capabilities. Historical receipts
must not be used to qualify the current attachment implementation.

The communication worker is a separate native provider for one explicitly
approved node and its participant. It does not expose a socket API. A logical
`context.open` creates no connected transport. The node's complete context and
node authority group must remain valid before the supervisor transfers traffic
pipes. Model worker syscall policy is unchanged and remains independently closed.

## Startup and authority boundary

1. Trusted deployment selects a finite image manifest, launcher digest, complete
   backend/runtime closure, immutable configuration profile and finite limits.
   Every source is copied into a sealed memfd and its full SHA-256 verified.
   Runtime images must be ELF; configuration and ament/XML resource files use
   the nonexecutable configuration role. The budget reserves both the sealed
   image and bubblewrap's read-only copy. No child starts during `prepare`.
2. `launch` enters fresh user, PID, network, IPC, UTS and cgroup namespaces while
   retaining the existing 64 GiB zero-swap memory cgroup. It drops all capabilities,
   clears ambient environment and supplies only the sealed image closure. The
   trusted launcher reports the blocked namespace-init PID; `identify` obtains
   and verifies its live pidfd before allowing bootstrap execution.
3. Backend `prepare` receives trusted profile bytes. It creates the sole approved
   ROS node, participant, executor, all transport pumps and approved endpoint/codecs
   needed during operation. Traffic gates stay closed. No broker/network descriptor
   exists in the worker during this phase. Socket operations are already denied.
   Any startup discovery data must fit a separate finite local reservation.
4. A first `SECCOMP_FILTER_FLAG_TSYNC` filter covers every existing thread and
   denies new threads, file opens, exec and executable mappings. With the descriptor
   table now frozen, bootstrap verifies capabilities, no-new-privileges, seccomp,
   PID namespace init identity, finite live thread count and the entire FD closure.
   Apart from stdio/control and its temporary private proc inspection descriptors,
   only declared nonblocking eventfds, eventpolls and anonymous notification pipes
   are accepted. A retained regular file, socket or undeclared FD rejects startup.
   Bootstrap also reads its own actual `/proc/self/maps`: writable executable and
   anonymous executable mappings are refused. File-backed executable maps must
   match the device/inode of a full-digest-verified, read-only mounted ELF image;
   a matching path string alone is insufficient. Only kernel vdso/vsyscall maps
   are exempt from file identity. `READ_IMPLIES_EXEC` personality is refused, and
   personality changes are unavailable. `mremap` is permanently disabled from the
   first fence, so an existing unsafe executable region cannot move behind the
   mapping reader's cursor. No new executable map can appear after that fence.
5. Exact `ready` framing permits the supervisor to transfer four distinct,
   nonblocking anonymous pipe ends through receive-only control FD 3. Their fixed
   order is discovery receive, discovery transmit, user receive, user transmit.
   Both sides check type, kernel access mode, `O_NONBLOCK` and distinct pipe identity;
   supervisor duplicates descriptors before checking and transferring them.
6. Bootstrap closes control/proc descriptors and installs the second all-thread
   filter. It grants read/write only to the exact approved FD directions and local
   notification descriptors. It then calls backend `activate`, which may only
   attach the four ends and release existing gates. The broker remains idle until
   exact `attached` framing is received. Application stdout framing starts afterward.

Real middleware that creates another thread, opens a resource or lazily loads a
library after confinement is unsupported by that profile and must fail deployment
or the operation; the sandbox is not relaxed to make it pass. Approved runtime
image closure and native middleware behavior require a separate real ROS test.
The two-thread fixture alone does not qualify arbitrary FastDDS/ROS packages.

## Configuration and limits

`communication_sandbox::manifest` binds launcher, destination/length/SHA-256 of
each image, immutable profile identity/digest, explicit ROS prefix and limits.
All owned container allocation uses recoverable fast_io adapters. Invalid or
incomplete profiles fail closed. Source path policy is trusted host configuration;
an upstream `O_NOFOLLOW` open only constrains its final component. Immutable
copies and full digests bind the bytes actually executed and consumed.

Image/address/file bytes, CPU seconds, open-file count, scratch reservation,
request/response size, stderr budget, startup deadline, polling interval,
supervisor reservation, profile bytes and thread count are all explicit. The
scratch filesystem has a finite byte limit and no writable slots or creation
syscalls. Initialization may create pthreads only with the required shared thread
group flags; clone3's opaque argument block is rejected. The live thread count is
checked after the first TSYNC fence, and future creation is denied. This is **not**
a kernel-enforced peak thread quota during trusted initialization. Address/CPU
hard limits and the existing cgroup still apply during that phase.

`process` enforces its startup deadline during identity, readiness and activation.
Its owner must enforce the configured RPC and cumulative stderr budgets, retain
cleanup debt after cancellation, and prevent publication until every startup
check succeeds. There is no automatic rate limiting: operational authority
and finite capacity reservations are distinct. Request stop is idempotent and
uses retained pidfds; `reap` never substitutes elapsed time for observed exit.
The owner joins every I/O user before destroying the process.
Confirmed cleanup requires `cleanup_confirmed()`. Before the execution permit
has been sent, the forked child can only wait at its private gate or exit. After
positive reaping, that rejected child can release its reservation without
inventing a namespace identity. Once the permit has been sent, cleanup requires
both launcher reap and observed exit through the pinned namespace-init pidfd.
An early setup failure without that identity retains cleanup debt indefinitely.
A timeout, cancellation or launcher exit alone does not discharge that debt.

All child and parent nonblocking setup precedes fork. A one-use Unix
sequenced-packet gate transfers an eight-byte permit only after the parent has
published pidfd ownership and checked cancellation. Failure shuts down the gate
write side, including fork-inherited duplicates. The child closes its inherited
parent endpoint and rejects EOF before exec. `MSG_NOSIGNAL` prevents a refused
permit send from terminating the VM. Failure never falls back to a numeric-PID
signal or a wait under authority. The stage retains its first failure; retries
cannot relaunch it or reopen a retired or uncertain-cleanup state.

The fixed stdio/control channels are internal supervisor protocols, not guest
file or network authority. The backend cannot create sockets, connect, sendto,
sendmsg, obtain arbitrary FDs, inspect another process, execute a plugin, create
executable mappings or reopen files after confinement. `poll`/`ppoll` and exact
eventpoll descriptors permit bounded wake-up without granting network access.
The inspectable direct mmap ABI is checked for executable protection; i386/ARM
legacy pointer-mmap is rejected because seccomp cannot inspect its pointee.

Both the supervisor and worker ends of protocol stdio are nonblocking. Bootstrap
independently verifies the worker's stdin/stdout/stderr pipe types, directions and
`O_NONBLOCK` before backend initialization. A backend must handle `EAGAIN` with
bounded polling: idle input may wait without expiring a valid unused authority,
but once a frame begins its fixed deadline must include every later partial
read/write. Stderr writes must also remain nonblocking and bounded. The worker
has no `F_SETFL` permission to clear this property. Control FD 3 waits only within
the supervised launch-to-attachment deadline.

## Qualification

The communication launch-cleanup cohort uses VM SHA256
`13ee3655695e8ec6af98d4cad83ecc76c99ec18bef089546eb2dcda1537c65f5`.
`communication-launch-cleanup-qualification.json` binds 768 bounded VM cases to
that exact binary, including 62 communication/CLI cases, 24 new launch-refusal
cases and 128 scalar MuJoCo cases. Runtime and source hashes, command logs and
unchanged cgroup event counters are retained in the SDK `.tmp` evidence tree.
These cases do not establish arbitrary vendor ABI support or physical safety.

`communication-launch-failures-parent-setup/results.json` uses five distinct
kernel fault modes: pidfd refusal, permit-send refusal, both, first child-end
nonblocking setup and first parent-end nonblocking setup. A wrapper installs
the real kernel refusal at the fourth `F_SETFL`; it does not mock process
creation. The old source fails all five. Corrected Clang ASan/UBSan and GCC
pass them and the positive pre-launch cancellation control. The checks require
no endpoint publication, retained first cause, positive reap, released capacity
only with proof, inert retries and no child remaining. The normal sandbox,
allocation rollback, stage and actual sealed ROS deployment suites also pass.

The actual VM fault runner uses an inherited seccomp filter and an orphan
subreaper. All 24 interpreter/AOT, wasm32/64 and memory-index cases require that
no descendant remains after VM exit. Request descriptions retain the ledger's
conservative unknown effect even when process cleanup is proven: this repair
does not silently upgrade an effect observation to certainty.


Run `tools/ros_scene/communication_worker_checks.py` only through
`tools/ros_scene/in_existing_cgroup.py`. It freezes the transitive source closure,
hashes launcher/runtime images and builds with `-O3 -g`. Clang ASan/UBSan and GCC
execute the real kernel filters and actual isolated threaded fixture. Tests cover
the two-phase handshake, exact role ordering, reverse direction, reused pipes,
blocking/regular-file rejection, 14 probes on each of three existing threads plus
two main-thread notification/completion checks,
undeclared/false/duplicate infrastructure descriptors, too many startup threads,
deliberate RWX/anonymous executable/unapproved configuration-file executable maps,
startup timeout, malformed readiness, failed activation, live revocation and
pidfd cleanup. Real malloc/calloc/realloc failure injection verifies recoverable
fast_io allocation rollback without descriptor leaks before process publication.

These receipts are engineering evidence for the named fixture and profile. They
are not industrial safety certification, a native-performance result or evidence
that every ROS package has a valid sealed startup closure.

## Private native peer transport

`communication_transport::private_link` is the production route primitive for
two exact sealed workers. It opens no network socket. It retains both verified
namespace-init pidfds and both complete startup profile hashes, checks reciprocal
participant/direction policies, and attaches four one-way anonymous pipes to each
worker. A port, DDS GUID, process number or UDP source is never authentication.
Both workers must complete attachment before explicit bilateral activation.
Discovery and user traffic have separate bounded queues. Optional limits apply only to an already admitted direction; absent limits preserve its stable
scope, explicit zero drops that direction and reports uncertainty. Malformed or
replayed framing, partial-frame timeout, queue exhaustion or either process exit
fences the pair. Already forwarded effects are not rolled back.

The actual Jazzy/FastDDS qualification in
`communication-private-ros2-fifth/results.json` under the SDK `.tmp/ros-scene-security`
uses two independent sealed native workers with 107 approved runtime images per
side. Both sides exchange String/Bool topics, SetBool requests/responses and
LookupTransform action goals/results. Their graphs are mutually invisible before
activation and visible after bilateral attachment and activation. Both six-part
result sets passed, all four logical routes forwarded, and peer death fenced the
link through its retained pidfd. These are real native DDS semantics over the
private pipes, not an in-memory middleware fixture or an authenticated-UDP claim.
The report explicitly records reuse of the successful `-O3 -g` compiler outputs
from the preceding attempt after rechecking their complete frozen C++ closure,
compiler and external dependencies. The earlier graph-name sorting failure and
the separate original packed-image integrity incident remain preserved.

The earlier `communication-native-rpc-qos/results.json` also passes the same six
directions against a controlled native UDP test peer. That test is compatibility
evidence only. Its explicit action-status QoS is transient-local depth one;
the runtime does not silently replace an incompatible deployment QoS. RPC frame
sequence and each resource's mutation sequence are independent: reads and calls
on another resource cannot consume an endpoint's exact-retry mutation key.

## Staged factory and retirement contract

`communication_worker::paired_factory` implements the trusted backend's context
and node factory with `communication::provider_lifetime`. Its two manifests,
node/context plans, RPC capacities, retirement wait and bilateral admission
callback freeze at configuration. Configuration verifies profile bytes and seals
the closure but starts no process. `create_context` retains only a logical private
context. `prepare_node` requires the exact caller incarnation, node/context
capabilities, context resource, full request digest and retained context object;
it launches once and advances bounded startup work with `again`. Its `ok` means
both exact workers are attached, while transport still remains idle.

Each `create_node` runs under the dispatcher's fresh complete admission. Once both
nodes commit, the mandatory trusted callback checks both current origin groups in
the same authority environment. Two recursive `group_lease` objects remain held
through link activation; capability closure must still reject activation even if
both contexts and pidfds remain live. Cross-environment pairs are rejected by this
factory and require a separately authorized bridge path. The first node may be
retained privately while the second node is not yet committed.

A node grant publishes node discovery metadata only. The native worker warms
closed codecs before confinement, then destroys warm-up endpoints before readiness.
Manifest entries are a static permitted set, not guest authorization. Actual
topic/service/action endpoints require their own independently admitted open RPC.
The current factory deliberately reports unsupported for endpoint creation until
its corresponding host adapter is qualified; the private native RPC test alone
does not establish complete VM-to-native integration for all 30 operations.

Graph queries now attach through this factory using a counted, actual host node
lease. `peer_configuration::graph_query_bytes` is an explicit aggregate payload
reservation; its default zero rejects attachment. Each sealed graph binding
reserves two complete request/reply pairs for queue ownership and detached
encoding/decoding. Filters, profiles, bounds, context identity and node ownership
must match the immutable manifest. Repeating the same unpublished attachment
returns its retained source; changing the policy cannot replace it. Reservations
remain owned through provider retirement, including unknown results.

The trusted supervisor calls `pump_graphs()` outside registry and authority
locks. It advances nonblocking IPC on the peer's single sequenced transport,
without adding a thread or waiting for a reply under admission. Every send and
delivery validates the current caller and all four graph origins. Stop fences
new attachment and transport; retirement drains graph queues before releasing
the transport's borrowed descriptors. A node grant does not authorize a query.

`communication-factory-graph-checked/results.json` qualifies this path against
the real sealed native pair with Clang ASan/UBSan and GCC, both at `-O3 -g`.
It covers zero query capacity, real registry node leases, exact attachment retry,
changed-policy rejection, native discovery, original-context-capability closure,
and confirmed worker cleanup. This is provider qualification, not VM import or
CLI qualification for the communication catalogue.

The broker retains `provider_lifetime` before exposing its dispatchers. On teardown
it first calls `request_stop`, then calls `retire` outside authority and registry
locks, and only then destroys the dispatchers. Stop fences both RPC channels and
the private link before signalling both exact workers. Retirement joins the relay,
drains every borrowed RPC-descriptor user, and checks actual namespace-init exit
and launcher reap. `cleanup_confirmed()==false` retains explicit worker capacity
debt. A bounded retirement wait, early launcher exit, or object destruction cannot
turn unknown cleanup into a successful receipt or authorize a replacement worker.

The current factory qualification is
`communication-worker-provider-concurrent-final/results.json`: Clang ASan/UBSan
and GCC both run the actual sealed native pair with `-O3 -g -fno-rtti`. A real
second thread pauses in bilateral admission while the revoker holds authority,
inspects the factory and revokes the first node. This verifies that the factory
releases its internal mutex before acquiring authority, then reacquires it in
authority-to-factory order and rechecks the same stages before activation. No
provider lock is held while the callback acquires its two authority leases.
The stage and private-link allocation cohorts each pass eight commands; their
actual recoverable fast_io fault points are three before fork and twelve during
link preparation, with no leaked descriptors or accidental worker publication.

## Asynchronous endpoint creation

The development carrier for `node.open`, `topic.open`, `service.open` and
`action.open` is their complete semantic body followed by `creation_key:u64` and
`absolute_steady_deadline:u64`. Both must be nonzero and the deadline must fit
the finite deployment bound. The new contract requires this trailer; it does not
accept legacy open bodies. `context.open` can stay synchronous because it creates
only a logical private handle. Default zero authority still rejects all opens.

`communication_creation::ledger` reserves two finite canonical carrier slots per
immutable binding: a current receipt and a tentative next request. Reservation
owns the complete input before creation admission is charged. A successful commit
under the full origin group queues exactly one request. An identical key/carrier
returns the retained request; changing the caller incarnation, resource origins,
deadline or any byte rejects the retry. Keys advance exactly by one. The previous
released key is acknowledged only when the next reservation commits, so failure
of the next admission does not erase its predecessor's recovery information.
Commit compares the complete caller, ordered capability/action/claim-role tuple
and canonical-carrier-plus-48-byte-receipt cost, and consumes the charged group
exactly once. An equally sized unrelated group, validation-only preflight, changed
role, closed origin or previously consumed charge cannot publish a creation.
Begin, send and publication also match the complete current group. The managed
`enqueue` path reserves before invoking its trusted resource-validation/charging
callback, rejects concurrent/reentrant submissions with `again`, and does not
charge an accepted exact retry. Low-level methods cannot adopt its reservation.

The 48-byte open/cancel receipt is `[key,request,state,status,effect,cleanup_debt]`.
The request identity is separate from an endpoint handle. A known native result
first becomes private `ready`; fresh full admission and actual endpoint ownership
registration then produce `published`. `communication.open.take` returns the
64-byte `[receipt,endpoint_kind,handle]`. Copy failure leaves that exact result
available. `communication.open.cancel` requires its own exact action grant and
does not reuse or refund the original creation-rate budget.

Shared `request.describe` retains the common carrier
`[request,state:text,effect:text,provider_generation,reason:text]`, where generation
is the actual worker incarnation. Shared `request.release` returns its normal
8-byte boolean. Neither shared operation changes to the fixed-size open receipt.
Terminal unknown outcomes remain `outcome_unknown`/`unknown`; confirming process
cleanup does not fabricate a known native outcome. Unknown or unresolved native
cleanup debt refuses release and binding reuse. Cancelled unsent work can retain
an exact `none` effect, while an already written request cannot.

`communication_worker_async::transport` preallocates request/reply storage with
recoverable fast_io. Stage copies an owned frame without I/O. Send performs one
nonblocking pipe write while holding the current complete group; a full pipe
returns `again`, with no response wait or sleep under authority. Further sends
validate the same origins without charging creation again. Receive advances
bounded nonblocking reads outside the authority lease. The ledger records actual
sent bytes and cleanup debt under the same lock as the write, so cancellation
cannot occur between a native effect and its debt update. A failed or partially
written channel is fenced and cannot resend a potentially effective open.

`communication-creation-canonical/results.json` passes nine commands covering
Clang ASan/UBSan, GCC and TSan, including real concurrent send/cancel races,
changed retries, lost-result recovery, typed publication and shared receipt wire.
It is a transport/ledger fixture. Production VM endpoint adapters and arbitrary
message types require their own integration qualification. The pinned native
provider used in that historical receipt has seven closed codecs. Current
trusted builds can add [bounded generated topic adapters](ros-bounded-native-topics.md);
their native shape/hash checks and isolated-worker receipts are separate from
that earlier fixture. Generic service/action schemas remain outstanding.


## Owned operation queues and deferred state

`communication-creation-native-second/results.json` verifies sixteen explicit
asynchronous endpoint opens across two sealed native workers, followed by actual
bidirectional String/Bool topics, SetBool services and LookupTransform actions.
Each worker uses a hash-verified 107-image closure. Endpoint publication handles
in this test are fixture-owned; this receipt does not qualify real VM adoption.

`communication_jobs::queue` provides bounded per-endpoint request, response and
receipt storage. `configure` freezes the exact owner, endpoint handle, worker
binding/incarnation, slot counts, byte capacities and finite deadline bound.
All backing storage uses the recoverable fast_io allocator; an allocation failure
returns `no_memory` without publishing a usable queue. The ordinary terminating
fast_io allocation path is not selected for these deployment-owned reservations.

`reserve` copies the complete canonical request and the actual per-call caller,
capability and resource-origin chain before the first charge. `commit` requires
the complete current charged group, matched by caller incarnation, ordered
capabilities, exact actions, claim roles and byte costs. A matching count or an
unrelated live group is insufficient. The group retains grant-owned action text
and rechecks current membership and every handle, including after a recursive
same-thread close or revoke. It is usable only on its acquiring native thread.
Publication consumes its charged admission once; preflight and validation-only
groups cannot commit, and one charge cannot publish two different requests.

The managed `enqueue` entry serializes reservation, charging and publication
without waiting for another submitter. A concurrent or reentrant submit returns
`again`. Accepted exact retries return the same receipt without calling the
charging callback again. Low-level reserve/commit/abandon calls cannot take over
a managed reservation. An admission failure discards only the tentative owned
request. An explicit zero operation quota therefore cannot send an RPC byte.

`communication_worker::mutations` supplies closed typed encoders for topic
publication, liveliness, acknowledgement, service submission/response/abandonment,
action submission/transition/cancellation and context shutdown. Configuration
reserves the complete encoder buffer in addition to every queue request/reply
slot. Mutation calls perform no allocation, native I/O or reply wait. An exact
action transition must also match the token's secondary permission. Native
correlations are supplied only after the owning adapter has resolved them within
the same worker incarnation; the encoder does not treat a host ID as a native ID.

Topic, service and action cores freeze the trusted adapter's accounting mode at
creation. A queue-managed mutation retains full uncharged validation in the outer
dispatcher and charges once at queue publication. Ordinary synchronous providers
retain dispatcher charging. Receipt recovery can decode bounded input after the
last permitted mutation consumed its quota, and queue-managed exact retries do
not consume another operation. This does not authorize another effect: changed
retries reject, and a new mutation must obtain its own charged admission.

Topic and service cores retain pending keyed mutations until the exact host
request and actual worker generation settle. A later key cannot overwrite that
evidence. The supervisor retries a completion that races the submitting call;
identical settled results are inert and changed duplicates reject. Action
completion also checks its pending worker incarnation and preserves the original
state on known no-effect failure. The last recoverable receipt is updated to the
actual result. A successful RPC dispatch returning a receipt does not replace
that receipt's numeric native error with success.

The queue's primary byte cost is the complete request body, the reserved maximum
reply body and both 48-byte RPC envelopes. It is reserved before publication;
unused reply allowance is not refunded. Node/context/schema dependencies are
validation-only. An action response's transition-specific grant-only permission
adds a work unit without duplicating its byte cost. Pumping and copying use
zero-byte, uncharged revalidation. The integrating dispatcher must use this one
accounting boundary rather than independently charging the same operation twice.

Exact retry returns the retained host request
without enqueueing or charging again. Forgotten host request IDs and mutation
keys are rejected by permanent high-water marks. Explicit release can permit a
later unique request to reuse a finite slot; unknown outcomes cannot release.
Neither a worker-native correlation nor an action UUID is a host request ID.
Communication resources, action goals and operation receipts now use the shared
ROSv2 identity allocator so shared request routing cannot confuse those domains.

The closed token includes primary, node, context and every schema origin. An
`action.respond` also includes its exact transition-specific permission, for
seven origins in total. Missing context or schema origins, nonempty unused slots,
wrong endpoint kind, substituted caller/capability and an operation/token mismatch
are rejected. These tokens are constructed by trusted dispatch code, never read
as authority from guest memory, and retain immutable origin ownership through
stop/drain. The provider may not reuse the first opener's authority for later
publications, takes or action decisions.

`pump` asks its trusted callback to revalidate the full original token while
holding no queue mutex. It then rechecks the same request and performs at most one
nonblocking pipe write under the returned group. The group is released before
bounded reply reads. `copy` requires fresh full admission and copies a retained
complete result, leaving failed guest delivery recoverable. `completion` detaches
trusted bookkeeping notifications, which the owner applies after releasing all
queue and transport locks. Queue acceptance means only `locally_accepted`; it is
not an acknowledgement that native ROS accepted or completed the operation.
The supervisor overload of `completion` copies its complete body into separately
reserved host storage before consuming the notification. A short destination
leaves it pending. This permits correlation reconciliation after revocation; it
does not grant guest access to those bytes. Public copies still require fresh
complete authority. No guest callback runs under the queue lock.

A queued expiry or cancellation before any pipe byte is written is known `none`.
A partial write, lost reply or worker death after a write remains `unknown`; the
exact request and cleanup debt survive cancellation, revocation and teardown.
A complete reply already retained by the transport wins a later cancellation.
Positive namespace cleanup evidence may settle lifetime debt, but cannot invent
a native outcome or make an unknown receipt releasable. Queue cancellation is a
separate control path and does not consume the operation's optional traffic limit.

`communication-jobs-third/results.json` passes nine commands with `-O3 -g`:
Clang ASan/UBSan, GCC 13 and TSan. It exercises real nonblocking pipes, reentrant
admission, complete origin checks, exact retry, stale-ID rejection, cap closure,
expiry, twelve actual concurrent pump/cancel races and unreleasable unknown debt.
This is a queue/transport qualification, not VM endpoint registration or general
ROS type support.

The subsequent `communication-jobs-exact-admission-first/results.json` passes
nine dependency/compile/execution commands with Clang ASan/UBSan, GCC 13 and
TSan under `-O3 -g`. Added negative cases substitute an enrolled caller or caller
generation, a sibling capability, another permitted action, dependency order,
validation roles and byte costs. They also cover uncharged/preflight publication,
charge reuse, temporary action-string lifetime, move/release, cross-thread use,
recursive dependency close, alias revoke, caller retirement and whole-authority
retirement. Invalid pump groups write zero bytes; invalid delivery groups leave
the output untouched. `authority-exact-admission-regression/results.json` passes
the existing dependency/quota suites with all three toolchain/sanitizer variants.

`communication-jobs-native-exact-admission/results.json` passes dependency
capture, compilation and actual bidirectional native ROS 2 execution using the
two sealed image closures from `communication-private-ros2-fifth`. This verifies
the stricter admission and transition-specific claims with real workers. It
still uses fixture publication handles and does not qualify guest registration.

Deferred action server replies reserve their intended transition but leave the
observed state unchanged. The trusted `settle_operation` path advances that state
only after an actual complete worker result. Known no-effect failures preserve
the previous state; unknown results latch uncertainty. Pending work and unknown
outcomes prevent endpoint release. Client submit/cancel receipts similarly retain
separate host request IDs and do not confuse queue acceptance with remote action
acceptance or physical cancellation.

## Typed nonblocking observations

`communication_queries.h` provides closed context/node inspection, topic/service
QoS, service availability, action-state and filtered graph queries. It reserves
one queue slot plus separate encoder and reply-copy storage, totaling at least
twice the configured request/reply byte capacities. Allocation uses recoverable
fast_io storage. Configuration performs no native work and confers no authority.

A query carries the actual caller, exact primary capability, and complete
ordered node/context/schema origins. Graph queries additionally retain the
explicit `context.describe` role; readiness binds a separate exact watch handle.
The queue copies the complete request and reserves its reply before charging
the full RPC byte budget. Pending identical calls resume the same request without
another charge. A different caller, capability, action, watch or body cannot
replace or consume that result. Supervisor progress does not hold an authority
group while receiving a reply.

Before decoding, the complete reply is copied under fresh full authority into
the reserved host buffer. Only a trusted bounded decoder receives those bytes;
it must neither call guest code nor retain the span. Recoverable allocation
failure leaves the reply and its charge intact for retry. A normal failed native
query exposes its actual error without decoding its body. Unexpected effects,
unknown outcomes and malformed records fail closed and retain occupied capacity.
The adapter owns no hidden thread, socket, process or placement policy. CPU,
SMT/NUMA placement, worker reservations and stop/drain belong to deployment.

`communication-owned-queries-first/results.json` records nine successful
dependency/compile/execution commands with `-O3 -g`, ASan/UBSan, GCC 13 and TSan.
It uses the real graph dispatcher's origin tokens and nonblocking pipes to check
single charging, zero remaining budget, decoder allocation retry, substituted
callers/capabilities, concurrent submission, revocation, cancellation, malformed
replies and fork refusal. `communication-query-allocation-first/results.json`
exhausts all five real fast_io allocation points and verifies successful retry.
The graph watch token was subsequently tightened to carry its own incarnation;
the later graph receipts described in the provider document qualify that change.

`communication-typed-queries-native/results.json` records a fresh host compilation
and real bidirectional ROS 2 execution against the two sealed closures from
`communication-private-result-status`. It exercises typed observations together
with typed mutations, sixteen explicit asynchronous opens, String/Bool topics,
SetBool services and LookupTransform actions in both directions. Publication
handles in this fixture remain host test handles; this receipt does not qualify
VM registration, generic ROS IDL or the final deployment coordinator.

The preceding mutation qualification is retained in
`communication-mutation-dispatch-locks` and `action-queued-transition-admission`.
The former fixed a TSan-reported configure/admission lock-order cycle by making
encoder acquisition nonblocking; the original failed receipt remains available.
`communication-creation-exact-groups` checks the complete ordered creation claims,
caller, byte charge and single consumption of the charged group, including
managed reservation-first enqueue and concurrent exact retries. An equal count
of unrelated permissions cannot authorize creation or publication.
