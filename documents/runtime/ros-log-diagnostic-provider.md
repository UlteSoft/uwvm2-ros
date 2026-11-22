# Private logs and diagnostic observations

`log_store.h` implements the bounded private storage component for `log.emit`.
`diagnostic_snapshot.h` implements immutable pages of cached numeric counters.
The central broker, CLI, carrier tuple and detached debugger integration are
implemented. A local-only retirement regression exposed
that rechecking authority alone did not fence a directly retired provider.
`log-diagnostic-source-retirement` adds local source/binding checks before effects,
before output and after delivery. Seven dispatch groups pass ASan/UBSan, GCC
and TSan. The corrected full VM has now passed 111 actual log/diagnostic
Wasm/CLI cases, three live DAP sessions, both isolated module modes and actual
hostile memory/FP callbacks. Its broader registered-provider regression set
contains 959 actual VM/DAP cases.

The corrected frozen VM is SHA256
`468e1bcf9eeea2ba57af01a40d78147a52506bc5b8afdc2cd7f6a99f22cb9254`.
Its three actual translation-unit commands contain effective `-O3 -g` and its
binary retains full debug information. SDK `.tmp/ros-scene-security` contains
`log-retirement-qualification.json`, with 26 separately hashed receipts and a
3,457-file immutable source manifest. Intermediate cohorts, the reboot-interrupted
build, failing fixtures and negative controls remain preserved as history.

A log binding starts with no owner, no approved logger names, no enabled
severities and zero capacity. Trusted setup selects exact logger names, severity
bits, record count, complete encoded record size and total reserved storage.
The reservation covers ring metadata, ring records, one exact replay request
and one detached export copy. It excludes allocator metadata and process RSS.
Allocation uses the recoverable fast_io wrappers. Setup failure publishes no
partial stream; emit, replay, inspection and the component's drain path allocate
no memory after setup.

The record preserves a literal UTF-8 message, logger, severity, optional event
time and optional source file/function/line. This preserves useful ROS logging
information from the official [Log message](https://github.com/ros2/rcl_interfaces/blob/rolling/rcl_interfaces/msg/Log.msg)
and [rcutils logging API](https://github.com/ros2/rcutils/blob/rolling/include/rcutils/logging.h).
Format directives and terminal control characters remain opaque content.
No format string executes, no supplied file path is opened, and no record is
automatically written to stdout, disk, syslog, ROS transport or a model prompt.
Renderers/exporters must use literal output and escape controls for their
destination. Guest names, locations and event times are claims; they cannot
authorize a physical action or substitute for a security deadline.

Full storage rejects by default without consuming the request sequence.
`drop_oldest` is an explicitly selected overflow policy. Its receipt exposes
the exact cumulative loss count. A record reserved for export cannot be evicted,
so a full ring rejects during that export. The last accepted request and receipt
survive export/eviction for exact retry; changed bytes at the same key conflict.
The next accepted sequence acknowledges the previous receipt. This provides
volatile local deduplication, not crash recovery or remote exactly-once delivery.

Export is a trusted adapter operation with independent destination and content
authority. Its callback executes outside the store lock against reserved scratch.
Failed or throwing copies leave the record available. Revocation during the
callback prevents final consumption; bytes already observed by a sink cannot
be withdrawn. An external appender must retain its own outcome/acknowledgement
protocol and report uncertainty instead of silently replaying an external effect.

Diagnostic bindings select one exact source scope, a closed counter mask,
maximum row/page counts, storage and host-steady capture/cursor lifetimes.
Counters have explicit availability and saturation fields. An unavailable
counter is distinct from a measured zero. Source-specific schemas define the
counter codes and units; arbitrary text and payload inspection are absent.
The broker must recheck both `diagnostic.read` and source observation authority
for every page and retry. Numeric scope/cursor values grant no authority.

A snapshot captures cached counters once, validates the closed mask and unique
codes, and sorts them into stable pages. It never polls devices, expires jobs,
advances clocks or executes controls. Failed output preserves the exact page;
the next returned cursor acknowledges the preceding page. Cursors bind the
owner, configured scope and revision, and never extend their original expiry.
Debug inspection returns cached counts without enforcing elapsed deadlines.
Revocation fences capture and delivery. ROS diagnostic messages with arbitrary
keys or human text remain separately authorized communication payloads; these
metadata counters do not pretend to encode all such application messages.

## Qualification evidence

The earlier `log-diagnostic-source-central` snapshot contains 3,457 pinned
production files. `log-store-frozen`, `diagnostic-snapshot-frozen`,
`log-diagnostic-dispatch-frozen` and `log-diagnostic-configuration-frozen`
each record nine passing Clang ASan/UBSan, GCC 13 and TSan commands. Their
tests include real recoverable fast_io allocation failures, no hot-path store
allocation, exact lost-copy recovery, source/primary revocation inside trusted
callbacks, finite capacity, explicit loss and process-incarnation rejection.
`log-diagnostic-deployment-first` adds nine passing commands for complete
configuration, missing bounds, atomic policy freezing, compound authority and
inspection without expiry/capture. These are bounded fixture results, not an
industrial safety certification or a claim about every native ROS logger.

`log-diagnostic-host-first` compiles and runs the actual host carriers under
ASan/UBSan with controlled identity/memory APIs. It checks preflight before
guest reads/allocation, real fast_io large-allocation failure, partial failed
reads, alias rejection, failed output recovery and diagnostic page retention.
`log-diagnostic-debug-second` records eight formatter/DAP/editor-controller
checks. Debug metadata uses try-locks throughout; contention withholds a page.
It does not open a graphical IDE or read payloads. The LLVM isolated module
cohort adds 22 passing commands, including full-FP-guard assertions for both
new entries. The final retirement cohort separately repeats both LLVM and
interpreter module modes (22 commands each), actual host callbacks and real Wasm
qualification; earlier core receipts remain tied to their own source hashes.

`log-native-semantics-second` records six successful commands against the actual
retained ROS Jazzy `librcutils.so`, using Clang ASan/UBSan and GCC 13 at `-O3 -g`.
All five native severity levels, literal `%n`/`%s`/braces and UTF-8 content,
logger names, source file/function/line and captured timestamps survive a
canonical store/drain roundtrip. A fixture-only fixed output handler replaces
the native appender, and rcutils public allocations use a finite fast_io
allocator. Parent-level native filtering is checked separately; the VM uses
explicit exact logger names and severity masks instead of implicit descendants.
No native appender is thereby granted to guests, and this test makes no native
middleware allocator, logging throughput or remote delivery claim.

The final frozen VM's XML/cache comparison measured native 26.643 microseconds
and Wasm 79.661 microseconds per fourteen-document cycle (2.9899x). Seven to nine
other compilers were active on other cores, and system-load/cgroup observations
are retained. This is not an idle-system comparison or evidence of a stable
change relative to older boots. Four official VTune collections produced
qualified host source joins, 44 assembly artifacts and instruction/event maps
for five host functions. Unregistered JIT PCs account for 53.1489% of Hotspots
CPU samples. Implausible uarch CPU-time-derived frequency/utilization values
are excluded from conclusions. The result does not establish near-native ROS
performance. Shared cgroup memory-pressure counts are retained per receipt;
OOM and OOM-kill counts stayed zero.

## ARM32 allocation-failure recovery

Configuration portability must exercise throwing allocations with the actual
optimized ABI. A retained ARMv7 hard-float run found that passing the complete
logger allowlist by value created a 16,680-byte outgoing argument area. On the
first injected allocation failure, the exception landing pad retained that
stack adjustment and read invalid spilled locals. GDB recorded the fixed-frame
SP as `0x407f1e98` and the catch SP as `0x407edd70`. The generated call sequence
and unwind tables are retained in `configuration-arm-unwind-proof`.

Changing to a different exception unwinder reproduced the failure. Retaining a
frame pointer made that particular test pass, but is not the implemented fix.
`bind_log` now borrows a stable trusted plan by const reference for the duration
of the call; the binding and store retain owned policy copies. Diagnostics use
the same borrowing rule. No caller storage survives as a provider reference,
and no alias or storage reservation is published by a failed allocation.

`configuration-portable-owned/results.json` records twenty successful compile
and execution commands for log/diagnostic and recording configuration. AArch64
and ARMv7 hard-float execute under their recorded QEMU loaders; i686 executes
with its target loader. Native Clang ASan/UBSan and GCC 13 also pass. All builds
use `-O3 -g`; the ARMv7 fix passes with the original compiler flags and no added
frame-pointer option. This is provider-core ABI evidence, not full native ARM
VM or hardware timing qualification.

`log-diagnostic-owned-policy/results.json` additionally checks the real
dispatcher with Clang ASan/UBSan, GCC 13 and TSan. Its fixture modifies and then
destroys the caller's plan after binding. Emission and diagnostic reads must
still enforce the original logger, capacity, scope and counter selection. The
retained failing runs remain distinct from the passing receipts; this change
does not qualify arbitrary large by-value exception paths in the toolchain.
