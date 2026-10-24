# Darwin
macosx, iphoneos, watchos, tvos

## Prerequisites
- Ensure Command Line Tools are available:
  - `xcode-select --install`
- Use the latest available Darwin SDK (typically via the newest Xcode / Command Line Tools) when building.
- Optional package manager: Homebrew
  - Install LLVM: `brew install llvm`
  - Install GCC: `brew install gcc`
  - Install xmake: `brew install xmake`

## Examples
```shell
# GCC toolchain (default)
xmake f -m release
xmake

# LLVM/Clang toolchain
xmake f -m release --use-llvm-compiler=y
xmake

# Install
xmake i -o /usr/local
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
1. `--static=none|non-system|compiler` Static linking policy (`non-system` avoids Darwin's unsupported global `-static`)
2. `--march` defaults to `none` (the configured toolchain baseline). Use `--march=native` only for developer-local builds; release tiers must use the fixed target/feature settings below.
3. `--use-cxx-module=y` Use cpp module to compile, compiler may not be supported
4. `--apple-platform` Set Apple platform target and minimum OS version (see below)

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
1. `--static=none|non-system|compiler` Static linking policy (`non-system` avoids Darwin's unsupported global `-static`)
2. `--march` defaults to `none` (the configured toolchain baseline). Use `--march=native` only for developer-local builds; release tiers must use the fixed target/feature settings below.
3. `--use-cxx-module=y` Use cpp module to compile, compiler may not be supported
4. `--apple-platform` Set Apple platform target and minimum OS version (see below)

## Static release artifacts

Darwin does not support fully static executables in the ELF/Linux sense. For
release artifacts, use `--static=non-system` and keep Apple platform libraries
dynamic. If the LLVM installation only provides `libLLVM*.dylib` and not
`libLLVM*.a`, build the static release artifact with the LLVM AOT backend
disabled.

If a release artifact enables LLVM AOT and statically links LLVM archives, the
LLVM archives must be built for the same CPU baseline as the artifact. Do not
link an AVX2-built LLVM archive into an SSSE3/SSE4.2/AVX artifact; LLVM itself
may execute instructions that are unavailable on the target machine. Either
build one conservative LLVM archive for the lowest supported baseline and reuse
it for higher tiers, or build one LLVM archive per release tier.

Use `--march=none` for all release artifact builds. Select x86_64 ISA tiers with
explicit feature flags such as `--cxflags="-mssse3"` instead of `--march`.

Common environment:

```shell
export SYSROOT=/path/to/apple-darwin-sysroot
export PATH="$SYSROOT/../llvm/bin:$PATH"
```

Apple Silicon release artifact:

```shell
xmake f -y -c -m release \
  --plat=macosx --arch=arm64 \
  --use-llvm-compiler=y \
  --sysroot="$SYSROOT" \
  --llvm-target=aarch64-apple-darwin \
  --use-cxx-module=n \
  --static=non-system \
  --execution-jit=none \
  --march=none \
  --openssl-root=/opt/homebrew \
  --ccache=n \
  --ldflags="-nostdlib++"
xmake -j1 uwvm2-ros
cp build/macosx/arm64/release/uwvm2-ros build/aarch64-apple-darwin-uwvm2
```

Intel macOS release tiers:

| Artifact | Extra compiler flags | Intended baseline |
| --- | --- | --- |
| `x86_64_SSSE3-apple-darwin-uwvm2` | `-mssse3` | Core 2 Duo class x86_64 Macs |
| `x86_64_SSE4_2-apple-darwin-uwvm2` | `-msse4.2` | Nehalem / Westmere |
| `x86_64_AVX-apple-darwin-uwvm2` | `-mavx` | Sandy Bridge / Ivy Bridge |
| `x86_64_AVX2-apple-darwin-uwvm2` | `-mavx2 -mbmi -mbmi2` | Haswell and newer |
| `x86_64_AVX2_PRFCHW-apple-darwin-uwvm2` | `-mavx2 -mbmi -mbmi2 -mprfchw` | Broadwell and newer |

Build one Intel tier:

```shell
xmake f -y -c -m release \
  --plat=macosx --arch=x86_64 \
  --use-llvm-compiler=y \
  --sysroot="$SYSROOT" \
  --llvm-target=x86_64-apple-darwin \
  --use-cxx-module=n \
  --static=non-system \
  --execution-jit=none \
  --march=none \
  --openssl-root="$PWD/build/deps/openssl-src-x86_64" \
  --ccache=n \
  --cxflags="-mavx2 -mbmi -mbmi2" \
  --ldflags="-nostdlib++"
xmake -j1 uwvm2-ros
cp build/macosx/x86_64/release/uwvm2-ros build/x86_64_AVX2-apple-darwin-uwvm2
```

Check that only Apple system libraries remain dynamic:

```shell
file build/aarch64-apple-darwin-uwvm2
otool -L build/aarch64-apple-darwin-uwvm2
arch -x86_64 build/x86_64_AVX2-apple-darwin-uwvm2 --version
```

## Build for Apple platforms (iOS / watchOS / tvOS / visionOS)

`xmake/option.lua` defines an `apple-platform` option. It controls the `-mtargetos` flag and the minimum OS version (`-m*-version-min`) when using Clang/LLVM.

### Enumerated values

- macOS:
  - `MACOS_SEQUOIA`: macOS 15.0 Sequoia
  - `MACOS_SONOMA`: macOS 14.0 Sonoma
  - `MACOS_VENTURA`: macOS 13.0 Ventura
  - `MACOS_MONTEREY`: macOS 12.0 Monterey
  - `MACOS_BIG_SUR`: macOS 11.0 Big Sur
  - `MACOS_CATALINA`: macOS 10.15 Catalina
  - `MACOS_MOJAVE`: macOS 10.14 Mojave
  - `MACOS_HIGH_SIERRA`: macOS 10.13 High Sierra
  - `MACOS_SIERRA`: macOS 10.12 Sierra
  - `MACOS_EL_CAPITAN`: macOS 10.11 El Capitan
  - `MACOS_YOSEMITE`: macOS 10.10 Yosemite

- iOS:
  - `IOS_18`: iOS 18.0
  - `IOS_17`: iOS 17.0
  - `IOS_16`: iOS 16.0
  - `IOS_15`: iOS 15.0
  - `IOS_14`: iOS 14.0
  - `IOS_13`: iOS 13.0
  - `IOS_12`: iOS 12.0
  - `IOS_11`: iOS 11.0

- tvOS:
  - `TVOS_18`: tvOS 18.0
  - `TVOS_17`: tvOS 17.0
  - `TVOS_16`: tvOS 16.0
  - `TVOS_15`: tvOS 15.0
  - `TVOS_14`: tvOS 14.0
  - `TVOS_13`: tvOS 13.0

- watchOS:
  - `WATCHOS_11`: watchOS 11.0
  - `WATCHOS_10`: watchOS 10.0
  - `WATCHOS_9`: watchOS 9.0
  - `WATCHOS_8`: watchOS 8.0
  - `WATCHOS_7`: watchOS 7.0

- visionOS:
  - `VISIONOS_2`: visionOS 2.0
  - `VISIONOS_1`: visionOS 1.0

You can also use a custom format: `"platform:version"` (for example, `"macos:10.15"`, `"ios:13.0"`).

Use it at configure time, for example:

```shell
# iOS
xmake f -m release --use-llvm-compiler=y --apple-platform=IOS_18

# tvOS
xmake f -m release --use-llvm-compiler=y --apple-platform=TVOS_18

# watchOS
xmake f -m release --use-llvm-compiler=y --apple-platform=WATCHOS_11

# visionOS
xmake f -m release --use-llvm-compiler=y --apple-platform=VISIONOS_2

# Custom version (example)
xmake f -m release --use-llvm-compiler=y --apple-platform=ios:17.0
```

Then build and install as usual:

```shell
xmake
xmake i -o <install_path>
```

## Caveat
1. Add `--use-llvm-compiler` when you want to build with the LLVM/Clang compiler toolchain. This only selects the compiler toolchain and does not enable the LLVM AOT backend by itself.
2. Darwin does not support the global `-static` strategy used by `--static=compiler`. Use `--static=non-system` for release builds when your non-platform dependencies, such as LLVM, provide static archives.

## ROS development from macOS

The repository's native macOS Runtime can run ordinary Wasm applications, but
the current ROS host imports require a **Linux Runtime target**. The check is
explicit in [`xmake.lua`](../../xmake.lua): `--import-ros1=y` or
`--import-ros2=y` on `macosx` fails with
`ROS runtime host imports require Linux SO_PEERCRED`. Installing a macOS ROS
SDK does not change that transport and peer-authentication boundary.

| Workflow | Configuration |
| --- | --- |
| Native macOS Runtime | `-p macosx`, the Mac's native architecture, and both ROS import switches `n` |
| ROS-enabled Runtime and Gateway | Build and execute both in Linux, using an SSH host or Linux VM |
| Wasm application | Use its SDK example project and a qualified compiler/runtime bundle for the actual build host |
| Native ROS middleware | Install the selected distribution and native interface packages in the Linux Gateway environment |

### Native macOS Runtime without ROS imports

Select `arm64` for an Apple Silicon target or `x86_64` for an Intel target.
Run from the repository root with a compatible bootstrap compiler:

```sh
xmake f -y -c -m release -p macosx -a arm64 -o build/macos-runtime \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=n --import-ros2=n
xmake -j1 uwvm2-ros
```

For the LLVM execution backend, select `--execution-jit=llvm` and an explicit
dependency build budget such as `--llvm-build-jobs=1`. The pinned LLVM
dependency and the bootstrap compiler are separate selections. Use the Darwin
static-linking and CPU-baseline guidance above for release packaging.

Do not add `--ros-vendor-*`, `--ros-platform-isaac-*`,
`--ros-platform-groot=y` or `--ros-platform-jetson=y` to this macOS Runtime
configuration. Those selections require the Linux ROS deployment boundary;
the Jetson profile additionally requires Linux ARM64 and ROS2.

### Build the ROS products on an SSH Linux host

Author changes locally, transfer the complete source folder, and build in the
Linux copy. Substitute your host alias and an existing parent directory; the
destination folder below must be dedicated to this checkout:

```sh
LINUX_HOST=linux
scp -r ./uwvm2-ros "$LINUX_HOST:Documents/"
ssh "$LINUX_HOST"
```

Run the `scp` command from the local parent of `uwvm2-ros`, not from inside it.
For a large development checkout, prepare a clean source snapshot first so
build outputs and tool caches are not uploaded. Then, in the Linux shell:

```sh
cd "$HOME/Documents/uwvm2-ros"
xmake f -y -c -m release -p linux -a x86_64 -o build/ros2 \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=n --import-ros2=y
xmake -j1 uwvm2-ros
```

Use `-a arm64` on a native ARM64 Linux builder. A Linux VM on Apple Silicon
does not require an x86_64 Runtime merely because the editor runs on macOS.
An x86_64-to-ARM64 cross build still requires its explicit target compiler,
sysroot, native ROS libraries and a separately tested execution environment.

In that Linux environment, choose the imports deliberately:

| Intended Runtime | Root Xmake configuration |
| --- | --- |
| ROS1 only | `--import-ros1=y --import-ros2=n` |
| ROS2 only | `--import-ros1=n --import-ros2=y` |
| Both namespaces | `--import-ros1=y --import-ros2=y` |
| Reviewed vendor schemas | Add the vendor switch and matching absolute `-registry-header` path |
| Larger merged registry | Select explicit in-range `--ros-registry-max-interfaces` and `--ros-registry-max-cells` |

See the [Linux ROS configuration guide](linux.md#ros-build-configuration) for
complete vendor switches, merged-manifest rules, Gateway CMake commands,
original ROS2 control selection and ARM64 examples. Root Runtime switches do
not replace the SDK declarations `ros1_version` or `ros2_version`.

### Guest applications and runtime permissions

Each installed SDK example owns an `xmake.lua`. Its `set_language` and
`set_ros_platform` declarations select actual source code, while
`set_arch("wasm32")`/`set_arch("wasm64")` and `set_plat("wasm-wasip1")`
select the Guest ABI. A supported Linux compiler bundle does not imply that
the same SDK route already has a macOS-host compiler bundle. Check the
[current SDK scope](https://github.com/UlteSoft/uwvm2-ros-sdk/blob/main/document/REVIEW.en_US.md) and use the Linux builder
for routes without a qualified Darwin producer.

`xmake run` needs an installed Runtime and an explicit deployment. Keep the
ROS-enabled Runtime, its Gateway and their authenticated Unix socket inside
the same Linux environment. An SSH port forward is not a substitute for the
Linux peer-credential protocol. Configure [ROS1](../command-line/ros1.md) or
[ROS2](../command-line/ros2.md) connection fields and exact grants manually;
for preload deployments, bind grants to the exact loaded module name.

### Generated Mach exception server

The macOS native instruction-step debugger needs Apple's protected Mach
exception server. [`xmake/mach_exc_protected.lua`](../../xmake/mach_exc_protected.lua)
runs `xcrun mig` against the selected SDK's `mach/mach_exc.defs`, then compiles
the generated C server. The Lua rule itself is the explicit file dependency;
there is no separate checked-in `.uwvm_mig` placeholder.

The root build attaches this rule for macOS ARM64, and for Intel macOS when
`--macos-x64-native-step=y` is selected. Generated `mach_exc_server.c`,
`mach_exc.h` and their object stay below the target-specific build/autogen
directory. Editing the rule or changing the SDK definition invalidates that
object. Deleting generated build outputs is safe: the next applicable build
regenerates them. Keep the Lua rule and the SDK's original definition.
