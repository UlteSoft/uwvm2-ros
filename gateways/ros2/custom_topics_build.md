# Building custom ROS2 topic adapters

The Gateway partitions custom rclcpp publisher/subscriber template
instantiations across translation units. Its trusted generated provider still
selects the complete native type table. This changes compilation structure;
it does not select a smaller type catalog or grant any ROS operation.

The optional CMake setting is:

```text
UWVM_ROS2_CUSTOM_TOPIC_SHARDS=32
```

The default is 32; custom builds accept integer values 1 through 256. Existing
Gateway custom builds need no new argument. A smaller value assigns more
native types to each translation unit. A larger value can leave empty shards.
Every value retains the entire provider table, including tables longer than
the shard count. The implementation uses C++17 and the Gateway's existing SDK
compatibility layer. It does not lower Release optimization.

## Configuring and building

Continue to supply the existing three trusted custom selections: an absolute
common manifest header, an absolute native provider header, and the explicit
installed ROS2 package list. They must come from the same complete generated
interface selection. For example, with actual paths and packages substituted:

```sh
cmake -S gateways/ros2 -B build/ros2-custom \
  -DCMAKE_BUILD_TYPE=Release \
  -DUWVM_ROS_CUSTOM_REGISTRY_HEADER=/absolute/generated/manifest.hpp \
  -DUWVM_ROS_CUSTOM_ROS2_HEADER=/absolute/generated/native_ros2.hpp \
  '-DUWVM_ROS_CUSTOM_ROS2_PACKAGES=selected_msgs;selected_api'
cmake --build build/ros2-custom --target uwvm-ros2-gateway -- -j1
```

To change the partition count, add
`-DUWVM_ROS2_CUSTOM_TOPIC_SHARDS=<actual-count>` to configuration. Use a fresh
build directory for qualification. Build serially with `-j1` when controlling
peak compiler memory; the partition count is not a parallel-job setting or a
memory guarantee. Select and enforce process limits through the build owner.
No particular SDK's compatibility or memory use follows from these sources
alone.

With all custom selections empty, the builtin Gateway still compiles only
`main.cc`. No custom dispatcher or shard object is added to that target.

## Translation-unit and type boundaries

For a custom build with actual count `S`, the executable has exactly these
`S+2` translation units:

* `<source>/gateways/ros2/main.cc`
* `<source>/gateways/ros2/custom_topics_dispatch.cc`
* `<build>/custom-topic-shards/custom_topics_shard_0.cc`, continuing through
  `custom_topics_shard_<S-1>.cc`, with every ordinal present once.

The default custom build therefore has 34 translation units. CMake expands the
maintained `custom_topics_shard.cc.in` using only the numeric shard index and
count. All target provider/manifest definitions, installed package dependencies,
include paths, language settings, and optimization flags apply to main,
dispatcher and every generated source equally.

`custom_topics.h` contains metadata admission, session state and lightweight
dispatch declarations. The private publisher/subscriber method definitions
are in `custom_topics_impl.h`, which only generated shard sources include.
Neither main nor the dispatcher sees those rclcpp template bodies. This is
out-of-line dispatch, without an inline implementation that could defeat a
template-instantiation boundary.

The complete `UWVM_ROS_CUSTOM_ROS2_TYPES` macro creates the native/codec tuple.
For row ordinal `i`, the dispatcher selects shard `i%S`; that shard visits
ordinals `Shard`, `Shard+S`, `Shard+2*S`, and so on. Tuple bounds are checked
with `if constexpr` before selecting a row. Each real row therefore belongs
to exactly one shard, even when there are more rows than shards. No separate
hand-written type list or provider-text parser can omit a native row.

For example, 32 GO+API rows use one typed pair per default shard. An 11-row HG
table uses 11 nonempty and 21 empty shards. An empty topic table is valid. The
dispatcher references every shard's external functions, so an omitted shard
object fails linking instead of silently qualifying a partial table.

Metadata qualification still checks the real official name, complete native
schema/hash, codec identity and policy. The unchanged typed method bodies keep
lazy first-publication publisher creation, payload bounds/validation, actual
topic name and QoS checks, alias/limits equality, and canonical field conversion.
Subscription callbacks remain gated until native admission, check finite
active size before allocation, and retain the same queue and retirement
lifetime. Registration creates no entity or permission. There is no generic
CDR substitute, fake native type, automatic grant, or discovery workaround.

## Other native adapter consumers

`custom_topics.cmake` exposes the shared source-setup function used by Gateway
and the standalone custom-topic fixture:

```cmake
include("/absolute/source/gateways/ros2/custom_topics.cmake")
uwvm_ros2_custom_topic_sources(existing_target)
```

Call it once after creating a real target and supplying its actual paired
provider/manifest compile definitions, SDK/package dependencies, and normal
language/optimization settings. It adds the dispatcher and complete generated
shard set to that same target. It does not discover packages, choose a provider,
or change those existing target settings. Header-only metadata consumers do
not need shard objects. A consumer that invokes the adapter's publish or
subscribe operations must link this implementation; including the private
implementation header in the caller defeats the intended boundary.

## Build evidence and qualification

The earlier “one Gateway translation unit per variant” assumption applies only
to builtin builds now. A custom producer and independent qualifier must bind
the actual shard count and require the complete `S+2` source/object set. Use
actual CMake FileAPI source records, the compile database and generated build
rules to establish the source-to-object mapping. Verify generated source bytes
against the frozen template and numeric substitutions; a filename or an
`isGenerated` flag alone is insufficient.

Retain a full compiler dependency receipt for every translation unit, including
its actual source, object, dependency file, compile argv/status/log and resolved
physical header inputs. The main report may reference each receipt by its
complete physical pin instead of repeating all header lists, while preserving
and reopening the entire dependency union and complete source/SDK inventories.
Require a bijection among FileAPI sources, compile records, dependency receipts,
real objects and final link inputs. Missing, duplicate, stale or unlinked shard
evidence fails qualification. Independent helper/probe targets retain their
own source/object inventories and are not Gateway shards.

This implementation was reviewed as source only. No compiler, CMake configure,
test, Runtime, ROS participant or performance measurement was run while
authoring it. Actual builds must establish SDK compatibility, complete native
type coverage and linking; actual runtime witnesses remain necessary for
startup and ROS behavior. Build success alone does not establish any of those
runtime results.

## ARM fixture integration

The ARM custom-topic controller uses this same helper with three shards. Its
ROS2 target has five translation units: the unchanged original native fixture,
the dispatcher, and all three complete-provider shards. Its ROS1 target still
has one original fixture translation unit. This partition setting does not
restrict which generated message types either fixture can use.

The ARM preparation format now records every actual TU separately. It retains
the FileAPI source set, compile database, unique verbose compiler command and
final system-inclusive dependency output, native AArch64 object, original and
archived source/object/dependency bytes, pinned per-TU receipt, and the final
link's exact complete object set and SDK/dynamic-link arguments. Each TU also
needs observed compiler and frontend ARM/QEMU process mappings whose argv names
that source. Reuse reopens all those records and full dependency closures; a
compile database alone cannot qualify compilation. The single pre-main SIGSTOP
constructor remains on the original native fixture source only, so adding
shards cannot introduce repeated stops. Existing payload, ROS behavior, process
ownership, cgroup and cleanup checks remain required.

## Separately owned original-RMW custom publishers

`ros2_custom_topics::create_owned_rmw_publisher(node, capability, binding,
native_incarnation, endpoint, receipt)` adds the eager owned path. It reuses the
separately reviewed `native_rmw_publisher::typed_endpoint<Native,Codec>`; it does
not call the legacy adapter, publish a dummy value, or use a second publisher
for metadata. All original lazy publish/subscribe/close bodies stay unchanged.
The actual authenticated Registry must reserve its owner, exact source binding,
fresh nonreusing native incarnation and rollback BEFORE calling this factory.
No provider row or factory call grants authority or creates an IPC registration.

The same complete provider macro produces the existing Native/Codec tuple and
an untyped name/native-identity/codec-identity/extent table. The public wrapper
counts matching names without calling the legacy all-table `known` function.
A missing or duplicate row refuses; an empty table returns95 without custom
objects. For its unique ordinal `i`, the dispatcher addresses only the external
factory symbol for shard `i%S`. The generated shard TU alone recursively visits
`Shard, Shard+S, ...`, qualifies the exact original row literals through
`binding<Native,Codec>::known`, and instantiates the genuine typed endpoint.
This retains modern complete TypeDescription/default/hash proof, old strict
introspection/RIHS, exact common descriptor and bounded/full payload codec.

Main and dispatcher see no actual owned-publisher rclcpp implementation body.
The dispatcher includes only `custom_topics.h`, takes function addresses and
routes ordinals. A shard includes `custom_topics_impl.h`, which now includes the
actual owned publisher template. No extra generated source, type list, CMake
parser, row limit or compilation-language change is introduced. The original
complete `S+2` TU/object/link/dependency rules still apply. Every shard object is
required when the table is nonempty; an omitted object must fail linking.

The returned endpoint keeps the same actual publisher through inspect, business
publish and retire, with its real actual QoS/GID and full original native extent.
The factory leaves both outputs unchanged on every refusal, including missing
complete-QoS/API95, invalid original arguments22, row-policy identity13, limits90,
allocation12 and original native errors. There is no automatic replay. Native
retirement-before-raw-owner-close and all failed-cleanup evidence remain the
external Registry's responsibility. Builtin String/Bool/Twist/Image dispatch is
separate; this generic custom table never projects or replaces those adapters.

`tests/ros_rmw_native/custom_publisher_factory_contract.cc` is an unexecuted
selected-SDK source contract for full table metadata/order, all shard ordinal
coverage and invalid-argument output preservation. It deliberately creates no
fake Node, endpoint, native receipt or ROS participant. Actual positive native
publisher creation/retirement, modern/default/legacy schemas, SDK versions,
complete MD/link closure, Runtime permissions/transport and ROS delivery still
require independently owned actual qualification. The source contract's future
marker is not any of those results. No source, generator, CMake, compiler, test,
ROS process or Guest was run while authoring this addition.
