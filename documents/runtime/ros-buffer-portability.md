# Buffer and complete-publication VM qualification

The production buffer contract and `artifact.publish` were executed by the
frozen VM `vm-buffer-publication/uwvm2-ros`, SHA256
`606a8f2e1faf54c05af677e09fceecdcbab09d8335c0d08f1c1fe7830e7c15c7`.
Its full build receipt pins 3434 source files in
`provider-source-buffer-publication`. The runtime portion retains the earlier
qualified baseline; this is not a qualification of unrelated concurrent
workspace migrations. All receipts below live under
`../uwvm2-ros-sdk/.tmp/ros-scene-security/`. Every compiler, VM and debugger
process ran inside the existing 64 GiB cgroup with zero swap.

## Checked guest carriers and configuration

`buffer-vm-workspace-ready/results.json` records 54 passing VM/CLI cases.
`run_scene_buffers.py` executes interpreter and LLVM guests with memory32 and
memory64, and memory index zero and one. Each successful guest contains 333
named assertions covering all five buffer imports, exact profile/layout
admission, contiguous initialized writes, identical retries, mismatching
retries, SHA256 sealing, sealed-write refusal, exact describe records, bounded
readback, empty objects, cross-alias rejection, closed handles, default denial,
independent grant limits and explicit rate limiting. CLI checks reject
missing/duplicate/out-of-order plans, malformed digests, unknown masks and
insufficient aggregate reservations.

A 600-byte write needs its complete 624-byte encoded carrier reservation.
The actual host accepts a 624-byte workspace and rejects 623 or zero before
writing data. The refused object's unwritten range remains unreadable. Inline
writes remain possible with zero heap workspace; a zero call budget rejects
large requests before copying. Successful large writes are replayed, sealed
with the expected SHA256, and read back byte for byte.

`publication-vm-workspace-ready/results.json` records 75 passing cases from
`run_scene_publication.py`. The successful guest has 183 assertions per memory
layout. Tests exercise the existing shared staged/publication key stream,
maximum-sized append followed by publication, exact last-request replay,
conflicting bytes, capacity-failure receipts, empty publication, independent
reads/removal, and each missing action or exposed dependency. Separate cases
prove default-zero and undersized scratch, atomic three-unit admission without
partial debit, exact byte budgets, no refill on capability reopening, and
closed CLI configuration. See [the publication contract](ros-artifact-publication.md)
for cancellation and retained-receipt rules.

These five buffer imports do not expose native `pin()`. Actual seal results
identify the independently admitted capability and resource, but this VM suite
does not claim to test a downstream model/device consumer holding a pin.
Retained pin lifetime and revocation are separate provider-core obligations.

## Floating-point and memory protection

`data-boundaries-actual-second/results.json` records 184 passing cases from
`run_scene_data_boundaries.py`. Every new import receives distinct request,
output and length tag faults under real software MTE. Default violations must
terminate with the actual protection diagnostic and signal. Trusted diagnostic
mode separately permits different pointer encodings so physical overlap must
still be rejected without overwriting canaries or consuming effects. Both
engines, address widths and memory indices are covered.

Each FP success guest has 20 host-return sites with twelve dynamic arithmetic
checks per site, including subnormals, rounding, infinity and NaN. Short-output
and exact-replay returns are included. New buffer and publication wrappers
retain the full FP boundary. This is software-MTE qualification, not hardware
MTE or a native robotics safety certificate. The first boundary receipt remains
incomplete: its no-grant fixture initially omitted publication's three extra
exposures, correctly receiving exposure denial before bad-handle rejection.
The second receipt fixes that fixture and preserves the earlier evidence.

## Debugger and module ownership

`buffer-debug-contract-final/results.json` records eight successful formatter,
Clang ASan/UBSan, GCC, strict DAP and editor-controller checks.
`buffer-dap-workspace-ready/results.json` runs the actual VM, debug broker and
DAP under full, read-only and limited debugger permissions. Eleven aliases
exercise pagination, repeated reads, cached accounting, independent inspection
permission and stale-stop rejection. Rows expose only binding totals; no
schema, digest, payload, native address or buffer identity is exported.
`reserved_bytes`, `members`, `open_handles`, `lease_count` and `workspace_bytes`
remain exact decimal strings through DAP and the editor. Repeated inspection
leaves resource and authority counters unchanged.

The allocator migration exposed named-module attachment defects in the scene
model and host exporters. Their global module fragments now include the four
shared allocator/container/callback headers and `<exception>` before declaring
the named module. `provider-source-buffer-publication-module-fixed` is a
separately retained snapshot differing from the VM source in only those two
module exporters. Its full manifest remains unchanged during testing.
`modules-buffer-publication-llvm-fixed/results.json` and
`modules-buffer-publication-interpreter-fixed/results.json` each record 22
successful commands: real model/storage/debug modules, mixed textual/named hook
identity, duplicate-owner rejection, isolated host exports and concrete FP
policy assertions. The original VM snapshot's module failures remain recorded;
these focused successes do not claim a full frontend/runtime module-graph build.

All above completed runtime receipts observed the same cgroup epoch, with its
existing `memory.events.max` counter unchanged at 331 and `oom`/`oom_kill` zero.
