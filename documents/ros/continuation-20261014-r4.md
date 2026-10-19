# Restored Linux: actual ARM and custom-topic milestone

The original task remains incomplete. SSH is restored, and the original container
has been restarted with its existing 64 GiB memory limit, zero swap and CPUs
`0-31`. Actual compilation, ROS, WAT translation and Guest execution reported
here ran inside that cgroup. The immutable
[237-row original comparison](original-task-audit-20261013.md) remains the full
baseline; [this revision's evidence record](continuation-20261014-r4.json)
adds current results and explicitly lists every remaining G01–G14/L01 gap.
Earlier offline reports and failures have been preserved.

| Actual result | Count | Evidence boundary |
| --- | ---: | --- |
| Official Noetic/Jazzy custom package generation, build, install and compiled identity probes | 12 stages | Genuine native MD5/RIHS identities; package setup alone is not Guest execution. |
| Fixed custom Topic scanner, default-deny and explicit-grant contracts | 56 / 8 / 306 checks | Actual ARM native units; no ROS network claim. |
| Original ROS native units | 6 | Earlier r8 snapshot; not relabelled as final r10 parent acceptance. |
| WASIp1 configuration, authority32/64 and original positive regressions | 17 | Actual r9 ARM execution; CLI, LLVM and checkpoint requirements remain separate. |
| Fresh Runtime profiles: none, ROS1, ROS2, dual | 4 | Actual r10 ARM ELF compilation and QEMU execution. |
| ROS default configurations on the custom-enabled dual Worker | 6 | Each tests 32 ROS1 and 37 ROS2 forged API calls, preserves a 72-byte sentinel and observes zero private IPC accepts. |
| Fresh custom-enabled Gateway builds | 2 | Actual C++17 ARM compilation, full system-inclusive dependencies and preserved SDK/source inputs. |
| Native custom Topic fixtures | 2 | ROS1 23 bytes and ROS2 45 bytes; see their different scopes below. |
| Raw-import custom Wasm Guest loopbacks | 2 | Current dual Worker, authenticated IPC, qualified Gateways and actual native adapters. |

The official fixed Topic increment supports finite scalar values, fixed arrays
and fixed nested records. Trusted providers and the common registry are selected
at build time. Every Guest still needs explicit enablement, exact resource/type,
native identity, adapter codec identity, quotas and transport/QoS grants.
Registry compilation creates no authority. Dynamic strings/sequences,
constants/defaults and custom Service/Action registration remain unsupported.

The ROS1 native fixture uses a separately forked peer, opposite caller IDs,
connected TCPROS bus rows, an exact-byte acknowledgement and successful reaping.
The ROS2 native fixture exercises real RMW/DDS callbacks in the same process and
context, with 38 checks. It does not establish an independent-process transport
or production SROS2 result. Native fixtures construct explicit grants directly;
default Guest permission denial is established by separate tests.

Both raw WAT Guests assert noncanonical Bool2 refusal (`errno22`), every returned
record byte and its 23/45-byte length, successful close, and stale publication
handle refusal (`errno9`) before `proc_exit(0)`. Live Worker mappings are checked
before releasing the stdin gate into tested ROS calls. These are Gateway-native
loopbacks; they do not establish six-language Guest interoperability or an
independent native absence-of-side-effects oracle for the denied Bool2.

The WASIp1 r8 failures led to minimal fixes: explicit fast_io token views in the
test, unambiguous WASI test aliases, inclusion of the existing floating-point
probe header, and AArch64 `epoll_pwait` through existing
`fast_io::system_call` where `epoll_wait` is absent. Old test assertions remain
intact. Only the unrelated ROS1 fixture enum declaration changed between r9 and
r10; the WASIp1 report retains its r9 binding.

All failure receipts remain available, including native custom attempts r4–r9
and the r8 WASIp1 result with ten passes and seven failures. The strict ROS1
attempt1 rejected three undeclared Python caches created by a review import
before any compilation. Their bytes were preserved and retired, all 4,579
declared source hashes were reverified, and fresh attempt2 passed. The strict
validator was not relaxed. The corrected independent audit records that review
side effect explicitly.

The fast_io source review finds Worker formatting, scanning and file loading
already use fast_io; new custom codecs perform no I/O. Repeated native audit
number parsing and floating-bound stream parsing remain optimization candidates.
The current C++17 Gateways cannot directly include the vendored C++20 fast_io
API. Compatible caching/buffer work and measured performance remain pending.

At this record's cutoff, the original 18 native products are being rebuilt.
Successful compilation will require separate behavior results. SDK tool recovery
is also ongoing: the baseline 61 compiler rows, 17 native/pure model rows and
supplemental 18 custom-codec/example rows have not executed. Archive transport,
verified tool installation and source/model proposals are not test passes.

G02 unified seven-event delivery, generic backpressure, complete current
acceptance/recovery/security/attack/performance/regression/CI matrices, full
examples and three-Group scenarios, six-language runtime acceptance and unified
final delivery remain incomplete. G03 parent search and G04 Action QoS still
need their current native feature execution. The evidence record retains the
full gap list without a misleading completion percentage.
