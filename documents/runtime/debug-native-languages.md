# SDK native-language debug metadata

[简体中文](debug-native-languages.zh.md)

Build SDK examples with `xmake f -m debug` followed by `xmake`. Debug builds
retain compiler metadata and request the compiler's unoptimized mode. The
`releasedbg` mode retains metadata with optimized code; variables can then be
optimized out and adjacent statements can share an instruction. Release builds
do not inherit these debug flags.

Run the resulting Wasm module through a host-owned UWVM debug broker using the
same explicitly configured WASIp1 and ROS permissions as its normal SDK test.
Debugging neither grants an import nor permits a denied operation. The source
editor is a host UI; source-language evaluation never invokes a native host
interpreter or replaces execution of the Wasm artifact.

## SDK v2 guest qualification

The debugger consumes the metadata in the exact module loaded by the VM.
A function name supports Wasm function breakpoints. An executable line row
supports source stops and stepping. Reading a source variable also requires
a valid location and type at the current stop. The SDK language setting
alone provides no evidence for these capabilities.

| SDK v2 producer | Evidence from application artifacts |
| --- | --- |
| Zig | Guest modules built by the installed Zig 0.16.0 SDK reached the original source through the authenticated DBG broker and DAP at both Wasm widths. Line 10 exposed the read-only `v: u32 = 2`. Source stepping stopped at line 11 and the previous frame reference was refused. This check covers scalar variables. |
| C | The optimized SDK ABI probe reaches an exact `body.c` breakpoint. Its next mapped position is another column on the same line. |
| C++, Objective-C, D, Fortran, TinyGo | The sampled artifacts contain line tables naming original application files. Qualify breakpoints, stepping and variables for the specific producer and application. |
| Rust | The sampled artifact names the original application file. The requested ROS ABI call line was unverified through DAP for that optimized artifact. |
| C# | A sampled AOT module names the original `Filter.cs`. Source breakpoint and variable coverage for the C# producer is unqualified. |
| Go register-only producer | Wasm function breakpoints use the emitted names. With `-g`, an embedded map built from Go SSA positions supports authenticated DAP source breakpoints and stepping between original statements. The map carries no variable types or locations. |
| Java and Kotlin | Previously built TeaVM C modules support breakpoints in generated C. With the updated producer, authentic JVM paths and lines reach the original Java and Kotlin breakpoints through a trusted host source map. The checked source steps moved between locations on the same source line. Source variables still reflect generated C. |
| Nim | With the updated `wrnim -g`, wasm32 and wasm64 retain original Nim lines from the entry and imported Nim modules. Both guests reached the `compute.nim` breakpoint, exposed the scalar argument value, stepped to another statement on the same line and refused the retired frame. |
| F#, Visual Basic .NET | The sampled modules have DWARF but did not name the original application files. |
| JavaScript, TypeScript, Lua, Python, Ruby | The sampled guest modules retain interpreter or bridge debug metadata. Inspecting that runtime is possible at the Wasm level. Original script locations and variables require a separate language-aware producer and debugger path. |

This inventory covers sampled artifacts. A language with no artifact in the
inventory still needs qualification. The authenticated DAP runs exercise the
protocol used by IDEs. Each graphical IDE integration has its own UI tests.

Run `test/0084.ros-runtime/run_sdk_source_debug.py` with the original guest
source, module, two executable line numbers and the application's normal VM
flags. Add `--value NAME=VALUE` to require an actual source variable value and
`--source-map` if the compiler's trusted source root is mapped to the checkout.
The runner hashes the guest and debugger inputs and checks the stop-scoped
frame reference after resuming.

Host ELF debug symbols are separate from guest debug metadata. Linux releases
can keep the VM's original DWARF in an [ELF companion](../how-to-build/linux.md).
Guest source debugging still uses the metadata emitted into the Wasm module.

## Compiler metadata providers

C, C++, C FFI, Objective-C, D, Nim, Odin, Rust, Swift, TinyGo and Zig use their
genuine embedded DWARF where the compiler emits it. Rust's standard library may
contribute metadata larger than one megabyte; raw DWARF section and unit sizes
therefore default to the validated input extent rather than a fixed byte quota.
Parser bounds checks, checked arithmetic, structural validation and explicitly
configured metadata limits still apply.

Swift debug builds use `-g -gdwarf-types -no-clang-module-breadcrumbs
-Xcc -gno-modules`. This retains the real type information in the deployed
module and suppresses external Clang module-cache references. The VM does not
follow a `.pcm` path, download a type unit or silently weaken its external
metadata policy.

AssemblyScript and MoonBit's Wasm backend emit Source Map v3. The SDK embeds
the compiler's exact JSON in the `.debug_source_map` custom section after
linking, leaving Code-section byte offsets unchanged. The original compiler
map stays in the private build workspace, outside the deployment artifact
directory. These maps provide source lines; they do not fabricate DWARF types
or variable locations. MoonBit's C backend follows its C compiler's debug
metadata path.

The pinned upstream Go frontend disables DWARF on `wasip1/wasm`. The separate
upstream Go compiler path uses a version-checked source overlay to build its compiler
and linker with `-wasmsourcemap`. The assembler records real source positions
at emitted byte offsets; the linker adjusts those offsets for variable-length
relocation insertions and embeds Source Map v3. Go runtime `pclntab` virtual PCs
are never interpreted as Wasm byte offsets. This provides line and column
locations, not fabricated variable locations. The offline overlay builds only
two private host tools and leaves the installed compiler closure unchanged.
That overlay is distinct from the SDK v2 register-only Go producer. Release
builds on the upstream compiler path use the original tools. In debug mode, the exact non-standard
package graph reported by Go is compiled with optimization and inlining
disabled. The standard library retains optimization and genuine line maps;
its source positions can be coalesced or optimized out. This avoids expanding
the entire runtime into unoptimized Wasm/JIT code.

The current GHC Wasm backend retains Haskell SrcSpan ticks in its Cmm output
but omits them from its final assembly. Its `-g -opta-g` output contains RTS
C/assembly line tables but no Haskell application line rows. Full source-line
debugging requires authentic backend metadata support. A source-file
association or coarse procedure-entry map alone is not full statement
stepping and is not counted as a successful source-level qualification.
Ordinary compiled Wasm execution and Wasm/native instruction debugging are
separate capabilities.

## Path and stop identity

Use the `sourceMap` attachment option only when compiler paths differ from
the host checkout. C-family SDK builds commonly use `/sdk` and
`/application`; other frontends can emit their real build directory. Paths
are display metadata, not a filesystem grant. Breakpoints must select an
actual line emitted by the compiler. A comment, declaration or optimized-out
statement can have no executable location.

The IDE's source, Wasm and native views use the same authenticated module,
function generation and current stop. Inline source frames can share the
selected physical JIT frame's native instruction reference. They cannot
expose VM, libc or unrelated process code. Resume, replacement, detach and
transport failure invalidate references; stale memory and variable requests
are refused.

## Qualification requirements

A source-language acceptance run must compile an unchanged SDK example through
its genuine frontend, load that exact Wasm artifact, install a real IDE source
breakpoint, stop at the matching file and line, step to another genuine mapped
line, and query source values only when actual location metadata exists. The
receipt records the artifact hash and compiler paths. WASI/ROS observations
are checked independently with actual calls and lossless raw-byte exports.

Resource-constrained Linux UI tests must verify the extension host, broker and
VM's actual cgroup membership. Electron can register a new systemd user scope
after process creation; launching its CLI through a cgroup wrapper alone is
insufficient. Disposable automated IDE profiles disable that session-bus
registration and assert the resulting membership before testing. Earlier
functional receipts outside the requested cgroup are explicitly excluded from
constrained acceptance. Run editors serially and leave a process/thread budget
for concurrent builds.
