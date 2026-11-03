# CI lanes and evidence

The fast pull-request workflow performs file analysis only on disposable
Ubuntu 24.04 hosts: commit whitespace and bounded `ast.parse` of tracked
CI/helper Python files. Its isolated standard-library Python process imports
no repository modules and never evaluates the analyzed source. It runs no
functional tests, compiler, C++ probe or installer. `STATIC_ANALYSIS_ONLY` is
not execution acceptance and uses no self-hosted runner.

The trusted-main release workflow runs the actual source/evidence helper
tests, finite ROS capability/IPC units and Linux x86_64 AOT release build in
one admitted 64 GiB/zero-swap batch under the existing global lease. Its full
manual or weekly selection adds a distinct AArch64 qualification candidate
with the same guarded stages. Candidate names do not declare native ROS,
QEMU, performance or release support.

The separate upstream capability workflow checks out ordinary
`UlteSoft/uwvm2` at `999bb9439024b3c2adba54044b8374910efa411d` and runs its
complete strict interpreter suite. It does not compile an uwvm2-ros interpreter
product or alter the ROS product policy. There is no macOS ROS product lane.

## The observed failure

[Run 40234864388](https://github.com/UlteSoft/uwvm2-ros/actions/runs/40234864388)
tested source `c33f289c7843445336d0737df84d3f28b62ae0a1`. All four
`interpreter-products` jobs failed in compiler provisioning with exit 100:
`clang-23` had no candidate and the libc++/libc++abi 23 development packages
were absent from the selected unversioned Noble feed. Every product/unit/runtime
step was skipped. The upload step found no files and produced zero artifacts.
This run contains no successful build or execution evidence.

A fresh API/log review also inspected [CI run 40238824780](https://github.com/UlteSoft/uwvm2-ros/actions/runs/40238824780)
at source `35f6ce3747e7f56fea0ffbe434b64271c0e77d72`. Its Linux setup again
failed because `clang-23` and `lld-23` had no installation candidate and
`clang-tools-23` was absent. Other matrix entries were cancelled or skipped;
the macOS entry failed its Windows CRT/COFF check and FreeBSD ended with an
SSH transport failure. These are original workflow failures, not executions of
the new Linux-only workflow sources in this change.

The [LLVM package repository](https://apt.llvm.org/) now documents separate
versioned feeds. The corrected route is HTTPS `llvm-toolchain-noble-23`, with
the repository key restricted by `signed-by` and its primary fingerprint
checked. On 2026-10-26, read-only amd64/arm64 package indexes contained
`clang-23`, `lld-23`, `llvm-23`, `libc++-23-dev`, `libc++abi-23-dev` and
`libunwind-23-dev` at
`1:23.1.5~++20261023090852+395ba2c4d274-1~exp1~20261023090919.90`.
Index availability is not installation or compilation verification. The exact
installer and C++26 bootstrap probe remain unchanged. Installer qualification
is pending in the separately retained source-only manual Linux64
`ci-toolchain-route-r1` packet; it must run under its global lease and actual
64 GiB/zero-swap admission after remote reconnect/reconciliation. It has not
run and cannot supply a hosted CI pass. No hosted workflow installs packages
or runs that probe, and no persistent CI runner executes apt/sudo provisioning.
The bootstrap packages do not replace the source-paired LLVM/runtimes used by
the product.

## Persistent runner admission

Functional tests, full product, native distribution and upstream capability execution require
reviewed Linux runners with preprovisioned tools. CI performs no privileged
installation or container launch on them. Xmake, CMake, Ninja, Python 3, the
selected LLVM 23 bootstrap closure and native kernel facilities must already
exist. Do not put unrelated workloads on the same test lease.

`product.py` first runs the unchanged actual C++26 compile/execute probe using
the reviewed preprovisioned prefix, inside `guarded.py`. It then runs the
module checker, checker semantics, ROS profile contracts, qualification helper
unit tests, finite security units and the IPC/protocol/handshake/deadline suite.
The unit compiler and LLD come from that same admitted prefix. Real filesystem
socket paths use a fresh short private `/tmp` directory; logs, subreceipts and
unit executables are copied into evidence before owned temporary cleanup.
Every functional/compiler/probe child inherits the monitored original cgroup;
no hosted label is treated as a substitute for actual admission.

Configure repository variables as absolute paths:

| Runner label | Admission variable | Compiler-prefix variable |
| --- | --- | --- |
| `ros-x86_64-cgroup64` | `ROS_X86_64_ADMISSION` | `ROS_X86_64_LLVM23_PREFIX` |
| `ros-aarch64-cgroup64` | `ROS_AARCH64_ADMISSION` | `ROS_AARCH64_LLVM23_PREFIX` |

The native distribution job additionally selects its existing `ros-sdk` and
`ros-cgroup64` labels and uses the x86_64 admission. Give that physical runner
the matching primary label too. Admission JSON pins the original cgroup
incarnation, current boot, the existing shared lock's device/inode and a
reviewed provisioning manifest digest. All actor hashes in that manifest are
verified before build/test. See
[the acceptance checklist and executable helpers](../test/0080.product-qualification/README.md).
Missing variables, missing SDKs, stale boot identities or inadequate resources
are blockers requiring explicit reprovisioning; they must never select a
generic fallback runner or pass by skipping.

All self-hosted jobs are restricted to the canonical repository and trusted
`main`; none is triggered by `pull_request` or `pull_request_target`. Disable
fork access to these runner groups and restrict their allowed workflow paths
in repository administration. Branch protection and runner-group restrictions
are external configuration: a YAML change cannot prove those settings exist.
The workflows request only `contents: read`, do not persist checkout credentials,
pin checkout/upload actions by commit, and never publish or release artifacts.

## Review a result

Each guarded execution initializes a source manifest before resource admission.
Required stages, command arguments, return codes,
deadlines, stdout/stderr hashes and final artifact hashes are retained even on
failure. `if-no-files-found: error` prevents an empty upload from masquerading
as retained evidence. Missing required stages cannot yield a successful receipt.
Source bytes are rechecked at sealing. The product packet contains its exact
release ELF and finite smoke-test subreceipts.

The release receipt explicitly requires `resource`, `toolchain`,
`module-source`, `module-checker-tests`, `ros-profile-tests`,
`qualification-helper-tests`, `ros-security-units`, `ros-ipc-units`,
`configure`, `build`, `target`, `closure`, `version` and `basic`. Missing,
blocked, failed or timed-out stages prevent acceptance. The outer batch
deadline remains a hard limit; skipped later work acquires no pass. Finite
helper/security/IPC results cover implementation contracts, not actual native
ROS endpoints or complete product support. The fast static artifact remains
separate from these execution receipts.

Review the seven evidence dimensions independently. An actual product build
and `basic_test` do not certify all Wasm proposals, native ROS endpoints, SDK
languages, full-system QEMU or performance. The comprehensive checklist starts
those cells untested. Review official-suite discovery, disabled cases and the
exact tuple before making any broader support claim. Local authoring on macOS
does not count as a Linux CI run. This resource-policy correction is source
only; its file analysis, guarded tests, product execution and installer route
remain unverified until their own actual Linux receipts exist.
