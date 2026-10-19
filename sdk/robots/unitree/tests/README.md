# Complete-family validation

These are local SDK contracts. They do not replace official ROS/Unitree tests,
native schema binding, simulation, controller checks or hardware acceptance.
Run actual compilers and applications on the admitted Linux host inside its
pinned64GiB/swap0 cgroup. Keep fresh outputs, explicit tools, bounded logs and
deadlines. Compile and link serially unless the admitted aggregate memory
budget supports a larger job count.

## Correlate requests without reusing an ID

`topic_rpc_contract.c` retains the original local lifecycle scenarios.
`topic_rpc_lifetime_contract.c` additionally checks late responses after
completion, timeout and explicit abandonment, concurrent capacity, thousands
of storage reuses, output preservation, close and signed64 exhaustion.
`topic_rpc_lifetime_contract.cpp` checks the real noncopyable C++ wrapper.
`tests/ros_unitree/run_local_rpc_contracts.py` compiles/runs these sources in
optimized and UBSan configurations with an explicit original cgroup identity.
None of those programs sends a ROS or robot request.

## Check every original field against independent expected bytes

Prepare one whole locked family with `materialize_catalog.py`, explicitly
selecting `uwvm-ros-vendor-complete-bounded-v1`, and generate its actual Source
with the complete adapter described in the parent guide. Then create a fresh
independent test entry:

```sh
python3 -I -S -B sdk/robots/unitree/tests/full_family_codec_contract.py \
  --materialized /fresh/owned/unitree_go-materialized \
  --out /fresh/owned/unitree_go-oracle
```

The narrow oracle reads the retained original `.msg` declarations and their
locked preparation receipt. It imports neither the production schema parser
nor its codec emitter. Python `struct` constructs expected canonical bytes;
the resulting C11/C++20 program initializes every original field. Scalars and
fixed arrays vary between modes, as do empty/single/full dynamic prefixes.
Generation refuses unless each message has three distinct expected payloads.
The resulting `source-oracle.json` records their exact hashes and lengths.
This format is the bounded canonical SDK ABI, not native CDR or ROS1 wire data.

Compile `contract.c` as C11 using that family's generated include directory.
Compile the same entry separately with `-x c++ -std=c++20`, also adding
`-I sdk/include` from the unchanged SDK layout. The C++ path uses the genuine
`roswasmcpp::Codec<T>` adapter. Every program must return0. A failed check
prints its Source line and returns a nonzero status. Tests cover exact bytes,
decode/re-encode, short/trailing input, insufficient output capacity, malformed
dynamic sizes/booleans where present and unchanged output on refusal.

## Compare complete values with genuine ROS2 CDR

Select `--native-ros2` explicitly when generating a fresh oracle for GO, HG or
API. This adds actual generated Message headers and an independent, full-field
copy into genuine ROS2 values. It invokes `rclcpp::Serialization<T>` to serialize
and deserialize through the actual RMW, checks native equality, copies every
restored field back into a portable record and compares its encoding with the
independent expected bytes. Empty, single and full sequence/string prefixes and
three distinct scalar/fixed-array payloads remain covered. The original legacy
ROS1 catalog is deliberately refused by this ROS2-only mode.

Build `native_values/CMakeLists.txt` against one genuine installed Unitree
family and its matching ROS2 SDK. Preserve the SDK directory structure:
`sdk/include` also needs its real sibling `sdk/common`, including `abi.h` and
`codec.hpp`. An explicit example, using caller-owned absolute paths, is:

```sh
cmake -S /reviewed/source/sdk/robots/unitree/tests/native_values \
  -B /fresh/owned/native-values \
  -DCMAKE_PREFIX_PATH='/actual/unitree-prefix;/actual/ros-prefix' \
  -DUWVM_UNITREE_FAMILY=unitree_go \
  -DUWVM_UNITREE_ORACLE=/fresh/owned/go-native-oracle \
  -DUWVM_UNITREE_GENERATED=/reviewed/source/sdk/custom-types/unitree-go-complete \
  -DUWVM_SDK_INCLUDE=/reviewed/source/sdk/include
cmake --build /fresh/owned/native-values --parallel 1 --verbose
```

Use a real native C++20 compiler, full dependency output, original generated
package libraries and explicit RMW selection. Run the resulting executable only
under the admitted test owner with `--mapped-stop`. After **all** CDR cases, it
raises SIGSTOP. Its original parent must record the stopped process birth,
executable, concrete mapped libraries and cgroup, then resume it and supply the
single stdin byte `I`. A standalone invocation intentionally waits at this
checkpoint; a source file or a configured CMake target is not an execution.
Keep a deadline and clean up every owned child and private mount. No ROS Node
or endpoint is created and no Guest permission is requested by this witness.

This is a native message-value and serialization witness. Its field-copy bridge
is independent test code. It does **not** qualify the production Native
converter, complete-domain identity binding, Gateway, ROS network transport,
robot firmware, simulator, official upstream suite or performance. CDR remains
inside the native program; the Guest ABI remains canonical copied bytes.

For complete production Native binding and converter checks, additionally select
`--native-converters`. This mode requires `--native-ros2` and the genuine matching
bound manifest and native provider. It checks the production binding, decodes
independently expected bytes into the actual native Message, compares every
field with the independently populated native value, and encodes identical
bytes. The genuine CDR round trip remains covered. On a pre-RIHS SDK, build this
mode through the explicit [installed-source proof project](../../industrial/pre_rihs/README.md)
with its matching `UWVM_BOUND_GENERATION` and `UWVM_NATIVE_CODEC_ORACLE`. A
standalone field-copy witness does not supply these production checks. Both
modes still create no endpoint and establish no ROS network or robot delivery.

## Compile and execute the real Wasm program

Select an explicit Wasm producer and its matching standard library/sysroot.
For the C++ oracle, the qualified recipe includes:

```sh
/reviewed/wasi-sdk/bin/clang++ -x c++ --target=wasm32-wasip1 \
  --sysroot=/reviewed/wasi-sdk/share/wasi-sysroot -std=c++20 \
  -O1 -fno-exceptions -fno-rtti -Wl,--threads=1 \
  -Wl,-z,stack-size=1048576 -Wl,--initial-memory=2097152 \
  -Wl,--max-memory=33554432 \
  -I sdk/include -I /fresh/owned/unitree_go-generated \
  /fresh/owned/unitree_go-oracle/contract.c \
  -o /fresh/owned/unitree_go-oracle/contract.wasm
```

The two32769-byte buffers and three complete record objects exceed the
default64KiB stack in the large oracle. The explicit1MiB stack, 2MiB initial
memory and 32MiB maximum are test budgets, not permission grants. The earlier
default-stack run trapped and remains retained as a failed configuration.
Execute the real produced module through a separately identified uwvm2
binary. Keep that Runtime's exact hash and original build receipt; an older
Runtime result must not be assigned to newer Source.

For a native target, select its actual compiler, sysroot and ABI. Inspect the
compiled ELF before user-QEMU execution, including Arm32 float ABI. Supply the
matching physical loader and target libraries explicitly. An emulated codec
result establishes neither a native ROS installation nor upstream architecture
support. i686/armhf cross SDKs whose Linux-header packages contain relative
symlinks also need the exact authenticated common `linux-libc-dev` dependency;
do not replace missing target headers with host headers.

## Keep transport and policy observations separate

The complete telemetry examples are independent of this pure codec corpus.
Their default build must observe denial13, while an explicit receive build
with no ROS configuration must return13. Successful default rejection does
not establish a native publisher, subscription, robot or simulator connection.
The current evidence checkpoint records Native, QEMU, Wasm, permission and
original failed runs separately. It also lists the outstanding official-suite,
Native identity, controller, architecture and VTune acceptance work.
