# Complete pre-RIHS ROS2 Action source preparation

This candidate implements an explicit old-SDK Action metadata exporter and the
prepare/bind/plan path over the existing complete typed generators. It does not
run a generator/compiler, create a Node, issue an Action, grant authority or
qualify a vendor. Original source locks, installed native SDKs, full dependencies,
process ownership and real native RPC/value/cleanup witnesses remain mandatory.
The default identity mode remains `original-native-metadata`. Nothing catches a
modern hash failure and retries a legacy provider.

## Actual native facts and standard-derived facts

Official Dashing/Humble `rosidl_action_type_support_t` contains five pointers:
SendGoal and GetResult Services, the shared CancelGoal Service, FeedbackMessage,
and GoalStatusArray. The generated Action's `Native::Impl` names exactly those
five types. The new C++ helper requires the actual ActionSupport pointer's five
members to equal the separately obtained typed handles. It independently checks
real ServiceMembers namespace/name and typed Request/Response associations, full
Message introspection, original native capacities/default policy, nested payloads,
UUID and Time. Compile-time field types tie SendGoal.goal, GetResult.result and
FeedbackMessage.feedback to the actual Native::Goal/Result/Feedback aliases.
No same-shaped replacement native struct/provider is introduced.

The complete canonical Action RIHS description has six root fields: goal,
result, feedback, send_goal_service, get_result_service and feedback_message.
The two Services include their complete request/response and standard Event
schemas, bounded-singleton sequences, ServiceEventInfo, UUID and Time. This is
more than hashing three payloads. Old SDKs do not have native Event metadata;
that absence remains explicit in each Service's
`protocol_event_native_introspection: false`. A genuinely present Event pointer
must instead be nonnull and fully matched. The root provenance is
`LEGACY_NATIVE_ACTION_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01`, with
`standard_protocol: "ROS2_ACTION_EVENT_V1"`; it is a derivation after native
comparison, never a baked SDK hash.

CancelGoal and GoalStatusArray are shared runtime protocols, not additional
members silently added to the canonical Action hash. Their actual complete
native graphs are retained under separate `protocol_associations`. Seven roles
are mandatory: send_goal, get_result, feedback_message, cancel_goal, status, uuid
and time. The cancellation graph preserves GoalInfo/UUID/Time and the original
unbounded goals_canceling sequence; status preserves every GoalStatus field and
its unbounded status_list. Constants are not serialized fields. Canonical Guest
quotas do not change any native declaration capacity.

Every modern hash-capable Action/root, component, protocol Service/Message, UUID
or Time provider is refused by this path, even if its callback/hash is absent or
malformed. Declared defaults are also refused before legacy sidecar or exporter
source publication. The C++ helper rejects nonempty trailing native_default
annotations, including virtual Service root/Event declarations. This does not
change the separate modern/default implementation or infer defaults from null
native pointers.

## Source capabilities and historical boundaries

The acquired immutable official Dashing and Humble sources expose the five-link
Action API. Jazzy adds a real hash API and must take the modern exporter path.
The exact acquired Ardent/Bouncy rosidl trees contain neither the Action support
struct nor Action struct emitter paths. This is a bounded source observation,
not an invented installed version, complete distro inventory or successful old
Action availability. An actual SDK without those headers/types/support is
unavailable for this Action route. No macro, dummy provider, ROS1 protocol or
new capability is substituted to make it pass.

Dashing `example_interfaces/action/Fibonacci` Feedback is named **sequence**.
Humble `action_tutorials_interfaces/action/Fibonacci` uses **partial_sequence**.
Both exact original `.action` files are retained; the native contract chooses
one actual installed package explicitly and preserves its real field spelling.
The supplied unbound Dashing catalog is a parser/native-preparation fixture with
finite canonical sequence bounds64. Those bounds are not native sequence limits.
It carries no native identity or codec hash. The Humble builtin-reserved tutorial
root is a native helper contract only; it is not disguised as a custom registry
entry or vendor19 source lock.

## Current-source prepare and native exporter workflow

1. Acquire and review the actual vendor/model/controller revision, whole source
   tree lock, license, complete original `.action`/transitive `.msg` definitions,
   exact native headers and finite canonical bounds. The ordinary source lock
   still requires the actual vendor's official repository; the ROS Fibonacci
   fixture must not be relabelled as one of the nineteen vendors.
2. Select ROS2 and `identity_mode: "legacy-introspection"` explicitly. Inject the
   actual full-MF `load(relative,name)` and held-FD/root/mode/nlink/full7 `read`.
   Call `vendor_prepare.prepare(inventory, request, read=read, load=load, out=fresh)`.
   The original ten plus existing eight support roles remain mandatory; there
   is no ambient import or additional provider search. The original parser
   supplies complete records/interfaces and `ros2_interface_schema`; no new
   Field/default/parser or identity domain is invented here.
3. Inspect the actual native_identity.cc and declaration sidecar. The emitter
   statically selects the actual Action/Impl types and includes. It materializes
   the entire bounded metadata document, every typed/protocol introspection and
   lazy library dispatch **before SIGSTOP**. The independent native owner must
   retain real original pidfds/cgroup, complete source/SDK catalogs, full system
   MD/link/ELF and stopped mappings, then release exactly the original finite I
   gate. One complete at-most16MiB raw JSON document follows. A metadata failure
   emits no partial stdout. This library does not implement or replace that
   process owner.
4. Pass the genuine original metadata and original owner-evidence full7s to
   `bind_and_generate`. It reparses the actual retained definitions, reconstructs
   the entire native declaration/protocol sidecar and validates every raw root,
   component and protocol association. The original and copied metadata/owner
   receipts remain distinct. Existing `bind_catalog.bind` alone copies actual
   exported identities; Python does not calculate RIHS or MD5. The current real
   generator emits the full records/interfaces/native adapters/manifest and
   actual generation output ledger. Retain and reopen all outputs and source
   receipts before a coherent source freeze.

The public C++ source API is
`legacy_metadata_export::action<Native>(schema, root, runtime_schema)`. The
separate runtime_schema is the complete10-record CancelGoal/status declaration
closure; `action_runtime_schema()` exposes the trusted expected view for native
contracts. It is not installed type support or a registry. Real emitters retain
the independently source-associated runtime declarations as compiled input.

## Action-client plan and role separation

`adapter_plan.plan` accepts `mode: "action-client"` with the original request
keys plus `action_aliases`. `alias` must equal the selected send_goal alias.
The four mandatory, distinct roles are:

| Role key | Required Host operation |
|---|---|
| send_goal | action.send-goal |
| cancel_own | action.cancel-own-goal |
| feedback | action.receive-feedback |
| result | action.receive-result |
| cancel_any, optional | action.cancel-any-goal |

No omitted stronger cancel-any role falls back to cancel-own. One actual
registered whole `.action` and its complete Goal/Result/Feedback records must
match the generated root/component identities, both wire bounds and all output
bytes. `command_binding` remains null; `default_authority` must be `[]`. The plan
returns `required_operation: null` plus the exact `required_operations` map;
this prevents presenting an Action as a Topic or a single generic permission.
It never opens aliases or grants any of these rights. Sending an Action can
actuate hardware: actual model/controller operation policy and an explicitly
controlled native/simulator witness remain separate prerequisites.

The existing generated six core Guest families are reused, not stubbed:
C `..._send/_feedback/_result` over an actual caller-owned action_bundle; C++
`...Client` with real bundle ownership; Rust typed ActionClient; JavaScript
TypedActionClient over the actual Session/copied Guest transport; AssemblyScript
BoundedActionClient over actual Api/Codec; TinyGo's actual generated client.
For Dashing the derived class stem is
`Ros2Example_interfacesActionFibonacciClient` and C prefix is
`roswasmc_bounded_2_example_interfaces_action_Fibonacci`. The actual generated
bundle is authoritative for every symbol, component extent and codec identity.
The wider requested Java/C#/Python/QuickJS/MoonBit SDK/test work belongs to the
separate coordinated language work; this core cohort does not mark it complete
or remove it from the required scope.

Run default-denied and enabled instances separately. The denied instance must
observe genuine open13 and zero native endpoints/queues/effects. In the enabled
instance open the four explicit aliases, send at most one admitted Goal, and
retain its nonzero matching ID. Poll feedback/result only in caller-paced,
finite bounded steps. Idle11 is not completion or an observer exception success.
Compare complete typed payloads to an independent native peer; local counters
or terminal markers alone are not delivery. A cancel acknowledgement does not
retire the Goal; only a validated terminal result does. Unknown timeout/disconnect
outcome is retained without replay. Server callback, advertise/respond,
revocation, owned cancellation and checked cleanup remain separately authorized
and must preserve the primary error plus every cleanup result.

## Native and policy source contracts, not executed

`tests/ros2_compat/legacy-action/native_contract.cc` uses real selected installed
Fibonacci types and native support. It checks five associations and all complete
protocol/native declarations; mutations affect expected declaration views only,
never actual SDK providers. It warms actual native type support before SIGSTOP
and one I gate. Actual modern refusal is reported distinctly from legacy native
metadata success. It creates no ROS Node or Action server/client. CMake requires
real selected packages, Crypto, C++17, assertions, full MD and linker trace.

Example **unexecuted command shapes**, under the independent resource owner:

```sh
cmake -S "$SOURCE/tests/ros2_compat/legacy-action" -B "$FRESH_BUILD" \
  -DUWVM_SOURCE="$SOURCE" \
  -DUWVM_LEGACY_ACTION_FIXTURE_PACKAGE=example_interfaces
cmake --build "$FRESH_BUILD" --parallel 1
```

For the genuinely installed Humble/Jazzy tutorial package, select
`action_tutorials_interfaces` explicitly instead. Do not execute the stopped
contract from an unowned shell or send a synthetic I from a recipe. Root must
supply the original retained process/gate and actual mappings/SDK preservation.

`pipeline_models.suite(preparer, planner, emitter, parent, bounded, *,
emitter_source, helper_source)` has15 authored injected policy methods. Syntax
IDs/objects test source envelope associations only, not native SDK/hash proofs.
They test complete protocol and component refusal, no modern downgrade, source
capacity projection, strict aliases and early declared-default refusal. The
original Service11 and vendor19 source suites remain unchanged and separate.
No candidate import, generator, compiler, linker, native test, policy test, ROS
case or official vendor test was run during authoring. The actual six-core
compiles, complete native RPC/callback/value oracle, error/close/cancel/revoke/
disconnect, whole source/SDK and each vendor official suite remain required.
