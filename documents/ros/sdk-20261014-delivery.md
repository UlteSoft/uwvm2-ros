# WasmROS SDK source delivery — 2026-10-14

The new `sdk/` source distribution contains C11, C++20, Rust, TinyGo and
AssemblyScript guest APIs, plus a JavaScript client and concrete C Wasm bridge
source. AssemblyScript is the interpretation of the requested `asmscrip`.
Detailed English comments and progressive examples cover raw imports, canonical
codecs and ownership, then Node/resources and bounded guest-thread executors.
Interfaces cover all existing Runtime exports: topics, services, Fibonacci
Actions, parameters/events, TF, graph queries, lifecycle, clocks and logging.

[SDK entry](../../sdk/README.md) · [verification status](../../sdk/docs/verification.md)
· [build matrix](../../sdk/docs/build-matrix.md) · [delivery metadata](sdk-20261014-delivery.json).

## Executed checks

The exact source/model report is
[`verification-20261014-r3.json`](../../sdk/tests/verification-20261014-r3.json):
174 checked source inputs, 365 language/backend declarations, 88 C dispatch
slots plus their member layouts/signatures, 66 relative Markdown links, 117
relative source references, 822 Rust/TinyGo source checks, four synthetic Python
binary-reader test methods and 594 pure JavaScript checks. Shared independent
wire samples remain 57 positive and 11 negative. The separate C source checker
mutation review executed 170 models (168 expected rejections).

The source-only compiler matrix prepares 61 rows, including 12 complete import
inventories and seven AssemblyScript pure-fixture compile entries. Its
[latest review](../../sdk/tests/c/build-matrix-prepared-review-r5.json) records
source/pure-model scope. No compiler, validator, process signal or cgroup action
is part of that review. Older source/model reports remain preserved.

The frozen source-only bundle is
`/tmp/uwvm-sdk-20261014-source-r1/sdk.tar.gz` with its adjacent manifest and
SHA256 in the delivery metadata. All 178 SDK file bytes were checked before and
after packaging and verified from the archive without extracting/executing it.
The 126 protected earlier repository evidence files retain their original
sizes and SHA256 hashes. Generated redundant historical source staging paths
were consolidated with copy-on-write storage to recover disk space; their
paths and bytes remain present. Current project sources and raw reports were
not removed.

## BLOCKED acceptance

Linux remains offline; the final SSH probe returned 255 and performed no remote
compiler/guest/ROS command. Actual language compilation, emitted Wasm validation,
native model execution, real ROS behavior and the requested aarch64/QEMU
acceptance remain **BLOCKED_LINUX_SSH_UNAVAILABLE**. External toolchain/stdlib
closures are **UNQUALIFIED**. Requalify the existing 64 GiB cgroup and current
source/toolchain inputs before resuming. Earlier S3/S4 artifacts do not qualify
this expanded SDK.

The SDK grants no ROS resource implicitly. JavaScript uses an explicitly
supplied authenticated Wasm embedding. Node/resource APIs follow familiar ROS
patterns over the implemented Runtime adapters. Native ROS source/feature
parity and arbitrary custom-IDL support are not asserted. The original Runtime
task remains incomplete, including the separate WASIp1 stdio-only default
change. See the repository's earlier ROS acceptance and ARM progress records.
