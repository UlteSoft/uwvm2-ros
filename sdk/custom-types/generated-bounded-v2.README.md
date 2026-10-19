# Delivered bounded fixture codecs and typed interfaces

[`generated-bounded-v2/`](generated-bounded-v2/) is the complete twelve-file
SDK copy of the officially derived bounded fixture provider under
`tests/ros_custom_types/deployment-final/generated/bounded`; its
[current generation receipt](../../tests/ros_custom_types/deployment-final/generated/bounded/generation.json)
binds the generator source used at that historical generation and its outputs;
it does not bind today's maintained generator. Both delivered copies
remain byte-identical. A source-only revision renames the Rust decoder's private
length reader to `read_count`, avoiding shadowing by a fixed-array loop's local
`count`. The other ten generated products, canonical codecs and official native
identities are unchanged. The original official derivation receipts remain
preserved separately; `composition.json` records this later source revision
without rewriting its historical input provenance or granting a permission.
The directory contains codecs for C, C++, Rust, TinyGo, JavaScript and
AssemblyScript, typed C/C++ interface owners, and the paired trusted manifest
and native provider headers. Its `generation.json` retains the
`GENERATED_SOURCE_ONLY` status and exact output hashes.

The supplied definitions cover the `uwvm_ros_bounded_v2` fixture package for
ROS1 Noetic and ROS2 Jazzy: Point/Envelope messages, Echo and empty-component
Empty services, and the Digest Action. Their official native identities and
component schemas came from the separately prepared official package/export
workflow. This is a concrete finite fixture profile. It is not a registry of
arbitrary installed ROS interfaces, and copied source is not evidence of Guest
compilation or native Runtime delivery.

The [client/server examples](bounded-examples/README.md) include/import these
delivered codecs through `../generated-bounded-v2`. Select exactly one ROS
backend when compiling a program. C/C++ use `bounded_interfaces.h/.hpp`; the
other languages use their generated `bounded_records` module with the existing
SDK capability APIs. A program must validate every result and retain each role
owner until its corresponding delivery has completed.

For your own types, use [`sdk/tools/custom_types.py`](../tools/custom_types.py)
with your complete reviewed catalog, bounded definitions and independently
exported official identities. Generate into a new output directory; do not
overwrite this fixture copy or reuse its identities for different definitions.
Follow the [finite profile rules](BOUNDED_V2.md) and the
[official preparation workflow](../../tests/ros_custom_types/bounded-v2/README.md).

An Instance starts with basic stdin/stdout/stderr and zero ROS authority.
Codecs, SDK factories and registry headers grant no rights. Topic, Service,
Action, observer and optional cancellation roles still require independently
selected CLI aliases, types, resources, identities and finite limits. The
Worker/Gateway must use the matching trusted provider and official package.


## Historical provider versus the current Gateway protocol contract

The delivered Native headers above are preserved historical generated source,
not fresh output from the current bounded generator. In particular the retained
ROS2 header has no complete `custom_native_interface_schema` Service/Action
specializations. The current Gateway correctly refuses such a missing protocol
proof; do not patch a trait or new source SHA into this old product/receipt.
Its Native identities and old compiler/execution observations are not retagged.

For current Native admission use the actual installed-SDK export/bind workflow
and the explicit [fresh selection recipe](native_current/README.md) with a new
output directory. The current source generator can emit the required complete
root/Event/protocol trait, but that source has not been run for this successor.
Fresh source generation, actual paired compile/link and genuine Native/Guest
qualification remain distinct prerequisites. The old fixture remains useful
historical Guest codec/example source; this paragraph does not make it a
currently qualified Native provider or alter any of its twelve product bytes.
