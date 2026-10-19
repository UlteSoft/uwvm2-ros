# Current SDK acceptance: r14, 14 October 2026

The current SDK passed **all 61 compiler/import/full-validation rows and all
17 SDK model rows** in the original Linux container after the user selected
48 GiB and zero swap. This records a completed SDK acceptance milestone for the
frozen r14 source. It does not complete the original ROS Runtime task or establish
Wasm-to-ROS delivery from every SDK language.

The two authoritative receipts are:

| Receipt | Result | Exact SHA-256 |
|---|---|---|
| [r14 compiler matrix](continuation-20261014-r6-evidence/sdk48/raw/20261014-aarch64-r14/sdk-compiler-matrix-r1/report.json) | `COMPILER_MATRIX_PASS`, 61/61 | `7ec0d56b2387493604d22949381b7cd5bfcf32751bb3ad0615cbd5948c9996f0` |
| [r14 model matrix](continuation-20261014-r6-evidence/sdk48/raw/20261014-aarch64-r14/sdk-model-matrix-r1/report.json) | `SDK_MODEL_MATRIX_PASS`, 17/17 | `7eecac2c6e02dcd0b3cdc0a1c58bffa67034b9eac23e6f9b54f888888e14b632` |

Both reports retain `task_complete: false`. Their SDK source manifests are
unchanged before and after execution, with digest
`724d7d8a2154c4beebc799c545f83113e40eee4863e14b9ab3a0de99f33d6769`.
That SDK-tree digest is distinct from the full 4,591-file
[r14 source manifest](continuation-20261014-r6-evidence/sdk48/raw/20261014-aarch64-r14/inputs/source-manifest.json),
whose SHA-256 is
`807ca74a2d509fa4712d8d30253bf1b0dcea98ae276f1a269ee6920543a89363`.

## What the compiler matrix establishes

Each selected artifact compiled with the explicitly supplied existing toolchain,
passed its binary import contract, and passed the full Wasm validator. These are
separate recorded steps; compilation alone is not treated as validation or
Runtime execution.

| SDK interface | Passing rows |
|---|---:|
| C | 10 |
| C++ | 12 |
| Rust | 10 |
| TinyGo | 10 |
| AssemblyScript | 17 |
| JavaScript bridge C/Wasm modules | 2 |
| Total | 61 |

The two JavaScript bridge rows compile the bridge's C/Wasm modules; they do not
mean JavaScript source was compiled into Wasm. The pure AssemblyScript artifacts
have their own import contract. No compiler row starts a ROS node or demonstrates
an application's publication, subscription, Service or Action delivery through
uwvm.

## What the model matrix establishes

The 17 passing rows comprise one CMake relocation check, three C and four C++
assertion models, one native Rust test row, one native Go test row, and seven
AssemblyScript model rows. CMake verifies the relocated SDK package and its
exported targets. Native Rust executes five required tests across its three
reported binaries. Go separately attests **nine top-level tests and 57 declared
positive-vector subtests**. All required RUN and PASS records must occur once;
missing, duplicated, renamed, skipped, failed or incorrectly nested results are
refused.

Six import-free AssemblyScript models execute in host Node with exact assertion
counts: 175 codec-vector, 14 adapter-contract, 9 executor-mutation, 63 factory,
25 resource-role and 8 Unicode checks, totaling 294. The seventh is explicitly
recorded as `SEMANTIC_MODEL_EXECUTED`, without an invented assertion count. These
are host-native and host Node Wasm model results. They are not uwvm Guest or
AArch64/QEMU ROS behavior results.

The earlier
[r13 model report](continuation-20261014-r6-evidence/sdk48/raw/20261014-aarch64-r13/sdk-model-matrix-r1/report.json)
remains `MODEL_MATRIX_INCOMPLETE`, SHA-256
`1c229eb5cb114e66fcb6a3ca03a05c622eaeb08ab8549db682af4e254da933ee`.
Its Go commands exited successfully, but the old parser mixed unindented nested
RUN records into the top-level inventory and rejected that row. The r14 correction
keeps the original nine-test inventory and requires all 57 literal subtests
separately. It changes qualification of the transcript, not the Go test bodies.
The [r13 execution log](continuation-20261014-r6-evidence/sdk48/raw/20261014-aarch64-r13/sdk-model-matrix-r1/go-native-tests/execute-tests.log)
and [r14 execution log](continuation-20261014-r6-evidence/sdk48/raw/20261014-aarch64-r14/sdk-model-matrix-r1/go-native-tests/execute-tests.log)
are byte-identical, SHA-256
`de42d0e118d4b922fe4b238b031d0e18fb4e15b37a29879e656c830274e7f922`.
The failed r13 receipt is preserved; it was not rewritten as a pass.

## Exact execution boundary

The same original container is
`4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37`.
The parent measured its new StartedAt as `2026-10-14T07:22:19.151887711Z`.
Both SDK reports independently record the same before/after cgroup identity:

- Boot: `518a7b26-33dd-4044-9456-93e75aac9b63`.
- Leaf: `/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope`.
- Device/inode: `29` / `18927`; effective CPUs: `0-31`.
- Leaf and effective memory maximum: `51539607552` bytes, exactly 48 GiB.
- Leaf `memory.swap.max`: `0`; current-memory guard: 45,056 MiB, exactly 44 GiB.
- Aggregate sampled process RSS guard: 2,048 MiB.

All recorded whole-run memory-event deltas are zero, including `oom`, `oom_kill`
and `oom_group_kill`. Each of the 122 compiler/validator commands and 31 model
commands reports successful owned-process cleanup and no guard error. Process
sampling, held-launch identity, input/tool preservation and artifact checks remain
part of the receipts. The reports explicitly leave complete installed toolchain,
system dependency and global-configuration closure **unqualified**. These results
also provide no physical ARM performance or memory-order guarantee.

The host's global swap configuration is distinct from the tested leaf's zero-swap
limit. Earlier 64 GiB reports retain their original cgroup/boot scope. A new run
must measure its own live incarnation and cannot qualify 48 GiB by relabeling old
64 GiB evidence or accepting only an inherited ancestor limit.

## Authority and remaining acceptance

The [current security design](security-current-20261014.md) remains unchanged by
these SDK checks. ROS1 and ROS2 start disabled with empty connection fields and
no capabilities. WASIp1 starts with only directional standard input/output/error
rights and zero inheriting rights, with no inherited host environment or non-FD
authority. SDK installation, generated type registration and successful import
checking add no implicit grant, native socket/directory preopen or fallback ROS
permission.

An application must explicitly select and enable its target, provide its complete
connection, and grant only the required exact operations/resources/types with
all required limits. Any additional WASI permission must be explicitly supplied.
For the raw communication fixtures, the intended extra WASI authority is the
monotonic clock with host environment inheritance explicitly disabled. These
statements describe the preserved source policy; the SDK reports do not substitute
for actual Runtime default-denial and minimally granted Guest tests.

The separate [custom-codec supplement report](continuation-20261014-r6-evidence/events-sdk18/raw/sdk-supplement-execute-r4/report.json)
now records `SUPPLEMENTAL_CODEC_18_ROW_46_STEP_PASS`, SHA-256
`b89f3699afdb980833f6ee22a5aa4b01adbc9cfa5e65b23eeec3700ac93bfd50`.
It executes the r4 capsule against the same r14 source and original 48 GiB,
zero-swap incarnation, after its distinct `PREPARED_ONLY` receipt, SHA-256
`bc5035440633321658be1b4f731663e55cdca5d957fe181f17053e597edf6936`.
All 18 rows and 46 recorded commands passed with successful owned cleanup and
no guard error; all recorded whole-run memory-event deltas are zero.

There are three rows per language across C, C++, Rust, TinyGo, AssemblyScript
and JavaScript. Twelve codec models complete their exact pinned markers,
totaling **5,505 assertions**, including the models' 1,662 short-prefix trials
and their byte/golden/descriptor, Bool, nonfinite and bound checks. Ten of those
models compile and fully validate generated Wasm before execution in host Node;
the two JavaScript models execute directly in host Node. Of the six unchanged
usage tutorials, five receive compilation, import-contract and full-validation
qualification only. Their unsupported example-package alias is not promoted to
the official deployment identity or run as a ROS fixture. The JavaScript tutorial
executes its existing host-local roundtrip.

This supplement establishes the recorded pure codec and tutorial scope. It
explicitly records no ROS, native peer, uwvm Guest, ARM or default-denial proof,
leaves complete installed toolchain closure unqualified, and retains
`task_complete: false`. It does not establish Runtime delivery for the six
languages or complete custom-type support.

Current ROS/Gateway/IPC behavior, signed Enforce Guest communication, each
language's Runtime delivery, and the remaining original-task acceptance must be
qualified by their own exact source/product/architecture reports. See the
[immutable original task comparison](original-task-audit-20261013.md); the root
continuation record determines the broader task status.
