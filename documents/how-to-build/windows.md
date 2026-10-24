# Windows

## Prerequisites
- MSVC Build Tools (Visual Studio Build Tools, Not Supported Yet)
  - https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2026
- LLVM/Clang (optional): https://github.com/llvm/llvm-project/releases
- xmake: https://xmake.io or `winget install xmake-io.xmake`

## Examples
```powershell
# LLVM/Clang toolchain in the configured Windows SDK environment.
# The direct MSVC compiler route is currently rejected by windows_target().
$WindowsSysroot = "C:/toolchains/windows-msvc"
xmake f -m release --use-llvm-compiler=y "--sysroot=$WindowsSysroot"
xmake

# Install
xmake i -o C:/uwvm2
```

For Windows x64 LLVM-full debugging, the default build and install also produce
`uwvm-debug-server.exe`. Build just the host broker with
`xmake b uwvm-debug-server`; its [launch and connection instructions](../../tools/debug/README.windows.md)
describe secure late attach. The `uwvm2-ros` executable must also be built with LLVM JIT.

## Windows 9x note (thread_local)

Windows 9x targets (WIN95/WIN98/WINME) are only supported via the MinGW build. When targeting Windows 9x with libstdc++, pass `--use-thread-local=n`
to disable C++ `thread_local` in UWVM2, because libstdc++ TLS support may require at least Windows XP on some configurations.

Additionally, running on Windows 9x may require extra prerequisite components (depending on your toolchain/build options). Install prerequisites from:
https://github.com/UlteSoft/uwvm2-prerequisites/tree/master/win95

## Use MSVC
The direct MSVC compiler route is currently unsupported and is explicitly
rejected by `xmake/platform/windows.lua`. Use the LLVM/Clang route below with
the matching Windows SDK, C++ standard library and native link inputs.

### Additional Options
1. `--static=none|non-system|compiler` Static linking policy (`compiler` uses the static runtime/global static strategy where supported)
2. `--use-cxx-module=y` Use cpp module to compile, compiler may not be supported

## Use LLVM
1. Install [[xmake]](https://github.com/xmake-io/xmake/)
2. Install [[llvm]](https://github.com/llvm/llvm-project/releases)
3. Build
```shell
$ xmake f -m [debug|release|releasedbg|minsizerel] --use-llvm-compiler=y --sysroot=<unknown-windows-msvc sysroot path>
$ xmake
```
4. Install UWVM2
```shell
$ xmake i -o <install_path>
```

### Additional Options
1. `--static=none|non-system|compiler` Static linking policy (`compiler` uses the static runtime/global static strategy where supported)
2. `--march` defaults to `none` (the configured toolchain baseline). Fix and test the release CPU baseline through the selected Windows compiler/toolchain; host-native tuning is developer-only where that toolchain supports it.
3. `--use-cxx-module=y` Use cpp module to compile, compiler may not be supported

## ROS development from Windows

ROS host imports in the current repository require a **Linux Runtime target**.
[`xmake.lua`](../../xmake.lua) rejects `--import-ros1=y` and
`--import-ros2=y` for native `windows`, MinGW and other non-Linux Runtime
targets because the Gateway boundary uses AF_UNIX and Linux `SO_PEERCRED`.
A native Windows ROS installation does not bypass that build-time check.

| Workflow | Build and execution environment |
| --- | --- |
| Native Windows Runtime without ROS imports | Windows Clang plus matching Windows SDK/libraries |
| ROS-enabled Runtime and its native Gateway | WSL2, an explicit Linux VM, or an SSH Linux host |
| Wasm Guest | Its SDK example with a qualified producer for the actual host |
| Native ROS distribution and message packages | The selected Linux Gateway SDK environment |

### Native Windows Runtime without ROS imports

Use a configured Clang/Windows SDK environment. Replace the sysroot with the
actual installation; this path is an input, not a package download selector:

```powershell
$WindowsSysroot = "C:/toolchains/windows-msvc"
xmake f -y -c -m release -p windows -a x86_64 -o build/windows-runtime `
  --use-llvm-compiler=y "--sysroot=$WindowsSysroot" `
  --use-cxx-module=n --march=none `
  --execution-int=uwvm-int --execution-jit=none `
  --import-ros1=n --import-ros2=n
xmake -j1 uwvm2-ros
```

For an LLVM execution build, select `--execution-jit=llvm` and an explicit
`--llvm-build-jobs` budget. The pinned LLVM dependency is distinct from the
Clang bootstrap compiler. Switching to `-p linux` alone does not turn Windows
headers, libraries or binaries into a Linux development SDK.

### WSL2 Linux build

Enter an already provisioned Linux distribution from PowerShell, for example:

```powershell
wsl.exe -d Ubuntu
```

Inside its Linux shell, install/provision the selected Linux compiler, Xmake
and native ROS development SDK, then work in a dedicated Linux checkout:

```sh
cd "$HOME/src/uwvm2-ros"
xmake f -y -c -m release -p linux -a x86_64 -o build/ros2 \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=none \
  --import-ros1=n --import-ros2=y
xmake -j1 uwvm2-ros
```

The selected Runtime platform here is Linux. Its output is a Linux executable,
not a `.exe`. Build and run the Gateway in the same Linux environment so the
Unix socket and peer UID/PID checks have their ordinary Linux meaning. Keep
Windows and Linux build directories separate. The architecture must match the
Linux environment; an ARM64 environment uses `-a arm64` and matching native
SDKs rather than relabeling x86_64 libraries.

### SSH Linux build from PowerShell

Run this transfer from the parent of the locally edited source directory;
the remote `Documents` directory must already exist:

```powershell
$LinuxHost = "linux"
scp -r ./uwvm2-ros "${LinuxHost}:Documents/"
ssh $LinuxHost
```

Use a clean source snapshot when the local checkout contains large build/cache
trees. In the remote Linux shell, run the same configure/build commands with
`cd "$HOME/Documents/uwvm2-ros"`. This preserves local source visibility while
Linux owns compilation and execution.

The following selections belong to that Linux configure command:

| Intended Runtime | Root Xmake options |
| --- | --- |
| ROS1 only | `--import-ros1=y --import-ros2=n` |
| ROS2 only | `--import-ros1=n --import-ros2=y` |
| Both import namespaces | `--import-ros1=y --import-ros2=y` |
| ABB schemas | `--ros-vendor-abb=y --ros-vendor-abb-registry-header=<absolute-header>` |
| Other vendor schemas | The corresponding `--ros-vendor-<name>` and matching `-registry-header` |
| Original ROS2 control | `--ros-original-control-manifest-header=<absolute-header>` with ROS2 imports |
| Explicit compiled catalog ceilings | `--ros-registry-max-interfaces=<1..512>` and `--ros-registry-max-cells=<1..1048576>` |
| Jetson profile | `--ros-platform-jetson=y` only with Linux ARM64 and ROS2 |

The complete vendor list, shared-manifest constraints, ROS1/ROS2 Gateway CMake
commands and cross-target examples are in the
[Linux ROS configuration guide](linux.md#ros-build-configuration). Paths passed
to its registry switches must exist in the Linux filesystem. A Windows path
such as `C:/...` is not automatically translated for a remote Linux builder.
Quote CMake package lists containing semicolons in the shell in which CMake
actually runs.

### SDK applications and manual deployment

SDK `set_language`, `set_ros_platform`, `ros1_version`/`ros2_version`,
`set_arch("wasm32")`/`set_arch("wasm64")` and
`set_plat("wasm-wasip1")` configure a Guest application. They are separate
from the root Runtime's `-p linux -a x86_64` or `-a arm64`. Use each example's
own `xmake.lua` and check the [SDK review index](https://github.com/UlteSoft/uwvm2-ros-sdk/blob/main/document/REVIEW.en_US.md)
before assuming a Windows-host compiler or Wasm interpreter bundle is available.
The Linux producer is the appropriate build environment for Linux-only bundles.

Compiling ROS imports or a vendor registry grants no Guest authority. `xmake run`
needs an installed uwvm2-ros Runtime and manually supplied Gateway connection,
backend identity, audit configuration and exact resource grants. Read the
[ROS1](../command-line/ros1.md) and [ROS2](../command-line/ros2.md) launch
contracts. For preloaded modules, bind each Single policy to the exact loaded
module identity and keep ungranted modules ungranted. Do not replace the
authenticated Linux Unix socket with a Windows TCP relay.
