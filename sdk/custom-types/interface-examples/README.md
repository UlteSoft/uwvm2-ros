# Generated typed custom service and action facades

These modules adapt complete bounded-v2 custom types to the public ROS Wasm SDK.
They are an API for this capability ABI, not source compatibility with rclcpp,
rclpy or another native client library. Builtin Trigger, SetBool and Fibonacci
factories continue to use their existing routes and codecs unchanged.

`catalog.preparation.json` contains a small ROS2 Increment service and Count
action, with every request/response/goal/result/feedback field present. Its
native identities are deliberately absent. A guest-only source emission can
teach and compile the API; it cannot register a native type or establish a ROS
behavior result. For native use, first bind the real official root and component
identities using the existing native identity workflow and bounded catalog
binder. Then generate from that real bound catalog and select the matching
Gateway registry. Do not insert synthetic hashes into a deployed catalog.

## Generate and package

After Root applies and reviews the source patch, an explicit owner may run:

```sh
python3 -B sdk/tools/custom_types.py \
  sdk/custom-types/interface-examples/catalog.preparation.json \
  --guest-only --out sdk/custom-types/generated-rpc
```

The destination must be fresh. For actual validation, Root should use its
temporary complete source work area inside the existing 64GiB cgroup; this
documentation command is not a claim that generation or compilation ran.

An actual Service/Action catalog adds `bounded_interfaces.rs`, `.mjs`, `.go`
and `.ts` to the existing generated C/C++ interfaces and six record codecs.
All-message catalogs retain their existing eleven native-emitting product
bodies, or eight guest-only bodies, without reading these templates. Catalogs
with RPCs produce fifteen native-emitting products, or twelve guest-only
products. `generation.json` binds every emitted output and the exact four used
template bodies in `source_inputs.facade_templates`. Each template entry uses
`generator_relative` relative to `bounded_profile.py`, plus complete byte count
and SHA256. Templates are re-read before any output directory is committed.

JavaScript and AssemblyScript generated modules are designed for a directory
directly under `sdk/custom-types`, such as `generated-rpc`. Their two-parent SDK
imports then resolve to the real `sdk/javascript` and `sdk/assemblyscript`
modules. Moving the generated directory requires explicitly updating those SDK
import paths as a new source revision; do not silently load an ambient package.
The sample `.mjs` and `.ts` modules demonstrate the resulting relative imports.
Embedded JavaScript must include every actual imported generated/SDK module in
its existing closed module manifest. Use a real Session/CopiedTransport bridge;
do not replace ROS imports with a JavaScript network implementation.

Rust uses the actual `roswasm` crate with `alloc` and explicit `ros1`/`ros2`
features. There is no default backend. The two `#[path]` declarations in
`example.rs` put generated records and interfaces in sibling modules, matching
the generated `super::bounded_records` import. Pass the actual chosen Backend
to `Client::open`/`Server::open`. The SDK does not expose a numeric ROS version
method on Backend independent of the consumer's Cargo feature names, so this
facade does not invent one or conditionally match on unrelated consumer flags.
The actual Backend and host alias bind native authority; async event metadata
also checks the generated component's exact ROS version.

For TinyGo, copy only the two generated Go modules into this example's fresh
`customtypes` subdirectory. `go.mod` names `uwvm2.local/customrpc` and replaces
`uwvm2.local/roswasm` with the real `sdk/tinygo` package. The generated record
package is `customtypes`; `example.go` imports that actual module path. Select
exactly one of `ros1` and `ros2`, with the existing TinyGo wasm build tags.
The facade file has the same tag boundary as the SDK's actual imports; pure
record values remain usable separately on a host. TinyGo's SDK backend is
selected by build tags, not by a caller-invented numeric runtime selector.

## Explicit authority and ownership

Constructing a typed value grants nothing. Opening under the default empty
policy returns Denied13. The application must supply every actual alias:

| Owner | Explicit aliases |
| --- | --- |
| Service client | service-call source; optional independent completion observer |
| Service server | advertise and respond, distinct aliases |
| Action client | send, cancel-own, feedback and result, all distinct |
| Stronger cancellation | optional cancel-any, independently granted |
| Action server | advertise and respond, distinct aliases |

Each role must already name the correct backend, operation, native type,
resource, QoS, quotas and deadline in host policy. The facade does not choose a
topic/service/action resource from a type name, request a socket or filesystem
right, read a clock, start a native context, or elevate cancel-own to cancel-any.
All aliases are acquired before any server advertisement. A later open or
activation failure rolls back every earlier capability once and preserves the
original error separately from each cleanup result.

Rust owners are non-Clone and rely on ordinary exclusive borrowing plus an RAII
callback lease. JavaScript uses private owner fields and `finally`, including
across `await`. AssemblyScript releases the lease on every normal pass; an
unrecoverable Wasm abort cannot promise recovery. TinyGo facade value copies
share a private pointer state, so copying a facade cannot reset its lease or
resurrect a retired capability. Use one caller-driven execution context rather
than concurrently operating the same TinyGo facade from goroutines.

Each server dispatch lease covers poll, complete decoding, the handler and its
response. Nested poll/dispatch/close/revoke returns Conflict17 before dequeue or
retirement. The exact responder remains usable inside an action handler. A
service handler returns one response; it must not also send that response
manually. An Action handler explicitly chooses feedback and terminal responses.
Handlers receive complete copied typed values and opaque request/goal IDs,
never a capability, native pointer or function pointer passed into ROS.

Use checked close/revoke in the application's finalization and inspect every
returned status. Close after an import failure still runs once per owned role.
A107/110 notification error can follow actual host retirement and external
effects; it is not permission to repeat close, reopen a revoked alias, replay a
request or infer that a robot command was undone. JavaScript has no GC finalizer
that invokes imports; Rust Drop is best-effort and does not replace checked
cleanup. Revocation is always explicit rather than construction rollback or
destructor behavior.

## Service calls and real asynchronous completion

Synchronous `call`/`Call` performs one actual service_call and publishes a value
only after the full generated response decoder accepts it. Empty service
components remain genuinely zero-byte values; no native struct is serialized.

To use asynchronous service calls, supply the independent completion observer
alias at construction. Its host masks must explicitly permit the response and
error events required by the actual `service_submit` implementation. The facade
owns and exclusively polls that fresh, initially unconsumed observer. Do not
also call legacy poll or unified event poll on it from another consumer.

`submit`/`Submit` invokes the real public SDK service-submit primitive. Its
nonzero full-width ID means admission only. There is no deferred synchronous
call, invented completion, thread, Future timer or automatic timeout replay.
The local pending table has an explicit capacity1..1024; exhaustion refuses
before another submit import. Host quotas remain independent and can refuse11
despite local room or a positive readiness snapshot.

`pollCompletion`/`poll_completion`/`PollCompletion` reads at most one actual
observer event, uses the existing SDK unified envelope decoder, then checks
backend, exact source, instance/session, next stream sequence and a locally
admitted request ID. A validated response uses the generated response codec.
A matched numeric error is a terminal completion with that original code.
Unknown/foreign/malformed events never become a typed response. Source-retired
metadata reports BadHandle while pending IDs remain local until explicit close;
it does not claim external effects were rolled back. Again11 is ordinary idle
or a quota refusal, not success. The application supplies all scheduling.

JavaScript's real Session scratch capacity must fit the largest declared
request and response, including the64-byte unified envelope and action ID/kind
prefixes. Rust, AssemblyScript and TinyGo allocate bounded per-operation output
buffers from generated maxima. None resizes and replays a consumed legacy
operation after TooLarge90. Retain the original error and any actual required
length instead of treating that length as a new grant.

## Actions and exact correlation

The four-role action client owns a bounded set of nonzero returned goals.
Feedback and result reject a foreign local ID before importing, and reject a
different ID in the returned envelope before decoding. Cancel-own accepts only
an owned goal. Cancel-any requires its explicit fifth role; zero means the
stronger cancel-all operation only in that method. A cancellation
acknowledgement does not remove a goal. Only a complete result with terminal
kind2(success),3(abort),4(cancel) and a fully valid generated component removes
that exact local goal. Feedback never retires it.

The server similarly records only successfully decoded incoming nonzero IDs.
Service response and Action feedback/result require a locally delivered ID,
while the host independently validates principal, resource, session, deadline,
one-shot response and revocation. Failed encoding or response retains local
ownership. Action feedback kind1 retains the goal; only successful terminal
response retires it. The sample action steps show these explicit operations.

## Authored verification and remaining actual work

`facade_generation_models.py` defines seven pure emission/adversarial regressions,
including message-only output stability, exact template SHA bindings, mutation
before commit, unknown placeholder refusal, no overwrite and ROS1 separation.
`facade_javascript_models.mjs` defines fifteen named generated-facade/public-SDK
in-memory cases for denial, partial rollback, callback unwind, capacity,
correlation, terminal retirement, real-submit selection and checked cleanup.
The existing bounded profile models change only their RPC catalog product
counts to include the four new modules. These are contract/model scopes and
never substitute for real native ROS or default-denied runtime tests.

All sources and examples in this revision are authored for review only. Root
must apply the reviewed patch into a new complete source freeze, execute the
generation/model suites inside its actual memory/storage guard, compile all
four language examples for their real targets, validate actual Wasm import and
memory boundaries, then run positive/denied Service/Action interactions before
claiming actual behavior. No model or compilation result is asserted here.

## Closed fresh four-language build preparation

The [closed recipe](build-recipes.json) and [source preparer](prepare.py) now
make the actual relative-import layout concrete. They do not compile or start
an application. Run them only through the existing bounded Linux owner, with
its explicit current 64GiB/swap-zero cgroup, low aggregate memory, disk reserve,
deadline and pidfd descendant cleanup. Keep all generated/staged outputs in
fresh owned temporary directories; never write into an immutable source freeze.

First run the real current generator with this actual catalog and `--guest-only`
into a fresh owned `GENERATED` directory. Then select the actual complete source
root and manifest, its real SHA256, all existing tools/SDK paths and another
fresh owned `OUT` directory:

```sh
python3 -B /actual/source/sdk/tools/custom_types.py \
  /actual/source/sdk/custom-types/interface-examples/catalog.preparation.json \
  --guest-only --out /actual/fresh-owned/generated
python3 -B /actual/source/sdk/custom-types/interface-examples/prepare.py \
  --source /actual/source --manifest /actual/source-manifest.json \
  --manifest-sha256 ACTUAL_MANIFEST_SHA256 \
  --generated /actual/fresh-owned/generated \
  --selection /actual/explicit-tool-selection.json \
  --out /actual/another-fresh-owned/rpc-examples
```

These are unexecuted invocation shapes, not future identity pins or a PASS
receipt. The tool selection is a closed JSON object. Its exact file/alias keys
are `PYTHON`, `CARGO`, `RUSTC`, `NODE`, `ASC_JS`, `TINYGO`, `WASI_CLANG`,
`WASM_LD_ALIAS`, `WASI_LIBC`, `WASI_LIBM`, `WASI_PRINTSCAN`, `WASI_BUILTINS` and
`WASM_TOOLS`. Directory keys are `RUST_SYSROOT`, `TINYGO_ROOT`, `GO_ROOT`,
`QUICKJS`, `WASI_SYSROOT` and `WASI_RESOURCE_DIR`. Every value must be an actual
absolute selected path. Preserve the actual `wasm-ld` alias spelling for the
multicall driver; its physical bytes are pinned separately. The owner remains
responsible for qualifying complete current toolchain/archive/SDK roots,
matching compiler/sysroot associations and the original complete QuickJS lock.
The preparer never downloads, discovers tools from PATH or silently substitutes
a different target/SDK.

The preparer requires all twelve real guest-only outputs plus `generation.json`.
It checks the actual current catalog, both complete definitions, the four
used templates and both actual generator source hashes. It copies the complete
manifest-selected SDK tree, retaining source-relative paths, then places every
generated output under `sdk/custom-types/generated-rpc` and the two actual Go
modules under the example's `customtypes` package. Every source/generated/tool
pin is reopened before the metadata-only report is published. There is no
fabricated codec, declaration stub, catalog identity or source rewrite.

The resulting `report.json` lists four actual command recipes, each with
explicit argv, cwd, environment and artifact role:

- Cargo compiles the real Rust example library for `wasm32-unknown-unknown`,
  offline with one job, the explicit actual Rust sysroot and actual SDK crate.
  The output is a Wasm-target rlib, not a linked/validated Wasm command.
- AssemblyScript compiles the real example and generated modules using the
  existing SDK abort boundary and an explicit maximum memory setting.
- TinyGo compiles the new [command wrapper](tinygo-main/main.go), which refers
  to the real RPC example API and checks one full generated canonical request.
  It opens no alias, runs no service/action and cannot certify native behavior.
  Its executable search path contains only the explicitly selected Go and
  TinyGo `bin` directories; it does not discover an ambient Go installation.
- Embedded JS uses the real pinned QuickJS preparer with a closed manifest that
  includes the actual example, both generated JS modules and actual SDK modules.
  Its entry is an unexecuted reactor library. Follow its returned actual jobs
  for every C translation unit, link and full validator; metadata preparation
  alone is not compilation and must not count as a successful JS Wasm product.

The external owner must execute argv lists without a shell, serially with all
resource/process guards, and retain actual exits, tools, source/copy/generation
pins, dependency files and link traces. Validate each actual Wasm artifact and
its import/memory/start boundary; an rlib requires a separate genuine consumer
link before it can be run. Any later native-positive/default-denied service or
action experiment additionally needs a real native-bound catalog, matching
Gateway registry, explicit finite grants and an independent native witness.
The preparer's `original_selected_file_inputs_preserved` flag covers only the
source, generated, metadata and physical tool files it actually opened and
reopened. It does not qualify complete toolchain directory contents or every
possible alias; those remain the external owner's independent obligations.
