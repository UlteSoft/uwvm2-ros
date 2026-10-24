# Linux

## Prerequisites
- Use the latest available Linux kernel headers for your target toolchain before building.
- Install toolchains and xmake using your distro's package manager:
  - Debian/Ubuntu: `sudo apt install build-essential gcc g++ llvm clang xmake`
  - Fedora/RHEL: `sudo dnf install gcc gcc-c++ llvm clang xmake`
  - Arch: `sudo pacman -S base-devel gcc llvm clang xmake`

## Examples
```shell
# GCC toolchain
xmake f -m release
xmake

# LLVM/Clang toolchain
xmake f -m release --use-llvm-compiler=y
xmake

# Install
sudo xmake i -o /usr/local
```

## Use GCC
1. Install [[xmake]](https://github.com/xmake-io/xmake/)
2. Install [[GCC]](git://gcc.gnu.org/git/gcc.git)
3. Build
```shell
$ xmake f -m [debug|release|releasedbg|minsizerel]
$ xmake
```
4. Install UWVM2
```shell
$ xmake i -o <install_path>
```

### Additional Options
1. `--static=none|non-system|compiler` Static linking policy (`compiler` uses global `-static` where supported)
2. `--march` defaults to `none` (the configured toolchain baseline). Release artifacts must select and test a fixed baseline explicitly; `--march=native` is only for developer-local builds.
3. `--use-cxx-module=y` Use cpp module to compile, compiler may not be supported

## Use LLVM
1. Install [[xmake]](https://github.com/xmake-io/xmake/)
2. Install [[llvm]](https://github.com/llvm/llvm-project/releases)
3. Build
```shell
$ xmake f -m [debug|release|releasedbg|minsizerel] --use-llvm-compiler=y
$ xmake
```
4. Install UWVM2
```shell
$ xmake i -o <install_path>
```

### Additional Options
1. `--static=none|non-system|compiler` Static linking policy (`compiler` uses global `-static` where supported)
2. `--march` defaults to `none` (the configured toolchain baseline). Release artifacts must select and test a fixed baseline explicitly; `--march=native` is only for developer-local builds.
3. `--use-cxx-module=y` Use cpp module to compile, compiler may not be supported

## Caveat
1. Add `--use-llvm-compiler` when you want to build with the LLVM/Clang compiler toolchain. This only selects the compiler toolchain and does not enable the LLVM AOT backend by itself.
2. On some platforms such as the latest Android which only provide LLVM/Clang, you must also add `--use-llvm-compiler`, same as note 1.
3. `--static=compiler` may have no effect on some platforms, such as Android. Use `--static=non-system` for release builds that should statically link non-platform dependencies without requesting full system static linking.

## ROS build configuration

Run the commands below from the repository root. Their option names come from
[`xmake.lua`](../../xmake.lua), [`xmake/option.lua`](../../xmake/option.lua) and
[`xmake/ros_vendors.lua`](../../xmake/ros_vendors.lua). The Runtime executable
target is `uwvm2-ros`; `uwvm_runtime` is its internal object target.

There are three independent build products:

| Product | Build entry | Native ROS SDK needed to build? |
| --- | --- | --- |
| Linux Runtime and its ROS host imports | Repository-root Xmake project | No; middleware stays in the Gateway process |
| ROS1 or ROS2 Gateway | Matching native Gateway CMake project | Yes; use the chosen distribution's headers, libraries and generated interfaces |
| Guest application, normally a `.wasm` | Each SDK example's `xmake.lua` | Not for the Wasm build; execution needs the deployed Runtime and Gateway |

Both `--import-ros1` and `--import-ros2` default to `n`. Enabling either compiles
the corresponding import implementation. It does not start a ROS context,
select a distribution, grant a capability or build a Gateway. The Runtime uses
Linux AF_UNIX sockets and `SO_PEERCRED`; native macOS and Windows Runtime
targets explicitly reject these switches.

### ROS1, ROS2 and combined Runtime builds

These interpreter configurations make the execution backend explicit and avoid
requiring the separate LLVM AOT dependency. A compiler with the project's C++26
support is still required. `--use-llvm-compiler=y` selects the bootstrap Clang
toolchain; it does not itself enable the LLVM execution backend.

```sh
# ROS1 imports only.
xmake f -y -c -m release -p linux -a x86_64 -o build/ros1 \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=y --import-ros2=n
xmake -j1 uwvm2-ros

# ROS2 imports only, in a separate output directory.
xmake f -y -c -m release -p linux -a x86_64 -o build/ros2 \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=n --import-ros2=y
xmake -j1 uwvm2-ros

# Both import namespaces in the same Runtime.
xmake f -y -c -m release -p linux -a x86_64 -o build/ros-dual \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=y --import-ros2=y
xmake -j1 uwvm2-ros
```

The combined build still uses independently configured ROS1 and ROS2 Gateway
connections. It does not translate ROS1 traffic into ROS2 traffic. ROS1/ROS2
distribution selection belongs to the native Gateway and the SDK application,
not to a root-project option named `ros1_version` or `ros2_version`.

For an LLVM execution build, use the following configuration instead:

```sh
xmake f -y -c -m release -p linux -a x86_64 -o build/ros2-llvm \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=llvm \
  --llvm-build-jobs=1 --llvm-build-targets=Native \
  --import-ros1=n --import-ros2=y
xmake -j1 uwvm2-ros
```

`--execution-jit=llvm` is the retained build-option spelling for the full-module
LLVM AOT backend. Its pinned LLVM dependency is selected by
[`bundled_llvm.lua`](../../xmake/utility/bundled_llvm.lua); an ambient
`llvm-config` does not replace it. `--llvm-build-jobs` limits dependency build
parallelism, not memory. Named C++ modules are a separate
`--use-cxx-module=y` choice and require a compatible compiler/module toolchain;
the examples use the header route explicitly.

### Vendor and ecosystem selections

Every vendor and ecosystem selection defaults to `n`. For a vendor, enable
`--ros-vendor-<name>=y` and provide its existing
`--ros-vendor-<name>-registry-header=/absolute/manifest.hpp`. The header is
reviewed generated C++ schema source, not a YAML launch file, firmware version,
runtime policy or installed vendor driver.

| Vendor option suffix | Vendor option suffix | Vendor option suffix | Vendor option suffix |
| --- | --- | --- | --- |
| `abb` | `agibot` | `agilex` | `booster` |
| `boston-dynamics` | `clearpath` | `deep-robotics` | `dobot` |
| `doosan` | `elephant` | `fanuc` | `flexiv` |
| `fourier` | `franka` | `hello-robot` | `kinova` |
| `kuka` | `leju` | `limx` | `pal` |
| `realman` | `robotis` | `trossen` | `ubtech` |
| `ufactory` | `unitree` | `universal-robots` | `yaskawa` |

SDK directory identifiers use underscores in names such as `deep_robotics`,
`boston_dynamics`, `hello_robot` and `universal_robots`; the Runtime switches
above use hyphens.

For example, after generating and reviewing an ABB registry:

```sh
REGISTRY=/absolute/reviewed/abb/manifest.hpp
xmake f -y -c -m release -p linux -a x86_64 -o build/ros2-abb \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=n --import-ros2=y \
  --ros-vendor-abb=y \
  --ros-vendor-abb-registry-header="$REGISTRY"
xmake -j1 uwvm2-ros
```

Several selections must name the **same absolute merged manifest path**. The
build compares normalized paths; two different paths with equal bytes are not
a replacement for one deliberately merged catalog. Regenerate and review a
deduplicated catalog before combining vendors. An illustrative FANUC/KUKA
selection is:

```sh
REGISTRY=/absolute/reviewed/fanuc-kuka/manifest.hpp
xmake f -y -c -m release -p linux -a x86_64 -o build/ros2-fanuc-kuka \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=n --import-ros2=y \
  --ros-vendor-fanuc=y --ros-vendor-fanuc-registry-header="$REGISTRY" \
  --ros-vendor-kuka=y --ros-vendor-kuka-registry-header="$REGISTRY" \
  --ros-registry-max-interfaces=256 --ros-registry-max-cells=32768
xmake -j1 uwvm2-ros
```

Only explicitly enabled selections are compiled. If an old Xmake configuration
enabled another vendor, explicitly turn its switch off and reset its header to
`none`; selecting two vendors does not automatically disable earlier choices.

| Additional root option | Meaning and required configuration |
| --- | --- |
| `--ros-custom-registry-header=<path>` | Select a reviewed common registry directly; requires at least one ROS import. If vendor switches are also enabled, their registry path must agree. Default: `none`. |
| `--ros-original-control-manifest-header=<path>` | Add the separate original ROS2 control schema; requires `--import-ros2=y`. It does not replace the vendor registry. Default: `none`. |
| `--ros-registry-max-interfaces=<integer>` | Compiled Topic/Service/Action interface ceiling, `1..512`, default `128`. |
| `--ros-registry-max-cells=<integer>` | Aggregate compiled schema-cell ceiling, `1..1048576`, default `16384`. Runtime byte, node, depth and capability limits remain separate. |
| `--ros-platform-isaac-ros=y` | Select reviewed Isaac ROS schema source; requires ROS2 and `--ros-platform-isaac-ros-registry-header=<path>`. |
| `--ros-platform-isaac-sim=y` | Select reviewed Isaac Sim schema source; requires ROS2 and `--ros-platform-isaac-sim-registry-header=<path>`. |
| `--ros-platform-groot=y` | Select reviewed GR00T bridge schema source; requires ROS2 and `--ros-platform-groot-registry-header=<path>`. |
| `--ros-platform-jetson=y` | Select the Linux ARM64 deployment profile; requires ROS2 and an `arm64` target. It adds no CUDA, JetPack or native robot SDK. |

Registry and native-provider headers must be generated from the same reviewed
interface definitions and identities. Compiling a vendor selection alone does
not prove that its native driver, controller, model or firmware is supported.

### Build the independent native Gateways

Native Gateway sources are maintained in the separate
[SDK repository](https://github.com/UlteSoft/uwvm2-ros-sdk). Clone it with
Git LFS and set `UWVM_ROS_SDK_ROOT` to that checkout before these commands.

Use separate shells and build directories for different ROS distributions.
The examples below assume those development SDKs and their interface packages
have already been provisioned; changing a setup-script path does not establish
compatibility with another distribution.

```sh
# In a ROS1 development environment, for example a provisioned Noetic SDK.
source /opt/ros/noetic/setup.bash
cmake -S "$UWVM_ROS_SDK_ROOT/src/platform/ros1/native_gateway" -B build/gateway-ros1 \
  -DCMAKE_BUILD_TYPE=Release -DUWVM_ROS1_CXX_STANDARD=17 \
  -DUWVM_ROS1_BUILD_PROBES=OFF
cmake --build build/gateway-ros1 --target uwvm-ros1-gateway --parallel 1
```

```sh
# In a separate ROS2 development environment, for example a provisioned Jazzy SDK.
source /opt/ros/jazzy/setup.bash
cmake -S "$UWVM_ROS_SDK_ROOT/src/gateway/ros2" -B build/gateway-ros2 \
  -DCMAKE_BUILD_TYPE=Release -DUWVM_ROS2_CXX_STANDARD=AUTO \
  -DUWVM_ROS2_BUILD_PROBES=OFF
cmake --build build/gateway-ros2 --target uwvm-ros2-gateway --parallel 1
```

The ROS1 project discovers catkin by default and also has an `EXPLICIT`
include/library discovery mode. ROS2 probes the selected SDK's real compile/link
interfaces. Their native compiler/standard-library ABI is independent of the
Runtime's C++26 compiler. Building only `uwvm2-ros` does not produce either
Gateway target.

For custom interfaces, the corresponding Gateway CMake configuration additionally
requires all three paired inputs:

| Backend | Common registry | Native provider | Explicit package list |
| --- | --- | --- | --- |
| ROS1 | `UWVM_ROS_CUSTOM_REGISTRY_HEADER` | `UWVM_ROS_CUSTOM_ROS1_HEADER` | `UWVM_ROS_CUSTOM_ROS1_PACKAGES` |
| ROS2 | `UWVM_ROS_CUSTOM_REGISTRY_HEADER` | `UWVM_ROS_CUSTOM_ROS2_HEADER` | `UWVM_ROS_CUSTOM_ROS2_PACKAGES` |

Pass them as `-D<name>=<value>` at CMake configure time. Package lists use CMake
semicolon separators and must be quoted in the shell. The common header must
match the Runtime's registry; the native header must match the installed native
packages. Standard Gateway builds use no custom registry. Optional owned RCL
service and original-control profiles have additional explicit inputs in the
ROS2 CMake project; the two commands above do not enable those profiles.

### ARM64 and other native cross targets

The root `-a` option selects the **Runtime's native architecture**. Guest pointer
width is selected separately by the SDK application's `set_arch("wasm32")` or
`set_arch("wasm64")`. Building an ARM64 Runtime does not turn a wasm32 Guest
into a memory64 Guest.

On an actual ARM64 Linux builder, use `-a arm64` in the native commands above.
For a Clang cross build from x86_64 Linux, supply the real target sysroot and
target libraries:

```sh
TARGET_SYSROOT=/absolute/toolchains/aarch64-linux-gnu/sysroot
xmake f -y -c -m release -p linux -a arm64 -o build/ros2-arm64 \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --target=aarch64-linux-gnu --llvm-target=aarch64-linux-gnu \
  --sysroot="$TARGET_SYSROOT" \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=n --import-ros2=y
xmake -j1 uwvm2-ros
```

Add `--ros-platform-jetson=y` only for that explicitly selected deployment. The
Gateway must be built separately with an ARM64 ROS SDK and a matching CMake
cross-toolchain file. With LLVM execution enabled, also supply a matching
`--llvm-cmake-toolchain=<file>` that sets the target and `LLVM_HOST_TRIPLE`;
host libraries cannot stand in for target libraries. A QEMU user-mode smoke
command for the interpreter build is:

```sh
qemu-aarch64 -L "$TARGET_SYSROOT" \
  build/ros2-arm64/linux/arm64/release/uwvm2-ros --version
```

This checks only Runtime startup. Native ROS communication, custom codecs and
Gateway behavior require their own executed peers and explicit deployment.

### Guest builds, deployment and diagnostics

Build each Guest from its SDK example directory with `xmake`. The SDK uses
`set_language`, `set_ros_platform`, `ros1_version`/`ros2_version`, `set_arch` and
`set_plat("wasm-wasip1")`; these are SDK declarations, not root Runtime CLI
options. Consult the [SDK review index](https://github.com/UlteSoft/uwvm2-ros-sdk/blob/main/document/REVIEW.en_US.md) for
the installed platform/language scope and its outstanding qualification.

Runtime imports, compiled schema selections and Gateway linking grant no Guest
authority. Before running a ROS application, manually supply the expected
Gateway socket and UID, backend identity, node/namespace, timeouts, audit settings
and exact resource grants. Use the actual [ROS1](../command-line/ros1.md) and
[ROS2](../command-line/ros2.md) CLI documentation. For preloaded Wasm libraries,
bind each `--ros1-single-*` or `--ros2-single-*` policy to the exact loaded module
name; enabling ROS globally does not provide module isolation.

| Configure/build symptom | What to check |
| --- | --- |
| `ROS runtime host imports require Linux SO_PEERCRED` | The selected Runtime platform must be Linux; this is not fixed by installing ROS on macOS or Windows. |
| Vendor requires an existing registry header | Supply its paired `-registry-header` and enable the corresponding import; a language record header is not necessarily a native-bound registry. |
| Multiple manifests rejected | Select one reviewed, deduplicated merged manifest through the same normalized path. |
| Registry count/cell ceiling exceeded | Inspect the selected catalog and choose explicit in-range compiled ceilings; runtime permission limits are independent. |
| Runtime builds but Gateway cannot find native packages | Enter the selected ROS SDK environment and verify all native package/type-support dependencies and ABI. |
| Build succeeds but the Guest is denied | Inspect manual Runtime/Gateway policy and exact schema identities; build flags do not provision permissions. |
