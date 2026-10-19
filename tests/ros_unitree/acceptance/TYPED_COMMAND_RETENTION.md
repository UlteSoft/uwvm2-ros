# Retaining complete typed commands from a failed batch

These helpers handle one exact historical Source40 compiler report. They do not
start a compiler, validator, Wasm initializer, Runtime, Gateway, ROS peer or
simulator. Successful metadata predicates cannot qualify any persistent file
or native behavior on their own.

The immutable report has SHA256
`b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747`
and 3,911,428 bytes. It records nine individually compiled and fully validated
Rust, embedded JavaScript and AssemblyScript commands. Its tenth product row
is a **RUNNING** TinyGo attempt with no completed artifact or product receipt.
The batch itself remains **FAIL**. A continuation may separately qualify the
nine complete products and build three TinyGo commands plus the C and C++ API
commands afresh, for five fresh products. It
must never describe the incomplete TinyGo row as an existing product.

The actual producer's `Frame` reports `source`, `source_manifest_sha256` and
the full original manifest pins in `input_pins`. It does not serialize its
private `source_before` attribute. Four repeated manifest pins in this real
report are identical observations of one original manifest. The RSS-aborted
command has no `returncode` member because the guard interrupted command
collection before an exit status was recorded. Reading an absent member is a
consumer bug. Adding a fabricated `source_before`, null exit status or guessed
process result would corrupt the original evidence.

`typed_command_retention.admit_batch` verifies these exact metadata contracts,
including the original failure, successful owned cleanup, nine distinct complete
products, real successful command/validator statuses, empty rights and unexecuted
Runtime/native scopes. The caller must decode the very raw bytes that it opened
and hashed. `bind_current_source` then accepts the real complete
`tools/ros_distro/common.py:source_state` result and compares the current complete
manifest content with its original recorded content. It retains historical and
current manifest identities separately and explicitly leaves persistent-input
and Runtime qualification pending.

The complete continuation must additionally reopen all original source, SDK,
native generation, compiler and product roots; every selected persistent pin;
individual build receipts; genuine Rust pre-exec/linker stdout receipts; JS full
system-header depfiles and archive traces; and the actual Wasm binary's complete
imports, exports, entry and memory table. The original independent C++ object
producer has its own qualification. None of those checks is replaced by this
small metadata helper.

After a reboot or remount, an old device number is historical evidence. Use the
separate `requalify_after_restart` helper with explicit finite physical-root
mappings, full streamed content checks and current cgroup admission. Keep both
old and current full-seven pins. Never return an old-looking pin from a current
file opener or modify the embedded original report to look freshly produced.
Old process births, map observations, return codes and cgroup facts remain old;
current physical-file qualification does not rerun an old process.

`test_typed_command_retention.py` contains fourteen regressions using the actual
unmodified report as the positive fixture. Negative cases change in-memory
copies only. Root invokes it inside the explicitly selected current64GiB,
zero-swap cgroup, after separately pinning this entrypoint and both imported
sibling helpers in the actual control closure:

```text
python3 -B test_typed_command_retention.py \
  --source ABSOLUTE_ORIGINAL_FROZEN_SOURCE \
  --manifest ABSOLUTE_COMPLETE_SOURCE_MANIFEST \
  --manifest-sha256 ACTUAL_COMPLETE_MANIFEST_SHA256 \
  --cgroup-selection ABSOLUTE_CURRENT_CGROUP_SELECTION_JSON \
  --cgroup-selection-sha256 ACTUAL_SELECTION_SHA256 \
  --report ABSOLUTE_ORIGINAL_SOURCE40_FAILED_REPORT
```

Only the final actual execution can produce a metadata-regression result. This
source file records no such result. Even a successful regression run leaves
retained product requalification, five fresh builds, all28 native denied/positive
Runtime cases, physics and performance as separate work.

`derive_typed_command_continuation.py` creates a fresh source derivative of the
exact recovered90,333-byte continuation. It embeds this maintained metadata
predicate, replaces the two invalid schema assumptions and saves a source-only
derivation receipt. It preserves the original cgroup and full-seven equality
gates: after a reboot they must still reject until Root separately implements
and verifies the explicit current-incarnation bridge. It does not authorize a
new cgroup, migration, limit increase, executable substitution or test launch.
