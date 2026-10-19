# Qualify platforms, debugger behavior and performance

Qualify the exact final Wasm, Runtime, native Gateway and peer combination.
Record source/binary/tool/input SHA256 and actual target architecture. A source
recipe, emulator `--version`, manifest or native container run does not prove
the target Runtime executed the application correctly.

## ROS architecture scope

The official architecture tables inspected on 2026-10-18 list these relevant
Linux targets. ROS2 distro support and this project's actual acceptance are
different claims:

| Target | Official source and status in the inspected tables | WasmROS acceptance required |
| --- | --- | --- |
| x86_64 / amd64 | Jazzy/Kilted Ubuntu24.04 Tier1; Lyrical Ubuntu26.04 Tier1 | Native Linux plus QEMU coverage of the selected target images |
| aarch64 / arm64 | Jazzy/Kilted Ubuntu24.04 Tier1; Lyrical Ubuntu26.04 Tier1 | Real AArch64 ELF/ABI, native ROS peers and target Runtime under qualified emulation or hardware |
| armv7 / armhf / arm32 | Jazzy/Kilted Ubuntu24.04 Tier3; Lyrical Ubuntu26.04 Tier3; source builds | Actual hard-float ABI/sysroot and ARM execution; not an AArch64 substitute |
| Additional experimental Linux architectures | No ROS support inferred from an available QEMU executable | First establish toolchain/ABI/native ROS availability, then report the exact completed subset |

Sources: [REP2000](https://reps.openrobotics.org/rep-2000/) and its
[official repository text](https://raw.githubusercontent.com/ros-infrastructure/rep/master/rep-2000.rst).
The current REP explicitly ends its distro tables at Kilted and delegates
Lyrical onward to each release's documentation. Lyrical was checked separately
against its [official platform source](https://raw.githubusercontent.com/ros2/ros2_documentation/lyrical/source/Releases/lyrical/supported-platforms.rst)
on 2026-10-18; the rendered documentation returned an access-denied page.
Lyrical also lists Ubuntu24.04 as Tier3 for these three architectures. Its
minimum languages include C++20 and C17, so a build must use that distro's actual
source/dependency closure. The existing fixed source ports are not relabeled
as Lyrical-compatible merely because their architecture appears in the table.
[REP3](https://reps.openrobotics.org/rep-0003/)
lists amd64/arm32/arm64 for ROS1 Noetic, whose support period ended in May2025.
An existing ROS1 deployment remains an explicit compatibility target; report
its actual distro and dependencies rather than claiming current upstream
maintenance.

Keep i686, PPC64 endianness variants, MIPS64 endianness/ABI variants, RISC-V,
SPARC and LoongArch distinct. An emulator name alone establishes neither a
working libc/sysroot nor LLVM JIT/DBG support. Interpreter-only acceptance must
not be labeled LLVM acceptance. Target-specific lowering and exception handling
must be implemented before claiming that backend on another architecture.

## Execute inside the real task cgroup

All compilers, generators, controllers, native peers, VTune collectors and QEMU
processes must inherit the existing SSH Linux 64GiB/zero-swap cgroup. Verify
actual membership, cgroup inode/identity, boot identity, memory/swap settings,
events and CPU set at start and end. Observe peak memory and owned descendant
cleanup; a process exit is not proof that its descendants are gone.

Use fresh private outputs and authenticated cleanup of this task's own
discardable artifacts if space is low. Preserve raw failures, final evidence,
unrelated data and other agents' mounts/images. Check physical free space before
cross-builds or VM writes. A sparse image's virtual size does not describe the
required physical headroom.

For user-mode QEMU, record the actual loader/library path, emulator ELF, target
interpreter/sysroot and guest ELF class/machine/endian/ABI. Do not change a
global HOME variable to repair a wrapper. Verify the process that owns the real
target Runtime and the native peer PID model.

For system-mode QEMU, preserve existing base disks read-only and use a fresh
owned overlay. Retain the actual block graph, firmware/machine/CPU/memory,
guest OS/architecture, exact transfer hashes and real guest process identity.
A Windows disk found in Documents is not evidence of a bootable installed OS
or an available guest agent. Require actual guest command completion, raw
stdout/stderr, owned process-tree cleanup and clean guest/QEMU shutdown.

## Functional and mixed-application matrix

For each admitted architecture/backend, run the real language artifact through
default denial, positive native delivery, malformed inputs, ownership and
shutdown. Exercise rmw/rcl/rclcpp/rclpy/rcljava migrations with actual pinned
upstream application source and newly developed peers in other languages.
Keep original-library and finite-facade results separate.

Test ROS1, ROS2 and explicitly bound mixed deployments. ROS1 and ROS2 topic
types, QoS, errors and service/Action semantics are not automatically identical.
Retain both native peers' payload/correlation observations and enforce finite
conversion queues. For each vendor interface, qualify its real schema/identity,
native adapter and simulator/device target. Standard JointState/Twist delivery
does not qualify an unrelated proprietary robot command.

Run cache-off, cold-cache and warm-cache variants on the same final binary.
Repeat after changed binary bytes with the same manual Git/version, CPU feature
mask/target changes and malformed/legacy cache entries. Verify that cached
code never carries ROS handles, old generation authority or skipped policy.
Wasm3.0 proposal/type validation and independent WASI permissions still apply.

## Debugger acceptance

Every language needs real stop/continue/step/breakpoint/retirement tests with
the debugger transport actually in use. Verify source line, function, stack,
memory/local inspection where the actual producer supplies it, and reference
invalidation after update/unload. Keep stdout/ROS logs separate from control.
No-source-map output cannot be counted as source-level stepping.

ROS2 adds context: break inside a real topic callback, service handler and
Action path; observe exact payload/request/goal identity, blocked/canceled
work and bounded queues; continue to independent native completion. Exercise
paused-worker stop, policy denial, old handles after trusted update, exception
and cleanup behavior. A generic arithmetic DBG pass is not a ROS2 DBG pass.

State the producer's actual debug level. C DWARF inside CPython is not Python
frame/variable debugging. Mono runtime DWARF is not automatically C# managed
PDB debugging. QuickJS C symbols are not JS source/locals, and a source map is
not typed DWARF. Report the achieved layer and remaining bridge explicitly.

## Matched performance before and after migration

Measure the same useful work: same real schema, bytes, message rate, queue/QoS,
batch size, native endpoint and iteration count. Compare the original native
application with the actual migrated Wasm artifact and with the composed
multi-language application. Also compare Wasmtime, WAVM and WasmEdge where
the engine can execute that same artifact/ABI. Missing ROS imports, GC
proposals or runtime functions mean unsupported for that case, not a fast
result or a substitute workload.

Separate startup/validation/compile, cache lookup/load, steady-state work and
retirement. Include cold and warm code cache, keeping OS page-cache conditions
explicit. Record wall time, CPU time, throughput, latency distribution,
peak memory and failures. Show raw samples and uncertainty before ratios.
Do not declare performance unchanged without an agreed threshold and actual
matched measurements; preserve regressions and investigate their hot paths.

Use actual VTune hardware event sampling for Hotspots and Microarchitecture
Exploration on supported native hardware. Record collector version, sampling
configuration, actual PID/TID/CPU placement, CPU topology/frequency policy,
kernel/perf limits and competing collectors. Retain maps, JIT images and
Wasm/native symbol attribution. Check that required counters really sampled
the target's useful work and that the acquisition/cohort guards passed.
Software timers or a global counter total do not replace the requested PMU
analyses. A collection with environmental drift is retained as a failed
collection and rerun after resolving the cause.

QEMU hardware counters describe host emulator cost. They do not measure the
guest architecture's native pipeline. Label them accordingly and require
native target hardware for target microarchitecture claims. Compare matched
emulated variants only within the same emulator/machine/host setup.

Ordinary Guest instruction lowering can remain unchanged while enabled Host
interfaces add identity/TLS checks. Do not describe every non-threaded ROS or
preload Host call as literal zero overhead. Include representative Host-heavy
calls as well as compute kernels in the final binary's measured workload.

## Publish an evidence-bound result

Each result needs its exact case, architecture/backend/language/producer,
artifact identities, policy/native peer configuration, raw output, assertions,
cleanup and measured collection status. Use separate states for source-ready,
compiled, executed, functional pass, DBG pass and valid performance collection.
A partial matrix remains partial. This guide is an acceptance contract, not
an executed result table.
