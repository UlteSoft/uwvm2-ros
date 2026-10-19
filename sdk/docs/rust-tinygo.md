# Rust and TinyGo guest development

These packages provide the actual `uwvm_ros1_v1` and `uwvm_ros2_v1` ABI v1
imports, bounded canonical builtin codecs, owned capabilities, and familiar
application objects. They do not link native ROS libraries. The Node,
Publisher, Subscription, Client, Server and Executor APIs are designed for a
capability sandbox; they are not source or binary compatibility with roscpp,
rclcpp, rclrs or native Go ROS clients.

**Validation status:** the frozen r14 compiler matrix passed its Rust and TinyGo
Wasm compilation, binary import inspection and full-validation rows. Its model
matrix separately passed the native Rust and Go models. See the
[exact SDK acceptance receipts](../../documents/ros/sdk-current-20261014.md).
These results qualify the frozen source, not every language's ROS delivery.
Subsequent SDK changes require fresh compiler/model receipts. Actual acceptance
commands must execute in the authorized Linux 64 GiB (`68719476736` bytes),
zero-swap cgroup. Static registry/signature checks, formatter parsing and
generated golden tests do not establish native interoperability or ARM behavior.

## Files and progressive learning order

| Stage | Rust | TinyGo | What to learn |
| --- | --- | --- | --- |
| Exact imports | `rust/src/raw/ros1.rs`, `ros2.rs`; `examples/rust/raw.rs` | `tinygo/raw_ros1.go`, `raw_ros2.go`; `examples/tinygo/raw/main.go` | Explicit module/name, integer widths, memory index and LE output slot |
| Checked values | `rust/src/codec.rs`, `typed.rs`; `examples/rust/typed.rs` | `tinygo/codec.go`, `typed.go`; `examples/tinygo/typed/main.go` | Canonical typed wire, finite floats, exact signed integers, bounded scratch |
| Application objects | `rust/src/node.rs`; `examples/rust/node.rs` | `tinygo/node.go`; `examples/tinygo/node/main.go` | Owned resources and guest-thread polling callbacks |
| Other resources | `examples/rust/resources.rs` | `examples/tinygo/resources/main.go` | Operation-specific service, Action, parameter, graph, clock and log grants |
| Codec regression preparation | `rust/tests/wire_vectors.rs` | `tinygo/wire_vectors_test.go` | 57 independent positive vectors and 11 structural refusal vectors |

All paths in this table are relative to `sdk/`. Read the raw declarations first.
Use checked wrappers unless your module deliberately manages a nonzero linear
memory. Public APIs and examples contain English documentation at the call site.

## Authority is explicit

Creating a Node grants no resources. A Node name is a local application label;
it does not claim a native node identity. Opening `pub` uses an alias explicitly
configured for this guest by the host. The alias fixes operation, exact resource,
verified type identity, QoS, byte/rate/queue/deadline limits and backend. Guest
code cannot turn that alias into another topic, parameter or native endpoint.

Every operation has a distinct grant. Advertising a service or Action does not
grant responding; sending a goal does not grant receiving, own-cancel or
any-cancel. Scalar parameter get/set, metadata and parameter-event subscription
are distinct. Graph inspection, clocks, logging, TF and lifecycle operations
also require explicit aliases. Array parameter grants additionally require a
positive element limit; numeric scalar/array grants require inclusive value
bounds. Codecs and fixed guest buffers cannot increase any of these limits.

Use the repository's [ROS1 CLI](../../documents/command-line/ros1.md) and
[ROS2 CLI](../../documents/command-line/ros2.md) instructions to configure exact
grants. The current WASIp1 environment starts without host environment
inheritance, clock, random, signal or scheduler authority, and its standard
streams have the documented basic rights with zero inheriting rights. When ROS
is enabled, every applicable WASI target must preserve environment isolation;
an explicit noinherit option can document that choice. WASI permissions and
the language's WASI runtime requirements are separate from ROS grants. A Rust
or TinyGo timer or entropy call needs its own explicit WASI permission; this
SDK never adds one. See [current security boundaries](../../documents/ros/security-current-20261014.md),
[host setup](host-setup.md) for target selection and
[verification](verification.md) for the exact pending acceptance scope.

## Build selection and package use

The Rust library is `no_std`. It has no default ROS feature, no external crate
dependency and no allocator requirement for raw imports, Capability, fixed-buffer
codecs and the basic Node/resource layer. Enable `alloc` for owned Image,
Transform, parameter/event values, typed publishing and the registering Executor.
Supply an allocator when integrating `alloc` into a freestanding guest; a normal
WASI binary using Rust's standard runtime supplies its usual allocator.

For an application dependency, set an explicit feature:

```toml
[dependencies]
roswasm = { path = "/path/to/uwvm2-ros/sdk/rust", default-features = false,
            features = ["ros2", "alloc"] }
```

The library can enable both `ros1` and `ros2`; the application then explicitly
chooses `Backend::Ros1` or `Backend::Ros2` for each capability. Handles remain
bound to that choice. The examples deliberately require exactly one backend
feature so a beginner cannot silently select the wrong native stack.

With the selected compiler target already installed, the conventional example
commands are:

```sh
WASI_SDK=/absolute/already-installed/wasi-sdk
export CARGO_ENCODED_RUSTFLAGS
CARGO_ENCODED_RUSTFLAGS="$(printf '%s\037%s\037%s\037%s\037%s\037%s' \
  '-C' 'codegen-units=1' '-C' 'link-arg=--threads=1' \
  '-C' "link-arg=$WASI_SDK/share/wasi-sysroot/lib/wasm32-wasip1/libc.a")"
cargo build --offline -j1 --manifest-path sdk/examples/rust/Cargo.toml \
  --target wasm32-wasip1 --no-default-features --features ros2 --bin raw
cargo build --offline -j1 --manifest-path sdk/examples/rust/Cargo.toml \
  --target wasm32-wasip1 --no-default-features --features ros2 --bin typed
cargo build --offline -j1 --manifest-path sdk/examples/rust/Cargo.toml \
  --target wasm32-wasip1 --no-default-features --features ros2,alloc --bin node
cargo build --offline -j1 --manifest-path sdk/examples/rust/Cargo.toml \
  --target wasm32-wasip1 --no-default-features --features ros2,alloc --bin resources
```

Replace `ros2` with `ros1` to build for ROS1. These are prepared build recipes,
not a report that this target toolchain has compiled the new SDK. The explicit
Rust import-module attribute is described by the
[Rust Reference](https://doc.rust-lang.org/reference/items/external-blocks.html#r-items.extern.attributes.link.wasm_import_module).
Inspect emitted modules against `sdk/abi/abi-v1.json` after compilation; a single
backend build must not require the other ROS import module.
The exact WASIp1 C archive supplies compiler-generated memory intrinsics;
`no_std` does not mean that slice comparisons cannot lower to `memcmp`.
The shared compiler recipe pins the archive and uses independent encoded
tokens even when paths contain spaces. There is no target-family fallback,
host library lookup, custom intrinsic implementation or new WASI grant.
The import checker still rejects `env.memcmp`. Remove the exported encoded
flags before unrelated native Cargo model builds.

TinyGo uses one backend per guest: `tinygo`, `wasm` and exactly one of `ros1` or
`ros2` select the raw imports. Supplying neither or both ROS tags intentionally
fails selection. For an application module, require `uwvm2.local/roswasm` and
replace it with the local `sdk/tinygo` directory. The examples include that local
replacement and download no package dependencies.

Use a TinyGo release that supports the `//go:wasmimport` directive and the
standard packages used by these codecs. The directive and explicit
`uint32 -> i32`, `uint64 -> i64` mapping are documented by the
[Go compiler](https://pkg.go.dev/cmd/compile#hdr-WebAssembly_Directives).
TinyGo's WASI target workflow is described in its
[official guide](https://tinygo.org/docs/guides/webassembly/wasi/).
The compiler release and emitted signatures still need actual project validation;
do not infer TinyGo code-generation correctness from the Go reference alone.

```sh
cd sdk/examples/tinygo
tinygo build -target=wasi -tags=ros2 -o /tmp/roswasm-raw.wasm ./raw
tinygo build -target=wasi -tags=ros2 -o /tmp/roswasm-typed.wasm ./typed
tinygo build -target=wasi -tags=ros2 -o /tmp/roswasm-node.wasm ./node
tinygo build -target=wasi -tags=ros2 -o /tmp/roswasm-resources.wasm ./resources
```

Replace `ros2` with `ros1` for ROS1. A dual-stack TinyGo application can use
separate ROS1 and ROS2 guest modules, each with its own import module and grants;
this package does not silently forward requests between backends.

## Memory, ownership and errors

Every memory index is `i32`; offsets, lengths, capacities and handles are `i64`
on wasm32 as well as memory64. The import result is an unsigned 32-bit code,
except `abi_version`, which returns version1. Raw declarations use exact integer
widths. No length, handle, timestamp or signed parameter value is converted
through floating point. Returned handles and lengths occupy **eight
little-endian bytes**, even on wasm32. Do not cast an unaligned byte buffer into
an aligned native integer pointer.

Convenience wrappers accept ordinary wasm32 pointers into memory zero. They
reject unsupported pointer widths before conversion; they do not truncate a
64-bit pointer into a 32-bit address. For an explicitly managed nonzero memory,
call the raw import with its memory index and integer offsets. Language pointer
helpers do not establish that a compiler produces usable multi-memory/memory64
modules; those target/compiler configurations require their own validation.

The host copies bytes synchronously during an import. Rust slice borrowing
prevents overlapping request/output buffers. TinyGo Request explicitly rejects
nonempty overlapping slices and keeps referenced storage alive through the call.
Neither package retains guest pointers at the host nor receives an asynchronous
native ROS callback.

A Rust Capability is not Clone/Copy. Drop attempts close, while explicit close
reports notification errors. Go callers receive `*Capability` and must use
`defer Close()` after successful open. Go copies share a private retirement
state; copying a wrapper does not create independent ownership. No finalizer or
background goroutine closes resources nondeterministically. Server constructors
close already opened handles if a later open/activation fails.

Close and revoke retire local ownership before invoking the host. A transport
error can follow host retirement, so the wrapper will not reuse or automatically
retry that handle. Revocation invalidates all current handles for the alias;
later uses can report9 and reopening can report125. The SDK never constructs a
handle generation/slot or fabricates a service request or Action goal ID.

| Code | Application interpretation |
| --- | --- |
| 0 | Success; decode only the returned initialized prefix |
| 2 | Selected object/parameter missing |
| 9 | Invalid, foreign or retired handle |
| 11 | Empty event queue or an explicit quota refusal; schedule another poll if appropriate |
| 13 | Operation/payload denied by explicit policy |
| 17 | Conflicting configured contract |
| 22 | Invalid argument/memory/canonical structure |
| 90 | Capacity or byte quota refusal; a length slot may remain untouched |
| 95 | Unsupported backend operation, adapter or pointer model |
| 107 | Gateway disconnected |
| 110 | Deadline exceeded; an already admitted external effect may remain |
| 125 | Alias revoked |

Wrappers initialize output-length slots to `UINT64_MAX`. On90, Rust
`Error.required` or Go `Error.HasRequired/Required` reports a length only if that
sentinel was overwritten. A preliminary byte-quota refusal may leave it intact.
An insufficient receive buffer may already have consumed the queued event, and
an RPC may already have performed an admitted effect. There is no automatic
buffer growth, receive retry or write retry. The length is diagnostic, not proof
that an identical request can be repeated without consequence.

## Canonical typed values

The fixed-buffer codecs cover UTF-8 String, Bool, finite Twist, Trigger/SetBool
request/response, service request IDs/replies, Fibonacci goal IDs/feedback/result,
and exact u64 clock nanoseconds. `codec::RequestId`/`GoalId` and Go
`RequestID`/`GoalID` are constructed by checked decoding of Gateway-issued IDs.
The Gateway remains the authority on ownership and pending request/goal state.

ROS1 Image retains its full u32 seconds field; ROS2 Image is further restricted
to nonnegative i32 seconds by the selected host adapter. Both Transform adapters
and parameter-event stamps retain their nonnegative i32 seconds bound. No
backend timestamp field is truncated or silently wrapped.

The owned codecs cover Image, TransformStamped, all supported parameter scalar
and array adapters, legacy String parameter events, version2 typed events
including the separate deletion-only tag0, and lifecycle state. Rust uses the
`Canonical` trait and `ParameterValue::encode/decode`; Go uses `Message.Encode`
and `DecodeParameter(kind, bytes)`.

Parameter integers preserve signed widths: i32 is ROS1, i64 is ROS2. Bool arrays
and unsigned byte arrays are different types despite sharing a one-byte element
width. Float64 scalar/array/Twist/Transform values must be finite, including when
native policy uses no optional finite validator. Signed zero retains its IEEE754
bits. Array counts are u32 and all decoders reject trailing bytes; String arrays
are UTF-8 and NUL-free. ROS1 native String arrays additionally enforce XML1.0
validity, which the host checks independently.

Typed event tags0..9 keep the current wire values. Tag0 is a separately granted
NOT_SET deletion notification with no prior type/value attestation; only kind3
and empty selected payload are valid. Existing tag1..9 deleted events also carry
empty selected payloads and cannot re-prove native prior contents. New/changed
events require the selected canonical value kind and strict node/name grammar.
The outer payload length is distinct from a String's inner byte length and an
array's element count. Scalar String NUL allowance belongs to the canonical
wire; existing native RMW interoperability limits are documented in
[the ABI guide](../../documents/ros/abi.md).

## Service and Action server recipes

Service events and replies use different aliases. Poll once, treat11 as idle,
decode the owned request ID, perform the application work, and respond with the
same ID using the independently granted response capability:

```rust
let server = node.server("set-bool-advertise", "set-bool-respond")?;
let mut event = [0u8; 32];
match server.poll(&mut event) {
    Ok(n) => {
        let (id, requested) = roswasm::codec::decode_set_bool_event(&event[..n])?;
        let mut reply = [0u8; 128];
        server.respond_bool(id, true, if requested { "enabled" } else { "disabled" }, &mut reply)?;
    }
    Err(e) if e.code == roswasm::codes::AGAIN => {}
    Err(e) => return Err(e),
}
```

TinyGo follows the same explicit structure:

```go
server, err := node.CreateServer("set-bool-advertise", "set-bool-respond")
if err != nil { return err }
defer server.Close()
var event [32]byte
n, err := server.Poll(event[:])
if err != nil {
    if e, ok := err.(ros.Error); ok && e.Code == ros.Again { return nil }
    return err
}
id, requested, err := ros.DecodeSetBoolEvent(event[:n])
if err != nil { return err }
var reply [128]byte
return server.RespondBool(id, ros.ServiceResponse{Success: true,
    Message: map[bool]string{true: "enabled", false: "disabled"}[requested]}, reply[:])
```

For Trigger, use the empty request helper and decode the event as an eight-byte
RequestId, then return Bool+String. Never reuse an ID after response/deadline,
revocation or disconnect; never synthesize a reply ID from another queue.

ActionServer similarly opens `fibonacci-advertise` and `fibonacci-respond`, polls
an incoming Gateway-issued ID/order, computes the bounded Fibonacci work, then
calls `respond_fibonacci`/`RespondFibonacci` with kind1 feedback,2 success,3 abort,
or4 cancellation. Kind4 is accepted only for a native goal actually cancelling.
Host limits still apply to sequence count, bytes, rate and pending goals. Goal
ownership and native state cannot be inferred from the codec alone.

An ActionClient object intentionally owns **one operation** grant. Use separate
objects for `fibonacci-send`, `fibonacci-feedback`, `fibonacci-result`,
`fibonacci-cancel-own` and, only if explicitly intended, `fibonacci-cancel-any`.
Only `cancel_all` / `CancelAll` writes goal zero, and it dispatches exclusively
through the independently granted any-cancel operation. This stronger authority
is never opened automatically. Rust GoalId construction remains nonzero; the Go GoalID zero value is invalid
and typed own/known-any-cancel, feedback and result methods reject it before
calling the host. Keep the
relevant send/advertise resources open during the goal workflow; close, revocation,
deadline or disconnect can retire its ownership.

Pass the checked GoalId obtained from send or server polling to the receive/
cancel methods. A received-result buffer error does not justify blindly retrying
terminal consumption.

## Parameters, graph, clocks, TF, lifecycle and logs

These resources have typed ordinary methods and explicit canonical-byte escape
methods for their full registered operation set:

| Resource | Rust examples | TinyGo equivalents | Required grants |
| --- | --- | --- | --- |
| Parameters | `get_value(kind, out)`, `set_value(value)`, `declare_value(value)` | `GetValue`, `SetValue`, `DeclareValue` | Each exact scalar/array operation independently |
| Parameter events | `parameters(alias)?.subscribe_events()` then Subscription poll | `SubscribeEvents` then Poll | Separate exact legacy/typed projection subscription |
| Graph | `snapshot(Operation::GraphListTopics, out)` | `Snapshot(OpGraphListTopics, out)` | Exact bounded list/inspect filter |
| Clock | `read(Operation::TimeReadRos)` | `Read(OpTimeReadRos)` | Exact clock-domain alias |
| TF | `lookup_transform(time_ns, out)`, `broadcast_transform(value)` | `LookupTransform`, `BroadcastTransform` | Separate lookup/broadcast and exact frame pair |
| Lifecycle | `state(out)`, `change()` | `State`, `Change` | ROS2 get/change; transition fixed in the change grant |
| Logger | `write(text, scratch)` | `Write` | Exact allowed log level and byte/rate budget |

The byte methods retain full ABI naming: ROS1 parameter delete/exists/search,
ROS2 describe/declare/undeclare, graph topic/node/service/action inspection,
lifecycle get/change, TF lookup/broadcast and all clock domains are available.
A method belonging only to the other backend returns95; it never calls the other
backend's import module.

For typed parameter events, decode with `TypedParameterEvent::decode` (Rust
`Canonical` trait) or `DecodeTypedParameterEvent` (Go). Use a separately
configured host alias for exactly one selected node/name/tag. A deletion-only
notification has no value to merge into a cached typed parameter. Do not claim
that an absent wire value proves its prior native type or numeric bounds.

TF lookup supplies an exact u64 timestamp; zero selects latest. The grant fixes
parent/child frames. Static cache replacement and dynamic history semantics
remain host-controlled. Transform codecs preserve finite values without silently
normalizing quaternions. ROS2 lifecycle change supplies an empty request: the
configured transition cannot be overridden by guest payload.

## Polling and tests

Rust's basic `node::spin_once` needs no allocation. The optional registering
Executor owns subscriptions and callbacks with one fixed scratch buffer. TinyGo
Executor uses explicit Close and the same one-pass polling model. In Go, nested
SpinOnce and callback-time Close are refused with22 before scratch/ownership
changes. AddSubscription ignores nil arguments and callback-time registrations;
the caller retains that subscription and should register it after the pass.
The deferred guard restores state on normal return and on native Go recoverable
panic. This is not a TinyGo/WASI panic-recovery or cleanup guarantee: the
[official TinyGo language guide](https://tinygo.org/docs/reference/lang-support/#recover-builtin)
describes target-dependent recovery/defer limitations. A terminating panic/trap
requires the Runtime/Gateway to retire resources through their normal session
cleanup; the SDK cannot guarantee that Close/Drop ran. Private pure event-source
fixtures exercise the exact common
executor without pretending that a mock source is a native ROS subscription. Callback
bytes are borrowed only until the next poll overwrites scratch. Decode/copy
before storing them. Non11 errors stop the pass; there is no implicit sleep,
background worker, foreign callback, ROS spin thread or unbounded idle loop.

The checked-in golden tests are generated from the shared language-neutral
fixtures, with i64/u64 JSON values treated as decimal strings rather than floats:

```sh
python3 -B sdk/rust/tests/generate_vectors.py
python3 -B sdk/rust/tests/check_sources.py
```

These commands only generate/check source. Actual Rust codec tests can be run
with `cargo test --manifest-path sdk/rust/Cargo.toml --no-default-features
--features alloc`; Go's codec tests can be run from `sdk/tinygo` with `go test`.
They exercise codec/output-slot logic without claiming native ROS execution.
Project validation must run their compilation/runtime in the authorized cgroup,
then separately compile each Wasm example and verify imports and live ROS
behavior. A native Go codec test does not validate TinyGo's Wasm import ABI.

## Compiler-only full raw inventory probes

`examples/rust/coverage.rs` and `examples/tinygo/coverage/` retain all38/all43
raw functions for compiler/import-table checking. They are deliberately separate
from the four progressive runnable examples. Their exported
`roswasm_abi_coverage(index)` uses zero-valued placeholder arguments; **never
execute this probe** as a ROS behavior test. The empty main makes no ROS call.
For Rust coverage, retain the export explicitly:

```sh
RUSTFLAGS="-C link-arg=--export=roswasm_abi_coverage" \
  cargo build --manifest-path sdk/examples/rust/Cargo.toml \
  --target wasm32-wasip1 --no-default-features --features ros2 --bin coverage
```

For TinyGo, compile `./coverage` from `sdk/examples/tinygo` with the same selected
backend tags. Its `//go:wasmexport` requires explicit compiler support. Check the
produced module with `tools/check_wasm_imports.py --backend 2 --require-all`
(and only explicitly allow unavoidable language-runtime import modules), then
use a full Wasm validator. These compiler recipes remain prepared and must run
in the authorized cgroup for the current project's validation; the source probe
alone does not prove imports were retained or ROS executed.
