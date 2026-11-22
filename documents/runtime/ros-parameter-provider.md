# Bounded parameter scopes and node authority

The `rosv2` parameter provider implements a local transactional store attached
to one explicitly admitted node. It expresses the ROS1 and ROS2 parameter value
algebras through separate deployment profiles. It does not impersonate a native
ROS parameter server, load YAML from ambient paths, or inherit a ROS-v1/WASI
grant. All twelve imports now have central VM dispatch, CLI configuration and
cached debugger views. Native parameter-service bridging remains a separate
provider obligation. Actual Wasm qualification is tracked independently from
source registration and the core/native-node integration receipts below.

## Configuration and admission

Each `local-parameters` binding fixes a parameter profile, a distinct node
profile, a sorted exact key list, value/parser bounds, type masks, descriptor
constraints and optional declaration overrides. A guest descriptor may narrow
that policy; free-text descriptions cannot widen it. An override is already
owned canonical data supplied by trusted configuration, not a file lookup.
The deployment additionally fixes the exact node grant alias. A matching
profile in another worker cannot supply that origin. The five public CLI
options and mandatory aggregate/per-alias limits are specified in
[node-bound local parameters](../command-line/ros-scene.md#node-bound-local-parameters).

`parameters::configuration` is an inert builder. Call `profile(selected,
node_profile, algebra)`, assign every named `limit(name, value)` once, supply
`definition(owned_bytes)`, then `freeze(plan)`. The overload
`freeze(plan, aggregate_maximum, aggregate_used)` checks and reserves the whole
plan atomically before publishing it. Missing limits, repeated limits, missing
definition fields, an unknown type or a mismatched source profile all fail
closed. A deliberate zero is distinct from an omitted limit. The helper never
opens a file, loads a provider or grants authority.

The twenty required limit names are:

- `scopes`, `parameters`, `value_bytes`, `event_slots`, `event_bytes`, `cursors`;
- `workspace_bytes`, `batch`, `request_bytes`, `receipt_bytes`, `pending_bytes`,
  `cursor_bytes`;
- `validation_timeout_ns`, `cursor_timeout_ns`;
- `value.bytes`, `value.nodes`, `value.elements`, `value.depth`;
- `definition.bytes`, `definition.keys`.

These are finite capacities or parser/deadline bounds. A deployed maximum of
zero remains zero; there is no unlimited numeric sentinel. Aggregate accounting
separately sums configured stable capacity and owned definition storage,
including fixed descriptor metadata and override backing capacity. Per-binding
scope times event/cursor metadata products are also checked. A failed aggregate
freeze leaves both the output plan and the used budget unchanged.

The canonical definition starts with a text magic
`uwvm.ros.parameter.definition`, the parameter profile identity and a 32-byte
logical profile SHA256 blob. The digest covers every byte of the remaining
body, excluding these three header fields. The independently configured
parameter identity must also match. A transport SHA256 covers the entire file,
including the header. Deployment must verify that separate file digest while
performing its explicitly bounded trusted configuration read; the logical
digest is not a pathname or an authority token. Capacities and optional rates
are deployment restrictions outside the semantic profile body.

The body contains the exact node profile, `ros1` or `ros2`, explicit nonfinite
and deferred-validation booleans, absolute floating tolerance, the bounded ROS1
search root and maximum candidate count, and a sorted array of exact keys. The
search root and count are part of the hashed semantic profile. Every key supplies its complete descriptor, unique allowed
types in numeric tag order, explicit independent integer/floating constraint
presence, and explicit override presence. Present ranges carry minimum,
maximum and step; a present override carries one complete canonical value.
Absent fields and trailing bytes are invalid. Overrides must pass the same
algebra, type and both numeric constraints before configuration can freeze.

Trusted descriptor name, description and additional-constraints metadata are
fixed. Guest numeric ranges may narrow them; read-only status cannot relax,
and dynamic typing cannot be introduced without the configured permission.
ROS1 dynamic descriptors may select another explicitly allowed type without
converting the heterogeneous XML-RPC algebra into ROS2's type union. ROS2
`not_set` is an explicit value; ROS1 has no corresponding nil value here.

`tools/ros_scene/encode_parameter_definition.py` converts a closed JSON schema
into this canonical definition and prints both digests. It refuses unknown,
missing or duplicate JSON fields and existing output files. The root fields
are `profile`, `node_profile`, `algebra`, `nonfinite`, `deferred_validation`,
`floating_tolerance`, `search_root`, `search_candidates` and `keys`. `profile` contains `identity`; `node_profile`
contains `identity` and `sha256`. Each key contains `descriptor`, `types`,
`integer_range`, `floating_range` and `override`. Every descriptor contains
`name`, `type`, `description`, `additional_constraints`, `read_only`,
`dynamic_type` and `range`. Numeric range objects contain `minimum`, `maximum`
and `step`; a descriptor range additionally contains `kind` (`integer` or
`floating`). A null range or override is an explicit absence. A present
override is `{ "type": TYPE, "value": VALUE }`; the present ROS2 unset value
is `{ "type": "not_set", "value": null }`. Bytes use canonical base64.
ROS1 arrays contain typed values; ROS1 structs contain unique `{name, value}`
members. ROS2 arrays contain only values of their declared element type. The
VM parser remains authoritative for every configured bound and constraint.

`search_root: ""` with `search_candidates: 0` explicitly disables ancestor
search. An enabled search requires the ROS1 algebra, a canonical absolute root
(`/` is permitted), and a candidate count from 1 through 256. The count includes
the starting namespace; it limits work independently of the key/value capacity.
The encoder and VM reject incomplete or inconsistent pairs and any ROS2 profile
that enables ROS1 search. An empty root is not an ambient namespace grant.

## Existence and ancestor search

`parameter.exists` accepts `[scope, name]` and returns
`[exists: bool, initialized: bool, snapshot_revision: u64]`. It requires its own
action grant and an exact configured key before looking in the store. A missing
key differs from a declared ROS2 `not_set` value: the latter exists but is not
initialized. Neither result reveals the value or descriptor. An unauthorized
name returns `denied`, never a false existence result. The snapshot revision
identifies the coherent store view, including a negative result.

`parameter.search` accepts `[scope, leaf, start_namespace]` and returns
`[resolved_name: optional<text>, snapshot_revision: u64]`. The optional text is
encoded as a little-endian u64 presence flag followed by the text only when present. The leaf
must be canonical and contain no slash; the starting namespace must fall within
the configured root at a path-component boundary. The provider checks each exact
candidate key before observing existence and walks toward the root. An
unauthorized candidate stops with `denied`; it cannot be skipped to disclose a
parent value. Reaching the root without a match is the only negative-search
result. Exhausting the candidate budget returns `too_large`, not not-found.

Both operations use one locked store snapshot, reserve bounded detached output,
and perform final authority and cancellation checks before delivery. Short
output does not consume admission. Search does not access a filesystem, remap a
guest-selected native namespace or contact a remote parameter server.

The encoder reads at most 4 MiB from its input and produces at most 4 MiB.
Invoke it with explicit paths, for example
`python3 tools/ros_scene/encode_parameter_definition.py --input policy.json
--output policy.canonical --metadata policy.metadata.json`. Encoding performs
no native ROS action. The generated metadata distinguishes `definition_sha256`
from `profile.sha256`; substituting one for the other must fail verification.

`parameter.open` supplies both `node_capability` and the node resource identity.
The communication dispatcher resolves the actual node and its original context
authority. All later parameter operations retain that exact dependency and
revalidate it. The broker holds the parameter and node authority group through
the local effect and delivery. Previously created nodes are validation-only
dependencies: they do not consume another `node.open` creation-rate unit. Their
`max_observers` reservation limits retained attachments independently.

The dispatcher adapter is a nonowning pair of typed function pointers bound to
the trusted communication dispatcher. Deployment must keep that dispatcher and
its authority alive until parameter callers drain. No callback comes from
guest bytes, and a numeric resource alone never proves node ownership.

All permission starts at zero. An explicit scope enables stable execution;
optional call and byte limits apply to already authorized calls. An absent
rate is unlimited within the grant, while an explicit zero rejects the call.
The actual parameter request and measured reply are charged after output fits.
No short output or denied final admission commits a mutation. A revision race
after charging may return `again` for an attempted call, without changing data.

## Capacity and allocation

Every binding has finite limits for scopes, declared parameters, canonical
value bytes, event slots/bytes, cursor slots/filter bytes, pending candidates,
request/response receipts, workspace, batch size and encoded request size.
The number of preallocated event/cursor metadata slots per scope is checked
against the configured scope product. Capacity is never interpreted as a rate.

All VM-owned payloads and container backing storage use the recoverable
fast_io adapters. Before allocating, the provider reserves the appropriate
capacity. Old and new value bytes are accounted during replacement; detached
responses and old receipt data remain covered until their actual allocation is
destroyed. Move operations transfer both the pointer and its charged extent.
The ordinary terminating allocator is not used for these failure paths.

Closing a scope drops its local values, events, cursors and pending validation.
Its counted node attachment and retained scope slot survive until any active
copy finishes. A close/open loop therefore cannot evade the observer or scope
bound. Cleanup requires only the scope's parameter close grant, so parent-node
revocation cannot make that cleanup require fresh node authority.

## Transactions and validation

`declare`, `undeclare`, `set` and `set.atomic` share one monotonic key stream per
scope. The complete canonical request is retained for the last accepted key.
A different body under that key conflicts; exact retries return the current
receipt without applying values again. A new accepted mutation acknowledges
the previous key. Allocation, capacity and authority failures before admission
do not consume the key. Constraint refusals are themselves terminal receipts.

Non-atomic set returns one decision per unique key and commits its accepted
subset as a single coherent local revision. Atomic set commits every candidate
or none. Type, read-only and numeric-range checks apply again to any replacement
returned by a validator. Undeclare events carry the prior canonical value and
an explicit `undeclare` change tag.

Optional deferred validation reserves both a validation event and the future
committed event before queueing the candidate. A queued receipt means locally
accepted work, never committed parameters. `parameter.take` must successfully
deliver the validation event before `parameter.respond` can answer it. Failed
or throwing sinks do not consume the event; concurrent/reentrant takes return
`again`. Replacement values must cover exactly the staged candidate names.
The complete accepted batch moves into place without allocation at commit.

Validation uses a finite absolute host-steady deadline. Expiration is lazy on
admitted operations; there is no autonomous watchdog. Inspection and rejected
short-output preflight do not advance deadlines. A late validator cannot commit
the candidate, and timeout releases both pending and reserved future-event data.

## Reads, cursors and public request identities

Get and describe return one detached coherent snapshot. List returns canonical
`ros.parameter.name` messages with the `canonical.text` encoding. Its cursor
binds the exact scope, revision, filter, depth and expiry. A changed filter or
revision conflicts; an expired cursor times out. Failed delivery does not
publish a newly reserved cursor. Keys outside the trusted profile are denied.

`request.describe` and `request.release` use the origin parameter grant only.
They read or retire the last owned receipt and perform no new node access.
Release refuses pending validation. Retiring a public request identity does
not discard the bounded canonical last-key proof: the original mutation still
supports exact retry while its scope and action remain usable. A new accepted
mutation or scope close reclaims that proof. Failed release acknowledgement can
be retried; the second release reports `false` once the identity is retired.

Every public operation checks the creating process before acquiring registry
locks. Forked children cannot reuse node handles, workspace tokens or imported
authority as a way to operate the parent's provider.

## Qualification status

`parameter-store-owned-move/results.json` under the SDK
`.tmp/ros-scene-security` directory records six core security groups across
Clang ASan/UBSan, GCC and TSan. It covers local atomic/partial transactions,
deferred validation, cursor/replay behavior, finite capacity, concurrent
admission and node lifetime. Its retained source closure predates the newer
public request retirement and authority dispatcher additions.

`parameter-broker-allocation-frozen/results.json` records nine successful
Clang ASan/UBSan, GCC13 and TSan commands from a retained immutable source
closure, including six store and seven dispatcher security groups. The earlier
`parameter-broker-allocation-final` attempt passed its commands but is explicitly
incomplete because a concurrently edited communication dependency changed.
The dispatcher qualification adds exact parameter/node/context grants,
exhausted node-creation quotas, parent revocation, actual malloc-family failure
injection, close during delivery, and public request retirement. The first
configuration cohort, `parameter-configuration-first/results.json`, additionally
passed five configuration groups and predates the stricter override-consistency
check and canonical encoder roundtrips. The later
`parameter-configuration-tool/results.json` records fourteen successful
commands, all six store, seven broker and six configuration groups, 52 strict
JSON rejection cases, and actual canonical ROS1/ROS2 file decoding by both
Clang ASan/UBSan and GCC13. TSan also passed. That receipt retained 411 input
files and verified their unchanged hashes. These are provider and
broker tests, not industrial certification, native parameter transport, real
Wasm execution or a near-native performance claim. All compilation and tests
run inside the existing 64 GiB zero-swap cgroup; performance builds use
`-O3 -g` with source and disassembly retained for VTune attribution.
