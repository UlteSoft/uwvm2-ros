# Typed Unitree Guest examples and same-main commands

These examples use complete, generated Unitree values with the existing typed
Guest SDK. They do not implement a native ROS node, a DDS stack, a robot
firmware API or a locomotion policy. This document describes source and future
actual qualification; it does not report compiler or transport results.

For the complete ROS1 legacy family, use the six actual
[LowState/LowCmd applications](examples/legacy/README.md), their
[same-main command protocol](../../../tests/ros_unitree/legacy_command/README.md)
and [closed six-product recipes](recipes/legacy-command-recipes.json).
They require freshly exported Noetic identities for all eleven types and
separate actual TCPROS qualification; the ROS2 results below cannot supply it.

The independent operator must select a pinned generated family, its actual
native identity export, the matching Gateway registry and explicit aliases.
Use an isolated simulator/native fixture domain1 on loopback. These examples
choose no ROS domain, physical robot address, interface, participant, service
manager or network configuration. Their default authority is empty.

## Build and run a same-main command

The supplied reactors are library/source components. For usable positive typed
transport, compile the selected example and a command entry **into one Wasm
module**, then run that command as the Runtime main. Runtime intentionally
refuses ROS calls whose Wasm ancestry crosses modules. A separate WAT main
calling a preloaded typed reactor therefore remains a denial fixture, even
if both principals have aliases. Do not use that route for positive behavior
or weaken the Runtime guard.

The portable [same-main adapters](../../../tests/ros_unitree/typed_command/README.md)
and their [closed fourteen-product mapping](../../../tests/ros_unitree/typed_command/recipes.json)
show the complete source and build steps. C, C++, Rust and embedded JavaScript
link the real freestanding C `entry.c` into the selected actual module. AS
compiles its actual language entry and exported runtime initializer. TinyGo
uses the actual command startup and a separate Go main in an owned copy of the
selected example. All adapters call the unchanged real typed functions below;
none implements ROS, native handles, a codec replacement or a host bridge.
Full native-bound GO24/HG11/API8 generation remains required. These are fourteen
**new command products**, separate from the fourteen reactor products.

The command requires only the standard input/output/error descriptor budget
and the actual basic WASIp1 `fd_read`, `fd_write`, `proc_exit` imports. It opens
no file or socket, reads no environment, requests no random/clock/network
operation and introduces no optional WASI right. The actual import table must
also contain only the selected real ROS2 ABI signatures. The command has one
bounded own memory and no implicit start section; the trusted owner explicitly
selects its actual local void entry. C/C++/Rust/JS use `_start`; AS uses
`typed_command_entry`; TinyGo uses its genuine compiler-produced `_start`.
Do not call a reactor initializer in addition to a command's startup.

A fresh process prints `READY_TYPED_MAIN`. Write sixteen binary bytes:
`UTM1`, family `u32le` (GO1/HG2/API3), mode `u32le` (denied0/positive1), and
reserved `u32le` zero. Family is compile-bound. Setup selects behavior only;
it cannot grant authority. By default the Runtime main has no ROS aliases and
must report `TYPED_INITIALIZED 0` then `TYPED_DENIED_VERIFIED`, while a separate
native observer proves no Guest endpoint or payload. Send `F` to finish.

For a different positive instance, the trusted operator must supply exactly
the alias policy below **to main only**, use the matching native identity and
Gateway registry, and establish the independently observed native fixture
ordering in the adapter README. `R` requests one real SDK poll (maximum4096),
`S` submits the API Request once after native discovery, `C` closes after one
real delivery, and `F` finishes after independently observed native quiet.
The command reports all LowState tick/CRC/motor bit words or the validated
Request/Response acknowledgement. Success markers describe the real typed
functions' observations; they cannot replace the independent native payload
and endpoint witness. All acquired SDK owners are closed through their real
lifecycle, and external process ownership remains mandatory on trap or kill.

The command adapters/recipes are source, not a record of compilation or a
transport result. The separate compiler must produce current full dependency,
link, import/export/memory and full validation evidence. A later Runtime case
must additionally prove its actual Worker principal, policy, native identity,
loaded native files, delivery and cleanup. Keep original native and compiler
source hashes factual. This route adds no LowCmd, robot firmware operation,
physical interface selection, service/action or locomotion claim.

## Products and real SDK objects

| Language | GO/HG LowState | Request/Response topics |
|---|---|---|
| Rust | `examples/rust/lowstate.rs`, select exactly `--cfg unitree_go` or `--cfg unitree_hg` | `examples/rust/topic_rpc.rs` |
| JavaScript inside Wasm | `examples/javascript/lowstate.mjs`, select the actual generated GO or HG module | `examples/javascript/topic_rpc.mjs` |
| AssemblyScript | `examples/assemblyscript/lowstate_go.ts` or `lowstate_hg.ts` | `examples/assemblyscript/topic_rpc.ts` |
| TinyGo | `examples/tinygo/lowstate.go` plus one selected binding | `examples/tinygo/topic_rpc.go` |
| C11 | Existing C/C++ command/controller examples are separate | `examples/topic_rpc.c` |
| C++20 | Existing C/C++ command/controller examples are separate | `examples/topic_rpc.cpp` |

There are fourteen distinct compile products: two LowState families and one
API product for each of four languages, plus C and C++ API products. Denied
and positive modes are separate fresh **instances**, not extra compile
products. A case must not reinitialize a used or closed instance.

Rust uses `roswasm::node::Node`, the actual generated `Canonical` implementation,
`Executor::add_typed`, and typed `publish_value`/`poll_value`. JavaScript uses
`createGuestSession`, `Node`, typed codec objects and the SDK `Executor` in the
real embedded QuickJS Wasm engine. A Node syntax check or a host fake bridge
cannot qualify it. AssemblyScript uses concrete generated class/Codec pairs,
`NativeApi`, typed Node factories and noncapturing guest handlers. TinyGo uses
the actual ROS2 SDK Session/Node, Message interface, generated full decoder and
bounded Executor. C++ uses the actual `Publisher<Request>` and
`Subscription<Response>`; C uses owned SDK capabilities and complete checked
caller-buffer codecs. All callbacks run on the Guest thread after a copied
canonical frame. No native callback is registered by these examples.

The C++ generated records are aliases of the bounded C records. Their text and
sequence fields contain `size` and inline `data` storage; they are not native
`std::string`/`std::vector` ROS objects. The C++ example accesses that actual
generated shape. No pointer, handle or native object crosses a scalar export.

## Explicit, finite lifecycle

No alias is opened during module evaluation. A same-module caller inspects actual binary
imports, exports, memory and start section and performs the selected language's
explicit reactor initialization before any example call. The command adapters
above instead perform their genuine command startup once; these component
instructions must not be applied a second time to a command. TinyGo and the
AssemblyScript recipes require their actual exported `_initialize`; no
implicit Wasm start section is permitted. For other languages, use the actual
compiled export table and selected producer's initialization contract rather
than inventing an initializer export.

All exports take/return Wasm32 unsigned scalar words. Zero is a valid observed
float-bit/status value; first require a successful poll and nonzero received
count. The closed recipe catalog records every exact signature.

1. In a fresh default-policy instance, call `initialize_case(0,arg0,arg1)`.
   LowState uses `(arg0,arg1)=(1,0)` for GO or `(2,0)` for HG. API uses explicit
   positive fixture request/API ids, for example `(1,1)` only when the native
   fixture selects those values. Expected return0 means the actual first alias
   open returned13; `denied_verified()` must be1 and `received_count()`0.
   LowState does **not** activate a subscription before checking denial.
   An unexpected successful open is closed and returns22. The independent
   native observer must additionally show no new subscription/publication or
   payload; an application status alone cannot prove absence of native effects.
   The denied instance is already closed; a later `poll_once`, `submit_once`
   or `close_case` returns9 without another native operation.
2. For positive behavior, create a different actual same-main command or
   same-module caller instance and supply the independent trusted policy below. Call `initialize_case(1,arg0,arg1)` once.
   Success only proves factories opened/activated; it is not delivery.
3. Call `poll_once()` explicitly. Each invocation polls one SDK pass and returns
   11 when no new event was delivered, 0 only for a newly delivered validated
   value, or the original nonzero failure. There is no hidden retry, sleep,
   clock, thread or goroutine. At most4096 polling invocations are accepted;
   exhaustion closes owners and returns110. Root must impose a separate real
   process/Runtime wall deadline and resource/queue quotas.
4. Inspect the actual observation and close with `close_case()` after the last
   pass has unwound. Close attempts every acquired resource, even after an
   earlier error, preserving the first operation/cleanup status. Node/Session
   shared retirement avoids duplicate native closes. Fatal operation/decode
   errors also attempt cleanup. An unrecoverable Wasm trap, panic, allocation
   failure or process kill cannot promise application-level cleanup; the
   external owner remains responsible for its actual Runtime/Gateway teardown.

Reentry into a Rust state borrow is refused17. Other examples likewise use
the SDK's normal callback/resource leases; callers must not recursively invoke
the same reactor from a callback. Invalid configuration/getters are errors,
not defaults to native behavior. A getter's zero alone is never a success flag.

### LowState observations

The positive policy grants only `unitree-lowstate` as topic.subscribe for the
exact selected `unitree_go/msg/LowState` or `unitree_hg/msg/LowState` native
identity. Native resource names/QoS/type belong to the operator's Gateway
policy. This reader opens no publisher and sends no LowCmd.

The entire generated LowState is decoded and validated, including all nested
fields. The reader then exposes `observed_tick()`, `observed_crc()`,
`received_count()` and `motor_word(slot,field)`. GO has20 carrier slots; HG
has35. Field0 is q, field1 dq and field2 tau_est, returned as exact binary32
bit words. Every accessed float must be finite. Do not infer an actuator/model
mapping from carrier order or apply these observations as torque commands.
All carrier words and tick must match an independent actual native witness.
The CRC field is an observed native message field; this example neither packs
a command CRC nor calls CRC authentication. Independent CRC qualification is
the separate official-oracle route.

### Request/Response topic RPC

The positive policy grants only two independent aliases:

- `unitree-api-request`: topic.publish, actual `unitree_api/msg/Request`.
- `unitree-api-response`: topic.subscribe, actual `unitree_api/msg/Response`.

These messages and nested identity/header/status definitions come from the
[pinned official ROS2 sources](https://github.com/unitreerobotics/unitree_ros2/tree/668d1ec5a05d1c38d3306bdca7d59f2ba3581a88/cyclonedds_ws/src/unitree/unitree_api/msg).
Request id and api_id are explicit caller inputs in1..2147483647, represented
exactly as native signed64 values. They do not select a robot endpoint. This
fixture initializes the complete generated Request, sets parameter to `{}`,
keeps binary empty and uses default values for the other complete header
fields. The generated codec validates/encodes all fields; no field is omitted
from the canonical wire or the real native conversion.

After independent native Request-subscription and Guest Response-subscription
discovery is proven, call `submit_once()` exactly once. The current Gateway
creates its native Request publisher lazily during this first actual publish;
local capability-open does not create that entity. Check its actual discovery
and Request receipt after submission. Waiting for that publisher before
submission would deadlock this protocol. A successful publish return alone is
not an acknowledgement or a guarantee of first-message delivery. The
separate SIM-only native fixture must observe that real Request and publish a
real Response with the exact identity, status.code0, data
`typed-example-ack`, and empty binary. `poll_once()` validates all those values
before incrementing received_count. A wrong identity, failure status, wrong
ack, malformed/truncated/trailing bytes or a response before submission is
refused; cleanup is attempted and the case fails. Repeating submit returns9
without a second publication. `response_code()` is meaningful only after a
successful independently witnessed response and received_count1.

The fixed ack is this example's native test-peer protocol, **not** a promise
about any robot firmware API or official high-level API id. Do not publish
these fixture requests to a physical robot. No `service_call`, advertise,
respond, action, parameter or graph right is used.

## Complete generated build inputs

`recipes/closedtyped-example-recipes.json` is a closed source recipe catalog,
not a report of compiled artifacts. Each product has a fixed role, language,
backend2, complete family record count, source, mode arguments, scalar export
map, staged input rule and actual compiler/validator protocol. Root fills only
explicit physical tool/SDK/output inputs. Do not accept extra argv, linker
plugins, arbitrary module imports or silently discovered SDKs.

Use the actual native-bound complete GO24, HG11 or API8 generation. Preserve
its native export/catalog/source identities and full generated output pins,
and verify the current frozen generators/profiles still have the producing
bytes. Keep historical source hashes factual; do not relabel an old native
producer as the current freeze. A guest-only generator result or plausible
RIHS/MD5 string is insufficient for positive native transport.

Generated files are build inputs and are intentionally absent from these
source directories. Their seven exact source references are declared in the
catalog. Their producer is the real literal `products` table in
`sdk/custom-types/bounded_profile.py::generate`; a source-only declaration
does not materialize the file or prove compilation. Never create an empty
header/module merely to satisfy source checks.

- Rust: copy the entire selected `bounded_records.rs` beside the selected
  entry and `lifecycle.rs` in a fresh owned work tree. Build the actual SDK
  rlib with ros2+alloc, and preserve Rust depfiles and complete tool/sysroot
  roots. LowState requires exactly one family cfg; do not compile HG against a
  projected GO type or mix the generated symbols.
- JS: copy frozen runtime modules and these examples at their original
  source-relative paths. Copy the complete family mjs beside the selected
  entry. Include UTF-8 bootstrap, the complete actual SDK module closure,
  lifecycle, entry and generated codec in the real embedded-engine manifest.
  The catalog's manifest template deterministically expands the selected
  product export map to scalar export descriptors. Execute the maintained
  producer's actual generated compile/link/validation jobs, not host Node.
  Reserve input/output32768 bytes so a full bounded canonical frame fits.
- AS: copy the frozen SDK and selected examples at the same relative paths,
  and the complete module to `sdk/custom-types/typed-family/bounded_records.ts`
  in the owned tree. The generated `../../assemblyscript/codecs` import then
  resolves without a source rewrite. Use the actual SDK abortGuest trap;
  keep assertions and export the real start function as `_initialize`.
- TinyGo: copy the real SDK module and these files at their relative SDK
  locations, then place the complete family module in the example's
  `customtypes/bounded_records.go`. The checked local go.mod replacement
  resolves to the actual staged SDK. Use offline Go inputs, explicit selected
  Go/TinyGo roots and fresh owned caches. LowState selects exactly
  `ros2,unitree_lowstate,unitree_go` or `ros2,unitree_lowstate,unitree_hg`;
  API selects `ros2,unitree_topicrpc`. Use an owned target derived from the
  actual pinned `wasm-unknown.json`, adding a real64MiB maximum without
  modifying the SDK. Opt1/scheduler-none/c-shared is not by itself a binary or
  Runtime success claim; inspect the actual full import/start/memory table.
- C/C++: select the actual complete API8 include directory. Keep full compiler
  `-MD/-MF/-MT`, actual link trace, strict numeric flags and assertions. C++
  uses the separately qualified matching official C++ termination overlay;
  never invent a stdio stub or add fd_close/fd_seek rights to admit a binary.

Every language must undergo actual full Wasm validation and full import,
export, memory and start inspection. Only the selected real ROS2 ABI signatures
are allowed; their declaration grants no alias. JS's actual embedded native
module may retain basic fd_read/fd_write; the other recipes require no WASI
imports. No env/native file/network/clock/unknown/nonfunction import may be
silently admitted. Retain unexpected observed tables and fail. No implicit
start or imported memory is allowed; memory maximum is64MiB or lower under
the actual selected Runtime policy.

## Evidence and remaining family scope

`tests/ros_unitree/typed_example_cases.py` specifies the future real native-peer
cases. It is data describing actual prerequisites and observations, not a
fake native implementation or a PASS result. Each positive case needs actual
Runtime/Guest/Gateway artifacts, complete native identity/source closure, an
independently owned native peer, native payload observations, bounded execution
and complete cleanup. Preserve compiler, Runtime transport and physical/model
results as separate scopes. Compiler-only codec roundtrips do not prove typed
ROS delivery; this source addition does not promote earlier compile reports.

Existing C/C++ direct Go2 and sampled-hold LowCmd routes retain their explicit
official GO/HG native layout, reserved-zero rules, CRC oracle and simulator
mapping contracts. This addition creates no new LowCmd/CRC packer in the other
languages and claims no walking, balance or hardware-safety qualification.

Complete legacy ROS1 unitree_legged_msgs11 Guest codecs remain available for
codec examples. Positive native ROS1 examples require real matching Noetic
package builds, actual native traits/MD5/DataType export and an independent
peer. No MD5, type identity, ROS1 CRC field or legacy topic is invented here.

## TinyGo pure-target platform selection

The ROS platform guards accept `tinygo && (wasm || tinygo.wasm)` while keeping
the existing exclusive ROS1/ROS2 conditions. The official TinyGo v0.43.0
[`wasm-unknown` target](https://github.com/tinygo-org/tinygo/blob/v0.43.0/targets/wasm-unknown.json)
uses the LLVM Wasm triple with `goarch: arm` for Go standard-library selection
and supplies the `tinygo.wasm`/`wasm_unknown` target tags. Its
[`BuildTags` implementation](https://github.com/tinygo-org/tinygo/blob/v0.43.0/compileopts/config.go#L119-L140)
adds those target tags and compiler/user tags; it does not turn the LLVM triple
into a plain `wasm` Go architecture tag. Requiring plain `wasm` alone therefore
excludes the real ROS SDK and typed example files under that pure target.

Use the actual pinned target's supplied `tinygo.wasm` tag. Do not inject
`-tags=wasm`: doing so can select standard-library architecture-specific files
for two Go architectures. The alternative in the platform guard preserves
existing wasm-architecture targets and admits only the real TinyGo Wasm
platform on this new route; it grants no ROS operation or native transport
permission. Backend selection, every source body, ABI, ownership, bounded
reactor configuration and full binary/import/start checks remain unchanged.
Actual compilation and initialization still require their own results.
