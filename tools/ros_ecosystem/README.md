# ROS source evidence tools

These tools acquire source archives as data and construct an auditable review
queue. They never run downloaded build scripts, hooks, packages or executables.
SDK `.tmp` is an evidence workspace; the SDK API itself is not changed.

## Acquisition and resume

Run from the VM repository, with Python 3.12+ and PyYAML available:

```sh
python3 -B tools/ros_ecosystem/acquire.py --output ../uwvm2-ros-sdk/.tmp/ros-ecosystem --jobs 8
```

The first run pins the rosdistro index and distribution YAML by Git commit and
records ROS Index metadata hashes. Subsequent runs reuse `catalog.json` and
successful receipts. Successful source IDs are URL/ref identities, not claims
that the ref is immutable. Archive SHA256 identifies actual fetched bytes.

Source adapters are limited to explicitly supported HTTPS forges. Unsupported
VCS/hosts, unavailable refs, invalid archives and failed downloads produce
failed receipts. Do not replace them with guessed repositories or empty
successful results. HTTP 429, 5xx and transient transport failures get bounded
retry; 404 and acquisition bounds do not.

Downloads have a 4 GiB compressed bound; expanded archives have an 8 GiB and
one-million-member bound. Traversal, special members and escaping links fail.
Text/manifest scans have an 8 MiB per-file bound. Archives and trees still
consume aggregate disk space; these per-source acquisition bounds are not a
VM storage-capability implementation or a total-corpus storage quota.

`manifest.xml` and `package.xml` identify observed packages. Legacy rosbuild
dependency attributes and modern dependency element text are recorded. The
automated external-effect scan is incomplete by construction and never means
that source is safe or portable.

## Supplemental and transient recovery

```sh
python3 -B tools/ros_ecosystem/reconcile.py --root ../uwvm2-ros-sdk/.tmp/ros-ecosystem --inspect-pages --recover --jobs 6
python3 -B tools/ros_ecosystem/recover.py --root ../uwvm2-ros-sdk/.tmp/ros-ecosystem --jobs 4
```

With `--inspect-pages`, reconciliation captures the ROS Index sitemap if needed
and inspects package pages missing from distribution metadata. A plain report
does not fetch missing sitemap data. Empty repository-summary placeholders
are not source locations. Alternate source/doc metadata, release-upstream refs
and newly supported archive adapters use the separate `supplemental` tree.
Transient retries use `recovery`. Neither rewrites the primary acquisition.
Do not run two acquisition/recovery processes writing the same output tree.

Each recovery pass selects receipts present when it starts. After primary
acquisition finishes, rerun recovery for late failures. A successful later
receipt supersedes a failed original only in the combined evidence view.
Original evidence remains available. Submodule declarations are reported;
submodules are not fetched automatically.

## Reconciliation and semantic review

```sh
python3 -B tools/ros_ecosystem/reconcile.py --root ../uwvm2-ros-sdk/.tmp/ros-ecosystem
python3 -B tools/ros_ecosystem/scenarios.py --root ../uwvm2-ros-sdk/.tmp/ros-ecosystem
```

`reconciliation/summary.json` and `packages.json` distinguish expected source
IDs, observed source IDs, index names and sitemap pages. Unresolved packages, ambiguous root manifests
and failed snapshots remain separate files. Archive wrapper names are never
counted as confirmed rosbuild package names. Inventory generation while
acquisition runs gives a timestamped partial snapshot; regenerate when done.

`package-effect-inventory.json` assigns findings to the nearest enclosing
package, preserving unowned findings separately. Every automatic row remains
`semantic_review=PENDING` and `portability=NOT_TESTED`. A package observed in
one branch does not qualify all its distribution variants.

[review-cases.json](review-cases.json) records manually examined source ranges,
file/archive hashes, limited scope and required acceptance checks. Updating
this file requires reading the concrete source paths. Do not convert regex
hits into completed review rows. The corresponding design rationale is
[ROS ecosystem source evidence](../../documents/runtime/ros-ecosystem-review.md).

The first acquisition processes started before legacy dependency attributes
were added to the scanner. Their existing receipts/scans remain evidence of
that initial scan and may omit those dependency names. A complete dependency
closure requires rescanning those manifests and resolving native/system,
conditional, build and runtime dependencies. Neither the current scan nor the
example review cases establish that closure.

## Security tests: existing 64 GiB cgroup only

Do not run the test suite directly on the host. Do not create a replacement
container, increase memory, add swap or use an unrestricted build to work
around this gate. The existing container root filesystem is read-only;
`/dev/shm` is suitable for these small Python fixtures. The test transfer and
execution are:

```sh
docker exec uwvm-sdk-resource-gate-20261024 mkdir -p /dev/shm/uwvm-ros-source-coverage
tar -C tools/ros_ecosystem -cf - acquire.py reconcile.py recover.py scenarios.py test_acquisition.py | docker exec -i -w /dev/shm/uwvm-ros-source-coverage uwvm-sdk-resource-gate-20261024 tar --no-same-owner -xf -
docker exec -e PYTHONDONTWRITEBYTECODE=1 -e TMPDIR=/dev/shm/uwvm-ros-source-coverage -w /dev/shm/uwvm-ros-source-coverage uwvm-sdk-resource-gate-20261024 python3 -c 'from pathlib import Path; import unittest,sys; assert Path("/sys/fs/cgroup/memory.max").read_text().strip()=="68719476736"; assert Path("/sys/fs/cgroup/memory.swap.max").read_text().strip()=="0"; print("cgroup memory.max=68719476736 swap.max=0",flush=True); suite=unittest.defaultTestLoader.discover(".",pattern="test_acquisition.py"); result=unittest.TextTestRunner(verbosity=2).run(suite); sys.exit(not result.wasSuccessful() or result.testsRun!=16)'
```

The 16 offline tests cover archive traversal/escaping links, early expansion
rejection, bounded manifest parsing, legacy dependencies, unsupported source
locations, encoded refs, retry policy, complete atomic evidence publication,
package ownership and distinction between observation and qualification.
They execute no downloaded source. They are acquisition/evidence tests,
not upstream package portability or robot hardware tests.
