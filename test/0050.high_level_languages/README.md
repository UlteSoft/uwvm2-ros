# High-level language tests

This suite keeps compiler inputs, build scripts, semantic expectations and
reusable runtime/debugger checks. Compiler and runtime variants have separate
contracts; instruction or source-position debugging does not establish managed
language variable support.

## Sources and producers

| Language | Stable sources and build instructions |
| --- | --- |
| C/C++ | [C-family fixtures](c-family/) |
| C# / F# / Visual Basic | [C#](csharp/README.md), [other .NET languages](dotnet-languages/) |
| Java / Kotlin | [Java](java/), [Kotlin](kotlin/) |
| Python | [CPython fixtures](python/README.md) |
| AssemblyScript | [AssemblyScript](assemblyscript/) |
| JavaScript | [JavaScript](javascript/) |
| MoonBit | [MoonBit](moonbit/README.md) |
| Go / TinyGo | [Go and TinyGo](go/README.md) |
| Rust | [Rust](rust/README.md) |
| Zig | [Zig](zig/README.md) |

Pinned compiler versions, download hashes, producer flags and semantic limits
belong to each language's stable directory. Keep source maps and DWARF when
building debugger fixtures. A compiler's native/Wasmtime result does not qualify
a different UWVM product or its ROS integration.

## Running and inspecting the suite

[build_products.py](build_products.py) builds selected runtime products;
[prepare_cases.py](prepare_cases.py) prepares explicit cases and
[run_matrix.py](run_matrix.py) checks ordinary execution and debugger workflows.
[launch_functional.py](launch_functional.py) manages the functional cohort.
The [engine recipes](engines/recipes.json) and per-language case files are
producer inputs, not disposable run results.

Performance tools include [bench_engines.py](bench_engines.py),
[run_vtune_matrix.py](run_vtune_matrix.py),
[run_vtune_ordinary.py](run_vtune_ordinary.py) and
[record_environment.py](record_environment.py). New measurements must identify
the actual product, inputs, permissions and environment. Old reports do not
qualify changed sources or binaries.

Reusable ROS language witnesses, codec contracts, corpus generators and native
runners live in [the integration test sources](../0073.ros-tests/support/source/ros_language_integration/README.md).
The shared SDK contract tests remain under
[shared-language-sdk](../0073.ros-tests/support/source/shared-language-sdk/).
Language debugger selection is documented in
[language-options.md](../../documents/language-options.md).

## Retained producer dependencies

The unique pinned Linux MoonBit compiler/core library and native runtime inputs
are under [moonbit/toolchain](moonbit/toolchain/). The retained Linux Wasm
validator is [go/toolchain/wasm-tools](go/toolchain/wasm-tools).
These are dependency inputs; generated build trees and execution output should
be placed in an explicitly selected output directory.

Build products, runtime logs, profiling captures and dated execution packets
are regenerated in an explicitly selected output directory. Shared artifact
rules are in [test/.gitignore](../.gitignore); each suite also has a local
`.gitignore` for its output names and required fixture exceptions.
