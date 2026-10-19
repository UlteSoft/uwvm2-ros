# Process-lifetime Service and Action identifiers

These are source-ready regressions, not an execution receipt. No candidate was
compiled, imported, or run while authoring them. Root must compile and execute
them on Linux inside the original 64 GiB cgroup with swap disabled, alongside
the separately reviewed current-source Gateway rebuilds. An old Source39,
Source40, or Source43 executable cannot qualify these new header bytes.

## Correctness change

The old Service, ROS1 builtin Action, ROS1 custom Action, and ROS2 custom Action
allocators retained every randomly generated identifier in an ever-growing
`std::set`. They returned zero after 1,048,576 allocations. Successful completion,
expiry, endpoint closure, and refusal by an event queue did not remove those
entries. A process could therefore lose all future Service or Action operation
in that domain while no live request or goal remained.

`gateways/common/nonreusing_ids.h` now implements a checked, constant-space
ordering of every nonzero `uint64_t`. The existing kernel-backed
`ipc::random_session` selects its first value once. Each next value advances
through the unsigned domain, explicitly transitioning from the maximum to one.
After it returns the predecessor of its first value, all nonzero values have
been issued exactly once. The next and every later allocation returns zero.
No completion, queue refusal, close, or restart of an endpoint resets that
process-lifetime allocator. An initial entropy failure returns zero without
initialization, permitting a later attempt to obtain real entropy. Exhaustion
does not reseed.

Each existing identifier domain has one synchronized allocator. All Service
states share one domain; the builtin ROS1 Action, all selected custom ROS1
Actions, and all selected custom ROS2 Actions each retain their original
separate domain. There is no requirement that unrelated domains have different
integers. Calls within a domain, including calls by different sessions or types,
cannot repeat an old integer. The builtin ROS2 Action allocator already had a
checked fail-closed counter and is unchanged.

The value is a correlation, not bearer authority. The actual server first
authenticates the configured Worker using peer credentials and its live pidfd;
`session.bind` constructs a separate session-owned adapter. Service responses
still require matching resource/type/identity and that owner's live pending
entry. Action responses and own-goal cancellation still require the owner's
live goal, matching resource/type, and the separately granted operation.
Cancel-any remains its separate explicit permission. Predicting an integer
does not satisfy these checks. ROS1's native GoalID process nonce is unchanged.
No Guest permission, CLI default, event wire layout, deadline, quota, or native
cleanup transition changes in this patch.

## Regression sources

`contract.cc` needs Linux and a C++17 compiler; it does not require an installed
ROS SDK. It directly includes the maintained shared Service implementation.
Its seven groups check:

1. All 255 nonzero `uint8_t` seeds each traverse all 255 values once, followed
   by repeated zero; a maximum `uint64_t` seed crosses to one without premature
   exhaustion, and zero cannot seed a live sequence.
2. An initial entropy failure, a successful single initialization, and more
   than the former cap across an actual maximum-to-one transition. A bounded
   sorted oracle detects reuse; the production allocator keeps no oracle.
3. Eight concurrent callers produce 131,072 unique values with exactly one
   entropy initialization.
4. 1,048,608 real Service begin/event-poll/response/native-delivery cycles,
   alternating ROS1 and ROS2 capability forms. Every response is exact and
   pending state is empty after every successful delivery.
5. 1,048,608 real queue-rejected begins followed by a successful request on an
   independent healthy Service. Failed enqueue must not exhaust that domain.
6. Separate owners of the same resource cannot answer one another's requests;
   a foreign resource and stale completed, expired, or closed tickets are
   denied. Endpoint recreation receives a fresh ticket. The expiry check
   moves the genuine pending deadline under its actual mutex instead of using
   timing-sensitive sleeps.
7. Eight actual Service owners of the same resource complete 8,192 concurrent
   requests with unique IDs and empty final pending maps.

`action_ros1.cc` includes both actual ROS1 Action allocation wrappers and calls
each domain for 1,048,608 sequential and 131,072 concurrent allocations. It
requires the selected real ROS1 SDK, but creates no ROS Node, Master, or goal.
`action_ros2.cc` does the same for the actual ROS2 custom Action wrapper. A real
Action-capable ROS2 SDK is mandatory: missing Action support is a compile error,
never a skipped successful test. `action_contract.h` holds their common bounded
oracle and joins every created thread even if later thread creation fails.

The Action checks prove wrapper integration and allocation lifetime. Native
goal execution, cancellation, callbacks, event-queue refusal, and cleanup still
need the actual existing native Action acceptance suites; these allocation-only
executables cannot replace those tests or a Wasm/ROS end-to-end result.

## Root-owned build and execution contract

All paths below must refer to a newly frozen complete current source tree and
fresh owned output. Do not copy the new headers into an old source tree and
label its unexamined objects current. Select the actual compiler, its sysroot,
libraries, and every source dependency with current full pins. Use full `-MD`
dependency capture, retain the compiler/link command and output ELF identity,
and preserve the complete selected inputs before and after execution.

The SDK-free compile shape is:

```text
<actual-current-c++> -std=c++17 -O2 -g0 -pthread -I<current-source>/src
  -MD -MF <fresh-output>/contract.d
  <current-source>/tests/ros_ticket_lifetime/contract.cc
  -o <fresh-output>/contract
```

The ROS1 and ROS2 wrapper sources must use the actual selected native Gateway
SDK compile/link definitions, include paths, and libraries. Replace the main
source and fresh target/dependency paths; do not silently reuse stock host
headers or omit the SDK's generated feature header. Link with the selected SDK
runtime and execute with its qualified loader/library environment. Build one
target at a time initially. The test's own largest allocation oracle is about
9.5 MiB; this is a source bound for that vector, not a measured process RSS or
compiler budget. The Root owner must enforce actual per-process/global memory,
storage, and finite time limits and preserve its 8 GiB storage reserve.

Each executable accepts only no argument or `--owned-stop`. With the latter,
the actual executable stops itself before running any case so Root can prove
its parent, executable, cgroup, and mapped inputs, then resume the owned actor.
It emits one exact PASS record on success or a FAIL diagnostic and exit one.
The expected success records are:

```text
PASS ticket-lifetime groups=7 completed=1048608 queue_rejected=1048608 exact_uint8_nonzero_domain=255 parallel_ids=131072 parallel_Service_ids=8192 no_ROS_or_Guest_execution
PASS ticket-Action ROS1 domains=2 sequential_per_domain=1048608 parallel_per_domain=131072 no_ROS_or_Guest_execution
PASS ticket-Action ROS2 domains=1 sequential_per_domain=1048608 parallel_per_domain=131072 no_ROS_or_Guest_execution
```

Fixture output uses the maintained `ros_probe::print_fixture`: C++20 uses
`fast_io`; the supported C++17 native-SDK branch uses its bounded, locale-free
integer formatter. The allocator itself performs no text or file I/O and adds
no per-request heap allocation. Existing `ipc::random_session` supplies actual
kernel entropy; no custom cryptography or pseudo-random generator is added.

## Required affected rebuilds and remaining validation

Both `gateways/ros1/main.cc` and `gateways/ros2/main.cc` include the modified
Service/Action headers and require fresh compile/link qualification for every
selected native profile. ROS1 custom Action users must also rebuild every
translation unit including `gateways/ros1/custom_actions.h`. ROS2 custom Action
users must rebuild every translation unit including
`gateways/ros2/custom_actions.h`; the existing Topic-only partition sources do
not include these Action or Service headers. Reuse may be decided only from
actual complete dependency records, not from filenames or old PASS labels.

The actual Service deadline and Action ownership/deadline/cleanup acceptance
suites must run against those fresh native products. Default-denied and
explicit-grant scenarios retain their existing policy. A historical raw result
or a source review is not a result of those pending current-byte tests.
