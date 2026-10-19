# Explicit WASI C++ archives for Direct builds

Direct builds accept an explicitly selected pair of physical `libc++.a` and
`libc++abi.a` inputs from one nonthread, non-LTO WASIp1 library profile. Older
SDKs may use `SYSROOT/lib/wasm32-wasip1`; SDK33 also provides separate `noeh`
and `eh` directories below that path. These names describe library layouts.
They do not change the existing C++ exception flags or certify a compiler ABI.
The producer never searches these directories, chooses a preferred profile,
substitutes an archive, or changes a selected pin to make it fit.

`prepare_direct_guests.selected_cpp_archives(frame, api, sysroot, labels)`
requires the two existing Frame input labels. It loads the reviewed
`prepare_legacy_guests.wasi_cpp_archive_profile` definitions through the same
complete current manifest as the Direct producer. Both archives must be
physical files in the same exact supported profile, match the selected actual
full-seven observations, and belong to the existing source/whole-input scope.
Mixed profiles, another SDK, LTO, thread and WASIp2 variants, symbolic aliases,
unselected labels and changed bytes fail. No new root or Guest right is added.

The three current routes all use this association:

1. `prepare_direct_guests.build` checks the pair before any compiler owner is
   created. Every C and C++ main links the explicit two selected paths and must
   retain both actual full-seven archive pins in its real linker trace. The
   producer report records the selected pair; each applicable product receipt
   separately records `actual_link_trace_contains_both: true` only after that
   real link check. JavaScript links no C++ pair and receives no such claim.
2. `direct_independent_inputs.stage_configuration`, reached through
   `prepare_direct_library_configuration.py`, uses actual completed A1/A2
   library inputs and explicit current tool labels. Its pair observation is
   metadata only. The later real build repeats the check and proves the link.
3. `acceptance/make_direct_build_configuration.py` retains its strict completed
   Typed14 route and performs the same explicit pair check after its normal
   Frame admission. It still starts no compiler and publishes an unexecuted
   invocation. It does not manufacture terminal Typed14 or library receipts.

The C archives remain exactly `lib/wasm32-wasip1/libc.a`, `libm.a` and
`libc-printscan-long-double.a` inside the selected sysroot. Compiler/sysroot,
official C++ abort-object policy, complete-MD, actual object/link inputs,
validators, current roots, source preservation and default denial remain
unchanged. The previous independently reviewed Direct JavaScript closed8
association remains part of this successor without modification.

`acceptance/direct_build_configuration/derive_request_r2.py` and its sealed
Source43 metadata-launch owner are historical controls with an exact Source43
manifest and prestaged helper hashes. They retain the old flat archive lookup
as historical source and must not derive a current SDK33 request. They are not
rewritten or relabelled. A current v1 request must explicitly supply the actual
pair's two full-seven pins in `tools`; current A1/A2 v2 construction likewise
requires explicit labels. The current request template already has these two
roles. A profile name alone is never a file selection or permission.

`test_direct_cpp_archive_layout.suite(direct, legacy, guard, out)` authors ten
injected filesystem policies. Root must load both modules and the factory from
one complete current manifest and supply its live original64GiB/zero-swap guard
and an empty owned directory. The cases check real ordinary fixture files and
the production association; they do not run a compiler, parse a real archive,
link or execute Wasm, initialize ROS, or count as official upstream tests.
The shared six Legacy layout cases remain separate, unchanged policies.
Genuine SDK33 build/link/validation results require fresh owned execution.
