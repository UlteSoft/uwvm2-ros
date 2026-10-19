# Genuine C ownership in the fixed original rclcpp service path

This explicit source port repairs the dynamic ownership of the genuine C
`type_description_interfaces__srv__GetTypeDescription_Request` and `Response`
used by the fixed original Node type-description service. It preserves the real
Service/AnyServiceCallback API, NodeOptions, TypeSupport selection and callbacks.
It supplies no endpoint authority. The option is OFF by default. All authored
contracts and build steps below are unexecuted source, not qualification results.

The intended Guest route is original C/C++ source rebuilt to Wasm with the real
selected toolchain and RMW bridge, then the uwvm2 capability imports. C pointers
here point to actual typed objects inside that Guest's linear memory. This is
not a native ROS ABI between Guest and Host; no Host pointer, JNI, synthetic
TypeSupport identity or additional Guest import is introduced.

## Why zero storage and a finalizer must be separate

At rclcpp commit `53fa7aeea86e8ed484f9465fa14059faf13a71f1`,
`Service::create_request()` and nondeferred `AnyServiceCallback::dispatch()`
allocate with `std::make_shared<T>()`. That is correct for the generated C++
types, but a plain C aggregate does not have a destructor for its owned strings,
sequences and nested descriptions. An erased shared reference can outlive the
callback; freeing in the codec or immediately after response encoding is wrong.

At rcl commit `18d96c9c2b98e616324ae9a2c7701dd0b0886ff0`, the actual
`rcl_node_type_description_service_handle_request()` validates its pointers and
then calls `GetTypeDescription_Response__init(response)` itself. Eagerly using
`Response__create` or calling `__init` in the shared allocation would initialize
the response twice and overwrite live dynamic allocations. This port therefore
value-initializes a real C subobject and does not call either C initializer.

Only the two exact C types select `detail::zero_owner<T>`. The actual typed
subobject has a stable address inside a standard shared owner. The returned
aliasing `shared_ptr<T>` shares that control block; erased references, weak locks
and callback captures retain the same owner. Its destructor invokes the exact
linked C `__fini` once when the last shared owner disappears. Deferred original
callbacks still allocate no response and may retain the request normally.
Every other `T` follows the literal original `std::make_shared<T>()` branch.

Zero storage is **valid ownership storage**, not a fully initialized message or
its declared defaults. In particular the request's declared
`include_type_sources true` is supplied by the real decoder/initializer, not by
the zero allocation. No value may be published before actual decoding succeeds.
The proven C generator/runtime chain accepts null string/sequence data only with
zero size/capacity. Static C representation traits alone are insufficient proof;
the preserved original functions/template and complete actual schema closure
are part of this narrow port's admission.

The stage2 genuine C decoder must privately decode the complete canonical value,
initialize a private real C temporary exactly once, fill all fields, atomically
swap its complete owned state with the valid destination, and finalize the old
destination. Failed decode must leave the original destination unchanged. The
first request destination may be this zero-valid object. Response encode reads
owned fields and never frees them. The actual original handler is the response's
single initializer. C callbacks must not throw through a C ABI. Generated
`__init` may invoke its own rollback finalizers on failure; this internal cleanup
is distinct from the shared owner's one finalizer invocation. Terminal traps or
forced instance termination still require actual Host instance drain.

## Exact source attachment

The included whole adapted headers and `upstream/original-rclcpp-c-lifecycle.patch`
apply only to a fresh copy of that fixed rclcpp repository. Before/after hashes
and the unchanged original `rclcpp/CMakeLists.txt` hash are in
`upstream/port-plan.json`. Apply to the repository root, not an installed include
prefix. A source mismatch must refuse; do not force or fuzzy-apply the patch.
The other signal/platform source port is disjoint and must be composed explicitly.

Build/install this helper with the actual cross toolchain and static generated C
libraries using the explicit `STATIC_WASI` mode. Its function-scoped probe resets
`CMAKE_TRY_COMPILE_TARGET_TYPE` to `EXECUTABLE` and takes all four real init/fini
addresses, so a toolchain's static-only shortcut cannot satisfy the linker check.
No `try_run` is used. Missing actual generated headers/functions or libraries is
a configure failure, not a stubbed success.

The installed `cmake/FixedOriginalPort.cmake` exports:

```cmake
# Run after the genuine original add_library(rclcpp ...), e.g. a Root-owned
# deferred project hook. The helper writes no upstream file.
uwvm_attach_original_rclcpp_c_service_lifecycle(rclcpp)
```

It checks the actual source-built target's SOURCE_DIR and the complete hashes of
the unchanged original CMakeLists and both adapted headers. Imported/alias targets
are refused. The enabled installed target is
`uwvm_rmw_c_service_lifecycle::uwvm_rmw_c_service_lifecycle`; it must have exported
`UWVM_C_LIFECYCLE_LINK_ADMITTED=1`. PUBLIC linkage propagates the real dependencies
and `UWVM_RCLCPP_C_LIFECYCLE_PORT=1` into original template consumers. Rebuild the
whole rclcpp/library/application closure; mixing already-built template consumers
or claiming binary compatibility is unsupported. This function alone does not
remove the separate Root composite project hook's fail-closed refusal.

A source contract can require both actual adapted header markers equal to 1:
`RCLCPP_UWVM_SERVICE_C_LIFECYCLE_HEADER_VERSION` and
`RCLCPP_UWVM_CALLBACK_C_LIFECYCLE_HEADER_VERSION`. Defining a marker manually does
not satisfy the whole-source checks or actual link probe. The helper's public
source function is `uwvm_rmw::c_lifecycle::make_service_object<T>()`.

## Authored real-SDK contracts and example

`contracts/ownership_contract.cpp` contains fourteen groups: zero request and
response storage; late request and complete-response references; original callback
success and throw; both deferred callback forms; actual shared-allocation failure;
actual generated C init failure; private whole-owned commit; real generated C++
message behavior; generic C++ behavior; and the actual handler's null-node
preinitialization return. `header_compile.cpp` instantiates the real original
Service request allocation against an actual Service reference without inventing
a Node, service handle or TypeSupport. The full-response fixture owns all five
response fields, nested descriptions, fields, sources and key/value strings.

These Linux-native contracts require genuine static generated C, runtime C and
rcutils libraries so `--wrap=malloc` can fault the actual allocator chain. Fini
wrappers delegate to their real symbols; no ROS function is replaced by a success
stub. If the actual chain does not consume the fault, the contract fails. Original
C init rollback may also call fini, so that case does not claim only one total
fini call. Optional ASan/LSan is separate real instrumentation, not a fake native
counter. No test is automatically run by CMake. The small `owned_values.cpp`
lesson demonstrates actual C functions and last-reference ownership only; its
marker does not prove a ROS request, default Node, Native peer or Guest execution.

## Remaining qualification and authority boundaries

All source is unexecuted. Real generated SDK headers/libraries, complete stage2
canonical/default/TypeDescription/RIHS/layout validation, source-built rcl/rclcpp,
thread/signal/wait dependencies, default-control endpoints, policy grants and the
actual selected Wasm producer/import/memory checks must be composed and tested.
An unchanged default Node still creates all its real default endpoints and must
receive every explicit grant or fail through actual RCL. This port does not
quiet the Node, infer grants from type names or supply a default authority list.
Native local ownership checks, Guest model checks, Host deny checks and positive
Native transport qualification must be reported separately. No full rclcpp or
all-C-message compatibility is claimed; other C types require their own complete
schema/init/fini proof before any extension of the exact dispatch.

The source evidence uses newly reacquired durable files. Historical content
hashes may match prior observations, but old lost `/tmp` device/inode observations
are never relabeled as current. See the private source package's primary and
reference inventories for the actual physical observations.
