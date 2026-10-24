# Noetic r6: a fresh extraction seeded by authenticated CoW bytes

This directory contains source for an acquisition attempt, not a completed SDK
or a ROS compatibility result. The original r5 inner report is still `RUNNING`;
its outer owner is `FAIL`. The 329 package rows, partial ledger, and partial SDK
are retained failed-run provenance. They supply neither a completed prefix nor
permission to continue an old cgroup/process incarnation.

## Authority and the two extraction passes

Root explicitly selects a SHA-bound seven-field **current** cgroup context.
Before any project helper executes, staging, owner and producer check the
original task leaf spelling, current boot ID, physical leaf device/inode, exact
membership, hard 64 GiB memory limit, zero swap and CPUs `0-31`. No boot is
inferred from historical reports or a coincidentally equal Btrfs device number.
The current facts template records facts actually observed by Root after boot3;
Root must review them and supply the template's exact SHA. A changed fact causes
rejection. Editing the template does not make the stated fact true.

The original Source35 manifest, original r4 repository/key/seed/limit selection,
and r4 `SafeTree` implementation remain pinned. The only spec changes are the
explicit current cgroup, new r6 source root, r4/r5 DEB download roots, `/usr/sbin`
and the pinned `fstrim` executable. The original r1 DEB root was already in the
seed. The new output is disjoint from every immutable input root.

The producer first obtains fresh independently GPG-verified Releases and
SHA256-verified Packages and solves the exact 513-package closure against an
empty APT status file. Every old DEB is opened and fully hashed against that
fresh authority. Actual `dpkg-deb --field` results must also match package,
version and architecture. Any missing selected DEB is freshly downloaded with
isolated APT configuration, retained as an owned product, and independently
verified. Filename similarity, historical package counts and old ledger rows
never substitute for signed byte equality.

All 513 DEBs are freshly decoded in a read-only first pass. The signed tar
members produce a complete virtual physical tree and a new expected payload
ledger. The virtual model retains the independently reviewed r5 class literally;
its ledger sink writes newly generated rows, not comparisons to an old ledger.
It implements the maintained parent aliases, duplicate-file rules, rewritten
symlinks and deferred hardlink order. Special nodes, conflicting overlays,
unsafe paths and reserved temporary names are refused. A complete signed set
that cannot satisfy these extraction rules remains a failure.

Only then is the old partial SDK walked through held directory FDs, without
following physical aliases or crossing filesystems. Every ordinary old file
must equal the complete authenticated model's SHA256, size and mode. Symlinks
must equal the authenticated rewritten target. Unknown normal files are refused.
Recognized `.uwvm-acquire-*.pending` regular temporaries are fully read and
recorded but excluded **only from the fresh copy**. Deferred hardlink destination
names are excluded so `SafeTree.finish()` can recreate their relationships.
Nothing is removed from, renamed within, or written to the old SDK.

The source tree is inventoried read-only before copying; the same inventory is
rechecked during copying and at finalization. Ordinary files are cloned with
Linux `FICLONE`: same filesystem, distinct new inode, `nlink == 1`, complete
source and clone reads, and stable descriptor/name identities. No ordinary-data
copy fallback is permitted. Directories and relative symlinks receive separate
new identities. Future writes to a new clone must not affect old input bytes.
The Linux ioctl encoding is explicitly restricted to native x86_64, derived
from `_IOW(0x94, 9, int)` and documented in `owned_storage.py`.

A **new** `SafeTree` starts with counters and sequence zero, empty pending links
and a newly created empty ledger. Its 10,509-byte class body is byte-identical to
r5/r4. All 513 packages are then freshly decoded and extracted through it. No
historical row is skipped. Existing CoW files still pass its ordinary identical
content/mode overlay checks. Before success, the entire new ledger must equal
the independent expected ledger byte-for-byte, counters must agree, and a full
physical audit must match all signed names, modes, bytes, relative links and
complete hardlink classes. Extra, missing, split, merged or externally linked
files cause failure.

## Backing storage and cleanup

The sparse image's logical free space does not establish host backing space.
Active work retains the original 8 GiB floor on every visible output ancestor.
Once the signed tar model is known, each fresh package also reserves its actual
decoded tar size, largest temporary member and 64 MiB metadata headroom. Missing
compressed DEBs reserve their signed size before download.

Root's closed storage selection identifies only the already owned Btrfs UUID,
`loop29`, mount options, current mount-root device/inode, existing sparse image
path/inode/logical size, actual UUID-to-loop sysfs relation, and current pinned
`/usr/sbin/fstrim` file. Btrfs's root `st_dev` and mountinfo superblock device are
recorded separately; they need not be numerically equal. The image's allocation
and times change during owned writes/trim and are not claimed immutable. No
256 GiB image hash is computed.

Before each trim, all these relationships and the actual tool full7 are checked.
The only trim argv is `[selected_fstrim, '--verbose', selected_mountpoint]`.
There is no whole-host trim, root-filesystem trim, arbitrary mount, file cleanup
or deletion of old inputs. Fresh owned decoded tars are retired only after a
complete model audit or a successfully returned checked extraction; their
actual decoder argv/log/digest and disposition remain in durable receipts.
Trim barriers reclaim unused owned blocks after retirement and before further
active admissions. Compressed DEB reuse also uses independent-inode CoW rather
than writing duplicate compressed bytes.

The producer and outer owner keep cgroup, RSS and finite deadline checks during
finalization. A low disk floor cannot abort the direct same-birth coordinator's
600-second cooperative TERM grace or read-only preservation. Only owned pidfds
are signalled, followed by the maintained child-first fallback; no process group
or numeric PID kill is used. No new payload/tool command starts in finalization.
Whole input-root preservation computes the same maintained catalog rows and
SHA256 directly, without creating large `host-after` catalogs. This policy is
used for successful and failed finalization alike. Small final reports/logs and
already required actual receipt metadata remain durable writes. Every failure
retains its actual status; cleanup cannot turn a failed command into success.

## Root staging and actual execution

Do not run these files on the desktop or outside the selected Linux cgroup.
The package stager itself is metadata-only. Its source and both selected JSON
inputs must first be independently reviewed and SHA-bound by Root's existing
64 GiB execution wrapper. `reviewed-source-package.json` contains six complete
source roles: producer, rebuild, virtual model, owned storage, restart helper
and Root owner. The stager's own bytes are listed separately in `source-ready`.

After Root has staged these exact files, its metadata command is:

```text
/usr/bin/python3.14 -B /ROOT_REVIEWED_STAGE_PATH/stage.py \
  --request /ROOT_SELECTED_REQUEST_PATH/current-facts.json \
  --request-sha256 ROOT_ACTUAL_REVIEWED_REQUEST_SHA256 \
  --payload /ROOT_SELECTED_PAYLOAD_PATH/reviewed-source-package.json \
  --payload-sha256 ROOT_ACTUAL_REVIEWED_PACKAGE_SHA256
```

The request and package paths are explicit inputs; these placeholders are not
shell variables, network locations or permission defaults. The selected request
must contain the exact current facts and the actual source package SHA is in
`source-ready.json`. No future physical identity is filled into a template.

Staging writes the fresh fixed r6 code directory and actual source/control pins,
then emits `unexecuted_owner_argv` in its receipt. Root must invoke **that actual
argv** through the current cgroup wrapper. Copying an old owner command or
inventing the future `root-control.json` SHA is not supported. Staging starts no
actor, network request, extraction, clone, trim, compiler or Runtime.

The owner's terminal success is
`PASS_AUTHENTICATED_FRESH_ALL513_INDEPENDENT_COW_CONTENT_SYSROOT_ONLY`.
The inner status is `PREPARED_AUTHENTICATED_EXTRACTED_SYSROOT`. Both mean only an
authenticated extracted sysroot. They do not mean a configured OS, maintainer
scripts, executed compiler/target SDK, ROS compatibility, simulator result, or
any increase of Guest authority.

## Small independent regressions

`test_fresh_rebuild.py` is eleven meaningful small test cases. It verifies real
CoW independence, exclusion of only new temporary names, refusal of unsigned or
changed old files, exact expected ledger equality, full physical tree mismatch
refusals, hardlink topology and unsupported-CoW refusal. It also compares the
read-only preservation digest to the maintained catalog's actual rows. These
cases do not download packages, compile, extract the full SDK or run target
programs. Source presence/AST parsing is not an executed test result.

Root supplies an exact current context document and a five-role full7 helper
selection (`producer`, `resume`, `model`, `storage`, `restart`), both SHA-bound,
and a fresh scratch directory under the owned image. The selection is produced
from actual staged source files, not guessed future inode/time facts. The test
entry point checks actual current64GiB/swap0 membership before loading project
helpers and verifies helper/control pins again afterward. Root must run it with
a bounded outer owner and capture its actual terminal result. The completed
small CoW checks from another lane remain separate evidence; they do not claim
these eleven cases or the full Noetic6 acquisition ran.
