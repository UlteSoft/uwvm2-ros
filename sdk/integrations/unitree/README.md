# Official Unitree native identities and provider preparation

This integration is **source-prepared, not executed**. It supplies a complete
finite native identity exporter and an administrative preparation/composition
tool. It does not implement a robot relay, create a ROS Node/context/endpoint,
contact a robot, grant a Guest alias, download a package or certify a ROS
distribution. Root must perform all actual generation/build/export/admission
under the provided64GiB/swap0 Linux cgroup after the source checkpoint.

The external definitions and source locks are owned by
[`sdk/robots/unitree`](../../robots/unitree/README.md). No upstream definition,
legacy license placeholder or generated native identity is vendored here.
Obtain the actual upstream source at the exact reviewed commit and preserve its
license/notice and complete build dependency lock. Older Unitree legacy source
licensing remains the external dependency owner's responsibility.

| Profile | ROS | Official native package | Complete Messages |
| --- | --- | --- | --- |
| unitree_go |2| unitree_go |24|
| unitree_hg |2| unitree_hg |11|
| unitree_api |2| unitree_api |8|
| unitree_legged_msgs |1| unitree_legged_msgs |11|

These54 Messages retain every official field. Canonical finite string/sequence
ceilings do not change the native MD5/RIHS or pretend that native ROS
serialization equals the bounded Guest wire format. `unitree_api` Request/
Response are Messages on topics, not invented native ROS Services/Actions.

## Native metadata contract

`native_identity.cpp` includes a literal `selection.hpp` generated from the
preserved source preparation. Every header/type/package is derived from an
exact allowed profile Message name; there is no dynamic type name, eval, shell,
Node factory or Guest-controlled library lookup. C++17 uses the existing
checked `native_io` compatibility path because vendored fast_io requires newer
language concepts. No iostream or native object-padding serialization is used.

The exporter builds the entire bounded JSON before writing stdout. Exit0
means a successful metadata export only. A refusal exits nonzero, emits a
bounded stderr diagnostic and publishes no partial successful JSON. The top
level is exactly the existing binder's three fields:

```json
{"format":"uwvm-ros-official-bounded-native-v2","ros_version":2,"types":{}}
```

The actual nonempty `types` map contains the exact catalog names, each with
`native_identity` and `components.message.native_identity` (equal for a Message).
Additional per-type/component evidence records the real `official_type`,
`identity_source`, and actual schema or ROS1 Definition. The empty map above
illustrates shape only; it cannot qualify or bind any profile.

ROS1 uses the actual generated `ros::message_traits::DataType`, `MD5Sum` and
`Definition`, with exact selected DataType and lowercase32hex MD5 checks.
It never recomputes a Message MD5 from a catalog or changes field case.

ROS2 traverses actual official C++ introspection recursively, including the
nested `member.members_->data` MessageMembers. It derives every complete name,
field order/type, fixed array, native bounded/unbounded sequence and string
capacity from those actual structures. Package-only pre-IDL metadata receives
the standard `/msg/` category explicitly; it is not a suffix match to arbitrary
catalog text. A selected native root must match the full expected type.
The walker rejects cycles, duplicate/conflicting record identities, duplicate
field names, nonnull field defaults, unsupported scalars (wide strings/long
double included), inconsistent containers, malformed pointers/counts, and
unsupported empty records. The finite Unitree profiles have no empty Message.
Depth is at most16, records at most256, fields per record at most4096 and
aggregate emitted schema fields at most16384. Complete output is at most4MiB.

The canonical RIHS representation reuses
`gateways/ros2/compat/rihs.h::rihs_json` and OpenSSL SHA256. It uses **only this
actual introspection graph**, sorted referenced records, and native capacities;
no catalog hash/schema can stand in for missing native metadata. A modern
type-support struct with `get_type_hash_func` must supply a nonnull version1
nonzero32-byte hash equal to that actual schema. Missing/malformed/disagreeing
hashes fail; they never fall back. Only a struct without that member takes the
explicit `LEGACY_INTROSPECTION_RIHS01` path. That label says exactly how the hash
was obtained; it does not falsely claim a pre-Iron SDK had a native hash API.
Modern exports use `OFFICIAL_GET_TYPE_HASH`.

## Exact preparation and actual build sequence

The following are future Root commands, **not actions run while authoring**.
Use canonical absolute paths, an admitted frozen `$SOURCE`/`$MF`/`$MF_SHA`, a
fresh owned `$WORK`, actual pinned tools and a reviewed SDK setup. Reject
unfilled variables. Every actual native command is serial/-j1 with finite
deadline, complete child-tree RSS/ownership/log guards, original cgroup identity,
zero swap, reserve, no OOM and child-first pidfd cleanup. Retain failures.

First materialize each complete family using the existing source utility:

```sh
"$PYTHON" -B "$SOURCE/sdk/robots/unitree/materialize_catalog.py" \
  --upstream-root "$EXTERNAL_UPSTREAM" \
  --profile "$SOURCE/sdk/robots/unitree/profiles/$FAMILY.json" \
  --lock "$SOURCE/sdk/robots/unitree/upstream-lock.json" \
  --out "$WORK/$FAMILY-source"

"$PYTHON" -B "$SOURCE/sdk/integrations/unitree/prepare.py" prepare \
  --source "$SOURCE" --source-manifest "$MF" --source-manifest-sha256 "$MF_SHA" \
  --profile "$SOURCE/sdk/robots/unitree/profiles/$FAMILY.json" \
  --catalog "$WORK/$FAMILY-source/catalog.preparation.json" \
  --materialization-report "$WORK/$FAMILY-source/report.json" \
  --out "$WORK/$FAMILY-selection"
```

The second command executes metadata copying only: it reopens the exact frozen
source/profile/lock and original materialization inputs/outputs, verifies all
definition bytes against the upstream lock, and emits `selection.hpp`,
`selection.cmake`, `selection.json`, copied exporter/CMake source and a complete
catalog/definition snapshot. `preparation.json` has `PREPARED_SOURCE_ONLY`,
all opened input/output pins and explicit false compiler/export execution flags.
Outputs are exclusive, never resumed/overwritten. Failures preserve their
receipt and partial files; select a new output path for a retry.

Before compiling this exporter, actually build/install the official interface
package with its official generators. A base ROS runtime or Guest codec build
does not produce native type support. The explicit source package roots are:

* ROS2: `$UPSTREAM_ROS2/cyclonedds_ws/src/unitree/unitree_go`, `unitree_hg`,
  `unitree_api` at the commit in the profile/upstream lock.
* ROS1: `$UPSTREAM_LEGACY/unitree_legged_msgs` at its separately locked commit.

Use their actual official CMake projects, for example one package at a time:

```sh
"$CMAKE" -S "$OFFICIAL_PACKAGE_SOURCE" -B "$WORK/$FAMILY-package-build" \
  -DCMAKE_CXX_COMPILER="$NATIVE_CXX" -DCMAKE_INSTALL_PREFIX="$NATIVE_PREFIX" \
  -DBUILD_TESTING=OFF -DCATKIN_ENABLE_TESTING=OFF
"$CMAKE" --build "$WORK/$FAMILY-package-build" --target install -- -j1
```

Supply the actual compiler target/sysroot/toolchain and generator Python from
the reviewed SDK profile where necessary; do not reuse ARM SDK libraries in a
native x86 build or run a target Python extension as a host generator. Pin
official generator scripts/templates, package definitions, SDK/tool roots and
complete actual full-MD/link closure before/after. Installing into a new owned
native prefix is an explicit output, not modification of the user's SDK.
ROS2 needs `rosidl_default_generators`, actual C++ type-support/introspection
implementations, and OpenSSL; ROS1 needs its actual catkin/genmsg/gencpp and
declared dependencies. Missing APIs/packages/licenses are explicit preparation
blocks, not compatibility failures or a stubbed success. For authenticated
SDK acquisition prerequisites see
[`tools/ros_distro/README.acquisition.md`](../../../tools/ros_distro/README.acquisition.md).

Then configure/build the selected exporter, again in a fresh build directory:

```sh
"$CMAKE" -S "$WORK/$FAMILY-selection" -B "$WORK/$FAMILY-export-build" \
  -DUWVM_SOURCE="$SOURCE" \
  -DUWVM_UNITREE_SELECTION_DIR="$WORK/$FAMILY-selection" \
  -DUWVM_UNITREE_CXX_STANDARD=17 -DUWVM_UNITREE_CXX11_ABI=SDK_DEFAULT \
  -DCMAKE_CXX_COMPILER="$NATIVE_CXX" \
  -DCMAKE_PREFIX_PATH="$NATIVE_PREFIX;$REVIEWED_ROS_PREFIX"
"$CMAKE" --build "$WORK/$FAMILY-export-build" --verbose -- -j1
```

Select20/23 only if the actual SDK requires/supports it. ROS1 libstdc++0/1 may
be selected only from the existing actual ABI compile/link proof; SDK_DEFAULT
does not claim an ABI result. CMake performs no target `try_run`. It exports
`compile_commands.json`, `native-build-contract.txt`, explicit full-MD
dependency output, normal `link.txt` and actual verbose `--trace` link inputs.
The configured text itself is **not** a successful build report. Preserve the
actual final effective `-MD -MF`, every dependency, generated support/header,
linker input, executable ELF/architecture, mapped library, command status and
raw log. Cross/native loader and library roots must be explicitly admitted.

Run the actual exporter under the same owned guard, preserving separate raw
stdout/stderr. Its successful stdout is the new official metadata file:

```sh
"$WORK/$FAMILY-export-build/unitree-native-identities" --mapped-stop \
  > "$WORK/$FAMILY-native-metadata.json" 2> "$WORK/$FAMILY-native-metadata.stderr"
"$PYTHON" -B "$SOURCE/tests/ros_custom_types/bounded-v2/bind_catalog.py" \
  --template "$WORK/$FAMILY-selection/catalog/catalog.preparation.json" \
  --native-metadata "$WORK/$FAMILY-native-metadata.json" \
  --output "$WORK/$FAMILY-bound/catalog.json" \
  > "$WORK/$FAMILY-binding-receipt.json"
```

The guard must check actual exporter exit0 before invoking bind. Redirection
alone cannot prove exclusive/fresh output or cleanup; Root's admitted runner
must enforce those conditions. The binder's parent output directory must not
already exist. It retains original templates/metadata/complete definitions and
reports `NATIVE_IDENTITY_BINDING_SOURCE_ONLY`, not Runtime registration.

`--mapped-stop` is mandatory for Root's actual native-library evidence. Before
stopping, the exporter resolves every selected support/introspection dispatcher
so actual cached DSOs can be observed. It then cooperatively raises SIGSTOP,
before exporting any JSON. Root must observe the same owned birth/pidfd, actual
current64 cgroup/boot/device/inode and actual executable/support mappings, pin
their complete SDK inputs, use the existing `Lane.stopped(..., cooperative=True)`
or equivalent exact proof, resume once, and send exactly one stdin byte `I`.
EOF/wrong/short input refuses export; there is no clock/thread/ROS context.
A plain shell redirection is only the argv/output shape above; it will stop
until that supervisor acts. Standalone SDK usability allows no argument, which
exports without the stop but **does not qualify mapped-library evidence**.
Unknown/extra arguments are refused. Generic SerialRunner completion alone
cannot substitute for the stopped-map/native-support qualification.

## One common Worker/Gateway table per selected deployment

Repeat the independent native export/bind workflow for all four families. The
primary supported preparation is **one complete family per generated provider**:

```sh
"$PYTHON" -B "$SOURCE/sdk/tools/custom_types.py" \
  "$WORK/$FAMILY-bound/catalog.json" --out "$WORK/$FAMILY-generated"
```

Keep each selected Worker/Gateway pair on that same provider. Do not include
four separately generated `manifest.hpp` files or overwrite macro definitions.
If a deployment also needs retained official bounded-v2 fixture catalogs,
compose the **bound data** first and run the existing generator once. Pass
matching catalog/receipt pairs in the same order and select each intended
complete Unitree family explicitly. For example, to compose the GO family:

```sh
"$PYTHON" -B "$SOURCE/sdk/integrations/unitree/prepare.py" merge \
  --source "$SOURCE" --source-manifest "$MF" --source-manifest-sha256 "$MF_SHA" \
  --family unitree_go \
  --bound-catalog "$WORK/unitree_go-bound/catalog.json" \
  --binding-receipt "$WORK/unitree_go-binding-receipt.json" \
  --out "$WORK/unitree-combined-bound"
"$PYTHON" -B "$SOURCE/sdk/tools/custom_types.py" \
  "$WORK/unitree-combined-bound/catalog.json" --out "$WORK/unitree-generated"
```

Add explicitly paired `--bound-catalog` / `--binding-receipt` options for retained
fixtures. Merge reopens each original template/export/binding/catalog/definition,
requires every exact current type of each **selected** family and disjoint names,
and copies complete definitions under unique relative paths. `composition.json` states
`COMPOSED_SOURCE_ONLY`; it does not replace native binary/library qualification.
Actual generation must still enforce the unchanged4096-node per-record and
16384-total expanded-node ceilings. The complete GO/HG/API/legacy inventories
can exceed the aggregate ceiling if combined, especially with retained fixtures.
This tool does not promise an all54-type monolithic provider, raise limits,
silently omit fields, reduce declared capacities or turn composition into a
generation pass. Select independent family deployments if the actual generator
refuses a combined catalog. Each family remains a required independent source/
native preparation target; no rejection is renamed compatibility success.
The generator emits six-language codecs and the common `manifest.hpp` plus
`native_ros1.hpp`/`native_ros2.hpp` with their Topic macros and empty Service/
Action macros where no such interfaces were selected. Preserve `generation.json`.

If fixed-v1 remains part of the final task, compose its held generated provider
with this one new bounded provider using `sdk/custom-types/compose_profiles.py`
(`--fixed`, `--bounded`, `--output`). Retain the old bounded fixture catalogs in
the merge above as well; replacing them with Unitree is not full acceptance.
Freeze the complete resulting common/native products and all catalogs/export/
preparation/generation receipts into a **new** source manifest (or admit every
external header dependency explicitly). No mutable path-only provider is enough.

Both final Worker and corresponding Gateway must compile the **same exact**
common registry bytes via `UWVM_ROS_CUSTOM_TOPIC_MANIFEST`. The Gateway also
selects `UWVM_ROS_CUSTOM_ROS1_HEADER` or `UWVM_ROS_CUSTOM_ROS2_HEADER` and links
the actual packages. Existing native CMake inputs are:

* `UWVM_ROS_CUSTOM_REGISTRY_HEADER` = common manifest physical path.
* ROS1: `UWVM_ROS_CUSTOM_ROS1_HEADER`,
  `UWVM_ROS_CUSTOM_ROS1_PACKAGES=unitree_legged_msgs` plus retained fixture packages.
* ROS2: `UWVM_ROS_CUSTOM_ROS2_HEADER`,
  `UWVM_ROS_CUSTOM_ROS2_PACKAGES` = the explicitly selected `unitree_go`,
  `unitree_hg` or `unitree_api` package(s), plus retained fixtures. A semicolon
  list must exactly match the actually admitted combined provider.

Registry/type support **grants nothing**. Every live low-command publication,
low-state subscription or topic correlation still needs its own complete
explicit policy identity, canonical bounds, QoS, quotas and actual principal/
session binding. Default authority remains empty. Native compilation, metadata
binding, source generation and real ROS/robot/simulator delivery are separate
evidence stages; none may be silently promoted into the others.
