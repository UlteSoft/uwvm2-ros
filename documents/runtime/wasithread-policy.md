# Independent WASI Threads authority (I1T)

`wasi.thread-spawn` is governed by `wasithread` (short option family `I1T`),
independently of WASI Preview 1. The guest import ABI remains unchanged. There is
no WASIp1 `thread-spawn` interface or permission alias.

## Authority and scope

All scopes start disabled with a hidden interface. An otherwise unassigned
module uses Global. Creating a Single scope assigns that exact module; adding a
module to a Group replaces its implicit Global authority. Single and Group do
not inherit Global grants. A duplicate explicit membership, including a repeated
member of the same Group, is rejected during CLI configuration.

`enable` selects the namespace. `interface thread-spawn hidden` rejects the
import during loading; `deny` exposes its signature but returns a negative spawn
result; `allow` permits admission. Enabling a scope does not change its interface
setting. No numerical quota is imposed by an I1T grant unless explicitly set.

Global count limits are process-wide aggregate caps, including Single and Group
threads. A selected Single/Group cap is checked in addition to those aggregate
caps. Group members share one set of counters. Changing scopes therefore cannot
bypass an explicitly configured process cap. Import authority and thread counts
are separate: a disabled Global grant does not disable a separately granted
Single/Group.

## CLI

Use `--help wasithread` or `--help I1T` for the exact registered options.

- `--wasithread-single-create MODULE` creates and binds one module.
- `--wasithread-group-create GROUP` creates an initially disabled Group.
- `--wasithread-group-add-module GROUP MODULE` assigns an exact module once.
- `--wasithread-{global,single,group}-enable` and `-disable` select visibility.
- `--wasithread-{global,single,group}-interface ... thread-spawn hidden|deny|allow`
  selects import exposure and call admission.
- `--wasithread-{global,single,group}-limit ... KEY VALUE` sets a count cap.
- `--wasithread-{global,single,group}-limit-ratio ... KEY RATIO` sets a system-relative cap.
- `--wasithread-{global,single,group}-inspect ...` prints the cold policy.

Single and Group commands take the target name immediately after the option;
Global commands take no target. There is no Global membership command. Short
prefixes are `-I1T`, `-I1TS` and `-I1TG`, respectively (for example `-I1TSenable`).

Limit keys:

- `max-extra-threads`: concurrently admitted children. Zero refuses all children.
- `max-threads`: all admitted main and child threads. A main consumes a slot.
- `max-spawns`: cumulative accepted spawn admissions. Failed preparation releases
  occupancy but retains this optional attempt charge, preventing unbounded
  allocation/compilation retries. A permission/capacity refusal consumes nothing.

An omitted cap is unlimited. Explicit values are unsigned decimal or `unlimited`;
the reserved uint64 maximum cannot be supplied as a decimal value. Configured
limits are checked atomically before creation. Charges move with the actual
compile/native owners and end after physical join and resource cleanup. They
are not released merely when a guest function returns or when an observation
says that a worker finished. Configuration freezes at the first private-profile main admission;
debug inspection, restoration and Wasm code cannot expand it.

The ratio option accepts a finite `f32` in `[0,1]` through the fast_io floating
parser, for `max-extra-threads` or `max-threads`. The exact binary32 value is
multiplied by the observed ceiling and rounded down using integer arithmetic.
On Linux the ceiling is the minimum of kernel `threads-max`, soft `RLIMIT_NPROC`
and visible cgroup-v2 ancestor `pids.max` values. This is a cold snapshot, not a
promise of available resources. OS creation, address-space or memory limits may
still refuse creation. Unknown/unobservable ceilings reject the option; no
system limit is changed and no authority is granted by a ratio.

## Execution profile and explicit single-thread fallback

The execution profile `--wasi-threads` separately supplies worker/compiler and
memory capacities. It does not grant the I1T interface. Exposing I1T without
this execution profile still permits import validation; a call returns refusal
with an explicit profile diagnostic and never creates a native thread. The first two profile
counts accept `unlimited`; finite storage capacities remain explicit host
resource budgets. The module also needs grants for any actual WASIp1 imports.

Example independent authority for a main module named `main`:

```sh
--wasm-set-main-module-name main \
--wasithread-single-create main \
--wasithread-single-enable main \
--wasithread-single-interface main thread-spawn allow \
--wasithread-single-limit main max-extra-threads 0
```

The last option exposes the import but refuses creation, allowing a program that
handles `pthread_create` failure to run its work on the main thread. It does not
hide the import. An arbitrary pthread application is not automatically
serialized: fallback must be implemented by the application. The regression
`test/0084.ros-runtime/wasithread_fallback.c` uses exactly the same Wasm binary for
both explicit fallback and a real child followed by `pthread_join`. It imports
its shared memory from the same authenticated provider in both runs. A spawn
made inside a preloaded Wasm module creates that module's `wasi_thread_start`,
with that executing module's I1T authority; the main module is not substituted.

Call denials use `host_denied_wasithread.thread-spawn`; numerical refusals use
`host_limited_wasithread.thread-spawn`. Denial warnings are enabled by default,
quota warnings are opt-in. The ordinary warning disable/error controls apply.
Refusal itself returns the ABI's negative spawn result, unless the host has
explicitly promoted its diagnostic to a process-level error.

## Debug inspection

`info wasithread policy|members FIRST COUNT STOP` reads up to eight rows for a
current cooperative stop. Pagination does not limit the number of policies or
members. The independent **WASI Threads** IDE panel uses
`uwvm/wasithreadState` and offers authority/counts and membership tabs. Full and
read-only debugger permissions may inspect; limited and none may not. Neither
route creates threads, changes launch settings, resets counts or confers access
to host pointers. Unassigned modules are represented by Global, not enumerated
as an invented explicit membership list.

This metadata route is distinct from execution-state capture. It does not itself
supply dynamic-worker locals, native registers, checkpoints or deterministic
replay. Those features require the execution engine's authenticated private
worker/cohort adapters; an interface grant is not permission to bypass them.
