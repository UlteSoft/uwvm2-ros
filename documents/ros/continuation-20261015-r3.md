# Complete acceptance preparation: 2026-10-15

The implementation and the collected source repairs are ready for a new complete
source freeze. Complete acceptance is still pending. No compiler preparation,
source inspection, SDK model or historical result is a current ROS delivery
result.

The SourceB4 preparation batch actually ran all 15 independent selections,
strictly serially. Ten completed successfully. Five failed: the metrics output
used an ambiguous raw character pointer, the observed ROS2 Gateway compiler
exceeded its 2 GiB RSS bound, and three selections encountered the same remaining
`std::cout` in a shared probe header. All failures and their original logs are
retained. The temporary ARM64 binfmt registration was restored, source and SDK
preservation checks passed, and the cgroup reported no OOM events.

The collected repairs use explicit fast_io string views, the shared checked
C++17 native output adapter, and graph-only GCC heap collection controls. They
preserve the compiler's optimization, assertion, deadline and RSS controls.
Preparation reuse now checks the current boot, cgroup and explicit container
identity. The resources preparation also binds its complete source manifest,
SDK inventory, compiler artifacts and original companion architecture report.
The final input template includes the mandatory complete SDK tool selection.

The twenty-example renderer now requires both strict Gateway reports and uses
the shared qualified custom-library selector for its explicit native Gateway
environment. It preserves the exact source, SDK, aliases and selected product
bytes before and after rendering. The material-only time budget is 180 seconds
to cover these additional read-only checks. Its output remains
`PREPARED_NOT_EXECUTED`.

Native witness logs also retain explicit physical-file associations before
private workspace aliases are removed. Nested RPC aliases contribute their own
opened-file evidence before cleanup; the parent composes that captured chain
while its outer alias remains live. Normalization reopens the final physical
file and the original pinned producer report. Original Runner reports and logs
are preserved without rewriting their paths or inventing missing observations.

The next stages must use one new complete source snapshot: repeat all 15
preparations, build both Guest product sets and all 18 native products, qualify
both Gateways, then run the mandatory six-language SDK matrix, four Worker
profiles and complete native/Guest acceptance. The SDK matrix requires all 345
Wasm build plans and their fresh validators. The original regression aggregate
requires all 3,073 configurations and exactly 3,200 native invocations.

ROS authority remains empty by default. Basic stdin, stdout and stderr are the
only implicit WASIp1 facilities; additional operations require explicit command
line grants. Linux testing uses the existing 48 GiB cgroup with zero swap and
one build/test lane. Local build or test artifacts belong under `/tmp`, with
serial execution and low memory use.

The complete regression matrix retains all compiler products and raw evidence.
A separate task-owned Btrfs artifact volume provides transparent compression;
its sparse image is retained rather than deleting test artifacts. Its logical
capacity does not establish available physical capacity. Final execution must
monitor the backing filesystem separately and preserve a 4 GiB outer reserve.
The artifact mount is retained for delivery; individual test namespaces must
still clean up their own processes, sockets and mounts.
