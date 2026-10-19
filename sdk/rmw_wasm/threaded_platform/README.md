# Original-rclcpp WASI signal transport, explicit profile version 1

This source-only component implements a finite target-specific asynchronous
Host-control transport and a private port of the **original** rclcpp signal
handler. It has not been configured, compiled, linked, imported or executed.
The SDK33 producer, Guest contracts, original program and native ROS routes have
zero actual qualification. It is one component of Root's separate full threaded
RMW composition, not a claim that original rclcpp currently runs on WASI.

## Actual source and build boundary

The adapted private files are the original `rclcpp/src/rclcpp/signal_handler.hpp`
and `.cpp` at ros2/rclcpp commit
`53fa7aeea86e8ed484f9465fa14059faf13a71f1`. Their original attribution and Apache
license remain. All translation units using that private class must consume the
same two adapted source files and the same target definition. Merely replacing
one object while utilities.cpp sees the old private header is forbidden.
`FixedOriginalPort.cmake` checks both actual adapted hashes and the original
CMakeLists.txt hash before attaching the actual static library. The fixed source
port is selected only for real WASI with `UWVM_RCLCPP_WASI_SIGNAL_PORT=1`.
It suppresses that private header's generic POSIX sigaction inference; it does
not force a successful sigaction function check or link an emulated signal()
implementation as asynchronous Host delivery.

The library option `UWVM_BUILD_ORIGINAL_RCLCPP_WASI_SIGNAL` defaults OFF. An OFF
library contains actual 95 refusal bodies and does not include pthread/WASI
headers in its implementation. Enabling it requires a fresh genuine executable
compile/link probe, actual wasm32 WASI threads, static libc++/libc++abi/libunwind
with nonlegacy Wasm EH, real semaphores and the original Preview1 declarations.
The probe uses `CMAKE_TRY_COMPILE_TARGET_TYPE=EXECUTABLE` in function scope and
clears its private cache entry before and after checking. An inherited static
try-compile cannot qualify it. Unsupported native builds remain 95, not fake
WASI structs. No tool or probe has been invoked here.

This component deliberately does not alter the existing original-rclcpp fatal
in `sdk/rmw_wasm/cmake/project-hook.cmake`. Root's real threaded B/C ownership,
indefinite RMW wait/guard semantics, full default endpoint codecs, actual target
producer and initial-main/Runtime admission must independently close before a
composite private build hook may admit that source. The platform attachment
function is an exact source/link attachment, not a replacement for those gates.

## Root-only channel construction and permissions

Root must create a genuinely separate pipe, keep its write endpoint outside the
Guest and install only its read endpoint at a distinct Guest fd >= 3. That map
entry must retain the exact open-file-description identity, O_NONBLOCK, FD_READ
and POLL_FD_READWRITE until actual whole-Group physical retirement. Root must
separately grant the real WASI MONOTONIC clock operations required by poll and
libc++ timed synchronization. ROS Clock aliases used by original rcutils/A are
separate permissions and do not grant these WASI rights. No SDK operation opens
or upgrades a descriptor. stdin/stdout/stderr, a pathname, a ROS alias, a packet
cookie or ordinary Guest input is never a Host signal authority.

The currently reviewed threaded Runtime leaf has exact numeric fd_read and
poll_oneoff forms `(i32,i32,i32,i32)->i32` and an independently authorized mapped
FD route; no new Host import is required by this transport. Actual Root CLI/map
installation of this trusted channel remains a separate pending integration.
Source inspection of those leaves is not proof of an installed descriptor,
SDK archive, produced import catalog or executed Group.

The little-endian frame is exactly 32 bytes:

| Offset | Width | Required value |
|---|---:|---|
| 0 | 4 | ASCII URS1 |
| 4 | 2 | version 1 |
| 6 | 2 | extent 32 |
| 8 | 8 | actual immutable Source/channel cookie |
| 16 | 8 | contiguous sequence starting at 1 |
| 24 | 4 | 1 = SIGINT meaning, 2 = SIGTERM meaning |
| 28 | 4 | zero reserved |

Events map to the real selected SDK SIGINT/SIGTERM constants. They are not OS
numeric signal values copied into Wasm. Cookie and sequence are associations;
the inaccessible Host-owned write endpoint is the authority. Root must serialize
frames from its signal sources, write only complete admitted records under its
own deadline, preserve partial-write facts and never replay an uncertain record.
No update/debug/configuration command exists in this protocol. Host write-end
closure is an error, not a fabricated graceful signal.

The finite profile admits one preparation and at most one real activation per
Source lifetime, 1..1,048,576 frames and a positive poll slice <=100,000,000 ns.
Reinstallation after a completed activation is deliberately unsupported (17).
A fresh Source/Group, genuine initial-main and channel are required. This finite
boundary is not complete POSIX process-wide signal or repeat-init compatibility.
There is no guest fd close or process-reset function.

## Real receive, dispatch and retirement

The receiver has source-lifetime storage: actual std::thread owner, mutex,
callbacks, atomic stop/publication state, a 32-byte partial frame, and two real
WASI subscription/event slots. The real pthread cannot pass the publication
mutex before its owner is stored and original handler fields, semaphore and
original deferred worker are ready. It performs one fd-read subscription and
one real MONOTONIC timer subscription. All actual poll errno/count, event tag,
userdata, per-event errno, duplicate and flags are checked before using readiness.
Only fd readiness permits fd_read. Full read errno/count is checked; EAGAIN goes
back to a new actual readiness wait. Timer wake never becomes a signal or elapsed
credit. Positive reads assemble a single frame without allocating a second copy.

Only a complete valid frame may acquire its callback snapshot. Sequence and
quota never wrap; wrong cookie/sequence, malformed/reserved/unsupported fields,
EOF/HUP, exhausted quota/userdata and import errors make failure sticky. They
invoke only a separate failure wake. That wake wakes the real original deferred
worker, which observes the actual platform error and fails the Guest Group; it
does not set signal_received or call a fake shutdown handler. Bootstrap checks
terminal diagnostics again even if application code returned zero.

Uninstall first closes callback admission and genuinely joins the receiver.
Only then may it restore the original private handlers, wake/join the original
deferred worker and destroy its semaphore. A receiver join failure is recorded,
retains every owner and cannot be retried or detached. A successful stop before activation closes the domain as well; thread_created
remains false, so diagnostics do not claim an actual thread existed. Original
semaphore post, wait or destroy failure takes a genuine failure/terminate path instead of
pretending notification or cleanup succeeded. Runtime forced Group stop and
physical Source retirement remain mandatory and independent. A Guest std::thread
join is not evidence that Host native workers, effects, TLS or Runtime owners
have physically retired.

The two signal threads require two actual child slots plus the real original
GraphListener and any application/executor/clock threads. Thread quota, stack,
shared memory, private child storage/TLS/tables/segment initialization, EH and
first-terminal publication belong to the actual Runtime/source profile. No
permission or budget is increased here. Neither a Guest handler nor a private
pipe can veto Root stop/reset. This source port cannot make an unsafe RMW core
thread-safe or cover upper-layer callbacks with a nonexistent RMW lease.

## Original application and default behavior

`examples/original_context.cpp` is a genuine original-rclcpp Node and
SingleThreadedExecutor program body. It leaves the original public init and
SignalHandlerOptions::All, default NodeOptions, GraphListener, ROS clock thread,
rosout, parameter services/events and type-description service intact. It waits
for actual guard/data/shutdown; no timeout-null-to-zero conversion, busy poll,
quiet options or model readiness appears here.

`run_original` prepares the actual signal profile before entering an original
application. Root's real A/B/C profile preparation must occur first and outlive
all original Context/Node/Executor/clock/logger owners. The wrapper catches an
application exception, attempts the genuine public signal uninstall, records
receiver cleanup and terminal failure independently, and never returns success
because the application happened to return zero after a platform error. The
application still owns its real context/endpoint finalization. Root must detect
and refuse leaked contexts or outstanding B/C/platform references before closing
those profiles. The helper does not guess how to finalize arbitrary application
objects or relabel leaks as native cleanup success.

This source package has no fake expected RIHS, generated type, test profile as
production default, replacement Node class or unresolved weak provider. Full
original default codec support is still separate: rosout Log, parameter service
families/events and GetTypeDescription's actual generated **C** object layout,
plus all transitively required types. A missing descriptor/provider must refuse.
No default endpoint or public API is silently removed to make this program run.

## Future actual qualification

Follow PRODUCER.md only under Root's resource-owned executor after all source
implementation cohorts close. First verify actual SDK/submodule/patch source
closure, compile/link and archive MD/imports, then separately execute the genuine
Guest contracts described in CONTRACTS.md. Finally execute the complete original
program under explicit denied/granted profiles with real native ROS peers. Keep
local source/model, actual Wasm threading, actual Host transport, native RMW and
official original application evidence distinct. No native ROS, official case,
performance or Runtime qualification is claimed by this source-only package.

## Worker reentry and options observation (r2 source successor)

The actual receiver run entry installs a private lexical thread_local owner,
and the original deferred entry installs a separate private lexical TLS owner.
Both original install/uninstall check these real worker identities BEFORE taking
install_mutex. Worker reentry refuses 17 by throwing before touching handler,
installation or join state. A callback cannot deadlock behind a main uninstall
that already owns that mutex and is joining the callback's own worker. This is
not a caller-controlled flag or a name/thread-ID guess. No self detach/join retry
is introduced. The options enum is an atomic with release stores/acquire loads;
its getter does not acquire install_mutex or race a plain enum during callbacks.

The source attachment checks the actual ADAPTED after-tree hpp/cpp hashes; the
original before hashes remain separate patch admission inputs. The fresh link
probe refuses any direct --unresolved-symbols token regardless comma/equals/
-Xlinker spelling, plus --allow-undefined/import-undefined tokens in its actual
flag/library strings. It also inspects selected configuration flags. This is
not a claim to parse arbitrary transitive imported-target or response-file
contents; full real final commands/archive/import/MD evidence remains required.
