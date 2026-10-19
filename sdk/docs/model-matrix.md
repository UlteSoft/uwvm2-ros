# SDK model execution in the existing Linux cgroup

[`../tools/run_cgroup_models.py`](../tools/run_cgroup_models.py) fills the model
execution stage after the [compiler matrix](build-matrix.md). Its default mode
prints a fixed plan, starts no process and creates no output directory. Actual
execution requires `--run`, Linux, the existing 64 GiB cgroup's current leaf,
device, inode and boot identifier, and explicit paths to already installed tools.
The leaf and effective memory maximum must both be `68719476736` bytes, swap
must be zero, and effective CPUs must be `0-31`. An old 64 GiB receipt or an
inherited-only 64 GiB ancestor is refused.
The driver never creates a cgroup, changes its limits, downloads a toolchain,
grants ROS authority, starts ROS or runs a QEMU process.

## Fixed inventory and evidence boundaries

| Stage | Actual operations | Required result |
| --- | --- | --- |
| CMake relocation | Configure the source SDK with `LANGUAGES NONE`; install to prefix A; move A to B; configure a fresh consumer | Both imported targets exist; include/package roots resolve under B; C++20 interface feature remains present |
| C11 | Compile and execute seven native fixtures from the installed tree | Every `assert` completes with explicit `-UNDEBUG`, including ownership, local scheduling and the four/five-role Action bundle |
| C++20 | Compile and execute nine native fixtures from the installed tree | Every `assert`/checked condition completes with explicit `-UNDEBUG`, including ownership, local scheduling, Action bundle and intrinsic callback reentry/exception guards |
| Native Rust | Compile the no-backend, `alloc` test configuration without running it; execute four Cargo-reported ELF binaries separately | Exactly twenty named tests pass; no ignored, measured or filtered tests |
| Native Go | Compile a common-logic test binary with `go test -c`; execute it separately | Exactly twenty-one top-level tests and 57 declared positive-vector subtests each run and pass once, with correct nesting and no skipped/failed results |
| Pure AssemblyScript | Rebind ten actually compiled/full-validated, import-free Wasm artifacts; invoke each `_start` once through Node | Nine assertion fixtures report exactly 446 assertions; the tenth records semantic coverage only |

There are 29 report rows. The Rust row retains four separate executable
bindings and command records. The native models run directly on the Linux host
architecture, currently restricted to x86-64 or aarch64 ELF. These models do not
establish ROS behavior, aarch64/QEMU qualification or performance. The Go row
tests the actual shared codec/output-slot/executor logic using the native Go
compiler. It does not establish TinyGo Wasm code generation or TinyGo WASI panic
recovery. The existing [compiler matrix](build-matrix.md) separately checks actual
TinyGo Wasm imports and full module validation.

The C/C++ fixtures are compiled from `prefix-b/share/roswasm/sdk/tests/`, with
that installed SDK's include root. This preserves their legacy relative includes
and prevents the original source checkout from hiding a broken installation.
The CMake consumer itself selects no compiler and contacts no ROS service.
Installed-tree hashes remain bound around native model execution.

Rust uses `--no-default-features --features alloc` without `ros1` or `ros2`.
Its four binaries are the library's output-slot, seven actual server-dispatch/shared-owner and three event-codec tests, two wire-vector tests,
two Image/Transform timestamp tests, and five local scheduling tests. Cargo's machine-readable artifact
records must identify exactly those four binaries beneath the fresh target
directory. The driver invokes them with one test thread and requires both exact
test-name inventories and a complete, unfiltered summary.

## Existing tool inputs

All executable arguments must be absolute paths to actual executable ELF files.
Shell wrappers and implicit `PATH` compiler selection are refused at the command
boundary. An AssemblyScript compiler script is a separately hashed input.

| Option | Existing input | Used by |
| --- | --- | --- |
| `--cmake` | CMake ELF executable and its installed modules/generator support | Relocation and installed C/C++ fixtures |
| `--cc`, `--cxx` | Native C11/C++20 compilers with host headers/libraries | Native assertion fixtures |
| `--cargo`, `--rustc` | Cargo and the selected Rust compiler, including installed native standard library/linker | Rust test compilation |
| `--go` | Native Go compiler with its installed standard library | Common Go tests |
| `--node` | The same Node executable used by the admitted compiler report | Pure AssemblyScript execution |
| `--asc-js` | The same installed AssemblyScript compiler script used by that report | Compiler evidence rebinding; not invoked again by this driver |
| `--wasm-tools` | The same full validator ELF used by that report | Validator evidence rebinding; not invoked again by this driver |

Cargo is offline and automatic Rust installation is disabled. Rust compiler
wrappers and extra flag overrides are cleared for the native test configuration.
Go module/checksum proxies are disabled, automatic toolchain selection is local,
CGO is disabled and `GOFLAGS` is cleared. Native Go explicitly selects the live
host's Linux architecture. Mutable Cargo targets and Go caches live outside the
SDK. Rust/Go sources are copied to fresh work directories; original files in the
copies remain hashed, with only explicitly named new `Cargo.lock`/`go.sum` files
allowed. Existing lock files are immutable inputs too.

Every command records its environment digest without exposing inherited values.
These controls and executable hashes do not qualify all external system
headers, libraries, compiler modules, target standard libraries, global
configuration or transitive package inputs. The report therefore retains
`toolchain_sdk_closure=UNQUALIFIED` and `task_complete=false` even when its selected
model rows pass. Installed toolchain closure qualification remains separate.

## Admitting compiled AssemblyScript inputs

Supply an absolute `--compiler-report` and its explicit lower-case SHA-256 with
`--compiler-report-sha256`. The compiler run directory and its artifacts must
remain intact. Copying an old Wasm file to a fresh path does not satisfy this
contract. The report must be an actual `COMPILER_MATRIX_PASS`, with consistent
before/after cgroup and OOM evidence, and both complete SDK source manifests must
match the current SDK exactly. Complete all SDK edits before freezing the source
and starting the compiler matrix. A source change after compilation requires a
new compiler run.

The model driver admits only the ten `assemblyscript-pure-*` rows. It checks
each exact source path/digest, compiler argv, Node/compiler-script bytes, full
validator argv/bytes, row source finalization and unchanged artifact device,
inode, size, timestamps and SHA-256. All ten must have zero imports. A compiler
report that permitted WASI for a purported pure fixture is refused here. The
report and its source-manifest files remain bound throughout the model run.
Current cgroup leaf, boot, swap/cpuset and OOM counters must agree with the
compiler evidence. These checks establish consistency with the recorded run;
they are not a cryptographically signed attestation.

[`../javascript/tests/run_pure_wasm.mjs`](../javascript/tests/run_pure_wasm.mjs)
also proves its own current cgroup membership, verifies the artifact SHA-256 and
refuses every import before instantiation. It supplies an empty import object,
checks that counters start at zero, invokes `_start` once and requires:

| Fixture | Completed assertions |
| --- | ---: |
| `codec_vectors` | 175 |
| `adapter_contracts` | 14 |
| `executor_mutation` | 9 |
| `factory_transactions` | 75 |
| `resource_roles` | 25 |
| `unicode_strings` | 8 |
| `local_schedule` | 47 |
| `server_callbacks` | 60 |
| `unified_events` | 33 |
| `highlevel_coverage` | Semantic coverage only; no assertion count |

This runner is a pure model host. It is not the authenticated JavaScript ROS
bridge and supplies no ROS, WASI, clock, network, filesystem or logger imports to
the guest. Raw-inventory fixtures and actual ROS examples are never executed by
this model driver.

## Prepare and execute

Prepare the complete plan on any host without executing a compiler or guest:

```sh
python3 -B sdk/tools/run_cgroup_models.py --out /absolute/new/sdk-model-run
```

Inside the existing authorized Linux cgroup, resolve the current leaf and tool
paths from the actual environment. The following values are placeholders, not
remembered or invented cgroup identities:

```sh
python3 -B sdk/tools/run_cgroup_models.py --run \
  --out /absolute/new/sdk-model-run \
  --expected-leaf /sys/fs/cgroup/current-authorized-leaf \
  --expected-device CURRENT_DEVICE --expected-inode CURRENT_INODE \
  --expected-boot CURRENT_BOOT_UUID --expected-memory-max 68719476736 \
  --expected-swap-max 0 --expected-cpus 0-31 \
  --rss-limit-mib 2048 --cgroup-current-limit-mib 61440 \
  --cmake /absolute/cmake --cc /absolute/clang --cxx /absolute/clang++ \
  --cargo /absolute/cargo --rustc /absolute/rustc --go /absolute/go \
  --node /absolute/node --asc-js /absolute/assemblyscript/bin/asc.js \
  --wasm-tools /absolute/wasm-tools \
  --compiler-report /absolute/current-sdk-compiler-run/report.json \
  --compiler-report-sha256 EXACT_CURRENT_REPORT_SHA256
```

Use `--groups cmake c cpp`, `--groups rust go` or `--groups assemblyscript` for
explicit subsets. C/C++ groups always require a successful CMake relocation row;
there is no fallback to compiling the original checkout. Missing configuration
creates blocked rows. Compiler/test failures retain raw logs and fail their row;
independent rows can continue after ordinary failures. A subset pass concerns
only the selected inventory, never all language/ROS acceptance.

## Guards and artifacts

The driver uses the existing
[`../../tests/ros_cgroup/probe.py`](../../tests/ros_cgroup/probe.py) and the compiler
driver's held-launch guard. It verifies actual membership before continuing a
stopped same-PID launcher, observes owned births/cgroup membership/executable
identity and RSS every 20 ms, retains command deadlines, and checks cgroup OOM
counters. Default guards are aggregate sampled process RSS at 2 GiB (`2048` MiB)
and whole cgroup `memory.current` at 60 GiB (`61440` MiB), below the hard 64 GiB
(`65536` MiB) limit. Cleanup signals only verified owned births using checked
pidfds. Any failed source/tool/artifact,
scope, process, memory, cleanup or evidence-persistence guard aborts further
commands and retains available facts. Sampling does not observe every very short
child and is never promoted to exhaustive architecture evidence.

The fresh output directory contains `source-before.json`, `source-after.json`,
the relocated installation/work/cache directories, each row's `report.json`,
raw command logs and shared guard capsules, and the aggregate `report.json`.
Failed persistence gets a separately named best-effort fallback and explicit
failure status; an unavailable file is not claimed as saved evidence.
`SDK_MODEL_MATRIX_PASS` requires every selected fixed row to pass plus unchanged
source, current cgroup incarnation, successful observed cleanup and no new OOM.
Its report always keeps `ROS_execution` and `ARM_execution` as `NOT_PERFORMED`.

The historical frozen r14 SDK completed its then-current 17 model rows after its successful 61-row
compiler matrix; see the
[exact current SDK acceptance receipts](../../documents/ros/sdk-current-20261014.md).
That milestone qualifies those frozen bytes. Any subsequent SDK change,
including documentation in the complete SDK inventory, requires a fresh
compiler/model run before the new source is qualified. Real ROS behavior still
requires the separately qualified Worker/Gateway/native fixtures, explicit grants/default-deny cases,
native side-effect witnesses, deadlines, cleanup and current aarch64/cgroup
evidence.

## Additive event-dispatch source revision

The new [six-language resource contract](event-dispatch.md) adds one C and one
C++ fixture (current native source inventories8 and10), two Rust binding unit
tests, three TinyGo production dispatcher/executor tests and a separate pure
AssemblyScript `event_dispatch` fixture with20 source-defined assertions. Its
sync/async JavaScript fixture is selected by the maintained source/model runners.
The real Rust Stream/Executor Wasm fixture and finite mocked import host have
explicit invocation shapes in that guide. These new sources have not yet run;
older dated model receipts and their inventories remain historical facts. No
model result substitutes for real native ROS or Guest delivery.
