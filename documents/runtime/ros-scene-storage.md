# ROS scene storage

This document describes the new scene backend in `scene/storage.h`. It does
not change WASIp1, ROS1 v1 or ROS2 v1. It is a backend implementation contract;
its C++ methods are not automatically registered guest imports. The separate
Linux `sealed-input` provisioner and three immutable guest operations are now
connected through scene authority; see [CLI and wire](../command-line/ros-scene.md).
Disk-backed transactional output methods remain backend-only. The separate
`scene/outputs.h` registry implements finite `cache` / `volatile-output` objects
in private heap memory, with ordered exact-request replay and no persistence.
Its publication never implies disk durability or trusted schema validation.

## Authority and purpose

The scene layer must admit an exact action before calling this backend. A
successful interface-exposure check alone is insufficient. A resource owner
contains a trusted environment identity and a grant identity, so two modules
with the same textual alias in different environments cannot share artifacts
accidentally. Passing an artifact number does not transfer authority.

Package resources, configuration and model weights are input-only purposes.
Calibration and map snapshots may have separate input and output grants.
Logs, recordings and caches are output purposes with separately authorized
read and removal actions. The backend has no guest path, directory traversal,
socket, link, seek, truncate, allocation or descriptor-export operation.

Scene configuration binds a logical resource to a trusted provider. The host
passes a directory descriptor only during store provisioning. It must never
forward a guest integer as that descriptor. The broker keeps the scene
authority lease across each effect; blocking work requires an explicit
cancellation and lifecycle policy at the provider layer.

## Immutable inputs

The trusted loader reads and validates the selected package/configuration/map/
model object and checks its expected digest over a sealed memfd. Semantic
schema/publisher validation is still a deployment responsibility, not an
automatic guarantee of the opaque input provider. Input admission requires the kernel's write, growth, shrink and
seal seals. A descriptor opened read-only on an ordinary mutable file is
insufficient and is rejected.

Reads use bounded `pread` ranges and retain no guest pointers. They do not
modify a shared file offset or consume data. The core does not claim to verify
the content digest itself: the trusted binding provider must do so before
enabling a guest grant. Input IDs from a different environment or retired grant
must not be rebound to the old sealed object without a new explicit grant.

## Transactional outputs

An output follows this sequence:

1. Admit its purpose, resource and `artifact.begin` action. Reserve the maximum
   complete object size and one file slot before creating a staging file.
2. Append bounded contiguous chunks. The caller provides bytes, never a host
   path or write offset. Oversized chunks are refused before a filesystem write.
3. Validate staged content with a trusted codec through bounded reads.
4. Commit using atomic no-replace publication. The staging file is synchronized
   first; the containing directory is synchronized after publication.
5. Release only the unused portion of the reservation. Actual committed bytes
   and the file slot remain charged until separately authorized removal.

An ordinary artifact reader sees only committed content. The staging-read
method is for a trusted validator and must not be exposed as an alternative
guest read import. A committed object cannot be appended to. An update creates
a separate generation and reserves its capacity independently, including the
period when both generations exist.

Short kernel writes report exactly how many bytes were accepted. Retrying must
use only the remaining bytes. A failed commit reports publication and durability
separately: after a successful rename followed by a directory-sync failure,
the object is published but crash durability is unconfirmed. Retrying commit
on the same ID retries synchronization and cannot create a second copy.

Abort removes a staged object; removal deletes a committed object. An unlink
failure keeps both the file slot and its full reservation. After successful
unlink, the private file descriptor closes before capacity is released, so an
open-but-unlinked file cannot evade accounting. A subsequent directory-sync
failure reports an error even though in-process deletion already completed.
The next startup refuses a nonempty store instead of assuming deletion was
durable and resetting usage.

## Capacity and filesystem boundary

Each store requires explicit finite logical-byte, object-count, per-object and
transfer limits. Zero means zero allowance. No unlimited disk value is inferred
from an absent application handle. Staged maximum sizes and committed actual
sizes share one account. Empty files consume the object-count budget. Closing a
capability or another reference to the store does not refund persistent data.

Store identity is the backing directory's device and inode. Aliases to that
same live directory reuse the same account and must specify identical limits.
The private root must belong to the effective host UID, deny group/other access,
start empty and remain exclusively managed by the trusted broker. An advisory
lock excludes another cooperative process. Generated file names use exclusive
creation, no-follow lookup and no-replace commit. Before effects, the backend
checks inode identity, link count and expected length. Detected replacement,
hardlink or size drift quarantines the object and retains its reservation.

These are logical capacity controls. Filesystem metadata, allocation units,
copy-on-write, snapshots, filesystem journals and unrelated host writers have
different physical costs. Deployment needs a host-enforced project or volume
quota to guarantee a physical hard disk ceiling. The free-space headroom check
is an additional admission check, not a substitute for that quota. Untrusted
same-UID access to the private directory is outside this provider's threat
boundary and must be prevented by deployment isolation.

The current backend refuses adoption of nonempty stores after restart. Durable
inventory and recovery are required before persistent restart can be supported;
there is no directory scan that silently trusts arbitrary preexisting files.
Fork-inherited stores are rejected before touching inherited mutexes. Normal
process teardown preserves published artifacts; leftover staging files also
cause subsequent admission to fail.

## Execution and portability

The implemented provider is Linux-specific. A different operating system must
supply the same semantics or report the capability as unavailable; there is no
fallback to an unrestricted WASI filesystem. Linux regular-file I/O is
synchronous and does not establish a hard cancellation deadline. Deterministic
control loops must not depend on an unqualified storage completion deadline.

The 64-bit scene lengths do not imply a 64-bit native file ABI. Provisioning
rejects per-object/input limits outside this build's `off_t` range and transfer
limits outside `ssize_t` before opening or duplicating the supplied descriptor.
Every `pread`/`pwrite` range is checked again before the offset conversion,
including overflow and the end of the complete transfer. Aggregate store
capacity can exceed one file's native extent; individual files cannot.
There is no clipping or wraparound fallback. A 32-bit build that needs larger
files must consistently select its large-file ABI (for example
`_FILE_OFFSET_BITS=64`) before including system headers; this backend never
changes feature macros inside a translation unit.

Metadata inspection is bounded, attempts a nonblocking lock, and returns no
content, filenames or native descriptor identities. Inspecting storage never
commits, reads a model, grants authority or changes capacity accounting.

## Security validation

`test/0084.ros-runtime/storage.cc` exercises real filesystem operations as an
unprivileged UID: zero authority-shaped inputs, wrong purpose/owner, finite and
shared reservations, concurrent creation, empty-file limits, insufficient
headroom, denied cleanup, real kernel short writes, atomic no-overwrite commit,
hardlink and symlink substitution, unexpected file growth, actual memfd seals,
fork rejection and restart refusal. It tests backend contracts only, not ROS
package portability or physical-volume quota enforcement.

Native-width checks also cover a sealed sparse memfd with distinct data at zero
and above 4 GiB. Only two pages are written. A narrow `off_t` build rejects the
input and oversized output-store configuration; a large-file build reads the
high marker without truncating its offset. Oversized native transfer budgets
are rejected even when individual test buffers are small.

All compilation and execution use the existing 64 GiB, zero-swap cgroup. Logs
and binaries are retained under `uwvm2-ros-sdk/.tmp/ros-scene-security/storage`.
The native-width regression additionally retains six runs under
`ros-scene-security/storage-offsets`: Clang ASan/UBSan, GCC 13, ARM32 under QEMU
with 32/64-bit file offsets, and i686 through its native IA32 loader with
32/64-bit file offsets. Each run passes all 14 filesystem groups, with recorded
source hashes and unchanged cgroup memory events. QEMU establishes correctness
for these fixtures, not performance or device portability.

Reproduce the width suite from the VM repository using the existing toolchain
recipes (substitute the recipe directory only when using an equivalently
qualified toolchain):

```sh
python3 tools/ros_scene/in_existing_cgroup.py python3 tools/ros_scene/storage_portability.py --recipes .tmp/gc-allocation-fix-20261113/build --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/storage-offsets
```
