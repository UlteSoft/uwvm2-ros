# Complete bounded custom client and server applications

Each entry genuinely calls imports and checks the complete operation/cleanup
flow. Source existence is not a successful compilation or native-delivery claim.
The [bounded profile](../BOUNDED_V2.md) defines complete component grammar and
trusted native identity requirements. Root's unified guarded lane performs all
actual compilation, full validation and native acceptance after source freeze.

| Language | Client entry | Server entry | Ownership and typed boundary |
|---|---|---|---|
| C11 | `client.c` | `server.c` | Existing caller-owned capabilities/Action role bundle; generated typed functions |
| C++20 | `client.cpp` | `server.cpp` | Generated nonmoving typed Client/Server owners, checked reverse cleanup and callback guard |
| Rust | `client.rs` | `server.rs` | Existing Capability ownership, Canonical trait codecs and private typed handlers |
| TinyGo | `client.go` plus one backend alias file | `server.go` plus one backend alias file | Existing Capability API and complete generated customtypes package |
| AssemblyScript | `client_ros1.ts`, `client_ros2.ts` | `server_ros1.ts`, `server_ros2.ts` | Selected NativeApi, generated codec classes, checked direct WASI stdio |
| JavaScript | `client.mjs` | `server.mjs` | AsyncSession/AsyncCapability over an actual persistent compiled Wasm relay |

C/C++ select exactly `-DUWVM_BOUNDED_ROS_VERSION=1` or2. These are standard WASI
`main` entries. They do not define a freestanding `_start`. C++ WASI recipes use
`-fno-exceptions -fno-rtti`; host fake-backend models may enable exceptions to
check RAII reset. Rust selects exactly one `ros1`/`ros2` feature and the SDK's
`alloc` feature in a fresh offline temporary Cargo binary workspace. TinyGo
selects exactly one backend tag and copies only one client/server main into its
module, with both tag-controlled `aliases_ros*.go` files and the generated
`example.local/bounded/customtypes` package. It replaces `uwvm2.local/roswasm`
with the exact owned SDK copy. AssemblyScript selects one backend entry and
exports `_start`; that entry calls `proc_exit` even on success. Inspect actual
startup imports for every language. Additional WASI startup rights are never
inferred from a failed run or silently added by the controller.

Build work places these files under `sdk/custom-types/bounded-examples` and copies
the exact frozen composite's `bounded` directory to sibling
`generated-bounded-v2`. The generator is not run during Guest compilation. Its
products, dependency inputs and whole tool/standard-library package roots must
already be pinned. JavaScript's actual controller entry is
[`run_custom_bounded_host.mjs`](../../../tests/ros_custom_types/bounded-v2/run_custom_bounded_host.mjs):
`node ... 1|2 client|server`, with exclusive Worker response fd3 and request fd4.
The host creates no Worker/Gateway/native entity or socket itself. A compiler-only
or mathematical JavaScript model cannot satisfy this selection.

## Exact application flow and native prerequisites

Clients explicitly acquire `bounded-echo`, `bounded-empty`, `bounded-send`,
`bounded-cancel`, `bounded-feedback`, `bounded-result`. Echo and Empty each call
once. Digest sends once, validates a nonzero returned ID and consumes exactly
one matching feedback followed by one kind2 terminal result. Up to200 basic
stdin `R` bytes release externally paced bounded polls;11 retains owners and the
same Goal. After exact feedback decoding, the client prints
`SDK_BOUNDED_FEEDBACK_DELIVERED` and reads one checked `E`. The controller sends
that byte only after permitting the independent native server's terminal
publication. Native server stdin `F` publishes its sole feedback; native stdin
`E` is withheld until the actual Guest feedback marker. Separate feedback/result
DDS channels therefore cannot use publication order as delivery evidence.
Send/call/cancel are never automatically retried. The independent
native server must use `/uwvm_bounded_v2/native_echo`, `native_empty` and
`native_digest`. All complete payload fields, nested arrays/strings and values
are independently checked before `PASS_BOUNDED_CUSTOM_CLIENT` after cleanup.

Servers explicitly acquire `bounded-service-events/replies` and
`bounded-action-events/replies`, mapped to `/uwvm_bounded_v2/guest_echo` and
`guest_digest`. They activate both advertise roles and print
`SDK_BOUNDED_SERVERS_READY`. Each external `R` permits one bounded pass. A complete
native Request/Goal is decoded, exact nonzero ticket retained and a typed Service
response is sent once. After its sole successful typed feedback response, the
server prints `SDK_BOUNDED_FEEDBACK_SENT` and reads one checked `E`. The controller
sends `E` only after `FEEDBACK_DELIVERED_BOUNDED_NATIVE`, which follows the
independent native client's complete matching feedback callback. The Guest then
sends its terminal result once. The C++ lesson uses the generated facade's
explicit `send_result=false` plus typed `respond_result` to retain the exact Goal;
it does not alter the default facade semantics. The server
prints `SDK_BOUNDED_REPLIES_SENT` and retains both native endpoint owners. It then
reads one checked `D`, supplied only after the independent native client confirms
actual response/feedback/result delivery. Cleanup follows, then
`PASS_BOUNDED_CUSTOM_SERVER`. This gate protects delivery lifetime; neither RPC
admission nor11 is treated as native completion.

The complete independent sample has label `bounded-native`, samples `[3,5,8]`,
points `[{-7,true},{11,false}]` and labels `[first,second]`. Echo limit is32.
Digest feedback is processed3/phase `accepted`; terminal result is acceptedtrue,
sum16/note `bounded-native-result`. The canonical Envelope is61 bytes; native
TCPROS/CDR is separately serialized by official support. ROS2 domain205 uses all
five explicit reliable/volatile/KeepLast4 Action QoS channels. ROS1 uses the
separately owned Master `http://127.0.0.1:11384` and TCPROS. The controller maps the
native ARM ELF and proves its current process birth before ROS initialization.

All acquired owners are attempted in reverse during cleanup, preserving the
first operation failure. C's optional `UWVM_BOUNDED_DENIED=1` checks six independent
open13 refusals and no live handles. The actual cross-backend default/format
negative matrix is separately implemented by the controller's real-import WAT
rows, with protected output sentinels, stale9 and independent zero native effects.
A negative row's PASS qualifies its refusal, not custom communication.
