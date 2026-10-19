# Validation checkpoint after the third Linux admission

The current admitted boot is `501c54bd-af24-4d22-900c-8e24de274464`.
The task remains in its original container leaf, with cgroup device29,
inode18358, CPUs0–31, `memory.max=68719476736` and `memory.swap.max=0`.
The initial and small-policy-test observations show zero ancestor OOM counters.
The host backing device is now66312. These are new current observations, not
continuity of the earlier boots' processes or cgroup identities.

The original Btrfs artifact image was restored without formatting or filesystem
repair. Its UUID remains `3fe7718a-8039-4f1c-b540-7c5cf05b65ca`, with opened
artifact root device81/inode256 and `/dev/loop29`. The restoration receipt is
[`storage-restoration/report.json`](continuation-20261016-boot3-evidence/storage-restoration/report.json),
SHA256 `5989eecc3d7aab34b0070e4138720535014b9c848922df78f0ce117b2aac490e`.
It qualifies the restored volume and current guard only. A repeated device
number does not qualify an old process, old mount snapshot or old execution.

## Actual completed observations

Five small real Btrfs CoW/alias-policy tests passed in this current cgroup.
They exercised required FICLONE copying of ordinary/empty files and relative
aliases, private mutation of the clone's independent inode, and refusal of
hardlinked source files, absolute aliases and escaping relative aliases.
The source helper remains12125 bytes, SHA256
`8cd1081ff18a1123c228ece6e696750d5f607dcff660f3510e452979637ef82c`.

| Exact retained evidence | SHA256 |
| --- | --- |
| [Five-test inner report](continuation-20261016-boot3-evidence/cp310-cow-policy/inner.json),624 bytes | `1258060291675c143b55156f9a30b6edbbf77ad982757424d182b7d35c957964` |
| [Owned supervisor report](continuation-20261016-boot3-evidence/cp310-cow-policy/owner.json),7105 bytes | `8d109729aca6d168ed39d96eb43e51996ba93af15d05eb1b64fe69338e47202e` |
| [Actual test output](continuation-20261016-boot3-evidence/cp310-cow-policy/tests.log),800 bytes | `168c21a137f4b2fb654c044a1116dafca3ca3de692d91a30aa2bf95b6e2e5658` |

The owned test command returned0, used28905472 bytes peak owned RSS, preserved
its source and completed cleanup without remaining actors. It executed zero
Runtime cases, no CP310 code and no native ROS. This establishes the small
filesystem copy policy; the complete1.26GB CP310 view, actual namespaces,
ROS/MuJoCo physics and performance remain separate obligations.

Nine guarded metadata regressions also passed against the three genuine
family-selection documents and unchanged b7 raw report. They validate finite
same-device full-file identity admission and rejection of altered fields; they
start no compiler or Runtime. The [inner receipt](continuation-20261016-boot3-evidence/genuine-same-device9/inner.json)
is14459 bytes, SHA256
`a8b329d155d2053401fa1716c5efed4aa37659bd04667bca13da56c253b0fe03`;
the [owned supervisor](continuation-20261016-boot3-evidence/genuine-same-device9/owner.json)
is7976 bytes, SHA256
`2201a17d34e808b54b672e5bc8d4d10a949537bf4834c7c4d48b4f5f36be30e6`.
Their [actual output](continuation-20261016-boot3-evidence/genuine-same-device9/tests.log)
is1740 bytes, SHA256
`fb4a3b451db5f5f2eecd15fb24ba5adcf220dc24a67a81637638139119a359db`.

Eighteen real small-kernel hardlink-union/auxiliary-policy tests passed against
the exact unchanged helper SHA256
`8fbc2d1da44ef07bd8e6f69f4b174df70d802c471e989c1e752073cc95627d61`.
The [inner receipt](continuation-20261016-boot3-evidence/hardlink-union18/inner.json)
is11950 bytes, SHA256
`ac44224cd8f173386a5b8ffb2015fcf650c43f3687bb76e56c644c1b0931044e`;
the [owned supervisor](continuation-20261016-boot3-evidence/hardlink-union18/owner.json)
is10068 bytes, SHA256
`dcd7a11a09113066782573128841fb52c0a834f1b1d2da777b57b78013c3eea2`.
The [actual output](continuation-20261016-boot3-evidence/hardlink-union18/tests.log)
is2809 bytes, SHA256
`06256e53bc7eb65e04c58b44ba532c51c54c2ccbdaae2aa3c77004487c671460`.
These small tests do not substitute for the actual production input closure.

Six additional real-kernel tests passed for the separately selected exact-file
alias wrapper, using unchanged helper SHA256
`bd82f8a058b887462018102aa0f9d8a794e7196ee197e31c1a8661c7b4da07b9`
and guarded test source
`b61ab09c881fb157e96cc6263e820434094225ee45daa601061268c7e4827455`.
The [inner report](continuation-20261016-boot3-evidence/exact-alias6/inner.json)
is12274 bytes, SHA256
`e8b29af0c1e8f3477226993c6631e37f95299356a5f8e28fcb0d73f0eaebb795`;
the [owned supervisor](continuation-20261016-boot3-evidence/exact-alias6/owner.json)
is10673 bytes, SHA256
`f3ec190857eeb83ab604e29d18482283ef62119f56c3b396e1b1229301661349`.
The [1207-byte output](continuation-20261016-boot3-evidence/exact-alias6/tests.log)
is SHA256 `ac201b5ed6acca3801c13e3f3833eb84301d74b78d4cedc5911909a23cc5f043`.
They exercise tiny real hardlinks under `/tmp`, exact-file/sibling separation,
strict unknown-link refusal and preservation. They qualify no SDK, compiler,
Runtime, ROS transport or complete production union.

Twelve small Noetic continuation-policy checks also passed. The
[owned report](continuation-20261016-boot3-evidence/noetic6-small-r3-pass/owner.json)
is10105 bytes, SHA256
`3891fab7fbb0a747a9ddf9eb16c1fc5eee2b7d6f25024431cbc808232ac534cc`;
the [1972-byte output](continuation-20261016-boot3-evidence/noetic6-small-r3-pass/tests.log)
is SHA256 `7e5c1c899d1cfb63070cc6f7dab5b05c9742f7bf14ab712334e12bced4a271ca`.
This is a small-policy result. The separately started Noetic r6c513-package
continuation subsequently failed its active eight-GiB backing reserve guard.
Its actual [inner report](continuation-20261016-boot3-evidence/noetic6-r6c-failed/inner.json)
is1048328 bytes, SHA256
`dae93ed7054d73e483221317bd131de300dea7cbb1fa63d308835c1f74592940`,
status `FAILED_ACQUISITION`,109 commands and zero completed package records.
The owned cancellation was signal15. Its [outer report](continuation-20261016-boot3-evidence/noetic6-r6c-failed/owner.json)
is58936 bytes, SHA256
`23d3ab88e91faae9f9622f6246b54ad17865fd91fa6c20aaa345d2ea285160e9`,
status `FAIL`. Cleanup passed, source/input/product preservation passed and
finalization errors were empty; no ancestor OOM was reported. This new failure
is separate from old Noetic r5's329 unreconciled records. Neither the12 tests
nor this failed acquisition qualifies a complete SDK or native ROS1 transport.

The complete Source42 bytes were independently frozen in two full hash passes:
18063 files,218144333 bytes, manifest SHA256
`106bc26fb66a37cc3d98c47cf4525fff8383402ac0715857dfcbafcc1b7a48b5`.
The [6056-byte freeze-owner receipt](continuation-20261016-boot3-evidence/source42/owner.json)
has SHA256 `66ccfff92f4ccb3397fb48b72f1563eaa7dd2bf94a7bb0e9f90f8e605e0b37e4`
and explicitly reports bytes-only success with no compiler, candidate import,
Runtime, DDS or simulator execution. Later r4 control sources and this updated
checkpoint are separate from that immutable complete freeze.

The later Source43 complete freeze contains18108 files and219967127 bytes.
The unchanged [construction report](continuation-20261016-boot3-evidence/source43/construction-r1.json)
has a stale ancillary top-level manifest field naming Source42; its primary
`source_state` already names the actual Source43 bytes. Root retained that raw
report and independently repeated two complete current hash passes. The
[6249-byte requalification receipt](continuation-20261016-boot3-evidence/source43/requalification-r2.json),
SHA256 `de868c7040308ed8702399feba7e7ad823ef14da4fc93d7dd1d4b61933fb7d13`,
explicitly documents that summary-field error and qualifies the actual complete
Source43 manifest
`9a01f114e4091d4329392b291b5785952e99b22d8e2a48be52b7e02a7d8f0178`.
This remains a complete-source bytes-only result, with no build, candidate
import, Runtime, DDS or model execution. New r5 control code and the external
Typed28 maker draft remain separate from that unchanged frozen source.

## Retained failures and partial work

Noetic r5 failed its backing-filesystem reserve guard. Its original inner
report remains `RUNNING`, with329 package records and `cleanup=NOT_PERFORMED`;
its outer report is `FAIL`, records
`ValueError('owner backing-filesystem reserve exhausted')`, and reports owned
cleanup `PASS`. The same reserve failure also interrupted the outer
cooperative-grace and final read-preservation stages. Consequently there is
no complete-SDK, source/input-preservation or compatibility success to promote.
The329 records are not a completed reconciled package closure.

The unchanged raw reports are retained under
[`noetic5-failed`](continuation-20261016-boot2-evidence/noetic5-failed/inner.json):
inner6283350 bytes, SHA256
`fa300972b9639153446ac2005d31944b748f7d3de12cd2f799db5abcb668ceb1`;
outer41860 bytes, SHA256
`0c32ebee7563a3d8e25c0ad54a115850986beb40410debd36a4d63b18ba9901d`.
The [current passive reobservation](continuation-20261016-boot3-evidence/noetic5-current-facts.json)
separately binds the partial SDK, raw reports, payload ledger and pending tar.
That ledger is9988325 bytes, SHA256
`d4d6c432569455916b4fec9bfd1df2c63e7f98e6bb04975001b92526d403f021`.
A separately versioned continuation must reconcile those real partial bytes
and signed package inputs before any complete acquisition result is accepted.

Native74 r6 compiled and linked all74 fresh source translation units, including
the actual Go2 native controller. Its subsequent execution-catalog preparation
failed because a mounted `/dev/null` placeholder was opened as an ordinary
file. The original inner and outer statuses both remain `FAIL`. Source, inputs,
products and the execution view were preserved; cleanup passed, no mount
remained, and finalization recorded no errors. No Runtime, typed native peer or
physics controller execution was qualified by this result.

The [unchanged native inner report](continuation-20261016-boot2-evidence/native74-r6/inner.json)
has SHA256 `eb5e185dbb38efebe90faabb2a7e21c10a33005ba31af9ca3a6a071e54a72487`;
the [outer report](continuation-20261016-boot2-evidence/native74-r6/owner.json) is
`169b495fabd271f5c76ea88057659c6da6eb1f53551d0ee6dece5090dd880a7f`.
Their actual source is still Source39, complete manifest
`54d6c79978e36bd13fa37272d8dd5f2cc94a9d2e62ac437b51dfc4aa4f6ae5b9`.
The separate Source43 retained74 at-rest finalizer has now completed actual
admission of the full compilation, MD/FileAPI/link/product evidence and fresh
ordinary catalogs after cleanup. Its [inner receipt](continuation-20261016-boot3-evidence/native-retained-r2-pass/inner.json)
is1169663 bytes, SHA256
`8503a1b4c02c8b385648b2bd12a338202a8d31d4dfaf0ee8608b12c2b3cfbda7`,
status `PASS_RETAINED_NATIVE74_BUILD_PRODUCTS_FRESH_AT_REST_CATALOG_NOT_EXECUTED`.
The [owned supervisor](continuation-20261016-boot3-evidence/native-retained-r2-pass/owner.json)
is8858 bytes, SHA256
`d2f556b05bfa71bbe9271b0971527581e59b8eee5b3a44f2c1ecd94168111f0b`.
Source, input pins, products and whole roots were preserved; cleanup passed.
It ran zero compiler, Runtime, DDS or physics commands and performed no mount
operation. Both original r6 raw statuses remain `FAIL`. This is a distinct
retained-compilation/current-at-rest catalog result, retaining original Source39
compilation provenance; it does not qualify an actual mapped SDK or controller
execution. No fourth74-TU recompilation was required for the fresh catalog.

## Implemented successors awaiting actual acceptance

The separately maintained
[`typed40_boot2_r3`](../../tests/ros_unitree/acceptance/typed40_boot2_r3/README.md)
package has independent source review without a blocking finding. It eagerly
qualifies the original59 complete historical catalogs plus one exact cargo
catalog/report anchor before any compiler can start. Ten current-only ARM
names are bound to nine genuine old inode classes; their old extra names and
link counts remain `NOT_RECORDED`. The Frame limit stays64 roots. Exact
same-device81 observations require complete streamed equality of all seven
file fields and confer no boot/process continuity. Original source manifests,
failed reports, nine retained/five fresh product provenance and default empty
Guest authority remain unchanged.

Its eight-role source payload is442685 bytes, SHA256
`e9e91657c828fac93c2652d31e7e290ef406bc808af3944e6a81a83f0ee9e6ef`.
The independent peer receipt is3599 bytes, SHA256
`9b6e3aea546932769f2dfe67ac8758a88693bf4dc5ecd97771f81812720657a6`.
Its current-boot source staging and separate metadata preparation have passed.
The [stage receipt](continuation-20261016-boot3-evidence/typed40-r3-stage-prepare/stage.json)
is12399 bytes, SHA256
`9edd69d1a1290570bcd25bd1679605aa9bb20b1667e49964c909fbc8b56be31e`.
The [prepare supervisor](continuation-20261016-boot3-evidence/typed40-r3-stage-prepare/owner.json)
is170615 bytes, SHA256
`4ed5a0018254292cfe802f1d7492196dc509fceccca3d6b53029c2a75dcbc758`;
the [zero-actor inner metadata receipt](continuation-20261016-boot3-evidence/typed40-r3-stage-prepare/inner.json)
is116627 bytes, SHA256
`b45a628ac996f207b1bf535ef24357e4dc12e37f7df3157011fd61133f3aba3e`.
The separate actual r3 build then failed safely during eager input admission,
before any compiler command. Its unchanged [inner report](continuation-20261016-boot3-evidence/typed40-r3-failed/inner.json)
is122083 bytes, SHA256
`bf2cb9f2879f7c2d66ea9240a34787e6640d3ba9da9e9c0fa4cfcac11959b66c`;
its [outer report](continuation-20261016-boot3-evidence/typed40-r3-failed/owner.json)
is172599 bytes, SHA256
`289133bf5a0b16f5055d545980b19c96342211e04a3b8419f0a049459e781e45`.
Both raw statuses remain `FAIL`. The inner `commands` list is empty. The outer
owned coordinator returned1, used345853952 bytes peak owned RSS and completed
cleanup with no remaining actor; source/input/product preservation passed and
no finalization error was recorded. The strict refusal was:
`current nlink differs from all distinct selected historical/current auxiliary names; external alias refused`.

The [stat-only complete60-root class diagnostic](continuation-20261016-boot3-evidence/typed40-r3-failed/all60-class-diagnostic.json)
examined166009 distinct old classes. Exactly two actual library classes still
had link count2 while only one regular name was selected: `libc.so.6` old inode12359710
and `libstdc++.so.6.0.35` old inode12322103. Root separately discovered and
fully hashed their [two actual second names](continuation-20261016-boot3-evidence/typed40-r3-failed/two-host-lib-current-aliases.json)
in exact `/root/.debug` build-ID leaf directories. These are current-only names,
not invented old catalog entries. A separately maintained
[r4 source successor](../../tests/ros_unitree/acceptance/typed40_boot3_r4/README.md)
adds only those two one-file leaf inventories, leaving the helper, strict link
count equality,64-root bound, old r3 package, Source42 and empty Guest authority
unchanged. The actual r4 build also failed before any compiler command:
its unchanged [inner report](continuation-20261016-boot3-evidence/typed40-r4-failed/inner.json)
is115951 bytes, SHA256
`e56779aee0c20057cd60cedbb52d3482da69838bdc2d514e65c8892590a0a8f3`;
its [outer report](continuation-20261016-boot3-evidence/typed40-r4-failed/owner.json)
is172594 bytes, SHA256
`4688124808f2bbf99450a197f7464615d6dac0b1cd9a393728716a009af65d0e`.
The inner `commands` list remains empty; source/input/root preservation passed.
The outer owned child returned1, cleanup passed and finalization errors were
empty. Its strict refusal was `unanchored multi-name auxiliary inode refused`.

The [actual descriptor-read diagnostic](continuation-20261016-boot3-evidence/typed40-r4-failed/exact-auxiliary-guard-diagnostic.json),
14564 bytes, SHA256
`0639e2cf4bf5db3e626baf1091b666607f15bc76633804b130da59da9c7bc67a`,
showed that the libc build-ID leaf also has an unrelated hardlinked `debug`
file and an ordinary `probes` file; the libstdc++ leaf has `probes`. The earlier
one-file-leaf assumption was incorrect. A separately maintained
[r5 source candidate](../../tests/ros_unitree/acceptance/typed40_boot3_r5/README.md)
selects only the two genuine current `elf` files through a new explicit policy
wrapper, preserving strict global link counts and the complete ARM directory.
No debug sibling or parent directory is selected or claimed as a complete
inventory. Its actual admission and the five real continuation compiles remain
pending. Metadata preparation and the small9/18 policy tests do not qualify
either failed production closure.

R5 source staging and separate zero-actor preparation have now passed. The
[stage receipt](continuation-20261016-boot3-evidence/typed40-r5-stage-prepare/stage.json)
is12775 bytes, SHA256
`b455633237c361e57394fb199bb7af76c78dd1d08b1c93b748cab45ffb403698`;
the [prepare owner](continuation-20261016-boot3-evidence/typed40-r5-stage-prepare/owner.json)
is171041 bytes, SHA256
`d4ec75b77f4cd45945107cbfd4f527d1b416c5950e8e3bf993112c3845b918b7`;
the [inner metadata result](continuation-20261016-boot3-evidence/typed40-r5-stage-prepare/inner.json)
is117386 bytes, SHA256
`0697f93d3e39fb2a9148ebb9cb077ba60660c1e1db8688649b502b07a87a01ba`.
The first build launch was withheld when backing space crossed the unchanged
8GiB floor. The subsequent actual r5 build attempt again failed the same reserve guard
before any compiler command. Its [59975-byte owner report](continuation-20261016-boot3-evidence/typed40-r5-build-failed/owner.json)
has SHA256 `9f61cc6bd1ccb51d6105ec3b2e4180528d3ecba131b0cbfd221d593495d19ef6`
and status `FAIL`. Its unchanged [2974-byte inner report](continuation-20261016-boot3-evidence/typed40-r5-build-failed/inner.json),
SHA256 `e489149fd74bce4536729719096bee127242413d7f5ce38bcf16be6970f71522`,
remains `RUNNING` with zero commands. Owned cleanup passed with no remaining
actors, but the reserve failure also interrupted cooperative cleanup checking
and final source/root/pin preservation, recorded as two finalization errors.
The raw inner is not a successful continuation or a preserved terminal build.
The production60-root/exact-file proof and five fresh compiles remain pending.
The separately reviewed [r6 retry package](../../tests/ros_unitree/acceptance/typed40_boot3_r6/README.md)
changes only the four output path constants and stager receipt suffix. All
compiler functions, eight non-owner payload roles, exact alias selections,
limits and reserve guards are unchanged. Its independent source peer found no
blocking issue. After the [verified storage recovery](storage-recovery-20261017-boot3.md),
Actual r6 stage and zero-actor inner preparation passed, and Root launched its
build. The [stage receipt](continuation-20261016-boot3-evidence/typed40-r6-stage-prepare/stage.json)
is12775 bytes, SHA256
`83c3a6e5502b1fc2e7583f658d6799cce7aef740698437cd7fc3c3ab4731c2dd`;
its control is5177 bytes, SHA256
`4308bee56e83a9e60f18bbbf81dee21394013b04984b29ede04af4a7b0b8b7b1`.
The [171041-byte prepare owner](continuation-20261016-boot3-evidence/typed40-r6-stage-prepare/owner.json)
has SHA256 `b7bfd2da6d86213ebaf82af6049e54aa07ecaf973b87686dd442fce10c8cbc06`;
the [117386-byte inner](continuation-20261016-boot3-evidence/typed40-r6-stage-prepare/inner.json)
has SHA256 `69d814b47d3d46d901c0ab2d47f3244020de372b516633f4b05403c6d931b2f2`.
Both have status `PASS_TYPED_COMMAND_POSTBOOT_INPUTS_ONLY`; the inner started
zero actors. Owner preservation and cleanup passed with empty finalization
errors. The actual build is still running; this preparation qualifies no
compiler command or production-union result.

The Source43 CP310 [stage metadata receipt](continuation-20261016-boot3-evidence/cp310-stage/stage.json)
is16432 bytes, SHA256
`6a68dcc8c7d2f080aa4bbf1c74a10cda3a87b3e25665e5d0c4b3f5531e3654ac`.
The subsequent full-view prepare failed the same backing reserve guard. Its
[15185-byte owner report](continuation-20261016-boot3-evidence/cp310-prepare-r1-failed/owner.json)
has SHA256 `2399d4a43c4b29290db7c7dbf65ba2e984faa1042bc33d809156290290ca88ed`.
It requalified the cached1,266,016,500-byte ordinary-file/symlink content set,
but supplied no complete new independent CoW-view success. No child or target
CP310 process started; source/input/product preservation and cleanup passed,
finalization errors were empty and no mount remained. The later namespace,
real ROS/MuJoCo model and first Guest-callback work remain unexecuted.
The separate r2 full-view preparation also failed the reserve guard. Its
[15185-byte owner report](continuation-20261016-boot3-evidence/cp310-prepare-r2-failed/owner.json),
SHA256 `dc31cab85b4255350386ef404ffde4e2ae1f7504d7f27fe271a6df8905af62d1`,
records zero commands, preserved source/input/products, successful no-actor
cleanup and no finalization errors. It supplies no complete new view.
After storage recovery, r3 [9499-byte staging owner](continuation-20261016-boot3-evidence/cp310-stage-r3/owner.json),
SHA256 `45930f6bcd335022c24522dc4c950d258a8226c657d8afe77c67104a8066da4d`,
passed metadata preparation. Its [16432-byte stage](continuation-20261016-boot3-evidence/cp310-stage-r3/stage.json),
SHA256 `5573abd197b2e0ab16e19eb71588b280a4c5e5177f22ce6b30e97fcffd07f57d`,
is `STAGED_SOURCE_AND_CURRENT_METADATA_ONLY_NOT_EXECUTED`. The subsequent [17685-byte full-view preparation report](continuation-20261016-boot3-evidence/cp310-prepare-r3-pass/owner.json),
SHA256 `bcbc8323f0744d58a777d050735b268ae6e1dd1a07a7ac8d18f39a1825bf4644`,
is now `PREPARED_CONTENT_ONLY_NOT_EXECUTED`. It copied32046 files and1265
symlinks,1,266,016,500 file bytes, using31611 successful FICLONE operations for
nonempty files. Source and clone were fully hashed with independent inodes;
no hardlinks or ordinary-copy fallback were used. The fresh complete catalog
comparison, source/input/product preservation and no-actor cleanup passed,
with empty finalization errors and no remaining mounts. Commands remain empty
and target code was not executed. This qualifies the independent content view,
not namespace setup, CP310 import, ROS/MuJoCo stepping or a Guest callback.

The Noetic6d [cache-view receipt](continuation-20261016-boot3-evidence/noetic6d-cache-view/report.json)
is39015 bytes, SHA256
`9985cef5f0398c4f48b05142c9b6850e2b6c14b286c4f9de64b759af8ba6de9a`.
It prepared41 CoW DEB candidates with no ordinary-copy fallback, left old inputs
unchanged and started zero actors. It performs no new signature admission,
package extraction, network or compiler operation and promotes no old failure.
The [storage recovery record](storage-recovery-20261017-boot3.md) links all six
complete archive/audit/retirement chains, including the persistent external
archive locations. The final observed backing availability was14,495,014,912
bytes; the eight-GiB guard remains enforced for subsequent work.

Real standalone C11, C++20 and embedded-JavaScript Go2 mains, six binary build
recipes, independent receipts and the three-case controlled-physics coordinator
are implemented. They use the same SDK C kernel and official command CRC as
the native controller. The private CP310 owner/stager has a separate
SHA-bound current-context successor. Neither source review nor the five small
CoW tests qualify the complete copy, first real Guest callback, native DDS
delivery, actual model stepping or matched performance.

Positive/denied Typed transport, fresh native ROS1 transport,
controlled ROS/MuJoCo comparisons, actual timing, conditional genuine
VTune hardware-counter optimization, the patched LLVM provider, debugger
acceptance and the complete QEMU/distribution matrix remain unfinished.
All actual programs and descendants must remain in the current hard64GiB,
zero-swap cgroup; any permitted build concurrency is additionally limited by
its current headroom and owned RSS/process/deadline guards.

## 2026-10-17 complete fresh Noetic513 dependency result

The actual r6d owner exited0. Its unchanged
[inner report](continuation-20261016-boot3-evidence/noetic6d-fresh513-pass/inner.json)
(19,452,589 bytes, SHA256
`37ae443a6fbcd65d9e380cc58876c649ee43ce5ce55b401b20db452fa43846c2`)
has status `PREPARED_AUTHENTICATED_EXTRACTED_SYSROOT`, all513 freshly processed
packages, all4,414 actual commands PASS/return0,8,880 product pins, complete
acquisition and successful source/input/product preservation and owned cleanup.
The independently terminal
[owner report](continuation-20261016-boot3-evidence/noetic6d-fresh513-pass/owner.json)
(3,972,954 bytes, SHA256
`910228dcf1e030da62753519f6158944b586034c0a89e2b5b3170ec8114473b0`)
has status
`PASS_AUTHENTICATED_FRESH_ALL513_INDEPENDENT_COW_CONTENT_SYSROOT_ONLY`.
The exact [control](continuation-20261016-boot3-evidence/noetic6d-fresh513-pass/control.json)
and actual current full-seven observations accompany both raw reports.

The SDK contains39,414 regular files and1,711 symlinks,2,373,090,564 regular-file
bytes. Its actual final catalog is12,085,612 bytes, SHA256
`273281b99c58427130d862b3e3bc4cb0a42b303cc7de419e3db407a3f880c732`.
It remains a content/authentication result: no target/compiler, native ROS,
Wasm Runtime, TCPROS, simulator or performance case ran in acquisition. Old
failed reports and329 historical package rows were not promoted. The actual
same64GiB/swap0 context and zero-OOM evidence are retained in the reports.

## Maintained-source changes awaiting the next complete freeze

A source review found a genuine permanent lifetime limit in four Service/Action
identifier domains: each retained every historical ID and refused all future
allocations after1,048,576, even after completion or failed enqueue. The
[constant-space fix and regressions](../../tests/ros_ticket_lifetime/README.md)
are independently source-peered. Their actual compiler, native SDK, Service and
Action acceptance remain pending. Source43's immutable copies and every older
native product keep their original bytes and labels; they do not test the new
headers. Both native Gateway main translation units need fresh builds.

The maintained Unitree guard also avoids rescanning its entire output tree for
every input-catalog row. Cgroup and every backing-space check still occur on
every call; only the advisory output walk has a half-second interval, reset
before terminal validation. Five new bounded policy tests are source-authored
and independently reviewed, **not executed**. Current frozen40/43 jobs still use
their original unchanged guard; no in-flight code was patched.

A maintained [four-profile interpreter producer](../../tests/ros_unitree/INTERPRETER_WORKERS.md)
now replaces reliance on a private builder for future fresh registry builds.
Its recovered original source/provenance remains separate. It is independently
source-peered and unexecuted, with explicit current cgroup pre-import checks,
full-MD/link/ELF evidence,12 actual TU roles, bounded compiler ownership and
fresh terminal storage checks. It does not qualify LLVM/JIT or behavior.

ROS1 legacy review also found that recipes and peer sources were previously
present without a maintained end-to-end native/Guest/transport producer chain.
That chain is being implemented explicitly. The unsupported documentation
claim of an existing six-TU Noetic package is not actual build evidence; new
reports must count their genuine compiled role inventory. Native message
generation must not invent C++ translation units. No legacy12 or new native
package success is claimed by the newly completed SDK acquisition.

## Serial actor lifecycle correction and fresh native producer

Independent source review found that `Actors` chose its exclusive-create
`retired-N.jsonl` name from the number of active cleanup owners. Transport
cases close and remove an owner, so the next case could reuse an existing
filename and fail before starting its actors. Maintained `Frame` now owns a
monotonic retirement-ledger ordinal for its entire lifetime. Three authored
policy tests cover32 sequential removed owners, out-of-order removals, and an
exclusive-create collision preserving existing owner bytes. The
[independent source peer](../../tests/ros_unitree/acceptance/actor-retirement-independent-source-peer.json)
is complete; these tests have not executed. Source43 remains unchanged, and
its known-affected Typed28 execution is held pending a new complete freeze and
updated owner. The previously staged Source43 owner is not an acceptance result.

The maintained
[fresh Noetic native producer](../../tests/ros_unitree/NOETIC_NATIVE.md)
and helper are now independently source-peered. Its real build contract is
three primary native translation units: the eleven-type identity exporter,
custom-registry ROS1 Gateway, and independent legacy transport witness.
Official message generation contributes no invented C++ translation units.
The SDK-free finite tool observation reopened the actual completed SDK's
compiler, loader and six other selected tools without executing them;45,358
literal entries had no set-id/sticky modes. It is recorded separately as
[content observation](continuation-20261016-boot3-evidence/noetic-native-tool-observation/actual-root-finite-sdk-tools.json),
not native ROS or compiler acceptance. Fresh compile/link/full-MD, stopped
metadata mapping, complete original/copied SDK preservation and all later
positive/default-denied TCPROS cases remain required.

## Current implementation batch closed before the next source freeze

The maintained six-language legacy Guest compiler, fresh Noetic native3
producer, four-profile interpreter producer, legacy12 TCPROS runner, and
current-source Typed28 namespace owner are now source-authored and independently
reviewed. Their respective readiness records bind actual source bytes; none
contains a forecast complete-source manifest or an executed acceptance result.
The old Source43 owner remains staged historical material because its serial
actor ledger and custom Topic policy were unsuitable for the pending run.

The current Typed and Direct positive policies additionally carry each actual
native-bound `adapter-codec-identity`, as required by both production policy
validators. Generated record/root codec maps and the compiled Worker registry
must agree. Direct LowCmd/LowState records must also agree with that map before
their existing byte capacities are used. Seven authored policy tests cover
correspondence and refusal. Empty default-denied grants remain empty.

The unified current regression entry now selects45 actual Python policy tests
(5 output monitoring,3 actor-ledger,14 legacy protocol,16 legacy build,7 Typed
and Direct codec-policy) plus seven native SDK-free Service/ticket groups.
Its source peer requires nine actual serial commands, full-MD/fast_io/link/ELF
evidence and complete source/input/product preservation before any PASS.
At this checkpoint those commands have **not executed**. SDK-dependent Action
fixtures, affected fresh ROS1/ROS2 Gateway builds, genuine transport/physics,
LLVM/debugger and the full QEMU/distribution matrix retain their separate
outstanding acceptance requirements.

## Source44 actual policy and native Service regression result

The subsequent actual Source44 freeze contains 18,347 files and 225,069,980
bytes. Its complete manifest SHA-256 is
`485e2e3ea976c1dd6e3a4af36b6997a78b3cf306dc6ab1c6f63b8089ed0c43cc`.
It includes the explicitly recorded two-empty-directory adaptation for the
official Noetic message package's otherwise missing install source directory.
The original upstream source files and original Source43 remain unchanged.

The [actual inner receipt](continuation-20261016-boot3-evidence/source44-core45-seven-native-pass/inner.json)
now reports `PASS_FORTY_FIVE_POLICY_TESTS_AND_SEVEN_SDK_FREE_TICKET_GROUPS`.
All nine serial commands returned zero: compiler identification, five Python
suites containing 45 tests, fresh C++20 compilation and linking, and execution
of the seven-group native contract. The contract includes 1,048,608 completed
Service requests, 1,048,608 queue-refused attempts, the complete 255-value
nonzero uint8 identifier domain, 131,072 concurrent allocator identifiers and
8,192 concurrent Service identifiers. Its real dependency file includes
`fast_io.h`; this executes the C++20 formatting path.

The independently retained [Root owner receipt](continuation-20261016-boot3-evidence/source44-core45-seven-native-pass/owner.json)
also passes. Complete source and all 21 selected input-root inventories were
preserved, selected input and product pins remained unchanged, cleanup left
no owned process, and finalization reported no errors. Execution stayed in
the original boot3 64 GiB, zero-swap cgroup. The original reports, configuration,
factory receipts and all nine command logs are retained beside those receipts.

This result closes the preceding checkpoint's pending Core regression only.
It executes no ROS context, Gateway, Wasm Guest, simulator or SDK-dependent
Action fixture. Those real integration checks, fresh Noetic native3 and legacy
six-language products, Typed28 and legacy12 transport, physics comparison,
LLVM/debugger, and the architecture/distribution matrix remain outstanding.

## Source44 Noetic native preparation: first actual attempt failed

The subsequent [native3 receipt](continuation-20261016-boot3-evidence/source44-noetic-native3-r1-failed/inner.json)
and [Root receipt](continuation-20261016-boot3-evidence/source44-noetic-native3-r1-failed/owner.json)
both retain their actual `FAIL` status. All 12 recorded commands returned zero,
including the official eleven-message generation/install, identity-exporter
compile/link and three unmount commands. All eleven fresh devel headers matched
their installed counterparts. However, the actual GNU9/CMake3.16 verbose
compiler command contained `-MD` and `-MF` without an explicit `-MT`; the strict
build-evidence validator therefore rejected it before metadata export. No
Gateway, witness, ROS context or Guest ran in this attempt.

Owned process cleanup passed and no owned mount remained. The original failed
reports, stdout logs and actual compile database are retained without promotion.
The following source correction supplies an explicit CMake compile-object rule
with object-specific `-MD -MF <OBJECT>.full.d -MT <OBJECT>`. The original official
package and identity-exporter CMake files remain unchanged, and the validator's
full dependency-target requirements remain strict. A fresh source/configuration
and fresh output are required for retry; this correction is not a passing
native3 result.

## Source45/46 retries and the streaming ELF correction

The [Source45 failed native receipt](continuation-20261016-boot3-evidence/source45-noetic-native3-r1-failed/inner.json)
records successful metadata export and Gateway compilation, followed by the
actual GNU9 witness compilation failure: unqualified `pause()` was ambiguous
with POSIX `::pause()`. Renaming the private polling helper to `poll_pause()`
preserves its protocol and two-millisecond delay. The failed batch stays FAIL.

The [Source46 failed native receipt](continuation-20261016-boot3-evidence/source46-noetic-native3-r1-failed/inner.json)
records all27commands returning zero, including the three native target
compiles/links and the stopped genuine metadata exporter. Final ELF cataloging
then failed because the old decoder buffered whole files with a64MiB limit.
The actual copied SDK contains a123,305,304-byte `libLLVM-18.so.1`. This is an
SDK catalog member, not a qualified UWVM LLVM/JIT provider. Its raw failure,
all27stdout/stderr logs, three compile databases and dependency files remain
preserved. This batch also stays FAIL.

Source47 replaces that historical reader only in maintained Noetic preparation
with bounded positional metadata reads and same-descriptor streamed full SHA256.
Its complete source manifest is
`1912ecd52349b7b0f948ba7d01329885cf02ef54dc5023a27e45e521703a91aa`
(18,371files;225,417,231bytes). The construction operator accidentally retained
`SOURCE46` in its status label; the unchanged raw receipt is preserved. Its
actual source path, complete manifest/full7 and file count identify Source47;
no old source was renamed or relabeled.

The [actual Source47 physical-reader report](continuation-20261016-boot3-evidence/source47-elf25-and-owned-trim/elf25-report.json)
passes25physical cases without skips and independently inspects the actual
123,305,304-byte SDK ELF with its previously observed complete SHA256. Tests
include a65MiB sparse valid file, malformed loader metadata, same-byte named
inode replacement, symlink/FIFO refusal, a2GiB+1 sparse rejection and descriptor
closure on guard cancellation. Peak coordinator RSS was61,186,048bytes. This
is file-reader regression evidence only: it executes no ROS, Gateway, Runtime,
Guest or official upstream test. A new Source47 native3 run was then launched
in fresh directories; its terminal result is still required.

The three retained `trim*-report.json` receipts in the same evidence directory
record successful bounded `fstrim` of the exact owned loop29 Btrfs image. The
image inode/size/mount and complete selected sources remained unchanged. No
file was removed. Reclaiming unused image blocks does not authorize pruning
failed receipts, SDK inputs or old products.

## Expanded official upstream acceptance requirement

The user's expanded requirement includes all named ROS1/ROS2 distributions and
Unitree projects at the level of their complete official test suites. Existing
policy, SDK model, topic smoke, compilation and interpreter architecture results
are narrower evidence and cannot close that requirement. The current catalog
contains26named targets (13ROS1including five historical pre-Groovy targets;
13ROS2including Rolling). Merely listing those names proves no tested SDK cell.

Each actual cell must bind its official source version, complete upstream test
and workflow inventory, discovered/executed test identities and raw results,
including failures, errors, disabled/skipped/unexecuted cases and real hardware
requirements. Native upstream results and Wasm/Gateway differential results
must remain separate. Full official-suite execution is currently outstanding;
the maintained distro lane's basic native topic checks do not substitute for it.

## Additional reviewed source integration, not execution

Three independently reviewed packages are now integrated in the maintained
repository: generated Rust/JavaScript/AssemblyScript/TinyGo custom Service and
Action owners with complete examples; explicit foreign-target debugger build
and runtime admission for eight architecture identities; and a parent-paced
Direct3 hardware profiling route using real VTune hotspots and microarchitecture
collection. The36changed/new files are source integration only. Actual language
compilation, positive/default-denied ROS interactions, per-architecture patched
LLVM providers/debugger execution and supported hardware collection remain
required. Existing immutable Source47 and its active native run are unchanged.

## Source48 actual SDK models and native preparation failures

The complete Source48 manifest is
`8eed4dfa13f125c4cd94f8832c8c9eb32634f25623e013873b903cc4d1762fea`
(18,557 files; 239,806,453 bytes). The original 64 GiB, zero-swap cgroup,
boot identity and CPU scope remain selected. No OOM event was observed.

The [actual Python source-emission report](continuation-20261016-boot3-evidence/source48-sdk24-and-native-admission/sdk24-report.json)
passes 24 generator/template models without failures, errors or skips, with
70,311,936 bytes peak coordinator RSS. The [actual JavaScript report](continuation-20261016-boot3-evidence/source48-sdk-js15/actual-r2-pass/report.json)
passes all 15 generated Service/Action facade and public-SDK models. Its actual
Node command returned zero; peak owned RSS was 55,451,648 bytes. These are
39 local contract models, not official ROS suite cases, native ROS transport,
project Runtime execution, compiled language examples or simulator evidence.
Complete source/copy preservation and owned cleanup passed. The first JS owner
incorrectly expected the native-generation status for guest-only output;
its zero-command failed report is preserved separately from the corrected run.

The [first native admission report](continuation-20261016-boot3-evidence/source48-sdk24-and-native-admission/native48-admission-only-r1-failed.json)
refused before starting a child because cached memory left insufficient space
for the unchanged Root reservation. Its secondary preservation error occurred
before a source baseline existed and does not demonstrate a source mutation.
Bounded file-cache reclaim targeted only this cgroup, with `swappiness=0`;
memory limits, files and running jobs were preserved. Separate unused-block
discard targeted only the exact owned loop29 filesystem and removed no file.

The [fresh native r2 report](continuation-20261016-boot3-evidence/source48-noetic-native3-r2-failed/inner.json)
also remains FAIL. Private/read-only SDK mounts succeeded. During the first
official CMake configuration, an owned short-lived process exited between a
live pidfd observation and reading `/proc/148986/cgroup`; the resulting
`FileNotFoundError` stopped the attempt. Both cleanup receipts pass, no owned
mount remains, and complete input/source/root preservation passes. No target
compilation or completed native family is claimed. A maintained monitor fix
and fresh source/output are required; no historical failed result is promoted.

## Additional source integration after the Source48 freeze

The maintained source now includes an explicit three-provider pure-Wasm graph
for C, C++ and embedded JavaScript control commands: actual joint-state words
enter perception, then motion invokes a separate callback instance. Providers
retain private memories and no host authority. A distinct default-zero flag
preserves the original two-provider C++ branch literally. The authored protocol
fixture, 30 graph/contract policies, compilation and actual physics are still
pending. Joint-state processing does not qualify RGB camera vision.

The C++17 ROS2 Action client compatibility route preserves all five caller QoS
channels on Foxy through its public typed constructor/waitables interfaces and
prefers an actual options-taking factory where present. Independent source
review closed; SDK compilation and real communication remain pending. A
separate custom-Action/tutorial-availability probe split is not yet qualified.

The independent official libcxxabi library consumer now accepts its genuine
completed standalone producer report, rechecks all 58 original prerequisite
roots and the full dependency closure, and writes a distinct current sidecar.
It does not require or promote the still-running Typed14 main report. The
actual current sidecar qualification and six legacy Guest builds remain
pending, including their matching-current-source Noetic prerequisite.

The [complete upstream suite owner](../../tools/ros_distro/official_suites/README.md)
is integrated after independent source review. Native CTest/pytest/gtest/JUnit
universes, declared counter mismatches, skips/disabled/missing results and
Wasm case mappings remain distinct. Its executable per-case port owner requires
real source-bound adapters, held Worker/native-peer/Gateway identities, actual
captured effects and default-denied quiet witnesses. No API-specific port or
passing upstream result is fabricated. The 27 authored accounting/authority
policies and actual upstream workflows are unexecuted at this checkpoint.
The requirement for all 26 distribution targets and the inventoried Unitree
projects is therefore still outstanding.

## Source49 actual preparation and bounded regression results

The complete immutable Source49 manifest is
`b159ced2c416aec0184e67fdbd4cfa434e074238e7b9376a0bbe1514a1b9bd28`
(18,618 files; 240,422,440 bytes). The original 64 GiB, zero-swap cgroup and
boot/CPU identities remain unchanged; no OOM event was observed.

The [native Noetic producer and owner](continuation-20261016-boot3-evidence/source49-noetic-native3-pass/README.md)
now genuinely pass. All 33 actual serial commands returned zero, including
fresh exporter, ROS1 Gateway and native witness preparation with complete
dependencies. The held, stopped exporter provided all eleven official native
message identities. Complete source/input/SDK preservation and owned cleanup
pass. This supersedes the preparation blockers of the earlier failed attempts;
their original failed reports remain preserved. Six-language Wasm builds and
actual positive/default-denied TCPROS interactions are still pending.

The [actual race and policy reports](continuation-20261016-boot3-evidence/source49-race-and-policy/README.md)
pass 25 deterministic process-exit models, 512 real short-lived Linux children,
and 110 source policy models without failures, errors or skips. The latter
cover official-suite accounting (27), independent C++ input qualification (11),
the modular graph (26), the actual production direct contract (4), debugger
ELF/target parsing (17), Action feature admission (9) and Direct3 profiling
policy (16). These are policy/model results, not official upstream cases,
actual debugger targets, hardware profiling or simulated robot behavior.

The [two real C++17 Action contract executables](continuation-20261016-boot3-evidence/source49-action-cxx17-pass/README.md)
also pass, covering their sixteen source-defined groups through seven actual
serial compile/link/run commands. Peak aggregate owned-tree RSS was
202,506,240 bytes. Full dependencies, original silent execution logs, complete
22-root preservation and normal Frame/outer-owner cleanup are retained.
Real ROS Action SDK and transport execution remain separate pending work.

An actual stream of all 58 historical standalone C++ prerequisite catalogs
found 358 separately pinned regular symlink targets outside the selected
whole-root list. The common catalog format intentionally records those
observed regular targets; the later C++ qualifier currently treats them as
selected compiler inputs and refuses them. No extra broad system root or
larger root limit has been admitted. A narrow, separately reviewed observation
role is required before the six-language build can proceed; actual consumed
compiler/full-dependency inputs must retain their existing root restrictions.

After the user's disk cleanup, a fresh guarded observation reports
22,577,389,568 bytes (21.03 GiB) free on the physical root filesystem. Retaining
the existing eight-GiB reserve leaves approximately 13.03 GiB for active work.
The sparse artifact filesystem's larger virtual free-space figure does not
increase this physical backing capacity. Current cgroup usage at that
observation was 54,230,142,976 bytes, approximately 50.51 GiB.

## Reviewed successor source, not new execution results

The narrow catalog correction is now integrated. It independently reopens
only the external regular targets already witnessed by the same complete
original/current catalog pair, retains exact source aliases and device-only
observations, and preserves those observations in a separate ledger. Actual
compiler, full-MD, object and official-source inputs keep their original
selected-root restrictions. Fourteen additional filesystem policy cases are
authored; their execution under the new source is still pending.

The independently reviewed A1/A2-to-Direct v2 route is also integrated in
sixteen files. A1 prepares a genuinely separate official C++ object/proof leaf;
A2 prepares the actual original C52 GO24 catalog, definitions and generated
source. Normal successful Frame finalization is mandatory before either leaf
can be consumed. Direct B uses those completed leaves and the actual consumed
compiler/SDK closure, rather than importing every historical preparation root.
Its later physics/transport stage still independently requires actual native
Gateway/provider and Worker qualifications. Original C52's 195 package and one
exporter translation units remain distinct from its subsequently generated
conversion source, which C52 did not compile into a Gateway.

A fresh guarded scan of the two original C52 SDK catalogs found zero external
regular targets outside their explicit selected closure: 32,354 execution-SDK
rows with 1,101 regular target observations, and 32,320 acquisition-SDK rows
with 1,100. No artificial SDK-to-host filesystem bridge or broad input root was
added. This is actual metadata evidence, not A2's future complete qualification.

The additional ten A1 and eighteen A2 policy cases, actual A1/A2 preparation,
new generation, six-main Guest compilation, TCPROS/DDS, simulation and hardware
profiling remain pending for this successor. The complete new source must be
frozen before testing; matching-source native preparation is rerun with that
actual new manifest rather than relabelling the successful Source49 result.

## Source50 actual native preparation

The complete immutable Source50 manifest is
`f6a677f9c538faf2d2d24f5367abec6949f3f4b2e14875779f5ad862d72b1acf`
(18,660 files; 241,014,938 bytes). Its
[actual native producer, owner and raw command evidence](continuation-20261016-boot3-evidence/source50-noetic-native3-pass/README.md)
now pass. All33 actual serial commands returned zero, three fresh primary
translation units were qualified, and the held native exporter supplied all11
official Unitree legacy message identities. Complete preservation and owned
cleanup passed under the original64GiB/zero-swap cgroup without an OOM event.
The74 retained evidence members are exact original report/log/dependency bytes;
no earlier result was relabelled as Source50.

The83 selected catalog/library/Direct policy models, six fresh legacy Wasm
commands, four fresh Workers and subsequent real TCPROS/DDS/physics execution
remain unexecuted for this source. This native preparation is not an official
ROS or Unitree suite result. The all-distribution, multiarchitecture and full
official-case requirements remain outstanding.

## Current implementation work after Source50

The Unix client now requires request ID zero in the hello response and an empty
body in the policy-binding acknowledgement. A malformed response disconnects
the client before acceptance. A new real-socket fixture contains both refusal
cases and the valid acknowledgement. Its maintained owner compiles and runs
the same three cases separately as C++17 and C++26, with full dependencies,
actual link inputs, native ELF checks and retained process ownership. This
working-tree implementation and its authored fixtures have not yet been tested.

SDK event/executor integration, structured ROS debugger state and additional
official Python-suite support are being completed in the working tree. They
require a new complete immutable source before the next coordinated tests;
the successful Source50 native preparation cannot qualify changed inputs.


## Unified SDK, debugger, official-case and IPC implementation batch

The next complete freeze includes the reviewed six-language event resources
and executor integration, ROS DAP metadata inspection and explicit LLVM target
selection, strict Unix client handshake acknowledgement, and real unittest
collection/lifecycle accounting with one Git-locked rospy subscriber-first API
port. The source packages retain their independent reviews and exact changed
bytes. These changes preserve explicit grants and default-denied ROS authority.

At this checkpoint the new batch has executed no tests or compiler. The planned
unified Python run has164 parser/policy/data cases; the actual four unittest
observer fixtures, real socket handshakes, SDK native/pure/Wasm builds and ROS
transport runs are separate observations. None establishes completion of all26
ROS distro targets,9 Unitree upstream projects,8 architectures, simulation,
native performance comparison or conditional hardware profiling. Original
native official suite discovery and execution remain required even when an API
adaptation exists. No existing historical success is promoted to these bytes.


The actual Source51 complete freeze has18901 files and243087151 bytes, with
manifest SHA256 `ddaaa833bb305083c743f9cea56495fe7234fb8b163ccf5171a029c0acd47643`.
Its real C++17/C++26 Unix client handshake run passed six socket cases across
seven serial compiler/link/run commands. The complete raw reports, both full
MD files and command logs are retained in
[`source51-ipc-handshakes-pass`](continuation-20261016-boot3-evidence/source51-ipc-handshakes-pass/preservation.json).
The actual unittest observer also passed its separate contract: discovery exit0,
execution expected exit1, and exact PASS/SKIP/XFAIL/XPASS fixture observations.
These are four observer fixtures, not four passing official ROS testcases.
[Its raw evidence](continuation-20261016-boot3-evidence/source51-unittest-observer-four-pass/preservation.json)
retains the actual inventories and lifecycle records. Both owners completed
normal source/input/whole-root preservation and owned-process cleanup.

Source51's planned164-model run stopped after24 actual tests:14 catalog tests
passed, and two of the next10 library fixture tests errored because they had
not selected the generated copy directory as consumer input. The
[unaltered partial failure](continuation-20261016-boot3-evidence/source51-models-partial-failed/preservation.json)
is retained. The reviewed correction changes only that fixture, gives consumers
an independent source/output and explicitly inventoried copied-root input,
and verifies unadmitted producer output remains refused. Production input
scope checks are byte-identical. The corrected unified165-case run remains
pending the next complete freeze and actual Linux execution.

## Source52 actual policy models and SDK source-check failure

The immutable Source52 manifest is
`561dd7957463f667d74713cbd194583e4f9a98d5dbbad923637ec5539738a942`
(18,905 files). Its actual unified165-case run passed all selected library,
debugger, foreign-target selection and official-accounting DATA tests in
18.402 seconds, with peak coordinator RSS194,560,000 bytes. Normal complete
source/input/22-root preservation and cleanup passed; no Runtime, middleware,
Guest, simulator or compiler ran in that lane. See the
[original raw evidence](continuation-20261016-boot3-evidence/source52-model165-pass/preservation.json).
These165 cases are not official upstream ROS cases.

The separate actual unified SDK attempt failed during its first maintained
source-validation command. No C/C++/Rust/Go/TinyGo/AssemblyScript compiler ran.
The Host whole-file ABI hash was stale, and the Rust/TinyGo source checker
recognized only the older Go build-tag spelling. All eight pure JavaScript
checks, including the new event-dispatch contract, passed in that attempt.
The [unaltered failed reports](continuation-20261016-boot3-evidence/source52-sdk-source-validation-failed/preservation.json)
retain the original failure, successful source/input/24-root preservation and
owned cleanup. The detailed SDK diagnostic was read separately after producer
termination and is explicitly distinguished from producer-pinned outputs.

## Trusted update, ROS authority and debug-reference integration

The current implementation adds no Guest update, debugger or policy-management
import. A genuine successful trusted function replacement marks the affected
compiled module's current incarnation permanently ROS-retired before any new
entry is published. The primary Runtime identity query refuses that incarnation,
including its first future ROS open if it previously had no Host state.
Observed identity remains diagnostic only. Failed preparation/commit does not
set the retirement flag.

After leaving the pause-domain and Runtime-publication mutexes, while the Guest
remains paused, the native hook drains admitted Host leases, invalidates old
handles, closes local connections and discards queued/pending/reserved payload
and request identities. Inert diagnostic registrations, revocation and quota
counters remain; other modules retain their independent state. Whole Runtime
reset notifies only after execution drain and before old epoch/storage retirement.
ROS reuse requires a fresh trusted launch with explicit CLI policy. No Guest or
debugger re-enable request is added. See the
[SDK trust-boundary guide](../../sdk/docs/update-and-debug-boundary.md).

The new regression sources include12 injected adapter/actual-registry DATA
cases, real manager and local Host/Unix retirement components, and a narrow
extension of the existing authenticated Runtime integration: Guest fd1 prints
console-looking commands and the actual separate controller must remain running.
These sources still require actual execution. Their components do not substitute
for genuine successful/failed LLVM replacement acceptance.

The SDK source-check repair preserves all38 ROS1 and43 ROS2 import names/forms/
widths and changes only the committed ABI authority-source hash. A bounded Go
constraint parser now checks all32 assignments of the five supported tags;
11 positive/negative model methods join the maintained19-command aggregate.
These implementation and source-review observations are not compiler or test
results. A new complete freeze is required before unified retesting. Full26
distribution suites,9 Unitree upstream projects,8 real architecture routes,
simulation/native comparison and conditional genuine hardware profiling remain
separate outstanding requirements.

## Source53 actual observations and the next repair cohort

Source53 froze18,931 files/243,421,837 bytes under manifest
`a6858eb4c9e98f6dae69c9f737f29ca5873fd012d072f1c2beffbfe6686c1b85`.
Its [actual177 DATA cases](continuation-20261016-boot3-evidence/source53-model177-pass/preservation.json)
all passed in18.912 seconds with peak RSS194,576,384 bytes and normal complete
source/input/22-root preservation. The added12 update/ROS-debugger cases are
included in177; they are not genuine Runtime replacement or official ROS cases.

All19 maintained SDK child checks passed on Source53, including current ABI
metadata/bindings and the new TinyGo constraint models. The
[overall SDK attempt still failed](continuation-20261016-boot3-evidence/source53-sdk-generated-input-contracts-failed/preservation.json):
12 relative-source dependencies lacked complete admitted build contracts.
No SDK compiler ran. Eleven are genuine external codec/interface generation
inputs; one embedded-JavaScript C file has a static source-header include that
needs a correct relative path. The next source repair supplies exact reviewed
producer/staging/recipe bindings and complete interface-example preparation,
without pretending that generated files exist or allowing arbitrary missing
includes. Its source, compiler and native ROS checks remain distinct.

The separate real C++26 component attempt compiled and executed the diagnostic
core35 and console47 checks successfully. The management executable compiled
and linked, then stopped at fixture policy validation: its two aliases repeated
the same operation/resource/type grant, which production correctly rejects.
The overall attempt is a failure; Host-profile compilation had not begun.
The reviewed fixture correction gives the revocation alias its own explicit
resource and leaves production validation unchanged. A fresh run must exercise
the actual retirement assertions.

The next component source also calls the genuine central Runtime reset twice,
observing cleanup at the old epoch through a static forwarding observer and
requiring exact subsequent epoch advancement, empty registries and denied old
handles. Six real unresolved-management-import WAT sources require12 routes
across enabled ROS1/ROS2/dual Workers. These are authored, unexecuted additions;
the existing component owner compiles Host objects only and cannot count them
as linked reset or Guest executions. Full successful/failed LLVM replacement
integration remains separately required.

The earlier Source40 fourteen-product build has now terminated with failure
after its command deadline expired during preservation. All14 compiler
commands completed, but the producer never achieved successful finalization.
The [unaltered terminal reports](continuation-20261016-boot3-evidence/source40-typed14-compiler-completed-terminal-failed/preservation.json)
retain both FAIL statuses and successful owned cleanup; the compiler observations
are not promoted to current Runtime or ROS acceptance.

## Source54 actual compiler and component observations

Source54 manifest is
`5cf45a2246cb382d561f98f5a5de7993bd1995a1b83ae236cf3d48e0855a36a2`
(18,956 files/243,680,212 bytes). Its177 parser/policy/DATA models all passed
with normal complete-source/input/22-root finalization; see the
[unaltered evidence](continuation-20261016-boot3-evidence/source54-model177-pass/preservation.json).

The maintained SDK source aggregate now passed: all19 child checks and zero
unresolved source dependencies. The subsequent real AssemblyScript matrix
completed32 rows, with29 compiler/import/full-Wasm validation passes and3
TS2322 compiler failures in the same EventSubscription constructor's void early
return. The [SDK attempt remains FAIL](continuation-20261016-boot3-evidence/source54-sdk-as-compile-failed/preservation.json).
A narrow source-reviewed if/else correction preserves invalid/no-watch and
valid/single-watch behavior and still requires a fresh actual compile.

The real C++26 core35, console47 and management35 assertion components compiled,
linked and executed successfully. The next ROS1 Host compile failed because
the external compile-only recipe omitted the standard Interpreter/JIT-selection
macros. The [overall attempt remains FAIL](continuation-20261016-boot3-evidence/source54-components-host-flags-failed/preservation.json);
no Host object, linked Runtime reset, active Guest drain or genuine JIT function
replacement is credited. The external recipe successor selects only the normal
existing Interpreter backend definitions and still requires actual retesting.

## Real Host reset and unified SDK results

The Source54 real Host/Runtime lane passed all17 serial commands, with9 freshly
compiled full-MD translation units and three genuine strict links/executions.
ROS1/ROS2/dual passed91/91/92 component assertions, respectively. Each actual
executable called the central Runtime reset twice and verified old-epoch Host
cleanup, exact epoch advancement, empty registries and refusal of retired
identities/handles; local connection teardown produced genuine Unix EOF.
Both owner layers completed source/input/22-root/product preservation and owned
cleanup. The [original evidence](continuation-20261016-boot3-evidence/source54-real-host-runtime-reset-pass/preservation.json)
is retained. These are empty-execution resets; active Guest drain, successful/
failed genuine JIT replacement and native ROS transport remain separate.

Source57 manifest
`704b948f1af606ec1514c1706c23f90b51a5cdc25e5617fef28de605a32732cf`
passed the full maintained SDK validation owner:19 source checks,32 actual
AssemblyScript compiler/import/full-Wasm validation rows,32 native/pure model
rows, and the real Rust wasm32-unknown-unknown event fixture build/link/full
validation plus finite mocked-Wasm execution. The Rust/Go native rows executed
22/24 tests. All seven phases,24 complete input roots, current source and
selected input/product checks, and normal owned cleanup passed. See the
[original SDK evidence](continuation-20261016-boot3-evidence/source57-sdk-unified-pass/preservation.json).
This is SDK/compiler/model coverage; no Worker/Gateway/real ROS/simulator or
upstream official suite is counted. Original Source55/56 failures remain
separately preserved; none was relabelled.

The new four-profile Interpreter build now has an actual current-source54
selection. The genuine retained ROS2 compiler MD consumes the registry once;
its separately pinned generated record codecs are metadata, not falsely
claimed compiler dependencies. The external owner explicitly admits6GiB for
the unchanged5GiB serial compiler, within the original64GiB/swap0 cgroup. No
new Worker execution or12 management-import refusal result is claimed until
the actual build and corresponding Guest routes complete.


## Source54 actual Guest management refusal and Source58 layout policy increment

The Source54 four-configuration Interpreter producer completed18realcommands:12freshfullMD translation units,four genuine native links and two version invocations. Its original producer status remains `PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED`; normal23root Frame and independent owner both completed `PASS_FOUR_FRESH_INTERPRETER_WORKERS_12TUS_4LINKS_UNCHANGED_PRODUCER_OUTER_6GIB_NOT_EXECUTED`. The unchanged compiler command cap was5GiB/600seconds/jobs1; outer aggregate6GiB. Source54 manifest `5cf45a2246cb382d561f98f5a5de7993bd1995a1b83ae236cf3d48e0855a36a2`. Byte-preserved reports and fullMD/logs: [source54-worker4-build-pass](continuation-20261016-boot3-evidence/source54-worker4-build-pass/README.md).

Using those actual matching workers,the separate management-negative lane completed `PASS_12_REAL_MANAGEMENT_IMPORT_REFUSALS_AND_8_ENABLED_REGISTRY_CONTROLS`. All30realcommands completed:ten WAT compilations,twelve exact resolver refusals and eight enabled-registry controls. ROS1-only and ROS2-only each exercised three absent management imports; dual exercised allsix. Every refusal specifically identified the selected missing `runtime_update`,`debug_configure` or `policy_set` function in its enabled ROS namespace, returned actual127,and emitted no forbidden Guest startup marker. Eight controls independently proved ABI version1 and default-denied capability_open result13 with unchanged sentinel memory. No ROS grant,Gateway,network orrobot was used. Normal25root Frame/fullsource/input preservation and pidfd-owned cleanup passed;64GiB/swap0 and8GiB physical backing reserve stayed unchanged. Original owner37,854bytes SHA`adb0f2e2d1c4383e2c2f81ccb962bd20b016140146a92ed936cd271787bd5e80`; original Frame7,127,315bytes SHA`f580008c2d22f56f2766c49a9ae9243fc707f5c4965b9c717400473e34929fc2`. Actual duration67.829seconds. [Original reports,commands and diagnostics](continuation-20261016-boot3-evidence/source54-real-management-import-refusals-pass/README.md); ten original compiled Wasm files and four generated control WAT sources are additionally preserved with original full7 identities under its `compiled-products` directory. No command was rerun to preserve evidence.

Source58 manifest `51464116cb66e5507b749918c4e49047b5decbc5b45dbc540088ac9b08f41e2a` freezes18,977files/244,136,329bytes. The genuine observed WASI SDK33 places C++ archives in its explicit nonthread WASIp1 noeh directory; the reviewed producer now admits an explicit physical sameSDK flat/noeh/eh pair, requires the actual compile/link target and both selected archive pins in the actual link trace,and preserves original recipes,official abort body and32command contract. Its six new finite filesystem selection/refusal policy cases passed alongside177existing models: `PASS_183_ACTUAL_LIBRARY_DEBUGGER_OFFICIAL_POLICY_DATA_MODELS_ONLY`,20.146seconds/194,560,000bytes peakRSS,zeroactors. This is policy-only evidence; fixture bytes are not actual archives. [Original183case evidence](continuation-20261016-boot3-evidence/source58-model183-pass/README.md).

The earlier three-profile actual Host/Runtime empty-execution central resets and SDK57 unified results remain separately valid in their original receipts. The successful/failed LLVM-full replacement,activeGuest execution drain,authenticated-controller Guest-stdout integration and foreign/QEMU JIT tests still have **no new actual execution credit** from the management-negative or183case lanes. A fresh patched LLVM13 provider is still required. Passive actual input observations alone do not qualify that provider:ten selected files/eight native tools and17root metadata were reopened with literal aliases and ELF64LEmachine62 checks; original oldboot config remains historical. Actual physical backing free was18,085,072,896bytes before later builds; the256GiB loop image logical capacity is not physical available storage. Complete original task acceptance across all26ROS distributions,nine Unitree upstreams,eight target architectures,official suites,simulation and performance remains unfinished.


## Source58 actual mixed compiler outcomes and the next source repair

The fresh Source58 Noetic native prerequisite passed all 33 real commands,
including three independently compiled executables and 11 official message
identity checks. Its complete source, SDK, physical-storage and owned cleanup
checks passed. [Original evidence](continuation-20261016-boot3-evidence/source58-noetic-native3-pass/README.md)
qualifies these prerequisites only; it does not establish TCPROS delivery, a
ROS2 distribution, or an upstream official test suite.

Two subsequent compiler lanes terminated with failure. The six-language legacy
lane completed and fully validated its C, C++, Rust, TinyGo and AssemblyScript
command products, including actual use of the explicitly selected SDK33 noeh
C++ archive pair. JavaScript preparation then exposed an evidence-schema
mismatch before its C compilation: the SDK producer returns eight fields,
including the opened file's raw mode, while the consumer compared that record
with a seven-field Frame pin. Its overall status remains FAIL. The original
17-command attempt and its original reports are retained in
[legacy6 failure evidence](continuation-20261016-boot3-evidence/source58-legacy6-sdk8-contract-failed/README.md).

The custom RPC compiler lane separately completed Rust relocatable-library
validation and the AssemblyScript and TinyGo linked command products. It then
failed at the same JavaScript evidence mismatch before the JavaScript C jobs.
Its nine commands do not qualify a complete four-language lane or actual
Runtime RPC behavior. The unchanged failed reports are retained in
[RPC4 schema failure evidence](continuation-20261016-boot3-evidence/source58-rpc4-sdk8-contract-failed/README.md).
An earlier attempt rejected legitimate TinyGo merged-GOROOT aliases after
compilation had started; its separate
[output-guard failure evidence](continuation-20261016-boot3-evidence/source58-rpc4-output-guard-failed/README.md)
also remains a failure, rather than being merged into either later result.

The source repair admits a closed eight-field SDK record, verifies its full
hash and raw file mode on the same opened descriptor, then separately binds
its normalized seven-field identity to the Frame. It preserves and rechecks
both records. The maintained legacy and Direct consumers and the external RPC
factory require this repair; dropping the mode or ignoring extra fields is not
an acceptable fix. New filesystem-policy cases require guarded execution on
the next complete source freeze. Direct's related C++ input routes are also
being checked against SDK33's explicit archive profile. Source review is not
an actual successful rerun.

The genuine patched LLVM 23.1.1-uwvm-ros.13 provider build has started in the
original 64 GiB, swap-zero cgroup. It selects seven LLVM backends covering the
eight requested architecture variants. Actual CMake configuration completed
and the serial Ninja compiler is active; no terminal provider PASS has yet
been observed. Its independent owner retains finite deadlines, original
process budgets and an 8 GiB physical backing reserve. This host-native
provider build does not execute a foreign Runtime, QEMU, or the replacement
integration. Those remain separate acceptance obligations.
