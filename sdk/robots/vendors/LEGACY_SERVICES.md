# Explicit old-SDK ROS2 Service identity preparation

This source candidate extends the existing explicit `legacy-introspection`
Message path to complete Services. It has not been imported, generated, compiled
or run. It creates no Node, endpoint, ROS traffic, Runtime import or grant. The
normal `original-native-metadata` mode remains the default. A modern support
structure with a hash API always uses that original exporter; a missing or
malformed modern hash cannot select this old-SDK route.

## Three different facts

The actual installed Service support is dispatched to the literal
`rosidl_typesupport_introspection_cpp` implementation. Its genuine
`ServiceMembers` namespace/name must match the selected Service. Its initialized
Request/Response pointers must be the same pointers independently obtained from
the statically selected `Native::Request` and `Native::Response` types. Every
ordered native field, nested dependency, default-pointer policy and original
native string/sequence/array capacity is compared before deriving an identity.
Canonical wire quotas never replace these native capacities. Empty Service
components preserve their official representation-only uint8; they add no wire
field. Zero-field Topic preparation remains a separate required implementation.

Older ServiceMembers, including Humble, have Request and Response pointers and
no Event pointer. The canonical modern Service description additionally has
`request_message`, `response_message` and `event_message`. Its Event contains
ServiceEventInfo and bounded-singleton request/response sequences. The new
helper checks this exact standard declaration grammar and real native Time,
then derives the complete root RIHS using the existing bounded implementation
and actual target OpenSSL. It labels the root
`LEGACY_NATIVE_SERVICE_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01` and
`standard_protocol: "ROS2_SERVICE_EVENT_V1"`. This is a derivation, never a claim
that an old SDK contains a baked hash or installed Event type. The separate
`protocol_event_native_introspection` field is false when that native member is
absent. If an actual ServiceMembers type has an Event member, its non-null
complete metadata must match too; a malformed present Event cannot fall back to
an absent one. Request/Response objects retain the existing
`LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01` provenance and their own identities.

Finally, native execution and ownership are separate facts. A JSON object,
identity syntax, source preparation or successful generation establishes none
of the actual compiler, mapped-library, endpoint or delivery proofs.

## Source-to-six-language workflow

1. Admit the actual complete vendor/model/SDK revision and immutable upstream
   source lock. Include each original `.srv` and every transitive `.msg`
   dependency in the closed unbound catalog. Keep real native type names and
   complete original fields. For standard Header/Time dependencies use the
   explicit official-standard profile; it cannot shadow a Runtime builtin ID.
   Use only the actual current-MF parser modules, exact installed native headers
   and the existing 18 current-source support roles. There are no new ambient
   module or guessed SDK search paths.
2. Select `identity_mode: "legacy-introspection"`, ROS major 2 explicitly, then
   call `vendor_prepare.prepare(inventory, request, read=..., load=..., out=...)`
   with a fresh caller-owned directory. The sidecar keeps a V2 complete native
   interface graph and Request/Response component graphs; Message-only V1
   receipts remain unchanged. `guest-only/` has source codecs, not identities.
3. The independent native owner compiles `native_identity.cc` against the actual
   selected SDK and Crypto with full system-inclusive MD, exact native ELF,
   actual link inputs and frozen whole SDK/source evidence. The executable
   materializes ALL root/component metadata and lazy loads before SIGSTOP. The
   owner observes original process birth/pidfd, cgroup and complete mappings,
   then releases one original bounded stdin `I` gate. One bounded raw stdout
   document follows; no partially constructed document is emitted.
4. Admit that original raw document and its original owner evidence with same-FD
   full7/mode/nlink/root checks. `bind_and_generate(...)` reparses copied original
   definitions, recomputes the source sidecar and checks every complete native
   declaration/provenance/component association before the existing binder
   copies identities. It emits actual six-language records, interfaces, native
   adapters and registry source. Python never calculates MD5 or RIHS.
5. Feed the actual generation/full-file ledger and binding receipt to
   `adapter_plan.plan(..., read=...)` with `mode: "service-call"`. The result is
   an unexecuted source plan requiring `service.call`, exact selected alias and
   resource, separate root/codec identities, and both actual component wire
   bounds/identities. `command_binding` must be null and `default_authority`
   remains `[]`. No alias is opened, no Service is advertised and no robot
   action occurs. Call, advertise and respond authorities remain independent.

For a real catalog Service `acme_interfaces/srv/Measure`, existing generated
public names are derived without renaming the native type:

| Language | Complete actual generated Service API |
|---|---|
| C | `roswasmc_bounded_2_acme_interfaces_srv_Measure_call` and full Request/Response record codecs |
| C++ | `uwvm_custom::Ros2Acme_interfacesSrvMeasureClient` and real generated component codecs |
| Rust | `Ros2Acme_interfacesSrvMeasureClient` alias over typed `ServiceClient<Request,Response>` |
| JavaScript | `Ros2Acme_interfacesSrvMeasureClient` over the real Session/record codecs |
| AssemblyScript | `Ros2Acme_interfacesSrvMeasureClient` over the selected real Api |
| TinyGo | `NewRos2Acme_interfacesSrvMeasureClient` and `Call`/checked cleanup |

`acme_interfaces` is an explanatory name, not a supplied official vendor schema
or fake native provider. The actual generated bundle is authoritative for names
and capacities. All six `bounded_records.*` and `bounded_interfaces.*`, selected
native adapter, manifest and `generation.json` must be retained and re-opened.
No generated files, native identities or maximum wire sizes are forecast here.

## Required validation before acceptance

The new real installed-SDK contract at
`tests/ros2_compat/legacy-service/native_contract.cc` uses actual
`std_srvs::srv::Trigger`, `SetBool` and builtin Time. It covers the genuine empty
Request representation, typed Service association, full native/protocol graph,
wrong protocol/member/capacity/refusal cases and unconditional modern-SDK
refusal. These builtin test types are not registered through a custom catalog.
The contract creates no ROS Node. The CMake source entry requires actual
installed packages and Crypto; no mock struct or replacement container exists.

Run the old-SDK native-positive and modern-refusal routes separately with their
actual SDK/architecture/owner evidence. Preserve raw outputs, exact commands,
MD/link/ELF and stopped mappings; a modern refusal is not an old-SDK success.
Also run the 11 injected source/DATA policy methods with their actual MF-loaded
modules; these never count as native tests. Then compile and qualify the real
six-language bound bundle, default-denied instance, complete native RPC/value
oracle, finite cancellation/close and disconnect witnesses in the unified
64 GiB, zero-swap cgroup. Java/C# paths remain untouched.

## Primary source basis

- [Humble ServiceMembers](https://raw.githubusercontent.com/ros2/rosidl/humble/rosidl_typesupport_introspection_cpp/include/rosidl_typesupport_introspection_cpp/service_introspection.hpp)
  and [actual lazy initializer](https://raw.githubusercontent.com/ros2/rosidl/humble/rosidl_typesupport_introspection_cpp/resource/srv__type_support.cpp.em).
- [Jazzy ServiceMembers with Event](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_typesupport_introspection_cpp/include/rosidl_typesupport_introspection_cpp/service_introspection.hpp).
- [Service/Event declaration construction](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_parser/rosidl_parser/definition.py),
  [ServiceEventInfo fields](https://raw.githubusercontent.com/ros2/rcl_interfaces/jazzy/service_msgs/msg/ServiceEventInfo.msg),
  and [official .msg char-to-IDL uint8 mapping](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_adapter/rosidl_adapter/msg/__init__.py).

Downloaded primary files are bounded source observations in the review package,
not immutable SDK locks or evidence of an installed native version. Real SDK
comparison remains mandatory. The separately implemented
[complete Action workflow](LEGACY_ACTIONS.md) supplies SendGoal/GetResult,
CancelGoal/status/UUID, feedback and actual action-support associations; the
Service-only route never derives an Action root from three payload hashes.
Both source reviews are complete, with actual installed-SDK and native delivery
qualification still required.
