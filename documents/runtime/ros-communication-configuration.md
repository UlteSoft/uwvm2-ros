# Communication deployment definitions

`communication_configuration.h` owns the semantic plans for context, node, type,
topic, service, action, graph and event families. Parsing is inert and grants no
authority. A deployer first selects one exact family, closed provider and profile,
supplies every applicable named limit once, parses the complete pinned definition,
checks aggregate deployment capacity, and freezes it. Only a separately admitted
dispatcher and a successfully prepared worker may create native resources.

The closed providers are `ros2-loopback-worker` for context/node/topic/service/
action/graph, `canonical-schema` for types, and `local-event-stream` for events.
`unconfigured`, native paths, native FD numbers, owner identities, arbitrary codec
pointers and arbitrary dynamic library providers are refused. The current codec
registry contains String, Bool, SetBool request/response and LookupTransform
goal/feedback/result. The `bounded-message` type codec also accepts an explicit,
owned structural schema for ROS1 bodies or plain XCDR1. It supports validation,
conversion and inert `type.describe`; it does not extend the closed native
endpoint registry. Other native types remain unsupported until an approved
adapter and worker closure are implemented and tested. See
[Bounded message schemas](ros-bounded-messages.md) for the supported shapes,
restrictions, ownership rules and native qualification boundary.

## Compile a definition

`tools/ros_scene/make_communication_definition.py --input plan.json --output plan.bin`
checks exact JSON field names, duplicate keys, integer ranges, finite values and
all mandatory capacity fields. Output creation is exclusive. It produces the
canonical file and a `.json` receipt with the full-file SHA256, logical profile
SHA256 and supplied limits. For example:

```json
{
  "family": "context",
  "provider": "ros2-loopback-worker",
  "identity": "robot-context",
  "limits": {"definition.bytes": 4096, "instances": 1},
  "definition": {
    "domain": 12,
    "enclave": "",
    "transport": "unauthenticated-loopback"
  }
}
```

This example describes a requirement; it is not an active ROS deployment. The
native static-peer worker additionally requires its independently pinned runtime
closure, fixed network plan, syscall seal and admitted context capability.
Loopback source/port and participant-prefix filters do not provide encryption or
authenticate another process running on the same host.

## Canonical body and profile references

The carrier is `text magic, text profile identity, blob profile SHA256, body`, with
magic `uwvm.ros.communication.definition`. Scalars, booleans and lengths use the
canonical little-endian wire codec. The logical SHA256 covers the entire exact
body, including its leading family and provider names. The deployment loader
also verifies the complete-file SHA256. A referenced profile is an identity plus
exact 32-byte digest; matching only the identity is insufficient.

Each body begins with family and provider. Context supplies domain, empty enclave
and the explicit unauthenticated loopback selection. Node supplies context
profile, node name and namespace. Type supplies either a closed registered codec,
or `bounded-message` with an explicit type identity and complete schema. The
canonical type body pins codec, identity, encoding and complete descriptor.
Native dependency checks compare the validator as well as those bytes; a matching
name cannot substitute a generic or permissive validator for a compiled codec.
Topic supplies node/type profiles,
absolute name, publisher/subscription role, complete QoS and overflow policy.
Service supplies node/request/response profiles, absolute name, SetBool type,
client/server role and QoS. Action supplies node plus three schema profiles and
codecs, absolute name, LookupTransform type, client/server role and five QoS sets.
Graph supplies context/node profiles, an ordered unique absolute node allowlist
and field mask. Event supplies its closed field mask.

Every QoS set includes history, reliability, durability, liveliness, depth and
deadline/lifespan/lease seconds plus nanoseconds. Enum values and nanoseconds are
checked; there is no implicit remapping from an invalid native value. Native
effective QoS still must be read back and compared after creation.

## Mandatory capacity families

All families require `definition.bytes`. Context, node, topic, service, action and
graph additionally require `instances`, currently exactly one per binding. Node
requires `observers`, an explicit capacity for dependent providers. Type requires
`descriptor.bytes`.

Topic requires `message.bytes`, `queue.items`, `queue.bytes`, `deadline.ns`.
Publisher queue capacities are explicitly zero; a subscription must reserve its
complete declared queue. Service requires `pending`, `request.bytes`,
`response.bytes`, `queue.bytes`, `deadline.ns`. Action requires `goals`,
`cancels_per_goal`, `feedback.items`, `goal.bytes`, `feedback.bytes`, `result.bytes`,
`queue.bytes`, `transitions`, `deadline.ns`, `retention.ns`; its queue calculation
includes pending goals, critical outcomes, cancellation receipts and feedback.

Graph requires `rows`, `page.items`, `result.bytes`, `cursor.ns`, `capture.ns`.
Event requires `streams`, `sources`, `events`, `result.bytes`, `workspace.bytes`,
`deadline.ns`. Fixed retained records and output workspace remain separately
bounded by their dispatchers.

Every applicable field must be present even when its allowed value is zero.
Missing fields do not acquire permissive defaults. Required positive capacities
reject zero, and zero feedback payload capacity is valid for the empty
LookupTransform feedback schema. Capacities are reservations, not rates.
Optional attenuation belongs to explicit granted authority and must preserve the
distinction between unlimited within the grant and an explicitly zero rate.

The aggregate budget counts contexts, nodes, types, topics, services, actions,
graphs, streams, observer slots, declared queue bytes and definition bytes.
Definition accounting includes the compiled bounded schema's retained graph,
names, capacities and owned descriptor. The deployment aggregate separately
charges startup-file bytes plus that retained schema storage, so a serialized
file-size allowance alone cannot admit an expanded schema. Failed aggregate
updates or worker admission leave previous totals unchanged. These are logical
owned-capacity bounds; allocator metadata, external middleware storage and RSS
also require worker process limits.

A single-node worker can reserve up to 1,024 type descriptions independently of
the native codec registry. Its total declaration ceiling is 1,146: one context,
one node, 1,024 types, 64 topics, 32 services, 16 actions and eight graph profiles.
These are implementation ceilings, not implicit reservations or grants. The
actual declaration count must fit the sum of the explicitly supplied family
capacities before any declaration vector is allocated. A context/node-only
manifest may reserve zero types and zero endpoints. The 16 MiB manifest and
definition-storage ceilings still apply, including compiled schema storage.
Duplicate profiles reject transactionally. Increasing the type catalogue does
not install native factories: unsupported topic/service/action schemas continue
to reject before an endpoint is created.

## Qualification scope

The retained configuration receipts exercise all eight families, every carrier
truncation, corrupted logical hashes, unknown/duplicate/missing fields, profile
phase errors, aggregate rollback and actual recoverable allocation failures.
They use `-O3 -g` with Clang ASan/UBSan, GCC and TSan in the existing 64 GiB,
zero-swap cgroup. These receipts qualify owned configuration and dispatch
composition. They do not substitute for real VM imports, worker isolation,
native interoperability, industrial certification or complete ROS type coverage.

A native worker can require staged `node.open`. Deployment preparation only seals
and validates its images/profile; it does not spawn a process. Under a current
complete context/node group, `node.open` first checks the exact creation budget
and finite resource capacity, then calls the provider's nonblocking startup hook.
A pending stage returns `again` without repeatedly charging the successful node
creation unit. In particular an explicit zero creation allowance never starts a
private worker. The stage retains the original caller identity/incarnation,
context owner and handle, both capability handles, fixed node plan and complete
canonical request digest. Another caller or changed request cannot reuse it.

When startup is ready, the dispatcher charges the creation unit and performs a
fresh complete-origin checkpoint before the nonblocking creation/activation
step. The provider must keep its relay idle until this step. The actual startup
clock starts at process launch; preparation does not cause a stable permission to
expire. Pending startup is bounded by an explicit deadline and staging capacity.
Successful capability close must notify `communication_dispatch::capability_closed`
to fence unpublished stages bound to that capability. Context close, revocation
and environment retirement likewise notify the provider. Capacity cannot be
refunded while actual process cleanup is uncertain. SDK retry adaptation is
future work; this staged contract does not claim the native synchronous ABI.

The portable hook and exact-origin/rate behavior are qualified by
`communication-dispatch-staging-second` (Clang ASan/UBSan, GCC, TSan, `-O3 -g`,
existing 64 GiB zero-swap cgroup). This receipt does not itself qualify a native
worker supervisor or an externally visible deployment.
