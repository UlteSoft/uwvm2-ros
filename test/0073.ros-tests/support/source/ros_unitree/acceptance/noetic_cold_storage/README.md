# Source-only Noetic cold-storage candidate plan

This plan authorizes no archive or retirement. The source author ran no SSH,
candidate import, test, compiler, clone, archive or deletion. Root's r6d owner
was already running when Root supplied the static control-selection packet.
The packet binds control SHA256
`344eb5d9bd35436b5d30d9f817d46978110842dfdc22a3ea45bcf376b4cc8c9e`;
it is preserved as data in `current-selected-roots.root-observed.json`.
The machine's current free space and active jobs can change after that snapshot.

## What must stay online

The actual spec has27 complete input roots. It selects the entire old
`r1/apt/downloads`, `r4/apt/downloads`, `r5/apt/downloads` and new
`cacheview-r6d/debs` as its four cache roots. Current owner and producer inventory
every member of those roots before/after; the scanner's `.deb` filter does not
weaken the whole-root preservation contract. For example, the historical r4
`apt/downloads/00255/data.tar` is inside a protected whole root. It cannot be
removed while r6d runs, even though its payload is not a DEB.

The old r4 CODE directory is also a complete selected root. Preserve it, its
seed spec and producer, all current controls/source packages, official locks,
all old source manifests, raw reports and their unchanged status bodies,
payload ledgers and selected signing-key roots. Preserve r5/sdk: it is the
actual unqualified partial source used for fresh authenticated subset checks.
The original r4/sdk was transferred into r5/sdk; a newly found r4/sdk or alias
requires investigation rather than automatic archival.

R6d's finite cache receipt validator additionally reopens the entire original
42-file r6c model-DEB tree, including the excluded partial, and the41-file view
at admission/finalization. Those original files and every catalog/receipt remain
online. The new r6d actor and supervisor outputs are active owned directories;
they and all evolving reports are excluded from any historical archival plan.

Source evidence is explicit:

- `noetic6_r4/owner.py` constructs the exact spec/cache closure, calls
  `admit_cache_view` before execution and during final preservation, and compares
  all whole-root catalogs and current finite pins.
- `noetic6_r4/acquire-sdk.py` validates all cache roots as whole spec inputs,
  inventories their complete contents, and reads old report provenance as raw
  failed history. It does not recursively reopen every historical product pin.
- `noetic6_r4/fresh_rebuild.py` consumes only the selected r5 report/outer/ledger
  and current r5/sdk for old-content reconciliation, followed by the new fresh
  signed513 model. No old r1/r2/r3/r4 Packages file supplies new authority.
- `noetic6_next/cacheview_contract.py` binds the actual whole42/whole41 tree
  catalogs and current source/clone pins. Nested historical report paths are
  not rewritten or silently treated as current file identities.

## Finite candidates for passive collection

`passive-collection-request.json` lists56 **exact** possible large Packages
files:14 historical filenames for each of r1, r2, r3 and r4. Seven are decoded
`apt/indices/*-Packages` files and seven are APT's retained
`apt/state/lists/*_Packages` counterparts. Names come from the genuine historical
r4 product list; their existence, current sizes and pins in the other revisions
are not assumed. Root records an absent candidate as absent. There is no glob
retirement and no whole actor/apt/state/indices directory removal.

Release, InRelease, GPG-status and the empty `apt/state/status` stay online, as
do raw reports, logs, current selected DEBs and ledgers. Keeping small status
and signing metadata also avoids conflating a report's historical status with
the availability of its large archived index bodies.

The reference r4 list has231,762,722 logical bytes across these14 files. Four
identical complete lists would be927,050,888 logical bytes, approximately884MiB.
This is a historical **logical** upper reference for those named copies, not
an observation of current files or a prediction of space reclaimed. Btrfs
compression, shared extents and delayed frees can make actual savings smaller.

Secondary passive requests cover old r1/r2/r3 `sdk` directories if they exist,
and exact registered TAR files only below r2/r3 `apt/downloads`. R1/r4/r5 TARs
inside selected download roots are excluded. Secondary SDKs may contain links;
the existing nlink1-only frozen-source archive owner cannot be applied blindly.
Root must either prove every entry admissible under its exact policy or review
a separately bounded link-preserving archival owner. Unknown external hardlink
classes, followed symlinks, mounted subtrees and special files are blockers.

Before any archive proposal Root collects ordinary opened current full7 pins,
permission/type/uid/gid/nlink, logical bytes, allocated512-byte blocks, same-FS
directory identities and exact membership. It checks every candidate against
the actual current and future consumer closure: both path containment directions,
canonical parents, symlink resolution and device/inode aliases matter. A
candidate that is outside the27 spec roots can still be a finite selected file.
An extra physical hardlink to a protected input also prevents retirement.

## Future ROS1 native consumers

Maintained `tools/ros_distro/profile.py` accepts explicit SDK roots, ROS prefix,
setup, distribution index, source lock, tool roots and sysroot. In
`tools/ros_distro/probe.py`, `Preparation.begin` catalogs those complete SDK/tool
roots and configured option roots, pins the explicit files, reads package
metadata under the selected ROS prefix and runs the selected setup. The
`tools/ros_distro/build.py` native witness uses that actual SDK/ABI/toolchain and
retains compiler dependencies and ELF facts. These consumers contain no
automatic old Noetic acquisition-index lookup.

The older `tests/ros_native_audit_io/run_noetic_build.py` explicitly consumes
the baseline SDK's `gateway/noetic-rootfs`, not old r1..r4 acquisition apt data.
The AArch64 ROS1 search lane consumes its supplied whole rootfs and compiler
receipts. The existing derived Noetic rootfs is therefore kept online too,
even though the current r6d spec inventories only its keyring subtree.

This is a current source observation, not permission to discard arbitrary
future inputs. Root must close the next ROS1 native control/profile before any
retirement, select the newly completed r6d SDK only after its genuine terminal
receipt, and repeat the finite/whole-root overlap check. If a future verifier
intentionally reopens archived old products, restore and requalify their bytes
first. A historical report's original device/inode/ctime or execution continuity
cannot be reconstructed from an archive and must never be fabricated.

## Archive before any separately reviewed retirement

Use small serial batches, initially at most512MiB logical input and512MiB self
RSS, under the exact current hard64GiB/swap0 guard. Keep the8GiB active backing
floor. Root reported roughly8GB Mac persistent capacity; actual destination
free space, complete-stream bounds and archive expansion overhead must be
checked before every batch. The actual persistent destination is Root-selected
and remains null in this source-only request. Stream directly to it rather than
writing a second archive on the Linux backing filesystem.

Each new finite-file archive owner must prove stable opened source identities
and full bytes, exact tar members, full metadata in its catalog, complete-stream
size/SHA256, and complete original preservation during export. The receiver
independently validates every header/member and byte hash, compressed stream
hash/count, then fsyncs the persistent archive and directory and records its
actual full pin. No successful export alone authorizes unlinking originals.

Only Root's separately reviewed retirement step can recheck the now-current
consumer closure and exact original pins, then unlink those explicitly named
ordinary entries. Parent directories, reports and siblings remain. Observe
actual backing/image allocation before/after and use only the already-owned
Btrfs free-block trim barrier. Shared-block estimates are not claimed as free
space; only actual post-operation measurements count.

These old metadata candidates may yield modest capacity. Their historical
logical size alone does not establish room for a full patched LLVM23 build,
eight architecture SDKs and distribution tests. Those future tasks require
their own measured source/toolchain/artifact/transient space admission and a
closed serial retention plan; this plan lowers no resource reserve.
