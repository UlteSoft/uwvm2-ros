# Prepared G09 Guest and independent signed ROS2 witness

`run_g09_guest_enforce.py` prepares one dedicated native witness and, in its
separate execution mode, runs one raw Wasm32 Guest through a qualified ARM
Worker and Gateway. The other participant is an independently owned native
ROS2 process. Both participants use Jazzy, `rmw_fastrtps_cpp`, domain 74 and
signed SROS2 Enforce credentials. The prior r16 run compiled the native witness
and verified signed credentials, then failed its strict input check before
Guest execution. The witness-only post-setup correction described below is
prepared source pending fresh combined execution; it establishes no DDS or
Guest behavior PASS.
For the post-setup correction, this document does not report a compilation or
a combined execution; the earlier r16 compilation remains a separate preserved
result.

The tested resources are `/uwvm_test/pubString` and
`/uwvm_test/subString`, with the official `std_msgs/msg/String` type identity:

```text
RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18
```

The Gateway node is `/uwvm_test/uwvm_ros2_gateway_probe` in enclave `/gateway`.
The independent witness is `/uwvm_test/secure_witness` in enclave `/witness`.
The task-owned logical keystore is `/tmp/g09-keys`. These choices are fixed by
the fixture and controller; there is no CLI option to change domain, security
mode, enclave, node identity or tested topics.

The selected Jazzy `ros_environment` setup hook supplies
`ROS_AUTOMATIC_DISCOVERY_RANGE=SUBNET` when the controller's literal environment
leaves it unset. An actual ARM post-setup diagnostic confirmed that this was the
only forbidden witness variable present, while all five explicit domain/RMW/
security settings remained correct. The witness-only launch wrapper now unsets
that variable after SDK setup, checks that it is absent, and then directly
executes the same native argv. The C++ strict environment predicate, stdin
gates, signed policy and Guest permissions remain unchanged. Normal native
tool/Gateway setup is unchanged, including the deliberate Gateway enclave
override test below. This fixture correction does not claim a production
discovery configuration change or a successful combined run.

## Current execution boundary and immutable inputs

All actual preparation, translation, verification and execution must remain in
the original parent-proven Linux container/cgroup and its private mount
namespace. The current budget is **64 GiB** (`memory.max=68719476736`) with
`memory.swap.max=0`. Before acceptance, measure the current kernel boot ID,
leaf device/inode, exact membership, container `StartedAt`, readable ancestor
OOM counters and active fixed-QEMU binfmt lease. Bracket each accepted run with
the same incarnation and require zero new OOM events. A reboot, replaced leaf,
changed container start, missing counter or changed limit requires fresh
evidence. Historical 64 GiB receipts retain their original scope and cannot
qualify this controller's current acceptance.

The parent supplies an exact frozen Source manifest and a qualified Jazzy SDK
root. Keep Source and static SDK inputs read-only and bind every executed helper
to that manifest. Preparation compiles only the dedicated
`g09-secure-witness` target from
`test/0073.ros-tests/support/source/ros_security/guest_enforce_witness_ros2.cc`, using C++20, enabled
assertions, actual system-inclusive `-MD` and target-owned `-MF` dependencies,
observed ARM GCC13/frontend/CMake, the original verbose link recipe/object,
explicit native-library closure and the full SDK dependency inventory. The
native `ld`/`collect2` image is not separately observed by this controller.
The original closed eighteen-product baseline inventory is unchanged.

Preparation must preserve its successful compile recipe, dependency files,
original objects, native tool mappings, linked ARM product and architecture
receipt. It has zero Guest behavior cases. Execution separately requalifies the
prepared witness, current Worker, current Gateway, their original compilation
reports and configurations, full source/SDK dependencies and opened-file pins.
The Worker report retains the fresh four-profile inventory (`none`, `ros1`,
`ros2`, `dual`); this one Guest uses its qualified current `dual` product.
It uses separate immutable image snapshots and records original identities;
an equal hash at a replaced inode is insufficient. It translates only the
frozen `test/0073.ros-tests/support/source/ros_security/guest_enforce_ros2.wat` using the explicitly selected
`wat2wasm` tool. A prepared binary or translated Wasm file alone establishes no
successful signed exchange.

A qualified custom-enabled Gateway may also link official generated native
message libraries. The controller derives its optional native loader directory
only from the requalified custom selection, original SDK link inputs and full
immutable SDK inventory: `/opt/uwvm-custom-types/lib`. It pins the contained
selected ARM shared libraries and supplies that path only to the trusted
Gateway's literal environment. It inherits no ambient loader path and creates
no additional Guest grant. A default Gateway must have no custom SDK link
inputs; the trusted Jazzy setup supplies its ordinary base library paths.

## Closed command interface

Use a fresh output directory for every attempt. The parent must assign the
execution lane and provide current qualified absolute paths. Start the
controller through the existing verified binfmt/private-mount launch wrapper
with `PYTHONDONTWRITEBYTECODE=1` and `python3 -B`.

Preparation requires these arguments:

```text
python3 -B SOURCE/test/0073.ros-tests/support/source/ros_aarch64/run_g09_guest_enforce.py \
  --mode prepare \
  --source SOURCE --manifest SOURCE_MANIFEST --rootfs QUALIFIED_JAZZY_ROOT \
  --out FRESH_PREPARATION --qemu PINNED_QEMU \
  --binfmt-evidence CURRENT_RUNNING_LEASE_JSON \
  --container-started-at CURRENT_OBSERVED_STARTED_AT
```

The preparation mode refuses execution-only arguments. Execution requires all
common arguments above, selects `--mode execute` and a new output directory,
and additionally requires:

```text
--prepared FRESH_PREPARATION/report.json \
--worker CURRENT_ARM_WORKER --worker-report CURRENT_WORKER_REPORT \
--worker-config FROZEN_WORKER_CONFIGURATION \
--gateway CURRENT_ARM_ROS2_GATEWAY --gateway-report STRICT_GATEWAY_REPORT \
--wat2wasm PINNED_WAT2WASM
```

The controller owns the domain-74 lease, temporary credentials, Unix socket,
mounts and all observed process births. It checks subprocess deadlines, bounded
logs/RSS and remaining cgroup headroom. Cleanup uses owned PID/birth/pidfd
records, never a guessed PID or an unrelated process group. Compiler preparation
is serial. Keep every failed attempt and its evidence immutable; choose a new
preparation or execution directory for a retry.

## Explicit Guest rights and two Guest gates

The Worker explicitly enables only the ROS2 target `main`, with separately
granted `pubString` publication and `subString` subscription. The controller
mirrors the exact immutable capability manifest into the Gateway configuration
and binds its authenticated session to the reserved owned Worker PID/UID. Every
quota, QoS, audit and connection field is explicit. Importing a ROS function,
choosing a native type or supplying signed credentials grants no Guest authority.

The Guest's WASI target `main` is also explicitly created and enabled, with host
environment inheritance disabled and only `monotonic-clock` granted. The
fixture uses baseline stdin/stdout for its private one-byte gates and exact
markers. Its other WASI import is `proc_exit`; it requests no process creation,
filesystem, socket, randomness, wall clock or scheduling rights. Production
default-deny behavior remains a separate contract: this scoped positive test
explicitly supplies its two topic grants and does not prove a broad default-deny
matrix from successful native communication.

The native witness waits for byte `I` before initializing its context. Its live
stopped ARM mapping must precede that byte. The Gateway's mapping is a live
admission proof before Guest byte `G`; it is **not** a Gateway pre-initialization
proof. The Worker may authenticate IPC during preflight before the WAT body.
At `G`, the Guest is blocked before its tested ROS calls, so this gate likewise
makes no claim to precede all Worker IPC.

The Guest accepts exactly byte `G` (`0x47`), opens its two explicit aliases,
subscribes and checks that valid but ungranted alias `notGranted` returns errno
13. Its protected handle output, receive buffer and separate length sentinel
must remain unchanged. `capability_open` has no length-output parameter; the
length sentinel protects against unrelated memory mutation.

One successful bootstrap publication creates the granted Gateway publisher.
The Guest writes exactly `SECURE_ENDPOINTS_CREATED\n` (25 bytes), then reads
exactly byte `M` (`0x4d`). The controller releases `M` only after independent
native discovery and exact endpoint matching. There is no time-delay substitute
for that matching proof. Both Guest gate buffers protect the seven bytes after
the one-byte input, and every marker write checks its complete byte count.

## Signed native exchange and evidence association

The outbound canonical String body is 31 bytes: a four-byte little-endian length
27 followed by `uwvm real ROS exact payload`. The distinct reply body is 23
bytes: length 19 followed by `native signed reply`. These records contain no CDR
framing or native padding. After `M`, the Guest sends one final outbound record.
The native witness checks all 27 text bytes and the actual publisher GID against
the independently discovered Gateway publisher; intra-process delivery is
refused. One or two received samples are permitted because the bootstrap sample
may arrive. Exactly two admitted Guest publication sites do not imply exactly
two native receives.

The witness publishes the distinct reply at a bounded rate until completion.
The Guest permits only errno 11 for an empty queue, preserving all output
sentinels, while checking its explicit monotonic ten-second deadline both before
and after `event_poll`. Successful delivery requires length 23 and equality of
all 23 canonical bytes, plus protected buffer/length guards and the unused
41-byte buffer tail. This deadline rejects late poll success; it cannot interrupt
a host import that is already running. The external owned-process deadline
also bounds native middleware and library teardown.

The Guest closes both original opaque handles, checks stale publication errno 9,
writes exactly `PASS_SECURE_GUEST\n` (18 bytes) and exits zero. Only then may the
controller send native byte `F`. The witness subsequently attempts a valid
native publisher on `/uwvm_test/not_permitted`, which is absent from the signed
allow rules. The immutable compiled witness requires its topic-specific
FastDDS permission predicate and exactly one accepted public log-consumer event,
an appropriate failed native publisher initialization, and no forbidden writer.
A generic exception or allocation error is insufficient.

The witness exposes only the source-bound completion fields `denied_native=1`
and `permission_events=1`. Its borrowed raw `Log::Entry` body is not captured or
retained in the controller report. Attribute those fields to the exact immutable
compiled witness, its full SDK closure and pinned reviewed native predicate;
do not describe them as a separately captured raw DDS diagnostic. The witness
disarms its tiny scanner without post-context `ClearConsumers`/`Flush`. Static
library teardown remains covered by the external owned-process deadline.

The three native markers are `READY_SECURE_WITNESS`, `READY_SECURE_MATCHED` and
`PASS_SECURE_WITNESS`. Their closed public metadata includes the independent
witness PID, fixed node/type/directions, canonical sizes, bounded counters and
four distinct nonzero 16-byte endpoint GIDs, each printed as exactly 32 lowercase
hexadecimal digits. Require exact-once ordered markers,
canonical numeric values, unchanged GIDs and association with current owned
PID/birth/map facts. Strings that merely contain a successful marker prefix
cannot establish completion.
The qualified ARM Jazzy SDK's `rmw/types.h` defines
`RMW_GID_STORAGE_SIZE` as `16u`; its complete header SHA256 is
`c1e7be9e420d3c32cf6b24db62c3fbd6b97d0518811f6f4677db09d835cf3830`.
The witness asserts that exact storage width and copies every native GID byte.
It neither pads nor truncates a GID and does not accept another width. The
separate 32-byte RIHS01 type hash retains its complete 64-digit representation.
The completion marker retains the matching GIDs and counts observed before the
Guest's final exchange and closes; it does not assert that the closed Guest
subscription remains a live native endpoint at final witness completion.

Signed credential checks must associate both `/gateway` and `/witness` with the
same pinned policy, generated signed files and verified plaintext, certificate
subjects and trust chain, and matching certificate/key public data. Keep their
checked verification commands and return codes. Recheck the admitted keystore
before removal. A status flag from another key directory or an old successful
native-only subrow cannot replace these bindings. The witness's source-bound
public RMW context checks attest domain, implementation, enclave, Enforce mode
and security root. The Gateway boundary records its checked source selection,
cleared inherited enclave override and positive signed communication; it does
not invent a separate public Gateway context measurement.
For this test, the controller deliberately supplies the literal Gateway value
`ROS_SECURITY_ENCLAVE_OVERRIDE=/witness` while its explicit configuration stays
`/gateway`. The witness receives no override. This controlled input is not
ambient environment inheritance: the positive asymmetric signed exchange tests
the Gateway's reviewed clear-before-init behavior while its directly used
native security root remains unobserved.

## Pure models and acceptance limits

Run the independent source/metadata models without launching any native tool:

```text
PYTHONDONTWRITEBYTECODE=1 python3 -B \
  SOURCE/test/0073.ros-tests/support/source/ros_aarch64/g09_guest_models.py \
  --source SOURCE --out FRESH_MODEL_RECEIPT.json
```

All PIDs, process maps, credentials, signature results, GIDs and cgroup samples
inside those models are explicitly synthetic. The oracle keeps trusted expected
facts outside the candidate receipt and checks coherent substitutions, missing
evidence, malformed markers, changed boot/leaf, current budget, explicit gates,
denied/stale source assertions and cleanup. It does no cryptography and cannot
establish actual ROS or Guest behavior. Preserve its source hashes separately
from actual runtime reports.
The same script extracts only the controller's reviewed deterministic marker and
completion functions from its Python AST and supplies independent malicious
inputs. It never imports the controller module or executes its launch helpers.
Those direct parser/predicate cases are recorded separately from the abstract
independent receipt oracle; the latter is not the production report schema.

A focused actual PASS requires the fresh combined Worker/Gateway/native-witness
execution, both distinct byte directions, exact signed-policy association,
checked refusal/completion predicates, current owned ARM/QEMU maps, one unchanged
64 GiB zero-swap incarnation, zero new OOM events, all input pins preserved and
complete owned process/mount/socket/secret/domain cleanup. Native-only historical
success and prepared-source/model success do not count as this combined case.
This fixture leaves G09 revocation/failure/reconnect, memory64, other message
types, other RMW implementations, full six-language SDK interoperability and the
complete original task acceptance as separate pending work.
