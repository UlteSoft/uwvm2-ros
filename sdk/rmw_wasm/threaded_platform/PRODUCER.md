# SDK33 and original-rclcpp producer contract

Every command below is a future Root-owned template. The author has run no
configure, generator, compile, link, probe, model, Guest or native operation.
Expand placeholders only from actual admitted complete source/tool roles.
Partial research directories and historical SDK24 paths are not build inputs.

## Actual fixed source route

The retained official SDK33 tag object
b34e4c813e72ccc2e7d6bafcacb99dd964586e00 resolves to commit
c10c0507deb3e5aad506f1f9f32084e49a21834b. Its actual complete tree selects LLVM
4434dabb69916856b824f68a64b029c67175e532, wasi-libc
161b3195fc2558d2b1ba3eb9ffae3b2b47407623 and config
f992bcc08219edb283d2ab31dd3871a4a0e8220e. Root retains that provenance, full
SDK tree and three actual LLVM patches 168449/186054/185770. A full recursive
working tree with those real patches is required before building. Supplementary
raw libc/LLVM files have fixed-commit HTTPS URLs/computed Git blob hashes; where
a recursive submodule tree was not acquired, no independent tree-membership
claim is made. No installed SDK/tool/archive or native identity is forecast.

Actual SDK33 sysroot source supports WASI_SDK_EXCEPTIONS=ON or DUAL and requires
Clang >=22 for either. Its threaded target path enables real pthread libc++ and
libc++abi plus static libunwind; nonlegacy EH flags are in the actual source.
The original CppExceptions document requires compilation with
-fwasm-exceptions -mllvm -wasm-use-legacy-eh=false and linking with
-fwasm-exceptions -lunwind. Do not mix SDK24 archives: its actual fixed sysroot
source disables libc++/libc++abi exceptions and the unwinder. Historical actual
ordinary wasm32-wasi producers do not qualify this profile.

A complete SDK33 sysroot source recipe, using an actual coherent LLVM >=22
bootstrap SDK, is:

```sh
<cmake> -G Ninja -S <complete-fixed-sdk33-source> -B <fresh-sdk33-sysroot-build> \
  -DCMAKE_TOOLCHAIN_FILE=<fixed-sdk33-source>/wasi-sdk-p1.cmake \
  -DWASI_SDK_PREFIX=<actual-bootstrap-sdk> \
  -DWASI_SDK_TARGETS=wasm32-wasi-threads \
  -DWASI_SDK_EXCEPTIONS=ON -DWASI_SDK_LTO=OFF
<cmake> --build <fresh-sdk33-sysroot-build> --parallel 1
```

Retain real build/install/export outputs; a sysroot build does not supply a new
compiler by inference. Final SDK selection includes the real matching clang,
clang++, llvm-ar/ranlib, compiler-rt/resource-dir and produced sysroot/archives.
ON is one finite exception profile; DUAL requires independent actual header and
archive selection evidence. The official pthread toolchain selects
wasm32-wasi-threads, -pthread, imported/exported memory and real tools. The
probe also accepts actual wasm32-wasip1-threads when its source/import closure
is independently admitted. No generic -D_LIBCPP_HAS_THREADS override is used.

## Signal package and actual executable link check

```sh
<cmake> -G Ninja -S <coherent-source>/sdk/rmw_wasm/threaded_platform \
  -B <fresh-signal-build> \
  -DCMAKE_TOOLCHAIN_FILE=<actual-sdk33>/share/cmake/wasi-sdk-pthread.cmake \
  -DWASI_SDK_PREFIX=<actual-sdk33> \
  -DCMAKE_PREFIX_PATH=<actual-static-cross-prefix> \
  -DCMAKE_INSTALL_PREFIX=<actual-static-cross-prefix> \
  -DUWVM_BUILD_ORIGINAL_RCLCPP_WASI_SIGNAL=ON -DBUILD_SHARED_LIBS=OFF
<cmake> --build <fresh-signal-build> --parallel 1
<cmake> --install <fresh-signal-build>
```

The actual installed toolchain path is an input role, not a predicted pin.
The fresh probe forces EXECUTABLE in function scope, removes its private cache
entry before/after checking, selects C++17, and retains real pthread create/join,
mutex/recursive mutex/CV/timed wait/once, semaphore, TLS destructor and C++ EH,
plus real wasi/api.h types and fd_read/poll symbols. No probe binary is run.
Direct permissive unresolved-symbol tokens in supplied flag/library/config
strings are rejected; arbitrary transitive target/response contents still need
the independent full final-command/archive/import audit. Final complete MD/archive-
member/import checks remain mandatory; a toolchain can otherwise hide wrong
libraries or extra imports despite one successful test source link.

The exported UWVM_WASI_SIGNAL_ADMITTED property is one only after that future
actual source compiles and links. It does not attest any Runtime, descriptor,
Guest or native ROS route. Current actual qualification remains zero.

## Fixed private original source port

Admit complete actual original ROS sources and their real transitive package
and generator graphs. Apply original-rclcpp-signal-port.patch to a fresh private
source copy after checking both exact original preimages and source membership.
All rclcpp TUs must consume the same adapted private header/implementation and
target definition. Keep original CMakeLists and public NodeOptions unchanged;
facade include paths are forbidden. Root's separate composite project hook can
invoke this after the original target exists:

```cmake
include(<actual-signal-install>/share/uwvm_rmw_threaded_platform/cmake/FixedOriginalPort.cmake)
uwvm_attach_original_rclcpp_wasi_signal(rclcpp)
```

It checks the fixed adapted pair/original CMake hashes, adds the target-specific
port definition and links the actual library. It does not remove the existing
project-hook fatal. Root must first close its real thread-safe B/C ownership,
infinite guard/data/shutdown wait, logger/clock, full default codecs, static
single-provider original typesupport and Runtime threaded initial-main/storage/
TLS/segments/EH/import profile. No quiet options or synthetic endpoints.

Original rclcpp::init initializes Context before installing signals. Therefore
run_original prepares the channel before entering the original application.
Enclosing Root bootstrap first prepares real A/B/C selections and keeps them
alive through original Context/Node/Executor/clock/logger finalization. Every
actual default endpoint needs its real codec and explicitly granted capability.
The example builds an actual static program/bootstrap body, not a standalone
executable with invented profiles. Final initial-main links it into Root's real
prepared bootstrap. There are no weak/fake provider functions in this package.
Alternatively map an unchanged official application's main symbol per-TU and
pass that real entry to run_original, preserving its original source/license.

## Runtime/Host qualification remains independent

Record the final module's complete actual imports, source/MD and selected archive
members, including genuine wasi::thread-spawn/start, clock/fd/poll/proc_exit,
shared imported memory/max and all ROS tuples. No link flag grants permissions.
Root must actually enforce finite thread/stack/shared-memory/ID budgets, genuine
per-child local storage/TLS/tables/segment initialization, once-only initial-main,
EH/first-terminal publication, external Native-effect completion and physical
joins. Source inspection is not execution evidence for those contracts.

Root must install the trusted nonblocking read fd and preserve its map/OFD while
holding the private Host write endpoint through whole-Group physical retirement.
Its real signal source sends only admitted SIGINT/SIGTERM URS1 meaning, serializes
writes and never replays an uncertain partial frame. No Guest pathname/config
or stdin can replace that authority. Actual trusted-fd CLI/map installation is
still pending, as are the B/C/default-codec and complete original execution
cohorts. Execute CONTRACTS.md only after implementation/admission is complete,
under Root's actual resource-owned executor and real physical cleanup watchdog.
