# Controlled CP310 view and physics owner

These source files prepare and own the CP310 environment for the maintained
`run_direct_physics.py` coordinator. They are not a physics result. Root must
review and stage their actual complete bytes, freeze Source42, and perform all
execution in the explicitly selected current 64 GiB, zero-swap cgroup. The owner
never creates, moves to, resizes or weakens a cgroup.

`copy_view.py` creates independent same-filesystem inodes using required Linux
FICLONE for nonempty files. It never hardlinks source files, uses an ordinary
copy fallback, merges packages or executes the cached Python. Every source and
clone is fully read and hashed; permissions, sizes, stable source identities and
distinct new inode identities are checked. Empty files receive fresh empty
inodes. The ledger records successful ioctl calls rather than assuming that
all bytes received shared extents. Unsupported cloning fails and preserves
partial evidence. The x86_64 ioctl encoding is derived from the upstream
[Linux fs.h declaration](https://github.com/torvalds/linux/blob/master/include/uapi/linux/fs.h)
and [generic ioctl encoding](https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/ioctl.h).
The [Linux FICLONE manual](https://man7.org/linux/man-pages/man2/FICLONE.2const.html)
describes the same-filesystem and copy-on-write contract.

`owner.py --phase prepare` reads the genuine old completed preflight report as
historical content provenance, freshly requalifies every regular file and link
against its exact old catalog, clones the entire cached tree to a fresh owned
root, and independently requalifies every new file and link again. It preserves
the original reports, catalog, cached tree and their physical current pins.
The resulting receipt has exactly the contract consumed by the coordinator:
kind `unitree-cp310-controlled-physics-view`, status
`PREPARED_CONTENT_ONLY_NOT_EXECUTED`, actual `root` and `python`, and true
`source_preserved`, `inputs_preserved` and `products_preserved`. This is content
qualification only; the historical preflight is never promoted to a new run.

`stage.py` materializes metadata and source files without starting an actor.
The caller supplies an exact SHA256 for its request and source package:

```text
/usr/bin/python3.14 -B /actual/reviewed/stage.py \
  --phase prepare \
  --request /actual/reviewed/prepare-request.json --request-sha256 ACTUAL_SHA \
  --payload /actual/reviewed/source-package.json --payload-sha256 ACTUAL_SHA \
  --out /actual/fresh/prepare-stage-receipt
```

The source package is a closed object with `format_version: 1`, kind
`unitree-cp310-physics-reviewed-source-package`, and `files` containing exactly
`owner`, `copy` and `restart`. Each role has exactly `name`, `bytes`, `sha256`
and canonical `base64`. Destination names are `owner.py`, `copy_view.py` and
`requalify_after_restart.py`. The restart role remains the independently
reviewed 47,606-byte `0f14a3e8...` helper; later experimental adapters do not
silently replace it.

The request template is documentation until Root fills its null complete
Source42 manifest SHA. Four host tools retain explicitly historical physical
pins and are independently reopened through the exact 66307 to 66308 mapping.
The two new namespace/network tools use separately observed current full pins.
The cached `catalog` field is deliberately the genuine historical catalog pin
from the raw report. It is not a current pin: the restart helper records its
new current observation separately. All newly generated controls carry actual
current pins only. No future inode, timestamp, namespace ID or report success
is predicted by the template.

After staging, Root invokes only the exact `unexecuted_owner_argv` published in
the stage receipt. Source staging never launches preparation automatically.
The view root and preparation receipt directory are separate siblings. This
allows the Frame to inventory the immutable receipt directory without walking
the later `/proc`, cgroup or writable mounts inside the view root.

After the real preparation succeeds, Root supplies a run request with actual
full-seven observations for its `view_receipt`, `baseline_config` and
`physics_template`, plus a finite `readonly_bind_roots` list. All producer and
Guest selections in the baseline and physics template must already name real
completed reports and actual files. The baseline retains the standard closed
Unitree schema. Its controllers/direct-runtime fields remain null; the physics
coordinator records each derived language selection in its own result.

The run stage adds only actual `cp310_python`, `cp310_chroot` and
`cp310_view_receipt` input associations, selects the actual COPY Python for the
simulation, and adds immutable copied subtrees/receipt roots. It keeps every
original root and reserves one root slot for the final runtime controls. A
conflicting existing input is rejected. The physics template's network
namespace must remain null until a namespace is actually created. The run
metadata stage has the same CLI with `--phase run` and a fresh external stage
output; it does not modify the initial sealed source directory or prepare
controls.

`owner.py --phase run` starts pinned `unshare --mount --net -- ...` without
`--fork`. Its same-PID exec preserves the direct outer/inner parent relationship
for birth and pidfd ownership. The inner owner verifies the actual distinct
mount/network namespaces and direct parent handoff before running any mount or
network tool. It makes propagation private, brings up only the new namespace's
`lo`, and uses the maintained socket enumeration/read-only ioctl proof of the
current namespace. Inherited sysfs is not used as network evidence, and `/sys`
is never remounted over the real cgroup hierarchy.

The initial make-rprivate operation can remove mount propagation annotations.
The owner records both actual current cgroup snapshots and permits only that
specific annotation removal: physical mount facts, mount IDs, mount options,
superblock options and all cgroup/process policy fields must agree. Subsequent
checks use complete unmodified `cgroup_preserved` snapshots. Old boot/cgroup
facts are never used as new execution admission.

The inner owner then writes the final actual network namespace into a fresh
physics selection and writes its SHA into the final actual Unitree config.
Source42, its full manifest, final controls, explicit artifact/provider roots
and immutable COPY subtrees are bound read-only at their genuine absolute paths
inside the view. The source `/proc` and exact `/sys/fs/cgroup` subtree are bound
read-only for observation. Only `/dev/null`, `/dev/zero`, `/dev/urandom` and
`/dev/random` with their exact expected character-device identities are admitted.
No physical robot, GPU or other hardware device enters the view.

The only writable bind is a new dedicated run parent. Its `run` child does not
exist when the host coordinator starts, because Frame itself creates the fresh
output and HOME/TMP/cache directories. CP310 HOME/TMP/cache selections are placed
below that actual run output. The coordinator runs on host Python3.14. Only
`mujoco_driver.py` and `native_peer.py` use the selected chrooted CP310 Python;
host-linked Worker/Gateway/controller executables retain the host linker and
library view. Actual map-files descriptors are compared against current host
physical catalogs, including the immutable COPY `usr`, `opt` and `etc` subtrees.
No inventory traverses the whole mounted root or the writable run tree.

Success requires the real three-language coordinator's complete preserved
physics result, not merely successful namespace creation. The owner then stops
all owned descendants, unmounts only its recorded destinations in reverse order,
checks that the exact initial namespace mountpoint set is restored, and rechecks
the complete frozen source, selected inputs, products and immutable COPY
subtrees. Same-birth direct coordinators receive up to 600 seconds of cooperative
TERM cleanup before child-first owned-pidfd fallback. Failure remains failure;
the kernel also drops this private namespace when its last owner exits.

All source authoring, AST parsing and packaging are separate from actual Linux
tests. No CP310 execution, ROS/DDS traffic, physics state or performance is
established by these files or their metadata-only stage receipts.
