# Retained input admission after a Linux restart

`requalify_after_restart.py` bridges **content identity** between two explicitly
selected boot incarnations. A Linux reboot can change a filesystem device
number, the cgroup inode and every live process identity. Substituting the old
seven-field pin into a new report would therefore be incorrect even when the
bytes survived on disk.

The helper keeps each historical pin unchanged and obtains a separate current
pin by opening and streaming the complete current file. Its terminal success
status is `CONTENT_REQUALIFIED`, with `execution_qualification` set to
`NOT_PERFORMED`. It runs no compiler, Runtime, native ROS process, package
manager, network client, emulator or service. A historical `FAIL` or `RUNNING`
report remains exactly that report. A retained successful build still needs the
consumer's ordinary source, dependency, binary and execution admission; this
helper does not establish ROS compatibility, a complete SDK, valid compile-MD
closure, current executable mappings, process ownership or performance.

The coordinator runs the helper as a supervised metadata actor inside the
selected **64 GiB / zero-swap cgroup**, with its own finite deadline and aggregate
RSS limit. The helper checks the current boot, actual leaf device/inode/path,
effective CPUs, controller ancestry, four-GiB working reserve and zero OOM
counters while it hashes. It neither creates nor changes a cgroup. Build
parallelism is unrelated to this single content-reading actor.

## Inputs and invocation

Supply an absolute current source root and the exact complete source manifest
used by the retained artifacts. `--historical-source-root` separately records
the original producer's source path; the tool never edits an old build receipt
to claim it was produced from a newer source freeze. If the retained source
still has the same physical path, pass that path for both source arguments.
The manifest must cover the complete current source file set, including
`tests/ros_cgroup/probe.py`. The helper independently binds that probe's bytes
before loading it for current cgroup admission. The helper itself may be a new,
separately reviewed coordinator input; the coordinator must pin the exact
physical helper it invokes.

Prepare a bounded JSON configuration with this closed structure. The following
values are placeholders, not qualified observations:

```json
{
  "format_version": 1,
  "kind": "uwvm-ros-restart-content-selection",
  "historical_boot_id": "11111111-1111-1111-1111-111111111111",
  "cgroup": {
    "memory_max": 68719476736,
    "memory_swap_max": 0,
    "expected_boot_id": "22222222-2222-2222-2222-222222222222",
    "expected_leaf": "/sys/fs/cgroup/owned-leaf",
    "expected_device": 29,
    "expected_inode": 12345,
    "expected_cpus": "0-31"
  },
  "limits": {
    "file_bytes": 2147483648,
    "catalog_bytes": 33554432,
    "catalog_entries": 60000,
    "total_bytes": 17179869184,
    "seconds": 1800
  },
  "files": [
    {
      "label": "raw-build-report",
      "historical_pin": {
        "path": "/owned/build/report.json",
        "bytes": 123,
        "sha256": "0000000000000000000000000000000000000000000000000000000000000000",
        "device": 81,
        "inode": 5678,
        "mtime_ns": 1700000000000000000,
        "ctime_ns": 1700000000000000000
      },
      "current_path": "/owned/build/report.json"
    }
  ],
  "trees": []
}
```

Invoke the helper using the selected Python executable under the coordinator's
existing process owner. Use literal absolute paths and actual SHA256 values:

```sh
/selected/python3 -B /selected/requalify_after_restart.py \
  --historical-source-root /retained/source \
  --current-source-root /retained/source \
  --historical-manifest /retained/source-manifest.json \
  --historical-manifest-sha256 ACTUAL_MANIFEST_SHA256 \
  --config /owned/restart-content-selection.json \
  --config-sha256 ACTUAL_CONFIGURATION_SHA256 \
  --out /owned/fresh-requalification-output
```

The output directory must not exist. Its existing parent must be an ordinary
directory, and the output must not overlap the selected source, inputs or
catalogs. Each absolute parent component is opened with `O_NOFOLLOW`; input
symlinks are not silently resolved. The process writes only its fresh owned
output directory. Raw reports, package caches, SDK trees, source manifests and
historical catalogs are read-only inputs.

## Optional complete regular-file and symlink catalogs

A `trees` entry selects one retained JSON Lines catalog produced by
`tools/ros_distro/common.py::catalog`, its historical seven-field pin, and an
explicit current root:

```json
{
  "label": "retained-sdk-copy",
  "historical_root": "/owned/old-sdk-root",
  "historical_catalog_pin": {
    "path": "/owned/old-sdk-before.jsonl",
    "bytes": 123,
    "sha256": "0000000000000000000000000000000000000000000000000000000000000000",
    "device": 81,
    "inode": 5678,
    "mtime_ns": 1700000000000000000,
    "ctime_ns": 1700000000000000000
  },
  "current_root": "/owned/old-sdk-root"
}
```

The helper compares the **exact regular-file and symlink name set**, every
regular file's complete SHA256 and size, its executable-bit classification, and
every symlink's literal target text. It writes a fresh catalog with current
file/link observations, then independently walks and rehashes the tree again.
The retained historical catalog is also reopened and checked at the end.
With the default `hardlink_policy: "reject"`, regular hardlinks with
`st_nlink != 1` fail admission. Sockets, devices, FIFOs, directory
replacement, filesystem-device crossings, extra or missing names and changed
files fail admission. Every directory is traversed through anchored directory
descriptors with finite depth and entry bounds.

The old catalog format does not record empty directories, full permission
modes, xattrs or ACLs, so these are not part of its historical content claim.
Symlinks are inventoried without following their targets. Historical
`resolved`, `target_file` and dangling-link observations stay historical;
they establish no current target closure. Select any consumed target file or
target root separately, and let the later SDK/compiler/Runtime owner enforce
the complete actual dependency closure. In particular, this receipt never
turns an incomplete decoded package directory into a completed SDK.

### Explicit historical hardlink classes

Some retained Debian tool roots contain ordinary installed hardlinks. A
reviewed consumer may explicitly select
`hardlink_policy: "historical-complete-intra-root"` for one selected tree.
Omitting the field keeps strict rejection. The optional field belongs to that
tree selection; it is not an ambient environment switch or a global waiver:

```json
{
  "label": "retained-tool-root",
  "historical_root": "/retained/usr/bin",
  "historical_catalog_pin": {"path": "/retained/tool-catalog.jsonl", "bytes": 1,
    "sha256": "0000000000000000000000000000000000000000000000000000000000000000",
    "device": 81, "inode": 1, "mtime_ns": 1, "ctime_ns": 1},
  "current_root": "/retained/usr/bin",
  "hardlink_policy": "historical-complete-intra-root"
}
```

The example's seven-field pin is a placeholder. Actual consumers supply the
retained physical catalog observation and independently pin the reviewed
helper bytes. The opt-in policy derives classes only from regular-file rows
with identical historical `(device, inode)` pairs. Every member must agree on
the old recorded size, SHA256, executable bit and timestamps. The exact name
set remains mandatory; content equality alone does not admit a new alias.

Each current name is opened and completely hashed. For a multi-name class,
all members must share one actual current device/inode, complete file hash,
size, mode, timestamps and link count. The current `st_nlink` must equal
**exactly the number of historical member names inside this selected root**.
An unaccounted outside hardlink, a newly merged class, an independently copied
member, an extra name or a missing member fails. An old singleton must still
have current `st_nlink == 1`. The helper neither follows symlinks nor writes to
the selected input tree to discover or repair aliases. A class that cannot
account for every current link within this exact root remains rejected.

The historical catalog did not record `st_nlink`. Its absence is preserved as
`historical_nlink: "NOT_RECORDED"`; the helper never claims that the old inode
had no outside aliases. `hardlink_topology` records the old names and inode
pair separately from the newly observed current identity and exact current
link count. The generic tree API remains a content/topology bridge: a copied
current root may have different current inode numbers. A consumer requiring
persistent inode identity must additionally use the stricter persistent-pin
API described below.

Current catalogs emitted in this explicit mode include `current_nlink` for
each regular file. All multi-name members are independently reopened after
the first walk. Terminal preservation rebuilds the known classes from the
same hash-bound historical catalog, rehashes every current name, rechecks link
counts and actual topology, and compares the complete current catalog and
topology with the first observation. A link added or removed between the two
passes fails even if file contents remain unchanged. This is current
filesystem evidence, not a promotion of any historical execution status.

## Reusable API and bounded resource use

The two main entry points are `requalify_file(old_pin, current_path, guard)` and
`requalify_tree(old_catalog_pin, current_root, output_catalog, guard, limits)`.
For the explicit class mode, pass the keyword-only argument
`hardlink_policy="historical-complete-intra-root"`. Other strings, booleans and
implicit truthy values fail. `_preserve_tree(receipt, guard, limits)` preserves
the policy and actual topology recorded by that receipt; it does not silently
fall back to default rejection for a previously admitted class.
The caller supplies a live guard callback; using a no-op callback is not an
execution admission. The CLI supplies `CurrentGuard` and additionally verifies
the complete historical source manifest before and after the selection.

`requalify_persistent_pin(old_pin, current_path, guard,
historical_device=OLD_DEVICE, current_device=NEW_DEVICE,
observed_current=actual_current_pin)` is the stricter API for a consumer that
permits only a known filesystem device renumbering. It also requires the path,
inode, timestamps, size and complete SHA256 to match. The optional current pin
must equal the independent reopened current pin in all seven fields. The
consumer must admit a finite explicit path-root/device mapping, keep the
returned bridge separate from its current physical pin ledger, and retain all
ordinary producer-status and dependency checks. The API never supplies an
object that impersonates an old physical observation.

`PersistentPinSet(expected, roots, guard)` is a finite accessor for consumers
with many retained file comparisons. `expected` is an explicit list of distinct
historical seven-field pins. Each `roots` row contains only `root`,
`historical_device` and `current_device`; a more specific component-prefix root
may select a separately mounted filesystem. `admit(old_pin,
observed_current=actual_pin)` requires membership in that preselected closure,
fully reopens the file, and returns its actual current pin. Its `evidence`
property records historical/current observations separately. `preserve()`
reopens every consumed file before terminal admission; use
`preserve(require_complete=True)` when every registered pin must be consumed.
After the first complete streamed admission, repeated comparisons may reuse
the receipt when the caller supplies an exactly matching current seven-field
observation; they still run the live guard and the final preservation pass.
An admission without `observed_current` always performs another complete read.
It does not rewrite nested reports, replace `Frame.pin`, or change historical
status/cgroup fields. The consumer must deliberately use this accessor at its
specific old/current comparisons and keep current physical ledger entries.

File bodies use a one-MiB streaming buffer. Small JSON controls are limited to
16 MiB, historical JSONL catalogs to at most 64 MiB, individual catalog rows to
64 KiB, source/tree entries to 100,000 and traversal depth to 128. The helper
retains bounded catalog metadata, never SDK file bodies. Each operation checks
its stricter configured byte, entry and time limits. Choose practical limits
and the outer actor RSS budget for the actual catalog size.

Downstream consumers must select `current_pin` for current physical identity
and retain `historical_pin` only as provenance. They must also require this
helper's exact content scope and separately re-run every execution-dependent
qualification required by their acceptance contract. A top-level status or a
matching filename alone is never an SDK, binary or test qualification.

## Synthetic hardlink policy tests

`test_restart_hardlinks.py` supplies 15 meaningful filesystem tests using real
kernel hardlinks and fixtures exclusively under `/tmp`. They cover strict
default rejection, complete nested classes, new merges and splits, unknown
inside/outside aliases, incoherent historical alias metadata, changes during
and after the first walk, symlink non-following, honest new current inode
observations and the absence of historical link-count evidence. The test
source itself does not claim that those tests passed.

Root runs the separately pinned test script and reviewed helper as an owned
actor in the actual current original 64-GiB/swap-zero Linux cgroup. The script
requires that exact current incarnation and the Source39-bound probe; it
starts no subprocess and executes no compiler or ROS/Runtime actor:

```sh
python3 -B /reviewed/controls/test_restart_hardlinks.py \
  --helper /reviewed/controls/requalify_after_restart.py \
  --helper-sha256 ACTUAL_REVIEWED_HELPER_SHA256 \
  --out /var/tmp/uwvm-ros-unitree-20261016-r1/hardlink-policy-tests-r1
```

The resulting receipt reports actual test/failure/error/skip counts, helper
identity and current cgroup observations. Its scope is synthetic filesystem
policy only; it establishes no SDK completeness, ROS delivery or Runtime
qualification. Local source authoring uses AST checks and does not run these
tests.
