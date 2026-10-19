# ROS continuation r6 — measured 48 GiB Linux acceptance

The original task remains incomplete. This record separates completed tests,
failed attempts and pending work. The original requirement-by-requirement
[audit](original-task-audit-20261013.md) remains unchanged; its 35 chapters,
50 acceptance requirements, 55 attacks, 20 examples, 12 phases and 23 output
categories are still the reference checklist.

All new actual compiler, validator, native, Guest, ROS and QEMU executions use
the original Linux container with `memory.max=51539607552` (48 GiB),
`memory.swap.max=0` and CPUs `0-31`. The accepted boot is
`518a7b26-33dd-4044-9456-93e75aac9b63`; the cgroup leaf is device 29/inode
18927. Readable ancestor OOM counters are compared only within that same
incarnation. Historical 64 GiB results retain their original source and boot
scope and do not qualify a new 48 GiB run.

## Completed, independently bound results

| Frozen source | Actual result | Meaning and limit |
|---|---|---|
| r16 | SDK compiler/import/full validation 61/61 | C, C++, Rust, TinyGo, AssemblyScript and JavaScript bridge artifacts. Compiler input closure remains explicitly UNQUALIFIED; this is not ROS delivery. |
| r16 | SDK host models 17/17 | Includes exact Go 9 top-level tests and 57 named subtests, all run and passed once. Native host models are distinct from ARM or Guest ROS behavior. |
| r17 | Four ARM Workers and QEMU version launches | `none`, `ros1`, `ros2`, `dual` products. Short version launches do not provide live mapping or a pre-main barrier. |
| r17 | Complete CLI 952/952 | Includes 32 generated WAT fixtures and 140 WASI permission recipes. Missing enabled Gateway peers must fail preflight with exact exit 126; explicit environment inheritance is still refused. |
| r17 | Default ROS denial Guest 6/6 | Actual mapped ARM Guest: 71 denied ROS calls, nine preserved output sentinels and zero accepted IPC connections on both listeners. |
| r17 | Signed Guest/native SROS2 Enforce exchange PASS | One raw Wasm Guest and an independent native witness exchange exact canonical String messages through the qualified ROS2 Gateway. Twelve native credential/signature checks pass; one forbidden native publisher is refused. Revocation, fault and fresh-session behavior remain a separate case. |
| r20 | Revoked reconnect Guest/native combination PASS, independently bound | Exact old Gateway kill/reap, fresh authenticated session/GID, surviving publication delivered once and revoked publication absent through a two-second quiet period. Twelve signed checks and ten gates pass. Complete semantic review qualifies this bounded case; broader G09 remains open. |
| r18 | ROS2 resources 16/16, independently bound | Static initialization one case, Action byte limits eight cases and Action lifetime seven cases. Raw dependencies and actual compiler MD/MF/MT arguments bind; missing cwd/compiler-image maps prevent a unique successful complete recipe claim. The Gateway has separate full MD qualification. No Guest/A15 claim. |
| r18 | Action QoS native behavior 4/4, independently bound | Four C++17 native contract/probe cases, including controlled incoming-QoS test interposition. Exact counts 128/77/1057/113, ARM mappings, zero OOM and cleanup bind. Broader forty-field adoption and Guest behavior remain open. |
| r19 | ROS1 parent search and feedback budgets 2/2, independently bound | Parent-search 16 checks; feedback/result framing 17 scenarios and 225 checks. Complete frozen source, 817 raw files, compiler dependencies, ARM mappings and cleanup independently bind. Both are native ROS1/IPC cases; no Guest or complete original acceptance claim. |
| r15 | Original native units 6/6 | Actual ARM security, IPC, deadline, sorted rows, C++17 sorted rows and SDK assertions with stopped constructor mappings. |
| r15 | WASIp1 authority native units 17/17 | Default denial and explicit positive grants, including 32/64-bit and bounded hybrid cases. These are native assertions; actual Wasm CLI recipes are recorded separately. |
| r15 | Local ROS2 parameter event behavior 2/2 | 33 trusted contract checks; 27 complete mutations, 135 filtering checks, three retirements and three overflow policies. No native ParameterEvent writer or duplicate Gateway DDS events. |
| r14 | SDK custom codec supplement 18/18, 46/46 steps | Six-language generated codecs/examples. No Runtime/ROS delivery claim. |
| r14 | ARM audit cache and ROS2 environment contracts | 138 C++17 numeric-cache checks with zero hot C++ allocations; 13 checked environment assertions. |

The Runtime and SDK keep default-empty ROS authority. SDK installation, native
library discovery and wrapper conveniences create no Guest permission. WASIp1
starts with directional basic stdio and its required metadata/readiness; other
authority and inherited environment require explicit configuration. The tests
for ROS and WASI defaults remain separate, and no hidden level or role preset
is introduced.

## Preserved failures and concrete repairs

The r14 local event IPC attempt failed before behavior because the custom ROS2
shared library was missing from the native loader path. A shared qualifier now
derives only `/opt/uwvm-custom-types/lib` from the selected immutable SDK,
pins its directory and ARM shared objects, and retains the full SDK boundary.
The r15 event pair passed with this repair. It creates no Guest loader or
filesystem grant.

The first G09 witness compilation assumed a 24-byte RMW GID. The actual pinned
Jazzy `rmw/types.h` defines 16 bytes. The native witness, controller and oracle
now require exactly 16 bytes and 32 lowercase hex digits; no padding,
truncation or flexible-width acceptance is allowed. r16 native preparation
passed with the original compiler depfile, object, link recipe, 1,279 inputs
and 79 native libraries independently bound.

The r15 CLI report remains FAIL with 948 of 950 rows passed. Its two remaining
rows enabled ROS while pointing at a missing Gateway and incorrectly expected
successful startup. The repaired matrix separately validates the configuration
and requires the precise missing-peer preflight rejection. r16 passed all 952
rows without changing Runtime privilege logic.

The r16 G09 execution generated signed credentials and passed all twelve
native public certificate/key-association/permissions/governance checks. It
then failed before the witness initialization gate; the Guest did not run.
An actual ARM bash/Python diagnostic confirms that Jazzy setup adds
`ROS_AUTOMATIC_DISCOVERY_RANGE=SUBNET`, which violates the witness's strict
input absence rule. The r17 repair applies a checked unset only after witness
SDK setup, before its original exec. Ordinary tools, Gateway override testing,
native checks and Guest grants remain unchanged. Its 636 source/model checks
passed, followed by actual r17 combined signed communication PASS. The native
witness received two exact Guest messages, sent six replies, observed zero
mismatches and recorded one denied native operation and one permission event.
The Guest passed thirteen ungranted-operation and nine stale-handle checks.
Source, SDK and products remained unchanged; private credentials were removed,
and no owned processes or mounts remained.

Private keystores were removed during owned cleanup and are never included in
the public evidence package. Failed reports and logs remain unmodified.

## Active work and original gaps

At this checkpoint, r17 singleton native preparation, strict C++17 Gateway,
four Workers, CLI matrix, default-denial Guests and combined signed Enforce
execution have completed. Independent semantic qualification binds all 4,598
frozen source files, 353 raw files, twelve signed credential checks, three
distinct live ARM roles, exact gates and message bytes, default-denial checks,
same-incarnation zero-OOM evidence and cleanup. The
[combined receipt](continuation-20261014-r6-evidence/r17-positive-enforce/uwvm-r17-positive-enforce-combined-independent-proof-r1.json)
retains its compiler-closure and broader G09 limits.

The separate r18 revoked reconnect case compiled its witness, Gateway and four
Workers, then failed in the native witness's fresh-discovery phase. Earlier
partial observations include native baseline counts one/one, revoke errors
9/125, exact owned Gateway SIGKILL/reap, fault error 107 and a different
authenticated replacement session. They do not establish final delivery or
quiet-period denial. All 336 raw files and the original FAIL report are
preserved. Its `actual_guest_behavior=NOT_PERFORMED` and unfinished command rows
remain unchanged, despite logs showing partial Guest execution; future
failure-metadata repair must describe those terminal states accurately.
The complete 4,606-file frozen r18 source has been recovered and independently
rehashed, retaining the exact files used before later fixture edits.

The r18 ROS1 parent-search attempt also remains FAIL: a forged native IPC body
hits the common dispatcher's zero-byte request quota and returns 90 before the
backend's 22 check. Its fixture incorrectly expected 22. The second native probe
was not attempted. The repaired fixture requires exactly 90 and an empty
response, while Worker and backend validation remain unchanged. r19 freezes
this correction plus fixed public predicate diagnostics and accurate failure
terminal metadata. Its strict ROS1 Gateway and corrected native pair have now
passed. Its separate revoked case remains FAIL and precisely identifies
`old-discovery/old_publisher/name_unknown` before old endpoint matching. It
records `NOT_ACCEPTED` and actual return codes/log bindings for all seventeen
commands. This diagnostic does not establish the exact earlier r18 predicate.
Source, SDK, products and cleanup remain qualified; production and SDK bytes
have not changed.

The frozen r20 native-only repair waits within the existing deadline for two
exact documented GraphCache placeholder pairs. Waiting is not endpoint
admission: stable type/hash/QoS/GIDs and graph bounds remain checked, and final
matching still requires the full exact node identity and endpoint counts. It
has passed all four Runtime builds and the actual revoked reconnect
combination in the original 48 GiB cgroup. All seven native and seven Guest
markers, ten gates, stale errors 9, reopen errors 125, dead-peer error 107,
exact new authenticated session/GID and final delivery bind. The native
witness observed one baseline message on each topic and one surviving message
after reconnect, zero unauthorized or fault-period deliveries, and 195 quiet
observations over 2.004 seconds. All 336 raw files and all 4,606 frozen source
files are archived; full independent semantic review is complete. Its 946
source/model checks and independent 47-check review are separate evidence. The
[Jazzy GraphCache implementation](https://raw.githubusercontent.com/ros2/rmw_dds_common/jazzy/rmw_dds_common/src/graph_cache.cpp)
describes those paired placeholders; the actual r19 failure log is retained.

The [r19 ROS1 combined receipt](continuation-20261014-r6-evidence/r19-ros1-search-pair-pass/uwvm-ros1-pair19-combined-independent-proof-r1.json)
qualifies both native cases and retains the original r18 failure.

Further original acceptance still includes broader ROS1 integration, broader
resource and Action/QoS coverage, complete current Runtime regressions and native ROS suites,
broader lifecycle combinations, six-language actual ROS delivery, unified
event coverage, recovery/attack effect evidence, missing metrics and CI/backend
coverage, and complete example/Group/final packaging requirements. Safe
unsupported boundaries must remain explicit rather than being counted as
successful functionality.

Raw reports, compiler/object/link records, translated Wasm products, architecture
samples, failures, immutable source manifests and independent data qualification
are in [r6 evidence](continuation-20261014-r6-evidence/). The
[current measured milestones](continuation-20261014-r6-evidence/current48-completed-milestones-r8.json)
and [original-task crosswalk](continuation-20261014-r6-evidence/uwvm-original-task-r6-crosswalk-readonly-20261014-r8.json)
retain their exact source/stage scopes; they are not an overall completion claim.

The r20 full native product prerequisite build was unfinished when the user
closed Linux SSH. Its last observed state had nine compiled targets; the local
SSH client exited 255 after owned termination. The remote final status and
cleanup remain unconfirmed, so full native qualification is not accepted. Future r21 JavaScript SDK work
adds a freestanding real-Wasm command relay and distinct asynchronous APIs;
it preserves synchronous APIs and default-empty ROS authority. This source
work does not yet establish actual JavaScript or six-language delivery.

The [r20 combined receipt](continuation-20261014-r6-evidence/r20-revoke-reconnect-pass/uwvm-revoke20-pass-combined-independent-proof-r1.json)
retains every bounded success and original failure scope. The current requested
mainline is complete C/C++/Rust/JavaScript/AssemblyScript/TinyGo high-level APIs
and detailed English examples. All new local tests use fresh `/tmp` copies,
builds, caches and logs; commands run serially with one compile job and bounded
memory. Local compiler/model results remain separate from native ROS effects.

## 2026-10-15 SDK client-library follow-up

The six requested client libraries and their English progressive examples now
cover the existing complete 34/39-import ABI. The
[new SDK delivery record](sdk-client-libraries-20261015-r1.md) and
[byte-bound summary](sdk-client-libraries-20261015-r1.json) record 334 actual
Wasm products, including every C/C++ selector and five fixed-record guest
applications. All products passed current import inspection and full validation;
139 retained compiler products were admitted after the interrupted run and 195
fresh compiler commands completed. Earlier failures and missing historic
artifact/tool/environment fingerprints remain explicit.

Installed C/C++ models, Rust17, Go18 plus57 vector subtests, JS54 API cases and
824 protocol checks, AS413 assertions, compiled-relay80 cases and compiled
scheduling12 cases all passed within their local scopes. Commands/builds/caches/
logs remained in `/tmp`, ran serially with one build job, and used a512MiB RSS
limit; maximum sampled command-group RSS was367.406MiB. Runtime/Gateway bodies,
original raw ABI inventories and earlier native evidence were not promoted by
these local SDK results. Exact native rclcpp/rclc/rclpy compatibility, arbitrary
dynamic native adapters and current SDK ROS/QEMU delivery are not claimed.

Linux SSH remains closed at the user's request. The remote native20 prerequisite
completion/cleanup and original broad Runtime acceptance remain pending; the
bounded r20 revoke/reconnect PASS retains its earlier exact scope.
