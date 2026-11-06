# Production updates

The `update` command category is independent of `debug`. `uwvm2-ros --help`
lists both; `--help update` lists the launch options. Updates use RELEASE
execution with private compilation and publication machinery; no debugger
controller or DeveloperGrant is installed.

## Supported changes and execution boundary

Linux and the product's bundled LLVM full compiler are supported, with the real
native owner table, CFI and unwind ownership checks retained. One privately owned
immutable main Wasm image is admitted. No Wasm start section, preloaded module,
shared memory, imported memory/global/table, checkpoint profile or unknown native
provider is admitted. An exported `_start` command executes once after authorized launch; an
update does not rerun it. Synchronous builtin WASIp1 imports may retain their
explicit contract for function and policy transactions. Other providers are
rejected until their actual writer/borrower handoff is implemented.

Four change kinds can be represented, with the following concrete boundaries:

| Change | What is applied | Required restriction |
|---|---|---|
| Function | Selected bodies and one merged native metadata/entry publication | All non-code sections and unselected bodies remain identical; same function count and types |
| Global/configuration | Exact numeric mutable reset, or a new immutable storage binding | Explicit tagged scope; same index/type/mutability; simple finite numeric initializers; actual old bits checked for configuration-only changes |
| Module | All bodies and a same-layout state preserve/reset/migrate plan | Explicit scope for every local function and global; no imports, tables, references in signatures/locals, global exports, start, passive data or element segments |
| Permission/interface | Exact namespace/function exposure, operation grants, limits and approved args/environment | Existing module/environment and native resource incarnations; separately signed target contract and policy generation; no implicit new resource creation |

A function update selects at most 64 scope entries, each body at most 64 KiB,
from a complete target image at most 64 MiB. Function indices are local indices,
excluding imported functions. Each body passes the real feature-aware Wasm
validator and LLVM compiler. Non-code changes cannot be hidden in a function
permit. Custom sections are included in this comparison.

An immutable update requires every local function explicitly selected. Each
new generated engine maps its immutable loads to a new independently owned
numeric object. Old immutable objects are never written, and a guest still
cannot execute `global.set` on them. All subsequent function preparations retain
the current binding; old code engines keep their own old owners. Global exports,
initializer dependencies, table/reference aliases and active data/element
initialization are excluded from this configuration-only path. Local memory can
remain when there is no such initialization dependency.

Module transitions preserve the actual type/index/import/export/memory layout;
they do not replace an arbitrary module graph. All function and global scopes
must be explicit, including unchanged definitions. Numeric globals, active data
and code may change under reset/migrate. Memory declarations, data segment count,
active flags and target-memory indices remain identical. Literal i32 data offsets
and payload bytes may change if fully in bounds. At most 16 unshared Memory32
objects and 64 MiB total initial memory are admitted; actual current memory sizes
must still equal the declarations. Already-grown memory is refused.

- **preserve** keeps runtime state. Global and data sections must be unchanged;
  only the complete code replacement is applied.
- **reset** builds private zeroed memories and applies target active data in Wasm
  segment order. It installs every target numeric global initializer, even when
  the files match but business execution changed mutable values. Immutable globals
  receive new storage. Publication copies only prepared bytes under the retained
  fence, updates active-data metadata to owned target bytes, and retains that
  image independently of later function-only updates. Active data remains dropped.
- **migrate** first prepares the same private target initialization, then copies
  only signed memory ranges and mutable numeric global fields from the real final
  old state after drain. Unselected destination state retains target initialization.
  Destination ranges must not overlap; global destinations must be unique and
  source/destination types identical. At most 64 memory ranges, 64 global mappings
  and 64 MiB total copied bytes are admitted. There is no guest migration program,
  native pointer, host handle, resource rebinding or external call. Existing floating
  state is transferred as raw IEEE bits, including NaN payloads; new configuration
  initializers still reject NaN and infinity. The complete final candidate memory
  and scalar content is hashed into PreparedMeasurement.

The VM does not prove business algorithms, units or field meanings. The external
review approves the exact code, values, state operation and permission difference.
It must choose reset or a reviewed migration when the old state is untrustworthy.

Permission changes operate on the same actual environment and process budget
ledger. Closing the namespace or removing exposure makes already compiled bridges
return denial. Operation grants are independent of exposure. Stdio expansion can
only use an existing, still-live, approved native object; a closed object is never
reopened, and a guest's own fd-right reductions are preserved unless that operation
is explicitly newly granted. Filesystem resource creation/rights changes and
socket providers are unsupported by this transition. Arguments/environment are
owned by the environment after publication. No budget, close history or native
resource identity is reset. Tightening or changing approved argv/env requires a
strictly increased revocation generation; every policy transition increments the
policy generation exactly once.

At most 32 candidates and 2,048 retained bodies are admitted per process. Failed
candidate IDs are consumed. Capacity exhaustion refuses preparation. Arbitrary
module graph/type/layout replacement, asynchronous ROS/native handoff, isolated
concurrent epochs, WSEA-required artifact updates and multi-module publication
remain unsupported. Required WSEA verification is never disabled to admit an
update. The VM/compiler/native libraries/drivers/Gateway binaries are outside this
Wasm API. No SDK, cosign or uwgit executable is required by or called from the VM.

## Launch and trust

```text
uwvm2-ros --runtime-aot --management-profile release \
  --update-control-fd <connected-unix-stream-fd> \
  --update-public-key <64-lowercase-hex-ed25519-public-key> \
  --update-contract <approved-initial-contract-reference-sha256> \
  --update-scope <deployment-component-device-scope-sha256> \
  --update-call-export <optional-approved-business-export> \
  --run component.wasm
```

The optional business export authorizes one local export and its exact scalar ABI.
Without it the channel cannot invoke guest functions. It never exposes arbitrary
private calls, memory, registers or evaluation. Guest WASI/ROS grants remain
separate. Management options are not guest arguments or environment.

The inherited AF_UNIX stream must identify the direct parent and same effective
UID via SO_PEERCRED. Before exec or sending any bytes, the supervisor must enable
SO_PASSCRED on the child endpoint. Enabling it after data is queued cannot
reliably authenticate the queued sender and is refused. Every recvmsg fragment,
including fragments inside one frame, must carry exactly the launch peer's
kernel SCM_CREDENTIALS (PID/UID/GID). A pidfd pins that peer's lifetime; passing
the peer endpoint to another process does not transfer the management grant.
Unexpected ancillary data, passed descriptors, truncation and sender changes
terminate the channel. Received descriptors are closed even on rejection. Aliases through stdin/stdout/stderr are refused. The VM takes
a close-on-exec duplicate and closes the inherited descriptor before guest setup.
The retained management endpoint is sealed against guest descriptor/preopen
aliases. A separate `--diagnostic-control-fd` may coexist; it has its own sealed
endpoint and a strictly read-only dispatcher described in
[RELEASE diagnostics](release-diagnostics.md).
The launch Ed25519 key stays fixed; OpenSSL verifies every permit. No caller
verification callback or `verified=true` is accepted. An internal cryptographic
RNG creates a fresh 256-bit nonce independently of guest random permission.

Every managed process begins in `awaiting_launch` (phase 8), inactive, with
installed and authenticated epochs both zero. Cold validation/compilation uses a
private real execution lease while public root admission remains closed. Before
any `_start` or approved business call, the supervisor must obtain fresh status
and send operation 9 with a nonce-bound launch permit. Before that handshake only
status, policy facts and launch are accepted. A missing handshake or disconnected
channel stops the CLI with exit 75; it never infers launch authority from a local
file, command-line epoch or an earlier process's permit.

The launch permit uses action 4, the actual image as both base and target,
base epoch zero, and nonzero target epoch, transaction ID and decision sequence.
Prepared, READY and change-declaration digests are all zero. It binds the fixed
issuer, current process nonce, actual measured contract and deployment scope.
The signed policy/revocation generations must be at least the configured baseline
(policy generation is nonzero). The target epoch and decision sequence establish
the supervisor's latest committed recovery watermarks; subsequent update epochs
and decisions must increase beyond them. The supervisor, which owns the durable
log, is responsible for asserting the latest decision and approving the concrete
recovery image and contract. The VM cannot independently discover a newer external
decision, and does not treat a new nonce as evidence of external freshness.

This launch schema always constructs fresh guest state from the approved image;
it does not restore an in-memory migration or private heap across process death.
The supervisor must explicitly approve that reset recovery. Runtime budget
counters are process-local: a supervisor enforcing a deployment-wide budget must
persist consumption and supply only the remaining allowance in the new measured
contract, together with current revocation generations. Neither a process restart
nor a launch receipt proves that previously spent device quota is available again.

After all runtime preflight checks, launch records these authenticated facts and
opens the retained admission gate once. A failure after authentication remains
inactive. Repeating the exact signed launch only returns current facts; it cannot
rerun `_start`, reset state/budgets, reopen a failed launch or interfere with a
later update fence. A changed launch permit cannot replace the recorded launch,
and its transaction ID cannot be reused by PREPARE. The initial worker's real root
lease is acknowledged before processing subsequent management requests.

The effective contract is SHA-256 of the approved external reference followed by
the VM's canonical effective-policy fingerprint. The latter includes the actual
module binding, exact interfaces, operations, limits, approved args/env and native
resource incarnations. Dynamic consumption, offsets and normal close operations
do not replenish the contract. Prepare, READY and publication recheck the actual
base policy. For a policy transaction the target reference and exact policy bytes
are signed; the VM computes the target effective contract and seals it into the
prepared measurement. COMMIT still authenticates against the captured BASE
contract. Successful publication reports the new contract; repeated COMMIT uses
the original base-bound permit and only confirms the existing fact.

The trusted supervisor verifies external artifacts, approves the concrete change,
owns stable deployment/component/device scope and persists the mutually exclusive
COMMIT/ABORT decision. A publisher signature, recommended policy or audit ID is
not local device authority.

## Transaction and lifetime

1. PREPARE authenticates the exact base, target, scope and declaration, and owns
   immutable target bytes. No later path read can substitute the artifact.
2. Actual maintenance ownership and root-admission fencing exclude another update,
   debug mutation or reset. Real execution leases drain; old callers finish on A.
   B and migration never read/write production mutable state while writers remain.
   New calls return busy, with no unbounded queue. A five-second drain deadline
   returns busy without fabricating completion. Closing admission does not cancel
   an admitted WASI/native quota or timer wait. If the five-second deadline expires,
   the private candidate is refused and unchanged A may resume admitting roots;
   the old invocation retains its actual wait. Genuine stop/proc_exit remains a
   separate cancellation decision with the configured per-operation exit policy.
3. Compilation, new immutable storage, policy strings, private initialized/migrated
   state and merged native metadata are prepared before publication. Actual compiler
   owners and source/native proofs remain retained. A private failure/cancellation
   may resume unchanged A; it does not reset authority or budgets.
4. READY seals prepared identity, actual base, nonce and the retained final state
   view. The same real fence remains owned until a matching authenticated decision.
5. After the external durable COMMIT is authenticated, the VM records the committed
   epoch before local publication. All preflight checks precede one bounded
   publication of policy, state, native metadata and all entries. No compiler,
   migration, native object creation or new allocation runs in that publication.
6. Installed identity and authenticated committed epoch are distinct. Only after
   actual admission reopening is the new epoch active. Failure remains
   `committed_but_not_active`, with no reopening of A. A repeated matching COMMIT
   does not repeat start, migration, reset, resource creation or budget charging.
7. ABORT can cancel only its own uncommitted READY candidate. A conflicting or
   late decision cannot resurrect a cancelled transaction or undo COMMIT.

Exclusive transfer covers direct, indirect, function-reference and tail-call
routes by draining every old chain before changing cells. It does not claim
isolated-epoch concurrency or native/DMA cancellation. If the manager disconnects
while PREPARED/READY/inactive, the CLI contains the process with exit 75 and does
not infer ABORT. Device watchdogs and the supervisor own safe output and durable
recovery. Restart uses a fresh nonce and the latest separately authorized recovery
deployment; code identity alone is not persisted runtime state.

## Wire format

Integers are unsigned little-endian. Requests are `UUP1`, operation u32, payload
length u32, then payload. The maximum is 64 MiB plus the bounded manifest and
permit. All reserved fields, unknown flags, inconsistent counts, trailing bytes
and malformed encodings are refused.

| Operation | Payload |
|---|---|
| 0 status | empty |
| 1 function/configuration prepare | permit + scope count u32 + sorted scopes u64[] + complete Wasm |
| 2 ready | empty |
| 3 decision | COMMIT or ABORT permit |
| 4 private cancel | empty; forbidden after READY |
| 5 approved business call | function index u32 + input length u32 + output length u32 + packed parameters |
| 6 stop | empty; only active with initial entry complete |
| 7 declared prepare | permit + manifest length u32 + manifest + complete Wasm |
| 8 policy facts | empty; current canonical policy and pending target-contract measurement |
| 9 managed launch | exact nonce-bound LAUNCH permit; retries are receipts only |

Scope bit 63 selects a mutable global reset; bit 62 selects an immutable global
binding. Both bits together are invalid. Remaining bits are the global index.
Untagged entries select local functions. Scopes must be unique and sorted.

The fixed 472-byte permit contains:

- 32-byte domain `uwvm.local-update.activation.v1` with trailing NUL;
- action byte (1 prepare, 2 commit, 3 abort, 4 launch), seven zero bytes;
- ten 32-byte fields: issuer-key SHA-256, nonce, transaction ID, actual base,
  complete target, BASE contract, prepared measurement, READY measurement,
  deployment scope, change declaration SHA-256;
- six u64: base epoch, target epoch, BASE policy generation, BASE revocation
  generation, expiry Unix seconds, external decision sequence;
- Ed25519 signature (64 bytes) over the preceding 408 bytes.

For operation 1, declaration SHA-256 hashes the exact LE64 scope list. For
operation 7 it hashes the exact complete manifest. PREPARE has zero prepared/READY
and decision sequence. A decision binds the returned measurements, same candidate
and a strictly increasing sequence. Expiry has a one-hour maximum future window.
Status can query previously recorded facts without an unexpired application permit.
LAUNCH follows the separate field rules above and shares the same expiry window.

`UUT2` has a 64-byte header: magic[4], flags u32, scope count u32, policy length
u32, target approved-contract reference[32], target policy generation u64, target
revocation generation u64; then scopes u64[] and exact policy bytes. Flags are
bit 0 policy change, bit 1 module reset, bit 2 module preserve. Reset and preserve
are exclusive. Without a policy change its reference/generations/length are zero.
An empty scope list is allowed only for a policy-only change with identical Wasm.

`UUT3` has the same first 64 bytes, with magic `UUT3`, plus migration length u32
and reserved-zero u32, then scopes, policy bytes and migration bytes. Flag bit 3
is required and denotes module migrate, exclusive with reset/preserve. It can
combine with bit 0. Migration is at most 3,600 bytes.

The `UWM1` migration encoding is magic[4], memory-copy count u32, global-copy count
u32, reserved-zero u32, followed by memory rows of five u64
(source memory, destination memory, source offset, destination offset, length),
then global rows of two u64 (source index, destination index). At least one row
is required. This declares exact bounded copies, not a programmable write API.

Interface exposure/call states are part of the policy measurement. The current
wire format includes one byte per registry entry after enabled/permissions/fd
limit: 0 hidden, 1 deny, 2 allow. Non-WASIp1 entries must be hidden in a WASI
policy. Truncated maps, unknown states and old wire versions are rejected; no
compatibility decoder is retained. A copied environment/checkpoint preserves
the exact map.

Canonical policy encoding begins with eight bytes `UWPOL5\0\0`, followed by
LE64 values: enabled, operation mask, fd limit; one u8 interface state per
entry in the complete host registry, in registry order; twelve operation
budget rows, each containing six LE64 limits (request bytes, window bytes,
window calls, total bytes, total calls, concurrent), then six u8 actions in the
same order (`0` reject, `1` synchronous wait), then six u8 guest-exit actions
(`0` cancel, `1` wait), and one u8 native-wait guest-exit action (same encoding);
four LE64 clock granularity values;
max poll subscriptions, max poll wait nanoseconds, max args/env bytes, filesystem
base rights, filesystem inheriting rights; argv count and length-prefixed UTF-8
strings, then env count and strings. `UINT64_MAX` means unlimited. Defaults are
unlimited with reject actions and cancel-on-exit actions; clock granularity
defaults to 1 ns. Descriptor counts are bounded by the WASI descriptor ABI range. Wait actions
are valid only for window-byte, window-call and concurrent-operation limits.
Guest-exit wait is likewise valid only for those three quota dimensions; the
separate native-wait guest-exit action controls actual provider waiting. Each
operation row occupies 61 bytes. With empty argv/env lists, a policy occupies
`852 + N` bytes, where `N` is the complete host-registry entry count (currently
217 entries and 1,069 bytes). Changing
either action requires a new revocation generation, as do other restrictions.

The management payload is at most 64 KiB; this is a control-frame bound, not a
runtime args/env quota. NUL, malformed UTF-8, duplicate environment keys and
unknown bits/actions are refused. The twelfth budget is filesystem I/O; the
preceding eleven indices retain their order. Older `UWPOL1` through `UWPOL4` payloads
are rejected. Budget order and permission/function bit identities are defined in
`environment/resource_limits.h` and `environment/environment.h`. Read operation 8
or use `current_wasi_policy`/`encode_wasi_policy` to preserve canonical fields.
Encoding/decoding alone does not authorize application.

Replies are `UUR1`, payload length u32, then: status, phase, active, connected
(four u32); installed epoch, authenticated commit epoch, decision sequence
(three u64); nonce, installed image, prepared, READY, transaction, contract, scope,
issuer (eight digests). These base facts occupy 296 bytes. Business results follow.
Operation 8 additionally returns pending contract[32], current policy/revocation
and pending policy/revocation generations (four u64), policy length u32 and current
canonical policy bytes. No module binding returns an empty policy. Pending fields
are cleared on cancellation or completed application. Enum values are in
`utils/update/protocol.h`.

## Verification

Development evidence and actual SDK-derived hosts live in
`.tmp/implementation-20261103`, under `memory.max=68719476736` and
`memory.swap.max=0`. Reports retain exact binary/source hashes, actual outcomes
and refused combinations. They include real signatures, native unwind, actual
old writer drain, cached WASI bridge revocation, immutable binding continuity,
module state transitions and invalid candidates. Results do not imply validation
on robot hardware, unknown providers or untested platforms.
