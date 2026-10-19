# Revision2: independent directory-name observations

The original Noetic6 small suite ran under Root's actual boot3 hard64GiB,
zero-swap owner. It recorded10 PASS and1 ERROR. The positive complete physical
verification failed at a combined exact directory-name/stat check. The actual
and expected payload ledgers were byte-identical (1,331 bytes each), while its
physical-verification output was empty. A later passive diagnostic on new
held FDs found complete directory names and stable seven-field metadata.

Root subsequently reproduced the unchanged original positive on a fresh owned
fixture with diagnostic logging at the failed guard. Its exact root locals
recorded an initial name list `[]`, a subsequent full list `['usr',
'value-alias']`, equal before/after values for **all seven** directory stat
facts, the actual expected SDK-root FD target and a cursor of0 afterward. This
demonstrates inconsistent enumeration observations without an observed metadata
mutation. It does not identify a specific prior cursor mover or kernel/runtime
cause. The reviewed `SafeTree.finish()` contains no directory enumeration;
separate tiny held-before-population cases did not reproduce the failure. Do
not attribute the result to a proved SafeTree, Btrfs or CPython defect. All
original source, failed reports and actual diagnostic evidence remain unchanged.

The new verifier opens `.` relative to the held directory FD for each complete
name observation. This creates a separate open file description of exactly the
same current inode; `dup()` would share the original description. Before and
after enumeration it requires all seven stat facts of the new and held FDs to
agree. It closes only its own reader and leaves the caller's description alone.
Both initial and terminal name snapshots use this policy. Every original exact
name, same-filesystem, metadata, SHA256, mode, relative-link, complete hardlink
class and final complete signed-set check remains mandatory. No failed check
is retried, ignored, downgraded or given a timestamp tolerance.

The original positive case still opens the `SafeTree` root FD before package
population, then requires the entire independently generated model/ledger and
physical tree. An additional positive case opens a fresh post-population owner
FD, actually consumes `os.scandir(fd)`, records its nonzero cursor, verifies all
signed physical rows, and requires that caller cursor and all stat facts remain
unchanged. This explicitly exercises a consumed description without claiming
it reproduced the original failure. Negative missing/foreign/byte/link cases
now require the intended refusal message, so an unrelated generic directory
error cannot count as successful content or topology validation. The changed
byte fixture preserves length to reach the SHA256 comparison itself.

The source count is12 tests; none was executed by the source author or peer.
Root owns the actual successor run and any failing-walk diagnostic. Until an
actual terminal result exists, this revision is only reviewed source, not a
replacement PASS for the original failed suite or the full513 acquisition.

Production producer, virtual signed-tar model, owned storage and baseline
restart helper are byte-identical to r1. Only the physical verifier and test
source change. Owner/stager use new fixed `r6b` control/actor/owner/receipt paths;
this path-only delta preserves every earlier source/staged/output attempt.
Current cgroup, signed repository/package authority, resource floors,
independent-inode CoW, unchanged `SafeTree`, tar retirement, owned trim and
600-second cooperative cleanup contracts remain unchanged.
