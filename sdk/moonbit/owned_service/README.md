# Optional retained ROS 2 Services for MoonBit

This package is a separate, explicitly selected ROS 2 SDK. It calls all twelve
numeric imports in `uwvm_ros2_rmw_srv_v1` from the same Wasm module. It does not
load a native ROS shared library, embed a native node, discover permissions, or
provide an in-process port of rcl, rclcpp, rclpy or rcljava. Those source ports and
their native Gateway builds have separate contracts and qualification work.

The optional package compiled with the pinned MoonBit producer on 2026-10-19.
Its four private protocol models passed on both `wasm` and `wasm-gc` through the
[reusable model runner](../MODEL_TESTS.md), with zero ROS import calls. The
[retained current evidence](../../../test/0050.high_level_languages/results/native-budget/language-runtime-control-edges-20261019/README.md)
binds those exact test artifacts and output. The separate generated control
application and nonempty native Service scenarios remain source preparation;
these eight local model executions do not qualify native delivery, full official
client-library parity, allocation traps, DBG, persistent cache, QEMU or VTune.

## Select the package and authority

The original `ros1`, `ros2`, original81 ABI and optional client ABI stay separate.
Existing business packages do not import `owned_service`. This optional package
requires backend 2 and ABI version 1; a different backend is an error rather
than a fallback to legacy RPC. Generation requires the explicit
`--rmw-service-enabled 1 --backend 2` selection; generation defaults to disabled.

Open each role with your actual `Session` and `Node`:

| Endpoint | Required independent caller aliases | Optional independent alias |
| --- | --- | --- |
| Client | `service.call`, `event.poll_v1` | `service.available` |
| Server | `service.advertise`, `service.respond` | None |

Pass alias names to `Node::owned_client_owners(call, observe, availability)` or
`Node::owned_service_owners(advertise, respond)`. The names select existing CLI
grants; they are not native ROS names. The SDK creates no policy, wildcard,
network address, cancel-any permission, native identity or default grant.
Both handles must belong to this actual Session and remain distinct and live.

Local objects and labels protect SDK consistency within one Guest. They do not
isolate mutually malicious components in that Guest. The Host independently
checks the true caller Instance, backend, grant, generation, session, operation,
native type, pairing, bounds and quotas on every import. Directly calling raw
imports does not bypass those Host checks. Use distinct Wasm Instances for
distinct untrusted principals, and audit actual Host policy and denial results.

## Prepare and activate

`prepare_endpoint(owners, expected_qos, maximum_body, history_budget)` allocates
the endpoint's receipt, rows, ticket/native-ID history and retirement frame.
It creates no native entity. `expected_qos` contains **two exact 80-byte
protocol QoS records** captured independently for the intended native endpoint.
The SDK validates their concrete enum, depth, reserved and duration fields.
Do not substitute a guessed default or a ROS text-profile serialization.

`maximum_body` is the application's explicit bound, at most 4,193,626 bytes;
the selected full control profile currently uses the canonical per-type bounds
and may require a smaller policy quota. `history_budget` is 1 through 65,536
for the entire endpoint lifetime. It limits SDK bookkeeping, not native DDS
queue storage, Guest RSS, Host memory or an actual native client quota. History
is never reset or recycled after abandon, completion or a duplicate ID.

Call `activate()` once. A successful native activation provides UMRE256 with
the complete real instance/generation/session/incarnation, both actual QoS rows
and the actual GID of up to 32 bytes. The client compares the caller's expected
QoS only after retaining a complete genuine binding for cleanup. A mismatch
refuses live admission while preserving separate native-retire eligibility.
Malformed or unassociated bytes never establish that eligibility. `inspect()`
compares the current complete receipt, including both QoS rows and GID.

The public application APIs in
`sdk/examples/moonbit_owned_service/app/application.mbt` retain a failure carrier
before the first open. Keep that object until cleanup has finished, even when
`start_description` or `start_parameter_types` fails. `close()` attempts native
and raw cleanup, closes any remaining partial Session opens and returns a
nonzero result if business work or cleanup failed. `primary()`, `endpoint()`,
`endpoint_cleanup()`, `session_cleanup()` and `session()` expose independent
facts; a printed summary does not replace them. A terminal allocation/VM trap
does not promise a MoonBit unwind: Runtime Instance retirement must physically
clean up native work and must itself be tested.

## Complete native records and all seven Services

`tools/generate_control_moonbit.py` executes captured exact helper source bytes
over the actual shared `custom_types.py` and `bounded_profile.py` resolved IR.
It requires the full original `default_control/catalog.preparation.json`, all
25 selected original definitions, all 32 resolved records and all seven Service
roots. It also reads a complete same-source canonical C/C++ product bundle and
its exact output hashes. This is a consistency check on explicitly admitted
inputs; it does not authenticate the producer's history or discover a native
RIHS identity. The Linux keeper must independently admit the actual entry,
interpreter/startup/stdlib, all inputs and toolchain before their first use.

| Complete original Service | Generated Client/Server type suffix |
| --- | --- |
| `rcl_interfaces/srv/DescribeParameters` | `DescribeParametersClient` / `Server` |
| `rcl_interfaces/srv/GetParameterTypes` | `GetParameterTypesClient` / `Server` |
| `rcl_interfaces/srv/GetParameters` | `GetParametersClient` / `Server` |
| `rcl_interfaces/srv/ListParameters` | `ListParametersClient` / `Server` |
| `rcl_interfaces/srv/SetParameters` | `SetParametersClient` / `Server` |
| `rcl_interfaces/srv/SetParametersAtomically` | `SetParametersAtomicallyClient` / `Server` |
| `type_description_interfaces/srv/GetTypeDescription` | `GetTypeDescriptionClient` / `Server` |

Actual names have their complete `Ros2<Package>Srv` prefix, for example
`Ros2Rcl_interfacesSrvGetParameterTypesServer`. Construct a typed wrapper with
its `::new(endpoint)` before activation. The constructor checks the actual
endpoint role and complete request/response wire bounds. Its methods are:

| Method | Behavior |
| --- | --- |
| `activate`, `inspect`, `close` | Explicit endpoint lifecycle; status is an Int |
| `submit(request)` | Client: complete canonical encode before one native submit; returns the genuine retained Request |
| `available(wait_ns)` | Client: one real query using the independent availability grant; wait at most 60 seconds |
| `abandon(request)` | Client: once from admitted state; local abandonment, never a DDS cancellation promise |
| `take()` | One full metadata/body decode before a single consume ACK |
| `value()` | Existing complete decoded value only after successful ACK; absent for control or abandoned delivery |
| `request(ticket)` | Server: resolve an actual committed local Request, not an arbitrary bearer ticket |
| `respond(request, response)` | Server: complete canonical encode and one response attempt |

Each record has `::new()` and a corresponding `Codec::new().encode/decode`.
All original fields, scalar constants, defaults, nested aggregates, Cartesian
capacities and canonical identities are retained. `schema_bytes()` publishes
complete UTF-8 metadata; it is observational data, not a grant. Native
`ParameterDescriptor.type`, `ParameterValue.type` and `Field.type` use the legal
MoonBit spelling `type_value`. Their native names and canonical identities
remain unchanged and the metadata records the exact spelling map. Collisions
and other unsupported keywords are refused.

`TypeSource.raw_file_contents` is `Bytes`, preserving embedded NUL and invalid
UTF-8. Other original strings use the actual bounded UTF-8 rules. Full unsigned
64-bit values never pass through Float or Double. Signed native request sequence
and timestamps retain their exact Int64 bits. A 32-byte receipt GID is never
substituted for the separate 16-byte native request writer GUID. The CONTROL IR
does not add a Time.nanosec normalization rule; protocol QoS duration validation
is a separate domain.

## Delivery, errors and cleanup

Every successful `take` verifies full UMRC160 plus URMI192, exact body extent,
real current owner/peer/incarnation, positive sequence, fresh reservation nonce,
ticket, writer GUID, native signed sequence/timestamps, error/control flags and
reserved fields. It then decodes the **complete** generated body before ACK.
The returned Delivery exposes all of those observed facts through getters.
No length slot is invented for the retained peek ABI.

A native request-error control record with flags 0 completes only its associated
request; flag 1 separately says the source is retired. The raw flags/error stay
observable. A control record never becomes a successful typed application value.
An abandoned response is still fully decoded and consumed once, then discarded.
The SDK never retries an ambiguous submit, respond or ACK. It cancels a borrow
only after a fully associated sequence/nonce was established and only before an
unknown commit. Ticket and native writer/sequence histories remain retained.

`last_take_idle()` is true only for the actual raw peek returning 11. A decoder
returning the same numeric 11 is a failure; applications must not treat it as an
empty queue. A timeout or any other errno does not prove no Host/native effect.
The caller schedules each poll and owns the overall finite deadline. There is
no background worker, sleep, probe client, automatic request replay or permission
discovery. The example entries use a finite explicit poll count; that is not a
hard wall-clock deadline or a throughput benchmark.

`close()` first keeps the endpoint busy, retains its primary error, attempts
native retirement once for a genuine known binding, releases its Session lease,
then closes all independently acquired raw owners in reverse order. It continues
after an earlier failure, retains each original outcome and refuses recursive
entry with 17. An externally closed owner must not suppress sibling cleanup.
Repeated close reports retained facts and does not replay unknown effects.

## Real example preparation and build

The examples contain a genuine complete GetTypeDescription client and a
GetParameterTypes server backed by a caller-selected local parameter table.
They open actual SDK owners, activate a retained endpoint, perform the real
raw12 protocol and check both business and cleanup failure. They have no legacy
RPC/native-library fallback. Follow their separate
`sdk/examples/moonbit_owned_service/README.md` for exact preparation and entry
configuration. Building a source example does not qualify its actual Native
service, ROS distro, shared/static dependencies or runtime policy.

The initial toolchain identity is MoonBit `0.10.15+943464bfa` / moon
`0.1.20261012` with core archive
`96d30f4dbcc6a9e1736ef26fbdd0e8fb0e97cb01d50e55d2a0072208b8061ae5`.
Record and independently check the actual executable/core/package/source
identities again on Linux. The module manifests use the maintained `moon.mod`
and local `moon.work`; use `--frozen`. Build both `wasm` and `wasm-gc`, debug and
release, in separate fresh owned output trees. Keep all emitted symbols,
source maps, compiler diagnostics, actual imports and full Wasm validation.

## Final binary and memory admission

The example export `abi_coverage` retains all 43 selected original ROS 2 imports
plus all 12 optional imports for inspection. **Never execute valid selectors**:
the inspection probe contains deliberate zero placeholder arguments.
`check_final_imports.py` requires exact numeric signatures for all 55 and an
independently supplied exact table of every non-ROS function/tag import. It
rejects duplicates, additional ROS namespaces, wrong backends, altered types,
missing imports and module-wide allowances. This is not a whole Wasm validator.

The same module uses one actual nonshared wasm32 memory 0. It reserves stdout
`[0,289)` and ROS arena `[65536,8454160)`. Input starts at 65536, output at
4259840 and the slot/padding at 8454144. There are initially 256 memory pages
and a maximum of 1024. Managed arrays/GC references are copied through explicit
loads and stores; they are never cast to memory addresses. Guarded retained
frames prepare all return storage before calling their one numeric import.

The checker conservatively rejects unknown/passive DATA initialization, active
DATA overlapping a reservation, imported/shared/memory64 memory and an incorrect
memory export. The source link option `heap-start-address: 8519680` is **not**
proof of the actual allocator. Actual compiler/link layout and allocator codegen
evidence is mandatory for each emitted artifact, including wasm-gc. Without
the externally admitted evidence receipt the checker returns HOLD/nonzero. Even
with one, its report remains a signature/static-layout subcheck with
`full_memory_safety_pass=false` and `heap_start_proved_by_binary=false`; JSON
self-assertion cannot authenticate a keeper or establish runtime safety.

## Required Linux qualification

Use the existing SSH Linux 64 GiB cgroup with swap 0 and the coordinated task
keeper; do not create a replacement cgroup. Admit entries/interpreters/helpers,
toolchain, source and complete child-process closure before execution. Retain
bounded output, actual process exit, cgroup/OOM events and physical cleanup
facts. Delete only owned, no-longer-needed artifacts after preserving evidence.

Qualification must include full32 canonical C/C++ cross-vectors, both real
applications, all seven real client/server Services, denied/mispaired/revoked/
stale grants, malformed UMRE/UMRC/URMI and bodies, high-bit handles, native IDs,
late abandonment, unknown ACK, memory growth/guards, OOM before and after each
effect, partial-open cleanup, reentry, DBG and ROS 2 debugging. Test executable
SHA cache invalidation, CPUID/features, WASM 3.0/WASI interactions and ROS cache
refusal/compatibility under the actual runtime mode. Run x86_64 and aarch64 QEMU
and the additionally supported ROS architectures under that same keeper.

VTune hardware timing/counters, Hotspots and Microarchitecture Exploration need
real native controlled workloads and comparable build/policy/input/lifetime
settings. Keep cold startup, generation/compile, steady-state service work and
cleanup separately measurable. Compare the same eligible Wasm and host adapter
against Wasmtime, WAVM and WasmEdge only where each truly supports the required
ABI/proposals. Missing imports are not a speedup. No such performance result has
been measured for this source delivery.
