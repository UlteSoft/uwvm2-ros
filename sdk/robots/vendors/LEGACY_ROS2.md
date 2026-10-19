# Preparing complete Message identities with older ROS2 SDKs

`legacy_ros2_exporter.emit(parent, bounded, records, interfaces)` returns the
complete C++17 source of a native Message identity exporter. The administrative
preparer must pass the actual current-MF `custom_types` and `bounded_profile`
modules plus the complete unbound values returned by their `load_catalog`.
It performs no filesystem/process/network operation, computes no native hash in
Python, loads no SDK and grants no Guest authority. Generated source remains
uncompiled and native identities remain unavailable until its original owner
performs the actual native run. The vendor preparation request now accepts the
explicit `identity_mode: "legacy-introspection"` selection and carries its
complete Message metadata through binding and planning. This is integrated
source; actual native qualification has not run.

The complete catalog must contain ROS2 Messages only, including all explicitly
catalogued dependencies. Original Header/Time/JointState spellings require the
explicit official-standard profile. Ordinary custom Messages use their original
profile. The emitter refuses bound identities, another ROS major, partial or
duplicate records and Service/Action roots. A complete Service/Action exporter
needs its official protocol/event graph and native component validation; it
cannot substitute a hash over only the application payload messages.

Every static native declaration preserves the complete ordered original fields
and actual fixed-array/string/sequence capacities. The stricter canonical wire
bounds are deliberately separate. Every include names the actual generated
native Message header; no stub, projected native struct, fabricated layout or
replacement type support is emitted. Source construction stops at 16 MiB while
building the text, instead of first accumulating an oversized graph.

The emitted executable selects only the genuine pre-hash Message path in
`gateways/ros2/compat/metadata_export.h`. That helper first compares the full
installed native introspection graph, then derives RIHS01 using the existing
bounded official representation and real target OpenSSL. Each metadata object
retains `LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01` provenance and explicitly
reports that a baked native hash is unavailable. Any type-support structure
with the modern hash API is refused even when its hash is absent or malformed.
There is no automatic downgrade from the normal modern native exporter.

## The actual execution boundary

Build the emitted source with C++17, `-DUWVM_CUSTOM_NATIVE_ROS_VERSION=2`, the
actual target OpenSSL feature/link selection and the real native SDK. Retain
the full system-inclusive dependency files, original compiler/frontend/ELF,
complete linked and installed native support packages, raw stdout/stderr and
whole frozen source evidence. The generated program creates no Node or endpoint.

The program materializes every selected introspection library and complete
metadata object before its owned SIGSTOP checkpoint. Its at-most-16-MiB document
stays in native memory while the owner observes the actual stopped process birth
and mappings, including the concrete introspection libraries. Only one original
bounded stdin `I` gate permits the document to be written to stdout. All JSON
appends are bounded before mutation; the program emits no partial stdout if a
schema, modern-SDK downgrade, Crypto, size or pre-checkpoint preparation fails.
Do not qualify lazy-loaded libraries with a snapshot taken before materialization.

The independent owner must still perform selected-root/full7/same-FD admission,
original pidfd/child ownership, exact cgroup limits, raw stream capture, terminal
status and cleanup. JSON status fields do not establish those proofs. A failed
modern-only invocation is a refusal and cannot count as a successful old-SDK
native identity export.

Feed only the original actual exporter document through the existing exact
inventory binder, retain its original owner evidence and emit the six codecs
from that bound catalog. Existing Runtime/Gateway startup checks still compare
complete actual native schema and recompute/compare the derived identity before
any endpoint creation. An alias stays denied unless trusted Host policy grants
it explicitly. No update/debug/policy/management import is exposed to Guest.

All Linux generation/compilation/execution belongs in the verified 64 GiB,
zero-swap cgroup; local test artifacts belong under `/tmp`. The emitter and
Message preparation/binding/planning chain have received independent source
review and are integrated. They have not been imported, invoked, generated,
compiled or tested. Subsequent same-source old/modern native exporter tests
and original-owner process evidence are mandatory before describing this
preparation path as qualified. No vendor, distribution,
architecture, native delivery or official test count is inferred from it.
