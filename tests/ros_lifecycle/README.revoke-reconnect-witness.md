# Prepared signed ROS2 revoke/fault/reconnect witness

`ros2_revoke_reconnect_witness.cc` is a standalone C++20 native witness for one
bounded G09 case. It receives actual typed DDS samples through two independent
subscriptions. It does not use the ROS IPC client, create a Gateway, spawn a
process, generate credentials or grant Guest permissions. The original
Source18 fixture was compiled and reached its real revocation/fault/reconnect
exchange, then failed during fresh discovery. Source19 diagnosed a separate
initial-discovery failure with an official unknown-node placeholder. Source20
adds a bounded association wait and is prepared source; it reports no
compilation, signing or runtime PASS for this revision and does not replace
either preserved failed result.

The controller owns a raw Wasm32 Guest, its current qualified Worker, two
successive owned Gateway processes, signed credentials and all execution
evidence. The Guest retains the same instance and opaque handles across the
controlled Gateway fault. A changed publisher GID is native endpoint evidence;
the controller separately proves a newly authenticated IPC session. Neither
fact can replace the other.

All actual work must use the original parent-proven Linux container/cgroup,
currently exactly 64 GiB (`memory.max=68719476736`), zero swap and effective CPUs
`0-31`. Re-measure leaf device/inode, boot, membership, container `StartedAt`,
QEMU, the active task-owned binfmt lease and readable ancestor OOM counters.
Require the same incarnation, zero new OOM events, immutable source/SDK/tool
inputs and complete owned cleanup. Keep historical 64 GiB receipts unchanged.
The existing positive r16 G09 files are separate frozen inputs.

## Fixed native context and endpoints

Invoke the witness with exactly one argument:

```text
ros2-revoke-reconnect-witness /tmp/revoke-reconnect-keys
```

Use a literal native environment with `ROS_DOMAIN_ID=75`,
`RMW_IMPLEMENTATION=rmw_fastrtps_cpp`, `ROS_SECURITY_ENABLE=true`,
`ROS_SECURITY_STRATEGY=Enforce` and the indicated `ROS_SECURITY_KEYSTORE`.
Ambient enclave, namespace, discovery, XML QoS and preload overrides are
refused. The witness checks its initialized public RMW context for domain 75,
implementation, `/witness`, Enforce and the exact selected security root
`/tmp/revoke-reconnect-keys/enclaves/witness`. The controller verifies both
signed `/gateway` and `/witness` roles, explicit topic permissions, certificate
and key association, and default deny before releasing either participant.

The qualified Jazzy setup hook supplies `ROS_AUTOMATIC_DISCOVERY_RANGE=SUBNET`
when that variable is absent. The witness-only controller wrapper sources the
SDK, checks that unsetting this variable succeeds, verifies its absence and
then directly executes the witness. Its C++ environment predicate still
requires absence. This fixture normalization grants no authority and leaves
the ordinary Gateway and native-tool setup paths unchanged. The earlier
positive-case r16 input failure and actual SDK diagnostic remain separate
preserved evidence. Source18/19 passed native input and reached discovery with
this wrapper; that does not qualify this Source20 discovery revision.

The witness node is `/uwvm_revoke_test/revoke_witness`. Both Gateway processes
use `/uwvm_revoke_test/uwvm_ros2_gateway_probe`. Topics are
`/uwvm_revoke_test/dead` and `/uwvm_revoke_test/live`; the official type is
`std_msgs/msg/String`, with identity:

```text
RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18
```

Both actual local subscriptions must use reliable, volatile, KeepLast8,
automatic liveliness, infinite deadline/lifespan/liveliness lease and normal
ROS namespace conventions. The graph must report exact node, namespace, topic
type/hash, direction, nonzero GID and advertised reliable/volatile fields.
Remote DDS discovery does not supply qualified history/depth evidence; the
witness does not infer complete remote QoS from it.

The qualified Jazzy `rmw/types.h` defines `RMW_GID_STORAGE_SIZE=16u`. The fixture
asserts exactly sixteen bytes, copies every native byte and prints exactly
32 lowercase hexadecimal digits. It refuses another width, zero identities,
padding or truncation. The separate RIHS01 type hash remains 32 bytes and
64 hexadecimal digits. The original four endpoint GIDs are distinct; the new
live publisher GID must differ from all four.

## Controller gates and independent delivery

The controller's private native stdin pipe has exactly three one-byte gates:

1. `I` permits native initialization after actual stopped ARM image evidence.
2. `N` follows the Guest's checked `REVOKED_STALE9_REOPEN125` marker. It freezes
   the exact native baseline and permits fresh endpoint discovery.
3. `F` follows `PASS_REVOKE_RECONNECT` plus actual Guest exit zero. It starts
   the final two-second native quiet observation.

The native witness consumes one readable byte per gate using `fast_io`; a
formatted scanner cannot read ahead. Wrong bytes, EOF, a missing gate or a
deadline fail. A single sixty-second monotonic deadline after `I` covers
initialization, discovery, baseline, `N`, fresh discovery and `F`. Native calls
cannot interrupt themselves; an external owned-pidfd deadline also bounds
middleware calls and teardown. The final quiet loop lasts at least two seconds
and must record at least thirty actual executor samples.

The Guest first publishes `WARMUP-DEAD` and `WARMUP-LIVE` once to create lazy
native endpoints. Each exact warmup may be received zero or one time because
volatile discovery can lose a pre-match sample. If received, its actual
`MessageInfo.publisher_gid` must match the independently discovered old
publisher. Warmups never substitute for a positive baseline.

After `MATCHED_REVOKE_OLD`, the controller releases the Guest's baseline gate.
The witness must receive `BEFORE-DEAD` and `BEFORE-LIVE` exactly once, from the
respective old publisher GIDs, before emitting `BASELINE_REVOKE_WITNESS`.
Duplicate baseline messages, intra-process deliveries, wrong source GIDs,
wrong text or another publisher fail. No payload is logged or retained.

The controller then releases the Guest's revoke gate. The Guest revokes the
dead handle, requires stale publish errno 9, and requires reopening the revoked
alias to return errno 125 without changing its protected output. After `N`,
the witness preserves dead baseline count one and live baseline count one.
Any later dead delivery, including another warmup or baseline, is a forbidden
native side effect. The snapshot marker alone is not an unlimited absence
proof; the complete case also needs the final quiet observation.

After `FROZEN_REVOKE_WITNESS`, the controller kills and reaps only its original
Gateway and descendants using retained birth/pidfd facts, and removes only its
exact old socket. The same live Guest attempts `FAULT-LIVE` using the surviving
authorization. It must receive errno 107; the witness refuses any actual
`FAULT-LIVE` delivery. Timeout errno 110 cannot replace the required fault.

The controller starts and maps the same immutable Gateway product with the same
explicit policy and Worker peer. The surviving Guest publishes
`RECONNECT-WARMUP` once to create the replacement session/publisher, then waits
for native matching. The witness requires no dead publisher and exactly one
live publisher with a fresh GID. A new dead publisher resurrects revoked
authority and fails. An old advertisement can disappear asynchronously; it
does not satisfy fresh matching. If the new warmup was received, its source
must match the newly discovered GID.

The killed old live publisher's advertisement may temporarily coexist with
the replacement advertisement after the dead publisher has already retired.
Fresh discovery permits only the pinned old live GID plus at most one distinct
candidate whose name, direction, type hash and advertised QoS all qualify. It
continues the existing bounded wait until the old entry disappears and native
matching reports exactly one replacement. Duplicate entries, two fresh
publishers, a third entry or a foreign endpoint fail. Initial discovery remains
strictly one publisher per topic, and the final matched-new snapshot still has
zero dead publishers and exactly one live publisher. No sleep duration or
expired lease is treated as retirement evidence.

After `MATCHED_REVOKE_NEW`, the controller releases the Guest's final gate.
The Guest repeats the dead stale/reopen checks, publishes `AFTER-LIVE` once,
emits `AFTER_MESSAGE_SENT` and waits behind its additional `Q` gate with the
publisher retained. The witness must receive `AFTER-LIVE` exactly once from the
fresh publisher GID before emitting `DELIVERED_REVOKE_NEW`. Only both the Guest
and native delivery markers permit the controller to send `Q`; the Guest then
closes the survivor and verifies its stale handle. This handshake establishes
delivery before native writer retirement; a publish return value or fixed sleep
cannot replace it. `F` then drains native
callbacks for the bounded quiet window while preserving dead count one, old
live count one, final live count one and zero forbidden/fault/unknown effects.
Only the main thread runs the executor; no spinner or detached thread survives.

## Closed public marker protocol

All seven LF-ended markers repeat this common field order:

```text
pid node enclave domain rmw type dead_topic live_topic dead_sub_gid live_sub_gid
```

The values are the fixed checked context and two independent native subscriber
identities. Each marker has exactly these additional fields:

| Marker | Additional fields |
| --- | --- |
| `READY_REVOKE_WITNESS` | None |
| `MATCHED_REVOKE_OLD` | `old_dead_pub_gid old_live_pub_gid dead_publishers=1 live_publishers=1` |
| `BASELINE_REVOKE_WITNESS` | `old_dead_pub_gid old_live_pub_gid dead_before=1 live_before=1` |
| `FROZEN_REVOKE_WITNESS` | `old_dead_pub_gid old_live_pub_gid dead_total=1 live_before=1 unauthorized_dead=0` |
| `MATCHED_REVOKE_NEW` | `old_dead_pub_gid old_live_pub_gid new_live_pub_gid dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=0` |
| `DELIVERED_REVOKE_NEW` | `old_dead_pub_gid old_live_pub_gid new_live_pub_gid dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=1` |
| `PASS_REVOKE_RECONNECT_WITNESS` | `old_dead_pub_gid old_live_pub_gid new_live_pub_gid dead_publishers=0 live_publishers=1 dead_total=1 live_before=1 live_after=1 unauthorized_dead=0 fault_live=0 unknown=0 quiet_samples quiet_ns warmup_dead warmup_live warmup_new` |

Require exact order, exact keys, one occurrence of every marker, canonical
unsigned decimal counts, stable common/old GIDs and independent current
PID/birth/map/context/signature association. Final warmup counts are each zero
or one, `quiet_samples` is at least 30 and `quiet_ns` is at least 2000000000.
The new and final publisher counts are the matched-new snapshot before the
Guest closes; they do not claim that its closed publisher is still present
at final witness completion. Generic successful text is insufficient.

Only fixed public metadata is printed with `fast_io`. A failure prints
`FAIL_REVOKE_RECONNECT_WITNESS phase=FIXED_PHASE site=FIXED_SITE field=FIXED_FIELD`
and exits one. Raw exceptions,
DDS payloads, credentials and private roots are suppressed. The controller must
still qualify the immutable compiled predicate and every actual process; a
marker from an unqualified substitute is not evidence.

## Diagnostic revision after the preserved Source18 failure

The Source18 exchange observed both exact baseline messages and the frozen
dead counter, checked Guest stale/reopen errors 9/125 and dead-peer error 107,
retired the old owned Gateway with the requested signal, and admitted a fresh
authenticated session's live publish. Its native witness then printed only
`phase=fresh-discovery`; it never emitted `MATCHED_REVOKE_NEW`. That phase alone
does not identify the failed predicate. The complete case remains failed.

The Source19 diagnostic revision preserved every Boolean admission/refusal condition,
success marker, counter, GID width, native context/QoS check, gate and deadline.
It adds only fixed refusal-site fields. Exact name, namespace, type, endpoint
direction, hash, nonzero GID, reliability and durability checks are split into
separate refusals with the same conjunction. Reading a GID before these checks
chooses a diagnostic label only; it admits nothing. Known old endpoints and
fresh candidates still undergo the original complete validation.

The Jazzy graph cache stores node associations separately from endpoint rows.
Its public endpoint descriptions can contain fixed unknown-node placeholders
while associations are pending or retiring. This is a source-supported
hypothesis for the failure, not its established cause. The diagnostic labels
distinguish those placeholders without printing a discovered name or changing
their refusal in that revision. No unknown-node wait or endpoint relaxation
was included in Source19.
See the [official GraphCache source](https://raw.githubusercontent.com/ros2/rmw_dds_common/jazzy/rmw_dds_common/src/graph_cache.cpp).

## Source20 bounded association wait after the diagnosed Source19 refusal

The actual Source19 record was exactly
`FAIL_REVOKE_RECONNECT_WITNESS phase=old-discovery site=old_publisher field=name_unknown`.
It had emitted `READY_REVOKE_WITNESS` and observed Guest endpoint creation;
only native `I` and Guest `G` had been released. It reached no old matched
marker, baseline or revoke stage. This establishes an initial unknown-name
observation. It does not retroactively identify the exact Source18 predicate
that failed during fresh discovery, and neither exchange is accepted.

The new discovery helper recognizes only the two complete pairs emitted by
the official Jazzy GraphCache:

| Unresolved node name | Unresolved node namespace |
| --- | --- |
| `_NODE_NAME_UNKNOWN_` | `_NODE_NAMESPACE_UNKNOWN_` |
| `_CREATED_BY_BARE_DDS_APP_` | `_CREATED_BY_BARE_DDS_APP_` |

These are WAIT observations. They never qualify a publisher, select an old or
new public GID, satisfy a matched marker, or establish authenticated identity.
Mixed pairs, empty or foreign named nodes and namespaces remain refusals.
Before waiting, every pending row must retain the exact official String
type/hash, publisher direction, nonzero sixteen-byte GID and advertised
reliable/volatile fields. A permanent bare DDS endpoint remains unresolved
and fails the shared deadline; the placeholder is not permission to admit it.
The original strict `endpoint()` predicate is unchanged and must validate
every endpoint selected for a successful snapshot. Local witness subscription
discovery remains strict.

Initial graphs still contain at most one writer per topic. Raw identities of
present rows are checked against both witness subscriber GIDs and each other
before retrying; pending rows cannot conceal duplicate identities. The old
selected GIDs are assigned only after both complete exact node associations
and actual subscription publisher counts one/one. The original final
four-way distinctness and warmup-origin checks still run.

The fresh snapshot scans both topic graphs before any association wait. It
allows at most the pinned retired dead writer, and rejects another dead GID
even with a placeholder. Its live rows allow at most the pinned old live GID
and one independent candidate. Third rows, duplicate old/candidate GIDs,
two fresh candidates, subscriber/dead GID reuse and foreign named endpoints
remain refusals. A surviving old dead row or any pending association forces
WAIT. Final selection still requires no dead writer, no old live writer,
one fully exact replacement and actual publisher counts zero/one.

The shared sixty-second exchange deadline is never restarted. Every retry
continues executor sampling and the existing effect counters. Callback bodies
are unchanged: at most one exact pre-match warmup is retained as a source GID
and count, then reconciled against the fully qualified selected old/new GID.
No received payload is silently dropped or stored for later acceptance.
Early `BEFORE`/`AFTER`, duplicate warmups, unknown deliveries and `FAULT-LIVE`
remain failures. Baselines, revoked-side counts, the delivery-before-close
`Q` gate, the seven success markers and final two-second quiet oracle are
unchanged. Source/synthetic results for this revision are not actual ROS,
Guest, cryptographic or QEMU acceptance.

The closed site/field catalog is:

| Fixed site | Fixed fields |
| --- | --- |
| `native_exception`, `contract` | `unspecified` |
| `callback_origin` | `implementation`, `zero_gid` |
| `callback_source` | `null_or_intraprocess` |
| `dead_delivery` | `warmup_rejected`, `baseline_rejected`, `unexpected_text` |
| `live_delivery` | `fault_text`, `warmup_rejected`, `baseline_rejected`, `reconnect_rejected`, `after_rejected`, `unexpected_text` |
| `callback_counters` | `forbidden_effect`, `positive_count`, `warmup_count` |
| `frozen_counters` | `baseline_count`, `warmup_count` |
| `own_subscription_graph`, `old_publisher_graph`, `fresh_dead_graph` | `shape` (`fresh_dead_graph` also has `old_gid`) |
| `own_subscription`, `old_publisher`, `fresh_old_dead`, `fresh_dead_candidate`, `fresh_old_live`, `fresh_live_candidate` | `name_unknown`, `name_bare_dds`, `name_mismatch`, `namespace_unknown`, `namespace_bare_dds`, `namespace_mismatch`, `type`, `direction`, `hash`, `zero_gid`, `reliability`, `durability` |
| `fresh_live_graph` | `shape`, `duplicate_old`, `duplicate_fresh`, `independent_gid`, `warmup_origin` |
| `fresh_snapshot` | `delivery_count` |
| `fresh_own_subscriptions` | `identity` |
| `exchange_deadline` | `before_spin`, `after_spin`, `after_sleep`, `before_new_marker` |

The native exception fallback is a fixed category, never an exception string.
A closed failure record remains failure evidence and cannot be accepted as any
of the seven success markers. Independent diagnostic models exercise the
catalog, malformed records and the unchanged Boolean endpoint truth table.
The added graph oracles exercise bounded pending/resolved associations and
refuse malformed rows and traces; they do not execute a DDS transition.

## Independent source models and acceptance limits

The new native-only oracle can run without any compiler, ROS, QEMU or crypto:

```text
PYTHONDONTWRITEBYTECODE=1 python3 -B \
  SOURCE/tests/ros_lifecycle/revoke_reconnect_witness_models.py \
  --source SOURCE --out FRESH_SOURCE_MODEL_RECEIPT.json
```

It checks separately generated synthetic deliveries and the closed native
marker protocol against trusted expected facts held outside candidate data.
It exercises lost warmups, exact baselines, late unauthorized/fault deliveries,
duplicate/missing messages, wrong/coherently substituted identities, endpoint
retirement, canonical GID widths and bounded quiet observations. Source guards
bind the intended public API/gate/callback/IO contracts. These models perform
no native callback, signature verification or runtime execution.

Actual acceptance still requires the complete controller-owned Guest/Worker,
old and replacement authenticated Gateway sessions, signed native witness,
exact byte directions, explicit errno assertions, stopped/live ARM maps,
same current 64 GiB incarnation, zero OOM and full owned cleanup. This case
does not imply remote rollback, cross-process revocation of another Instance,
WASIp1 default rights, other transports/RMWs, memory64 or complete G09/task
acceptance. Preserve the prepared-source and actual-runtime receipts separately.
