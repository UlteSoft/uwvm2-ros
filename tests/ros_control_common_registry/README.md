# Original control common-registry contracts

This directory contains host-side **source contracts**, not a ROS emulator or
native interoperability evidence. The three source files have not been
configured, generated, compiled, linked or executed in this authoring cohort.
The commands below describe a later, separately admitted validation run. There
is no automatic generation, `try_run`, `add_test`, RPC, ROS node construction or
network access in this CMake project.

The contracts include the real production capability manager, policy/schema,
message validation and custom-registry headers. They also include the actual
`bounded_records.h` generated with the supplied control manifest. They do not
reimplement registry admission, capability validation, handle allocation,
revocation or canonical decoding. A source-compatible C aggregate is a genuine
generated canonical record; it is not a stand-in for a native ROS object.

## Inputs and distinct trust boundaries

Supply an absolute, coherent UWVM2 source snapshot through `UWVM_SOURCE`. It must
contain the reviewed raw-source/aggregate-quota generator and wire grammar, the
original-control registry changes, and the independent legacy/control manifest
composition changes. A partial overlay with only three changed headers is not
a source tree. Do not combine unrelated header revisions by include order.
The source author must bind all these revisions before freezing an executable.

The generator's exact control source profile is
`uwvm-ros-original-node-control-c-cpp-v1`. Its complete selected closure has 32
canonical records, two registered Topics (`Log` and full `ParameterEvent`) and
seven registered Services. Other records, including Time, ParameterValue,
TypeSource and Request/Response components, are dependencies. A dependency
schema is not a new control-publishable Topic. An independently supplied legal
old provider row is still selected by that old explicit provider, where the old
name rules permit it; it does not acquire the control marker or implicit rights.
Native Service protocol/event graphs are
separate complete native proofs; a canonical component inventory does not
replace those proofs.

Use separately generated product directories:

- **Preparation:** `control_manifest.hpp` has empty native identities. Actual
  constexpr registry admission must be false, and every control lookup must be
  null. No placeholder RIHS string is accepted as a preparation convenience.
- **Frozen:** the caller supplies a complete catalog populated from independently
  exported real installed native identities, with its source/export provenance.
  `control_manifest.hpp` and `bounded_records.h` must come from the same reviewed
  generation. The contract does not populate identities, compute RIHS or claim
  that declaration-level validation qualifies the native SDK.
- **Independent legacy provider, optional:** `UWVM_CONTROL_LEGACY_MANIFEST`
  identifies a real separately frozen vendor/standard `manifest.hpp`. Its native
  IDs must also have independent provenance. The input may contain real Topic,
  Service and Action rows. Supplying an empty provider is an error for this
  optional coexistence contract. Do not relabel control records as vendor types
  or reuse one real type's identity for another type to manufacture a positive.

The independent production macros are `UWVM_ROS_ORIGINAL_CONTROL_MANIFEST` and
`UWVM_ROS_CUSTOM_TOPIC_MANIFEST`. Control globals live in
`uwvm2::imported::ros::ipc::original_control_generated`; the legacy global
`custom_*_entries()` functions keep their existing source API. Admission and
lookups use the real `effective_custom_topic_entries()`,
`effective_custom_service_entries()` and `effective_custom_action_entries()`
views. Their combined count/node limits and duplicate/whole-table checks remain
production policy. Linking either table grants no alias or operation.

## Reviewed generation and explicit builds

Generation is a separate step. The source contract project never invokes this
command. After the complete generator cohort is independently reviewed, a
caller may use its actual existing entry point with a fresh output directory:

```sh
coherent=/absolute/reviewed/coherent/source
python3 "$coherent/sdk/tools/custom_types.py" \
  "$coherent/sdk/rmw_wasm/default_control/catalog.preparation.json" \
  --guest-only --out /tmp/control-preparation-products-fresh
```

The same existing `--guest-only --out` interface accepts a separately reviewed
complete frozen control catalog supplied by the caller. The name `--guest-only`
is deliberate: canonical C/C++ sources and the trusted declaration manifest are
not a native registry, native TypeSupport object or capability grant. This
directory supplies no fake exporter, generated product, native header or hash.
Inspect the actual generation proof and every output/source hash before building
the contract. In particular, a legacy generator which does not emit
`control_manifest.hpp` is an incomplete prerequisite, not a reason to hand-write
one.

All build options default OFF. With no selection, CMake adds no contract target.
The following is a future preparation build using explicit absolute inputs:

```sh
cmake -S "$coherent/tests/ros_control_common_registry" \
  -B /tmp/control-registry-preparation-build-fresh \
  -DUWVM_SOURCE="$coherent" \
  -DUWVM_CONTROL_BUILD_PREPARATION=ON \
  -DUWVM_CONTROL_PREPARATION_MANIFEST=/tmp/control-preparation-products-fresh/control_manifest.hpp
cmake --build /tmp/control-registry-preparation-build-fresh \
  --target control_registry_preparation -j1
```

To build the real frozen and negative whole-table images:

```sh
cmake -S "$coherent/tests/ros_control_common_registry" \
  -B /tmp/control-registry-frozen-build-fresh \
  -DUWVM_SOURCE="$coherent" \
  -DUWVM_CONTROL_BUILD_FROZEN=ON \
  -DUWVM_CONTROL_FROZEN_MANIFEST=/absolute/actual-frozen-products/control_manifest.hpp
cmake --build /tmp/control-registry-frozen-build-fresh \
  --target control_registry_frozen control_registry_poisoned -j1
```

For a genuine legacy-provider coexistence run, add
`-DUWVM_CONTROL_LEGACY_MANIFEST=/absolute/actual-vendor-products/manifest.hpp`
to that configure command. It does not replace the control manifest or rewrite
the legacy provider. The CMake file validates input existence and include-path
characters, but does not attest hashes, native provenance or immutability; those
are the caller's source-admission obligations.

The targets require the actual repository fast_io include tree, C++20 and the
host thread library used by the real manager. No installed ROS SDK is needed
for this source-contract layer. A successful host compiler run would not qualify
WASI, generated native C/C++ ownership, native TypeSupport, DDS, Gateway IPC,
original default Node construction or the actual Runtime caller classifier.

Execution is separately explicit, under caller-owned resource limits:

```sh
/tmp/control-registry-preparation-build-fresh/control_registry_preparation
/tmp/control-registry-frozen-build-fresh/control_registry_frozen
/tmp/control-registry-frozen-build-fresh/control_registry_poisoned
```

These lines have not been run for this source cohort. A future report must name
the actual commands, coherent source/product hashes, toolchain, exit status and
resource observations. Do not quote a `PASS` line from source text as a result.

## What each image checks

All images test the exact 32-record/2-Topic/7-Service name domain, backend
separation, generated descriptor/name association, dependency non-Topics and
old aggregate trailing-marker defaults. The old generic name/component grammar
is exercised without inventing an identity. A blank-ID generic descriptor must
remain invalid. An old generic **native positive** requires the optional real
legacy provider; an arbitrary plausible hash is not an adequate substitute.

All images also use the real `capability_manager` to register a trusted local
instance with default-empty targets, attempt open/authorization, check output
handle and lease state, and retire that instance. Registration here tests the
manager's real local contract; it is not a simulated Wasm source admission.
No grant means error13 for open even when a valid type registry is linked.

The old `param.subscribe-events` contract is exercised in every image:

- The old builtin/native projection identity remains selected for that operation.
- Its real operation schema does not acquire `adapter-codec-identity`.
- Its independently specified 38-byte node/name/value payload remains byte-for-
  byte unchanged and valid. A changed name is still refused by actual message
  validation.
- Full control `ParameterEvent` selection is null for this parameter operation.
  The same native name, or even the same native RIHS value, does not merge two
  different canonical domains or grant new operations.

The normal frozen image additionally tests complete real control selection,
both Topic directions, non-Topic operation refusal, all seven Service owner
associations, mixed control/generic and dual control/official markers, swapped
Request/Response roles, wrong backend and wrong owner. It constructs actual
generated complete C Log and ParameterEvent values and passes their actual
encoded bytes to production common validation. Full Topic and legacy parameter
payloads reject each other's grammar. Missing codec ID, a different actual
component's native ID and insufficient byte quota remain failures.

The explicit-grant test uses a real validated target and actual manager
registration/open/authorize/close/reopen/revoke/unregister operations. It checks
the retained lease's identity, operation separation, cross-instance handle
isolation, stale handles, fresh full-handle generation, denied reopen output,
and continued independent legacy parameter authorization. Leases are released
before close/revoke; no callback or concurrency semantics are fabricated.

With a real legacy manifest selected, the frozen image also checks every old
Topic/Service/Action lookup and identity remains available beside control, and
that old non-control operation-neutral lookup semantics are unchanged. It
does not infer that arbitrary operations are authorized or supported merely
because an old descriptor lookup returns a row.

The poisoned image includes the real frozen manifest but changes only one copied
control Topic row's codec declaration to empty. Both original native identities
are retained. Through the actual constexpr production seam, that one damaged
row must disable the whole effective registry: the valid-looking first control
Topic, all control Services, and all supplied old provider rows must become
unavailable. A local-array validator test alone would not establish that actual
global lookup behavior. No malformed row is ever registered or sent to Native.

The preparation image expects five named groups; the frozen image expects eight
(nine with an explicit real legacy provider); the poisoned image expects five.
These are authored inventories only. Actual success counts are pending.

## Memory, authority and further qualification

Canonical wire limits do not describe inline C storage or compiler stack use.
The generated complete values retain all fields/capacities. Larger fixture
records are owned on the heap; actual `sizeof`, allocator peaks and total
resource limits must be measured in the future validation run. The poisoned
declaration table is constexpr and contains no native object or runtime loader.

No source in this directory opens an endpoint, sends a service call, alters
permissions, creates a ROS node or claims a native lease. The configured socket
path belongs only to the production policy-value contract and is never used
for a connection. Complete native metadata/TypeSupport verification, actual
C/C++ ownership, explicit per-endpoint grants, real Gateway behavior and all
Runtime lifecycle checks remain independently mandatory. This contract cannot
turn declaration-level support into full original default Node acceptance.
