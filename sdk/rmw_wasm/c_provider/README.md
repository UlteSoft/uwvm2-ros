# Real upstream RMW cohort C: owned delivery, guards, finite wait and services

This is source for the real upstream `rmw` ABI, with the original selected
`rmw`, `rcl`, `rosidl`, `rcutils`, `std_msgs`, and `example_interfaces` headers
and actual generated C objects. It is separate from the finite
`sdk/compat/rclc` source facade. Do not put the facade's replacement include tree
on this build's search path. Nothing in this package has been imported,
compiled, linked, executed, or qualified against ROS. The 35 model groups are
authored source. A model's fake Host is labelled explicitly and is never native
metadata, native discovery, Runtime policy, or ROS delivery evidence.

## Integration and ownership

C owns `src/{guard,wait,subscription,service,metadata,add_two_ints_codec}.c`,
`src/c_internal.h`, and `include/uwvm_rmw_c/provider.h`, with this independent
recipe, model source and real witness. A owns the existing platform and CMake
entry; B owns its four bridge headers and context/node/publisher/selection/String
codec implementation. The source-selected boundary is B revision7. Its actual held headers and
QoS comparison source are preserved under package `references/current-boundary/`.
The original B revision3/4/5 references remain unmodified historical inputs. Their native-retire and context-child signatures are real
source dependencies, not a forecast of a qualified implementation.

`rmw_uwvm` is the actual target at `sdk/rmw_wasm/cmake/CMakeLists.txt`. It already
requires the five core C sources by name. The example-specific AddTwoInts adapter
is built separately by `c_provider/CMakeLists.txt`; the core does not acquire an
implicit dependency on a tutorial message package. The standalone project links
the actual exported `rmw_uwvm::rmw_uwvm` target and original generated C,
introspection and type-support targets. `recipe.json` spells out the concrete
commands with actual build/SDK paths supplied later. These commands have not
been executed. A complete original static WASI SDK and the selected original
modern service introspection/hash/Description/Event headers are mandatory.
Missing files/libraries must fail; there is no stand-in native header or stub.

B admits actual public addresses before dereferencing private state. C uses those
admission functions and one real context scope per RMW operation. A wait or take
returns and releases that scope before original upper-layer rcl/rclcpp code
invokes its callback. C does not impose a fictitious whole-upstream callback
lease or transfer a finite SDK facade's callback policy to genuine upstream ROS.
The first implementation targets a single-threaded WASI instance. Registry
mutation and teardown are not advertised as an implementation of concurrent
RMW calls or multithreaded executors.

A guard is a real Guest-local atomic binary notification object. B's private
graph hook calls private guard construction under B's existing scope; it does
not recursively call the public guard factory. Guard/wait allocation retains a
real B context-child reference before exposing the handle, and releases it after
retiring the local registry. `context_fini` refuses while any child remains.
Guard notification may wake an already retained guard during an actual wait;
it cannot create/retire owners or invoke user callbacks. A local graph wake is
not permission for a DDS graph or evidence of a matched endpoint.

## Real endpoint and native-fact seam

`uwvm_rmw_c_install_profile` is one-time explicit bootstrap before the first C
object, including B's graph guard. The profile stays immutable; provider state
has a stable actual owner until the last context/object retires. A missing
profile installs no callback, alias, clock, wait driver, thread, or permission.

New D activation is the sole native endpoint construction path. Source owner
open authorizes a logical capability; D `activate` must construct the actual
typed endpoint and return its real binding, native incarnation and actual QoS.
C does not first call the old `subscribe`, `service_advertise`, `event_watch`,
`service_submit`, or `service_respond`, which could activate a different legacy
object or queue. The old 81 imports and legacy queues keep their own semantics.
Missing activation/native-delivery/request providers make endpoint construction
return raw95 before its first owner open. This package does not supply a fake
D provider and does not claim Root's partly implemented optional transport is
ready or installed.

Every activation receipt has backend/source/Runtime instance/generation/session/
native incarnation. Topic QoS has one actual getter result; service QoS has two
actual results for request and response. C compares both complete service QoS
results to the single declared effective profile in the current B selection.
A deployment requiring different request/response effective profiles is therefore
rejected until an independently specified selection extension exists; C never
silently treats one getter as both. Concrete enum values and duration extents
are checked. Each inspect must return the same binding/incarnation/GID and
actual QoS, with no reopen or retry. Native retirement is reserved before effects
and B invokes it exactly once before raw owner close, including shutdown. Raw
and local cleanup still run after a native-retire error; B records all outcomes.
Host instance drain is still required for traps or unknown native effects.

The native delivery provider must acquire complete canonical body and native
metadata from the SAME consumed D URMI record and validate the complete record,
source, generation, native incarnation, session and sequence. It may reconstruct
an SDK UWRE header64 plus body only from that same original record and must
preserve error, retired and external-effects flags. Native presence flags and
control flags are distinct. Native missing-field/width refusal95 must occur
before consumption. Ordinary poll followed by a guessed timestamp/GID or an
unrelated metadata lookup is invalid. Full original native record/extent and
bindings remain independently owned transport evidence. No last-message cache,
second metadata queue, argv, endpoint declaration, source handle or local counter
can manufacture native proof.

This C contract uses the selected original Guest `RMW_GID_STORAGE_SIZE` (16 in
the pinned SDK). All transport native extents must be preserved. An old native
24-byte GID may not be truncated into Guest16 or represented by a process-local
counter. Incoming native request IDs of incompatible extent require95 before
consumption; a consistent domain-wide mapping needs its own specification. The
optional client native GID receipt keeps its complete extent up to32. Public
`rmw_get_gid_for_client` succeeds only for an actual inspected complete matching
extent; absence/different width yields95 and preserves output. A native GID is
never inferred from the opaque Gateway request ticket.

## Owned data and actual RMW service behavior

Subscription readiness copies one complete canonical value and full native
message info into already owned storage. Take performs the complete selected
transactional generated-object decoder before returning the caller's original
native object. Strings/sequences are actual original generated storage; no
native struct memcpy or queue-borrowed pointer escapes. `take_with_info` does
not return invented publisher/time facts. Actual publication/reception numbers,
or the original explicit unsupported sentinel where the native protocol says
so, are preserved. Delivery traverses Host IPC, so Guest intra-process is false.
A complete consumed malformed record becomes a terminal error and is not replayed.

Each client reserves a pending slot and complete request/response buffers before
native admission. It encodes into its own bytes before returning from
`rmw_send_request`. This RMW assigns its own positive signed int64 request
sequence and local writer identity; an independent full-u64 Gateway ticket maps
the response back to that RMW sequence. The actual native writer/sequence/time
from the SAME URMI response remains separate in diagnostics. When an actual
matching-width global native client GID is supplied, it can also be this RMW's
writer identity; otherwise its process-local writer is not advertised as global.
IDs do not wrap. Slot exhaustion and sequence exhaustion happen before submit.
A zero/duplicate output ticket or ambiguous admission poisons the client,
retains the failure and prevents automatic replay. Explicit submit11 is no
admission credit. Submit success is admission, never completion.

Response take checks the full event/backend/source/instance/session/contiguous
queue sequence and complete codec payload. Unknown tickets or malformed records
fail. The upstream response header returns the saved local sequence; the raw
native request identity remains separately available in
`uwvm_rmw_c_client_diagnostics`. These identities are not labelled equivalent.
Resource error/closed state retains its original reason and flags; an
external-effects-possible indication never grants a retry.

A server requires the actual incoming native request writer GUID, native
sequence and source/receive timestamps. Before consuming it, C has a pending
slot and owned request storage. The full native `rmw_request_id_t` maps to an
independent opaque Gateway ticket; ticket alone is not a native header. Duplicate
native IDs/tickets are rejected. After complete decode, original upper code
can retain its actual shared request/header objects beyond the take call. A
response re-encodes into owned bytes, finds the exact header and marks its one
attempt BEFORE the real D respond call. No success, timeout or cleanup error
makes that native request replayable. Complete source and response authorities
remain distinct owners. Source-wide shutdown/retirement is not a fabricated
per-request native cancellation.

The concrete AddTwoInts adapter uses only original generated
`example_interfaces__srv__AddTwoInts_{Request,Response}` and the complete modern
Service type support. Three mandatory RIHS identities must come from actual
independent selected SDK exports for deployment. It checks actual message
introspection fields/offsets/native capacities, exact full8/16-byte logical
extents, original Request/Response/Event members and modern hash/Description
support. No codec ID, native ID or native hash is invented/recomputed. A model
may obtain its self-selected actual linked SDK hashes for a logical fixture;
that is explicitly not independent deployment qualification. Trigger/SetBool or
arbitrary custom registries require their own complete actual adapters/selection;
they are not claimed by this one concrete adapter.

## Finite wait and limits

A wait set admits the whole same-context, duplicate-free non-NULL entity list
before observing a queue. Readiness may prefetch a record into owned storage;
it does not run application callbacks. A zero-duration wait makes an actual
nonblocking observation without a clock grant. Positive waits require a real
selected finite wait provider and A's prepared steady-clock authority, retained
through the wait. Actual clock reads before/after driver steps determine elapsed
nanoseconds. Clock rollback, failed driver, overflow, or exhausted iteration
budget is an error; a poll count or stdio credit never becomes elapsed time.
Each wait slice needs an independent real enforcement deadline in its owner.
The external Root must enforce the complete process/tree deadline as well.

Null/indefinite wait is unsupported95 rather than a secretly shortened timeout.
No driver is selected by default. A successful readiness pass or actual measured
timeout updates caller arrays and consumes only returned ready guard triggers.
A failure keeps the caller arrays unchanged. Max-conditions0 admits all conditions
available within this implementation's actual finite resources (256 guards plus
256 endpoint resources per context), with no smaller caller-provided capacity.
Missing QoS events, graph counts, availability, unique-flow/CFT/ignore-local,
loaned/serialized/dynamic types, Actions and other original RMW APIs remain
explicit required work in `required-work.json`. A small Topic/Service witness
is not full RMW, rclcpp/rclc, vendor, language or distribution completion.

## Future validation

`recipe.json` contains35 isolated model commands (the original27 plus8 boundary
regressions). None has run. Models link actual
A/B/C and original generated SDK objects, with an explicit fake transport. They
cover complete codec extents, pre-effects denial, guard/wait/context lifetime,
real measured model-clock transitions, owned prefetch, full native/local ID
separation, ticket/sequence/closed errors, pending capacity, QoS drift, once-only
reply and native-before-raw teardown. Their source names do not certify results.

`examples/checked_service.c` is a concrete finite real-RMW integration witness.
It requires a separately authored and reviewed
`uwvm_rmw_c_selected_bootstrap` actual deployment object. There is no weak stub.
It sends10+32, checks the whole received request/native header, replies42 once,
checks the matching response/local sequence/native diagnostic, and attempts all
ordinary cleanup before printing its scoped marker. Actual SDK/source/MD,
protocol permissions, closed peer, normal process retirement and full raw reports
are required independently. No generated/native identity, future source manifest,
Runtime success, WASI import grant or ROS delivery is forecast here.

## Three-finding successor and exact boundaries

The original14-file source HOLD and its complete independent Root peer remain
preserved under `before-r1/`. This successor changes only the three confirmed
correctness findings, their explicit contract comments, build gate, models and
English recipe. It does not execute either source version or revise historical
results. No D transport, WASI wait driver, native endpoint, identity or permission
is synthesized here. The unchanged six old model transport helpers remain fake.

A successful actual metadata provider attests the complete same-record presence,
selected full extent and source/incarnation/sequence association. A genuine GID
or native writer whose bytes are all zero remains data. Endpoint inspection still
requires exact same GID extent and all bytes, including the zero tail. A response
still requires its explicit `info_present == 1`; absent metadata is not inferred
from content. The four zero-value regression paths cover endpoint global GID,
Topic publisher GID, incoming native request writer and native response writer.
The global-GID case also changes one inspected byte and requires refusal with
caller output preserved; a separate response case requires refusal for absent
explicit presence. None labels a fake model receipt as actual native evidence.

Original RMW time permits non-normalized nanoseconds. Wait compares the exact
algebraic total `sec * 1000000000 + nsec`, checking the signed-monotonic bound
before multiplication/addition. Equal representations receive identical finite
behavior. A valid duration too large for this bounded implementation returns
unsupported95 before observing queues or borrowing clock authority; it is not
mislabelled invalid22, wrapped, saturated or silently shortened. A missing real
wait driver remains95 even for a legal non-normalized duration. Zero timeout
still makes exactly the existing nonblocking observation without a clock grant.
The model compares `{1,7}` with `{0,1000000007}`, preserves guard readiness on
all unsupported signed/infinite/overflow extents, and checks zero actual clock
reads and wait steps in those refusals. B revision7 separately supplies original
linked `rmw_time_equal` for complete QoS duration comparisons; this cohort does
not replace B source or invent the actual linked SDK library.

All model operations remain active in Release. CMake undefines `NDEBUG` for the
model target and `tests/contract.c` rejects a remaining `NDEBUG` at preprocessing.
The author has not invoked CMake, the preprocessor, a compiler, a model or any
other candidate command. The source gates are requirements for the future actual
validation, not a claimed successful negative or Release build.
