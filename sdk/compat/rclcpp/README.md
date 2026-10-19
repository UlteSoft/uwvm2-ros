# Finite rclcpp source-compatibility lane

This opt-in source adapter implements the complete API path used by the two
actual official ROS2 Jazzy `minimal_publisher/member_function.cpp` and
`minimal_subscriber/member_function.cpp` sources pinned under upstream/. Both
original application bodies and their copyright/license remain byte-exact.
The sources use their original includes, names, Node subclasses, shared owners,
callbacks,500ms wall timer, String value, logger and init/spin/shutdown calls.
The adapter uses the actual vendored fast_io C++20 headers for bounded memory
logging and exact one-byte stdio input. The include map selects a real adapter over the existing checked C++ Wasm SDK;
it does not merely rename the source or return synthetic publication success.

This is a finite source-compatibility profile, not all rclcpp, native binary ABI,
rcl/rclc/rclpy/rcljava compatibility or a replacement RMW implementation. No
candidate has been imported, compiled or run by its author. No native ROS,
Wasm/Guest, vendor, distribution or hardware result is claimed. Java/Python/
C#/MoonBit integration remains separately coordinated with the language core.
This additive directory changes none of those cores or existing SDK headers.

## Existing gaps and concrete implemented boundary

The current roswasmcpp header expressly describes a distinct capability API.
Its Node constructor requires a backend, factories return value owners and
need aliases/capacity, Executor is explicitly caller-driven, Timer requires
explicit time, and Logger requires its own capability. C's roswasmc/session API
has no rcl_allocator_t/rcl_node_t/rclc_support_init or rclc executor mapping.
Native rclcpp headers cannot simply be included inside a Wasm application:
they require an installed RMW/native process and do not call our Guest ABI.

The new include map supplies exactly:

| Original application expression | Actual adapted operation |
| --- | --- |
| rclcpp::init(argc,argv) | Validates one trusted static Selection, ABI1 and explicit ROS2 backend; no alias opens |
| Node("minimal_...") | Selects the exact approved local Node row, not a native node identity or grant |
| create_publisher<String>("topic",10) | Looks up one exact node/topic/publish/depth row and opens its alias through real Publisher<String> |
| create_subscription<String>("topic",10,callback) | Opens/activates actual SDK subscription, fixed complete byte quota, typed full canonical decode and synchronous callback |
| create_wall_timer(500ms,callback) | Reads independently granted actual steady time, anchors the fixed period, skips missed periods without drift, calls at most once per pass |
| RCLCPP_INFO(logger,format,string) | Validates bounded one-%s/%% formatting, prints exact fragments/argument with fast_io into bounded string storage, then uses an independently granted actual typed log capability |
| spin(shared_ptr<Node>) | Holds Node lifetime, snapshots strong resource owners for each pass, respects shutdown, enforces a finite Host-paced budget |
| shutdown()/destruction | Stops local dispatch and closes every live owner once, preserving primary and independent cleanup errors |

String remains its complete data member with real existing canonical length+
UTF-8 bytes. The adapter uses the SDK's actual String encode/decode; it does not
pretend to be native generated type support or invent an MD5/RIHS identity.
Native type/transport checks remain the actual Gateway/Host's responsibility.

Only integral-millisecond wall-timer construction and String const-reference
subscription callbacks used by these sources are implemented. Generic QoS,
remapping/options, arbitrary message packages, parameter/service/action APIs,
callback groups, multi-threaded executors, native RMW handles and broad printf
logging overloads remain unsupported. Extra CLI arguments, unknown Node/topic,
duplicate bindings, depth mismatch and unknown formats fail explicitly instead
of being silently ignored or granted. Unsupported APIs are absent at compile
time. Backend import callbacks must return numeric status normally, as in the
existing SDK; they must not throw across the raw C callback boundary.

## Explicit authority and lifetime

Default selection is nullptr and fails13 before an alias can open. The concrete
selected header examples/selected_stdio_binding.hpp is a separately source-
locked compile input; it is not a Host grant or mutable environment discovery.
It selects only exact node/topic+operation+KeepLast-depth mappings and explicit
wire quotas. The actual Host policies must independently select matching String
native endpoints, native identities, history/depth, byte/time limits and module
principal. A compile-time depth table alone is not evidence of native QoS.
Unknown requested depth returns95 before open; it never reconfigures a granted
endpoint. No wildcard topic, management import or native plugin is introduced.

The example table requires these distinct actual Host roles:

- minimal_publisher: topic publish alias compat-publish on the separately
  admitted /uwvm_rclcpp_compat/guest_to_native endpoint, KeepLast10, complete
  String1028-byte maximum; steady-time alias compat-steady; INFO logger alias
  compat-pub-log.
- minimal_subscriber: topic subscribe alias compat-subscribe on separately
  admitted /uwvm_rclcpp_compat/native_to_guest, same complete native type/depth
  and byte limit; INFO logger alias compat-sub-log. It reads no clock.

The source table grants none of them. An actual default-denied Host run must
use the SAME selected source while withholding Host aliases and record real
errno13/no native entity. The local nullptr-selection refusal is a separate
source-admission observation, not a native default-denial witness.

Configuration and backend storage must be immutable and outlive every owner.
Each Core contains a stable roswasmc_context, and each Publisher/Subscription
retains that Core while the SDK borrows its context address. No temporary
context or moved live owner is passed to the SDK. Node closure retires external
shared resources too; later use is stale9. Weak registries avoid cycles. A spin
holds a strong Node copy and a per-pass strong resource snapshot, so dropping an
application pointer or replacing a callback cannot destroy the active object.
Creation during dispatch refuses17 before opening; SDK subscription polling
leases retain existing nested-poll/close protections. There is one Guest-thread
executor and no implied native thread or guest management concurrency.

No ROS operation is retried on95/107/110 and no consumed event is replayed.
A finite spin budget exhaustion is110, not a fake normal shutdown. The selected
step function uses only separately admitted basic stdin pacing: R releases one
pass, Q asks local shutdown, EOF110/unknown byte22 fails. Stdout/stderr cannot
update/debug the Runtime. Steady clock and log aliases remain separate even
when basic WASI stdio is permitted.

The optional C observation functions expose last_error and cleanup_error for an
owned runner. They confer no ROS right. Normal spin cleanup failure raises an
error too. With exceptions enabled, RCLError carries the actual code and RAII
cleans on unwind. A no-exception Wasm profile aborts on uncaught errors; the
external owner must inspect error/Host audit evidence and prove module/Host
resource drain. A successful cleanup cannot be inferred from a killed Guest.

## Source-only compilation and actual test recipe

Root must first review/freeze a coherent full source, freshly admit current
Boot4 tools/SDK/source and stage a fresh owned output. Every command below is
UNEXECUTED. Run serially inside the unchanged actual64GiB/zero-swap cgroup:
compiler<=2GiB/600s, model/value executable<=512MiB/30s, total bounded aggregate
and real8GiB backing reserve plus declared output growth. The original owner
must retain fullMD/link/tool/std-library/full Wasm-validator data, same-birth
pidfd cleanup and normal Frame.finish. fast_io is a real additional transitive source dependency: admit its complete
vendor root/headers and actual same-SDK allocator/libc++/WASI imports in the
whole source/MD/link evidence. C++20 was already required by the existing SDK;
no global library/allocator or ROS permission is changed. This source-only
choice is not proof that the selected WASI compiler has built it. No new actor can overlap another owner
without explicit current physical/memory commitments.

Pure host API/model build (both original sources are unchanged on disk):

```sh
"$CXX" -std=c++20 -O1 -UNDEBUG -MD -MF "$OUT/pub.d" \
  -I "$SOURCE/sdk/compat/rclcpp/include" -I "$SOURCE/sdk/include" \
  -I "$SOURCE/third-parties/fast_io/include" \
  -I "$SOURCE/sdk/compat/rclcpp/tests" \
  -DUWVM_RCLCPP_COMPAT_SELECTION_HEADER='"bridge.hpp"' \
  -Dmain=official_pub_main -c "$SOURCE/sdk/compat/rclcpp/upstream/rclcpp/topics/minimal_publisher/member_function.cpp" -o "$OUT/pub.o"
"$CXX" -std=c++20 -O1 -UNDEBUG -MD -MF "$OUT/sub.d" \
  -I "$SOURCE/sdk/compat/rclcpp/include" -I "$SOURCE/sdk/include" \
  -I "$SOURCE/third-parties/fast_io/include" \
  -I "$SOURCE/sdk/compat/rclcpp/tests" \
  -DUWVM_RCLCPP_COMPAT_SELECTION_HEADER='"bridge.hpp"' \
  -Dmain=official_sub_main -c "$SOURCE/sdk/compat/rclcpp/upstream/rclcpp/topics/minimal_subscriber/member_function.cpp" -o "$OUT/sub.o"
"$CXX" -std=c++20 -O1 -UNDEBUG -MD -MF "$OUT/model.d" \
  -I "$SOURCE/sdk/compat/rclcpp/include" -I "$SOURCE/sdk/include" \
  -I "$SOURCE/third-parties/fast_io/include" \
  -I "$SOURCE/sdk/compat/rclcpp/tests" \
  -DUWVM_RCLCPP_COMPAT_SELECTION_HEADER='"bridge.hpp"' \
  "$SOURCE/sdk/compat/rclcpp/tests/official_source_contract.cc" "$OUT/pub.o" "$OUT/sub.o" -Wl,-t -o "$OUT/model"
"$OUT/model"
```

Only the two main LINK SYMBOLS are renamed for this single host model driver.
The source/API bodies remain original. The model supplies a finite fake raw
backend and explicitly earns ZERO native ROS/hardware/official credit. It
checks the actual SDK wrappers and complete original publisher→canonical bytes→
subscriber callback/log flow, real returned13/95/110, no replay, malformed data,
clock/logger failure, stale resource lifetime and phase-anchored timer behavior.
The r2 I/O change preserves the same format grammar, exact bytes and4095-byte
ceiling; no printf varargs/parser or truncation remains. Logger output uses
fast_io::ostring_ref_std over a once-reserved std::string after complete size
admission. Each step uses read_some(posix_stdin(),&byte,&byte+1), never token
scanning/whitespace skipping or libc read-ahead. EOF remains110 and only R/Q
have meanings. Actual OS/WASI I/O failure still fails the owned run; it never
opens another alias or retries a ROS effect.
It does not replace the required real imported-Wasm/native exchange below.

Actual Wasm recipes compile each original source as its own ordinary main,
without main renaming and with the REAL imported ROS2 backend selector:

```sh
"$WASI_CXX" --target=wasm32-wasi --sysroot="$ACTUAL_WASI_SYSROOT" \
  -std=c++20 -O1 -fno-exceptions -fno-rtti -MD -MF "$OUT/publisher.d" \
  -I "$SOURCE/sdk/compat/rclcpp/include" -I "$SOURCE/sdk/include" \
  -I "$SOURCE/third-parties/fast_io/include" \
  -I "$SOURCE/sdk/compat/rclcpp/examples" \
  -DUWVM_RCLCPP_COMPAT_SELECTION_HEADER='"selected_stdio_binding.hpp"' \
  "$SOURCE/sdk/compat/rclcpp/upstream/rclcpp/topics/minimal_publisher/member_function.cpp" \
  -Wl,-t -o "$OUT/publisher.wasm"
```

Use the same real selected SDK/flags for the original subscriber, then full
validate both artifacts with the actual admitted wasm-tools and inspect all
imports. Do not fabricate a target/sysroot/archive pair, native identity or new
rights when an import is unavailable. The selected source instantiates the real
v1 SDK backend; raw ROS imports remain business-only, never update/debug/policy.
Basic libc/WASI startup/stdin/termination rights need their own explicit policy.

Build native/peer.cc using its actual installed ROS2 SDK CMake recipe, with NO
Guest compatibility include root. This is a REAL rclcpp/String peer; it creates
one test endpoint only after I and waits for a matching peer after M. P allows
one native publish or one native subscription result; F allows checked close.
The owner must hold its initial SIGSTOP and inspect actual loaded libraries,
strict fullMD, source/type/role data and real cgroup membership independently.
Root must admit the actual generated/native endpoints and controls; a copied
old product or generic JSON true is never native proof.

Required future actual two directions, each in a fresh module/Host session:

1. Real native publisher→unchanged original Wasm subscriber: establish both
   endpoint identities and explicit grants; hold P until the actual Guest has
   activated its subscription. Observe complete `I heard: 'Hello, world! 0'`
   through the independently granted log path, then Q and F; record resource
   closure/zero residue.
2. Unchanged original Wasm publisher→real native subscriber: establish native
   subscribe and hold actual initial clock/time; release a Guest R only after
   actual native match. Permit one500ms timer callback, require one complete
   native String `Hello, world! 0`, then Q and F; close/inspect all owners.

Default-denied and wrong-binding/quota/cancellation paths need separate real
runs without treating denial as functional publication. These source recipes
still require an independently reviewed unified Root execution owner and actual
results; no native-delivery or compatibility pass is predeclared here.

## Version/source/license evidence and remaining compatibility

source-acquisition.json retains the actual observed Jazzy examples commit,
actual commit.tree and seven raw Git-blob verified sources including Apache2
LICENSE. Their original CMake/package declarations are retained as evidence,
not used to link native rclcpp into Wasm. The actual rcl timer source/commit/tree
is separately retained under research/ to justify phase-preserving missed-tick
semantics; this research is not a materialized SDK/full-tree qualification.

Remaining concrete work includes full rcl/rclc allocator/error/waitset/executor
surfaces, broad rclcpp message/options/QoS/services/actions/parameters/lifecycle,
all supported ROS-version semantics, rclpy/rcljava source/runtime bridges owned
by the other language collaboration, and real cross-vendor/native/official
performance testing. This first two-source closed loop cannot be relabelled as
complete compatibility with any of those products.

A failed logger or steady-clock open can leave a C++ wrapper object without an
acquired capability. Cleanup checks that wrapper's actual `is_open()` state
before calling close, then destroys the wrapper. This preserves the original
open error and avoids inventing a stale-handle cleanup error for an operation
that never acquired a handle. Real acquired-owner close failures still remain
separate cleanup errors. The existing denied-logger host-model scenario checks
that distinction; it has not yet been executed.
