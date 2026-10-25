# ROS runtime qualification, 2026-10-26

The qualified Linux product uses live-checked native unwind. The public
`instruction`, `auto`, `none` and `unwind-uncheck` stack policies are refused.
A failed native probe stops execution rather than selecting instrumentation.
This report records measured cases; it does not certify every ROS interface,
CPU, vendor device, filesystem or deployment.

## Cache identity and permissions

The actual executing ELF is SHA-256 hashed, independently of its version string
or path. Complete translated IR, target/CPU features, native ABI and emission
policy identify compatible objects. The full bounded envelope is authenticated
with Ed25519 before decompression. The source-built LLVM, C++ ABI, libunwind and
OpenSSL bytes are linked into and covered by that ELF identity.

The [admission contract](ros-cache-admission.md) distinguishes these code inputs
from live ROS/WASI grants and unrelated source trees, paths, timestamps and log
contents. Objects never restore a previous process's capabilities. Canonical
ROS provider ownership, request/response validation, quotas, revocation and
authenticated IPC remain enforced. No audit or permission check was removed
to obtain the performance results below.

Regression cases include instruction changes, executable-byte changes,
corrupted signed objects, restored original bytes, unchanged module relocation,
changed initial memory data, ordinary redirected output, read-only cache
storage, pseudo-file refusal and a revoked WASI clock grant. Cache refusal
diagnostics distinguish admission, absence, I/O, compatibility and signature
failures. A cold miss prints current identity fingerprints; it does not invent
an old entry or a reason that cannot be established from the available data.

The optional [administrator helper](../../tools/cache-origin/README.md) restores
a root-only persistent origin into the target UID keyring before VM startup.
Both x86_64 and AArch64 passed two actual full-system Linux boots with different
boot IDs, unchanged VM/Wasm bytes, a signature-verified hit and byte-identical
cache objects after reboot. The VM ran as a dedicated unprivileged UID. Loose
permissions, links, truncated files and a conflicting origin were refused;
the original live key and cache still worked after those refusal tests.
The systemd unit supplies deployment ordering; these boots exercised the
helper directly, not a running systemd installation. Those two-boot checks
belong to the retained pre-revision-14 executables and unchanged administrator
helper. Fresh-process cache qualification of a later ELF is recorded separately;
it is not relabeled as a new two-boot run.

## Native execution and debugger

| Target | Actual checks | Limit |
| --- | --- | --- |
| x86_64 | Native product build, `xmake basic_test`, cache-identity regressions, ROS ancestry/reentry, full-system QEMU cache/unwind/debugger, 24 native call ABI guests | Native ROS measurement used the specified local peer workload. |
| AArch64 | LLVM 23 paired cross-build, full-system QEMU basic checks, 24 native call ABI guests, persistent-origin reboot checks | This exact libc/runtime closure is qualified; a stock older distro or GPU is not implied. |
| ARMv7 hard-float | Paired LLVM 23 revision 14 build, full-system QEMU cache store/two fresh hits, checked three-frame unwind, all 24 call ABI guests and policy refusals | Tested target libc is 2.35. ARM32 native debugger, stock Focal and native ROS endpoints remain unqualified; no QEMU performance is claimed. |

The retained provider revision `23.1.1-uwvm-ros.14` was rebuilt from its verified
source inventory for all three targets. ARM TailCC now uses the target's actual
stack alignment for callee-pop size. Functions with incoming callee-popped
arguments no longer shrink-wrap away the epilogue on an early return. The paired
libunwind consults the owned JIT exidx registry; static-image lookup uses bounded
loader-owned program headers. Its ABI marker and live unwind probe remain
mandatory. ARM EHABI setup is independent of the debugger build option.

The x86_64 and AArch64 providers and products were then rebuilt from the complete
paired revision-15 source inventory. The native product passed `basic_test`,
cache-identity and permission regressions, ROS ancestry/reentry, the corrected
live debugger driver, all 24 ABI guests and all twelve retained KUKA programs.
On AArch64, full-system QEMU passed the basic cache/WASI/ROS,
checked-unwind and debugger checks, all 24 call ABI guests, and all twelve
retained KUKA TypeScript example/regression programs across wasm32 and wasm64.
The original memory64 failure and its caller-frame repair are described below.
These results do not relabel the revision-14 ARMv7 product or earlier timing
cohorts as revision-15 measurements.

The 24 ABI guests exercise direct, indirect and typed-reference calls, genuine
tail calls, mutual recursion, stack/vector arguments, multi-value results,
table mutation and the corresponding null/type/bounds traps. Wasm frames come
from native unwind; the three-frame smoke case requires exact function indices.
AArch64 can still report truncation beyond an owned native boundary; this is
not a claim that every external host frame is recoverable.

The live debugger tests the current executable with its matching broker and
adapter: native disassembly, a 400-instruction window, native stepping and
stale-reference refusal. ROS1 and ROS2 ancestry tests retain denial across
cross-module direct/reference/indirect/tail calls and restore the suspended
owner after host reentry and exceptions. ROS provenance travels in a private
native argument, not in per-function diagnostic push/pop hooks.

The standalone debugger driver now verifies every source-manifest entry in both
the historical `vm/` layout and the repository-relative layout. It rejects
missing, changed, truncated, duplicate, noncanonical and escaping paths. Eight
Linux checks passed; each actual manifest layout verified 27,313 complete files.
This corrects a driver omission and does not replace the outer build's own
source and ELF verification.

Selected ROS release architecture boundaries and NVIDIA hardware limitations
are documented in [ARCHITECTURES.md](../../test/0073.ros-tests/ARCHITECTURES.md).

## Native performance

Measurements ran on an Intel Core i9-14900HX, with workload affinity fixed to
CPU 0. Every batch checked its actual membership in a 64 GiB cgroup with zero
swap and held the global test lease. No QEMU timing is used. Service results
are medians of nine interleaved measured rounds after two discarded warm-up
rounds. Startup, compilation, discovery and twenty warm-up exchanges per
launch precede the timed loop. Every reply and total native callback count is
validated; timeouts are failures, not silently retried samples.

| Workload | Retained native client | Before: Wasm + gateway | After: Wasm + gateway | Change |
| --- | ---: | ---: | ---: | ---: |
| ROS2 Jazzy Trigger, 10,000 calls/round | 20.981 µs | 40.221 µs | 37.153 µs | 7.63% lower; 1.77× native |
| ROS1 Noetic Trigger, 1,000 calls/round | 38.852 µs | 313.575 µs | 46.011 µs | 85.33% lower; 1.18× native |

The ROS2 cohort recorded 551,100 actual callbacks across its five comparison
paths. Its IPC-only path changed from 34.110 to 32.447 µs. The optimization
borrows the immutable capability under its retained authorization lock, avoids
redundant readiness polling and combines small frame headers/payloads without
changing wire bytes. The same cancellation precedence and absolute deadline
remain; ready-transfer checks and 57 cancellation checks passed.

ROS1 now reuses a validated TCPROS connection under the complete capability,
endpoint and session owner. Each request keeps its deadline. Invalid metadata,
timeout, malformed response and disconnect retire the failed stream without
replaying a request. A waiter that times out does not discard another active
request's connection. Actual TCPROS fault, retirement and concurrency tests
passed. The Noetic measurement uses an explicit compatible loader for this
newer VM build; it does not qualify the VM against stock Focal libc.

ROS1 ROS/wall clocks and ROS2 ROS/system/steady clocks were also measured.
Native reads were approximately 0.011–0.021 µs; gateway IPC approximately
6.8–8.1 µs; Wasm plus gateway approximately 10.5–12.6 µs. These local operations
expose fixed IPC overhead. Replacing a gateway clock with an unrelated local
clock would change the ROS clock-domain contract and was not done.

VTune hardware Hotspots and Microarchitecture Exploration summary collection
completed for native ROS and the Wasm/gateway paths. The latter is actual
driverless per-process hardware counting, not the unavailable detailed-PMU
sampling mode. ROS2 before/after process-tree instructions were approximately
21.39/19.43 billion; cycles 12.58/11.34 billion. ROS1 native/Wasm process-tree
instructions were 25.11/28.48 billion; cycles 13.07/14.61 billion. These totals
include native peers and profiler descendants: they are not VM-only counters
or uninstrumented latency measurements. Unresolved kernel symbols remain
unresolved; host security settings were not relaxed to label them.

### Graph and parameter operations

A later source-built native VM and the same retained-peer method cover eleven
ROS1 and ten ROS2 graph/parameter cases. These use five interleaved measured
rounds after two warm-up rounds, 1,000 calls per round, CPU 0, explicit manual
grants and allow-and-deny auditing. Every result is decoded and checked against
the live isolated peer. No native result is cached. The VM SHA for this cohort
is `95d0229551b7b15caf76a0fd761e79c5845cc1425890347e7c110abe25ae6502`.

ROS1 retained XMLRPC connections belong to the exact capability, native endpoint
and session. Calls retain their absolute deadlines. A timed-out waiter cannot
retire another active call's connection; transport/protocol failure retires the
failed stream without replay. Tests changed a parameter through an independent
native client and observed the fresh value on the retained connection. They also
covered policy changes, alias retirement, malformed replies and timeouts.

| ROS1 operation | Native (µs) | Before (µs) | After (µs) |
| --- | ---: | ---: | ---: |
| Graph: inspect node | 86.291 | 245.757 | 111.868 |
| Graph: inspect service | 85.959 | 245.364 | 110.976 |
| Graph: inspect topic | 61.477 | 207.774 | 79.927 |
| Graph: list nodes | 86.216 | 247.617 | 112.972 |
| Graph: list services | 86.817 | 247.965 | 112.107 |
| Graph: list topics | 61.466 | 205.346 | 79.386 |
| Parameter: exists | 59.902 | 203.805 | 77.080 |
| Parameter: get | 59.847 | 207.008 | 78.518 |
| Parameter: list | 62.731 | 208.796 | 81.531 |
| Parameter: search | 59.951 | 204.898 | 77.736 |
| Parameter: set | 98.342 | 290.675 | 123.912 |

The new Wasm/gateway measurements are approximately 54–62% lower and
1.26–1.31× the corresponding native client. Setter samples varied more than
read-only queries. The Noetic loader qualification limit above still applies.

| ROS2 operation | Native (µs) | IPC only (µs) | Wasm + IPC (µs) |
| --- | ---: | ---: | ---: |
| Graph: inspect node | 0.422 | 7.640 | 11.741 |
| Graph: inspect service | 10.014 | 14.779 | 18.939 |
| Graph: inspect topic | 1.703 | 8.801 | 13.173 |
| Graph: list nodes | 0.429 | 7.682 | 11.840 |
| Graph: list services | 11.267 | 16.313 | 20.685 |
| Graph: list topics | 1.684 | 8.752 | 15.880 |
| Parameter: describe | 25.319 | 37.113 | 44.584 |
| Parameter: get | 25.459 | 38.776 | 46.370 |
| Parameter: list | 30.167 | 41.981 | 44.581 |
| Parameter: set | 26.635 | 39.115 | 44.898 |

ROS2 node/topic discovery can read a native local graph cache. Its sub-microsecond
native time cannot include the authenticated cross-process hop present in the
IPC and Wasm paths. The table exposes that fixed cost rather than attributing
all of it to Wasm. List-topic Wasm samples ranged from 12.815 to 16.365 µs;
the displayed median does not eliminate that variability.

A separate 50,000-call ROS1 parameter-get VTune Hardware Hotspots cohort measured
native/old/new process-tree CPU time of 3.547/12.228/5.063 seconds and estimated
instructions of 33.34/92.40/40.52 billion. Hardware Microarchitecture summary
counting for native/new observed 17.357/23.011 billion cycles and 32.822/39.990
billion instructions, with multiplexing reliability 0.953/0.984. The old-path
retry had reliability 0.080 and is excluded from precise microarchitecture
comparison even though its command exited successfully. These profiler totals
are separate from the uninstrumented latency table.

### Original Uwvm2 comparison

The immutable original source commit
`999bb9439024b3c2adba54044b8374910efa411d` was built with the same LLVM 23 compiler,
paired LLVM/C++/unwind/OpenSSL closure and checked-unwind policy. Four original
unused-code diagnostic categories remained warnings so the unmodified source
could build; no original runtime code was changed. Fifteen interleaved native
rounds after two discarded warm-ups measured the VM's execution timer, with
cache disabled. Process wait time is not used as a startup metric.

| Workload | Original (ms) | ROS before (ms) | ROS after (ms) |
| --- | ---: | ---: | ---: |
| 10 million direct calls | 20.857 | 23.555 | 23.592 |
| 10 million indirect calls | 25.544 | 32.447 | 28.894 |
| 100,000 WASI clock calls | 1.836 | 2.067 | 2.052 |

The optimization folds redundant normalization of the private ROS provenance
token. Module-owner identifiers are even: equality with the complete incoming
token proves both the same owner and an untainted chain. Any other token retains
the crossing bit. This is equivalent to masking the token and separately
combining that bit; no authority check or cross-module propagation is removed.
Native ancestry/reentry, permission denial, debugger/basic checks and all 24 ABI
guests passed on the optimized executable. Indirect-call time fell by about 11%;
direct calls were unchanged. Both remain approximately 13% slower than the
original in this microbenchmark, so performance parity is not claimed.

Original executable SHA-256:
`d77f39c4dc5b7f52056273881c4b1d384fb81ebbfe51197262c643e44ac61c2a`.
Measured optimized executable SHA-256:
`910c2bbebf31008f54ebd2acea8273a06d8bf9eb4ad090fca855b31add0cd8cc`.
The formal provider rebuild is a separate qualification and does not inherit
these exact timing samples.

The original PDF's approximately 1.8× comparison used recreated native clients
and an older interpreter/gateway path. It was neither a retained-client
comparison nor a cache-hit startup measurement. The figures above do not
relabel that old result.

## Evidence boundaries

The public inventory contains 111 imports in seven ROS namespaces. The
measured coverage comprises two backend-specific Trigger calls, five clock
operations and 21 graph/parameter operations. Topic/subscription/event, action,
TF, lifecycle and direct RMW performance comparisons remain incomplete; scalar
string parameter cases do not qualify every parameter type or event route. Functional fixtures and source
presence do not constitute performance acceptance for those interfaces.
Named C++ module cache builds, filesystem-preopen cache contexts and arbitrary
native providers also remain unqualified.

Final revision-14 native VM SHA-256:
`b5b661b1f255af39c0f5f20f0a20dc56e591935008f81f592805d14c9d530355`.
Final revision-14 AArch64 VM SHA-256:
`b52cfb514643b38da9960a2527fbfc87da1266dbb8479b7acd6a02743e279aae`.
Final revision-14 ARMv7 hard-float VM SHA-256:
`ee64de7f31af0aa2bcbabea099cef559e99089d6ecddc1edb494e8f5facc1ae9`.
Their frozen source identity is
`sha256:ce64a459ef5e58090a86600d921d72967f19782205f32463949be0a328320c8d`.
The standalone debugger witness separately records its corrected driver hash;
its explicit source-root, complete source inventory and actual ELF checks remain.
Earlier native/AArch64 qualification receipts retain their original hashes
`74ba561a29006f8f30aa1e2e4b5d603a8e0a478fd7086185a6f4106cc07222cb` and
`c7638cfd73d148f8b878b7a4fa2727735edbaebb9557232dc85ff46ef3c2c312`.
The measured transport cohort used native VM SHA-256
`8f7e386515a2df7e370264f5c8f800a85198268a4c4eb1837f1993722f9ec9b2`.
Later native qualification changes do not silently acquire those exact timing
measurements.

The SDK public ROS deployment serializer and NVIDIA empty-authority Xmake helper
now select AOT plus checked unwind. The maintained real-Wasm launch probe passed
through both launchers for both ROS import families, including denied clock/ROS
operations. Two retained TypeScript wasm32/wasm64 programs also executed through
the AOT launcher. This finite probe does not certify every SDK language/platform,
the PAL Java preload application, or a physical NVIDIA endpoint. PAL's updated
shell launch command was separately syntax-checked.

The complete SDK `run.py` launcher now also has eight real-Wasm checks covering
both ROS import families. A deployment with every module's ROS policy disabled
runs without a gateway or a matching sourced ROS distribution. An enabled
preload still requires a gateway before worker creation. These checks use the
final revision-14 native executable and explicit stream-only authority.

Further TypeScript coverage exposed an AArch64 revision-14 failure: three KUKA
wasm32 examples passed, but the first wasm64 example trapped during QuickJS
context initialization. All twelve retained KUKA example/regression artifacts
passed on the exact revision-14 x86_64 executable. The original failing Wasm is
`bdc93a619bf17729f9f6a72c2c4975e0b1acd503bc5f248b357a92e221cf3bc3`
and the failed receipt retains its actual Wasm frames.

The access was in bounds. A guaranteed-tail callee had advanced SP, and LLVM
reloaded a caller spill before restoring the reserved caller frame. Revision 15
moves that existing restoration immediately after the call. The old provider
fails independent direct/indirect machine and executable controls; the repaired
provider passes both endian machine checks and a real little-endian executable.
The exact original QuickJS guest then passes `pb-o1`, `legacy-light` and `pb-o3`.
No bounds, permission, provenance or unwind check was removed. See
[the compiler repair and portable regression](llvm-aarch64-callee-pop.md).

The fresh, source-paired native x86_64 revision-15 ELF has SHA-256
`d8892127689d175e6344a14c9ec4d28515493c283969a95091ab05b30317b88e`.
The corresponding AArch64 ELF has SHA-256
`7015f8650bf8b19d0e670dc8681a31007b07e7464123aa5a33664c1e9067e466`.
Their frozen source identity is
`sha256:5761395cfae8912fae119f3ffd0ed0f6ccdf0c49faf0e8836996ccca742789b0`.
Both complete formal runs pass all twelve unchanged KUKA artifacts. This
qualifies those finite codec programs under AOT and checked unwind; it does not
qualify a native KUKA peer, physical controller, or every SDK language/library.

A second private optimization that removed unused provenance operands produced
no stable native latency improvement in fifteen measured, interleaved rounds
against the formal revision-14 executable. It is not part of the product.

Raw receipts, source manifests, stdout/stderr, failed attempts and VTune data
remain private test artifacts, outside the SDK and Git. Export archive digests:

- `qualification-results-r2.tar.gz`: `bf78a4969a4857797c94a2e72e914f94add4062aaf4677a0c001d806438ac87e`
- `qualification-results-r3.tar.gz`: `13ba0f0ff5bed9bda4b816de7a991de3f3048799243dffebb36038e2ef20f1a1`
- `qualification-noetic-r1.tar.gz`: `94119ddc3ee6a49114a463899443b3f1087ae0d6a1189049946d9415d4c26491`
- `formal14-qualified-products-r1.tar.gz`: `6c5bd83d3f64797f59879f0222053c48afd5f7257120b1f4f0ee5a5e0cf9aeee` (exact three executables; no signing origins)
- `formal14-final-receipts-r1.tar.gz`: `caf178b7159482e797141f3c75e812da11ae8d69eecba0e4fd07aa2cabe01bbe`
- `native-results-recovery-r6.tar.gz`: `c9267593e51a3e2066f9015a20d48df90c82366693c34b2a850aeb27d652fb9f`
- `native-results-recovery-r5.tar.gz`: `61d38008bbadf9d636aa51e86f9195549da2899d3cd7d22653d4bf631e7e613d`
- `ros1-resource-results-recovery-r2.tar.gz`: `50f85e49c3f45b42ea17053a9780d8dec8b10373e8b16bd8215c1a0ed2e7baea`
- `ros1-resource-vtune-raw-recovery-r1.tar.gz`: `3b51ac4baa326880de8444dd2869279e5336323095fc74f4623be99e79adea4a`
- `qualification-cache-origin-r1.tar.gz`: `6c62deaad0d76dd85d8f555b46386d9bad370c66b0054a8ed9e6c583aa998363`
- `review-evidence-r8/results.tar.gz`: `1d4b249f79fd5e42288467f4acda363d5ad2e1d31d0e890aaa2dafc4baaa4d9f` (352 verified receipt members, including the AArch64 failure and SDK launcher checks)
- `formal-aarch64-r15-r1/receipts.tar.gz`: `14bb66f260ea7715a9489da7a7ab543ddc997ae0ee2a468af46deb37a581db86` (32 verified receipt members; fresh paired build, machine/executable controls, full-system basic/24-ABI/12-SDK checks)
- `formal-native-r15-r1/receipts.tar.gz`: `16440d1c59f8ab5afb4c3f45ab3ce0f813d2d3b33086087bf42f66fd17b06286` (225 verified receipt members; fresh paired build, basic/cache/ancestry/debugger/24-ABI/12-SDK checks)

Earlier aggregate failures remain failures in their original receipts. Examples
include an unsupported WAT assembler grammar, an ANSI-stripping checker error,
a service-discovery timeout and an incorrectly writable initramfs root. The
replacement runs identify the corrected fixture and exact tested executable;
they do not overwrite the failed records or loosen production checks.
