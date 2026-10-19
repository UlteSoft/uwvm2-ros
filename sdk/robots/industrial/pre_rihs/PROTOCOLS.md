# Complete installed-source Service and Action proofs

This optional path binds complete ROS2 Message, Service and Action catalogs to
an actual SDK which predates native hashes and ServiceEvent. It is a separate
explicit protocol source format. The existing Message-only workflow and modern
hash/TypeDescription contracts retain their own formats and checks. A modern
hash-capable support cannot use this path, including when its hash is missing.

The identity is RIHS01 derived from the selected **legacy** declaration graph.
A Service root contains its actual request and response; an Action root contains
its actual payloads, goal/result services and feedback wrapper. A ServiceEvent
which does not exist in that SDK is not projected into its graph. This derived
identity is not claimed to be a baked SDK hash or the modern event-bearing
protocol's identity. Rebind and independently qualify each actual SDK/build
domain. The native declaration and original-source model follows the
[official Humble rosidl definitions](https://github.com/ros2/rosidl/blob/humble/rosidl_parser/rosidl_parser/definition.py).

## Capture the complete real protocol

Keep one original unbound `uwvm-ros-vendor-complete-bounded-v1` catalog with every
Message, Service and Action definition, constant, declared default and dependency.
Create an explicit request with format
`uwvm-ros2-installed-protocol-idl-source-v1` and an `interfaces` array. Each row
has exactly `type` and `idl`, for example:

```json
{
  "format": "uwvm-ros2-installed-protocol-idl-source-v1",
  "interfaces": [
    {"type": "my_robot/srv/Transform", "idl": "/actual/prefix/share/my_robot/srv/Transform.idl"}
  ]
}
```

This shows request syntax, not a delivered robot interface. Supply the complete
actual inventory and its physical absolute installed paths. For an Action the
inventory must also close the genuine SDK's Time, UUID, CancelGoal,
GoalStatusArray and all their dependencies. The native proof covers these
standard dependencies without adding public Guest descriptors for them.

Run `protocol_idl_export.py` with explicit `--request`, `--parser-root`, `--out`,
`--parser-sha256`, `--definition-sha256` and `--helper-sha256` arguments. The last
hash identifies the actual reviewed `idl_export.py` helper. The real SDK parser
must run in its genuine SDK Python environment. It derives the actual legacy
protocol messages, retains all payload defaults/constants and source facts, and
requires the complete include closure. The SDK parser's injected Action includes
are explicitly retained; they are not invented original source lines.

Run `protocol_source_proof.py` with `--source-root`, `--catalog`,
`--installed-idl`, a fresh `--out`, and actual reviewed `--parent-sha256`,
`--bounded-sha256`, `--adapter-sha256` values. This emits SOURCE, not an execution
receipt or an authenticated identity. All original payload declarations must
match the actual installed IDL exactly, including default presence. Unexpected,
missing, cyclic, disconnected or untyped protocol declarations are refused.

## Compile, own and export

Configure the [native proof project](CMakeLists.txt) against the real SDK with
`UWVM_ROS2_BUILD_INSTALLED_SOURCE=ON`, `UWVM_INSTALLED_PROTOCOL_PROOF=ON`,
`UWVM_SOURCE`, `UWVM_SOURCE_GENERATION`, and its explicitly selected native
packages. `UWVM_NATIVE_PACKAGE` selects one package. Alternatively,
`UWVM_NATIVE_PACKAGES` accepts an explicit CMake list for one merged, deduplicated
catalog. Each package must be actually installed; selecting several packages
does not concatenate incompatible generated bundles or enable Runtime vendors.

Build and run `native-source-contract` and the original-owned
`native-source-identities --mapped-stop` exporter. Preserve the complete SDK,
original source, compiler dependency files, link inputs and actual executable.
Before accepting exporter bytes, its original admitted parent must observe the
stopped direct-child birth, cgroup and kernel-mapped library objects, resume the
child and supply stdin byte `I`. Bound output, a deadline, reaping and mount
cleanup are mandatory owner duties. Source generation, successful configuration,
or a public digest alone supplies none of this execution evidence.

The compiled proof verifies every real payload and protocol message's field
types, container bounds, offsets, defaults and native constants. A Service's
actual introspection pointers must identify its exact compiled request and
response. An Action's five actual links must identify its compiled goal,
result, cancellation, feedback and status support. The complete cancellation
and status message graphs are independently proved even though those standard
associations are not fields of the Action's derived type-description root.

## Bind and select the matching native provider

Use `prepare.py --mode bind` with the unchanged original catalog, actual native
metadata, native owner report and explicit `--installed-source-proof` naming the
generated `native_source_proof.json`. The protocol exporter uses
`uwvm-ros-industrial-native-pre-rihs-protocol-v1`; the proof uses
`uwvm-ros2-installed-protocol-source-proof-v1`. Neither modern metadata nor a
Message-only proof can substitute for these protocol roles. The complete-vendor
source operator API also accepts this proof through its admitted optional input.

Compile the matching `native_source_provider.hpp`, bound manifest and generated
native converters coherently into the selected Gateway. The wrapper includes
the explicit Message and interface proof specializations. Without that wrapper,
complete legacy binding stays refused. The generic modern interface-schema
trait still describes the modern path; the explicit legacy interface trait
supplies this SDK's actual graph. Cross-pairing them must fail qualification.

For the actual production Gateway, an explicit build selection has this form:

```sh
cmake -S /reviewed/source/gateways/ros2 -B /fresh/owned/gateway \
  -DCMAKE_PREFIX_PATH='/actual/robot-prefix;/actual/ros-prefix' \
  -DCMAKE_CXX_FLAGS='-O1 -g0 -I/fresh/owned/bound/generated' \
  -DUWVM_ROS_CUSTOM_REGISTRY_HEADER=/fresh/owned/bound/generated/manifest.hpp \
  -DUWVM_ROS_CUSTOM_ROS2_HEADER=/fresh/owned/proof-source/native_source_provider.hpp \
  -DUWVM_ROS_CUSTOM_ROS2_PACKAGES=my_robot
cmake --build /fresh/owned/gateway --parallel 1 --verbose
```

The extra generated include directory pairs the wrapper with its actual bound
`native_ros2.hpp`; it must be the same reviewed generation used by the manifest.
The Gateway target and its actual SDK compile/link probes include the reviewed
repository root for these proof headers. Its native feature manifest must report
the actual selected Action lifecycle result; do not replace a failed probe with
a manually asserted capability. Successful compilation and a no-configuration
startup refusal do not establish a running ROS endpoint or network delivery.

Reconfigure with `UWVM_BOUND_GENERATION` to build `native-protocol-contract`,
`native-protocol-binding` and `native-protocol-unproved-binding`. The latter
compiles the same bound converters without the explicit proof wrapper and
requires refusal. A matching independent value/CDR oracle can be selected with
`UWVM_PROTOCOL_VALUE_ORACLE`; its C++20 target additionally requires the actual
reviewed `UWVM_FAST_IO_INCLUDE` tree. Retain full physical input closure.

The shared production Action admission rejects absent codec fields and accepts
only the seven implemented send, receive, cancel, advertise and respond
operations. Metadata qualification cannot enable the concrete Action lifecycle:
that remains gated by the separate actual compile/link availability probes.

## Authority and acceptance remain explicit

All vendor switches stay default-off and every Guest begins with zero ROS
authority. Select each actual module, backend, alias, resource, operation, type
identity, QoS and finite limit through the existing explicit policy. These tools
add no update, debugger, credential, file, socket or GPU capability to Wasm.

The owned fixtures under `tests/ros2_compat/fixtures/installed-source` use the
genuine SDK generators. Their independent oracle initializes every original
field and constructs expected canonical bytes without the production parser or
codec emitter. Actual compiled converter and CDR runs are separate owner stages.
These are local authored tests. They do not replace official upstream suites,
vendor/controller/firmware checks, ROS network delivery, simulation, hardware,
remaining architectures or equivalent Native/Wasm VTune measurements.
