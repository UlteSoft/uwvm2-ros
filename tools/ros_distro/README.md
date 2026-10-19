# Explicit ROS distribution and architecture lane

The expanded acceptance requirement includes every named ROS1/ROS2 target and
the complete official upstream test suites for the selected, locked source
versions, together with the relevant Unitree projects. This lane's feature
probes and native behavior checks alone do **not** satisfy that requirement.
For every distribution/architecture/RMW cell, retain the official source lock,
upstream test-discovery inventory, actual native test results, disabled and
skipped cases, and separate Wasm/Gateway differential results. A successful
native upstream suite is a baseline, not evidence that Wasm executed that suite.
Missing environments, unexecuted tests and hardware-only cases remain explicit
acceptance gaps. Never aggregate a small policy suite or an unrelated older
binary into an "all distributions" or "all official tests" pass.

This directory adds an independent native distribution lane. The existing
Noetic/Jazzy AArch64 acceptance remains a separately qualified baseline. Merely
listing a distribution, finding an archive, or configuring a project never
establishes compatibility. No tool in this directory grants Guest capabilities,
WASI preopens, clocks, random, networking or environment inheritance. The Guest
still begins with only its explicitly documented basic standard streams.

The target inventory is [distributions.json](distributions.json): the eight ROS1
and thirteen ROS2 distributions in the official
[index-v4](https://raw.githubusercontent.com/ros/rosdistro/master/index-v4.yaml),
including Lyrical and Rolling, plus five official historical ROS1 distributions
named by [REP3](https://raw.githubusercontent.com/ros-infrastructure/rep/master/rep-0003.rst):
Box Turtle (`boxturtle`), C Turtle (`cturtle`), Diamondback, Electric and Fuerte.
There are **thirteen ROS1 and thirteen ROS2 named targets**. Each pre-Groovy
target requires an explicit source lock and an actually configured SDK or the
`EXPLICIT` include/library discovery path. Legacy `manifest.xml` and `stack.xml`
metadata are retained alongside modern `package.xml`. Missing public APIs fail
their real compiler/link probes; those five names do not imply a stock catkin
installation or a tested backport. Private ports still use `distro: "legacy"`,
a nonempty provenance description and an explicit source-lock document.
The catalog is dated metadata and contains **no validated SDK cells**. The
physical index supplied by a profile is pinned and checked again at execution.

## Provision and select an SDK

Copy [example-profile.json](example-profile.json) to a new administrator-owned
file, then replace every path and toolchain selection with real local inputs.
Replace the deliberately non-live cgroup placeholder leaf/device/inode/boot
values with the original admitted cgroup's actual complete identity. Another
64 GiB leaf cannot satisfy those pins. The frozen shared `tests/ros_cgroup/probe.py`
checks visible mount mapping, actual live membership, effective limits and the
qualified controllerless-root exception; this lane does not weaken that probe.
The editor schema is [profile.schema.json](profile.schema.json); `profile.py`
also enforces the cross-field contracts which the structural schema cannot
express. Validation of that JSON reports `CONFIGURATION_ONLY`.

Every profile explicitly specifies:

* Backend and distribution, native target triple, ELF machine/class/endianness.
* SDK inventory roots, ROS prefix and setup script, a retained official index,
  and any source/archive lock. Neither `ROS_DISTRO` nor ambient `PATH` selects
  an SDK. Any exported distribution must agree. For the five locked historical
  source targets only, an absent old `ROS_DISTRO` hook is retained as
  `HISTORICAL_LOCKED_SOURCE_LABEL_NO_SETUP_DISTRO_EXPORT` in a separately pinned
  setup identity document. No fabricated environment value is injected; indexed
  modern distributions still require their exact export. Actual API/build/run
  checks remain mandatory, and this capture does not certify upstream origin.
* Native C++ compiler, CMake, make, shell, modern host Python and a separate ROS
  generator Python. Old ROS1 Python2 generators need not become the host driver.
  Toolchain roots, sysroot and a CMake cross-toolchain file are explicit inputs.
* C++17, C++20 or C++23 and ABI flags. Lyrical's official platform requirements
  specify C++20; actual compile probes still decide whether the selected SDK
  works. The interpreter's C++26 compiler is a separate product.
* RMW, isolated ROS2 domain, or ROS1 numeric loopback Master URI. ROS1's Master
  must be explicitly provisioned before the native behavior stage; this tool
  neither installs nor quietly launches a global Master. OCI ROS1 profiles need
  explicit `network: "host"` when reaching that host loopback Master.
* Current **64 GiB (`68719476736`) memory.max and zero swap**, a memory-bounded compile job cap,
  an aggregate owned-command RSS bound, log bound and per-command deadline.
  A cgroup namespace must expose its actual unified leaf at `/sys/fs/cgroup`.
  Its boot, leaf device/inode, CPUs and every visible ancestor's OOM counters
  must remain unchanged. Historical 48 GiB receipts are retained in their own
  scope and do not satisfy this new lane's resource admission.

Actual execution currently accepts `host` profiles, including execution **inside
an already provisioned user container which has been placed in the required
original cgroup**. A host profile may use an explicit SDK/sysroot from a user
rootfs and an explicit user-QEMU binary. The tool-host platform is explicit and
must match the actual coordinator; container binfmt emulation is not inferred.

The schema also describes explicit `oci` and `rootfs` provisioning inputs. An
OCI descriptor requires `repository@sha256:...`; a user rootfs descriptor
requires its physical path. These two automatic launch modes are deliberately
refused **before any engine or compiler actor** with
`UNSUPPORTED_UNPROVEN_CGROUP_CONTAINMENT`. A Docker/Podman daemon's container is
not a child of its client, and another container's matching memory.max would
not prove membership in the original user cgroup. A future automatic launcher
needs a separate owned cgroup-parent/stopped-init/pidfd admission protocol. No
unsafe engine launch, privileged fallback or silently external container is
implemented. Enter the explicitly admitted container first and use `host` mode.

EOL sources and binary archives require their own verified provisioning. These
tools do not disable package signatures, TLS validation, expiry checks or
repositories' authenticity rules. A user may supply a previously reviewed SDK
without network access. An absent image, unavailable archive, missing API or
failed compile is retained as an actual failure/unverified target, never a
successful skipped test.

## Architectures and user-QEMU

The current ROS platform acceptance scope is x86 and Arm: x86_64, i386/i686,
AArch64, Arm32, explicit `armel`/`armhf`, and the separately named big-endian
`armeb`/`armebhf` and `aarch64_be`/`aarch64be` identities. The ELF reader checks
the actual class, machine and byte order. Explicit Arm32 float-ABI selections
also require EABI5 or later and matching base/hard-float calling conventions;
an endian or ABI label cannot convert a physical SDK. A big-endian selector
does not establish upstream ROS support or an executed QEMU result. Each cell
still requires its actual locked source, target SDK and upstream support evidence.
Older non-x86/Arm identities remain available for explicit private profiles;
ordinary Runtime qualification on those architectures is a separate workstream.

For cross builds, select the real cross compiler/toolchain and that target's
own sysroot. Set `runtime.mode: "qemu"`, the absolute user-QEMU executable and
the exact physical target loader under the sysroot. The actual compiled ELF
class, byte order and machine must match the profile. Its `PT_INTERP` must select
that exact loader; execution uses `[qemu, "-L", sysroot, artifact]`. An AArch64 SDK
cannot become a LoongArch SDK by changing a label. `build-only` never claims a
run, and native mode must agree with the explicit actual tool-host platform.

## Run a fresh source-bound matrix

Freeze the complete implementation. The current freeze helper does not select
the whole `tools/` directory, so explicitly include this new narrow subtree:

```sh
python3 -B tests/ros_acceptance/freeze_source.py \
  --source /absolute/uwvm2-ros --out /var/tmp/ros-distro-source-r1 \
  --extra tools/ros_distro=/absolute/uwvm2-ros/tools/ros_distro
```

Create a closed selection document containing only absolute profile paths:

```json
{"schema_version": 1, "profiles": ["/reviewed/noetic-amd64.json", "/reviewed/lyrical-amd64.json"]}
```

Invoke the tool **from that frozen source**, in the explicitly admitted cgroup:

```sh
FROZEN=/var/tmp/ros-distro-source-r1
python3 -B "$FROZEN/source/tools/ros_distro/matrix.py" \
  --profiles /reviewed/profiles.json \
  --source "$FROZEN/source" \
  --source-manifest "$FROZEN/source-manifest.json" \
  --out /var/tmp/ros-distro-results-r1 \
  --minimum-distributions 2 --require-behavior
```

For a configuration/feature-only investigation, call the frozen `probe.py` with
`--profile`, `--source`, `--source-manifest`, and a fresh `--out`. It performs real
CMake compile/link feature probes, but it does not build or run a Gateway.
`build.py` takes the same four arguments and builds the actual selected Gateway.
All invocations require fresh outputs and atomically retain progress and failures.
The matrix runs profiles in order and never parallelizes containers. Within
one Gateway or native-witness build, `limits.jobs` is an explicit upper bound
from 1 to 64. Existing profiles keep their serial `jobs: 1` behavior. Before
each such build, the driver records a new job plan and limits it by all of:

* Available memory below the original cgroup's **60 GiB working ceiling**,
  retaining its existing four-GiB reserve under the unchanged 64-GiB hard cap.
* Actual host `MemAvailable` minus an additional conservative **8 GiB reserve**.
  Host memory is only an extra restriction, never an alternative resource pool.
* The unchanged aggregate owned-command `limits.rss_bytes` ceiling divided by
  a conservative per-job memory reservation. The default reservation is
  **2 GiB**, or the existing smaller aggregate ceiling. It includes each job's
  share of make, compiler and linker descendants and is an estimate; the
  existing owner continues enforcing the total command-tree RSS limit.
* The intersection of actual coordinator CPU affinity and the admitted cgroup
  cpuset, reduced by every visible finite ancestor CPU quota.

For example, a reviewed profile with `"jobs": 8` and
`"rss_bytes": 4294967296` can use at most two default 2-GiB reservations, even
when 32 CPUs and much more cgroup headroom are available. This does not raise
the aggregate RSS limit. If only one reservation fits, the driver uses `-j1`;
if none fits, it fails before launching the build. A later change in headroom
is still subject to the existing continuous cgroup/RSS checks and owned cleanup.

Two optional controls are captured before sourcing any SDK setup and retained
in `report.json` under `build_parallelism`:

* `UWVM_ROS_BUILD_MAX_JOBS`: a positive decimal cap from 1 to 64. It may reduce
  `limits.jobs` but cannot raise the explicitly reviewed profile maximum.
* `UWVM_ROS_BUILD_JOB_RSS_BYTES`: a positive decimal reservation from 64 MiB to
  the unchanged aggregate command RSS ceiling. Set a larger measured value
  for heavy translation units; this reduces the allowed job count. A smaller
  value needs evidence from that compiler/workload and does not relax the
  aggregate RSS or cgroup guards.

For a profile reviewed to permit eight jobs with a 4-GiB total command budget:

```sh
UWVM_ROS_BUILD_MAX_JOBS=4 UWVM_ROS_BUILD_JOB_RSS_BYTES=2147483648 \
  python3 -B "$FROZEN/source/tools/ros_distro/build.py" \
  --profile /reviewed/jazzy-amd64.json --source "$FROZEN/source" \
  --source-manifest "$FROZEN/source-manifest.json" --out /var/tmp/ros-distro-build-r2
```

The same two controls pass through a matrix invocation. Configuration probes,
SDK acquisition, decompression, behavior actors and individual profiles remain
sequential. Build commands retain the exact selected job count in their argv
and environment; ambient `MAKEFLAGS`, `-j$(nproc)` and setup exports cannot
silently increase it. This policy does not change previously frozen command
contracts or qualify any historical report under a new policy.

The outer Python coordinator has a derived deadline of ten command budgets plus
600 seconds for streaming metadata, and a process-tree ceiling of one command's
RSS budget plus 64 MiB. Individual inner commands retain their original bound.
The example selects the existing native 2 GiB command ceiling; it does not raise
the separate complete-SDK lane's 512 MiB compiler limit.

The Gateway CMake entry emits `uwvm_ros1_features.json` or
`uwvm_ros2_features.json` with `format_version`, `backend`,
`cxx_standard`, and a Boolean `features` object. The consumer checks this exact
four-field shape for either backend. The matrix pins this physical
manifest. A profile's required feature subset must actually be true. No missing
manifest or missing feature is filled from the distribution name. ROS1's actual
`UWVM_ROS1_SELECTED_CXX11_ABI` cache selection (`0`, `1`, or `NOT_LIBSTDCXX`) is
reused by the native basic fixture. Optional official Trigger/SetBool backports
and explicit non-catkin SDK discovery use only the bounded `cmake_options`
allowlist documented by `profile.py`; generated includes stay in the build.

## Read the evidence correctly

| Observation | Reported scope |
| --- | --- |
| Profile/catalog parses | Configuration only; no actor was run |
| Actual selected-SDK feature configure succeeds | `PROBED_BUILD_ONLY` |
| Actual Gateway compile/link, matching ELF and preserved inputs | `BUILD_COVERED` |
| Basic fixture publishes once after match and receives exact String once, then verifies quiet counters | `VALIDATED_NATIVE_SDK_TOPIC_ONLY` |
| Feature missing in actual manifest | `UNSUPPORTED_REQUIRED_FEATURES` with failed selected cell |
| Target not selected or behavior not requested/executable | `UNVERIFIED` scope |

The native fixture uses one process: ROS1 pub/sub handles share its node; ROS2
creates two nodes. It is useful native SDK/RMW behavior evidence, **not** an
independent external native peer, real Wasm communication or Gateway policy
test. The full original Guest/native, default-denied, custom-type, resource,
Action/QoS, TF, lifecycle and Enforce suites remain required for those claims.
Neither expected error13 nor Unsupported95 counts as positive ROS functionality.

SDK and tool roots are streamed to before/after JSONL inventories, with complete
byte digests, physical timestamps/inodes and bounded regular-file handling.
Directory symlink targets must stay within explicitly inventoried roots; select
the additional physical root when a package legitimately aliases sibling
headers. Regular file symlink targets are independently opened and pinned even
outside a root (for example GCC's sibling libstdc++ library); special files are
refused. Physical compiler binaries, setup,
index, locks, QEMU/loader and provider inputs are independently pinned. Actual
verbose build logs, compile databases, retained dependency files and their
opened headers are kept. This records the real retained dependency closure; it
does not manufacture a unique full-MD invocation or complete external toolchain
and runtime-library closure. Those scopes remain explicitly unqualified.

`tests/ros_distro/test_profiles.py` provides pure configuration/argv contracts.
The new CI workflow runs them on pull requests and performs **real** serial
Gateway builds/native SDK topic runs only on a manually selected, provisioned
self-hosted Linux runner carrying at least two explicit host profiles. That
runner can itself be the user's explicitly admitted container. Its evidence
artifacts retain successes and failures; the workflow cannot auto-promote all26
targets from a successful two-profile job.
