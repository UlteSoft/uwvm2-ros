# Source39 native74 retry after the second Linux boot

This directory contains a **source candidate** for a fresh native acceptance
build. Its presence, syntax checks, the retained SDK, and any old report do not
establish a successful build or ROS behavior. Run it only after independent
source review, completed current-boot content requalification, and actual
staging of all seven-field control pins. The templates intentionally contain
`null` for facts that require those future physical opens; they fail closed.

The selected implementation remains the complete original Source39 manifest
`54d6c79978e36bd13fa37272d8dd5f2cc94a9d2e62ac437b51dfc4aa4f6ae5b9`.
The external owner/helper inputs are separately reviewed and physically pinned.
This does not relabel Source39 as a later implementation freeze.

## Retained inputs and fresh outputs

The SDK source is exclusively the preserved original failed-r1 COPY at
`source39-native-gateways-physics-peer-r1/rootfs`. Its old exporter failed after
72 TUs; that failed batch remains failed. The r2 and interrupted r3 rootfs or
objects are not selected. All **74** Gateway, portable-peer, gate, String
identity, typed-witness and Go2 stand-controller TUs must be compiled into new
r4 object paths. Preexisting native interface libraries and generated registry
source retain their explicitly admitted historical provenance.

Fresh directories use the `-r4` suffix for controls, native products and the
outer supervisor. Old raw reports, sources, dependency files, logs, binaries,
SDK files and historical configuration are never edited. No second SDK COPY
is created. The retained SDK is mounted read-only in a new private mount
namespace, and original existing bind destinations are required. Writable
`/owned`, `/tmp` and `/root` bind only the fresh r4 output directories.

The original mapped-stop exporter, ELF decoding, compiler-input/full-MD
qualification, File API source bijection, loader/RPATH policy, linker-input
trace and pidfd/mount cleanup functions remain unchanged from the recovered r3
source. `source-ready.json` identifies those exact unchanged function bodies.
The exporter fix has not been proved by authoring this candidate; it still
requires its actual r4 execution and complete mapping witness.

## Memory-aware parallel compilation

The cgroup remains the original restarted container's current leaf with
**64 GiB memory.max, zero swap, and the existing 60 GiB working ceiling**:

* Boot `ec0d5f0f-3060-4220-bee2-2a7791ed4e72`.
* Cgroup device `29`, inode `18315`, effective CPUs `0-31`.
* The exact original Docker scope path is recorded in both templates.

Every fresh object batch requests at most **four** jobs and reserves **5 GiB
per heavy job**. The separately pinned shared planner further reduces that
count using the unchanged 60-GiB working guard, actual host `MemAvailable`
minus 8 GiB, current coordinator affinity, the admitted cpuset and all visible
finite ancestor CPU quotas. If only one reservation fits, the batch uses one
job. If no reservation fits, it does not launch that batch.

The compiler command's aggregate owned descendant RSS ceiling is **20 GiB**;
the outer owner permits **24 GiB** for the child coordinator and compiler tree.
The producer's own coordinator limit remains 2 GiB, and the outer metadata
coordinator retains its existing limit. Root must additionally retain its
explicit parent-owner budget, normally 2 GiB. None of these values changes the
cgroup's hard limit, swap limit, working reserve or membership requirements.
All existing finite deadlines, backing-filesystem reserve, log bounds and
exact-owned cleanup remain active.

The original code invoked Make separately for each object. Merely changing its
`-j1` token would therefore provide no compilation parallelism. The new worker
selects a bounded chunk of exact, absent target-object paths and passes those
real generated Make targets to one `make -jN --output-sync=target` command.
The synchronization groups each target's raw output; it does not synthesize a
compiler command. The unchanged evidence parser must still observe every real
compiler argv and match all configured sources, objects and full dependency
files bijectively. It rejects ambiguous directory events or malformed output.
The final CMake dependency/link step uses one job after all selected target
objects exist. Single-TU native CMake targets also use one job.

Each launch-time plan is retained under `build_job_plans`, including its
actual memory observations and every restricting cap. A planned cap is not a
claim that four compilers ran simultaneously. Real command completion and
qualified fresh objects are still required. No other producer batch runs
concurrently in this protocol.

## Requalifying retained content after reboot

A reboot changed the artifact device number from `81` to `68`, the persistent
host device from `66307` to `66308`, and the cgroup incarnation. Historical
seven-field pins and raw cgroup records remain historical. The code never
constructs an old-looking physical pin or recursively rewrites a report.

`make_restart_selection.py` is a bounded metadata-only selector. Run it as an
owned current-cgroup actor to create a fresh configuration, for example:

```sh
python3 -B /reviewed/controls/make_restart_selection.py \
  --out /var/tmp/uwvm-ros-unitree-20261016-r1/native74-r4-restart-selection-r1
```

The selector opens the exact recovered r3 configuration, its three original
provider reports, and the two preserved failed-r1 reports. It emits all known
historical pins directly from those retained bodies. The six complete trees
are:

1. The original execution-view Humble source SDK.
2. The preserved original r1 SDK COPY.
3. The complete native `unitree_go` derived tree.
4. The complete native `unitree_hg` derived tree.
5. The complete native `unitree_api` derived tree.
6. The complete GO+API composed provider tree, selected through the actual
   retained generation/catalog association.

Host tool roots do not require comparison with an old whole-root catalog:
the native build already creates fresh current before/after inventories for
all configured host roots. The selector also explicitly includes the selected
old host executable pins and the original host-tool metadata report. It never
uses the interrupted r3 native report as SDK, object or execution qualification.

Next, Root invokes the separately reviewed `requalify_after_restart.py` with
the emitted `restart-selection.json`, its actual SHA-256, the same Source39
root/manifest, and a new output directory. Only a terminal
`CONTENT_REQUALIFIED` report with unchanged current inputs can fill
`restart_requalification.completed_receipt`. That report is content-only;
its `execution_qualification` remains `NOT_PERFORMED`.

`restart_support.py` then independently reopens every specifically consumed
historical/current file comparison. Only the explicit root/device mappings
are permitted. Path, inode, timestamps, size and complete SHA-256 must remain
unchanged; the actual new device is recorded separately. New comparisons
against current r4 inputs retain exact full-seven equality. Consumed file
bridges are written to a separate bounded JSONL product and all consumed files
are reopened again before terminal qualification.

Old `common.py` whole-catalog hashes contain physical device fields and cannot
be compared as equal after the reboot. For each of the six selected roots,
the support requires the matching completed prerequisite and independently
rehashes the exact file/link set, sizes, content hashes, executable bits and
raw symlink text. It compares that fresh semantic result with the worker's
current physical catalog. Historical resolved symlink targets are not treated
as current target evidence; the normal current catalog opens and pins current
targets separately. Existing same-boot physical before/after comparisons remain
unchanged. Empty-directory history was not present in the old catalogs and is
not invented; current directory metadata is captured and preserved by the
original SDK reuse checks.

Current cgroup admission remains separate from old producer resource
provenance. No cgroup-continuity assertion spans the two boots. The old raw
snapshots remain in explicitly named retained-content provenance records,
while current continuous guards enforce the new original leaf and unchanged
policy throughout this attempt.

## Final staging and execution order

1. Independently review these candidate files and the shared planner and
   requalification helper.
2. Run the selection maker and the complete content-requalification actor in
   the current original cgroup under Root's bounded owner.
3. Stage the reviewed producer, owner, support, shared planner and requalifier
   into fresh `source39-native-gateways-physics-peer-controls-r4` controls.
4. Open each staged file and the terminal requalification report to obtain its
   actual current seven-field pin. Fill the four `restart_requalification`
   entries in `native-config.template.json`, then record that configuration's
   actual SHA in the fresh root control. Do not copy old device fields into
   these new pins.
5. Run the physical owner with the exact filled control SHA. Do not invoke a
   worker body by pretending stdin has a physical `__file__` path.
6. Require actual 74-TU full-MD/link/export completion, zero OOM changes,
   complete input/product preservation, no remaining mounts and successful
   exact-owned cleanup before any later native transport or physics consumer
   may select the products.

The outcome scope remains native compilation and bounded String metadata
execution. It does not qualify DDS delivery, Runtime execution, Guest examples,
controlled MuJoCo physics, matched native/Wasm performance, hardware counters
or robot actuation. No ROS/WASI capability is granted by this build lane.
