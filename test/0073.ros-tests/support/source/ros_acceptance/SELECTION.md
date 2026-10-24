# Complete serial acceptance selection

These sources are prepared and have not been executed. Root freezes all
implementations before compilation or execution. Syntax, pure models,
compile-only receipts, document presence and historical PASS never supply a
current Runtime or native-delivery result.

Freeze SourceA first. Build official bounded-v2 ROS1/ROS2 packages, export
their actual identities through separately qualified full-MD programs, bind
the catalogs, generate bounded providers and compose them with unchanged
fixed-v1. Copy the complete bundle into a new immutable SourceB under
`test/0073.ros-tests/support/source/ros_custom_types/deployment-final/generated`. Freeze its full
manifest. All final Workers, Gateways, fixtures and SDK Guests select SourceB;
an earlier fixed-only product cannot substitute. Preserve SourceA derivation.

`make_manifest.py` takes explicit --source, --manifest, --selection, --out
and --execution-out. Plan output and future execution output are separate new
directories. Its selection JSON has format_version1, kind
`ros-final-explicit-selection` and the exact SourceB manifest hash. `inputs`
contains literal current sdk-workspace/qemu/binfmt-evidence/StartedAt,
Worker/report/config, both Gateways/reports, wat2wasm, prepared-sdk,
prepared-recovery, prepared-metrics, prepared-observer, delivery-products and
node. Paths may name earlier producing jobs' future output, with explicit
dependency order. No old boot/leaf inode/lease/StartedAt survives a reboot.

`jobs` lists all existing suites/preparations in order. Each job has id, driver,
arguments, requires, timeout_seconds, rss_bytes, raw_report, raw_success_path,
raw_success_value, source_association, raw_architecture and records. Reviewed
old argv/selectors are explicit because legacy schemas differ. Three data
references are allowed; shell interpolation is never performed:

| Reference | Meaning |
|---|---|
| `{"input":"worker"}` | Exact reviewed input literal |
| `{"output":"report.json"}` | This job's dedicated raw output |
| `{"product_of":"workers","path":"dual/uwvm2-ros"}` | Earlier raw product |

All old argv select the current source and dedicated raw output. A supported
manifest flag selects the current manifest. A helper without that flag keeps
its parser unchanged. `normalize.py` preserves the old report, argv, logs,
physical products and architecture. Its raw-manifest association checks the
original manifest field. A helper without that field may use qualified-worker:
revalidate current full Worker dependencies, original physical ARM product,
and the helper's actual Worker hash. Missing historical fields stay explicitly
NOT_RECORDED; no original receipt is modified.

Raw selectors are arrays of `{"key":"field"}`, `{"index":0}` or
`{"where":"name","equals":"exact-name"}`. Exact-one matching is mandatory.
Each record has id, kind, path and nonempty facts. A fact selects raw data and
uses only eq/length/gte/lte with an exact expected type; bool cannot be a count.
Actual commands require original outcomes/argv/log pins. A legacy driver
without individual command outcomes can select `{"driver_exit":true}`:
this records only the actual source-bound driver exit/logs and never invents
its unrecorded inner exits. Per-record explicit semantic facts remain required.

| Kind | Mandatory evidence | Scope |
|---|---|---|
| runtime | Actual selected ARM labels, original commands, exact raw facts | Selected real Runtime behavior |
| native | Same mapping/command evidence | Native fixture only |
| compiler | Original commands/logs and ARM_product_selectors | Actual built bytes; their execution is not inferred |
| bounded-rejection | Actual95, untouched outputs, independent zero effects, reason | Qualified supported scope; unsupported feature stays unsupported |
| material | Exact physical regular material pins | Delivered files; no Runtime execution |

Bounded rejection is allowed only for an original criterion explicitly
permitting a reason, such as sections13/27, with mapping
bounded_unsupported_allowed=true. Security requires expected_errno,
actual_errno, output_sentinels_unchanged, independent before/after native
effects, observer_kind, quiet_samples and quiet_ns. Each example binds the
seven-part material object plus actual Guest exit0 and independent native
verification. Material presence cannot satisfy a behavior record.

`coverage` names all26 mandatory base suites. The renderer adds the39 gap
cases. `original_requirements` maps every chapter35, phase12, criterion50,
attack55, example20 and output23 numeric string ID to nonempty launch/record
references. Missing IDs/raw facts, ambiguous rows, SKIP/BLOCKED, failed
dependencies, stale products and old source hashes keep acceptance incomplete.

Run generated `launches.json` using `run.py` with explicit --source/--manifest/
--sdk-workspace/--out/--qemu/--binfmt-evidence/--container-started-at and
--launch-manifest, through the current parent's exclusive lease and private
`unshare -m` namespace. Every nested driver also receives a private namespace.
All invocations are serial and failures remain in their own raw directories.

## Mandatory complete SDK supplement

The selection starts with `run_sdk_matrix.py`, before the four Worker builds.
Workers depend on its success. This adds the separate mandatory
`sdk-complete-matrix` scope while preserving all original65 behavior requirements
and their original numeric crosswalks. A `sdk-compiler-model` record establishes
host compiler, ownership and mathematical Wasm model checks only. Its ROS
delivery and ARM Runtime execution fields remain `NOT_EXECUTED`; it cannot
satisfy an original ROS, security, Runtime or example-delivery requirement.

Supply `sdk-matrix-tool-config` as an absolute path to a new immutable JSON
object with exactly `format_version`, `kind`, `tools` and `tool_roots`.
`format_version` is1 and `kind` is `sdk-final-tool-selection`. The `tools` object
has exactly these keys, each selecting an existing absolute physical path:

| Keys | Selected inputs |
|---|---|
| `python`, `cc`, `cxx`, `cmake` | Host Python, C/C++ compilers and CMake ELF files |
| `cargo`, `rustc` | Actual installed Rust toolchain binaries, without rustup proxies |
| `go`, `tinygo` | Installed Go and TinyGo binaries |
| `node`, `asc_js` | Node ELF and AssemblyScript compiler script |
| `wasi_sdk`, `wasm_tools` | Whole WASI SDK directory and validator ELF |

`tool_roots` lists complete canonical WASI, Rust, Go, TinyGo, Node,
AssemblyScript/dependency and validator package directories. Every package is
inventoried before and after execution; escaping links are refused. The four
host tools may instead be explicit standalone file roots. Their transitive
system headers, Python/CMake modules and libraries stay explicitly unqualified;
host assertions do not imply a complete operating-system dependency closure.

The adapter retains fresh SDK outputs below `/tmp`, uses one compile job and
a 512-MiB sampled actor-tree RSS guard, and checks all 349 current six-language
Wasm example/conditional products with fresh full validators and import
inspection. This includes four dedicated C/C++ complete-ABI compilation probes
(one per language and ROS backend), while retaining all 28 typed service/action
variants and checking their actual import subsets. The probes establish compile
and import coverage; they do not establish ROS execution or grant permissions.
It also requires installed/relocated SDK checks, current host
models, ten pure AssemblyScript fixtures/446 assertions,80 mathematical relay
cases and12 compiled scheduling executions. Separate bounded-profile and
feedback Python suites require17/9 named cases; actual host C++17/20 wire and
C++20 facade/exception-disabled models require67/67/27/26 assertions. These
models supplement, and do not replace, the30 real bounded custom ROS cases.

Mandatory base suites include four Worker profiles; original native/CLI/
security/payload/ingress/revoke/flood/multi-module/network; ROS1 search/ROS2
resources; Action policy and actual native40-field adoption; unified event
contracts and seven real kinds; old+new custom types/Guest delivery; signed
Enforce and revoke/fault/reconnect; WASI/Trusted-DL/Runtime regressions; bounded
service transport; native-versus-Wasm performance. `run_all.py` and its
mandatory four-profile regressions remain intact. Its documented nine-command
subset alone does not certify every other interpreter/JIT/checkpoint test.
CI-equivalent SSH execution and hosted GitHub execution are distinct facts.

Native preparation suites are sdk/recovery/bounded-service/metrics/
graph-gateway/events. They bind original full-MD and verbose compile/link recipes,
full SDK inventories/aliases, actual stopped ARM tools/products and cleanup.
`build_delivery.py` performs22 fresh builds/24 selections with explicit whole
tool/stdlib inventories, GOMAXPROCS1, Cargo/link jobs1,2GiB aggregate sampled
RSS and600s tool deadlines. Its result is PREPARED with ROS NOT_EXECUTED.
TinyGo startup clock/entropy is a separate explicit caller choice, never an
automatic retry grant. Current64GiB/swap0/ancestors/OOM/births must be rechecked.

JS runs outside the Wasm engine. A retained real C relay owns the Instance and
all38/43 selected ROS imports; only fd_read/fd_write/proc_exit are additional
WASI imports. Node receives copied results on private pipes and never connects
Gateway IPC or supplies an Instance identity. Positive/empty-rights rows share
relay bytes and use different explicit grants. Historical34/39 relay80 models
remain historical; additive38/43 products need fresh tests. Native queue
high-water is measured under its mutex; isolated socketpair overhead is
separate from authenticated IPC/permission/ROS cost. QEMU is simulation.

## Concrete current selection

`selection_template.py` supplies the concrete current case selectors and
original numeric crosswalk. It takes --source, --manifest, --inputs, --out and
--execution-out. Its input JSON supplies every literal named in COMMON_INPUTS
and LEGACY_INPUTS, including current preparations, the independent
expected-cgroup JSON, tools and products. It refuses SourceB without the
composed provider. It writes selection.json only; it does not run a compiler,
model, Runtime or native process. The first job rebuilds all four Worker
profiles; every subsequent selected product uses that exact current build.
Signed Enforce/reconnect receive --expected-cgroup with the parent's
independently measured current path/device/inode/boot ID.

The original-all job invokes run_all's full selection and resolves each
suites[].evidence through its actual path/SHA. RPC's denial-retention wrapper
uses pinned adapter_evidence. Unit and feature architecture references are
resolved individually. Per-record architecture_document prevents using a
network Worker's mapping for a native unit. Dynamic native snapshot labels
use mapped_launches: every retained original PID and required image must
appear in that same architecture's live samples.

Current network/RPC records add native_witnesses while preserving their
original assertions. Completed witnesses retain actual exit0 and full log
SHA. A persistent positive native source instead retains post-Guest liveness;
its natural terminal exit remains NOT_RECORDED. Count/absence observers must
finish with real0. The five-module projection retains eight exact topic/text
counts, two messages each, plus five distinct actual Instance identities.

The original55 includes positive memory growth, allowed2/denied98 floods,
revoke, Trap and native races. Their security_contract selects exact actual
assertions instead of pretending every test has zero total effects. Missing
individual import return values remain NOT_RECORDED. Attack10 selects original
graph_denied plus a separate native query-denial13 window. Attack44 selects
allowed2 then denied98/11. These windows have distinct native counter records.

The bounded-v2 derivation uses prepare.py --mode derive with the explicitly
selected unchanged fixed provider. Copy its entire26-member composite and
both qualified official package prefixes into new SourceB/new SDKWorkspace.
Then prepare.py --mode native and build_guests.py prepare SourceB products.
The selection runs30 bounded cases:24 client/server rows over the six
languages, plus two each of Topics, default-denied and malformed canonical
data. Old fixed-v1 Guest loopbacks remain a separate scope. These executions
supplement the65 logical mandatory suites/cases; they replace no original ID.

Gateway normal exit0 after Worker pidfd retirement is expected. Delivery and
recovery reject early retirement while the retained Worker lives and every
nonzero exit. A later0 is accepted only alongside complete actual Guest/native
transcripts; Worker exit alone never qualifies delivery. All these new sources
remain unexecuted until Root freezes and runs the complete serial lane.
