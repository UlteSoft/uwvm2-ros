# Select vendor schemas with xmake

All vendor and platform options below default to `n`. The default binary has
no automatically selected vendor registry. Native ROS libraries and vendor SDKs
belong to separately built Gateway or model-bridge processes; xmake does not
link every robot SDK into `uwvm`.

Each data adapter has two independent configuration inputs:

* `--ros-vendor-NAME=y` selects that vendor's schema build.
* `--ros-vendor-NAME-registry-header=/absolute/bundle/manifest.hpp` supplies the
  actual reviewed, Native-bound generated registry for the exact selected model,
  controller, SDK revision, ROS backend and canonical capacities.

Missing input is an error. A registry supplied for a disabled vendor is also an
error. Selecting a vendor never enables `--import-ros1` or `--import-ros2`
implicitly. A research inventory entry or available xmake option does not mean
that an unimplemented driver or unqualified robot is supported.

| Vendor | Option suffix |
| --- | --- |
| Unitree | `unitree` |
| AgileX | `agilex` |
| Elephant Robotics | `elephant` |
| UFACTORY / xArm | `ufactory` |
| DOBOT | `dobot` |
| Flexiv | `flexiv` |
| Leju | `leju` |
| DEEP Robotics | `deep-robotics` |
| RealMan | `realman` |
| Clearpath | `clearpath` |
| ROBOTIS | `robotis` |
| Universal Robots | `universal-robots` |
| Franka | `franka` |
| PAL Robotics | `pal` |
| Doosan | `doosan` |
| Trossen / Interbotix | `trossen` |
| AgiBot | `agibot` |
| UBTECH | `ubtech` |
| Fourier | `fourier` |
| FANUC | `fanuc` |
| KUKA | `kuka` |
| Yaskawa | `yaskawa` |
| Boston Dynamics / Spot | `boston-dynamics` |
| Kinova | `kinova` |
| ABB | `abb` |
| Booster Robotics | `booster` |
| LimX Dynamics | `limx` |
| Hello Robot / Stretch | `hello-robot` |

## Build one vendor

First export and independently bind the actual Native identities, then generate
the coherent complete catalog. Guest-only SDK preparation deliberately supplies
no `manifest.hpp`; it cannot substitute for this Native-bound input. See
[complete vendor preparation](../../sdk/robots/vendors/COMPLETE_VENDORS.md) and
[the six-language SDK guide](../../sdk/custom-types/COMPLETE_VENDOR_SDK.md).

For an already reviewed Unitree ROS2 bundle:

```sh
xmake f -c -p linux --import-ros2=y \
  --ros-vendor-unitree=y \
  --ros-vendor-unitree-registry-header=/absolute/unitree-bound/manifest.hpp
xmake -j1 uwvm
```

These are invocation examples, not a statement that the required Native-bound
bundle is delivered or qualified. The options select trusted compiler input.
They do not parse or certify arbitrary C++ headers. Retain actual full compiler
dependencies, the selected source revision and final artifacts during acceptance.
The binary's fixed Runtime, interpreter, LLVM and cryptographic components have
their own independent options and size costs.

SDK CMake configuration can also invoke the linker while identifying the cross
compiler. For bounded WASI builds, include `--threads=1` in the initial
`-DCMAKE_EXE_LINKER_FLAGS='-Wl,--threads=1,...'` argument, as well as limiting
the build to `--parallel 1`. A target-level link option takes effect after
compiler identification; it cannot constrain that earlier linker process.

## Build an explicitly merged multi-vendor application

Merge and deduplicate complete compatible declarations first, then generate one
bundle. Every selected vendor must name exactly the same normalized absolute
registry pathname. Two independently generated bundles are refused even when
their headers happen to have similar names. Shared Header, Time, trajectory and
codec declarations must have one coherent definition throughout the Runtime,
Worker and separately compiled Gateway.

```sh
xmake f -c -p linux --import-ros2=y \
  --ros-vendor-unitree=y \
  --ros-vendor-unitree-registry-header=/absolute/merged-bound/manifest.hpp \
  --ros-vendor-kinova=y \
  --ros-vendor-kinova-registry-header=/absolute/merged-bound/manifest.hpp
```

Only the explicitly supplied registry enters `UWVM_ROS_CUSTOM_TOPIC_MANIFEST`.
No SDK directory is globbed into the executable. All Runtime/CLI/test targets
using `def_build()` select this same input. The older explicit
`--ros-custom-registry-header` interface remains available for generic custom
types; when combined with vendor options it must identify that same registry.
The build operator must actually generate only the desired interface set;
setting a vendor label does not remove unrelated declarations from an arbitrary
header supplied by the operator.

Native Gateways must separately select the matching
`UWVM_ROS_CUSTOM_REGISTRY_HEADER`, `UWVM_ROS_CUSTOM_ROS1_HEADER` or
`UWVM_ROS_CUSTOM_ROS2_HEADER`, and explicit SDK package dependencies in their
CMake builds. Enabling a Runtime schema cannot start a Gateway, install an SDK,
select credentials or load an unselected Native library.

## Enable A and B and explicitly disable everything else

Use the configuration printer when changing an existing build. It writes all
28 vendor switches, all three data-platform switches, Jetson, both ROS backends
and every registry input explicitly. It also clears the generic/original-control
registry selections. The generated shell script stops if configuration fails.
It executes nothing until the operator runs the printed commands.

For example, A = FANUC and B = KUKA, with a previously reviewed combined ROS2
bundle and a build directory outside the source checkout:

```sh
python3 -I -S -B sdk/tools/ros_build_config.py \
  --backend ros2 --arch x86_64 --vendor fanuc --vendor kuka \
  --registry /absolute/fanuc-kuka-bound/generated/manifest.hpp \
  --build /absolute/build-fanuc-kuka > /tmp/build-fanuc-kuka.sh
cat /tmp/build-fanuc-kuka.sh
sh /tmp/build-fanuc-kuka.sh
```

Replace the two `--vendor` values with any reviewed pair from the table. Use
one `--vendor` for a single vendor, or omit `--vendor` and `--registry` for the
builtin-only backend. `--backend none` omits both ROS backends; `ros1`, `ros2`
and `dual` select only the named backends. A source/interface inventory cannot
establish that a vendor actually implements both ROS versions. `--platform
isaac-ros`, `--platform isaac-sim` and `--platform groot` independently select
the data platforms, using that same reviewed merged registry.

`-c` clears the project's cached configuration; `-o` selects its output directory.
The project configuration cache still belongs to the checkout. Use separate
source snapshots/checkouts for independent concurrent configurations. Keep the
printed JSON (`--format json`), the actual xmake command log and actual compiler
dependencies with the resulting binary. The printer chooses `minsizerel`,
explicit stripping, function/data sections with linker section collection,
interpreter execution and no LLVM execution backend; it
leaves other security/product options at their project defaults. It does not
claim to disable every optional non-ROS feature in the whole project.

### Prepare the combined source domain

There is now a source-catalog union tool for complete vendor profiles:

```sh
python3 -I -S -B sdk/robots/vendors/merge_catalogs.py \
  --catalog "$UWVM_SOURCE/sdk/robots/industrial/fanuc/catalogs/ros2/catalog.preparation.json" \
  --catalog "$UWVM_SOURCE/sdk/robots/industrial/kuka/catalogs/ros2/catalog.preparation.json" \
  --out /absolute/fresh-fanuc-kuka-source
```

The output contains complete unchanged definition bytes, one deduplicated
`catalog.preparation.json` and an exact source receipt. Duplicate types must
have byte-identical original definitions and identical metadata, registration
and bounds. Differences are refused; the tool does not silently choose a larger
quota, register an extra endpoint or rewrite an old message. It accepts one ROS
backend, at most eight selected catalogs, 512 unique definition roots and 64 MiB of source. Other preparation, native\nbinding, per-record wire/node and original-input inventory ceilings remain\nindependent and may refuse a large source union. The legacy bounded-v2 source\npipeline retains its separate ceiling.
Its output directory must be fresh and outside every selected input tree.

This output is **not** a Native-bound registry. Apply the existing complete
preparation/export/source-proof/binding pipeline to the merged catalog, then
regenerate once. Build both Runtime and Gateway with that one resulting domain
and its explicit native packages. Source-only `bounded_records.*` cannot replace
the missing native proof. If two actual SDK revisions disagree on a shared
type, select a compatible reviewed revision or use separate modules/processes
with separately qualified domains; concatenating generated headers is refused.

### Target platform and ABI are separate from vendor selection

| Desired Linux target | xmake architecture | Explicit compiler target example |
| --- | --- | --- |
| x86-64 | `x86_64` | `x86_64-unknown-linux-gnu` |
| i686 | `i386` | `i686-unknown-linux-gnu` |
| ARM32 base/soft float | `arm` | `arm-unknown-linux-gnueabi` |
| ARM32 hard float | `arm` | `armv7-unknown-linux-gnueabihf` |
| ARM32 big endian | `arm` | `armeb-unknown-linux-gnueabi` |
| ARM32 big endian, hard float | `arm` | `armeb-unknown-linux-gnueabihf` |
| AArch64 little endian | `arm64` | `aarch64-unknown-linux-gnu` |
| AArch64 big endian | `arm64` | `aarch64_be-unknown-linux-gnu` |

These are compiler/ABI selections, not a claim that every ROS release supplies
each target. Select an actual distribution-supported SDK, matching libc/C++
runtime, native packages and QEMU CPU/sysroot. The printer normalizes `i686`,
`armhf`, `armeb` and AArch64 aliases to the project's actual xmake architectures;
float ABI and endian aliases require an explicit matching `--target`.

```sh
python3 -I -S -B sdk/tools/ros_build_config.py \
  --backend ros2 --arch armhf --target armv7-unknown-linux-gnueabihf \
  --sysroot /absolute/reviewed-armhf-sysroot \
  --build /absolute/build-armhf > /tmp/build-armhf.sh
```

Use a matching Clang/C++26 toolchain visible to xmake. `--use-llvm-compiler=y`
selects Clang as the host compiler; `--execution-jit=none` omits the bundled LLVM
execution backend. These switches have different purposes. Jetson is separately
selected with `--jetson --arch arm64`; it does not install CUDA or qualify a
particular Orin/Thor board. Run builds, executables and emulation inside the
admitted hard 64 GiB/swap0 cgroup. A target flag is not a substitute for ELF,
float-ABI, byte-order and actual middleware validation.

## Control and measure executable size

Start with `-m minsizerel --strip=ident --execution-jit=none`. The standard
`release` rule optimizes for speed and preserves symbols by default; these
profiles are not directly comparable without recording their flags. LTO is
independent (`--enable-lto=y` or printer `--lto`) and needs a compatible compiler
and linker. Disabling a runtime backend changes the available execution modes;
verify the intended Wasm workload and its performance before choosing a product
profile. Do not remove permission checks or replace genuine SDKs to save space.

Measure the actual final Linux executable, not object-file totals or the native
Gateway, with the non-executing ELF reader:

```sh
python3 -I -S -B sdk/tools/ros_binary_size.py /absolute/build/uwvm \
  --max-file-bytes 33554432 --max-load-bytes 33554432 > /absolute/size.json
```

The 32 MiB values are an example deployment budget, not a universal acceptance
limit or a claimed current result. The command exits 1 when either explicit
budget is exceeded. The JSON binds the exact file SHA-256, byte count, ELF
class/order, sections, debug/symbol bytes and PT_LOAD file/memory extents.
PT_LOAD memory extents include zero-fill; they are not measured RSS. Separate
native ROS/Gateway libraries, shared dependencies, models and Guest `.wasm`
files require their own deployment inventory and budget.

The serial direct-build recipe in
`tests/ros_build_options/measure_interpreter_sizes.py` compares `none`, `ros1`,
`ros2` and `dual` from one frozen source tree. It records exact commands and
dependencies and measures both original and separately stripped artifacts. Its
explicit exclusions include LLVM execution, WSEA verification, vendor schemas
and native debugging; it does not represent the default xmake product or an
A+B Native-bound vendor build. Retain those distinctions in size reports.

The [r35 actual size matrix](continuation-20261018-evidence/platform-build-size-and-mobile-sdk-r35/README.md)
measures frozen-source compact interpreter files at 5.38 MiB (no ROS), 6.10 MiB
(ROS1), 6.18 MiB (ROS2) and 6.27 MiB (dual). All four pass explicit 8 MiB file
and PT_LOAD file budgets. Their direct recipe excludes LLVM execution, WSEA,
native debugging and custom vendor schemas; it does not qualify the default
xmake build or a Native-bound A+B product's size/performance.

## Select NVIDIA layers separately

The ROS2 data layers use `--ros-platform-isaac-ros`,
`--ros-platform-isaac-sim` and `--ros-platform-groot`, each with its matching
`-registry-header` option and explicit `--import-ros2=y`. Their selected
registries obey the same single coherent bundle rule as vendor schemas.
`--ros-platform-jetson=y` instead validates an explicitly selected Linux ARM64
deployment with ROS2 enabled; it adds no robot schema or CUDA/JetPack library.
Isaac ROS communication is not direct Guest CUDA access, and Jetson deployment
selection is not physical hardware qualification. No Optimus control adapter is
declared without a verified public interface.

## Keep build selection separate from authority and acceptance

Every Guest still starts with no ROS authority. Each actual module, backend,
alias, operation, resource, type identity, QoS and finite limit requires its
existing explicit Runtime grant. Build switches supply no update, debug,
policy, credential, socket or GPU-pointer capability.

The Linux contract in `tests/ros_build_options/test_xmake_vendors.py` invokes the
actual xmake interpreter serially for every option, missing/disabled registry,
shared versus conflicting multi-vendor bundles, backend requirements and Jetson
target selection. Run it only inside the admitted64GiB/swap0 cgroup, with its
explicit xmake executable and fresh output directory. Its small header fixtures
are configuration DATA. A configuration pass is not compilation, a measured
binary-size result, ROS delivery or complete official upstream-suite acceptance.

On2026-10-18, the actual Linux run passed104 configuration cases serially in
the original hard64GiB/swap0 cgroup. The observed child peak was13,606,912 bytes.
The [retained result and original attempts](continuation-20261018-evidence/vendor-xmake-options-linux-104-r1/integration.json)
bind the exact production Lua, xmake executable, commands and logs. A separate
full-project configuration attempt used an incomplete retained remote source
tree and stopped at missing bundled OpenSSL source; it supplies no full-project
build result. No stand-in crypto provider or ignored dependency was used.


## Select one complete TurtleBot3 source profile

The [TurtleBot3 SDK](../../sdk/robots/mobile_bases/robotis/turtlebot3/README.md)
provides independent noetic/humble/jazzy/main bundles. Its CMake target switches
default OFF; enabling a target additionally requires an explicit profile,
absolute source/bundle paths, matching full original bodies and exact generated
products. Main is a development snapshot. A Humble bundle cannot substitute for
a Jazzy bundle merely because their catalog paths look alike.

Runtime selection remains the existing default-off --ros-vendor-robotis option.
A positive native build requires its separately source-bound manifest and the
explicit corresponding ROS backend, as with every other vendor. SDK preparation
emits no native manifest. Adding these four source families therefore adds no
automatic Robotis code to the default Runtime and no Guest authority. Use the
A+B printer above to enable Robotis with another vendor and explicitly clear
all remaining options; first produce one coherent merged native-bound registry.

The r42 test reused the previously measured 6,573,776-byte dual-ROS interpreter.
It did not rebuild a native-bound Robotis Runtime. The new installed SDK source
is 6,967,783 bytes across 645 files; that is source/storage size, not executable
growth. Keep the r35 size matrix's actual build exclusions and 8 MiB acceptance
budget when comparing future equivalent Runtime builds.

## Select a complete Clearpath application and source domain

The [Clearpath SDK](../../sdk/robots/mobile_bases/clearpath/platform/README.md)
contains four independent original domains and six application choices.
For example, use noetic_husky or noetic_jackal with the complete Noetic bundle;
use humble, jazzy or main with its matching original bundle. Main is a pinned
development snapshot. Missing/mismatched choices are refused by the SDK source
selector and CMake configuration. All example/contract targets default OFF.

```sh
cmake -S /absolute/src/sdk/robots/mobile_bases/clearpath/platform \
  -B /tmp/clearpath-husky -DUWVM_CLEARPATH_BUILD_CONTRACTS=ON \
  -DUWVM_CLEARPATH_PROFILE=noetic_husky \
  -DUWVM_CLEARPATH_SOURCE_ROOT=/absolute/src \
  -DUWVM_CLEARPATH_GENERATED=/absolute/src/sdk/custom-types/clearpath-noetic-complete
cmake --build /tmp/clearpath-husky --parallel 1
```

Runtime selection remains --ros-vendor-clearpath=y plus the corresponding
explicit --import-ros1=y or --import-ros2=y. Supply the actual independently
native-bound manifest; the prepared SDK emits no native registry. To combine
Clearpath with Unitree, use the existing A+B printer with those two names, clear
all other switches, and supply one coherent merged native-bound registry.
Preparing these source families does not enable a vendor or any Guest rights.

The r43 SDK qualification reused the same 6,573,776-byte dual-ROS interpreter.
It did not rebuild a Clearpath native-bound Runtime, so this source delivery
does not measure additional executable growth. Keep SDK source/storage bytes
separate from the r35 actual executable size matrix and its stated exclusions.
The entire SDK is not linked into the default binary.

## Select an Elephant Robotics model and combine vendors

The [Elephant SDK](../../sdk/robots/arms/elephant/mycobot/README.md) provides six
separate original source domains: Kinetic, Melodic, Noetic, Foxy, Galactic and
Humble. Thirty explicit model selections cover only the original communication
packages in those exact snapshots. The SDK table lists every selection and
matching domain. All CMake targets default OFF. For example:

~~~sh
cmake -S /absolute/src/sdk/robots/arms/elephant/mycobot \
  -B /tmp/elephant-noetic -DUWVM_ELEPHANT_BUILD_CONTRACTS=ON \
  -DUWVM_ELEPHANT_SELECTION=noetic_mycobot \
  -DUWVM_ELEPHANT_SOURCE_ROOT=/absolute/src \
  -DUWVM_ELEPHANT_GENERATED=/absolute/src/sdk/custom-types/elephant-mycobot-noetic-complete
cmake --build /tmp/elephant-noetic --parallel 1
~~~

Select Runtime code with --ros-vendor-elephant=y plus the explicit ROS backend
and actual native-bound registry. To build A=Clearpath and B=Elephant with every
other vendor OFF, use the same configuration printer as above:

~~~sh
python3 -I -S -B sdk/tools/ros_build_config.py \
  --backend ros1 --arch arm64 --vendor clearpath --vendor elephant \
  --registry /absolute/clearpath-elephant-bound/generated/manifest.hpp \
  --build /absolute/build-clearpath-elephant > /tmp/build-clearpath-elephant.sh
cat /tmp/build-clearpath-elephant.sh
sh /tmp/build-clearpath-elephant.sh
~~~

The explicit architecture spelling must match the project printer; consult its
--help and architecture table before selecting another target. The combined
[Noetic C/C++ application](../../sdk/robots/arms/elephant/mycobot/examples/multi_vendor/README.md)
uses one regenerated complete domain and holds two independent, read-only
capabilities. Its own build option also defaults OFF. The merge preserves the
original ROS1 same-name Mission Message and Action; other cross-category
collisions, conflicting original bytes/bounds/registration and mixed backends
are refused. A successfully opened alias never grants access to the other.

This SDK delivery reuses the measured 6,573,776-byte dual-ROS interpreter. It
does not rebuild a native-bound Elephant or combined Runtime, so no additional
Runtime-size or VTune performance result is inferred. SDK source/storage and
individual Wasm application sizes are reported separately in r44 evidence.
Keep the r35 actual Runtime size matrix, its exclusions and the 8 MiB acceptance
budget when comparing equivalent future builds. None of these SDK source
families is automatically linked into the default Runtime.
