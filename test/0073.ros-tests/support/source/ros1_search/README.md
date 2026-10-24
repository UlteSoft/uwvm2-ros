# Explicit ROS1 parent search and Action byte boundaries

These changes add a fixed-key, bounded parent search without increasing the
authority of an existing grant. They also correct one advertiser feedback-byte
boundary in ROS1 Action responses. New compilation and execution use the
existing, freshly verified Linux **64 GiB**, zero-swap cgroup. Source/Python
models are a separate evidence category. The r18 strict ROS1 Gateway and both
native probes compiled successfully. Its first behavior probe failed at an
incorrect IPC error expectation; the second probe did not run. The corrected
fixture still requires fresh actual acceptance.

## Search contract

The existing `param.search` resource is a canonical absolute initial candidate,
for example `/robot/controller/child/gain`. Its final component, `gain`, is the
fixed key. The preceding components define the initial namespace. The request
remains empty: a guest cannot substitute a resource or relative search key.

The Worker request validator rejects a nonempty search request with EINVAL
(22). A native IPC client can bypass that Worker validator and forge a frame;
the common Gateway dispatcher first enforces the operation's zero-byte request
quota, returning project `too_large` (90) and an empty response before calling the backend.
The backend also retains its independent empty-request check. The native probe
tests that IPC admission boundary with exactly 90, while the Worker contract
test keeps its exact 22 expectation. Neither permits a caller-selected key.

An absent `search-parent-namespaces` field, or the explicit value `false`,
retains exact-name behavior. Only the explicit value `true` permits the fixed
key in successive parent namespaces. Both Worker and Gateway accept this
optional boolean only for ROS1 `param.search`; all other spellings and
operations reject it. It participates in the immutable capability manifest.

For the initial candidate above, `true` examines:

1. `/robot/controller/child/gain`
2. `/robot/controller/gain`
3. `/robot/gain`
4. `/gain`

Every candidate is additionally restricted by the explicit, segment-aware
`allowed-name-prefix`. For `/robot`, the fourth candidate is outside authority
and is never queried. For `/robot/controller`, only the first two candidates
are queried. A neighboring prefix `/robot_bad` denies the initial resource
with 13 before a native query. A permitted walk with no allowed match returns
2, regardless of whether an outside ancestor exists. A prefix alone never
enables parent search. Search authority returns only names and never grants
`param.get`, `param.set`, or any other operation on those names.

The Gateway uses bounded `hasParam` calls for the permitted candidates and
checks the same absolute deadline before and after each call. Their remaining
time comes from the minimum of the call timeout and the capability timeout.
This bounds admission to subsequent calls and refuses a result observed after
the deadline. It does **not** prove that a native XMLRPC operation can be
interrupted during a blocking phase. The canonical 1024-byte name ceiling also
bounds the number of candidate namespaces.

Successful search results use the existing canonical String encoding:
`u32_le UTF8_length` followed by UTF-8 name bytes. The complete encoding is
subject to `max-parameter-bytes`. The Worker checks this framing and byte
limit before applying the same exact/parent candidate allowlist to the
returned name. A forged ancestor, different key, sibling namespace, malformed
path, or name outside the prefix is refused with 13.

[The native ROS1 implementation](https://raw.githubusercontent.com/ros/ros_comm/noetic-devel/tools/rosmaster/src/rosmaster/paramserver.py)
searches a relative single-component key from its initial namespace upward.
Absolute keys retain exact lookup. Native multi-component relative keys have
additional first-component partial-match behavior, including names that do
not yet exist. This fixed-basename projection does not implement that
multi-component behavior, arbitrary relative keys, private names, or guest
remapping. No full native `searchParam` equivalence is claimed.

Existing Worker and Gateway configuration validation requires a non-root
`param.search` resource; `/` is still rejected as a configured search grant.
The shared helper defensively preserves exact `/` membership when called
directly, and treats it as having no parent candidates. These checks do not
introduce any new configured root authority.

Example additive configuration for the fixed candidate above:

```text
grant search-gain param.search /robot/controller/child/gain builtin/string-v1 builtin:param-string-v1
set search-gain allowed-name-prefix /robot
set search-gain search-parent-namespaces true
```

The remaining required parameter limits, validator, and audit settings must
also be explicit. The optional field may be used through the existing ROS1
`--ros1-set` CLI and trusted Gateway `set` syntax; no ABI export changes.

## Action byte accounting

Canonical Fibonacci feedback has `goal:u64`, `count:u32`, and `count` signed
32-bit values: `12 + 4N` bytes. An `action.respond` request additionally includes
`kind:u8`, making `13 + 4N` bytes. The responder must authorize the complete
request. For kind 1 only, the advertiser bounds the canonical feedback without
that outcome byte. Both success/abort/canceled result envelopes retain the
outcome byte and remain `13 + 4N` bytes against both capabilities. Existing
ownership, cancellation, deadline, and native-goal state checks remain in
their original order.

## Prepared fixture inventory and execution requirements

| Source | Standard and dependencies | Behavioral inventory |
| --- | --- | --- |
| `contract_test.cc` | C++20, project `src`, vendored fast_io includes, pthread | Actual Worker response framing/candidate authorization; Worker/Gateway optional field acceptance and rejection; valid non-search/ROS2 calibration; Global/Single/Group CLI insertion, invalid/duplicate fields, explicit false and version isolation; empty fixed request; ordered parent helper and root boundary |
| `gateway_contract_test.cc` | C++17, project `src`, pthread; no ROS | Actual Gateway validation and C++17 candidate helper; legacy exact grant; explicit boolean; prefix-limited walk and neighboring namespace |
| `native_ros1.cc` | C++17, project `src`, ROS1 Noetic roscpp/xmlrpcpp, pthread, freshly built ROS1 Gateway, isolated Master | Exact and false preserve behavior; true selects nearest/exact/next ancestor; native single-key agreement; permitted-prefix boundary and no returned outside name; String quota; independent operation; exact manifest binding; revoke |
| `../ros_action/feedback_budget_ros1.cc` | C++17, project `src`, ROS1 Noetic roscpp/actionlib/actionlib_tutorials/actionlib_msgs, pthread, isolated Master | Eleven feedback scenarios for 0/1/2/48 values; six success/abort result boundary scenarios; separate advertiser and responder one-byte-short denials; real feedback/value/status witnesses; denial retains goal; stale goal refusal |

Use the frozen cross SDK and actual catkin include/library flags from the
qualified Noetic build. Do not guess a middleware link line or reuse an old
Gateway product after these header changes. A concrete build/run sequence in
the already verified cgroup is:

```sh
# These placeholders must be replaced by paths/flags captured from the actual
# qualified compiler, cross SDK, source freeze, and catkin build environment.
"$CXX20" --sysroot="$SYSROOT" -std=c++20 -I"$SOURCE/src" -I"$FAST_IO_INCLUDE" \
  "$SOURCE/test/0073.ros-tests/support/source/ros1_search/contract_test.cc" -pthread -o "$OUT/search-worker-contract"
"$CXX17" --sysroot="$SYSROOT" -std=c++17 -I"$SOURCE/src" \
  "$SOURCE/test/0073.ros-tests/support/source/ros1_search/gateway_contract_test.cc" -pthread -o "$OUT/search-gateway-contract"

# Build native_ros1.cc and feedback_budget_ros1.cc with the captured ROS1
# catkin flags, full depfiles, current-source qualification, and the same
# process-tree/cgroup sampler used by the existing ARM acceptance drivers.
# Rebuild the actual Gateway before passing it to the search native fixture.
"$ARM_LAUNCHER" "$OUT/search-worker-contract"
"$ARM_LAUNCHER" "$OUT/search-gateway-contract"
"$ARM_LAUNCHER" "$OUT/search-native-ros1" "$FRESH_ROS1_GATEWAY"
"$ARM_LAUNCHER" "$OUT/feedback-budget-ros1"
```

All four executions require current source/toolchain/product qualification
and the cgroup process evidence. The native Action fixture exercises the real
adapter and middleware; it is not a substitute for a Wasm/native E2E test.
The native search fixture confirms output isolation but does not independently
count every Master query. Internal query traces are currently covered by
source checks and independent Python models only.

The only locally permitted verification for this scope is:

```sh
python3 -B test/0073.ros-tests/support/source/ros1_search/verify_models.py --output /tmp/fresh-search-models.json
```

That command does not compile C++, instantiate Wasm, or contact ROS. Existing
reports are never replaced. The immutable original audit and historical
runtime evidence remain unchanged.
