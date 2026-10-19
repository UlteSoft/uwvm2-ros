# Distribution and architecture compatibility

The implementation now has a separate explicit native distribution lane at
[tools/ros_distro](../../tools/ros_distro/README.md). Its catalog names all eight
indexed ROS1 and thirteen ROS2 distributions in the official
[ROS index-v4](https://raw.githubusercontent.com/ros/rosdistro/master/index-v4.yaml).
Five additional named historical ROS1 targets come from
[REP3](https://raw.githubusercontent.com/ros-infrastructure/rep/master/rep-0003.rst),
making thirteen ROS1 and thirteen ROS2 targets.
The catalog is a set of compatibility targets. It does not say that all targets
have been built or validated. Existing Noetic/Jazzy receipts retain their exact
original source, platform and resource scope.

ROS1 targets are Box Turtle (`boxturtle`), C Turtle (`cturtle`), Diamondback,
Electric, Fuerte, Groovy, Hydro, Indigo, Jade, Kinetic, Lunar, Melodic and Noetic.
ROS2 targets are Ardent, Bouncy, Crystal, Dashing, Eloquent, Foxy, Galactic,
Humble, Iron, Jazzy, Kilted, Lyrical and Rolling. The five pre-Groovy targets are
absent from index-v4 and each requires a retained source lock plus an actually
configured SDK or explicit include/library discovery. Their original rosbuild
layout is not assumed to provide catkin. Missing APIs remain precise probe or
build failures. Private backports use the separate `legacy` source profile.

Historical ROS1 platform/compiler/Python differences are documented by
[REP3](https://raw.githubusercontent.com/ros-infrastructure/rep/master/rep-0003.rst).
An old SDK can require its Python2 generator and old libstdc++ ABI while the
build driver uses modern host Python and a C++17-capable compiler. Both are
explicit profile inputs. The old distribution's stock compiler need not be
capable of compiling this implementation. A modern toolchain plus old SDK must
still pass real header/library ABI link probes and target runtime checks.

ROS2 platform history is described in
[REP2000](https://raw.githubusercontent.com/ros-infrastructure/rep/master/rep-2000.rst).
That document can lag the current release index. For example, the current
official [Lyrical platform requirements](https://raw.githubusercontent.com/ros2/ros2_documentation/rolling/source/Releases/lyrical/supported-platforms.rst)
specify C++20; Lyrical is included independently of an older REP snapshot.
[Rolling](https://raw.githubusercontent.com/ros2/ros2_documentation/rolling/source/Releases/Release-Rolling-Ridley.rst)
changes in place and always requires a pinned actual SDK and fresh feature
probes. The tools do not guess API availability from version names.

Each profile explicitly selects its SDK/prefix/setup, compiler/sysroot, CMake,
host and generator Python, C++ standard/ABI, RMW, architecture, ELF identity and
runtime. User-provided SDK/sysroot paths and execution inside an already
admitted user container are supported without automatic installation or network
access. OCI/rootfs provisioning descriptors are configuration-only: automatic
daemon launch is refused before actors until original-cgroup containment and
owned-init identity have a separately qualified implementation. EOL archive
availability is recorded as provisioning, never as a passed test. Missing SDKs
and APIs remain unverified or unsupported. The current ROS qualification scope
is the selected upstream-supported x86 and ARM platforms: x86_64, i386/i686,
AArch64, generic Arm, explicit armel/armhf, armeb/armebhf and aarch64_be (also
spelled aarch64be). These are exact selectable ELF identities, not claims that
every ROS release supports every combination. In particular, big-endian ports
require their own actual SDK/source-support evidence, sysroot and loader.
Explicit armel/armhf/armebhf profiles additionally check the actual ARM ELF
soft/hard-float flags, including the selected dynamic loader. The ordinary
Runtime's additional ISA matrix is separate; its RISC-V, LoongArch, PowerPC,
s390x and MIPS selector identities do not establish ROS support.

The ARM calling-convention interpretation follows the official
[AAELF32 ELF-header specification](https://github.com/ARM-software/abi-aa/blob/main/aaelf32/aaelf32.rst#elf-header):
ABI-v5 executables with both float flags clear use the base procedure-call
standard; they are not hard-float executables. Legacy versions cannot silently
qualify an explicitly selected armhf/armel/armebhf lane. The actual Linux
[twenty-case metadata contract](continuation-20261018-evidence/ros-target-ELF-metadata-linux20-r1/integration.json)
checks these distinctions, byte order, machine/class mismatches and truncated
ELF64 headers. Its synthetic metadata files are not runnable SDK fixtures or
QEMU/ROS acceptance results.

The new lane uses the current 64 GiB/zero-swap target, serial `-j1` builds,
bounded command RSS/logs/deadlines, current leaf/boot identity and zero ancestor
OOM deltas. Native and user-QEMU modes are separate. QEMU must use that target's
own sysroot and the exact loader named by the actual ELF, with matching machine,
class and endianness. Cross compilation alone is build coverage.

Reports distinguish configuration, actual feature probes, real Gateway linkage,
and narrowly scoped native SDK topic behavior. The basic fixture is a same
process SDK pub/sub check; it does not qualify Guest communication or the
Gateway's rights boundary. Full independent-native/Guest, default authority,
custom Topics/Services/Actions, Action/QoS, parameters/events, graph, TF,
lifecycle and SROS2 acceptance require their own actual evidence. A successful
configuration or expected denied/unsupported response never substitutes for a
positive feature test.

Distribution selection changes no authority. Every Guest begins with empty ROS
authority and only the established basic standard streams. All extra WASI and
ROS rights remain explicit CLI selections. A compatibility shim may report an
unsupported feature; it must never compensate by granting broader rights or
disabling requested security.

The [new CI workflow](../../.github/workflows/ros-distro-matrix.yml) checks
configuration contracts on pull requests and can execute an administrator's
explicit multi-distribution profile list on a provisioned serial self-hosted
runner. Actual outcomes are per source/profile/SDK/toolchain/architecture/RMW
cell. No new whole-distribution validation result is asserted by this document.
