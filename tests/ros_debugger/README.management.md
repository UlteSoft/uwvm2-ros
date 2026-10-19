# Update, ROS authority and debugger isolation regressions

These sources are authored regression contracts. They have not been imported,
compiled or executed by their author. Root must freeze the complete current
source, select actual tools and link inputs, and execute under its current
64 GiB/swap-zero cgroup with narrower serial process/RSS/deadline/output limits.
No source recipe or mathematical reply is an official ROS test, an actual
Runtime replacement result, a foreign-architecture result or a current PASS.

## Separate executable components

| Source | Actual mechanism exercised when executed | Boundary |
| --- | --- | --- |
| `management_contract.cc` | Real capability manager and native atomic hook: both-backend stale handles and new opens denied after retirement; tombstone blocks re-registration; original quotas/revocation/identity retained; unrelated owner and default denial unchanged; repeated retirement; distinct runtime-drain notification. | Native fixture registrations, no Runtime identity, Wasm, ROS SDK, IPC or network. |
| `host_retirement_contract.cc` | Real Host hook, manager, event streams and two Unix clients: actual hello sessions followed by EOF with no intervening RPC; old handle invalidation; queued/pending bytes discarded; session/watch rows retained as retired; unrelated module unchanged. A separate fixture then calls the genuine central Runtime reset twice and checks actual notification/cleanup/epoch order and no inherited authority. | Explicit native Host fixture metadata, never an active Runtime import context or fake identity API. Real current Runtime linkage is mandatory. Linux only; exit77 elsewhere. |
| `test_update_ros_boundary.py` | Twelve injected DATA cases using the actual manifest-loaded DAP adapter and raw ABI extractor: pre-send invalidation, failure/transport uncertainty, genuine-status requirement, script transcript refusal, old cohort refusal, monotonic IDs, output-event separation and exact38/43 registered business imports. | Broker replies are mathematical data; no Runtime, IDE, Gateway or ROS process is started. |
| `integration_guest.py` + `integration_runtime.py` | Existing real ROS1/ROS2/dual LLVM-full lane now additionally emits exact console-looking bytes through Guest WASI fd1, reaches the next application checkpoint and verifies the real authenticated controller remains running. | Actual selected Runtime/Guest/control route only. All original payload, denial, stopped-map, native peer, queue and cleanup assertions remain mandatory. |

`management_test_policy.h` defines explicit bounded String subscription policies
for these components. They are trusted native test data, not Runtime defaults,
SDK-created authority or instructions for granting a production application.
The accepting Host fixture threads implement only real IPC hello framing. They
are not a ROS Gateway; no policy, event or cleanup RPC is accepted. The test
requires the client to close both actual descriptors and the peers to observe
EOF. Both owner-only socket paths are created before the first peer thread, so
the finite umask change cannot race another fixture thread. All sockets and
their directories belong to the selected `TMPDIR` and are removed on normal
scope exit; the external owner remains responsible for abnormal-exit cleanup.

The Host fixture constructs its explicit metadata on the actual production
types and calls the installed production hook. It checks that this metadata
cannot give an out-of-context `capability_open` a Runtime identity or memory
range. It does not call a fabricated implementation of
`ros_caller_identity_host_api`, and such a stub is forbidden in its build.

### Genuine central reset inside the linked Host component

The original direct `management::retire(runtime_retirement, ...)` case remains
an explicitly local notification test. After it, a different native fixture is
registered at the epoch returned by the real
`observe_compiler_runtime_generation_host_api()`. Its two handles are verified
live; every authorization lease and the test's extra shared owner are released
before the test calls the actual `reset_runtime_state_host_api()`.

A static, ownerless observation callback temporarily wraps the already installed
production retirement hook. It records the notification and actual epoch, takes
and releases the Host registry lock to observe the current-epoch fixture, then
forwards the original arguments to `host::retire_authority`. It observes empty
Host state afterward and requires both epoch observations inside the callback
to equal the original epoch. Only after genuine reset returns may the epoch be
exactly the next value. The test then checks the original handles, old-epoch
opens and new-epoch opens are refused, and the real primary caller identity
still rejects this native thread outside Runtime execution.

The exact production hook is restored before a second genuine reset. That call
must advance the actual epoch once again, leave Host registrations empty and
inherit no old handle or authority. The observation callback must have run only
once, proving the second reset uses the restored production callback. Neither
reset invocation replaces a Runtime function, constructs a JIT/provider, installs
a Guest import context or claims that a Guest execution was drained in this
fresh component process: there was no active Guest. When actually executed,
this establishes the real empty-execution central reset-to-Host wiring and
epoch order; active-worker drain and successful/failed code publication remain
separate integration obligations.

The epoch observation API is a direct acquire read of the Runtime generation
counter. It acquires no publication/maintenance/Host lock and creates no TLS or
execution lease. The callback releases its Host observation lock before calling
the real cleanup; it neither reenters reset nor calls an execution/provider API.
The two existing socket peers are joined before installing the wrapper. Actual
reset is called from native `main`, without a controller/pause/publication scope,
so the fixture does not manufacture a callback-depth exemption or risk a
same-thread reset from an active Runtime entry.

## Manifest-bound invocation

The new Python file imports only standard-library modules. The guarded owner
loads all three exact current files from the admitted complete source manifest:

```python
# Pseudocode names for the owner's existing verified loader, not ambient imports.
dap = load("tools/debug/dap_adapter.py", "current_dap")
abi = load("sdk/tools/abi_contract.py", "current_ros_abi")
policy = load("tests/ros_debugger/test_update_ros_boundary.py", "current_update_policy")
suite = policy.suite(dap, abi.extract)
```

The extractor reads the real current Host registration declarations. No stored
ABI JSON, generated test registry or previous freeze can substitute for them.
It must remain associated with this same complete source. A future intentional
business ABI extension requires review of the explicit finite expected-name
sets; it cannot silently widen this management-negative contract.

The owner supplies the actual standard-library unittest runner and enforces a
finite outer watchdog. Loading the factory or obtaining a suite is not a PASS.
Record every actual failure, skip and error separately. The twelve cases must
not be added to any count of official upstream ROS tests.

`management-recipes.json` supplies source-only argv/profile requirements for
the native components. The manager component uses the same modern compiler
and header include closure as `core_contract.cc`. The Host component uses the
genuine current Runtime/ROS header link closure, as `host_contract.cc`, for
each explicitly selected ROS1, ROS2 and dual profile. Runtime symbol stubs,
interpreter results or previous build products cannot satisfy this closure.
Keep assertions enabled. Actual terminal markers print the checks executed;
the recipes retain prefixes rather than inventing an observed count.

## Real unknown-management-import Guest fixtures

`management_negative_imports/` contains six ordinary WAT sources. Each imports
exactly one absent management name (`runtime_update`, `debug_configure` or
`policy_set`, signature `() -> i32`) from `uwvm_ros1_v1` or `uwvm_ros2_v1`.
Its exported `_start` would write a unique `FORBIDDEN_MANAGEMENT_START` marker
through WASI stdout before calling that import. The intended refusal is actual
import linking, so the marker must never execute.

The recipe selects a genuine `wat2wasm` and real current Worker binaries. Run
the three ROS1 fixtures on the ROS1 Worker, the three ROS2 fixtures on the ROS2
Worker, and all six on the dual Worker: twelve distinct actual routes are
required. A `none` Worker only demonstrates that an entire module is absent;
it cannot qualify refusal of management names in an enabled business registry.
There are no ROS grants or extra WASI rights. The basic WASI stdio interface
exists only to make an unexpected `_start` unmistakable, not to grant a
management path.

For each route preserve the actual compiled Wasm, original WAT, selected tool
and Worker/source/profile identities, complete stdout/stderr, actual exit and
owned cleanup. A nonzero exit alone is insufficient. After stripping ANSI
presentation escapes, stderr must specifically identify `Missing export`, the
selected exact function and its exact ROS module, as emitted by the production
import resolver. An unknown module, malformed Wasm, parser error, quota failure,
Guest trap, timeout or signal is not this result. The unique marker must be
absent from both output streams. No exact future nonzero code, fixture count
PASS or actual run is predicted by these source recipes; Root's existing owned
Worker runner must establish and record each result. These twelve routes are
separate from the twelve DAP DATA cases and all official ROS test counts.

## Runtime commit obligations

The local components, including actual empty-execution reset, do not establish
the Runtime publication or active-worker drain boundary. Actual
Runtime integration must separately demonstrate all of these on the real
selected LLVM-full provider:

1. A rejected preparation or failed commit leaves the original module's ROS
   authority unchanged. The DAP still retires copied references conservatively.
2. A genuine successful commit sets the affected compiled module's permanent
   `ros_authority_retired` flag before publishing replacement entries. The
   primary caller-identity API then rejects that module incarnation; observed
   identity remains diagnostic only.
3. `ros_retire_replaced_module_host_api(module)` runs outside the stopped-domain
   publication mutex while the Guest remains paused, before the trusted host
   acknowledges replacement. A previously admitted Host lease may drain; there
   is no Guest callback, Gateway RPC or implicit regrant.
4. Another module keeps its own valid handles/queues, and code elsewhere in the
   replaced module cannot reuse its old aliases or open fresh handles.
5. Whole Runtime reset notifies after execution drain and before the old epoch
   or module storage retires. A new trusted launch still has no ROS resources
   unless its CLI explicitly grants them.

These are independent acceptance obligations, not assertions attributed to the
native fixture or synthetic DAP broker. Preserve raw failed attempts and current
tool/source/SDK/catalog/mapped-file/process/cleanup evidence for the actual lane.
Foreign/QEMU providers need their own actual commit/integration results,
including LoongArch; a native result cannot qualify an unexecuted target.

The user-facing trust boundary and recovery rule are in
[`sdk/docs/update-and-debug-boundary.md`](../../sdk/docs/update-and-debug-boundary.md)
and [`tools/debug/README.ros.md`](../../tools/debug/README.ros.md).
