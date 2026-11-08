# Dynamically sized debugger cohorts

The production pause domain, private capture registries, native worker registry,
and complete-cohort query storage grow with their actual live inputs. The
default debugger does not select a fixed participant quota. A host that
explicitly constructs a sized `cooperative_pause_domain` still selects that
quota. This does not grant a Wasm module any debugger or management authority.

## Component checks

`../0003.utils/thread/cooperative_pause_dynamic.cc` verifies 4,097 enrolled
leases, concurrent identity reads during further growth, 257 actual parked
native threads, an explicit one-participant quota, old-ticket rejection, and
repeated resume/admission races. It uses real domain participants and tickets.
It does not stand in for actual JIT capture or code-publication authorization.

`debug_metadata_unbounded.cc` checks source paths, source contents, nested
Source Map JSON, inline/source scope paths, and an activation ledger above the
former 256 MiB default. Explicit test-selected quotas continue to reject
transactionally.

## Actual runtime checks

Build `debug_wasm_blocked_wait_cohort_runtime.cc` with the same source,
definitions, ABI and libraries as the actual full-JIT runtime, and add
`-DUWVM_TEST_COHORT_SIZE=257`. Link the freshly built runtime and host API
objects, replacing only the CLI main object with this native witness.
The default test build retains two workers.

Compile and validate each `fixtures/debug_blocked_wait_cohort_A_C.wat`,
where A and C are independently 32 or 64, using a Wasm tool with threads,
memory64 and GC support. Run:

```text
fixture fixture-A-C.wasm observe A C
fixture fixture-A-C.wasm observe A C owned
fixture fixture-32-32.wasm observe 32 32 shutdown
fixture fixture-32-32.wasm observe 32 32 shutdown-immediate
```

The first form creates actual external native threads. The `owned` form uses
the production registered worker factory and verifies physical native joins.
Both forms execute genuine Wasm frames, perform two complete pauses, and check
every participant's typed locals, a live GC structure, dynamic operand prefix
and caller frame. Missing members, duplicates, foreign control blocks and old
pause captures are rejected. After a memory mutation, actual Wasm notification
must wake all linked waiters without re-running or re-comparing the wait.

The shutdown forms verify that a managed stop wakes and retires all live
waiters without inventing a successful Wasm wait result. All fixtures must run
inside the configured Linux memory/swap cgroup. Native worker counts must fit
the independently configured OS PID budget.

The observation profile is intentional: these checks do not claim executable
checkpoint restoration of an in-flight atomic wait. Native retirement/join
transition tests remain in `debug_checkpoint_native_cohort_retirement_runtime.cc`.
