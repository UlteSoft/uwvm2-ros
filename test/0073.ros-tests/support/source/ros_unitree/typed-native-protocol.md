# Independent native witness for typed Unitree examples

This is an implementation and invocation contract. It does **not** report a
successful build, native transport, simulated physics or performance result.
The fourteen Guest products and twenty-eight case descriptions are listed in
`typed_example_cases.py`. The native witness source is
`native_typed_ros2_peer.cpp`; its explicit CMake input is
`native_typed_ros2_CMakeLists.txt`. A future actual driver must supply all
physical selections and produce independent evidence before accepting a case.

## Native build and identity

Stage the exact CMake input as `CMakeLists.txt` in a fresh owned directory.
Select the complete frozen project as `UWVM_SOURCE`, actual native ROS SDK,
and the real installed GO24, HG11 and API8 generated packages. Use the same
native definitions and exported identities as the trusted Gateway registry.
Select `UWVM_UNITREE_NATIVE_CXX_STANDARD=17` or `20` according to the actual SDK;
the latter uses vendored fast_io. Configure and build serially, retaining
actual compiler invocation aliases, complete system-header dependency files,
link traces and native ELF. The configure probe links the actual quiet Node
API and never executes a ROS process. Missing APIs stop the build.

The witness is an ordinary native process, not a Wasm Runtime or Guest
capability boundary. Before native imports or process execution, the actual
driver must validate the frozen source and every selected native tool/SDK
input. Run only in the original selected 64 GiB/swap0 cgroup, with a finite
owned process-tree RSS, wall-clock, log and disk budget. The helper itself
does not create, enlarge, move to or repair a cgroup. Keep all actual compiler
and test processes in one serial lane.

Select `ROS_DOMAIN_ID=1`, `RMW_IMPLEMENTATION=rmw_cyclonedds_cpp`, and an
independently pinned Cyclone XML that restricts DDS to loopback. Supply no
`ROS_NAMESPACE` or `LD_PRELOAD`. The helper has no CLI address/domain override.
The driver must inspect the real XML and actual environment; the helper's
environment string checks alone do not prove network isolation. Never run
this fixture on a physical robot's domain or translate its topic names into
firmware endpoints.

## Framing, held ownership and fixed resources

stdin and stdout are owned nonblocking pipes. Send exactly sixteen setup
bytes: ASCII `UTN1`, followed by three unsigned little-endian 32-bit integers:
family (`1` GO LowState, `2` HG LowState, `3` API), positive (`0` or `1`), and
reserved (`0`). The helper accepts no process arguments. API identity and
API id are both exactly `1`, matching the closed Guest case description.

The helper prints `READY_NATIVE_TYPED_V1 FAMILY POSITIVE` and blocks for the
next explicit control byte. While it is held, reopen its actual executable,
complete mapped native files and generated typesupport DSOs through its owned
PID/starttime/pidfd. Compare these with the selected SDK and registry identity
exports. A printed readiness label cannot replace this physical observation.
Never stop or signal the process merely to obtain a mapping snapshot.

These are the only fixture topics:

| Role | Topic | Native endpoints |
|---|---|---|
| GO/HG | `/uwvm_unitree_typed/lowstate` | One publisher and one independent native observer |
| API Request | `/uwvm_unitree_typed/request` | One native Request subscription |
| API Response | `/uwvm_unitree_typed/response` | One publisher and one independent native observer |

GO and HG use separate fresh processes and type identities despite sharing the
fixture topic spelling. Every endpoint is reliable, volatile, keep-last1.
Intra-process delivery is disabled. No LowCmd endpoint, service, action,
parameter service, parameter-event publisher or rosout is created. Graph
checks fail if a `/lowcmd` publisher or any service appears in the isolated
domain. The actual driver additionally audits Guest and Gateway effects.

## Ordered native controls

Each control is one byte, with no newline. Records are newline-terminated
ASCII, at most4096 bytes. At most eight controls are accepted, each only in
its documented lifecycle state. Reads, writes, discovery and callback waits
have finite deadlines and poll budgets; the external parent independently
enforces its entire process-tree deadline and cleanup.

1. `M` waits for actual graph counts and emits
   `NATIVE_TYPED_MATCHED FAMILY POSITIVE`. In denied mode the native baseline
   remains unchanged. In positive mode it requires the actual Guest LowState
   subscription, or the API response subscription and native request reader.
   For LowState the actual native self-observer is the baseline subscription;
   positive count is2. For API, the native Request subscriber count is1,
   Request publishers are0 at this pre-submission barrier, Response publisher
   count is1 and Response subscribers equal1+POSITIVE. Capability-open is local;
   the Gateway creates its Request publisher only during real publication.
   Waiting for that publisher before submission would deadlock. This discovery
   marker is not delivery or a guarantee of first-message delivery. Native
   publisher/subscriber match counts are checked in addition to advertised
   graph counts; single volatile LowState/Response publication waits for those
   actual matches.
2. Denied case: run the separate actual empty-policy Guest, independently
   verify alias-open13 and its documented closed-state scalar results, then
   send `Q`. The helper spins for at least two seconds and thirty samples,
   requiring unchanged baseline graph and zero payload on every sample.
   It emits `NATIVE_TYPED_QUIET FAMILY SAMPLES ELAPSED_NS STATES REQUESTS RESPONSES`.
3. Positive LowState: after actual Guest initialization and `M`, send `P`.
   The helper publishes one complete native generated message, then waits for
   its independent actual DDS subscription. It checks full generated-message
   equality before emitting `NATIVE_TYPED_STATE FAMILY TICK CRC CARRIER`,
   followed on the same line by q/dq/tau_est binary32 words for every carrier.
   GO has20 and HG35 carriers. Tick is42, CRC field is0x13572468, and slot i
   uses q=(i+1)/32, dq=-(i+1)/64, tau_est=(i+1)/128. Every other field retains
   the actual generated default. These exact binary fractions simplify
   comparison; the CRC is deliberately an observed fixture field, not an
   authenticated robot command. Separately poll the real Guest at most4096
   times and compare all60/105 words, tick, CRC, count1 and successful status.
4. Positive API: after actual Guest initialization and `M`, invoke its
   `submit_once` once and verify the repeated-submit9 result. Send `A` to the
   native helper. It waits for one actual Request and actual Request publisher
   discovery, then compares the complete
   generated message with identity1/api_id1, parameter `{}`, empty binary,
   and all remaining default fields. Only then does it publish a complete
   Response with matching identity, status.code0, data `typed-example-ack`,
   empty binary and other default fields. Its independent native Response
   subscription checks complete equality. The record is
   `NATIVE_TYPED_RPC 1 1 0 typed-example-ack`. The real Guest must separately
   poll, decode and verify the Response before count1/status0 qualifies it.
5. Positive case: after the Guest's successful close and repeated-close9,
   send `Z`. The helper waits for actual Guest endpoint removal, then
   observes the same two-second/thirty-sample quiet baseline. No additional
   payload is permitted. This does not replace external Gateway/Runtime
   teardown or actual process ownership checks.
6. `F` accepts only the completed denied or positive sequence. It emits
   `PASS_NATIVE_TYPED_WITNESS FAMILY POSITIVE`, destroys its native endpoints
   and context, and exits0. A failure emits `FAIL_NATIVE_TYPED_WITNESS CODE`
   where possible and exits1. Root must independently observe the real exit,
   preserved source/full pins/whole SDK roots, and child-first owned cleanup.

## Guest policy and result boundaries

Use different fresh Runtime instances for denial and positive modes. The
denied policy is actually empty. Positive LowState grants only the exact
`unitree-lowstate` subscription alias and native identity, mapped to the
selected fixture topic. Positive API grants only `unitree-api-request`
publication and `unitree-api-response` subscription with the actual matching
identities and these independent fixture topics. Do not grant graph access,
services, actions, parameters, clocks, random, filesystem or network access to
make a case pass. Every participating preload library retains its own
capability ancestry and bounded separate memory.

The parent coordinates these controls with actual Wasm calls; this document
and the helper do not manufacture that driver or its observations. Retain the
Guest's actual initializer, binary/import/export/memory contract, owned
Worker identity, copied return values and native witness records separately.
A codec/compiler receipt, status0 without count1, self-declared identity,
stdout label or independently successful native process cannot qualify the
whole case. These static fixture values do not establish simulated control,
motion, official API behavior, ROS1 compatibility or native/Wasm performance.
