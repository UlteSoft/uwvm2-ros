# Authenticated Gateway owned-subscription routing

This is a source-only additive Gateway integration. The two existing source
files are `src/ros/gateway/common/server.h` and `src/ros/gateway/ros2/main.cc`. No Host, Runtime,
loader, raw SDK, original81 import catalog, client declaration, publisher codec,
NodeOptions, parameter-event flow or other native stream is changed by this
package. No candidate, generator, configure/probe, compiler, linker, model,
test, Native participant, Guest, SSH or repository mutation was executed while
authoring. Source review is not an actual execution result.

The exact registry dependency is Memory's r3 complete owned receive registry,
with independently retained primary, cancel, commit and retire outcomes. The
factory dependency is the separately reviewed whole custom shard factory and
explicit builtin/custom `create_selected` selector. Both are independent source
cohorts: use their actual final source seals, peers and same-byte integration.
A missing header, symbol, actual SDK API or selected feature must fail honestly;
there is no local mock/stub in a production target. This package neither
self-reviews its author's factory nor supplies future source-generation hashes.

## Peer, source and endpoint admission

The existing actual Unix peer UID/PID, retained worker pidfd, complete hello,
random session, monotonic request ID, exact target manifest binding, configured
alias, immutable policy/limits, revoked-alias and request deadline checks remain.
The following private actions exist only for backend2 and a configured
`topic.subscribe` source:

* `rmw.subscription-endpoint-v1` carries exactly one UMRE256 request for role
  subscription and this actual authenticated session. Activate is independently
  rate-limited by `activation_quotas`; inspect/retire are control metadata. The
  returned complete UMRE must associate the exact request binding, role,
  operation and nonreusing native incarnation.
* `rmw.subscription-endpoint-retire-v1` requires a complete same-session UMRE
  retire request and the same configured source. It precedes revoked-alias
  denial solely to permit safe known-row cleanup. Registry full binding,
  incarnation and alias lookup remain mandatory. It cannot create, inspect,
  reopen, manage a process or address an unknown endpoint. A previously retired
  row may report its real125 rather than fabricate another successful cleanup.
* `rmw.subscription-transfer-v1` carries a fixed UMRS96 peek/commit/cancel
  request. The server validates its complete wire shape, full source/instance/
  generation/session binding and incarnation before the owning registry.
  ACKs echo the exact sequence and independent reservation token. There is no
  automatic retry of commit, cancel, activation or an ambiguous transport.

Each Adapter owns an actual
`native_rmw_subscription::registry{node_, &create_selected}`. Only the trusted
compiled selector chooses the four real builtin or whole generated custom
Native/Codec endpoint. It never uses graph membership as authorization or calls
the legacy listen path to obtain fake native metadata. The full genuine native
schema/hash/default/codec/QoS checks and complete original native GID extents
remain in the reviewed endpoint/helper; no handle, zero check or process-local
counter substitutes for those facts.

A `subscription_routes_` alias entry reserves legacy or owned receive mode
before the corresponding native creation. A legacy attempt cannot bypass an
owned source's `max-subscribers`, nor can an owned source activate over a legacy
subscription. Every owned source handle still has its own registry entry and
finite history/count admission. A failed creation does not silently switch to
another route. Only final alias close erases route selection. Parameter events
are deliberately excluded from this topic-only choice. Existing event.watch
payload subscriptions enter the same legacy subscribe method and obey the same
route exclusion; metadata-only watches do not invent a native consumer.

## One borrowed record, full validation, exact quota

The registry peeks the actual callback's one owned URMI192 plus whole canonical
body with an independent nonreusing reservation nonce. It retains the borrow
before allocating or serializing a packet. On its own errors it cancels/retains
separate failure observations. A successful registry return is an owned vector
move; the server immediately arms an allocation-free per-request delivery
obligation before any validation, quota map, audit or output preparation.
A failed/second peek does NOT arm and therefore cannot cancel a previous valid
attempt's still-outstanding borrow.

The server checks the complete UMRS reply, exact same call and record extent,
URMI type/flags/metadata presence/full binding/incarnation/sequence/GID extent,
then complete type-specific canonical payload. Custom messages use the original
whole descriptor/native/codec association and canonical validator; builtins
validate every original field, including UTF-8/NUL extent, image dimensions and
all Twist values/policy bounds. Source body max applies before delivery.
Validation uses a borrowed canonical view: no second owned full-body copy is
introduced solely to strip the288 metadata bytes.

Only after that proof does `quotas` debit ONE actual message and its exact
canonical `payload_bytes`. It does not debit UMRS96 or URMI192. Actual native
capture has its own bounded ingress admission; the separate Gateway output
admission is not a second debit of the same bucket. Endpoint inspect/retire and
commit/cancel ACKs are metadata with no message/body charge. The three private
actions are excluded from the old generic full-response byte debit so the peek
body is not charged twice and a burst1 source can still commit its first peek.
There is no refund/replay promise after performed work or an uncertain effect.

Canonical, quota, serialization, late-operation or final-write failure after a
successful peek invokes explicit `registry.delivery_failed` for that binding
and incarnation. It cancels this known pending attempt without popping a record,
retains the primary plus real cancel code, and tombstones/retires if cancellation
itself fails. The server never overwrites its primary error with cleanup error.
A C++ exception during downstream validation/audit/output preparation executes
the same guard with an explicit error5; arbitrary process traps still require
independent Host/worker/session drain.

Only a complete reply written before the operation deadline disarms that local
delivery obligation and leaves the reservation for the client's explicit ACK.
A failed socket write always ends the session after cancelling the peek borrow.
A late peek reply also cancels and ends the session, because the peer may have
observed bytes while its own operation deadline refused them. Commit/cancel can
already have taken effect: an error, malformed, lost or late ACK ends the session
and retires all its known endpoints rather than replaying or claiming rollback.
The registry's actual commit/cancel result and late/transport error are distinct.
A successful endpoint response converted to a final error likewise forces
session retirement so an unacknowledged newly created endpoint is not stranded.

## Cleanup ordering and conservative limits

Native Adapter alias close/revoke first tombstones owned subscription ingress and known borrow,
then releases its route and other owners. Registry clear/close keep finite
once-only Native cleanup diagnostics. The actual connection cleanup guard is
declared after the asynchronous task pool and clears subscriptions THEN
publishers before the pool joins; exceptional unwinding uses the same order.
Adapter destruction also clears the owned registries before other resource
members. Original service availability and old async/event cleanup remain.
No cleanup path requires a new data token, user callback or normal publication.

A UMRS response may legally occupy the complete4MiB IPC payload. Its request is
only96 bytes and the reply is not a request wrapper; never subtract518 from the
wire response limit. The current real configuration parser deliberately retains
its old conservative topic BODY ceiling `4MiB - 518` for original81 compatibility.
UMRS wire BODY ceiling is `4MiB - 288`. These are different facts. This package
does NOT enlarge the configured topic permission. A direct synthetic4MiB wire
contract proves framing/validation only, not that an actual existing config
already grants such a body. Native positive tests must use their real configured
body limit. Whole caller capacity, header/body exact EOF and protocol max checks
remain in force independently.

## Build and synthetic wire contracts

Use one exact complete composed source tree and new owned outputs. Every source,
actual current original SDK/generated bundle, feature probe, system-inclusive MD,
compile command/object/library/ELF and final link must be independently retained.
The producer must build the genuine Gateway itself; these tests are not a weak
replacement backend. `recipe.json` has complete command DATA, for example:

```sh
cmake -S "$source/test/0073.ros-tests/support/source/ros2_compat/owned_subscription_gateway" \
  -B "$owned/wire" -DUWVM_SOURCE_ROOT="$source" \
  -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release \
  -DUWVM_OWNED_SUBSCRIPTION_WIRE_MODELS=ON
cmake --build "$owned/wire" \
  --target owned-subscription-gateway-wire --parallel 1
```

Choose an actual admitted17/20/23/26 compiler. All targets include the genuine
vendored fast_io tree explicitly; modern concepts branches cannot depend on
ambient include variables. The server contract has unconditional checks, not
assert-only Release false passes. It exercises actual socketpair credentials,
retained worker pidfd, real framing/manifest/Server and scope teardown, but its
adapter/URMI bytes are explicitly SYNTHETIC WIRE DATA, never native facts. It
covers absent-alias refusal before calls, body-only debit/ACK immunity, quota
cancel, malformed whole record cancel, final socket-write failure, full4MiB wire
shape, late ACK terminality and exception-time obligation. It executes no actual
Native API and supplies no SDK/ROS/Host/Guest qualification.

Every runtime process/thread, temporary directory, actual child tree, namespace,
network side effect, deadline, memory/cgroup/backing resource and cleanup belongs
to an independently admitted Root execution owner. CMake does not launch a test,
compiler probe, Gateway or ROS fixture implicitly. No command in this document
was executed while authoring. A source marker is not an executed result.

## Independent actual native Gateway witness

Build `owned-subscription-gateway-native` in another fresh owned directory with
`UWVM_OWNED_SUBSCRIPTION_NATIVE_WITNESS=ON`, the actual original SDK prefix and
actual `UWVM_ROS2_GATEWAY_FEATURE_DIR`. Those feature bits require genuine
compile/link probes; no forced feature or guessed SDK version is admitted.

The separately bounded controller starts this exact original-SDK worker with:

```sh
"$native_build/owned-subscription-gateway-native" --trusted-native-fixture \
  "$actual_immutable_gateway_config" "$exact_subscribe_alias"
```

The worker prints its actual PID/UID and waits for one `G` on its owned stdin.
Before sending G, the controller must materialize a fully qualified immutable
config with THIS worker PID/UID, the genuine Gateway's same UID, socket, actual
backend2 original implementation/domain/security, exact Native String RIHS and
all mandatory limits/QoS/audit/connection fields. Start/retain the real Gateway
in the independently controlled same native domain and namespace first. The
stdin gate and all processes require finite external deadlines; PID output is
data and must not be evaluated as shell. No file/PID/hash/model is forecast.

The actual source must admit canonical11 bytes, queue storage at least203 bytes,
burst>=2 and bandwidth>=22 so the two deliberate peek attempts are real separate
message admissions. Other original bounds remain explicit and finite. Native
NodeOptions and effects are unchanged; this is an explicit trusted Native
fixture, not a new Guest/WASI clock/network permission or a fake wait provider.

The worker uses the real configuration loader, authenticated IPC client, source
activation/inspect, explicit missing-alias denial and mixed legacy route17. It
creates an independent original `std_msgs::msg::String` publisher, confirms
actual native discovery, publishes exactly7 bytes `77 61 73 6d 00 72 6f` and
checks the exact11 canonical bytes, complete actual same-record metadata and
real original publisher GID returned through the genuine Gateway. It cancels,
repeeks the same real record with a new nonce, commits once, checks empty11,
privately retires, observes native subscription disappearance and requires125
after retirement before closing the source/session. Missing native metadata or
SDK API is a real nonzero failure, never a successful skip.

That source is unexecuted. Even a future actual pass would cover this selected
String/Gateway route only. Real revoked-alias private cleanup, worker loss,
partial write/late ACK, all cleanup ledgers, complete custom provider/schema
families/SDK versions, actual Host raw namespace, Guest memory-copy/ABI/default
rights/revocation/session ownership, all distributions/vendors/architectures and
performance/security requirements remain independently required. No ordinary
console marker, graph name, configured identity or model can replace them.
