# Language SDK platform qualification checkpoint

This is an upstream-target and local-tool availability record dated 2026-10-19.
It adds no QEMU, native ROS or performance pass. Current language/runtime
[actual results](../../test/0050.high_level_languages/results/native-budget/language-runtime-control-edges-20261019/README.md)
cover the frozen Linux x86_64 interpreter products described there.

## Upstream targets checked

The official [REP 2000 source](https://github.com/ros-infrastructure/rep/blob/master/rep-2000.rst)
defines support per OS, architecture and middleware combination. Its Humble and
Jazzy tables include amd64 and arm64 as Tier 1 on their selected Ubuntu LTS,
and arm32 as Tier 3. Therefore the requested ROS qualification matrix must also
include 32-bit ARM; x86_64 and aarch64 alone do not cover those tables.
This observation concerns these named distributions, not every ROS release.

The official [REP 3 source](https://github.com/ros-infrastructure/rep/blob/master/rep-0003.rst)
lists amd64, arm32 and arm64 for Noetic and dates its support window through
May 2025. A reproducible Noetic compatibility lane must retain that legacy
platform identity separately from current ROS 2 qualification.

| Qualification lane | Required coverage | Current receipt in this continuation |
| --- | --- | --- |
| ROS Linux x86_64 | Selected producer, full product, typed import/layout checks, enabled policy, native peer | Product and default-disabled MoonBit baseline only |
| ROS Linux aarch64 | Same language/artifact/native checks on target product | Pending |
| ROS Linux arm32 | Explicit ABI/width/dependency/RMW selection and the same checks | Pending |
| Additional requested Linux ISAs | ppc64/ppc64le, mips64, riscv64, sparc64, loongarch64, i686 | Pending UWVM portability lanes; no upstream ROS tier asserted |
| Ordinary/stable Windows | Existing Documents disk, owned disposable guest output, actual language artifacts | Pending |

The REP web pages returned an access-denial page during this lookup; the
upstream ros-infrastructure repository supplied the source. These links are
mutable references and do not authenticate a particular source snapshot.

## Existing Linux tools and retained images

The data-only [availability inventory](../../test/0050.high_level_languages/results/native-budget/language-runtime-control-edges-20261019/resources/platform-availability-20261019.json)
records selected physical executable paths, resolved files, extents and SHA256.
QEMU user-mode binaries are available under `/home/macromodel/.local/bin`,
including aarch64, arm, ppc64, mips64, riscv64, sparc64, loongarch64 and i386.
The initial noninteractive PATH lookup omitted that directory; this is not a
missing-tool finding. System-mode names resolve through `qemu-local-run` shell
wrappers. Their wrapper/loader/module/firmware closure still needs explicit
admission before execution. An executable's name or presence is no guest test.

Existing Windows/FreeBSD image directories were observed in Linux Documents;
none was opened, mounted or modified by this inventory. VTune directories
`2026.4` and `latest` were observed. No VTune command, hardware-counter sample,
Hotspots collection or Microarchitecture collection was launched here.

## Actual acceptance required on each target

Use the original existing 64 GiB/swap-zero cgroup and an owned bounded process
tree for the emulator, compiler and all helpers. Preserve the original disk;
place each test's guest changes in an owned disposable overlay. Retain the
selected tool/loader/sysroot, firmware/disk and target UWVM hashes, exact build
commands, resolved per-language numeric macros, complete output and ownership
closure. Successful source compilation does not replace target execution.

Run the same source-bound language expectations and control-end assertions on
the actual target product. Recheck pointer/size conversions, full-width u64
handles, little-endian canonical wire values, GC/linear-memory separation,
WASI arguments/environment and producer-specific imports. For ROS, complete
connection policy and authenticated Gateway preflight precede enabled zero-grant
and native positive/negative cases. A peer must check complete values and denied
side effects for every nonempty Topic/Service/Action selection.

Keep native before/after VTune hardware-counter, Hotspots and Microarchitecture
reports separate from emulator timings. Identify the exact native or emulated
workload and product in each receipt. This checkpoint supplies no cross-ISA
performance ratio, official client-library parity or sandbox certification.
