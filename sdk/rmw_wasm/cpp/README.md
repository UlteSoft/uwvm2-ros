# Real generated C++ codecs for the upstream RMW bridge

This is a source-only implementation over **actual** generated ROS2 C++ types,
not another `rclcpp` facade and not substitute `std_msgs`/`std_srvs` headers. No
candidate was imported, generated, compiled, tested, executed or sent by SSH.
The separately reviewed B5 and C2 source remain their own historical evidence;
this candidate has no execution credit and is not a complete upstream RMW.

## Implemented type and native graph closure

The first concrete families are `std_msgs::msg::String`,
`std_srvs::srv::Empty::{Request,Response}`, and
`example_interfaces::srv::AddTwoInts::{Request,Response}`. The actual Empty and
AddTwoInts service roots also validate their **complete** modern service graph:
Request, Response, Event, `service_msgs::msg::ServiceEventInfo` and
`builtin_interfaces::msg::Time`. There are eleven selected original native
RIHS01 identities, nine real message TypeSupport objects and two real service
TypeSupport objects. The application receives seven contiguous business codec
descriptors. Events and native metadata are dependencies, not extra ROS grants.

The implementation includes the real generated C++ public headers and actual
`rosidl_typesupport_cpp` / `rosidl_typesupport_introspection_cpp` accessors. It
warms and verifies the actual generic accessor's resolution to the actual
introspection accessor. Later validators accept only those retained actual
accessor objects, not a matching type name or a pointer to a made-up wrapper.
There is no C-object reinterpretation or arbitrary namespace dispatch. Actual
static forwarding remains A's required original build path; no fake `dlopen`
is provided.

Validation checks each exact original name/namespace, full member count, actual
`sizeof` and `offsetof` from the included generated C++ definitions, primitive
signedness, unbounded native String, no key/default-field metadata, complete
nested member links, fixed16-byte client GID, bounded1 Request/Response Event
sequences, and all scalar/array callback-presence rules from the real generator.
It does not assume a particular numeric `std::string`, vector, allocator or
native object layout. No callback is invoked to pretend that a malformed
introspection graph is a valid object. Native field sizes/capacities remain
separate from canonical transport bounds.

Every actual native hash callback must return modern version1 and match the
complete explicitly supplied selected identity. Missing modern callbacks do not
fall back to a legacy or typename-only route. Every actual TypeDescription is
checked against the complete original field/default/type/capacity/name graph,
including the exact duplicate-free transitive reference inventory. Source
metadata is bounded and must have the complete same graph. The original
`implicit` service component source has an empty source body, as the actual
pinned description generator specifies; it is not falsely rejected or replaced
with invented IDL. Root source encodings remain actual `msg`/`srv`.
The actual generated description/source strings are immutable bounded views
whose static `capacity` may equal `size`; they are not treated as owned runtime
String allocations. Matching reads only those exact bounded bytes, without an
out-of-range terminator probe or an assumed `size+1` capacity.

## Source contract and storage lifetime

`uwvm_rmw_codec.validation_state` is an explicit immutable trailing pointer owned
by its validator. It does **not** replace or overload `generated_support`, which
still points at the real native TypeSupport. The nonmoving `Registry` owns its
state, all eleven selected hashes and seven descriptors. No global map, guessed
container relationship, mutable plugin registration or native identity boolean
is used. Foreign/copied descriptor storage is refused instead of silently losing
its owning state association.

This descriptor is a **source-recompile-only seam**, not a stable binary ABI.
Its existing `version=1` selects the source callback contract; it is not an old
layout size/version acceptance claim. Existing C aggregate initializers acquire
a NULL trailing field when compiled from this new header. All actual B, C,
converter, adapter, provider and application consumers must be rebuilt against
this exact current header, with a complete qualified dependency/MD closure.
Old descriptor object files or plugins may not be cast or mixed because their
numeric version happens to be1. Original public ROS TypeSupport structures and
RMW signatures are unchanged. Source-only checks do not establish an old-binary
compatibility claim.

Construct `Registry(expected_identities, string_wire_bound)` before installing a
profile. It allocates its local state first, opens no capability, and eagerly
warms **all** actual metadata, child service members and generic dispatch before
returning. Initialization failures expose no descriptors. Warm it under the
actual owned loader/mapping phase **before** any SIGSTOP/native-map checkpoint;
native default TypeSupport can load libraries or initialize source caches on
first access. Root must qualify those actual libraries and source paths. WASI
uses genuine static forwarding; native contract builds may use the real native
loader under their separately owned process envelope.

The Registry, immutable selection, actual libraries and provider state must
outlive every borrowing options/context/entity. Install only a profile whose
complete codec array points at `registry.codecs()`. After the original
application has genuinely retired all entities/options/contexts, uninstall the
profile and then destroy the Registry. Destroying borrowed storage, changing a
profile in use, arbitrary caller memory corruption or `longjmp` is a caller
lifetime violation. Ordinary C++ allocation exceptions become12; other ordinary
metadata exceptions become22. With exceptions disabled, allocation/VM terminal
traps require real Host instance drain and external process ownership. There is
no claim of OOM immunity or invented native cleanup after a trap.

## Canonical bytes and truthful Empty

String is u32LE UTF-8 length plus exact bytes. Embedded NUL and a BOM are data;
malformed UTF-8 and oversized values are refused before output writes. Decode
validates the complete record, builds an independent `std::string`, and swaps
only after success, preserving the initialized destination on ordinary failure.
The canonical String bound is4..4194304 and does not claim to change the native
unbounded field. Encode receives the selected logical capacity from B/C; decode
must likewise receive a length already checked against that descriptor by the
RMW caller. The type-erased callback signature itself carries no per-call state;
it also enforces the independent whole transport ceiling.

AddTwoInts Request is two signed64 little-endian fields (`a`, `b`), Response one
signed64 (`sum`). Every bit, including both signed extrema, is retained. There
is no addition or overflow policy hidden in the middleware codec. It neither
submits a request nor converts a synchronous call into a Future.

Empty Request/Response use logical canonical extent0. The actual generated C++
objects contain the generator's `structure_needs_at_least_one_member` byte,
which is validated in the native graph and initialized on decode; that byte is
not introduced as a canonical business field. This is supported by the actual
official `Empty.srv` body (`---` only), the actual generated `Empty.idl`, and the
fixed original `rosidl_adapter/resource/struct.idl.em`: its empty `msg.fields`
branch inserts exactly that byte, while the nonempty branch emits original
fields. The complete actual native TypeDescription and RIHS identity still
include and validate the byte; they are not rewritten or treated as an empty
native graph.

This codec explicitly admits only a zero native placeholder on encode. A
caller-written nonzero placeholder returns22 before modifying the output or
length; it is never silently lost. That is this canonical adapter's admitted
profile, not a claim that the public C++ field cannot hold nonzero or that all
native serializers reject it. Decode of the canonical empty value initializes
the actual placeholder to zero. B/C reserve at least one private
scratch byte but pass the original logical capacity0. The encoder writes an
actual length0 and no payload. A nonzero Empty wire record is rejected. Real
C2 service buffers and full UWRE64 envelopes already support this contract;
they are not modified here.

## Actual build and selection

The optional `cpp/attach.cmake` composes with an **already created real** A RMW
target. It does not overwrite A's `cmake/`, generate missing source or replace
upstream headers. Call explicitly after the real target exists:

```cmake
include("<current-source>/sdk/rmw_wasm/cpp/attach.cmake")
uwvm_rmw_attach_cpp_codecs(rmw_uwvm STATIC_WASI)
```

The actual C++20 source and real package targets are required. `STATIC_WASI`
refuses nonstatic generated TypeSupport targets; A must independently configure
the original single-provider static forwarding and static RMW selection. For a
separate installed native SDK **component contract only**, explicit
`NATIVE_CONTRACT` permits actual native targets. The modes are not interchangeable
qualification. No build or configuration has been invoked here.

The attachment accepts either actual installed namespaced package exports or
the actual in-tree `rosidl_generate_interfaces` target. The original
`rosidl_get_typesupport_target` helper requires that in-tree target and is not
called with a fabricated installed package target. For a separately owned
native component build, call explicitly:

```cmake
uwvm_rmw_attach_cpp_codecs(rmw_uwvm NATIVE_CONTRACT)
uwvm_rmw_add_cpp_codec_contract(cpp_codec_contract rmw_uwvm)
```

This creates the real contract executable target but never runs it or registers
an automatic test. Its C-versus-C++ negative case additionally links the actual
generated `std_msgs` C TypeSupport target. A native installed shared SDK cannot
be reused as evidence that the static WASI targets exist or link correctly.

`examples/explicit_registry.hpp` binds only the seven exact codec records into
an otherwise explicit caller-owned B profile. It supplies no node/endpoint row,
provider or permission. `examples/native_objects.cc` demonstrates actual C++
String/Empty/AddTwoInts objects; its default identity selector is absent and
returns13. A source-locked admitted header may provide selected identities for a
real owned component run. No runtime environment/plugin/guest file discovers a
profile or permission.

Business endpoint indices are the public `Codec` enum: String0, Empty root3,
AddTwoInts root6; Request/Response records are referenced components. Every node,
resolved topic/service, native type/identity, QoS and alias remains explicitly
selected and independently authorized by the real Host. Missing actual
activation/inspect/publish/retire, guard/wait/subscription/service or metadata
providers remains a real error. There is no old lazy-publisher fallback.

Preserve the original application's NodeOptions. This first family does not
provide codecs for all default rosout/parameter/event/type-description endpoints
that an unchanged default upstream Node may create. Those are required next
implementations and explicit grants; missing entries must fail through real
RCL/RMW errors. Do not quietly disable them, claim full rcl/rclcpp compatibility,
or turn native metadata, observer state or default stdio into ROS authority.

## Evidence and unexecuted contracts

Official source DATA includes fixed rosidl commit
`85fa592b698b0f665e3120f48fac0d35e2f7d8a4` and rosidl_typesupport commit
`9fcbb58b797b35a69dcc8ba111049a08eaa4aba1`, with actual tree/blob associations.
Official buildfarm generated headers/IDL/JSON were acquired via TLS from
`repo.ros2.org/ubuntu/main`, checked against the actual Packages SHA256 rows and
retained with original `.deb` bytes. Concrete versions are std_msgs/std_srvs
5.3.8, example_interfaces0.12.1, builtin_interfaces/service_msgs2.0.4 and
unique_identifier_msgs2.5.1, built for Jazzy/Noble amd64 on2026-09-11. Only source
and metadata were extracted; no binary library or downloaded code was run.
These are independent primary observations, not current installed SDK or target
qualification. The generated header record is actual DATA, not hand-written
substitute code and not output from a generator run by this author.

The exact original [empty-struct adapter branch](https://github.com/ros2/rosidl/blob/85fa592b698b0f665e3120f48fac0d35e2f7d8a4/rosidl_adapter/rosidl_adapter/resource/struct.idl.em)
and [service adapter composition](https://github.com/ros2/rosidl/blob/85fa592b698b0f665e3120f48fac0d35e2f7d8a4/rosidl_adapter/rosidl_adapter/resource/srv.idl.em)
are retained as separate fixed Git-blob DATA. The
[official generated package index](https://repo.ros2.org/ubuntu/main/dists/noble/main/binary-amd64/Packages.gz)
associates the original archive extents and SHA256 values used here; it is an
observed index, not a future installed SDK pin.

The test identity fixture copies the eleven **published** RIHS01 values from
those actual official JSON documents. It does not calculate native hashes or
select production identities. A different installed SDK is a real mismatch that
requires an independently qualified complete export, not adjusting expectations
until a test passes.

`tests/cpp_codec_contract.cc` contains18 authored/unexecuted component groups:
actual complete native handles/graphs, no implicit selection, seven-descriptor
profile install without open, wrong/missing hashes and bounds, UTF-8/NUL/empty
String and output preservation, zero-wire Empty, complete signed64 Request and
Response, malformed extents, owning-state association, C-versus-C++ native
separation and exact service components. Actual generated objects/libraries and
original RMW functions are mandatory; the only model transport callbacks count
forbidden opens/closes and must remain unused. The marker's check count is the
actual future executed count, not the authored call-site inventory.

Required later evidence is distinct: complete source/header/MD closure with all
consumers rebuilt, actual serial compile/link/component tests, actual static WASI
validation/import/memory association, genuine typed native exporters/counterparts,
explicit default-denied/paired-grant routes, same-incarnation native endpoints,
complete original application behavior and terminal drain. All are unrun here.
