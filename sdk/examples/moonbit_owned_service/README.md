# Real caller-selected MoonBit ROS 2 control applications

These sources implement a complete retained GetTypeDescription client and a
GetParameterTypes server. The server uses the caller's local parameter table;
it does not pretend to query an existing native node. Both applications use
actual Session owners and the optional twelve-import retained Service ABI.
No native request, compiler, test or benchmark has run for this delivery.

Read `sdk/moonbit/owned_service/README.md` first. Do all preparation, generation,
compilation, checking and execution under the coordinated SSH Linux keeper in
the existing 64 GiB/swap-0 cgroup. A fresh directory is an owned output location,
not permission to select a different memory limit or execute outside the keeper.

## 1. Capture actual inputs

Use the exact reviewed SDK tree and actual pinned MoonBit compiler/core. Preserve
the original module layout: `sdk/moonbit` and this directory are separate local
workspace modules; this `moon.work` includes only this directory and the exact
`../../moonbit` SDK copy. Use a fresh admitted source/output tree for each build.
There is no package-registry fallback. Keep symbols and source maps.

Independently record full SHA-256 values for the actual generator entry and its
eight explicitly selected roles: `parent`, `bounded`, `emitter`, `services`,
`runtime`, `operator`, `catalog`, `wire`. They respectively refer to
`sdk/tools/custom_types.py`, `sdk/custom-types/bounded_profile.py`,
`sdk/custom-types/moonbit_profile.py`,
`sdk/moonbit/tools/control_services_moonbit.py`,
`sdk/custom-types/moonbit_runtime.mbt.in`, the generator entry itself,
`sdk/rmw_wasm/default_control/catalog.preparation.json` and
`sdk/moonbit/wire/wire.mbt`. Include all 25 actual original definition files.

Generate the canonical C/C++ bundle by the current authoritative preparation
workflow in `sdk/rmw_wasm/default_control/README.md`, using the same admitted
source inputs. Do not replace that workflow with a handwritten minimal schema.
Its fresh directory must contain `generation.json` plus all seven declared
canonical products. Canonical source output is not native compatibility proof.

## 2. Generate all records and typed Service wrappers

The following is a future command template. Every `*_SHA` value comes from the
reviewed captured input ledger; no digest shown here is a guessed placeholder
which the generator accepts. `PINNED_PYTHON` is the actual admitted interpreter,
and `ROOT`, `CANONICAL` and `GEN` are canonical absolute paths. `GEN` must be
absent and outside both source and canonical trees. The entry/interpreter/stdlib
must have been admitted before this command starts; self-reading cannot prove
process startup identity.

```sh
"$PINNED_PYTHON" -I -S "$ROOT/sdk/moonbit/tools/generate_control_moonbit.py" \
  --source-root "$ROOT" \
  --catalog "$ROOT/sdk/rmw_wasm/default_control/catalog.preparation.json" \
  --canonical "$CANONICAL" --out "$GEN" \
  --rmw-service-enabled 1 --backend 2 \
  --parent-sha256 "$PARENT_SHA" --bounded-sha256 "$BOUNDED_SHA" \
  --emitter-sha256 "$EMITTER_SHA" --services-sha256 "$SERVICES_SHA" \
  --runtime-sha256 "$RUNTIME_SHA" --operator-sha256 "$OPERATOR_SHA" \
  --catalog-sha256 "$CATALOG_SHA" --wire-sha256 "$WIRE_SHA"
```

Retain the full `generation.control-moonbit.json`. Inspect its complete 32
records, seven Service roots, native/codec identities, original member spellings,
default/constants/bounds and every product hash. Copy exactly the generated
`moonbit/` package into this fresh example module's `records/` directory; retain
the proof beside the build inputs. It contains complete `bounded_records.mbt`,
`control_services.mbt`, `default_contracts.mbt` and `moon.pkg`. Do not generate or
copy it into the public SDK or reuse a stale package from a different IR revision.

## 3. Select a real client or server

Supply your own bounded JSON file and its independently recorded full SHA-256
to `sdk/moonbit/tools/write_owned_selection.py`. This emits a fresh
`selection.mbt` plus `selection-proof.json`; it never changes a Host policy.
Capture the emitted Source and review it before compiling. Copy it into the
selected `description_client/` or `parameter_server/` package.

Both JSON forms have these required fields:

| Field | Exact meaning |
| --- | --- |
| `role` | `description-client` or `parameter-server` |
| `source_alias` | Explicit call or advertise CLI alias |
| `peer_alias` | Independent observe or respond CLI alias |
| `availability_alias` | A distinct client availability alias, or JSON null |
| `expected_qos_hex` | Two exact actual caller-observed protocol80 rows, each 160 lowercase hex characters |
| `maximum_body` | Explicit complete wire bound, 0 through 4193626 and sufficient for both actual selected codecs |
| `history_budget` | Lifetime bookkeeping budget 1 through 65536 |
| `poll_limit` | Explicit finite number of calls to `step`, 1 through 100000 |

The client additionally requires `type_name`, `type_hash`,
`include_type_sources` (an explicit JSON boolean; the original default is true)
and `availability_wait_ns` (0 through 60000000000). A nonzero availability wait
requires the independent availability alias. The requested type/hash must be a
real selection for your native type-description service, not a codec hash
substituted for a native RIHS identity. Responses retain complete parsed
descriptions, nested dependencies, raw source bytes and extra information.

The server additionally requires `parameters`, a list of at most 4096 unique
`{"name":"caller_parameter","type":2}` objects. Types 0 through 9 are the
original parameter type values; 0 means unset. Unknown names return 0, preserving
the original request order and arity. No environment, filesystem or graph query
provides hidden application state. For a device-backed resolver, use the same
`Application::step_parameter_types` API with an explicit local resolver and
separately reviewed/granted device integration; this Source table grants none.

All renderer strings are bounded printable ASCII. The original generated
canonical records still support the full bounded UTF-8 domain. There are no
empty alias defaults and no example QoS bytes presented as observed Native data.

```sh
"$PINNED_PYTHON" -I -S "$ROOT/sdk/moonbit/tools/write_owned_selection.py" \
  --selection "$SELECTION" --selection-sha256 "$SELECTION_SHA" \
  --out "$FRESH_SELECTION_OUTPUT"
```

The application module exposes a reusable interface rather than hiding the
protocol in the entry:

```moonbit
let app = match @app.prepare(selection) {
  Ok(value) => value
  Err(_) => abort("local preparation failed")
}
// Keep app even if start returns a nonzero status.
let status = app.start_parameter_types()
// The caller schedules explicit steps under its own overall deadline.
let step = if status == 0 { app.step_parameter_types(resolve_type) } else { status }
if step != 0 && step != 11 { let _code = app.fail(step) }
let cleanup = app.close()
if app.primary() != 0 || cleanup != 0 { abort("application or cleanup failed") }
```

An actual raw peek11 is the only idle poll. The application records a codec11
as primary failure, so its caller must also check `primary()` before polling
again. The shipped entries do this. They perform at most one admitted request
or one response, use a finite explicit poll count and close every partial open.
The client explicitly abandons its one admitted request on local poll timeout,
without replay or a DDS cancellation promise. Both entries refuse success on
cleanup failure. Fatal allocation/VM traps require actual Runtime retirement;
they do not guarantee language-level finally execution.

## 4. Build and admit the actual artifact

In the fresh admitted local workspace, build the selected real package using
the pinned `moon`, `--frozen`, targets `wasm` and `wasm-gc`, both debug and release.
For example the pinned toolchain's build interface is:

```sh
"$PINNED_MOON" -C "$EXAMPLE_PROJECT" build --frozen --target wasm --debug
```

Use separate fresh project/output revisions for other target/mode combinations.
Record the exact emitted path rather than guessing `_build` output names. Verify
that the selected entry's `_start` is the entry which will actually execute;
building unused packages is not an application run. Do not invoke `abi_coverage`
with any valid selector. This inspection-only export keeps all 55 import
signatures reachable and its zero placeholder arguments are not business data.

Require an actual whole-Wasm validator and the conservative
`sdk/moonbit/owned_service/check_final_imports.py` subcheck. Supply its own exact
Source SHA and the independent full signature table for all actual non-ROS
imports, including any compiler exception tag. The table is exact, not a
module-wide allowance. The checker also checks real memory/data declarations.
Its CLI includes `--source-root`, `--wasm`, `--target`, `--entry-sha256`,
`--external-signatures`, `--external-signatures-sha256` and a fresh `--report`.

Without independently reviewed actual compiler/link layout and allocator
evidence it returns HOLD/3. `--compiler-link-admission` and
`--compiler-link-admission-sha256` select the separately admitted receipt whose
closed schema is documented at the top of the checker Source. An arbitrary
JSON assertion is not evidence or authority. Even an accepted receipt only
completes a signature/static-layout subcheck; the checker explicitly does not
prove whole runtime memory safety. Preserve that limitation in build reports.

## 5. Run against real native ROS

Select actual native ROS 2 Services with reviewed full native headers/types,
identities, both actual QoS rows and finite quotas. Use explicit matching call /
observe / availability or advertise / respond grants in the real runtime CLI.
Do not use a Topic alias for a Service or map a legacy native process/library
into Guest memory. Run under a finite external deadline with bounded stdout,
complete exit/cgroup facts, all physical cleanup evidence and an independently
observed real response. A success marker is useful only with a real zero exit,
the exact artifact/policy/input identity and actual native observation.

The entries print one inert ASCII marker **after** all cleanup succeeds. Output
uses the explicit local stdio package and one checked WASIp1 `fd_write`.
Descriptor/data `[0,289)` and the ROS arena must be verified against actual
static DATA and allocator codegen. Do not replace it with an unreviewed println
shim or silently allow additional imports.

For an application which continuously serves a robot, replace the finite sample
entry with a caller-owned scheduled executor, bounded history/quotas and an
explicit shutdown policy. Keep full metadata/body validation before consume ACK
and execute application work after ACK. Native device/Gateway privileges remain
separately auditable Host policy. For ROS 1/2 mixed applications use distinct
Instances/backends and a reviewed Gateway, not automatic cross-backend fallback.

## 6. Qualification, debugging and performance

`tests/ros_moonbit_owned_service/guest` is a future real Guest value fixture.
Stage it as another package in this exact local example module, with the actual
generated records. It checks every complete record's default roundtrip plus an
independent query vector, all three keyword fields, raw non-UTF8 bytes, a full
signed scalar boundary and the actual CONTROL Time domain. It is not a Native
transport model and has not run. Keep independent C/C++ cross-language vectors
and all true Host/native negative and cleanup tests separate.

Test the real entries' nonzero exit on business, resolver and each independent
cleanup failure; checking only an endpoint method is insufficient. Capture
source-map/DBG behavior and ROS 2 debugging with exactly the same compiled
artifacts. An instruction offset, function boundary and actual native effect
are distinct observations; do not claim Python/Java/native language locals from
Wasm metadata alone. Include the current binary-SHA cache, CPU feature and
WASM 3.0/WASI/ROS compatibility/refusal tests.

Run x86_64 and aarch64 QEMU plus the additionally supported ROS architectures in
the existing cgroup. Benchmark real native work before/after migration with
VTune hardware counters/timing, Hotspots and Microarchitecture Exploration.
Compare Wasmtime/WAVM/WasmEdge only under equivalent actual supported host ABI,
inputs, policies, lifetimes and compiler settings. All of those execution,
debugging, architecture and performance results remain unmeasured here.
