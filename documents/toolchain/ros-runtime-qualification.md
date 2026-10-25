# ROS runtime qualification, 2026-10-25

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
helper directly, not a running systemd installation.

## Native execution and debugger

| Target | Actual checks | Limit |
| --- | --- | --- |
| x86_64 | Native product build, `xmake basic_test`, cache-identity regressions, ROS ancestry/reentry, full-system QEMU cache/unwind/debugger, 24 native call ABI guests | Native ROS measurement used the specified local peer workload. |
| AArch64 | LLVM 23 paired cross-build, full-system QEMU basic checks, 24 native call ABI guests, persistent-origin reboot checks | This exact libc/runtime closure is qualified; a stock older distro or GPU is not implied. |
| ARMv7 hard-float | EHABI provider and older-libc gaps identified; isolated repair/build work | Not accepted yet; no ARMv7 runtime, debugger or performance pass is claimed. |

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

The original PDF's approximately 1.8× comparison used recreated native clients
and an older interpreter/gateway path. It was neither a retained-client
comparison nor a cache-hit startup measurement. The figures above do not
relabel that old result.

## Evidence boundaries

The public inventory contains 111 imports in seven ROS namespaces. This
performance batch covers synchronous Trigger calls and five clock operations.
Topic/subscription/event, action, TF, parameter, graph, lifecycle, and direct
RMW performance comparisons remain incomplete. Functional fixtures and source
presence do not constitute performance acceptance for those interfaces.
Named C++ module cache builds, filesystem-preopen cache contexts and arbitrary
native providers also remain unqualified.

Qualified current native VM SHA-256:
`74ba561a29006f8f30aa1e2e4b5d603a8e0a478fd7086185a6f4106cc07222cb`.
Qualified current AArch64 VM SHA-256:
`c7638cfd73d148f8b878b7a4fa2727735edbaebb9557232dc85ff46ef3c2c312`.
The measured transport cohort used native VM SHA-256
`8f7e386515a2df7e370264f5c8f800a85198268a4c4eb1837f1993722f9ec9b2`.
Later native qualification changes do not silently acquire those exact timing
measurements.

Raw receipts, source manifests, stdout/stderr, failed attempts and VTune data
remain private test artifacts, outside the SDK and Git. Export archive digests:

- `qualification-results-r2.tar.gz`: `bf78a4969a4857797c94a2e72e914f94add4062aaf4677a0c001d806438ac87e`
- `qualification-results-r3.tar.gz`: `13ba0f0ff5bed9bda4b816de7a991de3f3048799243dffebb36038e2ef20f1a1`
- `qualification-noetic-r1.tar.gz`: `94119ddc3ee6a49114a463899443b3f1087ae0d6a1189049946d9415d4c26491`
- `qualification-cache-origin-r1.tar.gz`: `6c62deaad0d76dd85d8f555b46386d9bad370c66b0054a8ed9e6c583aa998363`

Earlier aggregate failures remain failures in their original receipts. Examples
include an unsupported WAT assembler grammar, an ANSI-stripping checker error,
a service-discovery timeout and an incorrectly writable initramfs root. The
replacement runs identify the corrected fixture and exact tested executable;
they do not overwrite the failed records or loosen production checks.
