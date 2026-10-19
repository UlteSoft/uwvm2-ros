# C and C++ guest development

The SDK imports ABI v1 from `uwvm_ros1_v1` or `uwvm_ros2_v1`. It requires no
native ROS headers/libraries and supplies no authority by default. The current
runtime registers exactly 38 ROS1 imports and 43 ROS2 imports. The machine
contract is [`../abi/abi-v1.json`](../abi/abi-v1.json); the full declarations remain
in `ros1/abi.h` and `ros2/abi.h` and are public through `include/roswasm/abi/`.
Do not import invented native ROS functions or use a different signature.

Choose a language layer according to its ownership and allocation requirements:

| Layer | Include | Requirements | Purpose |
| --- | --- | --- | --- |
| Raw C ABI | `roswasm/abi/ros1.h` or `ros2.h` | C11 or C++ | Every registered import; explicit memory index |
| Freestanding C | `roswasm/ros1.h` or `ros2.h`, then `roswasm/node.h` | C11, stdint/stddef | Explicit owners, caller buffers, typed codecs and fixed registrations |
| Freestanding C++ | `roswasm/abi/ros1_freestanding.hpp` or ROS2 equivalent | C++11, C ABI types | Move-only owner with complete raw-byte operation methods |
| Standard C++ ownership | `roswasm/abi/ros1.hpp` or `ros2.hpp` | C++20, span/string_view | Legacy move-only raw-byte operations |
| Typed C++ | Selected `roswasm/ros1.h` or `ros2.h`, then `roswasm/roswasmcpp.hpp` | C++20 standard library | Node, typed resources, complete codecs, guest-driven callbacks |

Keep the SDK directory layout intact because public entry headers forward the
stable declarations and complete codec library. Add `sdk/include` to the include
path. Include exactly one backend for a ROS1-only/ROS2-only Worker. The typed
headers contain no backend imports themselves; the selected backend dispatch
references only that version. A deliberate dual-backend guest may include both
headers and create two contexts. One backend's handles never authorize the other.

## Start with imports

Read `examples/c/01_raw_imports.c` or `examples/cpp/01_raw_imports.cpp` first. The
example opens alias `pub`, reads its eight-byte handle, publishes a canonical
String, and closes once. Alias names are short host-configured capability keys,
not topic/service/parameter/frame selectors. The host must explicitly enable
that target and grant that alias with its exact operation, resource, builtin type
identity, quotas and applicable QoS/limits. With no grant, opening returns13 and
creates no ROS endpoint. ABI-version reads alone confer no permission.

All memory indices are i32; offsets, lengths, handles and output lengths are i64
on both wasm32 and memory64. Handles/lengths occupy eight little-endian bytes,
including on wasm32. Never pass `size_t*` directly as the output length slot.
The host copies input during the import and never stores a guest pointer for a
native ROS callback. Raw imports accept explicit multi-memory indices; pointer
convenience helpers select memory0 and therefore require pointers into memory0.

Success makes output usable. Error11 means queue-empty/rate-limited work;13 means
denied;9 means stale/foreign/retired handle;17 means conflicting configuration;
22 means invalid input;90 means a byte/quota limit;95 means unsupported;107 means
disconnected;110 means timeout;125 means a revoked alias. Unknown-module2 is
also represented. Revocation retires existing handles (later use returns9) and
reopening the revoked alias returns125. Inspect the return from each operation.

Error90 may occur before the host writes a length. New wrappers sentinel-fill
an internal eight-byte slot and expose `has_length` only after an actual write;
legacy wrappers preserve caller length when90 left the slot untouched. A written
required length does not establish that an event/RPC can be repeated: the native
operation or dequeue may already have happened. Buffers must cover the granted
complete payload quota before the call. The SDK does not automatically grow,
replay, retry after timeout, or re-open aliases.

## Add canonical codecs and ownership

Layer2 examples show `roswasmc_capability` and `roswasmcpp::Capability`. C code
initializes each owner, opens one alias, uses caller-owned scratch storage and
closes once. Do not copy C owners; copying the struct does not duplicate the
host handle. C++ owners are move-only, close on destruction, and relinquish the
moved-from object. Context/Node must outlive dependent resources.

The runtime may retire a handle before native close reports a transport failure.
New explicit close/revoke wrappers relinquish their local owner after one attempt;
a nonzero return is not evidence that a handle remains live. Legacy close wrappers
retain their historical behavior and return numeric errors; applications should
not infer native resource lifetime from that local cached handle alone.

C `codec.h` has bounded writers/readers and typed String/Bool/Twist/Image/Transform,
all scalar/array parameter forms, legacy and typed/deletion parameter events,
Trigger/SetBool responses, Fibonacci goal/feedback/result, and Lifecycle state.
It uses no allocator or hosted C library. Canonical scalar String bytes retain embedded NUL (U+0000); StringArray elements
remain NUL-free. Native DDS interoperability for embedded NUL must be tested for
the selected adapter/RMW; canonical encoding alone does not establish it.
Decoded views borrow the exact supplied buffer; copy bytes before reusing that buffer. Array values expose canonical
count-prefixed wire views. Build an array with the bounded writer, validate it
with `roswasmc_decode_parameter`, then pass that value to typed parameter write.
Signed integer bits are explicit two's-complement: int32 uses its low32 bits,
int64 uses all64. Numeric bounds remain a host-policy decision.

C++ exposes the complete established `uwvm_ros::` codec library as
`roswasmcpp::wire`, including owning scalar/array values and typed parameter
v2/deletion events. `ParameterCodec<T>` provides typed scalar and array mapping;
`BooleanArray` avoids `vector<bool>`'s packed representation. Resource value decoders require exact wire consumption; high-level destination
values are preserved on malformed input. The established low-level codecs are
packing/structural decoding helpers, not complete host admission validation.
Typed String/Twist/Transform wrappers additionally check UTF-8/finite values;
Image stride/encoding/timestamp rules, TF frame/bounds/quaternion permissions and
all per-alias limits remain host checks. Low-level C writers may modify scratch before failing, so only
use scratch bytes when the encoder succeeds. Encoders do not grant rights.

The canonical payload limit used by these codecs is4193786 bytes (4MiB minus a
conservative IPC envelope allowance). IPC-frame payload limit4194304 is distinct.
A host-granted complete message/event/response quota may be much smaller.

## Add Node and guest-driven callbacks

Layer3 examples create a guest-local Node and independent `pub` and `sub` owners.
A Node label is diagnostic text; it does not select a native node identity. Native
namespaces/names remain host configured. `Executor::add` borrows a source; remove
its registration before moving/destroying the resource. During a pass, `add` returns0 without insertion and `remove`/`clear` are safely
ignored. Recursive `spin_some` returns22 without changing its delivered argument.
Defer changes until the pass ends; every source must outlive the entire pass, even
if its callback attempted removal. In an exception-enabled C++ build, a callback
exception resets the executor busy flag before propagating to its caller. In the
no-exception WASIp1 tutorial profile, callbacks must not throw. Numeric SDK/ROS
refusals retain the same behavior in both profiles.

`spin_some(maximum_events, delivered)` polls each registered source at most once
and invokes callbacks on the guest caller only. Error11 is idle work and is
skipped; other errors end the pass. Native ROS threads only queue events inside
the gateway. The SDK starts no thread, sleeps on no ungranted clock, and never
busy-spins automatically. A real application schedules later passes using its
own explicitly permitted execution mechanism. One example pass does not prove
DDS discovery or guarantee asynchronous delivery.

## Supported resource families

| Resource | C helpers | C++ typed API | Required explicit alias |
| --- | --- | --- | --- |
| Topics | create_publisher/subscription; typed publish and event codecs | `Publisher<T>`, `Subscription<T>` | Separate topic.publish/topic.subscribe |
| Trigger/SetBool | trigger_call/set_bool_call, advertise/poll/request/reply | `ServiceClient<Service>`, `ServiceServer<Service>` | Separate call/advertise/respond |
| Fibonacci Action | send_goal/cancel/receive/start/poll/reply | `ActionClient`, `ActionServer` | Each Action operation separately |
| Parameters | parameter_read/write and complete named raw resource methods | `Parameters`, `ParameterCodec<T>`, event `Subscription<T>` | One exact node/name/type and operation per alias |
| Graph | graph_read and inspect/list methods | `Graph` | One host-filtered inspect/list projection |
| Time | clock_read | `Clock` | One clock operation; no WASI grant inferred |
| Logging | log_string | `Logger` | One host-selected logger/severity |
| TF | tf_read/tf_write | `TF` | Separate lookup/broadcast frame-pair grants |
| Lifecycle | lifecycle_read/change | `Lifecycle` | ROS2-only separate get/change grants |

Service and Action server request/goal IDs are assigned by the gateway and checked
against the advertisement owner. Respond grants must match its resource/type and
applicable QoS contract. Cancel-any is an independent stronger alias, never
inferred from cancel-own. `ActionClient::cancel_all`/`roswasmc_action_cancel_all`
encode zero only through that cancel-any operation; own/feedback/result IDs remain
nonzero. Action clients wrap one chosen operation at a time;
keep independently opened send/cancel/feedback/result owners alive as needed.
An Action send helper's returned ID is not transferable between guest instances.

Supported typed adapters are the implemented builtins, not arbitrary user IDL.
ROS1 parent-namespace parameter search remains the runtime's exact-name safe
subset. Unsupported backend operations return95; no fake implementation is
substituted. This API is inspired by familiar ROS Node/resource/Executor patterns
and does not promise rclcpp/roscpp source compatibility or native feature parity.

See [the advanced example guide](../examples/c/ADVANCED.md) for complete
service/Action server, control/query, scheduling and Session applications.
Their executable `main` paths select one documented mode and explicit aliases;
the guide includes complete policy generation and required native endpoints.
Default Runtime authority remains empty and acquisition returns denial13.
The recipes have explicit Wasm export names, including `example_backend` for
constructing a guest-side SDK context with the selected immutable dispatch table.
This retains callable educational operations under optimized Wasm linking.
Pointers/references designate initialized guest objects and bounded guest storage;
the exports are not a native object-pointer API or a JavaScript marshalling
contract. Link the source into an application that owns those lifetimes, or
define a separately checked embedding protocol. An export acquires no authority.

## Build and validation status

With a configured WASI SDK, a representative C build is:

```sh
"$WASI_SDK_PATH/bin/clang" --sysroot="$WASI_SDK_PATH/share/wasi-sysroot" \
  -std=c11 -O2 -I sdk/include -DROSWASM_EXAMPLE_ROS_VERSION=2 \
  sdk/examples/c/01_raw_imports.c -o raw-c-ros2.wasm
```

Use the corresponding WASI C++ compiler, C++20, `-fno-exceptions` and its
standard-library/sysroot for the typed WASIp1 tutorials. The compiler matrix
selects this profile explicitly, so these examples need no Wasm exception ABI
or exception runtime imports. Numeric SDK/ROS errors still report permission,
malformed input, quota, transport and other ordinary refusals without throwing.
Callbacks must not throw. Owning standard-library buffers/callables still
allocate, and allocation exhaustion or target-library termination is not
translated into a recoverable ROS error. Use the caller-buffer C or freestanding
C++ layer when allocation-free operation is required. Native exception-enabled
applications and native model fixtures keep their existing unwinding behavior.

Change the macro to1 for ROS1. The base raw import and
freestanding ownership layers do not require a native ROS SDK. `sdk/tools` is the
integration entry for authoritative import/wire checking; inspect emitted Wasm
imports so the selected backend and ABI i64/i32 types are exact. Toolchain paths
above are configured inputs, not a claim that a toolchain exists on this machine.

The new sources/examples and pure model fixtures are prepared. Actual compilation,
model execution, Wasm interpreter/native ROS tests and aarch64/QEMU validation
must run in the existing authorized Linux 64 GiB cgroup with zero swap. Source review or a pure
codec/model result cannot establish actual ROS networking or ARM behavior.

## CMake integration

Use `add_subdirectory(sdk)` and link `roswasm::c` for the C headers or
`roswasm::cpp` for the C++20 headers. An installed SDK can be found with
`find_package(roswasm CONFIG REQUIRED)`. The installation preserves the complete
SDK tree under the data directory so public forwarding headers retain their
relative includes. These INTERFACE targets add headers/compile requirements only;
they do not compile/link native ROS, grant permissions or choose an import backend.
Choose the backend in application source as described above.
