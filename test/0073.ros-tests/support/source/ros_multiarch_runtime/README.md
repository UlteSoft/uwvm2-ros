# Actual multiarchitecture Runtime and independent ROS endpoints

This is a new, source-only test lane. It has not been compiled or executed.
It complements `tools/ros_distro`: a Gateway SDK build cannot demonstrate that
an interpreter for another architecture executed real Wasm imports. Existing
AArch64-specific admission functions and historical receipts are unchanged.

The Runtime choices are x86_64 (ELF62/64/little), AArch64 (183/64/little), Arm
(40/32/little), i386 (3/32/little), RISC-V64 (243/64/little), LoongArch64
(258/64/little), PPC64LE (21/64/little), and s390x (22/64/big). The actual bytes,
ELF class, endianness, target loader and opened live mapping descriptors decide the
architecture. A profile label does not. This implementation runs on a Linux
ELF62 host, with explicit host ELF62 user-QEMU for every other target.

The independent ROS Gateway and native peers are separately selected as ELF62
or ELF183. For example a real LoongArch interpreter under qemu-loongarch64 can
call an independently prepared Jazzy AArch64 Gateway under qemu-aarch64 through
authenticated local IPC. This is **LoongArch Runtime ROS support against an
AArch64 ROS endpoint**, not LoongArch native DDS or a LoongArch ROS binary SDK.
Reports preserve both architecture identities and each role's actual maps.

## Closed behavior inventory

Each single-backend profile requires all thirteen cases:

* Six languages (C, C++, Rust, TinyGo, AssemblyScript, JavaScript), each with a
  positive exact String exchange and a separate empty-capability-table case.
  Current prepared Guest signatures/full validation/tool package inputs are
  reopened through the existing `run_delivery.compiled_guest` admission path.
  Startup rights are copied only from its explicitly reviewed artifact record;
  none is inferred from a language or a failed attempt. The actual target Wasm
  relay retains JS ROS authority; host Node is separately identified.
* One real Wasm exchange for all seven event kinds: Topic message, incoming
  Service request, asynchronous Service response, Action feedback, Action
  result, correlated resource error, and resource closed. It reuses the exact
  existing policy/WAT and independent native seven-event fixture. In particular
  feedback delivery precedes the independent result gate, missing-Service
  timeout cannot become a native request, and final native effects are exactly
  one each. The existing event Guest checks versioned envelopes, owned
  correlation, too-small90 retention, memory1 and memory0 sentinels.

Positive String admission requires actual publisher/subscriber matching and
source identity. Final delivery and reply each count exactly one; warmup0..1
never counts as final delivery. Empty grants must yield13 and zero Gateway
Topic endpoints/effects. Both modes require the independent native peer's
at-least2second quiet interval and at least30samples, zero duplicates/foreign
effects, exact Guest completion and actual Worker exit0.

The complete matrix is sixteen explicit cells (eight Runtime architectures,
both ROS backends),208 actual cases. Missing SDK/compiler/QEMU/profile/Guest
inputs are BLOCKED, not passed or replaced by a previously successful source.
No default SDK, distribution, rootfs, source lock or target loader is inferred.
The current native fixtures retain their Noetic/Jazzy identity protocol and
strict modern native hash/QoS observations. Another native SDK must actually
compile and satisfy that protocol; absence is not all-distribution acceptance.

This initial architecture lane covers pub/sub, incoming/outgoing Services,
Action client feedback/result, all seven unified events and empty-grant native
effects. **Parameter, TF, graph, SROS2, dynamic-loader and the full original
Runtime regression families are not claimed by these208 cases.** Their existing
independent source-bound acceptance remains mandatory; merely running a native
parameter/TF/graph probe next to a foreign Worker would not prove that Worker
caused the effects. Additional architecture-specific Guest/native oracle pairs
must be implemented and reviewed before extending this closed inventory.

## Explicit provision and execution

Copy `example-profile.json` to an administrator-owned path and replace every
placeholder. It illustrates LoongArch Runtime and independent AArch64 native
ROS2. Set the actual original cgroup boot/leaf/device/inode/CPU identity. Both
the root and all visible ancestors must meet the current64GiB/swap-zero policy;
`memory.current` must leave4GiB headroom (60GiB ceiling). A different64GiB leaf
does not satisfy the profile. No driver changes cgroups, starts a container,
registers binfmt, downloads a package, installs a tool or reaches SSH.

`roots` names complete explicitly selected SDK/compiler/stdlib/QEMU/Node/Guest
tool packages. They are streamed to before/after JSONL catalogs, including
symlink identities and opened regular targets. Compiler-only loader variables
are applied to compiler commands, not inherited by the target Worker. Native
library/package variables are explicit and separate from Runtime variables.
Host OS module/header/link closure is not invented: full original system MD and
selected package catalogs are recorded, while DT_NEEDED/link closure remains
unqualified. The supplied source lock records where those SDKs came from; it
does not itself certify upstream authenticity.

Use the actual frozen source and its complete manifest. Include
`tools/ros_distro` with `freeze_source.py --extra` as documented by that lane,
because its already reviewed catalog/cgroup/pidfd primitives are shared. Invoke:

```sh
/absolute/host/python3 -B "$S/test/0073.ros-tests/support/source/ros_multiarch_runtime/prepare.py" \
  --source "$S" --manifest "$MF" --profile /reviewed/profile.json \
  --out /owned/tmp/new-profile-prepare
/absolute/host/python3 -B "$S/test/0073.ros-tests/support/source/ros_multiarch_runtime/run.py" \
  --source "$S" --manifest "$MF" --profile /reviewed/profile.json \
  --prepared /owned/tmp/new-profile-prepare/report.json \
  --out /owned/tmp/new-profile-execute
```

The compiler command is explicit. The Runtime uses the current real
`main.default.cpp`, `uwvm_runtime.default.cpp`, and `host_api.default.cpp`, with
the C++26 interpreter/dual-backend flags, JIT disabled and full system `-MD`.
Every compiled object and final executable must have the selected target
architecture. `stop.h` is injected only into its actual main translation unit.
The independent native CMake build targets the actual production Gateway and
the existing SDK/seven-event peers with their original assertions enabled.
Native `-D` flags must select the real compiler/toolchain/SDK and its ABI. A
cross native compiler is independent of the Runtime compiler; a misleading
target label cannot make its wrong ELF output admissible.

Both ROS1 peers consume the Gateway's actual `uwvm-ros1-sdk` interface, including
its compile/link-admitted headers, libraries and string ABI. A rootfs whose
catkin exports contain host-absolute paths can use `native_cmake_flags` to select
`UWVM_ROS1_DISCOVERY=EXPLICIT`, with ordered, absolute physical SDK include and
library lists. The existing Gateway path rejects missing inputs and still runs
all required feature/ABI link probes; this selection does not qualify an SDK or
grant a Guest operation. Host cross-toolchain and configuration-Python inputs
must also be explicit. A target rootfs alone is not a host-cross SDK binding.

Builds are serial. Runtime compiler/linker RSS is at most 8 GiB, Worker RSS
8 GiB and each native role subtree 2 GiB. After the Worker link, preparation
restores the selected native RSS cap (at most 2 GiB) before native CMake
configuration, feature probes or builds. Compiler flags and optimization remain
unchanged, with finite explicit per-command/actor deadlines and 16-MiB log
caps. The matrix's extra256MiB coordinator bound does not raise child leaf
limits. Root must review actual available disk space and its outer lease before
launching all sixteen complete profiles. Raw source/compile/MD/ELF/log/input
receipts are retained, including partial failures. There is no automatic retry.

All target roles stop in the real kernel before main/ROS initialization. The
new generic actor guard opens a pidfd for a proved child birth, checks original
cgroup membership, matches `/proc/PID/exe` to exact target or host-QEMU bytes,
and opens actual `map_files` descriptors for the exact target. The shared
frozen `test/0073.ros-tests/support/source/ros_aarch64/runtime.py` helper is admitted from its opened
manifest-bound bytes before loading. Raw maps device/inode remains display
data. Actual FD physical facts, full SHA256 and ELF architecture must match;
missing access fails closed. A unique already-owned stopped descendant may
be selected when a launcher waits for its child. Birth, T state, cgroup and
host executable are checked again immediately before the selected pidfd CONT. SIGCONT/cleanup signals use only
that pidfd. A first unproven descendant is not signalled; a remaining live
unproven birth produces CLEANUP_INCOMPLETE. The Worker stays stopped while its
Gateway is configured for that actual kernel PID. The owned ROS1 Master is an
explicit auxiliary Python/script command with XMLRPC PID/birth validation;
it is not mislabelled as a pre-main native endpoint proof. Existing private
Master/domain leases are shared; a foreign daemon occupying the port blocks.

`matrix.py` accepts a closed selection `{schema_version:1,cgroup:{...},profiles:
[{architecture:"loongarch64",backend:2,profile:"/absolute/profile.json"},...]}`.
All sixteen unique cells are required. It builds and executes each in order,
reopens current receipts and records PASS/FAIL/BLOCKED separately. Its final
PASS requires all208 actual cases, current source/input preservation and
checked cleanup; compiler PREPARED cannot supply behavior PASS. The fixed
numeric acceptance crosswalk of the original task is not changed by this new
supplemental scope.
