# Serial local SDK verification

Use [run_local_checks.py](../tools/run_local_checks.py) for local developer
checks. It requires Python 3.11 or newer and a fresh absolute output directory
below `/tmp`. It copies the SDK before starting a command; all compiler output,
work trees, caches, receipts and logs remain under that directory. Never use the
repository checkout as a compiler's output or cache directory.

Commands execute sequentially. C/C++ compile one translation unit at a time at
`-O0`; Cargo uses `-j1`, Go uses `-p 1`, and TinyGo uses `-p 1 -opt=0`.
TinyGo selects `-scheduler=none` for these caller-driven lessons. Node has a
96 MiB managed-heap limit. A sampled aggregate process-group RSS
watchdog and command deadline apply to compiler children as well as launchers.
Sampling reports the largest observed RSS; it cannot measure every transient
allocation. The caller may lower `--rss-limit-mib` below its 768 MiB default.

Select explicitly installed tools. This runner installs nothing, performs no
network request and selects no ROS permissions. For example:

```sh
python3 -B sdk/tools/run_local_checks.py \
  --out /tmp/fresh-sdk-host-checks --groups source cmake policy native \
  --node /absolute/node --cmake /absolute/cmake \
  --cc /absolute/clang --cxx /absolute/clang++ \
  --cargo /absolute/cargo --rustc /absolute/rustc --go /absolute/go \
  --rss-limit-mib 512
```

The CMake group installs the entire SDK under prefix A, moves it to prefix B,
and configures a fresh consumer. Both imported targets, the relocated include
directory and the C++20 requirement must remain usable. Following native groups
use that installed tree. Rust and Go operate on disposable copies with offline
dependency resolution and one test thread. Source checks also execute the JS
codec/ownership/executor models and the closed stream relay/API models. The
complete source verifier additionally checks every relative documentation and
source link; referenced repository guide/report files are copied and hashed as
read-only context for this frozen check.
Fixed-record source/generator models and the handwritten custom JavaScript
roundtrip also run in the owned temporary tree.

The policy group runs the advanced generator's real CLI for each supported
alias and its aggregate String/Float64 profiles. It checks generated config
bytes with the actual Worker and Gateway validators from a frozen Runtime
source copy, including all forty ROS2 Action QoS fields. Thirteen version-absent
selections must fail before creating output. Test UID/PID/socket values are
synthetic format inputs: these checks authenticate no principal, start no ROS
process and apply no capability. Run this verifier from the repository so it
can bind the Runtime registry and compiler-driver context. The installed SDK's
libraries, headers and applications are independently usable by consumers.

Compile the real example modules and inspect their emitted signatures:

```sh
python3 -B sdk/tools/run_local_checks.py \
  --out /tmp/fresh-sdk-wasm-checks --groups wasm \
  --languages c cpp rust tinygo assemblyscript javascript_bridge \
  --wasi-sdk /absolute/wasi-sdk --cargo /absolute/cargo \
  --rustc /absolute/rustc --go /absolute/go --tinygo /absolute/tinygo \
  --node /absolute/node --asc-js /absolute/assemblyscript/bin/asc.js \
  --wasm-tools /absolute/wasm-tools --execute-pure-models --execute-relay-models \
  --example-variants \
  --rss-limit-mib 512 --timeout-seconds 600
```

The selected WASI SDK must contain working `bin/clang`, `bin/clang++` and
`share/wasi-sysroot` paths, including C++ libraries. Rust needs its installed
`wasm32-wasip1` standard library. TinyGo needs installed Go and WASI support.
AssemblyScript needs its installed compiler package. A missing target or
dependency remains a failure; no fallback compiler silently changes scope.

Each module must compile, pass binary import-signature checking and pass the
full `wasm-tools validate` command. ROS2-only lessons are selected only for
ROS2. The five handwritten fixed-record ROS1 guest examples compile as well;
the Rust/Go application manifests are supplied locally with no fetched package
or fabricated native identity. Both JS bridge forms retain every ROS import; the command relay additionally
requires exactly the three basic WASIp1 imports documented in
[RELAY.md](../javascript/RELAY.md). Pure AssemblyScript fixtures must contain
zero imports. Their optional execution uses the separate
[local model runner](../tools/run_local_pure_wasm.mjs), supplies an empty import
object and checks exact assertion counts from the compiled artifacts.

`--example-variants` additionally compiles every C/C++ tutorial selector:
service/Action clients, servers, all query/control cases, deferred RPC,
revocation, the separate default-denied delivery fixture, and all twenty
Action-bundle case/cancel-any/revoke combinations per backend and language. Each variant has
its own log and artifact. Compiling only a default `main` would leave the other
conditional branches unchecked.

The relay execution models instantiate the actual compiled C command with
bounded mathematical ROS/WASI imports. They check dispatcher behavior, exact
64-bit arguments, default refusal, memory guards and short/failed stdio. They
apply no Runtime authority and supply no native ROS implementation.

Use `--groups scheduling --node /absolute/node --compiler-reports` followed by
the current local C/C++, Rust, TinyGo and AssemblyScript compiler reports to
execute their local scheduling lessons. C/C++ requires the variant report's
`defer0` entries. All ten artifacts are hash-bound, then instantiated serially
with basic stdio and empty guest environment metadata. TinyGo additionally
tests denied versus explicitly selected mathematical entropy input. No host
clock, native entropy, ROS resource, Runtime grant or external environment is
supplied. Compiler runtimes can introduce imports beyond the SDK: the TinyGo
recipes select `-scheduler=none`, and each actual import table remains visible
in its report. A compiler dependency never silently increases default rights.

`report.json` preserves every command, status, log digest and sampled RSS;
`source-manifest.json` and `context-manifest.json` bind the frozen inputs.
`wasm-results.json` binds module bytes, signature checks and validation results.
Any compiler, contract, timeout, memory or model failure makes the selected run
incomplete. Keep the failed directory when repairing a source or tool path;
use a new output directory for the next attempt.

These local checks establish source, ownership, codec, scheduling and Wasm
compatibility properties. They do not establish native ROS message delivery,
Runtime capability enforcement, toolchain dependency closure, performance or
Linux/QEMU qualification. The separate [Linux compiler](build-matrix.md) and
[model acceptance](model-matrix.md) drivers retain their exact cgroup admission
rules. Local receipts cannot substitute for those acceptance reports.
