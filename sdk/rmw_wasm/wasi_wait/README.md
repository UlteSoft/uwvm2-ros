# Explicit finite WASI Preview1 wait for original RMW

This source-only provider implements the current original-RMW C
`uwvm_rmw_c_wait_provider.wait_slice` with one genuine
`__wasi_poll_oneoff` call from the selected original `<wasi/api.h>`. It creates
one relative MONOTONIC clock subscription, one event and one returned-count
slot. All three are allocated before callback export and remain owned by the
provider. No runtime, WASI implementation, original RMW source, existing ABI
catalog, permission table, native provider or generated message is changed.
No candidate, probe, model, compiler, Guest, ROS peer or network has been run.

## Explicit authority and actual time

The application/controller must separately admit the real
`wasi_snapshot_preview1.poll_oneoff` import and explicitly authorize the
Runtime's `monotonic-clock` non-FD permission for this Guest module. The current
Runtime checks the clock permission for the complete subscription array before
any clock read, sleep or output. There is no separate non-FD poll permission
bit in the current Runtime. FD `POLL_FD_READWRITE` rights govern FD
subscriptions; this provider creates none. It does not infer a WASI grant from
a ROS clock alias, open an FD to obtain time, invoke CLI grant switches or
supply default rights. A missing actual import is a loader/admission failure,
not a fabricated numeric unsupported response from this provider.

The original RMW `wait.c` still requires the independently prepared A platform
and its actual steady-clock capability. Both system and steady aliases are
explicit A-profile inputs; A opens neither by default. The RMW loop retains A,
reads the actual steady clock before/after every slice, checks rollback and an
absolute deadline, and only returns RMW timeout from measured time. Exhausting
the finite iteration budget is an error, never a synthetic timeout. Timer
success may wake early and does not prove elapsed nanoseconds. Relative WASI
nanoseconds need no epoch conversion to the independent A steady clock. No
poll count, callback credit, stdio byte, service graph name or CPU instruction
count becomes time. The provider never reads a ROS event or consumes an RMW
queue; those real readiness paths remain in original RMW.

## Exact one-import transaction

`create` validates 1..60,000,000,000 ns total wait, a positive slice at most
100,000,000 ns and no larger than that wait, and 1..1,048,576 iterations.
`callbacks` exports those exact bounds. These are local implementation limits,
not new permissions. `wait_slice` validates its positive argument before any
import. There is a process-global nonblocking C11 atomic entry flag: concurrent
entry returns17 instead of waiting on another Guest thread. This is not a
claim that the rest of original RMW is multithread-safe.

Every actual import receives a positive process-wide 64-bit userdata value,
never reused even after unpublished-state disposal. UINT64_MAX is usable once;
subsequent attempts return90 without wrap/reset or another import. At most16
live states can exist. The event and count start with explicit sentinels; the
result is accepted only when the full call errno is zero, count is exactly1,
event type is CLOCK, full64 userdata matches, and the event's full errno is
zero. CLOCK's unspecified fd_readwrite/padding bytes are ignored as the actual
original ABI requires. There is no retry, replacement syscall, injected Host
callback, extra subscription, heap allocation or application callback on the
wait path.

Original WASI errno values are not POSIX/ROS errno numbers. The provider keeps
the complete actual call and event uint16 values in diagnostics, then maps
known permission errors to13, invalid to22, allocation to12, unsupported to95,
interruption to4, cancellation to125, timeout to110 and overflow to90. Unknown
nonzero values become5. No nonzero original errno can become success. Count,
type, userdata, last requested duration and normalized first/last errors remain
independent observations. `observe` copies them without an import. Non-P1 or
native builds return95 and never declare fake WASI types or weak imports.

## Lifetime and cancellation boundary

`create` allocates through the actual valid rcutils allocator and registers the
opaque address before it is exposed. Public functions locate an admitted
address before dereferencing it. `callbacks` marks export sticky BEFORE
publishing its callback/state, including when a later profile installation
fails. The current C profile has no uninstall; exported state, allocator,
profile strings, code and copied callback values must remain valid until
process/Guest-group retirement. They are not freed after node/context teardown.
`dispose_unpublished` is only for a state whose callbacks were never exposed;
exported disposal returns17 and leaves its pointer intact. There is no reset,
unexport or guessed safe cancellation API.

The current Runtime's single relative-clock path performs a real native sleep
and then returns one event. This SDK cannot itself interrupt that blocked Host
import, prove a physical thread join, make Guest memory atomic under concurrent
reset, or implement WASI Threads. HLL's separately owned runtime adapter must
supply actual whole-group cancellation/deadline and physically join every
active import before memory/code/state retirement. Trap or unknown cancellation
inside this provider requires retiring the whole group; never clear the entry
flag and reuse its buffers. The independent Root process deadline remains
necessary. A finite timer argument is not a scheduler/preemption guarantee.

## Original RMW composition

`examples/original_rmw_composite.c` uses real original RMW types, original
generated C String/Empty/AddTwoInts objects, the existing complete pure-C codec
Registry and the actual current raw5 publisher/raw7 subscription/raw12 service
providers. It does not cast C objects to C++ codecs. Eleven independently
exported actual native RIHS01 values are mandatory CLI inputs; no test identity
or expected hash is a production default. Full native source/type-description
validation remains in the real codec Registry. The current trailing
`validation_state`, C availability profile and all providers must be recompiled
from this coherent source set; no old binary-layout compatibility is claimed.

The example composes one String publisher/subscriber, an Empty server and an
AddTwoInts client in one actual Context. Six distinct business aliases, two
explicit A clock aliases, exact resources/domain, and optional independent
query alias are required inputs. Query `-` leaves the genuine optional
availability callback/third owner absent. An explicit query uses the existing
same-owned-client `rmw_service_server_is_available` once and refuses actual
false; it never discovers readiness by waiting on names. A separately owned
native String counterpart, Empty client and Add server must supply actual
traffic. The Guest has no authority to start or configure them.

The example first calls original `rmw_wait` with no conditions for a real20ms
A-clock-measured timeout. Then it publishes the complete seven-byte String
(including embedded NUL), submits17+-9 and uses actual wait sets for
subscription/request/response readiness. Original implementation `.data`
addresses populate every complete list; arrays are rebuilt after each wait.
A real five-second A-clock deadline bounds the application loop. Actual service
writer/sequence/time metadata and independent native-info diagnostics are
preserved by the existing provider; local request correlation is never relabeled
as native identity. Reverse cleanup destroys the wait set before endpoints and
then node/context/options. A lifetime lease is released and A closed only after
all original objects/options are gone and B's profile can actually be removed.
Failed native/local cleanup keeps exported storage and any required A lease
alive for process-level recovery; no teardown failure is converted to success.

## Source, build and remaining qualification

The fixed upstream wasi-libc commit is
`165235bc467d5fa52d424f5d82587dfb76ed9d54`, actual tree
`deb8596016b6cfc9df25c4fc014c97e67b255fd1`. The source-only package retains its
original api.h, wasip1.h, version template, real wrapper and LICENSE plus
actual commit/tree DATA and Git-blob verification. This is a wasm32 Preview1
contract; it does not claim Preview2/3, wasm64, any unselected libc or vendor
SDK. The native fallback includes no original WASI header.

The authored CMake probe freshly clears normal/cache results, forces
EXECUTABLE in function scope and takes a volatile pointer to the real imported
function. It checks real types/fields and compile+link without executing.
STATIC_WASI requires that actual probe; NATIVE_UNSUPPORTED requires its
absence. There is no try_run, add_test, dependency download or automatic actor.
Actual complete static original ROS targets/providers are mandatory for the
composition. Follow `recipe.json`; all acceptance rows are pending.

The18 DATA/lifecycle groups validate the same production validator with actual
original WASI types; they do not replace a syscall. Separate Guest contracts
make real granted/denied imports, and the full composite requires actual native
peers. Actual producer/full dependency/link/import inspection, real rights
matrix, measured A-clock timeout, queue/message/native-identity end-to-end
checks, fault/cleanup/quota/concurrency cancellation and complete physical
owner retirement are all still required. No source review or local validator
model is native ROS, official RMW-suite, WASI Threads or all-vendor acceptance.
