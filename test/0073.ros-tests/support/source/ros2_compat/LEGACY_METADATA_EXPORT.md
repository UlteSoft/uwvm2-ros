# Preparing Message identities on pre-RIHS ROS2 SDKs

`src/ros/gateway/ros2/compat/metadata_export.h` provides the native-only
`ros2_compat::legacy_metadata_export::message<Native>(schema, root)` helper.
It is intended for the separately owned native identity exporter, before any
ROS Node or endpoint exists. It returns one complete Message metadata object
compatible with the existing exporter's `official` and `components.message`
roles. It does not itself acquire an SDK, create a process, execute a compiler,
produce a bound catalog, or establish Runtime/native transport acceptance.

Supply the actual generated native Message type and the complete declaration
graph from admitted original interface sources. A native string/sequence bound
must remain the official native bound; a stricter canonical payload quota belongs
in the separate codec schema. The helper compares every actual installed
introspection field and transitive dependency against that graph first. It
refuses missing, reordered, renamed, narrowed, partial, duplicated, recursive,
disconnected or default-bearing declarations supported by neither current
parser nor complete native comparison. It does not fill a missing declaration
from a topic name, a vendor inventory, an expected hash or a canonical buffer.

Only after that native comparison succeeds does the helper derive RIHS01 using
the existing bounded canonical representation and actual OpenSSL SHA256. The
new `rihs_digest` preserves the output array on failure; `rihs_identity` returns
an empty string on refusal or a build without Crypto. `matches_rihs` retains its
raw digest comparison without allocating a new identity string per comparison.
The representation, reference ordering, hash function and prior complete-schema
requirements are unchanged. Completed export JSON transfers its owned buffer
without another whole-document copy.

The JSON states `LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01` and
`native_baked_hash_available: false`. This must never be reported as a baked
hash from an old SDK. A type-support structure that has the native hash API is
always refused by this helper, including null, malformed or unsupported modern
hashes. Use that SDK's original hash/TypeDescription exporter; a broken modern
provider cannot downgrade into this compatibility path.

A Service or Action root includes its complete protocol, event and transitive
standard declarations. Hashing only its Request/Response or Goal/Result/Feedback
payloads cannot establish that root. This helper is specifically for Message
metadata. The explicit vendor Message preparation, binding provenance and
planning chain is now integrated and independently source reviewed; its native
qualification has not run. See `sdk/origin/robots/vendors/LEGACY_ROS2.md` for the
complete Message workflow. Complete Service/Action export remains required;
source existence alone does not close that work. No SDK distribution or
architecture is marked compatible by this header.

## Unified qualification

All three contract sources must compile with assertions enabled (`-UNDEBUG`).
Compilation fails if `NDEBUG` is defined, preventing an empty assertion-disabled
success. Use a genuine selected target compiler, full system-inclusive MD/link
trace, exact source/SDK/sysroot/tool package, actual ELF dependencies and current
stopped-process mappings. All Linux commands must run inside the original
verified 64 GiB, zero-swap cgroup and local output must remain under `/tmp`.
None of the newly authored contracts has executed yet.

- `rihs_contract.cc` retains its independently exported native golden identities
  and complete Service protocol tests, and also checks the new identity/digest
  entry points plus destination preservation on failure. Existing golden data
  remains historical declaration evidence, not current binary qualification.
- `legacy_native_metadata_contract.cc` requires the actual installed
  `builtin_interfaces::msg::Time`, `std_msgs::msg::Header`, their original
  type-support/introspection libraries and actual target OpenSSL Crypto.
  On a genuine pre-hash SDK it checks complete positive metadata export and
  malformed-schema refusals. On a hash-capable SDK it checks mandatory refusal
  of the legacy exporter. It supplies no fake type support, native layout,
  mutable metadata stub or invented native identity.
- `rihs_no_crypto_contract.cc` compiles the production RIHS helper with Crypto
  genuinely disabled. It checks unavailable identity/digest and unchanged output
  on failure. This pure refusal check uses a declaration fixture and proves no
  ROS delivery, SDK compatibility or native identity availability.

Introspection selection may load its concrete native type-support library.
The exporter must materialize and retain all selected metadata before its owned
SIGSTOP/mapping checkpoint, then release the stored document only after the
original bounded `I` gate. The stopped mapping proof must cover those actually
loaded introspection libraries; a prior snapshot before lazy loading cannot
qualify them. Do not infer loaded-library proof from link flags alone.

The native exporter owner still must preserve its bounded startup `I` gate,
whole-MF admission, original per-process full7 provenance, native compiler and
frontend mapping proof, original child/cgroup ownership, separately captured
stdout/stderr, exact terminal status and complete cleanup. An otherwise valid
metadata document is never substituted for those actual process witnesses.
Default ROS rights remain empty; guests receive no update, debug or policy API.

## Primary specifications

- [Official RIHS01 serialization and hashing implementation](https://github.com/ros2/rosidl/blob/iron/rosidl_generator_type_description/rosidl_generator_type_description/__init__.py)
- [Official Humble C++ introspection structure](https://github.com/ros2/rosidl/blob/humble/rosidl_typesupport_introspection_cpp/include/rosidl_typesupport_introspection_cpp/message_introspection.hpp)
- [Official Humble type-support structure](https://github.com/ros2/rosidl/blob/humble/rosidl_runtime_c/include/rosidl_runtime_c/message_type_support_struct.h)

These moving upstream URLs are research references. Actual execution must pin
the independently acquired original source and installed SDK revisions rather
than treating documentation URLs as immutable qualification inputs.
