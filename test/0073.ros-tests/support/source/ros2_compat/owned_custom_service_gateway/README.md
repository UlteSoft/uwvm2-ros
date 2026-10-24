# Complete owned custom-service Gateway integration

These sources connect an independently proved custom canonical service provider
to the authenticated Gateway's existing owned transport. No import, permission,
Node, native endpoint, update authority or debugger authority is granted merely
by building or loading a profile. The entire implementation and these validation
sources still require real compilation and execution after the implementation
freeze. Source review does not establish native or Guest acceptance.

The optional Native Gateway is the controlled ROS network boundary. It is not
binary compatibility for Guest rcl/rclcpp and does not expose its native types,
pointers or symbols to Wasm. Application compatibility means compiling the old
C/C++ source to Wasm using the Guest SDK, or calling that SDK from CPython.wasm
and the Java IR-to-Wasm output. All resulting ROS calls use the same explicit
uwvm2 handle grants and current source/session/ownership checks.

## Complete type and storage selection

The original Registry constructor keeps its existing fixed-family factory and
adds one trailing size selector with a closed fixed-family default. A genuine
custom caller selects both the complete custom factory and the same provider's
canonical bounds. Independent actual SDK hashes, full request/response/event/
Time/ServiceEventInfo descriptions, sources and typed layout remain mandatory.
A numeric length or a capability type name alone never selects a custom codec.
The original fixed16 Native endpoint implementation remains unchanged.

The shared storage helper reserves history times the complete incoming body
plus each real64-byte UWRE header, and one separate control header/queue slot.
Client response and server request maxima are deliberately different inputs.
History must cover configured max-inflight. The existing two-value canonical
bounds keep the original additional requirement that history cover QoS depth.
A separately proved provider may return a trailing
`bodies::separately_owned_native_depth_limit` in1..1000 to select a different
storage model: canonical immutable lifetime history stays at most256, while the
same grant's positive native DDS QoS depth must fit that independent limit.
This permits the actual original Parameters/ParameterEvents depth1000 without
allocating1000 canonical history records or changing their exhaustion behavior.
The selected provider must own and separately admit the actual native queue and
its complete process memory budget. The number is finite trusted storage-model
metadata; it does not prove middleware allocation bounds or actual QoS.

Only the actual Native selected-bounds implementation, after its full original
profile/type/codec proof, supplies this tail. No capability key, packet flag,
Guest input, type-name shortcut or default constructor selects it. The ordinary
fixed-family and custom providers continue returning two-value bounds with zero
in the new tail. Their original admission and finite256-depth endpoint checks
remain unchanged. Every provider/Registry consumer must source-recompile this
extended bounds structure; old callback object files are not binary-compatible
merely because a source-level function name is unchanged.

Both storage models reject every addition/multiplication overflow, insufficient
source request/response grants, insufficient observer event bytes, body maxima
above32768, history below max-inflight, or a canonical queue without its separate
control slot. This does not enlarge a grant or replace admission of physical
Native objects, URMI records, allocator/container overhead and foreign DDS work.

The backend's trailing body validator preserves builtin behavior by default.
Only an explicitly selected real custom Gateway uses the custom proof cache
and complete canonical wire validator. Submit/respond validate before traffic
charge or Native send. A successful peek is already a borrowed record: any
subsequent custom validation/quota/deadline refusal follows the original real
cancel/retire compensation and preserves primary and cleanup facts separately.
Control records retain their existing independent envelope grammar.

## Explicit production build

Use a fresh owned build directory and the actual installed original SDK. Select
`UWVM_ROS2_CXX_STANDARD=20`, `UWVM_ROS2_BUILD_OWNED_RCL_SERVICE=ON` and
`UWVM_ROS2_BUILD_OWNED_CUSTOM_RCL_SERVICE=ON`. Supply the actual generated complete
`UWVM_ROS_CUSTOM_REGISTRY_HEADER`, `UWVM_ROS_CUSTOM_ROS2_HEADER` and explicit
`UWVM_ROS_CUSTOM_ROS2_PACKAGES`, plus the independently exported original native
`UWVM_ROS_CUSTOM_ROS2_OWNED_EXPORT_HEADER`. These are real absolute build inputs;
this directory supplies no fake header, native identity, SDK target or export.
The export includes complete typed sizeof/offsetof expectations generated from
the original type selection. The modern metadata executable probe is fresh and
uses actual imported SDK targets. An unavailable API keeps custom admission
unsupported; an old cached1 or local unbuilt library is not a successful link.

The adapters compile in a separate static target so the original Gateway's
ament plain-link convention and the new helper's keyword-link convention remain
coherent. Every actual shard address is required by the complete dispatcher.
The selected shard count is identical in Gateway and adapter translation units.
The original C++17 build and both options' default OFF state are preserved.
Only the new owned factory/validator is partitioned by this cohort. The existing
legacy custom-services inline dispatch remains in the historical main
translation unit and requires separate source-equivalent partition work for
large vendor catalogs. Selecting the new owned custom profile performs its
complete proof directly before Node creation; it need not first execute the
legacy known-type lookup. This ordering does not claim the old legacy template
instantiations have disappeared from the compiled main translation unit.

At run time pass the original trusted configuration and explicit
`--owned-rmw-service-profile` file. The profile still requires all14 explicit
codec/storage fields. It is storage/native identity selection, not ROS rights.
Its actual CPP10 graph proof and every selected custom proof are checked before
Node creation. Current independent service.call, event.observe,
service.advertise, service.respond and service.available grants remain required.
Guest code cannot construct the native selector, change the profile or enter a
host update/debug manager through these APIs.

## Authored validation and actual ownership

`storage_contract.cc` checks40 independent expected cases using the production
header, including unequal9000/512-byte bodies, exact header/control credit,
maximum history/body limits and real unsigned overflow boundaries. This is a
pure arithmetic contract, not proof of a native SDK or a service. The original20
cases remain unchanged;20 new cases cover default-coupled preservation, exact
client/server reservations under the explicit independent DDS model, depth0/1001
and invalid-limit refusals, and unchanged canonical quota/overflow boundaries.

`native_wire_contract.cc` uses the actual authenticated IPC client, genuine
Gateway and real retained RCL client/service instances. Run it once per full
original xArm GetFloat32List and SetInt16List selection. It preserves the empty
Request representation, signed16 extremes, ret=-7, all9 UTF8 bytes
`6e 61 74 69 76 65 2d c2 b5`, and both float32 values1.25/-2.5. The complete
canonical request/response lengths are0/27 and10/15. Canonical strings reject
NUL according to their original profile; builtin String's distinct NUL policy
must not be substituted. The actual native exporter supplies all identities;
these independent raw payload oracles cannot qualify an unknown native schema.

Root first launches and owns the witness process, learns its actual PID, writes
the trusted worker configuration, starts the actual selected Gateway and then
sends `G` on the witness's inherited stdin. Its six arguments select that real
config and the five independent aliases. It never forks or broadens authority.
The fixture binds the real policy/session, observes real readiness, submits
once, validates all bytes and genuine native correlation/GID/request sequence,
cancels and reborrows the same record with a fresh reservation, commits, responds
once and retires through both explicitly selected original lanes. Finite driver
polling is separate from the production's true same-client graph wait.

The arithmetic contract uses C++17. The real IPC witness explicitly requires
C++20 because it uses the actual full fast_io API; no header replacement or
concepts bypass is supplied.

Build contracts only by explicitly setting
`UWVM_BUILD_OWNED_CUSTOM_GATEWAY_CONTRACTS=ON`, `UWVM_SOURCE` to the reviewed
checkout and `UWVM_ROS_CUSTOM_REGISTRY_HEADER` to the exact same actual complete
generated common registry selected by the Gateway. The wire witness must parse
the actual custom capability grammar; an empty default catalog intentionally
refuses the adapter-codec-identity field. This is an independently retained real
header input, never a test-created fake catalog. Configure creates no process or test. All outputs/caches belong in
`/tmp` for local work; Linux build/test processes belong to the real64GiB
zero-swap cgroup with owner/watchdog and adequate backing storage. Preserve
original source/tool/SDK/full compiler dependencies/link evidence, real process
cleanup and terminal normal completion. This fixture uses logical Gateway
handles and does not fabricate Host manager lease or actual Guest acceptance.
Complete Native-opposite applications, permissions/revocation faults, original
rcl migrations, all distro/vendor official suites, QEMU and VTune remain required.
