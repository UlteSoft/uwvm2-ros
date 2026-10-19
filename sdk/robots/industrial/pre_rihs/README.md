# Explicit installed-source proofs for older ROS2 Message SDKs

This optional native build path supports complete Message schemas on SDKs that
predate native type hashes and TypeDescription. It creates no ROS Node, endpoint,
registry, Guest import or permission. Modern hash-capable SDKs always use their
existing hash/TypeDescription contract; a missing or broken modern hash cannot
select this path. Services and actions use the separate explicit
[complete protocol proof workflow](PROTOCOLS.md).

The trusted native provider must explicitly compile a generated specialization
of `installed_source::traits<Native>`. Without that specialization, complete
pre-RIHS Message binding remains refused. A matching digest or metadata document
does not authenticate a build. The independent build owner must retain the
original source, SDK, compiler dependencies, link inputs, executable, actual
mapped libraries, process identity and resource containment evidence.

## Capture actual installed IDL facts

Start with the original unbound complete catalog. Capture every selected
Message's installed `.idl`, including all referenced types, from the genuine
SDK package. Supply a closed request document:

```json
{
  "format": "uwvm-ros2-installed-message-idl-source-v1",
  "records": [
    {"type": "my_robot/msg/State", "idl": "/actual/prefix/share/my_robot/msg/State.idl"}
  ]
}
```

The example is a request shape, not a delivered robot interface. Use the exact
complete inventory of your catalog and fresh absolute output paths throughout.
Run `idl_export.py` with the genuine SDK Python environment and explicit
`--request`, `--out`, `--parser-root`, `--parser-sha256` and
`--definition-sha256`. The parser root is the directory containing the actual
`rosidl_parser` package. Both hashes identify reviewed SDK source bytes.

The exporter captures the original parser, definition and grammar sources and
parses captured IDL bytes. It retains fields, constants, default presence and
values, annotations, and source facts. It rejects incomplete inventories,
unsupported semantic annotations and changed inputs. It emits no native identity.
In particular, a null introspection default pointer cannot prove that a default
was absent. Original MSG spelling must be interpreted by the selected SDK's
adapter; escaped source spelling is not automatically its semantic value.

## Generate and execute the typed native proof

Run `source_proof.py` against the same reviewed source revision, passing
`--source-root`, `--catalog`, `--installed-idl`, `--out` and the actual
`--parent-sha256`, `--bounded-sha256`, `--adapter-sha256` values. Its generated
`native_source.hpp` checks actual field types and containers, native constants,
constructor defaults, typed offsets, complete genuine introspection graphs and
nested type-support associations. Floating defaults preserve signed zero.
Source digests bind the original complete declarations and installed-IDL facts.

Configure this directory's CMake project with explicit absolute inputs:

```sh
cmake -S /reviewed/source/sdk/robots/industrial/pre_rihs \
  -B /fresh/owned/native-proof \
  -DCMAKE_PREFIX_PATH='/actual/robot-prefix;/actual/ros-prefix' \
  -DUWVM_ROS2_BUILD_INSTALLED_SOURCE=ON \
  -DUWVM_SOURCE=/reviewed/source \
  -DUWVM_NATIVE_PACKAGE=my_robot \
  -DUWVM_SOURCE_GENERATION=/fresh/owned/proof-source
cmake --build /fresh/owned/native-proof --parallel 1 --verbose
```

Use genuine native headers, libraries and OpenSSL Crypto. Preserve full compiler
dependency output and link traces. Run `native-source-contract`, then run
`native-source-identities --mapped-stop` only under its original admitted owner.
After all actual native proofs and identity derivations, the exporter raises
SIGSTOP. Its original parent must capture the stopped child identity, executable,
concrete mapped libraries and cgroup, resume it, and supply stdin byte `I`.
Apply a deadline and reap the owned child. Exporter source generation, a CMake
configuration, or an unattended stopped process is not execution evidence.

## Bind the original catalog and select the provider

Use the unchanged original catalog with `prepare.py --mode bind`, adding actual
`--native-metadata`, `--native-owner-report` and
`--installed-source-proof=/fresh/owned/proof-source/native_source_proof.json`.
Pass the same source pins and a different fresh output directory. The explicit
proof parameter is required; the regular modern metadata route cannot silently
downgrade. The `vendors/complete_prepare.py` source API also accepts the explicit
proof through its separately admitted `installed_source_proof` input.

The generated bound manifest and converters must be used coherently throughout
the selected Runtime and separately built native Gateway. Set the Gateway's
`UWVM_ROS_CUSTOM_ROS2_HEADER` to the matching
`native_source_provider.hpp`, include its matching bound generation directory,
and select that bound manifest normally. This wrapper explicitly includes the
typed proof before the matching `native_ros2.hpp`. Do not combine independent
catalogs or mix proof, manifest and converter revisions. The per-vendor xmake
options remain default-off and select no native SDK automatically.

Reconfigure this CMake project with `UWVM_BOUND_GENERATION` to build both
`native-source-binding` and `native-source-unproved-binding`. The first exercises
the actual production binding and default-value conversions. The second compiles
the same bound converters without the proof wrapper and requires refusal.
For an independently generated full-field native codec oracle, also pass
`UWVM_NATIVE_CODEC_ORACLE`; its `contract.c` is compiled as C++20 against this
same provider. The original owner must separately execute these targets.
The C++20 oracle also requires the actual reviewed fast_io include tree. It
defaults to `third-parties/fast_io/include` under `UWVM_SOURCE`; a separately
admitted readonly tree can be supplied with `UWVM_FAST_IO_INCLUDE`. Retain its
complete source inventory and the compiler's actual dependency files. A header
name or a different checkout is not a substitute for those compiler inputs.

Every Guest still starts with zero ROS authority. Model, topic/service/action,
operation, type identity, QoS and finite limits require the existing explicit
grants. These native source tools add no socket, update, debugger, policy or
credential capability. Local native message proof does not establish network
delivery, controller compatibility, robot behavior, official-suite acceptance
or performance.
