# Exact current file aliases for historical hardlink classes

`requalify_exact_aliases.py` is a separate, finite opt-in extension of the
unchanged historical union helper. It addresses a specific distinction: a
selected compiler library can have a known current hardlink in a debug directory
whose other files were never historical inputs. Selecting that one alias does
not establish, require or grant a complete inventory of the containing debug
directory. Additional unknown links of the selected library remain fatal.

The strict restart helper stays at SHA256
`0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260`.
The complete historical union helper stays at SHA256
`8fbc2d1da44ef07bd8e6f69f4b174df70d802c471e989c1e752073cc95627d61`.
The new factory fully reads and binds both loaded helper files before deriving
its class. The calling frozen adapter must also bind the new wrapper's own
source bytes; the wrapper does not import an arbitrary sibling by name.

## Input contract

The factory accepts the same genuine complete old root/catalog selections,
explicit device transitions, guarded limits and complete auxiliary roots as
`HistoricalUnion`, with one additional argument:

```python
union = exact_alias_module.ExactAliasUnion(
    restart_base, unchanged_union_module, historical_selections, guard, limits,
    device_transitions=explicit_device_transitions,
    auxiliary_roots=complete_ARM_root_selection,
    exact_aliases=selected_current_file_aliases)
```

Each element of `selected_current_file_aliases` must have exactly these fields:

```text
current_pin: {path, bytes, sha256, device, inode, mtime_ns, ctime_ns}
historical_anchor: {device, inode}
```

There must be1..64 elements. The caller supplies actual current observations,
not predicted pins, content-only references or projected old JSON. Each path
must be normalized, absolute, physical, unique, absent from historical names,
and outside every selected complete auxiliary root. Its old anchor must exist
in the genuine selected catalog union. The current device must equal the
explicit transition for the old device and the inode must equal the genuine
old inode. The adapter for the observed two library aliases additionally fixes
the exact two ELF paths, anchor classes and full current pins; the generic
wrapper never discovers or admits more names by scanning their parents.

Current extra names and historical link counts were not recorded in the old
catalogs. The receipts explicitly retain those facts as `NOT_RECORDED`.
Original report bodies, statuses, timestamps and catalog rows are never
rewritten. Exact current files do not become Frame roots, executable or DSO
inputs, Guest permissions, process continuity, or proof of old execution.

## Admission and preservation order

1. Call `qualify_auxiliary(owned_output_directory)` exactly once before any
   historical root walk, even when the complete auxiliary-root list is empty.
   Every exact file is fully read through `_parent`/`_opened_at` with nofollow
   directory traversal, a held descriptor and named-entry verification. Its
   actual full7 must exactly equal the selected current pin. The unchanged
   class observer independently checks genuine old size, SHA, inode, both
   timestamps and executable bit through the explicit device transition.
2. The inherited complete auxiliary-root discovery and second walk still run.
   Thus an ARM directory remains a complete current inventory. The adapter
   continues to require its exact ten ordinary names and nine anchors. This
   method returns only complete-root receipts; exact-file receipts are separate.
3. The wrapper fully reopens every exact file after discovery. Its full mode
   and actual link count must stay stable, and the link count must equal every
   distinct historical, complete-auxiliary and selected exact name of its class.
   An unselected alias of the same inode refuses the class. No unrelated sibling
   can make that check succeed or establish authority for itself.
4. Walk all registered historical roots with `requalify_tree`, then call
   `finish` before admitting any compiler product. Historical observation is
   refused until exact-file qualification succeeds. Inherited global class
   finalization fully reopens all members, including the selected exact files.
5. Call `preserve` after the consuming work. It brackets the unchanged complete
   root and global class preservation with separate exact-file reopens, compares
   all evidence, and reopens the helper source bindings. No failed preservation
   can become a successful build or transport receipt.

## Evidence fields

The inherited evidence retains its complete-root policy and records a combined
selection SHA that includes the exact current file selections and the original
union selection SHA. The wrapper adds:

| Field | Meaning |
| --- | --- |
| `exact_current_alias_policy` | `current-exact-files-bound-to-old-anchors` |
| `exact_aliases_qualified` | Initial exact-file reads and final count closure completed |
| `exact_current_aliases` | Separate, finite per-file receipts |
| `original_union_selection_sha256` | Unchanged complete-root selection digest before adding exact files |
| `exact_alias_helper_bindings` | Actual full7 source pins for the reviewed base and union helpers |

Each per-file receipt has exactly `policy`, `historical_anchor`, `current_pin`,
`current_mode`, `current_nlink`, `historical_auxiliary_names`,
`historical_auxiliary_nlink`, `sibling_directories` and
`execution_qualification`. The two historical auxiliary fields are
`NOT_RECORDED`, siblings are `NOT_SELECTED`, and execution is `NOT_PERFORMED`.
Per-class evidence additionally separates `current_exact_alias_members` from
the inherited complete-root `current_auxiliary_members`, with explicit
`historical_exact_alias_names`/`historical_exact_alias_nlink` markers. Complete
parent-directory admission is never implied by an exact-file record.

## Six guarded kernel regressions

`test_restart_exact_aliases.py` defines six small real-filesystem tests. They
exercise a complete successful old/ARM/exact-file union and terminal preservation;
an unrelated sibling with two real hardlinks that stays unselected; rejection
of an extra unknown link of the selected class; selected full7 mismatch;
duplicate/historical alias names; symlink spelling; and same-size selected-file
mutation. The positive case also changes an unrelated sibling without admitting
it, proves `observe` refuses that sibling, then verifies that an extra ARM entry
still fails the complete-root policy.

The guarded CLI requires all of these explicit arguments:

```text
--source --manifest --manifest-sha256
--test-sha256
--base-helper --base-helper-sha256
--union-helper --union-helper-sha256
--exact-helper --exact-helper-sha256
--current-cgroup --current-cgroup-sha256 --out
```

The test entry and three helper files are explicit SHA-selected external
metadata controls, distinct from one another and disjoint from the selected
complete frozen Runtime source. No complete source copy or membership claim is
needed for this standalone kernel policy test. Initial metadata reads are
size-first, ordinary, nofollow and stable. Only the exact unchanged strict base
loads before constructing and forcing its actual current cgroup/source guard;
union and wrapper code load after that guard succeeds. The current cgroup
document must identify the actual selected64GiB, swap0 Linux incarnation. All
fixtures live below `/tmp`; `--out` must be a fresh, physical, disjoint owned
directory. The terminal receipt binds the test itself and all helper, manifest
and context bytes, actual before/after cgroup facts, all six real test results
and a zero-subprocess scope. Source authoring or review establishes no test
result; only a retained terminal actual receipt can do so.
