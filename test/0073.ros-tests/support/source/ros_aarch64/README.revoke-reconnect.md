# Prepared signed ROS2 revocation, fault and reconnection lane

This is a separate source fixture for the original lifecycle requirement. It
does not change or replace the frozen positive G09 Guest Enforce controller,
its historical failures or its evidence. The original frozen revision18 was
compiled and entered the actual signed lifecycle exchange in the original
48GiB cgroup, but failed in the native witness's fresh-discovery phase after
revocation and the owned Gateway fault. That immutable failure remains a
failure. The later failure-metadata and diagnostic changes described here are
source-only preparation; this document reports no compilation or behavior
acceptance for those changes. The Python model receipts are source-only.

The controller uses one raw-import Wasm32 Guest, one standalone native witness
and two sequential instances of the same qualified Gateway binary. The first
Gateway is deliberately killed through retained pidfds and reaped. The same
Worker and witness survive while a fresh Gateway starts. A successful case
requires a different authenticated IPC session and native publisher GID, with
the revoked alias still unavailable and the surviving publisher still usable.

The fixed native domain is 75. Signed enclaves are `/gateway` and `/witness`;
the keystore is the fresh owned `/tmp/revoke-reconnect-keys`. The Gateway node
is `/uwvm_revoke_test/uwvm_ros2_gateway_probe` and the witness node is
`/uwvm_revoke_test/revoke_witness`. The two topics are
`/uwvm_revoke_test/dead` and `/uwvm_revoke_test/live`. Both use the exact official
`std_msgs/msg/String` identity:

```text
RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18
```

Only the internal `deadPub` and `livePub` publisher grants are explicit. Their
limits include 64 canonical bytes, one publisher, 100 messages/s, burst eight,
explicit byte/audit/resource budgets and reliable/volatile KeepLast8 QoS with
infinite timers. Opening an alias, signed DDS permissions or reconnection does
not grant anything else. The Guest also receives the explicit WASI monotonic
clock right and basic standard descriptors. It receives no credentials, host
environment, preopened filesystem, sockets, process creation or graph rights.

The witness-only native wrapper accounts for the measured Jazzy setup hook
which adds `ROS_AUTOMATIC_DISCOVERY_RANGE=SUBNET` to an initially clean
environment. After sourcing the qualified SDK it checks `unset` and literal
absence before executing the exact witness argv. The C++ strict absence
predicate remains. Ordinary native tools and both Gateways retain their usual
SDK wrapper; no ambient parent environment is copied.

## Actual execution is pending and owner-only

Future execution must use the same original Linux container with a **64 GiB**
limit (`memory.max=68719476736`) and zero swap, its exact current boot/leaf,
cpuset, readable ancestor OOM counters, private mount namespace and active
fixed-QEMU binfmt lease. The controller refuses a changed incarnation. It pins
the complete frozen source, helper closure, original read-only SDK, actual
compiler recipe, full `-MD`/effective `-MF` dependency files, original object,
link libraries, products and process births. Live stopped ARM mappings precede
the witness's native initialization and the tested Guest phases. A Gateway
mapping is observed after its native initialization; no pre-init Gateway claim
is made.

The same closed preparation interface as the positive controller is used,
with a separate dedicated witness product. Preparation compiles only
`test/0073.ros-tests/support/source/ros_lifecycle/ros2_revoke_reconnect_witness.cc`; it performs zero Guest
behavior cases. Keep the entire successful prepared directory and its original
inodes. Do not copy only the witness ELF or substitute an old compilation.

The following are templates for the parent-controlled lane; they were not
executed. `SOURCE` must contain every new fixture and the manifest must pin
that exact source. `SDK` is the original task-owned Jazzy root. `OUT_PREPARE`
and `OUT_EXECUTE` are fresh separate paths. The parent supplies its current
fixed-interpreter binfmt evidence and launches inside a private mount namespace.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -B SOURCE/test/0073.ros-tests/support/source/ros_aarch64/run_revoke_reconnect.py \
  --mode prepare --source SOURCE --manifest SOURCE_MANIFEST \
  --rootfs SDK --out OUT_PREPARE --qemu QUALIFIED_QEMU \
  --binfmt-evidence CURRENT_LEASE --container-started-at CURRENT_STARTED_AT

PYTHONDONTWRITEBYTECODE=1 python3 -B SOURCE/test/0073.ros-tests/support/source/ros_aarch64/run_revoke_reconnect.py \
  --mode execute --source SOURCE --manifest SOURCE_MANIFEST \
  --rootfs SDK --out OUT_EXECUTE --qemu QUALIFIED_QEMU \
  --binfmt-evidence CURRENT_LEASE --container-started-at CURRENT_STARTED_AT \
  --prepared OUT_PREPARE/report.json --worker CURRENT_DUAL_WORKER \
  --worker-report CURRENT_FOUR_PROFILE_REPORT --worker-config CURRENT_WORKER_CONFIG \
  --gateway CURRENT_ROS2_GATEWAY --gateway-report CURRENT_GATEWAY_REPORT \
  --wat2wasm QUALIFIED_WAT_TRANSLATOR
```

Preparation refuses execution-only arguments. Execution requires all seven
prepared/product/translation arguments, exact same-source/current SDK
qualifications, all four Worker profiles and a separately qualified current
ROS2 Gateway. The trusted optional custom library directory is admitted only
through the shared closed SDK library helper; it never comes from ambient
loader settings. This new witness does not extend the original eighteen-product
native baseline inventory.

## Protocol and acceptance

The exact native protocol is documented in
[`README.revoke-reconnect-witness.md`](../ros_lifecycle/README.revoke-reconnect-witness.md).
All native GIDs represent every byte of the qualified Jazzy16-byte ABI as 32
lowercase hex digits. Different widths, padding, truncation, zero GIDs, unknown
fields, partial final lines and reordered/repeated markers fail closed.

1. Native `I` follows a stopped witness image proof. Guest `G` follows live
   mapping proofs for the same Worker, old Gateway and witness. Initial warmups
   create lazy publishers; volatile discovery may drop them.
2. Guest `B` follows exact old native endpoint matching. `BEFORE-DEAD` and
   `BEFORE-LIVE` must each arrive once from their independently discovered old
   writer GIDs before `R` permits revocation.
3. The Guest revokes `deadPub`, requires stale publish error **9**, and requires
   alias reopen error **125** with unchanged output sentinels. Native `N`
   freezes the independent dead-topic count at one.
4. The controller verifies and kills only owned old Gateway births via pidfds,
   requires the exact root `SIGKILL` result **-9**, reaps it and proves all
   observed old descendants retired. Only then may it remove that old socket
   inode and release Guest `F`. The still-open dead connection must produce
   **107**; timeout 110 or another error is insufficient. Any native
   `FAULT-LIVE` delivery fails.
5. The replacement maps the same immutable Gateway product, binds the same
   Worker PID and owns a new socket inode. `C` permits only the surviving handle
   to establish a fresh session. Exact native `session.bind` alias `target` and
   successful `livePub` publication must be recorded before `A`. The old
   resource-free preflight bind is not mistaken for the Guest session.
6. The new native live writer GID differs from the four old endpoints, and no
   dead publisher reappears. The same Guest repeats errors **9/125** and sends
   `AFTER-LIVE`. It emits `AFTER_MESSAGE_SENT` and waits behind `Q` while its
   publisher remains open. Only the witness's exact fresh-GID callback and
   `DELIVERED_REVOKE_NEW` permit `Q` and subsequent close/PASS.
7. Native `F` follows Guest PASS and exit zero. At least two seconds and 30
   actual executor samples preserve the dead count one and survivor counts
   one before/one after, with zero unauthorized dead, fault or unknown messages.
   The final marker retains the earlier new-endpoint graph snapshot; it does
   not claim a publisher remains after the intentional Guest close.

The final native audit has four exact new-session operations: bind, surviving
warmup publication, surviving final publication and surviving capability close.
No dead-alias RPC is accepted on the replacement. This is separate evidence
from the changed DDS GID. Immediately after a successful Guest `G` write and
flush, the report records `actual_guest_behavior=RUNNING`. A later failure is
`NOT_ACCEPTED`; `NOT_PERFORMED` is retained only when that release never
succeeded. Partial fixture observations do not become accepted behavior.

After bounded owned cleanup, every retired command has its actual integer
return code and final raw log binding, including failed partial lanes. The
ledger preserves previously completed tools and the expected old Gateway
SIGKILL. It distinguishes a native process that had already exited before
cleanup from a still-live process interrupted during failure cleanup. A
nonzero native exit racing that sample is recorded as `FAIL_DURING_CLEANUP`,
rather than attributed to a requested cleanup signal. Unretired children and
log-binding failures remain explicit failures. These metadata states do not
change the completion predicate: accepted replacement cleanup still permits
only zero or its expected termination signals and cannot conceal an unrelated
terminal failure.

The native refusal protocol is one LF-ended line:

```text
FAIL_REVOKE_RECONNECT_WITNESS phase=<fixed-phase> site=<fixed-site> field=<fixed-field>
```

Only bounded public diagnostic codes are exposed; no exception body, payload,
GID or credential path is added. The parser always rejects this line, including
when the native process exits before the controller polls its final output.
The phase and site/field pair must belong to the native source's closed literal
catalog before they can appear in the diagnostic; unknown tokens receive only
a generic refusal message. Malformed refusal lines are also rejected. Successful marker schemas, exact
GIDs, permissions, counts, error codes and gate ordering remain mandatory.

Success also requires unchanged source/SDK/control/product pins, same cgroup
incarnation, zero new ancestor OOM events, no owned live processes or mounts,
socket/domain-lock removal and removal of all temporary private credentials.
Public hashes and verified metadata remain; private key bodies are not archived.
The controller always leaves `G09_complete=false`: this bounded combination
does not establish cross-version fault recovery, arbitrary Guest traps,
corrupt-frame behavior, concurrent revoke races or all original requirements.

## Source-only checks

The combined model imports no execution controller or runtime helper. It
AST-extracts deterministic parsers/predicates and exercises an independent
synthetic lifecycle oracle plus adversarial marker/session/ownership evidence.
WAT source-byte/signature checks are not Wasm validation or execution.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -B test/0073.ros-tests/support/source/ros_aarch64/revoke_reconnect_models.py \
  --out /tmp/FRESH_REVOKE_MODELS.json
```

Run the separate native-only source models from the linked native README too.
Fresh compilation and the combined signed ROS/Guest acceptance remain pending
for the later source changes until the parent runs and archives a new qualified
Linux lane. The original revision18 failure is never replaced by a model PASS.
