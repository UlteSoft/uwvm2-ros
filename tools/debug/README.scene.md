# u2rosv2 robot scene inspector

`uwvm.openScene` and the DAP request `uwvm/sceneState` expose a detached,
read-only view of process-owned scene authority and resource metadata.

The console spelling is:

```
info scene authority|resources [environment=ID] [alias=ID] [first=N] [count=1..8] [if-stop=N]
```

The VM accepts the request only after ordinary debugger inspection permission,
a complete cooperative Wasm stop, and an unchanged stop identifier before and
after the bounded copy. Native instruction stops, lock contention, a retired
stop, malformed identifiers and scan limits return an unavailable result. The
hook uses nonblocking locks and scans at most 4096 records.

Rows contain copied logical labels, diagnostic IDs and counters. They never
contain provider calls, message bytes, filesystem paths, credentials, URLs,
native pointers, capability handles or grant/revoke operations. `id` is a diagnostic
identity and cannot be used as a capability. The DAP decoder keeps labels as
both display text and exact base64 bytes and rejects truncation, duplicate
fields, inconsistent pagination, stale stops and unknown fields.
`admissions` counts charged admissions; `refused` counts optional-budget
rejections at the charging admission step. Output preflight refusals and
permission denials are not included in that budget counter. Preflight checks
change no window or counter state. A
metadata snapshot is not a complete security audit log.

The Resources page includes immutable input artifacts as `type="artifact"`.
Their `size_bytes` is the registered byte length and `immutable=true` records
the admitted immutable-input guarantee. Both values are copied from stored
metadata: inspection does not call `artifact.open`, `artifact.read`,
`artifact.describe`, filesystem operations or digest computation. Artifact
contents, expected digests and native file identities are excluded. Other
resource and authority rows carry `size_bytes=0` and `immutable=false`.
The size remains an exact decimal string through DAP and the IDE, including
values beyond JavaScript's safe integer range. The decoder requires both
fields and rejects inconsistent artifact metadata; it does not accept the
older incomplete row schema as a fallback.

Volatile cache outputs use separate resource types `artifact.output` and
`artifact.receipt`. Live output rows have `state="staged"` or `"published"`,
`accepted_bytes` for their actual content length, `reserved_bytes` for the full
retained allocation, and `retained_receipts` of zero or one. Publication keeps
the complete begin reservation until removal; unused payload capacity is not
reported as refunded while its heap allocation remains retained. Publication
is session visibility only: `published` is true exactly for state `published`
and `durable` is always false for the volatile provider.

A receipt-only row carries state `none`, `aborted` or `removed`, zero reserved
payload bytes and one retained receipt. `none` has no object identity or
accepted bytes; terminal abort/remove rows retain their former diagnostic
object identity and accepted length. These rows expose that a result remains
accounted even after the live output is gone. Reading the inspector never
acknowledges, forgets or replays that result and cannot replenish receipt space.
Other resource/grant rows have zero/false output metadata. Immutable input
`artifact` rows keep their original separate byte-size and immutable fields.

Managed lifecycle bindings use `type="lifecycle"`, `binding="local-lifecycle"`
and `id=0`: each row represents a configured logical binding, while
`open_handles` counts its retained client/server handles. `state` is one of
`unconfigured`, `inactive`, `active`, `finalized`, `configuring`, `cleaning_up`,
`shutting_down`, `activating`, `deactivating` or `error_processing`.
`generation`, `transition`, `deadline_ns` and `status_code` remain exact decimal
strings. `callback_pending` distinguishes an intermediate state, and
`callback_delivered` records whether its callback was taken. Request identities,
retry keys and native nodes are excluded.

Lifecycle inspection copies cached fields using `managed_nodes.inspect()`.
It never calls the guest-facing `lifecycle.state` or `lifecycle.transitions`,
samples a clock, delivers a callback, or enforces a deadline. In particular,
a callback can remain visibly pending after its absolute steady deadline while
the guest is stopped; the next admitted lifecycle operation enforces expiry.
Finalized status `110` means an observed timeout and `125` means cancellation;
logical finalization is not a claim that a physical robot has stopped. DAP
independently rejects invalid state/transition/deadline combinations, unknown
status values, and lifecycle fields on any other row. The IDE offers no
open/state/transitions/submit/take/respond controls.

The console producer validates every row before emitting any page row; DAP
independently enforces the same closed states and field invariants. Output
counters remain decimal strings through JSON and the IDE. Unknown/missing or
duplicate fields, impossible publication/durability combinations, and payload,
path, descriptor, request-key or digest fields are rejected. The inspector adds
no begin/append/commit/abort/remove/receipt controls; all displayed fields are
cached metadata under independent debugger inspection permission.

The IDE's Robot scene window has Authority and Resources pages. Refresh and
pagination issue only `uwvm/sceneState`; no scene write, model invocation,
transport poll or queue consume is available from this surface. The existing
ROS and WASIp1 inspectors retain their separate contracts.
Artifact open/read/describe remain separately authorized guest operations;
debugger inspection never grants them or consumes their admission budgets.

Scene inspection is not a checkpoint/replay adapter. The resumable runtime
classifies the actual resolved scene import as an unadapted host effect and
marks the current capture non-replayable before dispatch. Restoring Wasm memory
alone must not recreate capabilities, replenish quotas, repeat an external
effect or revive a retired runtime generation. Scene resource serialization
and transactional restoration are not currently implemented. Record/replay
provider operations in the semantic catalogue are a separate planned feature.

For focused parser, serializer, DAP and editor-controller checks, run:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 test/0084.ros-runtime/run_scene_debug.py \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/debug-contract
```

The live Linux acceptance driver uses the current VM executable, a real
authenticated broker and a separate DAP process. It checks full/read-only/
limited debugger permissions, nonconsuming resource snapshots and rejection
after a stop retires. Use a fresh output directory:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 test/0084.ros-runtime/run_scene_dap_live.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-build/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/debug-live
```

`run_scene_lifecycle_dap_live.py` uses the same authenticated VM/broker/DAP
route for eleven managed bindings plus a steady clock. Guest assertions build
all ten lifecycle states, queued and delivered callbacks, and a pending
callback whose deadline passes before the debug stop. Repeated metadata reads
must preserve every state, callback, handle and admission counter. It tests
full/read-only/limited inspection, twelve-row pagination, privacy and retired
stop rejection. `--prepare-only` compiles guests but does not qualify the VM.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 test/0084.ros-runtime/run_scene_lifecycle_dap_live.py \
  --runtime PATH_TO_CURRENT_VM \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-dap-live
```

`run_scene_lifecycle_boundaries.py` exercises all six lifecycle carriers in
184 actual VM executions: both engines, memory32/64 and memory index 0/1.
Request, result and result-length tag faults are tested separately for every
import. A trusted diagnostic-mode fixture uses different tagged encodings of
the same physical spans and checks unchanged canaries, no retained handle,
and no transition/callback effect before a successful same-key operation.
Independent short-result, exact-retry, denied-import and missing-grant cases
execute dynamic Wasm arithmetic immediately after each host return. This is
software MTE boundary coverage, not hardware MTE or native ROS callback proof.
It accepts `--runtime PATH --output DIRECTORY` and `--prepare-only` under the
same mandatory cgroup runner.

The immutable-input acceptance driver additionally runs the guest's actual
artifact open/read/describe imports and verifies their returned bytes and
content identity. It checks mixed guard/artifact pagination, absence of private
provider data and unchanged admission counters across repeated inspection.
Full permission uses a Wasm breakpoint; read-only and limited permissions use
an independently configured `--debug-fault-action pause` after guest work.
The read-only observer must inspect that real stop while remaining unable to
continue execution. Limited permission must refuse the metadata query.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 test/0084.ros-runtime/run_scene_artifact_dap_live.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-artifacts-safe/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/debug-artifact-live
```

These drivers check the existing 64 GiB, zero-swap gate and record source
hashes and command outputs. The editor test uses the real controller with a
transport fixture; it does not qualify a graphical VS Code session. The live
driver qualifies the local scene ABI and debugger, not native ROS providers.

The artifact host-boundary driver complements inspection with 48 actual VM
executions across interpreter/AOT, memory32/memory64 and memory indices 0/1.
Eight executions run dynamic Wasm floating-point arithmetic after every host
return, covering rounding, subnormals, infinities and NaNs. Forty executions
exercise valid tagged memory, physically aliased request/result-length and
output/result-length spans with differing tag encodings, and default tag
mismatch crashes for each artifact import. Alias rejection must preserve
canaries, requests and the single-handle budget; a disjoint tag-zero length
slot is a positive control in the explicitly selected diagnostic mode.
The crashing cases disable OS core files and require the expected fault
classification, pointer, memory index and tag evidence.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 test/0084.ros-runtime/run_scene_artifact_boundaries.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-artifacts-safe/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/artifact-fp-memtag
```

Use a fresh output directory inside SDK `.tmp`. The receipt retains generated
guest hashes, input/runtime hashes, cgroup events and command diagnostics.
`--prepare-only` validates guest generation without executing the VM and must
not be cited as completed runtime qualification. These checks do not establish
native ROS performance parity or hardware memory-tagging coverage.

The volatile-output live driver verifies the actual guest's begin, append,
commit, abort, remove, status, receipt and published-byte read operations before
inspection. Separate bindings retain staged/published objects, aborted/removed
receipts and a failed-begin receipt with no object. Four guard resources bring
the mixed snapshot above the eight-row page limit. Repeated full-page and
single-row inspection must preserve the complete receipt metadata, capability
handle counts and admission counters. Full permission stops at a real Wasm
breakpoint, read-only uses an independently configured fault pause and cannot
continue, and limited permission cannot read the snapshot. Retired stop tokens
must fail. Private bytes and request keys never appear in inspector records.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 test/0084.ros-runtime/run_scene_output_dap_live.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-output-final/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/debug-output-live
```

The output host-boundary driver executes 120 cases across interpreter/AOT,
memory32/memory64 and memory indices 0/1. Every output wrapper and published
read gets dynamic floating-point checks after successful, denied and
ungranted returns. Matching tags, normalized aliases with unchanged canaries,
and default tag faults are independent cases. All eight imports get a
result-length tag fault; append additionally gets request and output faults.
The receipt and ordered mutation sequence must still be correct after each
rejected alias. Default crashes disable core files and must identify the
expected memory, pointer, tag pair and memory-protection fault classification.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 test/0084.ros-runtime/run_scene_output_boundaries.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-output-final/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-fp-memtag
```

Both output drivers accept `--prepare-only`; preparation alone is not runtime
qualification. They require a fresh SDK `.tmp` directory, retain hashes and
cgroup events, and qualify the bounded volatile cache only. Session publication
does not establish persistence, ROS-provider support or native performance
parity.

The seven volatile-cache and sixteen local scheduling wrappers use a native
register-control guard before any callback work, including caller identity,
activation and enrollment. Nominally integer-only code is not proof of FP
preservation: standard allocation may invoke a `std::new_handler` that changes
rounding or other controls before returning or throwing. The local guard
restores rounding, exception masks, subnormal controls and precision on every
returning path. Only LLVM targets with the existing native control guard
advertise `preserves_wasm_control` and replace the complete outer runtime guard.
Interpreter-only builds and unsupported targets retain `may_modify` and that outer
guard. Calls through arbitrary imported-memory callbacks also retain their
nested runtime FP guards. Immutable `artifact.open/read/describe` and every
managed lifecycle carrier remain `may_modify`. No native provider inherits
the local whitelist.

`run_wrapper_allocator_fp.py` calls the concrete wrapper tuple with the
production C++ runtime recipe. One build uses the default standard allocator
and lowers its child process address-space limit to trigger a real hostile
`std::new_handler`: both throwing and freeing storage before returning must
restore controls. A separate injection build targets first caller enrollment,
`guard.create`, output begin and append request allocation. The cgroup limit
is never raised or bypassed. Runtime identity and checked memory in this
focused wrapper test are fixtures; actual VM tests remain separate.

`run_scene_output_fp.py --mode core` tests the real broker/provider without an
outer FP guard under Clang ASan/UBSan and GCC 13, using 48 control profiles.
`--mode wasm --runtime PATH` separately checks arithmetic after actual host
returns in 24 interpreter/AOT, memory32/64, memory-index 0/1 executions,
including exact mutation replay, denied imports and missing grants. These are
distinct tests: core tests exercise hostile host control settings with an
injected throwing allocator; they do not qualify arbitrary standard allocation
handlers or justify an unguarded wrapper. The actual-wrapper new-handler
regression separately exercises that failure path. VM tests check actual
wrapper integration. Use a fresh output directory under
`../uwvm2-ros-sdk/.tmp/` and the existing cgroup runner for either mode.

The focused host preflight fixture invokes the production wrapper, deployment,
authority and provider with controlled runtime identity/memory APIs. ASan/UBSan
and an injected large-allocation failure distinguish refusal before copy from
an attempted allocation. It checks action denial, invalid capabilities, zero
call/byte budgets, per-binding transfer overflow and undersized receipts.
These paths must leave request-read and large-allocation counters at zero.
A sufficient receipt positively reaches the injected allocation failure;
retrying afterward must retain the original first mutation key. This fixture
is separate from the real VM's memory-tag tests.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 test/0084.ros-runtime/run_scene_output_host.py \
  --compile-argv ../uwvm2-ros-sdk/.tmp/ros-scene-security/modules/graph-flags.argv.json \
  --link-argv ../uwvm2-ros-sdk/.tmp/ros-scene-security/modules/link-flags.argv.json \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-host-preflight
```

The historical output-only Linux acceptance receipts under
`../uwvm2-ros-sdk/.tmp/ros-scene-security/` use
`vm-output-final/uwvm2-ros`, SHA256
`eb758b5d24c4e755f67f35204248b97d89562a0991fcc2d1549b0cf26ea2d64d`.
This build predates the managed lifecycle metadata and LLVM output whitelist
described above; these receipts qualify that exact historical executable.
`output-boundaries-final/results.json` records all 120 output FP/memory-tag
executions passing. `output-dap-final/results.json` records full, read-only and
limited permissions passing, including the nine-row mixed snapshot, five
transaction states, repeated nonconsuming inspection and stale-stop refusal.
The same binary also passes immutable-input regression in
`output-input-boundaries-final/results.json` (48 executions) and
`output-input-dap-final/results.json` (all three permission modes).
Each runtime receipt records unchanged input/runtime hashes through completion;
all recorded memory-event counters remain zero in the 64 GiB, zero-swap gate.

`output-host-preflight-r2/results.json` separately records the focused
ASan/UBSan host preflight test. `modules-output-final-llvm/results.json` and
`modules-output-final-interpreter/results.json` each record 22 passing commands
with unchanged tracked sources, including all seven output wrappers' complete
FP-guard policy in header and isolated-module form. These isolated module
checks do not claim qualification of the complete production module graph.

`output-debug-contract-final/results.json` records eight passing frozen-source
commands: Clang ASan/UBSan and GCC 13 formatters, strict DAP parsing, the real
scene editor controller, and editor regression/lifetime checks. Source hashes
remain unchanged throughout, and all recorded cgroup memory-event counters
remain zero. The editor checks use a transport fixture; graphical VS Code and
hardware MTE remain outside these acceptance results.

The host-fixture receipt is separate evidence from the frozen VM build. Its
recorded `uwvm_runtime.h` SHA256 is
`c9a5fa8393c8655bd1f0aa31f39d8b71049e26a605501fc5d34f079aea587184`,
whereas `output-source-final` freezes
`49568713fc26298c9f077705af101e27d5f3657b667604883420d90ee60c329e`.
The exact recorded header was reconstructed and hash-verified in
`output-debug-input-audit/`; its differences concern checkpoint/coredump and
WASIp1 snapshot declarations. The fixture's runtime identity and checked-memory
API declarations are byte-identical in the recorded, frozen and audited
workspace headers, and its scene source headers match the frozen build.
Both final module receipts' tracked scene hashes also match the frozen build.
This audit does not establish complete transitive infrastructure identity:
module receipts pin scene sources and fixtures, not every included runtime
header. The four actual-VM acceptance receipts above qualify the stated frozen
executable independently of this focused host-fixture limitation.

The later lifecycle/control-guard qualification uses separate receipts under
the same SDK `.tmp/ros-scene-security/` root. `lifecycle-debug-contract` records
eight passing formatter, DAP and editor-controller checks for the expanded
closed schema. `modules-lifecycle-fp-guard-llvm` and
`modules-lifecycle-fp-guard-interpreter` each record 22 passing commands,
including header/module policy assertions and the empty fallback guard type.
`wrapper-allocator-fp` records six passing compile/link/run commands for the
real standard new-handler and deterministic first-enrollment/allocation tests.
These receipts preserve their own before/after cgroup counters and source
hashes; they do not by themselves establish actual VM performance or complete
production-module-graph support.

Buffer bindings use `type="buffer"`, `binding="sealed-buffer"`, `id=0`.
`members` counts retained objects, `open_handles` counts live guest handles,
`reserved_bytes` counts retained payload, `lease_count` counts read-only pins,
and `workspace_bytes` counts detached read copies. Closed handles can retain
objects through pins. All counters remain exact decimal strings in DAP/IDE.
These are binding totals, not individual object state or a capability. The
registry is inspected with a try-lock; contention withholds the complete page.
Inspection cannot read buffer data, schema identities, digests or native addresses,
allocate a lease, hash a buffer or alter sealing. Other row types require the two
buffer-specific counters to be zero. The editor issues no buffer or publication
operations from this metadata view.

Model bindings use `type="model"`, `binding="local-model"`, `id=0`.
`open_handles` counts retained sessions; `members` counts retained requests.
`input_bytes` counts pinned input bytes, `output_bytes` and `output_items`
count reserved output capacity, and `workspace_bytes` counts carrier/job
workspace. A terminal request still held by a native worker may keep these
reservations after the guest releases its receipt. These are cached aggregates,
not a worker poll or a claim of successful inference. Prompt/output content,
profile digests, request keys and worker PIDs are absent. The formatter, DAP and
IDE preserve exact decimal counter strings; model-only fields on other rows
are rejected. The model inspector cannot issue any model/request operation.

Private log rows use `type="log"`, `binding="private-log"`, `id=0`.
`members` counts retained records; `reserved_bytes` counts fixed store storage.
`emitted_records`, `exported_records`, `lost_records`, `retained_bytes`,
`copy_failures` and `refused` are cached counters. `counters_saturated` reports
saturating refusal/copy-failure accounting; accepted/exported/lost counters
remain exact. `busy` denotes an outstanding detached drain. No logger, message,
location, request key or record identity is included. Diagnostic rows use
`type="diagnostic"`, `binding="local-diagnostic"`, `id=0`, cached row/storage
counts, `pending_page`, `busy` and `retired`. They expose no counter contents,
revision, cursor, expiry or source payload. These fields are rejected on other
row kinds. Formatter, DAP and IDE preserve exact decimal integers and booleans.
Both inspection paths use try-locks and never capture, drain, expire or grant.

Private recording rows use `type="recording"`, `binding="private-recording"`,
`id=0`. `open_handles` counts retained writer and checkpoint handles; `members`
counts sealed checkpoints, `retained_bytes` their complete MCAP bytes,
`reserved_bytes` the private store reservation, `sample_count` current segment
messages, `query_count` retained replays and `lease_count` object pins. State is
cached `active`, `idle` or `retired`; no durable-storage claim is made. Replay
rows use `type="replay"`, `binding="private-replay"`, with their own cursor
handle count in both `members` and `open_handles`, plus their reservation.
Object IDs, cursor positions, keys, digests and recorded bytes are absent.

Sensor rows use `type="sensor"`, `binding="bounded-sensor"`, `id=0`.
`open_handles`/`members` are live/retained source handles. `sample_count` counts
queued records, `retained_bytes` their payload, `reserved_bytes` source storage
and `lease_count` captures. `input_bytes` and `output_items` are native ingress
byte/frame counters, not model output. Loss, refusal, saturation, workspace and
busy metadata remain cached. State is `active`, `idle`, `failed` or `retired`,
with the cached native failure in `status_code`. Inspecting never reads the
descriptor or consumes a sensor frame. Native transport workspace is a separate
reservation; the source byte counter does not claim total process memory.

Proposal rows use `type="model.proposal"`, `binding="model-proposals"`, `id=0`.
`members` counts retained proposals, `lease_count` readers, `sample_count`
captures, `query_count` tool definitions and `workspace_bytes` detached scratch.
State is `active`, `idle` or `retired`. No prompt, proposed arguments, source
identity, approval token or target state is disclosed. Inspection cannot approve
or execute a proposal. These four new closed row schemas preserve exact decimal
integers through the formatter, DAP and editor, and reject all payload/effect
messages in the metadata controller.

## Design metadata and callable capability lists

The complete u2rosv2 design is documented in
[`ros-operation-reference.md`](../../documents/runtime/ros-operation-reference.md).
Its generated `operation_design.h` descriptors are documentation metadata, not
host registration. Debugger/IDE capability menus must use actual registered
imports and configured support. Planned device/control/media/bridge/component
entries must not appear as available commands merely because a descriptor exists.

Those providers' future inspection records are restricted to cached state,
profile/generation, finite counts, deadline/reason and exit/stop evidence. Debug
inspection cannot perform native polling, invoke plugin callbacks, reveal payloads
or credentials, renew a lease, reconcile an unknown effect or rearm a latch.



## u2rosv2 child platform configuration

Device and control rows now include the canonical `platform` module label and
`platform_enabled` startup selection. An empty label requires false. These
fields describe a child of the same u2rosv2 environment; they never indicate
that a device grant is active. A selected child may still have a revoked parent.
The strict console/DAP schema refuses malformed or truncated module names,
child metadata on other resource types and switches without an owner. The
IDE displays the cached fields and explains that enabling a module does not
grant device authority. Inspection provides no enable, register or invoke action.
See [platform extensions](../../documents/runtime/ros-platform-extensions.md)
for the common dispatcher and current vendor-registration boundary.

Registered-message resource rows expose the frozen `clear_after_copy` Boolean.
It is `true` by default. `false` records the trusted deployment's explicit
choice to retain input bytes in sender memory; the IDE displays a residual-data
warning. Broker storage and delivered regions still require lifecycle cleanup.
This field is read-only: neither console nor DAP/IDE can change it. Inapplicable
row kinds carry `false` and cannot claim an input-clear policy. Inspection does
not read payloads, consume slots or change any route's erasure behavior.

## Externally provisioned driver sessions

Driver resource rows are detached observations. States are prepared, handshake,
ready, pending, result, closed and failed. Sequence identifies the last submitted
request; status_code is an application result; query_count=1 records uncertainty.
No row contains PID, FD, payload or native path. Inspection performs no I/O,
restart, termination, lease refresh or physical safety determination. The IDE
footer explains this boundary. Component rows remain parseable for archived
packets; current imports do not expose component process lifecycle.

## Completed host calls

While stopped with full debugger permission, run `trace host on family=rosv2`.
Continue the guest, stop again, and use `trace host read family=rosv2` or
**Recorded calls** in the VS Code Robot scene inspector. A read-only session
can read existing records. Neovim accepts `:UwvmHostTrace rosv2`; JetBrains
provides the same filter in its Host calls panel. DAP clients can request
`uwvm/hostTrace` or `uwvm/exportHostTrace` with `family: "rosv2"`. The export
writes a private JSON file on the debug host and requires a stable cooperative
stop. Reading or exporting the journal leaves recording and guest grants unchanged.

Each entry records the registered namespace, import name, sequence and raw
32-bit return value at completion. For example, the `abi` import returns the
ABI version. Interpret other values using the named operation's contract.
Arguments, capability handles, guest data, caller identity and timing are
unavailable for this family. A zero in an unused transport field carries no
identity. The journal does not attest remote delivery or physical effects.

At most 4096 ROSv2 completions are retained between journal clears. A busy
journal, allocation failure or exhausted limit can discard an observation.
The cumulative `rosv2-dropped` field reports losses, including after a clear.
`trace host off family=rosv2` stops collection. `trace host clear` clears all
families. Older ROS1 and ROS2 host imports use the separate `ros1` and `ros2`
filters. This debugger export carries no replay or restore authority.
