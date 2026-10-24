# Noetic partial SDK continuation

The source implementation is in `noetic_sdk_acquisition_r5.py`,
`noetic_partial_resume.py`, `noetic_resume_root_owner.py` and
`noetic_resume_stage.py`. These files do not establish a replay receipt, an SDK
completion result or permission to reuse an unverified partial tree. Root owns
the actual guarded Linux staging, regressions, acquisition and final review.

The retained r4 inner report has content SHA256
`f37195059b17d68573edd9e1b072dc32c933dc3a1a9fc7fed8ec4a9f9f738bc3`
and 3,615,127 bytes. Its outer report has SHA256
`49272f388877f7fb118c9adf0f2ef32dd2e6fbee6654e5e86eafa051cbd01b0b`
and 57,193 bytes. Both remain failed historical acquisition evidence.
There are 255 committed package records (ordinals 0 through 254). The control
and data-tar commands for ordinal 255 completed, but extraction did not commit;
its SDK entries and ledger suffix therefore require independent reconciliation.

The intended r5 sequence is:

1. Obtain fresh GPG-verified Release and Packages metadata with the exact
   reviewed repository/key/expiry selection, and an empty-status APT solver
   closure. Do this before making any retained byte eligible.
2. Rehash retained DEBs against the fresh signed size/SHA256 identities and
   independently check their actual package/version/architecture control
   fields. Equal complete DEB content can share a control-field observation;
   every physical retained file still needs its own current pin.
3. Independently audit the first 255 decoded package streams against the exact
   retained ledger, then audit the recorded ordinal-255 ledger prefix against
   its authenticated tar. Decode at most one transient tar at a time; do not
   construct or copy a second SDK. Preserve old DEBs, reports, logs and ledger.
4. Replay tar metadata into a bounded in-memory virtual file/directory/symlink
   state. Derive physical-relative paths using the same confined alias rules
   as `SafeTree`, rather than trusting ledger path strings or resolving them
   against a potentially later alias state. Verify every file body, mode and
   symlink text against signed DEB tar content. Reject extra/missing physical
   SDK entries and special nodes. Deferred hardlinks are not physical SDK
   files before `SafeTree.finish()`; reconstruct their original order.
5. Save the committed ordinal-254 checkpoint separately from the authenticated
   ordinal-255 partial suffix. A fresh r5 ledger must contain only the committed
   prefix initially. Restore `SafeTree.entries`, `bytes`, `sequence` and
   deferred hardlinks from that committed checkpoint, not the partial suffix.
   Otherwise re-extracting ordinal 255 would duplicate pending hardlinks.
6. Hold source/destination parent directory FDs and the admitted SDK directory
   FD, prove same filesystem and exact identities, fsync a durable transfer
   intent, and atomically rename only `r4/sdk` to fresh `r5/sdk`. Confirm the
   held directory identity at the destination, fsync both parents, and publish
   a durable confirmed-transfer receipt. Do not move the entire old r4 output.
7. Re-extract ordinals 255 through 512 with the unchanged `SafeTree` extraction
   rules. Already authenticated partial ordinal-255 files can be identical
   overlays; no partial pending-hardlink state is carried twice. Write a new
   complete r5 ledger, finish deferred hardlinks, and compare full SDK catalogs
   before and after finalization before marking an extracted sysroot prepared.

The original 8 GiB backing-filesystem reserve and exact current 64 GiB/zero-swap
cgroup remain enforced. Old device/boot/cgroup observations are historical;
current file observations use separate newly opened pins. A complete extracted
SDK still does not establish native ROS execution or Runtime compatibility.

The metadata stager is also the control maker. Its CLI is:

```text
/usr/bin/python3.14 -B /absolute/reviewed/noetic_resume_stage.py \
  --payload /absolute/reviewed/source-package.json \
  --payload-sha256 ACTUAL_REVIEWED_PAYLOAD_SHA256
```

Run it only in the current exact Root-owned 64 GiB/zero-swap leaf. The source
package is a closed object with `format_version: 1`, kind
`noetic-r5-reviewed-partial-sdk-continuation-source-package`, and four `files`
roles: `producer`, `resume`, `restart` and `owner`. Each role contains exactly
`name`, `bytes`, `sha256` and canonical `base64`. Destination names are
`acquire-sdk.py`, `noetic_partial_resume.py`, `requalify_after_restart.py` and
`launcher.py`. Root must review the complete immutable payload and stager bytes
before invocation. The stager starts no process or network operation.

The stager creates only the fresh r5 controls directory and an external stage
receipt. The receipt publishes `unexecuted_owner_argv` using the actual newly
opened control SHA256. Invoking that command is a separate Root action; source
staging does not launch acquisition. The documentation-only JSON template
intentionally contains null future file pins and is rejected by the owner.

The original reviewed spec retains every repository URL, key fingerprint,
InRelease digest, expiry rule, package seed, selected host tool SHA and finite
extraction limit. The only spec deltas are the explicit current cgroup and two
additional complete input roots: fresh r5 controls and retained r4 DEB files.
The original r4 control root remains inventoried. The logical host resolver
selection is unchanged, while the Root-observed current resolver physical pin
and literal `/etc/resolv.conf` alias are admitted separately. They do not imply
continuous identity across the reboot.

The owner allows at most 512 MiB of its own resident memory and 1 GiB for its
owned producer tree. The producer keeps the original 512 MiB serial package
tool limit, 600-second command limit, 12-hour acquisition deadline and 8 GiB
backing-filesystem reserve. DEB decoder threads remain explicitly capped at one.
The original SafeTree extraction methods are compared byte-for-byte with the
genuine retained r4 source both during staging and owner admission.

During replay, all complete retained DEBs must match freshly verified signed
Packages and the fresh empty-status solver closure. Every physical file is
streamed independently. Identical complete DEB bytes can share only their new
actual `dpkg-deb --field` observation. Prefix packages are freshly decoded and
read-only replayed; their r5 rows explicitly say their original extraction was
independently reconciled rather than re-executed. The read-only tar retirement
proof has its own kind and never claims `SafeTree.extract()` returned. Original
checked-extract proofs remain separate historical provenance. Fresh package
ordinals 255 through 512 use the original checked-extract retirement proof.

Each retired audit tar is a fresh owned temporary, bounded to one package and
removed only after a persistent honest replay proof is retained. The decoded
byte quota includes the ordinal-255 read-only audit and its later fresh decode.
No second full SDK is written. Only the committed ledger byte prefix is copied
into the new owned ledger; the original ledger is reopened and preserved.

Crash behavior is fail-closed. Before rename, no SDK name has changed. After
rename, the durable intent identifies the original held inode and destination;
the confirmed receipt additionally requires the original name to be absent,
the destination to name the retained inode, and both parent directories to be
fsynced. If a crash leaves intent without confirmation, or an existing r5
output is found, the stager/owner refuses an automatic retry. Root must inspect
the actual held-name state and author a separately reviewed recovery admission.
This implementation never guesses whether to rename back, copy, replace or
delete a tree. A transferred tree remains unqualified until all remaining
extractions, hardlinks, full final catalogs and input/product preservation pass.

An extracted sysroot does not establish native ROS execution, a Runtime or
Gateway result, hardware behavior or performance. No test, package command,
compiler, SDK acquisition or SSH action was run while authoring these files.
