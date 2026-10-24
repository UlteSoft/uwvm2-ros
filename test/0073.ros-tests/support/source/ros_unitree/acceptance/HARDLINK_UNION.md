# Finite historical hardlink union and current auxiliary roots

`requalify_hardlink_union.py` is a separately named opt-in layer over the exact
reviewed `requalify_after_restart.py` bytes with SHA
`0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260`.
The base helper, all staged r2 controls and earlier raw reports remain unchanged.
The new policy qualifies metadata only; it starts no process and grants no
Guest authority or execution qualification.

The caller registers every complete original catalog before walking any root:

```python
union = HistoricalUnion(
    reviewed_base, selections, current_guard, finite_limits,
    device_transitions=explicit_old_to_actual_current_devices,
    auxiliary_roots=[{
        "root": "/usr/lib/arm-none-eabi/bin",
        "historical_anchors": [{"device": old_device, "inode": old_inode}],
    }],
)
union.qualify_auxiliary(fresh_owned_proof_directory)
for original in selections:
    union.requalify_tree(original["catalog"], original["root"], fresh_catalog)
initial_proof = union.finish()   # Mandatory before compilation.
# ... Root's separately owned real compiler work, if all other checks pass ...
terminal_proof = union.preserve()
```

`selections` is a bounded list of exact `{root, catalog}` objects. Each catalog
is a genuine old full7 pin with its whole original JSONL SHA. Every regular
file name in every selected catalog belongs to its historical device/inode
class. All observations for a class must agree on bytes, SHA, executable bit
and both timestamps. Overlapping selected roots may repeat a physical name;
it counts once and all old observations must agree. Symlink targets and old
`target_file` observations never count as hardlink names and are never followed
by this layer.

Each current historical name must retain its actual physical path, inode,
bytes, SHA, both timestamps and executable bit, with only the explicit device
transition admitted. A caller may select an identity device pair when an older
persistent filesystem already has the same device number; that establishes
current full-file evidence only. It does not assert continuity of a cgroup,
process, execution result or earlier same-numbered mount.

Without auxiliary roots, the actual current nlink must equal the number of
distinct physical names in the entire selected historical union. An unknown
external alias fails. Missing registered roots, missing or extra names,
merged/split inode classes, changed bytes/timestamps/modes, changed catalogs
and unaccounted aliases fail. After every selected root is walked, all actual
members of every multi-name class are fully reopened before admission.
Preservation walks every complete root again, compares its entire current
catalog SHA and directory-state proof, and repeats those alias reopens.
Current singleton files retain only the additional full-mode observation in
the topology table: their complete full7 is already bound by the whole current
catalog and exact nlink1. This avoids redundant large dictionaries for
hundreds of thousands of ordinary tool files.

## Explicit current auxiliary policy

The auxiliary mode is `current-complete-root-bound-to-old-anchors`. It exists
for the nine genuine ARM tool classes whose `/usr/bin` anchor names are in
the original catalogs, while the extra `/usr/lib/arm-none-eabi/bin` names were
not historically recorded. The complete current auxiliary root is descriptor
walked and fully hashed before any historical root can qualify. Only the
explicit finite historical device/inode anchors can admit multi-name files.
Their actual bytes, SHA, timestamps and executable bit must still equal the
genuine old anchor metadata. Ordinary unrelated current-only files must have
nlink1; an unanchored multi-name class fails. Auxiliary roots cannot overlap
historical roots or each other.

Additional auxiliary names are current observations. Historical auxiliary
names and historical nlink are explicitly `NOT_RECORDED`. They are never
inserted into `historical_members`. Current class membership is the exact
union of distinct genuinely recorded historical names and these explicitly
selected complete current auxiliary names. That count must equal actual
current nlink, so an outside alias remains forbidden. Every selected anchor
must actually contribute at least one current auxiliary name. A second full
descriptor walk closes the discovery/count interval. Full auxiliary catalogs,
directory state, current inode/mode/nlink and all global aliases are checked
again by `finish()` and `preserve()`.

The Typed r3 adapter supplies all original59 complete roots and ONE genuine
old `/usr/lib/cargo/bin/coreutils` catalog containing114 extra historical
names. Together with the original `/usr/bin/coreutils` name these account for
the actual115-name class. The adapter separately selects exactly nine ARM
anchors with old host inode numbers12327242,12327243,12327247,12327248,
12327249,12327250,12327251,12327252 and12327255. It can additionally restrict
the current auxiliary root to the observed flat ten-name set `ar`, `as`, `ld`,
`ld.bfd`, `nm`, `objcopy`, `objdump`, `ranlib`, `readelf`, `strip`. Those
extra ARM names remain current-only. They are never claimed to have appeared
in an old catalog. Complete current auxiliary proof is separate from the
Frame root count and adds no broad extra filesystem or Guest root.

## Guarded kernel regressions

`test_restart_hardlink_union.py` defines18 small genuine Linux filesystem
regressions. Root runs its CLI with explicit `--base-helper`,
`--base-helper-sha256`, `--union-helper`, `--union-helper-sha256`, `--source`,
`--manifest`, `--manifest-sha256`, `--current-cgroup`,
`--current-cgroup-sha256` and fresh `--out` arguments. The cgroup JSON is the
actual current closed64GiB/swap0 selection; no boot/device/inode is guessed.
All fixtures are below `/tmp`. There is no compiler, ROS node, Runtime, mount,
SDK or child-process execution.

Tests cover cross-root admission/preservation, omitted roots, overlapping-name
deduplication, unknown aliases, changed bytes/ctime, old catalog corruption,
inconsistent old same-inode metadata, split/merged classes, post-admission
aliases, explicit current auxiliary admission with separately reported history,
unaccounted external aliases, missing opt-in, wrong anchors, unanchored current
hardlinks, mutated auxiliary state and discovery ordering. Source authoring,
AST parsing and independent source peer review do not establish any test PASS.
Only Root's retained actual terminal receipt can report those18 tests passed.
