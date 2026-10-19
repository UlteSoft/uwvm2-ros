# Explicit Guest C subscription transport

This is a source-only candidate over the genuine current A/B/C RMW headers and
the independently authored Root raw7 subscription ABI. It has not been imported,
generated, configured, compiled, linked, probed, model-tested, run as Wasm/native
ROS, installed, or sent over SSH. No repository file is changed by its author.
Root owns later Host/Gateway/Runtime/CLI wiring and actual execution. No source
receipt is a native SDK qualification or permission. Default ROS rights are empty.

## Real sources and one native endpoint

The production C adapter exports `uwvm_rmw_c_activation_provider` and
`uwvm_rmw_c_native_metadata_provider` callbacks for **subscription only**. Its
original public types come from the selected real RMW/rcutils/rosidl SDK; no
native struct, generated header, TypeSupport, fake `rmw` function, or namespace
facade is supplied. The actual codec's complete native graph validator runs
before the raw activation. Generic generated C and C++ descriptors are equally
valid source-selected callbacks; this transport never casts a generated object.

The original C consumer has already selected the exact node/topic/type/hash/
requested-QoS row and opened the corresponding actual granted source. Its source
wrapper and context must stay alive/unmoved under B's real function-scope lease.
Root then authenticates the current full source/session/instance/true generation,
policy/native schema/codec/QoS and owns the retained original Subscription. The
UMRE receipt is anti-mixup DATA, not an authorization token or source of a new
permission. An alias string cannot choose another topic or grant access.

Activation, inspection and retirement use the SAME actual retained incarnation.
No temporary subscriber or graph name attests another object. Receipt kind must
be subscription2, one complete concrete actual QoS profile, no publisher GID,
all reserved/tail bytes zero, exact full64 source and nonzero instance/generation/
session/incarnation. Inspection compares the complete original wire except the
command byte, plus native QoS using B's genuine `rmw_time_equal` comparison.
Requested QoS remains separate; original C compares the returned actual QoS with
its explicitly selected effective row. No SYSTEM_DEFAULT resolution is invented.

A subscription receipt intentionally has no publisher GID. Only the real received
record supplies that actual native value. Native all-zero value bytes remain
legal when their independently checked extent and presence are valid.

## The exact optional raw ABI

The only production numeric functions are the actual Root header's
`uwvm_ros2_rmw_sub_v1` imports:

- `abi_version() -> i32`
- `subscription_activate(source64, memory32, receipt_offset64) -> i32`
- `subscription_inspect(source64, incarnation64, memory32, receipt_offset64) -> i32`
- `subscription_peek(source64, incarnation64, gid_width32, memory32, packet_offset64, capacity64) -> i32`
- `subscription_commit(source64, incarnation64, sequence64, reservation64) -> i32`
- `subscription_cancel(source64, incarnation64, sequence64, reservation64) -> i32`
- `subscription_retire(source64, incarnation64) -> i32`

There is no length-output pointer on peek. Its complete UMRS header provides the
bounded record extent; Root's successful Host copy publishes the entire packet
and then returns0, with no second output write/fallible post-copy operation.
Pointers select actual memory0 with zero-extended `uintptr_t` offsets. Widths
remain64-bit for both Wasm32 and Wasm64. Missing imports are real link/import
failures; no weak definition, old81 listener or fallback poll is supplied.

All ordinary source opens/closes still belong to genuine B/C ownership. This
transport does not open another capability, append a grant, read an environment
variable, create a thread, sleep, acquire a clock, wait on a fictional set or
invoke an application callback. It adds no debugger/update/policy interface.

## Preallocation and lifetime

`create(slots, maximum_body, allocator, &state)` allocates every permanent source
slot/diagnostic/receipt and the shared private packet scratch before effects.
The maximum body must fit the original4MiB IPC ceiling **minus all288 header
bytes**, not merely the B codec limit. Count is1..8*256, with overflow-checked
allocation. Actual caller memory/process budgets must admit the concrete sizeof
layout and chosen counts; this source does not report measured RSS.

One atomic nonwaiting entry flag protects shared scratch and fixed diagnostics.
Recursive use returns17. This does not implement multithreaded RMW, upper user
callback policy, or an artificial whole-Context callback lease. B/C's actual
scope and normal C public pointer/object lifetime contracts remain mandatory.

Each actual full source receives one permanent slot before even the ABI query.
Failed attempts are not recycled or retried as fresh incarnations. The complete
original activation and last inspect wires stay separately inspectable. No
allocation or container growth occurs in production callbacks after bootstrap.
Logical zero-body codecs remain logical0, including a NULL caller body pointer
with capacity0; allocation sizes are never exposed as a message length.

`callbacks` only copies two vtables. It installs nothing. The current original
C `install_profile` is an immutable **process singleton with no uninstall**.
Once callbacks have been exported, this state/profile/codec/allocator must have
process lifetime; `dispose_unpublished` refuses17. Only a never-exported bootstrap
can be freed locally. There is no fictional uninstallation or dangling callback
cleanup. An embedding application must own equivalent stable process storage.

Only the provider's non-subscription activation branch promises95 before its
own raw/native calls. Current C service/client factories open owners before
activation callbacks; this source does not rewrite that core or falsely claim
zero total opens for arbitrary misuse. The complete example's B selection has
ONLY its one subscription row, so unrelated endpoints fail genuine selection13
before their opens. A future multi-kind provider needs explicit real dispatch
and complete selected rows, not implicit service/client success.

## One record, explicit commit, unchanged failure outputs

`take_topic` first validates exact stable source/context/receipt association,
actual original metadata ABI width, complete caller capacity and nonoverlapping
caller output storage. Incompatible native width returns95 before borrowing;
a short output returns90 before peek. It reinspects the exact original native
receipt, then raw-peeks into the preallocated PRIVATE packet only.

UMRS96 must have its exact magic/version/command/backend/reserved bytes, complete
binding/current incarnation, admitted capacity, positive accepted sequence and a
separate positive reservation nonce. The record must fit both288+selected codec
bound and the complete packet. Accepted sequence must exceed the last committed
sequence; a new borrow token must exceed the last observed nonce. These are
stream/attempt anti-replay facts, not credentials or native publication numbers.

URMI192 must describe a topic message from the exact same source/incarnation/
sequence, with exact body extent, complete source/receive time and publisher GID
presence, exact native GID width, and valid zero tails/reserved/control fields.
Service request IDs, correlations and control errors cannot masquerade as a
topic. The full native signed timestamp bits are copied without narrowing.
Optional publication/reception sequence numbers come only from their real
presence bits. An absent field requires the actual original
`RMW_MESSAGE_INFO_SEQUENCE_NUMBER_UNSUPPORTED` sentinel; a supported field may
not use that reserved value. Neither field is guessed from the queue ordinal.
All-zero GID/time values do not imply absence.

The actual original `rmw_message_info_t` receives the real publisher GID bytes
under this Guest RMW's implementation identifier. `from_intra_process=false`
describes this genuine Host IPC receive path; it does not assert that the
original native publisher used or did not use native intra-process delivery.
The original C consumer repeats its correct Guest-local implementation marker.

Root's typed actual native producer proves the complete canonical payload and
source codec before enqueue. This transport validates its complete associated
wire and selected canonical extent; it does NOT deserialize a C++/C object into
an invented layout. The original C consumer performs the actual transactional
generated-object codec decode afterward. A post-consumption native-object
allocation/decode failure is real: C poisons the local source and does not replay
or claim the record was unconsumed. Generic codec bounds are not a new body
projection or an exception to complete typed decoding.

Before C canonical/length/info publication, the provider sends exactly ONE raw
commit for the exact accepted sequence/nonce. Only a successful genuine ACK
permits the prepared canonical memcpy and the two local result assignments.
There is no subsequent fallible import or allocation. Caller buffers are valid,
exclusive ordinary C objects held through the operation; C's actual subscriber
uses its independently allocated private canonical buffer and stack outputs.
This is the provider's function-level publication contract, not a claim of a
hardware-atomic multi-address store against concurrent invalid C aliases.

On validation failure with a trustworthy returned borrow header, cancel that
exact attempt once; preserve primary and actual cancel errors independently.
If the header cannot establish a trustworthy token, do not guess one: poison
and retire the known source instead. If cancellation fails, poison and attempt
retirement once. On **any commit error**, consumption may be ambiguous: never
retry it, never issue cancel as fictional rollback, never publish the prepared
body/info/length. Poison and attempt native retirement once, preserving both
results. Root's actual Host session/instance drain covers unknown effects.

A later C cleanup call sees that the once-native-retire was already attempted
and receives its saved actual result; it does not issue another native call or
replace a failed commit/cancel with a fabricated cleanup success. B still closes
the real raw owner once, regardless of native-retire error. Independent B owner
close diagnostics are authoritative for that attempted close. An optional joint
report attachment is DATA ONLY and cannot perform or prove a close; retain the
original B report as well. Fixed slots preserve first primary plus most recent
peek validation/cancel/commit/retire/rawclose outcomes separately, with counts.
Root must retain the complete external operation audit for every attempt.

Idle11 and metadata95/capacity90 guarantee no consumption under the real raw
contract and preserve C outputs. Unknown transport failures poison/retire, not
fallback. Native exceptions/traps or a blocked native finalizer still require
Root's genuine finite process/cgroup/session drain. No OOM/trap immunity or
network exactly-once guarantee is claimed.

## Actual SDK field gate and authored contracts

The production default metadata feature is0. CMake emits a feature only after
compiling/linking against actual original headers with exact signed-int64 fields and8-byte times,
uint64 publication/reception fields, exact1..32-byte GID storage, original
UINT64_MAX unsupported sentinel and the real intra-process member. Missing old
SDK fields refuse95; no fake header, synthetic struct or distro label enables
it. A compiled layout check is not actual RMW population/behavior qualification.

`tests/model_contract.cc` uses the real complete CPP10 Registry descriptors and
actual generated String/Empty layouts/TypeSupport, real original RMW metadata
and allocator. Its seven raw7 function definitions are explicit LOCAL PROTOCOL
MODELS linked only into the model executable. They are never part of production,
never fake native ROS functions and never proof of Host behavior or ROS rights.
Fifteen authored groups cover complete precommit metadata and actual generated-object
String/Empty decode after commit, native optional sentinels,
idle/short/wrong owner, inspect drift, malformed metadata, incompatible width,
malformed borrowing, ambiguous commit/no replay, three distinct failures,
independent close-report data, wrong kind refusal, logical0 and bootstrap disposal.
All are unexecuted. Model process allocations stay alive because exported
callbacks could still be borrowed; process retirement owns their local memory.

`examples/checked_subscription.c` supplies a COMPLETE standalone original-RMW C
bootstrap, explicit one-node/type/topic/QoS/alias selection, actual independent
RIHS input and actual local graph provider. No service/client/publisher row is
added. It installs C's singleton once, creates the real subscription, makes at
most4096 genuine zero-wait observations, compares seven bytes including NUL,
prints actual metadata, and attempts every native/raw/local/graph cleanup. The
poll count is not elapsed time. Positive waits need a genuine separately selected
clock/driver; this example adds neither. Default13 is a required negative result,
not an accepted positive route.

`tests/native_publisher.cc` is separate original native rclcpp source. It retains
real NodeOptions defaults, creates a real String publisher, coordinates finite
native discovery for the fixture only, prints actual publisher GID and performs
100 distinct publications. It links no Guest RMW or local raw model. Root must
qualify its real SDK, metadata behavior, native entities and whole cleanup.

`recipe.json` is unexecuted DATA with explicit real input roles. CMake options
are OFF by default and no `add_test`/`try_run` executes anything. Native and Guest
builds are separate actual toolchains. Later Root acceptance requires real
Host7/catalog/routing, exact actual source/SDK/MD/link/import/ELF closure, finite
serial ownership, default13 without grant, complete native positive same-record
comparison, wrong-width/no-borrow, injected transaction/ACK failures, process
retirement and fresh coherent source generation. Existing code, Source64 results,
old81 and other provider cohorts are not relabelled as these unrun results.
