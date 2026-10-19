# ROS2 native SDK compatibility

This layer adapts the native Gateway to the **selected, physical ROS2 SDK**.
It does not change the Guest ABI, attach a distribution name to Guest payloads,
register a capability, enable an RMW, or grant a ROS operation. Distribution
names are provenance metadata. Public headers, complete interface schemas,
actual library links and runtime observations determine availability.

Qualification applies only to the original captured SDK/source snapshots in the
checkpoint records. The Humble amd64 legacy protocol lane now includes a real
production Gateway and native peer; other SDK and architecture lanes retain
their individual incomplete states. The production Node uses
[endpoint-free public SDK composition](README.node.md).
A successful feature probe means that a small target program compiled and linked.
It does not mean that the program ran, that ROS discovery or delivery worked,
or that DDS security is qualified. A matrix report must retain those separate
states. Historical Noetic/Jazzy evidence remains evidence for its original bytes.

## Configuration and feature record

`CMakeLists.txt` finds the explicitly selected ament SDK and invokes
`compat/Probe.cmake`. Select `UWVM_ROS2_CXX_STANDARD=AUTO`, `17`, `20`, or `23`.
AUTO tries the exact effective language standard in increasing order; imported
SDK requirements cannot silently turn an alleged C++17 probe into C++20.
The native layer requires a modern C++17-or-later compiler/target library even
when building against older ROS headers. This is independent of the Runtime's
C++26 compiler and libstdc++ string ABI selection in the external build profile.
No target architecture or ROS distribution threshold chooses an API branch.

All probes are executable **compile and link** checks, never `try_run` checks.
Cached probe results are invalidated when configuring; a prior feature JSON is
removed before admission. The generated `uwvm_ros2_features.h` selects code paths.
The companion `uwvm_ros2_features.json` has exactly four top-level fields:
`format_version` (1), `backend` (`ros2`), `cxx_standard`, and `features`.
Its Boolean feature IDs are:

- `qos_class`, `node_options`, `quiet_node`, `parameter_declare`, `dynamic_typing`;
- `post_set_parameters`, `type_description`, `deferred_service_callback`;
- `legacy_deferred_service`, `enclave_argument`, `action`, `action_core`, `custom_action`, `lifecycle`;
- `action_graph`, `tf_buffer`, `actual_service_qos`, `openssl_rihs`.

An external profile can require an explicit subset and must preserve the actual
record. A missing optional API disables only the operation relying on it. The
exception is `quiet_node`: startup refuses before creating a context or Node
when the SDK cannot disable implicit publishers/services. Creating an automatic
`parameter_events` or `rosout` publisher is not a valid empty-authority fallback.

`UWVM_ROS2_BUILD_PROBES=ON` retains the historical independent witnesses on a
Jazzy-shaped SDK with native TypeDescription support. Older SDKs do not pretend
to compile or run those stricter witness protocols. Missing Action/Lifecycle/TF
packages do not disable unrelated Gateway APIs. Default modern target inventory
is unchanged. `UWVM_ROS2_BUILD_COMPAT_TESTS=ON` adds the explicitly selected
`uwvm-ros2-compat-contract` model executable. When the actual target OpenSSL
Crypto compile/link probe succeeds, it also adds `uwvm-ros2-rihs-contract` from
[`tests/ros2_compat/rihs_contract.cc`](../../../tests/ros2_compat/rihs_contract.cc).
That separate regression checks production RIHS01 against independently exported
native golden identities and rejects incomplete or malformed schemas, with
assertions enabled. Missing target Crypto is reported as unavailable and creates
no stub target. Both are model checks without ROS communication, and this option
is OFF by default; execution remains a separate qualification step.

## API differences and retained semantics

`api.h` chooses QoS-object or complete `rmw_qos_profile_t` overloads for typed
publishers, subscriptions, clients and services. It never substitutes an SDK
default profile for a selected field. `qos.h` checks actual endpoint QoS through
the installed public API. Missing actual-QoS methods return unavailable; requested
settings are never relabelled actual settings. The builtin client/server/topic
paths check method availability before creating their native endpoint. Some
older SDKs, including the official Foxy client/service headers, do not expose
complete request/response actual-QoS readback; those Service/Lifecycle paths
remain unavailable under this strict contract even though a basic native service
could be created by a less restrictive application.

The legacy incoming-service path subclasses the installed public typed
`rclcpp::Service<T>` and overrides its virtual request dispatch. It retains the
real request/header and responds only when the authenticated `service.respond`
operation succeeds. It does **not** invoke the old base dispatch, which would
otherwise send an automatic empty response. The native executor never waits for
a Guest reply. The existing bounded request table, deadlines, source leases,
independent response authority and close/revoke cleanup remain in charge.
The subclass constructor and node-service registration must pass their own actual
SDK compile/link probe. No safe deferred path means the feature is unavailable.

Pre-Iron parameter APIs do not offer a post-set callback. The compatibility path
reads the actual value only after a successful native commit while the existing
Gateway mutation mutex serializes all local writers. Parameter services and the
automatic event publisher are disabled. An on-set validator is never treated as
a committed event. Callback leases release the actual registered callback once,
including after moving the lease. Remote typed writes require a public static-
typing descriptor; absence of that member is treated conservatively as dynamic.
Local declaration/undeclaration and Action graph APIs have independent guards.

Foxy exposes a public typed Action Client constructor with all five explicit
QoS channels, but its convenience `create_client(node, name, group)` factory
has no fourth options operand. The compatibility wrapper prefers a real
options-capable factory when available; otherwise it passes the unchanged
options to that public typed constructor and registers/removes it through the
public node waitables interface. The lease holds only a weak waitables owner,
unregisters once after successful registration, and releases the native Client
even if retirement throws. It never calls the default-options factory as a
substitute. The Action feature probe instantiates, links and destroys this exact
route plus the production explicit server-options overload. If neither public
route is available, Action is unavailable rather than a later Gateway compile
failure. The SDK-free `uwvm-ros2-action-client-lease-contract` checks normal,
exceptional, copied and concurrent retirement when compatibility tests are on.
Actual old/new SDK builds and native Action traffic remain separate tests.

Action availability has three separate observed flags. `action_core` checks the
public options/waitable/native-library APIs without any tutorial message.
The historical `action` flag retains exactly its builtin Fibonacci meaning.
`custom_action` checks every row of the explicitly selected native Action macro;
it does not depend on the optional Fibonacci package. Each row instantiates the
real options-preserving client, public registration/destruction and server with
all five explicit QoS channels and its result timeout. CMake only compiles and
links these probe executables; it never initializes ROS or runs them.

The custom probe uses the same paired common-manifest/native-header definitions
and explicit package link interfaces as the Gateway target. A constexpr check
requires a nonempty, duplicate-free exact ROS2 Action set, matching interface
identities, codec identities and all three component descriptors. C++ type checks
also require `Native::Goal`, `Native::Result` and `Native::Feedback` to equal their
actual generated codecs. Missing/empty/mismatched catalogs produce an observed
failed compile/link result; they do not silently claim custom support. Generated
feature header, JSON, cache and the cross-build evidence include all three flags.
A passing native-type compile probe still grants no Guest alias and does not
replace runtime full native schema/typesupport/QoS, lease or ownership checks.

The optional `uwvm-ros2-action-catalog-contract` is an SDK-free eight-group data
check of the production pairing helpers. The nine injected data-policy cases in
[`action_capabilities_models.py`](../../../tests/ros2_compat/action_capabilities_models.py)
exercise the real configuration admission function, including a custom-capable
SDK with no tutorial type. Neither suite is a native SDK or DDS execution claim.
Actual SDK builds and builtin/custom Action traffic remain independent tests.

Foxy Action goal-response callbacks carry a ready shared future; newer callbacks
carry a shared pointer. `goal_response` extracts the same native goal handle
without waiting on a pointer or fabricating an acknowledgement. Missing
`remove_pending_request` is permitted only for a fresh single-request client:
its owning scope destroys the complete native pending map. This does not cancel
an already admitted remote operation and cannot authorize a retry.

The executor notification shim uses the public modern trigger, public guard
object, or the older raw guard under its public lock. Resource retirement is not
implemented by waiting for another message to arrive. No numeric resource ID,
callback count, GID or granted limit is inferred from a distribution name.

Lyrical/Rolling may generate unbounded `uint8[]` as
`rosidl::Buffer<uint8_t, Allocator>`. Outgoing Image buffers are fresh CPU-native
objects populated only after complete wire/quota validation. Incoming Buffer
storage must be proven to use the exact `CpuBufferImpl<T, Allocator>` before a
CPU pointer or iterator is obtained. A non-CPU backend is refused without an
implicit `to_vector`, GPU copy, or unbounded allocation. Bounded sequences still
use their selected generated bounded container.

## Type identity before Iron

A missing native RIHS member is not a native hash. `baked_hash` returns empty for
an absent or malformed native hash. On a modern support struct, a missing,
wrong-version or mismatched hash is a refusal; it never triggers legacy fallback.

A genuinely pre-RIHS binding must validate every ordered native introspection
field: exact normalized type/name, primitive/nested type, array/sequence shape,
string and sequence bounds, field count, offset validity, default absence, and
complete acyclic dependency closure. The schema is bounded to 256 records,
4096 fields per record and depth 16. Pre-IDL package/type names are normalized
only for the explicitly selected `/msg/`, `/srv/`, or `/action/` category.

`matches_rihs` reconstructs the official RIHS01 canonical TypeDescription JSON
from that trusted complete declaration schema and uses the selected OpenSSL
SHA-256 implementation. Its startup-only representation is bounded to 4 MiB;
there is no handwritten crypto, per-message hash, arbitrary package loading or
Guest-selected native typesupport. Missing OpenSSL makes legacy recomputation
unavailable and leaves modern native-hash validation intact.

A Service or Action root contains more than its two/three application payloads.
The canonical schema must include all official protocol/event roots and their
standard dependencies. Builtins have a closed complete catalog. Custom providers
supply `ipc::custom_native_interface_schema<Native>::schema()` from the trusted
generator, independently validate their actual native payload components, and
then recompute the selected root RIHS. An older provider with an empty root
schema cannot acquire legacy support merely by presenting a name or catalog ID.

Official `.msg char` maps to IDL `uint8`, so the ServiceEventInfo client GID is
canonical `uint8[16]`. Empty IDL records contain the official sole
`uint8 structure_needs_at_least_one_member` field, including in RIHS. Legacy
zero-member native introspection is accepted only for that exact otherwise-empty
canonical declaration; it cannot hide a missing application field. This does not
change any serialized Guest payload or claim that old middleware advertised RIHS.

## Deployment boundaries

Foxy through Lyrical/Rolling are the intended broad compatibility family; each
physical SDK still needs its own build, native delivery, cleanup and security
qualification. Humble and later expose more actual-QoS/deferred APIs; Iron adds
native RIHS/TypeDescription and post-set APIs. Dashing/Eloquent and earlier SDKs
are admitted only where public APIs meet the same contracts. Official Bouncy,
Dashing and Eloquent Node construction unconditionally creates a parameter event
publisher or lacks a complete quiet-node option. Such an SDK needs a separately
reviewed maintained public API overlay; this layer refuses startup rather than
quietly widening default authority. This is an explicit implementation boundary,
not a successful all-distribution claim.

Enforce mode keeps an exact checked credential directory. Public enclave
argument support selects the modern enclave/key-store path. Legacy directory
selection uses only the exact checked node-directory override, with inherited
modern/legacy selectors removed first. There is no longest-prefix fallback.
RMW security availability and signed permissions must be qualified separately;
a successful compile or a discovery count never proves security enforcement.
The existing explicitly selected Fast-DDS/CycloneDDS adapter policy is retained;
new RMW names are not authorized automatically.

The C++17 contract model uses small fake overload/callback/lease shapes plus
actual installed metadata. It covers invalid native hash refusal, full profile
conversion, missing actual-QoS refusal, future/pointer goal callbacks, bounded
buffer copying, callback single cleanup and schema/hash negative cases. Its fixed
success marker is `PASS_ROS2_COMPAT`. Building/running it is a model check only;
it initializes no ROS context or communication endpoint.

## Primary sources

- [REP 2000](https://raw.githubusercontent.com/ros-infrastructure/rep/master/rep-2000.rst)
- [Official distribution index-v4](https://raw.githubusercontent.com/ros/rosdistro/master/index-v4.yaml)
- [Lyrical release notes](https://raw.githubusercontent.com/ros2/ros2_documentation/rolling/source/Releases/Release-Lyrical-Luth.rst)
- [Foxy public Service dispatch](https://raw.githubusercontent.com/ros2/rclcpp/foxy/rclcpp/include/rclcpp/service.hpp)
- [Foxy public client API](https://raw.githubusercontent.com/ros2/rclcpp/foxy/rclcpp/include/rclcpp/client.hpp)
- [Foxy raw notify guard interface](https://raw.githubusercontent.com/ros2/rclcpp/foxy/rclcpp/include/rclcpp/node_interfaces/node_base_interface.hpp)
- [Bouncy implicit parameter publisher](https://raw.githubusercontent.com/ros2/rclcpp/bouncy/rclcpp/src/rclcpp/node_interfaces/node_parameters.cpp)
- [Official ROS IDL scalar mapping](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_adapter/rosidl_adapter/msg/__init__.py)
- [Official empty record declaration](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_adapter/rosidl_adapter/resource/struct.idl.em)
- [Canonical RIHS01 representation](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_generator_type_description/rosidl_generator_type_description/__init__.py)
- [Official Service/Action protocol declarations](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_parser/rosidl_parser/definition.py)
- [Buffer CPU access contract](https://raw.githubusercontent.com/ros2/rosidl/rolling/rosidl_buffer/include/rosidl_buffer/buffer.hpp)
