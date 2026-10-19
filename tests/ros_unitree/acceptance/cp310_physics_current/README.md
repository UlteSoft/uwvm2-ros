# Explicit current-context CP310 physics owner

This is a separate source successor to the sealed
[`cp310_physics`](../cp310_physics/README.md) package. Historical controls,
source bytes, reports and process observations remain unchanged. No actual
complete CP310 copy or controlled ROS/MuJoCo run is established by this source
package.

Root supplies the complete Source42 freeze and an explicitly SHA-bound staging
request/control. Its `cgroup` member is a closed seven-field selection:
`memory_max`, `memory_swap_max`, `expected_device`, `expected_inode`,
`expected_boot_id`, `expected_leaf` and `expected_cpus`. Before any project
helper loads, the entry checks the actual caller's boot, physical cgroup leaf,
membership and limits against those selected values. The original task leaf,
hard64GiB, zero swap and CPUs0–31 remain fixed; changing a boot or inode string
does not move a process, create a cgroup or requalify an earlier actor.

The original independent CoW implementation is byte-identical:
`copy_view.py`,12125 bytes, SHA256
`8cd1081ff18a1123c228ece6e696750d5f607dcff660f3510e452979637ef82c`.
Nonempty files require actual same-filesystem FICLONE, an independent inode,
single link and complete source/clone hashes. There is no ordinary-copy
fallback. Five small actual Btrfs policy tests are recorded in the
[boot3 checkpoint](../../../../documents/ros/validation-checkpoint-20261016-boot3.md);
they qualify that filesystem policy only.

The two phases retain the baseline contract. `prepare` qualifies genuine cached
view content and emits actual fresh Python/subtree observations without
executing target code. `run` consumes its completed receipt and real
Guest/Worker/native producer inputs. A same-PID mount/network unshare preserves
the direct-parent ownership proof; actual loopback enumeration/ioctl proves the
new network namespace. Source/manifest/config/providers and immutable copied
subtrees appear through read-only binds at genuine absolute names. Only a
dedicated owned run parent is writable; its `run` leaf stays absent until the
coordinator's Frame creates it. Host-linked Worker/Gateway/controller processes
remain outside chroot; only actual CP310 simulator/witness processes enter it.
Inherited sysfs is not remounted as a network-namespace proof.

Root supplies actual current file pins and the request's finite
`restart_mappings`: exact canonical existing directory roots with
`historical_device` and `current_device`, whose current physical root device is
checked. Each selected historical tool/catalog is fully reopened. Equality of
all seven fields records a separate current at-rest observation with explicit
no-execution-continuity scope. Any unequal observation requires its selected
longest matching root and a strict device-only transition bound to the actually
opened current pin. Same-device changes to inode, timestamps or bytes fail.
The tool and cached-catalog branches share this accessor; no historical device
pair is baked into the stager. Old boot, mount and process facts never become
current through metadata rewriting.

The active owner preserves the8GiB backing reserve. Finalizing cleanup/read
preservation does not stop merely because that reserve became low: cgroup,
RSS, ownership and finite deadlines remain active, including the600-second
cooperative direct-child grace before owned pidfd cleanup and reverse unmount.
This prevents the reserve failure itself from skipping cleanup or falsely
claiming preservation.

[`independent-current-context-source-peer.json`](independent-current-context-source-peer.json)
records the narrow current-context/cleanup source comparison with no blocking
finding, using source read, diff, AST and hashes only. The separate
[`independent-restart-mappings-source-peer.json`](independent-restart-mappings-source-peer.json)
reviews the later explicit device-mapping delta, including exact same-device
full-seven reopens. Neither review executed the staging/copy/physics chain.
`source-ready.json` binds the current stager, unchanged owner/payload and both
scoped peer receipts; Root must complete actual input admission and execution
separately. Default Guest authority is unchanged.


The current `--phase profile` hardware recipe permits optional
`collection_policy: "required-hardware"` for an explicit Root hardware-profile
request independent of speed. Omission (or `"measured-slowdown"`) retains the
original selected-language slowdown gate. Use the separate
`profile-run-inputs.required-hardware.template.json` as a DATA shape only; every
baseline/VTune/view full7 still comes from actual observations. The stager,
namespace owner and maintained VTune collector check and preserve the same
policy, while the original unprofiled baseline, physics selection, native
comparison and all source/model/artifact/rights identities stay unchanged.
Both hardware Hotspots and microarchitecture, positive raw PMU events,
parent-paced replay and normal pidfd cleanup remain required. This option
changes no sysctl, driver, privilege, Guest right or device access and cannot
replace unsupported PMU data with software or QEMU samples. See
[`DIRECT3_VTUNE.md`](../../DIRECT3_VTUNE.md) for the complete admission and report
contract. No hardware collection or policy test was run while authoring it.
