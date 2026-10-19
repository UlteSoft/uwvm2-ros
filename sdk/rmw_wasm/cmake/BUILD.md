# Original ROS sources, static WASI build

This is an implementation candidate and a concrete build recipe. It has not
been configured, generated, compiled, linked, imported or tested. It establishes
neither original rcl/rclc/rclcpp compatibility nor native ROS delivery. The
finite `sdk/compat/rclc` and `sdk/compat/rclcpp` facades are different products
and must never appear on this build's include path.

## Source and tool admission before any build command

Root must admit complete original source trees and their real transitive package
graph at immutable commits. The accompanying source-only acquisition contains
exact selected files and full Git tree objects, not complete working trees or
an installed ROS SDK. Do not configure that partial research directory as a
complete workspace. Each source package must have its actual package.xml,
CMakeLists, generator inputs and implementation files. Preserve the original
license and generated type-description sources.

The platform hook is pinned to the exact observed rcutils, rcl,
rmw_implementation and rosidl_typesupport source CMake bodies. It replaces only
the original rcutils target's `src/time_unix.c` and `src/shared_library.c` with
the two explicit adapters. It leaves the original formatting/time.c, error
handling, allocator, logging, rcl init, default options, rosout, YAML parsing,
parameter/type-description services and all public headers in place. A mismatch
is a configure failure, not permission to update expected hashes. All other
dependencies are original packages; absent libc/OS APIs remain real build errors.

Admit actual CMake >=3.20, Ninja, native Python plus its actual generator modules,
and the real WASI clang/clang++/llvm-ar/llvm-ranlib/sysroot. This candidate does
not download dependencies or guess an installed SDK/tree/native identity.
Use one fresh owned build/install prefix whose package resources contain only
cross-target libraries and explicitly admitted host CMake/Python generators.
`CMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY` deliberately refuses accidentally
using native ROS libraries. Host programs are separate; pass the actual native
`Python3_EXECUTABLE`. Required host Python modules are real generator inputs,
not cross-compiled Python binaries or substitutes for original templates.

## Serial package commands

The following are command templates for Root's independently resource-owned
executor. Placeholder paths are input roles, not current observations. Expand
each only after actual source/tool/full7 admission. Every build is serial,
and original package tests are a later separate bounded task. No command in
this document has been executed by the author.

For each actual package, use:

```sh
<cmake> -S <actual-package-source> -B <fresh-package-build> -G Ninja \
  -DCMAKE_MAKE_PROGRAM=<actual-ninja> \
  -C <current-rmw-source>/cmake/static-cache.cmake \
  -DCMAKE_TOOLCHAIN_FILE=<current-rmw-source>/cmake/wasi-toolchain.cmake \
  -DUWVM_WASI_CC=<actual-clang> -DUWVM_WASI_CXX=<actual-clang++> \
  -DUWVM_WASI_AR=<actual-llvm-ar> -DUWVM_WASI_RANLIB=<actual-llvm-ranlib> \
  -DUWVM_WASI_SYSROOT=<actual-wasi-sysroot> \
  -DUWVM_CROSS_PREFIX=<actual-cross-prefix> \
  -DCMAKE_PREFIX_PATH=<actual-cross-prefix> \
  -DCMAKE_INSTALL_PREFIX=<actual-cross-prefix> \
  -DPython3_EXECUTABLE=<actual-native-python> \
  -DUWVM_SDK_INCLUDE=<actual-coherent-source>/sdk/include
<cmake> --build <fresh-package-build> --parallel 1
<cmake> --install <fresh-package-build>
```

The dependency graph determines the complete order, not an invented abbreviated
package list. Specific required edges are:

1. Install the original host ament/generator resources and cross-target base
   headers/library dependencies into their explicitly admitted prefix roles.
2. Build `uwvm_rmw_platform` using `-S <rmw-source>/platform`. It has no rcutils
   dependency and opens no resources while being built or statically loaded.
3. Build original rcutils with the project hook, then original rosidl_runtime_c,
   rosidl_dynamic_typesupport and rmw plus their actual dependencies. The hook
   registers the platform export **before** original ament_package runs.
4. Build the original C/C++ generator, introspection and dispatcher packages.
   Their normal generators and complete interface source closures are required.
   Every C forwarding output selects only
   `rosidl_typesupport_introspection_c`; every C++ output selects only
   `rosidl_typesupport_introspection_cpp`. These are two language-specific
   single-provider branches, never two providers for the same dispatcher.
   The original static generator itself rejects multiple selected providers.
5. Build the complete original std_msgs interface family with its real generator,
   type hash/description/source and introspection/one-provider forwarding output.
   `rmw_uwvm` explicitly requires its real `std_msgs__rosidl_generator_c`,
   `std_msgs__rosidl_typesupport_introspection_c` and `std_msgs__rosidl_typesupport_c`
   targets; transitive accidental include paths do not supply this dependency.
   Build `rmw_uwvm` using `-S <rmw-source>/cmake` only after the actual B and C
   implementation sources are present/reviewed. No substitute sources are
   generated. The original rmw utility library remains a real dependency.
6. Build original rmw_implementation with its official
   `RMW_IMPLEMENTATION_DISABLE_RUNTIME_SELECTION=ON` and
   `RMW_IMPLEMENTATION=rmw_uwvm`. Its original extras directly link the actual
   implementation. The dynamic `functions.cpp` selector must not be built.
7. Build original rcl_logging_interface and this `rcl_logging_uwvm` package using
   `-S <rmw-source>/platform/logging`, then original rcl and rcl_action/rclc with
   their **complete** real dependencies. `RCL_LOGGING_IMPLEMENTATION` is an
   explicit package choice, not a default-noop backend.

The cache refuses contradictory selections, including environment overrides
used by the original get_used_typesupports function. No dlopen fallback, fake
type-support object, ignored link error, allowed undefined symbol, forged native
hash or synthesized generated struct is permitted. The final link must resolve
every actual public RMW entry referenced by the application and the original
libraries. Root must preserve compile/link commands, dependency files, actual
archives/native member proof, original and generated source bytes and the full
actual import catalog; static libraries alone prove no executable route.

## Explicit bootstrap and default behavior

`platform/examples/original_client_bootstrap.c` demonstrates explicit preparation
around an original application entry. Before original rcl initialization, supply
real system/steady clock aliases, exact logger names with five independently
selected severity aliases, B's actual endpoint/codec selection, and C's real
local graph provider. Raw Host opens still independently enforce permissions,
types, native identity, QoS and quotas. Missing selection is denied.

Do not turn off original rosout, parameter, graph or type-description defaults
to make a minimal publisher start. Every endpoint actually created by the
unmodified original library needs its complete real generated codec and an
explicit matching native capability selection. B currently provides String as
its first complete codec; this is not proof that the original default Log or
type-description/parameter service graphs are implemented. A missing codec or
provider must cause refusal. A node that fails to initialize is not a successful
default-denied execution witness.

The bootstrap lifetime reference spans original init through complete original
shutdown/finalization. Profile uninstall refuses while B still has live options
or contexts; in that case retain the platform and report the leak rather than
retiring clocks underneath live objects. An application must finalize its own
actual objects. Logging shutdown merely deactivates the external logger; explicit
platform close afterwards attempts every resource close in reverse order once.
Save the independent primary, per-owner close and concurrent-log refusal facts.

## Platform boundaries and remaining blockers

This first profile uses wasm32 WASI, actual clocks through the capability ABI,
explicit severity logging and a static implementation. It adds no ambient
filesystem preopen, environment variable, thread, signal, ROS endpoint, graph
query or management permission. Any WASI stdio/filesystem/environment imports
from the **original** dependencies must be separately inspected and admitted
by Root; package configuration is not an import grant. No ROS_HOSTNAME/DNS or
service availability support is inferred here.

The steady clock is genuine monotonic data; it is not a sleep primitive and is
not relabeled slew-free RAW. C must provide a genuine finite wait/driver with
actual elapsed-time checks or reject waits. A number of polls or stdio credits
is not a nanosecond duration. The rcutils RAW API returns a real error with
untouched output. Shared-library APIs explicitly refuse with error/NULL/false;
none ever presents a fake loaded library or symbol.

The actual original rclcpp graph listener, signal handler and executor use real
threading. This cohort contains no verified WASI pthread/signal/condition-variable
provider, so its project hook explicitly refuses rclcpp configuration. It does
not replace those APIs with no-ops or market the separate facade as the original
library. A later threading/platform cohort, actual generated C++ object-layout
codecs and complete default endpoint closure are required before claiming the
original rclcpp examples build/run on this target.

All original C/C++ declarations, actual native identity validation and generated
default values retain their own proof requirements. Run this candidate's 20
authored host-backend contract groups only under Root's owned local executor,
then compile the actual original packages and finally execute separate denied
and explicitly granted Guest/native routes. No tests, ROS case, ABI/native
compatibility or performance measurement is credited by this source package.
