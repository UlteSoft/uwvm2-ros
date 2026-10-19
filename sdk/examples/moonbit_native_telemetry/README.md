# MoonBit telemetry delivered to a native ROS2 subscriber

This example sends a bounded `std_msgs/msg/String` through the capability
Gateway to an independent native `rclcpp` subscriber. It also requires the real
Worker to reject the ungranted `ungranted-telemetry` alias with error 13. The
application cannot choose another resource, topic type, DDS domain or network
endpoint: the administrator supplies those through an explicit `pub` grant.

The current Linux x86_64 / ROS2 Humble checkpoint covers four retained producer
profiles (`wasm` and `wasm-gc`, Debug and stripped Release), repeated twice on
each default-on Runtime mode (Release and Debug). All sixteen new runs delivered
the complete value `bounded MoonBit telemetry`. The native
subscriber checked the publisher node, namespace, type, reliability, durability
and actual nonzero writer GID before admitting the message. It required one
final message, a quiet interval of at least two seconds, and zero matched
publishers after explicit Guest cleanup. Each Worker, native Gateway and native
subscriber exited naturally with status zero. See the
[default-on command records and independent data verifier](../../../test/0050.high_level_languages/results/native-budget/language-defaults-native-20261019/README.md).
The [earlier eight-run checkpoint](../../../test/0050.high_level_languages/results/native-budget/language-ros-native-telemetry-20261019/README.md)
retains the original producer/native-peer builds and the older Runtime. The
new replay preserves the exact four Wasm artifacts; it is not another MoonBit
compiler matrix. All 34 admitted SDK/application and peer source roles match
their earlier actual build inputs.

## Build the Wasm application

Use the admitted Linux MoonBit toolchain under your existing process/resource
owner. Copy this directory to a fresh absolute `PROJECT` directory, and copy
`sdk/moonbit` into `PROJECT/ros-sdk`. Keep `moon.mod`, `moon.work` and the exact
SDK source; the workspace resolves `uwvm/ros` locally. The recorded producer
requires the new `moon.mod` syntax rather than the deprecated JSON manifest.

```sh
MOON_HOME=ABS_MOON_HOME ABS_MOON_HOME/bin/moon -C PROJECT build --frozen \
  --target wasm --debug -j 1 --target-dir ABS_DEBUG_OUTPUT
MOON_HOME=ABS_MOON_HOME ABS_MOON_HOME/bin/moon -C PROJECT build --frozen \
  --target wasm --release --strip -j 1 --target-dir ABS_RELEASE_OUTPUT
```

Repeat with `--target wasm-gc` and separate output directories. The application
is `app_ros2/app_ros2.wasm` below the corresponding target/mode build directory.
The additional `--release --no-strip` products in the evidence preserve producer
names for inspection; they are separate artifacts from the executed stripped
Release products.

Validate the actual module with `wasm-tools validate` and inspect all 43 selected
ROS2 ABI signatures against `sdk/abi/abi-v1.json`. The only WASIp1 imports are
`fd_write(i32,i32,i32,i32)->i32` and `fd_read(i32,i32,i32,i32)->i32`. `abi_coverage`
is an inspection-only export: never invoke it with a valid selector. The package
reserves the low-memory stdio region and SDK arena explicitly, with an initial
memory of 256 pages and maximum 1024 pages. Preserve these reservations and
verify them against actual data segments and heap placement after rebuilding.

## Build and configure the native side

Build the ROS2 Gateway using the native SDK instructions in
[the C tutorial](../c/ADVANCED.md). Its SDK and middleware must match the native
subscriber. The independent subscriber source is
[`tests/ros_hll_native/telemetry_peer.cc`](../../../tests/ros_hll_native/telemetry_peer.cc).
It needs C++17, `rclcpp`, `std_msgs` and their exported include/link dependencies.
Use the official package export lists from a configured native Gateway build;
do not add every ROS include directory, which can shadow libc headers.

The recorded peer was compiled by Clang21 against the selected Humble sysroot,
using its GCC toolchain, the Gateway's actual CMake include/link exports, explicit
SDK dynamic loader, and both `rpath` and `rpath-link` for its three library
directories. The exact compile command, dependency file, link inputs and hashes
are retained in cohort r58. A usage-only invocation exits 1 and does not count
as ROS delivery. Start the real peer with exactly one absolute namespace:

```sh
ROS_DOMAIN_ID=206 RMW_IMPLEMENTATION=rmw_fastrtps_cpp ROS_SECURITY_ENABLE=false \
  ABS_NATIVE_PEER /uwvm_telemetry
```

For a relocated SDK, invoke its own loader with `--inhibit-cache --library-path`
and all required SDK library directories, as recorded in r58/r59. Do not mix an
SDK glibc loader with Host libc. The peer subscribes only to
`/uwvm_telemetry/telemetry` and expects the Gateway node `moonbit_gateway` in
`/uwvm_telemetry`. ROS logging, parameter services and parameter event publishing
are disabled in this witness.

## Review the explicit policy and launch protocol

Compile UWVM with `--import-ros2=y`. The new qualified Runtime products include
ROS1 too, use `--execution-int=uwvm-int --execution-jit=none --use-cxx-module=n`,
Clang21, LLVM/WSEA/LTO disabled and `--cxflags=-gz=zlib`. They use the fresh
default language selection: all fifteen independent adapter options enabled,
mask32767, in both Release and Debug. The original ELF sizes are 15,442,000 and
242,820,864 bytes respectively; exact hashes and original/stripped sizes are in
the linked records. These are frozen products, not a build of every later
dirty-checkout change. The older r30 record retains its original explicit
`--debug-languages=python` selection. A debugger adapter selection does not
select the Guest language or grant ROS access, and a Debug Runtime build does
not by itself qualify source debugging.

Generate the tutorial policy for the **actual held Worker PID** and a new private
socket directory. Replace every `ABS_*` and identity placeholder explicitly:

```sh
python3 sdk/tools/example_policy.py --backend 2 \
  --socket ABS_PRIVATE_DIR/g.sock --worker-uid WORKER_UID \
  --worker-pid ACTUAL_HELD_WORKER_PID --gateway-uid GATEWAY_UID \
  --node-name moonbit_gateway --namespace /uwvm_telemetry \
  --resource /uwvm_telemetry/telemetry --operation publish --domain-id 206 \
  --disabled-security-tutorial --out ABS_NEW_POLICY_OUTPUT
```

The generator writes reviewable Worker arguments and native Gateway config.
Its explicit `pub` grant fixes String identity, one publisher/inflight request,
4096-byte messages, 10 Hz, burst 4, queue/byte quotas, reliable/volatile keep-last
QoS depth 4, and bounded allow/deny auditing. The two explicit native session
slots accommodate preflight retirement overlapping the first business session;
they add no operation, resource or principal. Worker transport/authentication
fields stay outside native backend connection identity. Review all generated
files before the owner starts either program.

The owner must create the Worker first in a held state, generate config for its
actual PID, start the Gateway, verify its private socket and peer identity, then
release the Worker. Replacing the Worker PID with a shell PID breaks the binding.
Execute the app with `-Rint`, the generated `Worker_arguments`, the exact ROS2
business namespace, and the explicit Wasm features required by the producer.
The complete r59 and new r94/r95 argv are retained in the command records. Enabling ROS without a
reachable, correctly bound Gateway fails preflight before the Guest starts.

The Guest stdin is a two-byte receipt protocol, controlled by the independent
owner. After it emits `READY_MOONBIT_NATIVE_TELEMETRY`, wait for the peer's exact
`MATCHED_NATIVE_TELEMETRY` record and send **one ASCII `P` byte without newline**.
The Guest publishes the final message and emits
`AWAIT_NATIVE_TELEMETRY_DELIVERY`. Only after the peer reports the exact delivered
value and matching GID may the owner send **one ASCII `A` byte without newline**.
The Guest closes its publisher and session, prints
`PASS_MOONBIT_NATIVE_TELEMETRY`, and exits. Short reads, EOF or wrong gate bytes
abort. Merely observing a successful publish return is insufficient: the earlier
r54 attempt lost its GC/Release final message when cleanup preceded delivery.

Keep complete bounded stdout/stderr, all three exit statuses, native graph/GID
records, denial receipts, deadlines and checked owned cleanup. The native peer
remains alive for its quiet interval after receipt; the Gateway observes Worker
exit through its PID binding. There are no success-path cleanup signals in the
recorded eight old and sixteen new runs.

## Qualification boundary

This functional fixture explicitly uses `disabled-test` DDS security on an
isolated test domain. It does not qualify SROS Enforce, vendor authentication,
ROS1/mixed delivery, every SDK operation, official client-library parity,
managed-language debugging, other architectures, QEMU or VTune performance.
The native Gateway, middleware, loader and compiler are trusted native code.
Observed executable mappings are evidence of the code loaded during this run,
not proof of every future loader path or authenticated artifact provenance.
