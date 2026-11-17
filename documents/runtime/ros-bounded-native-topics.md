# Bounded native ROS 2 topic adapters

ROSv2 can bind a frozen bounded CDR schema to a generated ROS 2 message type in
an explicitly built, isolated communication worker. The adapter checks the native
representation and type hash before creating a context or participant. A type
name, a valid schema or an enlarged type catalogue alone cannot install a native
factory. Services and actions still require their separately implemented native
adapters; this topic path does not make their schemas generic.

## Trusted build and deployment

`make_bounded_ros2_schema.py` reads one generated ROS type-description JSON file
and resolves its complete nested type closure within that file. It accepts
explicit sequence, string, message-byte, traversal and depth limits, and an
explicit decision about nonfinite floating-point values. It does not follow
includes, evaluate source expressions, search package paths or execute code.
Its output includes the source hash, complete schema and upstream ROS type hashes.
These are configuration artifacts, not capabilities.

`make_bounded_topic_registry.py` takes a finite list of generated message type
descriptions and an explicit comparison-work bound. It emits a native C++ header
with generated type instantiations, exact upstream type hashes and literal
type-support library names. The worker build selects that header through
`UWVM_ROS_BOUNDED_TOPICS_HEADER`. This is trusted compiler input; there is no
corresponding guest import, VM path-selection option or dynamic registration
operation. The ordinary worker packaging step must include and pin the full
ELF/type-support/resource closure, including these literal preload libraries.

Before confinement, the worker prepares factories only for bounded types
referenced by its approved topic definitions. Each factory verifies:

- The exact top-level ROS package/category/type identity.
- Ordered field names, scalar kinds, nested records and fixed array sizes.
- Explicit sequence and string bounds that do not exceed upstream bounds.
- The independently selected upstream type hash in both introspection and
  serialization support. Missing, zero and mismatched hashes reject.
- Supported CDR layout, a finite comparison budget and the immutable descriptor.

Keyed types and unsupported primitive/layout forms reject. No guest-supplied
offset, callback or native pointer is dereferenced. A build without a bounded
registry rejects bounded topic definitions before creating a ROS context.
An unused type description may still be inspected without starting a worker.

## Runtime authority and data flow

Prepared factories are inert. `node.open` and endpoint creation still require
the existing independently granted context, node, type and topic origins. Native
factory matching includes the exact frozen profile and descriptor. All grants
start absent. Stable authorized execution remains separate from optional,
explicit attenuation; the adapter does not introduce a rate limit or new scope.
Closing or revoking a capability uses the ordinary retirement and cleanup path.

The generated typed publisher and subscription use rclcpp's serialized-message
API for bounded topics. This keeps the actual generated ROS type support while
avoiding construction and reserialization of native message objects merely to
carry CDR bytes. Before publication, the codec validates the complete canonical
input and checks the deadline. A native exception after publication begins
reports an unknown effect and never triggers an implicit retry.

On receive, the codec validates the middleware's owned serialized buffer and the
explicit message-size bound before copying into the ordinary receive queue.
The buffer remains borrowed only during the callback. The queue retains its own
bounded storage; no middleware pointer is returned to a guest. CDR byte order
remains explicit in the payload and is preserved by this path.

Jazzy/FastDDS can report a serialized extent rounded to four bytes even when its
encapsulation options are zero. Native ingress accepts at most three trailing
padding bytes, and only when the rounded extent contains an otherwise complete,
valid schema instance. Those padding bytes are never delivered to the guest.
Guest publication always uses strict canonical validation and rejects trailing
bytes. ROS1 body conversion remains an independently authorized bridge workflow.

The underlying object codec remains available for native adapters that actually
need generated objects. It uses caller-owned serialization buffers whose
allocator callbacks refuse replacement or growth. VM-owned state, schema graphs
and factories use recoverable fast_io allocation. Middleware caches, generated
native object storage and rclcpp's mandated factory/destructor pairs are external
ABI storage and remain subject to isolated-process limits. This is not a
zero-copy or bounded-middleware-allocation guarantee.

## Qualification and performance scope

The `bounded-native-type-support-serialized` cohort validates 31 generated Jazzy
types against their independent type-description JSON, native introspection and
actual RMW serializers. Clang ASan/UBSan and GCC exercise both byte orders,
strict truncation/tail rejection, wrong names, kinds, collection forms, bounds,
type hashes, descriptor mismatches and exhausted comparison budgets. The set
includes nested poses, covariance arrays, multi-arrays, joint states, laser
scans, images, IMU messages and point-cloud descriptions. It is a codec receipt,
not a qualification of every source package in the acquired corpus.

The `bounded-native-private-serialized` cohort exercises three generated
types between two sealed Jazzy/FastDDS workers: PoseWithCovariance,
Float64MultiArray and JointState. It verifies both directions and byte orders,
rejection before opening and after closing, malformed-input rejection, retained
graph isolation, native peer-death fencing and confirmed pidfd cleanup. The
existing String/Bool, SetBool and LookupTransform paths also run in that cohort.
That receipt exercises the serialized endpoint path. It does not qualify generic
service/action type factories or external authenticated DDS peers.

The `bounded-native-type-performance-profiled` cohort compares actual RMW object
round trips with the bounded object codec, using identical serialized inputs,
fresh generated objects and preallocated output capacity. At 1,024 finite doubles,
batch medians were 1,114.24 ns for raw native, 1,331.79 ns for native with a finite
check, and 1,410.99 ns for the bounded codec. The latter validates both wire
boundaries and exact extents; the native comparator scans its object once.
At 65,536 values the corresponding medians were 24,298.25, 37,050.37 and
38,558.63 ns. These are serialization costs, not robot request latency.

Local benchmark and adapter code use `-O3 -g`; the packaged ROS dependencies are
hashed, but their original compiler flags are not asserted. Official hardware
VTune Hotspots and Microarchitecture collections retain sampled C++/assembly
evidence. Measurements run on CPU 8 inside the existing shared 64 GiB zero-swap
cgroup, with inherited first-touch memory placement. They make no NUMA isolation,
quiet-machine, realtime, full-VM performance or industrial certification claim.
