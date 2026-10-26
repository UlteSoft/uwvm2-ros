# Portable algorithm fixture compilation evidence — 2026-10-26

This directory publishes preserved Linux compilation evidence for three **preview**
Wasm fixtures. It does not qualify a portable host implementation, a robot platform,
or an official simulator connection. Product correctness, permission-denial tests,
VM execution, native ROS, official simulation, unchanged-byte platform reuse, QEMU
and native performance remain **NOT_RUN** for these published fixtures.

The SDK source and precompiled files are listed by exact SHA256 in
[SDK-PUBLICATION.json](SDK-PUBLICATION.json). Their ABI and adapter documents are
candidate contracts; metadata does not implement an import or grant permission.
The corresponding SDK base commit is `64c2e9e0187088ee572e0b127e42568c97854ead`.

| Fixture | Compiler receipt | Whole-file SHA256 | Bytes |
| --- | --- | --- | ---: |
| joint-step v0.1.0 | r652 | `c0d886e864243938999e4adf4c0f4ce6fb3c2c001723b855cd8b203bfafe2043` | 4449 |
| planar-goal v0.3.0 | r658 | `54884d1513c2e5256450128627a5cc6f4130ca9ed533322a5035f77f81b59ff9` | 3797 |
| sensor-digest v0.3.0 | r658 | `7061f671b91860cca2867d52bc38bca4c12234426598951451b481cf31c69100` | 20095 |

## Recorded results and retained failures

- **r650: FAIL_OR_BLOCKED.** Configure/build/inspection commands exited zero, but
  the enclosing signature assertion failed because a zero-parameter Lua table was
  serialized as a JSON object. Its original result and stderr are preserved.
- **r652: compiler and recorded structure pass.** The inspector explicitly encoded
  empty arrays and checked the original joint Wasm SHA. C/header/project semantics
  and the 4449-byte output were unchanged. This is not runtime acceptance.
- **r656: FAIL_OR_BLOCKED.** The sensor compiler exited 255 because the thin packet
  omitted the physical WASI sysroot include tree (`inttypes.h` was unavailable).
  The successful navigation output does not turn that batch into a pass.
- **r658: compiler and recorded structure pass.** The missing existing include-tree
  assets were restored and hash-pinned. Configure/build/inspection exited zero for
  planar-goal, sensor-digest and the existing WASI C sum_arguments binary. That
  additional regression is compilation evidence only. The joint artifact was
  checked unchanged and was not rebuilt in this batch.

## Evidence layout and identity

Each revision contains an exact sealed source packet, original compiler/control
archives, transport outputs and available independent verification records.
`attempt1` contains that revision's interruption/lock proof; `compile-attempt1`
contains the actual compilation. These proof phases are resource-control evidence,
not ROS or simulator tests. Configured project archives retain emitted Wasm bytes.
`MANIFEST.json` records the published files, their original VM-relative locations,
and individual members of preserved raw/project archives. Existing raw archives
are copied byte for byte. New source-packet tarballs normalize only archive metadata;
all source bytes and the original packet-manifest remain unchanged.

Successful receipts record actual actor cgroups and the shared ancestor
`/sys/fs/cgroup/uwvmros.slice`: `memory.max=68719476736`, `memory.swap.max=0`,
plus the held global `/home/macromodel/Documents/uwvm2-ros-test/control/test.lock`.
They belong to boot `815b614a-e0a5-4600-9d7f-139b323d8190`. They are historical
execution evidence, not an admission for another process, boot or new run.

The prior coordinator reviews are retained unchanged. Their `shipping_approved:
false` fields mean that runtime/product shipping was not accepted. This publication
is limited to compiler-qualified developer fixtures and their evidence; it does
not promote any unexecuted acceptance dimension. Frozen source metadata can retain
an earlier availability snapshot; later artifact receipts record actual compilation.

The global portable index was staged as inert metadata in the compiler packets.
It is preserved there for exact provenance, but is not consumed by the reviewed
compiler/Xmake path and is excluded from the SDK publication with the uncompiled
v0.2 preview and global capability registry. The 28 selected SDK paths contain the
versioned example/ABI/contract closure plus the three artifacts and receipts.

## Reproduction prerequisites

Read the full sealed packet and its fixed paths before using it. These historical
scripts bind a particular boot, admission, image, tool identities and lock proof;
they must not be blindly replayed on a workstation or a different host. Obtain a
new owner-verified Linux admission, retain the shared 64 GiB/zero-swap bound and
lifetime lock, stage the complete thin source folder, and preserve new failures
under a new revision. Never weaken those guards to reproduce a result.

The sealed asset and Xmake program manifests identify external immutable inputs;
the existing SDK Git/LFS files and matched compiler bundle provide those bytes.
The source archives do not duplicate hundreds of megabytes of immutable tools.
Original compiler argv, stdout, stderr, cgroup/lock observations and output hashes
are retained in the raw archives. A new build is a new observation and cannot
replace these records. No compiler, Wasm, product test or repository code was
executed on macOS to assemble this publication.
