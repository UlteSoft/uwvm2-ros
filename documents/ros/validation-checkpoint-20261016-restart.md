# Validation checkpoint after the second Linux admission

The later boot and its current outcomes are recorded separately in the
[third-admission checkpoint](validation-checkpoint-20261016-boot3.md).
The earlier boot/source/process observations below retain their original scope.

This checkpoint records completed observations separately from implementation
and pending acceptance. The earlier dated observations in
`unitree-integration-20261016.md` remain historical results from their named
source snapshots and boot. A file surviving a restart does not make an old
process, device number, test status, or loaded mapping current.

## Restored resource boundary

The original task container is again running with `memory.max=68719476736`,
`memory.swap.max=0`, and CPUs `0-31`. The current boot is
`ec0d5f0f-3060-4220-bee2-2a7791ed4e72`; its admitted cgroup has device29 and
inode18315. All actual tests below ran in this same leaf. No memory limit was
raised to enable parallelism.

The original artifact filesystem was mounted from its existing sparse image
on `/dev/loop29`, retaining UUID `3fe7718a-8039-4f1c-b540-7c5cf05b65ca`.
No format or repair was requested. Its current filesystem device is68; the
backing filesystem is device66308. Earlier device81/66307 observations remain
historical. The restoration receipt is SHA256
`0a36b2c911037c7c842201812ff4cc2bd202af63762382aa5781985ced995eac`.

The interrupted Native39 r3 attempt has a retained `RUNNING` inner report with
one qualified translation unit and62 command records, and no terminal outer
report. It is not a completed build. Its old processes cannot be reused after
the reboot. The separate failed r1 report still records72 qualified TUs and a
failed exporter launch; that batch also remains failed.

## Memory-aware parallel build implementation and actual tests

`tools/ros_distro/build_parallelism.py` plans concurrency using the original
60-GiB working ceiling, host `MemAvailable` minus8GiB, the unchanged aggregate
command RSS limit, an explicit per-job reservation, current CPU affinity,
pinned cpuset, ancestor CPU quotas, and the profile's job maximum. The real
Gateway and native-basic CMake build entries use this plan. Existing `jobs:1`
profiles stay serial; an explicit larger profile cap permits more jobs only
when every budget allows them. Top-level profile and behavior commands remain
sequential. Neither environment controls nor CPU count raise a memory limit.

| Actual scope | Result | Evidence |
| --- | --- | --- |
| Planner boundary regressions in the current64-GiB leaf |14 methods passed | `continuation-20261016-boot2-evidence/memory-policy/report.json` |
| Real CMake build through `Preparation.build_target` | Selected `-j4`; four target compiler processes actually overlapped; all five C translation units linked and the checksum program exited0 | `continuation-20261016-boot2-evidence/cmake-parallel/report.json` |
| Genuine Source40 failed-batch schema regressions |14 methods passed using the unchanged real b7 report | `continuation-20261016-boot2-evidence/typed-schema/report.json` |
| Explicit historical hardlink policy |15 actual kernel-filesystem test methods passed; all fixtures were under remote `/tmp` | `continuation-20261016-boot2-evidence/hardlink-policy/hardlink-test.json` |
| Real native profile-selection membership |Six methods passed against the actual GO/HG/API output lists, including strict rejection of an unknown same-name historical pin | `continuation-20261016-boot2-evidence/typed-membership/typed-membership-inner.json` |

The policy report SHA256 is
`10e74720844621bb3d5615786ef8234ef8a301a9db724aa9bdfc9fef65f6a4df`.
The real parallel-build report SHA256 is
`ffb048c10962727a898194be11f00a0aecffff1fe439ff95a013da183c46d6c9`.
Its observed owned process-tree peak was405970944bytes, below the selected
512-MiB aggregate command ceiling. Cleanup left no actors, and OOM counters
remained zero. This generated C project verifies the build method and process
containment; it establishes no ROS interoperability or native/Wasm performance.

The genuine-schema report SHA256 is
`5aaf4f52521e0c516ce59ac9da77394311e809b91e8ad07ff26904f4b8dc543b`.
It establishes that the original failed Frame report has no `source_before`
member and its RSS-interrupted TinyGo command has no recorded `returncode`.
The corrected predicate does not manufacture either observation. It selects
exactly nine completed Rust/JavaScript/AssemblyScript products; the partial
TinyGo row remains incomplete. These metadata tests do not requalify the
retained binary files or execute the remaining five compiler targets.

Twenty-five actual typed protocol unit checks also passed, including all60/105
binary32 carrier words, canonical u32 parsing and independent quiet-record
bounds. Their inner receipt is SHA256
`d56f9ad3eb5c101cdf58072640e3d679b3b839f7a421d3f5cb8b9f5afcf450a0`;
the supervisor receipt is
`41c0822cac9b56b956dd54dbad11b5a6669661bb15228ccf20c90d8c2d131705`.
The exact reports/log are under
`continuation-20261016-boot2-evidence/typed-protocol-tests/`. They started
zero Runtime or native DDS cases.

No tests or compilers were run on the local Mac in this continuation. Local
operations were source authoring, syntax parsing, hashing and evidence copies.

## Pending acceptance

The durable restart helper keeps historical and current pins separate and
records only content requalification. The Native74 r4 prerequisites completed
actual content requalification for26 selected files and six complete trees.
Its inner receipt SHA256 is
`4755b806ec13325e2d26f845f1a7f16a609092d030f10dc2463dabea5c6b7ec0`.
That receipt does not execute an old binary or promote an old failed batch.

Native74 r4 failed before starting a compiler. Its retained model report named
three exact `/etc` files and one systemd resolver file, which were outside its
explicit historical/current file mapping. The three persistent files retain
the same inode, times and complete bytes; only device66307 to66308 changed.
The936-byte resolver has equal complete content, but a new inode and times,
so it needs a distinct content-only bridge with no identity continuity claim.
The raw failed inner and outer reports are preserved under
`continuation-20261016-boot2-evidence/native74-r4/`. Their SHA256 values are
`05378bf4173569bb416b03153995a7c2e2cf65a43c374e7c09e02faac73806cf`
and `259208b3ac6ee60dbaa3b1e58cadd3b61068cf4925fcfdedd54b1c32fc59d503`.
The outer source, inputs, products and reused SDK were preserved, all actors
were cleaned up, and current OOM counters remained zero.

The separately reviewed r5 successor adds only those exact metadata selections
and an earlier full retained-model-input check. It preserves bounded chunks
of up to four actual object targets, a5-GiB reservation per heavy compiler,
a20-GiB compile process-tree ceiling and a24-GiB outer ceiling. Linking stays
serial. The existing r4 staged helper and controls remain immutable.
The actual r5 build compiled and linked all69 Gateway TUs plus the portable
String peer, execution gate and String metadata exporter:72 TUs. It then
failed before the metadata exporter's cooperative mapped stop because the
copied Ubuntu loader selected the host libc. The SDK has separate physical
`lib/x86_64-linux-gnu` and `usr/lib/x86_64-linux-gnu` directories, and only the
latter had been included in the recorded library search. Actual stderr names
the host libc and missing `__tunable_is_initialized@GLIBC_PRIVATE`.
The raw failed inner/outer reports remain in
`continuation-20261016-boot2-evidence/native74-r5/`, SHA256
`26548e8e82d03c86d948149737180c33acd207156a5af70850edbee5399d615f`
and `e3e2f44de2dab5111f9a1ec82612727c929a292dc6d4b2556c99dcf6da71fafe`.
They are not a completed74-TU batch or DDS result.

A separate bounded actual preflight then executed the retained String
exporter with the copied libc directory added to its explicit environment.
It returned genuine String identity metadata and completed owned cleanup;
receipt SHA256 is
`1d946971507b32a7f8441332be3b4cd2345b941ca83dc2bd3c1b6640628ac719`.
That result qualifies this metadata preflight only, not complete mappings,
the old failed batch or native delivery. Its exact raw receipt and log are in
`continuation-20261016-boot2-evidence/native-libc-smoke/`.

The independently reviewed r6 producer adds the actual copied libc directory
to its shared native DT_RPATH/environment selection and records its physical
ELF beside the actual copied loader. No bind destination is created or guard
weakened in the preserved SDK. The r6 build compiled and linked all74 fresh translation units. Its next
ordinary execution-file catalog step failed when reopening the active
`/dev/null` character-device bind through its original ordinary mountpoint
name. The ordinary-file check correctly refused it. The raw failed inner/outer bytes are retained under
`continuation-20261016-boot2-evidence/native74-r6/`, SHA256
`eb5e185dbb38efebe90faabb2a7e21c10a33005ba31af9ca3a6a071e54a72487`
and `169b495fabd271f5c76ea88057659c6da6eb1f53551d0ee6dece5090dd880a7f`.
Source, inputs, products and reused SDK remained preserved; cleanup passed
with no finalization errors. The raw failed batch remains failed; a separate current at-rest admission is being implemented to
verify the complete recorded MD/object/link/source closure and catalog the
restored ordinary SDK files after owned unmount cleanup. This can retain
actual completed compilation without claiming another fresh74-TU build or
any typed DDS/physics behavior.

The separately versioned hardlink helper defaults to rejection. Its explicit
policy admits only complete historical in-root alias classes whose current
kernel link count equals the exact known member count. The15 test methods
exercise actual links, split/merged classes, unaccounted external aliases,
content/topology drift and symlink non-following. The test report SHA256 is
`1715a1518ff8c4f11cc58280d244dd53cbcd30850d5c0591a7441990e3428796`;
the outer receipt is
`efdee7d1eb008892100f0c2c389bf3a036df1c748ac036b3629a5180517c630e`.
This synthetic policy pass is not requalification of a complete SDK or proof
of ROS behavior.

Typed40 source staging and its separate metadata preparation have also
completed. The actual prepare owner receipt is SHA256
`cf0b89bdabaf4082d3a9cb3abc3b62c38d0b512b8924750ba02a71c6810a0d1b`;
its zero-actor inner preparation receipt is
`2e53963609a2bf1f0539294d712a155536764c079e4d6a308c338f76a05fafc8`.
Exact original bytes and remote file observations are under
`continuation-20261016-boot2-evidence/typed40-prepare/`. Both observed62
complete input roots before and after preparation, preserved source and
selected input/product files, and completed cleanup. This phase started no
compiler. Its separate first build phase failed before any compiler command:
the physical-pin comparison attempted to admit an unrelated differently named
entry while scanning a real native profile-selection output list. Source,
inputs and complete roots remained preserved, with owned cleanup and zero OOM.
The failed inner/outer reports are retained under
`continuation-20261016-boot2-evidence/typed40-build-r1/` (SHA256
`ad2b4c71d71c492353a37a556f5463292ebc20dfc8217b6eb780417316114e98`
and `22ea686bba03aacbae61984b01e57f1029a9d0ee965472b11d6e8da586a79d46`).

The r2 successor returns false for different full-seven paths before
attempting a historical/current bridge. Same-name unknown observations still
fail closed. The builder changes only its adapter hash; owner/stager output
paths are fresh r2 names. Six actual three-family selection-metadata regression
methods passed, with inner receipt SHA256
`506dddaf1cb13c6a20b7a46aaebd7cc8c51939480932f057ad30a46c72f8d75c`.
The r2 metadata prepare also passed with62 before/after roots, preserved
source/inputs/products and completed cleanup: outer SHA256
`b612e4e54c1d2521acfb4f9e7d5f88a563191daf9beb91a3bfc29c8a9685f93d`,
inner `60c8b3c63e7f061f72546934894f522045d0e3de320a75abe5f92428fa7ab683`.
Exact bytes are under `continuation-20261016-boot2-evidence/typed40-r2-prepare/`.
The separate r2 build also failed before any compiler command. Its strict
in-root hardlink policy found actual cross-root classes in `/usr/bin`: ARM
binutils aliases and coreutils have additional kernel hardlinks outside that
single catalog root. The raw failed inner/outer reports are under
`continuation-20261016-boot2-evidence/typed40-build-r2/`; the inner SHA256 is
`36194ac095d9f59d786eca448c772042134a682524f5090a36fe84ed83db33f2`.
A separate finite complete-catalog union policy is being implemented; no
unknown external alias or historical link count is assumed.

Root may admit the small compiler/metadata continuation beside Native74 only
after a conservative headroom check: current cgroup usage plus the full24-GiB
native outer allowance,1.5-GiB Typed outer allowance and2-GiB control reserve
must remain below the original60-GiB working ceiling. The original hard64-GiB,
zero-swap limit and each owner's live guards remain in force. Actual jobs and
source-only subagent work retain separate execution authority.

Ten actual Noetic partial-resume filesystem regressions passed under the
original64-GiB cgroup. The inner receipt SHA256 is
`e571ce9c070407d1624b78c9b47dfaa3b3e10bc4c524c82a5be4174ce98dc4ad`;
the owned supervisor receipt is
`88c5269ecc670d6d1d814a0bc2693e9849bf1168f25f5365b35786b4bfa5af6b`.
They exercise small real filesystem fixtures, including no-replace directory
transfer, and qualify no real SDK. Separately staged current r5 controls
completed the literal maintained-SafeTree comparison against the genuine r4
producer. The real r5 owner subsequently failed with
`ValueError('owner backing-filesystem reserve exhausted')`. Its outer report is
`FAIL` with owned cleanup `PASS`; its unchanged inner report remains `RUNNING`
with329 package records and `cleanup=NOT_PERFORMED`. The same reserve guard
also interrupted cooperative-grace and final read-preservation stages, so no
complete-SDK or input-preservation success is inferred. Exact raw inner/outer
reports are retained in `continuation-20261016-boot2-evidence/noetic5-failed/`,
SHA256 `fa300972b9639153446ac2005d31944b748f7d3de12cd2f799db5abcb668ceb1`
and `0c32ebee7563a3d8e25c0ad54a115850986beb40410debd36a4d63b18ba9901d`.
The separate current reobservation and continuation boundary are documented in
the third-admission checkpoint.

Typed40 continuation, the final native74 artifact admission, positive/denied
typed transport, completed Noetic acquisition and native ROS1 transport still
need their actual completed reports.

Controlled ROS/MuJoCo stepping, matched native/Wasm timing, conditional genuine
VTune hardware measurements, the patched LLVM provider, debugger acceptance,
and the complete QEMU/distribution matrix remain separate unfinished work.
Nothing in this checkpoint qualifies them or enables a default Guest right.
