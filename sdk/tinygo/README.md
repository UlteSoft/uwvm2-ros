# roswasm TinyGo guest package

Import `uwvm2.local/roswasm` through a local Go module replacement to this
folder. Select exactly one backend with `tinygo`, `wasm` and `ros1`/`ros2` build
tags; use the `wasi` TinyGo target with explicit compiler support for
`//go:wasmimport`. Supplying neither or both backend tags fails selection.

Raw declarations cover all38 ROS1/all43 ROS2 imports with uint32 memory/error
and uint64 offset/length/handle fields. Capability wrappers use only wasm32
memory-zero pointers, shared retirement state and explicit Close; codecs use
exact integer fields and bounded canonical wire. Node/resource factories open
existing aliases and never grant rights. Executor dispatches callbacks only
when the guest calls SpinOnce.

Read [the detailed language guide](../docs/rust-tinygo.md) and the nine
progressive examples in `../examples/tinygo`. Public API/field comments are
English. Codec tests require only the standard library and use57 positive plus
11 structural negative independent wire vectors. No external package is
required or downloaded by this SDK.

Sources/tests are prepared. Actual TinyGo compilation, emitted import signature
inspection and native ROS execution remain pending. A native Go codec test
alone does not validate TinyGo WebAssembly code generation or ROS behavior.

## Ownership groups and checked callbacks

`NewSession(maximumResources)` checks wasm32, ABI version1 and a local1..1024
live-handle budget. `session.NewNode(label)` attaches factories to that group.
Neither constructor creates a native Context/identity, connection or right.
`Session.Close` and `Node.Close` mark closed first, attempt all live resources
once and return every error; inspect all outcomes and preserve earlier failures.
Retained typed wrappers share retirement state and return9 after group shutdown.
Repeated Close also returns9 without an import. No finalizer or trap/panic-time
cleanup is promised. Every typed owner exposes checked Revoke and RawHandle;
servers expose RawHandles for their independent event/respond grants.

Factories roll back every successful acquisition when a later open/activation
fails, preserving the initial error. `Server.SpinOnce` validates a complete
Trigger/SetBool event and the handler's exact reply ID before responding.
`ActionServer.SpinGoalOnce` dispatches one bounded Fibonacci goal on the Guest
thread. Feedback, result and own/any/all cancellation remain separately granted.
Parameter arrays/events, graph, TF, clock/logger and ROS2 lifecycle reuse exact
existing imports; unsupported backend operations remain95. Custom native
services/Actions or native rcl interfaces are not synthesized by this SDK.

Prefer `NewCheckedExecutor` and `TryAddSubscription`; the former checks scratch
before allocation and the latter returns registration errors before mutation.
`RemoveSubscription` transfers ownership without a close. Executor.Close
attempts every registered queue, keeps the first failure and disables dispatch
with9. Existing callback-time mutation/nested spin refusals remain22; the old
void AddSubscription ignores refused registrations, leaving ownership with the
caller. Decode/copy callback bytes before retaining them across the next poll.

## Local scheduling without implicit permissions

`NewContext` creates a bounded local deferred queue and timers. Use package
function `Defer(context, function)` (Go methods cannot have type parameters)
to obtain `Future[T]`. Take returns pending11 or consumed9. Cancel completes
only not-started work with a local125 error; it does not revoke host authority
or interrupt a synchronous native RPC. CallbackGroup.Run rejects same-group
reentry with17. Context SpinOnce rejects reentry/mutation with17 and backwards
caller time with22. Timers accept explicit u64 nanoseconds and positive periods,
check overflow, and invoke at most once per pass without catch-up replay.
Shutdown cancels queued work and timers locally; close Session separately.

Lessons1/2 are raw and codec layers,3/4 are Node/resources,5 is `servers`,6 is
ROS2-only `events_tf_lifecycle`, and7 is `scheduling`. Each lesson names all its
independent required aliases. Their checked shutdown inspects cleanup failures;
examples trap on an application error instead of reporting successful completion.
There is no automatic retry/resize after90 and no assumption that timeout undoes
an admitted native effect. Empty polls are ordinary idle and caller scheduling
is explicit.

For current local verification use separate `/tmp` outputs and one job:
`go test -p 1 ./...` from this module exercises codec/local-state tests with no
native ROS imports. TinyGo Wasm compilation requires explicit backend tags and
the wasi target; inspect every emitted import before running a Guest. Do not
infer TinyGo/native delivery from a native Go test. Historical cgroup receipts
retain their own budgets and source pins; they do not qualify this increment.

## TinyGo scheduler and WASI startup permissions

Build these caller-driven SDK lessons with `-scheduler=none`. They perform each
step explicitly and do not use goroutines, `time.Sleep` or a cooperative runtime
idle loop. The wasi target otherwise defaults to an asyncify scheduler, which
can retain implicit clock/poll dependencies. This compiler choice does not grant
WASI authority or make arbitrary application/library code clock-free: inspect
the exact imports of every generated binary and validate its actual startup.

The selected TinyGo0.43 WASIp1 runtime can still require startup entropy through
`hardwareRand` and libc `arc4random`. `-scheduler=none` does not remove that
independent dependency. A denied `random_get` may abort before main; there is no
documented noncryptographic fallback that this SDK relies on. Do not infer that
a successful compilation can start under only basic stdin/stdout/stderr rights.
The Runtime's default permissions remain empty beyond those basic streams.

Application-defined WASIp1 raw imports should use the exact core Wasm scalar
ABI. For example, `fd_read` and `fd_write` take four `uint32` arguments and
return `uint32`: fd, iovec-array offset, vector count and output-count offset.
In wasm32, pass each address as `uint32(uintptr(unsafe.Pointer(&value)))` at the
call site. Keep the actual iovec as two `uint32` fields (address then length,
eight bytes total), and the output count as a distinct four-byte `uint32` slot.
Retain the referenced data, iovec and count with `runtime.KeepAlive` after the
synchronous import, then check both errno and the exact returned count.

The selected compiler rejects a plain typed `*iovec` import argument when its
Go struct lacks the required host-layout marker. Scalar raw declarations avoid
that compiler-specific struct contract without changing WASI's memory layout.
The SDK's ROS raw imports already use their exact `uint32`/`uint64` scalar ABI;
do not narrow their64-bit offsets to WASIp1's32-bit offsets. Ordinary/custom
lessons using `os`/`io` retain the official runtime's own implementation; this
fix neither patches that runtime nor supplies a new I/O or permission fallback.

If the actual compiled program requires entropy, the operator must review and
explicitly select the separate WASIp1 `random` permission, preferably for its
specific module/Group. This is not a ROS grant and is never added by Session,
Node, Context, an example or an automatic retry. See the
[WASI CLI](../../documents/command-line/wasi.md) for exact scope/merge rules.
Alternatively select and qualify a freestanding target without that dependency;
it needs its own entry, imports and runtime support. Do not patch the official
TinyGo/libc runtime or pretend that a fallback changes the permission boundary.
Preserve the prior19-row compiler receipt as historical evidence; the new
scheduler recipe/import/startup checks require separate source-bound receipts.

## Additive unified event source contract

[Unified event v1](../docs/unified-events.md) specifies the separately granted
observer, complete64-byte envelope, seven kinds, Source/observer lease checks,
capacity-retaining poll, finite readiness and real bounded `service_submit`.
The new APIs are additive; local deferred tasks still advance synchronously
when explicitly scheduled, and do not gain native asynchronous authority.
No implicit alias, clock, thread, retry or stronger cancellation grant is added.
The six runnable unified-event lessons and source fixtures are prepared for the
unified serial test stage; dated historical34/39 receipts certify their original
revisions only.

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
