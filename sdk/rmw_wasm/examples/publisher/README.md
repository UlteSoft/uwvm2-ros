# Original RMW C publisher with explicit Wasm transport

`checked_publisher.c` is a complete standalone bootstrap and publisher application
using the original `rmw`, `rcutils`, `rosidl_runtime_c` and generated `std_msgs` C
APIs. No external deployment function or weak stub is needed. It selects the
reviewed concrete five-import publisher transport and the real Guest-local graph
guard. Source selection grants no authority: the actual Host must separately
admit the exact configured ROS2 alias, resource, identity and limits. This source
has not been compiled or run and supplies no new native/Wasm acceptance result.

Use one complete original SDK rebuilt with the composed A/B/C and publisher
transport sources. Do not put `sdk/compat/rclc/include` in its include path; those
independent facade headers are not the upstream RMW ABI. Rebuild every consumer
after any descriptor layout change, including the optional C++ codec validation
state tail. The installed static `rmw_uwvm` must come from that same source tree.

The four application arguments are:

1. The exact explicitly granted `topic.publish` alias. There is no fallback.
2. Its absolute topic name, selected by both this profile and the Host policy.
3. The 64 hexadecimal RIHS01 digest digits from an independently exported genuine
   native `std_msgs/msg/String` type. Omit the `RIHS01_` presentation prefix. The
   real generated type-support accessor and codec validate the complete original
   graph/hash before the first capability open. A digest is data, not authority.
4. The decimal domain ID matching the independently configured Host target. This
   Guest argument cannot create or change a native Gateway domain.

The selected local node is `checked_rmw_publisher` in `/`. The fixed application
QoS is KEEP_LAST/depth10, RELIABLE, VOLATILE, AUTOMATIC liveliness, normal ROS
namespace conventions and the original `RMW_DURATION_INFINITE` for deadline,
lifespan and liveliness lease. Configure the Host with the corresponding explicit
values, including `qos.deadline=infinite`, `qos.lifespan=infinite` and
`qos.liveliness-lease-duration=infinite`. Effective QoS is an expected selection;
it never replaces the actual retained publisher's getter result. A mismatch
fails and keeps the binding for its once-only cleanup. It does not negotiate
arbitrary defaults, silently rewrite a requested QoS or activate a different
legacy publisher.

The source selects a1024-byte canonical bound, including the four-byte String
length. Explicit Host `max-message-bytes` must accommodate the actual11-byte
canonical message; the SDK bound does not enlarge that grant. The payload is
exactly seven bytes `77 61 73 6d 00 72 6f` (hex), including an embedded NUL, plus
its four-byte length on the canonical transport. Compare native String length
and every byte; console text or `strlen` cannot prove delivery. The true
canonical size is11 bytes, so configure a Host maximum of at least11 bytes.
All other mandatory topic/audit/queue/rate/inflight/connection/target fields remain
explicit under [the normal capability schema](../../../../documents/ros/capabilities.md)
and [CLI documentation](../../../../documents/command-line/ros2.md). This
example does not generate CLI rights, configure a Gateway or enable ROS1.

A native receiver must already be initialized and matched before the application
is launched. This application deliberately does not invent an acknowledgement,
sleep for an estimated discovery delay or label successful `rmw_publish` as
successful network delivery. The final marker attests only submitted publication
and checked cleanup within this process. Actual acceptance must correlate the
same retained publisher GID with a separately owned native receiver's complete
value record, and observe the publisher disappear after retirement. Host/Guest
source ownership, denial, deadlines and cleanup need their own real witnesses.

For a cross build, use a new owned output directory in `/tmp` on the local
machine (or the assigned bounded Linux cgroup). After the genuine original SDK
prefix and actual selected toolchain are independently prepared, the command
shape is:

```sh
cmake -S "$source_root/sdk/rmw_wasm/examples/publisher" -B "$build_root" \
  -DCMAKE_TOOLCHAIN_FILE="$actual_wasi_toolchain" \
  -DCMAKE_PREFIX_PATH="$actual_original_sdk_prefix" \
  -DUWVM_ROS_SOURCE_ROOT="$source_root"
cmake --build "$build_root" --parallel 1
```

These commands are instructions, not executed test results. Provide the actual
reviewed source and prefix paths; they are not downloadable SDK promises. The
CMake project adds the real optional transport component and links the genuine
static RMW and original generated libraries. It refuses local raw model mode.
No blanket undefined-import workaround or fake native raw symbols are supplied.
Qualify the entire final compiler dependency/object/library/link closure, fully
validate the Wasm and check actual import signatures before Runtime execution.
The immutable ROS2 backend references the selected original business ABI as well
as the five new `uwvm_ros2_rmw_v1` imports. Imports alone confer no permission.

The application checks arguments and complete native codec identity before raw
effects. Provider slots are allocated first and remain in static owned storage.
It then installs the immutable profile, initializes real options/context, opens
the selected logical node/publisher, queries actual QoS/GID, initializes a real
String, publishes once and performs reverse cleanup. Each successful local
initialization is finalized. Both native and raw-owner cleanup diagnostics are
checked before context finalization; provider diagnostics are copied before
local transport destruction. A native retirement error cannot skip raw source
close. If profile uninstall refuses a still-live borrower, static storage stays
alive until process/Host drain; it is never freed under an original context.
Arbitrary traps and uncertain native effects still require independent bounded
process/session cleanup.

No additional WASI filesystem/network/clock/environment grant is needed by this
direct-RMW source. Standard in/out/err remain the basic Runtime streams. Original
`rcl` applications must additionally prepare their actual clock/logging platform
and preserve their own ROS side effects; this example does not disable those
features or claim full rcl/rclc/rclcpp/rclpy/rcljava compatibility. Actual native
SDK, Wasm execution, all requested distributions/vendors/architectures and
performance/security qualification remain required.
