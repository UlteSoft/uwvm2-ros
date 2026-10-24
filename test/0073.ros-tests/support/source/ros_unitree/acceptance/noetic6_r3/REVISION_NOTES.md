# Revision3: observe all old/new directory names independently

Both previous source packages and their actual failed test reports remain
unchanged. Root's revision1 suite recorded10 PASS and1 ERROR. The unchanged
original positive was subsequently instrumented on a fresh tiny owned fixture:
its initial name snapshot was empty, a subsequent snapshot listed both expected
root names, all seven directory stat facts were equal, and the FD referred to
the actual SDK root. That demonstrated inconsistent name observations without
observed metadata mutation; a specific kernel/runtime cause remains unproven.

Revision2's independent-description policy fixed that original positive in
Root's actual next run. Its12-case suite nevertheless failed two cases. First,
`os.scandir(fd)` left position0 on the actual selected Python/runtime, so the
test's assumption that it must leave a nonzero cursor was wrong. Second, old
partial-tree admission still used held descriptions for its name snapshots;
an unsigned old file therefore triggered a generic inconsistent-directory error
rather than the required specific unsigned-file rejection. The stronger
revision2 negative assertion exposed this remaining path. These are actual
failures, not completed acquisition or compatibility results.

This revision moves the already reviewed independent-description routine to one
module-level `directory_names(held, guard)`. Both snapshots in `old_rows()` and
both snapshots in the final complete physical verifier call it. The routine
opens `.` relative to the pinned held FD, requires exact agreement of all seven
stat facts before and after reading, reads the complete sorted name set, and
closes only its independent reader. It neither seeks nor duplicates the held
description. The old tree still retains exact source root/name/file metadata,
full content/mode equality to the fresh signed model, confined symlinks,
independent-inode CoW, and complete final preservation. The new tree retains all
exact signed names/content/modes/link classes and metadata checks. There is no
retry, ignored name, stat/time tolerance or fallback.

The cursor regression still performs real `scandir` enumeration and verifies
its actual names. It then explicitly calls `lseek(owner, 1, SEEK_SET)`, requires
both the returned value and a subsequent `SEEK_CUR` to be1, runs the complete
physical verifier, and requires the final actual cursor to remain1 and all
stat facts unchanged. The existing old-subset CoW positive similarly seeds its
held source FD to1 and requires that position to survive both source snapshots,
the clone and the final old-source read. No inference from scandir's ending
position supplies the cursor evidence. The suite remains12 meaningful cases;
negative cases retain the specific missing/name/byte/link rejection assertions.

The producer, authenticated tar model, exact owned-storage source, baseline
restart helper and literal 10,509-byte SafeTree body remain byte-identical.
Only owner/stager remote control/output/receipt constants change from `r6b` to
fresh `r6c`. The source runs outside the frozen Runtime source tree, against the
same preserved complete Source35 and explicit current context; it does not
require a new Runtime freeze. Staging still only emits an actual unexecuted
owner argv. Actual small tests, full513 signed acquisition, clone/trim commands
and compatibility execution remain Root-owned steps, not actions by the source
author or source peer.
