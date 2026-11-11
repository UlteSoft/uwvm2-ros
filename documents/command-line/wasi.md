# WASI Commands

Source focus:

- `src/uwvm2/uwvm/cmdline/params/wasi_disable_utf8_check.h`
- `src/uwvm2/uwvm/cmdline/params/wasip1_*.h`
- `src/uwvm2/uwvm/cmdline/callback/wasi_disable_utf8_check.h`
- `src/uwvm2/uwvm/cmdline/callback/wasip1_*.h`
- `src/uwvm2/uwvm/imported/wasi/wasip1/storage/env.h`
- `src/uwvm2/uwvm/imported/wasi/wasip1/init/init_env.h`
- `src/uwvm2/uwvm/run/loader.h`
- `src/uwvm2/imported/wasi/wasip1/environment/environment.h`

## Availability

WASI Preview 1, memory64 bindings and both socket ABI implementations are
mandatory components of the Linux ROS build. Xmake rejects attempts to remove
them. The guest's namespace and exact interface states still default to hidden.
`--wasip1-socket-abi standard|wasix` selects the process's immutable memory32
socket ABI; it does not grant socket rights. Unix-domain preopens use the Linux
local-socket implementation.

## ROS product: zero initial authority

The `uwvm2-ros` target defines `UWVM_ROS_ZERO_HOST_DEFAULTS`; this policy does
not depend on whether the guest imports ROS, on debugger authority, or on the interpreter/JIT selection.
The retained ordinary uwvm shared headers, built without that product definition,
keep their previous defaults.

Every ROS guest starts with a disabled Global policy: no WASI namespace or host
imports, stdio objects, argv/environment, clocks, random, files, sockets or
process authority. A pure Wasm computation needs no host grant. A module without
a Single/Group binding selects Global; an explicit binding replaces Global
without inheriting its permissions or environment. Each selected target must
be enabled and each imported API must have an explicit `hidden|deny|allow`
state. The default is `hidden`. Namespace enablement grants no operation or
resource authority. Missing interfaces fail linking before start. Validation
checks namespace admission and import signatures without initializing
stdio/providers or running start. Explicit preopen configuration can open its
approved directory during command-line parsing.

Single and Group options take a target name. The corresponding `--wasip1-global-`
options omit that argument and configure the one shared default environment.
The obsolete trace and preload expose/hide controls are removed; ordinary uwvm
retains its separate defaults.

```bash
uwvm2-ros --wasm-set-main-module-name main \
  --wasip1-single-create main --wasip1-single-enable main \
  --wasip1-single-interface main fd_write allow \
  --wasip1-single-grant main stdout.write \
  --run stdout-only.wasm
```

Every libc import needs an exact interface state, including unused imports.
The deny state exposes its ABI but rejects calls; allow preserves all underlying
operation checks. See [exact host permissions](host-permissions.md) for the
complete three-state model and warning selectors. The SDK test launcher emits
the built module's exact import states within explicitly enabled namespaces.
Production run configurations always supply their own exact interface map.

| Command | Arguments | Effect |
| --- | --- | --- |
| `--wasip1-single-grant`, `--wasip1-group-grant` | `<target> <permission>` | Grant one operation permission. |
| `--wasip1-single-deny`, `--wasip1-group-deny` | `<target> <permission>` | Explicitly deny one operation permission for this target. |
| `--wasip1-single-limit`, `--wasip1-group-limit` | `<target> <limit> <decimal-u64|unlimited>` | Set a resource ceiling, without granting authority. |

A preopen can select its own initial base/inheriting masks using
--wasip1-single-mount-dir-rights or --wasip1-group-mount-dir-rights:

    uwvm2-ros --wasm-set-main-module-name main \
      --wasip1-single-create main --wasip1-single-enable main \
      --wasip1-single-mount-dir main /readonly /operator/readonly \
      --wasip1-single-mount-dir-rights main /readonly 270336 2097190 \
      --wasip1-single-mount-dir main /workspace /operator/workspace \
      --wasip1-single-mount-dir-rights main /workspace 271360 2097254 \
      --run app.wasm

Each command takes the target, an already-declared guest preopen name, and two
decimal uint64 masks. Guest-name matching uses the mount's lexical normalization.
Unknown roots, repeated rights for the same normalized root, missing masks,
overflow and unlimited are rejected. The explicit pair replaces the module
filesystem mask for that root only; a root without a pair uses the module masks,
which remain zero by default. These are operation rights, not I/O quotas.

The original pair is bound into the effective policy digest (WASI policy
encoding v5). Guest fd-rights reduction and closure do not rewrite that initial
host authorization. Current live policy transactions preserve these directory
bindings and reject unsupported rebinding; they cannot manufacture a wider
preopen. No default traffic quota, path-length cap or asynchronous mode is added.

Names are exact and case sensitive, without wildcards. Conflicting grant/deny is rejected. A single or group must already exist.

| Permission | Authorized operation (after namespace enablement) |
| --- | --- |
| `stdin.read` | Read the explicitly mapped inherited stdin source. |
| `stdout.write`, `stderr.write` | Write only the respective inherited output receiver. |
| `stdio.metadata` | Descriptor metadata, for an independently granted stream. |
| `stdio.readiness` | Poll readiness, for an independently granted stream. |
| `args` | Read only configured guest argv and its sizes. |
| `environ` | Read only configured environment and its sizes; no host inheritance by default. |
| `realtime-clock`, `monotonic-clock`, `process-clock`, `thread-clock` | `clock_time_get` for that clock ID. |
| `realtime-resolution`, `monotonic-resolution`, `process-resolution`, `thread-resolution` | `clock_res_get` for that clock ID. |
| `realtime-timer`, `monotonic-timer`, `process-timer`, `thread-timer` | Clock subscriptions for that ID, subject to backend support. |
| `random` | Bounded native CSPRNG reads; zero-length requests also consume call budget. |
| `proc-exit` | Request whole-process exit through the runtime; the first accepted code wins and per-operation exit wait policy applies. |
| `proc-raise` | A separately installed, scoped host signal handler; no process-wide fallback. |
| `sched-yield` | Bounded scheduler requests. |
| `thread-spawn` | Guest spawning through the separately authorized WASI threads bridge. Core atomics/threads do not grant this. |

`stdio-full-rights` is rejected in this product. It remains an upstream
ordinary uwvm input only when the ROS product policy is absent.

### Standard stream slots and bounded providers

Slots 0/1/2 are permanently reserved for stdin/stdout/stderr, including in a
fresh embedded environment. Unapproved slots contain no native object and have
zero rights. Closing leaves a non-allocatable tombstone; renumbering from or into
any reserved slot is rejected. Ordinary files and accepted sockets allocate
from 3. No `/dev/null` is installed. DEV does not change these mappings, rights
or source data. A cached bridge/descriptor cannot bypass current namespace enablement or
stream grants.

The Linux ROS stdio adapter retains the native read/write semantics of an
explicitly approved file, terminal or device binding. It does not modify inherited
open-file-description flags. Byte-stream pipe writes and stream sockets use
nonblocking operations and cancellable readiness waits. Pipe reads (including
blocking inputs) and packet-mode writes retain native read/write semantics,
including packet truncation; a read end cannot identify another writer's mode.
A PIPE_BUF-sized private buffer bounds temporary memory, not a guest request.
Blocking writes advance through every vector and chunk. Nonblocking writes and
reads may return short transfers. If cancellation or a native error occurs after
some bytes were written, the call reports that actual progress; with no progress
it returns the corresponding error. Callers must still handle short transfers.

A blocking stream has no implicit timeout. Quota waits use the configured
reject/wait policy and native timer. A pipe read, file, terminal or device syscall already
in progress retains its execution lease until it actually returns: lifecycle
cancellation does not falsely report its completion, and hot-update draining
must wait or reject before commit. The default proc_exit ends the process;
explicit native-wait exit policies may instead wait for that call to finish.
The VM does not claim an arbitrary device syscall has a cancellable deadline.
Nonblocking backpressure reports EAGAIN, disk exhaustion reports ENOSPC, and a
closed writer reports EPIPE without terminating the host through SIGPIPE.
Non-stream sockets are outside this byte-stream adapter. No guest output queue
is created. Management logging is a separate channel.

### Explicit resource limits and continuity

Exposure and operation authority start at zero. After an explicit grant, every
operation quota defaults to `unlimited`; granting an operation does not silently
install a byte, call, concurrency or time ceiling. Hosts may set limits with
`--wasip1-single-limit` or `--wasip1-group-limit`, using a decimal value or
`unlimited` (`UINT64_MAX` in the native policy). Clock granularity defaults to 1 ns,
which adds no coarsening. Poll subscription count, poll wait duration, approved
args/env bytes and descriptor count also have no additional default quota.
ABI integer ranges, guest memory bounds, native errors and allocation failures
still apply. Temporary I/O vectors and poll arrays report allocation failures
instead of enforcing the removed fixed vector/subscription ceilings.

All ROS environments share a stable, synchronized process ledger. Creating a
new module/environment or updating code does not reset window/total consumption.
Counters saturate instead of wrapping, including under unlimited policy, so a
later finite limit cannot discard prior consumption. Reservations precede native
RNG and I/O and guest output writes. Failed requests that reserved work do not
refund it. Internal VM clocks/nonce randomness remain separate from guest calls.

Limit names use prefixes `random`, `stdin`, `stdout`, `stderr`, `realtime`,
`monotonic`, `process`, `thread`, `poll`, `signal`, `yield`, `filesystem`, followed by
`.request-bytes`, `.window-bytes`, `.window-calls`, `.total-bytes`, `.total-calls`
or `.concurrent`. Window quotas replenish per one-second monotonic window. The host reads
`fast_io::posix_clock_gettime(posix_clock_id::monotonic)` and arms Linux
`timerfd` with `CLOCK_MONOTONIC` and `TFD_TIMER_ABSTIME`. Changing wall-clock
date/time does not advance quotas or restart a wait. EINTR and wake notifications
recheck the same deadline and live counters. A failed/backward monotonic reading
makes the current admission return `EIO` without refilling quota or performing
the requested native operation. A later valid reading can resume operation
under the original, unchanged counters and limits.
Each of these six dimensions has its own overflow action, selected with
`--wasip1-single-limit-action` or `--wasip1-group-limit-action`:

```bash
--wasip1-single-limit main random.request-bytes 4096
--wasip1-single-limit main random.window-calls 100
--wasip1-single-limit-action main random.window-calls wait
--wasip1-single-limit-exit-action main random.window-calls cancel
--wasip1-single-limit main random.total-bytes unlimited
--wasip1-single-limit main monotonic.granularity-ns 10000000
```

`reject` is the default action. A request larger than a request or entire-window
byte ceiling returns `E2BIG`; exhausted finite counters return `EAGAIN`.
`wait` is accepted only for window bytes, window calls and concurrency: these
can replenish. Request and lifetime ceilings cannot replenish and configuring
`wait` for them is rejected. Zero window-call/concurrency capacity also cannot
make progress and returns an error. Synchronous quota waits happen after the
provider has released its guest-memory and descriptor locks; a retry checks
current authority, descriptor identity and memory bounds again before effects.
`--wasip1-single-limit-exit-action` / `--wasip1-group-limit-exit-action`
also accepts `<budget>.native-wait` to select the exit behavior of actual
pipe/socket/poll waiting after the operation quota was admitted. For example,
`--wasip1-single-limit-exit-action main stdout.native-wait wait` preserves a
pending stdout transfer without introducing a finite stdout quota. It
selects `cancel` (default) or `wait` for an individual native wait or window/concurrency
dimension when another guest thread exits. `cancel` releases that quota wait;
`wait` lets it finish according to the host's approved policy. Other Wasm modules
cannot change this policy or cancel its quota. Trusted host stop/update controls
remain separate. Cancellation reports `ECANCELED`. No asynchronous quota mode
exists.

An authorized proc_exit from any module/thread requests exit of the entire
process, using the first accepted exit code. With only default cancel selections,
the process exits directly. Already admitted operations selected to wait retain
their native owner until the actual operation completes; newly arriving work
cannot acquire that hold after exit is claimed. Canceled provider calls join the
same exit instead of resuming Guest code and turning cancellation into a trap
that would stop another selected waiting owner.

Invalid Guest pointer extents return WASI `EFAULT`; an address that violates
WASIp1 ABI alignment returns `EINVAL`. This includes nested I/O vectors, both
paths of a two-path operation, and result pointers even when the vector count is
zero. All participating ranges are checked before native I/O or output writes.
These errors do not terminate the host or turn a refused operation into success.
Recoverable host allocation failures return `ENOMEM` where an allocation is
required. The application remains responsible for handling the returned errno;
known Guest traps are contained at the runtime's protected execution boundary.
The ordinary `uwvm2` product keeps its existing policy and fault behavior.

An explicit clock granularity rounds clock values and resolution independently
per clock ID, even when guest precision is zero. This limits the interface, not
all timing side channels. `poll-subscriptions`, `poll-wait-ns` and
`args-env-bytes` are optional explicit limits. Absolute and CPU-clock timers
remain unsupported by the current product poll adapter; no default one-second
poll timeout remains. These scalar bounds reject requests or report a poll
deadline; they do not use operation-quota wait actions.

Directory preopens are explicit objects with initially zero operation masks.
`filesystem.base-rights` and `filesystem.inheriting-rights` configure their
explicit WASIp1 rights masks; the module must also enable the namespace. Actual
procfs/sysfs handles are rejected, including alternate path spellings.
The VM imposes no additional fixed 4096-byte path limit. Existing WASI memory
bounds, resource rights and native filesystem semantics decide the result.
The independent `filesystem` budget covers ordinary-descriptor read/write and
positioned I/O, allocation and resizing, plus native path, metadata, directory,
seek, flag, advise and sync operations. Each authorized native operation reserves
one call before native work; multi-fd link/rename reserves once after both rights
checks. Close remains available to release resources after quota exhaustion.
Guest-local prestat queries, rights reductions and fd renumbering do not reserve
native filesystem work. Standard-stream metadata keeps its explicit grant and
does not debit the filesystem pool. Empty-vector I/O and zero-length
allocation still consume call quotas. Allocation reserves the requested final
extent (`offset + length`, checked for overflow); resizing reserves the target
size. These are conservative upper bounds, not actual newly allocated bytes:
a small extension of a large existing file needs a sufficiently large request
ceiling. Charging a size difference would race with other descriptors or host
aliases that shrink the file. Budget rejection occurs before mutation and does
not change file size/content. Counters are shared across authorized filesystem
objects; separate per-mount/per-file budgets are not currently provided.
The native filesystem implementation remains synchronous. Its calls are not a
cancellable broker and arbitrary filesystem/device operations do not have a
hard deadline. Applications requiring that property must not grant those calls.
Raw per-call WASI trace output is rejected; use bounded runtime diagnostics.
Socket functions return `ENOTSUP`, and socket preopens currently return a startup error in this product because their
connect/listen/queue lifetime has no qualified bounded provider contract.
When compiled in, the existing nonstandard `*_wasm64` imports are exposed by
the same namespace enable. They retain their i64-pointer ABI and the same
operation/resource checks; Core Memory64 remains a separate Wasm feature. Generic debugger descriptor
creation/rebinding and host-environment checkpoint restore are rejected; a
DeveloperGrant is not authority to change host resources or revive rights.

Permissions and resource limits are installed by the trusted host before
execution. Generic guest/debug writes cannot access them. Changes require a
host management transaction with execution quiescence; raw concurrent writes
to environment fields are not an authorization API.

## Explicit target binding

Create a Single target before setting its properties. Its module name must match
the loaded main/preload identity exactly. A Group must also be created before
members are added or properties configured. Adding a member does not expose its
imports; the group's namespace and exact interface states still apply.
Unassigned modules select Global, which is disabled until explicitly enabled.
`--wasip1-global-add-module` is optional; explicit Single/Group assignments take
precedence. Group members and Global members share their own environment,
arguments, descriptors, socket preopens and counters. Different policies share
none of these resources. A second explicit assignment or duplicate member is
rejected before Guest start.

Global has matching grant, environment, descriptor and limit controls; omit the
target argument used by Single/Group. Socket rights are still per descriptor.

## Arguments and environment

`single/group-set-argv0` and `single/group-force-args` configure guest argument
values. The latter consumes an explicit count and that many argument tokens.
`force-args` and `set-argv0` for the same target are mutually exclusive.
Configuring values does not grant the `args` operation or expose `args_get` and
`args_sizes_get`.

Host environment inheritance is off by default. A target can explicitly select
inheritance, then delete inherited entries or add/replace named values.
These settings do not expose the environment APIs or grant `environ`.
Secrets needed by the host launcher, debugger, signer or Gateway should remain
outside the guest environment. A guest's environment does not configure the
host management profile.

## Directory preopens and descriptor limits

`--wasip1-single-mount-dir MODULE GUEST HOST` and the corresponding Group
command bind an approved host directory handle. Parsing opens that directory;
validation mode does not undo that host action. The host selects the root,
while later guest traversal remains constrained by descriptor rights and
the path resolver. Choosing a root is not a grant to every filesystem API.

Guest mount names are normalized unless explicitly disabled. Empty names,
embedded NUL, disallowed dot segments and ambiguous repeated separators are
rejected. Duplicate and ancestor/descendant guest mounts conflict unless
`--wasip1-allow-overlapping-mount-paths` is selected. Disabling normalization
does not independently disable overlap checking. UTF-8 relaxation still rejects
embedded NUL.

Use `--wasip1-single-mount-dir-rights` or its Group counterpart for the explicit
base and inheriting rights on an already-declared guest root. The full pair
belongs to the immutable initial resource identity. Rights reduction by the
guest cannot create a broader child descriptor.

The target `set-fd-limit` option controls the descriptor resource policy.
It neither creates a descriptor nor grants an operation. Preopened socket FD
numbers and dynamically allocated descriptors must remain compatible with that
policy and the WASI FD integer range. No new implicit request or path-length
ceiling accompanies a mount or descriptor grant.

## Socket preopens and ABI selection

The Single/Group TCP connect/listen and UDP connect/bind commands describe the
approved socket at launch. Connect forms accept an IP address or DNS name with
a port; bind/listen forms require a concrete local address. Unix-domain forms
use `unix PATH`, including the Linux abstract namespace where supported.
Address resolution and native bind/connect failures are reported through the
host launch path. No public listener is created merely by exposing a WASI API.

Assign preopens outside the reserved standard-stream slots and avoid duplicate
FD assignments. Per-target wrappers reject duplicate descriptors. A socket
preopen is not sufficient authority for guest traffic. Use the corresponding
`socket-rights` command to select base rights and accepted-child rights:
`read,write,accept,shutdown,readiness,flags,metadata`, or `none`.

An inherited stdio object that is itself a socket must also satisfy its actual
socket authority. Renumbering or accepting a socket cannot turn it into an
unrestricted file or another module's approved endpoint. Closing and reopening
an object must not revive an old capability identity.

`--wasip1-socket-abi standard|wasix` selects the memory32 `sock_accept`
signature before linking. Standard uses three arguments; the supported WASIX
variant has the additional peer-address output. Both implementations are
compiled, but their conflicting import signatures cannot be selected
simultaneously in one process. Memory64 has its own explicit ABI. This switch
does not provide the rest of the WASIX API and grants no resources.

Networking is controlled by preopened objects and their rights. There is no
protocol-identification heuristic that claims to recognize all ROS, debugger
or tunneled traffic. A deployment requiring those boundaries must choose
endpoints and host network authority accordingly; guest code has no management
import regardless of its network grants.

## Tracing and diagnostics

`single/group-trace` selects `none`, `out`, `err` or `file PATH` for the
target's WASI trace. The output is a host diagnostic destination, not a guest
filesystem grant. Tracing is independent of permission-denial and quota warning
selectors described above. Debugger host observations and JSON export have a
separate [observation contract](../runtime/debug-host-observations.md).

All examples that only configure values or resources are launch fragments.
An actual application must also enable its namespace, expose every exact import,
grant its required operations, and select its module path. Adding broad grants
merely to silence a diagnostic defeats the explicit deployment contract.

## Current command inventory

The following entries are generated from the same actual binary help capture
as the [complete CLI catalog](cli-catalog.md). The usage line includes only
advertised spellings. It describes the accepted arguments; the authority and
lifetime rules above still apply.

### `--wasi-disable-utf8-check`

```text
[--wasi-disable-utf8-check|-Iu8relax]
```

Disable WASI path/name UTF-8 checks for command-line and runtime; does not affect wasm parsing or module-name validation.

### `--wasi-thread-signals`

```text
[--wasi-thread-signals] <read-fd> <cookie> <max-frames> <max-pipe-bytes> <grace-ms> <retire-ms>
```

Explicit Linux CLI SIGINT/SIGTERM delivery to the Source-owned WASI pipe; separately requires --wasi-threads.

### `--wasi-threads`

```text
[--wasi-threads] <max-active:count|unlimited> <max-spawns:count|unlimited> <native-stack-bytes> <per-child-storage> <per-child-code> <shared-memory-bytes>
```

Select finite WASI Threads quotas; the stack value bounds each Guest AND compiler mapping, including guards.

### `--wasip1-allow-overlapping-mount-paths`

```text
[--wasip1-allow-overlapping-mount-paths]
```

Allow duplicate or overlapping WASI Preview 1 mount guest paths.

### `--wasip1-disable-mount-path-normalization`

```text
[--wasip1-disable-mount-path-normalization]
```

Store raw WASI Preview 1 mount guest paths instead of normalized paths.

### `--wasip1-global-socket-rights`

```text
[--wasip1-global-socket-rights] <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>
```

Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces.

### `--wasip1-group-add-module`

```text
[--wasip1-group-add-module] <group:str> <module:str>
```

Bind one module name to an existing named WASI Preview 1 group.

### `--wasip1-group-add-or-replace-environment`

```text
[--wasip1-group-add-or-replace-environment] <group:str> <env:str> <value:str>
```

Add or replace one environment variable for one named group.

### `--wasip1-group-create`

```text
[--wasip1-group-create] <group:str>
```

Create one named shared WASI Preview 1 module group.

### `--wasip1-group-delete-system-environment`

```text
[--wasip1-group-delete-system-environment] <group:str> <env:str>
```

Delete one inherited host environment variable for one named group.

### `--wasip1-group-deny`

```text
[--wasip1-group-deny] <group:str> <permission:str>
```

Explicitly deny one finite WASI permission for the named group.

### `--wasip1-group-disable`

```text
[--wasip1-group-disable] <group:str>
```

Disable WASI Preview 1 for one named group.

### `--wasip1-group-disable-utf8-check`

```text
[--wasip1-group-disable-utf8-check] <group:str>
```

Disable WASI Preview 1 UTF-8 checks for one named group.

### `--wasip1-group-enable`

```text
[--wasip1-group-enable] <group:str>
```

Enable WASI Preview 1 for one named group.

### `--wasip1-group-enable-utf8-check`

```text
[--wasip1-group-enable-utf8-check] <group:str>
```

Enable WASI Preview 1 UTF-8 checks for one named group.

### `--wasip1-group-force-args`

```text
[--wasip1-group-force-args] <group:str> <num:size_t> [arg:str]...
```

Replace one named group's WASI Preview 1 argv vector with an explicit argument list.

### `--wasip1-group-grant`

```text
[--wasip1-group-grant] <group:str> <permission:str>
```

Explicitly grant one finite WASI permission for the named group.

### `--wasip1-group-inherit-system-environment`

```text
[--wasip1-group-inherit-system-environment] <group:str>
```

Inherit host environment variables into one named group.

### `--wasip1-group-interface`

```text
[--wasip1-group-interface] <group:str> <api:str> <hidden|deny|allow>
```

Configure one exact WASIp1 API: hidden fails linking; deny exposes but rejects; allow retains resource checks. Default hidden.

### `--wasip1-group-limit`

```text
[--wasip1-group-limit] <group:str> <limit:str> <value:u64|unlimited>
```

Set one explicit WASI resource limit (decimal or unlimited; no implicit grant).

### `--wasip1-group-limit-action`

```text
[--wasip1-group-limit-action] <group:str> <limit:str> <reject|wait>
```

Choose reject or synchronous wait for an explicit operation limit; wait supports windows and concurrency.

### `--wasip1-group-limit-exit-action`

```text
[--wasip1-group-limit-exit-action] <group:str> <limit:str> <cancel|wait>
```

Choose cancel or wait on guest exit for one window/concurrency quota or native-wait; default cancel.

### `--wasip1-group-mount-dir`

```text
[--wasip1-group-mount-dir] <group:str> <wasi dir:str> <system dir:path>
```

Mount a host directory into one named group's WASI Preview 1 sandbox.

### `--wasip1-group-mount-dir-rights`

```text
[--wasip1-group-mount-dir-rights] <group:str> <guest-dir:str> <base:u64> <inheriting:u64>
```

Set explicit base/inheriting rights for one previously declared preopen; other roots are unchanged.

### `--wasip1-group-noinherit-system-environment`

```text
[--wasip1-group-noinherit-system-environment] <group:str>
```

Do not inherit host environment variables into one named group.

### `--wasip1-group-set-argv0`

```text
[--wasip1-group-set-argv0] <group:str> <argv0:str>
```

Override WASI Preview 1 argv[0] for one named group.

### `--wasip1-group-set-fd-limit`

```text
[--wasip1-group-set-fd-limit] <group:str> <limit:size_t>
```

Set the WASI Preview 1 fd limit for one named group.

### `--wasip1-group-socket-rights`

```text
[--wasip1-group-socket-rights] <group:str> <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>
```

Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces.

### `--wasip1-group-socket-tcp-connect`

```text
[--wasip1-group-socket-tcp-connect] <group:str> <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 TCP client socket for one named group.

### `--wasip1-group-socket-tcp-listen`

```text
[--wasip1-group-socket-tcp-listen] <group:str> <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Listen on a WASI Preview 1 TCP server socket for one named group.

### `--wasip1-group-socket-udp-bind`

```text
[--wasip1-group-socket-udp-bind] <group:str> <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Bind a WASI Preview 1 UDP socket for one named group.

### `--wasip1-group-socket-udp-connect`

```text
[--wasip1-group-socket-udp-connect] <group:str> <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 UDP socket for one named group.

### `--wasip1-single-add-or-replace-environment`

```text
[--wasip1-single-add-or-replace-environment] <module:str> <env:str> <value:str>
```

Add or replace one environment variable for one single module.

### `--wasip1-single-create`

```text
[--wasip1-single-create] <module:str>
```

Create one anonymous module-local WASI Preview 1 group.

### `--wasip1-single-delete-system-environment`

```text
[--wasip1-single-delete-system-environment] <module:str> <env:str>
```

Delete one inherited host environment variable for one single module.

### `--wasip1-single-deny`

```text
[--wasip1-single-deny] <module:str> <permission:str>
```

Explicitly deny one finite WASI permission for the single module.

### `--wasip1-single-disable`

```text
[--wasip1-single-disable] <module:str>
```

Disable WASI Preview 1 for one single module.

### `--wasip1-single-disable-utf8-check`

```text
[--wasip1-single-disable-utf8-check] <module:str>
```

Disable WASI Preview 1 UTF-8 checks for one single module.

### `--wasip1-single-enable`

```text
[--wasip1-single-enable] <module:str>
```

Enable WASI Preview 1 for one single module.

### `--wasip1-single-enable-utf8-check`

```text
[--wasip1-single-enable-utf8-check] <module:str>
```

Enable WASI Preview 1 UTF-8 checks for one single module.

### `--wasip1-single-force-args`

```text
[--wasip1-single-force-args] <module:str> <num:size_t> [arg:str]...
```

Replace one single module's WASI Preview 1 argv vector with an explicit argument list.

### `--wasip1-single-grant`

```text
[--wasip1-single-grant] <module:str> <permission:str>
```

Explicitly grant one finite WASI permission for the single module.

### `--wasip1-single-inherit-system-environment`

```text
[--wasip1-single-inherit-system-environment] <module:str>
```

Inherit host environment variables into one single module.

### `--wasip1-single-interface`

```text
[--wasip1-single-interface] <single:str> <api:str> <hidden|deny|allow>
```

Configure one exact WASIp1 API: hidden fails linking; deny exposes but rejects; allow retains resource checks. Default hidden.

### `--wasip1-single-limit`

```text
[--wasip1-single-limit] <single:str> <limit:str> <value:u64|unlimited>
```

Set one explicit WASI resource limit (decimal or unlimited; no implicit grant).

### `--wasip1-single-limit-action`

```text
[--wasip1-single-limit-action] <single:str> <limit:str> <reject|wait>
```

Choose reject or synchronous wait for an explicit operation limit; wait supports windows and concurrency.

### `--wasip1-single-limit-exit-action`

```text
[--wasip1-single-limit-exit-action] <single:str> <limit:str> <cancel|wait>
```

Choose cancel or wait on guest exit for one window/concurrency quota or native-wait; default cancel.

### `--wasip1-single-mount-dir`

```text
[--wasip1-single-mount-dir] <module:str> <wasi dir:str> <system dir:path>
```

Mount a host directory into one single module's WASI Preview 1 sandbox.

### `--wasip1-single-mount-dir-rights`

```text
[--wasip1-single-mount-dir-rights] <single:str> <guest-dir:str> <base:u64> <inheriting:u64>
```

Set explicit base/inheriting rights for one previously declared preopen; other roots are unchanged.

### `--wasip1-single-noinherit-system-environment`

```text
[--wasip1-single-noinherit-system-environment] <module:str>
```

Do not inherit host environment variables into one single module.

### `--wasip1-single-set-argv0`

```text
[--wasip1-single-set-argv0] <module:str> <argv0:str>
```

Override WASI Preview 1 argv[0] for one single module.

### `--wasip1-single-set-fd-limit`

```text
[--wasip1-single-set-fd-limit] <module:str> <limit:size_t>
```

Set the WASI Preview 1 fd limit for one single module.

### `--wasip1-single-socket-rights`

```text
[--wasip1-single-socket-rights] <single:str> <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>
```

Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces.

### `--wasip1-single-socket-tcp-connect`

```text
[--wasip1-single-socket-tcp-connect] <module:str> <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 TCP client socket for one single module.

### `--wasip1-single-socket-tcp-listen`

```text
[--wasip1-single-socket-tcp-listen] <module:str> <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Listen on a WASI Preview 1 TCP server socket for one single module.

### `--wasip1-single-socket-udp-bind`

```text
[--wasip1-single-socket-udp-bind] <module:str> <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Bind a WASI Preview 1 UDP socket for one single module.

### `--wasip1-single-socket-udp-connect`

```text
[--wasip1-single-socket-udp-connect] <module:str> <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 UDP socket for one single module.

### `--wasip1-socket-abi`

```text
[--wasip1-socket-abi] <standard|wasix>
```

Choose the immutable wasm32 sock_accept ABI for this process: standard (default, 3 arguments) or wasix (4 arguments, peer address). Grants no interfaces or socket rights.

## Signed policy encoding

Live WASI policy manifests use `UWPOL5` with twelve operation-budget classes;
`filesystem` is appended after the preceding eleven. Each class encodes six
64-bit limits followed by six one-byte reject/wait actions and six one-byte
cancel/wait actions for guest exit, then one native-wait guest-exit action. The
interface-state map precedes these budget rows, after enabled/permissions/fd
limit. Older `UWPOL1` through `UWPOL4` payloads are rejected rather than
reinterpreted. The signed transition keeps the same
environment/resources and process ledger; lowering a limit does not erase
already consumed work. Policy expiration is not an implemented WASI grant
field: signature/activation-ticket expiry must not be described as automatic
expiration of an installed guest permission.


### Policy-scope resource accounting

The default global policy, each module-specific `single`, and each named
`group` have independent accounting. Members of one named group share that
group's environment and quota; different named groups do not spend each other's
budgets. Identical limit values do not merge independent policies. Group
membership never grants another module's native handle or memory. ROS1 and
ROS2 also retain distinct accounting. Omitted ceilings remain unlimited;
selecting an interface or granting a resource does not install a rate limit.
Code generation replacement and checkpoint-world cloning preserve an existing
policy's consumed budget. Closing/retiring resources releases active capacity,
but does not replenish spent operation/creation credits.
