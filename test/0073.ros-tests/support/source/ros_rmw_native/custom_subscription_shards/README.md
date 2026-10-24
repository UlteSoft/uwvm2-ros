# Owned custom ROS2 subscription factories

This additive source package composes the real retained subscription endpoint
with the existing complete custom provider sharding. It is source only: no
candidate, generator, CMake configuration, compiler, linker, model, test, ROS
process, Guest or SSH operation has been run by its author. It does not complete
the still-separate Host/server transport, Guest provider, all SDK versions,
all vendors or network/runtime acceptance.

The public custom API is:

```cpp
ros2_custom_topics::create_owned_rmw_subscription(
    node, capability, source_binding, native_incarnation, endpoint, observed);
```

Its exact signature returns `std::uint32_t`, is `noexcept`, takes a genuine
`rclcpp::Node::SharedPtr const&`, the original `ros::capability_spec const&`,
`ipc::rmw_endpoint::binding const&`, an actual `std::uint64_t` incarnation, a
`std::shared_ptr<native_rmw_subscription::endpoint>&` and a
`ipc::rmw_endpoint::packet&`. The authenticated caller must reserve and retain its
source lease, registry slot, nonreusing native incarnation and rollback BEFORE
calling. These numbers are associations, not credentials. No table registration
or native factory call grants a Guest permission.

A trusted native consumer can include
`src/ros/gateway/ros2/native_rmw_subscription_factory.h` and explicitly select
`native_rmw_subscription::create_selected`. It implements the exact owning
registry's existing factory signature. After that registry's independent source
review and integration, a caller may pass:

```cpp
native_rmw_subscription::registry registry(
    node, &native_rmw_subscription::create_selected);
```

This package does not change or self-review the separate registry. Its original
builtin-only default is unchanged. There is no Guest-selected function address,
automatic registry installation or fallback to the legacy `listen`/alias path.

## Complete types, identities and source boundaries

The original whole `UWVM_ROS_CUSTOM_ROS2_TYPES` macro produces the Native/Codec
tuple and the untyped `owned_publisher_rows` metadata already present for owned
publishers. Subscription deliberately reuses those exact whole row facts: a
second manually maintained type list could omit, reorder or weaken a row. The
old metadata's name is retained to preserve all existing publisher source bytes.
Every Name/NativeId/CodecId/Bytes literal still reaches the selected shard's
`binding<Native,Codec>::known`. Complete official generated type traits,
modern TypeDescription/default/hash or strict older introspection/RIHS,
common manifest and entire finite codec/schema validation are unchanged.
No generated identity, native field, ROS type or capacity is fabricated.

The public wrapper validates original arguments/operation and counts untyped
matching names. Empty/unknown/duplicate rows refuse95. Invalid original inputs
refuse22. It never calls the legacy all-table `known(cap)` function. For ordinal
`i`, the light dispatcher chooses `i % S`; only that generated shard visits
`Shard, Shard+S, ...` and instantiates
`native_rmw_subscription::typed_endpoint<Native,Codec>` with its actual complete
custom contract. The actual endpoint supplies its original same-callback owned
URMI192 plus whole canonical body queue, real actual QoS and strict native
metadata. An unsupported actual metadata extent/timestamp remains95 without
consuming a record; record sequence and independent reservation nonce remain
separate. This factory never peeks, encodes a substitute receipt or consumes.

The new composable selector explicitly dispatches the existing four builtin
String/Bool/Twist/Image subscription contracts first, and otherwise calls the
custom wrapper. A builtin error is final; it is not an opportunity to select a
different codec or reinterpret the same data as custom. Custom typed endpoint
bodies remain visible only in `custom_topics_impl.h`, included exclusively by
generated shard TUs. Four builtin templates are intentionally available in the
separate selector, not added to the light custom dispatcher. Its Node and all
native generated types come from the actual original installed SDK.

Every refusal preserves both caller outputs. Missing complete-QoS/native API
refuses95 through the reviewed native helper. Allocation12, limits90, identity13
and original native errors keep their original semantics; there is no retry,
reopen, dummy entity or success with an empty endpoint. The owning registry
must retire native state before raw source closure and retain uncertain cleanup
outcomes. Host disconnect/revoke/seal and transport publication remain separate
implementation and actual-witness obligations.

All previous owned publisher methods and the four original typed adapter/shard
bodies are unchanged. Only declarations/definitions/table addresses for this
additive subscription factory are inserted. The existing default32 partitions,
1..256 allowed range, complete table and original source/template/CMake helper
remain unchanged. A nonempty custom Gateway still needs main, light dispatcher
and all S generated shards: S+2 TUs. A header-only empty custom route needs no
custom dispatcher or shard objects.

## Actual build and negative-link recipes

`recipe.json` records full command shapes as DATA, with no executed result.
Use a fresh independently owned build directory and a complete integrated source
tree containing this exact package plus the reviewed original native helper,
queue, common manifest, native provider, SDK and Gateway features. Rebuild all
consumers after the codec descriptor's `validation_state` tail addition; this is
source-recompile composition, not compatibility with an old binary layout.
Do not supply fake ROS headers, weak endpoints, forced feature bits or native
raw stubs. Features must come from actual SDK compile/link probes.

An empty-provider source contract uses:

```sh
cmake -S "$source/test/0073.ros-tests/support/source/ros_rmw_native/custom_subscription_shards" \
  -B "$owned_build/empty" -DUWVM_SOURCE_ROOT="$source" \
  -DUWVM_ROS2_GATEWAY_FEATURE_DIR="$actual_features" \
  -DCMAKE_PREFIX_PATH="$actual_sdk_prefix" -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_BUILD_TYPE=Release
cmake --build "$owned_build/empty" \
  --target custom-owned-subscription-contract --parallel 1
```

Choose an actually admitted17/20/23/26 compiler standard. For a full custom
provider, configure another new directory with the same original arguments plus
all three existing complete selection inputs and the actual partition count:

```sh
-DUWVM_ROS_CUSTOM_REGISTRY_HEADER="$actual_manifest" \
-DUWVM_ROS_CUSTOM_ROS2_HEADER="$actual_native_provider" \
-DUWVM_ROS_CUSTOM_ROS2_PACKAGES="$complete_original_package_list" \
-DUWVM_ROS2_CUSTOM_TOPIC_SHARDS=32
```

Do not project the provider table to a sample. Preserve whole original SDK and
source inventories plus every TU's full system-inclusive MD, actual compiler
argv/source/object/log, final complete link input and generated shard bytes.
Require the exact source/object/compile/FileAPI/link bijection and a complete
S+2 TU set. Repeat the complete provider at S1 and S above its row count. No
particular compiler memory/performance result is claimed by source structure.

For an actual missing-shard negative, configure a third fresh full-provider
build with exactly the same arguments plus:

```sh
-DUWVM_CUSTOM_SUBSCRIPTION_OMIT_SHARD=0
cmake --build "$fresh_negative_build" \
  --target custom-owned-subscription-contract --parallel 1
```

That CMake option removes exactly the already generated source for ordinal0
from this NONQUALIFYING contract target. The production source helper/table is
unchanged. It checks the source exists before removing it. All retained objects
must compile successfully, and the final link must fail on the missing original
`topic_shard<0,S>` factory/dispatch symbols. A configure/compile failure or an
unexpected successful link does not satisfy this negative. Retain the full
actual command, stderr and complete source/object/link list. Repeat with an
empty shard where S exceeds the row count. Never run or use an incomplete
negative product as a Gateway or silently omit it from qualification.

The metadata/invalid-argument contract uses actual original SDK headers and
unconditional checks, including Release. Volatile factory pointers retain real
link references against constant-null-call optimization. Its marker covers only
whole row literals/order, shard ordinal coverage, ABI signature and invalid
argument output preservation. It creates no fake Node/entity. It is not native
transport, publisher discovery, metadata availability or ROS delivery credit.

## Selected-row real native witness

Enable a separate complete build with
`-DUWVM_CUSTOM_SUBSCRIPTION_NATIVE_WITNESS=ON` and
`-DUWVM_CUSTOM_SUBSCRIPTION_TEST_ROW=<actual-ordinal>`. Compile
`custom-owned-subscription-native`, then only an independently bounded execution
owner may run:

```sh
"$native_build/custom-owned-subscription-native" --trusted-native-fixture \
  "$unused_absolute_topic" "$actual_complete_canonical_fixture"
```

The row ordinal is an actual whole-provider compile selection, not an invented
public Codec class name. Supply canonical input from an independent complete
native full-value oracle for that actual schema; preserve the original fixture
bytes and identity closure. The tool bounds reads to the complete descriptor's
actual maximum and validates/decode/re-encodes every value before native effects.
The queue's header-plus-body bound must fit the actual unchanged native queue
protocol. Logical zero-byte records remain distinct from their URMI header
storage. No stub fixture, guessed maximum/codec hash or partial message is used.

The native witness uses the original NodeOptions, generated Native and typed
Publisher with actual QoS. It calls the real selected subscription factory,
waits for actual graph matches with an explicit finite native steady-clock
limit, publishes one complete real native value, and checks the same callback's
whole canonical body plus complete actual URMI owner/incarnation/sequence,
actual original publisher GID and timestamps. It rejects forged association and
wrong GID extent without consumption, cancels/reborrows with a new nonce,
rejects both stale commit/cancel, commits once and checks retirement/disappearance
plus no later accepted record. All checks are always active. Missing real API
or metadata returns a nonzero unsupported/error result, not a successful skip.
No CTest hook or implicit process is installed.

Fixture-local association numbers are explicitly NOT observed Host credentials.
Native DDS fixture creation is separately authorized only by that explicit
trusted-process opt-in and the external owner. Native clock/sleep is native
fixture scheduling, not a Guest wait implementation or an invented WASI right.
Arbitrary failed assertions/native traps still require external bounded process,
namespace and session cleanup. Even a real future witness pass would not replace
independent Host/Guest transport, exact source-lease/default-denial/revoke/close,
all selected SDK/schema-family routes, complete build/MD/native-image evidence,
all requested vendors/architectures, performance or security qualification.

## Narrow build closure correction

The actual project parameter parser includes the original vendored
`fast_io_freestanding.h` in the C++20/23/26 concepts branch. This recipe now
requires that real header and explicitly adds
`$source/third-parties/fast_io/include` to every contract target, including all
generated shards. C++17 does not excuse an ambient include dependency in the
other explicitly admitted standards. Full actual vendor-tree/system-inclusive
MD/link qualification is still required; a header existence check alone is not
that qualification. No production factory, witness, model or codec body changes.

The original r1 registry reference stays an immutable historical pending role.
This successor additionally binds the actual registry r3 final source seal and
its independent Recovery peer. Those are source-review facts only, not actual
Native/Host/Guest execution or future source-generation pins. The registry still
needs same-byte integration before a complete real producer build.
