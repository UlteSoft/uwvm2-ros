# Complete volatile-object publication

`artifact.publish` implements a bounded complete-object transaction in the
existing scene output registry. It uses the same object map, monotonic mutation
stream, last exact request, receipt, ownership checks and mutex as
`artifact.begin`, `artifact.append`, `artifact.commit`, `artifact.abort` and
`artifact.remove`. It does not create a second cache or reset the sequence.
The ordinary staged API remains available for incremental and larger outputs.

The registry and authority implementation is accompanied by native security
checks and [actual VM/CLI/FP/MTE qualification](ros-buffer-portability.md).
Those results identify their frozen binary and source manifest; a core receipt
alone does not establish them. The earlier complete-publication prototype remains historical
design evidence and is not substituted for this implementation.

## Wire and exact authority

The seven-argument output carrier carries little-endian
`[key:u64, complete_payload_size:u64, payload]`. The declared size must exactly
equal the owned copied request extent minus 16. The complete payload cannot
exceed the binding's existing transfer bound. A successful carrier delivers the
existing 72-byte receipt. Its operation code is 8, state is 2 and publication
flag is 1 when the effect succeeded; durable flag 2 remains clear. Accepted,
total and retained payload bytes equal the complete payload length. An empty
publication still consumes an object slot. A failed effect is reported inside
the receipt, independently of whether its bytes were successfully delivered.

Only the `cache` purpose with the `volatile-output` binding supports this
operation. The capability must explicitly grant **all four** actions
`artifact.publish`, `artifact.begin`, `artifact.append` and `artifact.commit`,
and each interface must be exposed as allowed. Empty payloads require the same
set. Actions from another capability or alias cannot complete the grant.
Read, remove and receipt queries keep their independent authority. No path,
descriptor, persistent file, model execution or actuator permission is implied.

`authority::admit_publish` checks the entire action/exposure mask atomically
under one lease. Its request-size parameter includes the 16-byte framing. The
generic output-enum and string admission paths also route publication through
this compound policy; they retain their request-plus-72-byte-receipt parameter
convention. A zero-cost, uncharged authority-only revalidation is supported,
but cannot omit any of the four actions. Invalid enum values, undersized
framing and arithmetic overflow fail before a debit or effect.

## Optional limits and fixed reservations

Publication charges three logical call units and `payload_size + 280` byte
units in one atomic admission. This is the cost of the existing nonempty
begin/one-append/commit path, including its request and receipt bytes. An empty
publication conservatively pays those same three units, exceeding the old
empty path's two calls and 184 bytes. Admission counters count admitted carrier
attempts; the optional call allowance counts the fixed work units. These costs
are trusted operation constants, not guest arguments.

Absent optional rates allow full execution inside the explicit grant. An
explicit zero allowance allows no admissions. Insufficient allowance never
partially charges a compound operation. Exact replay and admitted failures pay
again; failure does not replenish rate budgets. Preflight and short-receipt
queries perform no admission debit. Revocation and owner-generation checks
remain mandatory regardless of whether rates are configured.

`outputs::plan` additionally has finite `publication_scratch_bytes` and
`publication_scratch_calls`, both zero by default. They are independent capacity
reservations, not rate limiting, and do not change the existing staged
operations. Each admitted publication carrier must acquire a move-only
`outputs::registry::publication_scratch` before request allocation or copying.
For request extent `N`, its scratch charge is:

```
512 + (N > 512 ? N : 0)
```

The fixed 512-byte inline buffer exists even when the request uses separate
heap storage. The finite call count bounds per-call metadata and allocator
overhead as well. Object preparation uses the existing retained-payload and
object-count reservations, including the grant's resource-count limit. The
shared exact-replay buffer retains `transfer + 24` request bytes plus the
72-byte receipt; the larger old append framing is not reduced to publication's
16-byte header.

The broker owns the registry for the entire scratch lifetime. Host code must
keep its owning broker `shared_ptr` alive until the scratch token is reset or
destroyed. Tokens cannot be reused across registry, alias or request extent;
moving a token transfers its charge. Revocation does not prevent cleanup, and
forked cleanup does not acquire an inherited registry mutex.

## Preparation, cancellation and commit

The broker performs exact uncharged authority preflight, reserves scratch,
then makes the complete checked owned request copy. No bytes are parsed until
that copy succeeds. Short-receipt checks use `publication_size`, which validates
publication's 16-byte framing without reserving scratch; append's additional
eight framing bytes cannot admit an oversized publication.

After final charged admission, `registry::publish` prepares the transaction
under the existing registry mutex. It validates the shared key and exact replay
identity, reserves retained bytes and a pending object slot, allocates/copies
the payload, and allocates a private map node. Nothing is published and no key
or receipt is consumed during preparation. Existing begin operations count
pending slots, so releasing the mutex cannot overbook capacity.

The provider then releases its mutex and calls the broker's trusted
`final_check`. That callback observes the real runtime cancellation predicate
and revalidates authority using a second lease while the original recursive
authority lease stays held. It must not release and reacquire the original
lease, which could introduce a blocking interval after cancellation sampling.
Reentrant revocation or capability closure is checked again here. Arbitrary
cancellation predicates never run under the provider mutex.

Commit uses **one `try_lock` attempt**, not a blocking mutex acquisition. If
the registry is busy, it returns carrier `again`, releases the unpublished
allocation and reservation, and consumes no key. Once the lock is acquired,
the provider rechecks the live binding, token, exact stream position and any
continuously linked native `stop_token`. The prepared node moves into the
existing object map, and the bounded last request/receipt is replaced without
further allocation. Other operations observe object publication and receipt
replacement together.

The real cancellation decision point is the final predicate observation before
the nonblocking commit attempt; a linked stop token adds an atomic check under
the acquired mutex. A temporary stop source populated by an earlier one-time
sample is not a continuously linked runtime token. Cancellation after the
decision point cannot be claimed to withdraw an accepted publication. This
cache operation is not an actuator stop or a physical watchdog.

Allocation failure, cancellation, revalidation failure and commit-lock
contention release unpublished storage before returning its capacity. They do
not replace the previous receipt or consume a key. An ordinary object-capacity
refusal produces a retained failure receipt when its key can still commit;
scratch-capacity refusal is a pre-effect carrier refusal. If another mutation
advances the shared stream while preparation is unlocked, commit returns
conflict and preserves that newer receipt. Exact replay of an accepted request
returns the same receipt and object without allocating another object slot.
Failed guest delivery does not erase an accepted effect or receipt.

## Native security qualification

`test/0084.ros-runtime/artifact_publish.cpp` exercises the real production
authority, output registry and current `utils` allocator. Its test groups cover:

- Each missing dependency action/exposure, owner generation and handle, generic
  admission bypass attempts, invalid enums and size arithmetic.
- Explicit zero and insufficient rates, unchanged budgets after refusal,
  preflight without charging and competing atomic three-unit admissions.
- Default-zero scratch, exact framing bounds, finite byte/call reservations,
  token moves, wrong-registry/alias tokens and legacy operation availability.
- Interleaved old/new mutations, maximum-size old append followed by complete
  publication, exact replay, byte conflicts, independent reads and removal,
  empty objects, capacity failure receipts and ignored-result recovery.
- Pending-capacity protection against reentrant old mutations, rollback when
  those mutations advance the sequence, and reentrant revocation/closure.
- Failure of the actual strict allocator's payload and map-node allocations,
  previous receipt preservation, and zero allocations after the final callback.
- Real cancellation/linked stop, callback exceptions, a final mutex held by
  another thread, nonblocking refusal, and fork rejection while that mutex is
  held in the parent.

The runner captures the compiler-discovered transitive repository/vendor
dependency closure and compiles every variant from those retained snapshots.
Malloc-family linker wrappers target the current strict allocator; overriding
`operator new` would not exercise that allocator after the workspace migration.
Clang ASan/UBSan, GCC and TSan receipts retain all commands and before/after
source hashes. Run every compiler and executable inside the existing shared
64 GiB, zero-swap cgroup:

`artifact-publish-core-final/results.json` records nine successful commands:
three compiler dependency-closure captures, and Clang ASan/UBSan, GCC 13 and
Clang TSan compilation/execution pairs. All eight security groups passed in
each executable. All 390 retained repository/vendor source files and their
workspace inputs remained unchanged. The cgroup inode was 12409; its existing
`memory.events.max` counter was 331 before and after these runs, while `oom`
and `oom_kill` remained zero. The earlier core-first
receipt preserves a GCC test-fixture formatting failure and is not accepted.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 10-11 \
  /usr/bin/python3 tools/ros_scene/artifact_publish_checks.py \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/artifact-publish-core-final \
  --thread
```

Use a fresh output directory for each qualification. `--source-root` can replay
a retained `source/` tree; when running a copied runner, also give the actual
repository through `--repository-root`. Core test success does not replace
checked guest-memory, FP, module, debugger, CLI or actual VM timing results.

## Actual serialization performance

The [upstream XML output qualification](ros-upstream-output-portability.md) now
compares real staged and complete-publication guests on the same frozen VM,
with an identical native machine-code reference. For fourteen XML outputs,
4 KiB transfer/readback reduces the exercised artifact-call topology from
66 to 40 calls per cycle (plus one capability open per process). Three paired
rounds of 31 batches with 1000 cycles each measured 60.630 microseconds for
staged output and 52.892 for publication, versus 18.671 native: a 12.76%
reduction, still 2.833 times native. At 64-byte readback, the paired times were
98.937, 74.294 and 20.043 microseconds: 24.91% lower than staged output, still
3.707 times native. The complete publication retains a 4096-byte transfer
allowance and explicit scratch even in that readback-boundary experiment.

These are a serializer plus bounded in-memory transport slice, not a complete
ROS application or disk baseline. They replace neither authority checks nor
the requirement for near-native performance; that target remains unmet.

The initial production timing cohort used VM `-O3 -gline-tables-only`; those
ratios are historical and do not satisfy the later explicit full `-O3 -g`
performance-build requirement. Native/Wasm benchmark invocations already used
full `-g`. The fresh full-debug VM is now qualified separately: publication-full-debug-paired-batched and publication-full-debug-paired-boundary measure 2.9132x and 3.6416x native, respectively. The full-debug VTune/source/instruction receipts are documented in [the output portability report](ros-upstream-output-portability.md#full-debug-performance-cohort). The near-native target remains unmet.
