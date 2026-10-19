# Explicit original ROS2 control profile

This source integration connects the complete original default-Node control
adapters to the production ROS2 Gateway and to the Runtime's compiled canonical
catalog. It is source implementation awaiting complete source review and later
unified execution. Nothing here establishes a tested ROS distribution, a Native
SDK qualification, a Guest execution result or a performance claim.

The original Guest Node retains its real defaults. Its complete Log and
ParameterEvent publishers, TimeSource ParameterEvent subscription, six parameter
Services and GetTypeDescription Service need separately granted exact aliases.
Native DDS Parameters/ParameterEvents QoS depth1000 is preserved. Immutable
canonical service lifetime history remains finite and separately admitted;
those records are a different resource from the middleware's native queue.
Actual middleware objects/buffers, allocator overhead and process memory must
also fit the owning execution envelope. A canonical wire cap or numeric queue
ceiling does not prove a physical memory bound.

## Separate inputs and explicit selection

All relevant build and runtime defaults remain disabled. Select the build only
with `UWVM_ROS2_BUILD_DEFAULT_RMW_CONTROL_PROFILE=ON`, together with the existing
explicit owned RCL service transport and C++20. The Gateway consumes three
separate, already reviewed directories:

- `UWVM_DEFAULT_CONTROL_CANONICAL_DIR` contains actual generated
  `bounded_records.h`, `control_manifest.hpp`, `control-schema.json` and their
  generation proof. It uses the complete original source catalog and preserves
  every declared field/default. Preparation output has empty Native identities
  and is deliberately unavailable to production lookup.
- `UWVM_DEFAULT_CONTROL_GATEWAY_GENERATED_DIR` contains the separately generated
  all-C++ native Gateway proof/converters/typed endpoint translation units,
  `generated-sources.cmake`, required identity names and generation provenance.
  No generated ROS SDK structs or guessed layout/hash replaces the installed SDK.
- `UWVM_DEFAULT_CONTROL_GATEWAY_IDENTITIES_DIR` contains
  `default_control_gateway_identity_input.hpp` and `identity-input.json`, frozen
  from a separately acquired actual Native export packet and its explicitly
  supplied SHA256. A native getter must not provide its own expected identity.

The build does not run a source generator, exporter or test on the caller's
behalf. Before configuration, the external owner must retain actual source,
compiled-tool and SDK/library closure identities. A path, marker or matching
input JSON alone is not executable or Native qualification. The selected
adapter's genuine SDK compile-and-link probe must succeed before its complete
profile can become available; an unsupported SDK route refuses95.

The canonical control table uses `UWVM_ROS_ORIGINAL_CONTROL_MANIFEST`, alongside
an optional ordinary `UWVM_ROS_CUSTOM_TOPIC_MANIFEST` for Unitree or another
explicit vendor provider. Their namespaces differ. Common validation combines
their complete tables under the original count/node ceilings, rejects duplicate
native names and disables every lookup on an invalid row. No valid-looking
prefix or vendor family bypasses that whole-table check. Every consumer of the
extended descriptor/bounds structures must recompile from the same source.
A single build usage interface carries the complete selected catalog into the
Gateway main, every translation unit of the control static library, the existing
owned custom-service dispatch and all its shards, and linked control contracts.
The control library exports that interface to its actual consumers. Inline
registry validity and lookup definitions therefore use the same complete tables
in every object linked into one process. The optional legacy value contract also
consumes this selection. Independent probes that do not link those objects keep
their own separately selected catalogs.

When a vendor provider is selected, that interface includes both original
common/native header macros, the actual Topic/Service partition counts and each
explicit provider package's installed C++ generator, TypeSupport and introspection
export targets. A missing real target refuses this complete composition during
configuration. The existing owned custom-service export/layout selection stays
on its original owner; the control profile does not borrow its Native proof.
Selecting only one manifest on a subset of these objects is unsupported: it can
split whole-table poison checks and violate the C++ inline-definition contract.
Rebuild the complete linked source set after either catalog changes.

The Runtime's corresponding explicit build option is
`--ros-original-control-manifest-header=/absolute/reviewed/control_manifest.hpp`.
It requires `--import-ros2=y` and the existing supported Linux header build.
`--ros-custom-registry-header` may independently select the ordinary vendor
manifest. Neither option grants a connection, alias, clock, file, process,
update, debug or ROS operation. Use the existing Runtime policy commands to
supply each intended authority explicitly; an empty target still has no ROS
rights.

## Runtime storage file

After the complete original control build is selected, explicitly pass:

```text
uwvm-ros2-gateway /absolute/trusted/gateway.conf \
  --default-rmw-control-profile /absolute/trusted/control.storage
```

`control.storage` is an owned, safely permissioned ordinary Linux file. The
existing real no-follow/nonblocking config reader admits it before reading.
At most32 lines,256 bytes per line and4096 accumulated text bytes are accepted.
Empty/comment lines do not create fields. Exactly three unique two-column
fields are mandatory:

```text
maximum-entries <explicit positive bound at most65536>
history-per-endpoint <explicit positive bound at most256>
export-packet-sha256 <actual64-lowercase-hex digest from the selected frozen export>
```

These angle-bracket values describe required operator input; they are not a
ready-to-use profile, invented export or default grant. The digest must equal
the actual compiled `export_packet_sha256`. Unknown, repeated, missing or
malformed fields refuse startup. Ordinary numeric parsing uses the existing
fast_io-backed bounded config parser, without a separate decimal implementation.
The immutable Native control profile is prepared only after this explicit Host
selection and before constructing the real Gateway Node. It independently
validates actual complete C++ TypeSupport, sources, defaults, fields, hashes,
layouts, protocol Events and ownership metadata. A compiled schema alone cannot
substitute for that proof. Preparation output or a partial two-Topic/seven-
Service family refuses startup.

The existing `--owned-rmw-service-profile` remains independent. Both selections
may be passed explicitly for different service families, in either order. Their
service storage limits must be identical because one actual session registry
owns the shared service history. Conflicting limits refuse startup instead of
silently enlarging a profile. Selecting only the original control profile does
not require unrelated String, Empty or AddTwoInts identity inputs or an old
CPP10 Registry. The old fixed/custom provider is used only when its own runtime
profile was explicitly selected.

## Routing and lifetime

The cold publisher/subscription/service factories capture a strong reference to
the same immutable independently proved profile. Actual typed endpoint objects
retain it through their one native retirement and final release. No ambient
process singleton, Guest function pointer, raw unowned metadata capture or
cross-Source cache selects a profile. Publish/take/peek/commit/cancel continue
through each retained actual endpoint; the new Host factory callable is used
only during creation or canonical bounds admission.

The complete original control entities use the owned RMW transports. Legacy
lazy publication/subscription, transient service calls, legacy service advertise,
respond and availability paths return95 for these selected full control types.
There is no silent fallback creating a different Native object. Actual GID/QoS
receipts, incarnation, source/session binding, immutable event history and
whole-packet output transactions remain required by those owned transports.

Full `rcl_interfaces/msg/ParameterEvent` Topic traffic and the existing
`param.subscribe-events` node/name/value projection are distinct canonical
operations. Their identity/schema and payload selection uses the operation:
linking a complete Topic cannot add `adapter-codec-identity` to the old projection,
change its Native identity, or reinterpret its smaller wire body. The original
two-argument type-identity API remains available for existing source callers.

Every per-endpoint operation still crosses the Runtime capability manager and
its real source/session handle leases, exact identity/codec/QoS association,
explicit quotas, revocation and one-shot ownership checks. A storage profile
contains no permissions. Guest ROS calls cannot select this Host build/profile,
modify the update/debug manager, widen the policy or call a native process.
Signing, WasmGit, a hub and multiprogram download/update orchestration remain
outside this runtime integration.

## Future qualification

No commands in this document have been run for this source cohort. After all
source integration is complete, the external owner must freeze a coherent
Runtime/Gateway/Guest/SDK tree, generate in fresh caller-owned output, verify the
actual SDK compile-and-link closure and execute the genuine default Node and
all endpoint paths. Local test/build output belongs only in `/tmp`. Linux work
must verify its actual64GiB/swap0 cgroup before each payload and respect current
memory, disk and whole-command deadlines. Native-vs-Wasm measurements, real
VTune hardware hotspots/microarchitecture collection, official distro/vendor
suites and QEMU architecture results require separate actual execution evidence.
