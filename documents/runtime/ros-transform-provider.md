# Bounded frame and time semantics

The nine `transform.*` operations are registered under `uwvm_ros_v2`, backed by
the `local-transform` binding. Canonical decoding, bounded history, owned query
sessions, retained allocation, clock-aware authority, CLI and read-only debug
integration are implemented. Core, CLI and actual host-carrier fixtures have
passed. The frozen VM has also passed 320 actual Wasm/CLI cases and three
live DAP sessions, as detailed below. This binding
does not implicitly subscribe to or broadcast native TF traffic.

## Coordinate and time contract

A transform maps a child-frame point into its parent as `R*p+t`. Translation
uses metres and rotation uses an `x,y,z,w` quaternion. The deployment supplies
finite translation extent and an explicit quaternion-normalization tolerance.
Nonfinite data, zero-norm quaternions and values outside those bounds fail
without modifying the output. Repair is limited to normalization within that
tolerance; there is no silent clamp of a physical displacement.

Timestamp absence means latest. Zero and negative nanoseconds are explicit
instants, and epoch zero is valid for an initial steady/system clock. A clock
identity cannot be zero. The enclosing provider must independently verify its
clock grant, actual clock resource, started state and current epoch. The
canonical decoder cannot establish those facts from guest-supplied numbers.
Security deadlines are positive absolute host-steady times.

Interpolation requires a complete inclusive interval in the same frame/epoch.
Integer difference arithmetic supports the full signed nanosecond range
without overflow. Translation is linear and orientation follows the shortest
spherical path, treating `q` and `-q` as one orientation. Missing samples never
cause implicit extrapolation or a silently shortened time window.

## Velocity semantics

Velocity is the derivative of the tracking body's pose relative to the
observation frame over a complete configured interval. Angular velocity uses
the principal relative rotation, whose angle is at most pi; sampling frequency
and angular-speed bounds must therefore rule out ambiguous faster rotations.
A hard speed bound refuses an invalid result instead of clamping it.

For a requested reference point, point velocity is body-origin velocity plus
`angular_velocity × displacement`, with both positions expressed in the
observation frame. The result's reference frame changes vector coordinates; it
does not implicitly subtract that frame's motion. This states the intended
physical meaning directly rather than copying implementation quirks of a
specific native TF version. Every contributing frame, point, sample interval
and source must pass the enclosing provider's explicit policy.

## Decoder and implementation obligations

The canonical decoder limits frame names to 255 UTF-8 bytes and publication
batches to 128 records. It rejects empty/NUL-containing frame names, unknown
operations, truncation, trailing data, invalid booleans, nonfinite coordinates
and publication records whose parent equals their child. Request extents are
bounded before carrier allocation. Quaternion tolerance, permitted frame
edges, source authority and static replacement remain semantic admission
checks, not wire syntax.

The cache reserves two complete fast_io-backed banks before activation. Both
include a maximum publication batch of temporary space; data and workspace
budgets cover them independently. Runtime publication, lookup and velocity
perform no heap allocation. A batch is validated and staged before one bank
swap, so rejection does not publish a prefix. History retains one predecessor
for interpolation at its configured boundary; that predecessor does not extend
the permitted query window. Capacity pressure refuses a batch rather than
silently shortening its history.

Frames and permitted directed edges are explicit, bounded deployment data.
The union of possible edges must be acyclic. Approved temporal reparenting is a
discrete change at the new sample time; interpolation never blends coordinates
from different parents. A topology requiring a cycle in that union needs a
separately configured profile. Queries may traverse only approved frames.
`available` must not disclose ungranted frame existence. Debug inspection must
observe cached metadata without sampling a clock or expiring a query.

`transform.publish` carries `key`, `buffer`, `expected_generation` and the
transform array. Zero generation allows ordinary additions and unchanged static
records. Actual replacement of a static record requires both the exact
`transform.static.replace` action and a matching nonzero generation. A nonzero
generation fences the entire batch even if all records are dynamic. Changing a
child between static and dynamic history requires an explicit clear.

One session-wide increasing mutation key covers publication, asynchronous
submission and clearing. Exact canonical SHA256 replay returns a retained
receipt without repeating the mutation or obtaining new clock access. Rejected
mutations do not consume the next key. A new accepted mutation acknowledges the
previous receipt. Query slots and results have finite, preallocated capacity;
failed or throwing delivery retains the result. Clear, close and retirement
fence in-flight delivery. A successful copy is the condition for consuming a
result, not merely entering the delivery callback.

Queries expire at absolute host-steady deadlines on access: submission,
publication, take, description or readiness observation. This local provider
does not promise an autonomous timeout thread or physical watchdog. Clock epoch
changes reject old history until an explicit clear acknowledges the current
live epoch. Opening and fresh operations use the actual runtime clock resource
under the complete origin/clock authority group. Revalidating an already-owned
clock does not consume its creation quota again.

Closing a session removes guest reachability immediately. Its data, workspace,
query and metadata slot reservations remain charged while an active operation
still holds the object. Carrier-copy bytes have a separate aggregate budget.
All owning storage uses recoverable fast_io-backed allocation; allocation
failure rolls back unpublished handles and preserves reusable capacity.

## Canonical definition tool

`tools/ros_scene/transform_definition.py` converts an explicit JSON topology
into the exact `transforms::configuration` definition. The JSON root has only
`identity`, `frames`, `edges` and `physical_limits`. Every field is required.
The identity is nonempty UTF-8 without NUL, bounded to 96 bytes. Frames are
explicitly sorted, unique UTF-8 names, each from 1 to 255 bytes; there must be
1 to 128 frames. Edges refer to those exact array indices. Each edge contains
only integer `parent` and `child` indices and explicit `dynamic` and `static`
booleans. At least one mode must be true. Duplicate edges, self-edges and a
cycle in the union of all permitted modes are rejected; at most 512 edges are
accepted. The encoder sorts edges by parent/child before writing, while keeping
the explicitly supplied frame indices unchanged.

`physical_limits` contains exactly `translation_metres`,
`quaternion_tolerance`, `linear_metres_per_second` and
`angular_radians_per_second`. All four values are finite and nonnegative;
quaternion tolerance is strictly less than one. Booleans and strings are not
numbers. No unknown or duplicate JSON field can select a path, grant, provider
or implicit limit. An example is:

```json
{
  "identity": "robot-frames",
  "frames": ["base", "map"],
  "edges": [{"parent": 1, "child": 0, "dynamic": true, "static": false}],
  "physical_limits": {
    "translation_metres": 1000,
    "quaternion_tolerance": 1e-12,
    "linear_metres_per_second": 10,
    "angular_radians_per_second": 5
  }
}
```

Invoke the tool with explicit paths:

```sh
python3 tools/ros_scene/transform_definition.py \
  --input frames.json --output frames.canonical --metadata frames.metadata.json \
  --environment plant --alias robot-frames
```

The tool reads at most 1 MiB of JSON and produces at most 65,536 canonical
bytes. Existing output files are refused. The canonical record contains text
magic `uwvm.ros.transform.definition`, identity, frame count and names, edge
count and four fields per edge, then the four physical `f64` values. Text uses
UTF-8 with a little-endian `u64` byte length. Counts, indices and canonical
booleans also use little-endian `u64`; booleans are exactly zero or one.
The profile SHA256 covers this entire record, including its identity. There
is no digest field inside the record and no recursive digest calculation.

Output metadata includes the full digest, exact byte count and two suggested
CLI argument arrays for `--ros-scene-transform-profile` and
`--ros-scene-transform-definition`. They are reviewable data and are never
executed by the tool. They contain no grant and confer no authority. Explicit
environment/alias names replace placeholders only; the deployer must separately
provide the actual transform/clock grants, aggregate budget and all thirteen
limits: `cache.samples`, `cache.samples-per-child`, `publish.batch`,
`cache.history-ns`, `publish.future-ns`, `velocity.window-ns`,
`session.requests`, `query.deadline-ns`, `sessions`, `budget.data-bytes`,
`budget.workspace-bytes`, `budget.request-bytes` and `budget.carrier-bytes`.
Those capacities and deadlines remain outside the logical definition.

The VM's bounded configuration-file helper checks the exact file length and
full SHA256 on a pinned regular inode before policy parsing. Final symlinks,
FIFOs, sockets and devices are rejected before data I/O. The parser/freeze
checks remain authoritative even when the file came from this encoder.
Parent-directory selection is still the trusted deployer's responsibility;
this configuration read does not create guest filesystem authority.

`transform-definition-tool-first/results.json` in the SDK
`.tmp/ros-scene-security` directory records 69 successful commands using
`-O3 -g` inside the existing 64 GiB zero-swap cgroup. The retained 406-file
source closure stayed unchanged. The encoder refused 82 malformed JSON cases;
Clang ASan/UBSan and GCC13 each decoded, froze and exactly re-encoded four
valid files, including 255-byte UTF-8 names and the 128-frame/512-edge boundary.
Both independently refused 28 authentic-digest malformed canonical files.
The eight nested CLI invocations also checked exact output, metadata and
existing-output refusal. This qualifies configuration tooling, not native TF
transport or runtime performance.

## Current numerical qualification

`transform-math-upstream-first/results.json` in the SDK `.tmp/ros-scene-security`
directory records Clang ASan/UBSan and GCC builds with **`-O3 -g`** inside the
existing 64 GiB zero-swap cgroup. Each executes four boundary groups and 20,000
deterministic comparisons against the pinned upstream Humble `tf2/LinearMath`
headers for composition, inverse and spherical interpolation. The exact source
closure is retained beside the receipt. Analytic velocity tests cover rigid
rotation, translation, point shifts and coordinate changes; they do not claim
equivalence to native velocity-window clipping behavior.

This is numerical and carrier qualification, not package portability, native
TF transport, latency parity or industrial safety certification.

`transform-cache-native-generation/results.json` records six complete Clang
ASan/UBSan and GCC compile/run commands. Each variant performs six cache
boundary groups and **6,001** comparisons against the retained native Jazzy
`libtf2`: ordinary lookup, advanced two-time fixed-frame lookup and latest
common time. Both headers and native libraries are pinned beside the receipt.
Exact zero time and the new explicit safety limits are tested separately;
native tf2 treats zero as its latest sentinel. An earlier incomplete native
cohort stopped at upstream deprecation diagnostics, another at a transitive
library loader path; neither is reported as a passed cohort.

`transform-queries-generation/results.json` records seven query security groups
under Clang ASan/UBSan, GCC and TSan, including lost output, exact replay,
deadline boundaries, reentrant clear, concurrent retirement and fork rejection.
`transform-registry-second/results.json` adds six registry groups for retained
closed objects, carrier reservations, actual allocation failures and process
identity. These receipts pin their precise source versions. The dispatcher and
subsequent central integration require their own fresh qualification.

## Registered VM, CLI and debugger qualification

`transform-source-central` retains 3,451 production files and an exact source
manifest. `vm-transform-central/full-build-results.json` records the complete
VM SHA256 `9a816d916ce3589a98c4cda6c9ad04030ff288ae54bb2c8d7bceb97ef11ea8ad`.
All three translation units use effective `-O3 -g`; `.debug_info` and
`.debug_line` remain in the executable. The source delta overlays only the
new scene/CLI/debugger integration on the previous model cohort, preserving its
older ROS/WASI and runtime sources. It does not qualify unrelated live changes.

Receipts are retained under `../uwvm2-ros-sdk/.tmp/ros-scene-security`:

- `transform-deployment-second`: nine ASan/UBSan, GCC and TSan commands, covering
  complete mandatory limits, aggregate reservations, all-environment freeze
  rollback, shared request routing, source revocation and owned resource close.
- `transform-cli-first`: actual CLI callback compile/link/execution, including
  malformed numbers/digests/hex, duplicate/late settings and no implicit exposure.
- `transform-host-frozen`: three actual host-carrier fixture commands, including
  recoverable malloc-family failure, denied/short-output preflight, partial
  reads, disallowed result-length overlap, exact retry and all nine operations.
- `transform-modules-{llvm,interpreter}-frozen`: 22 commands each, with full
  `-O3 -g`, frozen include roots and all nine transform FP policy assertions.
- `transform-vm-final`: 156 generated Wasm files, 320 executed cases across
  memory32/memory64, memory indices zero/one and interpreter/LLVM. These include
  explicit zero authority, all nine operations and shared request calls,
  interpolation/velocity, static generation checks, real deadlines, source-clock
  closure, epoch clearing, tagged pointer aliases and wrong-tag rejection.
  Each successful FP guest checks 115 host-return boundaries.
- `transform-dap-first`: full/read-only/limited live sessions. A pending query
  remains pending across repeated inspection after a real 50 ms deadline;
  inspection does not expire it. Stale-stop and payload-disclosure rejection
  are checked using the independently frozen matching debugger tool schema.

The first host attempt had a fixture field-name compilation error; the second
passed execution but its broad live-source hash set changed during independent
communication work. Both remain incomplete historical receipts. The frozen
rerun above supersedes them. An initial module run was withheld because its
inherited recipe still selected live include roots and old optimization/debug
flags; the two fresh frozen runs above use verified effective `-O3 -g`.

On the same VM, 54 buffer and 75 complete-publication regression cases and
native/real-Wasm XML output checks pass. These tests establish the recorded
local contract; they do not certify an industrial control system, native ROS
transport integration, arbitrary hardware timing or complete package portability.
